///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATACTOR_PROXY_H_
#define _RPC_BATACTOR_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/BatActor_enum.h"

class BatActor_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_CommandSuccess(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd);
	virtual RPCResult CB_Inputed(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd);
	virtual RPCResult CB_AutoCount(LPCPEER pPeer, RPCContext &context, const UInt16 &autoCount);
	virtual RPCResult CB_BattleMsg(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType, const String &Msg, const UInt16 &subType);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo2(const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	virtual void OnUpdateIndex(const UInt8 &index);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);
	virtual void OnUpdateHP(const UInt32 &HP, const Int32 &dHP, const Int32 &dShowHP);
	virtual void OnUpdateMHP(const UInt32 &mHP, const Int32 &dMHP, const Int32 &dShowMHP);
	virtual void OnUpdateMaxHP(const UInt32 &maxHP);
	virtual void OnUpdateSP(const UInt32 &SP, const Int32 &dSP, const Int32 &dShowSP);
	virtual void OnUpdateMSP(const UInt32 &mSP, const Int32 &dMSP, const Int32 &dShowMSP);
	virtual void OnUpdateMaxSP(const UInt32 &maxSP);
	virtual void OnUpdateDP(const UInt16 &DP, const Int16 &dDP, const Int16 &dShowDP);
	virtual void OnUpdateMDP(const UInt16 &mDP);
	virtual void OnUpdateMaxDP(const UInt16 &maxDP);
	virtual void OnUpdateWC(const UInt16 &WC, const Int16 &dWC, const Int16 &dShowWC);
	virtual void OnUpdateWcExp(const UInt16 &WcExp);
	virtual void OnUpdateMaxWC(const UInt16 &maxWC);
	virtual void OnUpdateVP(const UInt16 &VP, const Int16 &dVP, const Int16 &dShowVP);
	virtual void OnUpdateMaxVP(const UInt16 &maxVP);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdateBuffCache(const String &buffCache);
	virtual void OnUpdateTeam(const UInt8 &team);
	virtual void OnUpdatePosition(const UInt8 &position);
	virtual void OnUpdateAction(const UInt8 &action);
	virtual void OnUpdateReaction(const UInt8 &reaction);
	virtual void OnUpdateTarget(const UInt8 &target);
	virtual void OnUpdateSubType(const UInt32 &subType);
	virtual void OnUpdateAdditional(const UInt32 &additional);
	virtual void OnUpdateLinkNext(const Boolean &linkNext);
	virtual void OnUpdateLeftBattle(const Boolean &leftBattle);
	virtual void OnUpdateGoodness(const Int32 &goodness);
	virtual void OnUpdatePet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex);
	virtual void OnUpdatePet_active(const bool &pet_active);
	virtual void OnUpdateTP(const UInt16 &TP);
	virtual void OnUpdateHitTogether(const Boolean &hitTogether);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		info2Mask	= Parent::NextFreeMask + 0,
		indexMask	= Parent::NextFreeMask + 1,
		levelMask	= Parent::NextFreeMask + 2,
		attrib1Mask	= Parent::NextFreeMask + 3,
		attrib2Mask	= Parent::NextFreeMask + 4,
		HPMask	= Parent::NextFreeMask + 5,
		mHPMask	= Parent::NextFreeMask + 6,
		maxHPMask	= Parent::NextFreeMask + 7,
		SPMask	= Parent::NextFreeMask + 8,
		mSPMask	= Parent::NextFreeMask + 9,
		maxSPMask	= Parent::NextFreeMask + 10,
		DPMask	= Parent::NextFreeMask + 11,
		mDPMask	= Parent::NextFreeMask + 12,
		maxDPMask	= Parent::NextFreeMask + 13,
		WCMask	= Parent::NextFreeMask + 14,
		WcExpMask	= Parent::NextFreeMask + 15,
		maxWCMask	= Parent::NextFreeMask + 16,
		VPMask	= Parent::NextFreeMask + 17,
		maxVPMask	= Parent::NextFreeMask + 18,
		stateMask	= Parent::NextFreeMask + 19,
		buffMask	= Parent::NextFreeMask + 20,
		buffCacheMask	= Parent::NextFreeMask + 21,
		teamMask	= Parent::NextFreeMask + 22,
		positionMask	= Parent::NextFreeMask + 23,
		actionMask	= Parent::NextFreeMask + 24,
		reactionMask	= Parent::NextFreeMask + 25,
		targetMask	= Parent::NextFreeMask + 26,
		subTypeMask	= Parent::NextFreeMask + 27,
		additionalMask	= Parent::NextFreeMask + 28,
		linkNextMask	= Parent::NextFreeMask + 29,
		leftBattleMask	= Parent::NextFreeMask + 30,
		goodnessMask	= Parent::NextFreeMask + 31,
		petMask	= Parent::NextFreeMask + 32,
		pet_activeMask	= Parent::NextFreeMask + 33,
		TPMask	= Parent::NextFreeMask + 34,
		hitTogetherMask	= Parent::NextFreeMask + 35,
		NextFreeMask	= Parent::NextFreeMask + 36
	};
	virtual RPCResult SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, const UInt8 &target, const UInt32 &sub_type, const UInt32 &additional);
	virtual RPCResult CheatCode(LPCPEER pPeer, RPCContext &context, const String &code);
	virtual RPCResult CancelAuto(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg) = 0;
	virtual RPCResult CB_CommandSuccess(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd) = 0;
	virtual RPCResult CB_Inputed(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd) = 0;
	virtual RPCResult CB_AutoCount(LPCPEER pPeer, RPCContext &context, const UInt16 &autoCount) = 0;
	virtual RPCResult CB_BattleMsg(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType, const String &Msg, const UInt16 &subType) = 0;
	const UInt8& GetCclass() const { return m.cclass; }
	const UInt8& GetSexClass() const { return m.sexClass; }
	const UInt8& GetFaction() const { return m.faction; }
	const UInt8& GetIndex() const { return m.index; }
	const UInt16& GetLevel() const { return m.level; }
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
	const UInt32& GetHP() const { return m.HP; }
	const Int32& GetDHP() const { return m.dHP; }
	const Int32& GetDShowHP() const { return m.dShowHP; }
	const UInt32& GetMHP() const { return m.mHP; }
	const Int32& GetDMHP() const { return m.dMHP; }
	const Int32& GetDShowMHP() const { return m.dShowMHP; }
	const UInt32& GetMaxHP() const { return m.maxHP; }
	const UInt32& GetSP() const { return m.SP; }
	const Int32& GetDSP() const { return m.dSP; }
	const Int32& GetDShowSP() const { return m.dShowSP; }
	const UInt32& GetMSP() const { return m.mSP; }
	const Int32& GetDMSP() const { return m.dMSP; }
	const Int32& GetDShowMSP() const { return m.dShowMSP; }
	const UInt32& GetMaxSP() const { return m.maxSP; }
	const UInt16& GetDP() const { return m.DP; }
	const Int16& GetDDP() const { return m.dDP; }
	const Int16& GetDShowDP() const { return m.dShowDP; }
	const UInt16& GetMDP() const { return m.mDP; }
	const UInt16& GetMaxDP() const { return m.maxDP; }
	const UInt16& GetWC() const { return m.WC; }
	const Int16& GetDWC() const { return m.dWC; }
	const Int16& GetDShowWC() const { return m.dShowWC; }
	const UInt16& GetWcExp() const { return m.WcExp; }
	const UInt16& GetMaxWC() const { return m.maxWC; }
	const UInt16& GetVP() const { return m.VP; }
	const Int16& GetDVP() const { return m.dVP; }
	const Int16& GetDShowVP() const { return m.dShowVP; }
	const UInt16& GetMaxVP() const { return m.maxVP; }
	const UInt8& GetState() const { return m.state; }
	const String& GetBuff() const { return m.buff; }
	const String& GetBuffCache() const { return m.buffCache; }
	const UInt8& GetTeam() const { return m.team; }
	const UInt8& GetPosition() const { return m.position; }
	const UInt8& GetAction() const { return m.action; }
	const UInt8& GetReaction() const { return m.reaction; }
	const UInt8& GetTarget() const { return m.target; }
	const UInt32& GetSubType() const { return m.subType; }
	const UInt32& GetAdditional() const { return m.additional; }
	const Boolean& GetLinkNext() const { return m.linkNext; }
	const Boolean& GetLeftBattle() const { return m.leftBattle; }
	const Int32& GetGoodness() const { return m.goodness; }
	const UInt32& GetPet_uid() const { return m.pet_uid; }
	const UInt32& GetPet_id() const { return m.pet_id; }
	const String& GetPetName() const { return m.petName; }
	const UInt16& GetPetColorIndex() const { return m.petColorIndex; }
	const bool& GetPet_active() const { return m.pet_active; }
	const UInt16& GetTP() const { return m.TP; }
	const Boolean& GetHitTogether() const { return m.hitTogether; }
	virtual void OnUpdateInfo2(const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction) {};
	virtual void OnUpdateIndex(const UInt8 &index) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI) {};
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD) {};
	virtual void OnUpdateHP(const UInt32 &HP, const Int32 &dHP, const Int32 &dShowHP) {};
	virtual void OnUpdateMHP(const UInt32 &mHP, const Int32 &dMHP, const Int32 &dShowMHP) {};
	virtual void OnUpdateMaxHP(const UInt32 &maxHP) {};
	virtual void OnUpdateSP(const UInt32 &SP, const Int32 &dSP, const Int32 &dShowSP) {};
	virtual void OnUpdateMSP(const UInt32 &mSP, const Int32 &dMSP, const Int32 &dShowMSP) {};
	virtual void OnUpdateMaxSP(const UInt32 &maxSP) {};
	virtual void OnUpdateDP(const UInt16 &DP, const Int16 &dDP, const Int16 &dShowDP) {};
	virtual void OnUpdateMDP(const UInt16 &mDP) {};
	virtual void OnUpdateMaxDP(const UInt16 &maxDP) {};
	virtual void OnUpdateWC(const UInt16 &WC, const Int16 &dWC, const Int16 &dShowWC) {};
	virtual void OnUpdateWcExp(const UInt16 &WcExp) {};
	virtual void OnUpdateMaxWC(const UInt16 &maxWC) {};
	virtual void OnUpdateVP(const UInt16 &VP, const Int16 &dVP, const Int16 &dShowVP) {};
	virtual void OnUpdateMaxVP(const UInt16 &maxVP) {};
	virtual void OnUpdateState(const UInt8 &state) {};
	virtual void OnUpdateBuff(const String &buff) {};
	virtual void OnUpdateBuffCache(const String &buffCache) {};
	virtual void OnUpdateTeam(const UInt8 &team) {};
	virtual void OnUpdatePosition(const UInt8 &position) {};
	virtual void OnUpdateAction(const UInt8 &action) {};
	virtual void OnUpdateReaction(const UInt8 &reaction) {};
	virtual void OnUpdateTarget(const UInt8 &target) {};
	virtual void OnUpdateSubType(const UInt32 &subType) {};
	virtual void OnUpdateAdditional(const UInt32 &additional) {};
	virtual void OnUpdateLinkNext(const Boolean &linkNext) {};
	virtual void OnUpdateLeftBattle(const Boolean &leftBattle) {};
	virtual void OnUpdateGoodness(const Int32 &goodness) {};
	virtual void OnUpdatePet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex) {};
	virtual void OnUpdatePet_active(const bool &pet_active) {};
	virtual void OnUpdateTP(const UInt16 &TP) {};
	virtual void OnUpdateHitTogether(const Boolean &hitTogether) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//info2
	UInt8 cclass;
	UInt8 sexClass;
	UInt8 faction;
	//index
	UInt8 index;
	//level
	UInt16 level;
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
	//HP
	UInt32 HP;
	Int32 dHP;
	Int32 dShowHP;
	//mHP
	UInt32 mHP;
	Int32 dMHP;
	Int32 dShowMHP;
	//maxHP
	UInt32 maxHP;
	//SP
	UInt32 SP;
	Int32 dSP;
	Int32 dShowSP;
	//mSP
	UInt32 mSP;
	Int32 dMSP;
	Int32 dShowMSP;
	//maxSP
	UInt32 maxSP;
	//DP
	UInt16 DP;
	Int16 dDP;
	Int16 dShowDP;
	//mDP
	UInt16 mDP;
	//maxDP
	UInt16 maxDP;
	//WC
	UInt16 WC;
	Int16 dWC;
	Int16 dShowWC;
	//WcExp
	UInt16 WcExp;
	//maxWC
	UInt16 maxWC;
	//VP
	UInt16 VP;
	Int16 dVP;
	Int16 dShowVP;
	//maxVP
	UInt16 maxVP;
	//state
	UInt8 state;
	//buff
	String buff;
	//buffCache
	String buffCache;
	//team
	UInt8 team;
	//position
	UInt8 position;
	//action
	UInt8 action;
	//reaction
	UInt8 reaction;
	//target
	UInt8 target;
	//subType
	UInt32 subType;
	//additional
	UInt32 additional;
	//linkNext
	Boolean linkNext;
	//leftBattle
	Boolean leftBattle;
	//goodness
	Int32 goodness;
	//pet
	UInt32 pet_uid;
	UInt32 pet_id;
	String petName;
	UInt16 petColorIndex;
	//pet_active
	bool pet_active;
	//TP
	UInt16 TP;
	//hitTogether
	Boolean hitTogether;
} m;

public:
	INLINE BatActor_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult BatActor_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1940390731>(pPeer, pBuf);
}

INLINE RPCResult BatActor_Proxy::SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, const UInt8 &target, const UInt32 &sub_type, const UInt32 &additional)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_SendCommand);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(cmd);
	pBuf->Write<UInt8>(target);
	pBuf->Write<UInt32>(sub_type);
	pBuf->Write<UInt32>(additional);
	return RPCSend<1036143315>(pPeer, pBuf);
}

INLINE RPCResult BatActor_Proxy::CheatCode(LPCPEER pPeer, RPCContext &context, const String &code)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_CheatCode);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(code);
	return RPCSend<2117226740>(pPeer, pBuf);
}

INLINE RPCResult BatActor_Proxy::CancelAuto(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_BatActor_CancelAuto);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1743848763>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1902991750)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1940390731)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1036143315)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2117226740)
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1743848763)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1236285136)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(766788022)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(91956427)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(573711624)
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(71531305)
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_BATACTOR_PROXY_H_
