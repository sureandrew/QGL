#pragma once
#ifndef COMMON_BINARY_H
#define COMMON_BINARY_H

#include <Reuben\System\Reuben.h>
#include <tchar.h>
#include <vector>
#include <map>

#define QG_NAME_MAXLEN						64
#define QG_FILENAME_MAXLEN					1024

struct BinaryServerNPCInstance;
struct BinaryServerOBJInstance;
struct BinaryServerJumppointInstance;
struct ServerMapZoneData;
struct BinaryServerZoneInfo;
struct MeetEnemyZone;

typedef StlVector<BinaryServerNPCInstance*, ManagedAllocator<BinaryServerNPCInstance*> > NpcInstList;
typedef StlVector<BinaryServerOBJInstance*, ManagedAllocator<BinaryServerOBJInstance*> > ObjInstList;
typedef StlVector<BinaryServerJumppointInstance*, ManagedAllocator<BinaryServerJumppointInstance*> > JpInstList;
typedef StlVector<ServerMapZoneData*, ManagedAllocator<ServerMapZoneData*> > SrvZoneDataList;
typedef StlMap<UInt16, BinaryServerZoneInfo*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BinaryServerZoneInfo*> > > SrvZoneInfoList;
typedef StlMap<UInt32, MeetEnemyZone*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MeetEnemyZone*> > > EnemyZoneMap;

/* Begin of COPY FROM Map Editor "common_binary.h" */

#define MAX_BUFF_NUM	20
#define MAX_CHAT_MSG_ICON	8
#define MAX_PET_HAPPINESS	100
#define MAX_PET_SPIRIT  7
#define DEFAULT_NPCFLAG_LINE_ID 255
#define MAX_FRIENDLY 1000
#define INCOGNITOBOX_EFF_TIME 8500

CONST Int invisibleBuff = 70;
struct BuffData;

enum GUILD_DONATE_RESULT
{
	GUILD_DONATE_RESULT_SUCC = 0,
	GUILD_DONATE_RESULT_FAIL_ONCE_ADAY,
	GUILD_DONATE_RESULT_FAIL_MONEY_ENOUGH
};
enum GUILD_WAR_STATE
{
	GUILD_WAR_STATE_NONE = 0,
	GUILD_WAR_STATE_BID,
	GUILD_WAR_STATE_BID_END,
	GUILD_WAR_STATE_WAR_PREPARE,
	GUILD_WAR_STATE_IN_WAR,
	GUILD_WAR_STATE_WAR_FINISH_CONTINUE,
	GUILD_WAR_STATE_WAR_FINISH_END,
	GUILD_WAR_STATE_SEND_RUMOR_GROUP,
};

enum GUILD_WARCW_STATE
{
	GUILD_WARCW_STATE_NONE = 0,
	GUILD_WARCW_STATE_BID,
	GUILD_WARCW_STATE_BID_END,
	GUILD_WARCW_STATE_WAR_PREPARE,
	GUILD_WARCW_STATE_IN_WAR,
	GUILD_WARCW_STATE_WAR_FINISH_CONTINUE,
	GUILD_WARCW_STATE_WAR_FINISH_END,
	GUILD_WARCW_STATE_SEND_RUMOR_GROUP,
};

enum MAP_TYPE
{
	MAP_TYPE_OUTSIDE = 0,
	MAP_TYPE_CITY,
	MAP_TYPE_MAZE,
	MAP_TYPE_GUILD,
	MAP_TYPE_HOUSE,
	MAP_TYPE_FACTION,
	MAP_TYPE_INSTANCE_MANOR,
	MAP_TYPE_INSTANCE_DUNGEON,
	MAP_TYPE_INSTANCE_HOUSE,
	MAP_TYPE_GUILD_WAR,
	MAP_TYPE_FREE_PK,
};
enum GUILD_POSITION
{
	GUILD_POSITION_NIL = 0,
	GUILD_POSITION_MASTER,
	GUILD_POSITION_SECMASTER,
	GUILD_POSITION_ADMIN01,
	GUILD_POSITION_ADMIN02,
	GUILD_POSITION_ELDER,
	GUILD_POSITION_DEPTHEAD,
	GUILD_POSITION_ELITE,
	GUILD_POSITION_WARRIOR,
	GUILD_POSITION_TRADER,
	GUILD_POSITION_MEMBER,
	MAX_GUILD_POSITION,

};

enum TITLE_TYPE
{
	TITLE_TYPE_DB_ADD = 0,
	TITLE_TYPE_RELATION,
	TITLE_TYPE_RANK,
	TITLE_TYPE_GUILD,
	TITLE_TYPE_FORCE_TOP,
};

enum TITLE_PRENAME_TYPE
{
	TITLE_PRENAME_TYPE_NONE = 0,
	TITLE_PRENAME_TYPE_MASTER,
	TITLE_PRENAME_TYPE_COUPLE,
	TITLE_PRENAME_TYPE_SIBLING,
	TITLE_PRENAME_TYPE_GUILD,
	TITLE_PRENAME_TYPE_HUSBAND,
	TITLE_PRENAME_TYPE_WIFE,
};

enum TITLE_STATE
{
	TITLE_STATE_NORMAL = 0,
	TITLE_STATE_ADD,
	TITLE_STATE_REMOVE,
	TITLE_STATE_UPDATE,
};

enum PET_EMOTION_TYPE
{
	PET_EMOTION_BORING = 1,
	PET_EMOTION_ANGRY,
	PET_EMOTION_UNHAPPY,
	PET_EMOTION_HAPPY,
	PET_EMOTION_HAPPY2,
	PET_EMOTION_NAU,
	PET_EMOTION_HUNGRY,
	MAX_PET_EMOTION_TYPE,
};

enum PET_TYPE
{
	PET_TYPE_LITTLE_PET = 0,
	PET_TYPE_RIDE_PET,
};
enum PET_BREED_TYPE
{
	PET_BREED_TYPE_RENT = 0,
	PET_BREED_TYPE_TRAIN,
};

enum PET_STATE_TYPE
{
	PET_STATE_TYPE_REST = 0,
	PET_STATE_TYPE_USE,
};

enum TITLE_TIME_LIMIT_TYPE
{
	TITLE_TIME_LIMIT_TYPE_NONE = 0,
	TITLE_TIME_LIMIT_TYPE_ADDDAY,
	TITLE_TIME_LIMIT_TYPE_EXACTDAY,
};

enum PET_TIME_LIMIT_TYPE
{
	PET_TIME_LIMIT_TYPE_NONE = 0,
	PET_TIME_LIMIT_TYPE_CREATE,
	PET_TIME_LIMIT_TYPE_EQUIP,
	PET_TIME_LIMIT_TYPE_USE,
};
enum PARTNER_TIME_LIMIT_TYPE
{
	PARTNER_TIME_LIMIT_TYPE_NONE = 0,
	PARTNER_TIME_LIMIT_TYPE_CREATE,
	PARTNER_TIME_LIMIT_TYPE_EQUIP,
	PARTNER_TIME_LIMIT_TYPE_USE,
};
enum ITEM_TIME_LIMIT_TYPE
{
	ITEM_TIME_LIMIT_TYPE_NONE = 0,
	ITEM_TIME_LIMIT_TYPE_CREATE,
	ITEM_TIME_LIMIT_TYPE_EQUIP,
	ITEM_TIME_LIMIT_TYPE_USE,
};
enum CHAR_RANK_TYPE
{
	CHAR_RANK_TYPE_LEVEL = 0,
	CHAR_RANK_TYPE_MONEY,
	CHAR_RANK_TYPE_CLASS,
	CHAR_RANK_TYPE_FAMOUS,
	CHAR_RANK_TYPE_EVIL,
	CHAR_RANK_TYPE_FACTION,
	CHAR_RANK_TYPE_PVPSCROE,
	CHAR_RANK_TYPE_PKVALUE,
	MAX_CHAR_RANK_TYPE,
	GUILD_RANK_TYPE,
};
enum HELP_TIPS_TYPE
{
	HELP_TIPS_NONE = 0,
	HELP_TIPS_FIRST_LOGIN,
	HELP_TIPS_EVERY_LOGIN,
	HELP_TIPS_FIRST_ENOUGH_EXP,
	HELP_TIPS_FIRST_ITEM_CLICK,
	HELP_TIPS_FIRST_ITEM_PAYBAG_CLICK,
	HELP_TIPS_FIRST_ITEM_NONPAYBAG_CLICK,
	HELP_TIPS_FIRST_SKILL_CLICK,
	HELP_TIPS_FIRST_MISSION,
	HELP_TIPS_FIRST_PARNTER,
	HELP_TIPS_FIRST_PET,
	HELP_TIPS_FIRST_SYSTEM,
	HELP_TIPS_LEVEL_UP,
	HELP_TIPS_ENTER_MAP,
	HELP_TIPS_FIRST_HELP,
	HELP_TIPS_FIRST_ATTACK,
	HELP_TIPS_FIRST_TRADE,
	HELP_TIPS_FIRST_GIVE,
	HELP_TIPS_FIRST_PARTY,
	HELP_TIPS_FIRST_CHAR,
	HELP_TIPS_FIRST_GUILD,
	HELP_TIPS_FIRST_FRIEND,
	HELP_TIPS_FIRST_MISSION_HINTS,
	HELP_TIPS_FIRST_BIGMAP,
	HELP_TIPS_FIRST_MIDMAP,
	HELP_TIPS_FIRST_NPCSEARCH,
	HELP_TIPS_FIRST_ONLINE_SHOP,
	HELP_TIPS_FIRST_CHANGELINE,
	HELP_TIPS_FIRST_GETGUEST,
	HELP_TIPS_FIRST_GETITEM,
	HELP_TIPS_FIRST_GETPARTNER,
	HELP_TIPS_FIRST_GETPET,
	HELP_TIPS_FIRST_FORMTEAM,
	HELP_TIPS_FIRST_LEARNSKILL,
	HELP_TIPS_FIRST_JOINGUILD,
	HELP_TIPS_FIRST_MAKEFRIEND,
	HELP_TIPS_FIRST_PARTNER_COLLECTION,
	MAX_HELP_TIPS_TYPE,
};

