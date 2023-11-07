//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuFriendProcess.h"
//-- Library
#include "Global.h"
#include "Configure.h"
#include "Proxy/Logic/Messenger.h"
#include "Proxy/Logic/Map.h"
#include "UI.h"
#include "QGLApplication.h"
#include "Common/CallLuaFunc.h"
#include "LuaPush.h"
#include "Proxy/Service/MessageService.h"
#include "CharacterControl.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "MenuTargetProcess.h"
#include "MenuSystemSetting.h"

UInt32 OnlineColor = 0xFFFFFFFF;
UInt32 OfflineColor = 0xFF888888;
CMenuFriendProcess::CMenuFriendProcess()
{
	m_Messengers = NULL;
	m_StartIndex = 0;
	m_CurrGroup = FRIEND_GROUP_ONE;
	m_TargetTalkingCharID = 0;
	m_pTargetMessager = NULL;
	m_ChangeGroup = 0;
	m_pOwnChar = NULL;
	m_bInit = false;
}

CMenuFriendProcess::~CMenuFriendProcess()
{
	m_TargetCharName.Empty();
	DeleteNetGroup(m_Messengers, BOOLEAN_TRUE);

	for (Int i = 0; i < MAX_FRIEND_GROUP_TYPE; i++)
	{
		m_MessengersList[i].clear();
	}

	for (CharMsgCharPtrMap::iterator Itr = m_MessageCharacters.begin(); Itr != m_MessageCharacters.end();
		++Itr)
	{
		CMessageCharacter * pMsgCharTmp = FindNetObject<CMessageCharacter>(Itr->second);
		if(pMsgCharTmp != NULL)
			DeleteNetObject(pMsgCharTmp);
	}
	m_TargetTalkingCharName.Empty();

	m_OpenFileName.Empty();

	m_TempFriendList.clear();
}

void CMenuFriendProcess::AddFriend(UInt8 group)
{
	UInt16Vector relations;
	relations.push_back(FRIEND_RELATION_FRIEND);

	MessageService_Proxy::CreateMessenger(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_pCharacter->GetMapActorID(), m_TargetCharID,
		relations, group, 0, m_TargetCharName);

}


void CMenuFriendProcess::InserTempFriend(UInt32 char_id, String NickName, BYTE state, bool force)
{
	if (char_id == gGlobal.g_pCharacter->GetMapActorID())
		return;

	if (!force && gGlobal.g_pMenuFriendProcess->FindMessenger(char_id) != NULL)
		return;

	CharTempFriendDataMap::iterator Itr = m_TempFriendList.find(char_id);
	if (Itr != m_TempFriendList.end())
	{
		Itr->second.name = NickName;
		Itr->second.state = state;
	}
	else
	{
		tempFriendData data;
		data.name = NickName;
		data.state = state;
		m_TempFriendList.insert(std::make_pair(char_id, data));
	}

	if (m_CurrGroup == FRIEND_GROUP_TEMP)
		RefreshMenu();
}
void CMenuFriendProcess::ConstructData()
{
	if (m_Messengers == NULL)
		return;

	for (Int i = 0; i < MAX_FRIEND_GROUP_TYPE; i++)
	{
		if (i == FRIEND_GROUP_TEMP)
			continue;
		m_MessengersList[i].clear();
	}

	for (Int i = 0; i < m_Messengers->GetCount(); i ++)
	{
		CMessenger * pMessager = CastNetObject<CMessenger>(m_Messengers->GetChild(i));
		if (pMessager && pMessager->GetFriend_group() < MAX_FRIEND_GROUP_TYPE)
		{
			CharTempFriendDataMap::iterator Itr = m_TempFriendList.find(pMessager->GetFriend_char_id());
			if (Itr != m_TempFriendList.end())
			{
				m_TempFriendList.erase(Itr);
			}

			MessengerKey key;
			CMessageCharacter * pInfo = FindInfo(pMessager->GetFriend_char_id());
			if (pInfo && pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE  && !m_bInit)
			{
				if (pMessager->IsRelation(FRIEND_RELATION_MASTER))
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_MASTER_ONLINE")), pInfo->GetNickName().c_str());
					gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
				}
				else if (pMessager->IsRelation(FRIEND_RELATION_STUDENT))
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_STUDENT_ONLINE")), pInfo->GetNickName().c_str());
					gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
				}
				else if (pMessager->IsRelation(FRIEND_RELATION_HUSBAND))
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_HUSBAND_ONLINE")), pInfo->GetNickName().c_str());
					gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
				}
				else if (pMessager->IsRelation(FRIEND_RELATION_WIFE))
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_WIFE_ONLINE")), pInfo->GetNickName().c_str());
					gGlobal.PrintMessage(text, CHANNELTYPE_NOTICE);
				}
			}
			if (pInfo)
				key.state = pInfo->GetFriend_status();
			key.friendly = pMessager->GetFriend_friendly();
			m_MessengersList[pMessager->GetFriend_group()].insert(std::make_pair(key, pMessager));
		}
	}

	RefreshMenu();
	
}

void CMenuFriendProcess::HideAllFriend()
{
	for (int i = 1; i <= MAX_FRIEND_IN_ONE_PAGE; i ++)
	{
		StringA name;
		name.Format("Friend%dIcon", i);
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
		if (pAniIcon)
		{
			pAniIcon->SetEnable(FALSE);
		}

		name.Format("Friend%dName", i);
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetEnable(FALSE);
		}

		name.Format("Friend%dAway", i);
		UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
		if (pIm)
		{
			pIm->SetEnable(FALSE);
		}
	}
}

void CMenuFriendProcess::ToPageTop()
{
	m_StartIndex = 0;
	RefreshMenu();
}
void CMenuFriendProcess::PageUp()
{
	if (m_StartIndex > 0 )
	{
		m_StartIndex --;
		RefreshMenu();
	}
}
void CMenuFriendProcess::PageDown()
{
	if (m_CurrGroup == FRIEND_GROUP_TEMP)
	{
		if (m_StartIndex + (UInt)MAX_FRIEND_IN_ONE_PAGE <= m_TempFriendList.size() )
		{
			m_StartIndex ++;
			RefreshMenu();
		}
	}
	else
	{
		if (m_StartIndex + (UInt)MAX_FRIEND_IN_ONE_PAGE <= m_MessengersList[m_CurrGroup].size() )
		{
			m_StartIndex ++;
			RefreshMenu();
		}
	}
}
void CMenuFriendProcess::ChangeGroup(Int index)
{
	if (index >= 0 && index < MAX_FRIEND_GROUP_TYPE)
	{
		m_CurrGroup = index;
		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("MessagerPanelScroll");
		if (scrollBar)
		{
			if (m_CurrGroup == FRIEND_GROUP_TEMP)
				scrollBar->SetNumCount(m_TempFriendList.size() + 1);
			else
				scrollBar->SetNumCount(m_MessengersList[m_CurrGroup].size() + 1);
		}
		ToPageTop();
	}
}

