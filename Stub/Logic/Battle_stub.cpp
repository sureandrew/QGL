//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Battle_stub.h"

void Battle_Stub::SetBattleType(const UInt8 &battleType)
{
	bool modified = false;
	if (m.battleType != battleType) { m.battleType = battleType; modified = true; }
	MarkUpdateAll(battleTypeMask, modified);
}



void Battle_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Battle_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(battleTypeMask))
	{
		buf.Write<UInt8>(m.battleType);
	}
}

RPCResult Battle_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Battle_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Battle_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Battle_EndTurn:
		{
			_result = EndTurn(pPeer, context);
		}
		break;
	case RPC_Battle_NextTalk:
		{
			_result = NextTalk(pPeer, context);
		}
		break;
	case RPC_Battle_ChooseResult:
		{
			UInt8 ans;
			pBuf->Read<UInt8>(ans);
			_result = ChooseResult(pPeer, context, ans);
		}
		break;
	case RPC_Battle_SendBattleMessage:
		{
			RPCNetID charNetID;
			pBuf->Read<RPCNetID>(charNetID);
			String Msg;
			pBuf->Read<String>(Msg);
			_result = SendBattleMessage(pPeer, context, charNetID, Msg);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Battle_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<1746015519>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_PlayTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const ByteArray &array, const UInt32 &totalSize)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_PlayTurn);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(turnNum);
	pBuf->Write<ByteArray>(array);
	pBuf->Write<UInt32>(totalSize);
	return RPCSend<568265299>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_PlayTurnMore(LPCPEER pPeer, RPCContext &context, const ByteArray &array, const UInt32 &totalSize)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_PlayTurnMore);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<ByteArray>(array);
	pBuf->Write<UInt32>(totalSize);
	return RPCSend<1191783985>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_NextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &turnTime, const UInt16 &autoCount, const Boolean &bInput, const Boolean &bPartnerInput)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_NextTurn);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(turnNum);
	pBuf->Write<UInt32>(turnTime);
	pBuf->Write<UInt16>(autoCount);
	pBuf->Write<Boolean>(bInput);
	pBuf->Write<Boolean>(bPartnerInput);
	return RPCSend<764877343>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_EndBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &endState)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_EndBattle);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(endState);
	return RPCSend<1872119112>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_AddActors(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &Team)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_AddActors);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<UInt8>(Team);
	return RPCSend<541138277>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_AddPartners(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &Team)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_AddPartners);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(grp);
	pBuf->Write<UInt8>(Team);
	return RPCSend<265845785>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_ControlCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_ControlCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(charIdx);
	return RPCSend<718021376>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_ControlPartner(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx, const UInt8 &mobIdx)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_ControlPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(charIdx);
	pBuf->Write<UInt8>(mobIdx);
	return RPCSend<331966810>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_Reward(LPCPEER pPeer, RPCContext &context, const Boolean &levelUp, const UInt32 &exp, const UInt32Vector &gotItems, const UInt32Vector &discardItems, const UInt32 &partnerExp, const Boolean &partnerLvlUp, const UInt32 &boundsExp, const UInt32 &boundsPartnerExp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_Reward);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(levelUp);
	pBuf->Write<UInt32>(exp);
	pBuf->Write<UInt32Vector>(gotItems);
	pBuf->Write<UInt32Vector>(discardItems);
	pBuf->Write<UInt32>(partnerExp);
	pBuf->Write<Boolean>(partnerLvlUp);
	pBuf->Write<UInt32>(boundsExp);
	pBuf->Write<UInt32>(boundsPartnerExp);
	return RPCSend<559032103>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_Penalty(LPCPEER pPeer, RPCContext &context, const UInt32 &exp, const UInt32 &money, const UInt32 &honor)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_Penalty);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(exp);
	pBuf->Write<UInt32>(money);
	pBuf->Write<UInt32>(honor);
	return RPCSend<439100286>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_NeedHelp(LPCPEER pPeer, RPCContext &context, const Boolean &bNeed)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_NeedHelp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(bNeed);
	return RPCSend<1185026100>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_WaitTalk(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &turnTime)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_WaitTalk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(turnNum);
	pBuf->Write<UInt32>(turnTime);
	return RPCSend<245784539>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_Choose(LPCPEER pPeer, RPCContext &context, const UInt8 &askerType, const UInt32 &askerId, const Boolean &bLeft, const UInt8 &total, const StringVector &msgList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_Choose);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(askerType);
	pBuf->Write<UInt32>(askerId);
	pBuf->Write<Boolean>(bLeft);
	pBuf->Write<UInt8>(total);
	pBuf->Write<StringVector>(msgList);
	return RPCSend<397361136>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, const int &imageType, const Boolean &bLeft, const UInt32 &npc_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_Say);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(msg);
	pBuf->Write<int>(imageType);
	pBuf->Write<Boolean>(bLeft);
	pBuf->Write<UInt32>(npc_id);
	return RPCSend<1684411447>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_CancelTalk(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_CancelTalk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<880754745>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_DisableCmd(LPCPEER pPeer, RPCContext &context, const UInt32 &blockCmd)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_DisableCmd);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(blockCmd);
	return RPCSend<1861091827>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_HideCmd(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const Boolean &hide)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_HideCmd);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(index);
	pBuf->Write<Boolean>(hide);
	return RPCSend<1201948691>(pPeer, pBuf);
}

RPCResult Battle_Stub::CB_MartialResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &money, const UInt8 &point)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Battle_CB_MartialResult);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(result);
	pBuf->Write<UInt32>(money);
	pBuf->Write<UInt8>(point);
	return RPCSend<1053907274>(pPeer, pBuf);
}

