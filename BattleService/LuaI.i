/* File : LuaI.i */
%module LuaI
%{
#include "Common.h"
#include "Common/common_binary.h"
#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/BatMob.h"
#include "Stub/Logic/BatCharacter.h"
#include "BattleService/BatScriptSystem.h"
#include "Stub/Logic/Battle.h"
#include "Common/StateCode.h"
#include "Common/BattleCode.h"
#include "Resource/ResSkill.h"
#include "Stub/Logic/BatItem.h"
#include "Resource/ResCharacter.h"

#pragma warning(disable : 4244 4101 4102)

#define  DEF_LuaPush(_type_)								\
void LuaPush(lua_State *L, _type_ *ptr)						\
{															\
	SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_##_type_, 0);		\
}

int Div(int a, int b)
{
	return (a / b);
}

inline UInt32 DivUInt32(UInt32 a, UInt32 b)
{
	return (a / b);
}

inline bool LAnd(bool a, bool b)
{
	return (a && b);
}

inline bool LOr(bool a, bool b)
{
	return (a || b);
}

inline int GetRand(int num)
{
	num = num + 1;
	if (num > 0)
	 return rand() % num;
	return 0;
}

DEF_LuaPush(CBatActor)
DEF_LuaPush(BatActorLinkLst)
DEF_LuaPush(CBatMob)
DEF_LuaPush(CBatCharacter)
DEF_LuaPush(CBatScriptSystem)
DEF_LuaPush(CBattle)
DEF_LuaPush(AttriStruct)
DEF_LuaPush(ExtractedBuffData)
DEF_LuaPush(SkillData)
DEF_LuaPush(actorlist)
DEF_LuaPush(CBatItem)
DEF_LuaPush(StartCharData)

void HelloWorld();

int GetGameTime(int Mode);
void SetTempResult(double result);
double GetTempResult();

%}

//
// interface c++ function
//
//%include "typemaps.i"			// you must have this for the typemaps for ptrs
%include "Common/StateCode.h"
%include "Common/BattleCode.h"

typedef unsigned char	UInt8;
typedef signed char		Int8;
typedef unsigned short	UInt16;
typedef signed short	Int16;
typedef unsigned int	UInt32;
typedef signed int		Int32;
typedef bool			Boolean;
typedef UInt16			ClassID;

#define CharA char
#define CharW wchar_t
#define Char  CharW

#define PSTRINGA CharA * 
#define PSTRINGW CharW * 

#define PCSTRINGA const CharA * 
#define PCSTRINGW const CharW * 

#define PSTRING PSTRINGW 
#define PCSTRING PCSTRINGW

void HelloWorld();

int GetGameTime(int Mode);
void SetTempResult(double result);
double GetTempResult();

struct ExtractedBuffData
{
	UInt16	GetBuffID();
	UInt16	GetBuffValue1();
	UInt16	GetBuffValue2();
	UInt16	GetBuffValue3();
	UInt32	GetBuffCoolDown();
	UInt8	GetBuffPro();
	
	void SetBuffValue1(UInt16 GivenValue);
	void SetBuffValue2(UInt16 GivenValue);
	void SetBuffValue3(UInt16 GivenValue);
};
struct SkillData
{
	UInt16	id;
	UInt8	type;
	UInt8	faction;
	UInt8	class_skill;
	UInt16	reqBaseSkill;
	UInt16	reqBaseSkillRank;
	UInt16	reqLv;
	UInt32	Req_Exp;
	UInt16	maxRank;
	UInt8	active;
	UInt8	battleorField;
	UInt8	form;
	UInt8	targetType;
	UInt8	skill_UseElement;
	UInt8	skill_Script_Type;
	UInt8	skill_cooldownType;
	UInt8	Skill_ThrowItemType;
	UInt8	AtkType;
	UInt8	mod_atk;
	UInt8	mod_Def;
	UInt8	mod_Spd;
	UInt8	mod_Pow;
	UInt8	mod_Hit;
	UInt8	mod_Eva;
	UInt8	mod_maxHP;
	UInt8	mod_maxSP;
	UInt8	mod_maxDP;
	UInt8	mod_maxWc;
	UInt16	preSkill;
	UInt8	animation_Motion;
	UInt8	animation_Type;
	
