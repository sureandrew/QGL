//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuSearchListProcess.h"
//-- Library
#include "QGL/Global.h"
#include "Resource/ClientResManager.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGL/QGLApplication.h"
//#include "MapNPCControl.h"
#include "Configure.h"
#include "QGL/MapControl.h"
#include "QGL/CharacterControl.h"
#include "Math.h"
#include "Common/PathFinder.h"
#include <algorithm>
#include <functional> 
#include "MiddleMap.h"
#include "Resource/ClientResManager.h"
#include "QGL/QGLApplication.h"
#include "QGL/PathMove.h"
#include "qgl/CrossMapPathFinder.h"
#include "Proxy/Service/MessageService_proxy.h"
#define MAX_OF_SEARCH_PARTY_EACH_PAGE	12

typedef StlDeque<BlockPos, ManagedAllocator<BlockPos> > PointList;

CMenuSearchListProcess::CMenuSearchListProcess()
{
	m_iStartListIndex = 0;
	m_iSelectedNPCListIndex = -1;
	m_iTimer = 0;
	m_mapID = 0;
	m_bShowNPClist = true;	
	m_NPCInfoLst.clear();
	m_CharInfoLst.clear();
	m_CharInfoLst_new.clear();
	m_sortMethod = PARTY_SORT_METHOD_DESC;
	m_sortMethodChar = PARTY_SORT_METHOD_DESC;
	m_iSelectedPartyListIndex = 0;
	m_iSelectedPartyPageIndex = 0;

	m_iSelectedCharListIndex = 0;
	m_iSelectedCharPageIndex = 0;
}

CMenuSearchListProcess::~CMenuSearchListProcess()
{
	m_iStartListIndex = 0;
	m_iSelectedNPCListIndex = -1;
	m_iTimer = 0;
	m_mapID = 0;
	m_NPCInfoLst.clear();
	m_bShowNPClist = true;
	m_CharInfoLst.clear();
}

bool CMenuSearchListProcess::Sorting(const NPCInfo &prev, const NPCInfo &next)
{
	String tempPrev = prev.EngName;
	String tempNext = next.EngName;
	if ( tempPrev.ToUpper() > tempNext.ToUpper())
		return false;
	
	return true;
}

bool CMenuSearchListProcess::SortPatryByDesc(const PartyInfo &prev, const PartyInfo &next)
{
	return (_tcslen(prev.desc) < _tcslen(next.desc) ||
		(_tcslen(prev.desc) == _tcslen(next.desc) && prev.level > next.level));
}

bool CMenuSearchListProcess::SortPatryByFaction(const PartyInfo &prev, const PartyInfo &next)
{
	return (prev.faction > next.faction ||
		(prev.faction == next.faction && prev.level > next.level));
}

bool CMenuSearchListProcess::SortPatryByCclass(const PartyInfo &prev, const PartyInfo &next)
{
	return (prev.cclass > next.cclass ||
		(prev.cclass == next.cclass && prev.level > next.level));
}

bool CMenuSearchListProcess::SortPatryByLevel(const PartyInfo &prev, const PartyInfo &next)
{
	return (prev.level > next.level ||
		(prev.level == next.level && prev.faction > next.faction));
}

bool CMenuSearchListProcess::SortPatryByCount(const PartyInfo &prev, const PartyInfo &next)
{
	return (prev.count > next.count ||
		(prev.count == next.count && prev.level > next.level));
}

//==============================================
//==============================================
bool CMenuSearchListProcess::SortCharByFaction(const CharInfo_new &prev, const CharInfo_new &next)
{
	return (prev.faction > next.faction ||
		(prev.faction == next.faction && prev.level > next.level));
}
bool CMenuSearchListProcess::SortCharByCclass(const CharInfo_new &prev, const CharInfo_new &next)
{
	return (prev.cclass > next.cclass ||
		(prev.cclass == next.cclass && prev.level > next.level));
}
bool CMenuSearchListProcess::SortCharByLevel(const CharInfo_new &prev, const CharInfo_new &next)
{
	return (prev.level > next.level ||
		(prev.level == next.level && prev.faction > next.faction));
}
//==============================================
//==============================================

void CMenuSearchListProcess::MakeNPCSearchList()
{
	if (gGlobal.m_mapdb== NULL || gGlobal.m_mapdb->m_basicinfo->m_mapid == m_mapID)
		return;

	m_mapID = gGlobal.m_mapdb->m_basicinfo->m_mapid;

	m_NPCInfoLst.clear();

	for( Index i = C_INDEX(0); i < gGlobal.m_npclist.size(); ++i )
	{
		NPCInfo temp;
		NPCDB_Client * tempNPC = gGlobal.LoadNpcTemplate(gGlobal.m_npclist[i]->nNPCUserID);
		if( tempNPC )
		{
			temp.EngName =  tempNPC->m_basicinfo->m_npc_pronounce;
			temp.ChiName = tempNPC->m_basicinfo->m_npc_mid_name;
			temp.BlockPosX = gGlobal.m_npclist[i]->npc_position_x;
			temp.BlockPosY = gGlobal.m_npclist[i]->npc_position_y;
			temp.NPC_id = gGlobal.m_npclist[i]->nNPCUserID;
			temp.NPC_type = tempNPC->m_basicinfo->m_npc_type;
			temp.PrevName = tempNPC->m_basicinfo->m_npc_pre_name;

			m_NPCInfoLst.push_back(temp);
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CMenuSearchListProcess::MakeNPCSearchList() Error (1) [npc not found in template] NpcID=%i, mapID=%i"), gGlobal.m_npclist[i]->nNPCUserID, gGlobal.m_mapdb->m_basicinfo->m_mapid);
		}
	}

	std::sort(m_NPCInfoLst.begin(), m_NPCInfoLst.end(), Sorting);

}

