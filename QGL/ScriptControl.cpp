//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "ScriptControl.h"
//-- Library
#include "Global.h"
#include "QGLApplication.h"
#include "Configure.h"
#include "CharacterControl.h"
#include "MapControl.h"
//#include "Render.h"
#include "Common/CallLuaFunc.h"
#include "Proxy/Logic/Party.h"

#include "MenuShopProcess.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResItem.h"
#include "MenuSkillProcess.h"
#include "MenuMixProcess.h"
#include "MenuHawkerProcess.h"
#include "MenuGivenProcess.h"
#include "MenuSystemSetting.h"
#include "MenuBankService.h"
#include "MapNPCControl.h"
#include "MenuPartnerCultivationUpProcess.h"

CScriptControl::CScriptControl(void)
{
	m_pScriptSys = NULL;
	m_bDoNextPage = BOOLEAN_FALSE;
	m_bWriteScriptMsg = BOOLEAN_FALSE;
	m_bLockPos = BOOLEAN_FALSE;
	ResetPage();

	ResetPartyChoose();

	m_nFinishScript = 1;
	//
	m_DisappearCenterX = 0;
	m_DisappearCenterY = 0;
	m_bWaitEnd = BOOLEAN_FALSE;
	m_bChoose = BOOLEAN_FALSE;
	m_bPrepareItemScript = BOOLEAN_FALSE;
	m_PreUpdateTime = 0;
	m_CountDownTime = 0;

	CharW FileName[50];
	PixelPos Center(0, 0);
	GetPrivateProfileString(_T("NUMBERSYSTEM"), _T("COUNTNUM"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	m_CountDownEnt.SetFileNameW(FileName);
	for (Int i = 0; i < (Int)m_CountDownEnt.m_NumEntLst.size(); i ++)
	{
		m_CountDownEnt.m_NumEntLst[i]->LockResources();
	}
	m_CountDownEnt.SetExtraZValue(0x20000000);
	m_CountDownEnt.SetEntPixelPos(Center);
}

CScriptControl::~CScriptControl(void)
{
	if ( m_pScriptSys )
	{
		DeleteNetObject(m_pScriptSys);
		m_pScriptSys = NULL;
	}

	m_ScriptMsgQueue.clear();
}

VOID CScriptControl::Init()
{
	ResetPage();

	m_bDoNextPage = BOOLEAN_FALSE;
	m_bWriteScriptMsg = BOOLEAN_FALSE;

	m_bLockPos = BOOLEAN_FALSE;

	ResetPartyChoose();

	m_nFinishScript = 1;
}

VOID CScriptControl::StartUp()
{ 
	m_nFinishScript = 0; 

	ResetPage();

	ResetPartyChoose();
}

VOID CScriptControl::ResetPage()
{
	m_CurrPage = 0;
	m_bOpen = BOOLEAN_FALSE;
	m_bWaitEnd = BOOLEAN_FALSE;
	m_ScriptMsgQueue.clear();
	m_CountDownTime = 0;
	m_PreUpdateTime = 0;
	m_bWaitOther = BOOLEAN_FALSE;
}

VOID CScriptControl::ResetData()
{
	CloseDialogue();
	//m_nFinishScript = 1;
	
	// - reset npc direction, cancel selected npc target
	ResetNPCDirection();
}

VOID CScriptControl::ResetPartyChoose()
{
	m_bPartyChoose = BOOLEAN_FALSE;
	m_bLockAndCount = BOOLEAN_FALSE;
	m_WaitTimeOut = 0;
	m_pretime = 0;
	m_uShowWaiting = 0;
}

VOID CScriptControl::ResetMouseState()
{
	// - resume the state of mouse
	if (gGlobal.g_CursorState && gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TALKING )
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();
		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

VOID CScriptControl::ResetNPCDirection()
{
	CMapActorControl* pMapCtrl = QGLApplication::GetInstance().GetTalkingMapControl();
	if( pMapCtrl )
	{
		CMapNPCControl * pMapNPCCtrl = dynamic_cast<CMapNPCControl *>(pMapCtrl);
		if( pMapNPCCtrl )
			pMapNPCCtrl->ChangeRotation(pMapNPCCtrl->GetInitRotation());
		QGLApplication::GetInstance().SetTalkingMapControl(NULL);
	}
}

VOID CScriptControl::ResetCounter()
{
	m_CountDownTime = 0;
	m_PreUpdateTime = 0;
	m_CountDownEnt.SetVisible(FALSE);
}

VOID CScriptControl::SetScriptSysNetObject(CScriptSystem * pScriptSys)
{
	if ( !pScriptSys ) return;

	m_pScriptSys = pScriptSys;
	m_pScriptSys->SetScriptControl(this);
}

CScriptSystem* CScriptControl::GetScriptSysNetObject()
{
	return m_pScriptSys;
}

VOID CScriptControl::SetDialogueImage(UInt32 templateid, Int imageType, bool bLeft, String imageName)
{
	//VMProtectBegin; VM_START;

	UIImage * NPCImage = gGlobal.pUIManager->FindUIImage("NPCPic");
	if ( !NPCImage ) return;

	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("DialogueImagePanel");
	if ( panel )
	{
		if ( bLeft )
			panel->SetPos(210, panel->GetY());
		else
			panel->SetPos(400, panel->GetY());
	}

	if (!imageName.IsEmpty())
	{
		String str = _T("Data\\NPCFACE\\");
		str += imageName.c_str();

		NPCImage->SetImageFile(str.c_str(), 100, 100);

		if (!(NPCImage->IsEnable()))
			NPCImage->SetEnable(true);
	}
	else
	{
		if (imageType == 0)
		{	// npc image by npc_id
			NPCDB_Client* npctemp = gGlobal.LoadNpcTemplate(templateid);
			if ( npctemp )
			{
				String str = _T("Data\\NPCFACE\\");
				str += npctemp->m_basicinfo->m_npc_image;

				NPCImage->SetImageFile(str.c_str(), 100, 100);

				if (!(NPCImage->IsEnable()))
					NPCImage->SetEnable(true);
			}
			else
			{
				if (NPCImage->IsEnable())
					NPCImage->SetEnable(false);
				/*
				Char FileName[50];
				GetPrivateProfileString(_T("LOADFILE"), _T("NPCIMAGE"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
				NPCImage->SetImageFile(FileName, 100, 100);
				*/
			}	
		}
		else if (imageType == 1)
		{	// character image by char_id
			CCharacterControl* pChar = NULL;
			if (templateid != 0)
				pChar = QGLApplication::GetInstance().FindCharCtrlByChar_id(templateid);
			if (pChar == NULL)
				pChar = gGlobal.g_pCharacter;

			UInt8 job = pChar->GetJobType();
			String imageName;
			imageName.Format(_T("Data\\NPCFACE\\imgM0000%d%c.tga"), (job % 10), 
				'a' + (job / 10));
			NPCImage->SetImageFile(imageName.c_str(), 100, 100);

			if (!(NPCImage->IsEnable()))
				NPCImage->SetEnable(true);
		}
		else
		{// no image 
			if (NPCImage->IsEnable())
				NPCImage->SetEnable(false);
		}
	}

	//VMProtectEnd; VM_END;
}

VOID CScriptControl::PushMsgIntoQueue(Int type, String Msg, UInt32 askerId, Int Image, 
									  bool bLeft, String imageName)
{
	if ( (Int)m_ScriptMsgQueue.size() > 100 )
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _T("CScriptControl PushMsgIntoQueue: too many script msgs since the player does not change the next page!!"));
		return;
	}

	if ( !m_bWriteScriptMsg )
	{
		m_bWriteScriptMsg = BOOLEAN_TRUE;

		SScriptMsg temp;

		temp.MsgLst.clear();
		temp.MsgLst.push_back(Msg);

		temp.Type = type;
		temp.askerId = askerId;
		temp.ImageType = Image;
		temp.Left = bLeft;

		m_ScriptMsgQueue.push_back(temp);

		m_bWriteScriptMsg = BOOLEAN_FALSE;
	}
}

VOID CScriptControl::PushMsgIntoQueue(Int type, StringVector MsgLst, UInt32 askerId, 
									  Int Image, bool waitOther, UInt16 timeout)
{
	if ( (Int)m_ScriptMsgQueue.size() > 100 )
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _T("CScriptControl PushMsgIntoQueue2: too many script msgs since the player does not change the next page!!"));
		return;
	}

	if ( !m_bWriteScriptMsg )
	{
		m_bWriteScriptMsg = BOOLEAN_TRUE;

		SScriptMsg temp;
		temp.MsgLst = MsgLst;
		temp.Type = type;
		temp.askerId = askerId;
		temp.ImageType = Image;
		temp.waitOther = waitOther;
		temp.timeout = timeout;

		m_ScriptMsgQueue.push_back(temp);

		m_bWriteScriptMsg = BOOLEAN_FALSE;
	}
}

