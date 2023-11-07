///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BUFFDATA_PROXY_H_
#define _RPC_BUFFDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/BuffData_enum.h"

class BuffData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt16 &buff_id);
	virtual void OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	virtual void OnUpdateValue(const UInt16 &buff_value);
	virtual void OnUpdateCoolDown(const UInt32 &cool_down);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		owner_infoMask	= Parent::NextFreeMask + 1,
		ValueMask	= Parent::NextFreeMask + 2,
		CoolDownMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt16& GetBuff_id() const { return m.buff_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const BYTE& GetOwner_type() const { return m.owner_type; }
	const UInt16& GetBuff_value() const { return m.buff_value; }
	const UInt32& GetCool_down() const { return m.cool_down; }
	virtual void OnUpdateInfo(const UInt16 &buff_id) {};
	virtual void OnUpdateOwner_info(const UInt32 &owner_id, const BYTE &owner_type) {};
	virtual void OnUpdateValue(const UInt16 &buff_value) {};
	virtual void OnUpdateCoolDown(const UInt32 &cool_down) {};
	void SetInfo(const UInt16 &buff_id);
	void SetBuff_id(const UInt16 &buff_id);
	void SetOwner_info(const UInt32 &owner_id, const BYTE &owner_type);
	void SetOwner_id(const UInt32 &owner_id);
	void SetOwner_type(const BYTE &owner_type);
	void SetValue(const UInt16 &buff_value);
	void SetBuff_value(const UInt16 &buff_value);
	void SetCoolDown(const UInt32 &cool_down);
	void SetCool_down(const UInt32 &cool_down);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt16 buff_id;
	//owner_info
	UInt32 owner_id;
	BYTE owner_type;
	//Value
	UInt16 buff_value;
	//CoolDown
	UInt32 cool_down;
} m;

public:
	INLINE BuffData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult BuffData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BuffData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<309682342>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(348675274)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(309682342)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_BUFFDATA_PROXY_H_