	UInt8 GetAnimationType();
};
class CBatActor
{
public:
	
	ClassID GetClassID();
	UInt8	GetCclass();
	UInt8	GetFaction();
	UInt16	GetLevel();
	UInt32	GetExp();
	UInt16	GetSTA();
	UInt16	GetSPR();
	UInt16	GetSTR();
	UInt16	GetCON();
	UInt16	GetAGI();
	UInt16	GetATK();
	UInt16	GetDEF();
	UInt16	GetSPD();
	UInt16	GetPOW();
	UInt16	GetHIT();
	UInt16	GetEVA();
	UInt16	GetHP();
	Int16	GetDHP();
	UInt16	GetMHP();
	UInt16	GetMaxHP();
	UInt16	GetSP();
	Int16	GetDSP();
	UInt16	GetMSP();
	UInt16	GetMaxSP();
	Int16	GetDMHP();
	Int16	GetDMSP();
	UInt16	GetDP();
	Int16	GetDDP();
	UInt16	GetMDP();
	UInt16	GetMaxDP();
	UInt16	GetWC();
	Int16	GetDWC();
	UInt16	GetWcExp();
	UInt16	GetMaxWC();
	UInt16	GetVP();
	UInt16	GetMaxVP();
	UInt8	GetIndex();
	UInt8	GetState();
	UInt8	GetTeam();
	UInt8	GetPosition();
	UInt8	GetAction();
	UInt8	GetReaction();
	UInt8	GetTarget();
	UInt8	GetCommand();
	UInt32	GetSubType();
	Boolean	GetLinkNext();
	Boolean	GetLeftBattle();
	Int32 GetGoodness();
	
	void ChangeHp(int change, bool show = true);
	void ChangeSp(int change, bool show = true);
	void ChangeDp(Int change, bool show = true);
	void ChangeWc(Int change, bool show = true);
	void ChangeVP(Int change, bool show = true);
	
	void ChangeMHp(int change, bool show = true);
	void ChangeMSp(int change, bool show = true);
	
	UInt16 GetSkillLv(UInt16 ID);
	void SkillFail();
	void SkillFail(PCSTRINGA Text);
	
	void AddBuff(UInt16 Buff_id, UInt16 Buff_Value1, UInt16 Buff_Value2, UInt16 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void RemoveBuff(UInt16 Buff_id);
	bool IsBuffExist(UInt16 Buff_id);
	ExtractedBuffData * FindBuff(UInt16 Buff_id);
	
	
	Boolean IsValidTarget();
	
	void InputCommand(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional_type);
	
	void ResetHpChange();
	void ResetSpChange();
	void ResetDpChange();
	void ResetVPChange();
	void ResetMHpChange();
	void ResetMSpChange();
	
	void MissSkill();
	
	void SetSkillCoolDown(UInt16 ID, DWORD countDown);
	
	bool IsPlayer();
	bool IsMob();
	bool IsParnter();
	bool IsBoss();
	
	UInt16 GetWeaponATK();
	
	void AddAttriChangeToLst(int hpChange, int spChange, int dpChange, int wcChange, UInt8 react, int action = -1);
	
	void BuffActive();
	
	void SetPostSkill(UInt16 skillId, UInt8 targetIndex, UInt8 turn, UInt16 skillBaseLv);
	bool HasPostSkill();
	void ClearPostSkill();
	void DoDefaultAttack(CBatActor * pBatTarget, int Dam, int crit);
	void SetTempReaction(UInt8 reaction);
	void SetTempAction(UInt8 action);
	UInt16 GetSkillItemATK();
	
	CBattle * GetBattle();
	UInt16 GetRandomSkill(int skill_form = -1);
	void SetAIType(PCSTRINGA value);
	
	void CallHelper(UInt32 mob_id, UInt16 Level, UInt16 skill_count);
	
	SkillData * GetSkillCsvData(UInt16 skill_id);
	
	void DoAction(CBatActor * pBatTarget);
	
	void IgnoreTeam(bool value);
	
	void ChangeTarget(UInt8 target);
	
	void ChangePlayerBatCmd(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional_type);
	
	void UseSkillMsg(PCSTRINGA Text);
	
	void CancelAction();
	void RestoreDropValue();
	
	void ItemFail(PCSTRINGA Text);
	void UseItemMsg(PCSTRINGA Text);
	
	UInt32 GetContActCount();
	UInt32 GetCurContAct();
	bool isLastHit();
	
	bool hasParty();
	
	void SetAttrib2(UInt16 ATK, UInt16 DEF, UInt16 HIT, UInt16 EVA, UInt16 POW, UInt16 SPD);
	void SetMaxHP(UInt16 maxHP);
	void SetMaxSP(UInt16 maxSP);
};

class CBatMob : public CBatActor
{
public:
	UInt32 GetMob_id();
	UInt16 GetHP_native();
	UInt16 GetSP_native();
	UInt16 GetATK_native();
	UInt16 GetDEF_native();
	UInt16 GetSPD_native();
	UInt16 GetPOW_native();
	UInt16 GetHIT_native();
	UInt16 GetEVA_native();
	float GetGrow();
};

class CBatCharacter : public CBatActor
{
public:
	UInt32 GetChar_id();
};

class CBattle
{
public:
	CBatActor* GetActor(UInt8 index);
	UInt16 GetActorCount();
	void PushReactionActor(CBatActor * pBatActor);
};

struct BatActorLinkLst
{
	CBatActor * GetActor();
	BatActorLinkLst * GetNext();
};

struct AttriStruct
{
	int GetAtk();
	int GetDef();
	int GetHit();
	int GetEva();
	int GetPow();
	int GetSpd();
	int GetMaxHp();
	int GetMaxSp();
	int GetMaxDp();
	int GetMaxWc();