VOID CScriptControl::AddStrToChatUI(String str, UInt32 dwMaxRow, Boolean bAppend, Boolean bReDraw)
{
	UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("DialogueChat");
	if ( pChat )
	{
		if ( !bAppend )
			pChat->ClearAllRow();

		StringW szbuff = str;
		pChat->AddStringW(szbuff.c_str());

		ReSizeChatControl(dwMaxRow);
	}

	if ( bReDraw )
		gGlobal.pUIManager->RenderAll();
}

VOID CScriptControl::AddOptionToSelectPopUpUI(const StringVector & StrLst, Boolean bReDraw)
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("DialogueChoosePopup");
	if ( pPopUpMenu )
	{
		StringVector szBuff = StrLst;
		Int size = (Int)szBuff.size();
		if ( size > 0 )
		{
			pPopUpMenu->ClearData();

			for ( Int i = 0; i < size; ++i )
				pPopUpMenu->AddString(szBuff[i].c_str(), i, "");

			ReSizeSelectPopUp();
		}
		szBuff.clear();
	}
}

VOID CScriptControl::ReSizeChatControl(UInt32 dwMaxRow, Boolean bReDraw)
{
	UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("DialogueChat");
	if ( pChat )
	{
		UInt32 uRow = pChat->RowListCount();
		if ( uRow > dwMaxRow )
			uRow = dwMaxRow;

		if ( uRow > 0 )
		{
			pChat->SetEnable(true);

			//if ( uRow < 5 )
			//	uRow = 5;
			pChat->SetImage(pChat->GetWidth(), uRow * 19 );
			pChat->SetDetectRect();

			pChat->HomeRow();
			pChat->Redraw();
		}
		else
		{// Invisiable if no row
			pChat->SetEnable(false);
		}
	}

	if ( bReDraw )
		gGlobal.pUIManager->RenderAll();
}

VOID CScriptControl::ReSizeSelectPopUp()
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("DialogueChoosePopup");
	if ( pPopUpMenu )
	{
		pPopUpMenu->SetEnable(true);

		UInt32 uCurrRow = 0;
		UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("DialogueChat");
		if ( pChat && pChat->IsEnable() )
		{
			uCurrRow = pChat->RowListCount();
			if ( (uCurrRow + pPopUpMenu->GetStringListSize()) > 16 )
				uCurrRow = 16 - pPopUpMenu->GetStringListSize();
		}

		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
		if ( pPanelEx )
		{
			pPopUpMenu->SetOffsetXY(14, 15 + uCurrRow * 19 );
			//pPopUpMenu->SetOffsetPos(14, 15 + uCurrRow * 18 );
		}

		//pPopUpMenu->SetPos(pPanelEx->GetX() + 14, pPanelEx->GetY() + 15 + uCurrRow * 18 );

		UInt32 OptCount = pPopUpMenu->GetStringListSize();
		if ( OptCount )
		{
			//pPopUpMenu->SetBarWidth(400);
			//pPopUpMenu->SetBarWidth(350);
			pPopUpMenu->SetImageWH(pPopUpMenu->GetWidth(), OptCount * (19 + 7) );
			//pPopUpMenu->SetImage(OptCount, 24);
			pPopUpMenu->SetDetectRect();
			pPopUpMenu->Redraw();
		}
		else
			pPopUpMenu->SetEnable(false);
	}
}

