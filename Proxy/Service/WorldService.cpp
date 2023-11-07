//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Proxy/Service/WorldService_proxy.h"
//-- Library
#ifdef QGL
#include "Proxy/Logic/Account.h"
#include "QGL/Global.h"
#include "QGL/AccountControl.h"
#include "QGL/MenuRankProcess.h"
#include "QGL/MenuOnlineShopProcess.h"
#include "QGL/WantedProcess.h"
#include "Proxy/Logic/SellYuanBaoGroup.h"
#include "QGL/RumorMsgProcess.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Resource/ResPet.h"
#include "Resource/ClientResManager.h"
#include "QGL/antihack.h"
#include "QGL/UI.h"
#include "QGL/ImageEntity.h"
#include "QGL/UIEntity.h"
#include "QGL/XMLPanel.h"
#include "QGL/Configure.h"

#include "Common/CallLuaFunc.h"
#include "WorldService.h"

#endif // QGL

#ifdef MAP_SERVICE
#include "Proxy/Logic/WorldEvent.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/Character.h"
#include "MapService/Global.h"
#endif // MapService

#include "ApolloClient.h"
#include "WorldService.h"

// NpcLiveInfo fucntions implementation
NpcLiveInfo::NpcLiveInfo() : npcId(0), mapId(0), lineId(0), x(0), y(0), eventId(0)
{
}

bool NpcLiveInfo::operator == (const NpcLiveInfo& b) const
{
	return (npcId == b.npcId && mapId == b.mapId && lineId == b.lineId &&
			x == b.x && y == b.y && eventId == b.eventId);
}

