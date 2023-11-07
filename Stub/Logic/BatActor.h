#pragma once
#ifndef _BATACTOR_H_
#define _BATACTOR_H_

#include "Stub/Logic/BatActor_Stub.h"
#include "Common/StateCode.h"
#include "Common/common_binary.h"
#include "Common/BattleCode.h"

class CBatCharacter;
class CBatMob;
class CBattle;
struct BatClient;
class CBatActor;
class CItemBag;
class CSkill;
struct SkillData;
class CItem;
class CPet;
class CScriptSystem;
class CCharacter;

#define	HOLDHURTTIME 500
//typedef StlVector<CBatActor*, ManagedAllocator<CBatActor*> > Actors;

CONST StringA validActionTargetStr = "ValidActionTarget";

struct AttriChangeData
{
	Int HpChange;
	Int SpChange;
	Int DpChange;
	Int WcChange;
	UInt8 reaction;
	Int	action;

	AttriChangeData()
		: HpChange(C_INT(0))
		, SpChange(C_INT(0))
		, DpChange(C_INT(0))
		, WcChange(C_INT(0))
		, reaction(C_UINT8(0))
		, action(C_INT(-1))
	{}

	AttriChangeData(Int hp, Int sp, Int dp, Int wc, UInt8 react, int act)
	{
		HpChange = hp;
		SpChange = sp;
		DpChange = dp;
		WcChange = wc;
		reaction = react;
		action = act;
	}
};

typedef StlMap<UInt16, const SkillData *, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, SkillData*> > > BlockSkillMap;
typedef StlVector<AttriChangeData, ManagedAllocator<AttriChangeData> > AttriChangeDataList;

class CBatActor : public BatActor_Stub
{
	typedef StlVector<RPCNetID, ManagedAllocator<RPCNetID> > Actors;
public:

	typedef StlMap<UInt16, CSkill*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CSkill*> > > SkillMap;

