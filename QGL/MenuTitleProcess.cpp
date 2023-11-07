//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuTitleProcess.h"
#include "Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGL/CharacterControl.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResTitle.h"

#define MAX_TITLE_ONE_PAGE	10
CMenuTitleProcess::CMenuTitleProcess()
{
	m_currSelIndex = 0;
	m_currIndex = 0;
}

CMenuTitleProcess::~CMenuTitleProcess()
{
	for (StlMap<UInt16, String>::iterator Itr = m_titleDataLst.begin();
		Itr != m_titleDataLst.end(); ++Itr)
	{
		Itr->second.Empty();
	}
}

void CMenuTitleProcess::HideAllText()
{
	for (UInt i = 1; i <= 10; i++)
	{
		StringA name;
		name.Format("TitleName%d", i);
		gGlobal.pUIManager->SetUIItemEnable(name.c_str(), FALSE);
	}

	gGlobal.pUIManager->SetUIItemEnable("TitleHighLight", FALSE);
}
void CMenuTitleProcess::RefreshMenu()
{
	HideAllText();

	UInt i = 0;
	if (m_currIndex == 0)
	{
		StringA name;
		name.Format("TitleName%d", i - m_currIndex + 1);
		gGlobal.pUIManager->SetUIItemEnable(name.c_str(), TRUE);
		gGlobal.SetUIStaticText(name,gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE")));
		i++;
	}
	for (StlMap<UInt16, String>::iterator Itr = m_titleDataLst.begin(); 
		Itr != m_titleDataLst.end(); ++Itr, ++i)
	{
		if (i >= m_currIndex + MAX_TITLE_ONE_PAGE)
			break;
		else if (i >= m_currIndex)
		{
			StringA name;
			name.Format("TitleName%d", i - m_currIndex + 1);
			UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
			if (pText)
			{
				pText->SetEnable(TRUE);
				pText->SetText(Itr->second);

				const TitleData * pTitleData = gGlobal.m_resource->GetTitleData(Itr->first);
				if (pTitleData)
				{
					if (pTitleData->Title_TimeLimitType != 0)
					{
						pText->SetFontColor(TextColor[ColorIndex_Yellow]);
					}
					else
					{
						pText->SetFontColor(TextColor[ColorIndex_White]);
					}
				}

				pText->Redraw();
			}
		}

	}
}
void CMenuTitleProcess::PageUp()
{
	if (m_currIndex > 0)
	{
		m_currIndex --;
		RefreshMenu();
	}
}
void CMenuTitleProcess::PageDown()
{
	if (m_currIndex < m_currIndex + MAX_TITLE_ONE_PAGE)
	{
		m_currIndex ++;
		RefreshMenu();
	}
}
void CMenuTitleProcess::Sroll()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("TitlePanelScroll");
	if (scrollBar)
	{
		UInt selNum = scrollBar->GetSelNum();
		if (selNum + MAX_TITLE_ONE_PAGE >  m_titleDataLst.size())
		{
			if (m_titleDataLst.size() >= MAX_TITLE_ONE_PAGE)
				selNum = m_titleDataLst.size() - MAX_TITLE_ONE_PAGE;
			else
				selNum = 0;
		}
		if (m_currIndex != selNum)
		{
			m_currIndex = selNum;
			RefreshMenu();
		}
	}
}
void CMenuTitleProcess::SetSelectIndex(Int index)
{
	m_currSelIndex = index + m_currIndex;
}
void CMenuTitleProcess::ConfirmChange()
{
	if (m_currSelIndex == 0)
	{
		gGlobal.g_pCharacter->GetCharNetObject()->ChangeTitle(gGlobal.pPeerProxy, RPCContext(), 0);
		return;
	}
	UInt i = 0;
	if (m_currIndex == 0)
		i++;
	for (StlMap<UInt16, String>::iterator Itr = m_titleDataLst.begin(); 
		Itr != m_titleDataLst.end(); ++Itr, ++i)
	{
		if (i == m_currSelIndex)
		{
			gGlobal.g_pCharacter->GetCharNetObject()->ChangeTitle(gGlobal.pPeerProxy, RPCContext(), Itr->first);
		}
	}
}
