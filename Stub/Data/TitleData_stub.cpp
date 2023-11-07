//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/TitleData_stub.h"

void TitleData_Stub::SetKey(const UInt32 &title_uid)
{
	bool modified = false;
	if (m.title_uid != title_uid) { m.title_uid = title_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void TitleData_Stub::SetTitle_uid(const UInt32 &title_uid)
{
	bool modified = false;
	if (m.title_uid != title_uid) { m.title_uid = title_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void TitleData_Stub::SetInfo(const UInt16 &title_id)
{
	bool modified = false;
	if (m.title_id != title_id) { m.title_id = title_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void TitleData_Stub::SetTitle_id(const UInt16 &title_id)
{
	bool modified = false;
	if (m.title_id != title_id) { m.title_id = title_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void TitleData_Stub::SetOwner(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void TitleData_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(ownerMask, modified);
}

void TitleData_Stub::SetExpireDate(const UInt32 &expireDate)
{
	bool modified = false;
	if (m.expireDate != expireDate) { m.expireDate = expireDate; modified = true; }
	MarkUpdateAll(expireDateMask, modified);
}


void TitleData_Stub::Clone(TitleData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(keyMask) && !(obj->m.title_uid == m.title_uid)) {
		m.title_uid = obj->m.title_uid;
		MarkUpdateAll(keyMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.title_id == m.title_id)) {
		m.title_id = obj->m.title_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(ownerMask) && !(obj->m.owner_id == m.owner_id)) {
		m.owner_id = obj->m.owner_id;
		MarkUpdateAll(ownerMask, true);
	}
	if (mask.IsBit(expireDateMask) && !(obj->m.expireDate == m.expireDate)) {
		m.expireDate = obj->m.expireDate;
		MarkUpdateAll(expireDateMask, true);
	}
}

void TitleData_Stub::ReadAttr(CONST Buffer &buf)
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

void TitleData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult TitleData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_TitleData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult TitleData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_TitleData_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