void CMenuFriendProcess::FriendScroll()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("MessagerPanelScroll");
	if (scrollBar)
	{
		UInt32 index = scrollBar->GetSelNum();

		if (m_CurrGroup == FRIEND_GROUP_TEMP)
		{
			if (index + (UInt)MAX_FRIEND_IN_ONE_PAGE > m_TempFriendList.size())
			{
				if (m_TempFriendList.size() + 1 > (UInt)MAX_FRIEND_IN_ONE_PAGE)
					index = m_TempFriendList.size() + 1 - (UInt)MAX_FRIEND_IN_ONE_PAGE;
				else
					index = 0;
			}
		}
		else
		{
			if (index + (UInt)MAX_FRIEND_IN_ONE_PAGE > m_MessengersList[m_CurrGroup].size())
			{
				if (m_MessengersList[m_CurrGroup].size() + 1 > (UInt)MAX_FRIEND_IN_ONE_PAGE)
					index = m_MessengersList[m_CurrGroup].size() + 1 - (UInt)MAX_FRIEND_IN_ONE_PAGE;
				else
					index = 0;
			}
		}

		if (index != m_StartIndex)
		{
			m_StartIndex =  index;
			RefreshMenu();
		}
	}
}

void CMenuFriendProcess::FriendChatScroll()
{
	UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("FriendChatScroll");
	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatMsg");
	if (scrollBar && pChat)
	{
		pChat->SetRow( scrollBar->GetSelNum(), pChat->RowListCount());
		gGlobal.pUIManager->AddDirtyUIItem(pChat);
	}
}

void CMenuFriendProcess::FriendHisScroll()
{
	UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("FriendChatHisScroll");
	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatHisMsg");
	if (scrollBar && pChat)
	{
		pChat->SetRow( scrollBar->GetSelNum(), pChat->RowListCount());
		gGlobal.pUIManager->AddDirtyUIItem(pChat);
	}
}

void CMenuFriendProcess::SetInfo(UInt32 friend_char_id, int index, UINT16 relation,UInt friendly, String charName, BYTE state)
{
	CMessageCharacter * pInfo = FindInfo(friend_char_id);
	if (pInfo)
	{
		StringA name;
		name.Format("Friend%dIcon", index);
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
		if (pAniIcon)
		{
			String file;
			file.Format(_T("facechatS%05da.ent"), pInfo->GetFriend_icon());
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(file.c_str());

			if (!pAniIcon->GetHasAni())
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
					_FE("friend info error got friend info buf friend icon not found friend icon id: %d char_id %d"), 
					pInfo->GetFriend_icon(), pInfo->GetChar_id());
			}
			pAniIcon->SetEnable(TRUE);
		}

		name.Format("Friend%dName", index);
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetText(pInfo->GetNickName().c_str());
			if (state == FRIENC_CHAT_STATUS_ONLINE ||
				state == FRIENC_CHAT_STATUS_ONLINE_AWAY)
			{
				if((relation & (1<< FRIEND_RELATION_MASTER)) > 0 || (relation & (1<< FRIEND_RELATION_STUDENT)) > 0)
					pText->SetFontColor(TextColor[ColorIndex_Blue]);
				else if((relation & (1<< FRIEND_RELATION_HUSBAND)) > 0 || (relation & (1<< FRIEND_RELATION_WIFE)) > 0)
					pText->SetFontColor(TextColor[ColorIndex_Violet]);
				else if((relation & (1<< FRIEND_RELATION_SIBLING)) > 0)
					pText->SetFontColor(TextColor[ColorIndex_Orange]);
				else if (friendly >= 10)
					pText->SetFontColor(TextColor[ColorIndex_Green]);
				else
					pText->SetFontColor(OnlineColor);
			}
			else
			{
				pText->SetFontColor(OfflineColor);
			}

			pText->Redraw();
			pText->SetEnable(TRUE);
		}

		name.Format("Friend%dAway", index);
		UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
		if (pIm)
		{
			if (pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE_AWAY)
			{
				pIm->SetEnable(true);
			}
			else
				pIm->SetEnable(false);
		}
	}
	else
	{
		StringA name;
		name.Format("Friend%dIcon", index);
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
		if (pAniIcon)
		{
			String file;
			file.Format(_T("facechatS%05da.ent"), 0);
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(file.c_str());
			pAniIcon->SetEnable(TRUE);
		}

		name.Format("Friend%dName", index);
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetText(charName.c_str());
			
			if (state == FRIENC_CHAT_STATUS_ONLINE ||
				state == FRIENC_CHAT_STATUS_ONLINE_AWAY)
			{
				if (friendly >= 10)
					pText->SetFontColor(TextColor[ColorIndex_Green]);
				else
					pText->SetFontColor(OnlineColor);
			}
			else
			{
				pText->SetFontColor(OfflineColor);
			}
			
			

			pText->Redraw();
			pText->SetEnable(TRUE);
		}

		name.Format("Friend%dAway", index);
		UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
		if (pIm)
		{
			pIm->SetEnable(false);
		}
	}
}

void CMenuFriendProcess::FindOwnCharacter()
{
	if (gGlobal.g_pCharacter)
	{
		CharMsgCharPtrMap::iterator Itr = m_MessageCharacters.find(gGlobal.g_pCharacter->GetMapActorID());
		if (Itr != m_MessageCharacters.end())
		{
			m_pOwnChar = FindNetObject<CMessageCharacter>(Itr->second);
		}
	}
}

void CMenuFriendProcess::RefreshMenu()
{
	if (m_CurrGroup >= MAX_FRIEND_GROUP_TYPE)
		return;

	HideAllFriend();

	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar)
		{
			UIButton* pBtn =NULL,*pBtnDisabled=NULL;
			if (pChar->GetLevel() < 50){
				pBtn = gGlobal.pUIManager->FindUIButton("OpenAsYounger");
				pBtnDisabled = gGlobal.pUIManager->FindUIButton("OpenAdElder");
			}
			else{
				pBtn = gGlobal.pUIManager->FindUIButton("OpenAdElder");
				pBtnDisabled = gGlobal.pUIManager->FindUIButton("OpenAsYounger");
			}
			if (NULL != pBtn && NULL != pBtnDisabled) {
				if (!IsBattle()) {
					pBtn->SetState(_ButtonNormal);
					pBtn->SetNotDetected(false);
				}
				else{
					pBtn->SetState(_ButtonClick);
					pBtn->SetNotDetected(true);
				}
				pBtnDisabled->SetState(_ButtonClick);
				pBtnDisabled->SetNotDetected(true);
			}
		}
	}
	Int startIndex = 0;
	if (m_StartIndex == 0 && m_CurrGroup != FRIEND_GROUP_TEMP && m_CurrGroup != FRIEND_GROUP_BLACK)
	{
		StringA name;
		name = "Friend1Icon";
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon(name.c_str());
		if (pAniIcon)
		{
			String file;
			file.Format(_T("facechatS%05da.ent"), m_pOwnChar->GetFriend_icon());
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(file.c_str());
			pAniIcon->SetEnable(TRUE);
		}
		
		name = "Friend1Name";
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetText(gGlobal.g_pCharacter->GetCharName().c_str());
			pText->SetFontColor(OnlineColor);
			pText->Redraw();
			pText->SetEnable(TRUE);
		}
		name = "Friend1Away";
		UIImage * pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
		if (pIm)
		{
			if (m_pOwnChar->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE_AWAY)
			{
				pIm->SetEnable(true);
			}
			else
				pIm->SetEnable(false);
		}
		startIndex ++;
	}

	if (m_CurrGroup == FRIEND_GROUP_TEMP)
	{
		CharTempFriendDataMap::iterator Itr = m_TempFriendList.begin();
		for (Int i = 0; i < m_StartIndex && Itr != m_TempFriendList.end(); ++i, ++Itr)
		{
		}

		for (Int i = startIndex; i < MAX_FRIEND_IN_ONE_PAGE && Itr != m_TempFriendList.end(); ++i, ++Itr)
		{	
			if (i + 1 > MAX_FRIEND_IN_ONE_PAGE)
				break;

			SetInfo(Itr->first, i + 1, 0, 0,Itr->second.name, Itr->second.state);
		}
	}
	else
	{
		std::multimap<MessengerKey, CMessenger *>::iterator Itr = m_MessengersList[m_CurrGroup].begin();

		for (Int i = 0; i < m_StartIndex - 1 && Itr != m_MessengersList[m_CurrGroup].end(); ++i, ++Itr)
		{
		}

		for (Int i = startIndex; i < MAX_FRIEND_IN_ONE_PAGE && Itr != m_MessengersList[m_CurrGroup].end(); ++i, ++Itr)
		{	
			if (i + 1 > MAX_FRIEND_IN_ONE_PAGE)
				break;

			CMessageCharacter * pInfo = FindInfo(Itr->second->GetFriend_char_id());
			if (pInfo)
				SetInfo(Itr->second->GetFriend_char_id(), i + 1, Itr->second->GetFriend_relation(), Itr->second->GetFriend_friendly(), String(), pInfo->GetFriend_status());
			else
				SetInfo(Itr->second->GetFriend_char_id(), i + 1, Itr->second->GetFriend_relation(), Itr->second->GetFriend_friendly(), String(), 0);
		}
	}
}

