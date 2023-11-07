///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILDAPPLICANT_PROXY_H_
#define _RPC_GUILDAPPLICANT_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GuildApplicant_enum.h"

class GuildApplicant_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &char_id);
	virtual void OnUpdateName(const String &name);
	virtual void OnUpdateApplyDate(const UInt32 &ApplyDate);
	virtual void OnUpdateLevel(const UInt16 &level);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		nameMask	= Parent::NextFreeMask + 1,
		applyDateMask	= Parent::NextFreeMask + 2,
		levelMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetName() const { return m.name; }
	const UInt32& GetApplyDate() const { return m.ApplyDate; }
	const UInt16& GetLevel() const { return m.level; }
	virtual void OnUpdateId(const UInt32 &char_id) {};
	virtual void OnUpdateName(const String &name) {};
	virtual void OnUpdateApplyDate(const UInt32 &ApplyDate) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 char_id;
	//name
	String name;
	//applyDate
	UInt32 ApplyDate;
	//level
	UInt16 level;
} m;

public:
	INLINE GuildApplicant_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult GuildApplicant_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildApplicant_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<3559346>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(480271848)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(3559346)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_GUILDAPPLICANT_PROXY_H_
