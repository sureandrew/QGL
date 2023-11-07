//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "QGL.h"
//-- Self
#include "EjectStudentProcess.h"
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Proxy/Logic/Character.h"
#include "QGL/CharacterControl.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "QGL/MenuFriendProcess.h"
#define MAX_EJECT_PER_PAGE 4

CEjectStudentProcess::CEjectStudentProcess()
{
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
}

CEjectStudentProcess::~CEjectStudentProcess()
{
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
}

void CEjectStudentProcess::ClearAllEject()
{
	m_ejectVector.clear();
}

void CEjectStudentProcess::AddEjectToList(UInt32 student_id)
{
	EjectVector::iterator iter = find(m_ejectVector.begin(),m_ejectVector.end(),student_id);
	if(iter != m_ejectVector.end())
	{
		m_ejectVector.erase(iter);
	}

	m_ejectVector.push_back(student_id);
}

void CEjectStudentProcess::RemoveEjectFromList(UInt32 student_id)
{
	EjectVector::iterator iter = find(m_ejectVector.begin(),m_ejectVector.end(),student_id);
	if(iter != m_ejectVector.end())
	{
		m_ejectVector.erase(iter);
	}
}

bool CEjectStudentProcess::IsOpenEject()
{
	UIPanel* panel = gGlobal.pUIManager->FindUIPanel("EjectStudentPanel");
	if (panel)
		return panel->IsEnable();
	return false;
}

void CEjectStudentProcess::ShowEjectStudent()
{
	StringA iconName;
	UIStaticText* pEjectStudentName = NULL;
	UIImage* pImage = NULL;

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("EjectStudentScroll");
	if (scrollBar)
	{
		if (m_ejectVector.size() > MAX_EJECT_PER_PAGE)
		{
			scrollBar->SetEnable(true);
			scrollBar->SetNumCount((UInt)(m_ejectVector.size()) - MAX_EJECT_PER_PAGE);
		}
		else
		{
			scrollBar->SetEnable(false);
			scrollBar->SetNumCount(0);
			scrollBar->HomeScroll();
		}

		if (m_iStartIndex > scrollBar->GetNumCount())
			m_iStartIndex = scrollBar->GetNumCount();
	}

	UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("EjectStudentSelected");
	if (pHighLight)
	{
		if (m_ejectVector.size() > 0 )
		{
			if (m_iSelectedIndex >= (int)m_iStartIndex && m_iSelectedIndex < (int)m_ejectVector.size()
				&& m_iSelectedIndex < (int)(m_iStartIndex + MAX_EJECT_PER_PAGE) )
			{
				iconName.Format("EjectStudentSelect%02d", (m_iSelectedIndex - m_iStartIndex));
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

	for (Index i = C_INDEX(0); i < MAX_EJECT_PER_PAGE; ++i)
	{
		iconName.Format("EjectStudentName%02d", i);
		pEjectStudentName = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		if (pEjectStudentName)
		{
			UInt index = m_iStartIndex + i;
			String name;
			CMessageCharacter* pMChar = gGlobal.g_pMenuFriendProcess->FindInfo(GetEjectByMenuIndex(index));
			if(pMChar)
				name = pMChar->GetNickName();
			if (!name.IsEmpty())
			{			
				pEjectStudentName->SetColor(ItemTextColor[ItemColorIndex_LightGolden]);
				pEjectStudentName->SetText(name.c_str());
				pEjectStudentName->Redraw();
				pEjectStudentName->SetEnable(true);
			}
			else
			{
				pEjectStudentName->SetEnable(false);
			}
		}
	}
}

void CEjectStudentProcess::SelectEject(int index)
{
	StringA iconName;
	UInt ejectIndex = m_iStartIndex + index;
	//String name = GetEjectByMenuIndex(journalIndex);
	String name;
	CMessageCharacter* pMChar = gGlobal.g_pMenuFriendProcess->FindInfo(GetEjectByMenuIndex(ejectIndex));
	if(pMChar)
		name = pMChar->GetNickName();
	if (index >= 0 && index < MAX_EJECT_PER_PAGE && !name.IsEmpty())
	{
		iconName.Format("EjectStudentSelect%02d", index);
		UIImage* pImage = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("EjectStudentSelected");

		if (pImage && pHighLight)
		{
			pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), pImage->GetOffsetY());
			pHighLight->SetEnable(true);
			m_iSelectedIndex = ejectIndex;
		}
	}
}

void CEjectStudentProcess::EjectScroll(int scroll)
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("EjectStudentScroll");
	if (scrollBar)
	{
		switch (scroll)
		{
		case 0:
			{
				if (m_iStartIndex > 0)
				{
					--m_iStartIndex;
					ShowEjectStudent();
				}
			}
			break;
		case 1:
			{
				if (scrollBar->GetNumCount() > m_iStartIndex)
				{
					++m_iStartIndex;
					ShowEjectStudent();
				}
			}
			break;
		case 2:
			{
				if (scrollBar->GetNumCount() > 0)
				{
					m_iStartIndex = scrollBar->GetSelNum();
					ShowEjectStudent();
				}
			}
			break;
		}
	}
}

UInt32 CEjectStudentProcess::GetEjectByMenuIndex(UInt menuIndex)
{
	if (menuIndex < m_ejectVector.size() )
	{
			EjectVector::iterator itr = m_ejectVector.begin();
			for (Index i = 0; i < menuIndex; ++i)
				++itr;
			if (itr != m_ejectVector.end())
				return *itr;
	}
	return 0;
}

void CEjectStudentProcess::DBClickEject(int index)
{
	UInt ejectIndex = m_iStartIndex + index;
	//UInt32 student_id = GetEjectByMenuIndex(ejectIndex);
	m_iSelectedIndex = ejectIndex;
	//if(student_id != 0)
	//{
	//	gGlobal.g_pCharacter->GetCharNetObject()->EjectStudent(gGlobal.pPeerProxy,gGlobal.g_context,student_id);
	//}
	EjectStudentButtonOK();

}
void CEjectStudentProcess::EjectStudentButtonOK()
{
	//UInt32 student_id = GetEjectByMenuIndex(m_iSelectedIndex);
	//if(student_id != 0)
	//{
	//	gGlobal.g_pCharacter->GetCharNetObject()->EjectStudent(gGlobal.pPeerProxy,gGlobal.g_context,student_id);
	//}


	String text = gGlobal.GetStringTable()->Get(_T("MSG_MS_EJECT_ASK"));
	if (!text.IsEmpty())
	{
		gGlobal.g_QuestionBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO,text.c_str(), "OpenQuestion");	
	}
	else
	{
		gGlobal.g_QuestionBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO,_T("MSG_MS_EJECT_ASK"), "OpenQuestion");
	}

	UIItem * pItem = gGlobal.pUIManager->GetUIItem("OpenQuestionYes");
	if (pItem)
	{
		pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "EjectStudent");
	}

	pItem = gGlobal.pUIManager->GetUIItem("OpenQuestionNo");
	if (pItem)
	{
		pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "CloseQuestion");
	}

}

void CEjectStudentProcess::EjectStudent()
{
	UInt32 student_id = GetEjectByMenuIndex(m_iSelectedIndex);
	if(student_id != 0)
	{
		gGlobal.g_pCharacter->GetCharNetObject()->EjectStudent(gGlobal.pPeerProxy,gGlobal.g_context,student_id);
	}
}