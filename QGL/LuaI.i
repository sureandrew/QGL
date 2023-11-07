/**************************************************************************
 *                          QGL Robot Lua API                             *
 *                                                                        *
 * Warning:                                                               *
 *  For programmer, please add comment for all methods                    *
 *  For lua script writter, DON'T modify or SVN commit this file          *
 *                                                                        *
 **************************************************************************/
%module LuaI
%{
// LxC: fix link error LNK2005: _SWIG_init_user already defined ...
// http://lua.2524044.n2.nabble.com/Using-multiple-modules-with-Swig-gt-Lua-td7585945.html
#define SWIG_init_user SWIG_init_user_LuaI

#include "Common.h"
#include "Proxy/Logic/Map.h"
#include "Proxy/Logic/Battle.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/Skill.h"
#include "Proxy/Logic/ChargeShop.h"
#include "Proxy/Logic/HawkGroup.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/Guild.h"
#include "QGL/CharacterControl.h"
#include "QGL/BattleControl.h"
#include "QGL/ScriptControl.h"

#pragma warning(disable : 4244 4101 4102)

#define  DEF_LuaPush(_type_)								\
void LuaPush(lua_State *L, _type_ *ptr)						\
{															\
	SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_##_type_, 0);		\
}

inline int Div(int a, int b)
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

void HelloWorld();
CMap* GetMap();
CQuestSystem* GetQuestSys();
CHawkGroup* GetMyHawk();
CHawkGroup* GetTargetHawk();
CParty* GetParty();
CItemBag* GetItemBag();
CPartnerGroup* GetPartnerGroup();
CPetGroup* GetPetGroup();
UInt16 GetSkillCount();
CSkill* GetSkill(UInt16 i);
CCharacterControl* GetCharacter();
CBattleControl* GetBattle();
CScriptControl* GetScript();
// convert actor to actor control object
CMapActorControl* GetMapActorCtrl(CMapActor *pActor);
CMapNPCControl* GetMapNPCCtrl(CMapNPC *pNpc);
CCharacterControl* GetCharacterCtrl(CCharacter *pChar);


// state enum for GetAction()
enum StateEnum {
	ST_ENTER_MAP = 1,
	ST_LEAVE_MAP,
	ST_JUMP_MAP,
	ST_STOP,
	ST_MOVE,
	ST_FOLLOW,
	ST_NOBATTLE,
	ST_BATTLE,
	ST_NEEDHELP,
	ST_MONITOR,
};

// external global varible for Lui access

// push class
DEF_LuaPush(CMap)
DEF_LuaPush(CParty)
DEF_LuaPush(CItemBag)
DEF_LuaPush(CItem)
DEF_LuaPush(CPartner)
DEF_LuaPush(CPartnerGroup)
DEF_LuaPush(CPet)
DEF_LuaPush(CPetGroup)
DEF_LuaPush(CSkill)
DEF_LuaPush(CChargeShop)
DEF_LuaPush(CHawkGroup)
DEF_LuaPush(CQuestSystem)
DEF_LuaPush(CGuild)
DEF_LuaPush(CBattleControl)
DEF_LuaPush(CScriptControl)	

%}

%import (module = "CommonI") "CommonI.i"

//%include "typemaps.i"			// you must have this for the typemaps for ptrs

///////////////////////////////////////////////////////////////////////////
///        NOTICE: Lua script writter, please read from here            ///
/**************************************************************************
 *                          SimClient Lua API                             *
 *                                                                        *
 *                                                                        *
 * Lua global variables in Robot.lua:                                     *
 *  robotVer - robot lua script version number                            *
 *  robotInterval - interval for calling lua MapAction function from C    *
 *                                                                        *
 * Lua function calling from C:                                           *
 *   MapAction(map, character)                                            *
 *    - call robot to do action in map for each interval                  *
 *   BattleTurn(battle, batchar, turnNum)                                 *
 *    - call robot to give battle command in each battle turn             *
 *   BattleEnd()                                                          *
 *    - call robot to do action just after battle ended                   *
 *   MonitorTurn(battle, turnNum)                                         *
 *    - call robot to do action when monitoring a battle in each turn     *
 *                                                                        *
 **************************************************************************/

