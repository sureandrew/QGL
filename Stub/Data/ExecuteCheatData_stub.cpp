//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/ExecuteCheatData_stub.h"

void ExecuteCheatData_Stub::SetInfo(const UInt32 &exe_id, const UInt32 &acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command)
{
	bool modified = false;
	if (m.exe_id != exe_id) { m.exe_id = exe_id; modified = true; }
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.target_acct_id != target_acct_id) { m.target_acct_id = target_acct_id; modified = true; }
	if (m.target_char_id != target_char_id) { m.target_char_id = target_char_id; modified = true; }
	if (m.command != command) { m.command = command; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Stub::SetExe_id(const UInt32 &exe_id)
{
	bool modified = false;
	if (m.exe_id != exe_id) { m.exe_id = exe_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Stub::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Stub::SetTarget_acct_id(const UInt32 &target_acct_id)
{
	bool modified = false;
	if (m.target_acct_id != target_acct_id) { m.target_acct_id = target_acct_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Stub::SetTarget_char_id(const UInt32 &target_char_id)
{
	bool modified = false;
	if (m.target_char_id != target_char_id) { m.target_char_id = target_char_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void ExecuteCheatData_Stub::SetCommand(const String &command)
{
	bool modified = false;
	if (m.command != command) { m.command = command; modified = true; }
	MarkUpdateAll(infoMask, modified);
}


void ExecuteCheatData_Stub::Clone(ExecuteCheatData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(infoMask) && !(obj->m.exe_id == m.exe_id)) {
		m.exe_id = obj->m.exe_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.acct_id == m.acct_id)) {
		m.acct_id = obj->m.acct_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.target_acct_id == m.target_acct_id)) {
		m.target_acct_id = obj->m.target_acct_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.target_char_id == m.target_char_id)) {
		m.target_char_id = obj->m.target_char_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.command == m.command)) {
		m.command = obj->m.command;
		MarkUpdateAll(infoMask, true);
	}
}

void ExecuteCheatData_Stub::ReadAttr(CONST Buffer &buf)
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

void ExecuteCheatData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult ExecuteCheatData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ExecuteCheatData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult ExecuteCheatData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_ExecuteCheatData_REQ_STATE:
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

