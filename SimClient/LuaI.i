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
#include "Common/common_char_map.h"
#include "Proxy/Logic/MapActor.h"
#include "SimClient/Character.h"
#include "SimClient/MapNPC.h"
#include "SimClient/Map.h"
#include "SimClient/Battle.h"
#include "Proxy/Logic/BatActor.h"
#include "SimClient/BatCharacter.h"
#include "SimClient/BatMob.h"
#include "SimClient/Party.h"
#include "SimClient/Item.h"
#include "SimClient/ItemBag.h"
#include "SimClient/Partner.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/Pet.h"
#include "SimClient/PetGroup.h"
#include "SimClient/Skill.h"
#include "SimClient/ScriptSystem.h"
#include "SimClient/ChargeShop.h"
#include "SimClient/HawkGroup.h"
#include "SimClient/QuestSystem.h"
#include "SimClient/Guild.h"

#pragma warning(disable : 4244 4101 4102)

#define  DEF_LuaPush(_type_)								\
void LuaPush(lua_State *L, _type_ *ptr)						\
{															\
	SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_##_type_, 0);		\
}

enum BlockEnum
{
	TILE_NORMAL	= 0x0000,
	TILE_BLOCK	= 0x0001,
	TILE_GUIDE	= 0x0020,
};

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
bool OppTradeLock();
bool SelfTradeLock();
bool TradeInProcess();

void Execute(char* cmd);
BlockPos* GetLastMove();
CCharacter* GetCharacter();
CMap* GetMap();
CScriptSystem* GetScript();
CQuestSystem* GetQuestSys();
CHawkGroup* GetMyHawk();
CHawkGroup* GetTargetHawk();
CGuild* GetGuild();

CChargeShop* GetShop();	
UInt32 GetShopItemCount();
CBatCharacter* GetBatChar();
CBattle* GetBattle();
CParty* GetParty();
CItemBag* GetItemBag();
CPartnerGroup* GetPartnerGroup();
CPetGroup* GetPetGroup();
CBatMob* GetBatPartner();
UInt16 GetSkillCount();
CSkill* GetSkill(UInt16 i);
UInt8 GetLineCount();
UInt8 GetNextLine(UInt8 line_id);
bool CheckBattle();
UInt32 GetTotalGuildNumber();
UInt32 GetTotalGuildMemberNumber();
UInt32 GetTotalGuildApplicantNumber();
Boolean CheckGuildMembership();
UInt8 GetShadowNum();
UInt32 GetClientTotal();

// external global varible for Lui access

// push struct
DEF_LuaPush(BlockPos)
DEF_LuaPush(JumpInfo)

// push class
DEF_LuaPush(CMapActor)
DEF_LuaPush(CCharacter)
DEF_LuaPush(CMapNPC)
DEF_LuaPush(CMap)
DEF_LuaPush(CBatActor)
DEF_LuaPush(CBatCharacter)
DEF_LuaPush(CBatMob)
DEF_LuaPush(CBattle)
DEF_LuaPush(CParty)
DEF_LuaPush(CItemBag)
DEF_LuaPush(CItem)
DEF_LuaPush(CPartner)
DEF_LuaPush(CPartnerGroup)
DEF_LuaPush(CPet)
DEF_LuaPush(CPetGroup)
DEF_LuaPush(CSkill)
DEF_LuaPush(CScriptSystem)	
DEF_LuaPush(CChargeShop)
DEF_LuaPush(CHawkGroup)
DEF_LuaPush(CQuestSystem)
DEF_LuaPush(CGuild)

%}

//%include "typemaps.i"			// you must have this for the typemaps for ptrs

//
// basic type definition
//
typedef unsigned char	UInt8;
typedef signed char		Int8;
typedef unsigned short	UInt16;
typedef signed short	Int16;
typedef unsigned int	UInt32;
typedef signed int		Int32;
typedef bool			Boolean;
typedef UInt16			ClassID;
typedef UInt32			ObjectID;
typedef UInt8			ServiceID;

struct RPCNetID			// unique ID of net object
{
	ServiceID srvId;
	ObjectID objId;
};