enum FRIEND_GROUP_TYPE
{
	FRIEND_GROUP_TEMP = 0,
	FRIEND_GROUP_BLACK,
	FRIEND_GROUP_ONE,
	FRIEND_GROUP_TWO,
	FRIEND_GROUP_THREE,
	FRIEND_GROUP_FOUR,
	MAX_FRIEND_GROUP_TYPE,
};

enum FRIEND_RELATION_TYPE
{
	FRIEND_RELATION_FRIEND = 0,
	FRIEND_RELATION_BEING_RECOMMEND,
	FRIEND_RELATION_RECOMMEND,
	FRIEND_RELATION_MASTER,
	FRIEND_RELATION_STUDENT,
	FRIEND_RELATION_SIBLING,
	FRIEND_RELATION_HUSBAND,
	FRIEND_RELATION_WIFE,
	FRIEND_RELATION_COUNT,
};
enum FRIENC_CHAT_STATUS
{
	FRIENC_CHAT_STATUS_ONLINE = 0,
	FRIENC_CHAT_STATUS_ONLINE_AWAY,
	FRIENC_CHAT_STATUS_OFFLINE,
};
enum BATTLE_ERROR_MSG
{
	BATTLE_ERROR_MSG_BLOCK_ACTION = 0,
	BATTLE_ERROR_MSG_CATCH_NO_SP,
	BATTLE_ERROR_MSG_CATCH_DIE,
	BATTLE_ERROR_MSG_CATCH_GOODNESS,
	BATTLE_ERROR_MSG_CATCH_FULL_PARTNER,
	BATTLE_ERROR_MSG_CATCH_LEVEL,
	BATTLE_ERROR_MSG_CATCH_SAME_TEAM,
	BATTLE_ERROR_MSG_GIVEN,
	BATTLE_ERROR_ITEMBLOCK,
	SKILL_USE_FAIL_TYPE_TARGET_INVALID,
	SKILL_USE_FAIL_TYPE_NO_TARGET,
	SKILL_USE_FAIL_TYPE_BLOCK,
	SKILL_USE_FAIL_TYPE_NOITEM,
	SKILL_USE_FAIL_TYPE_USING,
	BATTLE_ERROR_MSG_NOT_FAVOR,
	BATTLE_ERROR_MSG_NOT_HEALTH,
	BATTLE_ERROR_MSG_LEVEL_TOO_HIGH,
	BATTLE_USE_MSG_GIVEN,

};
enum SKILL_FORM_TYPE
{
	SKILL_FORM_TYPE_NIL = 0,
	SKILL_FORM_TYPE_NORMAL_ATTACK,
	SKILL_FORM_TYPE_SPIRIT_ATTACK,
	SKILL_FORM_TYPE_ITEM_ATTACK,
	SKILL_FORM_TYPE_SOLID_ATTACK,
	SKILL_FORM_TYPE_HEAL,
	SKILL_FORM_TYPE_POINT,
	SKILL_FORM_TYPE_BUFF,
	SKILL_FORM_TYPE_DEBUFF,
	SKILL_FORM_TYPE_CONTROL,
	SKILL_FORM_TYPE_CONTINUE_DAM,
	SKILL_FORM_TYPE_CLEAR_BUFF,
	SKILL_FORM_TYPE_SUPPORT,
	SKILL_FORM_TYPE_RELIEF,
	SKILL_FORM_TYPE_TRANSPORT,
	SKILL_FORM_TYPE_SUMMON,
	SKILL_FORM_TYPE_MEDITATION,
	SKILL_FORM_TYPE_ANY = 255,
};
/*enum SKILL_USE_FAIL_TYPE
{
	SKILL_USE_FAIL_TYPE_MSG = 0,
	SKILL_USE_FAIL_TYPE_TARGET_INVALID,
	SKILL_USE_FAIL_TYPE_NO_TARGET,
	SKILL_USE_FAIL_TYPE_BLOCK,
	SKILL_USE_FAIL_TYPE_NOITEM,
	SKILL_USE_FAIL_TYPE_USING,
};*/
enum SKILL_SHOP_FAIL
{
	SKILL_SHOP_FAIL_EXP = 0,
	SKILL_SHOP_FAIL_MONEY,
	SKILL_SHOP_FAIL_LV,
	SKILL_SHOP_FAIL_MAX_RANK,
	SKILL_SHOP_FAIL_LV2,
	SKILL_SHOP_FAIL_HONOR,
	SKILL_SHOP_FAIL_DONATE,
	SKILL_SHOP_FAIL_YUANBAO,
	SKILL_SHOP_LEARN_OK,
};
enum SKILL_USE_TIME
{
	SKILL_USE_TIME_ANY = 0,
	SKILL_USE_TIME_BATTLE,
	SKILL_USE_TIME_PEACE,
};

enum SKILL_ACTIVE_TYPE
{
	SKILL_ACTIVE = 0,
	SKILL_PASSIVE,
};

enum BUFF_TYPE
{
	BUFF_TYPE_NIL = 0,
	BUFF_TYPE_BUFF,
	BUFF_TYPE_DEBUFF,
	BUFF_TYPE_EQ,
	BUFF_TYPE_PASSIVE,
	BUFF_TYPE_WINE,
	BUFF_TYPE_RESIST,
	BUFF_TYPE_REPEAT,
	MAX_BUFF_TYPE
};

enum BUFF_BLOCK_COMMAND
{
	BUFF_BLOCK_COMMAND_NIL = 0,
	BUFF_BLOCK_COMMAND_ATTACK,
	BUFF_BLOCK_COMMAND_SKILL,
	BUFF_BLOCK_COMMAND_SPECAIL,
	BUFF_BLOCK_COMMAND_ITEM,
	BUFF_BLOCK_COMMAND_DEFEND,
	BUFF_BLOCK_COMMAND_FRIEND,
	BUFF_BLOCK_COMMAND_PROTECT,
	BUFF_BLOCK_COMMAND_CATCH,
	BUFF_BLOCK_COMMAND_HELP,
	BUFF_BLOCK_COMMAND_AUTO,
	BUFF_BLOCK_COMMAND_ESCAPE,
	BUFF_BLOCK_COMMAND_ALL = 255,

};
enum BUFF_PERIOD_TYPE
{
	BUFF_PERIOD_TYPE_TURN = 0,
	BUFF_PERIOD_TYPE_MINUTES,
	BUFF_PERIOD_TYPE_FOREVER,
	BUFF_PERIOD_TYPE_COUNT,
};

enum BUFF_ACTIVE_TIME_TYPE
{
	BUFF_ACTIVE_TIME_TYPE_TURN_START = 0,
	BUFF_ACTIVE_TIME_TYPE_TURN_END,
	BUFF_ACTIVE_TIME_TYPE_ATTRI_BASE,
	BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE,
	BUFF_ACTIVE_TIME_TYPE_DO_ACTION,
	BUFF_ACTIVE_TIME_TYPE_DO_HITTING,
	BUFF_ACTIVE_TIME_TYPE_DO_REACTION,
	BUFF_ACTIVE_TIME_TYPE_DO_LEAD_REACTION,
	BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER,
	BUFF_ACTIVE_TIME_TYPE_DO_PROTECTTING,
};
enum BUFF_FUNC_NAME
{
	BUFF_FUNC_NAME_ATTRI = 0,
	BUFF_FUNC_NAME_ACTIVE,
	BUFF_FUNC_NAME_DECS,
	BUFF_FUNC_NAME_TIMEOUT,
	BUFF_FUNC_NAME_ADD,
	BUFF_FUNC_NAME_REMOVE,
	BUFF_FUNC_NAME_REPEAT,
	MAX_BUFF_FUNC_NAME,
};

enum PARTY_MSG_INDEX
{
	PARTY_MSG_NO_MSG = 0,
	PARTY_MSG_NO_PARTY,
	PARTY_MSG_NOT_LEADER_CANT_REPLAY,
	PARTY_MSG_MYSELF_IN_BATTLE,
	PARTY_MSG_TARGET_NOT_FIND,
	PARTY_MSG_TARGET_IN_BATTLE,
	PARTY_MSG_TARGET_NOT_IN_SAME_MAP,
	PARTY_MSG_TARGET_HAVE_PARTY,
	PARTY_MSG_TARGET_NO_PARTY,
	PARTY_MSG_TARGET_HAVENOT_REQUEST,
	PARTY_MSG_REPLAY_DISTANCE_TOO_FAR,
	PARTY_MSG_PARTY_FULL,
	PARTY_MSG_MYSELF_HAVE_PARTY,
	PARTY_MSG_NO_LEADER,
	PARTY_MSG_MYSELF_HAVENOT_INVITED,
	PARTY_MSG_IREJECT,
	PARTY_MSG_JREJECT,
	PARTY_MSG_JOIN_FAIL,
	PARTY_MSG_INVITE_FAIL,
	PARTY_MSG_PARTY_CANNOT_JOIN_TALK,
};

enum LINE_STATE
{
	LINE_STATE_REPAIR = 0,
	LINE_STATE_NORMAL,
	LINE_STATE_BUSY,
	LINE_STATE_SUPERFULL,
};


const static StringA CommonBuffFuncName[MAX_BUFF_FUNC_NAME] = 
{
	"Attri",
	"Active",
	"Desc",
	"Timeout",
	"Add",
	"Remove",
	"Repeat",
};


enum BUFF_END_TYPE
{
	BUFF_END_TYPE_NEVER_END = 0,
	BUFF_END_TYPE_TIME_OUT_END,
};
enum SKILL_USE_TARGET_TYPE
{
	SKILL_USE_TARGET_PLAYER = 0,
	SKILL_USE_TARGET_MOB,
	SKILL_USE_TARGET_NPC,
	SKILL_USE_TARGET_PARNTER,
};

enum PARTNER_SKILL_TYPE
{
	PARTNER_NORMAL_SKILL = 0,
	PARTNER_SPECIAL_SKILL,
};