void CMenuFriendProcess::OpenFriendChat(int index)
{
	if (index == 0 && m_StartIndex == 0 &&
		m_CurrGroup != FRIEND_GROUP_TEMP && m_CurrGroup != FRIEND_GROUP_BLACK)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSelfSetting", String());
		return;
	}

	if (m_CurrGroup >= MAX_FRIEND_GROUP_TYPE)
		return;

	if (m_CurrGroup == FRIEND_GROUP_TEMP )
	{
		if (index < 0 ||
			(UInt)index + m_StartIndex >= m_TempFriendList.size())
			return;
	}
	else
	{
		if (index < 0 ||
			(UInt)index + m_StartIndex > m_MessengersList[m_CurrGroup].size())
			return;
	}
	
	if (m_CurrGroup != FRIEND_GROUP_TEMP && m_CurrGroup != FRIEND_GROUP_BLACK)
	{
		index --;
	}
	
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenFriendMsgPanel", String());

	if (m_CurrGroup == FRIEND_GROUP_TEMP)
	{
		CharTempFriendDataMap::iterator Itr = m_TempFriendList.begin();
		for (Int i = 0; i < index + m_StartIndex && Itr != m_TempFriendList.end(); ++i, ++Itr)
		{
		}

		if (Itr != m_TempFriendList.end())
		{	
			m_TargetTalkingCharID = Itr->first;
			m_TargetTalkingCharName = Itr->second.name;
			RefreshChatMenu();
		}
	}
	else
	{
		std::multimap<MessengerKey, CMessenger *>::iterator Itr = m_MessengersList[m_CurrGroup].begin();
		for (int i = 0; i < index + m_StartIndex; i++)
		{
			Itr ++;

		}
		if (Itr != m_MessengersList[m_CurrGroup].end())
		{
			m_TargetTalkingCharID = Itr->second->GetFriend_char_id();
			CMessageCharacter * pInfo = FindInfo(m_TargetTalkingCharID);
			if (pInfo)
			{
				m_TargetTalkingCharName = pInfo->GetNickName();
				RefreshChatMenu();
			}
		}
	}

	{
		String temp;

		TCHAR szBuffer[_MAX_PATH];
		//::GetCurrentDirectory(_MAX_PATH, szBuffer);
		::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
		*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

		temp.Format(_T("%s\\FriendHistory"), szBuffer);

		FILE* pFile = NULL;

		m_OpenFileName.Format(_T("%s\\%s%s.his"), temp.c_str(), gGlobal.g_pCharacter->GetCharName().c_str(), m_TargetTalkingCharName.c_str());
		pFile = _tfopen(m_OpenFileName.c_str(), _T("rb"));
		ULONGLONG size = 0;
		if (pFile)
		{
			fseek (pFile, 0, SEEK_END);
			size =	ftell (pFile);
			fseek (pFile, 0, SEEK_SET);
			TCHAR * pChar = SafeAllocate(TCHAR, (Reuben::System::Size)size + 1);

			ZeroMemory(pChar, (size_t)(size + 1) * sizeof(TCHAR));
			fread(pChar, (size_t)size,1,  pFile);

			String realText;
			if (size / 2 > 200)
			{
				realText = pChar  + size / 2 - 200;
				size_t pos = realText.find(_T("\\r#W"));
				if (pos != std::string::npos)
				{
					realText = realText.substr(pos + 2);
				}
			}
			else
				realText = pChar;
			
			String finalText;
			String tempText;
			PCSTRING pSay = gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_CHAT_SAY"))+2;
			while (!realText.IsEmpty())
			{
				size_t pos = realText.find(_T("\\r#W"));
				tempText = realText.substr(23,pos-21);
				realText = realText.substr(pos+4);
				pos = tempText.find(_T("\\r"));
				tempText.Insert(pos,pSay);
				finalText += tempText;
			}

			UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatMsg");
			if (pChat)
			{
				pChat->ClearAllRow();
				pChat->AddStringW(finalText.c_str());
				pChat->EndRow();

				UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("FriendChatScroll");
				if (scrollBar)
				{
					scrollBar->SetNumCount(pChat->RowListCount());
					scrollBar->SetSelNum(pChat->RowListCount() - 1);
				}
			}


			SafeDeallocate(pChar);

			fclose(pFile);
		}
	}

}

