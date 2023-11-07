///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTNER_STUB_H_
#define _RPC_PARTNER_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Partner_enum.h"

class Partner_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi);
	virtual RPCResult ChangePartnerName(LPCPEER pPeer, RPCContext &context, const String &newName);
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt8 &state);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		ownerMask	= Parent::NextFreeMask + 1,
		infoMask	= Parent::NextFreeMask + 2,
		levelMask	= Parent::NextFreeMask + 3,
		nextLvExpMask	= Parent::NextFreeMask + 4,
		expMask	= Parent::NextFreeMask + 5,
		attrib1Mask	= Parent::NextFreeMask + 6,
		attrib2Mask	= Parent::NextFreeMask + 7,
		attrPointMask	= Parent::NextFreeMask + 8,
		HPMask	= Parent::NextFreeMask + 9,
		SPMask	= Parent::NextFreeMask + 10,
		nativeMask	= Parent::NextFreeMask + 11,
		growMask	= Parent::NextFreeMask + 12,
		favorMask	= Parent::NextFreeMask + 13,
		healthMask	= Parent::NextFreeMask + 14,
		goodnessMask	= Parent::NextFreeMask + 15,
		stateMask	= Parent::NextFreeMask + 16,
		buffMask	= Parent::NextFreeMask + 17,
		soulboundStateMask	= Parent::NextFreeMask + 18,
		ExpireDateMask	= Parent::NextFreeMask + 19,
		ElementMask	= Parent::NextFreeMask + 20,
		OutLookColorMask	= Parent::NextFreeMask + 21,
		buffCacheMask	= Parent::NextFreeMask + 22,
		inWareHouseMask	= Parent::NextFreeMask + 23,
		assetLockMask	= Parent::NextFreeMask + 24,
		CultivationMask	= Parent::NextFreeMask + 25,
		PracticeMask	= Parent::NextFreeMask + 26,
		growLevelMask	= Parent::NextFreeMask + 27,
		NextFreeMask	= Parent::NextFreeMask + 28
	};
	virtual RPCResult RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi) = 0;
	virtual RPCResult ChangePartnerName(LPCPEER pPeer, RPCContext &context, const String &newName) = 0;
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt8 &state) = 0;
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_ChangePartnerName(LPCPEER pPeer, RPCContext &context, const bool &success);
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_UpdateSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID);
	virtual RPCResult CB_FalseJoinBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType);
	const UInt32& GetPartner_id() const { return m.partner_id; }
	const UInt32& GetMob_id() const { return m.mob_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const String& GetRaname() const { return m.raname; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetNextLvExp() const { return m.nextLvExp; }
	const UInt32& GetExp() const { return m.exp; }
	const UInt16& GetSTA() const { return m.STA; }
	const UInt16& GetSPR() const { return m.SPR; }
	const UInt16& GetSTR() const { return m.STR; }
	const UInt16& GetCON() const { return m.CON; }
	const UInt16& GetAGI() const { return m.AGI; }
	const UInt16& GetATK() const { return m.ATK; }
	const UInt16& GetDEF() const { return m.DEF; }
	const UInt16& GetHIT() const { return m.HIT; }
	const UInt16& GetEVA() const { return m.EVA; }
	const UInt16& GetPOW() const { return m.POW; }
	const UInt16& GetSPD() const { return m.SPD; }
	const UInt16& GetAttrPoint() const { return m.attrPoint; }
	const UInt32& GetHP() const { return m.HP; }
	const UInt32& GetMaxHP() const { return m.maxHP; }
	const UInt32& GetSP() const { return m.SP; }
	const UInt32& GetMaxSP() const { return m.maxSP; }
	const UInt16& GetHP_native() const { return m.HP_native; }
	const UInt16& GetSP_native() const { return m.SP_native; }
	const UInt16& GetATK_native() const { return m.ATK_native; }
	const UInt16& GetDEF_native() const { return m.DEF_native; }
	const UInt16& GetSPD_native() const { return m.SPD_native; }
	const UInt16& GetPOW_native() const { return m.POW_native; }
	const UInt16& GetHIT_native() const { return m.HIT_native; }
	const UInt16& GetEVA_native() const { return m.EVA_native; }
	const float& GetGrow() const { return m.grow; }
	const float& GetFavor() const { return m.favor; }
	const UInt16& GetHealth() const { return m.health; }
	const Int32& GetGoodness() const { return m.goodness; }
	const UInt8& GetState() const { return m.state; }
	const String& GetBuff() const { return m.buff; }
	const UInt8& GetSoulboundState() const { return m.soulboundState; }
	const UInt32& GetExpireDate() const { return m.expireDate; }
	const UInt8& GetElement() const { return m.element; }
	const UInt16& GetOutLookColor() const { return m.outLookColor; }
	const String& GetBuffCache() const { return m.buffCache; }
	const UInt8& GetInWareHouse() const { return m.inWareHouse; }
	const Boolean& GetLock() const { return m.lock; }
	const UInt16& GetCultivation() const { return m.cultivation; }
	const UInt16& GetPractice() const { return m.practice; }
	const UInt8& GetGrowLevel() const { return m.growLevel; }
	RPCMask m_modifyMask;
	INLINE Boolean IsModified() { return (!m_modifyMask.IsZero()); }
	INLINE Boolean IsModified(UInt bit) { return (m_modifyMask.IsBit(bit)); }
	INLINE Boolean IsModified(CONST RPCMask& mask) { return (m_modifyMask.IsMask(mask)); }
	INLINE VOID SetModifyMask(CONST RPCMask& mask) { m_modifyMask = mask; }
	INLINE CONST RPCMask& GetModifyMask() CONST { return m_modifyMask; }
	INLINE VOID ClearModifyMask() { m_modifyMask.Clear(); }
	INLINE VOID MarkModify(UInt bit) { m_modifyMask.MaskBit(bit); }
	INLINE VOID MarkModify(CONST RPCMask& mask) { m_modifyMask.Mask(mask); }
	INLINE VOID UnmarkModify(UInt bit) { m_modifyMask.UnmaskBit(bit); }
	INLINE VOID UnmarkModify(CONST RPCMask& mask) { m_modifyMask.Unmask(mask); }
	INLINE virtual VOID ClearAllMask() {
		Parent::ClearAllMask();
		m_modifyMask.Clear();
	}
	INLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {
		Parent::MarkUpdateAll(bit, modified);
		if (modified) m_modifyMask.MaskBit(bit);
	}
	void SetId(const UInt32 &partner_id, const UInt32 &mob_id);
	void SetPartner_id(const UInt32 &partner_id);
	void SetMob_id(const UInt32 &mob_id);
	void SetOwner(const UInt32 &owner_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetInfo(const String &raname);
	void SetRaname(const String &raname);
	void SetLevel(const UInt16 &level);
	void SetNextLvExp(const UInt32 &nextLvExp);
	void SetExp(const UInt32 &exp);
	void SetAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	void SetSTA(const UInt16 &STA);
	void SetSPR(const UInt16 &SPR);
	void SetSTR(const UInt16 &STR);
	void SetCON(const UInt16 &CON);
	void SetAGI(const UInt16 &AGI);
	void SetAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);
	void SetATK(const UInt16 &ATK);
	void SetDEF(const UInt16 &DEF);
	void SetHIT(const UInt16 &HIT);
	void SetEVA(const UInt16 &EVA);
	void SetPOW(const UInt16 &POW);
	void SetSPD(const UInt16 &SPD);
	void SetAttrPoint(const UInt16 &attrPoint);
	void SetHP(const UInt32 &HP, const UInt32 &maxHP);
	void SetHP(const UInt32 &HP);
	void SetMaxHP(const UInt32 &maxHP);
	void SetSP(const UInt32 &SP, const UInt32 &maxSP);
	void SetSP(const UInt32 &SP);
	void SetMaxSP(const UInt32 &maxSP);
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
	void SetFavor(const float &favor);
	void SetHealth(const UInt16 &health);
	void SetGoodness(const Int32 &goodness);
	void SetState(const UInt8 &state);
	void SetBuff(const String &buff);
	void SetSoulboundState(const UInt8 &soulboundState);
	void SetExpireDate(const UInt32 &expireDate);
	void SetElement(const UInt8 &element);
	void SetOutLookColor(const UInt16 &outLookColor);
	void SetBuffCache(const String &buffCache);
	void SetInWareHouse(const UInt8 &inWareHouse);
	void SetAssetLock(const Boolean &lock);
	void SetLock(const Boolean &lock);
	void SetCultivation(const UInt16 &cultivation);
	void SetPractice(const UInt16 &practice);
	void SetGrowLevel(const UInt8 &growLevel);
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
	//nextLvExp
	UInt32 nextLvExp;
	//exp
	UInt32 exp;
	//attrib1
	UInt16 STA;
	UInt16 SPR;
	UInt16 STR;
	UInt16 CON;
	UInt16 AGI;
	//attrib2
	UInt16 ATK;
	UInt16 DEF;
	UInt16 HIT;
	UInt16 EVA;
	UInt16 POW;
	UInt16 SPD;
	//attrPoint
	UInt16 attrPoint;
	//HP
	UInt32 HP;
	UInt32 maxHP;
	//SP
	UInt32 SP;
	UInt32 maxSP;
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
	//favor
	float favor;
	//health
	UInt16 health;
	//goodness
	Int32 goodness;
	//state
	UInt8 state;
	//buff
	String buff;
	//soulboundState
	UInt8 soulboundState;
	//ExpireDate
	UInt32 expireDate;
	//Element
	UInt8 element;
	//OutLookColor
	UInt16 outLookColor;
	//buffCache
	String buffCache;
	//inWareHouse
	UInt8 inWareHouse;
	//assetLock
	Boolean lock;
	//Cultivation
	UInt16 cultivation;
	//Practice
	UInt16 practice;
	//growLevel
	UInt8 growLevel;
} m;

public:
	INLINE Partner_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PARTNER_STUB_H_