void CMenuSearchListProcess::UpdateSearchNPC()
{
	m_bShowNPClist = true;
	//if (m_NPCInfoLst.empty())
		MakeNPCSearchList();

	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("NPCSearchPanel");

	if (panel)
	{
		int count = 0;
		StringA textIconName;

		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("NPCSearchScrollBar");
		
		if (scrollBar && m_NPCInfoLst.size() > 17)			
			scrollBar->SetNumCount((UInt32)m_NPCInfoLst.size());			
		
		while ( (int)m_NPCInfoLst.size() > (count + m_iStartListIndex) && count < 17 )
		{
			textIconName.Format("NPCName%d", count + 1);
			UIStaticText* npcNameText = (UIStaticText*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());
			textIconName.Format("NPCNamePos%d", count + 1);
			UIStaticText* npcPosText = (UIStaticText*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());

			if (npcNameText && npcPosText)
			{
				String temp;
				
				if (m_NPCInfoLst[count + m_iStartListIndex].NPC_type != NPC_INVISIBLE_EMENY)
				{
					if (m_NPCInfoLst[count + m_iStartListIndex].PrevName.length() > 0)
						temp.Format(_T("(%s)%s"), m_NPCInfoLst[count + m_iStartListIndex].PrevName.c_str(), m_NPCInfoLst[count + m_iStartListIndex].ChiName.c_str());
					else
						temp.Format(_T("%s"), m_NPCInfoLst[count + m_iStartListIndex].ChiName.c_str());
					npcNameText->SetText(temp.c_str() );
					temp.Format(_T( "(%d,%d)"), m_NPCInfoLst[count + m_iStartListIndex].BlockPosX, m_NPCInfoLst[count + m_iStartListIndex].BlockPosY) ;
					npcPosText->SetText(temp.c_str());
				}
				else
				{
					if (m_NPCInfoLst[count + m_iStartListIndex].PrevName.length() > 0)
						temp.Format(_T("(%s)%s"), m_NPCInfoLst[count + m_iStartListIndex].PrevName.c_str(), m_NPCInfoLst[count + m_iStartListIndex].ChiName.c_str());
					else
						temp.Format(_T("%s"), m_NPCInfoLst[count + m_iStartListIndex].ChiName.c_str());
					npcNameText->SetText(temp.c_str() );
					npcPosText->SetText(_T(""));
				}
				npcNameText->Redraw();
				npcPosText->SetColor(TextColor[ColorIndex_White]);	
				npcPosText->Redraw();
			}

			textIconName.Format("NPCNameHighLight%d", count+1);
			UIImage* button = (UIImage*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());
			button->SetEnable(true);
			++count;
		}

		UIImage* buttonHighLight = (UIImage*)gGlobal.pUIManager->GetUIItem("NPCNameHighLight");
		if (buttonHighLight)
		{
			if (m_iSelectedNPCListIndex < (int)m_iStartListIndex || m_iSelectedNPCListIndex > (int)m_iStartListIndex + 16)
				buttonHighLight->SetEnable(false);
			else
			{
				int index = m_iSelectedNPCListIndex - m_iStartListIndex;
				textIconName.Format("NPCNameHighLight%d", index + 1);
				UIImage* button = (UIImage*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());
				if (button)
				{
					button->SetEnable(false);
					buttonHighLight->SetOffsetXY(button->GetOffsetX(), button->GetOffsetY() );
					buttonHighLight->SetEnable(true);
				}
			}
		}
		
		gGlobal.pUIManager->AddDirtyUIItem(panel);
	}
}

void CMenuSearchListProcess::MoveCharacterToNPC()
{
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("NPCSearchPanel");
		
	if (panel && m_iSelectedNPCListIndex >= 0 && m_iSelectedNPCListIndex < (int)m_NPCInfoLst.size() && m_bShowNPClist && gGlobal.g_pCharacter)
	{
		int desX = 0;
		int desY = 0;

		desX = m_NPCInfoLst[m_iSelectedNPCListIndex].BlockPosX;
		desY = m_NPCInfoLst[m_iSelectedNPCListIndex].BlockPosY;
		gGlobal.g_pCharacter->SetEntityDesPos(desX, desY);
		PointList ptList;
		ptList = gGlobal.g_pCharacter->GetCharMainView()->m_PathFindData.m_ptList;
		if (ptList.size() > 10)
			gGlobal.g_pCharacter->SetEntityDesPos(ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY, true);
		else
			gGlobal.g_pCharacter->SetEntityDesPos(desX, desY, true);

		if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
			gGlobal.g_pMiddleMap->DrawPath(desX, desY);
		else
		{
			if (ptList.size() > 10)
				gGlobal.g_pMiddleMap->SetCharDes( ptList[ptList.size() - 10].PosX , ptList[ptList.size() - 10].PosY );
			else
				gGlobal.g_pMiddleMap->SetCharDes(desX, desY);
			CCrossMapPathFinder::GetInstance()->DestroyPathData();
			gGlobal.m_u32DestNpcId=0;
			gGlobal.g_pMiddleMap->SetPathEnd(desX, desY);
		}
	
	}
}

void CMenuSearchListProcess::NPCSearchListClick(int index)
{
	if (index > 0 && index <= 17)
	{
		StringA buttonName;
		buttonName.Format("NPCNameHighLight%d", index);
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("NPCSearchPanel");
		UIImage* button = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());

		for (Index i = C_INDEX(1); i <= 17; ++i)
		{
			buttonName.Format("NPCNameHighLight%d", i);
			UIImage* prevButton = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());

			if (prevButton)
				prevButton->SetEnable(true);
		}
		

		if (panel && button)
		{	
			m_iSelectedNPCListIndex = index - 1 + m_iStartListIndex;
			button->SetEnable(false);
			UIImage* buttonHighLight = (UIImage*)gGlobal.pUIManager->GetUIItem("NPCNameHighLight");
			if (buttonHighLight)
			{
				buttonHighLight->SetOffsetXY(button->GetOffsetX(), button->GetOffsetY());
				buttonHighLight->SetEnable(true);
			}
		}
	}
}

