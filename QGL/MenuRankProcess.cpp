//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuRankProcess.h"
//-- Library
#include "Global.h"
#include "QGLApplication.h"
#include "Proxy/Logic/RankCharacter.h"
#include "Proxy/Logic/RankGuild.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
#include "UI.h"

CMenuRankProcess::CMenuRankProcess()
{
	m_currGrp = NULL;
	m_StartIndex = 0;
	m_SelIndex = -1;
	m_CurRankSubType = 0;
	m_GroupStartIndex = 0;
	m_GroupEndIndex = 0;
}

CMenuRankProcess::~CMenuRankProcess()
{
	Release();
}

void CMenuRankProcess::Release()
{
	if (m_currGrp)
		DeleteNetGroup(m_currGrp, BOOLEAN_TRUE);
}

void CMenuRankProcess::ConstructDataGuild(NetGroup * givenGrp)
{
	if (m_currGrp && m_currGrp != givenGrp)
	{
		for (UInt i = 0; i < m_currGrp->GetCount(); i++)
		{
			if (!givenGrp->IsChild(m_currGrp->GetChild(i)))
			{
				NetObject * Obj = m_currGrp->GetChild(i);
				DeleteNetObject(Obj);
			}
		}
		
		DeleteNetGroup(m_currGrp, BOOLEAN_FALSE);
	}

	m_currGrp = givenGrp;
	m_CurRankType = GUILD_RANK_TYPE;
	m_CurRankSubType = -1;

	m_GroupStartIndex = 0;
	m_GroupEndIndex = m_currGrp->GetCount();

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("RankPanelScroll");
	if (scrollBar)
	{
		scrollBar->SetSelNum(0);
		scrollBar->SetNumCount(GetRankSize());
	}
}
void CMenuRankProcess::ConstructData(NetGroup * givenGrp, UInt8 rankType)
{
	if (m_currGrp && m_currGrp != givenGrp)
	{
		for (UInt i = 0; i < m_currGrp->GetCount(); i++)
		{
			if (!givenGrp->IsChild(m_currGrp->GetChild(i)))
			{
				NetObject * Obj = m_currGrp->GetChild(i);
				DeleteNetObject(Obj);
			}
		}
		
		DeleteNetGroup(m_currGrp, BOOLEAN_FALSE);
	}

	m_currGrp = givenGrp;
	m_CurRankType = rankType;
	m_CurRankSubType = -1;

	m_StartIndex = 0;
	m_SelIndex = -1;
	if (rankType == CHAR_RANK_TYPE_CLASS ||
		rankType == CHAR_RANK_TYPE_FACTION)
		SetRankSubType(1);
	else
	{
		m_GroupStartIndex = 0;
		m_GroupEndIndex = m_currGrp->GetCount();
	}

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("RankPanelScroll");
	if (scrollBar)
	{
		scrollBar->SetSelNum(0);
		scrollBar->SetNumCount(GetRankSize());
	}
}

void CMenuRankProcess::HideAllCharData()
{
	for (Int i = 1; i <= 10; i ++)
	{
		StringA name;
		name.Format("RankText%dNumber", i);
		UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetEnable(false);
		}

		name.Format("RankText%dName", i);
		pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetEnable(false);
		}

		name.Format("RankText%dValue", i);
		pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetEnable(false);
		}

		name.Format("RankText%dGuild", i);
		pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetEnable(false);
		}

		name.Format("RankText%dClass", i);
		pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
		if (pText)
		{
			pText->SetEnable(false);
		}
	}


}
void CMenuRankProcess::HideAllValue()
{
	UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankTypeLevel");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankTypeMoney");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankTypeClass");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankTypeFamous");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankTypeEvil");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankTypeFaction");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankTypeCouple");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankLevel");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankMoney");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankHonor");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankClass");
	if (pImage)
		pImage->SetEnable(false);
	
	pImage = gGlobal.pUIManager->FindUIImage("RankPKValue");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankGuild");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankGuildMaster");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankPvPScore");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankTypeFaction");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankTypePvP");
	if (pImage)
		pImage->SetEnable(false);

	pImage = gGlobal.pUIManager->FindUIImage("RankTypePvPGuild");
	if (pImage)
		pImage->SetEnable(false);
	
	HideAllCharData();
	
	for (Int i = 1; i <= 8; i++)
	{
		StringA name;
		name.Format("RankFaction%d", i);
		UIButton * pBu = gGlobal.pUIManager->FindUIButton(name.c_str());
		if (pBu)
			pBu->SetEnable(false);

		name.Format("RankClass%d", i);
		pBu = gGlobal.pUIManager->FindUIButton(name.c_str());
		if (pBu)
			pBu->SetEnable(false);
	}
}

