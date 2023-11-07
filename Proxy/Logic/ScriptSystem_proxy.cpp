//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/ScriptSystem_proxy.h"


void ScriptSystem_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void ScriptSystem_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void ScriptSystem_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult ScriptSystem_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ScriptSystem_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_ScriptSystem_CB_TalkEnd:
		{
			Boolean bWait;
			pBuf->Read<Boolean>(bWait);
			_result = CB_TalkEnd(pPeer, context, bWait);
		}
		break;
	case RPC_ScriptSystem_CB_TalkNext:
		{
			_result = CB_TalkNext(pPeer, context);
		}
		break;
	case RPC_ScriptSystem_CB_Choose:
		{
			UInt8 askerType;
			pBuf->Read<UInt8>(askerType);
			UInt32 askerId;
			pBuf->Read<UInt32>(askerId);
			UInt8 total;
			pBuf->Read<UInt8>(total);
			Boolean waitOther;
			pBuf->Read<Boolean>(waitOther);
			UInt16 timeout;
			pBuf->Read<UInt16>(timeout);
			StringVector msgList;
			pBuf->Read<StringVector>(msgList);
			_result = CB_Choose(pPeer, context, askerType, askerId, total, waitOther, timeout, msgList);
		}
		break;
	case RPC_ScriptSystem_CB_Say:
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
	case RPC_ScriptSystem_CB_OpenInterface:
		{
			StringA menu;
			pBuf->Read<StringA>(menu);
			Int32 param1;
			pBuf->Read<Int32>(param1);
			_result = CB_OpenInterface(pPeer, context, menu, param1);
		}
		break;
	case RPC_ScriptSystem_CB_OpenShopMenu:
		{
			UInt32 shopId;
			pBuf->Read<UInt32>(shopId);
			UInt8 title;
			pBuf->Read<UInt8>(title);
			UInt8 enable;
			pBuf->Read<UInt8>(enable);
			UInt16 buyDc;
			pBuf->Read<UInt16>(buyDc);
			UInt16 sellDc;
			pBuf->Read<UInt16>(sellDc);
			UInt16 repairDc;
			pBuf->Read<UInt16>(repairDc);
			UInt16 uiShopType;
			pBuf->Read<UInt16>(uiShopType);
			UInt32Vector itemLst;
			pBuf->Read<UInt32Vector>(itemLst);
			_result = CB_OpenShopMenu(pPeer, context, shopId, title, enable, buyDc, sellDc, repairDc, uiShopType, itemLst);
		}
		break;
	case RPC_ScriptSystem_CB_ShowHint:
		{
			String hint;
			pBuf->Read<String>(hint);
			_result = CB_ShowHint(pPeer, context, hint);
		}
		break;
	case RPC_ScriptSystem_CB_OnPlaySound:
		{
			StringA Name;
			pBuf->Read<StringA>(Name);
			_result = CB_OnPlaySound(pPeer, context, Name);
		}
		break;
	case RPC_ScriptSystem_CB_AskGiven:
		{
			String msg;
			pBuf->Read<String>(msg);
			UInt32 npc_id;
			pBuf->Read<UInt32>(npc_id);
			UInt8Vector gtypes;
			pBuf->Read<UInt8Vector>(gtypes);
			UInt32Vector gids;
			pBuf->Read<UInt32Vector>(gids);
			_result = CB_AskGiven(pPeer, context, msg, npc_id, gtypes, gids);
		}
		break;
	case RPC_ScriptSystem_CB_GivenFinish:
		{
			_result = CB_GivenFinish(pPeer, context);
		}
		break;
	case RPC_ScriptSystem_CB_AskString:
		{
			String text;
			pBuf->Read<String>(text);
			String defAns;
			pBuf->Read<String>(defAns);
			Boolean bLeft;
			pBuf->Read<Boolean>(bLeft);
			UInt8 askerType;
			pBuf->Read<UInt8>(askerType);
			UInt32 askerId;
			pBuf->Read<UInt32>(askerId);
			_result = CB_AskString(pPeer, context, text, defAns, bLeft, askerType, askerId);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult ScriptSystem_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_ScriptSystem_UP_STATE:
			_result = RPCDecode<271208504>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_TalkEnd:
			_result = RPCDecode<1665618048>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_TalkNext:
			_result = RPCDecode<64504023>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_Choose:
			_result = RPCDecode<687694366>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_Say:
			_result = RPCDecode<913659639>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_OpenInterface:
			_result = RPCDecode<79063645>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_OpenShopMenu:
			_result = RPCDecode<131608988>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_ShowHint:
			_result = RPCDecode<185803571>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_OnPlaySound:
			_result = RPCDecode<1454594331>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_AskGiven:
			_result = RPCDecode<518091936>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_GivenFinish:
			_result = RPCDecode<439170370>(pPeer, pBuf);
		break;
	case RPC_ScriptSystem_CB_AskString:
			_result = RPCDecode<480398226>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
