///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATMOB_STUB_H_
#define _RPC_BATMOB_STUB_H_

#include "Common/RPCFunc.h"
#include "BatActor.h"
#include "RPCEnum/BatMob_enum.h"

class BatMob_Stub : public CBatActor
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetInfo(const UInt32 &mob_id, const String &raname);
	void SetMob_id(const UInt32 &mob_id);
	void SetRaname(const String &raname);
	void SetOwner(const UInt32 &owner_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetPartner(const UInt32 &partner_id);
	void SetPartner_id(const UInt32 &partner_id);
	void SetMobType(const UInt8 &mob_type);
	void SetMob_type(const UInt8 &mob_type);
	void SetBoss(const Boolean &boss);
	void SetPartnerType(const UInt8 &partner_type);
	void SetPartner_type(const UInt8 &partner_type);
	void SetNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native);
	void SetHP_native(const UInt16 &HP_native);
	void SetSP_native(const UInt16 &SP_native);
	void SetATK_native(const UInt16 &ATK_native);
	void SetDEF_native(const UInt16 &DEF_native);
	void SetSPD_native(const UInt16 &SPD_native);
	void SetPOW_native(const UInt16 &POW_native);
	void SetHIT_native(const UInt16 &HIT_native);
	void SetEVA_native(const UInt16 &EVA_native);
	void SetGrow(const float &grow);
	void SetPartnerColor(const UInt16 &colorIndex);
	void SetColorIndex(const UInt16 &colorIndex);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE BatMob_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_BATMOB_STUB_H_