void CMenuFriendProcess::ClearHistory()
{
	FILE * pFile = _tfopen(m_OpenFileName.c_str(),  _T("wb"));
	if (pFile)
		fclose(pFile);

}
void CMenuFriendProcess::ConfirmChangeToGroup(int grpIndex)
{
	SetMoveGroupTarget(m_changeGrpTempID, grpIndex);
	if (m_pTargetMessager == NULL)
	{
		if (m_ChangeGroup == FRIEND_GROUP_BLACK)
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_MSG_TO_BLACKLIST")), m_TargetCharName);
			gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, text.c_str(), "FriendChat");
			UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
			if (pItem)
			{
				pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmChangeGroup");
			}
		}
		else
			MoveGroupImm();
	}
	else
		CheckChangeGroup();
}
void CMenuFriendProcess::RefreshPopUp(int memInx)
{
	if (!gConfigure.uNetwork)
		return;

	String Name;
	UInt32 char_id = 0;
	String About;
	UInt8 line = 0;
	UInt16 chatIcon = 0;

	if (memInx  + m_StartIndex == 0 &&
		m_CurrGroup != FRIEND_GROUP_TEMP && m_CurrGroup != FRIEND_GROUP_BLACK)
	{
		Name = gGlobal.g_pCharacter->GetCharName();
		char_id = gGlobal.g_pCharacter->GetMapActorID();
		About = m_pOwnChar->GetFriend_about();
		RemoveLink(About);
		chatIcon = m_pOwnChar->GetFriend_icon();
		if (gGlobal.g_pMap)
			line = gGlobal.g_pMap->GetLine_id();
	}
	else
	{
		if (m_CurrGroup != FRIEND_GROUP_TEMP && m_CurrGroup != FRIEND_GROUP_BLACK)
			memInx -- ;

		if (m_CurrGroup == FRIEND_GROUP_TEMP)
		{
			CharTempFriendDataMap::iterator Itr = m_TempFriendList.begin();
			for (Int i = 0; i < memInx + m_StartIndex && Itr != m_TempFriendList.end(); ++i, ++Itr)
			{
			}

			if (Itr != m_TempFriendList.end())
			{	
				Name = Itr->second.name;
				char_id = Itr->first;
				CMessageCharacter * pInfo = FindInfo(Itr->first);
				if (pInfo)
				{
					Name = pInfo->GetNickName();
					char_id = pInfo->GetChar_id();
					About = pInfo->GetFriend_about();
					RemoveLink(About);
					chatIcon = pInfo->GetFriend_icon();
					if (pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE ||
						pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE_AWAY)
					{
						line = pInfo->GetLine_id();
					}
					else
						line = 0;

				} 
			}
		}
		else
		{
			std::multimap<MessengerKey, CMessenger *>::iterator Itr = m_MessengersList[m_CurrGroup].begin();
			for (Int i = 0; (i < m_StartIndex || i - m_StartIndex < MAX_FRIEND_IN_ONE_PAGE) && Itr != m_MessengersList[m_CurrGroup].end(); ++i, ++Itr)
			{
				if (i == memInx + m_StartIndex)
				{
					CMessageCharacter * pInfo = FindInfo(Itr->second->GetFriend_char_id());
					if (pInfo)
					{
						Name = pInfo->GetNickName();
						char_id = Itr->second->GetFriend_char_id();
						About = pInfo->GetFriend_about();
						RemoveLink(About);
						chatIcon = pInfo->GetFriend_icon();
						if (pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE ||
							pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE_AWAY)
						{
							line = pInfo->GetLine_id();
						}
						else
							line = 0;

					} 
					break;
				}
			}
		}
	}
	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("FriendChatPopUpName");
	if (pText)
	{
		String text;
		text.Format(_T("%s(%d)"), Name.c_str(), char_id);
		pText->SetText(text.c_str());
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("FriendChatPopUpLine");
	if (pText)
	{
		String msg;
		if (line > 0)
		{
			msg.Format(_T("MSG_INTERFACE_LINE_%d"), line);
			pText->SetText(gGlobal.GetStringTable()->Get(msg.c_str()));
		}
		else
			pText->SetText(_T(""));
		pText->Redraw();
	}

	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatPopUpMsg");
	if (pChat)
	{
		pChat->ClearAllRow();
		pChat->AddStringW(About.c_str());
		pChat->EndRow();
	}

	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("FriendChatPopUpIcon");
	if (pAniIcon)
	{
		String file;
		file.Format(_T("facechatL%05da.ent"), chatIcon);
		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(file.c_str());
		pAniIcon->SetEnable(TRUE);
	}


}

void CMenuFriendProcess::RefreshChatMenu()
{
	if (!gConfigure.uNetwork)
		return;
	/*for (std::multimap<MessengerKey, CMessenger *>::iterator Itr = m_MessengersList[m_CurrGroup].begin();
		Itr != m_MessengersList[m_CurrGroup].end(); ++Itr)
	{
		if (Itr->second->GetFriend_char_id() == m_TargetTalkingCharID)
		{
			CMessageCharacter * pInfo = FindInfo(m_TargetTalkingCharID);
			if (pInfo)
			{
				UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("FriendChatTargetIcon");
				if (pAniIcon)
				{
					String file;
					file.Format(_T("facechatL%05da.ent"), pInfo->GetFriend_icon());
					pAniIcon->ClearAniData();
					pAniIcon->SetAnimationData(file.c_str());
					pAniIcon->SetEnable(TRUE);
				}
			}

			break;
		}
	}*/

	String target;
	CMessageCharacter * pInfo = FindInfo(m_TargetTalkingCharID);
	if (pInfo && (pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE || pInfo->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE_AWAY))
		target.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_CHAT_TARGET")),
		m_TargetTalkingCharName.c_str());
	else
	{
		CharTempFriendDataMap::iterator Itr = m_TempFriendList.find(m_TargetTalkingCharID);
		if (Itr != m_TempFriendList.end())
		{
			if (Itr->second.state == FRIENC_CHAT_STATUS_ONLINE ||
				Itr->second.state == FRIENC_CHAT_STATUS_ONLINE_AWAY)
				target.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_CHAT_TARGET")),
				m_TargetTalkingCharName.c_str());
			else
				target.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_CHAT_OFFLINE")),
				m_TargetTalkingCharName.c_str());
		}
		else
			target.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_CHAT_OFFLINE")),
			m_TargetTalkingCharName.c_str());
	}

	if (pInfo)
	{
		UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("FriendChatTargetIcon");
		if (pAniIcon)
		{
			String file;
			file.Format(_T("facechatL%05da.ent"), pInfo->GetFriend_icon());
			pAniIcon->ClearAniData();
			pAniIcon->SetAnimationData(file.c_str());
			pAniIcon->SetEnable(TRUE);
		}
	}
	
	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("FriendChatName");
	if (pText)
	{
		pText->SetText(target.c_str());
		pText->Redraw();
	}
	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("FriendChatOwnIcon");
	if (pAniIcon)
	{
		String file;
		file.Format(_T("facechatL%05da.ent"), m_pOwnChar->GetFriend_icon());
		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(file.c_str());
		pAniIcon->SetEnable(TRUE);
	}

	UIButton * pBu = gGlobal.pUIManager->FindUIButton("FriendChatNext");
	if (pBu)
	{
		if (HasNextMessage())
			pBu->SetNormalState();
		else
			pBu->SetDisableState();
	}

	UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("EditLine");
	if (pBox)
	{
		UIEditBox * pBox2 = gGlobal.pUIManager->FindUIEditBox("ChatEditLine");

		if(pBox2 == NULL)
			pBox2 = gGlobal.pUIManager->FindUIEditBoxEx("ChatEditLine");

		if (pBox2)
		{
			pBox2->SetFontColor(pBox->GetFontColor());
			pBox2->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(pBox2);
		}
	}
}
void CMenuFriendProcess::SendMessageToFriend()
{
	
	UIEditBox * pBox = gGlobal.pUIManager->FindUIEditBox("ChatEditLine");

	if(pBox == NULL)
		pBox = gGlobal.pUIManager->FindUIEditBoxEx("ChatEditLine");		

	if (pBox)
	{
		//pBox->SaveText();
		String Msg = pBox->GetText();

		//pBox->AddPastStr();

		gGlobal.BlockColorWord(Msg);
		gGlobal.ModifyMessage(Msg);
		//gGlobal.AddColorPrefix(Msg);
		
		MessageService_Proxy::SendPrivateMessage(gGlobal.pPeerProxy, gGlobal.g_context, String(Msg), gGlobal.g_pCharacter->GetMapActorID(),
		m_TargetTalkingCharID, m_TargetTalkingCharName, true);

		pBox->ClearText();
		//pBox->ResetAll();
		
		pBox->Redraw();
	}
	else
	{
		UIChatEditPanel *pChatEdit = gGlobal.pUIManager->FindUIChatEditPanel("ChatEditLine");

		if(pChatEdit)
		{
			pChatEdit->SaveText();
			
			String Msg = pChatEdit->GetText();

			gGlobal.BlockColorWord(Msg);
			gGlobal.ModifyMessage(Msg);
			//gGlobal.AddColorPrefix(Msg);
			
			MessageService_Proxy::SendPrivateMessage(gGlobal.pPeerProxy, gGlobal.g_context, String(Msg), gGlobal.g_pCharacter->GetMapActorID(),
			m_TargetTalkingCharID, m_TargetTalkingCharName, true);

			pChatEdit->ClearAllRow();
			pChatEdit->ClearText();
			pChatEdit->ResetAll();
			pChatEdit->Redraw();
			
		}
	}
	//CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseFriendChat");
}

