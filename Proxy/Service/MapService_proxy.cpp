//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/MapService_proxy.h"

#pragma optimize("", off)
RPCResult MapService_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
#ifndef CLIENT
	case RPC_MapService_CB_SyncProxy:
		{
			_result = CB_SyncProxy(pPeer, context);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_CB_ResumeSession:
		{
			Boolean valid;
			pBuf->Read<Boolean>(valid);
			_result = CB_ResumeSession(pPeer, context, valid);
		}
		break;
#endif // CLIENT
#ifndef CLIENT
	case RPC_MapService_CB_QueryInstance:
		{
			Boolean bExist;
			pBuf->Read<Boolean>(bExist);
			UInt16 allowCount;
			pBuf->Read<UInt16>(allowCount);
			_result = CB_QueryInstance(pPeer, context, bExist, allowCount);
		}
		break;
#endif // CLIENT
	case RPC_MapService_CB_Error:
		{
			UInt32 err;
			pBuf->Read<UInt32>(err);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, err, errMsg);
		}
		break;
	case RPC_MapService_CB_EnterSuccess:
		{
			NetObject* map;
			pBuf->Read<NetObject*>(map);
			NetObject* character;
			pBuf->Read<NetObject*>(character);
			_result = CB_EnterSuccess(pPeer, context, map, character);
		}
		break;
	case RPC_MapService_CB_CharacterStuff:
		{
			NetObject* bag;
			pBuf->Read<NetObject*>(bag);
			NetObject* scriptSys;
			pBuf->Read<NetObject*>(scriptSys);
			NetObject* questSys;
			pBuf->Read<NetObject*>(questSys);
			NetGroup* skillGroup;
			pBuf->Read<NetGroup*>(skillGroup);
			NetGroup* partnerGroup;
			pBuf->Read<NetGroup*>(partnerGroup);
			NetGroup* petGroup;
			pBuf->Read<NetGroup*>(petGroup);
			UInt8 addictHealth;
			pBuf->Read<UInt8>(addictHealth);
			_result = CB_CharacterStuff(pPeer, context, bag, scriptSys, questSys, skillGroup, partnerGroup, petGroup, addictHealth);
		}
		break;
	case RPC_MapService_CB_LeaveSuccess:
		{
			Boolean bDelete;
			pBuf->Read<Boolean>(bDelete);
			_result = CB_LeaveSuccess(pPeer, context, bDelete);
		}
		break;
	case RPC_MapService_CB_CharacterEnter:
		{
			_result = CB_CharacterEnter(pPeer, context);
		}
		break;
	case RPC_MapService_CB_PartyEnter:
		{
			_result = CB_PartyEnter(pPeer, context);
		}
		break;
	case RPC_MapService_CB_NpcEnter:
		{
			_result = CB_NpcEnter(pPeer, context);
		}
		break;
	case RPC_MapService_CB_EnterBattle:
		{
			NetObject* battle;
			pBuf->Read<NetObject*>(battle);
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			UInt8 PlayerTeam;
			pBuf->Read<UInt8>(PlayerTeam);
			UInt8 battleState;
			pBuf->Read<UInt8>(battleState);
			UInt32 npcId;
			pBuf->Read<UInt32>(npcId);
			_result = CB_EnterBattle(pPeer, context, battle, grp, PlayerTeam, battleState, npcId);
		}
		break;
#ifndef CLIENT
	case RPC_MapService_CB_CharacterCheat:
		{
			_result = CB_CharacterCheat(pPeer, context);
		}
		break;