RPCResult WorldService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("World Error: %d: %s"), err, errMsg.c_str());

	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetMapLines(LPCPEER pPeer, RPCContext &context, 
											 const Int32Vector &lineCounts, const UInt16Vector &lineState)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GetMapLines:"));

	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_ValidMap(LPCPEER pPeer, RPCContext &context, const Boolean &bOpen)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_JumpSuccess(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_UpdateLocation(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_LoginResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result)
{
#ifdef QGL
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SwitchOffAntiHackVerification", String());
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("result %d"), result);
	gGlobal.m_logined = result;
	if (result == LOGIN_SUCCESSED_NEED_KICK)
		gGlobal.g_pAccountControl->ConfirmKick(context);

	if (result == LOGIN_REACTIVATE_ACCOUNT)
	{
		String msg = gGlobal.GetStringTable()->Get(_T("MSG_REACTIVATE_ACCOUNT"));
		gGlobal.AddAlertBox(msg, false);
	}
	else if (result != LOGIN_SUCCESSED && result != LOGIN_SUCCESSED_NEED_KICK)
	{
		String msg;
		if (result == LOGIN_FAILED_WRONG_PASSWORD)
			msg = gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN_ERR_WAIT"));
		else if (result == LOGIN_FAILED_ACC_NOT_EXISTS)
			msg = gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN_NO_ACC"));
		else if (result == LOGIN_FAILED_ANTIHACK_TIMEOUT)
			msg = gGlobal.GetStringTable()->Get(_T("MSE_SYS_ANTIHACK_TIMEOUT"));
		else if (result == LOGIN_ANTIHACKING)
			msg = gGlobal.GetStringTable()->Get(_T("MSE_SYS_INPUT_ERROR"));
		else if (result == LOGIN_FAILED_KICKED)
			msg = gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN_KICKED"));

		gGlobal.AddAlertBox(msg, false, 3000, 3000);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SetLoginState", String(), 0);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "InitLogin", String());
	}
	else
	{
		//gGlobal.CloseAlertBox();
	}
#endif 
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_LoginAccount(LPCPEER pPeer, RPCContext &context, 
											  NetObject* account, const SYSTEMTIME &sysTime)
{
#ifdef QGL
	CAccount* pAccount = CastNetObject<CAccount>(account);
	if (pAccount == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CB_LoginAccount: invalid account object %d"), account);
		return RPC_RESULT_FAIL;
	}

	PROCEDURE_DO_METHOD2(CAccountControl, LoginAccount, gGlobal.g_pAccountControl, context, 
		CAccount*, pAccount, SYSTEMTIME, sysTime)
#endif

	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_VersionCorrect(LPCPEER pPeer, RPCContext &context, const Boolean &equal)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_WorldCheat(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetWorldGameTime(LPCPEER pPeer, RPCContext &context, const SYSTEMTIME &sysTime)
{
#ifdef MAP_SERVICE
	gGlobal.SetServiceTime(sysTime);
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetCharacterRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp, const UInt8 &rankType)
{
#ifdef QGL
	if(rankType != CHAR_RANK_TYPE_PKVALUE)
	{
		gGlobal.g_pMenuRankProcess->OpenMenu();
		gGlobal.g_pMenuRankProcess->ConstructData(charRankGrp, rankType);
		gGlobal.g_pMenuRankProcess->InitMenu();
		gGlobal.g_pMenuRankProcess->RefreshMenu();
	}
	else
	{
		CWantedProcess::GetInstance().OpenPanel(charRankGrp);
	}
#endif
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetGuildRank(LPCPEER pPeer, RPCContext &context, NetGroup* guildRankGrp)
{
#ifdef QGL
	gGlobal.g_pMenuRankProcess->OpenMenu();
	gGlobal.g_pMenuRankProcess->ConstructDataGuild(guildRankGrp);
	gGlobal.g_pMenuRankProcess->InitMenu();
	gGlobal.g_pMenuRankProcess->RefreshMenu();
#endif
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_RankChangeMsg(LPCPEER pPeer, RPCContext &context, const String &rankTitle, const String &char_name, const UInt16 &preRank, const UInt16 &curRank, const SessionIDVector &listeners)
{
#ifdef QGL
	String msg;
	if (preRank == 0 && curRank == 0)
	{
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_RANK_UNCHANGE")), 
		gGlobal.GetStringTable()->Get(rankTitle.c_str()),char_name.c_str());
	}
	else if(!char_name.empty())
	{
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_RANK_MESSAGE")), 
		char_name.c_str(),gGlobal.GetStringTable()->Get(rankTitle.c_str()), preRank, curRank);
	}
	else
	{
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_RANK_NOCHAR")), 
			gGlobal.GetStringTable()->Get(rankTitle.c_str()),char_name.c_str(), preRank, curRank);
	}
	gGlobal.g_pRumorMsgProcess->ShowRumorMessage(msg);
	gGlobal.PrintMessage(msg,CHANNELTYPE_RUMOR);
#endif

	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_StartEvent(LPCPEER pPeer, RPCContext &context,
											NetGroup* events)
{
#ifdef MAP_SERVICE
	TRACE_ENSURE(events);

	CWorldEvent* pEvent;
	for (UInt16 i = 0; i < events->GetCount(); i++)
	{
		pEvent = events->GetChild<CWorldEvent>(i);
		if (pEvent)
		{
			pEvent->Init();
			gGlobal.m_worldEventMap[pEvent->GetEventId()] = pEvent;
			if (pEvent->GetEventType() == CWorldEvent::WE_TYPE_SCRIPT)
			{
				StringA LuaFunc;
				StrToUTF8(pEvent->GetScript().c_str(),LuaFunc);
				if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
					CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),String());
				else
					TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: WorldService_Proxy::CB_StartEvent, not find lua function, %s"), pEvent->GetScript().c_str());
			}
			else
			{
				for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin();
					it != gGlobal.m_mapIndices.end(); ++it)
				{
					pEvent->Execute(it->second);
				}
				// unmark all closed map
				if (pEvent->IsMapEvent())
					pEvent->UnmarkClosedMap();
			}
		}
	}
	DeleteNetGroup(events, BOOLEAN_FALSE);
#endif

	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_StopEvent(LPCPEER pPeer, RPCContext &context, 
										   const UInt32Vector &eventIds, 
										   const Boolean &bForce)
{
#ifdef MAP_SERVICE
	UInt32 eventId;
	CWorldEvent* pEvent;

	for (UInt32Vector::const_iterator it = eventIds.begin();
		it != eventIds.end(); ++it)
	{
		eventId = *it;
		WorldEventMap::iterator it2 = gGlobal.m_worldEventMap.find(*it);
		if (it2 != gGlobal.m_worldEventMap.end()) {
			pEvent = it2->second;
			if (pEvent) {
				for (MapIndexList::iterator it3 = gGlobal.m_mapIndices.begin();
					it3 != gGlobal.m_mapIndices.end(); ++it3)
				{
					pEvent->Terminate(it3->second);
				}
				// mark all closed map
				if (pEvent->IsMapEvent())
					pEvent->MarkClosedMap();
				DeleteNetObject(pEvent);
			}
			gGlobal.m_worldEventMap.erase(it2);
		}
	}
#endif

	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_PeriodEvent(LPCPEER pPeer, RPCContext &context,
											 NetGroup* events, const UInt8 &line_id)
{
#ifdef MAP_SERVICE
	TRACE_ENSURE(events);

	CWorldEvent* pEvent;
	for (UInt16 i = 0; i < events->GetCount(); i++)
	{
		pEvent = events->GetChild<CWorldEvent>(i);
		if (pEvent) {
			for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin();
				it != gGlobal.m_mapIndices.end(); ++it)
			{
				const MapIndex &mi = it->first;
				if (line_id == 0 || mi.line_id == line_id)
					pEvent->Execute(it->second);
			}
		}
	}
	DeleteNetGroup(events, BOOLEAN_FALSE);
#endif

	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_TimeZoneChanged(LPCPEER pPeer, RPCContext &context,
												 const UInt8 &gameTime)
{
#ifdef MAP_SERVICE
	for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin();
		it != gGlobal.m_mapIndices.end(); ++it)
	{
		CMap *pMap = it->second;
		if (pMap)
			pMap->TimeZoneChanged(gameTime);
	}
#endif

	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String & forceOutMsg)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
#ifdef QGL
	gGlobal.g_ForceOutMsg = forceOutMsg;
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_WaitAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &waitTime)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_WaitAccount: waitTime %d"), waitTime);