	virtual RPCResult SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, const UInt8 &target, const UInt32 &sub_type, const UInt32 &additional);
	virtual RPCResult CheatCode(LPCPEER pPeer, RPCContext &context, const String &code);
	virtual RPCResult CancelAuto(LPCPEER pPeer, RPCContext &context);

	DEFINE_CLASS(BatActor_CLASSID);
	CBatActor();
	virtual ~CBatActor();
	INLINE CBattle* GetBattle() { return m_pBattle; }
	INLINE void SetBattle(CBattle *pBattle) { m_pBattle = pBattle; }
	virtual UInt8 GetSex() { return 0; }
	virtual void DoAction();
	virtual void DoAttack();
	virtual void DoComboAttack(Actors &fellowList);
	virtual void DoSkill();
	virtual void DoSpecial();
	virtual void DoItem();
	virtual void DoWine();
	virtual void DoFriend();
	virtual void DoCatch();
	virtual void DoHelp();
	virtual void DoEscape();
	virtual void DoEvent();
	virtual void WriteAction();
	virtual void WriteReaction();
	virtual void TurnClear();
	virtual void PreSend();					// prepare to send
	virtual void PreSendEnemy();
	virtual bool IsLeader(){return false;}
	INLINE virtual Boolean GetHelp() { return BOOLEAN_FALSE; }
	void InputCommand(UInt8 cmd, UInt8 target=0, UInt32 sub_type=0, 
						UInt32 additional_type=0);
	Boolean IsValidTarget(Boolean bAlive=BOOLEAN_TRUE);
	bool IsValidTargetType(UInt8 targetType, CBatActor * pTarget);
	INLINE Boolean IsDead() { return GetState() == CHAR_ST_DEAD; }
	INLINE Boolean IsEscaped() { return GetState() == CHAR_ST_ESCAPED; }
	INLINE Boolean IsBackup() { return GetState() == CHAR_ST_BACKUP; }
	INLINE Boolean IsSurrender() { return GetState() == CHAR_ST_SURRENDER; }
	INLINE Boolean IsHitTarget(CBatActor *pTarget);
	INLINE Boolean IsCatched(){return GetState() == CHAR_ST_CATCHED; }
	void AutoInput();
	void EscapeTogether();
	CBatActor* NextTarget(Boolean bEnemy = BOOLEAN_TRUE, Boolean bSelfInc = BOOLEAN_FALSE);
	float CalcDamage(CBatActor *pTarget, UInt32 atk, Boolean bAction = BOOLEAN_TRUE, 
					Boolean bBreak = BOOLEAN_TRUE);
	void HitAttack(CBatActor* pTarget, CBatActor* &protector, 
					UInt32 atk, Boolean bBreak, Boolean bCritHit);
	INLINE CONST SkillMap& GetSkillMap(VOID) CONST { return (m_SkillMap); }
	INLINE VOID SetSkillMap(CONST SkillMap& sm) { m_SkillMap = sm; }
	void UpdateSkillBuff(VOID);
	void ChangeHP(Int change, bool show = true);
	void ChangeSP(Int change, bool show = true);
	void ChangeMHP(Int change, bool show = true);
	void ChangeMSP(Int change, bool show = true);
	INLINE virtual void ChangeDP(Int change, bool show = true) {}
	INLINE virtual void ChangeTP(Int change, bool show = true) {}
	INLINE virtual void ChangeWC(Int change, bool show = true) {}
	INLINE virtual void ChangeVP(Int change, bool show = true) {}
	INLINE virtual void ChangeWcExp(Int change){}
	void ResetHP();
	void ResetMHP();
	void ResetSP();
	void ResetMSP();
	INLINE virtual void ResetDP() {}
	INLINE virtual void ResetVP() {}
	INLINE virtual void SetState(const UInt8 &state) { Parent::SetState(state); }
	void CheckDeadStatus();
	UInt16 GetSkillLv(UInt16 ID);
	CSkill* GetSkill(UInt16 skillId);
	void SetSkillCoolDown(UInt16 ID, DWORD countDown);
	INLINE void SkillFail() { m_ReturnType = 1; }
	void SkillFail(PCSTRINGA Text);

	inline void ClearReturnType(){ m_ReturnType = 0; }
	inline BYTE GetReturnType(){return m_ReturnType;}

	void DefaultInput();
	void MissSkill();
	bool HasSkill(UInt16 SkillId);
	bool AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void RemoveBuff(UInt16 Buff_id);
	void RemoveBuffSchool(UInt16 school_id);
	ExtractedBuffData * FindBuff(UInt16 Buff_id);
	bool HasBuff(UInt16 Buff_id);
	INLINE virtual UInt32 GetActorId() { return 0; }
	void ExtractBuffString();
	void ConstructBuffString();
	void ExtractBuffCacheString();
	void ConstructBuffCacheString();
	INLINE Boolean IsPlayer() { return (m_pBatChar != NULL); }
	INLINE Boolean IsMob() { return (m_pMob != NULL); }
	INLINE virtual bool IsParnter() { return false; }
	INLINE virtual bool IsBoss() { return false; }
	INLINE virtual bool IsGuest() { return false; }
	UInt16 GetWeaponATK();
	UInt16 GetSkillItemATK();
	void AddAttriChangeToLst(int hpChange, int spChange, int dpChange, int wcChange, UInt8 react, int action = -1);
	INLINE virtual void CalcAttrib() {}			// calculate the final attributes
	void DoBuffActive(CBatActor * pActor, BYTE ActiveType, StlVector<CBatActor *> * activeList = NULL);
	INLINE void BuffActive(){m_BuffActive = true;}
	void UpdateBuff(UInt32 DeTime);
	void PackBuff();
	Boolean IsCmdBlocked(UInt8 cmd, Boolean atInput=FALSE);
	INLINE virtual bool AllCmdBlock() { return true; }
	void UpdateBlockCmd();
	void UpdateBlockSkill();
	bool IsAntiBuff(UInt16 Buff_id);
	void SetPostSkill(UInt16 skillId, UInt8 targetIndex, UInt8 turn, UInt16 skillBaseLv);
	INLINE bool HasPostSkill() { return m_postSkillID > 0; }
	void ClearPostSkill();
	void DoDefaultAttack(CBatActor * pBatTarget, int Dam, int crit);
	INLINE UInt8 GetCommand() { return m_command; }
	void SetTempReaction(UInt8 reaction);
	void SetTempAction(UInt8 action);
	UInt16 GetRandomSkill(int skill_form = -1);
	void SetAIType(PCSTRINGA value);
	void CallHelper(UInt32 mob_id, UInt16 Level, UInt8 pos);
	const SkillData * GetSkillCsvData(UInt16 skill_id);
	
	void ResetDropValue();
	void RestoreDropValue();

	void DoAction(CBatActor * pBatTarget);
	INLINE void IgnoreTeam(bool value) { m_ignoreTeam = value; }
	void RemoveAllCacheBuff();
	void RemoveBuffAfterBattle();
	void ChangeTarget(UInt8 target);
	void TestChangeAttri(int type, int value);
	void ChangePlayerBatCmd(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional_type);

	void UseSkillMsg(PCSTRINGA Text);
	void DoContinueAction(CBatActor * pBatActor, bool LastOne, UInt32 & Time);
	void DoAttriAction(CBatActor * pBatActor, bool LastOne, UInt32 & Time, AttriChangeData data, bool first, bool LastHit);
	void CancelAction();
	void ItemFail(PCSTRINGA Text);
	void UseItemMsg(PCSTRINGA Text);
	void RemoveBuffAfterDead();
	bool IsProtectActive();
	UInt32 GetContActCount();
	INLINE UInt32 GetCurContAct() { return (m_buffActing ? m_CurBuffContAct : m_CurContAct); }
	INLINE bool IsLastHit() { return m_isLastHit; } 
	bool HasParty();
	bool CheckBlockItem(UInt8 itemType);
	bool TryAction();
	void ResetAction();
	INLINE virtual void InitPrevCmd() {}
	INLINE virtual void SaveLastCmd() {}
	void SendBattleMsg(UInt8 msgType, const String &msg, UInt16 subType);
	void SendCommandSuccess(UInt8 cmd);

	inline void SetPunish(bool value){m_punish = value;}
	bool GetPunish(){ return m_punish;}
	inline void SetJumpBack(bool value){m_jumpBack = value;}

	CItem * FindItemByid(UInt32 item_id);

	CPet * GetUsingPet();
	void PetActive();

	CScriptSystem * GetScript();
	INLINE virtual CCharacter* GetOwner() { return NULL; }

	inline void SetBoundsExp(UInt32 exp){ m_BoundsExp = exp;}
	inline UInt32 GetBoundsExp(){ return m_BoundsExp;}
	void WineUpdate();
	void ClearWineBuff();
	void SetIsInput(Boolean input);
	bool DetectBuff(UInt16 Buff_ID);
	UInt8	GetUseItemType();

	void CalSpeed();

	inline Int32 GetRealDHP(){ return m_RDHP;}
	inline Int32 GetRealDMHP(){ return m_RDMHP;}
	inline Int32 GetRealDSP(){ return m_RDSP;}
	inline Int32 GetRealDMSP(){ return m_RDMSP;}
	inline Int32 GetRealDDP(){ return m_RDDP;}
	inline Int32 GetRealDVP(){ return m_RDVP;}
	inline UInt8 GetTempAction() { return m_tempAction;}
	inline UInt8 GetTempReaction() { return m_tempReaction;}	

	void ResetProtector();
	bool RealDead();
	void IdleAction();
	Int GetFinalValue(Int change, CBatActor * pAtk, UInt8 tempAction, UInt8 valueType);
	CItem * GetSkillUseItem();
	inline CBatActor * GetProtector()
	{
		return m_pProtector;
	};
	enum TargetType {
		TARGET_ENEMY = 0,
		TARGET_CHARACTER,
		TARGET_PARTNER,
		TARGET_NPC,
	};

	BYTE		m_ReturnType;
	String		m_Msg;
	CBatCharacter*	m_pBatChar;					// pointer to battle character
	CBatMob*		m_pMob;						// pointer to mob
	BatClient*		m_pClient;					// pointer to client
	UInt8			m_command;					// actor command
	UInt8			m_cmdTarget;				// actor command target
	UInt8			m_cmdTargetType;			// actor command target type 0 same team 1 other team
	UInt32			m_cmdSubType;				// actor command sub type
	UInt32			m_cmdAdditional;			// actor command additional type
	Boolean			m_bInput;					// is inputed command
	Boolean			m_bAction;					// is already action?
	CItemBag*		m_pBag;						// actor inventory bag
	Boolean			m_MissSkill;
	Boolean			m_BuffActive;
	Boolean			m_ignoreTeam;
	StringA			m_AI;					// is AI controlled
	Boolean			m_buffActing;
	Boolean			m_isLastHit;
	Boolean			m_CancelAction;
	UInt16Vector	m_ActiveBuffIDs;

	BlockSkillMap m_BlockSkills;
	BuffMap		m_BuffMap;

	//buff that only active during online
	BuffMap		m_BuffCacheMap;

	AttriChangeDataList	m_AttriChangeLst;
	AttriChangeDataList	m_AttriBuffChangeLst;

	//post skill data
	UInt16		m_postSkillID;
	UInt8		m_postSkillTarget;
	UInt8		m_postSkillTurn;
	UInt16		m_postSkillBaseLv;

	bool		m_punish;
	bool		m_jumpBack;

	UInt8		m_protectCount;

	//bounds exp;
	UInt32		m_BoundsExp;
	CBatActor * m_pProtector;

	float		m_speed;
	//GetFinalValue lock
	bool m_inGetFinalValue;



