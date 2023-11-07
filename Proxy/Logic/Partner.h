#pragma once
#ifndef _PARTNER_H_
#define _PARTNER_H_

#include "Proxy/Logic/Partner_Proxy.h"

class CSkill;
class CSkillGroup;
struct MobDB_Client;

class CPartner : public Partner_Proxy
{
public:
	Int m_iBattleHP;
	Int m_iBattleSP;
	Int m_iBattleMaxHP;

	Int m_iBattleATK;
	Int m_iBattleDEF;
	Int m_iBattleHIT;
	Int m_iBattleEVA;
	Int m_iBattlePOW;
	Int m_iBattleSPD;
	bool m_bGiven;
	//bool m_bLevelUpSound;
	CSkillGroup * m_pSkillGrp;
	MobDB_Client * m_data;

	enum StateEnum
	{
		STATE_STANDBY = 0,
		STATE_FIGHT = 1,
		STATE_REST = 2,
	};

	DEFINE_CLASS(Partner_CLASSID);

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
	virtual void OnUpdateGrow(const float &grow);
	virtual void OnUpdateFavor(const float &favor);
	virtual void OnUpdateHealth(const UInt16 &health);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdateNative(const UInt16 &HP_native, const UInt16 &SP_native, const UInt16 &ATK_native, const UInt16 &DEF_native, const UInt16 &SPD_native, const UInt16 &POW_native, const UInt16 &HIT_native, const UInt16 &EVA_native);
	virtual void OnUpdateGoodness(const Int32 &goodness);
	virtual void OnUpdateOutLookColor(const UInt16 &outLookColor);
	virtual void OnUpdateAssetLock(const Boolean &lock);	
	virtual void OnUpdateCultivation(const UInt16 &cultivation);
	virtual void OnUpdatePractice(const UInt16 &practice);
	virtual void OnUpdateGrowLevel(const UInt8 &growLevel);

	void SetBattleHP(Int bhp);
	void SetBattleMaxHP(Int bmaxhp);
	void SetBattleSP(Int bsp);
	INLINE Int GetBattleHP(){return m_iBattleHP;}
	INLINE Int GetBattleSP(){return m_iBattleSP;}
	INLINE Int GetBattleMaxHP(){return m_iBattleMaxHP;}
	INLINE void SetGiven(bool given){ m_bGiven = given;}
	INLINE bool IsGiven(){return m_bGiven;}
	void SetSkillGroup(CSkillGroup* skillGroup);
	CPartner();
	virtual ~CPartner();
	INLINE void SetAttrib2(Int ATK, Int DEF, Int HIT, Int EVA, Int POW, Int SPD)
	{
		m_iBattleATK = ATK;
		m_iBattleDEF = DEF;
		m_iBattleHIT = HIT;
		m_iBattleEVA = EVA;
		m_iBattlePOW = POW;
		m_iBattleSPD = SPD;
	}
	INLINE Int GetBattleATK(){return m_iBattleATK;}
	INLINE Int GetBattleDEF(){return m_iBattleDEF;}
	INLINE Int GetBattleHIT(){return m_iBattleHIT;}
	INLINE Int GetBattleEVA(){return m_iBattleEVA;}
	INLINE Int GetBattlePOW(){return m_iBattlePOW;}
	INLINE Int GetBattleSPD(){return m_iBattleSPD;}

	Boolean CanCall();
	UInt16 GetSkillCount();
	CSkill* GetSkill(UInt16 i);
	CSkill* GetSkillById(UInt16 skill_id);

private:
	typedef Partner_Proxy Parent;

};

#endif //_PARTNER_H_
