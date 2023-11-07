#pragma once
#ifndef _SCRIPTSYSTEM_H_
#define _SCRIPTSYSTEM_H_

#include "Stub/Logic/ScriptSystem_stub.h"

class CCharacter;
class CBattle;
class CBatActor;
class CItem;
class CMapNPC;
class CInstance;
struct lua_State;

class CScriptSystem : public ScriptSystem_Stub,
					public Reuben::Simulation::IEventHandler<CScriptSystem>
{
public:
	virtual RPCResult ReplyAnswer(LPCPEER pPeer, RPCContext &context, const UInt8 &answer);
	virtual RPCResult ReplyGiven(LPCPEER pPeer, RPCContext &context, const UInt8Vector &gtypeList, const UInt32Vector &uidList);
	virtual RPCResult ReplyAnswerString(LPCPEER pPeer, RPCContext &context, const String &answerStr);
	virtual RPCResult StopTalk(LPCPEER pPeer, RPCContext &context, const Boolean &bForce);

	DEFINE_CLASS(ScriptSystem_CLASSID);
	typedef ScriptSystem_Stub Parent;
	CScriptSystem();
	~CScriptSystem();
	INLINE static CScriptSystem* NewInstance();
	INLINE CCharacter* GetOwner() { return m_pOwner; }
	INLINE CBattle* GetBattle() { return m_pBattle; }
	INLINE void	SetOwner(CCharacter* pChar) { m_pOwner = pChar; }
	INLINE void	SetBattle(CBattle* pBattle) { m_pBattle = pBattle; }
	INLINE void	SetRunOnce(Boolean bOnce) { m_bRunOnce = bOnce; }
	void SetRunning(Boolean bRunning);
	void SetNpc(CMapNPC* pNpc);

	Boolean	IsRunning() { return m_bRunning; }
	Boolean	IsTalking() { return m_bTalking; }
	Boolean	IsChoosing() { return m_answerTotal > 0; }
	Boolean IsNpcScript() { return (m_scriptType == SCRIPT_TYPE_NPC); }
	Boolean IsItemScript() { return (m_scriptType == SCRIPT_TYPE_ITEM); }
	Boolean IsBattleScript() { return (m_scriptType == SCRIPT_TYPE_BATTLE); }
	Boolean IsInstScript() { return (m_scriptType == SCRIPT_TYPE_INST); }
	Boolean ExecuteNpcScript(CMapNPC* pNpc);
	Boolean ExecuteItemScript(CItem *pItem, NetObject* pUser, NetObject* pTarget,
		PCSTRINGA method);
	Boolean ExecuteBattleScript(UInt32 npcId, const RPCNetID &netId);
	Boolean ExecuteInstScript(PCSTRINGA script, PCSTRINGA method, CInstance* pInst,
								CCharacter* pChar=NULL);

	// only for battle
	Boolean HasBatMethod(PCSTRINGA method);
	void CallBatFunc(PCSTRINGA func);
	void CallBatMethod(PCSTRINGA method, int turn, CBatActor *actor=NULL, int result=0);
	Boolean CallItemMethod(PCSTRINGA method, NetObject* pUser, NetObject* pTarget);

	// only for instance
	Boolean HasInstMethod(PCSTRINGA method);
	Boolean CallInstMethod(PCSTRINGA method, CInstance* pInst, CCharacter* pChar=NULL);

	// Lua
	Boolean InitLua();					// init lua thread
	void SetLuaGlobal(Boolean bSet);	// set/unset lua thread global variable
	void ReleaseLua();					// release lua thread
	Boolean ExecuteLua(StringA &filename);
	void PrintErr();
	void LogMessage(int level, PCSTRINGA msg);

	CCharacter* FindCharacter(UInt32 char_id);

public:
	/*  Default Script Function */
	UInt8 GetAnswer(UInt8 index=0);
	UInt8 GetOwnAnswer();
	CItem* GetUseItem();
	void SetAnswer(UInt8 answer) { m_answer = answer; }
	void ReplyAnswer(UInt8 ansIndex);
	UInt32 GetNpcId() { return m_npcId; }
	const RPCNetID& GetNpcNetId() { return m_npcNetId; }
	void SetNpcNetId(const RPCNetID& netId) { m_npcNetId = netId; }
	void ExitScript(PCSTRINGA errEsg);
	void StopScript();
	//void Suicide(UInt32 npc_uid);
	void Say(PCSTRINGA msg, int imageType=0, bool bLeft=true, UInt32 npc_id=0, 
				Boolean bParty=BOOLEAN_FALSE);
	void Rumor(PCSTRINGA msg, bool bShowTitle);
	void ShowHint(PCSTRINGA msg);
	void ShowError(PCSTRINGA msg);
	bool GenerateNPC(UInt32 npc_id, UInt16 map_id, UInt16 x, UInt16 y, bool enableAll, 
					UInt32 lifeTime=10, UInt8 line_id=0, UInt32 owner_id=0,UInt8 dir = 0);
	bool OpenInterface(PCSTRINGA menu, Int32 param1);
	bool OpenShopMenu(UInt32 shop_id);
	void Rank(UInt8 rankType);
	void PlaySound(PCSTRINGA soundName);
	void SetNpcTable(PCSTRINGA tableName)		{ m_npcTable = tableName; } 
	void SetItemTable(PCSTRINGA tableName)		{ m_itemTable = tableName; } 
	void SetBatTable(PCSTRINGA tableName)		{ m_batTable = tableName; } 
	void SetInstTable(PCSTRINGA tableName)		{ m_instTable = tableName; } 
	Boolean IsProcess() { return m_bProcess; }
	Boolean IsAsking() { return m_bAsking; }
	Boolean IsSaying() { return m_bSaying; }
	void SetProcess(Boolean bProcess);
	void SetAsking(Boolean bAsk) { m_bAsking = bAsk; }
	void SetSaying(Boolean bSay) { m_bSaying = bSay; }
	void GetMapLines(UInt16 map_id);
	RPCResult GetMapLines(RPCContext &context);

	//relation
	void GetMessengers(UInt32 char_id,UInt16 relation);
	RPCResult GetMessengers(RPCContext &context);
	void GetMessengersName(UInt32 char_id,UInt16 relation);
	RPCResult GetMessengersName(RPCContext &context);
	void GetFriendly(UInt32 char_id1,UInt32 char_id2);
	RPCResult GetFriendly(RPCContext &context);
	void ChangeFriendly(UInt32 char_id1,UInt32 char_id2,Int32 changed);
	RPCResult ChangeFriendly(RPCContext &context);
	void IsFriend(UInt32 char_id1,UInt32 char_id2);
	RPCResult IsFriend(RPCContext &context);
	void CreatMasterAndStudent(UInt32 master_id,UInt32 student_id);
	void DestroyMasterAndStudent(UInt32 master_id,UInt32 student_id);
	void SendPrivateMessage(PCSTRINGA msg, UInt32 SourceId, UInt32 TargetID, PCSTRINGA TargetName, Boolean isFriend);

	//global npc flag
	void HasGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	RPCResult HasGlobalNpcFlag(RPCContext &context);
	void GetGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	RPCResult GetGlobalNpcFlag(RPCContext &context);
	void SetGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id, UInt16 flag);
	RPCResult SetGlobalNpcFlag(RPCContext &context);
	void RemoveGlobalNpcFlag(UInt32 npc_id,UInt32 owner_id,UInt16 flag_id);
	RPCResult RemoveGlobalNpcFlag(RPCContext &context);

	// question and answer methods
	void SetQuestion(PCSTRINGA text, bool bLeft=true, UInt8 askerType=0, 
					UInt32 askerId=0, Boolean bParty=BOOLEAN_FALSE);
	void ClearQuestion();
	void AddAnswer(UInt8 index, PCSTRINGA text);
	void WaitAnswer(UInt8 defAns=0, UInt16 timeout=0);
	void NextAnswer(Boolean bParty=BOOLEAN_FALSE);
	void BackAnswer(UInt8 level=1);
	UInt8 ReverseAnswer(UInt8 ansIndex);
	void ClearAnswer();
	Boolean IsNewScript();
	void SetJump(UInt16 map_id, UInt16 x, UInt16 y, UInt8 line_id, UInt32 ownerId, UInt16 instId,Boolean bCheckLua = true);
	void SetNoJump();
	void CheckJump();
	Boolean NeedJump();
	void TalkEnd(Boolean bWait);
	void TalkNext();
	void NotifyAnswer();
	struct QuestInfo
	{
		String	text;
		UInt8	askerType;
		UInt32	askerId;
		UInt8	defAns;
		Boolean	bLeft;
		Boolean	bParty;
	};
	void MemberWaitAnswer(QuestInfo& question, StringVector &answerTexts, 
							UInt8 defAns, UInt16 timeout);
	void CancelWait();
	void AskString(PCSTRINGA text, PCSTRINGA defAns, bool bLeft=true, UInt8 askerType=0,
					UInt32 askerId=0);
	PCSTRINGA GetAnswerString();

	// given methods
	Boolean AskGiven(PCSTRINGA text, UInt32 npc_id=0);
	void AskGivenObj(UInt8 gtype, UInt32 gid);
	UInt8 GetGivenType(UInt16 i);
	UInt32 GetGivenID(UInt16 i);
	UInt32 GetGivenUID(UInt16 i);
	UInt16 GetGivenCount();
	void ClearGiven();
	void RemoveAllGiven();

	// event handle
	virtual void HandleEvent(CONST Event &event);
	//read pvp battle script
	Boolean ExecutePvPBattleScript(UInt32 pvpBattleID);
