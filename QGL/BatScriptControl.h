#pragma once
#ifndef _BATSCRIPTCONTROL_H_
#define _BATSCRIPTCONTROL_H_

enum SBatScriptMsgType
{
	BATSCRIPTMSG_NONE = 0,
	BATSCRIPTMSG_SAY,
	BATSCRIPTMSG_CHOOSE,
	BATSCRIPTMSG_ENDCMD,
	MAX_BATSCRIPTMSG_TYPE,
};

typedef struct _SBatScriptMsg
{
	StringVector		MsgLst;
	Int					Type;
	UInt32				askerId;
	Int					ImageType; // -1: no setting, 0: npc image, 1: player or party1, 2: party2, ...etc
	bool				Left;
	bool				FinalSay;
	_SBatScriptMsg()
	{
		MsgLst.clear();
		Type = BATSCRIPTMSG_NONE;
		askerId = 0;
		ImageType = -1;
		Left = true;
		FinalSay = false;
	};
	~_SBatScriptMsg()
	{
		MsgLst.clear();
	};
}SBatScriptMsg;

class CBatScriptControl
{
public:
	CBatScriptControl();
	~CBatScriptControl();
private:
	// ReInitial
	VOID ResetPage();

	// Msg Control
	VOID PushMsg(Int type, String Msg, UInt32 askerId = 0, Int Image = -1, bool bLeft = true, bool finalSay = false);
	VOID PushMsg(Int type, StringVector MsgLst, UInt32 askerId = 0, Int Image=-1);
	Boolean EnterNextPageHandle();
	VOID ExitNextPageHandle();
	VOID ChangeNextPage();

	// Battle Dialogue UI Control
	VOID SetBatDialogueImage(UInt32 templateid, Int imageType = 0, bool bLeft = true);

	VOID AddStrToChatUI(String str, UInt32 dwMaxRow = 16, Boolean bAppend = BOOLEAN_FALSE, Boolean bReDraw = BOOLEAN_FALSE);
	VOID AddOptionToSelectPopUpUI(const StringVector & StrLst, Boolean bReDraw = BOOLEAN_FALSE);

	VOID ReSizeChatControl(UInt32 dwMaxRow = 16, Boolean bReDraw = BOOLEAN_FALSE);
	VOID ReSizeSelectPopUp();
	VOID ReSizeBaseBGUI(Boolean bReDraw = BOOLEAN_FALSE);

	Boolean ShowOptionForChoose(UInt8 page, const StlDeque<SBatScriptMsg> & CopyMsg);

	VOID SetDiaglogueDetectALL(Boolean bFlag = BOOLEAN_TRUE);
public:
	VOID Init();

	VOID ResetData();
	// battle script operation
	Boolean DoAndGate(UInt32 index);
	Boolean DisEnableCmd();
	Boolean NeedToBlockCmd(Int cmd);
	// Battle Dialogue Operation
	VOID OpenBatDialog();
	VOID CloseBatDialog();

	VOID NextPageHandle();
	VOID DisappearDialogHandle();
	
	// Receive Message From Server Handle
	VOID OnBatSayHandle(UInt32 askerId, const String & SayMsg, Int ImageType = -1, Boolean bLeft = BOOLEAN_FALSE, Boolean finalSay = BOOLEAN_FALSE);
	VOID OnBatChooseHandle(UInt8 askerType, UInt32 askerId, Int Opt, Boolean bLeft, const StringVector &OptMsg);
	VOID OnBatPushEndCmdMsgHandle();
	VOID OnBatSetDisEnableCmd(UInt32 disEnableCmd);
	
	// Send Message To Server Handle
	VOID OnBatPlayerChooseResult(Int Opt);
private:
	UInt8							m_CurrPage;
	Boolean							m_bOpen;
	Boolean							m_bDoNextPage;
	Boolean							m_bWriteFlag;
	StlDeque<SBatScriptMsg>			m_BatScriptMsgQueue;

	UInt32							m_dwDisEnableCmd;
};

#endif // _BATSCRIPTCONTROL_H_