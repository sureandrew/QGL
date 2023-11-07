#pragma once
#ifndef BATTLECONTROL_H
#define BATTLECONTROL_H

#include "Render.h"
#include "MapObjectView.h"
#include "ParticleSystem.h"
#include "CharacterControl.h"
#include "Proxy\View\BattleView.h"
#include "Common/BattleCode.h"
#include "NumberEnt.h"
#include "MessageBox.h"

class CAnimatedEntity;
class CCharacterControl;
class CBattle;
class CBatBubbleControl;

CONST Int TempHp = 150;
CONST Int FullscreenEffectIndex = 14;
CONST Int MaxTestEffect = 19;

struct BuffData;
struct EffectData;

enum SKILL_ANIM_MOTION
{
	SKILL_ANIM_MOTION_SKILL = 0,
	SKILL_ANIM_MOTION_ATK1,
	SKILL_ANIM_MOTION_ATK2,
};

enum EFFECT_POS_TYPE
{
	EFFECT_POS_TYPE_ATTACH = 0,
	EFFECT_POS_TYPE_TOP,
	EFFECT_POS_TYPE_CENTER,
	EFFECT_POS_TYPE_DOWN,
};
enum BattleState
{
	BATTLE_STATE_NO_BATTLE = 0,
	BATTLE_STATE_FADE_IN,
	BATTLE_STATE_INWAR,
	BATTLE_STATE_FADE_OUT,
	MAX_BATTLE_STATE,
};

struct BattleEntData
{
	CCharacterControl * pChar;
	CAnimatedEntity * pEntHpBarBack;
	CAnimatedEntity * pEntHpBar;
	CAnimatedEntity * pEntHpDBar;
	Int BattlePoxX;
	Int BattlePoxY;
	UInt32 DeadTime;
	Int OrgRotation;
	Int Index;
	PixelPos OrgEntPos;

	CCharacterControl * pPet;
	
	BattleEntData()
	{
		pChar = NULL;
		pEntHpBarBack = NULL;
		pEntHpBar = NULL;
		BattlePoxX = 0;
		BattlePoxY = 0;
		DeadTime = 0;
		OrgRotation = 0;
		Index = -1;
		pPet = NULL;
		pEntHpDBar = NULL;
	};
};

struct AttriChange
{
	Int OldState;
	Int HPChange;
	Int HPOrg;
	Int SPChange;
	Int SPOrg;
	Int DPChange;
	Int DPOrg;
	Int WcChange;
	Int WcOrg;
	Int VPChange;
	Int VPOrg;
	UInt8 state;

	BuffDataList addBuffList;
	BuffDataList removeBuffList;

	AttriChange()
	{
		HPChange = 0;
		HPOrg = 0;
		SPChange = 0;
		SPOrg = 0;
		DPChange = 0;
		DPOrg = 0;
		WcChange = 0;
		WcOrg = 0;
		VPChange = 0;
		VPOrg = 0;
		state = 0;
		OldState = 0;
	}
	void Init()
	{
		HPChange = 0;
		HPOrg = 0;
		SPChange = 0;
		SPOrg = 0;
		DPChange = 0;
		DPOrg = 0;
		WcChange = 0;
		WcOrg = 0;
		VPChange = 0;
		VPOrg = 0;
		state = 0;
		OldState = 0;

		removeBuffList.clear();
		addBuffList.clear();
	}
	void cleanChange()
	{
		HPChange = 0;
		SPChange = 0;
		DPChange = 0;
		WcChange = 0;
		VPChange = 0;
	}
};
struct AttackerData
{
	Int AttackerIndex;
	//use skill?
	Int Skill;
	Int Item;
	PixelPos AttackDes;
	BOOL Stop;
	Int AttackIndex;
	UInt8	AtkAction;
	AttriChange Attri;
	BOOL Back;
	BOOL OneByOne;
	BYTE animationType;

	BOOL ShowNum;

	BOOL petActive;
	
	

	AttackerData()
	{
		AttackerIndex = -1;
		Skill = 0;
		Stop = TRUE;
		AttackIndex = 0;
		
		AtkAction = BATCMD_IDLE;
		Back = TRUE;
		ShowNum = TRUE;
		OneByOne = FALSE;
		animationType = 0;
		Item = 0;
		petActive = false;		
	}
	BOOL IsValid()
	{
		return AttackerIndex != -1;
	}

