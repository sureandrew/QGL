//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/BuffData_proxy.h"

void BuffData_Proxy::SetInfo(const UInt16 &buff_id)
{
	bool modified = false;
	if (m.buff_id != buff_id) { m.buff_id = buff_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void BuffData_Proxy::SetBuff_id(const UInt16 &buff_id)
{
	bool modified = false;
	if (m.buff_id != buff_id) { m.buff_id = buff_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void BuffData_Proxy::SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void BuffData_Proxy::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void BuffData_Proxy::SetOwner_type(const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void BuffData_Proxy::SetValue(const UInt16 &buff_value)
{
	bool modified = false;
	if (m.buff_value != buff_value) { m.buff_value = buff_value; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void BuffData_Proxy::SetBuff_value(const UInt16 &buff_value)
{
	bool modified = false;
	if (m.buff_value != buff_value) { m.buff_value = buff_value; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void BuffData_Proxy::SetCoolDown(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}

void BuffData_Proxy::SetCool_down(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}


void BuffData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt16>(m.buff_id);
	}
	if (m_updateMask.IsBit(owner_infoMask))
	{
		buf.Read<UInt32>(m.owner_id);
		buf.Read<BYTE>(m.owner_type);
	}
	if (m_updateMask.IsBit(ValueMask))
	{
		buf.Read<UInt16>(m.buff_value);
	}
	if (m_updateMask.IsBit(CoolDownMask))
	{
		buf.Read<UInt32>(m.cool_down);
	}
}

void BuffData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt16>(m.buff_id);
	}
	if (mask.IsBit(owner_infoMask))
	{
		buf.Write<UInt32>(m.owner_id);
		buf.Write<BYTE>(m.owner_type);
	}
	if (mask.IsBit(ValueMask))
	{
		buf.Write<UInt16>(m.buff_value);
	}
	if (mask.IsBit(CoolDownMask))
	{
		buf.Write<UInt32>(m.cool_down);
	}
}

void BuffData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.buff_id);
	if (IsUpdated(owner_infoMask)) OnUpdateOwner_info(m.owner_id, m.owner_type);
	if (IsUpdated(ValueMask)) OnUpdateValue(m.buff_value);
	if (IsUpdated(CoolDownMask)) OnUpdateCoolDown(m.cool_down);
}

#pragma optimize("", off)
RPCResult BuffData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_BuffData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult BuffData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_BuffData_UP_STATE:
			_result = RPCDecode<348675274>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
