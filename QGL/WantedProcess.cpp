//-- Common
#include "Common.h"
#include "Common/CallLuaFunc.h"
#include "QGL.h"
//-- Self
#include "WantedProcess.h"
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Proxy/Logic/Character.h"
#include "QGL/CharacterControl.h"
#include "Proxy/Logic/RankCharacter.h"
#include "Proxy/Service/MessageService.h"
#include "Proxy/Service/WorldService.h"
#define MAX_WANTED_PER_PAGE 10

CWantedProcess::CWantedProcess()
{
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
	m_nSelectedClass = 3;
	m_grpWantedClass3 = CreateNetGroup();
	m_grpWantedClass2 = CreateNetGroup();
	m_grpWantedClass1 = CreateNetGroup();
}

CWantedProcess::~CWantedProcess()
{
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
	DeleteNetGroup(m_grpWantedClass3,BOOLEAN_TRUE);
	DeleteNetGroup(m_grpWantedClass2,BOOLEAN_TRUE);
	DeleteNetGroup(m_grpWantedClass1,BOOLEAN_TRUE);
}
void CWantedProcess::SelectWantedClass(UInt32 nClass)
{
	if(nClass <1 || nClass>3)
		return;
	m_nSelectedClass = nClass;
	m_iSelectedIndex = -1;

	for (Int i = 1; i <= 3; i++)
	{
		StringA name;
		name.Format("WantedButtonClass%d", i);
		UIButton * pBu = gGlobal.pUIManager->FindUIButton(name.c_str());
		if (pBu)
		{
			pBu->SetEnable(true);
			if (i == m_nSelectedClass)
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

	RefreshWantedList();
}
void CWantedProcess::OpenPanel(NetGroup	* netGroup)
{
	ClearAllWanted();
	for(int i = 0; i < netGroup->GetCount(); i++)
	{
		CRankCharacter*pRank = CastNetObject<CRankCharacter>(netGroup->GetChild(i));
		if (pRank)
		{
			UInt32 nPkValue = pRank->GetPK_Value();
			if(nPkValue >= 6 && nPkValue<= 10)
			{
				m_grpWantedClass3->Add(pRank);
			}
			if(nPkValue >= 11 && nPkValue<= 15)
			{
				m_grpWantedClass2->Add(pRank);
			}
			if(nPkValue >= 16 && nPkValue<= 20)
			{
				m_grpWantedClass1->Add(pRank);
			}
		}
	}
	DeleteNetGroup(netGroup);
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenWantedPanel", String());
	SelectWantedClass(m_nSelectedClass);
}
void CWantedProcess::ClearAllWanted()
{
	for(int i = 0; i < m_grpWantedClass3->GetCount(); i++)
	{
		NetObject* p = m_grpWantedClass3->GetChild(i);
		DeleteNetObject(p);
	}
	for(int i = 0; i < m_grpWantedClass2->GetCount(); i++)
	{
		NetObject* p = m_grpWantedClass2->GetChild(i);
		DeleteNetObject(p);
	}
	for(int i = 0; i < m_grpWantedClass1->GetCount(); i++)
	{
		NetObject* p = m_grpWantedClass1->GetChild(i);
		DeleteNetObject(p);
	}
	m_grpWantedClass3->Clear();
	m_grpWantedClass2->Clear();
	m_grpWantedClass1->Clear();
	m_iStartIndex = 0;
	m_iSelectedIndex = -1;
}
NetGroup* CWantedProcess::GetWantedList()
{
	switch(m_nSelectedClass)
	{
	case 1:
		return m_grpWantedClass1;
	case 2:
		return m_grpWantedClass2;
	case 3:
		return m_grpWantedClass3;
	default:
		return NULL;
	}
	return NULL;
}


void CWantedProcess::RefreshWantedList()
{
	StringA iconName;
	UIImage* pImage = NULL;
	NetGroup* pWantedList = GetWantedList();
	if(pWantedList == NULL)
		return;

	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("WantedScroll");
	if (scrollBar)
	{
		if (pWantedList->GetCount() > MAX_WANTED_PER_PAGE)
		{
			scrollBar->SetEnable(true);
			scrollBar->SetNumCount((UInt)(pWantedList->GetCount()) - MAX_WANTED_PER_PAGE);
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

	UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("WantedSelected");
	if (pHighLight)
	{
		if (pWantedList->GetCount() > 0 )
		{
			if (m_iSelectedIndex >= (int)m_iStartIndex && m_iSelectedIndex < (int)pWantedList->GetCount()
				&& m_iSelectedIndex < (int)(m_iStartIndex + MAX_WANTED_PER_PAGE) )
			{
				iconName.Format("WantedSelect%02d", (m_iSelectedIndex - m_iStartIndex));
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

	for (Index i = C_INDEX(0); i < MAX_WANTED_PER_PAGE; ++i)
	{
		iconName.Format("WantedID%02d", i);
		UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		if (pText)
		{
			UInt index = m_iStartIndex + i;
			CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(index));
			String strTmp;
			if(pRank)
			{
				strTmp.Format(_T("%d"),pRank->GetChar_id());
			}
			
			if (!strTmp.IsEmpty())
			{			
				pText->SetText(strTmp.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			else
			{
				pText->SetEnable(false);
			}
		}

		iconName.Format("WantedName%02d", i);
		pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		if (pText)
		{
			UInt index = m_iStartIndex + i;
			CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(index));
			String strTmp;
			if(pRank)
			{
				strTmp = pRank->GetNickName();
			}

			if (!strTmp.IsEmpty())
			{			
				pText->SetText(strTmp.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			else
			{
				pText->SetEnable(false);
			}
		}

		iconName.Format("WantedLevel%02d", i);
		pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		if (pText)
		{
			UInt index = m_iStartIndex + i;
			CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(index));
			String strTmp;
			if(pRank)
			{
				strTmp.Format(_T("%d"),pRank->GetLevel());
			}

			if (!strTmp.IsEmpty())
			{			
				pText->SetText(strTmp.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			else
			{
				pText->SetEnable(false);
			}
		}

		iconName.Format("WantedPKValue%02d", i);
		pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		if (pText)
		{
			UInt index = m_iStartIndex + i;
			CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(index));
			String strTmp;
			if(pRank)
			{
				strTmp.Format(_T("%d"),pRank->GetPK_Value());
			}

			if (!strTmp.IsEmpty())
			{			
				pText->SetText(strTmp.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			else
			{
				pText->SetEnable(false);
			}
		}

		iconName.Format("WantedReward%02d", i);
		pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());
		if (pText)
		{
			UInt index = m_iStartIndex + i;
			CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(index));
			String strTmp;
			if(pRank)
			{
				strTmp.Format(_T("%d"),pRank->GetWantedReward()+pRank->GetPK_Value()*500);
			}

			if (!strTmp.IsEmpty())
			{			
				pText->SetText(strTmp.c_str());
				pText->Redraw();
				pText->SetEnable(true);
			}
			else
			{
				pText->SetEnable(false);
			}
		}
	}
}

void CWantedProcess::SelectWanted(int index)
{
	StringA iconName;
	UInt wantedIndex = m_iStartIndex + index;
	String name;

	NetGroup* pWantedList = GetWantedList();
	if(pWantedList == NULL)
		return;

	CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(wantedIndex));

	if (index >= 0 && index < MAX_WANTED_PER_PAGE && pRank)
	{
		iconName.Format("WantedSelect%02d", index);
		UIImage* pImage = gGlobal.pUIManager->FindUIImage(iconName.c_str());
		UIImage* pHighLight = gGlobal.pUIManager->FindUIImage("WantedSelected");

		if (pImage && pHighLight)
		{
			pHighLight->SetOffsetXY(pHighLight->GetOffsetX(), pImage->GetOffsetY());
			pHighLight->SetEnable(true);
			m_iSelectedIndex = wantedIndex;
		}
	}
}

void CWantedProcess::WantedScroll(int scroll)
{
	UIScrollBarV* scrollBar = (UIScrollBarV*)gGlobal.pUIManager->GetUIItem("WantedScroll");
	if (scrollBar)
	{
		switch (scroll)
		{
		case 0:
			{
				if (m_iStartIndex > 0)
				{
					--m_iStartIndex;
					RefreshWantedList();
				}
			}
			break;
		case 1:
			{
				if (scrollBar->GetNumCount() > m_iStartIndex)
				{
					++m_iStartIndex;
					RefreshWantedList();
				}
			}
			break;
		case 2:
			{
				if (scrollBar->GetNumCount() > 0)
				{
					m_iStartIndex = scrollBar->GetSelNum();
					RefreshWantedList();
				}
			}
			break;
		}
	}
}

void CWantedProcess::RequestWantedReward()
{
	NetGroup* pWantedList = GetWantedList();
	if(pWantedList == NULL)
		return;
	if(m_iSelectedIndex == -1)
	{
		return;
	}
	CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(m_iSelectedIndex));
	if(pRank == NULL)
		return;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenWantedRewardPanel", String());
}
void CWantedProcess::WantedReward(UInt32 money)
{
	NetGroup* pWantedList = GetWantedList();
	if(pWantedList == NULL)
		return;
	if(m_iSelectedIndex == -1)
	{
		return;
	}
	CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(m_iSelectedIndex));
	if(pRank == NULL)
		return;
	gGlobal.g_pCharacter->GetCharNetObject()->UpdateWantedReward(gGlobal.pPeerProxy,gGlobal.g_context,pRank->GetChar_id(),pRank->GetNickName(),money);
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseWantedPanel", String());
}
void CWantedProcess::WantedDetail()
{
	NetGroup* pWantedList = GetWantedList();
	if(pWantedList == NULL)
		return;
	if(m_iSelectedIndex == -1)
	{
		return;
	}
	CRankCharacter*pRank = CastNetObject<CRankCharacter>(pWantedList->GetChild(m_iSelectedIndex));
	if(pRank == NULL)
		return;
	//gGlobal.g_pCharacter->GetCharNetObject()->WantedDetail(gGlobal.pPeerProxy,gGlobal.g_context,pRank->GetChar_id());
	//MessageService_Proxy::RequestInfo(gGlobal.pPeerProxy, gGlobal.g_context, pRank->GetChar_id(), String(), false, true);
	WorldService_Proxy::WantedDetail(gGlobal.pPeerProxy, RPCContext(), pRank->GetChar_id());
}

void CWantedProcess::WantedJailDelivery(UInt32 char_id)
{
	gGlobal.g_pCharacter->GetCharNetObject()->JailDelivery(gGlobal.pPeerProxy,gGlobal.g_context,char_id);
}
void CWantedProcess::WantedBail(UInt32 char_id)
{
	gGlobal.g_pCharacter->GetCharNetObject()->WantedBail(gGlobal.pPeerProxy,gGlobal.g_context,char_id);
}
void CWantedProcess::WantedTrace(UInt32 char_id)
{
	gGlobal.g_pCharacter->GetCharNetObject()->WantedTrace(gGlobal.pPeerProxy,gGlobal.g_context,char_id);
}