enum ITEM_BASE_ELEMENT
{
	ITEM_ELEMENT_NONE = 0,
	ITEM_ELEMENT_GOLDEN ,
	ITEM_ELEMENT_TREE ,
	ITEM_ELEMENT_WATER ,
	ITEM_ELEMENT_FIRE ,
	ITEM_ELEMENT_EARTH ,
};

enum MixShopType
{
	MIX_EQUIPREPAIR = 0, 
	MIX_EQUIPDRILL,
	MIX_GEMINLAY,
	MIX_GEMREMOVE,
	MIX_WEAPONMAKING,
	MIX_CLOTHMAKING,
	MIX_ORNAMENTMAKING,
	MIX_COOKING,
	MIX_HERMETIC,
	MIX_MAKEWINE,
	MIX_GEMMIX,
	MIX_STONEMIX,
	MIX_UNSOULBOUND = 14,
	MIX_GEMMELTING = 15,
	MIX_GEMPOLISHING = 16,
	MIX_COLORSTONEMIX = 17,
	MIX_IRONMAKE,
	MIX_IRONREPAIR,
	MIX_NO_TYPE,
	
};

enum NpcType
{
	NPC_NORAML = 0,
	NPC_VISIBLE_EMENY_FOR_MANY_USER,
	NPC_INVISIBLE_EMENY,
	NPC_BUSINESSMAN,
	NPC_QUEST,
	NPC_VISIBLE_EMENY_FOR_ONE_USER,
};

enum AssetType
{
	ASSET_ITEM = 0,
	ASSET_PARTNER,
	ASSET_PET,
	ASSET_BANKMONEY,
	ASSET_YUANBAO,
};

enum AssetLockResultMsg
{
	ASSET_LOCK_ALREADY = 0,
	ASSET_UNLOCK_ALREADY,
	ASSET_LOCK_SUCCESS,
	ASSET_UNLOCK_SUCCESS,
	ASSET_LOCK_FAIL,
	ASSET_UNLOCK_FAIL,
	ASSET_RESET_PASS,
	ASSET_INIT_PASS,
	ASSET_PASSWORD_UNVERIFIED,
	ASSET_PASSWORD_INVALID,
	ASSET_LOCK_SCRIPT_ERR,
	ASSET_LOCK_CHAR_ERR,
	ASSET_LOCK_BATTLE_ERR,
	ASSET_LOCK_TRADE_ERR,
	ASSET_LOCK_HAWK_ERR,
	ASSET_LOCK_PASS_ERR,
	ASSET_INVALID_ERR,
};
enum MARK_TYPE
{
	MARK_ENABLE_PK = 0,
	MARK_VISIBLE,
	MARK_TYPE_COUNT,
};

struct AttriStruct
{
	int uIntAtk;
	int uIntDef;
	int uIntHit;
	int uIntEva;
	int uIntPow;
	int uIntSpd;
	int uIntMaxHp;
	int uIntMaxSp; 
	int uIntMaxDp; 
	int uIntMaxWc; 
	
	AttriStruct()
	{
		ZeroMemory(this, sizeof(AttriStruct));
	}
	int GetAtk(){return uIntAtk;}
	int GetDef(){return uIntDef;}
	int GetHit(){return uIntHit;}
	int GetEva(){return uIntEva;}
	int GetPow(){return uIntPow;}
	int GetSpd(){return uIntSpd;}
	int GetMaxHp(){return uIntMaxHp;}
	int GetMaxSp(){return uIntMaxSp;}
	int GetMaxDp(){return uIntMaxDp;}
	int GetMaxWc(){return uIntMaxWc;}

	void SetAtk(int value){uIntAtk = value;}
	void SetDef(int value){uIntDef = value;}
	void SetHit(int value){uIntHit = value;}
	void SetEva(int value){uIntEva = value;}
	void SetPow(int value){uIntPow = value;}
	void SetSpd(int value){uIntSpd = value;}
	void SetMaxHp(int value){uIntMaxHp = value;}
	void SetMaxSp(int value){uIntMaxSp = value;}
	void SetMaxDp(int value){uIntMaxDp = value;}
	void SetMaxWc(int value){uIntMaxWc = value;}
};

struct AttriStruct2
{
	int uIntSta;
	int uIntSpr;
	int uIntStr;
	int uIntCon;
	int uIntAgi;

	AttriStruct2()
	{
		ZeroMemory(this, sizeof(AttriStruct2));
	}
	int GetSta(){return uIntSta;}
	int GetSpr(){return uIntSpr;}
	int GetStr(){return uIntStr;}
	int GetCon(){return uIntCon;}
	int GetAgi(){return uIntAgi;}

	void SetSta(int value){uIntSta = value;}
	void SetSpr(int value){uIntSpr = value;}
	void SetStr(int value){uIntStr = value;}
	void SetCon(int value){uIntCon = value;}
	void SetAgi(int value){uIntAgi = value;}
};

struct ExtractedBuffData
{
	UInt16 ID;
	UInt32 Value1;
	UInt32 Value2;
	UInt32 Value3;
	UInt32 CoolDown;
	UInt8 Pro;
	UInt8 StartDelay;
	UInt32 Repeat;

	UInt32 PreUpdateTime;

	const BuffData * pBuffData;


	ExtractedBuffData()
	{
		ZeroMemory(this, sizeof(ExtractedBuffData));

		PreUpdateTime = ::timeGetTime();
		Repeat = 0;
	}

	bool operator >= (const ExtractedBuffData& b) const
	{
		if (Value1 > b.Value1)
			return true;
		if (Value1 == b.Value1 &&
			Value2 > b.Value2)
			return true;
		if (Value1 == b.Value1 &&
			Value2 == b.Value2 &&
			Value3 > b.Value3)
			return true;
		if (Value1 == b.Value1 &&
			Value2 == b.Value2 &&
			Value3 == b.Value3 &&
			ID >= b.ID)
			return true;
		return false;
	}

	UInt16 GetBuffID(){return ID;}
	UInt32 GetBuffValue1(){return Value1;}
	UInt32 GetBuffValue2(){return Value2;}
	UInt32 GetBuffValue3(){return Value3;}
	UInt32 GetBuffCoolDown(){return CoolDown;}
	UInt8	GetBuffPro(){return Pro;}

	void SetBuffValue1(UInt32 GivenValue)
	{
		Value1 = GivenValue;
	}

	void SetBuffValue2(UInt32 GivenValue)
	{
		Value2 = GivenValue;
	}

	void SetBuffValue3(UInt32 GivenValue)
	{
		Value3 = GivenValue;
	}
};

typedef StlMap<UInt16, ExtractedBuffData, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, ExtractedBuffData> > > BuffMap;
typedef StlMultiMap<UInt16, ExtractedBuffData, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, ExtractedBuffData> > > BuffMultiMap;

enum BUFF_MODFIY_TYPE
{
	BUFF_MODFIY_TYPE_REMOVE = 0,
	BUFF_MODFIY_TYPE_ADD,
	BUFF_MODFIY_TYPE_MODIFY,
};

enum BUFF_MOD_VALUE_TYPE
{
	BUFF_MOD_VALUE_NIL = 0,
	BUFF_MOD_VALUE_STR,
	BUFF_MOD_VALUE_CON,
	BUFF_MOD_VALUE_STA,
	BUFF_MOD_VALUE_AGI,
	BUFF_MOD_VALUE_SPR,
	BUFF_MOD_VALUE_ATK,
	BUFF_MOD_VALUE_DEF,
	BUFF_MOD_VALUE_HIT,
	BUFF_MOD_VALUE_EVA,
	BUFF_MOD_VALUE_PWD,
	BUFF_MOD_VALUE_SPD,
	BUFF_MOD_VALUE_MAX_HP,
};
enum SKILL_FUNC_NAME
{
	SKILL_FUNC_NAME_USE = 0,
	SKILL_FUNC_NAME_ATTRI,
	SKILL_FUNC_NAME_DESC,
	SKILL_FUNC_NAME_CONSUME,
	SKILL_FUNC_NAME_PERFORM,
	SKILL_FUNC_NAME_CANUSE,
	MAX_SKILL_FUNC_NAME,
};

const static StringA CommonSkillFuncName[MAX_SKILL_FUNC_NAME] = 
{
	"Use",
	"Attri",
	"Desc",
	"Consume",
	"Perform",
	"CanUse"
};

enum TARGET_TYPE
{
	TARGET_TYPE_ANY = 0,
	TARGET_TYPE_SELF,
	TARGET_TYPE_ANY_PARNTER,
	TARGET_TYPE_ANY_PET,
	TARGET_TYPE_ANY_SAME_TEAM,
	TARGET_TYPE_ANY_ENEMY_TEAM,
	TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PARNTER,
	TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS_PLAYER,
	TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PLAYER_PARNTER,
	TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_PLAYER_PARNTER,
	TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS,
	TARGET_TYPE_ANY_ITEM,
	TARGET_TYPE_ANY_MISSION,
	TARGET_TYPE_ANY_SAME_TEAM_PLAYER,
	TARGET_TYPE_ANY_SAME_TEAM_PARNTER,
	TARGET_TYPE_ANY_SAME_TEAM_PET,
};

enum SKILL_TARGET
{
	SKILL_TARGET_ONE = 0,
	SKILL_TARGET_WHOLE_TEAM,
	SKILL_TARGET_AND_PARNTER,
	SKILL_TARGET_VEC,
	SKILL_TARGET_HOR,
};