// print "Hello World" for testing
void HelloWorld();
// get current map
CMap* GetMap();
// get the quest system
CQuestSystem* GetQuestSys();
// get your own hawk group (your own hawking business, so you are hawker) 
CHawkGroup* GetMyHawk();
// get the target hawk object(get target hawker, so you are customer)
CHawkGroup* GetTargetHawk();
// get current party
CParty* GetParty();
// get current character item bag
CItemBag* GetItemBag();
// get current partner group of character
CPartnerGroup* GetPartnerGroup();
// get current pet group of character
CPetGroup* GetPetGroup();
// get current skill count of character
UInt16 GetSkillCount();
// get current skill of character
CSkill* GetSkill(UInt16 i);
// get current character in map
CCharacterControl* GetCharacter();
// get current battle
CBattleControl* GetBattle();
// get the script system
CScriptControl* GetScript();
// state enum for GetAction()
enum StateEnum {
	ST_ENTER_MAP = 1,
	ST_LEAVE_MAP,
	ST_JUMP_MAP,
	ST_STOP,
	ST_MOVE,
	ST_FOLLOW,
	ST_NOBATTLE,
	ST_BATTLE,
	ST_NEEDHELP,
	ST_MONITOR,
};

////////////////////////////
// defined structure here //
////////////////////////////

// Map class
class CMap
{
public:
	// get map ID
	UInt16 GetMap_id();
	// get line index
	UInt8 GetLine_id();
	// get map width
	UInt16 GetWidth();
	// get map height
	UInt16 GetHeight();
	// get block value in at position x,y
	UInt16 GetBlock(UInt16 x, UInt16 y);
	// check blocking position
	bool IsBlocked(UInt16 x, UInt16 y);
	// get actor count in map
	UInt16 GetActorCount();
	// get nearby character within radius with different flag
	// flag: 0=all, 1=single, 2=in party, 3=in battle, 4=battle help, 5=pk enable
	CCharacterControl* GetNearCharacter(UInt16 x, UInt16 y, UInt16 radius, UInt8 flag);
	
	// find a random hacker if any
	//UInt32 FindRandomHawker();
	// get the npc uid for a given id
	//UInt32 FindNpcUID(UInt32 npc_id);
	//auto walk 
	void MoveToRandomGuidePos();

%extend	{
	// find character by character ID
	CCharacterControl* FindCharacter(UInt32 char_id)
	{ return GetCharacterCtrl(self->FindCharacter(char_id)); }
	// find NPC by NPC UID
	CMapNPCControl* FindNpc(UInt32 npc_uid)
	{ return GetMapNPCCtrl(self->FindNpc(npc_uid)); }
}

};

// Party class, Character container
class CParty
{
public:
	// get member count
	UInt16 GetCount();
	UInt16 GetRealMemNum();
%extend {
	// get member by index
	CCharacterControl* GetMember(UInt16 index)
	{ return GetCharacterCtrl(self->GetMember(index)); }
	// get member by character ID
	CCharacterControl* FindMember(UInt32 char_id)
	{ return GetCharacterCtrl(self->FindMember(char_id)); }
	CCharacterControl* GetInPartyMember(UInt16 u16Index)
	{ return GetCharacterCtrl(self->GetInPartyMember(u16Index));}
}

};

// Item class
class CItem
{
public:
	// get item UID
	UInt32 GetItem_uid();
	// get item ID
	UInt32 GetItem_id();
	// get item owner character ID
	UInt32 GetOwner_id();
	// get item bag index
	UInt8 GetBagIndex();
	// get remain count
	UInt8 GetRemain();
	// get stack count
	UInt8 GetStack();
	// Is item really exist?
	Boolean IsExist();
	Boolean IsBound();
	// can use item in map or battle?
	Boolean CanUse(Boolean inMap);
	// can equip item?
	Boolean CanEquip();
	// get item type and target type
	UInt8 GetItemType();
	UInt8 GetTargetType();
};