enum BlockEnum
{
	TILE_NORMAL	= 0x0000,
	TILE_BLOCK	= 0x0001,
	TILE_GUIDE	= 0x0020,
};

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
 *   TradeAction(itemId, petId, partnerId)                                *
 *    - call robot to do trading action in map when trading confirmed     *
 *   TradeFreeHold()                                                      *
 *    - call robot to free trade holding in map when trading unlocked     *
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
// execute command to current SimClient
void Execute(char* cmd);
// get last move position of current SimClient
BlockPos* GetLastMove();

bool OppTradeLock();
bool SelfTradeLock();	
	
bool TradeInProcess();
bool CheckBattle();	


// get current character in map
CCharacter* GetCharacter();
// get current map
CMap* GetMap();
// get the quest system
CQuestSystem* GetQuestSys();
// get the script system
CScriptSystem* GetScript();
// get the guild
CGuild* GetGuild();
// get your own hawk group (your own hawking business, so you are hawker) 
CHawkGroup* GetMyHawk();
// get the target hawk object(get target hawker, so you are customer)
CHawkGroup* GetTargetHawk();
// get chargeshop
CChargeShop* GetShop();
UInt32 GetShopItemCount();
// get current character in battle
CBatCharacter* GetBatChar();
// get current battle
CBattle* GetBattle();
// get current party
CParty* GetParty();
// get current character item bag
CItemBag* GetItemBag();
// get current partner group of character
CPartnerGroup* GetPartnerGroup();
// get current pet group of character
CPetGroup* GetPetGroup();
// get current battle partner of character
CBatMob* GetBatPartner();
// get current skill count of character
UInt16 GetSkillCount();
// get current skill of character
CSkill* GetSkill(UInt16 i);
// get current map line of character
UInt8 GetLineCount();
// get next available map line of character
UInt8 GetNextLine(UInt8 line_id);
// get how many guild you got
UInt32 GetTotalGuildNumber();
// get how many guild members you got
UInt32 GetTotalGuildMemberNumber();
// get how many applicant for guild membership
UInt32 GetTotalGuildApplicantNumber();
// check if you already belonged to one guild
Boolean CheckGuildMembership();
// get the number of shadow account
UInt8 GetShadowNum();
// get the number of total running clients
UInt32 GetClientTotal();

////////////////////////////
// defined structure here //
////////////////////////////

// block position structure
struct BlockPos
{
	UInt16 PosX;
	UInt16 PosY;
};

// jump point information structure
struct JumpInfo
{
	UInt16 jpPosX;
	UInt16 jpPosY;
	UInt16 dstMapId;
	UInt16 dstPosX;
	UInt16 dstPosY;
};

// Base actor class in Map
class CMapActor
{
public:
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

	UInt8 GetSex();
	// get character job
	UInt8 GetCclass();
	// get sex and job code
	UInt8 GetSexClass();
	UInt8 GetFaction();
	UInt8 GetState();
	// get current map ID
	UInt16 GetMap_id();
	// get location when enter the map
	UInt16 GetMap_x();
	UInt16 GetMap_y();
	UInt8 GetDirection();
	// get reborn map ID and location
	UInt16 GetReborn_map();
	UInt16 GetReborn_x();
	UInt16 GetReborn_y();
	// get current position in map
	UInt16 GetCurPosX();
	UInt16 GetCurPosY();
	// get destination position in map
	UInt16 GetDstPosX();
	UInt16 GetDstPosY();
	// get current action and action time
	UInt16 GetAction();
	UInt32 GetActionTime();
	// get current moving speed
	UInt32 GetSpeed();
	// get current following actor ID
	UInt32 GetFollowId();

};

