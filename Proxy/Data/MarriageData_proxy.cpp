//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/MarriageData_proxy.h"

void MarriageData_Proxy::SetMarriage_id(const UInt32 &marriage_id)
{
	bool modified = false;
	if (m.marriage_id != marriage_id) { m.marriage_id = marriage_id; modified = true; }
	MarkUpdateAll(marriage_idMask, modified);
}

void MarriageData_Proxy::SetHusband_id(const UInt32 &husband_id)
{
	bool modified = false;
	if (m.husband_id != husband_id) { m.husband_id = husband_id; modified = true; }
	MarkUpdateAll(husband_idMask, modified);
}

void MarriageData_Proxy::SetWife_id(const UInt32 &wife_id)
{
	bool modified = false;
	if (m.wife_id != wife_id) { m.wife_id = wife_id; modified = true; }
	MarkUpdateAll(wife_idMask, modified);
}

void MarriageData_Proxy::SetMarriagetime(const UInt32 &marriage_time)
{
	bool modified = false;
	if (m.marriage_time != marriage_time) { m.marriage_time = marriage_time; modified = true; }
	MarkUpdateAll(marriagetimeMask, modified);
}

void MarriageData_Proxy::SetMarriage_time(const UInt32 &marriage_time)
{
	bool modified = false;
	if (m.marriage_time != marriage_time) { m.marriage_time = marriage_time; modified = true; }
	MarkUpdateAll(marriagetimeMask, modified);
}

void MarriageData_Proxy::SetState(const UInt32 &state)
{
	bool modified = false;
	if (m.state != state) { m.state = state; modified = true; }
	MarkUpdateAll(stateMask, modified);
}


void MarriageData_Proxy::ReadAttr(CONST Buffer &buf)
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

void MarriageData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

void MarriageData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(marriage_idMask)) OnUpdateMarriage_id(m.marriage_id);
	if (IsUpdated(husband_idMask)) OnUpdateHusband_id(m.husband_id);
	if (IsUpdated(wife_idMask)) OnUpdateWife_id(m.wife_id);
	if (IsUpdated(marriagetimeMask)) OnUpdateMarriagetime(m.marriage_time);
	if (IsUpdated(stateMask)) OnUpdateState(m.state);
}

#pragma optimize("", off)
RPCResult MarriageData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MarriageData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult MarriageData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_MarriageData_UP_STATE:
			_result = RPCDecode<462297967>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