%feature("compactdefaultargs") CItemBag::FindEmptyIndex;

// Item Bag class, Item container
class CItemBag
{
public:
	// get item by bag index
	CItem* GetItem(UInt8 bagIndex);
	// get item count for bag type
	UInt8 GetCount(UInt8 bagType);
	// get total item count
	UInt8 GetTotalCount();
	UInt16 GetItemCount(UInt32 item_id);
	void SetItems(NetGroup *itemGroup);
	UInt8 FindBagIndex(CItem* item);
	UInt8 FindEmptyIndex(UInt8 bagType, UInt8 startIdx=0);
	Boolean HasItem(UInt8 bagIndex);
	bool HasItemByUID(UInt32 uid);
	Boolean SwapItem(UInt8 SrcIndex, UInt8 DesIndex);
	Boolean IsBagEnabled(UInt8 bagType);
};

// Partner Group class, Partner container
class CPartnerGroup
{
public:
	// get partner count
	UInt16 GetCount();
	// get partner by index
	CPartner* GetPartnerByIndex(UInt16 index);
	// get partner by partner ID
	CPartner* GetPartner(UInt32 partner_id);
};

// Partner class in map
class CPartner
{
public:
	UInt32 GetPartner_id();
	UInt32 GetMob_id();
	UInt32 GetOwner_id();
	UInt16 GetLevel();
	
	// can partner be called?
	Boolean CanCall();
	// get partner state
	UInt8 GetState();
	// get partner skill count
	UInt16 GetSkillCount();
	// get partner skill
	CSkill* GetSkill(UInt16 i);
	// get partner skill by skill ID
	CSkill* GetSkillById(UInt16 skill_id);
};

// Pet Group class, Pet container
class CPetGroup
{
public:
	// get pet count
	UInt16 GetCount();
	// get pet by pet ID
	CPet* GetPet(UInt32 pet_id);
	// get pet by index
	CPet* GetPetByIndex(int index);
};

// Pet class in map
class CPet
{
public:
	// get pet attributes
	UInt32 GetPet_uid();
	UInt32 GetPet_id();
	UInt32 GetOwner_id();
	UInt8  GetSex();
	UInt16 GetLevel();
	UInt32 GetExp();
	UInt16 GetState();
};

// Skill class
class CSkill
{
public:
	// get skill ID
	UInt32 GetSkill_id();
	// get skill owner ID and type
	UInt32 GetOwner_id();
	UInt32 GetOwner_type();
	// get skill type
	UInt8 GetType();
	// get skill target type
	UInt8 GetTargetType();
	// can use skill in map or battle?
	Boolean CanUse(Boolean inMap);
};

class CChargeShop 
{
public:
	UInt32 GetTotalShopItem();
};

class CHawkGroup
{
public:
	void ConstructHawkItem();
};

class CQuestSystem 
{
public:
	// get finish NPC position of a quest
	//NpcPos GetFinishNpc(UInt32 questID);
};

class CGuild
{
public:
	UInt32 GetGuild_uid();
	UInt16 GetLevel();
	UInt16 GetTotalMember();
	UInt16 GetMaxMember();
};


%feature("compactdefaultargs") CBattleControl::GetActor;
%feature("compactdefaultargs") CBattleControl::GetCharacter;
%feature("compactdefaultargs") CBattleControl::GetMob;
%feature("compactdefaultargs") CBattleControl::GetActorCount;

// Battle class
class CBattleControl
{
public:
	// get current turn
	UInt8 GetCurTurn();
	// get actor by index, or by index & team
	CCharacterControl* GetActor(UInt8 index, UInt8 team=255);
	// get character by index, or by index & team
	CCharacterControl* GetCharacter(UInt8 index, UInt8 team=255);
	// get mob by index, or by index & team
	CCharacterControl* GetMob(UInt8 index, UInt8 team=255);
	// get actor count, or by team
	UInt16 GetActorCount(UInt8 team=255);
	// is need help for this battle?
	Boolean IsNeedHelp();
};

class CScriptControl
{
public:
};