void CMenuFriendProcess::PrintMessage(String Msg, String SrcName, String TargetName)
{
	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatMsg");
	if (pChat)
	{
		String Text;
		Text.Format(gGlobal.GetStringTable()->Get(_T("MSG_FRIEND_CHAT_SAY")),
			SrcName.c_str());
		pChat->AddStringW(Text.c_str());

		Text.Format(_T("  %s"), Msg.c_str());
		pChat->AddStringW(Text.c_str());
		pChat->EndRow();

		UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("FriendChatScroll");
		if (scrollBar)
		{
			scrollBar->SetNumCount(pChat->RowListCount());
			scrollBar->SetSelNum(pChat->RowListCount() - 1);
		}

	}
}

void CMenuFriendProcess::WriteHistory(String Msg, String MyName, String TargetName, String speaker)
{
	if (!gGlobal.g_pSystemSetting->IsSaveChatHistory())
		return;

	String temp;
	
	TCHAR szBuffer[_MAX_PATH];
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\FriendHistory"), szBuffer);
	if (Reuben::Platform::File::IsExists(temp.c_str()) ||
		::CreateDirectory(temp.c_str(), NULL))
		
	{
		FILE* pFile = NULL;
		String fileName;
		fileName.Format(_T("%s\\%s%s.his"), temp.c_str(), MyName.c_str(), TargetName.c_str());
		pFile = _tfopen(fileName.c_str(), _T("ab"));
		if ( pFile )
		{
			String dateStr;
			SYSTEMTIME st;

			//TCHAR buf[512];
			::GetLocalTime(&st);
			dateStr.Format(_T("%04d/%02d/%02d   %02d:%02d:%02d  %s\\r  %s\\r#W"), st.wYear % 10000, 
				st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,  speaker.c_str(), Msg.c_str());
			//wcscpy(buf, dateStr.c_str());
			fwrite(dateStr.c_str(), dateStr.GetLength() * sizeof(TCHAR), 1, pFile);
			fclose(pFile);
		}
	}
}
void CMenuFriendProcess::RefreshSelfSetting()
{
	if (!gConfigure.uNetwork)
		return;
	UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText("FriendSelfSettingID");
	if (pText)
	{
		String ID;
		ID.Format(_T("%d"), gGlobal.g_pCharacter->GetMapActorID());
		pText->SetText(ID.c_str());
		pText->Redraw();
	}

	pText = gGlobal.pUIManager->FindUIStaticText("FriendSelfSettingName");
	if (pText)
	{
		pText->SetText(gGlobal.g_pCharacter->GetCharName().c_str());
		pText->Redraw();
	}

	UIAniIcon * pAniIcon = gGlobal.pUIManager->FindUIAniIcon("FriendSelfSettingIcon");
	if (pAniIcon)
	{
		String file;
		file.Format(_T("facechatL%05da.ent"), m_pOwnChar->GetFriend_icon());
		pAniIcon->ClearAniData();
		pAniIcon->SetAnimationData(file.c_str());
		pAniIcon->SetEnable(TRUE);
	}

	UIEditBox * pEdit = gGlobal.pUIManager->FindUIEditBoxEx("FriendSelfSettingSign");
	if (pEdit)
	{
		String About(m_pOwnChar->GetFriend_about());
		RemoveLink(About);
		pEdit->SetText(About.c_str());
		pEdit->Redraw();
	}
	else
	{
		UIChatEditPanel * pEdit = gGlobal.pUIManager->FindUIChatEditPanel("FriendSelfSettingSign");
		if (pEdit)
		{
			if(pEdit)
			{
				String		About(m_pOwnChar->GetFriend_about());
				PCSTRING	StringPtr = About.c_str();
				Int			Len = About.size();
				String	MyString;

				for(Int i = 0; i < About.size(); ++i)
				{
					if(StringPtr[Len - 1] == 32)
						--Len;
					else
						break;
				}

				MyString = About.substr(0, Len);

				if(MyString.size())
				{
					pEdit->LoadText(MyString.c_str());
					pEdit->FillImage();
					pEdit->Redraw();
				}
			}
		}
	}

	pEdit = gGlobal.pUIManager->FindUIEditBoxEx("FriendSelfSettingOfflineReply");
	if (pEdit)
	{
		pEdit->SetText(m_pOwnChar->GetFriend_offlineAutoReply().c_str());
		pEdit->Redraw();
	}
	else
	{
		UIChatEditPanel * pEdit = gGlobal.pUIManager->FindUIChatEditPanel("FriendSelfSettingOfflineReply");
		if (pEdit)
		{
			if(pEdit)
			{
				PCSTRING	StringPtr = m_pOwnChar->GetFriend_offlineAutoReply().c_str();
				Int			Len = m_pOwnChar->GetFriend_offlineAutoReply().size();
				String		MyString;

				for(Int i = 0; i < m_pOwnChar->GetFriend_offlineAutoReply().size(); ++i)
				{
					if(StringPtr[Len - 1] == 32)
						--Len;
					else
						break;
				}

				MyString = m_pOwnChar->GetFriend_offlineAutoReply().substr(0, Len);

				if(MyString.size())
				{
					pEdit->LoadText(MyString.c_str());
					pEdit->FillImage();
					pEdit->Redraw();
				}
			}
		}
	}

	UICheckBox * pChkBox = gGlobal.pUIManager->FindUICheckBox("FriendSelfSettingAutoSave");
	if (pChkBox)
	{
		if (gGlobal.g_pSystemSetting->IsSaveChatHistory())
			pChkBox->SetState(true);
		else
			pChkBox->SetState(false);
	}

	UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatMsg");
	if (pChat)
	{
		pChat->ClearAllRow();
		pChat->EndRow();
	}

	UIImage * pImOnline = gGlobal.pUIManager->FindUIImage("FriendSelfSettingOnline");
	UIImage * pImOffline = gGlobal.pUIManager->FindUIImage("FriendSelfSettingOffline");

	if (pImOnline && pImOffline)
	{
		if (m_pOwnChar->GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE)
		{
			pImOnline->SetEnable(true);
			pImOffline->SetEnable(false);
		}
		else
		{
			pImOnline->SetEnable(false);
			pImOffline->SetEnable(true);
		}
	}
}