	VOID Init()
	{
		AttackerIndex = -1;
		Skill = 0;
		Stop = TRUE;
		Back = TRUE;
		ShowNum = TRUE;
		OneByOne = FALSE;
		AttackIndex = 0;
		AtkAction = BATCMD_IDLE;
		Item = 0;
		Attri.Init();
		petActive = false;
	}
};

struct DefenserData
{
	Int DefenserIndex;
	BatCmdEnum	DefReaction;
	AttriChange Attri;
	Int ProtectIndex;
	BOOL ShowNum;
	BOOL Move;
	Int Skill;
	Int Buff;
	UInt32 HurtDelay;
	BOOL die;
	BOOL Back;
	BOOL petActive;
	BOOL hitTogether;

	DefenserData()
	{
		DefenserIndex = -1;
		DefReaction = BATCMD_IDLE;
		ShowNum = TRUE;
		Move = TRUE;
		HurtDelay = 0;
		Buff = 0;
		Skill = 0;
		ProtectIndex = -1;
		die = FALSE;
		Back = TRUE;
		petActive = false;
		hitTogether = false;
	}

	BOOL IsValid()
	{
		return DefenserIndex != -1;
	}

	VOID Init()
	{
		DefenserIndex = -1;
		DefReaction = BATCMD_IDLE;
		Attri.Init();
		HurtDelay = 0;
		ShowNum = TRUE;
		Move = TRUE;
		Buff = 0;
		Skill = 0;
		ProtectIndex = -1;
		die = TRUE;
		Back = TRUE;
		petActive = false;
		hitTogether = false;
	}
};



struct AttackQuene
{
	//attack side data List
	StlVector<AttackerData> AttackerLst;
	//defenser side data
	StlVector<DefenserData> DefenserLst;

	BOOL Jump;
	AttackQuene()
	{
		Jump = FALSE;
	}

	~AttackQuene()
	{
		AttackerLst.clear();
		DefenserLst.clear();
	}
};
struct BattleMapData
{
	LPDIRECT3DTEXTURE9 BgTexture;
	Byte * pBgOrgTextureData;
	
	HANDLE hFile;
	HANDLE hMapFile;
	Byte * pInFile;
	Byte * pPicData;
	
	UInt32 width;
	UInt32 height;

	
	UInt32 StartBattleTime;
	UInt32 FadeInOutTime;
	Byte Alpha;
	BattleState State;

	BattleMapData()
	{
		BgTexture = NULL;
		hFile = NULL;
		hMapFile = NULL;
		pInFile = NULL;
		pPicData = NULL;
		pBgOrgTextureData = NULL;
		StartBattleTime = 0;
		FadeInOutTime = 0;
		State = BATTLE_STATE_NO_BATTLE;
		width = 0;
		height = 0;
	}
};

struct BeingAttackData
{
	Int HitAnimationIndex;
	UInt32 StartTime;
	UInt32 TimeToHit;
	DefenserData DefData;

	Int DefSkillIndex;
	//UInt32 DefTimeToHitSkill;
	//BOOL DefSkillPlayed;

	//CCharacterControl * pAtkChar;
	AttackerData AtkData;

	Int AtkSkillIndex;
	//UInt32 AtkTimeToHitSkill;
	//BOOL AtkSkillPlayed;

	BeingAttackData()
	{
		HitAnimationIndex = 0;
		StartTime = 0;
		TimeToHit = 0;
		DefSkillIndex = 0;
//		DefTimeToHitSkill = 0;
		
		//pAtkChar = NULL;
		AtkSkillIndex = 0;
//		AtkTimeToHitSkill = 0;
//		AtkSkillPlayed = TRUE;
//		DefSkillPlayed = TRUE;
	};
};

typedef StlVector<const BuffData*, ManagedAllocator<const BuffData* > > BuffDataList;
typedef StlVector<BattleEntData, ManagedAllocator<BattleEntData> > BattleEntDataList;
typedef StlVector<CAnimatedEntity *, ManagedAllocator<CAnimatedEntity *> > CAnimatedEntityPtrList;

class CBattleControl : public CBattleView
{

private:
	VOID UpdateBattleBgMap(UInt32 CurrTime);
	VOID UpdateAttackQueue(UInt32 CurrTime);
	VOID UpdateBeingAttackQueue(UInt32 CurrTime);
	VOID UpdateEffectDataLst(UInt32 CurrTime);
	VOID UpdateSoundDataLst(UInt32 CurrTime);

