#pragma once
#ifndef _SCRIPTCONTROL_H_
#define _SCRIPTCONTROL_H_

#include "Proxy/Logic/ScriptSystem.h"
#include "NumberEnt.h"

enum SScriptMsgType
{
	SCRIPTMSG_NONE = 0,
	SCRIPTMSG_SAY,
	SCRIPTMSG_CHOOSE,
	MAX_SCRIPTMSG_TYPE,
};

typedef struct _SScriptMsg
{
	StringVector		MsgLst;
	Int					Type;
	Int					ImageType; // -1: no setting, 0: npc image, 1: player image
	UInt32				askerId;
	bool				Left;
	bool				waitOther;
	UInt16				timeout;
	String				ImageName;
	_SScriptMsg()
	{
		MsgLst.clear();
		Type = SCRIPTMSG_NONE;
		ImageType = -1;
		askerId = 0;
		Left = true;
		waitOther = false;
		timeout = 0;
	};
	~_SScriptMsg()
	{
		MsgLst.clear();
		ImageName.Empty();
	};
}SScriptMsg;

class CScriptControl
{
private:
	CScriptSystem *					m_pScriptSys;

	UInt8							m_CurrPage;
	Boolean							m_bOpen;
	Boolean							m_bDoNextPage;
	Boolean							m_bWriteScriptMsg;
	Boolean							m_bLockPos;
	Boolean							m_bChoose;
	StlDeque<SScriptMsg>			m_ScriptMsgQueue;

	Boolean							m_bPartyChoose;
	Boolean							m_bLockAndCount;
	UInt32                          m_WaitTimeOut;
	UInt32							m_pretime;
	UInt16							m_uShowWaiting;

	Int								m_nFinishScript;
	//
	UInt16							m_DisappearCenterX;
	UInt16							m_DisappearCenterY;

	Boolean							m_bPrepareItemScript;
	Boolean							m_bWaitEnd;
	UInt32							m_PreUpdateTime;
	UInt32							m_CountDownTime;
	Boolean							m_bWaitOther;
	CNumEnt							m_CountDownEnt;

public:
	CScriptControl(void);
	~CScriptControl(void);
public:
	VOID SetScriptSysNetObject(CScriptSystem * pScriptSys);
	CScriptSystem *	 GetScriptSysNetObject();
public:
	VOID Init();
	VOID StartUp();
	// ReInitial
	VOID ResetPage();
	VOID ResetData();
	VOID ResetPartyChoose();
	VOID ResetMouseState();
	VOID ResetNPCDirection();
	VOID ResetCounter();
	Boolean IsOpen() { return m_bOpen; }
	void SetWaitEnd(Boolean bWait) { m_bWaitEnd = bWait; }
public:
	// Dialogue UI Relative
	INLINE UInt16 GetDisappearX() { return m_DisappearCenterX; };
	INLINE UInt16 GetDisappearY() { return m_DisappearCenterY; };
	VOID SetDisappearCenter(UInt16 x, UInt16 y);

	VOID OpenDialogue();
	VOID CloseDialogue();
	
	VOID ChangeNextPage();

	VOID SetDialogueImage(UInt32 templateid, Int imageType = 0, bool bLeft = true, String imageName = _T(""));
	
	VOID AddStrToChatUI(String str, UInt32 dwMaxRow = 16, Boolean bAppend = BOOLEAN_FALSE, Boolean bReDraw = BOOLEAN_FALSE);
	VOID AddOptionToSelectPopUpUI(const StringVector & StrLst, Boolean bReDraw = BOOLEAN_FALSE);

	VOID ReSizeChatControl(UInt32 dwMaxRow = 16, Boolean bReDraw = BOOLEAN_FALSE);
	VOID ReSizeSelectPopUp();
	VOID ReSizeBaseBGUI(Boolean bReDraw = BOOLEAN_FALSE);

	VOID SetDiaglogueDetectALL(Boolean bFlag = BOOLEAN_TRUE);

	VOID TerminateTalkingDialogue(Boolean bEnforceInterupt = BOOLEAN_FALSE);
public:
	VOID PushMsgIntoQueue(Int type, String Msg, UInt32 askerId = 0, 
						Int Image = -1, bool bLeft = true, String imageName = _T(""));
	VOID PushMsgIntoQueue(Int type, StringVector MsgLst, UInt32 askerId = 0, 
						Int Image = -1, bool waitOther=BOOLEAN_FALSE, UInt16 timeout=0);
	Boolean EnterNextPageHandle();
	VOID ExitNextPageHandle();
	VOID NextPageHandle();