enum LEARN_SKILL_RETURN
{
	LEARN_SKILL_RETURN_SUCCESS = 0,
	LEARN_SKILL_RETURN_FAIL,
};
enum PK_VALUE
{
	PK_VALUE_NOT_ALLOW = 0,
	PK_VALUE_PK,
	PK_VALUE_MATCH,
	PK_VALUE_LOW_MARTIAL_PK,
	PK_VALUE_HIGH_MARTIAL_PK,
};
enum ITEM_USE_TARGET_TYPE
{
	ITEM_USE_TARGET_TYPE_ANY = 0,
	ITEM_USE_TARGET_TYPE_SELF,
	ITEM_USE_TARGET_TYPE_ANY_SAME_TEAM,
	ITEM_USE_TARGET_TYPE_ANY_ENEMY_TEAM,
	ITEM_USE_TARGET_TYPE_ANY_PLAYER,
	ITEM_USE_TARGET_TYPE_ANY_PLAYER_SAME_TEAM,
	ITEM_USE_TARGET_TYPE_ANY_PLAYER_ENEMY_TEAM,
	ITEM_USE_TARGET_TYPE_ANY_PARNTER,
	ITEM_USE_TARGET_TYPE_ANY_PARNTER_SAME_TEAM,
	ITEM_USE_TARGET_TYPE_ANY_PARNTER_ENEMY_TEAM,
	ITEM_USE_TARGET_TYPE_ANY_MOB,
	ITEM_USE_TARGET_TYPE_ANY_PET,
};
enum SKILL_OWNER_TYPE
{
	SKILL_OWNER_TYPE_CHAR = 0,
	SKILL_OWNER_TYPE_PARTNER,
	SKILL_OWNER_TYPE_PET,
};

enum BATTLE_STATE
{
	BATTLE_STATE_IN_WAR = 0,
	BATTLE_STATE_MONITOR,
};
enum REQUEST_JOIN_BATTLE
{
	REQUEST_JOIN_BATTLE_OK = 0,
	REQUEST_JOIN_BATTLE_FAIL_ALREADY_INSIDE,
	REQUEST_JOIN_BATTLE_FAIL_TOO_FRE,
	REQUEST_JOIN_BATTLE_FAIL_TOO_MANY,
	REQUEST_JOIN_BATTLE_FAIL_PK_LV,
	REQUEST_JOIN_BATTLE_FAIL_PK_ENABLE,
	REQUEST_JOIN_BATTLE_FAIL_PK_FORCE,
	REQUEST_JOIN_BATTLE_FAIL,
};
enum JOIN_BATTLE_REUSLT
{
	JOIN_BATTLE_OK = 0,
	JOIN_BATTLE_FAIL_FULL,
	JOIN_BATTLE_REJECT,
	JOIN_BATTLE_ALREADYJOIN,
	JOIN_BATTLE_INVALID,
};
enum BATTLE_TYPE
{
	NPC_BATTLE = 0,
	PK_BATTLE,
	FORCE_PK_BATTLE,
	MATCH_BATTLE,
	LOW_MARTIAL_BATTLE,
	HIGH_MARTIAL_BATTLE,
	REJECT_MATCH_BATTLE,
	GUILD_WAR_BATTLE,
	FREE_PK_BATTLE,
	INCOGNITO_PK_BATTLE,
};

enum PARTNER_TYPE
{
	CANNOT_BE_PARTNER = 0,
	LEVEL_ZERO_PARTNER,
	LEVEL_FOLLOW_MOB_PARTNER,
	LEVEL_NOT_CHANGE_PARTNER,
	FORCE_BATTLE_PARTNER,
	LEVEL_NOT_CHANGE_FOLLOW_OWNER,
	LEVEL_FOLLOW_OWNER,
};

enum SKILL_ANIM_TYPE
{
	SKILL_ANIM_TYPE_ORG_POS = 0,
	SKILL_ANIM_TYPE_CENTER_POS,
	SKILL_ANIM_TYPE_TARGET_POS,
	SKILL_ANIM_TYPE_ORG_POS_1BY1,
	SKILL_ANIM_TYPE_CENTER_POS_1BY1,
	SKILL_ANIM_TYPE_TARGET_POS_1BY1,
	SKILL_ANIM_TYPE_LUA_SPEC,
};

enum PK_REQUEST_RESULT
{
	PK_REQUEST_RESULT_SUCCESS = 0,
	PK_REQUEST_RESULT_FAIL_NOT_FOUND_TARGET,
	PK_REQUEST_RESULT_FAIL_CANNOT_TARGET_SELF,
	PK_REQUEST_RESULT_FAIL_IN_BATTLE_TARGET,
	PK_REQUEST_RESULT_FAIL_MONITOR_FULL,
	PK_REQUEST_RESULT_FAIL_IN_MONITOR,
	PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_TARGET,
	PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_TARGET,
	PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_TARGET,
	PK_REQUEST_RESULT_FAIL_NOT_LEADER_TARGET,
	PK_REQUEST_RESULT_FAIL_NOT_FOUND_SELF,
	PK_REQUEST_RESULT_FAIL_IN_BATTLE_SELF,
	PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_SELF,
	PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_SELF,
	PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_SELF,
	PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_HONOR_SELF,
	PK_REQUEST_RESULT_FAIL_NOT_LEADER_SELF,
	PK_REQUEST_RESULT_MATCH_BATTLE_REQUEST,
	PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_REJECT,
	PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_TARGET_CHANGE,
	PK_REQUEST_RESULT_FAIL_PROTECT_PK,
	PK_REQUEST_RESULT_FAIL_BLOCK_PK,
	PK_REQUEST_RESULT_FAIL_TARGET_BUSY,
	PK_REQUEST_RESULT_FAIL_TARGET_HAWKING,
	PK_REQUEST_RESULT_FAIL_TARGET_ALREADY_ASKED,
	PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_MONEY,
	PK_REQUEST_RESULT_LOW_MARTIAL_BATTLE_REQUEST,
	PK_REQUEST_RESULT_HIGH_MARTIAL_BATTLE_REQUEST,
	PK_REQUEST_RESULT_FAIL_SAME_GUILD,
	PK_REQUEST_RESULT_FAIL_RESTING,
	PK_REQUEST_RESULT_FAIL_VP_NOT_ENOUGH,
	PK_REQUEST_RESULT_FAIL_WAR_NOT_START,
	PK_REQUEST_RESULT_FAIL_TARGET_NOT_ENABLE,
};
enum SKILL_TYPE
{
	SKILL_TYPE_GANG = 0,
	SKILL_TYPE_GANG_BASE,
	SKILL_TYPE_PARTNER_NORMAL,
	PET_SPEED_SKILL,
	SKILL_TYPE_GUILD,
	SKILL_TYPE_GUILD_BASE,
	SKILL_TYPE_LIVE,
	SKILL_TYPE_LIVE_BASE,
	SKILL_TYPE_PARTNER_SPECIAL,
	SKILL_TYPE_BATTLE,
	SKILL_TYPE_SPECIAL = 12,
	SKILL_TYPE_SPECIAL_BASE = 13,
	PET_NORMAL_SKILL = 14,
	MAX_SKILL_TYPE,
};
enum TalentEnum
{
	MOB_TALENT_A = 0,
	MOB_TALENT_B,
	MOB_TALENT_C,
};

enum ChargeShopIndex
{
	MIX = 0,
	ASSC,
	PET,
	PARTNER,
	CLOTH,
	OTHER,
	HOTSALE,
	LIMITED,
	DISCOUNT,
	DIAMOND,
	BOOK,
	CAP,
	FIREWORKS,
	MOULD,
};

enum ChargeShopItemType
{
	CHARGE_ITEM_TYPE_ITEM = 0,
	CHARGE_ITEM_TYPE_PARTNER,
	CHARGE_ITEM_TYPE_PET,
};

enum GetChargeShopIndex
{
	GETALL = 0,
	GETACTIVEONLY,
	GETDISACTIVEONLY,
	GETLIMITONLY,
};


enum LimitedItemSellDay
{
	ONE_TIME_ONLY = 0,
	EVERY_MONDAY,
	EVERY_TUESDAY,
	EVERY_WEDNESDAY,
	EVERY_THURDAY,
	EVERY_FRIDAY,
	EVERY_SATURDAY,
	EVERY_SUNDAY,
	EVERYDAY,
};

enum ChargeShopMsgCode
{
	CHARGESHOP_BUY_SUCCESS = 0,
	CHARGESHOP_NO_SELL_ITEM,
	CHARGESHOP_NO_ITEM_SLOT,
	CHARGESHOP_NO_PARTNER_SLOT,
	CHARGESHOP_NO_PET_SLOT,
	CHARGESHOP_RUN_SCRIPT,
	CHARGESHOP_CHAR_IN_BATTLE,
	CHARGESHOP_NO_YUANBAO,
	CHARGESHOP_NOT_SALEDAY,
	CHARGESHOP_NO_REMAIN,
	CHARGESHOP_BOUGHT_LIMIT_ALREADY,
	CHARGESHOP_ITEM_REACH_LIMIT,
	CHARGESHOP_CAHR_ERROR,
	CHARGESHOP_YUANBAO_LOCK_ERROR,	
};

enum MixShopMsgCode
{
	MIXSHOP_SUCCESS = 0,
	MIXSHOP_FAIL,
	MIXSHOP_ERROR,
	MIXSHOP_NO_VP,
	MIXSHOP_NO_EXP,
	MIXSHOP_NO_GMONEY,
	MIXSHOP_NO_MONEY,
	MIXSHOP_NO_HP,
	MIXSHOP_NO_MHP,
	MIXSHOP_NO_SP,
	MIXSHOP_NO_MSP,
	MIXSHOP_NO_GOODNESS,
	MIXSHOP_NO_HONOR,
	MIXSHOP_NO_FACTIONDONATE,
	MIXSHOP_NO_MANORDONATE,
	MIXSHOP_NO_ITEMSLOT,
	MIXSHOP_NO_EQ,
	MIXSHOP_NO_NEED_REBUILD,
	MIXSHOP_NO_NEED_REPAIR,
	MIXSHOP_NO_MENDURING,
	MIXSHOP_NO_HIGHER_RESULT,
	MIXSHOP_ITEM_REACH_LIMIT,
	MIXSHOP_SUCCESS_REDUCE_MENDURING,
	MIXSHOP_SUCCESS_REBUILD,
};

enum GiftBoxMsgCode
{
	GIFTBOX_SUCCESS = 0,
	GIFTBOX_ERROR,
	GIFTBOX_NO_ITEM_SLOT,
	GIFTBOX_NO_PARTNER_SLOT,
	GIFTBOX_NO_PET_SLOT,
	GIFTBOX_ITEM_REACH_LIMIT,
	GIFTBOX_UDPATE_SUCCESS,
	GIFTBOX_UDPATE_FALSE,
	GIFTBOX_UDPATE_NOT_IN_MAP,
	GIFTBOX_UDPATE_SCRIPT_RUNNING,
	GIFTBOX_UDPATE_TRADING,
	GIFTBOX_UDPATE_TIMEOUT,
};

