/**************************************************************************
 *                          MapService Lua API                            *
 *                                                                        *
 * Warning:                                                               *
 *  For programmer, please add comment for all methods                    *
 *  For lua script writter, DON'T modify or SVN commit this file          *
 *                                                                        *
 **************************************************************************/

%module LuaI
%{
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/StateCode.h"
#include "Common/BattleCode.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/MapActor.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/BatMob.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/QuestLog.h"
#include "Stub/Logic/QuestType.h"
#include "Stub/Logic/QuestSystem.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Proxy/Logic/WorldEvent.h"
#include "Proxy/Logic/Guild.h"
#include "MapService/Instance.h"
#include "Resource/ResSkill.h"
#include "Resource/ResCharacter.h"
#include "Resource/ResBuff.h"
#include "Resource/ResItem.h"
#include "Stub/Logic/RelationSystem.h"

#pragma warning(disable : 4244 4101 4102 4800)

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

DEF_LuaPush(BatActorLinkLst)
DEF_LuaPush(AttriStruct)
DEF_LuaPush(AttriStruct2)
DEF_LuaPush(ExtractedBuffData)
DEF_LuaPush(SkillData)
DEF_LuaPush(ItemData)
DEF_LuaPush(StartCharData)
DEF_LuaPush(actorlist)

DEF_LuaPush(CMap)
DEF_LuaPush(CMapActor)
DEF_LuaPush(CCharacter)
DEF_LuaPush(CMapNPC)
DEF_LuaPush(CItemBag)
DEF_LuaPush(CItem)
DEF_LuaPush(CParty)
DEF_LuaPush(CPartner)
DEF_LuaPush(CPartnerGroup)
DEF_LuaPush(CPet)
DEF_LuaPush(CPetGroup)
DEF_LuaPush(CScriptSystem)
DEF_LuaPush(CQuestLog)
DEF_LuaPush(CQuestType)
DEF_LuaPush(CQuestSystem)
DEF_LuaPush(CBatMob)
DEF_LuaPush(CBatCharacter)
DEF_LuaPush(CBattle)
DEF_LuaPush(CWorldEvent)
DEF_LuaPush(CInstance)
DEF_LuaPush(CRelationSystem)

// define LuaPush for CBatActor
void LuaPush(lua_State *L, CBatActor *ptr)
{
	if (ptr == NULL)
		lua_pushnil(L);
	else if (ptr->GetClassID() == BatCharacter_CLASSID)
		SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_CBatCharacter, 0);
	else if (ptr->GetClassID() == BatMob_CLASSID)
		SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_CBatMob, 0);
	else
		lua_pushnil(L);
}

// define LuaPush for NetObject
void LuaPush(lua_State *L, NetObject *ptr)
{
	if (ptr == NULL)
		lua_pushnil(L);
	else if (ptr->GetClassID() == Character_CLASSID)
		SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_CCharacter, 0);
	else if (ptr->GetClassID() == Partner_CLASSID)
		SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_CPartner, 0);
	else if (ptr->GetClassID() == Pet_CLASSID)
		SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_CPet, 0);
	else if (ptr->GetClassID() == BatCharacter_CLASSID)
		SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_CBatCharacter, 0);
	else if (ptr->GetClassID() == BatMob_CLASSID)
		SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_CBatMob, 0);
	else
		lua_pushnil(L);
}

void HelloWorld();
int GetGameTime(int mode);
UInt32 GetServiceTime();
void SetTempResult(double result);
double GetTempResult();
PCSTRINGA GetNpcName(UInt32 npc_id);
PCSTRINGA GetMobName(UInt32 mob_id);
PCSTRINGA GetMapName(UInt16 map_id);
PCSTRINGA GetItemName(UInt32 item_id);
CMap* GetMap(UInt16 map_id);
int GetGameTime(int mode);
PCSTRINGA ConvertANSII(PCSTRINGA src);
PCSTRINGA ConvertUTF8(const String &src);
const String& ConvertString(PCSTRINGA src);
const BuffData* GetBuffData(UInt16 buff_id);
UInt8 GetRealDay(UInt32 time = 0);
UInt8 GetRealWDay(UInt32 time = 0);
UInt8 GetRealMonth(UInt32 time = 0);
UInt16 GetRealYear(UInt32 time = 0);
UInt8 GetRealHour(UInt32 time = 0);
UInt8 GetRealMinute(UInt32 time = 0);
UInt8 GetGuildWarStatus();
void QuestionPrepare();
void QuestionStart();
const ItemData* GetItemCsvData(UInt32 item_id);
CWorldEvent* GetWorldEvent(UInt16 eventId);
void LogMessage(int level, PCSTRINGA msg);

void GuildWarPrepare();
void GuildWarStart();
void GuildWarEndContinue();
void GuildWarEndAll();
void GuildWarGenGroup();
void GuildWarSendGroupRumor();

void IncognitoPKStart();
void IncognitoPKEnd();
void InIncognitoPkLstOne(UInt32 charID);
void InIncognitoPkLstTwo(UInt32 charID);
bool isIncognitoPkTimeOver();
void RandomIncognitoPKBox(UInt16 map_id,UInt8 boxType);
%}

%warnfilter(4244, 4101, 4102, 4800) LuaI;
/* C part */
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
typedef unsigned int	UInt;
typedef signed int		Int;
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

#define VOID void 
#define CONST const 
#define Boolean bool

void HelloWorld();
int GetGameTime(int mode);
UInt32 GetServiceTime();
void SetTempResult(double result);
double GetTempResult();
PCSTRINGA GetNpcName(UInt32 npc_id);
PCSTRINGA GetMobName(UInt32 mob_id);
PCSTRINGA GetMapName(UInt16 map_id);
PCSTRINGA GetItemName(UInt32 item_id);
CMap* GetMap(UInt16 map_id);
PCSTRINGA ConvertANSII(PCSTRINGA src);
const BuffData* GetBuffData(UInt16 buff_id);
UInt8 GetRealDay(UInt32 time = 0);
UInt8 GetRealWDay(UInt32 time = 0);
UInt8 GetRealMonth(UInt32 time = 0);
UInt16 GetRealYear(UInt32 time = 0);
UInt8 GetRealHour(UInt32 time = 0);
UInt8 GetRealMinute(UInt32 time = 0);
UInt8 GetGuildWarStatus();
void QuestionPrepare();
void QuestionStart();
void GuildWarPrepare();
void GuildWarStart();
void GuildWarEndContinue();
void GuildWarEndAll();
void GuildWarGenGroup();
void GuildWarSendGroupRumor();
const ItemData* GetItemCsvData(UInt32 item_id);
CWorldEvent* GetWorldEvent(UInt16 eventId);
void LogMessage(int level, PCSTRINGA msg);

void IncognitoPKStart();
void IncognitoPKEnd();
void InIncognitoPkLstOne(UInt32 charID);
void InIncognitoPkLstTwo(UInt32 charID);
bool isIncognitoPkTimeOver();
void RandomIncognitoPKBox(UInt16 map_id,UInt8 boxType);
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

struct AttriStruct2
{
	int GetSta();
	int GetSpr();
	int GetStr();
	int GetCon();
	int GetAgi();

	void SetSta(int value);
	void SetSpr(int value);
	void SetStr(int value);
	void SetCon(int value);
	void SetAgi(int value);
};

struct ExtractedBuffData
{
	UInt16	GetBuffID();
	UInt32	GetBuffValue1();
	UInt32	GetBuffValue2();
	UInt32	GetBuffValue3();
	UInt32	GetBuffCoolDown();
	UInt8	GetBuffPro();
	
	void SetBuffValue1(UInt32 GivenValue);
	void SetBuffValue2(UInt32 GivenValue);
	void SetBuffValue3(UInt32 GivenValue);
};

