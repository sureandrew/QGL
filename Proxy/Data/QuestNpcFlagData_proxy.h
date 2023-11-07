///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTNPCFLAGDATA_PROXY_H_
#define _RPC_QUESTNPCFLAGDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestNpcFlagData_enum.h"

class QuestNpcFlagData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id);
	virtual void OnUpdateFlag(const Int32 &flag);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		flagMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	const UInt32& GetNpc_id() const { return m.npc_id; }
	const UInt16& GetLine_id() const { return m.line_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const UInt16& GetFlag_id() const { return m.flag_id; }
	const Int32& GetFlag() const { return m.flag; }
	virtual void OnUpdateId(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id) {};
	virtual void OnUpdateFlag(const Int32 &flag) {};
	void SetId(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id);
	void SetNpc_id(const UInt32 &npc_id);
	void SetLine_id(const UInt16 &line_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetFlag_id(const UInt16 &flag_id);
	void SetFlag(const Int32 &flag);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 npc_id;
	UInt16 line_id;
	UInt32 owner_id;
	UInt16 flag_id;
	//flag
	Int32 flag;
} m;

public:
	INLINE QuestNpcFlagData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult QuestNpcFlagData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestNpcFlagData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<772083845>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1391466966)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(772083845)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_QUESTNPCFLAGDATA_PROXY_H_
