//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
#include "Common/Version.h"
//-- Self
#include "Stub/Service/WorldService.h"
//-- Library
#include "Proxy/Data/AccountData.h"
#include "Proxy/Data/CharacterData.h"
#include "Proxy/Data/GuildData.h"
#include "Proxy/Data/ExecuteCheatData.h"
#include "Proxy/Service/LogService.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Stub/Logic/Account.h"
#include "Stub/Logic/CharacterInfo.h"
#include "Stub/Logic/WorldEvent.h"
#include "Stub/Logic/SellYuanBaoGroup.h"
#include "Stub/Logic/RankCharacter.h"
#include "Stub/Logic/SellYuanBao.h"
#include "WorldService/TaskScheduler.h"
#include "WorldService/WorldApplication.h"
#include "WorldService/Configure.h"
#include "WorldService/Global.h"
#include "WorldService/antigameaddiction.h"
#include "WorldService/ChargeShopControl.h"
#include "Stub/Logic/Account.h"
#include "Proxy/Data/SellYuanBaoData.h"

#define JUMP_MAP_TIME		1000
#define LOGIN_DELAY			10					// each login delay interval

enum LOGCHEAT_MESSAGE
{
	LOGCHEAT_SUCCEED = 0,
	LOGCHEAT_ERR_CHEATFAIL,
	LOGCHEAT_ERR_CHARNOTFOUNDINMAP,
	LOGCHEAT_ERR_CHARNETIDNOTFOUND,
	LOGCHEAT_ERR_CHARINFONOTFOUND,
	LOGCHEAT_ERR_MAPNOTFOUND,
};

// private functions here
namespace WorldService_Stub
{
	void ReleaseAccount(const SessionID &sessionId, Boolean bDelete);
	void TakeOverAccount(const SessionID &sessionId);
};

// NpcLiveInfo fucntions implementation
NpcLiveInfo::NpcLiveInfo() : npcId(0), mapId(0), lineId(0), x(0), y(0), eventId(0)
{
}

bool NpcLiveInfo::operator == (const NpcLiveInfo& b) const
{
	return (npcId == b.npcId && mapId == b.mapId && lineId == b.lineId &&
			ownerId == b.ownerId && x == b.x && y == b.y && eventId == b.eventId);
}

RPCResult WorldService_Stub::RegisterProxy(LPCPEER pPeer, RPCContext &context, 
										   const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("RegisterProxy: serviceId %d"), serviceId);

	// On Map initialization finish
	// re-count line
	int cutSince = (int)gGlobal.m_lineCounts.size();
	for( int i = 1; i < (int)gGlobal.m_lineCounts.size(); ++i )
	{
		if( gGlobal.m_lineCounts[i]==-1 )
		{
			cutSince = i;
			break;
		}
	}
	gGlobal.m_lineCounts.resize(cutSince);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("lines available (0-%d)"), (int)gGlobal.m_lineCounts.size()-1);
	// start task schedule for world event
	WorldApplication::GetInstance().RegisterProxy(pPeer, serviceId);
	if (!gGlobal.m_pTaskScheduler->IsStarted())
		gGlobal.m_pTaskScheduler->Start();

	return (RPC_RESULT_OK);
}

RPCResult WorldService_Stub::UnregisterProxy(LPCPEER pPeer, RPCContext &context, 
											 const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UnregisterProxy: serviceId %d"), serviceId);

	gGlobal.m_pTaskScheduler->Stop(pPeer);
	if (!WorldApplication::GetInstance().UnregisterProxy(serviceId))
		return (RPC_RESULT_FAIL);

	return (RPC_RESULT_OK);
}


