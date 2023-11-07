//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/WorldService_stub.h"

#pragma optimize("", off)
RPCResult WorldService_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_WorldService_RegisterProxy:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = RegisterProxy(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_UnregisterProxy:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = UnregisterProxy(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_RegisterMap:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			Boolean bOpen;
			pBuf->Read<Boolean>(bOpen);
			_result = RegisterMap(pPeer, context, serviceId, map_id, line_id, owner_id, bOpen);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_UnregisterMap:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = UnregisterMap(pPeer, context, serviceId, map_id, line_id, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_RegisterInstance:
		{
			UInt16 instId;
			pBuf->Read<UInt16>(instId);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = RegisterInstance(pPeer, context, instId, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_UnregisterInstance:
		{
			UInt16 instId;
			pBuf->Read<UInt16>(instId);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = UnregisterInstance(pPeer, context, instId, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_ValidateMap:
		{
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = ValidateMap(pPeer, context, map_id, line_id, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_QueryInstance:
		{
			UInt16 instId;
			pBuf->Read<UInt16>(instId);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = QueryInstance(pPeer, context, instId, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CanChangeLine:
		{
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			_result = CanChangeLine(pPeer, context, line_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_RemoteCreateNpc:
		{
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			UInt16 x;
			pBuf->Read<UInt16>(x);
			UInt16 y;
			pBuf->Read<UInt16>(y);
			UInt32 lifeTime;
			pBuf->Read<UInt32>(lifeTime);
			UInt16 eventId;
			pBuf->Read<UInt16>(eventId);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RemoteCreateNpc(pPeer, context, map_id, line_id, owner_id, npc_id, x, y, lifeTime, eventId, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_ReleaseSession:
		{
			SessionID sid;
			pBuf->Read<SessionID>(sid);
			Boolean bDelete;
			pBuf->Read<Boolean>(bDelete);
			_result = ReleaseSession(pPeer, context, sid, bDelete);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CharacterLeave:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CharacterLeave(pPeer, context, acct_id, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CharJumpMap:
		{
			EnterInfo info;
			pBuf->Read<EnterInfo>(info);
			UInt32Vector memberIdList;
			pBuf->Read<UInt32Vector>(memberIdList);
			AntiAddictInfo addictInfo;
			pBuf->Read<AntiAddictInfo>(addictInfo);
			String buffCache;
			pBuf->Read<String>(buffCache);
			UInt32 uiLineupId;
			pBuf->Read<UInt32>(uiLineupId);
			int iKeyPos;
			pBuf->Read<int>(iKeyPos);
			_result = CharJumpMap(pPeer, context, info, memberIdList, addictInfo, buffCache, uiLineupId, iKeyPos);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_NpcJumpMap:
		{
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt16 x;
			pBuf->Read<UInt16>(x);
			UInt16 y;
			pBuf->Read<UInt16>(y);
			UInt32 followId;
			pBuf->Read<UInt32>(followId);
			_result = NpcJumpMap(pPeer, context, npc_id, map_id, line_id, x, y, followId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_CharUpdateLocation:
		{
			EnterInfo info;
			pBuf->Read<EnterInfo>(info);
			_result = CharUpdateLocation(pPeer, context, info);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_GetWorldGameTime:
		{
			_result = GetWorldGameTime(pPeer, context);
		}
		break;
#ifndef CLIENT
	case RPC_WorldService_NotifyNpcCreate:
		{
			NpcLiveInfo npc;
			pBuf->Read<NpcLiveInfo>(npc);
			_result = NotifyNpcCreate(pPeer, context, npc);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_NotifyNpcDead:
		{
			NpcLiveInfo npc;
			pBuf->Read<NpcLiveInfo>(npc);
			_result = NotifyNpcDead(pPeer, context, npc);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_QueryNpcExist:
		{
			NpcLiveList npcList;
			pBuf->Read<NpcLiveList>(npcList);
			_result = QueryNpcExist(pPeer, context, npcList);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_ForceLogout:
		{
			UInt32 acct_id;
			pBuf->Read<UInt32>(acct_id);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String msg;
			pBuf->Read<String>(msg);
			_result = ForceLogout(pPeer, context, acct_id, char_id, msg);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_StartEvent:
		{
			UInt32 eventId;
			pBuf->Read<UInt32>(eventId);
			_result = StartEvent(pPeer, context, eventId);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_LoginAccount:
		{
			UInt32 ip;
			pBuf->Read<UInt32>(ip);
			String loginName;
			pBuf->Read<String>(loginName);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = LoginAccount(pPeer, context, ip, loginName, password);
		}
		break;
	case RPC_WorldService_KickAccount:
		{
			Boolean bKick;
			pBuf->Read<Boolean>(bKick);
			_result = KickAccount(pPeer, context, bKick);
		}
		break;
	case RPC_WorldService_GetMapLines:
		{
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			_result = GetMapLines(pPeer, context, map_id);
		}
		break;
	case RPC_WorldService_VersionChecking:
		{
			UInt32 versionNo;
			pBuf->Read<UInt32>(versionNo);
			_result = VersionChecking(pPeer, context, versionNo);
		}
		break;
#ifndef CLIENT
	case RPC_WorldService_CheckCanBuyLimitedItem:
		{
			UInt32 chargeShopID;
			pBuf->Read<UInt32>(chargeShopID);
			UInt16 stack;
			pBuf->Read<UInt16>(stack);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CheckCanBuyLimitedItem(pPeer, context, chargeShopID, stack, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_ReduceLimitItem:
		{
			UInt32 chargeShopID;
			pBuf->Read<UInt32>(chargeShopID);
			Boolean buySuccess;
			pBuf->Read<Boolean>(buySuccess);
			UInt16 reduceStack;
			pBuf->Read<UInt16>(reduceStack);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = ReduceLimitItem(pPeer, context, chargeShopID, buySuccess, reduceStack, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_SendCheatCommand:
		{
			NetGroup* cheatCommandGroup;
			pBuf->Read<NetGroup*>(cheatCommandGroup);
			_result = SendCheatCommand(pPeer, context, cheatCommandGroup);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_AnswerAntiHack:
		{
			UInt8 answer;
			pBuf->Read<UInt8>(answer);
			_result = AnswerAntiHack(pPeer, context, answer);
		}
		break;
#ifndef CLIENT
	case RPC_WorldService_WorldCheat:
		{
			UInt32 exe_id;
			pBuf->Read<UInt32>(exe_id);
			UInt32 srcAcct_id;
			pBuf->Read<UInt32>(srcAcct_id);
			UInt32 target_acct_id;
			pBuf->Read<UInt32>(target_acct_id);
			UInt32 target_char_id;
			pBuf->Read<UInt32>(target_char_id);
			String command;
			pBuf->Read<String>(command);
			_result = WorldCheat(pPeer, context, exe_id, srcAcct_id, target_acct_id, target_char_id, command);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_GetCharacterRank:
		{
			UInt8 rankType;
			pBuf->Read<UInt8>(rankType);
			_result = GetCharacterRank(pPeer, context, rankType);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_GetGuildRank:
		{
			_result = GetGuildRank(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_StoreApolloLog:
		{
			UInt32 msgTime;
			pBuf->Read<UInt32>(msgTime);
			UInt16 msgType;
			pBuf->Read<UInt16>(msgType);
			String TypeName;
			pBuf->Read<String>(TypeName);
			String msg;
			pBuf->Read<String>(msg);
			_result = StoreApolloLog(pPeer, context, msgTime, msgType, TypeName, msg);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_ApolloMsg:
		{
			ByteArray msg;
			pBuf->Read<ByteArray>(msg);
			_result = ApolloMsg(pPeer, context, msg);
		}
		break;
	case RPC_WorldService_GetSellYuanBao:
		{
			_result = GetSellYuanBao(pPeer, context);
		}
		break;
#ifndef CLIENT
	case RPC_WorldService_AddSellYuanBao:
		{
			UInt32 YBT;
			pBuf->Read<UInt32>(YBT);
			UInt32 pricePerUnit;
			pBuf->Read<UInt32>(pricePerUnit);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = AddSellYuanBao(pPeer, context, YBT, pricePerUnit, password);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_BuySellYuanBao:
		{
			UInt32 sellYuanBaoID;
			pBuf->Read<UInt32>(sellYuanBaoID);
			UInt32 YBT;
			pBuf->Read<UInt32>(YBT);
			UInt32 charTotalMoney;
			pBuf->Read<UInt32>(charTotalMoney);
			UInt32 moneyPerUnit;
			pBuf->Read<UInt32>(moneyPerUnit);
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = BuySellYuanBao(pPeer, context, sellYuanBaoID, YBT, charTotalMoney, moneyPerUnit, password);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_GetTempBank:
		{
			ByteArray password;
			pBuf->Read<ByteArray>(password);
			_result = GetTempBank(pPeer, context, password);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_WorldService_ClientDisconnect:
		{
			_result = ClientDisconnect(pPeer, context);
		}
		break;
#endif // CLIENT
	case RPC_WorldService_UpdateWantedReward:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			Int32 money;
			pBuf->Read<Int32>(money);
			_result = UpdateWantedReward(pPeer, context, targetId, money);
		}
		break;
	case RPC_WorldService_WantedDetail:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = WantedDetail(pPeer, context, targetId);
		}
		break;
	case RPC_WorldService_RemoveFromWantedRank:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = RemoveFromWantedRank(pPeer, context, targetId);
		}
		break;
	case RPC_WorldService_DeleteGenerationTrans:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = DeleteGenerationTrans(pPeer, context, uiTargetId, uiRelatedId);
		}
		break;
	case RPC_WorldService_RecvGenerationAccmulationTrans:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			UInt32 uiAcc;
			pBuf->Read<UInt32>(uiAcc);
			_result = RecvGenerationAccmulationTrans(pPeer, context, uiTargetId, uiRelatedId, uiAcc);
		}
		break;
	case RPC_WorldService_GenerationOnlineNotifyTrans:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = GenerationOnlineNotifyTrans(pPeer, context, uiTargetId, uiRelatedId);
		}
		break;
	case RPC_WorldService_GenerationOfflineNotifyTrans:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = GenerationOfflineNotifyTrans(pPeer, context, uiTargetId, uiRelatedId);
		}
		break;
	case RPC_WorldService_HasGlobalNpcFlag:
		{
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 flag_id;
			pBuf->Read<UInt16>(flag_id);
			_result = HasGlobalNpcFlag(pPeer, context, npc_id, owner_id, flag_id);
		}
		break;
	case RPC_WorldService_GetGlobalNpcFlag:
		{
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 flag_id;
			pBuf->Read<UInt16>(flag_id);
			_result = GetGlobalNpcFlag(pPeer, context, npc_id, owner_id, flag_id);
		}
		break;
	case RPC_WorldService_SetGlobalNpcFlag:
		{
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 flag_id;
			pBuf->Read<UInt16>(flag_id);
			UInt16 flag;
			pBuf->Read<UInt16>(flag);
			_result = SetGlobalNpcFlag(pPeer, context, npc_id, owner_id, flag_id, flag);
		}
		break;
	case RPC_WorldService_RemoveGlobalNpcFlag:
		{
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 flag_id;
			pBuf->Read<UInt16>(flag_id);
			_result = RemoveGlobalNpcFlag(pPeer, context, npc_id, owner_id, flag_id);
		}
		break;
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
#pragma optimize("", on)

RPCResult WorldService_Stub::CB_QueryInstance(LPCPEER pPeer, RPCContext &context, const Boolean &bExist, const UInt16 &allowCount)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_QueryInstance);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bExist);
	pBuf->Write<UInt16>(allowCount);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_JumpSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_JumpSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_ValidMap(LPCPEER pPeer, RPCContext &context, const Boolean &bOpen)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_ValidMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bOpen);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_UpdateLocation(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_UpdateLocation);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_CanChangeLine(LPCPEER pPeer, RPCContext &context, const Boolean &canChange)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_CanChangeLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(canChange);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_GetWorldGameTime(LPCPEER pPeer, RPCContext &context, const SYSTEMTIME &sysTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_GetWorldGameTime);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<SYSTEMTIME>(sysTime);
	return RPCSend<962679827>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_NpcNotExist(LPCPEER pPeer, RPCContext &context, const NpcLiveList &npcList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_NpcNotExist);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NpcLiveList>(npcList);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_StartEvent(LPCPEER pPeer, RPCContext &context, NetGroup* events)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_StartEvent);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(events);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_StopEvent(LPCPEER pPeer, RPCContext &context, const UInt32Vector &eventIds, const Boolean &bForce)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_StopEvent);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32Vector>(eventIds);
	pBuf->Write<Boolean>(bForce);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_PeriodEvent(LPCPEER pPeer, RPCContext &context, NetGroup* events, const UInt8 &line_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_PeriodEvent);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(events);
	pBuf->Write<UInt8>(line_id);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_TimeZoneChanged(LPCPEER pPeer, RPCContext &context, const UInt8 &gameTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_TimeZoneChanged);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(gameTime);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_GetMapLines(LPCPEER pPeer, RPCContext &context, const Int32Vector &lineCounts, const UInt16Vector &lineState)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_GetMapLines);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Int32Vector>(lineCounts);
	pBuf->Write<UInt16Vector>(lineState);
	return RPCSend<969285436>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_LoginResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_LoginResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(result);
	return RPCSend<277704283>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_WaitAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &waitTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_WaitAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(waitTime);
	return RPCSend<550449771>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_LoginAccount(LPCPEER pPeer, RPCContext &context, NetObject* account, const SYSTEMTIME &sysTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_LoginAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(account);
	pBuf->Write<SYSTEMTIME>(sysTime);
	return RPCSend<1343100704>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_VersionCorrect(LPCPEER pPeer, RPCContext &context, const Boolean &equal)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_VersionCorrect);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(equal);
	return RPCSend<318650066>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String &forceOutMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_ForceLogout);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(forceOutMsg);
	return RPCSend<427715877>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_CheckCanBuyLimitedItem(LPCPEER pPeer, RPCContext &context, const UInt8 &msgCode)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_CheckCanBuyLimitedItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(msgCode);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &data)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_AntiHack);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ByteArray>(image);
	pBuf->Write<AntiHackData>(data);
	return RPCSend<1517552885>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(err);
	pBuf->Write<String>(errMsg);
	return RPCSend<720059219>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_WorldCheat(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_WorldCheat);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1523673697>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_GetCharacterRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp, const UInt8 &rankType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_GetCharacterRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(charRankGrp);
	pBuf->Write<UInt8>(rankType);
	return RPCSend<1840188879>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_RankChangeMsg(LPCPEER pPeer, RPCContext &context, const String &rankTitle, const String &char_name, const UInt16 &preRank, const UInt16 &curRank, const SessionIDVector &listeners)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_RankChangeMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(rankTitle);
	pBuf->Write<String>(char_name);
	pBuf->Write<UInt16>(preRank);
	pBuf->Write<UInt16>(curRank);
	pBuf->Write<SessionIDVector>(listeners);
	return RPCSend<817654996>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_GetGuildRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_GetGuildRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(charRankGrp);
	return RPCSend<1603016263>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_ApolloMsg(LPCPEER pPeer, RPCContext &context, const ByteArray &msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_ApolloMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ByteArray>(msg);
	return RPCSend<2080578402>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_GetSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGroup, NetGroup* sellRecord)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_GetSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(sellYuanBaoGroup);
	pBuf->Write<NetGroup*>(sellRecord);
	return RPCSend<1616532101>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_AddSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Boolean &fromWorld)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_BuySellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(msgIndex);
	pBuf->Write<Boolean>(fromWorld);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &totalYBT, const UInt32 &totalMoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_GetTempBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(msgIndex);
	pBuf->Write<UInt32>(totalYBT);
	pBuf->Write<UInt32>(totalMoney);
	return RPCSend(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_UpdateWantedReward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(result);
	return RPCSend<695082008>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_WantedDetail(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_WantedDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(name);
	pBuf->Write<UInt32>(lineId);
	pBuf->Write<UInt32>(mapId);
	pBuf->Write<UInt32>(mapX);
	pBuf->Write<UInt32>(mapY);
	return RPCSend<1708657089>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_HasGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_HasGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(result);
	return RPCSend<242774850>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_GetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &flag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_GetGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(flag);
	return RPCSend<676280625>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_SetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_SetGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1994195922>(pPeer, pBuf);
}

RPCResult WorldService_Stub::CB_RemoveGlobalNpcFlag(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CB_RemoveGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1535713103>(pPeer, pBuf);
}