void CMenuSearchListProcess::ScrollUpOrDown(int up)
{
	if ( (m_NPCInfoLst.size() <= 17 && m_bShowNPClist) || (m_CharInfoLst.size() <= 17 && !m_bShowNPClist) )
	{
		m_iStartListIndex = 0;
		return;
	}
		
	if (up == 0)
	{
		if (m_iStartListIndex > 0)
		{
			--m_iStartListIndex;
		}
		else
		{
			UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("NPCSearchScrollBar");
			if (scrollBar)
			{
				scrollBar->HomeScroll();
			}
		}
	}
	else if (up = 1)
	{
		if ( ( ( m_NPCInfoLst.size() - m_iStartListIndex ) > 17 && m_bShowNPClist) || ( ( m_CharInfoLst.size() - m_iStartListIndex ) > 17 && !m_bShowNPClist) )
		{
			++m_iStartListIndex;
		}
		else
		{
			UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("NPCSearchScrollBar");
			if (scrollBar)
			{
				scrollBar->EndScroll();
			}
		}
	}

	if (m_bShowNPClist)
		UpdateSearchNPC();
	else
		UpdateSearchCharacter();
}

bool CMenuSearchListProcess::FindNPC(UInt16 index, String keyword, int lang)
{
	if (m_NPCInfoLst.size() > index)
	{
		String temp;
		
		if (lang == 0)
			temp = m_NPCInfoLst[index].EngName;
		else if (lang == 1)
			temp = m_NPCInfoLst[index].ChiName;

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

bool CMenuSearchListProcess::FindChar(UInt16 index, String keyword)
{
	if (m_CharInfoLst.size() > index)
	{
		String temp;
		temp = m_CharInfoLst[index].CharName;

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

void CMenuSearchListProcess::SearchNPCList(int lang)
{
	if (m_bShowNPClist) //search npc
	{
		m_iTimer = timeGetTime();
		UIEditBox* searchBox ;
		if (lang == 0)
			searchBox = gGlobal.pUIManager->FindUIEditBox("SearchTextBox");
		else if (lang == 1)
			searchBox = gGlobal.pUIManager->FindUIEditBox("SearchChiTextBox");

		if (searchBox)
		{
			String searchName = searchBox->GetText();
			if (searchName.length() == 0)
				return;

			int startSearchIndex = 0;
			if (m_iSelectedNPCListIndex >= 0)
				startSearchIndex = m_iSelectedNPCListIndex;

			for (UInt i = (startSearchIndex + 1); i < m_NPCInfoLst.size(); ++i)
			{
				if ( FindNPC(i, searchName, lang) )
				{
					int index = 1;
					if  (i > (m_iStartListIndex + 16) )
					{
						if ((i + 16) < (int)m_NPCInfoLst.size())
						{
							index = 1;	
							m_iStartListIndex = i;
						}
						else 
						{
							m_iStartListIndex = (UInt32)m_NPCInfoLst.size() - 17;
							index = i - m_iStartListIndex + 1;	
						}	
					}
					else if ( i >= m_iStartListIndex && (i < m_iStartListIndex + 17))
					{
						index = i - m_iStartListIndex + 1;
					}
					else
					{	
						index = 1;
						m_iStartListIndex = i;
					}

					UpdateSearchNPC();
					NPCSearchListClick(index);
					return;
				}
			}

			for (UInt i = 0; i <= (UInt)startSearchIndex; ++i)
			{
				if ( FindNPC(i, searchName, lang) )
				{			
					int index = 1;
					if  (i > (m_iStartListIndex + 16) )
					{
						if ((i + 17) < (int)m_NPCInfoLst.size())
						{

							index = 1;	
							m_iStartListIndex = i;
						}
						else 
						{

							m_iStartListIndex = (UInt32)m_NPCInfoLst.size() - 17;
							index = i - m_iStartListIndex + 1;
						}	
					}
					else if ( i >= m_iStartListIndex && (i < m_iStartListIndex + 17))
					{
						index = i - m_iStartListIndex + 1;
					}
					else
					{

						index = 1;
						m_iStartListIndex = i;
					}

					UpdateSearchNPC();
					NPCSearchListClick(index);
					return;
				}
			}
		}
	}
	else if (lang == 1) //search character
	{
		UIEditBox* searchBox ;
		searchBox = (UIEditBox*)gGlobal.pUIManager->GetUIItem("SearchChiTextBox");

		if (searchBox)
		{
			String searchName = searchBox->GetText();

			if (searchName.length() == 0)
				return;

			int startSearchIndex = 0;
			if (m_iSelectedNPCListIndex >= 0)
				startSearchIndex = m_iSelectedNPCListIndex;

			for (UInt i = (startSearchIndex + 1); i < m_CharInfoLst.size(); ++i)
			{
				if ( FindChar(i, searchName) )
				{
					int index = 1;
					if  (i > (m_iStartListIndex + 16) )
					{
						if ((i + 16) < (int)m_CharInfoLst.size())
						{
							index = 1;	
							m_iStartListIndex = i;
						}
						else 
						{
							m_iStartListIndex = (UInt32)m_CharInfoLst.size() - 17;
							index = i - m_iStartListIndex + 1;	
						}	
					}
					else if ( i >= m_iStartListIndex && (i < m_iStartListIndex + 17))
					{
						index = i - m_iStartListIndex + 1;
					}
					else
					{	
						index = 1;
						m_iStartListIndex = i;
					}

					UpdateSearchCharacter();
					NPCSearchListClick(index);
					
					return;
				}
			}

			for (UInt i = 0; i <= (UInt)startSearchIndex; ++i)
			{
				if ( FindChar(i, searchName) )
				{
					int index = 1;
					if  (i > (m_iStartListIndex + 16) )
					{
						if ((i + 17) < (int)m_CharInfoLst.size())
						{
							index = 1;	
							m_iStartListIndex = i;
						}
						else 
						{
							m_iStartListIndex = (UInt32)m_CharInfoLst.size() - 17;
							index = i - m_iStartListIndex + 1;
						}	
					}
					else if ( i >= m_iStartListIndex && (i < m_iStartListIndex + 17))
					{
						index = i - m_iStartListIndex + 1;
					}
					else
					{
						index = 1;
						m_iStartListIndex = i;
					}

					UpdateSearchCharacter();
					NPCSearchListClick(index);
					return;
				}
			}
		}
	}

	StringA imgName;
	UIImage* img = gGlobal.pUIManager->FindUIImage("NPCNameHighLight");
	if (img)
	{
		img->SetEnable(false);
		m_iSelectedNPCListIndex = -1;
	}
	
}

void CMenuSearchListProcess::Scroll()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("NPCSearchScrollBar");
	if (scrollBar && m_bShowNPClist)
	{
		if (m_NPCInfoLst.size() > 17)
		{
			UInt32 index = scrollBar->GetSelNum();
			if ((index + 16) < m_NPCInfoLst.size() )
			{
				if (index != m_iStartListIndex)
				{
					m_iStartListIndex = index;
					UpdateSearchNPC();
				}
			}
			else
			{
				if ( ((UInt32)m_NPCInfoLst.size() - 17) != m_iStartListIndex)
				{
					m_iStartListIndex = (UInt32)m_NPCInfoLst.size() - 17;
					UpdateSearchNPC();
				}
			}
		}
		else
			scrollBar->HomeScroll();
	}
}

void CMenuSearchListProcess::UpdateSearchTimer()
{
	if (gGlobal.pUIManager)
	{
		UIPanel* npcSearchList = gGlobal.pUIManager->FindUIPanel("NPCSearchPanel");
		UIEditBox* searchBox = gGlobal.pUIManager->FindUIEditBox("SearchTextBox");

		if (npcSearchList && searchBox)
		{
			String temp = searchBox->GetText();
			if (temp.length() > 0 )
			{
				if ( (timeGetTime() - m_iTimer) > 100)
					searchBox->ClearText();
			}
		}
	}
}

void CMenuSearchListProcess::ResetValue()
{
	m_iStartListIndex = 0;
	m_iSelectedNPCListIndex = -1;
	m_iTimer = 0;
}

void CMenuSearchListProcess::MakeCharSearchList()
{
	if (!NeedRemakeCharLst())
		return;

	UInt32CharCtrlPtrMap mapCharacter ;
	std::swap(mapCharacter, QGLApplication::GetInstance().GetMapCharacterLst() );
	UInt32CharCtrlPtrMap::iterator pCharItr = mapCharacter.begin();

	m_CharInfoLst.clear();
	UInt16 blockPosX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
	UInt16 blockPosY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
	while (pCharItr != mapCharacter.end())
	{
		if (pCharItr->second != NULL && gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id() != pCharItr->second->GetCharNetObject()->GetChar_id())
		{
			UInt32 distance = (UInt32)sqrt( pow( (double)((Int16)blockPosX - (Int16) pCharItr->second->GetCharMainView()->GetCharPos().PosX), (int)2) + pow( (double)((Int16)blockPosY - (Int16)pCharItr->second->GetCharMainView()->GetCharPos().PosY), (int)2) );
			if (distance < 32)
			{
				CharInfo charTemp;
				charTemp.Char_id = pCharItr->second->GetCharNetObject()->GetChar_id();
				charTemp.CharName = pCharItr->second->GetCharName();
				charTemp.Char_ptr = (UInt)pCharItr->second->GetCharNetObject();
				m_CharInfoLst.push_back(charTemp);
			}
		}
		pCharItr++;
	}

}

void CMenuSearchListProcess::UpdateSearchCharacter()
{
	m_bShowNPClist = false;
	UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("NPCSearchPanel");

	if (panel)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClearNPCList", String());
		MakeCharSearchList();

		int count = 0;
		StringA textIconName;

		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("NPCSearchScrollBar");
		if (scrollBar)
		{
			if (m_CharInfoLst.size() > 17)
				scrollBar->SetNumCount((UInt32)m_CharInfoLst.size());
		}

		while ( (int)m_CharInfoLst.size() > (count + m_iStartListIndex) && count < 17 )
		{
			textIconName.Format("NPCName%d", count + 1);
			UIStaticText* charNameText = (UIStaticText*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());
			textIconName.Format("NPCNamePos%d", count + 1);
			UIStaticText* charPosText = (UIStaticText*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());

			if (charNameText && charPosText)
			{
				String temp;
				temp.Format(_T("%s"),m_CharInfoLst[count + m_iStartListIndex].CharName.c_str() );
				charNameText->SetText(temp.c_str());

				temp.Empty();
				charPosText->SetColor(TextColor[ColorIndex_White]);				
				CCharacterControl* pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(m_CharInfoLst[count + m_iStartListIndex].Char_id);
				if (pCharCtrl)
				{
					if (pCharCtrl->HasParty())
					{
						if (pCharCtrl->IsLeader())
						{
							temp.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_PARTY_LEADER")));
							charPosText->SetColor(TextColor[ColorIndex_Yellow]);
						}
						else if (pCharCtrl->IsMember())
						{
							temp.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_PARTY_MEMBER")));
							charPosText->SetColor(TextColor[ColorIndex_LightBlue]);
						}
					}
				}

				charPosText->SetText(temp.c_str());
				charNameText->Redraw();
				charPosText->Redraw();
			}
			textIconName.Format("NPCNameHighLight%d", count+1);
			UIImage* button = (UIImage*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());
			button->SetEnable(true);
			++count;			
		}

		UIImage* buttonHighLight = (UIImage*)gGlobal.pUIManager->GetUIItem("NPCNameHighLight");
		if (buttonHighLight)
		{
			if (m_iSelectedNPCListIndex < (int)m_iStartListIndex || m_iSelectedNPCListIndex > (int)m_iStartListIndex + 16)
				buttonHighLight->SetEnable(false);
			else
			{
				int index = m_iSelectedNPCListIndex - m_iStartListIndex;
				textIconName.Format("NPCNameHighLight%d", index + 1);
				UIImage* button = (UIImage*)gGlobal.pUIManager->GetUIItem(textIconName.c_str());
				if (button)
				{
					button->SetEnable(false);
					buttonHighLight->SetOffsetXY(button->GetOffsetX(), button->GetOffsetY() );
					buttonHighLight->SetEnable(true);
				}
			}
		}
		gGlobal.pUIManager->AddDirtyUIItem(panel);
	}
}

bool CMenuSearchListProcess::NeedRemakeCharLst()
{
	if (m_bShowNPClist)
		return false;
	UInt32CharCtrlPtrMap mapCharacter ;
	std::swap(mapCharacter, QGLApplication::GetInstance().GetMapCharacterLst() );
	if (m_CharInfoLst.size() != mapCharacter.size())
		return true;

	for (UInt16 i = 0; i < m_CharInfoLst.size(); ++i)
	{
		if (mapCharacter.find(m_CharInfoLst[i].Char_ptr) == mapCharacter.end() )
			return true;
	}

	return false;

}

void CMenuSearchListProcess::ShowPartySearchList(int searchBy)
{
	m_sortMethod = searchBy;

	switch (m_sortMethod)
	{
	case PARTY_SORT_METHOD_DESC:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByDesc);
		break;
	case PARTY_SORT_METHOD_FACTION:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByFaction);
		break;
	case PARTY_SORT_METHOD_CCLASS:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByCclass);
		break;
	case PARTY_SORT_METHOD_LEVEL:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByLevel);
		break;
	case PARTY_SORT_METHOD_COUNT:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByCount);
		break;
	}

	UpdateSearchParty();
}

