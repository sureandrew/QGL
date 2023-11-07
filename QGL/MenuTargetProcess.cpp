//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuTargetProcess.h"
#include "MenuFriendProcess.h"
//-- Library
#include "Global.h"
#include "Configure.h"
#include "UI.h"
#include "QGLApplication.h"
#include "Common/CallLuaFunc.h"
#include "LuaPush.h"
#include "QGLWindow.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/Messenger.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "Proxy/Service/MessageService.h"

CMenuTargetProcess::CMenuTargetProcess()
{	
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_JOIN_PARTY] = "MSG_TARGET_POPUP_JOIN_PARTY";
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_INVITE_PARTY] = "MSG_TARGET_POPUP_INVITE_PARTY";
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_VIEW_INFO] = "MSG_TARGET_POPUP_VIEW_INFO";
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_PRIVATE_CALL] = "MSG_TARGET_POPUP_PRIVATE_CALL";
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_INVITE_FRIEND] = "MSG_TARGET_POPUP_INVITE_FRIEND";
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_COPY_NAME] = "MSG_TARGET_POPUP_COPY_NAME";
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_ADD_BLOCK_LIST] = "MSG_TARGET_POPUP_ADD_BLOCK_LIST";
	m_TargetPopUpCommand[TARGET_POPUP_COMMAND_CALL_GM] = "MSG_TARGET_POPUP_CALL_GM";

	m_char_id = 0;
	m_enablePK = false;
	m_level = 0;
	m_sexClass = 0;
	m_friendly = 0;

	m_faction = 0;
	m_hasParty = false;
	m_guild_uid = 0;

}

CMenuTargetProcess::~CMenuTargetProcess()
{
	for (UInt i = 0; i < MAX_TARGET_POPUP_COMMAND; i ++)
	{
		m_TargetPopUpCommand[i].Empty();
	}
	m_CurrTextInPopUp.clear();

}

VOID CMenuTargetProcess::PopUpFunctionCall()
{
	Int barIndex = -1;

	UIPopUpMenu * pPopUp = gGlobal.pUIManager->FindUIPopUpMenu("TargetPopup");
	if (pPopUp)
		barIndex = pPopUp->GetBarPos();
	if (barIndex < 0 || barIndex >= (Int)m_CurrTextInPopUp.size())
		return;

	Int funcIndex = 0;

	UInt i;
	for (i = 0; i < MAX_TARGET_POPUP_COMMAND; i ++)
	{
		if (m_TargetPopUpCommand[i] == m_CurrTextInPopUp[barIndex])
		{
			funcIndex = i;
			break;
		}
	}

	switch(i)
	{
	case TARGET_POPUP_COMMAND_JOIN_PARTY:
		QGLApplication::GetInstance().JoinParty(m_char_id);
		break;
	case TARGET_POPUP_COMMAND_INVITE_PARTY:
		QGLApplication::GetInstance().InviteParty(m_char_id);
		break;
	case TARGET_POPUP_COMMAND_VIEW_INFO:
		{
			ShowDetail();
			MessageService_Proxy::GetTargetInfo(gGlobal.pPeerProxy, gGlobal.g_context, m_char_id);
		}
		break;
	case TARGET_POPUP_COMMAND_PRIVATE_CALL:
		{
			UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
			if (pEditBox)
			{
				String Temp;
				Temp.Format( _T("/p %s "), m_char_name.c_str() );
				pEditBox->SetText(Temp);
				pEditBox->SetFocus(true);
				pEditBox->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
			}
		}
		break;
	case TARGET_POPUP_COMMAND_INVITE_FRIEND:
		break;
	case TARGET_POPUP_COMMAND_COPY_NAME:
		{
			UInt32		Len = ((UInt32)m_char_name.GetLength() + 1) * 2;
			HANDLE		hMem;
			LPVOID		lpData;
			OpenClipboard(QGLApplication::GetInstance().GetWindow().GetWindow());
			EmptyClipboard();

			hMem = GlobalAlloc(GHND, Len);

			lpData = (LPSTR)GlobalLock(hMem);
			memcpy(lpData, m_char_name.c_str(), Len);
			GlobalUnlock(hMem);
			SetClipboardData(CF_UNICODETEXT, hMem);

			CloseClipboard();

		}
		break;
	case TARGET_POPUP_COMMAND_ADD_BLOCK_LIST:
		break;
	case TARGET_POPUP_COMMAND_CALL_GM:
		break;
	}

}

