///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_WORLDSERVICE_PROXY_H_
#define _RPC_WORLDSERVICE_PROXY_H_

#include "Common/RPCFunc.h"
#include "Common/common_binary.h"
struct NpcLiveInfo
{
	UInt32 npcId;
	UInt16 mapId;
	UInt8  lineId;
	UInt32 ownerId;
	UInt16 x;
	UInt16 y;
	UInt16 eventId;

	NpcLiveInfo();
	bool operator == (const NpcLiveInfo& b) const;
};
typedef StlVector<NpcLiveInfo, ManagedAllocator<NpcLiveInfo> > NpcLiveList;
#include "RPCEnum/WorldService_enum.h"

namespace WorldService_Proxy {

RPCResult RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId);
RPCResult RegisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id, const Boolean &bOpen);
RPCResult UnregisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id);
RPCResult RegisterInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id);
RPCResult UnregisterInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id);
RPCResult ValidateMap(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id);
RPCResult QueryInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id);
RPCResult CB_QueryInstance(LPCPEER pPeer, RPCContext &context, const Boolean &bExist, const UInt16 &allowCount);
RPCResult CanChangeLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id);
RPCResult RemoteCreateNpc(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id, const UInt32 &npc_id, const UInt16 &x, const UInt16 &y, const UInt32 &lifeTime, const UInt16 &eventId, const UInt32 &char_id);
RPCResult ReleaseSession(LPCPEER pPeer, RPCContext &context, const SessionID &sid, const Boolean &bDelete);
RPCResult CharacterLeave(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id);
RPCResult CharJumpMap(LPCPEER pPeer, RPCContext &context, const EnterInfo &info, const UInt32Vector &memberIdList, const AntiAddictInfo &addictInfo, const String &buffCache, const UInt32 &uiLineupId, const int &iKeyPos);
RPCResult NpcJumpMap(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt16 &map_id, const UInt8 &line_id, const UInt16 &x, const UInt16 &y, const UInt32 &followId);
RPCResult CharUpdateLocation(LPCPEER pPeer, RPCContext &context, const EnterInfo &info);
RPCResult CB_JumpSuccess(LPCPEER pPeer, RPCContext &context);
RPCResult CB_ValidMap(LPCPEER pPeer, RPCContext &context, const Boolean &bOpen);
RPCResult CB_UpdateLocation(LPCPEER pPeer, RPCContext &context);
RPCResult CB_CanChangeLine(LPCPEER pPeer, RPCContext &context, const Boolean &canChange);
RPCResult GetWorldGameTime(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetWorldGameTime(LPCPEER pPeer, RPCContext &context, const SYSTEMTIME &sysTime);
RPCResult NotifyNpcCreate(LPCPEER pPeer, RPCContext &context, const NpcLiveInfo &npc);
RPCResult NotifyNpcDead(LPCPEER pPeer, RPCContext &context, const NpcLiveInfo &npc);
RPCResult QueryNpcExist(LPCPEER pPeer, RPCContext &context, const NpcLiveList &npcList);
RPCResult ForceLogout(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const String &msg);
RPCResult CB_NpcNotExist(LPCPEER pPeer, RPCContext &context, const NpcLiveList &npcList);
RPCResult CB_StartEvent(LPCPEER pPeer, RPCContext &context, NetGroup* events);
RPCResult CB_StopEvent(LPCPEER pPeer, RPCContext &context, const UInt32Vector &eventIds, const Boolean &bForce);
RPCResult CB_PeriodEvent(LPCPEER pPeer, RPCContext &context, NetGroup* events, const UInt8 &line_id);
RPCResult CB_TimeZoneChanged(LPCPEER pPeer, RPCContext &context, const UInt8 &gameTime);
RPCResult StartEvent(LPCPEER pPeer, RPCContext &context, const UInt32 &eventId);
RPCResult LoginAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &ip, const String &loginName, const ByteArray &password);
RPCResult KickAccount(LPCPEER pPeer, RPCContext &context, const Boolean &bKick);
RPCResult GetMapLines(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id);
RPCResult VersionChecking(LPCPEER pPeer, RPCContext &context, const UInt32 &versionNo);
RPCResult CB_GetMapLines(LPCPEER pPeer, RPCContext &context, const Int32Vector &lineCounts, const UInt16Vector &lineState);
RPCResult CB_LoginResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result);
RPCResult CB_WaitAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &waitTime);
RPCResult CB_LoginAccount(LPCPEER pPeer, RPCContext &context, NetObject* account, const SYSTEMTIME &sysTime);
RPCResult CB_VersionCorrect(LPCPEER pPeer, RPCContext &context, const Boolean &equal);
RPCResult CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String &forceOutMsg);
RPCResult CheckCanBuyLimitedItem(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const UInt16 &stack, const UInt32 &char_id);
RPCResult ReduceLimitItem(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const Boolean &buySuccess, const UInt16 &reduceStack, const UInt32 &char_id);
RPCResult CB_CheckCanBuyLimitedItem(LPCPEER pPeer, RPCContext &context, const UInt8 &msgCode);
RPCResult SendCheatCommand(LPCPEER pPeer, RPCContext &context, NetGroup* cheatCommandGroup);
RPCResult CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &data);
RPCResult AnswerAntiHack(LPCPEER pPeer, RPCContext &context, const UInt8 &answer);
RPCResult WorldCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &srcAcct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command);
RPCResult GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType);
RPCResult GetGuildRank(LPCPEER pPeer, RPCContext &context);
RPCResult StoreApolloLog(LPCPEER pPeer, RPCContext &context, const UInt32 &msgTime, const UInt16 &msgType, const String &TypeName, const String &msg);
RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg);
RPCResult CB_WorldCheat(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetCharacterRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp, const UInt8 &rankType);
RPCResult CB_RankChangeMsg(LPCPEER pPeer, RPCContext &context, const String &rankTitle, const String &char_name, const UInt16 &preRank, const UInt16 &curRank, const SessionIDVector &listeners);
RPCResult CB_GetGuildRank(LPCPEER pPeer, RPCContext &context, NetGroup* charRankGrp);
RPCResult ApolloMsg(LPCPEER pPeer, RPCContext &context, const ByteArray &msg);
RPCResult CB_ApolloMsg(LPCPEER pPeer, RPCContext &context, const ByteArray &msg);
RPCResult GetSellYuanBao(LPCPEER pPeer, RPCContext &context);
RPCResult CB_GetSellYuanBao(LPCPEER pPeer, RPCContext &context, NetGroup* sellYuanBaoGroup, NetGroup* sellRecord);
RPCResult AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password);
RPCResult CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex);
RPCResult BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &charTotalMoney, const UInt32 &moneyPerUnit, const ByteArray &password);
RPCResult CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Boolean &fromWorld);
RPCResult GetTempBank(LPCPEER pPeer, RPCContext &context, const ByteArray &password);
RPCResult CB_GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt32 &totalYBT, const UInt32 &totalMoney);
RPCResult ClientDisconnect(LPCPEER pPeer, RPCContext &context);
RPCResult UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money);
RPCResult CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const Boolean &result);
RPCResult WantedDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
RPCResult CB_WantedDetail(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY);
RPCResult RemoveFromWantedRank(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId);
RPCResult DeleteGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId);
RPCResult RecvGenerationAccmulationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId, const UInt32 &uiAcc);
RPCResult GenerationOnlineNotifyTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId);
RPCResult GenerationOfflineNotifyTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId);
RPCResult HasGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id);
RPCResult CB_HasGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const Boolean &result);
RPCResult GetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id);
RPCResult CB_GetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &flag);
RPCResult SetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id, const UInt16 &flag);
RPCResult CB_SetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context);
RPCResult RemoveGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id);
RPCResult CB_RemoveGlobalNpcFlag(LPCPEER pPeer, RPCContext &context);
// end of generated RPC functions

RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer *pBuf);
RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer *pBuf);

}

INLINE RPCResult WorldService_Proxy::RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_RegisterProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_UnregisterProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::RegisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id, const Boolean &bOpen)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_RegisterMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<Boolean>(bOpen);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::UnregisterMap(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_UnregisterMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::RegisterInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_RegisterInstance);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(instId);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::UnregisterInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_UnregisterInstance);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(instId);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::ValidateMap(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_ValidateMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::QueryInstance(LPCPEER pPeer, RPCContext &context, const UInt16 &instId, const UInt32 &owner_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_QueryInstance);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(instId);
	pBuf->Write<UInt32>(owner_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::CanChangeLine(LPCPEER pPeer, RPCContext &context, const UInt8 &line_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CanChangeLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(line_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::RemoteCreateNpc(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id, const UInt8 &line_id, const UInt32 &owner_id, const UInt32 &npc_id, const UInt16 &x, const UInt16 &y, const UInt32 &lifeTime, const UInt16 &eventId, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_RemoteCreateNpc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt16>(x);
	pBuf->Write<UInt16>(y);
	pBuf->Write<UInt32>(lifeTime);
	pBuf->Write<UInt16>(eventId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::ReleaseSession(LPCPEER pPeer, RPCContext &context, const SessionID &sid, const Boolean &bDelete)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_ReleaseSession);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<SessionID>(sid);
	pBuf->Write<Boolean>(bDelete);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::CharacterLeave(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CharacterLeave);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::CharJumpMap(LPCPEER pPeer, RPCContext &context, const EnterInfo &info, const UInt32Vector &memberIdList, const AntiAddictInfo &addictInfo, const String &buffCache, const UInt32 &uiLineupId, const int &iKeyPos)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CharJumpMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<EnterInfo>(info);
	pBuf->Write<UInt32Vector>(memberIdList);
	pBuf->Write<AntiAddictInfo>(addictInfo);
	pBuf->Write<String>(buffCache);
	pBuf->Write<UInt32>(uiLineupId);
	pBuf->Write<int>(iKeyPos);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::NpcJumpMap(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt16 &map_id, const UInt8 &line_id, const UInt16 &x, const UInt16 &y, const UInt32 &followId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_NpcJumpMap);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt16>(map_id);
	pBuf->Write<UInt8>(line_id);
	pBuf->Write<UInt16>(x);
	pBuf->Write<UInt16>(y);
	pBuf->Write<UInt32>(followId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::CharUpdateLocation(LPCPEER pPeer, RPCContext &context, const EnterInfo &info)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CharUpdateLocation);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<EnterInfo>(info);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GetWorldGameTime(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GetWorldGameTime);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend<617243634>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::NotifyNpcCreate(LPCPEER pPeer, RPCContext &context, const NpcLiveInfo &npc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_NotifyNpcCreate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NpcLiveInfo>(npc);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::NotifyNpcDead(LPCPEER pPeer, RPCContext &context, const NpcLiveInfo &npc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_NotifyNpcDead);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NpcLiveInfo>(npc);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::QueryNpcExist(LPCPEER pPeer, RPCContext &context, const NpcLiveList &npcList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_QueryNpcExist);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NpcLiveList>(npcList);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::ForceLogout(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &char_id, const String &msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_ForceLogout);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(acct_id);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<String>(msg);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::StartEvent(LPCPEER pPeer, RPCContext &context, const UInt32 &eventId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_StartEvent);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(eventId);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::LoginAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &ip, const String &loginName, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_LoginAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(ip);
	pBuf->Write<String>(loginName);
	pBuf->Write<ByteArray>(password);
	return RPCSend<64366381>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::KickAccount(LPCPEER pPeer, RPCContext &context, const Boolean &bKick)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_KickAccount);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bKick);
	return RPCSend<1747418293>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GetMapLines(LPCPEER pPeer, RPCContext &context, const UInt16 &map_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GetMapLines);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt16>(map_id);
	return RPCSend<1334859944>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::VersionChecking(LPCPEER pPeer, RPCContext &context, const UInt32 &versionNo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_VersionChecking);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(versionNo);
	return RPCSend<283667935>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::CheckCanBuyLimitedItem(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const UInt16 &stack, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_CheckCanBuyLimitedItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(chargeShopID);
	pBuf->Write<UInt16>(stack);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::ReduceLimitItem(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const Boolean &buySuccess, const UInt16 &reduceStack, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_ReduceLimitItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(chargeShopID);
	pBuf->Write<Boolean>(buySuccess);
	pBuf->Write<UInt16>(reduceStack);
	pBuf->Write<UInt32>(char_id);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::SendCheatCommand(LPCPEER pPeer, RPCContext &context, NetGroup* cheatCommandGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_SendCheatCommand);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(cheatCommandGroup);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::AnswerAntiHack(LPCPEER pPeer, RPCContext &context, const UInt8 &answer)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_AnswerAntiHack);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(answer);
	return RPCSend<275790643>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::WorldCheat(LPCPEER pPeer, RPCContext &context, const UInt32 &exe_id, const UInt32 &srcAcct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_WorldCheat);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(exe_id);
	pBuf->Write<UInt32>(srcAcct_id);
	pBuf->Write<UInt32>(target_acct_id);
	pBuf->Write<UInt32>(target_char_id);
	pBuf->Write<String>(command);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GetCharacterRank(LPCPEER pPeer, RPCContext &context, const UInt8 &rankType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GetCharacterRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(rankType);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GetGuildRank(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GetGuildRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::StoreApolloLog(LPCPEER pPeer, RPCContext &context, const UInt32 &msgTime, const UInt16 &msgType, const String &TypeName, const String &msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_StoreApolloLog);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(msgTime);
	pBuf->Write<UInt16>(msgType);
	pBuf->Write<String>(TypeName);
	pBuf->Write<String>(msg);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::ApolloMsg(LPCPEER pPeer, RPCContext &context, const ByteArray &msg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_ApolloMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ByteArray>(msg);
	return RPCSend<1724152093>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GetSellYuanBao(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GetSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend<627513020>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &YBT, const UInt32 &pricePerUnit, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_AddSellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(YBT);
	pBuf->Write<UInt32>(pricePerUnit);
	pBuf->Write<ByteArray>(password);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const UInt32 &charTotalMoney, const UInt32 &moneyPerUnit, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_BuySellYuanBao);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(sellYuanBaoID);
	pBuf->Write<UInt32>(YBT);
	pBuf->Write<UInt32>(charTotalMoney);
	pBuf->Write<UInt32>(moneyPerUnit);
	pBuf->Write<ByteArray>(password);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GetTempBank(LPCPEER pPeer, RPCContext &context, const ByteArray &password)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GetTempBank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ByteArray>(password);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::ClientDisconnect(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_ClientDisconnect);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_UpdateWantedReward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(targetId);
	pBuf->Write<Int32>(money);
	return RPCSend<1526680949>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::WantedDetail(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_WantedDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<1772356250>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::RemoveFromWantedRank(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_RemoveFromWantedRank);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(targetId);
	return RPCSend<434708509>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::DeleteGenerationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_DeleteGenerationTrans);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<1925659521>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::RecvGenerationAccmulationTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId, const UInt32 &uiAcc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_RecvGenerationAccmulationTrans);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	pBuf->Write<UInt32>(uiAcc);
	return RPCSend<511794514>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GenerationOnlineNotifyTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GenerationOnlineNotifyTrans);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<1225659217>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GenerationOfflineNotifyTrans(LPCPEER pPeer, RPCContext &context, const UInt32 &uiTargetId, const UInt32 &uiRelatedId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GenerationOfflineNotifyTrans);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(uiTargetId);
	pBuf->Write<UInt32>(uiRelatedId);
	return RPCSend<2097624507>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::HasGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_HasGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(flag_id);
	return RPCSend<1895772719>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::GetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_GetGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(flag_id);
	return RPCSend<666986740>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::SetGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id, const UInt16 &flag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_SetGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(flag_id);
	pBuf->Write<UInt16>(flag);
	return RPCSend<364331590>(pPeer, pBuf);
}

INLINE RPCResult WorldService_Proxy::RemoveGlobalNpcFlag(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const UInt32 &owner_id, const UInt16 &flag_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_WorldService_RemoveGlobalNpcFlag);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt16>(flag_id);
	return RPCSend<1483754257>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_ENCRYPT_HEAD(617243634)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(962679827)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(64366381)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1747418293)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1334859944)
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(283667935)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(969285436)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(277704283)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(550449771)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1343100704)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(318650066)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(427715877)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1517552885)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(275790643)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(720059219)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1523673697)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1840188879)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(817654996)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1603016263)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1724152093)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(2080578402)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(627513020)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1616532101)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1526680949)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(695082008)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1772356250)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1708657089)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(434708509)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1925659521)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(511794514)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1225659217)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2097624507)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1895772719)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(242774850)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(666986740)
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(676280625)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(364331590)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1994195922)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1483754257)
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1535713103)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_WORLDSERVICE_PROXY_H_