protected:

	typedef BatActor_Stub Parent;
	CBattle*		m_pBattle;
	
	String			m_SkillMsg;
	UInt32			m_CurContAct;
	UInt32			m_CurBuffContAct;
	SkillMap		m_SkillMap;
	UInt8			m_cmdBlockMap[MAX_ACTION_CMD + 1];		// command block map

	//temp action
	UInt8 m_tempAction;
	UInt8 m_tempReaction;

	//real drop value
	Int32 m_RDHP;
	Int32 m_RDMHP;
	Int32 m_RDSP;
	Int32 m_RDMSP;
	Int32 m_RDDP;
	Int32 m_RDVP;

private:

	void AttackAnimTime(CBatActor * pTarget);
};

INLINE CSkill* CBatActor::GetSkill(UInt16 skillId)
{
	//if (m_pSkillMap) 
	{
		SkillMap::iterator it = m_SkillMap.find(skillId);
		if (it != m_SkillMap.end())
			return it->second;
	}
	return NULL;
}

INLINE Boolean CBatActor::IsCmdBlocked(UInt8 cmd, Boolean atInput)
{
	return (cmd > MAX_ACTION_CMD || m_cmdBlockMap[cmd] > (UInt8) atInput);
}

INLINE UInt32 CBatActor::GetContActCount()
{
	return (m_buffActing ? (UInt32) m_AttriBuffChangeLst.size() : 
				(UInt32)m_AttriChangeLst.size());
}