	Boolean ShowOptionForChoose(UInt8 page, const StlDeque<SScriptMsg> CopyMsg);
	void ShowSelectedOption(UInt8 page, const StlDeque<SScriptMsg> CopyMsg, Int Opt);

	VOID StopTalkingBeforeBattle();

	VOID BattleDefaultDialogue();

	INLINE Int IsFinished() { return m_nFinishScript; };
	INLINE VOID SetFinish() { m_nFinishScript = 1 + (rand() % 1000); };
	INLINE Boolean IsItemScriptStandBy() { return m_bPrepareItemScript; }; 
	INLINE VOID CancelItemScriptStandBy() { m_bPrepareItemScript = BOOLEAN_FALSE; };
	
	INLINE Boolean IsLockedWhenTalking() 
	{
		if ( m_bPartyChoose )
			return BOOLEAN_TRUE;
		return m_bLockPos; 
	};

	VOID OnWaitForMember();
	Int CountDown();
	VOID ChooseTimeOutHandle();
	Boolean PrepareItemScriptHandle();
	INLINE Boolean NeedUpdate() { return (m_CountDownTime > 0); }
	void UpdateDialogue(UInt32 curTime);
	void SetCountDown(UInt32 timeout);

public:
	// Handle Script Event(Send)
	VOID PlayerChooseResult(Int Opt);
	VOID PartyChooseTimeOutHandle();
	VOID OnPlayerSelectPartyChooseHandle(Int Opt);
	VOID NpcGiven(UInt32Vector uidList, UInt8Vector typeList);
public:
	// Handle Script Event(Receive)
	VOID OnSayHandle(UInt32 askerId, const String & SayMsg, Int ImageType = -1, Boolean bLeft = BOOLEAN_FALSE, String imageName = _T(""));
	VOID OnPartySayHandle(UInt32 Npcid, const String & SayMsg, UInt32 InstanceId, Int ImageType = -1, Boolean bLeft = BOOLEAN_FALSE);
	VOID OnChooseHandle(UInt8 askerType, UInt32 askerId, Int Opt, Boolean waitOther, UInt16 timeout, const StringVector & OptMsg);
	VOID OnEndScriptHandle();

	VOID OnChooseFailed();

	VOID CB_OnExecuteAllHandle(UInt32 TemplateId, UInt32 InstanceId, UInt16 MapId, String filename);
	VOID OnLockPosHandle(Int lock = 0);

	VOID OnPartyChooseHandle(UInt32 Npcid, Int Opt, const StringVector & OptMsg, UInt32 time);
	VOID OnPartyChooseRelpy(UInt32 Result);

	VOID OnPartyChooseSuccessHandle();

	VOID CB_OnOpenInterfaceHandle(StringA szInterface, Int32 Param1);

	VOID OnOpenShopMenuHandle(Int32 ShopId, UInt8 Title, UInt8 Enable, UInt16 Buy, UInt16 Sell, UInt16 Repair, UInt16 uiShopType, UInt32Vector Lst);

	VOID OnShowSysHint(Int Type, UInt32 V1, UInt32 V2);
	VOID OnPopUpHint(String Hint);

	VOID OnPlaySound(StringA Name);
	Boolean IsOpenDialogue(){return m_bOpen;}
	VOID OnNpcGivenHandle(UInt32 npcID, UInt32Vector idList, UInt8Vector typeList, String npcMessage);
	VOID OnNpcGivenFinish();
	VOID OnAskString(String text, String defAns, bool left, UInt8 askerType, UInt32 askerId);
	VOID OnShowArr(UInt8 dir, UInt8 Arr, UInt16 dirX, UInt16 dirY);
	VOID OnShowGuiPic(String context, String img1_name="0", UInt16 img1_X=0, UInt16 img1_Y=0
					 ,String img2_name="0", UInt16 img2_X=0, UInt16 img2_Y=0
					 ,String img3_name="0", UInt16 img3_X=0, UInt16 img3_Y=0
					 ,String img4_name="0", UInt16 img4_X=0, UInt16 img4_Y=0
					 ,String img5_name="0", UInt16 img5_X=0, UInt16 img5_Y=0);
	VOID OnCloseGuide();
	VOID OnCloseArrow();
};

#endif // _SCRIPTCONTROL_H_