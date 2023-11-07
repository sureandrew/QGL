//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/MarriageData_stub.h"

void MarriageData_Stub::SetMarriage_id(const UInt32 &marriage_id)
{
	bool modified = false;
	if (m.marriage_id != marriage_id) { m.marriage_id = marriage_id; modified = true; }
	MarkUpdateAll(marriage_idMask, modified);
}

void MarriageData_Stub::SetHusband_id(const UInt32 &husband_id)
{
	bool modified = false;
	if (m.husband_id != husband_id) { m.husband_id = husband_id; modified = true; }
	MarkUpdateAll(husband_idMask, modified);
}

void MarriageData_Stub::SetWife_id(const UInt32 &wife_id)
{
	bool modified = false;
	if (m.wife_id != wife_id) { m.wife_id = wife_id; modified = true; }
	MarkUpdateAll(wife_idMask, modified);
}

void MarriageData_Stub::SetMarriagetime(const UInt32 &marriage_time)
{
	bool modified = false;
	if (m.marriage_time != marriage_time) { m.marriage_time = marriage_time; modified = true; }
	MarkUpdateAll(marriagetimeMask, modified);
}

void MarriageData_Stub::SetMarriage_time(const UInt32 &marriage_time)
{
	bool modified = false;
	if (m.marriage_time != marriage_time) { m.marriage_time = marriage_time; modified = true; }
	MarkUpdateAll(marriagetimeMask, modified);
}

void MarriageData_Stub::SetState(const UInt32 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}


void MarriageData_Stub::Clone(MarriageData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(marriage_idMask) && !(obj->m.marriage_id == m.marriage_id)) {
		m.marriage_id = obj->m.marriage_id;
		MarkUpdateAll(marriage_idMask, true);
	}
	if (mask.IsBit(husband_idMask) && !(obj->m.husband_id == m.husband_id)) {
		m.husband_id = obj->m.husband_id;
		MarkUpdateAll(husband_idMask, true);
	}
	if (mask.IsBit(wife_idMask) && !(obj->m.wife_id == m.wife_id)) {
		m.wife_id = obj->m.wife_id;
		MarkUpdateAll(wife_idMask, true);
	}
	if (mask.IsBit(marriagetimeMask) && !(obj->m.marriage_time == m.marriage_time)) {
		m.marriage_time = obj->m.marriage_time;
		MarkUpdateAll(marriagetimeMask, true);
	}
	if (mask.IsBit(stateMask) && !(obj->m.state == m.state)) {
		m.state = obj->m.state;
		MarkUpdateAll(stateMask, true);
	}
}

void MarriageData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(marriage_idMask))
	{
		buf.Read<UInt32>(m.marriage_id);
	}
	if (m_updateMask.IsBit(husband_idMask))
	{
		buf.Read<UInt32>(m.husband_id);
	}
	if (m_updateMask.IsBit(wife_idMask))
	{
		buf.Read<UInt32>(m.wife_id);
	}
	if (m_updateMask.IsBit(marriagetimeMask))
	{
		buf.Read<UInt32>(m.marriage_time);
	}
	if (m_updateMask.IsBit(stateMask))
	{
		buf.Read<UInt32>(m.state);
	}
}

void MarriageData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(marriage_idMask))
	{
		buf.Write<UInt32>(m.marriage_id);
	}
	if (mask.IsBit(husband_idMask))
	{
		buf.Write<UInt32>(m.husband_id);
	}
	if (mask.IsBit(wife_idMask))
	{
		buf.Write<UInt32>(m.wife_id);
	}
	if (mask.IsBit(marriagetimeMask))
	{
		buf.Write<UInt32>(m.marriage_time);
	}
	if (mask.IsBit(stateMask))
	{
		buf.Write<UInt32>(m.state);
	}
}

RPCResult MarriageData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MarriageData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult MarriageData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MarriageData_REQ_STATE:
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

