#pragma once
#ifndef _BATBUBBLECONTROL_H_
#define _BATBUBBLECONTROL_H_

struct lua_State;

enum BatBubbleMsgTypeEnum
{
	BATBUBBLE_IDLE = 0,
	BATBUBBLE_ATTACK,
	BATBUBBLE_HURT,
	BATBUBBLE_MISS,
	BATBUBBLE_DIE,
	BATBUBBLE_ESCAPE,
	BATBUBBLE_USESKILL,
	BATBUBBLE_CATCH,
	MAX_BATBUBBLE_COUNT,
};

typedef struct _BatBubbleMsg
{
	String	Msg;
	int		Percentage;
	_BatBubbleMsg()
	{
		Msg = _T("");
		Percentage = 0;
	};
}BatBubbleMsg;

class CBatBubbleControl
{
public:
	CBatBubbleControl();
	~CBatBubbleControl();
private:
	VOID InitBubbleLua();
	VOID ReleaseBubbleLua();
	VOID PrintLuaErr();
	Int32 UTF82StringW(StringW &Str, PCSTRINGA Text);
	Int32 StringW2UTF8(StringA &Des, PCSTRINGW Src);

	VOID Init();
public:
	VOID ResetParam();
	VOID ClearEnemyBubbleLst();
	VOID ClearUserBubbleLst();
	VOID ClearAllDialogueLst();

	// show battle pop up bubble
	VOID ShowUserBubble(Int posIndex, Int msgType);
	VOID ShowEnemyBubble(Int posIndex, Int msgType);
	
	// reset lua state 
	Boolean IsTimeToResetLua();
	VOID ResetBatBubbleLua(UInt32 npcId); // use for clear all object in lua state to prevent the state become too large

	// load, run lua file
	Boolean LoadDefaultBubbleFunc();
	Boolean LoadUserBubbleLua(UInt32 npcId);
	VOID RunUserBubbleLua();

	VOID StartUp(UInt32 npcId);
	
	// temp mod id list operation
	VOID AddMobId(UInt32 mobId);
	VOID ClearMobIdLst();

public:
	// Lua 
	void LuaAddBatBubble(Int pos, Int msgType, PCSTRINGA text);
	void LuaAddEnemyDialogue(UInt32 mobId, int msgType, PCSTRINGA text, int per);

	int LuaGetMobIdCount();
	UInt32 LuaGetMobId(int index);
	PCSTRINGA LuaGetMobDialgoueFile(int index);
private:
	lua_State *			m_pBubbleLua;

	typedef StlMap<UInt32, StringVector, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, StringVector> > > UserBubbleLst;
	typedef StlMap<UInt64, BatBubbleMsg, std::less<UInt64>, ManagedAllocator<std::pair<UInt64, BatBubbleMsg> > > EnemyBubbleLst;

	UserBubbleLst		m_DialogueLst;
	EnemyBubbleLst		m_EnemyDialogueLst;
	UInt32Vector		m_BatMobIdLst;	

	UInt32				m_ResetTime;

	StringA				m_szC2LuaBuff;

	Boolean				m_bLoadDefault;
	Boolean				m_bCanRun;
};

#endif // _BATBUBBLECONTROL_H_