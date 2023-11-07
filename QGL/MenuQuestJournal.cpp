//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "QGL.h"
//-- Self
#include "MenuQuestJournal.h"
#include "QGL/Global.h"
#include "Proxy/Logic/QuestJournal.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/QuestType.h"
#include "LuaPush.h"
#include "Proxy/Logic/Character.h"
#include "QGL/CharacterControl.h"

#define MAX_JOURNAL_PER_PAGE 10

CMenuQuestJournal::CMenuQuestJournal()
{
	//m_journalList.clear();
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
	m_journalEnableMap.clear();
	m_journalDisableMap.clear();
}

CMenuQuestJournal::~CMenuQuestJournal()
{
	//m_journalList.clear();
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
	m_journalEnableMap.clear();
	m_journalDisableMap.clear();
}

void CMenuQuestJournal::ClearAllJournal(bool bDelete)
{
	if (bDelete)
	{
		for (JournalMap::iterator itr = m_journalEnableMap.begin(); itr != m_journalEnableMap.end(); ++itr)
			DeleteNetObject(itr->second);

		for (JournalMap::iterator itr = m_journalDisableMap.begin(); itr != m_journalDisableMap.end(); ++itr)
			DeleteNetObject(itr->second);
	}
	m_journalEnableMap.clear();
	m_journalDisableMap.clear();
}

void CMenuQuestJournal::AddJournalToList(CQuestJournal* pJournal)
{
	JournalMap::iterator itr = m_journalEnableMap.find(pJournal->GetJournal_id());
	JournalMap::iterator itr1 = m_journalDisableMap.find(pJournal->GetJournal_id());
	if (itr != m_journalEnableMap.end())
	{
		//if the pJournal is disabled but can find it in enable list, remove it and change it to disable list
		if (!pJournal->GetEnable())
		{
			m_journalEnableMap.erase(itr);
			m_journalDisableMap.insert(std::make_pair(pJournal->GetJournal_id(), pJournal));
		}
	}
	else if (itr1 != m_journalDisableMap.end())
	{
		//if the pJournal is enabled but can find it in disable list, remove it and change it to enable list
		if (pJournal->GetEnable())
		{
			m_journalDisableMap.erase(itr1);
			m_journalEnableMap.insert(std::make_pair(pJournal->GetJournal_id(), pJournal));
		}
	}
	else
	{
		if (pJournal->GetEnable())
			m_journalEnableMap.insert(std::make_pair(pJournal->GetJournal_id(), pJournal));
		else
			m_journalDisableMap.insert(std::make_pair(pJournal->GetJournal_id(), pJournal));
	}
}

void CMenuQuestJournal::RemoveJournalFromList(CQuestJournal* pJournal)
{
	JournalMap::iterator itr = m_journalEnableMap.find(pJournal->GetJournal_id());
	if (itr != m_journalEnableMap.end())
		m_journalEnableMap.erase(itr);
	else
	{
		itr = m_journalDisableMap.find(pJournal->GetJournal_id());
		if (itr != m_journalDisableMap.end())
			m_journalDisableMap.erase(itr);
	}
}

bool CMenuQuestJournal::IsOpenJournal()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("QuestJournalPanel");
	if (panel)
		return panel->IsEnable();
	return false;
}

