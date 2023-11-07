//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Account_proxy.h"


void Account_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt32>(m.acct_id);
		buf.Read<String>(m.loginName);
	}
	if (m_updateMask.IsBit(addictMask))
	{
		buf.Read<UInt32>(m.disconnect_time);
		buf.Read<UInt32>(m.offLnTime);
		buf.Read<UInt32>(m.onLnTime);
	}
	if (m_updateMask.IsBit(banTimeMask))
	{
		buf.Read<UInt32>(m.banTime);
	}
	if (m_updateMask.IsBit(addictTypeMask))
	{
		buf.Read<Boolean>(m.addictType);
	}
}

void Account_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Account_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.acct_id, m.loginName);
	if (IsUpdated(addictMask)) OnUpdateAddict(m.disconnect_time, m.offLnTime, m.onLnTime);
	if (IsUpdated(banTimeMask)) OnUpdateBanTime(m.banTime);
	if (IsUpdated(addictTypeMask)) OnUpdateAddictType(m.addictType);
}

#pragma optimize("", off)
RPCResult Account_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Account_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Account_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_Account_CB_CheckCharacter:
		{
			UInt8 status;
			pBuf->Read<UInt8>(status);
			Int32Vector lineCounts;
			pBuf->Read<Int32Vector>(lineCounts);
			UInt16Vector lineState;
			pBuf->Read<UInt16Vector>(lineState);
			_result = CB_CheckCharacter(pPeer, context, status, lineCounts, lineState);
		}
		break;
	case RPC_Account_CB_GetOwnCharacters:
		{
			NetGroup* charInfos;
			pBuf->Read<NetGroup*>(charInfos);
			_result = CB_GetOwnCharacters(pPeer, context, charInfos);
		}
		break;
	case RPC_Account_CB_SelectCharacter:
		{
			_result = CB_SelectCharacter(pPeer, context);
		}
		break;
	case RPC_Account_CB_LogoutSuccess:
		{
			_result = CB_LogoutSuccess(pPeer, context);
		}
		break;
	case RPC_Account_CB_CreateSuccess:
		{
			_result = CB_CreateSuccess(pPeer, context);
		}
		break;
	case RPC_Account_CB_RemoveCharSuccess:
		{
			_result = CB_RemoveCharSuccess(pPeer, context);
		}
		break;
	case RPC_Account_CB_CheckCharacterName:
		{
			UInt8 index;
			pBuf->Read<UInt8>(index);
			_result = CB_CheckCharacterName(pPeer, context, index);
		}
		break;
	case RPC_Account_CB_RecoverCharSuccess:
		{
			NetObject* charInfo;
			pBuf->Read<NetObject*>(charInfo);
			_result = CB_RecoverCharSuccess(pPeer, context, charInfo);
		}
		break;
	case RPC_Account_CB_FreezeCharSuccess:
		{
			NetObject* charInfo;
			pBuf->Read<NetObject*>(charInfo);
			_result = CB_FreezeCharSuccess(pPeer, context, charInfo);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Account_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Account_UP_STATE:
			_result = RPCDecode<97800821>(pPeer, pBuf);
		break;
	case RPC_Account_CB_Error:
			_result = RPCDecode<1474721487>(pPeer, pBuf);
		break;
	case RPC_Account_CB_CheckCharacter:
			_result = RPCDecode<1339323336>(pPeer, pBuf);
		break;
	case RPC_Account_CB_GetOwnCharacters:
			_result = RPCDecode<310063790>(pPeer, pBuf);
		break;
	case RPC_Account_CB_SelectCharacter:
			_result = RPCDecode<1728582379>(pPeer, pBuf);
		break;
	case RPC_Account_CB_LogoutSuccess:
			_result = RPCDecode<1944146759>(pPeer, pBuf);
		break;
	case RPC_Account_CB_CreateSuccess:
			_result = RPCDecode<1890454112>(pPeer, pBuf);
		break;
	case RPC_Account_CB_RemoveCharSuccess:
			_result = RPCDecode<417215886>(pPeer, pBuf);
		break;
	case RPC_Account_CB_CheckCharacterName:
			_result = RPCDecode<1423646894>(pPeer, pBuf);
		break;
	case RPC_Account_CB_RecoverCharSuccess:
			_result = RPCDecode<237001887>(pPeer, pBuf);
		break;
	case RPC_Account_CB_FreezeCharSuccess:
			_result = RPCDecode<1536905089>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
