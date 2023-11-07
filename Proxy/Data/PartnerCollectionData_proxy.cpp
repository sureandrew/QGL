//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/PartnerCollectionData_proxy.h"

void PartnerCollectionData_Proxy::SetKey(const UInt32 &collection_uid)
{
	bool modified = false;
	if (m.collection_uid != collection_uid) { m.collection_uid = collection_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void PartnerCollectionData_Proxy::SetCollection_uid(const UInt32 &collection_uid)
{
	bool modified = false;
	if (m.collection_uid != collection_uid) { m.collection_uid = collection_uid; modified = true; }
	MarkUpdateAll(keyMask, modified);
}

void PartnerCollectionData_Proxy::SetInfo(const UInt32 &collection_id, const UInt32 &char_id)
{
	bool modified = false;
	if (m.collection_id != collection_id) { m.collection_id = collection_id; modified = true; }
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void PartnerCollectionData_Proxy::SetCollection_id(const UInt32 &collection_id)
{
	bool modified = false;
	if (m.collection_id != collection_id) { m.collection_id = collection_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void PartnerCollectionData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}


void PartnerCollectionData_Proxy::ReadAttr(CONST Buffer &buf)
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

void PartnerCollectionData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

void PartnerCollectionData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(keyMask)) OnUpdateKey(m.collection_uid);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.collection_id, m.char_id);
}

#pragma optimize("", off)
RPCResult PartnerCollectionData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PartnerCollectionData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult PartnerCollectionData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_PartnerCollectionData_UP_STATE:
			_result = RPCDecode<1650458909>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