void CMenuSearchListProcess::ShowCharSearchList(int searchBy)
{
	m_sortMethodChar = searchBy;

	switch (m_sortMethodChar)
	{
	case PARTY_SORT_METHOD_FACTION:
		std::sort(m_CharInfoLst_new.begin(), m_CharInfoLst_new.end(), SortCharByFaction);
		break;
	case PARTY_SORT_METHOD_CCLASS:
		std::sort(m_CharInfoLst_new.begin(), m_CharInfoLst_new.end(), SortCharByCclass);
		break;
	case PARTY_SORT_METHOD_LEVEL:
		std::sort(m_CharInfoLst_new.begin(), m_CharInfoLst_new.end(), SortCharByLevel);
		break;
	}
	UpdateSearchChar();
}

void CMenuSearchListProcess::ShowCharSearchList(const CharInfoVector &CharInfoList)
{
	m_CharInfoLst_new.clear();
	m_CharInfoLst_new = CharInfoList;
	switch(m_sortMethodChar)
	{
		case PARTY_SORT_METHOD_DESC:
			{
				break;
			}
		case PARTY_SORT_METHOD_FACTION:
			{
				break;
			}
		case PARTY_SORT_METHOD_CCLASS:
			{
				break;
			}
		case PARTY_SORT_METHOD_LEVEL:
			{
				break;
			}
	}

	m_iSelectedCharListIndex = 0;
	m_iSelectedCharPageIndex = 1;
	UpdateSearchChar();
}

