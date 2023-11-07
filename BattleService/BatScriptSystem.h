#pragma once
#ifndef _BATSCRIPTSYSTEM_H_
#define _BATSCRIPTSYSTEM_H_

struct lua_State;
class CBattle;
class CBatActor;
//class CBatMob;
//class CBatCharacter;

enum BatScriptEventEnum
{
	BSEVENT_NONE = 0,
	BSEVENT_SURRENDER,
	BSEVENT_ENDBATTLE,
	BSEVENT_SKIPTURN,
	BSEVENT_MAX,
};

struct SBatConfigure
{
	Boolean				m_bNoPunishment;
	UInt32				m_dwDisEnableCmd;

	SBatConfigure()
	{
		m_bNoPunishment = BOOLEAN_FALSE;
		m_dwDisEnableCmd = 0;
	};
};

struct SBatEventInfo
{
	Int					eventType;
	bool				bFinishPreTurn;
	SBatEventInfo()
	{
		eventType = -1;
		bFinishPreTurn = false;
	};
};

struct SBatChooseInfo
{
	StringVector		optLst;
	Int					imageType;
	UInt32				imageId;
	Boolean				bleft;
	Boolean				bDone;
	SBatChooseInfo()
	{
		optLst.clear();
		imageType = 0;
		imageId = 0;
		bleft = true;
		bDone = true;
	};
	~SBatChooseInfo()
	{
		optLst.clear();
	};
};


class CBatScriptSystem
{
public:
	CBatScriptSystem();

	~CBatScriptSystem();
private:
	VOID ReInitParam();
	VOID InitTurn();
	Boolean InitChildState();
	VOID ReleaseChildState();
	VOID PrintLuaErr();

	Int32 UTF82StringW(StringW &Str, PCSTRINGA Text);
	Int32 StringW2UTF8(StringA &Des, PCSTRINGW Src);

	Boolean LoadDefaultBatLuaFunc(CONST StringA& szfilename);
	Boolean LoadNpcLuaFunc(CONST StringA& szfilename);
	Boolean ExecuteLua(CONST StringA& szfilename);

	VOID ResetDialogImage();
	Int	ConvertUnit2Int(PCSTRINGA szUnit);

	Int Change2TeamIndex(PCSTRINGA szUnitID);
public:
	Boolean LoadBattleScript(UInt32 npcId);
	INLINE VOID SetOwner(CBattle * pbattle = NULL) { m_pOwner = pbattle; };
	INLINE Boolean IsScriptRunning() { return m_bScriptRunning; };

	VOID OnBatSelectHandle(Int Opt);
	VOID ShowChooseResult2Member(Int Opt);
	VOID PreTurnScriptEventHandle(Boolean bEndCmd = BOOLEAN_FALSE);
	Boolean EnforceEndBattleEventHandle(UInt8 nextTurn);
	Boolean MobCmdEventHandle(CBatActor * pActor);

	VOID DisEnableBatCmd();

	Boolean IsSkipTurn();
	Boolean NeedToAdjustEscapeRate(CBatActor * pActor);
	INLINE IsChooseTurn() { return m_bChooseTurn; };
	Boolean HasEvent(UInt8 turn);
	VOID SetSkipEvent(UInt8 turn);
	Boolean IsNoPunishment();
public:
	// Call Battle Script Lua Function
	VOID CallInitBatPart();
	VOID CallTurnEndPart();
	VOID CallMobCmdPart(CBatActor * pActor);
	VOID CallPlayerCmdPart(CBatActor * pActor);
	VOID CallPreTurnPart(CBatActor * pActor);
	VOID CallDefaultChooseAnsPart();
	VOID CallBatWinPart();
	VOID CallBatLosePart();
public:
	// Lua default battle script 
	void LuaSetBatNoPunish(bool bPunish = true);
	void LuaDisEnableBatCmd(UInt32 disEnbale, bool bUpdate = false);

	void LuaBatSay(PCSTRINGA szUnitID, PCSTRINGA Msg, PCSTRINGA szImage = NULL, bool bLeft = true, UInt32 NpcId = 0);
	void LuaBatSetChooseImage(PCSTRINGA szUnitID, UInt32 NpcId = 0, bool bLeft = true);
	bool LuaBatChoose(PCSTRINGA szQuestion, PCSTRINGA C1, PCSTRINGA C2 = NULL);
	void LuaBatEndCmdMsg();
	void LuaBatResumeBattleMenu();
	void LuaSetBatEvent(int nTurn, int eventType);

	void LuaBatCmd(CBatActor* pActor, int cmdType);

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
	
private:
	CBattle *						m_pOwner;

	lua_State *						m_pBattleLua;
	int								m_nChildRefKey;

	Boolean							m_bScriptRunning;
	int								m_iTotalSelection;	// reinit by turn

	Boolean							m_bImageLeft;
	UInt32							m_SelectNpcImage;
	Boolean							m_bModDialogImage;


	SBatConfigure					m_BatConfigure;

	StringA							m_szC2LuaBuff;

	typedef StlMap<Int, SBatEventInfo, std::less<Int>, ManagedAllocator<std::pair<Int, SBatEventInfo> > > BatScriptEvtLst;
	BatScriptEvtLst					m_BatScriptEventLst;

	SBatChooseInfo					m_TempChooseMsg; 

	Boolean							m_bChooseTurn;
};

#endif // _BATSCRIPTSYSTEM_H_