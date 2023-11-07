//-- Common
#include "Common.h"
//-- Self
#include "BatScriptControl.h"
//-- Library
#include "Global.h"
#include "Common/CallLuaFunc.h"
#include "Proxy/Logic/Party.h"
#include "QGLApplication.h"
#include "CharacterControl.h"
#include "BattleControl.h"

CBatScriptControl::CBatScriptControl()
{
	Init();
}

CBatScriptControl::~CBatScriptControl()
{
	m_BatScriptMsgQueue.clear();
}

/*
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


VOID CScriptControl::ResetData()
{
	CloseDialogue();
	//m_nFinishScript = 1;
	
	// - reset npc direction, cancel selected npc target
	ResetNPCDirection();
}
*/
VOID CBatScriptControl::Init()
{
	m_dwDisEnableCmd = 0;
	ResetPage();
	//m_bOpen = BOOLEAN_FALSE;
	m_bDoNextPage = BOOLEAN_FALSE;
	m_bWriteFlag = BOOLEAN_FALSE;
}

VOID CBatScriptControl::ResetPage()
{
	m_CurrPage = 0;
	m_bOpen = BOOLEAN_FALSE;
	m_BatScriptMsgQueue.clear();
}

VOID CBatScriptControl::ResetData()
{
	m_dwDisEnableCmd = 0;
	ResetPage();
}

VOID CBatScriptControl::OpenBatDialog()
{
	m_bOpen = BOOLEAN_TRUE;

	TRACE_INFODTL(GLOBAL_LOGGER, _F("CBatScriptControl::OpenBatDialog"));

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BDB_CloseDialog", String(), 100, 100);
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BDB_OpenDialog", String(), 100, 100);
}
	
VOID CBatScriptControl::CloseBatDialog()
{
	ResetPage();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "BDB_CloseDialog", String(), 100, 100);
	m_bOpen = BOOLEAN_FALSE;
}

Boolean CBatScriptControl::EnterNextPageHandle()
{
	if ( !m_bDoNextPage )
	{
		m_bDoNextPage = BOOLEAN_TRUE;
		return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

VOID CBatScriptControl::ExitNextPageHandle()
{
	m_bDoNextPage = BOOLEAN_FALSE;
}

VOID CBatScriptControl::NextPageHandle()
{
	if ( EnterNextPageHandle() )
	{
		ChangeNextPage();
		ExitNextPageHandle();
	}
}

VOID CBatScriptControl::PushMsg(Int type, String Msg, UInt32 NpcId, Int Image, bool bLeft, bool finalSay)
{
	if ( (Int)m_BatScriptMsgQueue.size() > 100 )
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CBatScriptControl PushMsg: too many battle msgs since the player does not change the next page!!"));
		return;
	}

	if ( !m_bWriteFlag )
	{
		m_bWriteFlag = BOOLEAN_TRUE;

		SBatScriptMsg temp;

		temp.MsgLst.clear();
		temp.MsgLst.push_back(Msg);

		temp.Type = type;
		temp.askerId = NpcId;
		temp.ImageType = Image;
		temp.Left = bLeft;
		temp.FinalSay = finalSay;

		m_BatScriptMsgQueue.push_back(temp);

		m_bWriteFlag = BOOLEAN_FALSE;
	}
}

VOID CBatScriptControl::PushMsg(Int type, StringVector MsgLst, UInt32 askerId, Int Image)
{
	if ( (Int)m_BatScriptMsgQueue.size() > 100 )
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CBatScriptControl PushMsg 2: too many battle msgs since the player does not change the next page!!"));
		return;
	}

	if ( !m_bWriteFlag )
	{
		m_bWriteFlag = BOOLEAN_TRUE;

		SBatScriptMsg temp;
		temp.MsgLst = MsgLst;
		temp.Type = type;
		temp.askerId = askerId;
		temp.ImageType = Image;

		m_BatScriptMsgQueue.push_back(temp);

		m_bWriteFlag = BOOLEAN_FALSE;
	}
}

