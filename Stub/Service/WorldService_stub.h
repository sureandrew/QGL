///////////////////////////////////////////////////////////
///        Generated RPC function header by IDL         ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_WORLDSERVICE_STUB_H_
#define _RPC_WORLDSERVICE_STUB_H_

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

namespace WorldService_Stub {

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



#endif //_RPC_WORLDSERVICE_STUB_H_