VOID CScriptControl::ReSizeBaseBGUI(Boolean bReDraw)
{
	UInt32 ChatRow = 0, PopUpRow = 0;
	UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("DialogueChat");
	if ( pChat && pChat->IsEnable() )
	{
		if ( pChat->RowListCount() > 0 )
			ChatRow = pChat->RowListCount();
	}

	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("DialogueChoosePopup");
	if ( pPopUpMenu && pPopUpMenu->IsEnable() )
	{
		if ( pPopUpMenu->GetStringListSize() > 0 )
			PopUpRow = pPopUpMenu->GetStringListSize();
	}

	UInt32 uTotal = 0;
	if ( ChatRow )
	{
		if ( PopUpRow )
		{// chat and popup
			if ( (ChatRow + PopUpRow) > 16 )
				uTotal = 16;
			else
				uTotal = ChatRow + PopUpRow;
		}
		else
		{// only chat
			uTotal = ( ChatRow > 16 )?16:ChatRow;
		}
	}
	else
	{
		if ( PopUpRow )
		{// only popup
			uTotal = PopUpRow;
		}
	}

	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
	if ( pPanelEx )
	{
		if ( uTotal < 5 )
			uTotal = 5;
		pPanelEx->ChangeCount(uTotal);
		pPanelEx->SetDetectRect();
	}

	if ( bReDraw )
		gGlobal.pUIManager->RenderAll();
}

VOID CScriptControl::OnSayHandle(UInt32 askerId, const String & SayMsg, Int ImageType, Boolean bLeft, String imageName)
{
	//clear msg queue if the dialogue is not opened
	if ( !m_bOpen )
	{
		m_ScriptMsgQueue.clear();
		m_CurrPage = 0;
	}

	// -push msg into queue
	if ( ImageType > -1 && ImageType < 46 )
		PushMsgIntoQueue(SCRIPTMSG_SAY, SayMsg, askerId, ImageType, bLeft, imageName);	
	else
		PushMsgIntoQueue(SCRIPTMSG_SAY, SayMsg, askerId);
	
	if ( !m_bOpen && (Int)m_ScriptMsgQueue.size() > 0 )
	{
		OpenDialogue();

		UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("DialogueChat");
		if ( !pChat )
		{
			ResetPage();
			TRACE_INFODTL(GLOBAL_LOGGER, _T("CScriptControl OnSayHandle: DialogueChat not find!"));
			return;
		}

		// - Set NPC Talk Image
		SetDialogueImage(askerId, ImageType, bLeft, imageName);

		// - Set NPC Talk Msg
		AddStrToChatUI(m_ScriptMsgQueue.front().MsgLst.front());
		ReSizeBaseBGUI();

		//SetDiaglogueDetectALL();

		gGlobal.pUIManager->RenderAll();
	}
}

VOID CScriptControl::OnChooseHandle(UInt8 askerType, UInt32 askerId, Int Opt, Boolean waitOther, 
									UInt16 timeout, const StringVector & OptMsg)
{
	// check first open or not?
	if ( !m_bOpen )
	{
		m_ScriptMsgQueue.clear();
		m_CurrPage = 0;
	}

	Int size = (Int)m_ScriptMsgQueue.size();
	if ( size > 0 )
	{
		StringVector szFinal = OptMsg;
		PushMsgIntoQueue(SCRIPTMSG_CHOOSE, szFinal, askerId, askerType, waitOther, timeout);
	}
	else
	{
		OpenDialogue();

		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
		if ( pPanelEx )
		{
			pPanelEx->SetEnable(true);
			SetCountDown(timeout * 1000);
			m_bWaitOther = waitOther;

			// - Set NPC Talk Image
			SetDialogueImage(askerId, askerType);

			StringVector strlist = OptMsg;

			if ( (Int)strlist.size() == (Opt + 1) )
				AddStrToChatUI(strlist.back(), 16 - Opt);

			strlist.pop_back();

			AddOptionToSelectPopUpUI(strlist);
			ReSizeBaseBGUI();
		}
		m_bChoose = BOOLEAN_TRUE;

		gGlobal.pUIManager->RenderAll();
	}
}

VOID CScriptControl::OnEndScriptHandle()
{
	UIItem* pItem = gGlobal.pUIManager->GetUIItem("DialogueBox");
	if ( !(pItem && pItem->IsEnable()) )
	{// when the dialogue is invisiable
		// may have some problem with this initialization
		ResetPage();
		ResetPartyChoose();
		ResetMouseState();
		ResetNPCDirection();

		// not sure 
		m_bLockPos = BOOLEAN_FALSE;
	}

	SetFinish();
}

VOID CScriptControl::SetDisappearCenter(UInt16 x, UInt16 y)
{
	m_DisappearCenterX = x;
	m_DisappearCenterY = y;
}

