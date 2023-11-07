///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_ITEM_PROXY_H_
#define _RPC_ITEM_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Item_enum.h"

class Item_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &item_uid, const UInt32 &item_id);
	virtual void OnUpdateOwner_id(const UInt32 &owner_id);
	virtual void OnUpdateBagIndex(const UInt8 &bagIndex);
	virtual void OnUpdateMaker(const String &maker);
	virtual void OnUpdateEq_element(const UInt8 &eq_element);
	virtual void OnUpdateEndure(const UInt16 &endure);
	virtual void OnUpdateMEndure(const UInt16 &mEndure);
	virtual void OnUpdateGrade(const UInt16 &grade);
	virtual void OnUpdateUpgExp(const UInt16 &upg_exp);
	virtual void OnUpdateRemain(const UInt8 &remain);
	virtual void OnUpdateStack(const UInt8 &stack);
	virtual void OnUpdateBaseAttr(const Int16 &base_STA, const Int16 &base_SPR, const Int16 &base_STR, const Int16 &base_CON, const Int16 &base_AGI, const Int16 &base_ATK, const Int16 &base_DEF, const Int16 &base_HIT, const Int16 &base_EVA, const Int16 &base_POW, const Int16 &base_SPD, const Int16 &base_maxHP, const Int16 &base_maxSP, const Int16 &base_maxVP);
	virtual void OnUpdateUpgAttr(const Int16 &upg_STA, const Int16 &upg_SPR, const Int16 &upg_STR, const Int16 &upg_CON, const Int16 &upg_AGI, const Int16 &upg_ATK, const Int16 &upg_DEF, const Int16 &upg_HIT, const Int16 &upg_EVA, const Int16 &upg_POW, const Int16 &upg_SPD, const Int16 &upg_maxHP, const Int16 &upg_maxSP, const Int16 &upg_maxVP);
	virtual void OnUpdateInlayGem(const UInt8 &eq_gem, const UInt16 &inlay_gem0, const UInt16 &inlay_gem1, const UInt16 &inlay_gem2, const UInt16 &inlay_gem3, const UInt16 &inlay_gem4, const UInt16 &inlay_gem5, const UInt16 &inlay_gem6, const UInt16 &inlay_gem7, const UInt16 &inlay_gem8, const UInt16 &inlay_gem9);
	virtual void OnUpdateInlayAttr(const Int16 &inlay_STA, const Int16 &inlay_SPR, const Int16 &inlay_STR, const Int16 &inlay_CON, const Int16 &inlay_AGI, const Int16 &inlay_ATK, const Int16 &inlay_DEF, const Int16 &inlay_HIT, const Int16 &inlay_EVA, const Int16 &inlay_POW, const Int16 &inlay_SPD, const Int16 &inlay_maxHP, const Int16 &inlay_maxSP, const Int16 &inlay_maxVP);
	virtual void OnUpdateAddon(const UInt16 &addon);
	virtual void OnUpdateEqColor(const UInt16 &eqColor);
	virtual void OnUpdateDetailLv(const UInt8 &detailLv);
	virtual void OnUpdateQuality(const UInt16 &item_quality);
	virtual void OnUpdateSoulboundState(const UInt8 &item_soulboundState);
	virtual void OnUpdateIdentifyState(const UInt8 &item_IdentifyState);
	virtual void OnUpdateExpireDate(const UInt32 &item_expireDate);
	virtual void OnUpdateValue(const UInt16 &itemValue1, const UInt16 &itemValue2, const UInt16 &itemValue3);
	virtual void OnUpdateBuffID(const UInt16 &buffID);
	virtual void OnUpdateBuffValue1(const UInt32 &buffValue1);
	virtual void OnUpdateBuffValue2(const UInt32 &buffValue2);
	virtual void OnUpdateBuffValue3(const UInt32 &buffValue3);
	virtual void OnUpdateBuffCoolDown(const UInt32 &buffCoolDown);
	virtual void OnUpdateBuffPro(const UInt8 &buffPro);
	virtual void OnUpdateInWareHouse(const UInt8 &inWareHouse);
	virtual void OnUpdateAssetLock(const Boolean &lock);
	virtual void OnUpdateSkillID(const UInt16 &skillID);
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
		detailLvMask	= Parent::NextFreeMask + 17,
		qualityMask	= Parent::NextFreeMask + 18,
		soulboundStateMask	= Parent::NextFreeMask + 19,
		IdentifyStateMask	= Parent::NextFreeMask + 20,
		ExpireDateMask	= Parent::NextFreeMask + 21,
		ValueMask	= Parent::NextFreeMask + 22,
		buffIDMask	= Parent::NextFreeMask + 23,
		buffValue1Mask	= Parent::NextFreeMask + 24,
		buffValue2Mask	= Parent::NextFreeMask + 25,
		buffValue3Mask	= Parent::NextFreeMask + 26,
		buffCoolDownMask	= Parent::NextFreeMask + 27,
		buffProMask	= Parent::NextFreeMask + 28,
		inWareHouseMask	= Parent::NextFreeMask + 29,
		assetLockMask	= Parent::NextFreeMask + 30,
		skillIDMask	= Parent::NextFreeMask + 31,
		NextFreeMask	= Parent::NextFreeMask + 32
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
	const UInt8& GetDetailLv() const { return m.detailLv; }
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
	virtual void OnUpdateId(const UInt32 &item_uid, const UInt32 &item_id) {};
	virtual void OnUpdateOwner_id(const UInt32 &owner_id) {};
	virtual void OnUpdateBagIndex(const UInt8 &bagIndex) {};
	virtual void OnUpdateMaker(const String &maker) {};
	virtual void OnUpdateEq_element(const UInt8 &eq_element) {};
	virtual void OnUpdateEndure(const UInt16 &endure) {};
	virtual void OnUpdateMEndure(const UInt16 &mEndure) {};
	virtual void OnUpdateGrade(const UInt16 &grade) {};
	virtual void OnUpdateUpgExp(const UInt16 &upg_exp) {};
	virtual void OnUpdateRemain(const UInt8 &remain) {};
	virtual void OnUpdateStack(const UInt8 &stack) {};
	virtual void OnUpdateBaseAttr(const Int16 &base_STA, const Int16 &base_SPR, const Int16 &base_STR, const Int16 &base_CON, const Int16 &base_AGI, const Int16 &base_ATK, const Int16 &base_DEF, const Int16 &base_HIT, const Int16 &base_EVA, const Int16 &base_POW, const Int16 &base_SPD, const Int16 &base_maxHP, const Int16 &base_maxSP, const Int16 &base_maxVP) {};
	virtual void OnUpdateUpgAttr(const Int16 &upg_STA, const Int16 &upg_SPR, const Int16 &upg_STR, const Int16 &upg_CON, const Int16 &upg_AGI, const Int16 &upg_ATK, const Int16 &upg_DEF, const Int16 &upg_HIT, const Int16 &upg_EVA, const Int16 &upg_POW, const Int16 &upg_SPD, const Int16 &upg_maxHP, const Int16 &upg_maxSP, const Int16 &upg_maxVP) {};
	virtual void OnUpdateInlayGem(const UInt8 &eq_gem, const UInt16 &inlay_gem0, const UInt16 &inlay_gem1, const UInt16 &inlay_gem2, const UInt16 &inlay_gem3, const UInt16 &inlay_gem4, const UInt16 &inlay_gem5, const UInt16 &inlay_gem6, const UInt16 &inlay_gem7, const UInt16 &inlay_gem8, const UInt16 &inlay_gem9) {};
	virtual void OnUpdateInlayAttr(const Int16 &inlay_STA, const Int16 &inlay_SPR, const Int16 &inlay_STR, const Int16 &inlay_CON, const Int16 &inlay_AGI, const Int16 &inlay_ATK, const Int16 &inlay_DEF, const Int16 &inlay_HIT, const Int16 &inlay_EVA, const Int16 &inlay_POW, const Int16 &inlay_SPD, const Int16 &inlay_maxHP, const Int16 &inlay_maxSP, const Int16 &inlay_maxVP) {};
	virtual void OnUpdateAddon(const UInt16 &addon) {};
	virtual void OnUpdateEqColor(const UInt16 &eqColor) {};
	virtual void OnUpdateDetailLv(const UInt8 &detailLv) {};
	virtual void OnUpdateQuality(const UInt16 &item_quality) {};
	virtual void OnUpdateSoulboundState(const UInt8 &item_soulboundState) {};
	virtual void OnUpdateIdentifyState(const UInt8 &item_IdentifyState) {};
	virtual void OnUpdateExpireDate(const UInt32 &item_expireDate) {};
	virtual void OnUpdateValue(const UInt16 &itemValue1, const UInt16 &itemValue2, const UInt16 &itemValue3) {};
	virtual void OnUpdateBuffID(const UInt16 &buffID) {};
	virtual void OnUpdateBuffValue1(const UInt32 &buffValue1) {};
	virtual void OnUpdateBuffValue2(const UInt32 &buffValue2) {};
	virtual void OnUpdateBuffValue3(const UInt32 &buffValue3) {};
	virtual void OnUpdateBuffCoolDown(const UInt32 &buffCoolDown) {};
	virtual void OnUpdateBuffPro(const UInt8 &buffPro) {};
	virtual void OnUpdateInWareHouse(const UInt8 &inWareHouse) {};
	virtual void OnUpdateAssetLock(const Boolean &lock) {};
	virtual void OnUpdateSkillID(const UInt16 &skillID) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	//detailLv
	UInt8 detailLv;
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
	INLINE Item_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Item_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Item_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<2078244581>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(98268314)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2078244581)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_ITEM_PROXY_H_
