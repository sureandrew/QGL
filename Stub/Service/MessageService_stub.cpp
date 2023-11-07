//-- Common
#include "Common.h"
//-- Self
#include "Stub/Service/MessageService_stub.h"

#pragma optimize("", off)
RPCResult MessageService_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_MessageService_RegisterProxy:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = RegisterProxy(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_UnregisterProxy:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = UnregisterProxy(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_RegisterMap:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			UInt16 map_id;
			pBuf->Read<UInt16>(map_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			_result = RegisterMap(pPeer, context, serviceId, map_id, line_id, owner_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_UnregisterMap:
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
	case RPC_MessageService_ReleaseSession:
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
	case RPC_MessageService_ApplyChange:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			RPCMask changeMask;
			pBuf->Read<RPCMask>(changeMask);
			Int32Vector updatedata;
			pBuf->Read<Int32Vector>(updatedata);
			UInt8 source;
			pBuf->Read<UInt8>(source);
			_result = ApplyChange(pPeer, context, guild_uid, changeMask, updatedata, source);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_GenerateGuildGroup:
		{
			UInt8 guildWarState;
			pBuf->Read<UInt8>(guildWarState);
			_result = GenerateGuildGroup(pPeer, context, guildWarState);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_GenerateGuildGroupCW:
		{
			UInt8 guildWarState;
			pBuf->Read<UInt8>(guildWarState);
			_result = GenerateGuildGroupCW(pPeer, context, guildWarState);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_GuildWarResult:
		{
			UInt32 guild_uid1;
			pBuf->Read<UInt32>(guild_uid1);
			UInt32 guild_uid2;
			pBuf->Read<UInt32>(guild_uid2);
			UInt32 win_guild_uid;
			pBuf->Read<UInt32>(win_guild_uid);
			_result = GuildWarResult(pPeer, context, guild_uid1, guild_uid2, win_guild_uid);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_KickMember:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 kicker_id;
			pBuf->Read<UInt32>(kicker_id);
			UInt32 member_id;
			pBuf->Read<UInt32>(member_id);
			String reason;
			pBuf->Read<String>(reason);
			_result = KickMember(pPeer, context, guild_uid, kicker_id, member_id, reason);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_ManorScaleChange:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt8 scale;
			pBuf->Read<UInt8>(scale);
			_result = ManorScaleChange(pPeer, context, guild_uid, scale);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_TransmitGuildMsg:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 guildUID;
			pBuf->Read<UInt32>(guildUID);
			_result = TransmitGuildMsg(pPeer, context, Msg, guildUID);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_GetAvailService:
		{
			_result = GetAvailService(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_SendHorseLightMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 SourceId;
			pBuf->Read<UInt32>(SourceId);
			UInt32 count;
			pBuf->Read<UInt32>(count);
			_result = SendHorseLightMessage(pPeer, context, Msg, SourceId, count);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_SendSystemMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 SourceId;
			pBuf->Read<UInt32>(SourceId);
			_result = SendSystemMessage(pPeer, context, Msg, SourceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_SendRumorMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 SourceId;
			pBuf->Read<UInt32>(SourceId);
			BYTE SubType;
			pBuf->Read<BYTE>(SubType);
			_result = SendRumorMessage(pPeer, context, Msg, SourceId, SubType);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_BattleEndUpdate:
		{
			UInt32Vector char_idList;
			pBuf->Read<UInt32Vector>(char_idList);
			_result = BattleEndUpdate(pPeer, context, char_idList);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_AddMsgSchedule:
		{
			UInt16 id;
			pBuf->Read<UInt16>(id);
			UInt32 startTime;
			pBuf->Read<UInt32>(startTime);
			UInt16 periodCount;
			pBuf->Read<UInt16>(periodCount);
			UInt16 count;
			pBuf->Read<UInt16>(count);
			UInt32 period;
			pBuf->Read<UInt32>(period);
			UInt8 msgType;
			pBuf->Read<UInt8>(msgType);
			String msg;
			pBuf->Read<String>(msg);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = AddMsgSchedule(pPeer, context, id, startTime, periodCount, count, period, msgType, msg, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_RemoveMsgSchedule:
		{
			UInt16 id;
			pBuf->Read<UInt16>(id);
			_result = RemoveMsgSchedule(pPeer, context, id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_UpdateCharInfo:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			_result = UpdateCharInfo(pPeer, context, grp);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_ChannelsBlockHandle:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8Vector bkChannels;
			pBuf->Read<UInt8Vector>(bkChannels);
			UInt8Vector unBkChannels;
			pBuf->Read<UInt8Vector>(unBkChannels);
			_result = ChannelsBlockHandle(pPeer, context, char_id, bkChannels, unBkChannels);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_ChangeMsgIcon:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt16 iconID;
			pBuf->Read<UInt16>(iconID);
			Boolean change;
			pBuf->Read<Boolean>(change);
			_result = ChangeMsgIcon(pPeer, context, char_id, iconID, change);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_AddSpeakCount:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			Int16 count;
			pBuf->Read<Int16>(count);
			_result = AddSpeakCount(pPeer, context, char_id, count);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_ChangeGDonate:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			Int gChange;
			pBuf->Read<Int>(gChange);
			UInt8 source;
			pBuf->Read<UInt8>(source);
			_result = ChangeGDonate(pPeer, context, guild_uid, char_id, gChange, source);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_RemoveMessengersByOwner:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RemoveMessengersByOwner(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_RemoveAllGuildApplicant:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RemoveAllGuildApplicant(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_CharacterLeave:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CharacterLeave(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_CreateGuild:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String guild_name;
			pBuf->Read<String>(guild_name);
			String guild_creator;
			pBuf->Read<String>(guild_creator);
			String guild_master;
			pBuf->Read<String>(guild_master);
			_result = CreateGuild(pPeer, context, char_id, guild_name, guild_creator, guild_master);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_OpenGuildApply:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = OpenGuildApply(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_DonateMoney:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = DonateMoney(pPeer, context, guild_uid, char_id, money);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_InitMessageChannel:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = InitMessageChannel(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_ResumeMessageChannel:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = ResumeMessageChannel(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_ReleaseMsgCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = ReleaseMsgCharacter(pPeer, context, char_id);
		}
		break;
#endif // CLIENT
	case RPC_MessageService_SendPrivateMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 SourceId;
			pBuf->Read<UInt32>(SourceId);
			UInt32 TargetID;
			pBuf->Read<UInt32>(TargetID);
			String TargetName;
			pBuf->Read<String>(TargetName);
			boolean isFriend;
			pBuf->Read<boolean>(isFriend);
			_result = SendPrivateMessage(pPeer, context, Msg, SourceId, TargetID, TargetName, isFriend);
		}
		break;
	case RPC_MessageService_SendFriendMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 Char_id;
			pBuf->Read<UInt32>(Char_id);
			_result = SendFriendMessage(pPeer, context, Msg, Char_id);
		}
		break;
	case RPC_MessageService_LeaveChannel:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String ChannelName;
			pBuf->Read<String>(ChannelName);
			_result = LeaveChannel(pPeer, context, char_id, ChannelName);
		}
		break;
	case RPC_MessageService_LeaveAllChannel:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = LeaveAllChannel(pPeer, context, char_id);
		}
		break;
	case RPC_MessageService_SetListen:
		{
			BOOL IsListen;
			pBuf->Read<BOOL>(IsListen);
			UInt32 char_Id;
			pBuf->Read<UInt32>(char_Id);
			_result = SetListen(pPeer, context, IsListen, char_Id);
		}
		break;
	case RPC_MessageService_JoinMessageChannel:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String char_Name;
			pBuf->Read<String>(char_Name);
			String ChannelName;
			pBuf->Read<String>(ChannelName);
			BYTE JoinMode;
			pBuf->Read<BYTE>(JoinMode);
			_result = JoinMessageChannel(pPeer, context, char_id, char_Name, ChannelName, JoinMode);
		}
		break;
	case RPC_MessageService_CreateMessenger:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			UInt16Vector relations;
			pBuf->Read<UInt16Vector>(relations);
			UInt8 group;
			pBuf->Read<UInt8>(group);
			UInt16 friendly;
			pBuf->Read<UInt16>(friendly);
			String targetName;
			pBuf->Read<String>(targetName);
			_result = CreateMessenger(pPeer, context, owner_id, target_id, relations, group, friendly, targetName);
		}
		break;
	case RPC_MessageService_RemoveMessenger:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			_result = RemoveMessenger(pPeer, context, owner_id, target_id);
		}
		break;
	case RPC_MessageService_RemoveRelation:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			UInt16 relation;
			pBuf->Read<UInt16>(relation);
			_result = RemoveRelation(pPeer, context, owner_id, target_id, relation);
		}
		break;
	case RPC_MessageService_RequestInfo:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String name;
			pBuf->Read<String>(name);
			bool update;
			pBuf->Read<bool>(update);
			bool tip;
			pBuf->Read<bool>(tip);
			_result = RequestInfo(pPeer, context, char_id, name, update, tip);
		}
		break;
	case RPC_MessageService_GetMessengers:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 relation;
			pBuf->Read<UInt16>(relation);
			_result = GetMessengers(pPeer, context, owner_id, relation);
		}
		break;
	case RPC_MessageService_GetMessengersName:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 relation;
			pBuf->Read<UInt16>(relation);
			_result = GetMessengersName(pPeer, context, owner_id, relation);
		}
		break;
	case RPC_MessageService_GetFriendly:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			_result = GetFriendly(pPeer, context, owner_id, target_id);
		}
		break;
	case RPC_MessageService_IsFriend:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			_result = IsFriend(pPeer, context, owner_id, target_id);
		}
		break;
	case RPC_MessageService_ChangeFriendly:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			Int32 changed;
			pBuf->Read<Int32>(changed);
			_result = ChangeFriendly(pPeer, context, owner_id, target_id, changed);
		}
		break;
	case RPC_MessageService_ApplyGuild:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			_result = ApplyGuild(pPeer, context, char_id, guild_uid);
		}
		break;
	case RPC_MessageService_RequestGuildAim:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			_result = RequestGuildAim(pPeer, context, guild_uid);
		}
		break;
	case RPC_MessageService_ChangeTitle:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			String title;
			pBuf->Read<String>(title);
			_result = ChangeTitle(pPeer, context, char_id, title);
		}
		break;
	case RPC_MessageService_SetMoney:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = SetMoney(pPeer, context, owner_id, money);
		}
		break;
	case RPC_MessageService_GetTargetFromHawk:
		{
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			_result = GetTargetFromHawk(pPeer, context, target_id);
		}
		break;
	case RPC_MessageService_BuyItemFromBusinessShop:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			Int16 shopId;
			pBuf->Read<Int16>(shopId);
			UInt16 count;
			pBuf->Read<UInt16>(count);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = BuyItemFromBusinessShop(pPeer, context, itemId, shopId, count, money, guild_uid, char_id);
		}
		break;
	case RPC_MessageService_SellItemToBusinessShop:
		{
			UInt32 itemId;
			pBuf->Read<UInt32>(itemId);
			Int16 shopId;
			pBuf->Read<Int16>(shopId);
			UInt16 count;
			pBuf->Read<UInt16>(count);
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = SellItemToBusinessShop(pPeer, context, itemId, shopId, count, guild_uid, char_id);
		}
		break;
	case RPC_MessageService_OpenBusinessShopMenu:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt16 shop_id;
			pBuf->Read<UInt16>(shop_id);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			_result = OpenBusinessShopMenu(pPeer, context, owner_id, shop_id, money);
		}
		break;
	case RPC_MessageService_UpdateBusinessShopList:
		{
			UInt16 shop_id;
			pBuf->Read<UInt16>(shop_id);
			_result = UpdateBusinessShopList(pPeer, context, shop_id);
		}
		break;
	case RPC_MessageService_ChangeBusinessProfit:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			Int32 profit;
			pBuf->Read<Int32>(profit);
			_result = ChangeBusinessProfit(pPeer, context, guild_uid, char_id, profit);
		}
		break;
	case RPC_MessageService_ApplyTrader:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = ApplyTrader(pPeer, context, guild_uid, char_id);
		}
		break;
	case RPC_MessageService_GetGenerationChars:
		{
			UInt32Vector uiVtRelatedIds;
			pBuf->Read<UInt32Vector>(uiVtRelatedIds);
			_result = GetGenerationChars(pPeer, context, uiVtRelatedIds);
		}
		break;
	case RPC_MessageService_GetTargetInfo:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = GetTargetInfo(pPeer, context, char_id);
		}
		break;
