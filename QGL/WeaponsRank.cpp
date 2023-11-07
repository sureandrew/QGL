//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "QGL.h"
//-- Self

#include "WeaponsRank.h"
#include "QGL/Global.h"
#include "LuaPush.h"
//-- Library
#include "Resource/QGLCSVTReader.h"
#include "Resource/ClientResManager.h"

CWeaponsRank::CWeaponsRank()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("111"));
	m_iStartIndex = 0;
	//m_WeaponsRankInfoLst.clear();
	m_iSelectedIndex = 0 ;
}

CWeaponsRank::~CWeaponsRank()
{
	m_iStartIndex = 0;
	//m_WeaponsRankInfoLst.clear();
	m_iSelectedIndex = 0;
}


void CWeaponsRank::ShowWeaponsRank()
{
	//AddWeaponsRankList();
	StringA iconName;

	UIStaticText* pWeaponsRank = NULL;
	UIStaticText* pWeaponsName = NULL;
	UIStaticText* pWeaponsOwner = NULL;
	UIStaticText* pWeaponsDepict = NULL;

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("WeaponsRankScroll");
	if (scrollBar)
	{
		scrollBar->SetNumCount(20);

		if (m_iStartIndex > scrollBar->GetNumCount())
			m_iStartIndex = scrollBar->GetNumCount();
	}

	UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("WeaponsRankSelected");
	if (pHighLight)
	{
		if(m_iSelectedIndex - m_iStartIndex < 10 && m_iSelectedIndex - m_iStartIndex >=0)
		{
			iconName.Format("WeaponsRankSelect%02d", (m_iSelectedIndex - m_iStartIndex));
			UIImage* temp = gGlobal.pUIManager->FindUIImage(iconName.c_str());
			if (temp)
			{
				pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), temp->GetOffsetY());
				pHighLight->SetEnable(true);
			}
			else
				pHighLight->SetEnable(false);
		}
		else
			pHighLight->SetEnable(false);
	}
	for(int index = 0;index<10;index++)
	{
		iconName.Format("WeaponsRank%02d", index);
		pWeaponsRank = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WeaponsName%02d", index);
		pWeaponsName = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WeaponsOwner%02d", index);
		pWeaponsOwner = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		iconName.Format("WeaponsDepict%02d", index);
		pWeaponsDepict = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());

		if (pWeaponsRank && pWeaponsName && pWeaponsOwner && pWeaponsDepict)
		{
			String text;
			text.Format(_T("%d"), index+m_iStartIndex+1);

			pWeaponsRank->SetText(text.c_str());
			pWeaponsRank->Redraw();
			pWeaponsRank->SetEnable(true);

			if(m_iStartIndex+index > 30)
				continue;

			if(m_iStartIndex+index < 0 )
				continue;

			const WeaponsRankInfo * pWeaponsRankInfo = gGlobal.m_resource->GetWeaponsRankData(m_iStartIndex+index+1);
			if(!pWeaponsRankInfo)
				continue;

			text = pWeaponsRankInfo->WeaponsName;

			pWeaponsName->SetText(text.c_str());
			pWeaponsName->Redraw();
			pWeaponsName->SetEnable(true);

			text = pWeaponsRankInfo->ChiOwnerName;

			pWeaponsOwner->SetText(text.c_str());
			pWeaponsOwner->Redraw();
			pWeaponsOwner->SetEnable(true);

			text = pWeaponsRankInfo->WeaponsDepict;

			pWeaponsDepict->SetText(text.c_str());
			pWeaponsDepict->Redraw();
			pWeaponsDepict->SetEnable(true);
		}
	}
}


void CWeaponsRank::SelectWeaponsRank(int index)
{
	if (index >= 0 && index < 10)
	{
		StringA iconName;
		iconName.Format("WeaponsRankSelect%02d", index);
		UIImage* pImage = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("WeaponsRankSelected");

		if ( pImage && pHighLight)
		{
			pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), pImage->GetOffsetY());
			pHighLight->SetEnable(true);

			m_iSelectedIndex = m_iStartIndex+index;
		}
	}
}