#ifdef QGL
	if (waitTime > 1000)
		gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN")), false, 0, waitTime);
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_NpcNotExist(LPCPEER pPeer, RPCContext &context, 
											 const NpcLiveList &npcList)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &data)
{
#ifdef QGL
			gGlobal.g_context = context;
			XML_UI::GetInstance().ReadFormFile("Data\\MenuAntiHack.xml", gGlobal.pUIManager);
			LoadLua("Data\\MenuAntiHack.lua");	
			gGlobal.g_pAntiHack->m_firstLogin = BOOLEAN_TRUE;	
			gGlobal.g_pAntiHack->OpenAntiHackPopUp(const_cast<ByteArray&>(image), const_cast<AntiHackData&>(data));
			gGlobal.m_logined = LOGIN_ANTIHACKING;
#endif 
	return RPC_RESULT_OK;	
}

RPCResult WorldService_Proxy::CB_CanChangeLine(LPCPEER pPeer, RPCContext &context, const Boolean &canChange)
{
	return RPC_RESULT_OK;	
}

RPCResult WorldService_Proxy::CB_CheckCanBuyLimitedItem(LPCPEER pPeer, RPCContext &context, const UInt8 &msgCode)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_QueryInstance(LPCPEER pPeer, RPCContext &context, 
											   const Boolean &bExist, const UInt16 &allowCount)
{
	return RPC_RESULT_OK;	
}