	VOID ReConstructAttackQueue();

public:
	CBattleControl();
	~CBattleControl();
	
	//init player list for battle mode
	VOID AddCharacterToBattleLst(CCharacterControl * pGivChar, LPDIRECT3DDEVICE9	pD3DDevice, Int BattlePosX, 
		Int BattlePosY, BOOL AddHpBar, CCharacterControl * pGivPet);
	VOID UpdateBattle(UInt32 CurrTime);
	VOID ResetBattlePos(PixelPos LeftUpPos, Int Width, Int Height);
	VOID ChangeWeapon(Int Index);
	VOID AttackUsingSkill(int Attacker, int Defenser, int SkillIndex);
	VOID CreateBgTexture(CMapObjectView * pMap, CRender * pRender);
	VOID InitBattleData(CRender * pRender, LPCTSTR filename);
	inline BattleMapData GetBattleMapData(){ return m_BattleMapData;};
	VOID SetBattleState(BattleState State);
	
	//attack move
	BOOL OnAttack(AttackQuene QueneData);

	BOOL OnBuffActive(AttackQuene QueneData);

	//skill attack move
	BOOL OnSkillAttack(AttackQuene QueneData);

	BOOL OnSkillComboAttack(AttackQuene QueneData);
	//test attack
	VOID TestAttack(CAnimatedEntity * pTarget);
	VOID TestJumpAttack(CAnimatedEntity * pTarget);

	VOID PerformGetHurt(BattleEntData * pEntData, Int HitAnimationIndex, Int HitTimeIndex, BYTE	DefReaction, BOOL Move, BOOL Back, 
		BOOL petActive, BOOL HurtBack = TRUE);
	VOID PerformMiss(CCharacterControl * pChar, Int Rotation);
	BOOL CheckBattleFinish();
	BattleEntData * FindBattleData(CCharacterControl * pChar);

	VOID RemoveShieldEffect();
	VOID AddShieldEffect();
	VOID SetAnimation(Int Index);
	VOID ResetAnimation();

	VOID UpdateEntityStatus(UInt32 CurrTime);

	//free the entity data
	VOID FreeBattleEntData();

	inline LPDIRECT3DTEXTURE9 GetBattleMapBg(){ return m_BattleMapData.BgTexture;}

	inline Byte GetBgAlphaValue(){ return m_BattleMapData.Alpha;}

	VOID SetBattleNetObject(CBattle* pGivBattle);

	//find the character by the body entity
	CCharacterControl * FindCharByBodyEnt(CEntityBase * pEntBase);

	VOID AttackSeletedChar(CCharacterControl * pTarget);
	VOID ProtectSeletedChar(CCharacterControl * pTarget);

	VOID NotifyWaitTalk(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &timeLeft);
	virtual VOID NotifyPlayTurn(LPCPEER pPeer, RPCContext &context, Buffer &buffer, UInt32 totalSize);

	virtual VOID NotifyLoseBattle(LPCPEER pPeer, RPCContext &context);

