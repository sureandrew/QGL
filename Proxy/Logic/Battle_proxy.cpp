//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Battle_proxy.h"


void Battle_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(battleTypeMask))
	{
		buf.Read<UInt8>(m.battleType);
	}
}

void Battle_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Battle_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(battleTypeMask)) OnUpdateBattleType(m.battleType);
}

#pragma optimize("", off)
RPCResult Battle_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Battle_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Battle_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_Battle_CB_PlayTurn:
		{
			UInt8 turnNum;
			pBuf->Read<UInt8>(turnNum);
			ByteArray array;
			pBuf->Read<ByteArray>(array);
			UInt32 totalSize;
			pBuf->Read<UInt32>(totalSize);
			_result = CB_PlayTurn(pPeer, context, turnNum, array, totalSize);
		}
		break;
	case RPC_Battle_CB_PlayTurnMore:
		{
			ByteArray array;
			pBuf->Read<ByteArray>(array);
			UInt32 totalSize;
			pBuf->Read<UInt32>(totalSize);
			_result = CB_PlayTurnMore(pPeer, context, array, totalSize);
		}
		break;
	case RPC_Battle_CB_NextTurn:
		{
			UInt8 turnNum;
			pBuf->Read<UInt8>(turnNum);
			UInt32 turnTime;
			pBuf->Read<UInt32>(turnTime);
			UInt16 autoCount;
			pBuf->Read<UInt16>(autoCount);
			Boolean bInput;
			pBuf->Read<Boolean>(bInput);
			Boolean bPartnerInput;
			pBuf->Read<Boolean>(bPartnerInput);
			_result = CB_NextTurn(pPeer, context, turnNum, turnTime, autoCount, bInput, bPartnerInput);
		}
		break;
	case RPC_Battle_CB_EndBattle:
		{
			UInt8 endState;
			pBuf->Read<UInt8>(endState);
			_result = CB_EndBattle(pPeer, context, endState);
		}
		break;
	case RPC_Battle_CB_AddActors:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			UInt8 Team;
			pBuf->Read<UInt8>(Team);
			_result = CB_AddActors(pPeer, context, grp, Team);
		}
		break;
	case RPC_Battle_CB_AddPartners:
		{
			NetGroup* grp;
			pBuf->Read<NetGroup*>(grp);
			UInt8 Team;
			pBuf->Read<UInt8>(Team);
			_result = CB_AddPartners(pPeer, context, grp, Team);
		}
		break;
	case RPC_Battle_CB_ControlCharacter:
		{
			UInt8 charIdx;
			pBuf->Read<UInt8>(charIdx);
			_result = CB_ControlCharacter(pPeer, context, charIdx);
		}
		break;
	case RPC_Battle_CB_ControlPartner:
		{
			UInt8 charIdx;
			pBuf->Read<UInt8>(charIdx);
			UInt8 mobIdx;
			pBuf->Read<UInt8>(mobIdx);
			_result = CB_ControlPartner(pPeer, context, charIdx, mobIdx);
		}
		break;
	case RPC_Battle_CB_Reward:
		{
			Boolean levelUp;
			pBuf->Read<Boolean>(levelUp);
			UInt32 exp;
			pBuf->Read<UInt32>(exp);
			UInt32Vector gotItems;
			pBuf->Read<UInt32Vector>(gotItems);
			UInt32Vector discardItems;
			pBuf->Read<UInt32Vector>(discardItems);
			UInt32 partnerExp;
			pBuf->Read<UInt32>(partnerExp);
			Boolean partnerLvlUp;
			pBuf->Read<Boolean>(partnerLvlUp);
			UInt32 boundsExp;
			pBuf->Read<UInt32>(boundsExp);
			UInt32 boundsPartnerExp;
			pBuf->Read<UInt32>(boundsPartnerExp);
			_result = CB_Reward(pPeer, context, levelUp, exp, gotItems, discardItems, partnerExp, partnerLvlUp, boundsExp, boundsPartnerExp);
		}
		break;
	case RPC_Battle_CB_Penalty:
		{
			UInt32 exp;
			pBuf->Read<UInt32>(exp);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			UInt32 honor;
			pBuf->Read<UInt32>(honor);
			_result = CB_Penalty(pPeer, context, exp, money, honor);
		}
		break;
	case RPC_Battle_CB_NeedHelp:
		{
			Boolean bNeed;
			pBuf->Read<Boolean>(bNeed);
			_result = CB_NeedHelp(pPeer, context, bNeed);
		}
		break;
	case RPC_Battle_CB_WaitTalk:
		{
			UInt8 turnNum;
			pBuf->Read<UInt8>(turnNum);
			UInt32 turnTime;
			pBuf->Read<UInt32>(turnTime);
			_result = CB_WaitTalk(pPeer, context, turnNum, turnTime);
		}
		break;
	case RPC_Battle_CB_Choose:
		{
			UInt8 askerType;
			pBuf->Read<UInt8>(askerType);
			UInt32 askerId;
			pBuf->Read<UInt32>(askerId);
			Boolean bLeft;
			pBuf->Read<Boolean>(bLeft);
			UInt8 total;
			pBuf->Read<UInt8>(total);
			StringVector msgList;
			pBuf->Read<StringVector>(msgList);
			_result = CB_Choose(pPeer, context, askerType, askerId, bLeft, total, msgList);
		}
		break;
	case RPC_Battle_CB_Say:
		{
			String msg;
			pBuf->Read<String>(msg);
			int imageType;
			pBuf->Read<int>(imageType);
			Boolean bLeft;
			pBuf->Read<Boolean>(bLeft);
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			_result = CB_Say(pPeer, context, msg, imageType, bLeft, npc_id);
		}
		break;
	case RPC_Battle_CB_CancelTalk:
		{
			_result = CB_CancelTalk(pPeer, context);
		}
		break;
	case RPC_Battle_CB_DisableCmd:
		{
			UInt32 blockCmd;
			pBuf->Read<UInt32>(blockCmd);
			_result = CB_DisableCmd(pPeer, context, blockCmd);
		}
		break;
	case RPC_Battle_CB_HideCmd:
		{
			UInt8 index;
			pBuf->Read<UInt8>(index);
			Boolean hide;
			pBuf->Read<Boolean>(hide);
			_result = CB_HideCmd(pPeer, context, index, hide);
		}
		break;
	case RPC_Battle_CB_MartialResult:
		{
			UInt8 result;
			pBuf->Read<UInt8>(result);
			UInt32 money;
			pBuf->Read<UInt32>(money);
			UInt8 point;
			pBuf->Read<UInt8>(point);
			_result = CB_MartialResult(pPeer, context, result, money, point);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Battle_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Battle_UP_STATE:
			_result = RPCDecode<336615913>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_Error:
			_result = RPCDecode<1746015519>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_PlayTurn:
			_result = RPCDecode<568265299>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_PlayTurnMore:
			_result = RPCDecode<1191783985>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_NextTurn:
			_result = RPCDecode<764877343>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_EndBattle:
			_result = RPCDecode<1872119112>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_AddActors:
			_result = RPCDecode<541138277>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_AddPartners:
			_result = RPCDecode<265845785>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_ControlCharacter:
			_result = RPCDecode<718021376>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_ControlPartner:
			_result = RPCDecode<331966810>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_Reward:
			_result = RPCDecode<559032103>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_Penalty:
			_result = RPCDecode<439100286>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_NeedHelp:
			_result = RPCDecode<1185026100>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_WaitTalk:
			_result = RPCDecode<245784539>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_Choose:
			_result = RPCDecode<397361136>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_Say:
			_result = RPCDecode<1684411447>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_CancelTalk:
			_result = RPCDecode<880754745>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_DisableCmd:
			_result = RPCDecode<1861091827>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_HideCmd:
			_result = RPCDecode<1201948691>(pPeer, pBuf);
		break;
	case RPC_Battle_CB_MartialResult:
			_result = RPCDecode<1053907274>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