void CMenuFriendProcess::OpenHistory()
{
	String temp;
	
	TCHAR szBuffer[_MAX_PATH];
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\FriendHistory"), szBuffer);

	FILE* pFile = NULL;
	
	m_OpenFileName.Format(_T("%s\\%s%s.his"), temp.c_str(), gGlobal.g_pCharacter->GetCharName().c_str(), m_TargetTalkingCharName.c_str());
	pFile = _tfopen(m_OpenFileName.c_str(), _T("rb"));
	ULONGLONG size = 0;
	if (pFile)
	{
		fseek (pFile, 0, SEEK_END);
		size =	ftell (pFile);
		fseek (pFile, 0, SEEK_SET);
		TCHAR * pChar = SafeAllocate(TCHAR, (Reuben::System::Size)size + 1);

		ZeroMemory(pChar, (size_t)(size + 1) * sizeof(TCHAR));
		fread(pChar, (size_t)size,1,  pFile);
		UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatHisMsg");
		if (pChat)
		{
			pChat->ClearAllRow();
			pChat->AddStringW(pChar);
			pChat->EndRow();

			UIScrollBarV* scrollBar = gGlobal.pUIManager->FindUIScrollBarV("FriendChatHisScroll");
			if (scrollBar)
			{
				scrollBar->SetNumCount(pChat->RowListCount());
				scrollBar->SetSelNum(pChat->RowListCount() - 1);
			}
		}


		SafeDeallocate(pChar);
		
		fclose(pFile);
	}

}

void CMenuFriendProcess::FindFriend(PCSTRINGW char_id, PCSTRINGW name)
{
	int id = _wtoi(char_id);
	MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, id, name, false, true);
}

CMessenger * CMenuFriendProcess::FindMessenger(UInt32 char_id)
{
	if (m_Messengers)
	{
		for (Int i = 0; i < m_Messengers->GetCount(); i ++)
		{
			CMessenger * pMessager = CastNetObject<CMessenger>(m_Messengers->GetChild(i));
			if (pMessager && pMessager->GetFriend_char_id() == char_id)
			{
				return pMessager;
			}
		}
	}

	return NULL;
}
void CMenuFriendProcess::SetMoveGroupTarget(UInt32 targetID, UInt8 grpIndex)
{
	m_pTargetMessager = FindMessenger(targetID);
	m_ChangeGroup = (UInt8)grpIndex;
	m_TargetCharID = targetID;
}

void CMenuFriendProcess::ConfirmDelete()
{
	if (m_pTargetMessager == NULL)
	{
		CharTempFriendDataMap::iterator Itr = m_TempFriendList.find(m_TargetCharID);
		if (Itr != m_TempFriendList.end())
		{
			if (Itr->first == m_TargetTalkingCharID)
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseFriendChat", String());
			m_TempFriendList.erase(Itr);
			RefreshMenu();
		}
	}
	else
	{
		//m_pTargetMessager->RemoveMessenger(gGlobal.pPeerProxy, gGlobal.g_context);
		if(m_pTargetMessager->GetFriend_relation() !=(1<<FRIEND_RELATION_FRIEND) && m_pTargetMessager->GetFriend_relation() != 0)
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REMOVE_MESSENGER_FAIL_RELATION")), CHANNELTYPE_SYSTEM);
			return;
		}
		MessageService_Proxy::RemoveMessenger(gGlobal.pPeerProxy, gGlobal.g_context, m_pTargetMessager->GetPlayer_char_id(), m_pTargetMessager->GetFriend_char_id());
		
	}
	m_pTargetMessager = NULL;
	m_TargetCharID = 0;
}

void CMenuFriendProcess::RemoveMessenger(const UInt32 &owner_id, const UInt32 &target_id)
{
	for (Int i = 0; i < m_Messengers->GetCount(); i ++)
	{
		CMessenger * pMessenger = CastNetObject<CMessenger>(m_Messengers->GetChild(i));
		if (pMessenger &&
			pMessenger->GetPlayer_char_id() == owner_id &&
			pMessenger->GetFriend_char_id() == target_id)
		{
			if (m_TargetTalkingCharID == pMessenger->GetFriend_char_id() &&
				m_TempFriendList.find(m_TargetTalkingCharID) == m_TempFriendList.end())
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseFriendChat", String());

			m_Messengers->Remove(pMessenger);
			DeleteNetObject(pMessenger);
			break;
		}
	}

	ConstructData();
}
void CMenuFriendProcess::CheckChangeGroup()
{
	if (m_pTargetMessager == NULL)
		return;

	if (m_pTargetMessager->GetFriend_group() == m_ChangeGroup)
		return;

	CMessageCharacter * pInfo = FindInfo(m_pTargetMessager->GetFriend_char_id());
	if (pInfo)
	{
		if ((m_pTargetMessager->GetFriend_group() == FRIEND_GROUP_ONE ||
			m_pTargetMessager->GetFriend_group() == FRIEND_GROUP_TWO ||
			m_pTargetMessager->GetFriend_group() == FRIEND_GROUP_THREE ||
			m_pTargetMessager->GetFriend_group() == FRIEND_GROUP_FOUR) && m_ChangeGroup == FRIEND_GROUP_TEMP)
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_MSG_BREAK_RELATION")), pInfo->GetNickName());

			gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, text.c_str(), "FriendChat");
			UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
			if (pItem)
			{
				pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmChangeGroup");
			}

		}
		else if (m_ChangeGroup == FRIEND_GROUP_BLACK)
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_MSG_TO_BLACKLIST")), pInfo->GetNickName());
			gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, text.c_str(), "FriendChat");
			UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
			if (pItem)
			{
				pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmChangeGroup");
			}
		}
		else if (m_pTargetMessager->GetFriend_group() == FRIEND_GROUP_BLACK)
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_MSG_REMOVE_FROM_BACKLIST")), pInfo->GetNickName());
			gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, text.c_str(), "FriendChat");
			UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
			if (pItem)
			{
				pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmChangeGroup");
			}
		}
		else
			m_pTargetMessager->MoveGroup(gGlobal.pPeerProxy, gGlobal.g_context, m_ChangeGroup);
	}
}
void CMenuFriendProcess::MoveGroup(int memInx, int grpIdx)
{
	if (m_CurrGroup >= MAX_FRIEND_GROUP_TYPE)
		return;
	if (m_CurrGroup == FRIEND_GROUP_TEMP)
	{
		if ((UInt)memInx + m_StartIndex < 0 || (UInt)memInx + m_StartIndex > m_TempFriendList.size() )
		return;
	}
	else
	{
	if ((UInt)memInx + m_StartIndex < 0 || (UInt)memInx + m_StartIndex > m_MessengersList[m_CurrGroup].size() )
		return;
	}

	if (m_CurrGroup != FRIEND_GROUP_BLACK &&
		m_CurrGroup != FRIEND_GROUP_TEMP)
		memInx --;

	if (m_CurrGroup == FRIEND_GROUP_TEMP)
	{
		CharTempFriendDataMap::iterator Itr = m_TempFriendList.begin();
		for (Int i = 0; (i < m_StartIndex || i - m_StartIndex < MAX_FRIEND_IN_ONE_PAGE) && Itr != m_TempFriendList.end(); ++i, ++Itr)
		{
			if (i == memInx + m_StartIndex)
			{
				m_TargetCharID = Itr->first;
				m_TargetCharName = Itr->second.name;
				m_ChangeGroup = (UInt8)grpIdx;
				m_pTargetMessager = NULL;
				if (m_ChangeGroup == FRIEND_GROUP_BLACK)
				{
					String text;
					text.Format(gGlobal.GetStringTable()->Get(_T("MSG_CHAT_MSG_TO_BLACKLIST")), m_TargetCharName);
					gGlobal.g_FriendChatBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO, text.c_str(), "FriendChat");
					UIItem * pItem = gGlobal.pUIManager->GetUIItem("FriendChatYes");
					if (pItem)
					{
						pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "ConfirmChangeGroup");
					}
				}
				else
					MoveGroupImm();

				break;
			}
		}
	}
	else
	{
		std::multimap<MessengerKey, CMessenger *>::iterator Itr = m_MessengersList[m_CurrGroup].begin();
		for (Int i = 0; (i < m_StartIndex || i - m_StartIndex < MAX_FRIEND_IN_ONE_PAGE) && Itr != m_MessengersList[m_CurrGroup].end(); ++i, ++Itr)
		{
			if (i == memInx + m_StartIndex)
			{
				m_pTargetMessager = Itr->second;
				m_TargetCharID = Itr->second->GetFriend_char_id();
				m_ChangeGroup = (UInt8)grpIdx;
				CheckChangeGroup();
				break;
			}
		}
	}
}