	virtual VOID NotifyNextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &timeLeft, const UInt16 &autoCount,
		const Boolean &bInput, const Boolean &bPartnerInput);
	virtual VOID NotifyEscapeBattle(LPCPEER pPeer, RPCContext &context);
	virtual VOID NotifyMonitorEnd(LPCPEER pPeer, RPCContext &context);
	virtual VOID NotifyWinBattle(LPCPEER pPeer, RPCContext &context);
	virtual VOID NotifyDrawBattle(LPCPEER pPeer, RPCContext &context);
	virtual VOID NotifyControlCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx);
	virtual VOID NotifyControlPartner(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx, const UInt8 &mobIdx);
	virtual VOID NotifyReward(LPCPEER pPeer, RPCContext &context, const Boolean &levelUp, const UInt32 &exp, 
		const UInt32Vector &dropItems, const UInt32Vector &discardItems, const UInt32 &partnerExp, const Boolean &partnerLvlUp
		, const UInt32 &boundsExp, const UInt32 &boundsPartnerExp);
	virtual VOID NotifyPenalty(LPCPEER pPeer, RPCContext &context, const UInt32 &exp, const UInt32 &money, const UInt32 &honor);
	virtual VOID NotifyEndBattle();
	virtual VOID NotifyCommandSuccess(const UInt8 &Index, const UInt8 &cmd);
	virtual VOID NotifyAddActors(LPCPEER pPeer, RPCContext &context, NetGroup* actors, const UInt8 &Team);
	virtual VOID NotifyMartialResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &money, const UInt8 &point);
	virtual VOID NotifyNeedHelp();
	BOOL IsSendCommandAvial();
	
	inline void SetCommandAvial(BOOL Value){ m_CommandAvial = Value;}
	INLINE BOOL GetCommandAvail() { return m_CommandAvial; };

	//find Hp Entity
	CAnimatedEntity * FindCharHpBar(CCharacterControl * pAttacker);

	VOID PerformJumpAttack(CCharacterControl * pAttacker, Int upX, Int upY, Int JumpHeight,
									Int JumpTime, UInt32 CurrTime, Int AttackIndex, UInt32 HoldTime, UInt32 StartStopTime,
									BOOL Back);

	VOID PerformRunAttack(CCharacterControl * pAttacker, Int upX, Int upY, Int RunTime,
										UInt32 CurrTime, Int AttackIndex, UInt32 HoldTime, UInt32 StartStopTime,
										BOOL Back);
	
	VOID PerformJumpProtect(BattleEntData * pProtecter, Int ProtectTargetIndex, Int JumpHeight, 
		Int JumpTime, UInt32 CurrTime, UInt32 LastTime, bool petActive);

	//check whether everyone is ready
	BOOL IsEveryReady();

	//test combine attack
	VOID TestCombinedAttack(CAnimatedEntity * pTarget);

	VOID AddBattleParticle(LPCTSTR filename, Int LiveBase, BOOL Destory, Int NoOfParticle,
									   CEntityBase * pTargetEntity, BOOL SetPartent, Int64 ZValue, Int Rotation, BOOL Free);

	inline CCharacterControl * GetControlPlayer(){ return pControlPlayer;}

	inline CCharacterControl * GetControlPartner(){ return pControlPartner;}

	CCharacterControl * FindCharCtrlByChar_id(UInt32 Char_id);

	VOID EndThisTurn();

	VOID PerformEscape(CCharacterControl * pCharCtrl, BOOL petActive);

	inline CNumEnt * GetCountDownEnt(){return &m_CountDownEnt;}

	inline VOID SetLeaveBattle(BOOL Value){m_LeaveBattle = Value;}
	
	inline BOOL GetLeaveBattle(){return m_LeaveBattle;}

	UInt8 GetControlPlayerTeam();

	VOID AddAndRemoveBuffEffect();

	VOID PlayAllMember();
	VOID StopAllMember();

	INLINE VOID SetBattleItemUseIndex(Int index) { m_BattleItemUseIndex = index; };
	INLINE Int GetBattleItemUseIndex() { return m_BattleItemUseIndex; };
	INLINE UInt GetBattleCountDownTime() { return m_CountDownTime; };
	void SetBattleCountDownTime(UInt countTime);

	VOID UseBattleItemForSelectedChar(CCharacterControl * pTarget);
	VOID UseWine();
	VOID OnCancelUseItemInBattle();

	VOID PerformPreEscape(CCharacterControl * pActorCtrl);

	VOID ResetPlayerState(CCharacterControl * pActorCtrl);

	VOID PerformEscapeFail(CCharacterControl * pActorCtrl);

	inline VOID SetCounterVisible(BOOL Value){m_CountDownEnt.SetVisible(Value);}

	VOID ShowAutoCountBox(UInt16 count);

	inline CMessageBox * GetAutoMessageBox(){return &m_AutoCountBox;};

	VOID SkillUseOnSeletedChar(CCharacterControl * pTarget);

	VOID CatchSelectedChar(CCharacterControl * pTarget);

	VOID PerformJumpEnterBattle(CCharacterControl * pChar);

	inline CBattle* GetBattleNetObject(){return pBattle;}
	VOID PerformCatch(CCharacterControl * pCharCtrl, CCharacterControl * pTarget, BOOL canCatch, BOOL petActive);

	VOID TestSkillAttack(CAnimatedEntity * pTarget, Int SkillIndex, BYTE Pos, BOOL All, Int AnimIndex, BOOL Stop);

	VOID StoreWaitingPartner(NetGroup* grp);

	UInt16 GetCountInLst(const AttackQuene & Quene, BattleEntData * pTarget, BOOL DefTeam);

	VOID Init();

	Int FindBattleEntByPosition(UInt16 BkX, UInt16 BkY);
	void ShowBattlePanel();
	VOID RandAttack();
	VOID RepeatRound();

	// battle script control start
	VOID BatSayHande(UInt32 Npcid, const String & SayMsg, Int ImageType, Boolean bLeft, Boolean finalSay);
	VOID BatChooseHandle(UInt8 askerType, UInt32 askerId, Int Opt, Boolean bLeft, const StringVector &OptMsg);
	VOID BatChooseFinish(Int Opt);
	VOID BatNextTalk();
	VOID BatPushEndCmdMsg();
	VOID BatHideBatCmdMenu(UInt8 index, Boolean bHide, Boolean bUpdateBlock);
	VOID BatSetDisEnableCmd(UInt32 disEnableCmd, Boolean bUpdate);
	// battle script control end

	// battle bubble pop up message start 
	INLINE UInt32 GetBatNpcId() { return m_NpcId; };
	UInt32 GetBatMobIdByIndex(Int Index);
	VOID ClearBatBubbleCtrlData();
	VOID AddBatBubbleMobId(UInt32 modId);
	VOID LoadNpcDialogueLua(UInt32 npcId);
	VOID ShowBattleBubbleMsg(Int posIndex, Int Type);
	Int CalcFinalBatPosition(CCharacterControl * pCharCtrl); // 0 ~ 39, -1: not find
	CCharacterControl * FindCharCtrlByBatPos(Int Pos, Boolean bEnemy = BOOLEAN_FALSE);

	CCharacterControl * FindCharCtrlByBatActor(CBatActor * pBatActor);
	// battle bubble pop up message end

	VOID ShowHideBuffEffects();
	VOID HideBuffEffects();

	BOOL IsPKBattle();
	inline BOOL IsPlayingAnim() {return m_PlayingBattleAnim;}

	UInt8 GetCurTurn();
	CCharacterControl* GetActor(UInt8 index, UInt8 team=255, bool bChar=false, bool bMob=false);
	CCharacterControl* GetCharacter(UInt8 index, UInt8 team=255);
	CCharacterControl* GetMob(UInt8 index, UInt8 team=255);
	UInt16 GetActorCount(UInt8 team=255);
	Boolean IsNeedHelp();

