#pragma once
#ifndef _MENUQUESTLOG_H_
#define _MENUQUESTLOG_H_

#include "QGL.h"

#define MAX_MAIN_QUESTLOG	19
#define MAX_SUBDESC_ROW		19
#define MAX_QUEST			10

enum {
	QUESTLOGTYPE_UNKNOWN = 0,
	QUESTLOGTYPE_NOGETTING = 1,
	QUESTYPE_GETTING = 2,
};

class CItem;
class CQuestLog;
class CQuestType;
class CPartner;
class CPartnerGroup;
class UIChatPanel;

enum QUESTLOGDISPLAY_TYPE
{
	QUESTMENU_FOLDER = 0,
	QUESTMENU_ITEM,
};


struct CSVQuestCase
{
	UInt16 nTypeID;
	String strTypeName;
	UInt16 nQuestID;
	String strQuestName;
	UInt32 nBeginNpcID;
	UInt32 nBeginNpcX;
	UInt32 nBeginNpcY;
	UInt32 nBeginNpcMapID;
	String strDesc;
	String strTitle;
	String strFinishDesc;
	UInt16 nLevel;
	UInt16 nGetFlagQuestID;
	UInt16 nGetFlag;
	UInt16 nFinishFlagQuestID;
	UInt16 nFinishFlag;
	UInt16 nRequireQuestID;
	UInt16 nRequireFlag;
	StlVector<UInt32> Itemlst;
	StlVector<UInt32> ItemCountlst;
	UInt16 uiItemNum;

	CSVQuestCase()
	{
		uiItemNum = 0;
		nRequireFlag = 0;
		nRequireQuestID = 0;
		nTypeID = 0;
		nQuestID = 0;
		nBeginNpcID = 0;
		nBeginNpcX = 0;
		nBeginNpcY = 0;
		nBeginNpcMapID = 0;
		nLevel = 0;
		nGetFlagQuestID = 0;
		nGetFlag = 0;
		nFinishFlagQuestID = 0;
		nFinishFlag = 0;
	};
};

class CMenuQuestLogProcess
{
private:
		typedef StlMap<UInt32, String, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, String> > > QMRowChildLst;

	struct QMenuRow
	{
		String			folderName;
		bool			expended;
		QMRowChildLst	itemLst;

		QMenuRow()
		{
			folderName = _T("");
			expended = false;
			itemLst.clear();
		};
		~QMenuRow()
		{
			itemLst.clear();
		};
	};

	struct QMDisPlayRow
	{
		String title;
		Int	type;
		bool bExpended;
		UInt16 questId;
		UInt16 typeId;
		UInt32 nLineNum;
		QMDisPlayRow()
		{
			title = _T("");
			type = QUESTMENU_FOLDER;
			bExpended = false;
			questId = 0;
			typeId = 0;
			nLineNum = 0;
		};
	};

public:
	CMenuQuestLogProcess();
	virtual ~CMenuQuestLogProcess();

	UInt32 GetItemId(const UInt8 SlotIndex);

	void OnSelectQuestTrace(bool bSel);
	BOOL IsQuestCompleted(UInt16 quest_id);
	BOOL IsQuestCompleted(CQuestLog* ql);
private:
	VOID ReInit();
	CQuestLog* FindQuestLog(UInt16 questId) const;
	bool IsQuestFailed(UInt32 questId);	

	//select quest trace
	void LoadHistory();
	void SaveHistory();
	bool IsTraceSelect(UInt16 questId);
	bool IsShowQuestHelp() const{return m_bQuestHelp;}
	void SetCurQuestTrace();
	void SetQuestTrace(UInt16 questId, bool bSel);