enum MixItemQuality
{
	MIXITEM_QUA_FAIL = 0,
	MIXITEM_QUA_NORMAL,
	MIXITEM_QUA_FINE,
	MIXITEM_QUA_PRETTY,
	MIXITEM_QUA_HIQUALITY,
	MIXITEM_QUA_BEST,
	MIXITEM_QUA_GOD,

};

enum MixShopConsumeType
{
	MIXSHOP_CONSUME_NONE = 0,
	MIXSHOP_CONSUME_VP,
	MIXSHOP_CONSUME_EXP,
	MIXSHOP_CONSUME_MONEY,
	MIXSHOP_CONSUME_YUANBAO,
	MIXSHOP_CONSUME_HP,
	MIXSHOP_CONSUME_MHP,
	MIXSHOP_CONSUME_SP,
	MIXSHOP_CONSUME_MSP,
	MIXSHOP_CONSUME_GOODNESS,
	MIXSHOP_CONSUME_HONOR,
	MIXSHOP_CONSUME_FACTIONDONATE,
	MIXSHOP_CONSUME_MANORDONATE,
};

enum SoulBoundType
{
	SOULBOUND_NO_BOUND = 0,
	SOULBOUND_GET_BOUND,
	SOULBOUND_EQ_BOUND,
	SOULBOUND_USE_BOUND,
	SOULBOUND_ALREADY_BOUND,

};

enum HawkerMenu
{
	HAWKER_ITEM = 0,
	HAWKER_PARTNER,
	HAWKER_PET,
	HAWKER_EXCHANGE_RECORD,
	HAWKER_CLOSE,
};

enum HawkMessageIndex
{
	HAWK_START = 0,
	HAWK_TIMEUP,
	HAWK_NOT_RIGHT_PLACE,
	HAWK_NO_TIME,
	HAWK_CANT_HAVE_PARTY,
	HAWK_NO_ITEM,
	HAWK_NO_MONEY,
	HAWK_ITEM_BIND,
	HAWK_PARTNER_BIND,
	HAWK_PET_BIND,
	HAWK_NO_ITEM_SLOT,
	HAWK_NO_PARTNER_SLOT,
	HAWK_NO_PET_SLOT,
	HAWK_NO_MESSAGE,
	HAWK_NOT_ENOUGH_LV,
	HAWK_PLACE_HAVE_HAWKER,
	HAWK_NOT_ENOUGH_HAWKITEM,
	HAWK_ITEM_LOCK,
	HAWK_PET_LOCK,
	HAWK_PARTNER_LOCK,
	HAWK_MIXPET_LEVEL_MAX,
	HAWK_PARTNER_REMOVE,
	HAWK_ITEM_FLOWER_MODEL,
};

enum CheckNameMsgIndex
{
	CHECKNAME_OK = 0,
	CHECKNAME_TOO_LONG,
	CHECKNAME_CANT_USE,
	CHECKNAME_HAS_CHARACTER,
};

enum ItemTextColorIndex
{
	ItemColorIndex_Grey = 0,
	ItemColorIndex_LightGreen,
	ItemColorIndex_LightBlue,
	ItemColorIndex_LightPurple,
	ItemColorIndex_LightGolden,
	ItemColorIndex_GodGolden,
	ItemColorIndex_Golden,
	ItemColorIndex_Red,
	ItemColorIndex_LightGrey,
};

enum IdentifyIndex
{
	IDENTIFY_NO_IDENTIFY_ITEM = 0,
	IDENTIFY_CAN_IDENTIFY ,
	IDENTIFY_SUCCESS,
	IDENTIFY_ERROR_ITEM,
	IDENTIFY_NO_ITEM,
	IDENTIFY_ALREADY_IDENTIFY,
	IDENTIFY_FAIL_REQ,
};

enum ITEM_MENU_TYPE
{
	ITEM_CHAR_MENU = 0,
	ITEM_PARTNER_MENU,
	ITEM_PET_MENU,
};

enum GiftType
{
	GIFT_ITEM = 0,
	GIFT_PARTNER,
	GIFT_PET,
};

enum ItemMsgIndex
{
	ITEM_MSG_REACH_LIMIT = 50,
};

enum GivenMsgIndex
{
	GIVEN_SUCCESS = 0,
	GIVEN_RUNNING_SCRIPT_CANT_GIVEN,
	GIVEN_MYSELF,
	GIVEN_CHAR_FAIL,
	GIVEN_BLOCK,
	GIVEN_TRADING_CANT_GIVEN,
	GIVEN_OFFLINE_HAWKING_CANT_GIVEN,
	GIVEN_DIS_FAIL,
	GIVEN_GIVE_NOTHING,
	GIVEN_ITEM_STACK_FAIL,
	GIVEN_ITEM_BOUND,
	GIVEN_ITEM_LOCK,
	GIVEN_ITEM_NO_SLOT,
	GIVEN_PARTNER_FAIL,
	GIVEN_PARTNER_BOUND,
	GIVEN_PARTNER_LOCK,
	GIVEN_PET_FAIL,
	GIVEN_PET_BOUND,
	GIVEN_PET_LOCK,
	GIVEN_MONEY_FAIL,
	GIVEN_FLOWER_NOT_MINE,
	GIVEN_FLOWER_NOT_SEX_WOMAN,
	GIVEN_FLOWER_NOT_SEX_MAN,
	GIVEN_FLOWER_NOT_ENOUGH_FRIENDLY,
	GIVEN_FLOWER_FULL_FRIENDLY,
	GIVEN_FLOWER_TIME_LIMIT,
};

enum TradingMsgIndex
{
	TRADING_ASKING = 0,
	TRADING_FAIL_SCRIPT,			//MSG_SCRIPT_TRADING_CANT
	TRADING_FAIL_CHAR_NOT_FIND,		//MSG_TRADING_CHAR_FAIL
	TRADING_FAIL_MYSELF,			//MSG_TRADING_MYSELF
	TRADING_FAID_BLOCK,				//MSG_BLOCK_TRADING
	TRADING_FAIL_BATTLE,			//MSG_TRADING_BATTLE
	TRADING_FAIL_HAWKING,			//MSG_HAWK_DISCONNECT_HAWKING_CANT_TRADING
	TRADING_FAIL_ALREADY_TRADING,	//MSG_TRADING_PLEASE_WAIT
	TRADING_FAIL_TARGET_ALREADY_TRADING, //MSG_TRADING_TARGET_BUSY
	TRADING_FAIL_REQ_TIMEOUT,		//MSG_TRADING_REQ_TIMEOUT
	TRADING_FAIL_REQ_CANCEL,		//MSG_TRADING_REQ_CANCEL
	TRADING_CONFIRM,				//MSG_TRADING_CONFIRM
	TRADING_FAIL_DISTANCE_TOO_FAR,	//MSG_TRADING_DIS_FAIL
	TRADING_FAIL_GUILD_WAR,		//MSG_TRADING_FAIL_GUILD_WAR
	TRADING_FAIL_NOT_SUIT_MAP,		//MSG_TRADING_FAIL_NOT_SUIT_MAP
};

enum WareHouseMsgIndex
{
	WAREHOUSE_SUCCESS = 0,
	WAREHOUSE_NOT_IN_MAP,
	WAREHOUSE_RUNNING_SCIRPT,
	WAREHOUSE_QUEST_ITEM,
	WAREHOUSE_FULL,
	WAREHOUSE_ITEM_STACK_WRONG,
	WAREHOUSE_NO_ITEM,
	WAREHOUSE_MOVE_SUCCESS,
	WAREHOUSE_NO_PARTNER,
	WAREHOUSE_PUT_IN_UING_STATE,
	WAREHOUSE_NO_PET,
	WAREHOUSE_CANNOT_TRADING,
};

enum WareHouseProcessIndex
{
	WAREHOUSE_PROCESS_PUT_ITEM = 0,
	WAREHOUSE_PROCESS_GET_ITEM,
	WAREHOUSE_PROCESS_MOVE_ITEM,
	WAREHOUSE_PROCESS_PUT_PARTNER,
	WAREHOUSE_PROCESS_GET_PARTNER,
	WAREHOUSE_PROCESS_PUT_PET,
	WAREHOUSE_PROCESS_GET_PET,
};

#define PARTNER_COLLECTION_BUFF_SCHOOL_ID	8000

enum PartnerCollectionEnableMsgIndex
{
	PARTNER_COLLECTION_ENABLE_OR_DISABLE_FAIL = 0,
	PARTNER_COLLECTION_ENABLE_SUCCESS,
	PARTNER_COLLECTION_DISABLE_SUCCESS,
};

enum PartnerCollectionAddMsgIndex
{
	PARTNER_COLLECTION_ADD_SUCCESS = 0,
	PARTNER_COLLECTION_ADD_NOT_EXIST,
	PARTNER_COLLECTION_ADD_USELESS,
	PARTNER_COLLECTION_ADD_ALREADY_EXIST_IN_COLLECTION,
	PARTNER_COLLECTION_ADD_LEVEL_REQUIRE,
	PARTNER_COLLECTION_ADD_LOCKED,
};

enum PartnerCultivationUpMsgIndex
{
	PARTNER_CULTIVATION_UP_SUCCESS = 0,
	PARTNER_CULTIVATION_UP_FAIL,
	PARTNER_CULTIVATION_UP_FAIL_NOT_PARTNER,
	PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_RACE,
	PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_LOCK,
	PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_RACE,
	PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_LOCK,
	PARTNER_CULTIVATION_UP_FAIL_PRACTICE,
	PARTNER_CULTIVATION_UP_FAIL_MONEY,
	PARTNER_CULTIVATION_UP_FAIL_MAX_CULTIVATION,
	PARTNER_CULTIVATION_UP_FAIL_REQLV,
	PARTNER_CULTIVATION_UP_FAIL_ITEMTYPE,
	PARTNER_CULTIVATION_UP_FAIL_ITEMLOCK,
	PARTNER_CULTIVATION_UP_FAIL_ITEMLEVEL,
};

