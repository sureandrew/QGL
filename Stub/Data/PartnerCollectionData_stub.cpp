//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/PartnerCollectionData_stub.h"

void PartnerCollectionData_Stub::SetKey(const UInt32 &collection_uid)
{
	bool modified = false;
	if (m.collection_uid != collection_uid) { m.collection_uid = collection_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void PartnerCollectionData_Stub::SetCollection_uid(const UInt32 &collection_uid)
{
	bool modified = false;
	if (m.collection_uid != collection_uid) { m.collection_uid = collection_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void PartnerCollectionData_Stub::SetInfo(const UInt32 &collection_id, const UInt32 &char_id)
{
	bool modified = false;
	if (m.collection_id != collection_id) { m.collection_id = collection_id; modified = true; }
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void PartnerCollectionData_Stub::SetCollection_id(const UInt32 &collection_id)
{
	bool modified = false;
	if (m.collection_id != collection_id) { m.collection_id = collection_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void PartnerCollectionData_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}


void PartnerCollectionData_Stub::Clone(PartnerCollectionData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(keyMask) && !(obj->m.collection_uid == m.collection_uid)) {
		m.collection_uid = obj->m.collection_uid;
		MarkUpdateAll(keyMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.collection_id == m.collection_id)) {
		m.collection_id = obj->m.collection_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.char_id == m.char_id)) {
		m.char_id = obj->m.char_id;
		MarkUpdateAll(infoMask, true);
	}
}

void PartnerCollectionData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(keyMask))
	{
		buf.Read<UInt32>(m.collection_uid);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt32>(m.collection_id);
		buf.Read<UInt32>(m.char_id);
	}
}

void PartnerCollectionData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(keyMask))
	{
		buf.Write<UInt32>(m.collection_uid);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt32>(m.collection_id);
		buf.Write<UInt32>(m.char_id);
	}
}

RPCResult PartnerCollectionData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerCollectionData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult PartnerCollectionData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PartnerCollectionData_REQ_STATE:
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