void CMenuRankProcess::ConstructSubData()
{
	if (m_currGrp == NULL)
		return;

	m_GroupStartIndex = 0;
	m_GroupEndIndex = 0;
	bool first = false;
	if (m_CurRankType == CHAR_RANK_TYPE_CLASS)
	{
		
		for (UInt i = 0; i < m_currGrp->GetCount(); i++)
		{
			CRankCharacter * pRkChar =  m_currGrp->GetChild<CRankCharacter>(i);
			if (pRkChar)
			{
				if (!first)
				{
					if (pRkChar->GetSexClass() == m_CurRankSubType)
					{
						m_GroupStartIndex = i;
						m_GroupEndIndex = i + 1;
						first = true;
					}
				}
				else
				{
					m_GroupEndIndex = i;
					if (pRkChar->GetSexClass() != m_CurRankSubType)
					{
						break;
					}		
				}
				
			}
		}
	}
	else if (m_CurRankType == CHAR_RANK_TYPE_FACTION)
	{
		for (UInt i = 0; i < m_currGrp->GetCount(); i++)
		{
			CRankCharacter * pRkChar =  m_currGrp->GetChild<CRankCharacter>(i);
			if (pRkChar)
			{
				if (!first)
				{
					if (pRkChar->GetFaction() == m_CurRankSubType)
					{
						m_GroupStartIndex = i;
						m_GroupEndIndex = i + 1;
						first = true;
					}
				}
				else
				{
					m_GroupEndIndex = i;
					if (pRkChar->GetFaction() != m_CurRankSubType)
					{
						break;
					}
				}
				
			}
		}
	}
}
void CMenuRankProcess::SetRankSubType(Int index)
{
	if (m_CurRankSubType != index && index >= 1 && index <= 8)
	{
		m_CurRankSubType = index;
		m_SelIndex = -1;
		m_StartIndex = 0;
		if (m_CurRankType == CHAR_RANK_TYPE_CLASS)
		{
			for (Int i = 1; i <= 8; i++)
			{
				StringA name;
				name.Format("RankClass%d", i);
				UIButton * pBu = gGlobal.pUIManager->FindUIButton(name.c_str());
				if (pBu)
				{
					pBu->SetEnable(true);
					if (i == m_CurRankSubType)
					{
						pBu->SetState(_ButtonNotice);
						pBu->SetLockState(true);
					}
					else
					{
						pBu->SetLockState(false);
						pBu->SetNormalState();
					}
				}
			}
		}
		else if (m_CurRankType == CHAR_RANK_TYPE_FACTION)
		{
			for (Int i = 1; i <= 8; i++)
			{
				StringA name;
				name.Format("RankFaction%d", i);
				UIButton * pBu = gGlobal.pUIManager->FindUIButton(name.c_str());
				if (pBu)
				{
					pBu->SetEnable(true);
					if (i == m_CurRankSubType)
					{
						pBu->SetState(_ButtonNotice);
						pBu->SetLockState(true);
					}
					else
					{
						pBu->SetLockState(false);
						pBu->SetNormalState();
					}
				}
			}
		}
		UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("RankPanelScroll");
		if (scrollBar)
		{
			scrollBar->SetSelNum(0);
		}
		ConstructSubData();
		HideAllCharData();
		RefreshMenu();
	}
}

