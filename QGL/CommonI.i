/**************************************************************************
 *                          QGL Robot Lua API                             *
 *                                                                        *
 * Warning:                                                               *
 *  For programmer, please add comment for all methods                    *
 *  For lua script writter, DON'T modify or SVN commit this file          *
 *                                                                        *
 **************************************************************************/

%module CommonI
%{
// LxC: fix link error LNK2005: _SWIG_init_user already defined ...
// http://lua.2524044.n2.nabble.com/Using-multiple-modules-with-Swig-gt-Lua-td7585945.html
#define SWIG_init_user SWIG_init_user_CommonI

#include "Common.h"
#include "QGL/MapActorControl.h"
#include "QGL/MapNPCControl.h"
#include "QGL/CharacterControl.h"

#pragma warning(disable : 4244 4101 4102)

#define  DEF_LuaPush(_type_)								\
void LuaPush(lua_State *L, _type_ *ptr)						\
{															\
	SWIG_NewPointerObj(L, ptr, SWIGTYPE_p_##_type_, 0);		\
}

// push class
DEF_LuaPush(CMapActorControl)
DEF_LuaPush(CMapNPCControl)
DEF_LuaPush(CCharacterControl)

%}

//%include "typemaps.i"			// you must have this for the typemaps for ptrs

//
// basic type definition
//
typedef unsigned char	UInt8;
typedef signed char		Int8;
typedef unsigned short	UInt16;
typedef signed short	Int16;
typedef bool			Boolean;
typedef UInt16			ClassID;
typedef int				Int;

#define CharA char
#define CharW wchar_t
#define Char  CharW

#define PSTRINGA CharA * 
#define PSTRINGW CharW * 

#define PCSTRINGA const CharA * 
#define PCSTRINGW const CharW * 

#define PSTRING PSTRINGW 
#define PCSTRING PCSTRINGW 

#define UInt32 unsigned int
#define Int32  signed int
#define PUInt32 unsigned int *
#define PInt32  signed int *
#define Boolean bool

struct RPCNetID			// unique ID of net object
{
	ServiceID srvId;
	ObjectID objId;
};

////////////////////////////
// defined structure here //
////////////////////////////

// Base actor class in Map
class CMapActorControl
{
public:
	// get actor type
	Byte GetMapActorType();
	// get actor sex
	UInt8 GetSex();
	// get actor job
	UInt8 GetCclass();
	// get sex and job code
	UInt8 GetSexClass();
	// get actor faction
	UInt8 GetFaction();
	// get current map ID
	UInt16 GetMap_id();
	// get current position in map
	UInt16 GetCurPosX();
	UInt16 GetCurPosY();
	// get destination position in map
	UInt16 GetDstPosX();
	UInt16 GetDstPosY();
	// get current action and action time
	UInt16 GetAction();
	// get current moving speed
	UInt32 GetSpeed();
	// get current following actor ID
	UInt32 GetFollowId();
};

// NPC class in map, inherit CMapActor
class CMapNPCControl : public CMapActorControl
{
public:
	// get npc UID
	UInt32 GetNpc_uid();
	// get npc ID
	UInt32 GetNpc_id();
};

// Character class in map, inherit CMapActor
class CCharacterControl : public CMapActorControl
{
public:
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
	UInt32 GetNextWcExp();
	// get VP and maxVP
	UInt16 GetVP();
	UInt16 GetMaxVP();	
	// get goodness
	Int32 GetGoodness();
	// get honer
	UInt32 GetHonor();
	// get manor
	UInt32 GetManor();
	// get yuanbao, game money and bank money
	UInt32 GetYuanBao();
	UInt32 GetMoney();
	UInt32 GetBankMoney();
	// get maximum partner and pet count
	UInt8 GetMaxPartner();
	UInt8 GetMaxPet();
	// get PK value
	UInt16 GetPK_Value();
	// get PK flag
	Boolean GetEnablePK();
	// character has party?
	bool HasParty();
	// character is party leader or member?
	bool IsLeader();
	bool IsMember();
	// character battle information
	UInt8 GetIndex();
	UInt16 GetTeam();
	
	void Stop();
	void SendBattleCommand(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional);
	void CheatCode(const String &code);

	// get skill level
	int GetSkillLv(int index);
	// get skill count
	UInt16 GetSkillCount();
	// get skill by index
	CSkill* GetSkill(UInt16 index);
	// get skill by skill ID
	CSkill* GetSkillById(UInt16 skill_id);
	// check buff 
	bool IsBuffExist(int Buff_id);
	bool DetectBuff(UInt16 Buff_ID);
	
%extend {
	// move character to map position
	void Move(UInt16 x, UInt16 y)	{ self->SetEntityDesPos(x, y); }
}

};
