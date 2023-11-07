///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATMOB_PROXY_H_
#define _RPC_BATMOB_PROXY_H_

#include "Common/RPCFunc.h"
#include "BatActor.h"
#include "RPCEnum/BatMob_enum.h"

class BatMob_Proxy : public CBatActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt32 &mob_id, const String &raname);
	virtual void OnUpdateOwner(const UInt32 &owner_id);
	virtual void OnUpdatePartner(const UInt32 &partner_id);
	virtual void OnUpdateMobType(const UInt8 &mob_type);
	virtual void OnUpdateBoss(const Boolean &boss);
	virtual void OnUpdatePartnerType(const UInt8 &partner_type);
	virtual void OnUpdateNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native);
	virtual void OnUpdateGrow(const float &grow);
	virtual void OnUpdatePartnerColor(const UInt16 &colorIndex);
***/

protected:
	typedef CBatActor Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		ownerMask	= Parent::NextFreeMask + 1,
		partnerMask	= Parent::NextFreeMask + 2,
		mobTypeMask	= Parent::NextFreeMask + 3,
		bossMask	= Parent::NextFreeMask + 4,
		partnerTypeMask	= Parent::NextFreeMask + 5,
		nativeMask	= Parent::NextFreeMask + 6,
		growMask	= Parent::NextFreeMask + 7,
		partnerColorMask	= Parent::NextFreeMask + 8,
		NextFreeMask	= Parent::NextFreeMask + 9
	};
	const UInt32& GetMob_id() const { return m.mob_id; }
	const String& GetRaname() const { return m.raname; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const UInt32& GetPartner_id() const { return m.partner_id; }
	const UInt8& GetMob_type() const { return m.mob_type; }
	const Boolean& GetBoss() const { return m.boss; }
	const UInt8& GetPartner_type() const { return m.partner_type; }
	const UInt16& GetHP_native() const { return m.HP_native; }
	const UInt16& GetSP_native() const { return m.SP_native; }
	const UInt16& GetATK_native() const { return m.ATK_native; }
	const UInt16& GetDEF_native() const { return m.DEF_native; }
	const UInt16& GetSPD_native() const { return m.SPD_native; }
	const UInt16& GetPOW_native() const { return m.POW_native; }
	const UInt16& GetHIT_native() const { return m.HIT_native; }
	const UInt16& GetEVA_native() const { return m.EVA_native; }
	const float& GetGrow() const { return m.grow; }
	const UInt16& GetColorIndex() const { return m.colorIndex; }
	virtual void OnUpdateInfo(const UInt32 &mob_id, const String &raname) {};
	virtual void OnUpdateOwner(const UInt32 &owner_id) {};
	virtual void OnUpdatePartner(const UInt32 &partner_id) {};
	virtual void OnUpdateMobType(const UInt8 &mob_type) {};
	virtual void OnUpdateBoss(const Boolean &boss) {};
	virtual void OnUpdatePartnerType(const UInt8 &partner_type) {};
	virtual void OnUpdateNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native) {};
	virtual void OnUpdateGrow(const float &grow) {};
	virtual void OnUpdatePartnerColor(const UInt16 &colorIndex) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt32 mob_id;
	String raname;
	//owner
	UInt32 owner_id;
	//partner
	UInt32 partner_id;
	//mobType
	UInt8 mob_type;
	//boss
	Boolean boss;
	//partnerType
	UInt8 partner_type;
	//native
	UInt16 HP_native;
	UInt16 SP_native;
	UInt16 ATK_native;
	UInt16 DEF_native;
	UInt16 SPD_native;
	UInt16 POW_native;
	UInt16 HIT_native;
	UInt16 EVA_native;
	//grow
	float grow;
	//partnerColor
	UInt16 colorIndex;
} m;

public:
	INLINE BatMob_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult BatMob_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatMob_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<39729214>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(270741010)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(39729214)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_BATMOB_PROXY_H_