void CMenuFriendProcess::MoveGroupImm()
{
	if (m_pTargetMessager)
	{
		
		if (m_ChangeGroup == FRIEND_GROUP_TEMP)
		{
			//m_pTargetMessager->RemoveMessenger(gGlobal.pPeerProxy, gGlobal.g_context);
			if(m_pTargetMessager->GetFriend_relation() !=(1<<FRIEND_RELATION_FRIEND) && m_pTargetMessager->GetFriend_relation() != 0)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REMOVE_MESSENGER_FAIL_RELATION")), CHANNELTYPE_SYSTEM);
				return;
			}

			CMessageCharacter * pInfo = FindInfo(m_pTargetMessager->GetFriend_char_id());
			if (pInfo)
			{
				InserTempFriend(pInfo->GetChar_id(), pInfo->GetNickName(), pInfo->GetFriend_status(), true);
			}
			else
				InserTempFriend(m_pTargetMessager->GetFriend_char_id(), String(), FRIENC_CHAT_STATUS_ONLINE, true);
		}

		if (m_ChangeGroup == FRIEND_GROUP_BLACK)
		{
			if(m_pTargetMessager->GetFriend_relation() !=(1<<FRIEND_RELATION_FRIEND) && m_pTargetMessager->GetFriend_relation() != 0)
			{
				gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REMOVE_MESSENGER_FAIL_RELATION")), CHANNELTYPE_SYSTEM);
				return;
			}
		}
		m_pTargetMessager->MoveGroup(gGlobal.pPeerProxy, gGlobal.g_context, m_ChangeGroup);
	}
	else
	{
		UInt16Vector relations;
		relations.push_back(FRIEND_RELATION_FRIEND);
		MessageService_Proxy::CreateMessenger(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_pCharacter->GetMapActorID(), m_TargetCharID,
		relations, m_ChangeGroup, 0, m_TargetCharName);
		if (m_ChangeGroup == FRIEND_GROUP_TEMP)
		{
			CMessageCharacter * pInfo = FindInfo(m_TargetCharID);
			if (pInfo)
				InserTempFriend(m_TargetCharID, m_TargetCharName, pInfo->GetFriend_status());
			else
				InserTempFriend(m_TargetCharID, m_TargetCharName, FRIENC_CHAT_STATUS_ONLINE);
		}
	}
}

CMessageCharacter * CMenuFriendProcess::FindInfo(UInt32 char_id)
{
	CharMsgCharPtrMap::iterator Itr = m_MessageCharacters.find(char_id);
	if (Itr != m_MessageCharacters.end())
		return FindNetObject<CMessageCharacter>(Itr->second);
	return NULL;
}

CMessageCharacter * CMenuFriendProcess::FindInfo(String char_name)
{
	for (CharMsgCharPtrMap::iterator Itr = m_MessageCharacters.begin();
		Itr != m_MessageCharacters.end(); ++Itr)
	{
		CMessageCharacter * pMsgCharTmp = FindNetObject<CMessageCharacter>(Itr->second);
		if (pMsgCharTmp != NULL && pMsgCharTmp->GetNickName() == char_name)
			return pMsgCharTmp;
	}
		
	return NULL;
}

void CMenuFriendProcess::OpenTalkingFriendDetail()
{
	CMessageCharacter * pInfo = FindInfo(m_TargetTalkingCharID);
	if (pInfo)
	{
		gGlobal.g_MenuTargetProcess->ResetTargetDetail();
		gGlobal.g_MenuTargetProcess->SetFriendDetail(pInfo);
		gGlobal.g_MenuTargetProcess->ShowDetail();
	}
	else
	{
		gGlobal.g_MenuTargetProcess->ResetTargetDetail();
		gGlobal.g_MenuTargetProcess->m_char_id = m_TargetTalkingCharID;
		gGlobal.g_MenuTargetProcess->m_char_name = m_TargetTalkingCharName;
		gGlobal.g_MenuTargetProcess->ShowDetail();	
		gGlobal.g_MenuTargetProcess->UpdateInfo();
	}
}