VOID CScriptControl::ChangeNextPage()
{
	{// test code
		/*
		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");

		if ( pPanelEx && pPanelEx->IsEnable() )
		{
			Int32 x = gGlobal.pUIManager->GetCursorX();
			Int32 y = gGlobal.pUIManager->GetCursorY();
			if ( pPanelEx->Detect(x, y) == false )
			{
				if ( !IsLockedWhenTalking() )
					QGLApplication::GetInstance().MoveToScreenPos(x, y, TRUE);
				return;
			}
		}
		*/
	}

//	{// Is selecting the option now ?
//		UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("DialogueChoosePopup");
//		if ( pPopUpMenu && pPopUpMenu->IsEnable() )
//			return;
//	}

	//SetDiaglogueDetectALL(BOOLEAN_FALSE);

	StlDeque<SScriptMsg> CopyMsgQueue = m_ScriptMsgQueue;
	Int MsgSize = (Int)CopyMsgQueue.size();
	UInt8 nextpage = m_CurrPage + 1;

	// handle wait msg when click next msg
	if ( m_uShowWaiting > 0 )
	{
		UInt32 currtime = ::timeGetTime();
		UInt32 remaintime = currtime - m_WaitTimeOut;
		if ( remaintime < gConfigure.uChooseRemainTime )
		{
			String sysmsg;
			sysmsg.Format(_T("Waiting Time Remain :  %d"), remaintime / 1000);
			gGlobal.PrintMessage(sysmsg, CHANNELTYPE_SYSTEM);
			return;
		}
	}
	//

	if ( nextpage >= MsgSize )
	{
		TerminateTalkingDialogue();
		return;
	}

	Int type = 0;
	if ( MsgSize > 0 )
		type = CopyMsgQueue[nextpage].Type;

	switch ( type )
	{
	case SCRIPTMSG_SAY:
		{
			Int size = (Int)CopyMsgQueue.size();
			if ( nextpage < size )
			{
				// check image change
				{
					UInt32 tmpnpcid = CopyMsgQueue[nextpage].askerId;
					Int imtype = CopyMsgQueue[nextpage].ImageType;
					if ( (CopyMsgQueue[m_CurrPage].ImageType != imtype) || ( CopyMsgQueue[m_CurrPage].askerId != tmpnpcid ) )
						SetDialogueImage(tmpnpcid, imtype, CopyMsgQueue[nextpage].Left, CopyMsgQueue[nextpage].ImageName);
				}

				String szbuff = CopyMsgQueue[nextpage].MsgLst.front();
				AddStrToChatUI(szbuff);
				ReSizeBaseBGUI();

				//SetDiaglogueDetectALL();

				// pop the useless msg
				if ( m_ScriptMsgQueue.size() > nextpage && (nextpage-1) >= 0)
				{
					Int temp = nextpage;
					for ( Int remove = 0; remove < temp; ++remove )
					{
						if ( !m_bWriteScriptMsg )
						{
							m_ScriptMsgQueue.pop_front();
							--nextpage;
						}
					}
				}
			}
		}
		break;
	case SCRIPTMSG_CHOOSE:
		{
			Int size = (Int)CopyMsgQueue.size();
			if ( nextpage < size )
			{
				// check image change
				m_bChoose = BOOLEAN_TRUE;
				UInt32 tmpnpcid = CopyMsgQueue[nextpage].askerId;
				Int imtype = CopyMsgQueue[nextpage].ImageType;
				if ( CopyMsgQueue[m_CurrPage].askerId != tmpnpcid )
					SetDialogueImage(tmpnpcid, imtype);
				SetCountDown(CopyMsgQueue[nextpage].timeout * 1000);
				m_bWaitOther = CopyMsgQueue[nextpage].waitOther;
				ShowOptionForChoose(nextpage, CopyMsgQueue);
			}
		}
		break;
	default:
		nextpage = 0;
		break;
	}// end switch-case 

	m_CurrPage = nextpage;

	gGlobal.pUIManager->RenderAll();
}

VOID CScriptControl::PlayerChooseResult(Int Opt)
{
	VMProtectBegin; VM_START;

	if ( m_pScriptSys && m_bChoose)
	{
		if ( m_bPartyChoose )
		{
			OnPlayerSelectPartyChooseHandle(Opt);
			m_bPartyChoose = BOOLEAN_FALSE;
		}
		else
			m_pScriptSys->ReplyAnswer(gGlobal.pPeerProxy, gGlobal.g_context, (UInt8) Opt);

		m_bChoose = BOOLEAN_FALSE;
		if (m_bWaitOther) {
			StringVector strList;
			strList.push_back(_T(""));
			AddOptionToSelectPopUpUI(strList);
			PCSTRINGW waitMsg = GetLocalStr("MSG_SCRIPT_WAIT");
			if (waitMsg) {
				String s;
				s.Format(_T("\\#G(%s)\\#W"), waitMsg);
				AddStrToChatUI(s, 2, BOOLEAN_TRUE);
			}
			//UIStaticText *pCountText = gGlobal.pUIManager->FindUIStaticText("DialogueCounter");
			//if (pCountText && pCountText->IsEnable()) {
			//	pCountText->SetText(waitMsg);
			//	pCountText->Redraw();
			//}
			//ShowSelectedOption(m_CurrPage, m_ScriptMsgQueue, Opt);
		}
	}

	VMProtectEnd; VM_END;
}

VOID CScriptControl::OnPlayerSelectPartyChooseHandle(Int Opt)
{
	VMProtectBegin; VM_START;

	if ( m_pScriptSys )
	{
		if ( m_bLockAndCount )
		{
			if ( (::timeGetTime() - m_WaitTimeOut) < gConfigure.uChooseRemainTime )
			{// can reply choose
				m_pScriptSys->ReplyAnswer(gGlobal.pPeerProxy, gGlobal.g_context, (UInt8) Opt);
				//m_pScriptSys->OnPartyChooseResult(gGlobal.pPeerProxy, gGlobal.g_context, Opt);
				return;
			}
		}

		gGlobal.PrintMessage(_T("Party Choose Time Out"), CHANNELTYPE_SYSTEM);
	}
	VMProtectEnd; VM_END;
}

VOID CScriptControl::OnChooseFailed()
{
	ResetPage();
	ResetMouseState();

	TRACE_INFODTL(GLOBAL_LOGGER, _T("CScriptControl OnChooseFailed: Player choose answer failed!"));
}

VOID CScriptControl::SetDiaglogueDetectALL(Boolean bFlag)
{
	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
	if ( pPanelEx )
	{
		if ( bFlag )
		{
			pPanelEx->SetDetectImage(false);
			pPanelEx->SetDetectRect(0, 0, QGLApplication::GetInstance().GetRenderClass()->GetWinWidth() - 1, 
			QGLApplication::GetInstance().GetRenderClass()->GetWinHeight() - 1);
		}
		else
		{
			pPanelEx->SetDetectImage(true);
			pPanelEx->SetDetectRect();
		}
	}
}

VOID CScriptControl::TerminateTalkingDialogue(Boolean bEnforceInterupt)
{
	Boolean bWait = m_bWaitEnd;
	ResetData();

	if ( bEnforceInterupt )
	{
		//SetFinish();
		//ResetMouseState();
	}
	else if ( IsFinished() )
	{
		ResetMouseState();
	}
	if (m_pScriptSys && (bWait || bEnforceInterupt)) {
		m_pScriptSys->StopTalk(gGlobal.pPeerProxy, RPCContext(), bEnforceInterupt);
		OnEndScriptHandle();
	}
	//TRACE_INFODTL(GLOBAL_LOGGER, _T("CScriptControl TerminateTalkingDialogue: Not enforce terminate talking dialogue"));
}

