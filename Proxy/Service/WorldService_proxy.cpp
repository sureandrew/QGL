//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/WorldService_proxy.h"

#pragma optimize("", off)
RPCResult WorldService_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_WorldService_CB_QueryInstance:
		{
			Boolean bExist;
			pBuf->Read<Boolean>(bExist);
			UInt16 allowCount;
			pBuf->Read<UInt16>(allowCount);
			_result = CB_QueryInstance(pPeer, context, bExist, allowCount);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_JumpSuccess:
		{
			_result = CB_JumpSuccess(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_ValidMap:
		{
			Boolean bOpen;
			pBuf->Read<Boolean>(bOpen);
			_result = CB_ValidMap(pPeer, context, bOpen);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_UpdateLocation:
		{
			_result = CB_UpdateLocation(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_CanChangeLine:
		{
			Boolean canChange;
			pBuf->Read<Boolean>(canChange);
			_result = CB_CanChangeLine(pPeer, context, canChange);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_CB_GetWorldGameTime:
		{
			SYSTEMTIME sysTime;
			pBuf->Read<SYSTEMTIME>(sysTime);
			_result = CB_GetWorldGameTime(pPeer, context, sysTime);
		}
		break;
#ifndef CLIENT
	case RPC_WorldService_CB_NpcNotExist:
		{
			NpcLiveList npcList;
			pBuf->Read<NpcLiveList>(npcList);
			_result = CB_NpcNotExist(pPeer, context, npcList);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_StartEvent:
		{
			NetGroup* events;
			pBuf->Read<NetGroup*>(events);
			_result = CB_StartEvent(pPeer, context, events);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_StopEvent:
		{
			UInt32Vector eventIds;
			pBuf->Read<UInt32Vector>(eventIds);
			Boolean bForce;
			pBuf->Read<Boolean>(bForce);
			_result = CB_StopEvent(pPeer, context, eventIds, bForce);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_PeriodEvent:
		{
			NetGroup* events;
			pBuf->Read<NetGroup*>(events);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			_result = CB_PeriodEvent(pPeer, context, events, line_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_TimeZoneChanged:
		{
			UInt8 gameTime;
			pBuf->Read<UInt8>(gameTime);
			_result = CB_TimeZoneChanged(pPeer, context, gameTime);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_CB_GetMapLines:
		{
			Int32Vector lineCounts;
			pBuf->Read<Int32Vector>(lineCounts);
			UInt16Vector lineState;
			pBuf->Read<UInt16Vector>(lineState);
			_result = CB_GetMapLines(pPeer, context, lineCounts, lineState);
		}
		break;
	case RPC_WorldService_CB_LoginResult:
		{
			UInt8 result;
			pBuf->Read<UInt8>(result);
			_result = CB_LoginResult(pPeer, context, result);
		}
		break;
	case RPC_WorldService_CB_WaitAccount:
		{
			UInt32 waitTime;
			pBuf->Read<UInt32>(waitTime);
			_result = CB_WaitAccount(pPeer, context, waitTime);
		}
		break;
	case RPC_WorldService_CB_LoginAccount:
		{
			NetObject* account;
			pBuf->Read<NetObject*>(account);
			SYSTEMTIME sysTime;
			pBuf->Read<SYSTEMTIME>(sysTime);
			_result = CB_LoginAccount(pPeer, context, account, sysTime);
		}
		break;
	case RPC_WorldService_CB_VersionCorrect:
		{
			Boolean equal;
			pBuf->Read<Boolean>(equal);
			_result = CB_VersionCorrect(pPeer, context, equal);
		}
		break;
	case RPC_WorldService_CB_ForceLogout:
		{
			String forceOutMsg;
			pBuf->Read<String>(forceOutMsg);
			_result = CB_ForceLogout(pPeer, context, forceOutMsg);
		}
		break;
#ifndef CLIENT
	case RPC_WorldService_CB_CheckCanBuyLimitedItem:
		{
			UInt8 msgCode;
			pBuf->Read<UInt8>(msgCode);
			_result = CB_CheckCanBuyLimitedItem(pPeer, context, msgCode);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_CB_AntiHack:
		{
			ByteArray image;
			pBuf->Read<ByteArray>(image);
			AntiHackData data;
			pBuf->Read<AntiHackData>(data);
			_result = CB_AntiHack(pPeer, context, image, data);
		}
		break;
	case RPC_WorldService_CB_Error:
		{
			UInt32 err;
			pBuf->Read<UInt32>(err);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, err, errMsg);
		}
		break;
	case RPC_WorldService_CB_WorldCheat:
		{
			_result = CB_WorldCheat(pPeer, context);
		}
		break;
	case RPC_WorldService_CB_GetCharacterRank:
		{
			NetGroup* charRankGrp;
			pBuf->Read<NetGroup*>(charRankGrp);
			UInt8 rankType;
			pBuf->Read<UInt8>(rankType);
			_result = CB_GetCharacterRank(pPeer, context, charRankGrp, rankType);
		}
		break;
	case RPC_WorldService_CB_RankChangeMsg:
		{
			String rankTitle;
			pBuf->Read<String>(rankTitle);
			String char_name;
			pBuf->Read<String>(char_name);
			UInt16 preRank;
			pBuf->Read<UInt16>(preRank);
			UInt16 curRank;
			pBuf->Read<UInt16>(curRank);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);
			_result = CB_RankChangeMsg(pPeer, context, rankTitle, char_name, preRank, curRank, listeners);
		}
		break;
	case RPC_WorldService_CB_GetGuildRank:
		{
			NetGroup* charRankGrp;
			pBuf->Read<NetGroup*>(charRankGrp);
			_result = CB_GetGuildRank(pPeer, context, charRankGrp);
		}
		break;
	case RPC_WorldService_CB_ApolloMsg:
		{
			ByteArray msg;
			pBuf->Read<ByteArray>(msg);
			_result = CB_ApolloMsg(pPeer, context, msg);
		}
		break;
	case RPC_WorldService_CB_GetSellYuanBao:
		{
			NetGroup* sellYuanBaoGroup;
			pBuf->Read<NetGroup*>(sellYuanBaoGroup);
			NetGroup* sellRecord;
			pBuf->Read<NetGroup*>(sellRecord);
			_result = CB_GetSellYuanBao(pPeer, context, sellYuanBaoGroup, sellRecord);
		}
		break;
#ifndef CLIENT
	case RPC_WorldService_CB_AddSellYuanBao:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_AddSellYuanBao(pPeer, context, msgIndex);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_BuySellYuanBao:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			Boolean fromWorld;
			pBuf->Read<Boolean>(fromWorld);
			_result = CB_BuySellYuanBao(pPeer, context, msgIndex, fromWorld);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CB_GetTempBank:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			UInt32 totalYBT;
			pBuf->Read<UInt32>(totalYBT);
			UInt32 totalMoney;
			pBuf->Read<UInt32>(totalMoney);
			_result = CB_GetTempBank(pPeer, context, msgIndex, totalYBT, totalMoney);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_CB_UpdateWantedReward:
		{
			Boolean result;
			pBuf->Read<Boolean>(result);
			_result = CB_UpdateWantedReward(pPeer, context, result);
		}
		break;
	case RPC_WorldService_CB_WantedDetail:
		{
			String name;
			pBuf->Read<String>(name);
			UInt32 lineId;
			pBuf->Read<UInt32>(lineId);
			UInt32 mapId;
			pBuf->Read<UInt32>(mapId);
			UInt32 mapX;
			pBuf->Read<UInt32>(mapX);
			UInt32 mapY;
			pBuf->Read<UInt32>(mapY);
			_result = CB_WantedDetail(pPeer, context, name, lineId, mapId, mapX, mapY);
		}
		break;
	case RPC_WorldService_CB_HasGlobalNpcFlag:
		{
			Boolean result;
			pBuf->Read<Boolean>(result);
			_result = CB_HasGlobalNpcFlag(pPeer, context, result);
		}
		break;
	case RPC_WorldService_CB_GetGlobalNpcFlag:
		{
			UInt32 flag;
			pBuf->Read<UInt32>(flag);
			_result = CB_GetGlobalNpcFlag(pPeer, context, flag);
		}
		break;
	case RPC_WorldService_CB_SetGlobalNpcFlag:
		{
			_result = CB_SetGlobalNpcFlag(pPeer, context);
		}
		break;
	case RPC_WorldService_CB_RemoveGlobalNpcFlag:
		{
			_result = CB_RemoveGlobalNpcFlag(pPeer, context);
		}
		break;
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}

RPCResult WorldService_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_WorldService_CB_GetWorldGameTime:
			_result = RPCDecode<962679827>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_GetMapLines:
			_result = RPCDecode<969285436>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_LoginResult:
			_result = RPCDecode<277704283>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_WaitAccount:
			_result = RPCDecode<550449771>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_LoginAccount:
			_result = RPCDecode<1343100704>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_VersionCorrect:
			_result = RPCDecode<318650066>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_ForceLogout:
			_result = RPCDecode<427715877>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_AntiHack:
			_result = RPCDecode<1517552885>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_Error:
			_result = RPCDecode<720059219>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_WorldCheat:
			_result = RPCDecode<1523673697>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_GetCharacterRank:
			_result = RPCDecode<1840188879>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_RankChangeMsg:
			_result = RPCDecode<817654996>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_GetGuildRank:
			_result = RPCDecode<1603016263>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_ApolloMsg:
			_result = RPCDecode<2080578402>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_GetSellYuanBao:
			_result = RPCDecode<1616532101>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_UpdateWantedReward:
			_result = RPCDecode<695082008>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_WantedDetail:
			_result = RPCDecode<1708657089>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_HasGlobalNpcFlag:
			_result = RPCDecode<242774850>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_GetGlobalNpcFlag:
			_result = RPCDecode<676280625>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_SetGlobalNpcFlag:
			_result = RPCDecode<1994195922>(pPeer, pBuf);
		break;
	case RPC_WorldService_CB_RemoveGlobalNpcFlag:
			_result = RPCDecode<1535713103>(pPeer, pBuf);
		break;
	}
	return _result;
}
#pragma optimize("", on)