struct ItemData
{
	UInt32	item_id;
	UInt8	item_type;
	UInt8	eqType;
	UInt8	color;
	UInt8	rare;
	UInt32	root_id;
	UInt32	g_price;
	UInt32	m_price;
	UInt8	memberUse;
	UInt8	item_unique;
	UInt16	item_TimeLimit;
	UInt8	item_TimeLimitType;
	UInt8	reqSexClass1;
	UInt8	reqSexClass2;
	UInt8	reqFaction;
	UInt16	reqLv;
	UInt8	consume;
	UInt8	questItem;
	UInt8	soulBound;
	UInt8	dumpable;
	UInt8	droppable;
	UInt8	godWeapon;
	UInt8	steal;
	UInt8	useCount;
	UInt8	overlap;
	UInt8	usePlace;
	UInt8	bBuff;
	UInt8	form;
	UInt8	targetType;
	UInt8	target;
	UInt16	stateCaster;
	UInt16	stateTarget;
	UInt16	stateClear;
	UInt8	coolType;
	UInt16	coolTime;
	UInt8	periodType;
	UInt16	period;
	UInt16	nTarget;
	UInt8	restoreType;
	Int16	restore_HP;
	Int16	restore_mHP;
	Int16	restore_SP;
	Int16	restore_mSP;
	Int16	restore_VP;
	Int16	restore_DP;
	Int16	restore_WC;
	Int16	restore_favor;
	UInt16	bagSlot;
	UInt8	eq_baseElement;
	UInt16	eq_maxEndure;
	UInt16	eq_maxUpgrade;
	UInt8	eq_minGem;
	UInt8	eq_maxGem;
	Int16	STA_d;
	Int16	STA_u;
	Int16	SPR_d;
	Int16	SPR_u;
	Int16	STR_d;
	Int16	STR_u;
	Int16	CON_d;
	Int16	CON_u;
	Int16	AGI_d;
	Int16	AGI_u;
	Int16	ATK_d;
	Int16	ATK_u;
	Int16	DEF_d;
	Int16	DEF_u;
	Int16	HIT_d;
	Int16	HIT_u;
	Int16	EVA_d;
	Int16	EVA_u;
	Int16	POW_d;
	Int16	POW_u;
	Int16	SPD_d;
	Int16	SPD_u;
	Int16	maxHP_d;
	Int16	maxHP_u;
	Int16	maxSP_d;
	Int16	maxSP_u;
	Int16	maxVP_d;
	Int16	maxVP_u;
	BYTE	iconLight;
	BYTE	weaponLight;
	TCHAR	icon1[33];
	TCHAR	icon2[33];
	UInt16	icon2_id;
	UInt16	eq_Color;
	UInt8	sellable;
	UInt8	repair;
	UInt8	Identify;
	UInt8	item_GenerateLimit;
    UInt16	item_GenerateLimit_Time;
	UInt16	item_GenerateLimit_Count;

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
	float GetSpd_CON();
	float GetSpd_STA();
};

struct BatActorLinkLst
{
public:
	CBatActor * GetActor();
	BatActorLinkLst * GetNext();
};

struct actorlist
{
public:
	CBatActor * GetCurrActor();
	actorlist * GetNext();
};

struct BuffData
{
	UInt16	id;
	UInt16	Buff_type;
	UInt16	Buff_school;
	UInt16	Buff_endtype;
};

///////////////////////////////////////////////////////////////////////////
///        NOTICE: Lua script writter, please read from here            ///
/**************************************************************************
 *                          MapService Lua API                            *
 *                                                                        *
 *                                                                        *
 * Lua global variables:                                                  *
 *  script - running script system (CScriptSystem)                        *
 *  player - current character (CCharacter)                               *
 *  party - current character party (CParty), nil if haven't              *
 *  npc - current talking npc (CMapNPC), nil if haven't                   *
 *  item - current using item (CItem), nil if haven't                     *
 *  target - current using target (CCharater, CPartner or CPet),          *
 *           use GetUserType() to discint, nil if haven't                 *
 *                                                                        *
 **************************************************************************/
 
class CMap
{
public:
	UInt32 GetMap_id();
	UInt32 GetLine_id();
	UInt32 GetInst_id();
	UInt32 GetOwner_id();
	UInt16 GetWidth();
	UInt16 GetHeight();
	Boolean IsBlocked(UInt16 x, UInt16 y);
	Boolean IsOccupied(UInt16 x, UInt16 y);
	int IsJumpPoint(UInt16 x, UInt16 y);
	
%extend {
	// get map name
	PCSTRINGA GetName()		{ return ConvertUTF8(self->GetName()); }
}

};

// CMapActor is a base class for actor in map
class CMapActor
{
public:
	// get current map (CMap) of actor
	CMap* GetMap();
	// get actor fields
	UInt8 GetSex();
	UInt8 GetCclass();
	UInt8 GetSexClass();
	UInt8 GetFaction();
	UInt8 GetState();
	UInt32 GetMap_id();
	UInt32 GetMap_ownerId();
	UInt16 GetReborn_map();
	UInt16 GetReborn_x();
	UInt16 GetReborn_y();
	UInt16 GetCurPosX();
	UInt16 GetCurPosY();
	UInt16 GetDstPosX();
	UInt16 GetDstPosY();
	UInt16 GetAction();
	UInt32 GetSpeed();
	UInt32 GetFollowId();	
	void SetReborn(UInt16 reborn_map, UInt16 reborn_x, UInt16 reborn_y);
	void SetSpeed(UInt32 speed);
	
%extend {
	// get actor user type
	UInt8 GetUserType()		{ return 0; }
	// get actor job
	UInt8 GetJob()			{ return self->GetCclass(); }
	// get actor name
	PCSTRINGA GetName()		{ return ConvertUTF8(self->GetNickName()); }
}

};

%feature("compactdefaultargs") CCharacter::ChangeExp;
%feature("compactdefaultargs") CCharacter::ChangeMoney;
%feature("compactdefaultargs") CCharacter::ChangeSMoney;
%feature("compactdefaultargs") CCharacter::ChangeBankMoney;
%feature("compactdefaultargs") CCharacter::SetFull;
%feature("compactdefaultargs") CCharacter::JumpMap;

// CCharacter inherit all CMapActor methods
class CCharacter : public CMapActor
{
public:
	// get character party (CParty)
	CParty* GetParty();
	// get character item bag (CItemBag)
	CItemBag* GetBag();
	// get character item bag (CItemBag)
	CItemBag* GetItems();
	// get character partners (CPartnerGroup)
	CPartnerGroup* GetPartners();
	// get character pets (CPetGroup)
	CPetGroup* GetPets();
	// get character quests (CQuestSystem)
	CQuestSystem* GetQuests();
	// get character script system (CScriptSystem)
	CScriptSystem* GetScript();
	// get character current partner (CPartner)
	CPartner* GetCurPartner();
	// get character current pet (CPet)
	CPet* GetCurPet();
	
	// get character fields
	UInt32 GetAcct_id();
	UInt32 GetChar_id();
	UInt16 GetLevel();
	UInt32 GetNextLvExp();
	UInt32 GetExp();
	UInt16 GetSTA();
	UInt16 GetSPR();
	UInt16 GetSTR();
	UInt16 GetCON();
	UInt16 GetAGI();
	UInt16 GetATK();
	UInt16 GetDEF();
	UInt16 GetHIT();
	UInt16 GetEVA();
	UInt16 GetPOW();
	UInt16 GetSPD();
	UInt16 GetAttrPoint();
	UInt16 GetHP();
	UInt16 GetMHP();
	UInt16 GetMaxHP();
	UInt16 GetSP();
	UInt16 GetMSP();
	UInt16 GetMaxSP();
	UInt16 GetDP();
	UInt16 GetMDP();
	UInt16 GetMaxDP();
	UInt16 GetWC();
	UInt16 GetMaxWC();
	UInt16 GetWcExp();
	UInt16 GetVP();
	UInt16 GetMaxVP();
	Int32 GetGoodness();
	UInt32 GetHonor();
	UInt32 GetManor();
	UInt32 GetFDonate();
	UInt32 GetTotalFDonate();
	UInt32 GetMDonate();
	UInt32 GetTotalMDonate();
	UInt32 GetYuanBao();
	UInt32 GetMoney();
	UInt32 GetBankMoney();
	UInt32 GetSMoney();
	UInt32 GetPartner_id();
	UInt32 GetPartnerMob_id();
	UInt16 GetPartnerColorIndex();
	UInt32 GetPet_uid();
	UInt32 GetPet_id();
	UInt16 GetPetColorIndex();
	UInt8 GetMaxPartner();
	UInt8 GetMaxPet();
	UInt16 GetPhoto_id();
	UInt16 GetHairStyle1();
	UInt16 GetHairStyle2();
	UInt16 GetHairColor();
	UInt32 GetEq_weapon();
	UInt16 GetEq_weaponColor();
	UInt32 GetEq_headBand();
	UInt16 GetEq_headColor();
	UInt32 GetEq_cloth();
	UInt16 GetEq_clothColor();
	//UInt32 GetMount();
	UInt8 GetState();
	UInt16 GetShortcut(UInt8 i);
	Boolean GetHasParty();
	Boolean GetIsLeader();
	RPCNetID GetPartyNetID();
	UInt16 GetPK_Value();
	Boolean GetEnablePK();
	Boolean GetEnableBlockGiven();
	Boolean GetEnableBlockParty();
	Boolean GetEnableBlockMsg();
	Boolean GetEnableBlockTrading();
	Boolean GetEnableBlockMakeFriend();
	UInt8 GetShortcuttype(UInt8 i);
	UInt32 GetHawkerTime();
	UInt8 GetAvailBackPack();
	UInt8 GetAvailItemWareHouse();
	UInt8 GetAvailPartnerWareHouse();
	UInt8 GetAvailPetWareHouse();
	UInt32 GetPvPScoreSingle();
	UInt32 GetGuildUid();
	UInt32 GetGuildManorScale();	
	