VOID CBatScriptControl::SetBatDialogueImage(UInt32 templateid, Int imageType, bool bLeft)
{
	UIImage * NPCImage = gGlobal.pUIManager->FindUIImage("BDB_NPCPic");
	if ( !NPCImage ) return;

	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("BDB_DialogImagePanel");
	if ( panel )
	{
		if ( bLeft )
			panel->SetPos(210, panel->GetY());
		else
			panel->SetPos(400, panel->GetY());
	}

	if ( imageType == 0 )
	{	// npc image by npc_id
		NPCDB_Client* npctemp = gGlobal.LoadNpcTemplate(templateid);
		if ( npctemp )
		{
			String str = _T("Data\\NPCFACE\\");
			str += npctemp->m_basicinfo->m_npc_image;

			NPCImage->SetImageFile(str.c_str(), 100, 100);
		}
		else
		{
			NPCImage->SetImageFile(_T("Data\\NPCFACE\\imgM20004a.tga"), 100, 100);
		}

		if (!(NPCImage->IsEnable()))
			NPCImage->SetEnable(true);
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

VOID CBatScriptControl::AddStrToChatUI(String str, UInt32 dwMaxRow, Boolean bAppend, Boolean bReDraw)
{
	UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("BDB_DialogueChat");
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

VOID CBatScriptControl::AddOptionToSelectPopUpUI(const StringVector & StrLst, Boolean bReDraw)
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("BDB_DialogueChoosePopup");
	if ( pPopUpMenu )
	{
		StringVector szBuff = StrLst;
		Int size = (Int)szBuff.size();
		if ( size > 0 )
		{
			pPopUpMenu->ClearData();
			//pPopUpMenu->Redraw();

			for ( Int i = 0; i < size; ++i )
			{
				String str = szBuff[i];
				pPopUpMenu->AddString(str.c_str(), i, "");
			}

			//pPopUpMenu->SetImage(size, 24);
			//pPopUpMenu->Redraw();

			//ReSizeBaseBGUI();

			ReSizeSelectPopUp();
		}
	}
}

VOID CBatScriptControl::ReSizeChatControl(UInt32 dwMaxRow, Boolean bReDraw)
{
	UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("BDB_DialogueChat");
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

VOID CBatScriptControl::ReSizeSelectPopUp()
{
	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("BDB_DialogueChoosePopup");
	if ( pPopUpMenu )
	{
		pPopUpMenu->SetEnable(true);

		UInt32 uCurrRow = 0;
		UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("BDB_DialogueChat");
		if ( pChat && pChat->IsEnable() )
		{
			uCurrRow = pChat->RowListCount();
			if ( (uCurrRow + pPopUpMenu->GetStringListSize()) > 16 )
				uCurrRow = 16 - pPopUpMenu->GetStringListSize();
		}

		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("BDB_DialogueBox");
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

VOID CBatScriptControl::ReSizeBaseBGUI(Boolean bReDraw)
{
	UInt32 ChatRow = 0, PopUpRow = 0;
	UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("BDB_DialogueChat");
	if ( pChat && pChat->IsEnable() )
	{
		if ( pChat->RowListCount() > 0 )
			ChatRow = pChat->RowListCount();
	}

	UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("BDB_DialogueChoosePopup");
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
			if ( ChatRow > 16 )
				uTotal = 16;
			else
				uTotal = ChatRow;
		}
	}
	else
	{
		if ( PopUpRow )
		{// only popup
			uTotal = PopUpRow;
		}
	}

	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("BDB_DialogueBox");
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

Boolean CBatScriptControl::ShowOptionForChoose(UInt8 page, const StlDeque<SBatScriptMsg> & CopyMsg)
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

VOID CBatScriptControl::SetDiaglogueDetectALL(Boolean bFlag)
{
	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("BDB_DialogueBox");
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

VOID CBatScriptControl::ChangeNextPage()
{
	{// Is selecting the option now ?
		UIPopUpMenu * pPopUpMenu = gGlobal.pUIManager->FindUIPopUpMenu("BDB_DialogueChoosePopup");
		if ( pPopUpMenu && pPopUpMenu->IsEnable() )
			return;
	}

	//SetDiaglogueDetectALL(BOOLEAN_FALSE);

	StlDeque<SBatScriptMsg> CopyMsgQueue = m_BatScriptMsgQueue;
	Int MsgSize = (Int)CopyMsgQueue.size();
	UInt8 nextpage = m_CurrPage + 1;

	/*
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
	*/
	//

	gGlobal.g_pBattleControl->BatNextTalk();
	if ( nextpage >= MsgSize )
	{
		if (nextpage > 0)
		{
			if (CopyMsgQueue[nextpage - 1].FinalSay)
			{
				if (gGlobal.g_pBattleControl->GetControlPlayer())
					gGlobal.g_pBattleControl->GetControlPlayer()->SendBattleCommand(BATCMD_IDLE, 0, 0, 0);

				if (gGlobal.g_pBattleControl->GetControlPartner())
					gGlobal.g_pBattleControl->GetControlPartner()->SendBattleCommand(BATCMD_IDLE, 0, 0, 0);
				
			}
		}
		DisappearDialogHandle();
		//if (MsgSize > 0 && gGlobal.g_pBattleControl)
		return;
	}

	Int type = 0;
	if ( MsgSize > 0 )
		type = CopyMsgQueue[nextpage].Type;

	switch ( type )
	{
	case BATSCRIPTMSG_SAY:
		{
			Int size = (Int)CopyMsgQueue.size();
			if ( nextpage < size )
			{
				// check image change
				{
					UInt32 tmpnpcid = CopyMsgQueue[nextpage].askerId;
					Int imtype = CopyMsgQueue[nextpage].ImageType;
					if ( (CopyMsgQueue[m_CurrPage].ImageType != imtype) || ( CopyMsgQueue[m_CurrPage].askerId != tmpnpcid ) )
						SetBatDialogueImage(tmpnpcid, imtype, CopyMsgQueue[nextpage].Left);
				}

				String szbuff = CopyMsgQueue[nextpage].MsgLst.front();
				AddStrToChatUI(szbuff);
				ReSizeBaseBGUI();

				//SetDiaglogueDetectALL();

				// pop the useless msg
				if ( m_BatScriptMsgQueue.size() > nextpage && (nextpage-1) >= 0)
				{
					Int temp = nextpage;
					for ( Int remove = 0; remove < temp; ++remove )
					{
						if ( !m_bWriteFlag )
						{
							m_BatScriptMsgQueue.pop_front();
							--nextpage;
						}
					}
				}
			}
		}
		break;
	case BATSCRIPTMSG_CHOOSE:
		{
			Int size = (Int)CopyMsgQueue.size();
			if ( nextpage < size )
			{
				// check image change
				{
					UInt32 tmpnpcid = CopyMsgQueue[nextpage].askerId;
					Int imtype = CopyMsgQueue[nextpage].ImageType;
					if ( CopyMsgQueue[m_CurrPage].askerId != tmpnpcid )
						SetBatDialogueImage(tmpnpcid, imtype);
				}

				ShowOptionForChoose(nextpage, CopyMsgQueue);
			}
		}
		break;
	case BATSCRIPTMSG_ENDCMD:
		{
			DisappearDialogHandle();

			if ( gGlobal.g_pBattleControl )
				gGlobal.g_pBattleControl->BatNextTalk();
			return;
		}
		break;
	default:
		nextpage = 0;
		break;
	}// end switch-case 

	m_CurrPage = nextpage;

	gGlobal.pUIManager->RenderAll();
}

VOID CBatScriptControl::DisappearDialogHandle()
{
	CloseBatDialog();
}

VOID CBatScriptControl::OnBatSayHandle(UInt32 askerId, const String & SayMsg, Int ImageType, Boolean bLeft, Boolean finalSay)
{
	//clear msg queue if the dialogue is not opened
	if ( !m_bOpen )
	{
		m_BatScriptMsgQueue.clear();
		m_CurrPage = 0;
	}

	// -push msg into queue
	PushMsg(BATSCRIPTMSG_SAY, SayMsg, askerId, ImageType, bLeft, finalSay);	
	
	if ( !m_bOpen && (Int)m_BatScriptMsgQueue.size() > 0 )
	{
		OpenBatDialog();

		UIChatPanel * pChat =  gGlobal.pUIManager->FindUIChatPanel("BDB_DialogueChat");
		if ( !pChat )
		{
			ResetPage();
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CBatScriptControl OnBatSayHandle: BDB_DialogueChat not find!"));
			return;
		}

		// - Set NPC Talk Image
		SetBatDialogueImage(askerId, ImageType, bLeft);

		// - Set NPC Talk Msg
		AddStrToChatUI(m_BatScriptMsgQueue.front().MsgLst.front());
		ReSizeBaseBGUI();

		//SetDiaglogueDetectALL();

		gGlobal.pUIManager->RenderAll();
	}
}

VOID CBatScriptControl::OnBatChooseHandle(UInt8 askerType, UInt32 askerId, Int Opt, Boolean bLeft, const StringVector &OptMsg)
{
	// check first open or not?
	if ( !m_bOpen )
	{
		m_BatScriptMsgQueue.clear();
		m_CurrPage = 0;
	}

	Int size = (Int)m_BatScriptMsgQueue.size();
	if ( size > 0 )
	{
		StringVector szFinal = OptMsg;
		PushMsg(BATSCRIPTMSG_CHOOSE, szFinal, askerId, askerType);
	}
	else
	{
		OpenBatDialog();
		//
		UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("BDB_DialogueBox");
		if ( pPanelEx )
		{
			pPanelEx->SetEnable(true);

			// - Set NPC Talk Image
			SetBatDialogueImage(askerId, askerType);

			StringVector strlist = OptMsg;

			if ( (Int)strlist.size() == (Opt + 1) )
				AddStrToChatUI(strlist.back(), 16 - Opt);

			strlist.pop_back();

			AddOptionToSelectPopUpUI(strlist);
			ReSizeBaseBGUI();
		}

		gGlobal.pUIManager->RenderAll();
	}
}

VOID CBatScriptControl::OnBatSetDisEnableCmd(UInt32 disEnableCmd)
{
	m_dwDisEnableCmd = disEnableCmd;
}

Boolean CBatScriptControl::DoAndGate(UInt32 index)
{
	if ( (m_dwDisEnableCmd & index) == index)
		return BOOLEAN_TRUE;
	return BOOLEAN_FALSE;
}

Boolean CBatScriptControl::DisEnableCmd()
{
	if ( m_dwDisEnableCmd == 0 ) 
		return BOOLEAN_FALSE;
	return BOOLEAN_TRUE;
}

Boolean CBatScriptControl::NeedToBlockCmd(Int cmd)
{
	if ( m_dwDisEnableCmd == BATBLOCK_ALLCMD )
		return BOOLEAN_TRUE;

	switch ( cmd )
	{
	case BATCMD_ATTACK:
		return DoAndGate(BATBLOCK_ATTACK);
	case BATCMD_SKILL:
		return DoAndGate(BATBLOCK_SKILL);
	case BATCMD_SPECIAL:
		return DoAndGate(BATBLOCK_SPECIAL);
	case BATCMD_ITEM:
		return DoAndGate(BATBLOCK_ITEM);
	case BATCMD_DEFEND:
		return DoAndGate(BATBLOCK_DEFEND);
	case BATCMD_FRIEND:
		return DoAndGate(BATBLOCK_FRIEND);
	case BATCMD_PROTECT:
		return DoAndGate(BATBLOCK_PROTECT);
	case BATCMD_CATCH:
		return DoAndGate(BATBLOCK_CATCH);
	case BATCMD_HELP:
		return DoAndGate(BATBLOCK_HELP);
	case BATCMD_ESCAPE:
		return DoAndGate(BATBLOCK_ESCAPE);
	}
	
	//BATCMD_AUTO,				// auto command
	//BATCMD_EVENT,				// similar as defend cmd, but do some battle event
	
	return BOOLEAN_FALSE;
}

VOID CBatScriptControl::OnBatPushEndCmdMsgHandle()
{
	if (m_bOpen)
	{
		ResetPage();
		//if ( gGlobal.g_pBattleControl )
		//{
		//	gGlobal.g_pBattleControl->BatNextTalk();
		//	return;
		//}
	}

	Int size = (Int)m_BatScriptMsgQueue.size();
	if ( size > 0 )
	{
		PushMsg(BATSCRIPTMSG_ENDCMD, _T(""));
	}
}

VOID CBatScriptControl::OnBatPlayerChooseResult(Int Opt)
{
	ResetPage();

	if ( gGlobal.g_pBattleControl )
		gGlobal.g_pBattleControl->BatChooseFinish(Opt);
}
