//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Account_stub.h"

void Account_Stub::SetInfo(const UInt32 &acct_id, const String &loginName)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.loginName != loginName) { m.loginName = loginName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Account_Stub::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Account_Stub::SetLoginName(const String &loginName)
{
	bool modified = false;
	if (m.loginName != loginName) { m.loginName = loginName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void Account_Stub::SetAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime)
{
	bool modified = false;
	if (m.disconnect_time != disconnect_time) { m.disconnect_time = disconnect_time; modified = true; }
	if (m.offLnTime != offLnTime) { m.offLnTime = offLnTime; modified = true; }
	if (m.onLnTime != onLnTime) { m.onLnTime = onLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void Account_Stub::SetDisconnect_time(const UInt32 &disconnect_time)
{
	bool modified = false;
	if (m.disconnect_time != disconnect_time) { m.disconnect_time = disconnect_time; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void Account_Stub::SetOffLnTime(const UInt32 &offLnTime)
{
	bool modified = false;
	if (m.offLnTime != offLnTime) { m.offLnTime = offLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void Account_Stub::SetOnLnTime(const UInt32 &onLnTime)
{
	bool modified = false;
	if (m.onLnTime != onLnTime) { m.onLnTime = onLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void Account_Stub::SetBanTime(const UInt32 &banTime)
{
	bool modified = false;
	if (m.banTime != banTime) { m.banTime = banTime; modified = true; }
	MarkUpdateAll(banTimeMask, modified);
}

void Account_Stub::SetAddictType(const Boolean &addictType)
{
	bool modified = false;
	if (m.addictType != addictType) { m.addictType = addictType; modified = true; }
	MarkUpdateAll(addictTypeMask, modified);
}



void Account_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Account_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt32>(m.acct_id);
		buf.Write<String>(m.loginName);
	}
	if (mask.IsBit(addictMask))
	{
		buf.Write<UInt32>(m.disconnect_time);
		buf.Write<UInt32>(m.offLnTime);
		buf.Write<UInt32>(m.onLnTime);
	}
	if (mask.IsBit(banTimeMask))
	{
		buf.Write<UInt32>(m.banTime);
	}
	if (mask.IsBit(addictTypeMask))
	{
		buf.Write<Boolean>(m.addictType);
	}
}

RPCResult Account_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Account_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Account_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Account_GetOwnCharacters:
		{
			_result = GetOwnCharacters(pPeer, context);
		}
		break;
	case RPC_Account_CheckCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = CheckCharacter(pPeer, context, char_id);
		}
		break;
	case RPC_Account_CheckCharacterName:
		{
			String nickName;
			pBuf->Read<String>(nickName);
			_result = CheckCharacterName(pPeer, context, nickName);
		}
		break;
	case RPC_Account_SelectCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			UInt8 line_id;
			pBuf->Read<UInt8>(line_id);
			_result = SelectCharacter(pPeer, context, char_id, line_id);
		}
		break;
	case RPC_Account_Logout:
		{
			_result = Logout(pPeer, context);
		}
		break;
	case RPC_Account_AddCharacter:
		{
			String nickName;
			pBuf->Read<String>(nickName);
			UInt8 sex;
			pBuf->Read<UInt8>(sex);
			UInt8 cclass;
			pBuf->Read<UInt8>(cclass);
			UInt16 hairStyle;
			pBuf->Read<UInt16>(hairStyle);
			UInt16 hairColor;
			pBuf->Read<UInt16>(hairColor);
			UInt16 clothStyle;
			pBuf->Read<UInt16>(clothStyle);
			UInt16 clothColor;
			pBuf->Read<UInt16>(clothColor);
			_result = AddCharacter(pPeer, context, nickName, sex, cclass, hairStyle, hairColor, clothStyle, clothColor);
		}
		break;
	case RPC_Account_RemoveCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RemoveCharacter(pPeer, context, char_id);
		}
		break;
	case RPC_Account_FreezeCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = FreezeCharacter(pPeer, context, char_id);
		}
		break;
	case RPC_Account_RecoverCharacter:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = RecoverCharacter(pPeer, context, char_id);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Account_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<1474721487>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &status, const Int32Vector &lineCounts, const UInt16Vector &lineState)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_CheckCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(status);
	pBuf->Write<Int32Vector>(lineCounts);
	pBuf->Write<UInt16Vector>(lineState);
	return RPCSend<1339323336>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_GetOwnCharacters(LPCPEER pPeer, RPCContext &context, NetGroup* charInfos)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_GetOwnCharacters);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(charInfos);
	return RPCSend<310063790>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_SelectCharacter(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_SelectCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1728582379>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_LogoutSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_LogoutSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1944146759>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_CreateSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_CreateSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1890454112>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_RemoveCharSuccess(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_RemoveCharSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<417215886>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_CheckCharacterName(LPCPEER pPeer, RPCContext &context, const UInt8 &index)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_CheckCharacterName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(index);
	return RPCSend<1423646894>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_RecoverCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_RecoverCharSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(charInfo);
	return RPCSend<237001887>(pPeer, pBuf);
}

RPCResult Account_Stub::CB_FreezeCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CB_FreezeCharSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(charInfo);
	return RPCSend<1536905089>(pPeer, pBuf);
}