public:
	//entity for battle mode
	BattleEntDataList m_BattlePlayerLst;
	CAnimatedEntityPtrList m_BattlePlayerHPBarBackLst;
	CAnimatedEntityPtrList m_BattlePlayerHPBarLst;
	PixelPos m_BattleLeftUpPos;

	UInt32 m_BattleFinishTime;

	UInt16 m_PlayerTeamIndex;
	UInt16 m_EnemyTeamIndex;

	UInt32 m_NpcId;
	UInt8 m_CurrTeam;
	UInt8 m_iShowState; 
	String m_S1,m_S2;

private:

	//proxy battle class 
	CBattle* pBattle;

	StlDeque<AttackQuene> m_AttackQueneData;

	//a battle bg texture
	BattleMapData m_BattleMapData;

	//effect in the battle
	CParticleSystem * m_BattleParticle;

	StlMultiMap<CCharacterControl *, BeingAttackData> m_EntityBeingAttackLst;

	StlMultiMap<UInt32, EffectData> m_EffectDataLst;

	StlMultiMap<UInt32, String> m_SoundDataLst;

	StlMap<String, EffectData> m_PlayedEffect;
	
	CCharacterControl * pControlPlayer;

	CCharacterControl * pControlPartner;

	//indicate leave the battle
	BOOL m_LeaveBattle;

	//indicate player can send command
	BOOL m_CommandAvial;

	BOOL m_PlayingBattleAnim;

	CNumEnt m_CountDownEnt;

	UInt m_PreUpdateTime;
	UInt m_CountDownTime;

	UInt32 m_NextAttackQueueTime;

	RECT m_CurrWorldRect;

	UInt32 m_ExtraWaitTime;

	//all battle object
	NetGroup *m_objects;
	NetGroup *m_tempPartnerObject;

	Int	m_BattleItemUseIndex;

	CMessageBox m_AutoCountBox;

	UInt16 m_CurrAutoCount;
	BOOL m_StartCountDown;

	CBatBubbleControl * m_pBatBubbleCtrl;

	UIStaticText *m_pStaticTextist;
	UIStaticText *m_pStaticTextsnd;
};

#endif
