//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/MapService_stub.h"

#pragma optimize("", off)
RPCResult MapService_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_MapService_RegisterProxy:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = RegisterProxy(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_UnregisterProxy:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = UnregisterProxy(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_ReleaseSession:
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
	case RPC_MapService_SyncProxy:
		{
			_result = SyncProxy(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_ChangeSessionId:
		{
			SessionID oldSessId;
			pBuf->Read<SessionID>(oldSessId);
			_result = ChangeSessionId(pPeer, context, oldSessId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_ResumeSession:
		{
			_result = ResumeSession(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_RemoteCreateNpc:
		{
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			NpcCreateList npcList;
			pBuf->Read<NpcCreateList>(npcList);
			_result = RemoteCreateNpc(pPeer, context, map_id, line_id, owner_id, npcList);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_QueryInstance:
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
	case RPC_MapService_CharacterEnter:
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
			_result = CharacterEnter(pPeer, context, info, memberIdList, addictInfo, buffCache, uiLineupId, iKeyPos);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_CharacterLeave:
		{
			Boolean bCallback;
			pBuf->Read<Boolean>(bCallback);
			_result = CharacterLeave(pPeer, context, bCallback);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_NpcEnter:
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
			_result = NpcEnter(pPeer, context, npc_id, map_id, line_id, x, y, followId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_ChangeLoadLevel:
		{
			UInt8 level;
			pBuf->Read<UInt8>(level);
			_result = ChangeLoadLevel(pPeer, context, level);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_RankTitleUpdate:
		{
			UInt32Vector char_ids;
			pBuf->Read<UInt32Vector>(char_ids);
			UInt16Vector title_ids;
			pBuf->Read<UInt16Vector>(title_ids);
			_result = RankTitleUpdate(pPeer, context, char_ids, title_ids);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_GuildUpdate:
		{
			NetGroup* addGrp;
			pBuf->Read<NetGroup*>(addGrp);
			_result = GuildUpdate(pPeer, context, addGrp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_GuildRemove:
		{
			UInt32Vector guild_uids;
			pBuf->Read<UInt32Vector>(guild_uids);
			_result = GuildRemove(pPeer, context, guild_uids);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_GuildMemberUpdate:
		{
			NetGroup* addGrp;
			pBuf->Read<NetGroup*>(addGrp);
			_result = GuildMemberUpdate(pPeer, context, addGrp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_GuildMemberRemove:
		{
			UInt32Vector char_ids;
			pBuf->Read<UInt32Vector>(char_ids);
			_result = GuildMemberRemove(pPeer, context, char_ids);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_MsgCharacterUpdate:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = MsgCharacterUpdate(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_ManorForceOut:
		{
			UInt32 guild_UID;
			pBuf->Read<UInt32>(guild_UID);
			UInt32Vector guild_charUIDs;
			pBuf->Read<UInt32Vector>(guild_charUIDs);
			Boolean memberKick;
			pBuf->Read<Boolean>(memberKick);
			_result = ManorForceOut(pPeer, context, guild_UID, guild_charUIDs, memberKick);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_CharacterCheat:
		{
			UInt32 exe_id;
			pBuf->Read<UInt32>(exe_id);
			UInt32 SrcAcct_id;
			pBuf->Read<UInt32>(SrcAcct_id);
			UInt32 target_acct_id;
			pBuf->Read<UInt32>(target_acct_id);
			UInt32 target_char_id;
			pBuf->Read<UInt32>(target_char_id);
			String command;
			pBuf->Read<String>(command);
			_result = CharacterCheat(pPeer, context, exe_id, SrcAcct_id, target_acct_id, target_char_id, command);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_UpdateLimitedItemRemain:
		{
			NetGroup* limitedItemData;
			pBuf->Read<NetGroup*>(limitedItemData);
			_result = UpdateLimitedItemRemain(pPeer, context, limitedItemData);
		}
		break;
#endif // CLIENT
	case RPC_MapService_ShowPublicItemDetail:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 item_uid;
			pBuf->Read<UInt32>(item_uid);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = ShowPublicItemDetail(pPeer, context, char_id, item_uid, char_idTo);
		}
		break;
	case RPC_MapService_ShowPublicPartnerDetail:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 partner_id;
			pBuf->Read<UInt32>(partner_id);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = ShowPublicPartnerDetail(pPeer, context, char_id, partner_id, char_idTo);
		}
		break;
	case RPC_MapService_ShowPublicPetDetail:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 pet_id;
			pBuf->Read<UInt32>(pet_id);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = ShowPublicPetDetail(pPeer, context, char_id, pet_id, char_idTo);
		}
		break;
	case RPC_MapService_OpenChargeShop:
		{
			_result = OpenChargeShop(pPeer, context);
		}
		break;
	case RPC_MapService_CloseChargeShop:
		{
			_result = CloseChargeShop(pPeer, context);
		}
		break;
	case RPC_MapService_SendMapMessage:
		{
			RPCNetID charNetID;
			pBuf->Read<RPCNetID>(charNetID);
			String Msg;
			pBuf->Read<String>(Msg);
			_result = SendMapMessage(pPeer, context, charNetID, Msg);
		}
		break;
	case RPC_MapService_SendMapEmotion:
		{
			RPCNetID charNetID;
			pBuf->Read<RPCNetID>(charNetID);
			UInt32 emo_id;
			pBuf->Read<UInt32>(emo_id);
			RPCNetID TargetNetID;
			pBuf->Read<RPCNetID>(TargetNetID);
			bool isPet;
			pBuf->Read<bool>(isPet);
			_result = SendMapEmotion(pPeer, context, charNetID, emo_id, TargetNetID, isPet);
		}
		break;
	case RPC_MapService_ChangeMoney:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			Int32 change;
			pBuf->Read<Int32>(change);
			_result = ChangeMoney(pPeer, context, owner_id, change);
		}
		break;
#ifndef CLIENT
	case RPC_MapService_ClientDisconnect:
		{
			_result = ClientDisconnect(pPeer, context);
		}
		break;
#endif // CLIENT
	case RPC_MapService_GetPosition:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			_result = GetPosition(pPeer, context, targetId);
		}
		break;
	case RPC_MapService_UpdateWantedReward:
		{
			UInt32 targetId;
			pBuf->Read<UInt32>(targetId);
			Int32 money;
			pBuf->Read<Int32>(money);
			_result = UpdateWantedReward(pPeer, context, targetId, money);
		}
		break;
	case RPC_MapService_DeleteGenerationFromWorld:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = DeleteGenerationFromWorld(pPeer, context, uiTargetId, uiRelatedId);
		}
		break;
	case RPC_MapService_RecvGenerationAccmulation:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			UInt32 uiAcc;
			pBuf->Read<UInt32>(uiAcc);
			_result = RecvGenerationAccmulation(pPeer, context, uiTargetId, uiRelatedId, uiAcc);
		}
		break;
	case RPC_MapService_GenerationOnlineNotify:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = GenerationOnlineNotify(pPeer, context, uiTargetId, uiRelatedId);
		}
		break;
	case RPC_MapService_GenerationOfflineNotify:
		{
			UInt32 uiTargetId;
			pBuf->Read<UInt32>(uiTargetId);
			UInt32 uiRelatedId;
			pBuf->Read<UInt32>(uiRelatedId);
			_result = GenerationOfflineNotify(pPeer, context, uiTargetId, uiRelatedId);
		}
		break;
	case RPC_MapService_GetGenenrationCharsResult:
		{
			NetGroup* pResult;
			pBuf->Read<NetGroup*>(pResult);
			_result = GetGenenrationCharsResult(pPeer, context, pResult);
		}
		break;
	case RPC_MapService_ShowPublicItemDetailFromOtherLine:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 item_uid;
			pBuf->Read<UInt32>(item_uid);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = ShowPublicItemDetailFromOtherLine(pPeer, context, char_id, item_uid, char_idTo);
		}
		break;
	case RPC_MapService_ShowPublicPartnerDetailFromOtherLine:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 partner_id;
			pBuf->Read<UInt32>(partner_id);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = ShowPublicPartnerDetailFromOtherLine(pPeer, context, char_id, partner_id, char_idTo);
		}
		break;
	case RPC_MapService_ShowPublicPetDetailFromOtherLine:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 pet_id;
			pBuf->Read<UInt32>(pet_id);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = ShowPublicPetDetailFromOtherLine(pPeer, context, char_id, pet_id, char_idTo);
		}
		break;
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
#pragma optimize("", on)

RPCResult MapService_Stub::CB_SyncProxy(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_SyncProxy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ResumeSession(LPCPEER pPeer, RPCContext &context, const Boolean &valid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ResumeSession);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(valid);
	return RPCSend(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_QueryInstance(LPCPEER pPeer, RPCContext &context, const Boolean &bExist, const UInt16 &allowCount)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_QueryInstance);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bExist);
	pBuf->Write<UInt16>(allowCount);
	return RPCSend(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(err);
	pBuf->Write<String>(errMsg);
	return RPCSend<950751607>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_EnterSuccess(LPCPEER pPeer, RPCContext &context, NetObject* map, NetObject* character)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_EnterSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(map);
	pBuf->Write<NetObject*>(character);
	return RPCSend<1732650242>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_CharacterStuff(LPCPEER pPeer, RPCContext &context, NetObject* bag, NetObject* scriptSys, NetObject* questSys, NetGroup* skillGroup, NetGroup* partnerGroup, NetGroup* petGroup, const UInt8 &addictHealth)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_CharacterStuff);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(bag);
	pBuf->Write<NetObject*>(scriptSys);
	pBuf->Write<NetObject*>(questSys);
	pBuf->Write<NetGroup*>(skillGroup);
	pBuf->Write<NetGroup*>(partnerGroup);
	pBuf->Write<NetGroup*>(petGroup);
	pBuf->Write<UInt8>(addictHealth);
	return RPCSend<1241854683>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_LeaveSuccess(LPCPEER pPeer, RPCContext &context, const Boolean &bDelete)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_LeaveSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(bDelete);
	return RPCSend<1047143100>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_CharacterEnter(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_CharacterEnter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1698965209>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_PartyEnter(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_PartyEnter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<688273772>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_NpcEnter(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_NpcEnter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<17040657>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_EnterBattle(LPCPEER pPeer, RPCContext &context, NetObject* battle, NetGroup* grp, const UInt8 &PlayerTeam, const UInt8 &battleState, const UInt32 &npcId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_EnterBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(battle);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<UInt8>(PlayerTeam);
	pBuf->Write<UInt8>(battleState);
	pBuf->Write<UInt32>(npcId);
	return RPCSend<1376742086>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_CharacterCheat(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_CharacterCheat);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_OpenChargeShop(LPCPEER pPeer, RPCContext &context, NetGroup* chargeShop, NetGroup* discountGroup, NetObject* limitedItem)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_OpenChargeShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(chargeShop);
	pBuf->Write<NetGroup*>(discountGroup);
	pBuf->Write<NetObject*>(limitedItem);
	return RPCSend<1218269948>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_SyncCharacter(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_SyncCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1421242889>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_SyncItem(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_SyncItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<406863196>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_SyncPartner(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_SyncPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<987312133>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_AddPartnersSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_AddPartnersSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1631546235>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ShowPublicItemDetail(LPCPEER pPeer, RPCContext &context, NetObject* item)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ShowPublicItemDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(item);
	return RPCSend<2122804530>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ShowPublicPartnerDetail(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillIDs)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ShowPublicPartnerDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(Partner);
	pBuf->Write<UInt16Vector>(skillIDs);
	return RPCSend<181617556>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ShowPublicPetDetail(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillIDs)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ShowPublicPetDetail);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(Pet);
	pBuf->Write<UInt16Vector>(skillIDs);
	return RPCSend<1441406415>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ReceiveMapMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ReceiveMapMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<String>(SrcName);
	pBuf->Write<SessionIDVector>(listeners);
	return RPCSend<1763596625>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ReceiveMapEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ReceiveMapEmotion);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(emo_id);
	pBuf->Write<String>(SrcName);
	pBuf->Write<String>(TargetName);
	pBuf->Write<bool>(isPet);
	return RPCSend<83128094>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ForceLogout(LPCPEER pPeer, RPCContext &context, const String &forceOutMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ForceLogout);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(forceOutMsg);
	return RPCSend<1131569201>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_GetPosition(LPCPEER pPeer, RPCContext &context, const UInt32 &mapX, const UInt32 &mapY)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_GetPosition);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(mapX);
	pBuf->Write<UInt32>(mapY);
	return RPCSend<1108702494>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const Boolean &result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_UpdateWantedReward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(result);
	return RPCSend<1292175556>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Partner, const UInt16Vector &skillIDs, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ShowPublicPartnerDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(Partner);
	pBuf->Write<UInt16Vector>(skillIDs);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<509741308>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* Pet, const UInt16Vector &skillIDs, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ShowPublicPetDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(Pet);
	pBuf->Write<UInt16Vector>(skillIDs);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<336362802>(pPeer, pBuf);
}

RPCResult MapService_Stub::CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt32 &char_idTo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MapService_CB_ShowPublicItemDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(item);
	pBuf->Write<UInt32>(char_idTo);
	return RPCSend<1415328226>(pPeer, pBuf);
}

