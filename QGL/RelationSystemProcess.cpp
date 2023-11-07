//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "QGL.h"
//-- Self
#include "RelationSystemProcess.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Configure.h"
#include "QGLApplication.h"
#include "Proxy/Logic/RelationSystem.h"
#include "Proxy/Logic/Character.h"
#include "QGL/CharacterControl.h"

#define  MAX_CHOOSE_TIME_PER_PAGE 4

CRelationSystemProcess::CRelationSystemProcess()
{
	m_pRelationSystem = NULL;
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
}

CRelationSystemProcess::~CRelationSystemProcess()
{
	m_pRelationSystem = NULL;
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
	m_chooseTime.clear();
}

void CRelationSystemProcess::SetRelationSystem(CRelationSystem* pRelationSystem)
{
	if(m_pRelationSystem == pRelationSystem ) return;
	DeleteNetObject(m_pRelationSystem);
	m_pRelationSystem = pRelationSystem;
}
void CRelationSystemProcess::OpenSetSiblingTitlePrefixPanel(const String &thirdWord)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSetSiblingTitlePrefixPanel", String());

	UIEditBox* pEdit = (UIEditBox*)gGlobal.pUIManager->GetUIItem("SetSiblingTitlePrefixWord3");
	if (pEdit)
	{
		pEdit->SetText(thirdWord.c_str());
		pEdit->Redraw();
	}
}

void CRelationSystemProcess::OpenSetSiblingTitleSuffixPanel(const String &prefix)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSetSiblingTitleSuffixPanel", String());
	UIEditBox* pEdit = (UIEditBox*)gGlobal.pUIManager->GetUIItem("SetSiblingTitlePrefixText");
	if (pEdit)
	{
		pEdit->SetText(prefix.c_str());
		pEdit->Redraw();
	}
}

void CRelationSystemProcess::OpenSelectMarriageTimePanel()
{
	InitTime();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenChooseTimePanel", String());
}

void CRelationSystemProcess::SetSiblingTitlePrefix()
{
	VMProtectBegin; VM_START;

	UIEditBox* pEditWord1 = (UIEditBox*)gGlobal.pUIManager->GetUIItem("SetSiblingTitlePrefixWord1");
	UIEditBox* pEditWord2 = (UIEditBox*)gGlobal.pUIManager->GetUIItem("SetSiblingTitlePrefixWord2");
	UIEditBox* pEditWord4 = (UIEditBox*)gGlobal.pUIManager->GetUIItem("SetSiblingTitlePrefixWord4");
	if (pEditWord1 && pEditWord2 && pEditWord4)
	{

		String strPrefix = String(pEditWord1->GetText()) + String(pEditWord2->GetText()) + String(pEditWord4->GetText());
		if (m_pRelationSystem)
		{
			m_pRelationSystem->SetSiblingTitlePrefix(gGlobal.pPeerProxy, gGlobal.g_context, strPrefix);
		}
	}

	VMProtectEnd; VM_END;
}

void CRelationSystemProcess::SetSiblingTitleSuffix()
{
	VMProtectBegin; VM_START;

	UIEditBox* pEditSuffix = (UIEditBox*)gGlobal.pUIManager->GetUIItem("SetSiblingTitleSuffixText");
	if (pEditSuffix)
	{

		String strSuffix(pEditSuffix->GetText()) ;
		if (m_pRelationSystem)
		{
			m_pRelationSystem->SetSiblingTitleSuffix(gGlobal.pPeerProxy, gGlobal.g_context, strSuffix);
		}
	}

	VMProtectEnd; VM_END;
}

void CRelationSystemProcess::ChooseTimeScroll(UInt16 scroll)
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("ChooseTimeListScroll");
	if (scrollBar)
	{
		switch (scroll)
		{
		case 0:
			{
				if (m_iStartIndex > 0)
				{
					--m_iStartIndex;
					ShowChooseTime();
				}
			}
			break;
		case 1:
			{
				if (scrollBar->GetNumCount() > m_iStartIndex)
				{
					++m_iStartIndex;
					ShowChooseTime();
				}
			}
			break;
		case 2:
			{
				if (scrollBar->GetNumCount() > 0)
				{
					m_iStartIndex = scrollBar->GetSelNum();
					ShowChooseTime();
				}
			}
			break;
		}
	}
}