RPCResult WorldService_Stub::ClientDisconnect(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("[session not found], sessionId %s"),
			context.sessionId.GetString().c_str());
		return RPC_RESULT_FAIL;
	}

	if (psess->pAccount->m_lastSessId == context.sessionId)
		psess->pAccount->Disconnect(context.sessionId);

	// check if lock too long, then timeout fail the locked procedure
	ProcedureManager::GetInstance().CheckLockTimeout(context.sessionId);

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::ReleaseSession(LPCPEER pPeer, RPCContext &context, 
											const SessionID &sid, 
											const Boolean &bDelete)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ReleaseSession: sessionId %s, sid %s, bDelete %d"), 
		context.sessionId.GetString().c_str(), sid.GetString().c_str(), bDelete);

	Session *psess = GetSession(sid);
	if (psess == NULL || psess->pAccount == NULL || sid.serviceId == INVALID_SERVICE_ID) {
		// try to remove any procedures when waiting in login account
		ProcedureManager::GetInstance().RemoveBySession(context.sessionId);
		return RPC_RESULT_FAIL;
	}
	
	UInt32 acct_id = psess->pAccount->GetAcct_id();
	Reuben::Network::IP ip = psess->ip;

	bool bLast = (psess->pAccount->m_lastSessId == sid);
	bool bWait = (psess->pAccount->m_waitSessId == sid);

	if (bLast)
		psess->pAccount->Disconnect(sid);

	if (!bLast && !bWait) {	// not related session id
		RemoveSession(sid);
	} else if (bWait) {		// wait session id, just cancel kick
		psess->pAccount->m_waitSessId.serviceId = INVALID_SERVICE_ID;
		RemoveSession(sid);
	} else if (bLast) {		// last session id
		if (psess->pAccount->m_waitSessId.serviceId != INVALID_SERVICE_ID)
			TakeOverAccount(sid);				// wait session id to take over
		else
			ReleaseAccount(sid, bDelete);		// just release account
	}

	if (gGlobal.pLogService)
		LogService_Proxy::LogAccount(gGlobal.pLogService->pPeer, context, acct_id, ip, LogService_Proxy::Account::ACT_LOGOUT);

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::RegisterMap(LPCPEER pPeer, RPCContext &context, 
										 const ServiceID &serviceId, const UInt16 &map_id, 
										 const UInt8 &line_id, const UInt32 &owner_id,
										 const Boolean &bOpen)
{
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("serviceId %d, map_id %d, line_id %d, owner_id %d, bOpen %d"),
		serviceId, map_id, line_id, owner_id, bOpen);
	MapIndex mapidx(map_id, line_id, owner_id);
	gGlobal.RegisterMap(mapidx, pPeer, bOpen);

	// remote create npc in creation list when map activated and open
	NpcCreateMap::iterator it = gGlobal.m_npcCreateMap.find(mapidx);
	if (bOpen && it != gGlobal.m_npcCreateMap.end())
	{
		NpcCreateList* pNpcList = it->second;
		MapService_Proxy::RemoteCreateNpc(pPeer, context, map_id, line_id, owner_id, *pNpcList);
		// remove create npc list after use
		SafeDeleteObject(pNpcList);
		gGlobal.m_npcCreateMap.erase(it);
	}
	
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::UnregisterMap(LPCPEER pPeer, RPCContext &context, 
										   const ServiceID &serviceId, const UInt16 &map_id, 
										   const UInt8 &line_id, const UInt32 &owner_id)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("serviceId %d, map_id %d, line_id %d, owner_id %d"),
		serviceId, map_id, line_id, owner_id);
	gGlobal.UnregisterMap(MapIndex(map_id, line_id, owner_id));

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::CharacterLeave(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &acct_id, const UInt32 &char_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CharacterLeave: sessionId %s, acct_id %d, char_id %d"), 
		context.sessionId.GetString().c_str(), acct_id, char_id);

	AccountMap::iterator it = gGlobal.m_accountMap.find(acct_id);
	if (it == gGlobal.m_accountMap.end())
	{
		// account not found
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CharacterLeave Error: cannot found loginned acct_id %d"),
			acct_id);
		return RPC_RESULT_FAIL;
	}

	CAccount* pAccount = it->second;			// old account
	CCharacterInfo* pCharInfo = pAccount->GetCharInfo(char_id);
	if (pCharInfo == NULL)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
			_T("CharacterLeave Error: invalid character of account, acct_id %d, char_id %d"),
			acct_id, char_id);
		return RPC_RESULT_FAIL;
	}

	// character leave world only when already disconnected
	if (pCharInfo->IsDisconnect())
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CharacterLeave: sessionId %s, char_id %d"),
			context.sessionId.GetString().c_str(), char_id);
		MessageService_Proxy::ReleaseMsgCharacter(gGlobal.pMessageService->pPeer, context,
			char_id);
		ObjectService_Proxy::ReleaseCharacter(gGlobal.pObjectService->pPeer, context, 
			acct_id, char_id);
		pCharInfo->SetStatus(CCharacterInfo::ST_LEAVE);
	}
	else
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CharacterLeave: [still in world], sessionId %s, char_id %d"),
			context.sessionId.GetString().c_str(), char_id);
	}

	// delete account if related session removed
	Session* psess1 = NULL;
	Session* psess2 = NULL;
	
	if (pAccount->m_lastSessId.serviceId != INVALID_SERVICE_ID)
		psess1 = GetSession(pAccount->m_lastSessId);
	if (pAccount->m_waitSessId.serviceId != INVALID_SERVICE_ID)
		psess2 = GetSession(pAccount->m_waitSessId);

	if (psess1 == NULL && psess2 == NULL) {
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CharacterLeave : account deleted, acct_id %d"), acct_id);
		gGlobal.m_accountMap.erase(it);
		ObjectService_Proxy::ReleaseAccount(gGlobal.pObjectService->pPeer, context, 
			pAccount->GetAcct_id());
		DeleteNetObject(pAccount);
	} else {
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CharacterLeave : account remained, acct_id %d, last sid %s, wait sid %s"),
			acct_id, psess1 ? pAccount->m_lastSessId.GetString().c_str() : _T(""),
			psess2 ? pAccount->m_waitSessId.GetString().c_str() : _T(""));
	}

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::LoginAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &ip, const String &loginName, const ByteArray &password)
{
	String login_name(loginName);
	login_name.ToUpper();

	PROCEDURE_START_RPC3(WorldService, LoginAccount, NULL, UInt32, ip, String, loginName, ByteArray, password)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("LoginAccount, sessionId %s, name %s"), 
			context.sessionId.GetString().c_str(), loginName.c_str());
		context.bCallback = BOOLEAN_FALSE;
		CB_AntiHack(pPeer, context, ByteArray(), AntiHackData());
		PROCEDURE_SET_TIMEOUT(gConfigure.antiHackTimeout, BOOLEAN_TRUE);

	PROCEDURE_WAIT1(1, WorldService, AnswerAntiHack, UInt8, answer)

		PROCEDURE_SET_TIMEOUT(30000, PCD_DEFAULT_TIMEOUT_FAIL);

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("AnswerAntiHack, sessionId %s, name %s"), 
			context.sessionId.GetString().c_str(), loginName.c_str());
		ObjectService_Proxy::GetAccountByName(gGlobal.pObjectService->pPeer, context, login_name,
			BOOLEAN_FALSE);

	PROCEDURE_WAIT1(2, ObjectService, CB_GetAccountData, NetObject*, acctData)

		CAccountData *pAcctData = CastNetObject<CAccountData>(acctData);
		TRACE_ENSURE(pAcctData != NULL);
		
		
		//check gm ip
		if (gConfigure.bGMCheckIP)
		{
			for (Int i = 0; i < (Int)gConfigure.gmIdList.size(); i++)
			{
				if (gConfigure.gmIdList[i] == pAcctData->GetAcct_id())
				{
					String ipStr = Reuben::Network::IPToString(ip);
					Boolean IPFound = false;
					for (UInt i = 0; i < gConfigure.strGMIPList.size(); i++)
					{
						if (ipStr == gConfigure.strGMIPList[i])
						{
							IPFound = true;
							break;
						}
					}
					if (!IPFound)
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ForceLogout, gm account ban ip, sessionId %s"), context.sessionId.GetString().c_str());
						CB_ForceLogout(pPeer, context, _T("MSG_VERIFY_FAILED"));
						PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)	
					}
				}
			}
		}	
		

		UInt32 banDate = pAcctData->GetBanTime();
		UInt32 timeNow = (UInt32) time(NULL);
        if ((pAcctData->GetPassword_hash() != password) || (timeNow <= banDate))
		{
			// wrong password, so release account if not logined
			CAccount* pAccount = gGlobal.GetAccount(pAcctData->GetAcct_id());
			if (pAccount == NULL)
				ObjectService_Proxy::ReleaseAccount(gGlobal.pObjectService->pPeer, context, pAcctData->GetAcct_id());
			DeleteNetObject(acctData);
			if (timeNow <= banDate)
				CB_LoginResult(pPeer, context, LOGIN_FAILED_KICKED);
			else
				CB_LoginResult(pPeer, context, LOGIN_FAILED_WRONG_PASSWORD);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ForceLogout, banned or wrong password, sessionId %s"), context.sessionId.GetString().c_str());
			CB_ForceLogout(pPeer, context, _T("MSG_VERIFY_FAILED"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		if (pAcctData->GetInactive())
		{
			// try to reactivate inactive account
			CB_LoginResult(pPeer, context, LOGIN_REACTIVATE_ACCOUNT);
			ObjectService_Proxy::GetAccountByName(gGlobal.pObjectService->pPeer, context, login_name,
				BOOLEAN_TRUE);
			DeleteNetObject(acctData);
		}
		else
		{
			// normal active account
			PROCEDURE_GO_STEP1(3, NetObject*, acctData)
			DeleteNetObject(acctData);
		}

	PROCEDURE_WAIT1(3, ObjectService, CB_GetAccountData, NetObject*, acctData)

		CAccountData *pAcctData = CastNetObject<CAccountData>(acctData);
		TRACE_ENSURE(pAcctData != NULL);

		UInt32 acct_id = pAcctData->GetAcct_id();
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_F("sessionId %s, acct_id %d, name %s, disconnect time %d"), 
			context.sessionId.GetString().c_str(), acct_id, loginName.c_str(),
			pAcctData->GetDisconnect_time());	

		Session* psess = GetSession(context.sessionId);
		if (psess == NULL) {
			psess = AddSession(context.sessionId, Session());
			psess->SetPeer(pPeer);
			psess->SetSessionId(context.sessionId);
		}

		CAccount* pAccount = gGlobal.GetAccount(acct_id);
		//if (pAccount)
		//	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("WorldService::LoginAccount - the disconnect time value in existing account: %d"), pAccount->GetDisconnect_time());			
		bool needKick = (pAccount && pAccount->IsConnected());
		if (pAccount == NULL) {
			pAccount = CAccount::NewInstance();
			pAccount->SetData(pAcctData);
			pAccount->SetSendMask(CAccountData::AllMask);
			gGlobal.m_accountMap[acct_id] = pAccount;
		}
		//// set the most updated AntiAddiction data
		pAccount->SetDisconnect_time(pAcctData->GetDisconnect_time());
		pAccount->SetOnLnTime(pAcctData->GetOnLnTime());
		pAccount->SetOffLnTime(pAcctData->GetOffLnTime());
		pAccount->Connect(context.sessionId);

		//TRACE_INFODTL_1(GLOBAL_LOGGER, _F("WorldService::LoginAccount - the disconnect time value in new account: %d"), pAccount->GetDisconnect_time());	
		psess->pAccount = pAccount;
		psess->ip = ip;
		DeleteNetObject(acctData);

		// login success /w kick need
		CB_LoginResult(pPeer, context, needKick ? LOGIN_SUCCESSED_NEED_KICK : LOGIN_SUCCESSED);
		if (gGlobal.pLogService)
			LogService_Proxy::LogAccount(gGlobal.pLogService->pPeer, context, 
				pAccount->GetAcct_id(), ip, LogService_Proxy::Account::ACT_LOGIN);
		if (needKick) {								// account already login
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("kick confirm, acct_id %d, sessionId %s"), 
				acct_id, context.sessionId.GetString().c_str());
			// kick waiting client if have
			if (pAccount->m_waitSessId.serviceId != INVALID_SERVICE_ID)
			{
				LPCPEER waitPeer = WorldApplication::GetInstance().GetProxy(
									pAccount->m_waitSessId.serviceId);
				TRACE_INFODTL_1(GLOBAL_LOGGER, 
					_F("CB_ForceLogout, wait session kick, sessionId %s"), 
					pAccount->m_waitSessId.GetString().c_str());
				CB_ForceLogout(waitPeer, RPCContext(pAccount->m_waitSessId), 
					_T("MSG_VERIFY_FAILED"));
				RemoveSession(pAccount->m_waitSessId);
			}
			pAccount->m_waitSessId = context.sessionId;
			// direct doing kick without asking client
			KickAccount(pPeer, context, BOOLEAN_TRUE);
		} else {
			pAccount->SetSessionId(context.sessionId);

			UInt32 waitTime = 0;
			if (pAccount->GetLoginChar() == NULL)
				waitTime = gGlobal.CalcWaitTime();
			if (waitTime == 0)
				ReturnAccount(context.sessionId);
			else {
				CB_WaitAccount(pPeer, context, waitTime);	// need to wait account
				gGlobal.AddWaitQueue(context.sessionId);
			}
		}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_TIMEOUT

			CB_LoginResult(pPeer, context, LOGIN_FAILED_ANTIHACK_TIMEOUT);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ForceLogout, exc. timeout, sessionId %s"), context.sessionId.GetString().c_str());
			CB_ForceLogout(pPeer, context, _T("MSG_VERIFY_FAILED"));

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			CB_LoginResult(pPeer, context, LOGIN_FAILED_ACC_NOT_EXISTS);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ForceLogout, exc., sessionId %s"), context.sessionId.GetString().c_str());
			CB_ForceLogout(pPeer, context, _T("MSG_VERIFY_FAILED"));

		PROCEDURE_EXCEPTION_DEFAULT

			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ForceLogout, exc. default, sessionId %s"), context.sessionId.GetString().c_str());
			CB_ForceLogout(pPeer, context, _T("MSG_VERIFY_FAILED"));
			RemoveSession(context.sessionId);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GetMapLines(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id)
{
	// return map line count
	Int32Vector countList;
	UInt16Vector stateList;
	bool found = false;

	for (int i = 0; i < (int)gGlobal.m_lineCounts.size(); i++)
	{
		stateList.push_back(LINE_STATE_REPAIR);
		countList.push_back(-1);
	}

	for (MapIndexList::iterator itrID = gGlobal.m_mapIndices.begin(); itrID != gGlobal.m_mapIndices.end(); itrID++)
	{
		if (itrID->first.map_id == map_id)
		{
			countList[itrID->first.line_id] = 0;
			found = true;
		}
	}

	for (int i = 0; i < (int)gGlobal.m_lineCounts.size(); i++)
	{
		if (countList[i] == 0 || !found)
		{
			countList[i] = gGlobal.m_lineCounts[i];
			
			if (countList[i] < gConfigure.normalLineState)
				stateList[i] = LINE_STATE_NORMAL;
			else if (countList[i] < gConfigure.busyLineState)
				stateList[i] = LINE_STATE_BUSY;
			else
				stateList[i] = LINE_STATE_SUPERFULL;
		}
	}

	return CB_GetMapLines(pPeer, context, countList, stateList);
}

RPCResult WorldService_Stub::ValidateMap(LPCPEER pPeer, RPCContext &context, 
										 const UInt16 &map_id, const UInt8 &line_id, 
										 const UInt32 &owner_id)
{
	const MapInfo &mInfo = gGlobal.GetAvailMapInfo(MapIndex(map_id, line_id, owner_id));
	if (mInfo.pPeer == NULL)
	{
		CB_Error(pPeer, context, 1, _T("ValidateMap Error: invalid map peer"));
		return RPC_RESULT_FAIL;
	}

	CB_ValidMap(pPeer, context, mInfo.bOpen);
	return RPC_RESULT_OK;
}


// RPC Jump Map
RPCResult WorldService_Stub::CharJumpMap(LPCPEER pPeer, RPCContext &context, 
										 const EnterInfo &info,
										 const UInt32Vector &memberIdList,
										 const AntiAddictInfo &addictInfo,
										 const String & buffCache,
										 const UInt32 &uiLineupId,
										 const int &iKeyPos)
{
	CCharacterInfo *pCharInfo = FindNetObject<CCharacterInfo>(info.charNetId);
	if (pCharInfo == NULL)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CharJumpMap Error: CharInfo netID %d,%d not found"),
			info.charNetId.srvId, info.charNetId.objId);
		CB_Error(pPeer, context, 1, _T("CharJumpMap Error: CharInfo netID not found"));
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_RPC6(WorldService, CharJumpMap, NULL, EnterInfo, info,
						UInt32Vector, memberIdList, AntiAddictInfo, addictInfo, 
						String, buffCache,UINT32,uiLineupId,int,iKeyPos)

		// just validate map peer
		TRACE_INFODTL_7(GLOBAL_LOGGER, 
			_F("sessionId %s, char_id %d, map_id %d, line_id %d, owner_id %d, x %d, y %d"),
			context.sessionId.GetString().c_str(), pCharInfo->GetChar_id(), info.map_id, 
			info.line_id, info.ownerId, info.x, info.y);

	// 1 second wait for timeout
	PROCEDURE_WAIT_TIMEOUT(1, JUMP_MAP_TIME)
	
		const MapInfo &mInfo = gGlobal.GetAvailMapInfo(
			MapIndex(info.map_id, info.line_id, info.ownerId));
		LPCPEER pMapPeer = mInfo.pPeer;
		if (pMapPeer == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharJumpMap Error: invalid map peer"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (gGlobal.IsCharacterEnterLocked(pCharInfo->GetChar_id()))
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("(1) CharacterEnter locked, char_id %d"), pCharInfo->GetChar_id());
			CB_Error(pPeer, context, 1, _T("CharJumpMap Error: same char entering in system"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CCharacterData charData;
		charData.SetMap(info.map_id, info.x, info.y, mInfo.index.owner_id);
		ObjectService_Proxy::UpdateCharacter(gGlobal.pObjectService->pPeer, context,
			pCharInfo->GetChar_id(), &charData);

	PROCEDURE_WAIT(2, ObjectService, CB_UpdateCharacter)
		
		if (pCharInfo->IsDisconnect()) 
		{
			PROCEDURE_RETURN
		}

		Session* psess = GetSession(context.sessionId);
		if (psess == NULL || psess->pAccount == NULL) 
		{
			PROCEDURE_RETURN
		}

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CharJumpMap: char_id %d, line_id %d, owner_id %d"), 
			pCharInfo->GetChar_id(), info.line_id, info.ownerId);

		// go to destination map
		const MapInfo &mInfo = gGlobal.GetAvailMapInfo(
			MapIndex(info.map_id, info.line_id, info.ownerId));
		LPCPEER pMapPeer = mInfo.pPeer;
		if (pMapPeer == NULL)
		{
			CB_Error(pPeer, context, 1, _T("CharJumpMap Error: invalid map peer"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		if (gGlobal.IsCharacterEnterLocked(pCharInfo->GetChar_id()))
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("(2) CharacterEnter locked, char_id %d"), pCharInfo->GetChar_id());
			CB_Error(pPeer, context, 1, _T("CharJumpMap Error: same char entering in system"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("char_id %d, line_id %d"), pCharInfo->GetChar_id(), pCharInfo->GetLine_id());

		// change line count
		if (pCharInfo->GetStatus() != CCharacterInfo::ST_DISCONNECT)
			gGlobal.ChgLineCount(pCharInfo->GetLine_id(), mInfo.index.line_id);	
		pCharInfo->SetMap(info.map_id, mInfo.index.line_id, mInfo.index.owner_id, info.x, info.y);

		EnterInfo newInfo = info;
		newInfo.line_id = mInfo.index.line_id;
		newInfo.ownerId = mInfo.index.owner_id;

		MapService_Proxy::CharacterEnter(pMapPeer, context, newInfo, memberIdList, addictInfo,
			buffCache,uiLineupId,iKeyPos);

	//PROCEDURE_WAIT(3, MapService, CB_CharacterEnter)

		// tell original map service jump success
	//	CB_JumpSuccess(pPeer, context);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CharJumpMap Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

		PROCEDURE_EXCEPTION2(MapService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CharJumpMap Error: MapService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::NpcJumpMap(LPCPEER pPeer, RPCContext &context, 
										const UInt32 &npc_id, const UInt16 &map_id, const UInt8 &line_id, 
										const UInt16 &map_x, const UInt16 &map_y,
										const UInt32 &followId)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::CharUpdateLocation(LPCPEER pPeer, RPCContext &context, 
												const EnterInfo &info)
{
	// just validate map peer
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("CharUpdateLocation: sessionId %s, map_id %d, line_id %d, owner_id %d, x %d, y %d"),
		context.sessionId.GetString().c_str(), info.map_id, info.line_id, info.ownerId, 
		info.x, info.y);

	CCharacterInfo *pCharInfo = FindNetObject<CCharacterInfo>(info.charNetId);
	if (pCharInfo == NULL)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CharUpdateLocation Error: CharInfo netID %d,%d not found"),
			info.charNetId.srvId, info.charNetId.objId);
		CB_Error(pPeer, context, 1, _T("CharUpdateLocation Error: CharInfo netID not found"));
		return RPC_RESULT_FAIL;
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("char_id %d, line_id %d"), pCharInfo->GetChar_id(), pCharInfo->GetLine_id());

	if (pCharInfo->GetStatus() != CCharacterInfo::ST_DISCONNECT)
		gGlobal.ChgLineCount(pCharInfo->GetLine_id(), info.line_id);		// change line count
	pCharInfo->SetMap(info.map_id, info.line_id, info.ownerId, info.x, info.y);
	
	// callback update success
	CB_UpdateLocation(pPeer, context);

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::VersionChecking(LPCPEER pPeer, RPCContext &context, const UInt32 &versionNo)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::WorldCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &srcAcct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Account %d on acct_id %d Char_id %d CheatCode: %s"), 
		srcAcct_id, target_acct_id, target_char_id, command.c_str());

	// check if global cheat
	if (DoGlobalCheat(pPeer, context, exe_id, srcAcct_id, target_acct_id, target_char_id, command))
		return RPC_RESULT_OK;

    CCharacterInfo *pCharInfo = gGlobal.GetCharInfo(target_char_id);
	if (pCharInfo == NULL)
	{
		if (gGlobal.pLogService)
			LogService_Proxy::LogCheat(gGlobal.pLogService->pPeer, context, exe_id, srcAcct_id, target_acct_id, target_char_id, command, LOGCHEAT_ERR_CHARINFONOTFOUND);
		TRACE_WARNDTL_3(GLOBAL_LOGGER, _F("WorldCheat Error: [CharInfo not found], Account %d on Char_id %d CheatCode: %s"), 
			srcAcct_id, target_char_id, command.c_str());
		return RPC_RESULT_FAIL;
	}
	// just validate map peer
	LPCPEER pMapPeer = gGlobal.GetMapInfo(MapIndex(pCharInfo->GetMap_id(), 
		pCharInfo->GetLine_id(), pCharInfo->GetMap_ownerId())).pPeer;
	if (pMapPeer == NULL)
	{
		if (gGlobal.pLogService)
			LogService_Proxy::LogCheat(gGlobal.pLogService->pPeer, context, exe_id, srcAcct_id, target_acct_id, target_char_id, command, LOGCHEAT_ERR_MAPNOTFOUND);
		CB_Error(pPeer, context, 1, _T("WorldCheat Error: invalid map peer"));
		return RPC_RESULT_FAIL;
	}

	 TCHAR *buf = SafeAllocate(TCHAR, command.size() + 1);

	_tcscpy(buf, command.c_str());
	TCHAR *cmd = NULL; 
	TCHAR *delim = _T(" ");
	TCHAR *arg1;
	cmd = _tcstok(buf, delim);
	
	if (cmd && (_tcsicmp(cmd, _T("kick"))==0 || _tcsicmp(cmd, _T("superkick"))==0))
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			CAccount* pAccount = gGlobal.GetAccount(pCharInfo->GetAcct_id());
			UInt banMinute = _ttoi(arg1);
			if (banMinute > 0 && pAccount)
			{
				time_t now;
				time(&now);
				pAccount->SetBanTime((UInt32)(now + 60 * banMinute));
				CAccountData* pData = CreateNetObject<CAccountData>();
				pData->ClearUpdateMask();
				pData->SetBanTime(pAccount->GetBanTime());
				pData->SetSendMask(CAccountData::banTimeMask);
				ObjectService_Proxy::UpdateAccount(gGlobal.pObjectService->pPeer, context, pCharInfo->GetAcct_id(), pData);
				DeleteNetObject(pData);
			}
		}
	}

	SafeDeallocate(buf);

	MapService_Proxy::CharacterCheat(pMapPeer, context, exe_id, srcAcct_id, target_acct_id, target_char_id, command);

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GetWorldGameTime(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("WorldService_Stub::GetWorldGameTime"));

	PROCEDURE_START_RPC(WorldService, GetWorldGameTime, NULL)

		SYSTEMTIME st;
		gGlobal.GetLocalTime2(&st);
		
		WorldService_Stub::CB_GetWorldGameTime(pPeer, context, st);

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(MapService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("GetWorldGameTime:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType)
{
	if (rankType < MAX_CHAR_RANK_TYPE)
	{
		Session* psess = GetSession(context.sessionId);
		if (psess != NULL) 
		{
			if (gGlobal.m_Rankings[rankType])
			{
				gGlobal.PreSendRankGroup(gGlobal.m_Rankings[rankType]);
				CB_GetCharacterRank(psess->GetPeer(), context, gGlobal.m_Rankings[rankType], rankType);
			}		
		}
	}
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GetGuildRank(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	if (psess != NULL) {
		if (gGlobal.m_GuildRanks)
		{
			gGlobal.PreSendGuildRankGroup(gGlobal.m_GuildRanks);
			CB_GetGuildRank(psess->GetPeer(), context, gGlobal.m_GuildRanks);
		}		
	}

	return RPC_RESULT_OK;
}

VOID WorldService_Stub::ReturnAccount(SessionID &sessionId)
{
	Session *psess = GetSession(sessionId);
	if (psess == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("ReturnAccount Error: no session found, sessionId %s"),
			sessionId.GetString().c_str());
		return;
	} else if (psess->pAccount == NULL) {
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("ReturnAccount Error: null account, sessionId %s"),
			sessionId.GetString().c_str());
		RemoveSession(sessionId);
		return;
	}
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ReturnAccount: sessionId %s"),
		sessionId.GetString().c_str());

	// try to get proxy peer by service Id
	LPCPEER pPeer = WorldApplication::GetInstance().GetProxy(sessionId.serviceId);
	if (pPeer == NULL) // still no peer
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("ReturnAccount Error: null proxy peer, sessionId %s"),
			sessionId.GetString().c_str());
		return;
	}

	SYSTEMTIME st;
	RPCContext context(sessionId);

	memset(&st, 0, sizeof(SYSTEMTIME));
	::GetLocalTime(&st);
	WORD wDH = 0;
	if ( gGlobal.m_serviceTime.wHour > st.wHour )
		wDH = 24 - (gGlobal.m_serviceTime.wHour - st.wHour);
	else
		wDH = (st.wHour - gGlobal.m_serviceTime.wHour);
	st.wHour = wDH;
	psess->SetOwnObject(psess->pAccount);
	CB_LoginAccount(pPeer, context, psess->pAccount, st);

	if (gGlobal.m_loginTime > (UInt32) gConfigure.loginInterval)
		gGlobal.m_loginTime -= gConfigure.loginInterval;
	else
		gGlobal.m_loginTime = 0;
}

