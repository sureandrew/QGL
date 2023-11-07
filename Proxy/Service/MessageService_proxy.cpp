//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/MessageService_proxy.h"

#pragma optimize("", off)
RPCResult MessageService_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_MessageService_CB_BattleEndUpdate:
		{
			_result = CB_BattleEndUpdate(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_CB_GetAvailService:
		{
			ServiceID serviceId;
			pBuf->Read<ServiceID>(serviceId);
			_result = CB_GetAvailService(pPeer, context, serviceId);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_CB_ChangeMsgIcon:
		{
			Boolean canChange;
			pBuf->Read<Boolean>(canChange);
			_result = CB_ChangeMsgIcon(pPeer, context, canChange);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_CB_RemoveMessengersByOwner:
		{
			_result = CB_RemoveMessengersByOwner(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_CB_CreateGuild:
		{
			UInt8 bResult;
			pBuf->Read<UInt8>(bResult);
			_result = CB_CreateGuild(pPeer, context, bResult);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MessageService_CB_DonateMoney:
		{
			UInt32 addedGDonate;
			pBuf->Read<UInt32>(addedGDonate);
			UInt8 returnFlag;
			pBuf->Read<UInt8>(returnFlag);
			UInt32 addGMoney;
			pBuf->Read<UInt32>(addGMoney);
			_result = CB_DonateMoney(pPeer, context, addedGDonate, returnFlag, addGMoney);
		}
		break;
#endif // CLIENT
	case RPC_MessageService_CB_Error:
		{
			UInt32 err;
			pBuf->Read<UInt32>(err);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, err, errMsg);
		}
		break;
	case RPC_MessageService_CB_InitMessageChannel:
		{
			BYTE Result;
			pBuf->Read<BYTE>(Result);
			_result = CB_InitMessageChannel(pPeer, context, Result);
		}
		break;
	case RPC_MessageService_CB_ReceiveMessageChannel:
		{
			NetObject* channel;
			pBuf->Read<NetObject*>(channel);
			BYTE Type;
			pBuf->Read<BYTE>(Type);
			_result = CB_ReceiveMessageChannel(pPeer, context, channel, Type);
		}
		break;
	case RPC_MessageService_CB_SendPrivateMessage:
		{
			BYTE Result;
			pBuf->Read<BYTE>(Result);
			boolean isFriend;
			pBuf->Read<boolean>(isFriend);
			UInt32 resultFlag;
			pBuf->Read<UInt32>(resultFlag);
			_result = CB_SendPrivateMessage(pPeer, context, Result, isFriend, resultFlag);
		}
		break;
	case RPC_MessageService_CB_ReceiveFriendMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			String SrcName;
			pBuf->Read<String>(SrcName);
			_result = CB_ReceiveFriendMessage(pPeer, context, Msg, SrcName);
		}
		break;
	case RPC_MessageService_CB_SendHorseLightMessage:
		{
			BYTE Result;
			pBuf->Read<BYTE>(Result);
			_result = CB_SendHorseLightMessage(pPeer, context, Result);
		}
		break;
	case RPC_MessageService_CB_SendSystemMessage:
		{
			BYTE Result;
			pBuf->Read<BYTE>(Result);
			_result = CB_SendSystemMessage(pPeer, context, Result);
		}
		break;
	case RPC_MessageService_CB_SendRumorMessage:
		{
			BYTE Result;
			pBuf->Read<BYTE>(Result);
			_result = CB_SendRumorMessage(pPeer, context, Result);
		}
		break;
	case RPC_MessageService_CB_SendShutdownMessage:
		{
			UInt32 downTime;
			pBuf->Read<UInt32>(downTime);
			_result = CB_SendShutdownMessage(pPeer, context, downTime);
		}
		break;
	case RPC_MessageService_CB_ReceivePrivateMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			String SrcName;
			pBuf->Read<String>(SrcName);
			UInt32 SrcChar_id;
			pBuf->Read<UInt32>(SrcChar_id);
			String TargetName;
			pBuf->Read<String>(TargetName);
			UInt32 TargetID;
			pBuf->Read<UInt32>(TargetID);
			boolean isFriend;
			pBuf->Read<boolean>(isFriend);
			_result = CB_ReceivePrivateMessage(pPeer, context, Msg, SrcName, SrcChar_id, TargetName, TargetID, isFriend);
		}
		break;
	case RPC_MessageService_CB_ReceiveHorseLightMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			UInt32 count;
			pBuf->Read<UInt32>(count);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);
			_result = CB_ReceiveHorseLightMessage(pPeer, context, Msg, count, listeners);
		}
		break;
	case RPC_MessageService_CB_ReceiveSystemMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);
			_result = CB_ReceiveSystemMessage(pPeer, context, Msg, listeners);
		}
		break;
	case RPC_MessageService_CB_ReceiveRumorMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			BYTE SubType;
			pBuf->Read<BYTE>(SubType);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);
			_result = CB_ReceiveRumorMessage(pPeer, context, Msg, SubType, listeners);
		}
		break;
	case RPC_MessageService_CB_SetListen:
		{
			_result = CB_SetListen(pPeer, context);
		}
		break;
	case RPC_MessageService_CB_JoinMessageChannel:
		{
			BYTE Result;
			pBuf->Read<BYTE>(Result);
			_result = CB_JoinMessageChannel(pPeer, context, Result);
		}
		break;
	case RPC_MessageService_CB_UpdateMessengers:
		{
			NetGroup* messengers;
			pBuf->Read<NetGroup*>(messengers);
			_result = CB_UpdateMessengers(pPeer, context, messengers);
		}
		break;
	case RPC_MessageService_CB_UpdateMsgCharInfo:
		{
			NetGroup* magCharInfoGrp;
			pBuf->Read<NetGroup*>(magCharInfoGrp);
			_result = CB_UpdateMsgCharInfo(pPeer, context, magCharInfoGrp);
		}
		break;
	case RPC_MessageService_CB_RemoveMessengers:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32 target_id;
			pBuf->Read<UInt32>(target_id);
			_result = CB_RemoveMessengers(pPeer, context, owner_id, target_id);
		}
		break;
	case RPC_MessageService_CB_RequestInfo:
		{
			NetObject* msgCharData;
			pBuf->Read<NetObject*>(msgCharData);
			bool update;
			pBuf->Read<bool>(update);
			_result = CB_RequestInfo(pPeer, context, msgCharData, update);
		}
		break;
	case RPC_MessageService_CB_MessageReturn:
		{
			String msg;
			pBuf->Read<String>(msg);
			String name;
			pBuf->Read<String>(name);
			_result = CB_MessageReturn(pPeer, context, msg, name);
		}
		break;
	case RPC_MessageService_CB_GetMessengers:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			UInt32Vector messgers;
			pBuf->Read<UInt32Vector>(messgers);
			_result = CB_GetMessengers(pPeer, context, owner_id, messgers);
		}
		break;
	case RPC_MessageService_CB_GetMessengersName:
		{
			UInt32 owner_id;
			pBuf->Read<UInt32>(owner_id);
			StringVector messgers_name;
			pBuf->Read<StringVector>(messgers_name);
			_result = CB_GetMessengersName(pPeer, context, owner_id, messgers_name);
		}
		break;
	case RPC_MessageService_CB_GetFriendly:
		{
			UInt32 friendly;
			pBuf->Read<UInt32>(friendly);
			_result = CB_GetFriendly(pPeer, context, friendly);
		}
		break;
	case RPC_MessageService_CB_IsFriend:
		{
			UInt32 isFriend;
			pBuf->Read<UInt32>(isFriend);
			_result = CB_IsFriend(pPeer, context, isFriend);
		}
		break;
	case RPC_MessageService_CB_ChangeFriendly:
		{
			_result = CB_ChangeFriendly(pPeer, context);
		}
		break;
	case RPC_MessageService_CB_ApplyGuild:
		{
			UInt32 guild_uid;
			pBuf->Read<UInt32>(guild_uid);
			_result = CB_ApplyGuild(pPeer, context, guild_uid);
		}
		break;
	case RPC_MessageService_CB_Guilds:
		{
			NetGroup* GuildGrp;
			pBuf->Read<NetGroup*>(GuildGrp);
			bool openApply;
			pBuf->Read<bool>(openApply);
			_result = CB_Guilds(pPeer, context, GuildGrp, openApply);
		}
		break;
	case RPC_MessageService_CB_GuildMembers:
		{
			NetGroup* GuildMemberGrp;
			pBuf->Read<NetGroup*>(GuildMemberGrp);
			_result = CB_GuildMembers(pPeer, context, GuildMemberGrp);
		}
		break;
	case RPC_MessageService_CB_RequestApplicant:
		{
			NetGroup* pGrp;
			pBuf->Read<NetGroup*>(pGrp);
			_result = CB_RequestApplicant(pPeer, context, pGrp);
		}
		break;
	case RPC_MessageService_CB_CreateGuildMsg:
		{
			String guildName;
			pBuf->Read<String>(guildName);
			String creator;
			pBuf->Read<String>(creator);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);
			_result = CB_CreateGuildMsg(pPeer, context, guildName, creator, listeners);
		}
		break;
	case RPC_MessageService_CB_GetTargetFromHawk:
		{
			bool isonline;
			pBuf->Read<bool>(isonline);
			String char_name;
			pBuf->Read<String>(char_name);
			_result = CB_GetTargetFromHawk(pPeer, context, isonline, char_name);
		}
		break;
	case RPC_MessageService_CB_BuyItemFromBusinessShop:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			UInt16 finalCount;
			pBuf->Read<UInt16>(finalCount);
			UInt32 finalPrice;
			pBuf->Read<UInt32>(finalPrice);
			_result = CB_BuyItemFromBusinessShop(pPeer, context, success, finalCount, finalPrice);
		}
		break;
	case RPC_MessageService_CB_SellItemToBusinessShop:
		{
			Boolean success;
			pBuf->Read<Boolean>(success);
			UInt32 finalPrice;
			pBuf->Read<UInt32>(finalPrice);
			_result = CB_SellItemToBusinessShop(pPeer, context, success, finalPrice);
		}
		break;
	case RPC_MessageService_CB_OpenBusinessShopMenu:
		{
			Boolean sell;
			pBuf->Read<Boolean>(sell);
			UInt16 shop_id;
			pBuf->Read<UInt16>(shop_id);
			UInt8 shopName;
			pBuf->Read<UInt8>(shopName);
			UInt32 SynTime;
			pBuf->Read<UInt32>(SynTime);
			UInt32 BusinessMoney;
			pBuf->Read<UInt32>(BusinessMoney);
			BusinessShopItemInfoVector ItemInfoList;
			pBuf->Read<BusinessShopItemInfoVector>(ItemInfoList);
			_result = CB_OpenBusinessShopMenu(pPeer, context, sell, shop_id, shopName, SynTime, BusinessMoney, ItemInfoList);
		}
		break;
	case RPC_MessageService_CB_UpdateBusinessShopList:
		{
			BusinessShopItemInfoVector ItemInfoList;
			pBuf->Read<BusinessShopItemInfoVector>(ItemInfoList);
			_result = CB_UpdateBusinessShopList(pPeer, context, ItemInfoList);
		}
		break;
	case RPC_MessageService_CB_GetTargetInfo:
		{
			NetObject* msgTargetData;
			pBuf->Read<NetObject*>(msgTargetData);
			_result = CB_GetTargetInfo(pPeer, context, msgTargetData);
		}
		break;
	case RPC_MessageService_CB_ShowPublicItemDetailFromOtherLine:
		{
			CItem item;
			pBuf->Read<CItem>(item);
			_result = CB_ShowPublicItemDetailFromOtherLine(pPeer, context, item);
		}
		break;
	case RPC_MessageService_CB_ShowPublicPartnerDetailFromOtherLine:
		{
			CPartner partner;
			pBuf->Read<CPartner>(partner);
			UInt16Vector skillIDs;
			pBuf->Read<UInt16Vector>(skillIDs);
			_result = CB_ShowPublicPartnerDetailFromOtherLine(pPeer, context, partner, skillIDs);
		}
		break;
	case RPC_MessageService_CB_ShowPublicPetDetailFromOtherLine:
		{
			CPet pet;
			pBuf->Read<CPet>(pet);
			UInt16Vector skillIDs;
			pBuf->Read<UInt16Vector>(skillIDs);
			_result = CB_ShowPublicPetDetailFromOtherLine(pPeer, context, pet, skillIDs);
		}
		break;
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}