void CWeaponsRank::DBClickWeaponsRank(int index)
{
	
}


//0: up, 1:down, 2: scroll
void CWeaponsRank::WeaponsRankScroll(int scroll)
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("WeaponsRankScroll");
	if (scrollBar)
	{
		switch (scroll)
		{
		case 0:
			{
				if (m_iStartIndex > 0)
				{
					--m_iStartIndex;
					ShowWeaponsRank();
				}
				else
				{
					m_iStartIndex = 0;
					ShowWeaponsRank();
				}
			}
			break;
		case 1:
			{
				if (scrollBar->GetNumCount() > m_iStartIndex)
				{
					++m_iStartIndex;
					ShowWeaponsRank();
				}
				else
				{
					m_iStartIndex = scrollBar->GetNumCount();
					ShowWeaponsRank();
				}
			}
			break;
		case 2:
			{
				if (scrollBar->GetNumCount() > 0)
				{
					m_iStartIndex = scrollBar->GetSelNum();
					ShowWeaponsRank();
				}
			}
			break;
		}
	}
}

void CWeaponsRank::WeaponsRankSearchList(int lang)
{
	UIEditBox* searchBox ;
	if (lang == 0)
		searchBox = gGlobal.pUIManager->FindUIEditBox("WeaponsRankSearchTextBox");
	else if (lang == 1)
		searchBox = gGlobal.pUIManager->FindUIEditBox("WeaponsRankSearchChiTextBox");

	if (searchBox)
	{
		String searchName = searchBox->GetText();
		if (searchName.length() == 0)
			return;

		for (UInt i = 0; i < 30; ++i)
		{
			if ( FindWeaponsRank(i, searchName, lang) )
			{
				if(i<m_iStartIndex)
				{
					m_iStartIndex = i;
				}
				else if( i>=m_iStartIndex+10 )
				{
					m_iStartIndex = (i-9);
				}
				m_iSelectedIndex = i;
				ShowWeaponsRank();
				return;
			}
		}
	}
}

void CWeaponsRank::AddWeaponsRankList()
{
	CQGLCSVTReader reader;
	String strWeaponsName;
	String strWeaponsOwner;
	String strWeaponsDepict;

	//m_WeaponsRankInfoLst.clear();

	if( reader.Open(_T("Data/CSV/WeaponsRank.csv"), true) )
	{
		try{
			while( !reader.IsLineEmpty()  )
			{
				WeaponsRankInfo temp;
				reader.ReadString(_T("WeaponsName"), strWeaponsName);
				reader.ReadString(_T("WeaponsOwner"), strWeaponsOwner);
				reader.ReadString(_T("WeaponsDepict"), strWeaponsDepict);

				temp.ChiOwnerName = strWeaponsOwner;
				temp.WeaponsName = strWeaponsName;
				temp.WeaponsDepict = strWeaponsDepict;

				//m_WeaponsRankInfoLst.push_back(temp);

				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("readererror"));
		}
		reader.Close();
	}else
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("OpenFALSE"));
	}

}

bool CWeaponsRank::FindWeaponsRank(UInt16 index, String keyword, int lang)
{
	if (30 > index)
	{
		const WeaponsRankInfo * pWeaponsRankInfo = gGlobal.m_resource->GetWeaponsRankData(index);
		if(!pWeaponsRankInfo)
			return false;

		String temp;

		if (lang == 0)
			temp = pWeaponsRankInfo->EngOwnerName;
		else if (lang == 1)
			temp = pWeaponsRankInfo->ChiOwnerName;

		String tempKeyword = keyword;

		temp.ToUpper();
		tempKeyword.ToUpper();
		temp = temp.substr(0, tempKeyword.length() );

		if ( temp.find(tempKeyword) != -1)
			return true;
		return false;
	}
	return false;
}