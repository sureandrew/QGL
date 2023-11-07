///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTNER_PROXY_H_
#define _RPC_PARTNER_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Partner_enum.h"

class Partner_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_ChangePartnerName(LPCPEER pPeer, RPCContext &context, const bool &success);
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_UpdateSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID);
	virtual RPCResult CB_FalseJoinBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &partner_id, const UInt32 &mob_id);
	virtual void OnUpdateOwner(const UInt32 &owner_id);
	virtual void OnUpdateInfo(const String &raname);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateNextLvExp(const UInt32 &nextLvExp);
	virtual void OnUpdateExp(const UInt32 &exp);
	virtual void OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);
	virtual void OnUpdateAttrPoint(const UInt16 &attrPoint);
	virtual void OnUpdateHP(const UInt32 &HP, const UInt32 &maxHP);
	virtual void OnUpdateSP(const UInt32 &SP, const UInt32 &maxSP);
	virtual void OnUpdateNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native);
	virtual void OnUpdateGrow(const float &grow);
	virtual void OnUpdateFavor(const float &favor);
	virtual void OnUpdateHealth(const UInt16 &health);
	virtual void OnUpdateGoodness(const Int32 &goodness);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdateSoulboundState(const UInt8 &soulboundState);
	virtual void OnUpdateExpireDate(const UInt32 &expireDate);
	virtual void OnUpdateElement(const UInt8 &element);
	virtual void OnUpdateOutLookColor(const UInt16 &outLookColor);
	virtual void OnUpdateBuffCache(const String &buffCache);
	virtual void OnUpdateInWareHouse(const UInt8 &inWareHouse);
	virtual void OnUpdateAssetLock(const Boolean &lock);
	virtual void OnUpdateCultivation(const UInt16 &cultivation);
	virtual void OnUpdatePractice(const UInt16 &practice);
	virtual void OnUpdateGrowLevel(const UInt8 &growLevel);
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
	virtual RPCResult RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi);
	virtual RPCResult ChangePartnerName(LPCPEER pPeer, RPCContext &context, const String &newName);
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt8 &state);
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_ChangePartnerName(LPCPEER pPeer, RPCContext &context, const bool &success) = 0;
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_UpdateSuccess(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID) = 0;
	virtual RPCResult CB_FalseJoinBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType) = 0;
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
	virtual void OnUpdateId(const UInt32 &partner_id, const UInt32 &mob_id) {};
	virtual void OnUpdateOwner(const UInt32 &owner_id) {};
	virtual void OnUpdateInfo(const String &raname) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void OnUpdateNextLvExp(const UInt32 &nextLvExp) {};
	virtual void OnUpdateExp(const UInt32 &exp) {};
	virtual void OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI) {};
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD) {};
	virtual void OnUpdateAttrPoint(const UInt16 &attrPoint) {};
	virtual void OnUpdateHP(const UInt32 &HP, const UInt32 &maxHP) {};
	virtual void OnUpdateSP(const UInt32 &SP, const UInt32 &maxSP) {};
	virtual void OnUpdateNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native) {};
	virtual void OnUpdateGrow(const float &grow) {};
	virtual void OnUpdateFavor(const float &favor) {};
	virtual void OnUpdateHealth(const UInt16 &health) {};
	virtual void OnUpdateGoodness(const Int32 &goodness) {};
	virtual void OnUpdateState(const UInt8 &state) {};
	virtual void OnUpdateBuff(const String &buff) {};
	virtual void OnUpdateSoulboundState(const UInt8 &soulboundState) {};
	virtual void OnUpdateExpireDate(const UInt32 &expireDate) {};
	virtual void OnUpdateElement(const UInt8 &element) {};
	virtual void OnUpdateOutLookColor(const UInt16 &outLookColor) {};
	virtual void OnUpdateBuffCache(const String &buffCache) {};
	virtual void OnUpdateInWareHouse(const UInt8 &inWareHouse) {};
	virtual void OnUpdateAssetLock(const Boolean &lock) {};
	virtual void OnUpdateCultivation(const UInt16 &cultivation) {};
	virtual void OnUpdatePractice(const UInt16 &practice) {};
	virtual void OnUpdateGrowLevel(const UInt8 &growLevel) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE Partner_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Partner_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1403781319>(pPeer, pBuf);
}

INLINE RPCResult Partner_Proxy::RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_RaiseAttrib);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt16>(sta);
	pBuf->Write<UInt16>(spr);
	pBuf->Write<UInt16>(str);
	pBuf->Write<UInt16>(con);
	pBuf->Write<UInt16>(agi);
	return RPCSend<2124095243>(pPeer, pBuf);
}

INLINE RPCResult Partner_Proxy::ChangePartnerName(LPCPEER pPeer, RPCContext &context, const String &newName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_ChangePartnerName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(newName);
	return RPCSend<1642414924>(pPeer, pBuf);
}

INLINE RPCResult Partner_Proxy::ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt8 &state)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Partner_ChangePartnerState);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(state);
	return RPCSend<584076774>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(169612404)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1403781319)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2124095243)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1642414924)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(584076774)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(318600191)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(469255547)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1103260278)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(371536329)
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1173311946)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(649489524)
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_PARTNER_PROXY_H_