VOID CMenuTargetProcess::HideTargetMenu()
{
	UIPanelEx * pPanelEx = gGlobal.pUIManager->FindUIPanelEx("TargetPopUpPanel");
	if (pPanelEx)
	{
		pPanelEx->SetEnable(FALSE);
	}
	ShowTargetDetail(NULL);

	UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("TargetDetailInfoPanel");
	if (pPanel)
		pPanel->SetEnable(FALSE);
}

bool CMenuTargetProcess::IsOpenDetail()
{
	UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("TargetDetailInfoPanel");
	return pPanel != NULL && pPanel->IsEnable();
}

VOID CMenuTargetProcess::ShowDetail()
{
	UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("TargetDetailInfoPanel");
	if (pPanel == NULL)
	{
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile("Data\\TargetInfoPanel.xml", gGlobal.pUIManager);
		//UINameList::GetInstance().LoadUIFile(gGlobal.pUIManager, "TargetInfoPanel");

		pPanel = gGlobal.pUIManager->FindUIPanel("TargetDetailInfoPanel");
		if (pPanel == NULL)
			return;

		pPanel->SetEnable(false);
	}

	UIAniIcon * pAni = gGlobal.pUIManager->FindUIAniIcon("TargetInfo3DFace");
	if (pAni)
		pAni->SetAnimationData(&m_Entity);
	UIButton * pBu = gGlobal.pUIManager->FindUIButton("TargetInfoFind");
	if (pBu)
		pBu->SetDisableState();
	pBu = gGlobal.pUIManager->FindUIButton("TargetInfoTempFriend");
	if (pBu)
		pBu->SetDisableState();
	pBu = gGlobal.pUIManager->FindUIButton("TargetInfoFriend");
	if (pBu)
		pBu->SetDisableState();

	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoPanelName");
	if (pText)
	{
		pText->SetText(m_char_name);
		pText->Redraw();
	}
	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoID");
	if (pText)
	{
		String ID;
		ID.Format(_T("%d"), m_char_id);
		pText->SetText(ID);
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoFaction");
	if (pText)
	{
		String tempRing;

		switch ( m_faction)
		{
		case 1:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SIXFANDOOR"));
			break;
		case 2:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TWELVEDOCK"));
			break;
		case 3:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_HOLY"));
			break;
		case 4:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SILVERCASINO"));
			break;
		case 5:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TONGDOOR"));
			break;
		case 6:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_10THOUSAND"));
			break;
		case 7:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GREENCLOTH"));
			break;
		case 8:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GHOSTAREA"));
			break;
		default:
			tempRing =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE"));
			break;

		}

		pText->SetText(tempRing);
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoLevel");
	if (pText)
	{
		String Lv;
		Lv.Format(_T("%d"), m_level);
		pText->SetText(Lv);
		pText->Redraw();
	}
	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoClass");
	if (pText)
	{
		String Text;
		switch(m_sexClass)
		{
		case 1: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_1")); break;
		case 2: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_2")); break;
		case 3: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_3")); break;
		case 4: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_4")); break;
		case 5: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_5")); break;
		case 6: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_6")); break;
		case 7: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_7")); break;
		case 8: Text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_8")); break;
		}
		pText->SetText(Text);
		pText->Redraw();
	}
	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoPKEnable");
	if (pText)
	{
		if (m_enablePK)
			pText->SetText(gGlobal.GetStringTable()->Get(_T("MSG_OPEN")));
		else
			pText->SetText(gGlobal.GetStringTable()->Get(_T("MSG_CLOSE")));
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoGuild");
	if (pText)
	{
		if (m_guild_uid >0)
			pText->SetText(m_guild_name.c_str());
		else
			pText->SetText(gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
		pText->Redraw();
	}
	//find back whether they have relation
	CMessenger * pMsger = gGlobal.g_pMenuFriendProcess->FindMessenger(m_char_id);
	CharTempFriendDataMap::iterator Itr = gGlobal.g_pMenuFriendProcess->m_TempFriendList.find(m_char_id);
	if (pMsger)
	{
		UIButton * pBu = gGlobal.pUIManager->FindUIButton("TargetInfoAddFriend");
		if (pBu)
			pBu->SetEnable(false);
		pBu = gGlobal.pUIManager->FindUIButton("TargetInfoChangeGrp");
		if (pBu)
			pBu->SetEnable(true);
		m_friendly = pMsger->GetFriend_friendly();

		pBu = gGlobal.pUIManager->FindUIButton("TargetInfoBreak");
		if (pBu)
		{
			pBu->SetNormalState();
		}

		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoRelation");
		if (pText)
		{
			if (pMsger->GetFriend_relation() == 0)
			{
				pText->SetText(gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
				pText->Redraw();
			}
			else
			{
				/*TCHAR *buf = SafeAllocate(TCHAR, pMsger->GetFriend_relation().size() + 1);
				TCHAR *bufOrg = buf;
				_tcscpy(buf, pMsger->GetFriend_relation().c_str());
				TCHAR *valueDelim = _T(",");
				_tcstok(buf, valueDelim);

				UInt32 ID = 999;
				while (true)
				{
					if (buf)
					{
						UInt32 currentID = _ttoi(buf);
						if (currentID == ID)
							break;
						ID = currentID;
						String nameText;
						nameText.Format(_T("MSG_CHAT_RELATION%d"),ID);

						pText->SetText(gGlobal.GetStringTable()->Get(nameText.c_str()));
						pText->Redraw();
						_tcstok(buf, valueDelim);
					}
					else
						break;
				}

				SafeDeallocate(bufOrg);*/
				//UInt16 i = 1;
				UInt16 ID = FRIEND_RELATION_FRIEND;
				TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CMenuTargetProcess::ShowDetail Relation %d"),pMsger->GetFriend_relation());
				while (true)
				{
					if (pMsger->IsRelation(ID))
					{
						String nameText;
						nameText.Format(_T("MSG_CHAT_RELATION%d"),ID);
						pText->SetText(gGlobal.GetStringTable()->Get(nameText.c_str()));
						pText->Redraw();
					}
					if (ID >= FRIEND_RELATION_COUNT)
						break;
					//i <<= 1;					
					++ ID;
				}
			}
		}
	}
	else if (Itr != gGlobal.g_pMenuFriendProcess->m_TempFriendList.end())
	{
		UIButton * pBu = gGlobal.pUIManager->FindUIButton("TargetInfoAddFriend");
		if (pBu)
			pBu->SetEnable(false);
		pBu = gGlobal.pUIManager->FindUIButton("TargetInfoChangeGrp");
		if (pBu)
			pBu->SetEnable(true);
		m_friendly = 0;

		pBu = gGlobal.pUIManager->FindUIButton("TargetInfoBreak");
		if (pBu)
		{
			pBu->SetNormalState();
		}

		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoRelation");
		if (pText)
		{	
			pText->SetText(gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
			pText->Redraw();				
		}
	}
	else
	{
		UIButton * pBu = gGlobal.pUIManager->FindUIButton("TargetInfoAddFriend");
		if (pBu)
			pBu->SetEnable(true);
		pBu = gGlobal.pUIManager->FindUIButton("TargetInfoChangeGrp");
		if (pBu)
			pBu->SetEnable(false);
		m_friendly = 0;

		pBu = gGlobal.pUIManager->FindUIButton("TargetInfoBreak");
		if (pBu)
		{
			pBu->SetDisableState();
		}
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoFriendship");
		if (pText)
		{
			pText->SetText(gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
			pText->Redraw();
		}

		pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoRelation");
		if (pText)
		{	
			pText->SetText(gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
			pText->Redraw();				
		}
	}
	//m_relation = 0;
	//m_friendly = 0;

	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoFriendship");
	if (pText)
	{
		String buf;
		buf.Format(_T("%d"), m_friendly);
		pText->SetText(buf.c_str());
		pText->Redraw();
	}
	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("TargetInfoAbout");
	if (pChat)
	{
		pChat->ClearAllRow();
		pChat->AddStringW(m_About.c_str());
		pChat->EndRow();
	}
	pText = gGlobal.pUIManager->FindUIStaticText("TargetInfoTitle");
	if (pText)
	{
		pText->SetText(m_title.c_str());
		pText->Redraw();
	}
	pPanel->SetEnable(true);
	gGlobal.pUIManager->SetPanel2Top(pPanel);
}

void CMenuTargetProcess::ResetTargetDetail()
{
	m_char_id = 0;
	m_char_name = _T("");

	m_enablePK = false;
	m_title = gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE"));
	m_title = _T("");
	m_level = 0;
	m_sexClass = 0;
	m_faction = 0;
	m_hasParty = false;

	m_Entity.InitEntPicInfo();
	m_Entity.SetHighLight(false);

	m_About = gGlobal.GetStringTable()->Get(_T("MSG_TARGET_UPDATING"));
	m_relations.clear();
	m_guild = _T("");
	m_gang = _T("");
	m_friendly = 0;
	m_guild_uid = 0;
	m_guild_name = gGlobal.GetStringTable()->Get(_T("MSG_TARGET_UPDATING"));
}
void CMenuTargetProcess::SetFriendDetail(CCharacterControl * pInfo)
{
	if (pInfo == NULL || pInfo->GetCharNetObject() == NULL)
		return;

	ResetTargetDetail();
	m_char_id = pInfo->GetMapActorID();
	m_char_name = pInfo->GetCharName();

	m_enablePK = pInfo->GetCharNetObject()->GetEnablePK();
	m_title = pInfo->GetCharNetObject()->GetTitle();
	m_level = pInfo->GetCharNetObject()->GetLevel();
	m_sexClass = pInfo->GetCharNetObject()->GetSexClass();
	m_faction = pInfo->GetCharNetObject()->GetFaction();
	m_hasParty = pInfo->GetCharNetObject()->GetHasParty();

	//m_Entity = *pInfo->GetCharMainView();
	m_Entity = gGlobal.GetCharModel(m_sexClass, pInfo->GetCharNetObject()->GetHairStyle1(), pInfo->GetCharNetObject()->GetHairColor(), 
		pInfo->GetCharNetObject()->GetEq_cloth(), pInfo->GetCharNetObject()->GetEq_clothColor(), pInfo->GetCharNetObject()->GetEq_headBand(), 
		pInfo->GetCharNetObject()->GetEq_weapon());
	m_Entity.SetHighLight(false);
}
void CMenuTargetProcess::SetFriendDetail(CMessageCharacter * pInfo)
{
	m_About = pInfo->GetFriend_about();
	RemoveLink(m_About);
	m_char_id = pInfo->GetChar_id();
	m_char_name = pInfo->GetNickName();
	//m_relation = Itr->second->GetFriend_relation();
	m_enablePK = pInfo->GetEnablePK();
	m_title = pInfo->GetTitle();
	m_level = pInfo->GetLevel();
	m_sexClass = pInfo->GetSexClass();

	m_guild_uid = pInfo->GetGuild_uid();
	m_guild_name = pInfo->GetGuild_name();
	//String	m_guild;
	//String	m_gang;
	
	m_faction = pInfo->GetFaction();
	m_hasParty = FALSE;

	UInt8 SexClass = pInfo->GetSexClass();;
	UInt32 eqCloth = pInfo->GetOrg_eq_cloth();
	UInt16 eqClothColor = pInfo->GetOrg_eq_clothColor();
	UInt16 hairStyle = pInfo->GetOrg_hairStyle1();
	UInt16 hairColor = pInfo->GetOrg_hairColor();

	if (pInfo->GetHairStyle1() > 0)
		hairStyle = pInfo->GetHairStyle1();
	if (pInfo->GetHairColor() > 0)
		hairColor = pInfo->GetHairColor();
	if (pInfo->GetEq_cloth() > 0)
	{
		eqCloth = pInfo->GetEq_cloth();
		eqClothColor = pInfo->GetEq_clothColor();
	}

	m_Entity = gGlobal.GetCharModel(SexClass, hairStyle, hairColor, eqCloth, eqClothColor, pInfo->GetEq_headBand(), pInfo->GetEq_weapon() );
}

VOID CMenuTargetProcess::ShowTargetDetail(CCharacterControl * pTargetCtrl)
{
	UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("TargetInfo");

	if (pPanel != NULL)
	{
		if (pTargetCtrl == NULL)
		{
			pPanel->SetEnable(FALSE);
		}
		else
		{
			//m_TargetInfoMapControl = *pTargetCtrl;
			if (pTargetCtrl->GetCharNetObject() == NULL)
				return;

			SetFriendDetail(pTargetCtrl);

			UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("TargetInfo");


			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon("TargetInfoFace");
			//UIImage * pImage = gGlobal.pUIManager->FindUIImage("TargetInfoFace");
			UIStaticText * pTextName = gGlobal.pUIManager->FindUIStaticText("TargetInfoName");
			UIStaticText * pTextID = gGlobal.pUIManager->FindUIStaticText("TargetInfoUID");
			UIImage * pImage = gGlobal.pUIManager->FindUIImage("TargetInfoPK");

			if (pAniIcon != NULL && pTextName != NULL && pTextID != NULL && pImage != NULL)
			{
				pTextName->SetText(m_char_name);
				pTextName->Redraw();
				String ID;
				ID.Format(_T("%d"), m_char_id);
				pTextID->SetText(ID.c_str());
				pTextID->Redraw();

				String imageFile;
				imageFile.Format(_T("faceS%05ia.ent"), m_sexClass);
				pAniIcon->ClearAniData();
				pAniIcon->SetAnimationData(imageFile.c_str());
				/*pImage->SetImageFileA(imageFile.c_str(), 0, 0, 0, 0);*/

				if (m_sexClass)
					pImage->SetEnable(true);
				else
					pImage->SetEnable(false);
				pPanel->SetEnable(TRUE);
			}
			else
				pPanel->SetEnable(FALSE);
		}
		gGlobal.pUIManager->AddDirtyUIItem(pPanel);
	}
}


VOID CMenuTargetProcess::ShowTargetPopUp()
{

	m_CurrTextInPopUp.clear();
	if (gGlobal.g_pParty && gGlobal.g_pParty->IsLeader())
		m_CurrTextInPopUp.push_back(m_TargetPopUpCommand[TARGET_POPUP_COMMAND_INVITE_PARTY]);

	if (m_hasParty && gGlobal.g_pParty == NULL)
		m_CurrTextInPopUp.push_back(m_TargetPopUpCommand[TARGET_POPUP_COMMAND_JOIN_PARTY]);

	m_CurrTextInPopUp.push_back(m_TargetPopUpCommand[TARGET_POPUP_COMMAND_VIEW_INFO]);
	m_CurrTextInPopUp.push_back(m_TargetPopUpCommand[TARGET_POPUP_COMMAND_PRIVATE_CALL]);
	m_CurrTextInPopUp.push_back(m_TargetPopUpCommand[TARGET_POPUP_COMMAND_COPY_NAME]);

	UIPanelEx * pPanel = gGlobal.pUIManager->FindUIPanelEx("TargetPopUpPanel");
	if (pPanel)
	{
		pPanel->SetPos(gGlobal.pUIManager->GetCursorX() + 5,gGlobal.pUIManager->GetCursorY() + 5);
		pPanel->ChangeCount((UInt32)m_CurrTextInPopUp.size());
		UIPopUpMenu * pPopUp = gGlobal.pUIManager->FindUIPopUpMenu("TargetPopup");
		if (pPopUp)
		{
			pPopUp->SetImage((UInt32)m_CurrTextInPopUp.size(), 12);
			pPopUp->FillImage();
			pPopUp->ClearData();
			for (Int i = 0; i < (Int) m_CurrTextInPopUp.size(); i++)
			{
				pPopUp->AddString(gGlobal.GetStringTable()->Get(String(m_CurrTextInPopUp[i].c_str()).c_str()), i, NULL);
			}
			pPopUp->Redraw();
		}
		pPanel->SetEnable(true);
	}
}

void CMenuTargetProcess::UpdateInfo()
{
	MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, m_char_id, String(), true, true);
}

void CMenuTargetProcess::FindCharNameFriend(PCSTRINGW Name)
{
	MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, 0, Name, true, true);
}


void CMenuTargetProcess::PrivateMsg()
{
	UIEditBox * pEditBox = gGlobal.pUIManager->GetUIThing<UIEditBox *>("EditLine");
	if (pEditBox)
	{
		String Temp;
		Temp.Format( _T("/p %s "), m_char_name.c_str() );
		pEditBox->SetText(Temp);
		pEditBox->End();
		pEditBox->SetFocus(true);
		pEditBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(pEditBox);
	}
}
void CMenuTargetProcess::AddToFriend()
{
	UInt16Vector relations;
	relations.push_back(FRIEND_RELATION_FRIEND);

	MessageService_Proxy::CreateMessenger(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_pCharacter->GetMapActorID(), m_char_id,
		relations, FRIEND_GROUP_ONE, 0, m_char_name);
}
void CMenuTargetProcess::ChangeGrp()
{
	gGlobal.g_pMenuFriendProcess->m_changeGrpTempID = m_char_id;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenChangeGroup", String());
	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("FriendChangeMsg");
	if (pText)
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_CHANGE_GROUP")), m_char_name.c_str());
		pText->SetText(text.c_str());
		pText->Redraw();
	}
}
void CMenuTargetProcess::BreakRelation()
{
	gGlobal.g_pMenuFriendProcess->SetMoveGroupTarget(m_char_id, FRIEND_GROUP_TEMP);
	//gGlobal.g_pMenuFriendProcess->CheckChangeGroup();
	String text;
	text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_MSG_BREAK_RELATION")), m_char_name.c_str());

	gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, text.c_str(), "FriendChat");
	UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
	if (pItem)
	{
		pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmDeleteFriend");
	}
	
}
void CMenuTargetProcess::BlackList()
{
	gGlobal.g_pMenuFriendProcess->SetMoveGroupTarget(m_char_id, FRIEND_GROUP_BLACK);
	gGlobal.g_pMenuFriendProcess->CheckChangeGroup();
}