//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/common_binary.h"
//-- Self
#include "Proxy/Service/WorldService.h"
//-- Library
#include "SimClient/Map.h"
#include "SimClient/Global.h"
#include "SimClient/SimClient.h"
#include "SimClient/Account.h"
#include "SimClient/Character.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"
#include "ApolloClient.h"
#include "SimClient/SellYuanBaoGroup.h"

// private function and namespace global variable here
namespace WorldService_Proxy
{
};

#define LINE_MAX_DIFF			10

RPCResult WorldService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetMapLines(LPCPEER pPeer, RPCContext &context, 
											 const Int32Vector &lineCounts, const UInt16Vector &lineState)
{
	String strLines;
	UInt8 minLine = 255;
	Int32 minCount = INT32_MAX_VALUE;
	Int32 curCount = 0;
	CMap* pMap = NULL;

	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_pMap && gGlobal.m_pSimClient->m_pCharacter)
		pMap = gGlobal.m_pSimClient->m_pMap;

	for (int i = 1; i < (int) lineCounts.size(); i++)
	{
		if (lineCounts[i] != -1) {
			if (lineCounts[i] < minCount) {
				minLine = i;
				minCount = lineCounts[i];
			}
			if (pMap && pMap->GetLine_id() == i)
				curCount = lineCounts[i];
			strLines.AppendFormat(_T("L%d=%d, "), i, lineCounts[i]);
		}
	}

	TRACE_ALWAYSDTL_1(GLOBAL_LOGGER, _F("Map Lines: %s"), strLines.c_str());

	/*
	// change to min line count is too different
	if (minLine !=  255 && pMap && pMap->GetLine_id() != minLine && 
		curCount > LINE_MAX_DIFF + minCount)
	{
		String cmd;
		cmd.Format(_T("mapline %d"), minLine);
		gGlobal.m_pSimClient->WaitExecute(cmd.c_str());
	}
	*/

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
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_login == LS_LOGINING) {
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("result %d"), result);
		if (result == LOGIN_SUCCESSED || result == LOGIN_SUCCESSED_NEED_KICK)
			gGlobal.m_pSimClient->m_login = LS_LOGINED;
		else
			gGlobal.m_pSimClient->m_login = LS_LOGOUT;	

	}


	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_LoginAccount(LPCPEER pPeer, RPCContext &context, 
											  NetObject* account, const SYSTEMTIME &sysTime)
{
	CAccount *pAccount = CastNetObject<CAccount>(account);
	TRACE_ENSURE(pAccount);
	gGlobal.m_pSimClient->m_pAccount = pAccount;
	gGlobal.m_pSimClient->m_accountName = pAccount->GetLoginName();

	PROCEDURE_DO_METHOD1(CAccount, LoginAccount, pAccount, context, SYSTEMTIME, sysTime)

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
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetCharacterRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp, const UInt8 &rankType)
{

	DeleteNetGroup(charRankGrp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}



RPCResult WorldService_Proxy::CB_RankChangeMsg(LPCPEER pPeer, RPCContext &context, const String &rankTitle, const String &char_name, const UInt16 &preRank, const UInt16 &curRank, const SessionIDVector &listeners)
{
	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_StartEvent(LPCPEER pPeer, RPCContext &context, NetGroup* events)
{
	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_StopEvent(LPCPEER pPeer, RPCContext &context, const UInt32Vector &eventIds, const Boolean &bForce)
{
	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_PeriodEvent(LPCPEER pPeer, RPCContext &context, NetGroup* events, const UInt8 &line_id)
{
	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_TimeZoneChanged(LPCPEER pPeer, RPCContext &context, const UInt8 &gameTime)
{
	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_WaitAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &waitTime)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_WaitAccount: waitTime %d"), waitTime);

	return RPC_RESULT_OK; 
}

RPCResult WorldService_Proxy::CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String & forceOutMsg)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("%s"), forceOutMsg.c_str());

	if (gGlobal.m_pSimClient) {
		gGlobal.m_pSimClient->DisconnectServer();
	}
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray & image, const AntiHackData &data)
{
	AnswerAntiHack(pPeer, context,  data.answer);	
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_CanChangeLine(LPCPEER pPeer, RPCContext &context, const Boolean &canChange)
{
	return RPC_RESULT_OK;	
}

RPCResult WorldService_Proxy::CB_GetGuildRank(LPCPEER pPeer, RPCContext &context, NetGroup* guildRankGrp)
{
	DeleteNetGroup(guildRankGrp, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Boolean &fromWorld)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_ApolloMsg(LPCPEER pPeer, RPCContext &context, const ByteArray &msg)
{
	SendDataToApolloCT((unsigned char *)(msg.GetData()), (UInt32) msg.GetSize());
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("CB_ApolloMsg SendDataToApolloCT size %d"), msg.GetSize());

	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGroup, NetGroup* sellRecord)
{
	CSellYuanBaoGroup* temp = CastNetObject<CSellYuanBaoGroup>(sellYuanBaoGroup);
	if (temp && sellRecord)
	{
		if (gGlobal.m_pSimClient->m_pSellYuanBaoGroup && gGlobal.m_pSimClient->m_pSellYuanBaoGroup != temp)
			DeleteNetGroup(gGlobal.m_pSimClient->m_pSellYuanBaoGroup, BOOLEAN_TRUE);
		gGlobal.m_pSimClient->m_pSellYuanBaoGroup = temp;
		gGlobal.m_pSimClient->m_pSellYuanBaoGroup->InitSellRecord(sellRecord);		
	}
	DeleteNetGroup(sellRecord);
	
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &totalYBT, const UInt32 &totalMoney)
{
	return RPC_RESULT_OK;
}

int WorldService_Proxy::SendApolloMsg(int msgId, UInt32 arg1, UInt32 arg2)
{
	if (gGlobal.m_pSimClient == NULL)
		return CALLBACK_OK;

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

			TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("Apollo: CALLBACK_MSG_WAIT_SEND_DATA"));
		}
		break;

	case CALLBACK_MSG_INSTANT_MESSAGE:
		{
			msg.SetSize(arg2);
			memcpy(msg.GetData(), (Byte*) arg1, (size_t) msg.GetSize());
		}
		break;

	default:
		return CALLBACK_ERROR_UNKNOWN_MSG;
	}
	
	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("WorldService_Proxy::ApolloMsg msgId %d, msg size %d"),
		msgId, msg.GetSize());
	WorldService_Proxy::ApolloMsg(gGlobal.m_pSimClient->m_pProxyPeer, 
		gGlobal.m_pSimClient->m_context, msg);
		
	return CALLBACK_OK;
}

RPCResult WorldService_Proxy::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	return RPC_RESULT_OK;
}

RPCResult WorldService_Proxy::CB_WantedDetail(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY)
{
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