	void ChangeBaseSpeed(UInt16 speed);
	void ChangeSTA(Int change);
	void ChangeSPR(Int change);
	void ChangeSTR(Int change);
	void ChangeCON(Int change);
	void ChangeAGI(Int change);
	void ChangeATK(Int change);
	void ChangeDEF(Int change);
	void ChangeHIT(Int change);
	void ChangeEVA(Int change);
	void ChangePOW(Int change);
	void ChangeSPD(Int change);
	void ChangeHP(Int change);
	void ChangeMHP(Int change);
	void ChangeMaxHP(Int change);
	void ChangeSP(Int change);
	void ChangeMSP(Int change);
	void ChangeMaxSP(Int change);
	void ChangeDP(Int change);
	void ChangeMDP(Int change);
	void ChangeMaxDP(Int change);
	void ChangeWC(Int change);
	void ChangeMaxWC(Int change);
	void ChangeWcExp(Int change);
	void ChangeVP(Int change);
	void ChangeMaxVP(Int change);
	void ChangeFDonate(Int change);
	void ChangeGDonate(Int change, UInt8 source = 0);
	void ChangeMDonate(Int change);
	UInt32 GetGDonate();
	UInt32 GetGTotalDonate();
	void ChangeSpeakCount(Int16 count);
	void ChangeLevel(Int16 change);
	void ChangeExp(Int32 change, Int32 exp_type = 0);
	void ChangeMoney(Int32 change, Int32 src_type = 0);
	void ChangeBankMoney(Int32 change, Int32 src_type = 0);
	void ChangeHonor(Int32 change);
	void ChangeGoodness(Int32 change, Int32 exp_type = 0);
	void ChangeSMoney(Int32 change, Int32 src_type = 0);
	void ChangePvPScoreSingle(Int32 change);
	void SetFaction(UInt8 faction);
	void SetLevel(UInt16 level);
	void SetExp(UInt32 exp);
	void SetAttrib1(UInt16 STA, UInt16 SPR, UInt16 STR, UInt16 CON, UInt16 AGI);
	void SetAttrib2(UInt16 ATK, UInt16 DEF, UInt16 HIT, UInt16 EVA, UInt16 POW, UInt16 SPD);
	void SetGoodness(Int32 goodness);
	void SetHonor(UInt32 honor);
	void SetMoney(UInt32 money);
	void SetBankMoney(UInt32 money);
	void SetSMoney(UInt32 money);
	void SetPK_Value(UInt16 pk);
	void SetEnablePKEx(Boolean enable);
	void SetReborn(UInt16 map_id, UInt16 x, UInt16 y);
	void SetHP(UInt16 HP, UInt16 mHP, UInt16 maxHP);
	void SetSP(UInt16 SP, UInt16 mSP, UInt16 maxSP);
	void SetHP(UInt16 HP);
	void SetMHP(UInt16 mHP);
	void SetMaxHP(UInt16 maxHP);
	void SetSP(UInt16 SP);
	void SetMSP(UInt16 mSP);
	void SetMaxSP(UInt16 maxSP);
	void SetDP(UInt16 DP);
	void SetMDP(UInt16 mDP);
	void SetMaxDP(UInt16 maxDP);
	void SetMaxPartner(UInt8 maxPartner);
	void SetMaxPet(UInt8 maxPet);
	void SetPet_id(UInt32 pet_id);
	void SetHawkerTime(UInt32 time);
	void SetFull(bool HP=true, bool SP=true, bool DP=true, bool VP=true);
	void ForceAntiHackPopUp();	
	bool IsHawking();
	Boolean IsInMap();
	Boolean IsInBattle();
	Boolean IsSit();
	