void CMenuQuestJournal::ShowQuestJournal()
{
	StringA iconName;
	UIStaticText* pQuestJournalTime = NULL;
	UIStaticText* pQuestJournalName = NULL;
	UIStaticText* pQuestJournalNpc = NULL;
	UIStaticText* pQuestJournalPlace = NULL;
	UIStaticText* pQuestJournalParty = NULL;
	UIStaticText* pQuestJournalCount = NULL;
	UIStaticText* pQuestJournalLevel = NULL;
	UIImage* pImage = NULL;

	iconName.Format("QuestJournalNPCTalk%d", gGlobal.g_pCharacter->GetCharNetObject()->GetSex());
	pImage = gGlobal.pUIManager->FindUIImage(iconName.c_str());
	if (pImage)
		pImage->SetEnable(true);

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("QuestJournalScroll");
	if (scrollBar)
	{
		if ((m_journalDisableMap.size() + m_journalEnableMap.size()) > MAX_JOURNAL_PER_PAGE)
			scrollBar->SetNumCount((UInt)(m_journalDisableMap.size() + m_journalEnableMap.size()) - MAX_JOURNAL_PER_PAGE);
		else
		{
			scrollBar->SetNumCount(0);
			scrollBar->HomeScroll();
		}

		if (m_iStartIndex > scrollBar->GetNumCount())
			m_iStartIndex = scrollBar->GetNumCount();
	}

	UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("QuestJournalSelected");
	if (pHighLight)
	{
		if ((m_journalDisableMap.size() + m_journalEnableMap.size()) > 0 )
		{
			if (m_iSelectedIndex >= (int)m_iStartIndex && m_iSelectedIndex < (int)(m_journalDisableMap.size() + m_journalEnableMap.size())
				&& m_iSelectedIndex < (int)(m_iStartIndex + MAX_JOURNAL_PER_PAGE) )
			{
				iconName.Format("QuestJournalSelect%02d", (m_iSelectedIndex - m_iStartIndex));
				UIImage* temp = gGlobal.pUIManager->FindUIImage(iconName.c_str());
				if (temp)
				{
					pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), temp->GetOffsetY());
					pHighLight->SetEnable(true);
				}
			}
			else
				pHighLight->SetEnable(false);
		}
		else
			pHighLight->SetEnable(false);
	}

	for (Index i = C_INDEX(0); i < MAX_JOURNAL_PER_PAGE; ++i)
	{
		iconName.Format("QuestJournalTime%02d", i);
		pQuestJournalTime = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("QuestJournalName%02d", i);
		pQuestJournalName = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("QuestJournalNPC%02d", i);
		pQuestJournalNpc = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("QuestJournalPlace%02d", i);
		pQuestJournalPlace = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("QuestJournalParty%02d", i);
		pQuestJournalParty = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("QuestJournalCount%02d", i);
		pQuestJournalCount = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("QuestJournalLevel%02d", i);
		pQuestJournalLevel = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());

		if (pQuestJournalTime && pQuestJournalName && pQuestJournalNpc && 
			pQuestJournalPlace && pQuestJournalParty && pQuestJournalCount && pQuestJournalLevel)
		{
			UInt index = m_iStartIndex + i;
			CQuestJournal* pJournal = GetJournalByMenuIndex(index);
			if (pJournal)
			{			
				if (pJournal->GetEnable())
				{
					pQuestJournalTime->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
					pQuestJournalName->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
					pQuestJournalNpc->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
					pQuestJournalPlace->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
					pQuestJournalParty->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
					pQuestJournalCount->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
					pQuestJournalLevel->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
				}
				else
				{
					pQuestJournalTime->SetColor(ItemTextColor[ItemColorIndex_Grey]);
					pQuestJournalName->SetColor(ItemTextColor[ItemColorIndex_Grey]);
					pQuestJournalNpc->SetColor(ItemTextColor[ItemColorIndex_Grey]);
					pQuestJournalPlace->SetColor(ItemTextColor[ItemColorIndex_Grey]);
					pQuestJournalParty->SetColor(ItemTextColor[ItemColorIndex_Grey]);
					pQuestJournalCount->SetColor(ItemTextColor[ItemColorIndex_Grey]);
					pQuestJournalLevel->SetColor(ItemTextColor[ItemColorIndex_Grey]);
				}

				String text;
				if (pJournal->GetTimeStr().IsEmpty())
					text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_FULL_DAY")));
				else
					text.Format(_T("%s"), pJournal->GetTimeStr().c_str());
				pQuestJournalTime->SetText(text.c_str()); 
				pQuestJournalTime->Redraw();
				pQuestJournalTime->SetEnable(true);

				if (pJournal->GetName().IsEmpty())
					text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
				else
					text.Format(_T("%s"), pJournal->GetName().c_str());
				pQuestJournalName->SetText(text.c_str());
				pQuestJournalName->Redraw();
				pQuestJournalName->SetEnable(true);

				if (pJournal->GetNpcName().IsEmpty())
					text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
				else
					text.Format(_T("%s"), pJournal->GetNpcName().c_str());
				pQuestJournalNpc->SetText(text.c_str());
				pQuestJournalNpc->Redraw();
				pQuestJournalNpc->SetEnable(true);

				if (pJournal->GetMapName().IsEmpty())
					text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
				else
					text.Format(_T("%s"), pJournal->GetMapName().c_str());
				pQuestJournalPlace->SetText(text.c_str());
				pQuestJournalPlace->Redraw();
				pQuestJournalPlace->SetEnable(true);

				if (pJournal->GetTeamDescript().IsEmpty())
					text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
				else
					text.Format(_T("%s"), pJournal->GetTeamDescript().c_str());
				pQuestJournalParty->SetText(text.c_str());
				pQuestJournalParty->Redraw();
				pQuestJournalParty->SetEnable(true);

				if (pJournal->GetLimitRound() != 0 && gGlobal.g_QuestSystem)
				{
					CQuestType* pType = gGlobal.g_QuestSystem->GetType(pJournal->GetType_id());     
					text.Format(_T("%d/%d"), pType ? pType->GetTotalRound() : 0, pJournal->GetLimitRound());
				}
				else
					text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_INFINITY")));
				pQuestJournalCount->SetText(text.c_str());
				pQuestJournalCount->Redraw();
				pQuestJournalCount->SetEnable(true);	

				if (pJournal->GetMaxLv() == pJournal->GetMinLv())
				{
					if (pJournal->GetMaxLv() == 0)
						text.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_SYB_NONE")));
					else
						text.Format(_T("%d"), pJournal->GetMaxLv());
				}
				else if (pJournal->GetMaxLv() == 0)
					text.Format(_T("%d"), pJournal->GetMinLv());
				else
					text.Format(_T("%d~%d"), pJournal->GetMinLv(), pJournal->GetMaxLv());
				pQuestJournalLevel->SetText(text.c_str());
				pQuestJournalLevel->Redraw();
				pQuestJournalLevel->SetEnable(true);
			}
			else
			{
				pQuestJournalTime->SetEnable(false);
				pQuestJournalName->SetEnable(false);
				pQuestJournalNpc->SetEnable(false);
				pQuestJournalPlace->SetEnable(false);
				pQuestJournalParty->SetEnable(false);
				pQuestJournalCount->SetEnable(false);
				pQuestJournalLevel->SetEnable(false);
			}
		}
	}
}