void CMenuRankProcess::InitMenu()
{
	HideAllValue();

	if (m_CurRankType == CHAR_RANK_TYPE_LEVEL)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankLevel");
		if (pImage)
			pImage->SetEnable(true);
		pImage = gGlobal.pUIManager->FindUIImage("RankTypeLevel");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
		if (pImage)
			pImage->SetEnable(true);
	}
	else if (m_CurRankType == CHAR_RANK_TYPE_MONEY)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankMoney");
		if (pImage)
			pImage->SetEnable(true);
		pImage = gGlobal.pUIManager->FindUIImage("RankTypeMoney");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
		if (pImage)
			pImage->SetEnable(true);
	}
	else if (m_CurRankType == CHAR_RANK_TYPE_CLASS)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankLevel");
		if (pImage)
			pImage->SetEnable(true);
		pImage = gGlobal.pUIManager->FindUIImage("RankTypeClass");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
		if (pImage)
			pImage->SetEnable(true);

		for (Int i = 1; i <= 8; i++)
		{
			StringA name;
			name.Format("RankClass%d", i);
			UIButton * pBu = gGlobal.pUIManager->FindUIButton(name.c_str());
			if (pBu)
				pBu->SetEnable(true);
		}
	}
	else if (m_CurRankType == CHAR_RANK_TYPE_FAMOUS)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankHonor");
		if (pImage)
			pImage->SetEnable(true);
		pImage = gGlobal.pUIManager->FindUIImage("RankTypeFamous");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
		if (pImage)
			pImage->SetEnable(true);
	}
	else if (m_CurRankType == CHAR_RANK_TYPE_EVIL)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankPKValue");
		if (pImage)
			pImage->SetEnable(true);
		pImage = gGlobal.pUIManager->FindUIImage("RankTypeEvil");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
		if (pImage)
			pImage->SetEnable(true);
	}
	else if (m_CurRankType == CHAR_RANK_TYPE_FACTION)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankLevel");
		if (pImage)
			pImage->SetEnable(true);
		pImage = gGlobal.pUIManager->FindUIImage("RankTypeFaction");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
		if (pImage)
			pImage->SetEnable(true);

		

		for (Int i = 1; i <= 8; i++)
		{
			StringA name;
			name.Format("RankFaction%d", i);
			UIButton * pBu = gGlobal.pUIManager->FindUIButton(name.c_str());
			if (pBu)
				pBu->SetEnable(true);
		}
	}
	else if (m_CurRankType == CHAR_RANK_TYPE_PVPSCROE)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankTypePvP");
		if (pImage)
			pImage->SetEnable(true);
		
		pImage = gGlobal.pUIManager->FindUIImage("RankPvPScore");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankClass");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankFaction");
		if (pImage)
			pImage->SetEnable(true);
		
	}
	else if (m_CurRankType == GUILD_RANK_TYPE)
	{
		UIImage * pImage = gGlobal.pUIManager->FindUIImage("RankTypePvPGuild");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankNickName");
		if (pImage)
			pImage->SetEnable(false);

		pImage = gGlobal.pUIManager->FindUIImage("RankGuild");
		if (pImage)
			pImage->SetEnable(true);
		pImage = gGlobal.pUIManager->FindUIImage("RankPvPScore");
		if (pImage)
			pImage->SetEnable(true);

		pImage = gGlobal.pUIManager->FindUIImage("RankGuildMaster");
		if (pImage)
			pImage->SetEnable(true);
	}
}
void CMenuRankProcess::RefreshMenu()
{
	if (m_currGrp == NULL)
		return;

	//for (rankMap::iterator Itr = m_CurrDisplayRank.begin(); 
	//	Itr != m_CurrDisplayRank.end() && i < m_StartIndex + 10; ++Itr, ++i)
	if (m_CurRankType == GUILD_RANK_TYPE)
	{
		for (UInt i = m_StartIndex;
			i < GetRankSize() ; ++i)
		{
			//if (i >= m_StartIndex)
			CRankGuild * pRkGuild =  m_currGrp->GetChild<CRankGuild>(i + m_GroupStartIndex);
			if (pRkGuild)
			{
				UInt Num = i - m_StartIndex + 1;

				StringA name;
				String text;
				name.Format("RankText%dNumber", Num);
				UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					text.Format(_T("%d"), i + 1);
					pText->SetText(text.c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}

				name.Format("RankText%dName", Num);
				pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					pText->SetText(pRkGuild->GetName().c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}

				name.Format("RankText%dValue", Num);
				pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					String text;
					text.Format(_T("%d"), pRkGuild->GetGuildPvPScore());
					pText->SetText(text.c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}

				name.Format("RankText%dGuild", Num);
				pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					pText->SetText(pRkGuild->GetMaster().c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}

			}
		}
	}
	else
	{
		for (UInt i = m_StartIndex;
			i < GetRankSize() ; ++i)
		{
			//if (i >= m_StartIndex)
			CRankCharacter * pRkChar =  m_currGrp->GetChild<CRankCharacter>(i + m_GroupStartIndex);
			if (pRkChar)
			{
				UInt Num = i - m_StartIndex + 1;

				StringA name;
				String text;
				name.Format("RankText%dNumber", Num);
				UIStaticText * pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					text.Format(_T("%d"), i + 1);
					pText->SetText(text.c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}

				name.Format("RankText%dName", Num);
				pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					pText->SetText(pRkChar->GetNickName().c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}


				name.Format("RankText%dValue", Num);
				pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					if (m_CurRankType == CHAR_RANK_TYPE_LEVEL ||
						m_CurRankType == CHAR_RANK_TYPE_CLASS ||
						m_CurRankType == CHAR_RANK_TYPE_FACTION)
					{
						text.Format(_T("%d"), pRkChar->GetLevel());
						pText->SetText(text.c_str());
					}
					else if (m_CurRankType == CHAR_RANK_TYPE_MONEY)
					{
						text.Empty();
						String temp;
						temp.Format(_T("%d"), pRkChar->GetMoney() + pRkChar->GetBankMoney());
						Int startPos = temp.GetLength() - 3;
						while (true)
						{
							if (startPos <= 0)
							{
								UInt num = 3 + startPos;
								startPos = 0;
								String subStr = temp.substr(startPos, num);
								if (text.IsEmpty())
									text =  subStr.c_str();
								else
								{
									text.Format(_T("%s,%s"), subStr.c_str(), text.c_str());
								}
								break;
							}
							else
							{
								String subStr = temp.substr(startPos, 3);
								if (text.IsEmpty())
									text =  subStr.c_str();
								else
								{
									text.Format(_T("%s,%s"), subStr.c_str(), text.c_str());
								}
							}

							startPos -= 3;
						}
						pText->SetText(text.c_str());
					}
					else if (m_CurRankType == CHAR_RANK_TYPE_FAMOUS)
					{
						text.Format(_T("%d"), pRkChar->GetHonor());
						pText->SetText(text.c_str());
					}
					else if (m_CurRankType == CHAR_RANK_TYPE_EVIL)
					{
						text.Format(_T("%d"), pRkChar->GetPK_Value());
						pText->SetText(text.c_str());
					}
					else if (m_CurRankType == CHAR_RANK_TYPE_PVPSCROE)
					{
						text.Format(_T("%d"), pRkChar->GetPvPScoreSingle());
						pText->SetText(text.c_str());
					}
					pText->Redraw();
					pText->SetEnable(true);
				}

				name.Format("RankText%dGuild", Num);
				pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());
				if (pText)
				{
					switch ( pRkChar->GetFaction())
					{
					case 1:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SIXFANDOOR"));
						break;
					case 2:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TWELVEDOCK"));
						break;
					case 3:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_HOLY"));
						break;
					case 4:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_SILVERCASINO"));
						break;
					case 5:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_TONGDOOR"));
						break;
					case 6:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_10THOUSAND"));
						break;
					case 7:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GREENCLOTH"));
						break;
					case 8:
						text =gGlobal.GetStringTable()->Get(_T("MSG_FACTION_GHOSTAREA"));
						break;
					default:
						text =gGlobal.GetStringTable()->Get(_T("MSG_ACTOR_CLASS_NONE"));
						break;
					}
					pText->SetText(text.c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}

				name.Format("RankText%dClass", Num);
				pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());

				if (pText)
				{
					String tempClassName;
					tempClassName.Format(_T("MSG_ACTOR_CLASS_%d"), pRkChar->GetSexClass());
					text = gGlobal.GetStringTable()->Get(tempClassName.c_str());
					pText->SetText(text.c_str());
					pText->Redraw();
					pText->SetEnable(true);
				}
			}
		}

	}
	if (m_SelIndex >= (Int)m_StartIndex && m_SelIndex < (Int)m_StartIndex + 10 )
	{
		UIImage * pIm = gGlobal.pUIManager->FindUIImage("RankHightLight");
		if (pIm)
		{
			UInt TargetX = 9;
			UInt TargetY = 95 + 26 * (m_SelIndex - m_StartIndex);
			pIm->SetOffsetXY(TargetX, TargetY);
			pIm->SetEnable(true);
		}
	}
	else
	{
		UIImage * pIm = gGlobal.pUIManager->FindUIImage("RankHightLight");
		if (pIm)
			pIm->SetEnable(false);
	}

}

