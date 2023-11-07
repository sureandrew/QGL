///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTNERDATA_STUB_H_
#define _RPC_PARTNERDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/PartnerData_enum.h"

class PartnerData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		ownerMask	= Parent::NextFreeMask + 1,
		infoMask	= Parent::NextFreeMask + 2,
		levelMask	= Parent::NextFreeMask + 3,
		expMask	= Parent::NextFreeMask + 4,
		attrib1Mask	= Parent::NextFreeMask + 5,
		attrPointMask	= Parent::NextFreeMask + 6,
		nativeMask	= Parent::NextFreeMask + 7,
		growMask	= Parent::NextFreeMask + 8,
		HPMask	= Parent::NextFreeMask + 9,
		SPMask	= Parent::NextFreeMask + 10,
		favorMask	= Parent::NextFreeMask + 11,
		stateMask	= Parent::NextFreeMask + 12,
		buffMask	= Parent::NextFreeMask + 13,
		healthMask	= Parent::NextFreeMask + 14,
		goodnessMask	= Parent::NextFreeMask + 15,
		soulboundStateMask	= Parent::NextFreeMask + 16,
		ExpireDateMask	= Parent::NextFreeMask + 17,
		ElementMask	= Parent::NextFreeMask + 18,
		OutLookColorMask	= Parent::NextFreeMask + 19,
		inWareHouseMask	= Parent::NextFreeMask + 20,
		assetLockMask	= Parent::NextFreeMask + 21,
		CultivationMask	= Parent::NextFreeMask + 22,
		PracticeMask	= Parent::NextFreeMask + 23,
		NextFreeMask	= Parent::NextFreeMask + 24
	};
	const UInt32& GetPartner_id() const { return m.partner_id; }
	const UInt32& GetMob_id() const { return m.mob_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const String& GetRaname() const { return m.raname; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetExp() const { return m.exp; }
	const UInt16& GetSTA() const { return m.STA; }
	const UInt16& GetSPR() const { return m.SPR; }
	const UInt16& GetSTR() const { return m.STR; }
	const UInt16& GetCON() const { return m.CON; }
	const UInt16& GetAGI() const { return m.AGI; }
	const UInt16& GetAttrPoint() const { return m.attrPoint; }
	const UInt16& GetHP_native() const { return m.HP_native; }
	const UInt16& GetSP_native() const { return m.SP_native; }
	const UInt16& GetATK_native() const { return m.ATK_native; }
	const UInt16& GetDEF_native() const { return m.DEF_native; }
	const UInt16& GetSPD_native() const { return m.SPD_native; }
	const UInt16& GetPOW_native() const { return m.POW_native; }
	const UInt16& GetHIT_native() const { return m.HIT_native; }
	const UInt16& GetEVA_native() const { return m.EVA_native; }
	const float& GetGrow() const { return m.grow; }
	const UInt16& GetHP() const { return m.HP; }
	const UInt16& GetMaxHP() const { return m.maxHP; }
	const UInt16& GetSP() const { return m.SP; }
	const UInt16& GetMaxSP() const { return m.maxSP; }
	const float& GetFavor() const { return m.favor; }
	const UInt8& GetState() const { return m.state; }
	const String& GetBuff() const { return m.buff; }
	const UInt16& GetHealth() const { return m.health; }
	const Int32& GetGoodness() const { return m.goodness; }
	const UInt8& GetSoulboundState() const { return m.soulboundState; }
	const UInt32& GetExpireDate() const { return m.expireDate; }
	const UInt8& GetElement() const { return m.element; }
	const UInt16& GetOutLookColor() const { return m.outLookColor; }
	const UInt8& GetInWareHouse() const { return m.inWareHouse; }
	const Boolean& GetLock() const { return m.lock; }
	const UInt16& GetCultivation() const { return m.cultivation; }
	const UInt16& GetPractice() const { return m.practice; }
	void SetId(const UInt32 &partner_id, const UInt32 &mob_id);
	void SetPartner_id(const UInt32 &partner_id);
	void SetMob_id(const UInt32 &mob_id);
	void SetOwner(const UInt32 &owner_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetInfo(const String &raname);
	void SetRaname(const String &raname);
	void SetLevel(const UInt16 &level);
	void SetExp(const UInt32 &exp);
	void SetAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	void SetSTA(const UInt16 &STA);
	void SetSPR(const UInt16 &SPR);
	void SetSTR(const UInt16 &STR);
	void SetCON(const UInt16 &CON);
	void SetAGI(const UInt16 &AGI);
	void SetAttrPoint(const UInt16 &attrPoint);
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
	void SetHP(const UInt16 &HP, const UInt16 &maxHP);
	void SetHP(const UInt16 &HP);
	void SetMaxHP(const UInt16 &maxHP);
	void SetSP(const UInt16 &SP, const UInt16 &maxSP);
	void SetSP(const UInt16 &SP);
	void SetMaxSP(const UInt16 &maxSP);
	void SetFavor(const float &favor);
	void SetState(const UInt8 &state);
	void SetBuff(const String &buff);
	void SetHealth(const UInt16 &health);
	void SetGoodness(const Int32 &goodness);
	void SetSoulboundState(const UInt8 &soulboundState);
	void SetExpireDate(const UInt32 &expireDate);
	void SetElement(const UInt8 &element);
	void SetOutLookColor(const UInt16 &outLookColor);
	void SetInWareHouse(const UInt8 &inWareHouse);
	void SetAssetLock(const Boolean &lock);
	void SetLock(const Boolean &lock);
	void SetCultivation(const UInt16 &cultivation);
	void SetPractice(const UInt16 &practice);
	void Clone(PartnerData_Stub* obj, const RPCMask &mask);
	friend void PartnerData_Stub::Clone(PartnerData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 partner_id;
	UInt32 mob_id;
	//owner
	UInt32 owner_id;
	//info
	String raname;
	//level
	UInt16 level;
	//exp
	UInt32 exp;
	//attrib1
	UInt16 STA;
	UInt16 SPR;
	UInt16 STR;
	UInt16 CON;
	UInt16 AGI;
	//attrPoint
	UInt16 attrPoint;
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
	//HP
	UInt16 HP;
	UInt16 maxHP;
	//SP
	UInt16 SP;
	UInt16 maxSP;
	//favor
	float favor;
	//state
	UInt8 state;
	//buff
	String buff;
	//health
	UInt16 health;
	//goodness
	Int32 goodness;
	//soulboundState
	UInt8 soulboundState;
	//ExpireDate
	UInt32 expireDate;
	//Element
	UInt8 element;
	//OutLookColor
	UInt16 outLookColor;
	//inWareHouse
	UInt8 inWareHouse;
	//assetLock
	Boolean lock;
	//Cultivation
	UInt16 cultivation;
	//Practice
	UInt16 practice;
} m;

public:
	INLINE PartnerData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PARTNERDATA_STUB_H_