	void SetAtk(int value);
	void SetDef(int value);
	void SetHit(int value);
	void SetEva(int value);
	void SetPow(int value);
	void SetSpd(int value);
	void SetMaxHp(int value);
	void SetMaxSp(int value);
	void SetMaxDp(int value);
	void SetMaxWc(int value);
};

class CBatScriptSystem
{
public:
	void LuaSetBatNoPunish(bool bPunish = true);
	void LuaDisEnableBatCmd(UInt32 disEnbale, bool bUpdate = false);
	void LuaBatSay(PCSTRINGA szUnitID, PCSTRINGA Msg, PCSTRINGA szImage = NULL, bool bLeft = true, UInt32 NpcId = 0);
	void LuaBatSetChooseImage(PCSTRINGA szUnitID, UInt32 NpcId = 0, bool bLeft = true);
	bool LuaBatChoose(PCSTRINGA szQuestion, PCSTRINGA C1, PCSTRINGA C2 = NULL);
	void LuaBatEndCmdMsg();
	void LuaBatResumeBattleMenu();
	void LuaSetBatEvent(int nTurn, int eventType);
	
	void LuaAddExp(PCSTRINGA szUnitID, UInt32 exp);
	void LuaAddGameMoney(PCSTRINGA szUnitID, UInt32 money);
	void LuaAddItem(PCSTRINGA szUnitID, UInt32 itemid, UInt32 count = 0);
	void LuaAddHonor(PCSTRINGA szUnitID, UInt32 honor);
	void LuaSetGoodness(PCSTRINGA szUnitID, Int32 goodness);
	
	void LuaTestSetAttri(CBatActor* pActor, int type, int value);
	
	void LuaAddMob(UInt32 modId, int batPos);
	UInt32 LuaHasMob(int batPos);
	int LuaGetFreeSlot(PCSTRINGA szUnitID, int bagIndex = 0);
	bool LuaNoMobExist();
	void LuaNoEndBat();
	
	void LuaBatCmd(CBatActor* pActor, int cmdType);
};

struct actorlist
{
	CBatActor * GetCurrActor();
	actorlist * GetNext();
};

class CBatItem
{
	public:
	int GetItem_quality();
};

struct StartCharData
{
	float GetMaxHP_STA();
	float GetMaxSP_SPR();
	float GetAtk_STR();
	float GetDef_CON();
	float GetHit_AGI();
	float GetEva_AGI();
	float GetPwd_SPR();
	float GetPwd_STA();
	float GetPwd_CON();
	float GetPwd_STR();
	float GetSpd_AGI();
	float GetSpd_STR();
};