void CRelationSystemProcess::ShowChooseTime()
{

	UIStaticText* pChooseTime = NULL;
	StringA iconName;

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("ChooseTimeListScroll");
	if (scrollBar)
	{
		if (m_chooseTime.size() > MAX_CHOOSE_TIME_PER_PAGE)
			scrollBar->SetNumCount((UInt)m_chooseTime.size() - MAX_CHOOSE_TIME_PER_PAGE);
		else
		{
			scrollBar->SetNumCount(0);
			scrollBar->HomeScroll();
		}

		if (m_iStartIndex > scrollBar->GetNumCount())
			m_iStartIndex = scrollBar->GetNumCount();
	}

	UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("ChooseTimeSelected");
	if (pHighLight)
	{
		if (m_chooseTime.size() > 0 )
		{
			if (m_iSelectedIndex >= (int)m_iStartIndex && m_iSelectedIndex < m_chooseTime.size()
				&& m_iSelectedIndex < (int)m_iStartIndex + MAX_CHOOSE_TIME_PER_PAGE)
			{
				iconName.Format("ChooseTimeSelect%02d", (m_iSelectedIndex - m_iStartIndex));
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

	for (Index i = C_INDEX(0); i < MAX_CHOOSE_TIME_PER_PAGE; ++i)
	{
		iconName.Format("ChooseTime%02d", i);
		pChooseTime = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		if (pChooseTime)
		{
			UInt index = m_iStartIndex + i;
			POINT choosetime = GetChooseTimeByIndex(index);
			if (choosetime.x != 0)
			{
				String text;
				text.Format(_T("%02d:%02d"),choosetime.x,choosetime.y);
				pChooseTime->SetText(text.c_str());
				pChooseTime->Redraw();
			}
		}
	}

}


POINT CRelationSystemProcess::GetChooseTimeByIndex(UInt16 menuIndex)
{

	if (menuIndex < m_chooseTime.size() )
		return m_chooseTime[menuIndex];
	else
	{
		POINT temp;
		temp.x = 0;
		temp.y = 0;
		return temp;
	}
}

void CRelationSystemProcess::InitTime()
{
	m_chooseTime.clear();
	time_t now = ::time(NULL);
	struct tm* ptime = localtime(&now);
	for(UInt i=0; i<gConfigure.uMarriageTimeCount; i++)
	{
		if(gConfigure.pMarriageTime[i].x > ptime->tm_hour || (gConfigure.pMarriageTime[i].x == ptime->tm_hour && gConfigure.pMarriageTime[i].y > ptime->tm_min))
			m_chooseTime.push_back(gConfigure.pMarriageTime[i]);
	}
}

void CRelationSystemProcess::SelectChooseTime(int index)
{
	UInt journalIndex = m_iStartIndex + index;
	POINT ptime = GetChooseTimeByIndex(journalIndex);
	if (index >= 0 && index < MAX_CHOOSE_TIME_PER_PAGE && ptime.x != 0)
	{
		StringA iconName;
		iconName.Format("ChooseTimeSelect%02d", index);
		UIImage* pImage = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("ChooseTimeSelected");

		if (pImage && pHighLight)
		{
			pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), pImage->GetOffsetY());
			pHighLight->SetEnable(true);
			m_iSelectedIndex = journalIndex;
		}
	}
}

void CRelationSystemProcess::EstablishChooseTime()//check OK bottom
{
	POINT tmpTime = GetChooseTimeByIndex(m_iSelectedIndex);
	if (tmpTime.x == 0 && tmpTime.y == 0)
		return;
	else if(m_pRelationSystem)
	{
		m_pRelationSystem->SelectMarriageTime(gGlobal.pPeerProxy, gGlobal.g_context,tmpTime.x,tmpTime.y);
	}
}