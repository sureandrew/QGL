#include "MapServer.h"
#include "User.h"
#include "Player.h"
#include "Render.h"

#include "Reuben\Diagnostics\DebugNew.h"

typedef StlVector<CPlayer *> CPlayers;

Peer gPeer;
RPCContext gContext;
CUser *gpUser = NULL;
extern CPlayer * Player;
extern bool ServerConnected;
extern CPlayers OtherPlayerLst;
extern CRender * pRender;

Peer& GetPeer()
{
	return gPeer;
}

RPCContext& GetContext()
{
	return gContext;
}

void MapServer_Proxy::Init()
{
	LocalRPCManager::GetInstance().SetCloseHandle(MapServer_Proxy::OnDisconnect);
}

void MapServer_Proxy::OnDisconnect(const Peer &peer)
{
	ServerConnected = false;
}

bool MapServer_Proxy::ConnectServer(String addr)
{
	//char *localAddr = "127.0.0.1";
	String localAddr = _T("127.0.0.1");
	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();

	if (addr == _T(""))
		addr = localAddr;
	gPeer = Peer(INVALID_SOCKET_ID, Reuben::Network::StringToIP(addr.c_str()),
		8699, Reuben::Network::PROTOCOL_TCP, BOOLEAN_TRUE);

	// connect to server
	return rpcmgr.GetModel().Connect(gPeer);
}

// update routine for room server
void MapServer_Proxy::Update()
{
	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
	//rpcmgr.Process(BOOLEAN_TRUE);
}

RPCResult MapServer_Proxy::NewPlayer(const String &name)
{
	return CreateUser(gPeer, gContext, name);
}

RPCResult MapServer_Proxy::CB_CreateUser(const Peer &peer, const RPCContext &context, const RPCSrvID &uSrvId, const RPCNetID &uNetId, const RPCSrvID &pSrvId, const RPCNetID &pNetId)
{
	printf("CB_CreateUser: User %d:%d, Player %d:%d\n", uSrvId, uNetId, 
		pSrvId, pNetId);

	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();
	CUser *user = reinterpret_cast<CUser *> (rpcmgr.CreateObject(RPC_User_CLASSID, uSrvId, uNetId));
	if (user == NULL) {
		printf("Error: CB_CreateUser failed\n");
		return RPC_RESULT_FAIL;
	}

	CPlayer *player = reinterpret_cast<CPlayer *> (rpcmgr.CreateObject(RPC_Player_CLASSID, pSrvId, pNetId));
	if (player == NULL)
	{
		printf("Error: CB_CreateUser failed\n");
		rpcmgr.DestoryObject(user);
		return RPC_RESULT_FAIL;
	}

	gpUser = user;
	Player = player;		// set extern global player
	player->AddObserver(user);
	OtherPlayerLst.push_back(player);
	user->Init(peer, player);

	return RPC_RESULT_OK;
}

RPCResult MapServer_Proxy::CB_GetPlayer(const Peer &peer, const RPCContext &context, const RPCSrvID &srvId, const RPCNetID &netId)
{
	LocalRPCManager &rpcmgr = LocalRPCManager::GetInstance();

	if (rpcmgr.FindObject(srvId, netId) == NULL) {
		CPlayer *player = reinterpret_cast<CPlayer *> 
			(rpcmgr.CreateObject(RPC_Player_CLASSID, srvId, netId));
		// user observer player changed
		*player = *Player;
		player->RemoveAllChildren();
		pRender->AddEntity(player);
		//if (gpUser)
		//	player->AddObserver(gpUser);
		OtherPlayerLst.push_back(player);
	}

	return RPC_RESULT_OK;
}