enum StartHawkAddType
{
	START_HAWK_ADD_ITEM = 0,
	START_HAWK_ADD_PARTNER,
	START_HAWK_ADD_PET,
};

enum WarningMessageIndex
{
	WARN_CHARACTER_OVERFLOW_EXP = 0,
	WARN_CHARACTER_CHANGE_LINE_FAIL,
	WARN_CHARACTER_CHANGE_LINE_TIME_LIMIT,
	WARN_CHARACTER_CHANGE_LINE_SUCCESS,
	WARN_CHAT_CHANGE_ICON_NO_YUANBAO,
	WARN_PARTY_REGROUP_FAIL,
	WARN_PARTY_CANT_CHANGE_LINE,
	WARN_PARTY_CANT_INVITE_MYSELF,
	WARN_TRADING_CANT_CHANGE_LINE,
	WARN_HAWKING_CANT_CHANGE_LINE,
	WARN_HAWKING_CANT_PARTY,
	WARN_SYS_LINE_FULL,
	WARN_INSTANCE_FULL,
	WARN_GUILDWAR_CANT_CHANGE_LINE,
	WARN_SPECIFICMAP_CANT_CHANGE_LINE,
	WARN_CHAT_CHANGE_ICON_YUANBAO_LOCK,	
	WARN_TRADING_CANT_SELLYUAN,
	WARN_HAWKING_CANT_SELLYUAN,
	WARN_NOINMAP_CANT_SELLYUAN,
	WARN_SCRIPTING_CANT_SELLYUAN,
};

enum WarningShowInIndex
{
	WARN_SHOW_IN_CHATBOX = 0,
	WARN_SHOW_IN_MIDDLE,
	WARN_SHOW_IN_ALL,
};

enum ChangeYBType
{
	CHANGEYBT_YB_TO_YBT = 0,
	CHANGEYBT_YBT_TO_YB,
};

enum MarriageBowState
{
	BOW_WAIT = 0,
	BOW_HEAVEN_EARTH = 1,
	BOW_FOREFATHER = 2,
	BOW_COUPLE = 3,
	BOW_FINISH = 4,

};

enum SellYBMsgIndex
{
	CHANGE_YB_SUCCESS = 0,
	CHANGE_YB_FAIL,
	SELL_YB_ADD_RECORD_SUCCESS,
	SELL_YB_IN_TRADING,
	SELL_YB_IN_HAWKING,
	SELL_YB_NOT_IN_MAP,
	SELL_YB_SCRIPT_RUNNING,
	SELL_YB_MEET_MAX_RECORD,
	SELL_YB_NOT_ENOUGH_MONEY,
	SELL_YB_NOT_ENOUGH_YBT,
	SELL_YB_ERROR,
	BUY_YB_SUCCESS,
	BUY_YB_ERROR,
	BUY_YB_SOLD_ALL,
	BUY_YB_NO_MONEY,
	BUY_YB_IN_TRADING,
	BUY_YB_IN_HAWKING,
	BUY_YB_NOT_IN_MAP,
	BUY_YB_SCRIPT_RUNNING,
	BUY_YB_EXPIRE,
	TEMPBANK_SUCCESS,
	TEMPBANK_ERROR,
	TEMPBANK_IN_TRADING,
	TEMPBANK_IN_HAWKING,
	TEMPBANK_NOT_IN_MAP,
	TEMPBANK_SCRIPT_RUNNING,
	CHANGE_YB_IN_TRADING,
	CHANGE_YB_IN_HAWKING,
	CHANGE_YB_NOT_IN_MAP,
	CHANGE_YB_SCRIPT_RUNNING,
	CHANGE_YB_YUANBAOLOCK,
	CHANGE_YB_WRONGPASSWORD,
	YB_MSG_WRONG_PASSWORD,
	YB_MSG_NONE,
};

struct BinaryBlockInfoHeader
{
	UInt16 version;
	UInt16 width; // of first row
	UInt16 height;
	UInt8 blocktype; // 0 for even distribution across rows (eg 9-9-9..), 1 for uneven distribution across rows (eg 9-8-9..)
	UInt32 blocksizeinbyte;

	BinaryBlockInfoHeader()
	{
		version = 0;
		width = 0;
		height = 0;
		blocktype = 0;
		blocksizeinbyte = 0;
	}
};

struct DefaultInfoHeader
{
	UInt16 version;

	DefaultInfoHeader()
	{
		version = 0;
	}
};

struct DefaultPFHeader
{
	UInt16 version;
	UInt32 numGuildPoint; // num of guild points
	UInt32 sizeWeightGraph;
	UInt32 sizeDistGraph;
	UInt32 sizeLinkGraph;

	DefaultPFHeader()
	{
		version			= 0;
		numGuildPoint	= 0;
		sizeWeightGraph	= 0;
		sizeDistGraph	= 0;
		sizeLinkGraph	= 0;
	}
};

struct BinaryServerMapInfo
{
	UInt32 m_mapid;
	TCHAR m_mapname[32];
	UInt8 m_maptype;
	UInt8 m_mapinoutdoor;
	UInt8 m_mapdaynight;
	UInt8 m_mapweathermode;
	UInt8 m_mapweathertype;
	UInt16 m_zoneencounterrate;
	UInt8 m_maplevel;
	UInt8 m_oneventopen;
	UInt8 m_changelineenabled;

	BinaryServerMapInfo()
		: m_mapid(0)
		, m_maptype(0)
		, m_mapinoutdoor(0)
		, m_mapdaynight(0)
		, m_mapweathermode(0)
		, m_mapweathertype(0)
		, m_zoneencounterrate(0)
		, m_maplevel(0)
		, m_oneventopen(0)
		, m_changelineenabled(0)
	{
		ZeroMemory(m_mapname, sizeof(TCHAR) * 32);
	}
};

struct BinaryClientMapInfo
{
	UInt32 m_mapid;
	TCHAR m_mapmapfile[32];
	TCHAR m_mapname[32];
	UInt8 m_maptype;
	UInt8 m_mapinoutdoor;
	UInt8 m_mapdaynight;
	UInt8 m_mapweathermode;
	UInt8 m_mapweathertype;
	TCHAR m_minimapfile[32];
	TCHAR m_soundfile[32];
	UInt8 m_changelineenabled;

	BinaryClientMapInfo()
		: m_mapid(0)
		, m_maptype(0)
		, m_mapinoutdoor(0)
		, m_mapdaynight(0)
		, m_mapweathermode(0)
		, m_mapweathertype(0)
		, m_changelineenabled(0)
	{
		ZeroMemory(m_mapmapfile, sizeof(TCHAR) * 32);
		ZeroMemory(m_mapname, sizeof(TCHAR) * 32);
		ZeroMemory(m_minimapfile, sizeof(TCHAR) * 32);
		ZeroMemory(m_soundfile, sizeof(TCHAR) * 32);
	}
};

struct BinaryServerNPCTemplate
{
	UInt32 m_npc_id;
	UInt8 m_npc_type;
	TCHAR m_npc_pre_name[20];
	TCHAR m_npc_mid_name[20];
	UInt8 m_npc_pos_name_type;
	TCHAR m_npc_name_csv[32];
	UInt8 m_npc_faction_name;
	UInt8 m_npc_faction;
	UInt8 m_npc_job;
	UInt8 m_npc_sex;
	UInt8 m_npc_direction;
	DWORD m_npc_appeartime;
	UInt8 m_npc_squad;
	UInt8 m_npc_mini_lackey;
	UInt8 m_npc_max_lackey;
	TCHAR m_npc_script[32];
	UInt32 m_npc_exp;
	UInt32 m_npc_money;
	TCHAR m_npc_dropitem[32];
	TCHAR m_npc_battlescript[32];

	BinaryServerNPCTemplate()
	{
		ZeroMemory(this, sizeof(BinaryServerNPCTemplate));
	}
};

struct BinaryClientNPCTemplate
{
	UInt32 m_npc_id;
	TCHAR m_npc_model[32];
	TCHAR m_npc_image[32];
	UInt8 m_npc_type;
	TCHAR m_npc_pronounce[32];
	TCHAR m_npc_color[32];
	TCHAR m_npc_pre_name[20];
	TCHAR m_npc_mid_name[20];
	TCHAR m_npc_dialoguectrl[32];
	
	BinaryClientNPCTemplate()
	{
		ZeroMemory(this, sizeof(BinaryClientNPCTemplate));
	}
};

struct BinaryServerNPCInstance
{
	UInt32	nNPCUserID;							
	TCHAR   npc_raname[QG_NAME_MAXLEN];			
	UInt16	npc_position_zoneid;				
	UInt16	npc_position_x;						
	UInt16	npc_position_y;						
	UInt8	npc_direction;						
	UInt32	npc_encount;

	BinaryServerNPCInstance()
	{
		nNPCUserID = 0;
		ZeroMemory(npc_raname, sizeof(npc_raname));
		npc_position_zoneid = 0;
		npc_position_x = 0;
		npc_position_y = 0;
		npc_direction = 0;
		npc_encount = 0;
	};
};

struct BinaryClientNPCInstance
{
	UInt32	nNPCUserID;
	TCHAR   npc_raname[QG_NAME_MAXLEN];
	UInt16	npc_position_x;
	UInt16	npc_position_y;
	UInt8	npc_direction;

	BinaryClientNPCInstance()
	{
		nNPCUserID = 0;
		ZeroMemory(npc_raname, sizeof(TCHAR) * QG_NAME_MAXLEN);
		npc_position_x = 0;
		npc_position_y = 0;
		npc_direction = 0;
	};
};

struct BinaryServerJumppointInstance
{
	UInt16 m_posx;
	UInt16 m_posy;
	UInt32 DesMapIdNum;
	UInt16 DesBlockX;
	UInt16 DesBlockY;

	BinaryServerJumppointInstance()
	{
		m_posx = 0;
		m_posy = 0;
		DesMapIdNum = 0;
		DesBlockX = 0;
		DesBlockY = 0;
	}
};