VOID CScriptControl::CB_OnExecuteAllHandle(UInt32 TemplateId, UInt32 InstanceId, UInt16 MapId, String filename)
{
	CCharacter* pChar = CastNetObject<CCharacter>(gGlobal.g_pCharacter->GetCharNetObject());
	CMapActor* pActor = CastNetObject<CMapActor>(pChar);
	if ( pActor && ( pActor->GetMap_id() == MapId ) )
	{
		CMapNPC* pnpc = gGlobal.g_pMapView->GetMapObject()->FindNpc(InstanceId);
		if ( pnpc )
		{
			if ( (pnpc->GetNpc_id() == TemplateId) && 
				(pnpc->GetMap_id() == MapId) &&
				(gGlobal.g_pCharacter->GetCharNetObject()->GetMap_id() == MapId) )
			{
				pnpc->ResponseExecuteAll(filename);
			}
		}
	}
}

VOID CScriptControl::StopTalkingBeforeBattle()
{
	CloseDialogue();

	OnEndScriptHandle();

	gGlobal.g_bClickNPCFlag = FALSE;

	CMapActorControl* pMapCtrl = QGLApplication::GetInstance().GetTalkingMapControl();
	if( pMapCtrl )
	{
		CMapNPCControl * pMapNPCCtrl = dynamic_cast<CMapNPCControl *>(pMapCtrl);
		if( pMapNPCCtrl )
			pMapNPCCtrl->ChangeRotation(pMapNPCCtrl->GetInitRotation());
		QGLApplication::GetInstance().SetTalkingMapControl(NULL);
	}

	if ( gGlobal.g_CursorState && gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TALKING )
	{
		if ( gGlobal.g_CursorState->IsBlocked() )
			gGlobal.g_CursorState->Unblock();

		State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
	}
}

Boolean CScriptControl::EnterNextPageHandle()
{
	if ( !m_bDoNextPage )
	{
		m_bDoNextPage = BOOLEAN_TRUE;
		return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

VOID CScriptControl::ExitNextPageHandle()
{
	m_bDoNextPage = BOOLEAN_FALSE;
}

VOID CScriptControl::NextPageHandle()
{
	if ( EnterNextPageHandle() )
	{
		// Is not selecting the option
		UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("DialogueChoosePopup");
		if (pPopUpMenu == NULL || !pPopUpMenu->IsEnable())
			ChangeNextPage();
		ExitNextPageHandle();
	}
}

VOID CScriptControl::BattleDefaultDialogue()
{
	//find the npc picture
	if ( !m_bOpen )
	{
		m_ScriptMsgQueue.clear();
		m_CurrPage = 0;
	}

	String SayMsg(_T("Enter Battle"));
	// -push msg into queue
	PushMsgIntoQueue(SCRIPTMSG_SAY, SayMsg);
	
	if ( !m_bOpen && (Int)m_ScriptMsgQueue.size() > 0 )
	{
		OpenDialogue();

		{
			UIItem* pItem = gGlobal.pUIManager->GetUIItem("DialogueBox");
			if ( pItem && !pItem->IsEnable() )
				pItem->SetEnable(true);
		}

		UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("DialogueChat");
		if ( !pChat )
		{
			ResetPage();
			TRACE_INFODTL(GLOBAL_LOGGER, _T("CScriptControl OnSayHandle: DialogueChat not find!"));
			return;
		}

		// - Set NPC Talk Image
		{
			UIImage * NPCImage = gGlobal.pUIManager->FindUIImage("NPCPic");
			if (NPCImage)
			{
					//Char FileName[50];
					//GetPrivateProfileString(_T("LOADFILE"), _T("NPCIMAGE"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
					NPCImage->SetImageFile(_T("Data\\NPCFACE\\imgM20004a.tga"), 100, 100);
			}
		}

		// - Set NPC Talk Msg
		AddStrToChatUI(m_ScriptMsgQueue.front().MsgLst.front());
		ReSizeBaseBGUI();

		//SetDiaglogueDetectALL();

		gGlobal.pUIManager->RenderAll();
	}
}

VOID CScriptControl::OnLockPosHandle(Int lock)
{
	if ( lock == 0 )
		m_bLockPos = BOOLEAN_FALSE;
	else
		m_bLockPos = BOOLEAN_TRUE; 
}

VOID CScriptControl::OnPartySayHandle(UInt32 Npcid, const String & SayMsg, UInt32 InstanceId, Int ImageType, Boolean bLeft)
{
	if ( !gGlobal.g_pMap ) return;

	//CMapNPC* pnpc = gGlobal.g_pMap->FindNpc(InstanceId);
	//if ( !pnpc ) return;

	if ( gGlobal.g_pParty || gGlobal.g_pTempLeaveParty) 
	{
		CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
		CCharacter* pleader = pParty->GetMember(0);
		if ( pleader ) 
		{
			m_DisappearCenterX = pleader->GetCurPosX();
			m_DisappearCenterY = pleader->GetCurPosY();
		}
	}
	else 
	{
		m_DisappearCenterX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
		m_DisappearCenterY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
	}

	// test code
	if ( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NPC || 
		gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_NORMAL )
	{
		if ( State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINPCTalkingState)) )
		{
			gGlobal.g_CursorState->Block();
			OnSayHandle(Npcid, SayMsg, ImageType, bLeft);
		}
	}
	else if ( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TALKING )
	{
		OnSayHandle(Npcid, SayMsg, ImageType, bLeft);
	}
}

VOID CScriptControl::OnPartyChooseHandle(UInt32 Npcid, Int Opt, const StringVector & OptMsg, UInt32 time)
{
	m_bLockPos = BOOLEAN_TRUE;
	m_bPartyChoose = BOOLEAN_TRUE;
	m_bLockAndCount = BOOLEAN_TRUE;

	m_WaitTimeOut = time;
	// check first open or not?
	if ( !m_bOpen )
	{
		m_ScriptMsgQueue.clear();
		m_CurrPage = 0;
	}

	if ( (Int)m_ScriptMsgQueue.size() > 0 )
	{
		StringVector szFinal = OptMsg;
		PushMsgIntoQueue(SCRIPTMSG_CHOOSE, szFinal, Npcid);
	}
	else
	{// first open the dialogue
		OpenDialogue();

		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
		if ( pPanelEx )
		{
			pPanelEx->SetEnable(true);

			// - Set NPC Talk Image
			SetDialogueImage(Npcid);

			StringVector strlist = OptMsg;
			if ( (Int)strlist.size() == (Opt+1) )
				AddStrToChatUI(strlist.back(), 16 - Opt);

			strlist.pop_back();

			AddOptionToSelectPopUpUI(strlist);
			ReSizeBaseBGUI();
		}

		gGlobal.pUIManager->RenderAll();
	}
}

