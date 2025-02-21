#pragma once
#include "GameObject.h"
#include "NetworkBase.h"
#include "NetworkState.h"
#include "../GameTech/YiEventSystem.h"
#include "../../Gameplay/Player.h"
namespace NCL {
	namespace CSC8503 {

		struct FullPacket : public GamePacket {
			int		objectID = -1;
			NetworkState fullState;

			FullPacket() {
				type = Full_State;
				size = sizeof(FullPacket) - sizeof(GamePacket);
			}
		};//

		struct DeltaPacket : public GamePacket {
			int		fullID = -1;
			int		objectID = -1;
			char	pos[3] = { 0 };
			char	orientation[4] = { 0 };

			DeltaPacket() {
				type = Delta_State;
				size = sizeof(DeltaPacket) - sizeof(GamePacket);
			}
		};

		struct ClientPacket : public GamePacket {
			int		playerID = -1;
			int		lastID = -1;
			char	buttonstates[8] = { 0 }; 
			int     angles[3] = { 0 };
			ClientPacket() {
				size = sizeof(ClientPacket);
			}
		};

		struct EventPacket : public GamePacket {
			int networkID = -1;
			int playerID = -1;
			GAME_EVENT_ID  eventID;
			EventPacket() {
				type = Event_State;
				size = sizeof(EventPacket);
			}
		};

		struct SyncPacket :public GamePacket {
			int SyncTotalCount = -1;
			int objectID = -1;
			GameObjectType objType = GameObjectType::GameObjectType_invalid;
			NetworkState fullState;
			SyncPacket() {
				type = Sync_Obj;
				size = sizeof(SyncPacket);
			}
		};

		struct SyncStatePacket :public GamePacket {
			PlayerPro state;
			float timer = 0;
			SyncStatePacket() {
				type = Sync_State;
				size = sizeof(SyncStatePacket);
			}

		};

		class NetworkObject {
		public:
			NetworkObject(GameObject& o, int id);
			virtual ~NetworkObject();

			//Called by clients
			virtual bool ReadPacket(GamePacket& p);
			//Called by servers
			virtual bool WritePacket(GamePacket** p, bool deltaFrame, int stateID);

			//virtual bool WriteSpawnPacket(SpawnPacket** p, int networkID, int playerID);

			void UpdateStateHistory(int minID);

			NetworkState& GetLatestNetworkState();

			int GetNetworkID() { return networkID; }
			int GetWorldID() { return object.GetWorldID(); }
			GameObjectType GetObjType() { return object.type; }
		protected:

			bool GetNetworkState(int frameID, NetworkState& state);

			virtual bool ReadDeltaPacket(DeltaPacket& p);
			virtual bool ReadFullPacket(FullPacket& p);

			virtual bool WriteDeltaPacket(GamePacket** p, int stateID);
			virtual bool WriteFullPacket(GamePacket** p, int stateID);
			

			GameObject& object;

			NetworkState lastFullState;

			std::vector<NetworkState> stateHistory;

			int deltaErrors;
			int fullErrors;

			int networkID;//�������Ϳͻ��˶����������Ҫͬ���������� �����ŶԷ������Ϳͻ��˶���һ����
			//������������ ���1��1000 ���2��2000 �ڿͻ��˺ͷ������������ǵı�Ż���һ����
		};
	}
}



