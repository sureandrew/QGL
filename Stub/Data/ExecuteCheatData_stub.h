///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_EXECUTECHEATDATA_STUB_H_
#define _RPC_EXECUTECHEATDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ExecuteCheatData_enum.h"

class ExecuteCheatData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		NextFreeMask	= Parent::NextFreeMask + 1
	};
	const UInt32& GetExe_id() const { return m.exe_id; }
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const UInt32& GetTarget_acct_id() const { return m.target_acct_id; }
	const UInt32& GetTarget_char_id() const { return m.target_char_id; }
	const String& GetCommand() const { return m.command; }
	void SetInfo(const UInt32 &exe_id, const UInt32 &acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command);
	void SetExe_id(const UInt32 &exe_id);
	void SetAcct_id(const UInt32 &acct_id);
	void SetTarget_acct_id(const UInt32 &target_acct_id);
	void SetTarget_char_id(const UInt32 &target_char_id);
	void SetCommand(const String &command);
	void Clone(ExecuteCheatData_Stub* obj, const RPCMask &mask);
	friend void ExecuteCheatData_Stub::Clone(ExecuteCheatData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt32 exe_id;
	UInt32 acct_id;
	UInt32 target_acct_id;
	UInt32 target_char_id;
	String command;
} m;

public:
	INLINE ExecuteCheatData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_EXECUTECHEATDATA_STUB_H_
