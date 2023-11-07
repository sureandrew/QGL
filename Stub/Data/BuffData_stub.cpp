//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/BuffData_stub.h"

void BuffData_Stub::SetInfo(const UInt16 &buff_id)
{
	bool modified = false;
	if (m.buff_id != buff_id) { m.buff_id = buff_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void BuffData_Stub::SetBuff_id(const UInt16 &buff_id)
{
	bool modified = false;
	if (m.buff_id != buff_id) { m.buff_id = buff_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void BuffData_Stub::SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void BuffData_Stub::SetOwner_id(const UInt32 &owner_id)
{
	bool modified = false;
	if (m.owner_id != owner_id) { m.owner_id = owner_id; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void BuffData_Stub::SetOwner_type(const BYTE &owner_type)
{
	bool modified = false;
	if (m.owner_type != owner_type) { m.owner_type = owner_type; modified = true; }
	MarkUpdateAll(owner_infoMask, modified);
}

void BuffData_Stub::SetValue(const UInt16 &buff_value)
{
	bool modified = false;
	if (m.buff_value != buff_value) { m.buff_value = buff_value; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void BuffData_Stub::SetBuff_value(const UInt16 &buff_value)
{
	bool modified = false;
	if (m.buff_value != buff_value) { m.buff_value = buff_value; modified = true; }
	MarkUpdateAll(ValueMask, modified);
}

void BuffData_Stub::SetCoolDown(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}

void BuffData_Stub::SetCool_down(const UInt32 &cool_down)
{
	bool modified = false;
	if (m.cool_down != cool_down) { m.cool_down = cool_down; modified = true; }
	MarkUpdateAll(CoolDownMask, modified);
}


void BuffData_Stub::Clone(BuffData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(infoMask) && !(obj->m.buff_id == m.buff_id)) {
		m.buff_id = obj->m.buff_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(owner_infoMask) && !(obj->m.owner_id == m.owner_id)) {
		m.owner_id = obj->m.owner_id;
		MarkUpdateAll(owner_infoMask, true);
	}
	if (mask.IsBit(owner_infoMask) && !(obj->m.owner_type == m.owner_type)) {
		m.owner_type = obj->m.owner_type;
		MarkUpdateAll(owner_infoMask, true);
	}
	if (mask.IsBit(ValueMask) && !(obj->m.buff_value == m.buff_value)) {
		m.buff_value = obj->m.buff_value;
		MarkUpdateAll(ValueMask, true);
	}
	if (mask.IsBit(CoolDownMask) && !(obj->m.cool_down == m.cool_down)) {
		m.cool_down = obj->m.cool_down;
		MarkUpdateAll(CoolDownMask, true);
	}
}

void BuffData_Stub::ReadAttr(CONST Buffer &buf)
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

void BuffData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult BuffData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BuffData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult BuffData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_BuffData_REQ_STATE:
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