void CMenuQuestJournal::SelectJournal(int index)
{
	UInt journalIndex = m_iStartIndex + index;
	CQuestJournal* pJournal = GetJournalByMenuIndex(journalIndex);
	if (index >= 0 && index < MAX_JOURNAL_PER_PAGE && pJournal)
	{
		StringA iconName;
		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("QuestJournalDesc");
		iconName.Format("QuestJournalSelect%02d", index);
		UIImage* pImage = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("QuestJournalSelected");

		if (pChatPanel && pImage && pHighLight)
		{
			pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), pImage->GetOffsetY());
			pHighLight->SetEnable(true);

			pChatPanel->ClearAllRow();

			String talkTitle;
			if ( gGlobal.g_pCharacter->GetCharNetObject()->GetSex() == 0)
				talkTitle.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUEST_JOURNAL_FOR_BOY")), pJournal->GetDescript().c_str());
			else
				talkTitle.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_QUEST_JOURNAL_FOR_GIRL")), pJournal->GetDescript().c_str());
			
			pChatPanel->AddStringW(talkTitle.c_str());
			pChatPanel->EndRow();
			pChatPanel->Redraw();
			pChatPanel->SetEnable(true);

			m_iSelectedIndex = journalIndex;
		}
	}
}

void CMenuQuestJournal::JournalScroll(int scroll)
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("QuestJournalScroll");
	if (scrollBar)
	{
		switch (scroll)
		{
		case 0:
			{
				if (m_iStartIndex > 0)
				{
					--m_iStartIndex;
					ShowQuestJournal();
				}
			}
			break;
		case 1:
			{
				if (scrollBar->GetNumCount() > m_iStartIndex)
				{
					++m_iStartIndex;
					ShowQuestJournal();
				}
			}
			break;
		case 2:
			{
				if (scrollBar->GetNumCount() > 0)
				{
					m_iStartIndex = scrollBar->GetSelNum();
					ShowQuestJournal();
				}
			}
			break;
		}
	}
}

CQuestJournal* CMenuQuestJournal::GetJournalByMenuIndex(UInt menuIndex)
{
	
	if (menuIndex < (m_journalEnableMap.size() + m_journalDisableMap.size()) )
	{
		if (menuIndex < m_journalEnableMap.size())
		{
			JournalMap::iterator itr = m_journalEnableMap.begin();
			for (Index i = 0; i < menuIndex; ++i)
				++itr;
			if (itr != m_journalEnableMap.end())
				return itr->second;
		}
		else
		{
			UInt tempIndex = menuIndex - m_journalEnableMap.size();
			JournalMap::iterator itr = m_journalDisableMap.begin();
			for (Index i = 0; i < tempIndex; ++i)
				++itr;
			if (itr != m_journalDisableMap.end())
				return itr->second;
		}
	}

	return NULL;
}

void CMenuQuestJournal::DBClickJournal(int index)
{
	UInt journalIndex = m_iStartIndex + index;
	CQuestJournal* pJournal = GetJournalByMenuIndex(journalIndex);
	if (index >= 0 && index < MAX_JOURNAL_PER_PAGE && pJournal)
	{

		StringA LuaFunc;
		if (!pJournal->GetDBClickScript().empty())
		{
			char buf[1024];
			int len = WideCharToMultiByte(CP_UTF8, 0, pJournal->GetDBClickScript().c_str(), -1, buf, 1024, NULL, NULL);
			buf[len] = '\0';
			LuaFunc.Format("%s", buf);
			//StrToUTF8(pJournal->GetDBClickScript().c_str(),LuaFunc);
			if (HasLuaFunc(gGlobal.pUIManager->GetLuaState(), LuaFunc.c_str()))
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), LuaFunc.c_str(),String());
		} 

		
	}
}