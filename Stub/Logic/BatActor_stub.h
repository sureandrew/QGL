///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATACTOR_STUB_H_
#define _RPC_BATACTOR_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/BatActor_enum.h"

class BatActor_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, const UInt8 &target, const UInt32 &sub_type, const UInt32 &additional);
	virtual RPCResult CheatCode(LPCPEER pPeer, RPCContext &context, const String &code);
	virtual RPCResult CancelAuto(LPCPEER pPeer, RPCContext &context);
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
	virtual RPCResult SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, const UInt8 &target, const UInt32 &sub_type, const UInt32 &additional) = 0;
	virtual RPCResult CheatCode(LPCPEER pPeer, RPCContext &context, const String &code) = 0;
	virtual RPCResult CancelAuto(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_CommandSuccess(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd);
	virtual RPCResult CB_Inputed(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd);
	virtual RPCResult CB_AutoCount(LPCPEER pPeer, RPCContext &context, const UInt16 &autoCount);
	virtual RPCResult CB_BattleMsg(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType, const String &Msg, const UInt16 &subType);
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
	void SetInfo2(const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	void SetCclass(const UInt8 &cclass);
	void SetSexClass(const UInt8 &sexClass);
	void SetFaction(const UInt8 &faction);
	void SetIndex(const UInt8 &index);
	void SetLevel(const UInt16 &level);
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
	void SetHP(const UInt32 &HP, const Int32 &dHP, const Int32 &dShowHP);
	void SetHP(const UInt32 &HP);
	void SetDHP(const Int32 &dHP);
	void SetDShowHP(const Int32 &dShowHP);
	void SetMHP(const UInt32 &mHP, const Int32 &dMHP, const Int32 &dShowMHP);
	void SetMHP(const UInt32 &mHP);
	void SetDMHP(const Int32 &dMHP);
	void SetDShowMHP(const Int32 &dShowMHP);
	void SetMaxHP(const UInt32 &maxHP);
	void SetSP(const UInt32 &SP, const Int32 &dSP, const Int32 &dShowSP);
	void SetSP(const UInt32 &SP);
	void SetDSP(const Int32 &dSP);
	void SetDShowSP(const Int32 &dShowSP);
	void SetMSP(const UInt32 &mSP, const Int32 &dMSP, const Int32 &dShowMSP);
	void SetMSP(const UInt32 &mSP);
	void SetDMSP(const Int32 &dMSP);
	void SetDShowMSP(const Int32 &dShowMSP);
	void SetMaxSP(const UInt32 &maxSP);
	void SetDP(const UInt16 &DP, const Int16 &dDP, const Int16 &dShowDP);
	void SetDP(const UInt16 &DP);
	void SetDDP(const Int16 &dDP);
	void SetDShowDP(const Int16 &dShowDP);
	void SetMDP(const UInt16 &mDP);
	void SetMaxDP(const UInt16 &maxDP);
	void SetWC(const UInt16 &WC, const Int16 &dWC, const Int16 &dShowWC);
	void SetWC(const UInt16 &WC);
	void SetDWC(const Int16 &dWC);
	void SetDShowWC(const Int16 &dShowWC);
	void SetWcExp(const UInt16 &WcExp);
	void SetMaxWC(const UInt16 &maxWC);
	void SetVP(const UInt16 &VP, const Int16 &dVP, const Int16 &dShowVP);
	void SetVP(const UInt16 &VP);
	void SetDVP(const Int16 &dVP);
	void SetDShowVP(const Int16 &dShowVP);
	void SetMaxVP(const UInt16 &maxVP);
	void SetState(const UInt8 &state);
	void SetBuff(const String &buff);
	void SetBuffCache(const String &buffCache);
	void SetTeam(const UInt8 &team);
	void SetPosition(const UInt8 &position);
	void SetAction(const UInt8 &action);
	void SetReaction(const UInt8 &reaction);
	void SetTarget(const UInt8 &target);
	void SetSubType(const UInt32 &subType);
	void SetAdditional(const UInt32 &additional);
	void SetLinkNext(const Boolean &linkNext);
	void SetLeftBattle(const Boolean &leftBattle);
	void SetGoodness(const Int32 &goodness);
	void SetPet(const UInt32 &pet_uid, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex);
	void SetPet_uid(const UInt32 &pet_uid);
	void SetPet_id(const UInt32 &pet_id);
	void SetPetName(const String &petName);
	void SetPetColorIndex(const UInt16 &petColorIndex);
	void SetPet_active(const bool &pet_active);
	void SetTP(const UInt16 &TP);
	void SetHitTogether(const Boolean &hitTogether);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE BatActor_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_BATACTOR_STUB_H_