void CMenuRankProcess::OpenMenu()
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenRankPanel", String());
}

void CMenuRankProcess::PageUp()
{
	if (m_StartIndex > 0)
	{
		m_StartIndex --;
		RefreshMenu();
	}
}
void CMenuRankProcess::PageDown()
{
	if (m_currGrp == NULL)
		return;

	if (m_StartIndex + 10 < GetRankSize())
	{
		m_StartIndex ++;
		RefreshMenu();
	}
}
void CMenuRankProcess::SetIndex(Int index)
{
	if (m_currGrp == NULL)
		return;
	if (index >= 0)
	{
		if (index + 10 > (Int)GetRankSize())
		{
			m_StartIndex = GetRankSize() - 10;
		}
		else
		{
			m_StartIndex = index;
		}
		RefreshMenu();
	}
}

void CMenuRankProcess::RankScroll()
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("RankPanelScroll");
	if (scrollBar)
	{
		SetIndex(scrollBar->GetSelNum());
	}
}

void CMenuRankProcess::FindRank(PCSTRINGW name)
{
	if (m_currGrp == NULL)
		return;
	UInt searchCount = 0;
	UInt searchStart = m_SelIndex + 1 + m_GroupStartIndex;
	UInt currIndex = ((searchStart - m_GroupStartIndex) >= GetRankSize())? m_GroupStartIndex : searchStart;
	
	//rankMap::iterator Itr = m_CurrDisplayRank.begin();
	//while (Itr != m_CurrDisplayRank.end())
	//for (rankMap::iterator Itr = m_CurrDisplayRank.begin(); Itr != m_CurrDisplayRank.end(); ++Itr, ++currIndex)
//	for (UInt currIndex = searchStart; currIndex < m_GroupEndIndex; currIndex ++)
	for (UInt searchCount = 0; searchCount < GetRankSize(); ++currIndex)
	{
		//if (currIndex >= searchStart)
		CRankCharacter * pRkChar =  m_currGrp->GetChild<CRankCharacter>(currIndex);
		if (pRkChar)
		{
			if (pRkChar->GetNickName().find(name) != std::string::npos)
			{
				m_SelIndex = currIndex - m_GroupStartIndex;
				SetIndex(m_SelIndex);
				return;
			}
//			searchCount ++;
		}
		searchCount ++;	

//		if (searchCount >= GetRankSize())
//			break;

		if ((currIndex - m_GroupStartIndex + 1) >= GetRankSize())
		{
			currIndex = m_GroupStartIndex - 1;
			//searchStart = m_GroupStartIndex;
			//Itr = m_CurrDisplayRank.begin();
		}

	}
	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_RANK_SEARCH_FAIL")));
}

void CMenuRankProcess::SetSelIndex(Int index)
{
	m_SelIndex = m_StartIndex + index;
}