void CMenuFriendProcess::OpenFriendDetail(int index)
{
	if (index == 0 && m_StartIndex == 0 &&
		m_CurrGroup != FRIEND_GROUP_TEMP && m_CurrGroup != FRIEND_GROUP_BLACK)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSelfSetting", String());
		return;
	}

	if (m_CurrGroup >= MAX_FRIEND_GROUP_TYPE)
		return;

	if (m_CurrGroup == FRIEND_GROUP_TEMP )
	{
		if (index < 0 ||
			(UInt)index + m_StartIndex >= m_TempFriendList.size())
			return;
	}
	else
	{
		if (index < 0 ||
			(UInt)index + m_StartIndex > m_MessengersList[m_CurrGroup].size())
			return;
	}

	if (m_CurrGroup != FRIEND_GROUP_TEMP && m_CurrGroup != FRIEND_GROUP_BLACK)
	{
		index --;
	}

	if (m_CurrGroup == FRIEND_GROUP_TEMP)
	{
		CharTempFriendDataMap::iterator Itr = m_TempFriendList.begin();
		for (Int i = 0; i < index + m_StartIndex && Itr != m_TempFriendList.end(); ++i, ++Itr)
		{
		}

		if (Itr != m_TempFriendList.end())
		{	
			CMessageCharacter * pInfo = FindInfo(Itr->first);
			if (pInfo)
			{
				gGlobal.g_MenuTargetProcess->ResetTargetDetail();
				gGlobal.g_MenuTargetProcess->SetFriendDetail(pInfo);
				gGlobal.g_MenuTargetProcess->ShowDetail();
				MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_MenuTargetProcess->m_char_id, String(), true, false);
				return;
			}
			
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(Itr->first);
			if (pCtrl != NULL)
			{
				gGlobal.g_MenuTargetProcess->ResetTargetDetail();
				gGlobal.g_MenuTargetProcess->SetFriendDetail(pCtrl);
				gGlobal.g_MenuTargetProcess->ShowDetail();
				MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_MenuTargetProcess->m_char_id, String(), true, false);
				return;
			}
			gGlobal.g_MenuTargetProcess->ResetTargetDetail();
			gGlobal.g_MenuTargetProcess->m_char_id = Itr->first;
			gGlobal.g_MenuTargetProcess->m_char_name = Itr->second.name;
			gGlobal.g_MenuTargetProcess->ShowDetail();	
			MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_MenuTargetProcess->m_char_id, String(), true, false);
		}
	}
	else
	{
		std::multimap<MessengerKey, CMessenger *>::iterator Itr = m_MessengersList[m_CurrGroup].begin();
		for (int i = 0; i < index + m_StartIndex; i++)
		{
			Itr ++;

		}
		if (Itr != m_MessengersList[m_CurrGroup].end())
		{
			CMessageCharacter * pInfo = FindInfo(Itr->second->GetFriend_char_id());
			if (pInfo)
			{
				gGlobal.g_MenuTargetProcess->ResetTargetDetail();
				gGlobal.g_MenuTargetProcess->SetFriendDetail(pInfo);
				gGlobal.g_MenuTargetProcess->ShowDetail();
				MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_MenuTargetProcess->m_char_id, String(), true, false);
				return;
			}

			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(Itr->second->GetFriend_char_id());
			if (pCtrl != NULL)
			{
				gGlobal.g_MenuTargetProcess->ResetTargetDetail();
				gGlobal.g_MenuTargetProcess->SetFriendDetail(pCtrl);
				gGlobal.g_MenuTargetProcess->ShowDetail();
				MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_MenuTargetProcess->m_char_id, String(), true, false);
				return;
			}
			gGlobal.g_MenuTargetProcess->m_char_id = Itr->second->GetFriend_char_id();
			gGlobal.g_MenuTargetProcess->ShowDetail();	
			MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, gGlobal.g_MenuTargetProcess->m_char_id, String(), true, false);
		}
	}
}

void CMenuFriendProcess::ConfirmSettingChange()
{
	bool onlineChange = false;
	String replyText;
	String aboutText;
	UIEditBox * pEdit = gGlobal.pUIManager->FindUIEditBoxEx("FriendSelfSettingOfflineReply");
	if (pEdit)
	{
		replyText = pEdit->GetText();
	}
	else
	{
		UIChatEditPanel * pEdit = gGlobal.pUIManager->FindUIChatEditPanel("FriendSelfSettingOfflineReply");
		if (pEdit)
		{
			pEdit->SaveText();
			replyText = pEdit->GetText();
		}
	}

	pEdit = gGlobal.pUIManager->FindUIEditBoxEx("FriendSelfSettingSign");
	if (pEdit)
	{
		aboutText = pEdit->GetText();
	}
	else
	{
		UIChatEditPanel * pEdit = gGlobal.pUIManager->FindUIChatEditPanel("FriendSelfSettingSign");
		if (pEdit)
		{
			pEdit->SaveText();
			aboutText = pEdit->GetText();
		}
	}
	UIItem * pBu = gGlobal.pUIManager->GetUIItem("FriendSelfSettingOnline");
	if (pBu && pBu->IsEnable())
	{
		onlineChange = true;

		//replyText.Format(
	}

	if (onlineChange)
	{
		m_pOwnChar->SelfSettingChange(gGlobal.pPeerProxy, gGlobal.g_context, aboutText,
			FRIENC_CHAT_STATUS_ONLINE, replyText);
	}
	else
		m_pOwnChar->SelfSettingChange(gGlobal.pPeerProxy, gGlobal.g_context, aboutText,
		FRIENC_CHAT_STATUS_ONLINE_AWAY, replyText);

	UICheckBox * pBox = gGlobal.pUIManager->FindUICheckBox("FriendSelfSettingAutoSave");
	if (pBox)
	{
		gGlobal.g_pSystemSetting->SetSaveChatHistory(pBox->GetState());
	}
}

void CMenuFriendProcess::NextChatMessage()
{
	if (m_nextMessageQue.size() > 0)
	{
		StlVector<MessageString>::iterator Itr = m_nextMessageQue.begin();

		if ((*Itr).type == 0)
		{
			m_TargetTalkingCharID = (*Itr).TargetCharID;
			m_TargetTalkingCharName = (*Itr).TargetCharName;

			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenFriendMsgPanel", String());
			RefreshChatMenu();
			UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("FriendChatMsg");
			if (pChat)
			{
				pChat->ClearAllRow();

			}
			for (Int i = 0; i < (Int)(*Itr).MsgLst.size(); i++)
			{
				PrintMessage((*Itr).MsgLst[i], (*Itr).SrcName, (*Itr).TargetName);
			}

		}
		else
		{
			for (Int i = 0; i < (Int)(*Itr).MsgLst.size(); i++)
			{
				gGlobal.PrintPlayerMessage((*Itr).MsgLst[i]);
			}
		}

		m_nextMessageQue.erase(Itr);

		UIButton * pBu = gGlobal.pUIManager->FindUIButton("FriendChatNext");
		if (pBu)
		{
			if (m_nextMessageQue.size() == 0)
			{
				pBu->SetDisableState();
			}
			else
				pBu->SetNormalState();
		}

		pBu = gGlobal.pUIManager->FindUIButton("DownBFriendShip");
		if (pBu && m_nextMessageQue.size() == 0)
		{
			pBu->SetLockState(false);
			pBu->SetNormalState();
		}

		
	}
}

void CMenuFriendProcess::PushNextMessage(String Msg, String TargetName, String SrcName, String TargetCharName, UInt32 TargetCharID,
										 UInt8 type)
{
		UIButton * pBu = gGlobal.pUIManager->FindUIButton("DownBFriendShip");
		if (pBu)
		{
			pBu->SetState(_ButtonNotice);
			pBu->SetLockState(true);
		}

		for (Int i = 0; i < (Int)m_nextMessageQue.size(); i ++)
		{
			if (m_nextMessageQue[i].TargetCharID == TargetCharID)
			{
				m_nextMessageQue[i].MsgLst.push_back(Msg);
				return;
			}
		}

		MessageString msgStr;
		msgStr.MsgLst.push_back(Msg);
		msgStr.SrcName = SrcName;
		msgStr.TargetName = TargetName;
		msgStr.TargetCharID =TargetCharID;
		msgStr.TargetCharName = TargetCharName;
		msgStr.type = type;
		m_nextMessageQue.push_back(msgStr);

		pBu = gGlobal.pUIManager->FindUIButton("FriendChatNext");
		if (pBu)
		{
			pBu->SetNormalState();
		}
	
}

bool CMenuFriendProcess::IsCurrMsgIcon(UInt iconID)
{
	if (m_pOwnChar && m_pOwnChar->GetFriend_icon() == iconID)
		return true;
	return false;
}