	// gotask.csv
	BOOL LoadCSV(LPCTSTR szPath, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	UInt32 GetCompareValue(UInt32 typeInfo, CItem* item);
	UInt32 GetCompareValue(UInt32 typeInfo, CPartner* partner);

	// Quest Info Operation
	VOID Release();
	bool SetMenuLstInfo();

	bool IsShowQuest( CSVQuestCase* pQuestCase);
	bool SetSubDescInfo(CQuestLog* log);
	void SetSubDescRoundInfo(UInt32 typeId);
	VOID SelectRow(Int selIndex);
	VOID CreateDisplayLst();
	VOID ClearDisplayLstData();
	VOID ClearQuestMenuLstData();
	void RefreshQuestNote();

	VOID ClearNoteDisplayLstData();
	
	// Quest Menu UI Relative
	VOID ResetMainQuestLstUI();
	VOID ResetMainCheckBoxState();
	VOID SelectMainTypeHandle(bool bExtend, int nSel, UInt questIndex);
	VOID RefreshSubDescScrollV();
	VOID RefreshMainLstScrollV();
	void RefreshNoteScrollV();
	VOID PostfixTargetNum(String& szText, UInt32 curr, UInt32 max);
	VOID DisEnableQuestRemainTime();
	VOID HighLightSelectRow(Int row);

	VOID ReDrawAllMenuItem();
	VOID ReDrawSelMenuItem(Int displayIndex, const QMDisPlayRow* pRow, bool bRedraw = false);

	VOID DetectSelMenuItemStatus();
	Int CalcTargetItemCount(UInt32 itemId, UInt32 type01, UInt32 value01, UInt32 type02, UInt32 value02);
public:
	Boolean				m_bQuestHelp; // nofinishquest list

	void OnNoteToMenu(UInt16 nTypeID, UInt16 nQuestID);
	void OnUpdateNoteUI();
	bool IsNullQuestNote();
	void ClearNoFinishCsvData();
	void LoadCsv();	// init nofinishtask.csv
	VOID ResetQuestLstData();
	VOID CreateNoteDisplayLst();
	VOID AddNoteInfo(UInt16 typeId, UInt16 questId);
	BOOL DelNoteInfo(UInt16 typeId, UInt16 questId);
	BOOL IsNoteTimeCount() const;

	void OnSelectLogDType(UInt16 nType);
	VOID UpdateCurrDescByType(UInt32 mainTypeId);
	void UpdateQuestLogMenu();
	void UpdateSubDescUI(bool bClear = false, bool bHome = true);
	void UpdateTargetUI(UIChatPanel* pChat, UInt32 tid, UInt16 v1, UInt16 v2, UInt16 v3,
						UInt8 vtype, UInt16 num, UInt16 max, const String &tname, UInt8 otype);
	void UpdateMainQuestLstUI(int index, bool bClearDesc = false);   // if index: -1 , update the main quest list without condition
	bool NeedToUpdateQuestRemainTime();
	
	VOID RefreshQuestTimeLimit(bool bRedraw = true);
	VOID RefreshQuestMenu();


	VOID CalcQuestRemainTime();

	VOID RemoveQuestHandle(UInt16 questId);
	VOID CancelQuestHandle();
	VOID ModifyQuest(UInt32 questId, UInt32 typeId, CQuestLog* pLog, Boolean bUpdateIcon);
	VOID TestReConstructAllData(Boolean bClear=BOOLEAN_FALSE);
	VOID TestNoticeFail(UInt32 questId);

	VOID RefreshTargetItemCount();
	void CallMissionButtonEffect();
	VOID SelectQuest(UInt16 questId, UInt16 typeId);
	VOID SetCurQuest(UInt16 questId, UInt16 typeId);

	// Lua Call C Function
	void LuaMainLstUpRow();
	void LuaMainLstDownRow();
	void LuaMainLstScrollMove();

	void LuaUpdateSubDescScrollV();
	void LuaSubDescUpRow();
	void LuaSubDescDownRow();
	void LuaSubDescScrollMove();

	void LuaCancelQuestClient();
private:
	typedef StlMap<UInt32, QMenuRow*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, QMenuRow*> > > QMenuRowLst;
	typedef StlMap<UInt32, CSVQuestCase*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CSVQuestCase*> > > QCsvRowLst;
	typedef StlMap<UInt32, UInt32, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt32> > > QID2TypeID;
	typedef StlMap<UInt16, bool, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, bool> > > QTraceLst;

	typedef StlVector<QMDisPlayRow*, ManagedAllocator<QMDisPlayRow*> > QMDisPlayRowLst;

	String				m_OpenFileName;
	Int					m_nSelStartIndex;
	Int					m_nCurrSelIndex;
	UInt16				m_CurrSelQuestId;
	UInt16				m_CurrSelTypeId;
	UInt32				m_preUpdate;

	CQuestLog*			m_selQuest;
	CQuestType*			m_selType;

	QTraceLst			m_Qtracelst;
	QCsvRowLst			m_CsvRowLst;
	QMenuRowLst			m_MenuRowLst;
	QMDisPlayRowLst		m_DisplayRowLst;
	QMDisPlayRowLst		m_NoteDisplayRowLst;
	QID2TypeID			m_tempSwitch;

	Boolean				m_bSetMenuLstFlag;
	Boolean				m_bSetDisplayFlag;
	Boolean				m_bSetDescFlag;

	Boolean				m_bNoticeFail;

	UInt16				m_nLastType; // nofinishquest list
};

#endif 