RPCResult MessageService_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_MessageService_CB_Error:
			_result = RPCDecode<1330994912>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_InitMessageChannel:
			_result = RPCDecode<1627015832>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ReceiveMessageChannel:
			_result = RPCDecode<1478577585>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_SendPrivateMessage:
			_result = RPCDecode<2136552202>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ReceiveFriendMessage:
			_result = RPCDecode<1000022068>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_SendHorseLightMessage:
			_result = RPCDecode<1771708645>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_SendSystemMessage:
			_result = RPCDecode<225803454>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_SendRumorMessage:
			_result = RPCDecode<850679110>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_SendShutdownMessage:
			_result = RPCDecode<2019961070>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ReceivePrivateMessage:
			_result = RPCDecode<608457820>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ReceiveHorseLightMessage:
			_result = RPCDecode<1076901719>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ReceiveSystemMessage:
			_result = RPCDecode<1718880279>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ReceiveRumorMessage:
			_result = RPCDecode<302327494>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_SetListen:
			_result = RPCDecode<1326528604>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_JoinMessageChannel:
			_result = RPCDecode<595267595>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_UpdateMessengers:
			_result = RPCDecode<1042965161>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_UpdateMsgCharInfo:
			_result = RPCDecode<698774636>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_RemoveMessengers:
			_result = RPCDecode<1832067147>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_RequestInfo:
			_result = RPCDecode<1752050440>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_MessageReturn:
			_result = RPCDecode<2020163493>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_GetMessengers:
			_result = RPCDecode<333594757>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_GetMessengersName:
			_result = RPCDecode<689261880>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_GetFriendly:
			_result = RPCDecode<963340648>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_IsFriend:
			_result = RPCDecode<173611235>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ChangeFriendly:
			_result = RPCDecode<1097009387>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ApplyGuild:
			_result = RPCDecode<286279200>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_Guilds:
			_result = RPCDecode<1080500677>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_GuildMembers:
			_result = RPCDecode<1004997677>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_RequestApplicant:
			_result = RPCDecode<1398277389>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_CreateGuildMsg:
			_result = RPCDecode<769019868>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_GetTargetFromHawk:
			_result = RPCDecode<355163471>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_BuyItemFromBusinessShop:
			_result = RPCDecode<579892677>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_SellItemToBusinessShop:
			_result = RPCDecode<1460503>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_OpenBusinessShopMenu:
			_result = RPCDecode<313945116>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_UpdateBusinessShopList:
			_result = RPCDecode<677403017>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_GetTargetInfo:
			_result = RPCDecode<1879651114>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ShowPublicItemDetailFromOtherLine:
			_result = RPCDecode<541660631>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ShowPublicPartnerDetailFromOtherLine:
			_result = RPCDecode<409940819>(pPeer, pBuf);
		break;
	case RPC_MessageService_CB_ShowPublicPetDetailFromOtherLine:
			_result = RPCDecode<202073965>(pPeer, pBuf);
		break;
	}
	return _result;
}
#pragma optimize("", on)