INLINE void CBatActor::AddAttriChangeToLst(int hpChange, int spChange, int dpChange, 
										   int wcChange, UInt8 react,int action)
{
	if (m_buffActing)
		m_AttriBuffChangeLst.push_back(AttriChangeData(hpChange, spChange, dpChange, wcChange, react, action));
	else
		m_AttriChangeLst.push_back(AttriChangeData(hpChange, spChange, dpChange, wcChange, react, action));
}

INLINE void CBatActor::SetPostSkill(UInt16 skillId, UInt8 targetIndex, UInt8 turn, 
									UInt16 skillBaseLv)
{
	m_postSkillID = skillId;
	m_postSkillTarget = targetIndex;
	m_postSkillTurn = turn;
	m_postSkillBaseLv = skillBaseLv;
}

INLINE void CBatActor::ClearPostSkill()
{
	m_postSkillID = 0;
	m_postSkillTarget = 0;
	m_postSkillTurn = 0;
	m_postSkillBaseLv = 0;
}

struct actorlist
{
	CBatActor * actor;
	actorlist * next;

	CBatActor * GetCurrActor()
	{
		return actor;
	}

	actorlist * GetNext()
	{
		return next;
	}

	actorlist()
	{
		actor = NULL;
		next = NULL;
	}
};

#endif //_BATACTOR_H_