private:
	struct GivenType;
	// resume script (use this instead of lua_resume for global variables safety)
	bool Resume(bool bReturn=true, bool start=false);
	bool ScriptReturn(int r, CONST String& errMsg);		// handle script return
	Boolean CanGiveObject(GivenType* pGiven);
	Boolean HaveGivenObject(GivenType* pGiven);

	template<typename T>
	void PushLocalInt(T intValue, int varIndex);
	void PushLocalString(const String &str, int varIndex);
	template<typename T>
	void PushIntArray(const StlVector<T, ManagedAllocator<T> > &intList, int varIndex);
	void PushStringArray(const StringVector &strList, int varIndex);

	enum EventEnum
	{
		EVT_ANS_TIMEOUT		= MAKE_EVENTID(ScriptSystem_CLASSID,  1),
		EVT_TALK_TIMEOUT	= MAKE_EVENTID(ScriptSystem_CLASSID,  2),
	};

	enum ScriptType
	{
		SCRIPT_TYPE_NONE	= 0,
		SCRIPT_TYPE_NPC,
		SCRIPT_TYPE_ITEM,
		SCRIPT_TYPE_BATTLE,
		SCRIPT_TYPE_INST,
	};

	enum GivenEnum
	{
		GIVEN_ITEM			= 1,
		GIVEN_PARTNER		= 2,
		GIVEN_PET			= 3,
	};

	struct JumpPos
	{
		UInt16	map_id;
		UInt16	x;
		UInt16  y;
		UInt8	line_id;
		UInt32	owner_id;
		UInt16	instId;
		Boolean bCheckLua;
		JumpPos() : map_id(0), x(0), y(0), line_id(0), owner_id(0), instId(0),bCheckLua(true) {}
	};

	struct GivenType
	{
		UInt8	gtype;
		UInt32	gid;
		UInt32	uid;
		GivenType() : gtype(0), gid(0), uid(0) {}
		GivenType(UInt8 gtype, UInt32 gid) : gtype(gtype), gid(gid), uid(0) {}
	};

	typedef StlVector<GivenType, ManagedAllocator<GivenType> > GivenList;
	typedef StlMap<UInt8, String, std::less<UInt8>, 
		ManagedAllocator<std::pair<UInt8, String> > > AnswerMap;

	lua_State*		m_pLuaState;		// lua thread state
	int				m_refKey;			// lua thread reference key
	UInt8			m_scriptType;		// script type
	CCharacter*		m_pOwner;			// script owner
	RPCNetID		m_npcNetId;			// talking npc netID
	CBattle*		m_pBattle;			// for battle
	NetObject*		m_pUser;			// user object for item use
	NetObject*		m_pTarget;			// target object for item use
	RPCNetID		m_itemNetId;		// item use netID
	Boolean			m_bTalking;			// talking to npc?
	Boolean			m_bAsking;			// show npc asking question?
	Boolean			m_bSaying;			// show npc saying text?
	Boolean			m_bGiving;			// giving npc object?
	Boolean			m_bRunning;
	Boolean			m_bRunOnce;			// script only run once?
	StringA			m_scriptFile;		// running script file
	UInt32			m_npcId;			// engage npc id
	UInt8			m_answer;
	UInt8			m_answerTotal;		// no. of answers
	UInt8			m_answerLevel;		// current level of answer
	UInt8Vector		m_answerList;		// processing answer list
	UInt8Vector		m_answerIndices;	// script set answer indices
	StringVector	m_answerTexts;		// script set answer text
	String			m_answerString;		// script answer string
	QuestInfo		m_question;			// question information
	GivenList		m_givenList;		// returned given object list
	Boolean			m_bProcess;			// is in process status?
	StringA			m_npcTable;			// running lua npc table name
	StringA			m_itemTable;		// running lua item table name
	StringA			m_batTable;			// running lua battle table name
	StringA			m_instTable;		// running lua inst table name
	JumpPos			m_jumpPos;			// jump map position
	UInt16			m_instId;			// instance id
	UInt32			m_ownerId;			// instance owner id
};

INLINE CScriptSystem* CScriptSystem::NewInstance()
{
	return CreateNetObject<CScriptSystem>();
}

INLINE void CScriptSystem::SetJump(UInt16 map_id, UInt16 x, UInt16 y, UInt8 line_id, 
								   UInt32 ownerId, UInt16 instId,Boolean bCheckLua)
{
	m_jumpPos.map_id = map_id;
	m_jumpPos.x = x;
	m_jumpPos.y = y;
	m_jumpPos.line_id = line_id;
	m_jumpPos.owner_id = ownerId;
	m_jumpPos.instId = instId;
	m_jumpPos.bCheckLua = bCheckLua;
}

INLINE void CScriptSystem::SetNoJump()
{
	m_jumpPos.map_id = 0;
}

INLINE Boolean CScriptSystem::NeedJump()
{
	return (m_pOwner && m_jumpPos.map_id != 0);
}

#endif //_SCRIPTSYSTEM_H_