VOID CScriptControl::OnPartyChooseRelpy(UInt32 Result)
{
	OnWaitForMember();
	TRACE_INFODTL(GLOBAL_LOGGER, _T("CScriptControl OnPartyChooseRelpy: Player choose answer failed!"));
}

VOID CScriptControl::PartyChooseTimeOutHandle()
{
	if (m_pScriptSys)
		//m_pScriptSys->OnLeaderReplyPartyChoose(gGlobal.pPeerProxy, RPCContext());
		m_pScriptSys->ReplyAnswer(gGlobal.pPeerProxy, RPCContext(), 255);
}

VOID CScriptControl::OnWaitForMember()
{
	if ( m_bLockAndCount )
	{
		m_uShowWaiting = 1 + (UInt16)(rand() % 1000);

		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("DialogueBox");
		if ( pPanelEx )
		{
			if ( pPanelEx->IsEnable() == false )
			{
				m_ScriptMsgQueue.clear();
				m_CurrPage = 0;

				OpenDialogue();
			}

			String WaitMsg(_T("Waiting for the another member(s)..."));
			AddStrToChatUI(WaitMsg);
			ReSizeBaseBGUI();
			gGlobal.pUIManager->RenderAll();
		}
	}
}

Int CScriptControl::CountDown()
{
	if ( m_bLockAndCount )
	{
		UInt32 currtime = ::timeGetTime();
		// check count down finish 
		if ( (currtime - m_WaitTimeOut) > gConfigure.uChooseRemainTime )
		{
			m_bLockAndCount = BOOLEAN_FALSE;
			m_WaitTimeOut = 0;
			ChooseTimeOutHandle();
		}
		else
		{// count down 
			if ( (currtime - m_pretime) > 1000 )
				m_pretime = currtime;
			
			return ((currtime - m_WaitTimeOut) / 1000);
		}
	}
	return -1;
}

VOID CScriptControl::ChooseTimeOutHandle()
{
	m_uShowWaiting = 0;
	m_bPartyChoose = BOOLEAN_FALSE;

	{// no choose until count down have finished
		if (gGlobal.g_pParty && gGlobal.g_pParty->IsLeader())
			PartyChooseTimeOutHandle();
	}
	
	StlDeque<SScriptMsg> CopyMsgQueue = m_ScriptMsgQueue;
	Int MsgSize = (Int)CopyMsgQueue.size();
	UInt8 nextpage = m_CurrPage + 1;

	// check nextpage which have any msg or not?
	if ( nextpage >= MsgSize )
	{
		TerminateTalkingDialogue();
		return;
	}

	// handle next page
	Int type = SCRIPTMSG_NONE;
	if ( MsgSize > 0 )
		type = CopyMsgQueue[nextpage].Type;

	switch ( type )
	{
	case SCRIPTMSG_SAY:
		{
			Int size = (Int)CopyMsgQueue.size();
			if ( nextpage < size )
			{
				String szbuff = CopyMsgQueue[nextpage].MsgLst.front();
				AddStrToChatUI(szbuff);
				ReSizeBaseBGUI();

				//SetDiaglogueDetectALL();

				// pop the useless msg
				if ( m_ScriptMsgQueue.size() > nextpage && (nextpage-1) >= 0)
				{
					Int temp = nextpage;
					for ( Int remove = 0; remove < temp; ++remove )
					{
						if ( !m_bWriteScriptMsg )
						{
							m_ScriptMsgQueue.pop_front();
							--nextpage;
						}
					}
				}
			}
		}
		break;
	case SCRIPTMSG_CHOOSE:
		{
			Int size = (Int)CopyMsgQueue.size();
			if ( nextpage < size )
			{
				m_bChoose = BOOLEAN_TRUE;
				ShowOptionForChoose(nextpage, CopyMsgQueue);
			}
		}
		break;
	default:
		nextpage = 0;
		break;
	}// end switch-case 

	m_CurrPage = nextpage;

	gGlobal.pUIManager->RenderAll();
}