void CMenuSearchListProcess::ShowPartySearchList(const PartyInfoVector &PartyInfoLst)
{
	m_PartyInfoLst.clear();
	m_PartyInfoLst = PartyInfoLst;

	//OutputDebugString(m_PartyInfoLst[0].desc);
	switch (m_sortMethod)
	{
	case PARTY_SORT_METHOD_DESC:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByDesc);
		break;
	case PARTY_SORT_METHOD_FACTION:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByFaction);
		break;
	case PARTY_SORT_METHOD_CCLASS:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByCclass);
		break;
	case PARTY_SORT_METHOD_LEVEL:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByLevel);
		break;
	case PARTY_SORT_METHOD_COUNT:
		std::sort(m_PartyInfoLst.begin(), m_PartyInfoLst.end(), SortPatryByCount);
		break;
	}
	m_iSelectedPartyListIndex = 0;
	m_iSelectedPartyPageIndex = 1;
	UpdateSearchParty();
}

void CMenuSearchListProcess::UpdateSearchParty()	
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartySearchPanel");
	if (pPanel)
	{
		StringA IconName;
		UIStaticText* pStaticText(NULL);
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("PartySearchPageNo");
		if (pStaticText)
		{
			String PageNo;
			PageNo.Format(_T("%d/%d"), m_iSelectedPartyPageIndex, m_PartyInfoLst.size() / MAX_OF_SEARCH_PARTY_EACH_PAGE + 1);
			pStaticText->SetText(PageNo);
			pStaticText->Redraw();
		}
		for (UInt16 i = 1; i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; i++)
		{
			IconName.Format("PartyInfoName%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoFaction%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoCclass%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoLevel%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoCount%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoDesc%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
		}
		UInt16 offSetMin = (m_iSelectedPartyPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE;
		UInt16 offSetMax = m_iSelectedPartyPageIndex * MAX_OF_SEARCH_PARTY_EACH_PAGE;
		for (UInt16 i = offSetMin; i < m_PartyInfoLst.size() && i < offSetMax; i++)
		{
			IconName.Format("PartyInfoName%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				pStaticText->SetText(m_PartyInfoLst[i].nickName);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoFaction%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempFaction;
				switch ( m_PartyInfoLst[i].faction)
				{
				case 1:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SIXFANDOOR"));	break;
				case 2:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TWELVEDOCK"));	break;
				case 3:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_HOLY"));		break;
				case 4:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SILVERCASINO"));break;
				case 5:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TONGDOOR"));	break;
				case 6:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_10THOUSAND"));	break;
				case 7:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GREENCLOTH"));	break;
				case 8:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GHOSTAREA"));	break;

				}
				pStaticText->SetText(tempFaction);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoCclass%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempCclass;
				switch(m_PartyInfoLst[i].cclass)
				{
				case 1: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_1")); break;
				case 2: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_2")); break;
				case 3: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_3")); break;
				case 4: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_4")); break;
				case 5: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_5")); break;
				case 6: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_6")); break;
				case 7: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_7")); break;
				case 8: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_8")); break;
				}
				pStaticText->SetText(tempCclass);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoLevel%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempLevel;
				tempLevel.Format(_T("%d %s"), m_PartyInfoLst[i].level, gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
				pStaticText->SetText(tempLevel);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoCount%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempCount;
				tempCount.Format(_T("%d"), m_PartyInfoLst[i].count);
				pStaticText->SetText(tempCount);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoDesc%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				pStaticText->SetText(m_PartyInfoLst[i].desc);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
		}
		gGlobal.pUIManager->SetPanel2Top(pPanel);
	}
}

void CMenuSearchListProcess::UpdateSearchChar()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("CharSearchPanel");
	if (pPanel)
	{
		StringA IconName;
		UIStaticText* pStaticText(NULL);
		pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText("CharSearchPageNo");
		if (pStaticText)
		{
			String PageNo;
			PageNo.Format(_T("%d/%d"), m_iSelectedCharPageIndex, m_CharInfoLst_new.size() / MAX_OF_SEARCH_PARTY_EACH_PAGE + 1);
			pStaticText->SetText(PageNo);
			pStaticText->Redraw();
		}
		for (UInt16 i = 1; i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; i++)
		{
			IconName.Format("CharInfoName%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("CharInfoFaction%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("CharInfoCclass%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("CharInfoLevel%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("CharInfoCount%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("CharInfoDesc%02d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
		}
		UInt16 offSetMin = (m_iSelectedCharPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE;
		UInt16 offSetMax = m_iSelectedCharPageIndex * MAX_OF_SEARCH_PARTY_EACH_PAGE;
		for (UInt16 i = offSetMin; i < m_CharInfoLst_new.size() && i < offSetMax; i++)
		{
			IconName.Format("CharInfoName%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				pStaticText->SetText(m_CharInfoLst_new[i].nickName);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("CharInfoFaction%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempFaction;
				switch ( m_CharInfoLst_new[i].faction)
				{
				case 1:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SIXFANDOOR"));	break;
				case 2:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TWELVEDOCK"));	break;
				case 3:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_HOLY"));		break;
				case 4:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SILVERCASINO"));break;
				case 5:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TONGDOOR"));	break;
				case 6:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_10THOUSAND"));	break;
				case 7:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GREENCLOTH"));	break;
				case 8:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GHOSTAREA"));	break;

				}
				pStaticText->SetText(tempFaction);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("CharInfoCclass%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempCclass;
				switch(m_CharInfoLst_new[i].cclass)
				{
				case 1: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_1")); break;
				case 2: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_2")); break;
				case 3: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_3")); break;
				case 4: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_4")); break;
				case 5: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_5")); break;
				case 6: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_6")); break;
				case 7: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_7")); break;
				case 8: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_8")); break;
				}
				pStaticText->SetText(tempCclass);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("CharInfoLevel%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempLevel;
				tempLevel.Format(_T("%d %s"), m_CharInfoLst_new[i].level, gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
				pStaticText->SetText(tempLevel);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("CharInfoCount%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempCount;
				tempCount.Format(_T("%d"), 1);
				pStaticText->SetText(tempCount);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("CharInfoDesc%02d",i - offSetMin + 1);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempDesc;
				//tempDesc = "222";
				pStaticText->SetText(tempDesc);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
		}
		gGlobal.pUIManager->SetPanel2Top(pPanel);
	}
}
void CMenuSearchListProcess::UpdateSelectPartyInfo()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartySearchPanel");
	UIPanel* pSelectPanel = gGlobal.pUIManager->FindUIPanel("PartyInfoSelectPanel");
	if (pPanel && pSelectPanel && !m_SelectPartyInfoLst.empty())
	{
		Int PosX(pPanel->GetX() + pPanel->GetWidth());
		Int PosY(pPanel->GetY());
		if (PosX + pSelectPanel->GetWidth() > DefaultWinWidth)
		{
			PosX = pPanel->GetX() - pSelectPanel->GetWidth();
		}
		if (PosY + pSelectPanel->GetHeight() > DefaultWinHeight)
		{
			PosY = DefaultWinHeight - pSelectPanel->GetHeight();
		}
		if (PosY < 0)
		{
			PosY = 0;
		}
		if (PosX < 0)
		{
			PosX = 0;
		}
		pSelectPanel->SetPos(PosX, PosY);
		pSelectPanel->SetEnable(true);

		StringA IconName;
		UIStaticText* pStaticText(NULL);
		for (UInt16 i = 1; i <= 4; i++)
		{
			IconName.Format("PartyInfoSelectName%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoSelectLevel%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoSelectFaction%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
			IconName.Format("PartyInfoSelectCclass%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
				pStaticText->SetEnable(false);
		}
		for (UInt16 i = 1; i <= m_SelectPartyInfoLst.size(); i++)
		{
			IconName.Format("PartyInfoSelectName%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				pStaticText->SetText(m_SelectPartyInfoLst[i-1].nickName);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoSelectLevel%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempLevel;
				tempLevel.Format(_T("%d %s"), m_SelectPartyInfoLst[i-1].level, gGlobal.GetStringTable()->Get(_T("MSG_MANOR_LEVEL")));
				pStaticText->SetText(tempLevel);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoSelectFaction%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempFaction;
				switch ( m_SelectPartyInfoLst[i-1].faction)
				{
				case 1:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SIXFANDOOR"));	break;
				case 2:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TWELVEDOCK"));	break;
				case 3:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_HOLY"));		break;
				case 4:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SILVERCASINO"));break;
				case 5:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TONGDOOR"));	break;
				case 6:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_10THOUSAND"));	break;
				case 7:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GREENCLOTH"));	break;
				case 8:	tempFaction = gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GHOSTAREA"));	break;

				}
				pStaticText->SetText(tempFaction);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
			IconName.Format("PartyInfoSelectCclass%d",i);
			pStaticText = (UIStaticText*)gGlobal.pUIManager->FindUIStaticText(IconName.c_str());
			if (pStaticText)
			{
				String tempCclass;
				switch(m_SelectPartyInfoLst[i-1].cclass)
				{
				case 1: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_1")); break;
				case 2: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_2")); break;
				case 3: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_3")); break;
				case 4: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_4")); break;
				case 5: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_5")); break;
				case 6: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_6")); break;
				case 7: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_7")); break;
				case 8: tempCclass =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_8")); break;
				}
				pStaticText->SetText(tempCclass);
				pStaticText->SetEnable(true);
				pStaticText->Redraw();
			}
		}
		gGlobal.pUIManager->SetPanel2Top(pSelectPanel);
	}
}

void CMenuSearchListProcess::ShowSelectPartyInfo(const PartyInfoVector &PartyInfoLst)
{
	m_SelectPartyInfoLst.clear();
	m_SelectPartyInfoLst = PartyInfoLst;
	UpdateSelectPartyInfo();
}

void CMenuSearchListProcess::OnShowPartyInfo()
{
	m_PartyInfoLst.clear();
	m_SelectPartyInfoLst.clear();
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		gGlobal.g_pCharacter->GetCharNetObject()->SearchParty(gGlobal.pPeerProxy,gGlobal.g_context);
	}
}

void CMenuSearchListProcess::OnShowCharInfo()
{
	m_CharInfoLst_new.clear();
	//m_SelectPartyInfoLst.clear();
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
	{
		gGlobal.g_pCharacter->GetCharNetObject()->SearchChar(gGlobal.pPeerProxy,gGlobal.g_context);
	}
}


void CMenuSearchListProcess::ShowLeaderInfo()
{
	UInt16 indexReal = (m_iSelectedPartyPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + m_iSelectedPartyListIndex - 1;
	if (indexReal < m_PartyInfoLst.size())
	{
		MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, RPCContext(), m_PartyInfoLst[indexReal].char_id, String(), false, true);
	}
}

void CMenuSearchListProcess::ShowLeaderInfoChar()
{
	UInt16 indexReal = (m_iSelectedCharPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + m_iSelectedPartyListIndex - 1;
	if (indexReal < m_CharInfoLst_new.size())
	{
		MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, RPCContext(), m_CharInfoLst_new[indexReal].char_id, String(), false, true);
	}
}

void CMenuSearchListProcess::PartySearchClickList(int index)
{
	if (((m_iSelectedPartyPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + index) > m_PartyInfoLst.size())
		return;
	if (index > 0 && index <= MAX_OF_SEARCH_PARTY_EACH_PAGE)
	{
		StringA buttonName;
		buttonName.Format("PartyInfoHighLight%d", index);
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("PartySearchPanel");
		UIImage* button = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());

		for (Index i = C_INDEX(1); i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; ++i)
		{
			buttonName.Format("PartyInfoHighLight%d", i);
			UIImage* prevButton = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());

			if (prevButton)
				prevButton->SetEnable(true);
		}

		if (gGlobal.g_pCharacter &&
			gGlobal.g_pCharacter->GetCharNetObject() &&
			m_iSelectedPartyPageIndex > 0)
		{
			UInt16 indexReal = (m_iSelectedPartyPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + index - 1;
			if (indexReal < m_PartyInfoLst.size())
			{
				gGlobal.g_pCharacter->GetCharNetObject()->GetPartyInfo(gGlobal.pPeerProxy,
					gGlobal.g_context, m_PartyInfoLst[indexReal].netID);
			}
		}

		if (panel && button)
		{	
			m_iSelectedPartyListIndex = index;
			button->SetEnable(false);
			UIImage* buttonHighLight = (UIImage*)gGlobal.pUIManager->GetUIItem("PartyInfoHighLight");
			if (buttonHighLight)
			{
				buttonHighLight->SetOffsetXY(button->GetOffsetX(), button->GetOffsetY());
				buttonHighLight->SetEnable(true);
			}
		}
	}
}


void CMenuSearchListProcess::CharSearchClickList(int index)
{
	if (((m_iSelectedCharPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + index) > m_CharInfoLst_new.size())
		return;
	if (index > 0 && index <= MAX_OF_SEARCH_PARTY_EACH_PAGE)
	{
		StringA buttonName;
		buttonName.Format("CharInfoHighLight%d", index);
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("CharSearchPanel");
		UIImage* button = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());

		for (Index i = C_INDEX(1); i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; ++i)
		{
			buttonName.Format("CharInfoHighLight%d", i);
			UIImage* prevButton = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());

			if (prevButton)
				prevButton->SetEnable(true);
		}

		//if (gGlobal.g_pCharacter &&
		//	gGlobal.g_pCharacter->GetCharNetObject() &&
		//	m_iSelectedCharPageIndex > 0)
		//{
		//	UInt16 indexReal = (m_iSelectedCharPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + index - 1;
		//	if (indexReal < m_CharInfoLst_new.size())
		//	{
		//		gGlobal.g_pCharacter->GetCharNetObject()->GetPartyInfo(gGlobal.pPeerProxy,
		//			gGlobal.g_context, m_CharInfoLst_new[indexReal].netID);
		//	}
		//}

		if (panel && button)
		{	
			m_iSelectedCharListIndex = index;
			button->SetEnable(false);
			UIImage* buttonHighLight = (UIImage*)gGlobal.pUIManager->GetUIItem("CharInfoHighLight");
			if (buttonHighLight)
			{
				buttonHighLight->SetOffsetXY(button->GetOffsetX(), button->GetOffsetY());
				buttonHighLight->SetEnable(true);
			}
		}
	}
}
void CMenuSearchListProcess::OnPartyInfoNextPage()
{
	if (m_iSelectedPartyPageIndex < (m_PartyInfoLst.size() / MAX_OF_SEARCH_PARTY_EACH_PAGE + 1))
	{
		m_iSelectedPartyPageIndex ++;
		m_iSelectedPartyListIndex = 0;
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("PartySearchPanel");
		if (panel)
		{
			StringA buttonName;
			UIImage* pButton;
			for (Index i = C_INDEX(1); i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; ++i)
			{
				buttonName.Format("PartyInfoHighLight%d", i);
				pButton = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());
				if (pButton)
					pButton->SetEnable(true);
			}
			pButton = (UIImage*)gGlobal.pUIManager->GetUIItem("PartyInfoHighLight");
			if (pButton)
			{
				pButton->SetEnable(false);
			}
		}
		UpdateSearchParty();
	}
}

void CMenuSearchListProcess::OnCharInfoNextPage()
{
	if (m_iSelectedCharPageIndex < (m_CharInfoLst_new.size() / MAX_OF_SEARCH_PARTY_EACH_PAGE + 1))
	{
		m_iSelectedCharPageIndex ++;
		m_iSelectedCharListIndex = 0;
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("CharSearchPanel");
		if (panel)
		{
			StringA buttonName;
			UIImage* pButton;
			for (Index i = C_INDEX(1); i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; ++i)
			{
				buttonName.Format("CharInfoHighLight%d", i);
				pButton = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());
				if (pButton)
					pButton->SetEnable(true);
			}
			pButton = (UIImage*)gGlobal.pUIManager->GetUIItem("CharInfoHighLight");
			if (pButton)
			{
				pButton->SetEnable(false);
			}
		}
		UpdateSearchChar();
	}
}

void CMenuSearchListProcess::OnPartyInfoPrevPage()
{
	if (m_iSelectedPartyPageIndex > 1)
	{
		m_iSelectedPartyPageIndex --;
		m_iSelectedPartyListIndex = 0;
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("PartySearchPanel");
		if (panel)
		{
			StringA buttonName;
			UIImage* pButton;
			for (Index i = C_INDEX(1); i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; ++i)
			{
				buttonName.Format("PartyInfoHighLight%d", i);
				pButton = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());
				if (pButton)
					pButton->SetEnable(true);
			}
			pButton = (UIImage*)gGlobal.pUIManager->GetUIItem("PartyInfoHighLight");
			if (pButton)
			{
				pButton->SetEnable(false);
			}
		}
		UpdateSearchParty();
	}
}

void CMenuSearchListProcess::OnCharInfoPrevPage()
{
	if (m_iSelectedCharPageIndex > 1)
	{
		m_iSelectedCharPageIndex --;
		m_iSelectedCharListIndex = 0;
		UIPanel* panel = (UIPanel*)gGlobal.pUIManager->GetUIItem("CharSearchPanel");
		if (panel)
		{
			StringA buttonName;
			UIImage* pButton;
			for (Index i = C_INDEX(1); i <= MAX_OF_SEARCH_PARTY_EACH_PAGE; ++i)
			{
				buttonName.Format("CharInfoHighLight%d", i);
				pButton = (UIImage*)gGlobal.pUIManager->GetUIItem(buttonName.c_str());
				if (pButton)
					pButton->SetEnable(true);
			}
			pButton = (UIImage*)gGlobal.pUIManager->GetUIItem("CharInfoHighLight");
			if (pButton)
			{
				pButton->SetEnable(false);
			}
		}
		UpdateSearchChar();
	}
}

void CMenuSearchListProcess::OnJoinParty()
{
	if (gGlobal.g_pCharacter &&
		gGlobal.g_pCharacter->GetCharNetObject() &&
		m_iSelectedPartyPageIndex > 0 &&
		m_iSelectedPartyListIndex > 0)
	{
		UInt16 index = (m_iSelectedPartyPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + m_iSelectedPartyListIndex - 1;
		if (index < m_PartyInfoLst.size())
		{
			gGlobal.g_pCharacter->GetCharNetObject()->PartyJoinRequest(gGlobal.pPeerProxy,
				gGlobal.g_context, m_PartyInfoLst[index].netID);
		}
	}
}

void CMenuSearchListProcess::OnJoinChar()
{
	if (gGlobal.g_pCharacter &&
		gGlobal.g_pCharacter->GetCharNetObject() &&
		m_iSelectedCharPageIndex > 0 &&
		m_iSelectedCharListIndex > 0)
	{
		UInt16 index = (m_iSelectedCharPageIndex - 1) * MAX_OF_SEARCH_PARTY_EACH_PAGE + m_iSelectedCharListIndex - 1;
		if (index < m_CharInfoLst_new.size())
		{
			QGLApplication::GetInstance().InviteParty(m_CharInfoLst_new[index].char_id);
		}
	}
}