void WorldService_Stub::ReleaseAccount(const SessionID &sessionId, Boolean bDelete)
{
	Session *psess = GetSession(sessionId);
	if (psess == NULL || psess->pAccount == NULL || psess->pAccount->m_lastSessId != sessionId)
		return;

	RPCContext context(sessionId);
	CAccount* pAccount = psess->pAccount;
	CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ReleaseAccount: acct_id %d, sessionId %s, char_id %d"), 
		pAccount->GetAcct_id(), sessionId.GetString().c_str(), 
		(pCharInfo ? pCharInfo->GetChar_id() : 0));

	if (pCharInfo)		// last session character disconnect
	{
		pCharInfo->Disconnect(sessionId);
		pCharInfo->Release(sessionId);
	}
	else					// no selected character, then directly logout
	{
		// delete account 
		gGlobal.m_accountMap.erase(pAccount->GetAcct_id());
		ObjectService_Proxy::ReleaseAccount(gGlobal.pObjectService->pPeer, context, 
			pAccount->GetAcct_id());
		DeleteNetObject(pAccount);
	}

	// try to remove session id in waiting queue
	gGlobal.RemoveWaitQueue(sessionId);
	RemoveSession(sessionId);
}

void WorldService_Stub::TakeOverAccount(const SessionID &sessionId)
{
	Session *psess = GetSession(sessionId);
	if (psess == NULL || psess->pAccount == NULL || psess->pAccount->m_lastSessId != sessionId)
		return;

	SessionID waitSid = psess->pAccount->m_waitSessId;
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("TakeOverAccount: session Id %s, wait session Id %s"), 
		sessionId.GetString().c_str(), waitSid.GetString().c_str());

	RPCContext context(sessionId);
	CAccount* pAccount = psess->pAccount;
	CCharacterInfo* pCharInfo = pAccount->GetLoginChar();

	if (pCharInfo)			//  disconnect last login character
	{
		pCharInfo->Disconnect(sessionId);
		pCharInfo->Release(sessionId);
	}

	// another player waiting login
	if (waitSid.serviceId != INVALID_SERVICE_ID) {
		psess = GetSession(waitSid);
		pAccount->m_waitSessId.serviceId = INVALID_SERVICE_ID;
		if (psess == NULL) {
			TRACE_WARNDTL_1(GLOBAL_LOGGER, 
				_T("TakeOverAccount Warning: invalid wait session Id %s"), 
				waitSid.GetString().c_str());
			return;
		}
	}

	pAccount->Connect(waitSid);
	pAccount->SetSessionId(waitSid);		// should set account sessionId here

	UInt32 waitTime = 0;
	if (pCharInfo == NULL && gGlobal.RemoveWaitQueue(sessionId)) // remove in waiting queue
		waitTime = gGlobal.CalcWaitTime();
	if (waitTime == 0) {
		ReturnAccount(waitSid);
	} else {
		gGlobal.AddWaitQueue(waitSid);
		CB_WaitAccount(psess->GetPeer(), psess->GetContext(), waitTime);
	}
}

