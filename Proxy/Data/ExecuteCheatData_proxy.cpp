//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/ExecuteCheatData_proxy.h"

void ExecuteCheatData_Proxy::SetInfo(const UInt32 &exe_id, const UInt32 &acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command)
{
	bool modified = false;
	if (m.exe_id != exe_id) { m.exe_id = exe_id; modified = true; }
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.target_acct_id != target_acct_id) { m.target_acct_id = target_acct_id; modified = true; }
	if (m.target_char_id != target_char_id) { m.target_char_id = target_char_id; modified = true; }
	if (m.command != command) { m.command = command; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Proxy::SetExe_id(const UInt32 &exe_id)
{
	bool modified = false;
	if (m.exe_id != exe_id) { m.exe_id = exe_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Proxy::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Proxy::SetTarget_acct_id(const UInt32 &target_acct_id)
{
	bool modified = false;
	if (m.target_acct_id != target_acct_id) { m.target_acct_id = target_acct_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Proxy::SetTarget_char_id(const UInt32 &target_char_id)
{
	bool modified = false;
	if (m.target_char_id != target_char_id) { m.target_char_id = target_char_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Proxy::SetCommand(const String &command)
{
	bool modified = false;
	if (m.command != command) { m.command = command; modified = true; }
	MarkUpdateAll(infoMask, modified);
}


void ExecuteCheatData_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt32>(m.exe_id);
		buf.Read<UInt32>(m.acct_id);
		buf.Read<UInt32>(m.target_acct_id);
		buf.Read<UInt32>(m.target_char_id);
		buf.Read<String>(m.command);
	}
}

void ExecuteCheatData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt32>(m.exe_id);
		buf.Write<UInt32>(m.acct_id);
		buf.Write<UInt32>(m.target_acct_id);
		buf.Write<UInt32>(m.target_char_id);
		buf.Write<String>(m.command);
	}
}

void ExecuteCheatData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.exe_id, m.acct_id, m.target_acct_id, m.target_char_id, m.command);
}

#pragma optimize("", off)
RPCResult ExecuteCheatData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ExecuteCheatData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult ExecuteCheatData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_ExecuteCheatData_UP_STATE:
			_result = RPCDecode<1117547812>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