Boolean CScriptControl::ShowOptionForChoose(UInt8 page, const StlDeque<SScriptMsg> CopyMsg)
{
	Int size = (Int)CopyMsg.size();
	if ( page < size )
	{
		StringVector strlist;
		strlist.clear();
		strlist = CopyMsg[page].MsgLst;

		//StringW szbuff = strlist.back();
		int strcount = (int)strlist.size();
		StringW szbuff = strlist[strcount-1];
		AddStrToChatUI(szbuff, 16 - (strcount-1));

		strlist.pop_back();

		AddOptionToSelectPopUpUI(strlist);
		ReSizeBaseBGUI();

		strlist.clear();

		return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

void CScriptControl::ShowSelectedOption(UInt8 page, const StlDeque<SScriptMsg> CopyMsg, Int Opt)
{
	Int size = (Int)CopyMsg.size();
	if ( page < size )
	{
		const StringVector &strlist = CopyMsg[page].MsgLst;
		UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("DialogueChat");
		if (pChat == NULL)
			return;

		//StringW szbuff = strlist.back();
		int strcount = (int)strlist.size();
		UInt32 maxRow = 16 - (strcount-1);
		StringW szbuff = strlist[strcount-1];
		pChat->ClearAllRow();
		pChat->AddStringW(szbuff.c_str());

		for (int i = 0; i < (int) strlist.size() - 1; i++) {
			szbuff.Format(_T("\\#%c%s"), (i == Opt ? _T('R') : _T('G')),
				strlist[i].c_str());
			pChat->AddStringW(szbuff.c_str());
		}
		ReSizeChatControl(maxRow);
		pChat->Redraw();
		ReSizeBaseBGUI();
	}
}

VOID CScriptControl::OnPartyChooseSuccessHandle()
{
	CloseDialogue();

	ResetMouseState();
}


VOID CScriptControl::CB_OnOpenInterfaceHandle(StringA szInterface, Int32 Param1)
{
	if ( szInterface == "shop" )
	{
	}
	else if ( szInterface == "skillshop" )
	{
		gGlobal.g_ShopSkillFlag = true;
		gGlobal.g_pSkillProcess->ConstructByShopID(Param1);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSkillPanel", String(), 0, 0);		
		gGlobal.g_pSkillProcess->RefreshShopDetailItem();
	}
	else if (szInterface == "mixshop")
	{
		gGlobal.g_pMenuMixProcess->OpenMixShop((UInt8)Param1);
	}
	else if (szInterface == "HawkerOwnerMenu")
	{
		gGlobal.g_pHawkerProcess->OpenHawkerOwnerMenu();
	}
	else if (szInterface == "bankdeposit")
	{
		gGlobal.g_pMenuBankService->OpenDepositInterface();
	}
	else if (szInterface == "bankwithdraw")
	{
		gGlobal.g_pMenuBankService->OpenWithdrawalInterface();
	}
	else if (szInterface == "cultivation")
	{
		gGlobal.g_pPartnerCultivationUpProcess->OpenPartnerCultivationUp((int)Param1);
	}
}

VOID CScriptControl::OnOpenShopMenuHandle(Int32 ShopId, UInt8 Title, UInt8 Enable, UInt16 Buy, UInt16 Sell, UInt16 Repair, UInt16 uiShopType, UInt32Vector Lst)
{
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->OpenShopMenu(ShopId, Title, Enable, Buy, Sell, Repair, uiShopType, Lst);
}

VOID CScriptControl::OnShowSysHint(Int Type, UInt32 V1, UInt32 V2)
{
	String sysMsg(_T(""));
	switch ( Type )
	{
	case 1:
		{// game money
			String tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SCRIPT_ADDGAMEMONEY"));
			sysMsg.Format(tempstr.c_str(), V1);
		}
		break;
	case 2:
		{// exp
			String tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SCRIPT_ADDEXP"));
			sysMsg.Format(tempstr.c_str(), V1);
		}
		break;
	case 3:
		{// item
			String tempstr =gGlobal.GetStringTable()->Get(_T("MSG_SCRIPT_ADDITEM"));
			const ItemData* ptr = gGlobal.m_resource->GetItemData(V1);
			if ( ptr )
			{
				sysMsg.Format(tempstr.c_str(), ptr->name);
			}
		}
		break;
	}

	//gGlobal.PrintPlayerMessage(popupMsg);
	gGlobal.PrintMessage(sysMsg, CHANNELTYPE_SYSTEM);
}

VOID CScriptControl::OnPopUpHint(String Hint)
{
	String temp = Hint;
	gGlobal.PrintPlayerMessage(temp);
}

VOID CScriptControl::OnPlaySound(StringA Name)
{
	StringA temp = Name;
	FSound *Sound = FSOUND_Manager::GetInstance().CreateSoundA(temp.c_str());
	Sound->SetVolume( gGlobal.g_pSystemSetting->GetSoundEffectVolume() );
	Sound->Play();
}

Boolean CScriptControl::PrepareItemScriptHandle()
{
	if ( IsFinished() && IsLockedWhenTalking() == BOOLEAN_FALSE )
	{
		if (gGlobal.g_pCharacter)
			gGlobal.g_pCharacter->Stop();

		if ( State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINPCTalkingState)) )
		{
			gGlobal.g_CursorState->Block();

			// - Set Disappear Position Center
			m_DisappearCenterX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
			m_DisappearCenterY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;

			StartUp();
			OnLockPosHandle(BOOLEAN_TRUE);

			m_bPrepareItemScript = BOOLEAN_TRUE;
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

VOID CScriptControl::OpenDialogue()
{
	m_bOpen = BOOLEAN_TRUE;

	m_DisappearCenterX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
	m_DisappearCenterY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseDialogueBox", String(), 100, 100);
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenDialogueBox", String(), 100, 100);
}

VOID CScriptControl::CloseDialogue()
{
	ResetPage();
	ResetPartyChoose();

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseDialogueBox", String(), 100, 100);
	m_bLockPos = BOOLEAN_FALSE;

	UIImage * NPCImage = gGlobal.pUIManager->FindUIImage("NPCPic");
	if (NPCImage )
	{
		NPCImage->Dispose();
	}

}

void CScriptControl::UpdateDialogue(UInt32 curTime)
{
	if (m_CountDownTime == 0)
		return;

	UInt32 dt = curTime - m_PreUpdateTime;
	UInt32 Num;

	if (m_CountDownTime > dt)
		m_CountDownTime -= dt;
	else
		m_CountDownTime = 0;
	Num = (m_CountDownTime + 999) / 1000;
	m_CountDownEnt.SetNumber(Num);
	UIStaticText *pCountText = gGlobal.pUIManager->FindUIStaticText("DialogueCounter");
	if (pCountText) {
		String s;
		s.Format(_T("%d"), Num);
		pCountText->SetText(s);
		pCountText->Redraw();
	}
	if (m_CountDownTime == 0) {
		m_CountDownEnt.SetVisible(FALSE);
		//ChangeNextPage();
	}
	m_PreUpdateTime = curTime;
}

VOID CScriptControl::SetCountDown(UInt32 timeout)
{
	bool enable = (timeout > 0);
	m_CountDownTime = timeout;
	m_PreUpdateTime = ::timeGetTime();
	UIPanel *pCountPanel = gGlobal.pUIManager->FindUIPanel("DialogueCountPanel");
	if (pCountPanel)
		pCountPanel->SetEnable(enable);
	m_CountDownEnt.SetVisible(enable);
}

VOID CScriptControl::OnNpcGivenHandle(UInt32 npcID, UInt32Vector idList, UInt8Vector typeList, String npcMessage)
{
	gGlobal.g_pGivenProcess->OpenNpcGivenPanel(npcID, idList, typeList, npcMessage);
}

VOID CScriptControl::NpcGiven(UInt32Vector uidList, UInt8Vector typeList)
{
	if (m_pScriptSys && gGlobal.pPeerProxy)
		m_pScriptSys->ReplyGiven(gGlobal.pPeerProxy, gGlobal.g_context, typeList, uidList);
}

VOID CScriptControl::OnNpcGivenFinish()
{
	gGlobal.g_pGivenProcess->SetIsNpcGiven(false);
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseGivenPanel", String());		
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayFinishGivenEffect", String());
}

VOID CScriptControl::OnAskString(String text, String defAns, bool left, UInt8 askerType,
								 UInt32 askerId)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenStringReply", String());
	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("StringReplyText");
	if (pText)
	{
		pText->SetText(text);
		pText->Redraw();
	}

	UIEditBox * pEdit = gGlobal.pUIManager->FindUIEditBox("StringReplyEdit");
	if (pEdit)
	{
		pEdit->SetText(defAns);
		pEdit->Redraw();
	}
}

VOID CScriptControl::OnShowArr(UInt8 dir, UInt8 Arr, UInt16 dirX, UInt16 dirY)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenGuideArrow", String());
	UIAniIcon* Arrow1 = gGlobal.pUIManager->FindUIAniIcon("ClickArrowLeft");
	UIAniIcon* Arrow2 = gGlobal.pUIManager->FindUIAniIcon("ClickArrowRight");
	UIAniIcon* Arrow3 = gGlobal.pUIManager->FindUIAniIcon("ClickArrowDown");
	UIAniIcon* Arrow4 = gGlobal.pUIManager->FindUIAniIcon("ClickArrowLeft02");

	switch(dir)
	{
		case 1:
			{
				if (Arr == 1)
				{
					Arrow1->SetOffsetXY(dirX, dirY);
					Arrow1->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(Arrow1);
				}
				else
					Arrow1->SetEnable(false);				
			}
			break;
		case 2:
			{
				if (Arr == 1)
				{
					Arrow2->SetOffsetXY(dirX, dirY);
					Arrow2->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(Arrow2);
				}
				else
					Arrow2->SetEnable(false);	
			}
			break;
		case 3:
			{
				if (Arr == 1)
				{
					Arrow3->SetOffsetXY(dirX, dirY);
					Arrow3->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(Arrow3);
				}
				else
					Arrow3->SetEnable(false);	
			}
			break;
		case 4:
			{
				if (Arr == 1)
				{
					Arrow4->SetOffsetXY(dirX, dirY);
					Arrow4->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(Arrow4);
				}
				else
					Arrow4->SetEnable(false);	
			}
			break;
	}
}

VOID CScriptControl::OnShowGuiPic(String context, String img1_name/* = */, UInt16 img1_X/* =0 */, UInt16 img1_Y/* =0  */,String img2_name/* = */, UInt16 img2_X/* =0 */, UInt16 img2_Y/* =0  */,String img3_name/* = */, UInt16 img3_X/* =0 */, UInt16 img3_Y/* =0  */,String img4_name/* = */, UInt16 img4_X/* =0 */, UInt16 img4_Y/* =0  */,String img5_name/* = */, UInt16 img5_X/* =0 */, UInt16 img5_Y/* =0 */)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenNewGuide", String());
	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("NewGuideContext");
	UIAniIcon* img1AniIcon = gGlobal.pUIManager->FindUIAniIcon("NewGuideImg1");
	UIAniIcon* img2AniIcon = gGlobal.pUIManager->FindUIAniIcon("NewGuideImg2");
	UIAniIcon* img3AniIcon = gGlobal.pUIManager->FindUIAniIcon("NewGuideImg3");
	UIAniIcon* img4AniIcon = gGlobal.pUIManager->FindUIAniIcon("NewGuideImg4");
	UIAniIcon* img5AniIcon = gGlobal.pUIManager->FindUIAniIcon("NewGuideImg5");

	if(!context.Equals(_T("0")))
	{
		pChat->ClearAllRow();
		pChat->AddStringW(context.c_str());
		pChat->HomeRow();
		UInt Line = pChat->FindHighLightWord(0);
		pChat->SetRow(Line - 1, pChat->RowListCount());
	}
	else
		OnCloseGuide();
		
	if (!img1_name.Equals(_T("0")))
	{		
		img1AniIcon->ClearAniData();
		img1AniIcon->SetOffsetXY(img1_X, img1_Y);
		img1AniIcon->SetAnimationData(img1_name.c_str());
		img1AniIcon->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(img1AniIcon);
	}
	else
		img1AniIcon->SetEnable(false);

	if (!img2_name.Equals(_T("0")))
	{		
		img2AniIcon->ClearAniData();
		img2AniIcon->SetOffsetXY(img2_X, img2_Y);
		img2AniIcon->SetAnimationData(img2_name.c_str());
		img2AniIcon->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(img2AniIcon);
	}
	else
		img2AniIcon->SetEnable(false);

	if (!img3_name.Equals(_T("0")))
	{
		img3AniIcon->ClearAniData();
		img3AniIcon->SetOffsetXY(img3_X, img3_Y);
		img3AniIcon->SetAnimationData(img3_name.c_str());
		img3AniIcon->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(img3AniIcon);
	}
	else
		img3AniIcon->SetEnable(false);

	if (!img4_name.Equals(_T("0")))
	{
		img4AniIcon->ClearAniData();
		img4AniIcon->SetOffsetXY(img4_X, img4_Y);
		img4AniIcon->SetAnimationData(img4_name.c_str());
		img4AniIcon->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(img4AniIcon);
	}
	else
		img4AniIcon->SetEnable(false);

	if (!img5_name.Equals(_T("0")))
	{
		img5AniIcon->ClearAniData();
		img5AniIcon->SetOffsetXY(img5_X, img5_Y);
		img5AniIcon->SetAnimationData(img5_name.c_str());
		img5AniIcon->SetEnable(true);
		gGlobal.pUIManager->AddDirtyUIItem(img5AniIcon);
	}
	else
		img5AniIcon->SetEnable(false);
}

VOID CScriptControl::OnCloseGuide()
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseNewGuide", String());
}

VOID CScriptControl::OnCloseArrow()
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseNewArrow", String());
}