// Character class in map, inherit CMapActor
class CCharacter : public CMapActor
{
public:
	// get account ID
	UInt32 GetAcct_id();
	// get character ID
	UInt32 GetChar_id();
	UInt16 GetLevel();
	// get next level up exp
	UInt32 GetNextLvExp();
	UInt32 GetExp();
	// get STA, SPR, STR, CON, AGI attributes
	UInt16 GetSTA();
	UInt16 GetSPR();
	UInt16 GetSTR();
	UInt16 GetCON();
	UInt16 GetAGI();
	// get ATK, DEF, HIT, EVA, POW, SPD attributes
	UInt16 GetATK();
	UInt16 GetDEF();
	UInt16 GetHIT();
	UInt16 GetEVA();
	UInt16 GetPOW();
	UInt16 GetSPD();
	// get available attribute point
	UInt16 GetAttrPoint();
	// get HP, mHP and maxHP
	UInt16 GetHP();
	UInt16 GetMHP();
	UInt16 GetMaxHP();
	// get SP, mSP and maxSP
	UInt16 GetSP();
	UInt16 GetMSP();
	UInt16 GetMaxSP();
	// get DP, mDP and maxDP
	UInt16 GetDP();
	UInt16 GetMDP();
	UInt16 GetMaxDP();
	// get WC, maxWC, WC exp
	UInt16 GetWC();
	UInt16 GetMaxWC();
	UInt16 GetWcExp();
	// get VP and maxVP
	UInt16 GetVP();
	UInt16 GetMaxVP();	
	// get goodness
	Int32 GetGoodness();
	// get honer
	UInt32 GetHonor();
	// get manor
	UInt32 GetManor();
	// get faction, total faction, mansion, total mansion donations
	UInt32 GetFDonate();
	UInt32 GetTotalFDonate();
	UInt32 GetMDonate();
	UInt32 GetTotalMDonate();
	// get yuanbao, game money and bank money
	UInt32 GetYuanBao();
	UInt32 GetMoney();
	UInt32 GetBankMoney();
	// get current partner ID
	UInt32 GetPartner_id();
	// get current pet ID
	UInt32 GetPet_uid();
	// get maximum partner and pet count
	UInt8 GetMaxPartner();
	UInt8 GetMaxPet();
	// get character image photo ID
	UInt16 GetPhoto_id();
	// get hair styles and color
	UInt16 GetHairStyle1();
	UInt16 GetHairStyle2();
	UInt16 GetHairColor();
	// get current equipement weapon item ID and color
	UInt32 GetEq_weapon();
	UInt16 GetEq_weaponColor();
	// get current equipement head band item ID and color
	UInt32 GetEq_headBand();
	UInt16 GetEq_headColor();
	// get current equipement cloth item ID and color
	UInt32 GetEq_cloth();
	UInt16 GetEq_clothColor();
	// get current mount pet ID
	UInt8 GetState();
	// get current shortcut ID
	UInt16 GetShortcut(UInt8 i);
	// get PK value
	UInt16 GetPK_Value();
	// get PK flag
	Boolean GetEnablePK();
	
	// get current party
	CParty* GetParty();
	// current character has party?
	Boolean HasParty();
	// current character is party leader?
	Boolean IsLeader();
	// show character detail in console
	void ShowDetail();
	// get a valid item slot number
	int GetValidItem();
	// get the guild name	 
	String GetTitle();

};

// NPC class in map, inherit CMapActor
class CMapNPC : public CMapActor
{
public:
	// get npc UID
	UInt32 GetNpc_uid();
	// get npc ID
	UInt32 GetNpc_id();
};

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
	// get object count in map
	UInt16 GetObjectCount();
	// get map guide point position
	BlockPos* GetGuidePoint(int i);
	// get map guide point count
	UInt16 GetGuideCount();
	// get map jump point information
	JumpInfo* GetJumpPoint(int i);
	// get map jump point count
	UInt16 GetJumpCount();
	// find other character by character ID 
	CCharacter* FindCharacter(UInt32 char_id);
	// find NPC by NPC UID
	CMapNPC* FindNpc(UInt32 npc_id);
	// get nearby character within radius with different flag
	// flag: 0=all, 1=single, 2=in party, 3=in battle, 4=battle help, 5=pk enable
	CCharacter* GetNearCharacter(UInt16 x, UInt16 y, UInt16 radius, UInt8 flag);
	// find a random hacker if any
	UInt32 FindRandomHawker();
	// getthe current map line ID
	UInt16 GetMapLineID();
	// get the npc uid for a given id
	UInt32 FindNpcUID(UInt32 npc_id);
	// get the character with guild different from you
	CCharacter* FindEnemyCharacterNearby();
	// get the character with the same guild
	CCharacter* FindGuildMemberNearby();

};