RPCResult WorldService_Stub::NotifyNpcCreate(LPCPEER pPeer, RPCContext &context, 
										   const NpcLiveInfo &npc)
{
	TRACE_INFODTL_6(GLOBAL_LOGGER, 
		_T("NotfiyNpcCreate: npcId %d, mapId %d, lineId %d, pos %d,%d, eventId %d"),
		npc.npcId, npc.mapId, npc.lineId, npc.x, npc.y, npc.eventId);

	NpcLiveInfo* pNpc = SafeCreateObject(NpcLiveInfo);
	*pNpc = npc;
	gGlobal.m_npcLiveMap.insert(std::make_pair(npc.npcId, pNpc));

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::NotifyNpcDead(LPCPEER pPeer, RPCContext &context, 
										   const NpcLiveInfo &npc)
{
	TRACE_INFODTL_6(GLOBAL_LOGGER, 
		_T("NotfiyNpcDead: npcId %d, mapId %d, lineId %d, pos %d,%d, eventId %d"),
		npc.npcId, npc.mapId, npc.lineId, npc.x, npc.y, npc.eventId);

	if (gGlobal.m_pTaskScheduler && npc.eventId > 0) {
		CWorldEvent* pEvent = gGlobal.m_pTaskScheduler->GetEvent(npc.eventId);
		if (pEvent)
			pEvent->NotifyNpcDead(npc.npcId, npc.lineId);
	}

	NpcLiveInfo* pNpc;
	for (NpcLiveMap::iterator it = gGlobal.m_npcLiveMap.find(npc.npcId);
		it != gGlobal.m_npcLiveMap.end() && it->first == npc.npcId; ++it)
	{
		pNpc = it->second;
		if (*pNpc == npc) {
			SafeDeleteObject(pNpc);
			gGlobal.m_npcLiveMap.erase(it);
			break;
		}
	}

	return RPC_RESULT_OK;
}


RPCResult WorldService_Stub::QueryNpcExist(LPCPEER pPeer, RPCContext &context, 
										   const NpcLiveList &npcList)
{
	bool found;
	NpcLiveInfo* pNpc;
	NpcLiveList deadNpcList;
	String s;

	for (NpcLiveList::const_iterator it = npcList.begin(); it != npcList.end(); ++it)
	{
		found = false;
		const NpcLiveInfo &npc = *it;
		for (NpcLiveMap::iterator it2 = gGlobal.m_npcLiveMap.find(npc.npcId);
			it2 != gGlobal.m_npcLiveMap.end() && it2->first == npc.npcId; ++it2)
		{
			pNpc = it2->second;
			if (pNpc->npcId == npc.npcId && pNpc->mapId == npc.mapId && 
				pNpc->x == npc.x && pNpc->y == npc.y)
			{
				found = true;
				break;
			}
		}
		// assume npc exist in instance
		if (!found && gGlobal.GetMapType(npc.mapId) > 2)
			found = true;
		// find in npc creation list
		/*
		if (!found && gGlobal.GetMapType(npc.mapId) > 2)
		{
			for (NpcCreateMap::iterator it3 = gGlobal.m_npcCreateMap.begin(); 
				!found && it3 != gGlobal.m_npcCreateMap.end(); ++it3)
			{
				if (it3->first.map_id == npc.mapId)
				{
					NpcCreateList *pNpcList = it3->second;
					for (NpcCreateList::iterator it4 = pNpcList->begin(); 
						it4 != pNpcList->end(); ++it4)
					{
						const NpcCreateType &npc2 = *it4;
						if (npc2.npc_id == npc.npcId && npc2.x == npc.x && npc2.y == npc.y)
						{
							found = true;
							break;
						}
					}
				}
				if (it3->first.map_id > npc.mapId)
					break;
			}
		}
		*/
		// if still not found, npc is dead
		if (!found)
		{
			s.AppendFormat(_T("%d(%d:%d %d,%d), "), npc.npcId, npc.mapId, npc.ownerId, npc.x, npc.y);
			deadNpcList.push_back(npc);
		}
	}
	// return those npc not exist
	if (!s.IsEmpty())
		TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CB_NpcNotExist: %s"), s.c_str());
	
	CB_NpcNotExist(pPeer, context, deadNpcList);

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::KickAccount(LPCPEER pPeer, RPCContext &context, const Boolean &bKick)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL || psess->pAccount == NULL) {
		TRACE_WARNDTL(GLOBAL_LOGGER, 
			_T("KickAccount Warrning: null session or null account"));
		CB_Error(pPeer, context, 1, "LoginAccount Warrning: null session or null account");
		return RPC_RESULT_FAIL;
	}

	CAccount* pAccount = psess->pAccount;
	// if this session is waiting login
	if (pAccount->m_waitSessId == context.sessionId && pAccount->IsConnected())
	{
		// prevent duplicate login by kicking either old client or new client
		if (!bKick)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_F("CB_ForceLogout, kick false, acct_id %d, sessionId %s"), 
				pAccount->GetAcct_id(), context.sessionId.GetString().c_str());
			CB_ForceLogout(pPeer, context, _T("MSG_LOGIN_KICK"));
			RemoveSession(context.sessionId);
			pAccount->m_waitSessId = SessionID();
		}
		else
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_F("CB_ForceLogout, kick true, acct_id %d, sessionId %s"), 
				pAccount->GetAcct_id(), pAccount->m_lastSessId.GetString().c_str());
		
			RPCContext context2(pAccount->m_lastSessId);
			CCharacterInfo* pCharInfo = pAccount->GetLoginChar();
			if (pCharInfo)			// disconnect client first
				pCharInfo->Disconnect(pAccount->m_lastSessId);

			LPCPEER proxyPeer = WorldApplication::GetInstance().GetProxy(
				pAccount->m_lastSessId.serviceId);
			CB_ForceLogout(proxyPeer, context2, _T("MSG_LOGIN_KICK"));
		}
	}

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::SendCheatCommand(LPCPEER pPeer, RPCContext &context, NetGroup* cheatCommandGroup)
{
	for( Index i = C_INDEX(0); i < cheatCommandGroup->GetCount(); ++i )
	{
		CExecuteCheatData* data = cheatCommandGroup->GetChild<CExecuteCheatData>(i);
		if( data )
			WorldCheat(pPeer, context, data->GetExe_id(), data->GetAcct_id(), data->GetTarget_acct_id(), data->GetTarget_char_id(), data->GetCommand());
	}
	DeleteNetGroup(cheatCommandGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::AnswerAntiHack(LPCPEER pPeer, RPCContext &context, const UInt8 &answer)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::CanChangeLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id)
{
	Boolean canChange = BOOLEAN_FALSE;

	if (line_id >= 0 && line_id < gGlobal.m_lineCounts.size())
	{
		if (gGlobal.m_lineCounts[line_id] < gConfigure.busyLineState)
			canChange = BOOLEAN_TRUE;
	}

	CB_CanChangeLine(pPeer, context, canChange);

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::CheckCanBuyLimitedItem(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const UInt16 &stack, const UInt32 &char_id)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckCanBuyLimitedItem: session %s char_id %d chargeShopID %d stack %d"), context.sessionId.GetString().c_str(), char_id, chargeShopID, stack);	
	CB_CheckCanBuyLimitedItem(pPeer, context, gGlobal.m_pChargeShopControl->CanBuyLimitedItem(chargeShopID, stack, char_id));
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::ReduceLimitItem(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const Boolean &buySuccess, const UInt16 &reduceStack, const UInt32 &char_id)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("ReduceLimitItem: session %s char_id %d chargeShopID %d buySuccess %d"), context.sessionId.GetString().c_str(), char_id, chargeShopID, (buySuccess)?1:0);	
	gGlobal.m_pChargeShopControl->ReduceLimitItem(chargeShopID, buySuccess, reduceStack, char_id);
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::ForceLogout(LPCPEER pPeer, RPCContext &context, 
										 const UInt32 &acct_id, const UInt32 &char_id,
										 const String &msg)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL || psess->pAccount == NULL)
		return RPC_RESULT_FAIL;

	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_T("ForceLogout: session %s, acct_id %d, char_id %d, %s"),
		context.sessionId.GetString().c_str(), acct_id, char_id, msg.c_str());

	LPCPEER pProxyPeer = WorldApplication::GetInstance().GetProxy(context.sessionId.serviceId);
	// use procedure for safety
	PROCEDURE_DO_RPC2(WorldService, ReleaseSession, NULL, pProxyPeer, context, 
		SessionID, context.sessionId, Boolean, BOOLEAN_TRUE)
	PROCEDURE_DO_RPC2(WorldService, CharacterLeave, NULL, pPeer, context, 
		UInt32, acct_id, UInt32, char_id)
	//ReleaseSession(pProxyPeer, context, context.sessionId, BOOLEAN_TRUE);
	//CharacterLeave(pPeer, context, acct_id, char_id);
	CB_ForceLogout(pProxyPeer, context, msg);

	return RPC_RESULT_OK;
}

