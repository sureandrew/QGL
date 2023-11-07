//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/QuestFlagData_proxy.h"

void QuestFlagData_Proxy::SetId(const UInt32 &char_id, const UInt16 &flag_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.flag_id != flag_id) { m.flag_id = flag_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestFlagData_Proxy::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestFlagData_Proxy::SetFlag_id(const UInt16 &flag_id)
{
	bool modified = false;
	if (m.flag_id != flag_id) { m.flag_id = flag_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestFlagData_Proxy::SetFlag(const Int32 &flag)
{
	bool modified = false;
	if (m.flag != flag) { m.flag = flag; modified = true; }
	MarkUpdateAll(flagMask, modified);
}


void QuestFlagData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.char_id);
		buf.Read<UInt16>(m.flag_id);
	}
	if (m_updateMask.IsBit(flagMask))
	{
		buf.Read<Int32>(m.flag);
	}
}

void QuestFlagData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.char_id);
		buf.Write<UInt16>(m.flag_id);
	}
	if (mask.IsBit(flagMask))
	{
		buf.Write<Int32>(m.flag);
	}
}

void QuestFlagData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.char_id, m.flag_id);
	if (IsUpdated(flagMask)) OnUpdateFlag(m.flag);
}

#pragma optimize("", off)
RPCResult QuestFlagData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestFlagData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult QuestFlagData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_QuestFlagData_UP_STATE:
			_result = RPCDecode<1277844274>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