	bool HasBuff(UInt16 buff_id);
	bool AddBuff(UInt16 buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	bool HasEquipment(UInt8 index);
	void RemoveBuff(UInt16 buff_id);
	void RemoveBuffSchool(UInt16 school_id);
	ExtractedBuffData * FindBuff(UInt16 buff_id);
	
	UInt32 CheckBattle();
	void EncounterBattle(UInt32 targetNpc);
	Boolean CanJumpMap();
	void JumpMap(UInt16 map_id, UInt16 x, UInt16 y, UInt8 line_id=0, UInt32 ownerId=0,
				UInt16 instId=0,Boolean bCheckLua = BOOLEAN_TRUE);
	void OpenHawkerOwnerMenu();
	void JumpReborn();
	void ShowRumor(PCSTRINGA msg, Boolean bTitle);
	void ShowRank(UInt8 rankType);
	void ShowGuildRank();
	void EnterInstance(PCSTRINGA script, UInt16 instId, UInt32 ownerId, UInt16 map_id, 
						UInt16 x, UInt16 y);
	void EnterEmptyInstance(PCSTRINGA script, UInt16 instId, UInt32 ownerId, UInt16 map_id, 
						UInt16 x, UInt16 y);
	void LeaveInstance(UInt16 instId, UInt32 ownerId, UInt16 map_id, UInt16 x, UInt16 y, 
						UInt8 line_id);
	void ResetInstance(UInt16 instId, UInt16 ownerId);
	void CloseInstance(UInt16 instId, UInt16 ownerId);
	
	bool HasSkill(UInt16 skill_id);
	void AddSkill(UInt16 skill_id);
	void RemoveSkill(UInt16 skill_id);
	bool HasParty();
	bool HasTempParty();
	bool IsLeader();
	void ItemFail(PCSTRINGA Text);
	void UseItemMsg(PCSTRINGA Text);
	void SkillFail(PCSTRINGA Text);
	void UseSkillMsg(PCSTRINGA Text);
	UInt16 GetSkillCount(UInt16 type,UInt16 level);
	
	UInt16 GetSkillLv(UInt16 ID);
	VOID ChangeSkillLv(UInt16 skillId, Int level);
	void SetSkillCoolDown(UInt16 ID, DWORD countDown);
	void SetEncounterBattle(bool value);
	void SetStepMod(int stepMod);
	void ResetCharLevel(UInt16 lv);
	Boolean ReverseAttrib(UInt8 attrType, UInt16 point);
	Boolean HasTitle(UInt16 title_id);
	void AddTitle(UInt16 title_id);
	void RemoveTitle(UInt16 title_id);
	
	void SetTradingTarget(RPCNetID pCharNetID);
	void ResetTradingTarget();
	CTrading* GetTradingObject();
	void SetTradingObject(CTrading* tradingObject);
	bool IsTrading();
	void ClearReqTrading();
	
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
	bool AddEquipmentBuff(UInt8 index, UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void OpenGuildApplyMenu();
	bool CreateGuild(PCSTRINGA nameA);
	void OpenCreateGuildMenu();
	bool HasGuild();
	void OpenGiftBox();
	void RemoveGuildApply();
	void AddAvailBackPack(UInt8 numOfAvail);
	void AddAvailItemWareHouse(UInt8 numOfAvail);
	void AddAvailPartnerWareHouse(UInt8 numOfAvail);
	void AddAvailPetWareHouse(UInt8 numOfAvail);

	Boolean HasFollower(UInt32 mob_id);
	UInt32 GetFollowerId(UInt16 i);
	UInt16 GetFollowerCount(UInt32 mob_id);
	UInt16 GetFollowerTotal();
	
	void OpenWareHouse();
	
	CGuild * GetGuild();
	UInt8 GetGuildPos();
	UInt32 GetJoinGuildPeriod();
	
	void SetInitAutoCount(UInt16 autoCount);
	UInt16 GetInitAutoCount();
	void SetAntiHackEnable(Boolean enable);
	Boolean GetAntiHackEnable();
	
	bool IsGuildWarWin();
	
	CRelationSystem* GetRelationSystem();
	UInt32 GetSibling_id();
	UInt32 GetWantedReward();
	void SetWantedReward(UInt32 wantedReward);
	void OpenSellYuanBao();
	void UpdateWantedReward(UInt32 targetId, Int32 money);
	void RequestJailDeliveryId();
	void RequestBailId();
	void RequestTraceId();
	Boolean IsExcuteMarriage();
	
	void OpenBusinessShop(UInt16 shopId);
	void ChangeBusinessProfit(Int32 profit);
	void OpenBusinessLog();
	void ApplyTrader();
	UInt16 GetPartnerCollectionCount(UInt8 race);
	
	Boolean Sit();
	void ShowGuildMsg(PCSTRINGA msg, UInt32 guildUID);
	void ShowAwardMsg(PCSTRINGA msg);
	
	void OpenSearchPartyPanel();
	void OpenWeaponsRankPanel();
	CItemBag * GetPlayerEquipData(UInt32 ID);
	
	void HandleGuideEvent(UInt16 ID);
	
	void ChangeToMob(UInt32 mob_id);
	void ChangeModel(UInt32 mob_id);
	void ChangeModelSkill(UInt32 mob_id);
	
	Boolean AddPartnerCollection(UInt32 mob_id);
%extend {
	UInt8 GetUserType()			{ return 1; }
	PCSTRINGA GetTitle()		{ return ConvertUTF8(self->GetTitle()); }
	PCSTRINGA GetTitle(UInt16 title_id)		{ return ConvertUTF8(self->GetTitle(title_id)); }
	PCSTRINGA GetPartnerName()	{ return ConvertUTF8(self->GetPartnerName()); }
	PCSTRINGA GetPetName()		{ return ConvertUTF8(self->GetPetName()); }
	PCSTRINGA GetCharNameLink() { return ConvertUTF8(self->GetCharNameLink()); }
	PCSTRINGA GetCharPartnerNameLink(UInt32 partnerID,UInt16 partnerIndex = -1) { return ConvertUTF8(self->GetCharPartnerNameLink(partnerID,partnerIndex));}
	PCSTRINGA GetCharPetNameLink(UInt32 petID,UInt16 petIndex = -1) { return ConvertUTF8(self->GetCharPetNameLink(petID,petIndex));}
	PCSTRINGA GetCharItemNameLink(UInt32 itemID,UInt8 itemIndex = -1) { return ConvertUTF8(self->GetCharItemNameLink(itemID,itemIndex));}
	void ChangeHp(Int change)	{ self->ChangeHP(change); }
	void ChangeVp(Int change)	{ self->ChangeVP(change); }
	void ChangeMHp(Int change)	{ self->ChangeMHP(change); }
	void ChangeSp(Int change)	{ self->ChangeSP(change); }
	void ChangeMSp(Int change)	{ self->ChangeMSP(change); }
	void ChangeDp(Int change)	{ self->ChangeDP(change); }
	void ChangeWc(Int change)	{ self->ChangeWC(change); }
	bool IsBuffExist(UInt16 buff_id)	{ return self->HasBuff(buff_id); }
}

};

class CMapNPC : public CMapActor
{
public:
	UInt32 GetNpc_uid();
	UInt32 GetNpc_id();
	Boolean GetAppearWorld();
	void Destroy();
	void SetQuestHint(UInt16 quest_id, UInt16 minLv, UInt16 maxLv, 
						UInt8 sex, UInt8 job, UInt8 faction);
	void AddHintFlag(UInt16 quest_id, UInt16 flag_id, UInt16 value);
	
	void SetEnableBattle(Boolean enable);
	Boolean GetEnableBattle();
	
	Boolean CanTalk(CCharacter* pChar);
	void SetCreaterCharId(UInt32 CharId);
	UInt32 GetCreaterCharId();
};

%feature("compactdefaultargs") CItemBag::FindItemById;
%feature("compactdefaultargs") CItemBag::FindEmptyIndex;
%feature("compactdefaultargs") CItemBag::AddItem;
%feature("compactdefaultargs") CItemBag::RemoveItem;

class CItemBag
{
public:
	CItem* Get(int index);
	CItem* GetItem(UInt8 bagIndex);
	CItem* FindItemById(UInt32 item_id, UInt8 bagType, UInt8 startIdx = 0);
	Boolean FindEmptyIndex(UInt8 bagType, UInt8 &bagIndex, UInt8 startIdx = 0);
	Boolean HasItem(UInt32 item_id);
	Boolean IsSlotFree(UInt8 bagIndex);
	CItem* NewItem(UInt32 item_id, UInt8 count);
	UInt16 AddItem(UInt32 item_id, UInt16 count=1, UInt16 quality = 0, UInt8 bagIndex=0);
	Boolean RemoveItem(UInt32 item_id, UInt16 count=1);
	Boolean AddItem(CItem* item, UInt8 bagType);
	Boolean IsEquiped(CONST UInt32 item_id);
	UInt8 GetFreeSlot(UInt8 bagIndex);
	UInt16 GetTotalFreeSlot();
	UInt16 GetItemCount(UInt32 item_id);
	UInt16 GetUnlockItemCount(UInt32 item_id);	
	Boolean IsBagEnabled(UInt8 bagType);
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
	Boolean CanAddItem(UInt8 bagIndex, UInt32 item_id, UInt32 count);
	Boolean CanCreateLimitItem(UInt32 itemID, UInt16 stack);
	Boolean RemoveUnLockItem(UInt32 item_id, UInt16 count=1);
	UInt16 GetBusinessItemCount();
	void RemoveAllBusinessItem();
	UInt32 GetBusinessMoney();
	Boolean ChangeBusinessMoney(Int32 nChange);
	
%extend {
	UInt8 GetUserType()			{ return 1; }
	CItem* AddRandItem(PCSTRINGA csvFile) {
		return self->AddRandItem(ConvertString(csvFile));
	}
}

};

class CItem
{
public:
	UInt32 GetItem_uid();
	UInt32 GetItem_id();
	UInt32 GetOwner_id();
	UInt8 GetItemColor();
	UInt32 GetEqGemID(UInt8 nIndex);
	UInt8 GetBagIndex();
	UInt8 GetStack();
	UInt8 GetRemain();
	UInt16 GetSTA();
	UInt16 GetSPR();
	UInt16 GetSTR();
	UInt16 GetCON();
	UInt16 GetAGI();
	UInt16 GetATK();
	UInt16 GetDEF();
	UInt16 GetHIT();
	UInt16 GetEVA();
	UInt16 GetPOW();
	UInt16 GetSPD();
	UInt16 GetMaxHP();
	UInt16 GetMaxSP();
	UInt16 GetMaxVP();
	UInt16 GetItemValue1();
	UInt16 GetItemValue2();
	UInt16 GetItemValue3();
	void SetItemValue1(UInt16 itemValue1);
	void SetItemValue2(UInt16 itemValue2);
	void SetItemValue3(UInt16 itemValue3);
	UInt16	GetBuffID();
	UInt32	GetBuffValue1();
	UInt32	GetBuffValue2();
	UInt32	GetBuffValue3();
	UInt32	GetBuffCoolDown();
	UInt8	GetBuffPro();
	Boolean GetLock();
	void SetBuffValue1(UInt32 GivenValue);
	void SetBuffValue2(UInt32 GivenValue);
	void SetBuffValue3(UInt32 GivenValue);

	Boolean IsExist();
	Boolean ReduceCount();
	
	void AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void RemoveBuff();
	
	bool AddSkill();
	void RemoveSkill();
	
	UInt16 GetItem_quality();
};

%feature("compactdefaultargs") CParty::Join;
%feature("compactdefaultargs") CParty::Leave;
%feature("compactdefaultargs") CParty::Destroy;
%feature("compactdefaultargs") CParty::Promote;

class CParty
{
public:
	CCharacter* GetLeader();						// get party leader
	void SetLeader(CCharacter* pchar);				// set party leader
	Boolean Join(CCharacter* pchar, Boolean bNotify, Boolean bForce=BOOLEAN_FALSE, Boolean shown=BOOLEAN_TRUE);
	Boolean Leave(CCharacter* pchar, Boolean bKick = BOOLEAN_FALSE, Boolean shown=BOOLEAN_TRUE);
	void Destroy(Boolean shown=BOOLEAN_TRUE);
	Boolean Promote(CCharacter* pchar, Boolean bLeft=BOOLEAN_FALSE, Boolean shown=BOOLEAN_TRUE);
	CCharacter* Get(int index);						// get by index
	CCharacter* GetMember(UInt16 index);			// get member by index
	CCharacter* FindMember(UInt32 char_id);			// find member by char_id
	float GetAvgLevel();
	UInt8 GetSexCount(int sex);
	UInt8 GetJobCount(int job);
	UInt16 GetCount();
	UInt16 GetRealMemNum();							//get member counter except temporarily leave member
	CCharacter* GetInPartyMember(UInt16 u16Index);  //get in party member
};

%feature("compactdefaultargs") CPartner::SetFull;
%feature("compactdefaultargs") CPartner::ChangeExp;

class CPartner
{
public:
	UInt32 GetPartner_id();
	UInt32 GetMob_id();
	UInt32 GetOwner_id();
	UInt16 GetLevel();
	UInt32 GetExp();
	UInt32 GetNextLvExp();
	UInt16 GetSTA();
	UInt16 GetSPR();
	UInt16 GetSTR();
	UInt16 GetCON();
	UInt16 GetAGI();
	UInt16 GetATK();
	UInt16 GetDEF();
	UInt16 GetSPD();
	UInt16 GetPOW();
	UInt16 GetHIT();
	UInt16 GetEVA();
	UInt32 GetHP();
	UInt32 GetMaxHP();
	UInt32 GetSP();
	UInt32 GetMaxSP();
	float GetGrow();
	Int32 GetGoodness();
	float GetFavor();
	UInt16 GetHealth();
	UInt8 GetElement();
	UInt32 GetExpireDate();
	UInt8 GetRace();
	Boolean GetLock();	
	UInt16 GetAttrPoint();
	void SetAttrPoint(UInt16 attrPoint);
	
	UInt16 GetHP_native();
	UInt16 GetSP_native(); 
	UInt16 GetATK_native();
	UInt16 GetDEF_native();
	UInt16 GetSPD_native();
	UInt16 GetPOW_native();
	UInt16 GetHIT_native();
	UInt16 GetEVA_native();
	UInt8 GetPartnerType();
	UInt16 GetReqLv();
	
	void SetOwner(UInt32 owner_id);
		
	void SetFavor(float favor);
	void SetGoodness(Int32 goodness);
	void SetElement(UInt8 element);
	void SetNextLvExp(UInt32 nextLvExp);
	void SetFull(bool HP=true, bool SP=true, bool favor=true);
	void ChangeState(UInt8 state);
	void ChangeExp(Int32 change, Int32 exp_type = 0);
	void ChangeFavor(float change);
	void ChangeHP(Int change);
	void ChangeSP(Int change);
	void ChangeHealth(Int change);
	void ChangeHP_native(Int change);
	void ChangeSP_native(Int change); 
	void ChangeATK_native(Int change);
	void ChangeDEF_native(Int change);
	void ChangeSPD_native(Int change);
	void ChangePOW_native(Int change);
	void ChangeHIT_native(Int change);
	void ChangeEVA_native(Int change);
	void ChangeGrow(float change);
	
	Boolean ReverseAttrib(UInt8 attrType, UInt16 point);
	
	bool AddBuff(UInt16 buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void RemoveBuff(UInt16 buff_id);
	void RemoveBuffSchool(UInt16 school_id);
	bool HasBuff(UInt16 buff_id);
	ExtractedBuffData * FindBuff(UInt16 buff_id);
	void SetSkillCoolDown(UInt16 ID, DWORD countDown);
	void SetAttrib2(UInt16 ATK, UInt16 DEF, UInt16 HIT, UInt16 EVA, UInt16 POW, UInt16 SPD);
	void SetSTA(UInt16 STA);
	void SetSPR(UInt16 SPR);
	void SetSTR(UInt16 STR);
	void SetCON(UInt16 CON);
	void SetAGI(UInt16 AGI);
	void SetHP(UInt16 HP, UInt16 maxHP);
	void SetSP(UInt16 SP, UInt16 maxSP);
	bool HasSkill(UInt16 skill_id);
	void AddSkill(UInt16 skill_id);
	void RemoveSkill(UInt16 skill_id);
	UInt16 GetRandomSkillID(UInt16 skillType);
	UInt8 GetNormalSkillCount();
	UInt8 GetSpecialSkillCount();
	void ResetLevel(UInt16 lv);
	void ResetPotential();
	void ResetPartner();
	
	UInt16 GetCultivation();
	void ChangeCultivation(Int change);
	UInt16 GetPractice();
	void ChangePractice(Int change);
	UInt8 GetGrowLevel();
	
%extend {
	UInt8 GetUserType()		{ return 2; }
	PCSTRINGA GetName()		{ return ConvertUTF8(self->GetRaname()); }
	bool IsBuffExist(UInt16 buff_id)	{ return self->HasBuff(buff_id); }
}

};

%feature("compactdefaultargs") CPartnerGroup::AddPartner;
%feature("compactdefaultargs") CPartnerGroup::SetAllFull;

class CPartnerGroup
{
public:
	CPartner* GetPartner(UInt32 partner_id);
	CPartner* GetPartnerByMobId(UInt32 mob_id);
	CPartner* GetPartnerByIndex(UInt16 index);
	CPartner* GetFightPartner();
	CPartner* Get(int index);
	UInt16 GetCount();
	UInt32 GetFightPartnerID();
	Boolean AddNewPartner(CPartner* partner);
	void Destroy();
	UInt32 GetPartnerID(UInt16 index);
	UInt16 GetPartnerCount(UInt32 modId);
	void AddPartner(UInt32 mob_id, UInt8 level=0);
	Boolean RemovePartner(UInt32 mob_id);
	Boolean RemovePartner(CPartner* partner);
	Boolean RemovePartnerByUID(UInt32 partner_id);
	void SetAllFull(bool HP=true, bool SP=true, bool favor=true);
	void ChangeFavor(UInt8 targetType, float value);
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
};

%feature("compactdefaultargs") CScriptSystem::Say;
%feature("compactdefaultargs") CScriptSystem::SetQuestion;
%feature("compactdefaultargs") CScriptSystem::GetAnswer;
%feature("compactdefaultargs") CScriptSystem::WaitAnswer;
%feature("compactdefaultargs") CScriptSystem::BackAnswer;
%feature("compactdefaultargs") CScriptSystem::AskGiven;
%feature("compactdefaultargs") CScriptSystem::AskString;
%feature("compactdefaultargs") CScriptSystem::GenerateNPC;

class CScriptSystem
{
public:
	Boolean	IsRunning();
	Boolean	IsTalking();
	void SetAnswer(UInt8 answer);
	UInt8 GetAnswer(UInt8 index=0);
	void BackAnswer(UInt8 level=1);
	UInt32 GetNpcId();
	void TalkEnd(Boolean bWait);
	void ExitScript(PCSTRINGA errEsg);
	void StopScript();
	void Say(PCSTRINGA msg, int imageType=0, bool bLeft=true, UInt32 npc_id=0, 
				Boolean bParty=BOOLEAN_FALSE);
	void Rumor(PCSTRINGA msg, bool bShowTitle);
	void ShowHint(PCSTRINGA Msg);
	void ShowError(PCSTRINGA Msg);
	void LogMessage(int level, PCSTRINGA msg);
	bool GenerateNPC(UInt32 npc_id, UInt16 map_id, UInt16 x, UInt16 y, bool enableAll, 
					int lifeTime=10, UInt8 line_id=0, UInt32 owner_id=0,UInt8 dir = 0);
	bool OpenInterface(PCSTRINGA menu, Int32 param1);
	bool OpenShopMenu(UInt32 shop_id);
	void Rank(UInt8 rankType);
	void PlaySound(PCSTRINGA soundName);
	void SetNpcTable(PCSTRINGA tableName);
	void SetItemTable(PCSTRINGA tableName);
	void SetBatTable(PCSTRINGA tableName);
	void SetInstTable(PCSTRINGA tableName);
	Boolean IsProcess();
	Boolean IsAsking();
	Boolean IsSaying();
	void SetProcess(Boolean bProcess);
	void SetAsking(Boolean bAsk);
	void SetSaying(Boolean bSay);
	void GetMapLines(UInt16 map_id);
	
	void GetMessengers(UInt32 char_id,UInt16 relation);
	void GetMessengersName(UInt32 char_id,UInt16 relation);
	void GetFriendly(UInt32 char_id1,UInt32 char_id2);
	void ChangeFriendly(UInt32 char_id1,UInt32 char_id2,Int32 changed);
	void CreatMasterAndStudent(UInt32 master_id,UInt32 student_id);
	void DestroyMasterAndStudent(UInt32 master_id,UInt32 student_id);
	void SendPrivateMessage(PCSTRINGA msg, UInt32 SourceId, UInt32 TargetID, PCSTRINGA TargetName, Boolean isFriend);
	void IsFriend(UInt32 char_id1,UInt32 char_id2);
	
	void HasGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	void GetGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	void SetGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id, UInt16 flag);
	void RemoveGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	
	void SetQuestion(PCSTRINGA text, bool bLeft=true, UInt8 askerType=0, UInt32 askerId=0,
					Boolean bParty=BOOLEAN_FALSE);
	void AddAnswer(UInt8 index, PCSTRINGA text);
	void WaitAnswer(UInt8 defAns=0, UInt16 timeout=30);
	void NextAnswer(Boolean bParty);
	Boolean AskGiven(PCSTRINGA text, UInt32 npc_id=0);
	void AskGivenObj(UInt8 gtype, UInt32 gid);
	UInt8 GetGivenType(UInt16 i);
	UInt32 GetGivenID(UInt16 i);
	UInt32 GetGivenUID(UInt16 i);
	UInt16 GetGivenCount();
	void ClearGiven();
	void RemoveAllGiven();
	void AskString(PCSTRINGA text, PCSTRINGA defAns, bool bLeft=true, UInt8 askerType=0,
					UInt32 askerId=0);
	PCSTRINGA GetAnswerString();
	CCharacter* FindCharacter(UInt32 char_id);
};

%feature("compactdefaultargs") CQuestLog::CheckFinished;
%feature("compactdefaultargs") CQuestLog::SetStartNPC;
%feature("compactdefaultargs") CQuestLog::SetEndNPC;
%feature("compactdefaultargs") CQuestLog::SetTargetNPC;
%feature("compactdefaultargs") CQuestLog::SetTargetItem;
%feature("compactdefaultargs") CQuestLog::SetTargetPartner;
%feature("compactdefaultargs") CQuestLog::SetTargetPet;

class CQuestLog
{
public:
	UInt32 GetChar_id();
	UInt16 GetQuest_id();
	UInt16 GetType_id();
	Boolean CheckFinished(UInt32 npc_id=0, UInt16 map_id=0);
	Boolean IsFailed();
	void SetFail(Boolean fail);
	Boolean IsCompleted();
	void SetComplete(Boolean completed);
	Boolean IsOverTime();
	void SetTimeLimit(UInt32 uTime);
	UInt32 GetTimeLimitEx();
	Boolean IsCancel();
	Boolean IsFailOverTime();
	Boolean IsFailEscape();
	Boolean IsFailPlayerDie();
	Boolean IsFailTeamDie();
	Boolean IsFailFlowerDie();
	Boolean IsFailNpcDie();
	Boolean IsFailJoinParty();
	Boolean IsCancelOverTime();
	void SetCancel(Boolean enable);
	void SetFailOverTime(Boolean enable);
	void SetFailEscape(Boolean enable);
	void SetFailPlayerDie(Boolean enable);
	void SetFailTeamDie(Boolean enable);
	void SetFailFlowerDie(Boolean enable);
	void SetFailNpcDie(Boolean enable);
	void SetFailJoinParty(Boolean enable);
	void SetCancelOverTime(Boolean enable);

	UInt32 GetTargetItemId(int order);
	UInt16 GetTargetItemNum(int order);
	UInt16 GetTargetItemMax(int order);
	UInt32 GetTargetMobId(int order);
	UInt16 GetTargetMobNum(int order);
	UInt16 GetTargetMobMax(int order);
	UInt32 GetTargetNpcId(int order);
	UInt16 GetTargetNpcMap(int order);
	UInt16 GetTargetNpcX(int order);
	UInt16 GetTargetNpcY(int order);
	UInt16 GetTargetNpcNum(int order);
	UInt32 GetTargetPartnerId(int order);
	UInt16 GetTargetPartnerNum(int order);
	UInt16 GetTargetPartnerMax(int order);
	UInt16 GetTargetNewBieNum(int order);
	UInt16 GetTargetNewBieMax(int order);
	UInt32 GetTargetPetId(int order);
	UInt16 GetTargetPetNum(int order);
	UInt16 GetTargetPetMax(int order);
	UInt32 GetTargetFollowerId(int order);
	UInt16 GetTargetFollowerNum(int order);
	UInt8  GetTargetTotal();
	void SetName(PCSTRINGA name);
	void SetDescription(PCSTRINGA detail);
	void SetCancelEnable(bool bflag) { SetCancel(bflag); }
	void SetFailCondition(PCSTRINGA cond);
	void SetStartNPC(UInt32 npcId, UInt16 mapId, UInt16 posX=0, UInt16 posY=0);
	void SetEndNPC(UInt32 npcId, UInt16 mapId, UInt16 posX=0, UInt16 posY=0);
	void SetTargetNPC(int order, UInt32 npcId, UInt16 mapId, UInt16 posX=0, UInt16 posY=0);
	bool GetTargetNPC(int order);
	void FindTargetNPC(int order);
	void SetTargetItem(int order, UInt32 itemId, UInt16 maxCount, UInt8 t1, UInt16 v1,
						UInt8 t2, UInt16 v2);
	void SetTargetMOB(int order, UInt32 mobId, UInt16 maxCount);
	void SetTargetPartner(int order, UInt32 mobId, UInt16 maxCount, UInt8 t1, UInt16 v1, 
						UInt8 t2);
	void SetTargetPet(int order, UInt32 mobId, UInt16 maxCount, UInt8 t1, UInt16 v1, 
						UInt8 t2);
	void SetTargetNewBie(UInt16 minLv, UInt16 maxLv, UInt16 num);
	void SetEncountNPC(int order, UInt32 npcId, UInt16 mapId, int maxCount, UInt16 step);
	void SetTargetMap(int order, UInt16 mapId, UInt16 posX, UInt16 posY);
	void ArriveTargetMap(int order);
	void SetTargetFollower(int order, UInt32 mobId, UInt16 count);
	
	void UpdateData();
	void ResetData();
	void ResetTarget();
	
	void SetQuestFailAfterLeaderChanged(Boolean bFlag);
	
%extend	{
	UInt32 GetStartNpcId() { return self->GetBeginNpc(); }
	UInt16 GetStartMapId() { return self->GetBeginNpc_Map(); }
	UInt32 GetEndNpcId() { return self->GetFinishNpc(); }
	UInt16 GetEndMapId() { return self->GetFinishNpc_Map(); }
}

};

%feature("compactdefaultargs") CBatActor::IsValidTarget;
%feature("compactdefaultargs") CBatActor::GetRandomSkill;
%feature("compactdefaultargs") CBatActor::ChangeHP;
%feature("compactdefaultargs") CBatActor::ChangeMHP;
%feature("compactdefaultargs") CBatActor::ChangeSP;
%feature("compactdefaultargs") CBatActor::ChangeMSP;
%feature("compactdefaultargs") CBatActor::ChangeDP;
%feature("compactdefaultargs") CBatActor::ChangeWC;
%feature("compactdefaultargs") CBatActor::ChangeVP;
%feature("compactdefaultargs") CBatActor::AddAttriChangeToLst;
	
class CBatActor
{
public:
	CCharacter* GetOwner();
	
	UInt8	GetCclass();
	UInt8	GetFaction();
	UInt16	GetLevel();
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
	UInt32	GetHP();
	Int32	GetDHP();
	UInt32	GetMHP();
	UInt32	GetMaxHP();
	UInt32	GetSP();
	Int32	GetDSP();
	UInt32	GetMSP();
	UInt32	GetMaxSP();
	Int16	GetDMHP();
	Int16	GetDMSP();
	UInt16	GetDP();
	UInt16	GetTP();
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
	UInt32 GetAdditional();
	UInt8 GetSexClass();
	Boolean	GetLinkNext();
	Boolean	GetLeftBattle();
	Int32 GetGoodness();
	Int32 GetRealDHP();
	Int32 GetRealDMHP();
	Int32 GetRealDSP();
	Int32 GetRealDMSP();
	Int32 GetRealDDP();
	Int32 GetRealDVP();
	
	Boolean IsDead();
	Boolean IsEscaped();
	Boolean IsBackup();
	Boolean IsSurrender();
	Boolean IsHitTarget(CBatActor *pTarget);
	Boolean IsCatched();
	
	void SetAttrib2(UInt16 ATK, UInt16 DEF, UInt16 HIT, UInt16 EVA, UInt16 POW, UInt16 SPD);
	void SetHP(UInt32 HP, Int32 dHP, Int32 dShowHP);
	void SetSP(UInt32 SP, Int32 dSP, Int32 dShowSP);
	void SetMaxHP(UInt16 maxHP);
	void SetMaxSP(UInt16 maxSP);
	void SetLevel(UInt16 level);
	
	void ChangeHP(Int change, bool show = true);
	void ChangeMHP(Int change, bool show = true);
	void ChangeSP(Int change, bool show = true);
	void ChangeMSP(Int change, bool show = true);
	void ChangeDP(Int change, bool show = true);
	void ChangeWC(Int change, bool show = true);
	void ChangeVP(Int change, bool show = true);
	
	void ChangeTP(Int change);
	void ChangeWcExp(Int change);
	void ResetHP();
	void ResetMHP();
	void ResetSP();
	void ResetMSP();
	void ResetDP();
	void ResetVP();
	
	UInt16 GetSkillLv(UInt16 ID);
	void SkillFail();
	void SkillFail(PCSTRINGA Text);
	void MissSkill();
	bool HasSkill(UInt16 SkillId);
	
	bool AddBuff(UInt16 buff_id, UInt16 Buff_Value1, UInt16 Buff_Value2, UInt16 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void RemoveBuff(UInt16 buff_id);
	void RemoveBuffSchool(UInt16 school_id);
	bool HasBuff(UInt16 buff_id);
	ExtractedBuffData * FindBuff(UInt16 buff_id);
	
	Boolean IsValidTarget(Boolean bAlive=BOOLEAN_TRUE);
	void InputCommand(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional_type);
	
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
	UInt8	GetUseItemType();
	
	CBattle* GetBattle();
	UInt16 GetRandomSkill(int skill_form = -1);
	void SetAIType(PCSTRINGA value);
	
	void CallHelper(UInt32 mob_id, UInt16 Level, UInt8 pos);
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
	bool IsLastHit();
	bool HasParty();
	
	void SetPunish(bool value);
	bool GetPunish();
	void SetJumpBack(bool value);
	
	CItem * FindItemByid(UInt32 item_id);
	CItem * GetSkillUseItem();
	CPet * GetUsingPet();
	void PetActive();
	
	void SetBoundsExp(UInt32 exp);
	UInt32 GetBoundsExp();
	bool IsLeader();
	bool DetectBuff(UInt16 Buff_ID);
	
	Int GetFinalValue(Int change, CBatActor * pAtk, UInt8 tempAction, UInt8 valueType);
	CBatActor * GetProtector();
	
	
%extend {
	void ChangeHp(Int change, bool show = true)	{ self->ChangeHP(change, show); }
	void ChangeMHp(Int change, bool show = true)	{ self->ChangeMHP(change, show); }
	void ChangeSp(Int change, bool show = true)	{ self->ChangeSP(change, show); }
	void ChangeMSp(Int change, bool show = true)	{ self->ChangeMSP(change, show); }
	void ChangeDp(Int change, bool show = true)	{ self->ChangeDP(change, show); }
	void ChangeWc(Int change, bool show = true)	{ self->ChangeWC(change, show); }	
	bool IsBuffExist(UInt16 buff_id)	{ return self->HasBuff(buff_id); }
	CBatMob* ToBatMob()			{ return self->m_pMob; }
	CBatCharacter* ToBatChar()	{ return self->m_pBatChar; }
}

};

%feature("compactdefaultargs") CBatMob::SetData;

class CBatMob : public CBatActor
{
public:
	UInt32 GetMob_id();
	UInt32 GetOwner_id();
	UInt32 GetPartner_id();
	UInt8 GetMob_type();
	UInt8 GetRace();
	Boolean GetBoss();
	UInt8 GetPartner_type();
	UInt16 GetHP_native();
	UInt16 GetSP_native();
	UInt16 GetATK_native();
	UInt16 GetDEF_native();
	UInt16 GetSPD_native();
	UInt16 GetPOW_native();
	UInt16 GetHIT_native();
	UInt16 GetEVA_native();
	float GetGrow();
	UInt16 GetColorIndex();
	float GetFavor();
	UInt16 GetHealth();
	void ChangeFavor(float change);
	void ChangeHealth(Int change);
	void SetData(UInt32 mob_id, UInt16 level = 0);
};

class CBatCharacter : public CBatActor
{
public:
	CBatMob* GetCurPartner();
	UInt32 GetAcct_id();
	UInt32 GetChar_id();
	UInt8  GetSex();
	UInt8  GetMaxPartner();
	Boolean GetAttackBoss();
};

enum BatEventEnum				// battle event enum
{
	BSEVENT_NONE = 0,			// no event
	BSEVENT_SURRENDER,			// surrender event
	BSEVENT_ENDBATTLE,			// force end battle event
	BSEVENT_SKIPTURN,			// skip turn event
};

enum BatEndEnum					// battle result enum
{
	END_WIN = 1,				// player team win
	END_LOSE,					// player team lose
	END_ESCAPED,				// player team escaped
	END_DRAW,					// no team win or lose 
	END_MONITOR,				// monitor end
};

%feature("compactdefaultargs") CBattle::AddMob;
%feature("compactdefaultargs") CBattle::GetProtector;

class CBattle
{
public:

	CScriptSystem* GetScript();
	CBatActor* GetActor(UInt8 index);
	UInt16 GetActorCount();
	CBatCharacter* GetTeamLeader(UInt8 team);
	CBatCharacter* GetCharacter(UInt8 index);
	UInt16 GetCharacterCount();
	CBatMob* GetMob(UInt8 index);
	UInt16 GetMobCount();
	void PushReactionActor(CBatActor * pBatActor);
	void PushExtraActor(CBatActor * pBatActor);
	void SetForceEnd(Boolean bEnd);
	UInt8 GetTeamExist(UInt8 teamIndex);
	UInt16 GetTeamAvgLevel(UInt8 teamIndex);
	void SetPunish(Boolean bPunish);
	Boolean GetPunish();
	void SetJumpReborn(Boolean bJump);
	void SetBatEvent(UInt8 turn, UInt8 eventType);
	void SetBatEvent(UInt8 eventType);
	void CheckTeamStatus();
	void BatSay(PCSTRINGA msg, int imageType, bool bLeft, UInt32 npc_id, UInt8 teamIndex);
	void DisableCmd(UInt8 cmd);
	void AddMob(UInt32 mob_id, UInt8 pos=0, UInt8 level = 0, UInt8 team = 0);
	void AddGuest(UInt32 mob_id, UInt8 pos);
	Boolean IsGuestExist(UInt8 pos);
	
	CBatActor* GetProtector(CBatActor *attacker, CBatActor *target, Boolean bCritHit = BOOLEAN_FALSE);
	
	CBatActor* GetActorByPosition(UInt8 pos, UInt8 team);
	
	UInt8 GetBattleType();
	
	UInt16 GetPvPCharacterCountFromTeam(UInt8 team);
	CBatCharacter* GetPvPCharacterFromTeam(UInt8 index,UInt8 team);
	void PvPAddMob(UInt32 mob_id, UInt8 pos, UInt8 level, UInt8 team);
	UInt16 GetPvPAddMobCount();
	CBatCharacter* GetPvPLeaderFromTeam(UInt8 team);
	
	%extend {
	UInt8 GetCurTurn()			{ return self->m_curTurn; }
}
};

class CGuild
{
public:
	UInt32 GetGuild_uid();
	UInt32 GetAlliance_id();
	UInt32 GetEnemy_id();
	UInt16 GetLevel();
	UInt32 GetProsper();
	UInt16 GetMax_prosper();
	UInt32 GetFund();
	UInt16 GetTotalMember();
	UInt16 GetMaxMember();
	UInt32 GetGuildPvPScore();
	UInt32 GetGuildPvPBid();
	UInt8 GetGuildPvPGroup();
	UInt8 GetManorScale();
	UInt32 GetMaterial();
	UInt8 GetRoom();
	UInt8 GetAura();
	UInt8 GetTrainCharExPow();
	UInt8 GetTrainCharInPow();
	UInt8 GetTrainPartnerExPow();
	UInt8 GetTrainPartnerInPow();
	UInt8 GetDrugLv();
	UInt8 GetCookLv();
	UInt8 GetWineLv();
	UInt8 GetHpLv();
	UInt8 GetSpLv();
	UInt32 GetMaintainNum();
	Boolean GetAutoManageBusiness();
	
	void SetNewManorScale(UInt8 scale);
	void ChangeFund(Int32 change, UInt8 source = 5);
	void ChangeGuildPvPBid(Int32 change);
	void ChangeProsper(Int32 change, UInt8 source = 2);
	void ChangeMaterial(Int32 change);
	void ChangeRoom(Int8 change);
	void SetAuraValue(Int8 change);
	void ChangeTrainCharEx(Int8 change);
	void ChangeTrainCharIn(Int8 change);
	void ChangeTrainPartnerEx(Int8 change);
	void ChangeTrainPartnerIn(Int8 change);
	void ChangeDrug(Int8 change);
	void ChangeCook(Int8 change);
	void ChangeWine(Int8 change);
	void ChangeManorHP(Int8 change);	
	void ChangeManorSP(Int8 change);
	void ChangeAutoManageBusiness(Boolean bOpen);
	
	%extend {
	// get guild name
	PCSTRINGA GetName()		{ return ConvertUTF8(self->GetName()); }
	PCSTRINGA GetAim()		{ return ConvertUTF8(self->GetAim()); }
	PCSTRINGA GetRule()		{ return ConvertUTF8(self->GetRule()); }
	PCSTRINGA GetCreator()		{ return ConvertUTF8(self->GetCreator()); }
	PCSTRINGA GetMaster()		{ return ConvertUTF8(self->GetMaster()); }
	PCSTRINGA GetSecMasterName()		{ return ConvertUTF8(self->GetSecMasterName()); }
}
	
};

class CRelationSystem
{
public:	
	//master and student
	void CreateMasterAndStudent(UInt32 master_id,UInt32 student_id);
	void DestroyMasterAndStudent(UInt32 master_id,UInt32 student_id);

	//sibling
	void RequestSetSiblingTitlePrefix();
	void RequestSetSiblingTitleSuffix();
	void CreateSibling(UInt32 sibling_id1,UInt32 sibling_id2,UInt32 sibling_id3,UInt32 sibling_id4,UInt32 sibling_id5);
	void RemoveSibling(UInt32 sibling_id);
	void AddSibling(UInt32 sibling_id1,UInt32 sibling_id2,UInt32 sibling_id3,UInt32 sibling_id4);
	UInt32 GetSiblingRank(UInt32 sibling_id);

	//husband and wife
	
	void RequestSelectMarriageTime(UInt32 husband_id,UInt32 wife_id);
	void CreateMarriage(UInt32 husband_id,UInt32 wife_id,UInt32 marriage_time);
	void DestroyMarriage(UInt32 husband_id,UInt32 wife_id);
	void ExecuteMarriage(UInt32 husband_id,UInt32 wife_id);
	UInt32 GetMarriageTime();
	UInt32 GetRealMarriageTime();
	UInt32 GetHusbandId();
	UInt32 GetWifeId();
	UInt32 GetMarriageId();
	
	%extend 
	{
	PCSTRINGA GetSiblingTitlePrefix()			{ return ConvertUTF8(self->GetSiblingTitlePrefix()); }
	}

};

class CPet
{
public:
	UInt32 GetPet_uid();
	UInt32 GetPet_id();
	UInt16 GetLevel();
	UInt16 GetHappiness();
	UInt32 GetVP();
	UInt32 GetMaxVP();
	UInt8 GetElement();
	UInt16 GetSpirit();
	void ChangeLevel(Int change);
	void ChangeVP(Int change);
	void ChangeHappiness(Int change);
	void ChangeExp(Int change);
	void ChangeSpirit(Int change);
	bool HasSkill(UInt16 skill_id);
	void ResetPet();
	Boolean GetLock();	
	UInt8 GetPetType();
	UInt8 GetPetTimeLimitType();
	void RemoveSkill(UInt16 skillId);
	void AddSkill(UInt16 skillId);
	UInt8 GetSkillCount();
	UInt16	GetRandomSkillID(); 
	Boolean GetPetState();	
%extend {
	UInt8 GetUserType()			{ return 3; }
	PCSTRINGA GetName()			{ return ConvertUTF8(self->GetName()); }
}

};

class CPetGroup
{
public:
	void AddPet(UInt32 pet_id);
	void RemovePet(UInt32 pet_id);
	void RemovePet(CPet* pPet);
	void RemovePetByUID(UInt32 pet_uid);
	CPet* Get(int index);
	CPet* GetPet(UInt32 pet_uid);
	CPet* GetCurPet();
	UInt16 GetIdCount(UInt32 pet_id);
	UInt16 GetCount();
	
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
};

class CQuestType
{
public:
	UInt16 GetType_id();
	String GetName();
	UInt16 GetRound();
	UInt16 GetMaxRound();
	UInt16 GetTotalRound();
	UInt16 GetLimitRound();
	UInt16 GetLimitType();
	UInt32 GetLastClearTime();
	void SetRound(UInt16 round);
	void SetTotalRound(UInt16 totalRound);
	void ChangeRound(Int change);
	void ChangeTotalRound(Int change);
};

%feature("compactdefaultargs") CQuestSystem::ClearTotalRound;
%feature("compactdefaultargs") CQuestSystem::GetTypeCount;
%feature("compactdefaultargs") CQuestSystem::CheckFinished;

class CQuestSystem
{
public:
	// quest log method
	Boolean RemoveEncounterByNpcID(UInt16 quest_id, UInt32 uiNpcID);
	Boolean RemoveEncounter(UInt16 quest_id);
	Boolean RemoveFollower(UInt16 quest_id);
	bool HasQuest(UInt16 quest_id);
	CQuestLog* GetQuest(UInt16 quest_id);
	CQuestLog* AddQuest(UInt16 quest_id);
	CQuestLog* AddQuest(UInt16 quest_id, PCSTRINGA quest_name, UInt16 type_id, PCSTRINGA type_name);
	Boolean RemoveQuest(UInt16 quest_id);
	CQuestLog* GetEachQuest(int index);
	UInt16 GetQuestCount();

	// quest type method
	bool HasType(UInt16 type_id);
	CQuestType* GetType(UInt16 type_id);
	CQuestType* AddType(UInt16 type_id);
	Boolean RemoveType(UInt16 type_id);
	void ClearTotalRound(UInt16 type_id=0);
	void CheckTotalRound();
	void SetRoundInfo(UInt16 type_id, UInt16 maxRound, UInt16 limitRound, UInt16 limitType);
	CQuestType* GetEachType(int index);
	UInt16 GetTypeCount(UInt16 type_id=0);

	// quest flag method
	bool HasFlag(UInt16 flag_id);
	UInt32 GetFlag(UInt16 flag_id);
	Boolean SetFlag(UInt16 flag_id, UInt32 flag);
	Boolean RemoveFlag(UInt16 flag_id);
	UInt32 GetTimeMark(UInt16 markId);
	Boolean SetTimeMark(UInt16 markId);
	Boolean RemoveTimeMark(UInt16 markId);
	Boolean TimeMarkOver(UInt16 markId, UInt16 diffMin);
	
	bool HasNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	UInt32 GetNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	Boolean SetNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id, UInt16 flag);
	Boolean RemoveNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);

	// quest check method
	void CheckOverTime();				// quest battle lose when over time
	void CheckAfterBattle(UInt8 result, UInt32 npcId, const UInt32Vector& mobIndices);
	void CheckAllTarget();
	Boolean CheckFinished(UInt16 quest_id, UInt32 npc_id=0, UInt16 map_id=0);
	
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event
	void Update();					// raise quest system update
	
%extend {
	UInt8 GetUserType()			{ return 3; }
	PCSTRINGA GetTypeName(UInt16 quest_id) {
		return ConvertUTF8(self->GetTypeName(quest_id));
	}
}

};	// class CQuestSystem

class CWorldEvent
{
public:
	UInt16 GetEventId();
	UInt8  GetEventType();
	UInt8  GetMapType();
	UInt32 GetEndTime();
	UInt32 GetTimeLeft();
	UInt32 GetNpcLiveTime();
	
%extend {
	PCSTRINGA GetScript()		{ return ConvertUTF8(self->GetScript()); }
}
};

class CInstance
{
public:
	UInt16 GetInstId();
	UInt32 GetOwnerId();
	void Reset();
	void Close();
	void OnEnter(CCharacter* pchar);
	void OnLeave(CCharacter* pchar);
	void AddMap(UInt16 map_id);
	UInt16 GetPlayerLimit();
	void SetPlayerLimit(UInt16 playerMax);
	void SetLasting(int lasting);

	static CInstance* CreateInstance(UInt16 instId, UInt32 owner_id, UInt8 inst_type);
	static void DestroyInstance(UInt16 instId, UInt32 owner_id);
	static CInstance* GetInstance(UInt16 instId, UInt32 ownerId);
};	// class CInstance