struct BinaryClientJumppointInstance
{
	UInt16 m_posx;
	UInt16 m_posy;
	TCHAR szName[32];
	TCHAR szFileName[32];

	BinaryClientJumppointInstance()
	{
		m_posx = 0;
		m_posy = 0;
		ZeroMemory(szName, sizeof(TCHAR) * 32);
		ZeroMemory(szFileName, sizeof(TCHAR) * 32);
	}
};

struct BinaryServerMOBInfo
{
	UInt32 mob_id;
	UInt8 mob_type;
	TCHAR pre_name[20];
	TCHAR mid_name[20];
	UInt16 pos_name;
	TCHAR name_csv[32];
	Boolean bFaction;
	UInt8 race;
	UInt8 cclass;
	UInt8 faction;
	UInt8 sex;
	Int32 goodness;
	Boolean boss;
	UInt8 elite_type;
	UInt16 ai_type;
	UInt8 element;
	UInt8 partner_type;
	UInt16 req_lv;
	UInt8 talent;
	UInt8 potential;
	UInt16 lv_d;
	UInt16 lv_u;
	UInt16 STA_base_d;
	UInt16 STA_base_u;
	UInt16 SPR_base_d;
	UInt16 SPR_base_u;
	UInt16 STR_base_d;
	UInt16 STR_base_u;
	UInt16 CON_base_d;
	UInt16 CON_base_u;
	UInt16 AGI_base_d;
	UInt16 AGI_base_u;
	float grow_d;
	float grow_u;
	UInt16 HP_native_d;
	UInt16 HP_native_u;
	UInt16 SP_native_d;
	UInt16 SP_native_u;
	UInt16 ATK_native_d;
	UInt16 ATK_native_u;
	UInt16 DEF_native_d;
	UInt16 DEF_native_u;
	UInt16 SPD_native_d;
	UInt16 SPD_native_u;
	UInt16 POW_native_d;
	UInt16 POW_native_u;
	UInt16 HIT_native_d;
	UInt16 HIT_native_u;
	UInt16 EVA_native_d;
	UInt16 EVA_native_u;
	UInt32 maxHealth;
	TCHAR model[32];
	UInt16 skill_file_id;
	UInt8 rare;
	UInt16 TimeLimit;
	UInt8 TimeLimitType;
	UInt8 soulbound;
	UInt8 unique;
	UInt16 outlookColor;

	BinaryServerMOBInfo()
	{
		ZeroMemory(this, sizeof(BinaryServerMOBInfo));
	};
};

struct BinaryClientMOBInfo
{
	UInt32 mob_id;
	TCHAR model[32];
	TCHAR color[32];
	UInt8 partner_type;
	UInt32 desc_id;
	TCHAR mob_faceL[32];
	TCHAR mob_faceS[32];
	TCHAR mob_sound_atk1[32];
	TCHAR mob_sound_atk2[32];
	TCHAR mob_sound_skill1[32];
	TCHAR mob_sound_skill2[32];
	TCHAR mob_sound_hurt[32];
	TCHAR mob_sound_die[32];
	TCHAR mob_dialogue[32];
	UInt8 mob_type;
	TCHAR pre_name[20];
	TCHAR mid_name[20];
	UInt16 pos_name;
	TCHAR name_csv[32];
	Boolean bFaction;
	UInt8 race;
	UInt8 cclass;
	UInt8 faction;
	UInt8 sex;
	Int32 goodness;
	Boolean boss;
	UInt8 elite_type;
	UInt16 ai_type;
	UInt8 element;
	UInt16 req_lv;
	UInt8 talent;
	UInt8 potential;
	UInt16 lv_d;
	UInt16 lv_u;
	UInt16 STA_base_d;
	UInt16 STA_base_u;
	UInt16 SPR_base_d;
	UInt16 SPR_base_u;
	UInt16 STR_base_d;
	UInt16 STR_base_u;
	UInt16 CON_base_d;
	UInt16 CON_base_u;
	UInt16 AGI_base_d;
	UInt16 AGI_base_u;
	float grow_d;
	float grow_u;
	UInt16 HP_native_d;
	UInt16 HP_native_u;
	UInt16 SP_native_d;
	UInt16 SP_native_u;
	UInt16 ATK_native_d;
	UInt16 ATK_native_u;
	UInt16 DEF_native_d;
	UInt16 DEF_native_u;
	UInt16 SPD_native_d;
	UInt16 SPD_native_u;
	UInt16 POW_native_d;
	UInt16 POW_native_u;
	UInt16 HIT_native_d;
	UInt16 HIT_native_u;
	UInt16 EVA_native_d;
	UInt16 EVA_native_u;
	UInt32 maxHealth;
	UInt16 skill_file_id;
	UInt8 rare;
	UInt16 TimeLimit;
	UInt8 TimeLimitType;
	UInt8 soulbound;
	UInt8 unique;

	BinaryClientMOBInfo()
	{
		ZeroMemory(this, sizeof(BinaryClientMOBInfo));
	};
};

struct BinaryServerOBJInstance
{
	TCHAR m_filename[32];
	UInt32 m_objid;
	UInt16 m_posx;
	UInt16 m_posy;

	BinaryServerOBJInstance()
	{
		ZeroMemory(m_filename, sizeof(TCHAR) * 32);
		m_objid = 0;
		m_posx = 0;
		m_posy = 0;
	}
};


struct BinaryClientOBJInstance
{
	TCHAR m_filename[32];
	UInt32 m_objid;
	UInt16 m_posx;
	UInt16 m_posy;

	BinaryClientOBJInstance()
	{
	}
};

struct AntiAddictInfo
{
	UInt32 onlineTime;
	UInt32 offlineTime;
	UInt32 disconnectTime;
	UInt32 loginTime;
	Boolean startHack;
	Boolean stopPlay;
	Boolean needAntiAddict;
	AntiAddictInfo()
	{
        onlineTime = 0;
		offlineTime = 0;
		disconnectTime = 0;
		loginTime = 0;
		startHack = BOOLEAN_FALSE;
	    stopPlay = BOOLEAN_FALSE;
		needAntiAddict = BOOLEAN_FALSE;
	}
};

struct AntiHackData
{
	Char chars[5];
	UInt8 answer;

	AntiHackData()
	{
		ZeroMemory(chars, sizeof(Char)* 5);
		answer = 255;	
	}
};


struct BinaryServerZoneInfo
{
	UInt16	zoneid;
	UInt32	zone_PK;
	UInt32	zone_Hawker;
	UInt32	zone_special;

	BinaryServerZoneInfo()
	{
		zoneid			= 0;
		zone_PK			= 0;
		zone_Hawker		= 0;
		zone_special	= 0;
	}
};

struct BinaryClientZoneInfo
{
	UInt16 m_zoneid;
	UInt8 m_zonetype; // zonetype: 0 = normal zone type; 1 = meet enemy zone type

	BinaryClientZoneInfo()
	{
		m_zoneid = 0;
		m_zonetype = 0;
	}
};

struct BinaryServerMeetEnemy
{
	UInt16 nZoneID;
	UInt32 nRefID;
	UInt32 nEncount;
};

/* End of COPY FROM Map Editor "common_binary.h" */

struct MeetEnemyData
{
	UInt NpcRate;
	UInt NpcId;

	MeetEnemyData()
	{
		NpcRate = 0;
		NpcId = 0;
	};
};

struct MeetEnemyZone
{
	UInt encountTotals[24];
	StlVector<MeetEnemyData*, ManagedAllocator<MeetEnemyData*> > encountList;

	MeetEnemyZone()
	{
		for (Int i = 0; i < 24; i++)
			encountTotals[i] = 0;
	};

	~MeetEnemyZone()
	{
		for( Index i = C_INDEX(0); i<encountList.size(); ++i )
		{
			if( encountList[i] )
				SafeDeleteObject(encountList[i]);
		}
		encountList.clear();
	};
};

struct ServerMapZoneData
{
	WORD sBlockType; // BlockType
	WORD MeetEnemyZoneId;
	UInt32 ZoneIdCount;
	WORD* ZoneIdLst;
	ServerMapZoneData() : 
		sBlockType(0), MeetEnemyZoneId(0), ZoneIdCount(0), ZoneIdLst(NULL) {}
	~ServerMapZoneData()
	{
		if( ZoneIdLst )
			SafeDeallocate( ZoneIdLst );
	}
};
	
struct ClientMapZoneData
{
	WORD sBlockType; // BlockType
	int ZoneIdCount;
	WORD* ZoneIdLst;

	ClientMapZoneData() { ZoneIdLst = NULL; };

	~ClientMapZoneData()
	{
		if( ZoneIdLst )
			SafeDeallocate( ZoneIdLst );
	}
};
	
struct NPCDB
{
	BinaryServerNPCTemplate* m_basicinfo;
	StlVector<int, ManagedAllocator<int> > m_mob_leader;
	StlVector<int, ManagedAllocator<int> > m_mob_lackey;
	StlVector<int, ManagedAllocator<int> > m_shopid;
	StlVector<int, ManagedAllocator<int> > m_quest;
	StlVector<int, ManagedAllocator<int> > m_item;

	NPCDB()
	{
		m_basicinfo = NULL;
		m_shopid.clear();
		m_quest.clear();
		m_mob_leader.clear();
		m_mob_lackey.clear();
		m_item.clear();
	}

	~NPCDB()
	{
		m_quest.clear();
		m_mob_leader.clear();
		m_mob_lackey.clear();
		m_shopid.clear();
		m_item.clear();
		SafeDeleteObject(m_basicinfo);
	}
};

struct NPCDB_Client
{
	BinaryClientNPCTemplate* m_basicinfo;

	NPCDB_Client()
	{
		m_basicinfo = NULL;
	}

	~NPCDB_Client()
	{
		SafeDeleteObject(m_basicinfo);
	}
};