#ifndef CLIENT
	case RPC_MessageService_ClientDisconnect:
		{
			_result = ClientDisconnect(pPeer, context);
		}
		break;
#endif // CLIENT
	case RPC_MessageService_ShowPublicPartnerDetailFromOtherLine:
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
	case RPC_MessageService_ShowPublicPetDetailFromOtherLine:
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
	case RPC_MessageService_ShowPublicItemDetailFromOtherLine:
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
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}
#pragma optimize("", on)

RPCResult MessageService_Stub::CB_BattleEndUpdate(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_BattleEndUpdate);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_GetAvailService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_GetAvailService);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<ServiceID>(serviceId);
	return RPCSend(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const Boolean &canChange)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ChangeMsgIcon);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(canChange);
	return RPCSend(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_RemoveMessengersByOwner(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_RemoveMessengersByOwner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt8 &bResult)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_CreateGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt8>(bResult);
	return RPCSend(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &addedGDonate, const UInt8 &returnFlag, const UInt32 &addGMoney)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_DonateMoney);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(addedGDonate);
	pBuf->Write<UInt8>(returnFlag);
	pBuf->Write<UInt32>(addGMoney);
	return RPCSend(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(err);
	pBuf->Write<String>(errMsg);
	return RPCSend<1330994912>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_InitMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_InitMessageChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BYTE>(Result);
	return RPCSend<1627015832>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ReceiveMessageChannel(LPCPEER pPeer, RPCContext &context, NetObject* channel, const BYTE &Type)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ReceiveMessageChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(channel);
	pBuf->Write<BYTE>(Type);
	return RPCSend<1478577585>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_SendPrivateMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const boolean &isFriend, const UInt32 &resultFlag)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_SendPrivateMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BYTE>(Result);
	pBuf->Write<boolean>(isFriend);
	pBuf->Write<UInt32>(resultFlag);
	return RPCSend<2136552202>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ReceiveFriendMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ReceiveFriendMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<String>(SrcName);
	return RPCSend<1000022068>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_SendHorseLightMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_SendHorseLightMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BYTE>(Result);
	return RPCSend<1771708645>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_SendSystemMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_SendSystemMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BYTE>(Result);
	return RPCSend<225803454>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_SendRumorMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_SendRumorMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BYTE>(Result);
	return RPCSend<850679110>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_SendShutdownMessage(LPCPEER pPeer, RPCContext &context, const UInt32 &downTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_SendShutdownMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(downTime);
	return RPCSend<2019961070>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ReceivePrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const UInt32 &SrcChar_id, const String &TargetName, const UInt32 &TargetID, const boolean &isFriend)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ReceivePrivateMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<String>(SrcName);
	pBuf->Write<UInt32>(SrcChar_id);
	pBuf->Write<String>(TargetName);
	pBuf->Write<UInt32>(TargetID);
	pBuf->Write<boolean>(isFriend);
	return RPCSend<608457820>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ReceiveHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &count, const SessionIDVector &listeners)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ReceiveHorseLightMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<UInt32>(count);
	pBuf->Write<SessionIDVector>(listeners);
	return RPCSend<1076901719>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ReceiveSystemMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const SessionIDVector &listeners)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ReceiveSystemMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<SessionIDVector>(listeners);
	return RPCSend<1718880279>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ReceiveRumorMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const BYTE &SubType, const SessionIDVector &listeners)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ReceiveRumorMessage);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(Msg);
	pBuf->Write<BYTE>(SubType);
	pBuf->Write<SessionIDVector>(listeners);
	return RPCSend<302327494>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_SetListen(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_SetListen);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1326528604>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_JoinMessageChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BYTE>(Result);
	return RPCSend<595267595>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_UpdateMessengers(LPCPEER pPeer, RPCContext &context, NetGroup* messengers)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_UpdateMessengers);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(messengers);
	return RPCSend<1042965161>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_UpdateMsgCharInfo(LPCPEER pPeer, RPCContext &context, NetGroup* magCharInfoGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_UpdateMsgCharInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(magCharInfoGrp);
	return RPCSend<698774636>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_RemoveMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_RemoveMessengers);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32>(target_id);
	return RPCSend<1832067147>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_RequestInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgCharData, const bool &update)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_RequestInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(msgCharData);
	pBuf->Write<bool>(update);
	return RPCSend<1752050440>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_MessageReturn(LPCPEER pPeer, RPCContext &context, const String &msg, const String &name)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_MessageReturn);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(msg);
	pBuf->Write<String>(name);
	return RPCSend<2020163493>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_GetMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32Vector &messgers)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_GetMessengers);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<UInt32Vector>(messgers);
	return RPCSend<333594757>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_GetMessengersName(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const StringVector &messgers_name)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_GetMessengersName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(owner_id);
	pBuf->Write<StringVector>(messgers_name);
	return RPCSend<689261880>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_GetFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &friendly)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_GetFriendly);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(friendly);
	return RPCSend<963340648>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_IsFriend(LPCPEER pPeer, RPCContext &context, const UInt32 &isFriend)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_IsFriend);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(isFriend);
	return RPCSend<173611235>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ChangeFriendly(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ChangeFriendly);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	return RPCSend<1097009387>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ApplyGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ApplyGuild);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<UInt32>(guild_uid);
	return RPCSend<286279200>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_Guilds(LPCPEER pPeer, RPCContext &context, NetGroup* GuildGrp, const bool &openApply)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_Guilds);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(GuildGrp);
	pBuf->Write<bool>(openApply);
	return RPCSend<1080500677>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_GuildMembers(LPCPEER pPeer, RPCContext &context, NetGroup* GuildMemberGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_GuildMembers);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(GuildMemberGrp);
	return RPCSend<1004997677>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_RequestApplicant(LPCPEER pPeer, RPCContext &context, NetGroup* pGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_RequestApplicant);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetGroup*>(pGrp);
	return RPCSend<1398277389>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_CreateGuildMsg(LPCPEER pPeer, RPCContext &context, const String &guildName, const String &creator, const SessionIDVector &listeners)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_CreateGuildMsg);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<String>(guildName);
	pBuf->Write<String>(creator);
	pBuf->Write<SessionIDVector>(listeners);
	return RPCSend<769019868>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_GetTargetFromHawk(LPCPEER pPeer, RPCContext &context, const bool &isonline, const String &char_name)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_GetTargetFromHawk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<bool>(isonline);
	pBuf->Write<String>(char_name);
	return RPCSend<355163471>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt16 &finalCount, const UInt32 &finalPrice)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_BuyItemFromBusinessShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(success);
	pBuf->Write<UInt16>(finalCount);
	pBuf->Write<UInt32>(finalPrice);
	return RPCSend<579892677>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt32 &finalPrice)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_SellItemToBusinessShop);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(success);
	pBuf->Write<UInt32>(finalPrice);
	return RPCSend<1460503>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, const Boolean &sell, const UInt16 &shop_id, const UInt8 &shopName, const UInt32 &SynTime, const UInt32 &BusinessMoney, const BusinessShopItemInfoVector &ItemInfoList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_OpenBusinessShopMenu);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<Boolean>(sell);
	pBuf->Write<UInt16>(shop_id);
	pBuf->Write<UInt8>(shopName);
	pBuf->Write<UInt32>(SynTime);
	pBuf->Write<UInt32>(BusinessMoney);
	pBuf->Write<BusinessShopItemInfoVector>(ItemInfoList);
	return RPCSend<313945116>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const BusinessShopItemInfoVector &ItemInfoList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_UpdateBusinessShopList);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<BusinessShopItemInfoVector>(ItemInfoList);
	return RPCSend<677403017>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_GetTargetInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgTargetData)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_GetTargetInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<NetObject*>(msgTargetData);
	return RPCSend<1879651114>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CItem &item)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ShowPublicItemDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<CItem>(item);
	return RPCSend<541660631>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPartner &partner, const UInt16Vector &skillIDs)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ShowPublicPartnerDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<CPartner>(partner);
	pBuf->Write<UInt16Vector>(skillIDs);
	return RPCSend<409940819>(pPeer, pBuf);
}

RPCResult MessageService_Stub::CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPet &pet, const UInt16Vector &skillIDs)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageService_CB_ShowPublicPetDetailFromOtherLine);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<CPet>(pet);
	pBuf->Write<UInt16Vector>(skillIDs);
	return RPCSend<202073965>(pPeer, pBuf);
}

