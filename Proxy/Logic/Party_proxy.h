///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTY_PROXY_H_
#define _RPC_PARTY_PROXY_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/Party_enum.h"

class Party_Proxy : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, const UInt32 &memberId, const UInt8 &memberIdx, const Boolean &shown);
	virtual RPCResult CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, const Boolean &shown);
	virtual RPCResult CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject* member, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, const UInt8 &memberIdx);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateAvail(const Boolean &avail);
	virtual void OnUpdateLineup(const UInt32 &m_uLineupId);
	virtual void OnUpdateLineupkeypos(const int &m_iLineupKeyPos);
	virtual void OnUpdateDesc(const String &desc);
	virtual void OnUpdateLeaveTemporary(const Boolean &m_bTempLeave0, const Boolean &m_bTempLeave1, const Boolean &m_bTempLeave2, const Boolean &m_bTempLeave3, const Boolean &m_bTempLeave4);
***/

protected:
	typedef NetGroup Parent;

public:
	enum UpdateMask {
		availMask	= Parent::NextFreeMask + 0,
		lineupMask	= Parent::NextFreeMask + 1,
		lineupkeyposMask	= Parent::NextFreeMask + 2,
		DescMask	= Parent::NextFreeMask + 3,
		LeaveTemporaryMask	= Parent::NextFreeMask + 4,
		NextFreeMask	= Parent::NextFreeMask + 5
	};
	virtual RPCResult Promote(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult Kick(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult Leave(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult Destroy(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult JoinChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult ChangeDesc(LPCPEER pPeer, RPCContext &context, const String &desc);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg) = 0;
	virtual RPCResult CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, const UInt32 &memberId, const UInt8 &memberIdx, const Boolean &shown) = 0;
	virtual RPCResult CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, const Boolean &shown) = 0;
	virtual RPCResult CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject* member, const bool &kick, const Boolean &shown) = 0;
	virtual RPCResult CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, const Boolean &shown) = 0;
	virtual RPCResult CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, const UInt8 &memberIdx) = 0;
	const Boolean& GetAvail() const { return m.avail; }
	const UInt32& GetM_uLineupId() const { return m.m_uLineupId; }
	const int& GetM_iLineupKeyPos() const { return m.m_iLineupKeyPos; }
	const String& GetDesc() const { return m.desc; }
	const Boolean& GetM_bTempLeave(const UInt8 i) const { return m.m_bTempLeave[i]; }
	virtual void OnUpdateAvail(const Boolean &avail) {};
	virtual void OnUpdateLineup(const UInt32 &m_uLineupId) {};
	virtual void OnUpdateLineupkeypos(const int &m_iLineupKeyPos) {};
	virtual void OnUpdateDesc(const String &desc) {};
	virtual void OnUpdateLeaveTemporary(const Boolean &m_bTempLeave0, const Boolean &m_bTempLeave1, const Boolean &m_bTempLeave2, const Boolean &m_bTempLeave3, const Boolean &m_bTempLeave4) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//avail
	Boolean avail;
	//lineup
	UInt32 m_uLineupId;
	//lineupkeypos
	int m_iLineupKeyPos;
	//Desc
	String desc;
	//LeaveTemporary
	Boolean m_bTempLeave[5];
} m;

public:
	INLINE Party_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Party_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1428171303>(pPeer, pBuf);
}

INLINE RPCResult Party_Proxy::Promote(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_Promote);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(memberId);
	return RPCSend<1734695565>(pPeer, pBuf);
}

INLINE RPCResult Party_Proxy::Kick(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_Kick);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCNetID>(memberId);
	return RPCSend<1415006757>(pPeer, pBuf);
}

INLINE RPCResult Party_Proxy::Leave(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_Leave);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<887490946>(pPeer, pBuf);
}

INLINE RPCResult Party_Proxy::Destroy(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_Destroy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<367799813>(pPeer, pBuf);
}

INLINE RPCResult Party_Proxy::JoinChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_JoinChannel);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1174107192>(pPeer, pBuf);
}

INLINE RPCResult Party_Proxy::ChangeDesc(LPCPEER pPeer, RPCContext &context, const String &desc)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_ChangeDesc);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(desc);
	return RPCSend<223169529>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(148968060)
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1428171303)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1734695565)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1415006757)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(887490946)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(367799813)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1174107192)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(223169529)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(861240151)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(154238635)
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(794261500)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(948005376)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(809373378)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1355247430)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_PARTY_PROXY_H_