struct MapDB
{
	DefaultInfoHeader*	m_header;
	BinaryServerMapInfo*	m_basicinfo;
	BinaryBlockInfoHeader*	m_zonedata_header;
	SrvZoneDataList	m_zonedata;
	SrvZoneInfoList	m_zoneinfo;
	NpcInstList		m_npc;
	ObjInstList		m_obj;
	JpInstList		m_jp;
	EnemyZoneMap	m_mezoneinfo;
	Boolean			m_bLoaded;
	WORD*			m_blockData;

	MapDB()
	{
		m_header = NULL;
		m_basicinfo = NULL;
		m_zonedata_header = NULL;
		m_zoneinfo.clear();
		m_zonedata.clear();
		m_npc.clear();
		m_obj.clear();
		m_jp.clear();
		m_mezoneinfo.clear();
		m_bLoaded = FALSE;
		m_blockData = NULL;
	}

	MeetEnemyZone* GetMEZone(UInt meZoneId) CONST
	{
		EnemyZoneMap::const_iterator itr = m_mezoneinfo.find(meZoneId);
		if (itr==m_mezoneinfo.end())
			return NULL;
		return itr->second;
	}

	~MapDB()
	{
		SafeDeleteObject(m_zonedata_header);
		SafeDeleteObject(m_header);
		SafeDeleteObject(m_basicinfo);
		SafeDeleteObject(m_blockData);
		for (SrvZoneDataList::iterator itr2 = m_zonedata.begin(); itr2!=m_zonedata.end(); itr2++)
		{
			SafeDeleteObject(*itr2);
		}
		for (SrvZoneInfoList::iterator itr2 = m_zoneinfo.begin(); itr2!=m_zoneinfo.end(); itr2++ )
		{
			SafeDeleteObject(itr2->second);
		}
		m_zoneinfo.clear();
		for (NpcInstList::iterator itr3 = m_npc.begin(); itr3!=m_npc.end(); itr3++ )
		{
			SafeDeleteObject(*itr3);
		}
		for (ObjInstList::iterator itr4 = m_obj.begin(); itr4!=m_obj.end(); itr4++ )
		{
			SafeDeleteObject(*itr4);
		}
		for (JpInstList::iterator itr5 = m_jp.begin(); itr5!=m_jp.end(); itr5++ )
		{
			SafeDeleteObject(*itr5);
		}
		EnemyZoneMap::iterator itr6;
		for( itr6 = m_mezoneinfo.begin(); itr6!=m_mezoneinfo.end(); ++itr6 )
		{
			if( itr6->second )
				SafeDeleteObject(itr6->second);
		}
	}
};

struct MapDB_Client
{
	DefaultInfoHeader* m_header;
	BinaryClientMapInfo* m_basicinfo;
	BinaryBlockInfoHeader* m_zonedata_header;
	StlMap<UInt16, BinaryClientZoneInfo*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BinaryClientZoneInfo*> > > m_zoneinfo;
	StlVector<ClientMapZoneData*, ManagedAllocator<ClientMapZoneData*> > m_zonedata;
	WORD* m_combinedblock; // BlockType

	MapDB_Client()
	{
		m_header = NULL;
		m_basicinfo = NULL;
		m_zonedata_header = NULL;
		m_zoneinfo.clear();
		m_zonedata.clear();
		m_combinedblock = NULL;
	}

	~MapDB_Client()
	{
		if( m_header )
			SafeDeleteObject(m_header);
		if( m_basicinfo )
			SafeDeleteObject(m_basicinfo);
		if( m_zonedata_header )
			SafeDeleteObject(m_zonedata_header);
		StlMap<UInt16, BinaryClientZoneInfo*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BinaryClientZoneInfo*> > >::iterator itr;
		for( itr = m_zoneinfo.begin(); itr != m_zoneinfo.end(); ++itr )
			SafeDeleteObject(itr->second);
		m_zoneinfo.clear();
		for( size_t i = 0; i < m_zonedata.size(); i++ )
			SafeDeleteObject(m_zonedata[i]);
		m_zonedata.clear();
		if( m_combinedblock )
			SafeDeallocate(m_combinedblock);
	}
};

struct MobDB
{
	BinaryServerMOBInfo* m_basicinfo;

	MobDB()
	{
		m_basicinfo = NULL;
	}

	~MobDB()
	{
		SafeDeleteObject(m_basicinfo);
	}
};

struct MobDB_Client
{
	BinaryClientMOBInfo* m_basicinfo;

	MobDB_Client()
	{
		m_basicinfo = NULL;
	}

	~MobDB_Client()
	{
		SafeDeleteObject(m_basicinfo);
	}
};

struct PathFindLoadData
{
	Real32* weightData;
	Real32* distData;
	UInt16* linkData;

	PathFindLoadData()
	{
		weightData	= NULL;
		distData	= NULL;
		linkData	= NULL;
	};

	~PathFindLoadData()
	{
		if(weightData)
			SafeDeallocate(weightData);
		if(distData)
			SafeDeallocate(distData);
		if(linkData)
			SafeDeallocate(linkData);
	}
};

struct NPCMobInfo
{
	UInt32						MobId;
	UInt32						Type;
	UInt32						Encount;
	NPCMobInfo()
	{
		MobId = 0;
		Type = 0;
		Encount = 0;
	};
};

struct EnterInfo
{
	UInt32 acct_id;
	RPCNetID charNetId;
	UInt32 char_id;
	UInt16 map_id;
	UInt8 line_id;
	UInt32 ownerId;
	UInt16 x;
	UInt16 y;
	UInt32 followId;
	UInt16 instId;
	bool	partnerCollection;

	EnterInfo() : acct_id(0), char_id(0), map_id(0), line_id(0), ownerId(0), 
				x(0), y(0), followId(0), instId(0), partnerCollection(false)
	{
	}
};

struct MapIndex
{
	UInt16 map_id;
	UInt8 line_id;
	UInt32 owner_id;

	MapIndex()
		: map_id(0)
		, line_id(0)
		, owner_id(0)
	{}

	MapIndex(UInt16 map_id, UInt8 line_id, UInt32 owner_id)
		: map_id(map_id)
		, line_id(line_id)
		, owner_id(owner_id)
	{}

	bool operator < (const MapIndex& b) const
	{
		return (map_id < b.map_id || (map_id == b.map_id && line_id < b.line_id) ||
			(map_id == b.map_id && line_id == b.line_id && owner_id < b.owner_id));
	}
};

struct NpcFlagIndex
{
	UInt32 npc_id;
	UInt16 line_id;
	UInt32 owner_id;
	UInt16 flag_id;

	NpcFlagIndex()
		: npc_id(0)
		, line_id(0)
		, owner_id(0)
		, flag_id(0)
	{}

	NpcFlagIndex(UInt32 npc_id, UInt16 line_id, UInt32 owner_id, UInt16 flag_id)
		: npc_id(npc_id)
		, line_id(line_id)
		, owner_id(owner_id)
		, flag_id(flag_id)
	{}

	bool operator < (const NpcFlagIndex& b) const
	{
		return (npc_id < b.npc_id || (npc_id == b.npc_id && line_id < b.line_id) ||
			(npc_id == b.npc_id && line_id == b.line_id && owner_id < b.owner_id)||
			(npc_id == b.npc_id && line_id == b.line_id && owner_id == b.owner_id && flag_id < b.flag_id));
	}
};

struct InstanceIndex
{
	UInt16 instId;
	UInt32 ownerId;

	InstanceIndex()
		: instId(0)
		, ownerId(0)
	{}

	InstanceIndex(UInt16 instId, UInt32 ownerId)
		: instId(instId)
		, ownerId(ownerId)
	{}

	bool operator < (const InstanceIndex& b) const
	{
		return (instId < b.instId || (instId == b.instId && ownerId < b.ownerId));
	}
};

struct BusinessShopItemInfo
{
	UInt16	count;
	UInt32	item_id;
	UInt32	buyPrice;
	UInt32	sellPrice;
	BusinessShopItemInfo()
	{
		ZeroMemory(this, sizeof(BusinessShopItemInfo));
	};
};
typedef StlVector<BusinessShopItemInfo, ManagedAllocator<BusinessShopItemInfo> > BusinessShopItemInfoVector;

struct PartyInfo
{
	TCHAR		nickName[32];
	UInt8		faction;
	UInt8		cclass;
	UInt16		level;
	UInt16		count;
	TCHAR		desc[32];
	RPCNetID	netID;
	UInt32		char_id;
	PartyInfo()
		:faction(0),
		cclass(0),
		level(0),
		count(0),
		netID(RPCNetID()),
		char_id(0)
	{
		ZeroMemory(nickName, sizeof(TCHAR) * 32);
		ZeroMemory(desc, sizeof(TCHAR) * 32);
	}
	PartyInfo(const String &NickName, UInt8 faction, UInt8 cclass, UInt16 level, UInt16 count,const String &Desc, RPCNetID netID, UInt32 char_id)
		:
		faction(faction),
		cclass(cclass),
		level(level),
		count(count),
		netID(netID),
		char_id(char_id)
		{
			ZeroMemory(nickName, sizeof(TCHAR) * 32);
			ZeroMemory(desc, sizeof(TCHAR) * 32);
			memcpy(nickName, NickName.c_str(), sizeof(TCHAR) * NickName.GetLength());
			memcpy(desc, Desc.c_str(), sizeof(TCHAR) * Desc.GetLength());
		}
};
typedef StlVector<PartyInfo, ManagedAllocator<PartyInfo> > PartyInfoVector;

struct CharInfo_new
{
	String		nickName;
	UInt8		faction;
	UInt8		cclass;
	UInt16		level;
	RPCNetID	netID;
	UInt32		char_id;
	CharInfo_new()
		: nickName(_T("")),
		faction(0),
		cclass(0),
		level(0),
		netID(RPCNetID()),
		char_id(0)
	{}
	CharInfo_new(String nickName, UInt8 faction, UInt8 cclass, UInt16 level, RPCNetID netID, UInt32 char_id)
		: nickName(nickName),
		faction(faction),
		cclass(cclass),
		level(level),
		netID(netID),
		char_id(char_id)
	{}
};
typedef StlVector<CharInfo_new, ManagedAllocator<CharInfo_new> > CharInfoVector;


#endif
