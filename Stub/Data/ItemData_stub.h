///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_ITEMDATA_STUB_H_
#define _RPC_ITEMDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/ItemData_enum.h"

class ItemData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		owner_idMask	= Parent::NextFreeMask + 1,
		bagIndexMask	= Parent::NextFreeMask + 2,
		makerMask	= Parent::NextFreeMask + 3,
		eq_elementMask	= Parent::NextFreeMask + 4,
		endureMask	= Parent::NextFreeMask + 5,
		mEndureMask	= Parent::NextFreeMask + 6,
		gradeMask	= Parent::NextFreeMask + 7,
		upgExpMask	= Parent::NextFreeMask + 8,
		remainMask	= Parent::NextFreeMask + 9,
		stackMask	= Parent::NextFreeMask + 10,
		baseAttrMask	= Parent::NextFreeMask + 11,
		upgAttrMask	= Parent::NextFreeMask + 12,
		inlayGemMask	= Parent::NextFreeMask + 13,
		inlayAttrMask	= Parent::NextFreeMask + 14,
		addonMask	= Parent::NextFreeMask + 15,
		eqColorMask	= Parent::NextFreeMask + 16,
		qualityMask	= Parent::NextFreeMask + 17,
		soulboundStateMask	= Parent::NextFreeMask + 18,
		IdentifyStateMask	= Parent::NextFreeMask + 19,
		ExpireDateMask	= Parent::NextFreeMask + 20,
		ValueMask	= Parent::NextFreeMask + 21,
		buffIDMask	= Parent::NextFreeMask + 22,
		buffValue1Mask	= Parent::NextFreeMask + 23,
		buffValue2Mask	= Parent::NextFreeMask + 24,
		buffValue3Mask	= Parent::NextFreeMask + 25,
		buffCoolDownMask	= Parent::NextFreeMask + 26,
		buffProMask	= Parent::NextFreeMask + 27,
		inWareHouseMask	= Parent::NextFreeMask + 28,
		assetLockMask	= Parent::NextFreeMask + 29,
		skillIDMask	= Parent::NextFreeMask + 30,
		NextFreeMask	= Parent::NextFreeMask + 31
	};
	const UInt32& GetItem_uid() const { return m.item_uid; }
	const UInt32& GetItem_id() const { return m.item_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const UInt8& GetBagIndex() const { return m.bagIndex; }
	const String& GetMaker() const { return m.maker; }
	const UInt8& GetEq_element() const { return m.eq_element; }
	const UInt16& GetEndure() const { return m.endure; }
	const UInt16& GetMEndure() const { return m.mEndure; }
	const UInt16& GetGrade() const { return m.grade; }
	const UInt16& GetUpg_exp() const { return m.upg_exp; }
	const UInt8& GetRemain() const { return m.remain; }
	const UInt8& GetStack() const { return m.stack; }
	const Int16& GetBase_STA() const { return m.base_STA; }
	const Int16& GetBase_SPR() const { return m.base_SPR; }
	const Int16& GetBase_STR() const { return m.base_STR; }
	const Int16& GetBase_CON() const { return m.base_CON; }
	const Int16& GetBase_AGI() const { return m.base_AGI; }
	const Int16& GetBase_ATK() const { return m.base_ATK; }
	const Int16& GetBase_DEF() const { return m.base_DEF; }
	const Int16& GetBase_HIT() const { return m.base_HIT; }
	const Int16& GetBase_EVA() const { return m.base_EVA; }
	const Int16& GetBase_POW() const { return m.base_POW; }
	const Int16& GetBase_SPD() const { return m.base_SPD; }
	const Int16& GetBase_maxHP() const { return m.base_maxHP; }
	const Int16& GetBase_maxSP() const { return m.base_maxSP; }
	const Int16& GetBase_maxVP() const { return m.base_maxVP; }
	const Int16& GetUpg_STA() const { return m.upg_STA; }
	const Int16& GetUpg_SPR() const { return m.upg_SPR; }
	const Int16& GetUpg_STR() const { return m.upg_STR; }
	const Int16& GetUpg_CON() const { return m.upg_CON; }
	const Int16& GetUpg_AGI() const { return m.upg_AGI; }
	const Int16& GetUpg_ATK() const { return m.upg_ATK; }
	const Int16& GetUpg_DEF() const { return m.upg_DEF; }
	const Int16& GetUpg_HIT() const { return m.upg_HIT; }
	const Int16& GetUpg_EVA() const { return m.upg_EVA; }
	const Int16& GetUpg_POW() const { return m.upg_POW; }
	const Int16& GetUpg_SPD() const { return m.upg_SPD; }
	const Int16& GetUpg_maxHP() const { return m.upg_maxHP; }
	const Int16& GetUpg_maxSP() const { return m.upg_maxSP; }
	const Int16& GetUpg_maxVP() const { return m.upg_maxVP; }
	const UInt8& GetEq_gem() const { return m.eq_gem; }
	const UInt16& GetInlay_gem(const UInt8 i) const { return m.inlay_gem[i]; }
	const Int16& GetInlay_STA() const { return m.inlay_STA; }
	const Int16& GetInlay_SPR() const { return m.inlay_SPR; }
	const Int16& GetInlay_STR() const { return m.inlay_STR; }
	const Int16& GetInlay_CON() const { return m.inlay_CON; }
	const Int16& GetInlay_AGI() const { return m.inlay_AGI; }
	const Int16& GetInlay_ATK() const { return m.inlay_ATK; }
	const Int16& GetInlay_DEF() const { return m.inlay_DEF; }
	const Int16& GetInlay_HIT() const { return m.inlay_HIT; }
	const Int16& GetInlay_EVA() const { return m.inlay_EVA; }
	const Int16& GetInlay_POW() const { return m.inlay_POW; }
	const Int16& GetInlay_SPD() const { return m.inlay_SPD; }
	const Int16& GetInlay_maxHP() const { return m.inlay_maxHP; }
	const Int16& GetInlay_maxSP() const { return m.inlay_maxSP; }
	const Int16& GetInlay_maxVP() const { return m.inlay_maxVP; }
	const UInt16& GetAddon() const { return m.addon; }
	const UInt16& GetEqColor() const { return m.eqColor; }
	const UInt16& GetItem_quality() const { return m.item_quality; }
	const UInt8& GetItem_soulboundState() const { return m.item_soulboundState; }
	const UInt8& GetItem_IdentifyState() const { return m.item_IdentifyState; }
	const UInt32& GetItem_expireDate() const { return m.item_expireDate; }
	const UInt16& GetItemValue1() const { return m.itemValue1; }
	const UInt16& GetItemValue2() const { return m.itemValue2; }
	const UInt16& GetItemValue3() const { return m.itemValue3; }
	const UInt16& GetBuffID() const { return m.buffID; }
	const UInt32& GetBuffValue1() const { return m.buffValue1; }
	const UInt32& GetBuffValue2() const { return m.buffValue2; }
	const UInt32& GetBuffValue3() const { return m.buffValue3; }
	const UInt32& GetBuffCoolDown() const { return m.buffCoolDown; }
	const UInt8& GetBuffPro() const { return m.buffPro; }
	const UInt8& GetInWareHouse() const { return m.inWareHouse; }
	const Boolean& GetLock() const { return m.lock; }
	const UInt16& GetSkillID() const { return m.skillID; }
	void SetId(const UInt32 &item_uid, const UInt32 &item_id);
	void SetItem_uid(const UInt32 &item_uid);
	void SetItem_id(const UInt32 &item_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetBagIndex(const UInt8 &bagIndex);
	void SetMaker(const String &maker);
	void SetEq_element(const UInt8 &eq_element);
	void SetEndure(const UInt16 &endure);
	void SetMEndure(const UInt16 &mEndure);
	void SetGrade(const UInt16 &grade);
	void SetUpgExp(const UInt16 &upg_exp);
	void SetUpg_exp(const UInt16 &upg_exp);
	void SetRemain(const UInt8 &remain);
	void SetStack(const UInt8 &stack);
	void SetBaseAttr(const Int16 &base_STA, const Int16 &base_SPR, const Int16 &base_STR, const Int16 &base_CON, const Int16 &base_AGI, const Int16 &base_ATK, const Int16 &base_DEF, const Int16 &base_HIT, const Int16 &base_EVA, const Int16 &base_POW, const Int16 &base_SPD, const Int16 &base_maxHP, const Int16 &base_maxSP, const Int16 &base_maxVP);
	void SetBase_STA(const Int16 &base_STA);
	void SetBase_SPR(const Int16 &base_SPR);
	void SetBase_STR(const Int16 &base_STR);
	void SetBase_CON(const Int16 &base_CON);
	void SetBase_AGI(const Int16 &base_AGI);
	void SetBase_ATK(const Int16 &base_ATK);
	void SetBase_DEF(const Int16 &base_DEF);
	void SetBase_HIT(const Int16 &base_HIT);
	void SetBase_EVA(const Int16 &base_EVA);
	void SetBase_POW(const Int16 &base_POW);
	void SetBase_SPD(const Int16 &base_SPD);
	void SetBase_maxHP(const Int16 &base_maxHP);
	void SetBase_maxSP(const Int16 &base_maxSP);
	void SetBase_maxVP(const Int16 &base_maxVP);
	void SetUpgAttr(const Int16 &upg_STA, const Int16 &upg_SPR, const Int16 &upg_STR, const Int16 &upg_CON, const Int16 &upg_AGI, const Int16 &upg_ATK, const Int16 &upg_DEF, const Int16 &upg_HIT, const Int16 &upg_EVA, const Int16 &upg_POW, const Int16 &upg_SPD, const Int16 &upg_maxHP, const Int16 &upg_maxSP, const Int16 &upg_maxVP);
	void SetUpg_STA(const Int16 &upg_STA);
	void SetUpg_SPR(const Int16 &upg_SPR);
	void SetUpg_STR(const Int16 &upg_STR);
	void SetUpg_CON(const Int16 &upg_CON);
	void SetUpg_AGI(const Int16 &upg_AGI);
	void SetUpg_ATK(const Int16 &upg_ATK);
	void SetUpg_DEF(const Int16 &upg_DEF);
	void SetUpg_HIT(const Int16 &upg_HIT);
	void SetUpg_EVA(const Int16 &upg_EVA);
	void SetUpg_POW(const Int16 &upg_POW);
	void SetUpg_SPD(const Int16 &upg_SPD);
	void SetUpg_maxHP(const Int16 &upg_maxHP);
	void SetUpg_maxSP(const Int16 &upg_maxSP);
	void SetUpg_maxVP(const Int16 &upg_maxVP);
	void SetInlayGem(const UInt8 &eq_gem, const UInt16 &inlay_gem0, const UInt16 &inlay_gem1, const UInt16 &inlay_gem2, const UInt16 &inlay_gem3, const UInt16 &inlay_gem4, const UInt16 &inlay_gem5, const UInt16 &inlay_gem6, const UInt16 &inlay_gem7, const UInt16 &inlay_gem8, const UInt16 &inlay_gem9);
	void SetEq_gem(const UInt8 &eq_gem);
	void SetInlay_gem(const UInt8 i, const UInt16 &inlay_gem);
	void SetInlayAttr(const Int16 &inlay_STA, const Int16 &inlay_SPR, const Int16 &inlay_STR, const Int16 &inlay_CON, const Int16 &inlay_AGI, const Int16 &inlay_ATK, const Int16 &inlay_DEF, const Int16 &inlay_HIT, const Int16 &inlay_EVA, const Int16 &inlay_POW, const Int16 &inlay_SPD, const Int16 &inlay_maxHP, const Int16 &inlay_maxSP, const Int16 &inlay_maxVP);
	void SetInlay_STA(const Int16 &inlay_STA);
	void SetInlay_SPR(const Int16 &inlay_SPR);
	void SetInlay_STR(const Int16 &inlay_STR);
	void SetInlay_CON(const Int16 &inlay_CON);
	void SetInlay_AGI(const Int16 &inlay_AGI);
	void SetInlay_ATK(const Int16 &inlay_ATK);
	void SetInlay_DEF(const Int16 &inlay_DEF);
	void SetInlay_HIT(const Int16 &inlay_HIT);
	void SetInlay_EVA(const Int16 &inlay_EVA);
	void SetInlay_POW(const Int16 &inlay_POW);
	void SetInlay_SPD(const Int16 &inlay_SPD);
	void SetInlay_maxHP(const Int16 &inlay_maxHP);
	void SetInlay_maxSP(const Int16 &inlay_maxSP);
	void SetInlay_maxVP(const Int16 &inlay_maxVP);
	void SetAddon(const UInt16 &addon);
	void SetEqColor(const UInt16 &eqColor);
	void SetQuality(const UInt16 &item_quality);
	void SetItem_quality(const UInt16 &item_quality);
	void SetSoulboundState(const UInt8 &item_soulboundState);
	void SetItem_soulboundState(const UInt8 &item_soulboundState);
	void SetIdentifyState(const UInt8 &item_IdentifyState);
	void SetItem_IdentifyState(const UInt8 &item_IdentifyState);
	void SetExpireDate(const UInt32 &item_expireDate);
	void SetItem_expireDate(const UInt32 &item_expireDate);
	void SetValue(const UInt16 &itemValue1, const UInt16 &itemValue2, const UInt16 &itemValue3);
	void SetItemValue1(const UInt16 &itemValue1);
	void SetItemValue2(const UInt16 &itemValue2);
	void SetItemValue3(const UInt16 &itemValue3);
	void SetBuffID(const UInt16 &buffID);
	void SetBuffValue1(const UInt32 &buffValue1);
	void SetBuffValue2(const UInt32 &buffValue2);
	void SetBuffValue3(const UInt32 &buffValue3);
	void SetBuffCoolDown(const UInt32 &buffCoolDown);
	void SetBuffPro(const UInt8 &buffPro);
	void SetInWareHouse(const UInt8 &inWareHouse);
	void SetAssetLock(const Boolean &lock);
	void SetLock(const Boolean &lock);
	void SetSkillID(const UInt16 &skillID);
	void Clone(ItemData_Stub* obj, const RPCMask &mask);
	friend void ItemData_Stub::Clone(ItemData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 item_uid;
	UInt32 item_id;
	//owner_id
	UInt32 owner_id;
	//bagIndex
	UInt8 bagIndex;
	//maker
	String maker;
	//eq_element
	UInt8 eq_element;
	//endure
	UInt16 endure;
	//mEndure
	UInt16 mEndure;
	//grade
	UInt16 grade;
	//upgExp
	UInt16 upg_exp;
	//remain
	UInt8 remain;
	//stack
	UInt8 stack;
	//baseAttr
	Int16 base_STA;
	Int16 base_SPR;
	Int16 base_STR;
	Int16 base_CON;
	Int16 base_AGI;
	Int16 base_ATK;
	Int16 base_DEF;
	Int16 base_HIT;
	Int16 base_EVA;
	Int16 base_POW;
	Int16 base_SPD;
	Int16 base_maxHP;
	Int16 base_maxSP;
	Int16 base_maxVP;
	//upgAttr
	Int16 upg_STA;
	Int16 upg_SPR;
	Int16 upg_STR;
	Int16 upg_CON;
	Int16 upg_AGI;
	Int16 upg_ATK;
	Int16 upg_DEF;
	Int16 upg_HIT;
	Int16 upg_EVA;
	Int16 upg_POW;
	Int16 upg_SPD;
	Int16 upg_maxHP;
	Int16 upg_maxSP;
	Int16 upg_maxVP;
	//inlayGem
	UInt8 eq_gem;
	UInt16 inlay_gem[10];
	//inlayAttr
	Int16 inlay_STA;
	Int16 inlay_SPR;
	Int16 inlay_STR;
	Int16 inlay_CON;
	Int16 inlay_AGI;
	Int16 inlay_ATK;
	Int16 inlay_DEF;
	Int16 inlay_HIT;
	Int16 inlay_EVA;
	Int16 inlay_POW;
	Int16 inlay_SPD;
	Int16 inlay_maxHP;
	Int16 inlay_maxSP;
	Int16 inlay_maxVP;
	//addon
	UInt16 addon;
	//eqColor
	UInt16 eqColor;
	//quality
	UInt16 item_quality;
	//soulboundState
	UInt8 item_soulboundState;
	//IdentifyState
	UInt8 item_IdentifyState;
	//ExpireDate
	UInt32 item_expireDate;
	//Value
	UInt16 itemValue1;
	UInt16 itemValue2;
	UInt16 itemValue3;
	//buffID
	UInt16 buffID;
	//buffValue1
	UInt32 buffValue1;
	//buffValue2
	UInt32 buffValue2;
	//buffValue3
	UInt32 buffValue3;
	//buffCoolDown
	UInt32 buffCoolDown;
	//buffPro
	UInt8 buffPro;
	//inWareHouse
	UInt8 inWareHouse;
	//assetLock
	Boolean lock;
	//skillID
	UInt16 skillID;
} m;

public:
	INLINE ItemData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_ITEMDATA_STUB_H_
