//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/TitleData_proxy.h"

void TitleData_Proxy::SetKey(const UInt32 &title_uid)
{
	bool modified = false;
	if (m.title_uid != title_uid) { m.title_uid = title_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void TitleData_Proxy::SetTitle_uid(const UInt32 &title_uid)
{
	bool modified = false;
	if (m.title_uid != title_uid) { m.title_uid = title_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void TitleData_Proxy::SetInfo(const UInt16 &title_id)
{
	bool modified = false;
	if (m.title_id != title_id) { m.title_id = title_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void TitleData_Proxy::SetTitle_id(const UInt16 &title_id)
{
	bool modified = false;
	if (m.title_id != title_id) { m.title_id = title_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void TitleData_Proxy::SetOwner(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void TitleData_Proxy::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void TitleData_Proxy::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(expireDateMask, modified);
}


void TitleData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(keyMask))
	{
		buf.Read<UInt32>(m.title_uid);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt16>(m.title_id);
	}
	if (m_updateMask.IsBit(ownerMask))
	{
		buf.Read<UInt32>(m.owner_id);
	}
	if (m_updateMask.IsBit(expireDateMask))
	{
		buf.Read<UInt32>(m.expireDate);
	}
}

void TitleData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(keyMask))
	{
		buf.Write<UInt32>(m.title_uid);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt16>(m.title_id);
	}
	if (mask.IsBit(ownerMask))
	{
		buf.Write<UInt32>(m.owner_id);
	}
	if (mask.IsBit(expireDateMask))
	{
		buf.Write<UInt32>(m.expireDate);
	}
}

void TitleData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(keyMask)) OnUpdateKey(m.title_uid);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.title_id);
	if (IsUpdated(ownerMask)) OnUpdateOwner(m.owner_id);
	if (IsUpdated(expireDateMask)) OnUpdateExpireDate(m.expireDate);
}

#pragma optimize("", off)
RPCResult TitleData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_TitleData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult TitleData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_TitleData_UP_STATE:
			_result = RPCDecode<432293902>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