#endif // CLIENT
	case RPC_MapService_CB_OpenChargeShop:
		{
			NetGroup* chargeShop;
			pBuf->Read<NetGroup*>(chargeShop);
			NetGroup* discountGroup;
			pBuf->Read<NetGroup*>(discountGroup);
			NetObject* limitedItem;
			pBuf->Read<NetObject*>(limitedItem);
			_result = CB_OpenChargeShop(pPeer, context, chargeShop, discountGroup, limitedItem);
		}
		break;
	case RPC_MapService_CB_SyncCharacter:
		{
			_result = CB_SyncCharacter(pPeer, context);
		}
		break;
	case RPC_MapService_CB_SyncItem:
		{
			_result = CB_SyncItem(pPeer, context);
		}
		break;
	case RPC_MapService_CB_SyncPartner:
		{
			_result = CB_SyncPartner(pPeer, context);
		}
		break;
	case RPC_MapService_CB_AddPartnersSuccess:
		{
			_result = CB_AddPartnersSuccess(pPeer, context);
		}
		break;
	case RPC_MapService_CB_ShowPublicItemDetail:
		{
			NetObject* item;
			pBuf->Read<NetObject*>(item);
			_result = CB_ShowPublicItemDetail(pPeer, context, item);
		}
		break;
	case RPC_MapService_CB_ShowPublicPartnerDetail:
		{
			NetObject* Partner;
			pBuf->Read<NetObject*>(Partner);
			UInt16Vector skillIDs;
			pBuf->Read<UInt16Vector>(skillIDs);
			_result = CB_ShowPublicPartnerDetail(pPeer, context, Partner, skillIDs);
		}
		break;
	case RPC_MapService_CB_ShowPublicPetDetail:
		{
			NetObject* Pet;
			pBuf->Read<NetObject*>(Pet);
			UInt16Vector skillIDs;
			pBuf->Read<UInt16Vector>(skillIDs);
			_result = CB_ShowPublicPetDetail(pPeer, context, Pet, skillIDs);
		}
		break;
	case RPC_MapService_CB_ReceiveMapMessage:
		{
			String Msg;
			pBuf->Read<String>(Msg);
			String SrcName;
			pBuf->Read<String>(SrcName);
			SessionIDVector listeners;
			pBuf->Read<SessionIDVector>(listeners);
			_result = CB_ReceiveMapMessage(pPeer, context, Msg, SrcName, listeners);
		}
		break;
	case RPC_MapService_CB_ReceiveMapEmotion:
		{
			UInt32 emo_id;
			pBuf->Read<UInt32>(emo_id);
			String SrcName;
			pBuf->Read<String>(SrcName);
			String TargetName;
			pBuf->Read<String>(TargetName);
			bool isPet;
			pBuf->Read<bool>(isPet);
			_result = CB_ReceiveMapEmotion(pPeer, context, emo_id, SrcName, TargetName, isPet);
		}
		break;
	case RPC_MapService_CB_ForceLogout:
		{
			String forceOutMsg;
			pBuf->Read<String>(forceOutMsg);
			_result = CB_ForceLogout(pPeer, context, forceOutMsg);
		}
		break;
	case RPC_MapService_CB_GetPosition:
		{
			UInt32 mapX;
			pBuf->Read<UInt32>(mapX);
			UInt32 mapY;
			pBuf->Read<UInt32>(mapY);
			_result = CB_GetPosition(pPeer, context, mapX, mapY);
		}
		break;
	case RPC_MapService_CB_UpdateWantedReward:
		{
			Boolean result;
			pBuf->Read<Boolean>(result);
			_result = CB_UpdateWantedReward(pPeer, context, result);
		}
		break;
	case RPC_MapService_CB_ShowPublicPartnerDetailFromOtherLine:
		{
			NetObject* Partner;
			pBuf->Read<NetObject*>(Partner);
			UInt16Vector skillIDs;
			pBuf->Read<UInt16Vector>(skillIDs);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = CB_ShowPublicPartnerDetailFromOtherLine(pPeer, context, Partner, skillIDs, char_idTo);
		}
		break;
	case RPC_MapService_CB_ShowPublicPetDetailFromOtherLine:
		{
			NetObject* Pet;
			pBuf->Read<NetObject*>(Pet);
			UInt16Vector skillIDs;
			pBuf->Read<UInt16Vector>(skillIDs);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = CB_ShowPublicPetDetailFromOtherLine(pPeer, context, Pet, skillIDs, char_idTo);
		}
		break;
	case RPC_MapService_CB_ShowPublicItemDetailFromOtherLine:
		{
			NetObject* item;
			pBuf->Read<NetObject*>(item);
			UInt32 char_idTo;
			pBuf->Read<UInt32>(char_idTo);
			_result = CB_ShowPublicItemDetailFromOtherLine(pPeer, context, item, char_idTo);
		}
		break;
	default:
		_result = RPC_NOT_FOUND;
	}
	return _result;
}

RPCResult MapService_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_MapService_CB_Error:
			_result = RPCDecode<950751607>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_EnterSuccess:
			_result = RPCDecode<1732650242>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_CharacterStuff:
			_result = RPCDecode<1241854683>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_LeaveSuccess:
			_result = RPCDecode<1047143100>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_CharacterEnter:
			_result = RPCDecode<1698965209>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_PartyEnter:
			_result = RPCDecode<688273772>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_NpcEnter:
			_result = RPCDecode<17040657>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_EnterBattle:
			_result = RPCDecode<1376742086>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_OpenChargeShop:
			_result = RPCDecode<1218269948>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_SyncCharacter:
			_result = RPCDecode<1421242889>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_SyncItem:
			_result = RPCDecode<406863196>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_SyncPartner:
			_result = RPCDecode<987312133>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_AddPartnersSuccess:
			_result = RPCDecode<1631546235>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ShowPublicItemDetail:
			_result = RPCDecode<2122804530>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ShowPublicPartnerDetail:
			_result = RPCDecode<181617556>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ShowPublicPetDetail:
			_result = RPCDecode<1441406415>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ReceiveMapMessage:
			_result = RPCDecode<1763596625>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ReceiveMapEmotion:
			_result = RPCDecode<83128094>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ForceLogout:
			_result = RPCDecode<1131569201>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_GetPosition:
			_result = RPCDecode<1108702494>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_UpdateWantedReward:
			_result = RPCDecode<1292175556>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ShowPublicPartnerDetailFromOtherLine:
			_result = RPCDecode<509741308>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ShowPublicPetDetailFromOtherLine:
			_result = RPCDecode<336362802>(pPeer, pBuf);
		break;
	case RPC_MapService_CB_ShowPublicItemDetailFromOtherLine:
			_result = RPCDecode<1415328226>(pPeer, pBuf);
		break;
	}
	return _result;
}
#pragma optimize("", on)