//just for cheat
RPCResult WorldService_Stub::StartEvent(LPCPEER pPeer, RPCContext &context, const UInt32 &eventId)
{
	CWorldEvent* pEvent = gGlobal.m_pTaskScheduler->GetEvent(eventId);
	if(pEvent != NULL)
	{
		pEvent->StopEvent();
		pEvent->StartEvent();
		//RPCContext context;
		//NetGroup grp;
		//LPCPEER pPeer;

		//grp.Add(pEvent);
		//for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
		//	it != gGlobal.m_mapPeers.end(); ++it)
		//{
		//	pPeer = *it;
		//	if (pPeer)
		//		WorldService_Stub::CB_StartEvent(pPeer, context, &grp);
		//}
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("WorldService_Stub::StartEvent,eventId:%d"),eventId);
	}
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::RegisterInstance(LPCPEER pPeer, RPCContext &context, 
											  const UInt16 &instId, const UInt32 &owner_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("instId %d, owner_id %d"), instId, owner_id);
	gGlobal.m_instanceMap[InstanceIndex(instId, owner_id)] = pPeer;

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::UnregisterInstance(LPCPEER pPeer, RPCContext &context, 
												const UInt16 &instId, const UInt32 &owner_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("instId %d, owner_id %d"), instId, owner_id);
	gGlobal.m_instanceMap.erase(InstanceIndex(instId, owner_id));

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::QueryInstance(LPCPEER pPeer, RPCContext &context, 
										   const UInt16 &instId, const UInt32 &owner_id)
{
	PROCEDURE_START_RPC2(WorldService, QueryInstance, NULL, UInt16, instId, UInt32, owner_id)

		InstanceMap::iterator it = gGlobal.m_instanceMap.find(InstanceIndex(instId, owner_id));
		Boolean bExist = (it != gGlobal.m_instanceMap.end());
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("instId %d, ownerId %d, exist %d"), 
			instId, owner_id, bExist);
		if (!bExist)
		{
			CB_QueryInstance(pPeer, context, BOOLEAN_FALSE, 0);
			PROCEDURE_RETURN
		}

		MapService_Proxy::QueryInstance(it->second, context, instId, owner_id);

	PROCEDURE_WAIT2(1, MapService, CB_QueryInstance, Boolean, bExist, UInt16, allowCount)

		CB_QueryInstance(pPeer, context, bExist, allowCount);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT

			CB_QueryInstance(pPeer, context, BOOLEAN_FALSE, 0);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::RemoteCreateNpc(LPCPEER pPeer, RPCContext &context, 
											 const UInt16 &map_id, const UInt8 &line_id, 
											 const UInt32 &owner_id, const UInt32 &npc_id,
											 const UInt16 &x, const UInt16 &y, 
											 const UInt32 &lifeTime, const UInt16 &eventId,
											 const UInt32 &char_id)
{
	TRACE_INFODTL_6(GLOBAL_LOGGER, 
		_F("npc_id %d, map_id %d, line_id %d, owner_id %d, x %d, y %d"), 
		npc_id, map_id, line_id, owner_id, x, y);

	MapIndex mapidx(map_id, line_id, owner_id);
	const MapInfo& mapInfo = gGlobal.GetMapInfo(mapidx);

	NpcCreateType npc;
	npc.npc_id = npc_id;
	npc.x = x;
	npc.y = y;
	npc.lifeTime = lifeTime;
	npc.eventId = eventId;
	npc.char_id = char_id;
	if (mapInfo.pPeer && mapInfo.bOpen)
	{
		NpcCreateList npcList;
		npcList.push_back(npc);
		MapService_Proxy::RemoteCreateNpc(mapInfo.pPeer, context, map_id, line_id, 
			owner_id, npcList);
	}
	else
	{
		// store npc information for further creation
		NpcCreateList* pNpcList = NULL;
		NpcCreateMap::iterator it = gGlobal.m_npcCreateMap.find(mapidx);

		if (it != gGlobal.m_npcCreateMap.end())
			pNpcList = it->second;
		if (pNpcList == NULL)
		{
			pNpcList = SafeCreateObject(NpcCreateList);
			gGlobal.m_npcCreateMap[mapidx] = pNpcList;
		}
		pNpcList->push_back(npc);
	}

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::StoreApolloLog(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &msgTime, const UInt16 &msgType,
											const String &typeName, const String &msg)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("sessionId %s, msgType %d, typeName %s, msg size %d"), 
		context.sessionId.GetString().c_str(), msgType, typeName.c_str(), msg.size());

	Session *psess = GetSession(context.sessionId);
	CAccount* pAccount = psess ? psess->pAccount : NULL;
	CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

	LogService_Proxy::LogHack(gGlobal.pLogService->pPeer, context, 
		pAccount ? pAccount->GetLoginName() : _T(""), 
		pCharInfo ? pCharInfo->GetNickName() : _T(""), 
		msgTime, msgType, typeName, msg);

	return RPC_RESULT_OK;
}


