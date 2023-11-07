///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MARRIAGEDATA_PROXY_H_
#define _RPC_MARRIAGEDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/MarriageData_enum.h"

class MarriageData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateMarriage_id(const UInt32 &marriage_id);
	virtual void OnUpdateHusband_id(const UInt32 &husband_id);
	virtual void OnUpdateWife_id(const UInt32 &wife_id);
	virtual void OnUpdateMarriagetime(const UInt32 &marriage_time);
	virtual void OnUpdateState(const UInt32 &state);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		marriage_idMask	= Parent::NextFreeMask + 0,
		husband_idMask	= Parent::NextFreeMask + 1,
		wife_idMask	= Parent::NextFreeMask + 2,
		marriagetimeMask	= Parent::NextFreeMask + 3,
		stateMask	= Parent::NextFreeMask + 4,
		NextFreeMask	= Parent::NextFreeMask + 5
	};
	const UInt32& GetMarriage_id() const { return m.marriage_id; }
	const UInt32& GetHusband_id() const { return m.husband_id; }
	const UInt32& GetWife_id() const { return m.wife_id; }
	const UInt32& GetMarriage_time() const { return m.marriage_time; }
	const UInt32& GetState() const { return m.state; }
	virtual void OnUpdateMarriage_id(const UInt32 &marriage_id) {};
	virtual void OnUpdateHusband_id(const UInt32 &husband_id) {};
	virtual void OnUpdateWife_id(const UInt32 &wife_id) {};
	virtual void OnUpdateMarriagetime(const UInt32 &marriage_time) {};
	virtual void OnUpdateState(const UInt32 &state) {};
	void SetMarriage_id(const UInt32 &marriage_id);
	void SetHusband_id(const UInt32 &husband_id);
	void SetWife_id(const UInt32 &wife_id);
	void SetMarriagetime(const UInt32 &marriage_time);
	void SetMarriage_time(const UInt32 &marriage_time);
	void SetState(const UInt32 &state);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//marriage_id
	UInt32 marriage_id;
	//husband_id
	UInt32 husband_id;
	//wife_id
	UInt32 wife_id;
	//marriagetime
	UInt32 marriage_time;
	//state
	UInt32 state;
} m;

public:
	INLINE MarriageData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult MarriageData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MarriageData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1093954973>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(462297967)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1093954973)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_MARRIAGEDATA_PROXY_H_