RPCResult WorldService_Proxy::CB_GetSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGroup, NetGroup* sellRecord)
{
#ifdef QGL
	CSellYuanBaoGroup* temp = CastNetObject<CSellYuanBaoGroup>(sellYuanBaoGroup);
	if (temp && sellRecord)
	{
		if (gGlobal.g_pSellYuanBaoGroup && gGlobal.g_pSellYuanBaoGroup != temp)
			DeleteNetGroup(gGlobal.g_pSellYuanBaoGroup, BOOLEAN_TRUE);
		gGlobal.g_pSellYuanBaoGroup = temp;
		gGlobal.g_pSellYuanBaoGroup->InitSellRecord(sellRecord);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSellYuanBaoChoice", String());
	}
	DeleteNetGroup(sellRecord);
#endif
	return RPC_RESULT_OK;
}
RPCResult WorldService_Proxy::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
#ifdef QGL

#endif
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Boolean &fromWorld)
{
#ifdef QGL

#endif
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex,  const UInt32 &totalYBT, const UInt32 &totalMoney)
{
#ifdef QGL

#endif
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_ApolloMsg(LPCPEER pPeer, RPCContext &context, const ByteArray &msg)
{
#ifdef CLIENT
	SendDataToApolloCT((unsigned char *)(msg.GetData()), (UInt32) msg.GetSize());
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ApolloMsg SendDataToApolloCT size %d"), msg.GetSize());
#endif

	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_WantedDetail(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY)
{
#ifdef QGL
	if(mapId!=0)
	{
		const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(mapId);
		if(pCliMapInfo)
		{
			String temp;
			if(lineId != 0)
				temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL")), lineId,pCliMapInfo->m_mapname,mapX,mapY);
			else
				temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL_INSTANCE")), pCliMapInfo->m_mapname,mapX,mapY);
			gGlobal.PrintPlayerMessage(temp.c_str());
			gGlobal.PrintMessage(temp,CHANNELTYPE_SYSTEM);
			return RPC_RESULT_OK;
		}
	}
	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL_NOT_ONLINE")));
	gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL_NOT_ONLINE")),CHANNELTYPE_SYSTEM);

#endif
	return RPC_RESULT_OK;
}
RPCResult WorldService_Proxy::CB_HasGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{	
	return RPC_RESULT_OK;
}
RPCResult WorldService_Proxy::CB_GetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &flag)
{	
	return RPC_RESULT_OK;
}
RPCResult WorldService_Proxy::CB_SetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context)
{	
	return RPC_RESULT_OK;
}
RPCResult WorldService_Proxy::CB_RemoveGlobalNpcFlag(LPCPEER pPeer, RPCContext &context)
{	
	return RPC_RESULT_OK;
}
#ifdef CLIENT
int WorldService_Proxy::SendApolloMsg(int msgId, UInt32 arg1, UInt32 arg2)
{
	ByteArray msg;
	int r;

	switch (msgId)
	{
	case CALLBACK_MSG_WAIT_SEND_DATA:
		{
			unsigned long	msgSize;
			r = GetDataFromApolloCT(NULL, &msgSize);
			if (msgSize == 0)
				return CALLBACK_ERROR_UNKNOWN_MSG;

			msg.SetSize(msgSize);
			r = GetDataFromApolloCT((unsigned char *)msg.GetData(), &msgSize);

			TRACE_INFODTL(GLOBAL_LOGGER, _F("Apollo: CALLBACK_MSG_WAIT_SEND_DATA"));
		}
		break;

	case CALLBACK_MSG_INSTANT_MESSAGE:
		{
			msg.SetSize(arg2);
			memcpy(msg.GetData(), (Byte*) arg1, (size_t) msg.GetSize());
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Apollo: CALLBACK_MSG_INSTANT_MESSAGE"));
		}
		break;

	default:
		return CALLBACK_ERROR_UNKNOWN_MSG;
	}
	
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("WorldService_Proxy::ApolloMsg msgId %d, msg size %d"),
		msgId, msg.GetSize());
	WorldService_Proxy::ApolloMsg(gGlobal.pPeerProxy, gGlobal.g_context, msg);
		
	return CALLBACK_OK;
}
#endif	// CLIENT