RPCResult WorldService_Stub::ApolloMsg(LPCPEER pPeer, RPCContext &context, const ByteArray &msg)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GetSellYuanBao(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	CAccount* pAccount = psess ? psess->pAccount : NULL;
	CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;
	if (psess && pCharInfo && gGlobal.m_pSellYuanBaoGroup)
	{
		NetGroup sendGrp;
		gGlobal.m_pSellYuanBaoGroup->InsertCharContext(pCharInfo->GetChar_id(), psess->GetContext());
		gGlobal.m_pSellYuanBaoGroup->GetSendGroup(&sendGrp);
		SetOwnObject(psess->GetSessionId(), gGlobal.m_pSellYuanBaoGroup);
		CB_GetSellYuanBao(psess->GetPeer(), psess->GetContext(), gGlobal.m_pSellYuanBaoGroup, &sendGrp);
	}
	
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money)
{
	CCharacterInfo* pCharInfo = gGlobal.GetCharInfo(targetId);
	if(pCharInfo)
	{
		if(gConfigure.prisonMapId == pCharInfo->GetMap_id() && money > 0)
		{
			CB_UpdateWantedReward(pPeer, context, BOOLEAN_FALSE);
			return RPC_RESULT_OK;
		}
		LPCPEER pMapPeer = gGlobal.GetMapInfo(MapIndex(pCharInfo->GetMap_id(), pCharInfo->GetLine_id(), pCharInfo->GetMap_ownerId())).pPeer;
		MapService_Proxy::UpdateWantedReward(pMapPeer,RPCContext(),targetId,money);
	
		if (gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE])
		{
			for(int i = 0; i < gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE]->GetCount(); i++)
			{
				CRankCharacter*pRank = CastNetObject<CRankCharacter>(gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE]->GetChild(i));
				if(pRank && pRank->GetChar_id() == targetId)
				{
					pRank->SetWantedReward(pRank->GetWantedReward() + money);
					break;
				}
			}
		}
		CB_UpdateWantedReward(pPeer, context, BOOLEAN_TRUE);
	}
	else
	{
	//	ObjectService_Proxy::UpdateWantedReward(gGlobal.pObjectService->pPeer,context,targetId,money);
		CB_UpdateWantedReward(pPeer, context, BOOLEAN_FALSE);
	}
	return RPC_RESULT_OK;

}