// Base actor class in battle
class CBatActor
{
public:
	// get actor index in battle actor list
	UInt8 GetIndex();
	UInt16 GetLevel();
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
	UInt16 GetHP();
	Int16 GetDHP();
	UInt16 GetMHP();
	UInt16 GetMaxHP();
	UInt16 GetSP();
	Int16 GetDSP();
	UInt16 GetMSP();
	UInt16 GetMaxSP();
	UInt8 GetState();
	// get team index
	UInt8 GetTeam();
	// get battle position
	UInt8 GetPosition();
	// get action, see BattleCode.h for reference
	UInt8 GetAction();
	// get reaction, see BattleCode.h for reference
	UInt8 GetReaction();
	// get target index
	UInt8 GetTarget();
	// get sub type value
	UInt32 GetSubType();
	// next action is linked to this action?
	Boolean GetLinkNext();
	// actor is already left battle?
	Boolean GetLeftBattle();

	// get class ID for distinct CBatMob or CBatCharacter
	ClassID GetClassID();
	// this actor is a valid target to attack?
	Boolean IsValidTarget();
};

// Mob class in battle, inherit CBatActor
class CBatMob : public CBatActor
{
public:
	// get Mob ID
	UInt32 GetMob_id();
	// get mob skill count
	UInt16 GetSkillCount();
	// get mob skill by index
	CSkill* GetSkill(UInt16 i);
	// get mob skill by skill ID
	CSkill* GetSkillById(UInt16 skill_id);
};

// Character class in battle, inherit CBatActor
class CBatCharacter : public CBatActor
{
public:
	// get character ID
	UInt32 GetChar_id();
};

// Battle class
class CBattle
{
public:
	// get actor by index
	CBatActor* GetActor(UInt8 index);
	// get character by index
	CBatCharacter* GetCharacter(UInt8 index);
	// get mob by index
	CBatMob* GetMob(UInt8 index);
	// get actor count
	UInt16 GetActorCount();
	// get existing team actor count
	UInt16 GetTeamCount(UInt8 team);
	// is need help for this battle?
	Boolean IsNeedHelp();
};

// Party class, Character container
class CParty
{
public:
	// get member count
	UInt16 GetCount();
	// get member by index
	CCharacter* GetMember(UInt16 index);
	// get member by character ID
	CCharacter* FindMember(UInt32 char_id);
	// show member information in console
	void ShowMember();
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
	// Is item really exist? (remain > 0 and stack > 0)
	Boolean IsExist();
	Boolean IsBound();
	// can use item in map or battle?
	Boolean CanUse(Boolean inMap);
	// can equip item?
	Boolean CanEquip();
};

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
	void ShowItems(Boolean bSimple=BOOLEAN_FALSE);
	UInt32 GetSlotItemId(UInt8 bagIndex);
	UInt32 RandItemId();
};

// Partner Group class, Partner container
class CPartnerGroup
{
public:
	// get partner count
	UInt16 GetCount();
	// get partner by index
	CPartner* Get(UInt16 index);
	// get partner by partner ID
	CPartner* GetPartner(UInt32 partner_id);
};

// Partner class in map
class CPartner
{
public:
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

class CScriptSystem
{
public:
	bool GetTalkStatus();
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
	// get the map ID of a finish NPC of a quest with the specified quest ID
	UInt32 GetQuestJumpMap(UInt32 questID);
	// get the X Coordinate ID of a finish NPC of a quest with the specified quest ID
	UInt32 GetQuestJumpXCor(UInt32 questID);
	// get the Y Coordinate ID of a finish NPC of a quest with the specified quest ID	
	UInt32 GetQuestJumpYCor(UInt32 questID);
	// check if a mob quest ID is finished or not
	bool IsMobQuestCompleted(UInt32 questID);
	// get the quest ID of the existing quest randomly
	UInt32 GetRandomQuestID();
	// get the finish NPC ID of a specified quest
	UInt32 GetFinishNPCID(UInt32 questID);	
	// add the quest tasks that will be tackled in the robot	
	bool AddQuestTask(UInt32 questID, UInt16 type);
	// check if the quest is included in m_questTaskMap
	bool CheckQuestTaskInProcess(UInt32 questID);
	// check if there is outstanding quest tasks currently
	bool IsOutstandingQuestTask();
	// robot tell SimClient that the quest task is already cheated
	void SetQuestTaskCheated(UInt32 questID);
	// check all quests remaining tasks
	void CheckAllQuestRemainingTasks();	
};

class CGuild
{
public:

	// get the guild bid if any
	UInt32 GetGuildBidAmount();	
	// get the guild position	
	UInt32 GetGuildPosition();
	UInt32 GetGuildFund();
	UInt8 GetGuildWarGroup();	
	
};


