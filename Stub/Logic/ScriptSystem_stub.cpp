//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/ScriptSystem_stub.h"



void ScriptSystem_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void ScriptSystem_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

RPCResult ScriptSystem_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult ScriptSystem_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ScriptSystem_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_ScriptSystem_ReplyAnswer:
		{
			UInt8 answer;
			pBuf->Read<UInt8>(answer);
			_result = ReplyAnswer(pPeer, context, answer);
		}
		break;
	case RPC_ScriptSystem_ReplyAnswerString:
		{
			String answerStr;
			pBuf->Read<String>(answerStr);
			_result = ReplyAnswerString(pPeer, context, answerStr);
		}
		break;
	case RPC_ScriptSystem_ReplyGiven:
		{
			UInt8Vector gtypes;
			pBuf->Read<UInt8Vector>(gtypes);
			UInt32Vector uids;
			pBuf->Read<UInt32Vector>(uids);
			_result = ReplyGiven(pPeer, context, gtypes, uids);
		}
		break;
	case RPC_ScriptSystem_StopTalk:
		{
			Boolean bForce;
			pBuf->Read<Boolean>(bForce);
			_result = StopTalk(pPeer, context, bForce);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult ScriptSystem_Stub::CB_TalkEnd(LPCPEER pPeer, RPCContext &context, const Boolean &bWait)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_TalkEnd);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(bWait);
	return RPCSend<1665618048>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_TalkNext(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_TalkNext);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<64504023>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_Choose(LPCPEER pPeer, RPCContext &context, const UInt8 &askerType, const UInt32 &askerId, const UInt8 &total, const Boolean &waitOther, const UInt16 &timeout, const StringVector &msgList)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_Choose);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(askerType);
	pBuf->Write<UInt32>(askerId);
	pBuf->Write<UInt8>(total);
	pBuf->Write<Boolean>(waitOther);
	pBuf->Write<UInt16>(timeout);
	pBuf->Write<StringVector>(msgList);
	return RPCSend<687694366>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, const int &imageType, const Boolean &bLeft, const UInt32 &npc_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_Say);
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
	return RPCSend<913659639>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_OpenInterface(LPCPEER pPeer, RPCContext &context, const StringA &menu, const Int32 &param1)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_OpenInterface);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<StringA>(menu);
	pBuf->Write<Int32>(param1);
	return RPCSend<79063645>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_OpenShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &shopId, const UInt8 &title, const UInt8 &enable, const UInt16 &buyDc, const UInt16 &sellDc, const UInt16 &repairDc, const UInt16 &uiShopType, const UInt32Vector &itemLst)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_OpenShopMenu);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(shopId);
	pBuf->Write<UInt8>(title);
	pBuf->Write<UInt8>(enable);
	pBuf->Write<UInt16>(buyDc);
	pBuf->Write<UInt16>(sellDc);
	pBuf->Write<UInt16>(repairDc);
	pBuf->Write<UInt16>(uiShopType);
	pBuf->Write<UInt32Vector>(itemLst);
	return RPCSend<131608988>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_ShowHint(LPCPEER pPeer, RPCContext &context, const String &hint)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_ShowHint);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(hint);
	return RPCSend<185803571>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_OnPlaySound(LPCPEER pPeer, RPCContext &context, const StringA &Name)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_OnPlaySound);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<StringA>(Name);
	return RPCSend<1454594331>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_AskGiven(LPCPEER pPeer, RPCContext &context, const String &msg, const UInt32 &npc_id, const UInt8Vector &gtypes, const UInt32Vector &gids)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_AskGiven);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(msg);
	pBuf->Write<UInt32>(npc_id);
	pBuf->Write<UInt8Vector>(gtypes);
	pBuf->Write<UInt32Vector>(gids);
	return RPCSend<518091936>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_GivenFinish(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_GivenFinish);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<439170370>(pPeer, pBuf);
}

RPCResult ScriptSystem_Stub::CB_AskString(LPCPEER pPeer, RPCContext &context, const String &text, const String &defAns, const Boolean &bLeft, const UInt8 &askerType, const UInt32 &askerId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ScriptSystem_CB_AskString);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(text);
	pBuf->Write<String>(defAns);
	pBuf->Write<Boolean>(bLeft);
	pBuf->Write<UInt8>(askerType);
	pBuf->Write<UInt32>(askerId);
	return RPCSend<480398226>(pPeer, pBuf);
}