RPCResult WorldService_Stub::WantedDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	PROCEDURE_START_RPC1(WorldService, WantedDetail, NULL, UInt32, targetId)

	CCharacterInfo* pCharInfo = gGlobal.GetCharInfo(targetId);
	if(!pCharInfo)
	{
		WorldService_Stub::CB_WantedDetail(pPeer,context,String(),0,0,0,0);
		PROCEDURE_RETURN;
	}
	
	LPCPEER pMapPeer = gGlobal.GetMapInfo(MapIndex(pCharInfo->GetMap_id(), pCharInfo->GetLine_id(), pCharInfo->GetMap_ownerId())).pPeer;
	MapService_Proxy::GetPosition(pMapPeer,context,targetId);
	PROCEDURE_WAIT2(1, MapService, CB_GetPosition, UInt32, x,UInt32,y);
	CCharacterInfo* pCharInfo = gGlobal.GetCharInfo(targetId);
	if(!pCharInfo)
	{
		WorldService_Stub::CB_WantedDetail(pPeer,context,String(),0,0,0,0);
		PROCEDURE_RETURN;
	}
	WorldService_Stub::CB_WantedDetail(pPeer,context,pCharInfo->GetNickName(),pCharInfo->GetLine_id(),pCharInfo->GetMap_id(),x,y);
	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION_DEFAULT

		WorldService_Stub::CB_WantedDetail(pPeer,context,String(),0,0,0,0);

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
RPCResult WorldService_Stub::RemoveFromWantedRank(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE])
	{
		for(int i = 0; i < gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE]->GetCount(); i++)
		{
			CRankCharacter * pChar = gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE]->GetChild<CRankCharacter>(i);
			if(pChar && pChar->GetChar_id() == targetId)
			{
				gGlobal.m_Rankings[CHAR_RANK_TYPE_PKVALUE]->Remove(pChar);
				return RPC_RESULT_OK;
			}
		}
	}
	return RPC_RESULT_OK;
}
RPCResult WorldService_Stub::AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password)
{
	PROCEDURE_START_RPC3(WorldService, AddSellYuanBao, NULL, UInt32, YBT, UInt32, pricePerUnit, ByteArray, password)
		Session *psess = GetSession(context.sessionId);
		CAccount* pAccount = psess ? psess->pAccount : NULL;
		CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("char_id %d, YBT %d, pricePerUnit %d, sessionId %s"), 
			pCharInfo ? pCharInfo->GetChar_id() : 0, YBT, pricePerUnit, context.sessionId.GetString().c_str());

		if (pCharInfo && gGlobal.m_pSellYuanBaoGroup)
		{
			if (gGlobal.m_pSellYuanBaoGroup->CanAddSellYuanBao(pCharInfo->GetChar_id()))
			{
				ObjectService_Proxy::AddSellYuanBao(gGlobal.pObjectService->pPeer, context, pCharInfo->GetChar_id(), 0, YBT, pricePerUnit, password); 
			}
			else
			{
				CB_AddSellYuanBao(pPeer, context, SELL_YB_MEET_MAX_RECORD);
				PROCEDURE_RETURN
			}
		}
		else
		{
			NetGroup temp;
			PROCEDURE_GO_STEP1(2, NetGroup*, &temp)
		}

		PROCEDURE_WAIT2(2, ObjectService, CB_GetSellYuanBao, NetGroup*, sellYBDataGrp, UInt8, msgIndex)

			if (msgIndex == SELL_YB_ADD_RECORD_SUCCESS)
			{
			
				if (gGlobal.m_pSellYuanBaoGroup)
					gGlobal.m_pSellYuanBaoGroup->AddSellYuanBao(sellYBDataGrp);

				if (sellYBDataGrp->GetCount() > 0)
				{
					Session *psess = GetSession(context.sessionId);
					CAccount* pAccount = psess ? psess->pAccount : NULL;
					CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

					if (pCharInfo)
					{
						for (Index i = C_INDEX(0); i < sellYBDataGrp->GetCount(); ++i)
						{
							CSellYuanBaoData* pData = sellYBDataGrp->GetChild<CSellYuanBaoData>(i);
							if (pData)
							{
								LogService_Proxy::LogSellYuanBao(gGlobal.pLogService->pPeer, context, pAccount->GetAcct_id(), 
									pCharInfo->GetChar_id(), LogService_Proxy::SellYuanBao::SELL_YBT, 0, 0, YBT, pricePerUnit, 0, 1000, pData->GetExpireDate());
							}
						}
					}
				}
			}
			
			CB_AddSellYuanBao(pPeer, context, msgIndex);

			DeleteNetGroup(sellYBDataGrp, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT

			CB_AddSellYuanBao(pPeer, context, SELL_YB_ERROR);

			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Error YBT %d, pricePerUnit %d, sessionId %s"), 
			 YBT, pricePerUnit, context.sessionId.GetString().c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &charTotalMoney, const UInt32 &moneyPerUnit, const ByteArray &password)
{
	PROCEDURE_START_RPC5(WorldService, BuySellYuanBao, NULL, UInt32, sellYuanBaoID, UInt32, YBT, UInt32, charTotalMoney, UInt32, moneyPerUnit, ByteArray, password)

		Session *psess = GetSession(context.sessionId);
		CAccount* pAccount = psess ? psess->pAccount : NULL;
		CSellYuanBao* pData = gGlobal.m_pSellYuanBaoGroup->GetSellYuanBaoByID(sellYuanBaoID);

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("sellYuanBaoID %d, YBT %d, sessionId %s"), 
			sellYuanBaoID, YBT, context.sessionId.GetString().c_str());

		UInt msgIndex = BUY_YB_SUCCESS;
		do
		{
			if (pAccount == NULL)
			{
				msgIndex = BUY_YB_ERROR;
				break;
			}

			if (gGlobal.m_pSellYuanBaoGroup == NULL)
			{
				msgIndex = BUY_YB_ERROR;
				break;
			}
			
			if (pData == NULL || pData->GetRemain() < YBT)
			{
				msgIndex = BUY_YB_SOLD_ALL;
				break;
			}

			if (moneyPerUnit != pData->GetMoneyPerYBT())
			{
				msgIndex = BUY_YB_NO_MONEY;
				break;
			}

			time_t now;
			time(&now);

			 //if the object is near to expire(one min left), then also don't do the transition for safety
			if (pData->GetExpireDate() < (UInt)now + 60)
			{
				msgIndex = BUY_YB_EXPIRE;
				break;
			}

			if (YBT * pData->GetMoneyPerYBT() > charTotalMoney)
			{
				msgIndex = BUY_YB_NO_MONEY;
				break;
			}
		}
		while(false);

		if (msgIndex == BUY_YB_SUCCESS && pData)
		{
			pData->Reserve(YBT);
			ObjectService_Proxy::BuySellYuanBao(gGlobal.pObjectService->pPeer, context, pAccount->GetAcct_id(), sellYuanBaoID, YBT, password);
		}
		else
		{
			CB_BuySellYuanBao(pPeer, context, msgIndex, BOOLEAN_TRUE);
			PROCEDURE_RETURN
		}

	PROCEDURE_WAIT1(2, ObjectService, CB_BuySellYuanBao, UInt8, msgIndex)

		CSellYuanBao* pData = gGlobal.m_pSellYuanBaoGroup->GetSellYuanBaoByID(sellYuanBaoID);
		if (pData)
		{
			if (msgIndex == BUY_YB_SUCCESS)
			{
				pData->AddSold(YBT);
				
				Session *psess = GetSession(context.sessionId);
				CAccount* pAccount = psess ? psess->pAccount : NULL;
				CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

				if (pCharInfo)
				{
					LogService_Proxy::LogSellYuanBao(gGlobal.pLogService->pPeer, context, pAccount->GetAcct_id(), 
						pCharInfo->GetChar_id(), LogService_Proxy::SellYuanBao::BUY_YBT, pData->GetChar_id(), 0, YBT, pData->GetMoneyPerYBT(), 0, 0, 0);
				}
			}
			else
				pData->Reserve(-(Int)YBT);
		}

		CB_BuySellYuanBao(pPeer, context, msgIndex, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT

			CB_BuySellYuanBao(pPeer, context, BUY_YB_ERROR, BOOLEAN_TRUE);

			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Error sellYuanBaoID %d, YBT %d, sessionId %s"), 
			sellYuanBaoID, YBT, context.sessionId.GetString().c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GetTempBank(LPCPEER pPeer, RPCContext &context, const ByteArray &password)
{
	PROCEDURE_START_RPC1(WorldService, GetTempBank, NULL, ByteArray, password)

		Session *psess = GetSession(context.sessionId);
		CAccount* pAccount = psess ? psess->pAccount : NULL;
		CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("char_id %d, sessionId %s"), 
			pCharInfo ? pCharInfo->GetChar_id() : 0, context.sessionId.GetString().c_str());

		if (pCharInfo)
		{
			ObjectService_Proxy::GetTempBank(gGlobal.pObjectService->pPeer, context, pCharInfo->GetChar_id(), password);
		}
		else
		{
			UInt32Vector temp;
			PROCEDURE_GO_STEP3(2, UInt32Vector, temp, UInt32, 0, UInt32, 0)
		}

		PROCEDURE_WAIT3(2, ObjectService, CB_GetTempBank, UInt32Vector, deleteSellYBIDs, UInt32, remainYBT, UInt32, totalMoney)


			Session *psess = GetSession(context.sessionId);
			CAccount* pAccount = psess ? psess->pAccount : NULL;
			CCharacterInfo* pCharInfo = pAccount ? pAccount->GetLoginChar() : NULL;

			if (pCharInfo)
			{
				gGlobal.m_pSellYuanBaoGroup->RemoveExpireList(pCharInfo->GetChar_id(), deleteSellYBIDs);
				if (remainYBT > 0 || totalMoney > 0)
				{
					LogService_Proxy::LogSellYuanBao(gGlobal.pLogService->pPeer, context, pAccount->GetAcct_id(), 
						pCharInfo->GetChar_id(), LogService_Proxy::SellYuanBao::GET_IN_TEMPBANK, 0, 0, remainYBT, 0, totalMoney, 0, 0);
				}
			}

			UInt8 msgIndex = TEMPBANK_SUCCESS;
			if (remainYBT == 0 && totalMoney == 0)
				msgIndex = YB_MSG_WRONG_PASSWORD;
			CB_GetTempBank(pPeer, context, msgIndex, remainYBT, totalMoney);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION_DEFAULT

			CB_GetTempBank(pPeer, context, TEMPBANK_ERROR, 0, 0);

			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Error sessionId %s"), 
				context.sessionId.GetString().c_str());


	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

#pragma optimize("", off)		// turn off "if-else" optimization here

