///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_EXECUTECHEATDATA_PROXY_H_
#define _RPC_EXECUTECHEATDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ExecuteCheatData_enum.h"

class ExecuteCheatData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt32 &exe_id, const UInt32 &acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command);
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
	virtual void OnUpdateInfo(const UInt32 &exe_id, const UInt32 &acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command) {};
	void SetInfo(const UInt32 &exe_id, const UInt32 &acct_id, const UInt32 &target_acct_id, const UInt32 &target_char_id, const String &command);
	void SetExe_id(const UInt32 &exe_id);
	void SetAcct_id(const UInt32 &acct_id);
	void SetTarget_acct_id(const UInt32 &target_acct_id);
	void SetTarget_char_id(const UInt32 &target_char_id);
	void SetCommand(const String &command);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE ExecuteCheatData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult ExecuteCheatData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_ExecuteCheatData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<905398378>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1117547812)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(905398378)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_EXECUTECHEATDATA_PROXY_H_