Boolean WorldService_Stub::DoGlobalCheat(LPCPEER pPeer, RPCContext &context,
										 UInt32 exe_id, UInt32 src_acct_id, 
										 UInt32 target_acct_id, UInt32 target_char_id, 
										 const String &code)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("src_acct_id %d, %s"), src_acct_id, code.c_str());

	TCHAR *delim = _T(" ");
	TCHAR *cmd = NULL; 
	TCHAR *arg1, *arg2, *arg3, *arg4, *arg5, *arg6;
	TCHAR *buf = SafeAllocate(TCHAR, code.size() + 1);

	_tcscpy(buf, code.c_str());
	cmd = _tcstok(buf, delim);
	if (cmd == NULL)
		goto G_CHEAT_FAIL;

	if (_tcsicmp(cmd, _T("GM")) == 0 && gGlobal.pMessageService->pPeer)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1 && arg2)
		{
			if (_tcsicmp(arg1, _T("say")) == 0)
			{
				UInt16 type = _ttoi(arg2);
				if (type == 0)
				{
					arg3 = _tcstok(NULL, delim);
					if (arg3)
					{
						UInt16 count = _ttoi(arg3);
						String Msg = _tcstok(NULL, _T(""));
						MessageService_Proxy::SendHorseLightMessage(gGlobal.pMessageService->pPeer, 
							context, Msg, 0, count);
					}
				}
				else if (type == 1)
				{
					String Msg = _tcstok(NULL, _T(""));
					
					MessageService_Proxy::SendSystemMessage(gGlobal.pMessageService->pPeer, 
						context, Msg, 0);
				}
				else if (type == 2)
				{
					arg3 = _tcstok(NULL, delim);
					if (arg3)
					{
						String Msg = _tcstok(NULL, _T(""));
						UInt16 type = _ttoi(arg3);
						MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
							context, Msg, 0, (BYTE) type);
					}
				}
			}
		}
	}
	else if (_tcsicmp(cmd, _T("addMsgSchedule")) == 0 && gGlobal.pMessageService->pPeer)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		arg5 = _tcstok(NULL, delim);
		arg6 = _tcstok(NULL, delim);
		String msg = _tcstok(NULL, _T(""));

		if (arg1 && arg2 && arg3 && arg4 && arg5 && arg6 && !msg.IsEmpty())
		{
			UInt16 id = _ttoi(arg1);
			UInt32 startTime = _ttoi(arg2);
			UInt16 periodCount = _ttoi(arg3);
			UInt16 count = _ttoi(arg4);
			UInt32 period = _ttoi(arg5); 
			UInt8 msgType = _ttoi(arg6); 

			MessageService_Proxy::AddMsgSchedule(gGlobal.pMessageService->pPeer, 
				context, id, startTime, periodCount, count, period, msgType, msg, 0);
		}
	}
	else if (_tcsicmp(cmd, _T("removeMsgSchedule")) == 0 && gGlobal.pMessageService->pPeer)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 id = _ttoi(arg1);
			MessageService_Proxy::RemoveMsgSchedule(gGlobal.pMessageService->pPeer, 
				context, id);
		}
	}
	else if (_tcsicmp(cmd, _T("se")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			UInt32 eventId = _ttoi(arg1);
			StartEvent(pPeer, context, eventId);
		}
	}
	else if (_tcsicmp(cmd, _T("superkick")) == 0)
	{
		// force to kick account and remove procedure lock of that session
		arg1 = _tcstok(NULL, delim);
		UInt32 acct_id = arg1 ? _ttoi(arg1) : target_acct_id;
		CAccount* pAccount = gGlobal.GetAccount(acct_id);
		if (pAccount)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("superkick, acct_id %d, sessionId %s"),
				acct_id, pAccount->m_lastSessId.GetString().c_str());
			// remove procedure of that account, prevent procedure dead lock
			ProcedureManager::GetInstance().RemoveBySession(pAccount->m_lastSessId);
			CCharacterInfo* pCharInfo = pAccount->GetLoginChar();
			if (pCharInfo)
			{
				LPCPEER pMapPeer = gGlobal.GetMapInfo(MapIndex(pCharInfo->GetMap_id(), 
					pCharInfo->GetLine_id(), pCharInfo->GetMap_ownerId())).pPeer;
				MapService_Proxy::CharacterCheat(pMapPeer, RPCContext(), exe_id, 
					src_acct_id, acct_id, pCharInfo->GetChar_id(), code);
			}
			else
			{
				// direct kick account
				LPCPEER lastPeer = WorldApplication::GetInstance().GetProxy(pAccount->m_lastSessId.serviceId);
				CB_ForceLogout(lastPeer, RPCContext(pAccount->m_lastSessId), _T("MSG_LOGIN_KICK"));
			}
		}
	}
	else
	{
		goto G_CHEAT_FAIL;
	}

	LogService_Proxy::LogCheat(gGlobal.pLogService->pPeer, context, exe_id, src_acct_id,
		target_acct_id, target_char_id, code, 0);

	SafeDeallocate(buf);
	return BOOLEAN_TRUE;

G_CHEAT_FAIL:
	SafeDeallocate(buf);
	return BOOLEAN_FALSE;
}

#pragma optimize("", on)			// resume optimization here

RPCResult WorldService_Stub::DeleteGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	CCharacterInfo* pChar=gGlobal.GetCharInfo(uiTargetId);
	if (NULL != pChar) 
	{
		const MapInfo& mapinfo = gGlobal.GetMapInfo(MapIndex(pChar->GetMap_id(),pChar->GetLine_id(),pChar->GetMap_ownerId()));
		MapService_Proxy::DeleteGenerationFromWorld(mapinfo.pPeer,context,uiTargetId,uiRelatedId);
	}
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::RecvGenerationAccmulationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId,const UInt32 &uiAcc)
{
	CCharacterInfo* pChar=gGlobal.GetCharInfo(uiTargetId);
	if (NULL != pChar) 
	{
		const MapInfo& mapinfo = gGlobal.GetMapInfo(MapIndex(pChar->GetMap_id(),pChar->GetLine_id(),pChar->GetMap_ownerId()));
		MapService_Proxy::RecvGenerationAccmulation(mapinfo.pPeer,context,uiTargetId,uiRelatedId,uiAcc);
	}
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GenerationOnlineNotifyTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	CCharacterInfo* pChar=gGlobal.GetCharInfo(uiTargetId);
	if (NULL != pChar) 
	{
		const MapInfo& mapinfo = gGlobal.GetMapInfo(MapIndex(pChar->GetMap_id(),pChar->GetLine_id(),pChar->GetMap_ownerId()));
		MapService_Proxy::GenerationOnlineNotify(mapinfo.pPeer,context,uiTargetId,uiRelatedId);
	}
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GenerationOfflineNotifyTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	CCharacterInfo* pChar=gGlobal.GetCharInfo(uiTargetId);
	if (NULL != pChar) 
	{
		const MapInfo& mapinfo = gGlobal.GetMapInfo(MapIndex(pChar->GetMap_id(),pChar->GetLine_id(),pChar->GetMap_ownerId()));
		MapService_Proxy::GenerationOfflineNotify(mapinfo.pPeer,context,uiTargetId,uiRelatedId);
	}
	return RPC_RESULT_OK;
}


RPCResult WorldService_Stub::HasGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("npc_id = %d"), npc_id);
	NpcFlagIndex flagIndex(npc_id,DEFAULT_NPCFLAG_LINE_ID,owner_id,flag_id);
	CB_HasGlobalNpcFlag(pPeer,context,gGlobal.m_AllNpcFlags.find(flagIndex) != gGlobal.m_AllNpcFlags.end());
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::GetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("npc_id = %d"), npc_id);
	NpcFlagIndex flagIndex(npc_id,DEFAULT_NPCFLAG_LINE_ID,owner_id,flag_id);
	NpcFlagMap::iterator iter = gGlobal.m_AllNpcFlags.find(flagIndex);
	if (iter != gGlobal.m_AllNpcFlags.end())
	{
		CB_GetGlobalNpcFlag(pPeer,context,iter->second);
		return RPC_RESULT_OK;
	}
	CB_GetGlobalNpcFlag(pPeer,context,0);
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::SetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id, const UInt16 &flag)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("npc_id = %d"), npc_id);
	NpcFlagIndex flagIndex(npc_id,DEFAULT_NPCFLAG_LINE_ID,owner_id,flag_id);
	NpcFlagMap::iterator iter = gGlobal.m_AllNpcFlags.find(flagIndex);
	if (iter != gGlobal.m_AllNpcFlags.end() && iter->second == flag)
	{
		CB_SetGlobalNpcFlag(pPeer,context);
		return RPC_RESULT_OK;
	}

	gGlobal.m_AllNpcFlags[flagIndex] = flag;
	gGlobal.m_npcFlagChgSet.insert(flagIndex);

	CB_SetGlobalNpcFlag(pPeer,context);
	return RPC_RESULT_OK;
}

RPCResult WorldService_Stub::RemoveGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("npc_id = %d"), npc_id);
	NpcFlagIndex flagIndex(npc_id,DEFAULT_NPCFLAG_LINE_ID,owner_id,flag_id);
	NpcFlagMap::iterator iter = gGlobal.m_AllNpcFlags.find(flagIndex);
	if (iter != gGlobal.m_AllNpcFlags.end())
	{
		gGlobal.m_AllNpcFlags.erase(flagIndex);
		gGlobal.m_npcFlagChgSet.insert(flagIndex);
	}

	CB_RemoveGlobalNpcFlag(pPeer,context);
	return RPC_RESULT_OK;
}