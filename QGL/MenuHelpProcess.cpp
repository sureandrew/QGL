//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuHelpProcess.h"
//-- Library
#include "Global.h"
#include "QGLApplication.h"
#include "ScriptControl.h"
#include "CharacterControl.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResNewbie.h"
#include "Resource/ResNewbieTips.h"
#include "Resource/ResNewGuide.h"

#define levelXDiff 20
#define MAX_ITEM	21
#define LINEHEIGHT	18
#define HELPLIGHTY	34

enum FLASH_TYPE{
	FLASH_TYPE_NIL = 0,
	FLASH_TYPE_ATTACK_B,
	FLASH_TYPE_TRADE_B,
	FLASH_TYPE_GIVE_B,
	FLASH_TYPE_TEAM_B,
	FLASH_TYPE_SKILL_B,
	FLASH_TYPE_ITEM_B,
	FLASH_TYPE_PET_B,
	FLASH_TYPE_MISSION_B,
	FLASH_TYPE_GUILD_B,
	FLASH_TYPE_FRIEND_B,
	FLASH_TYPE_ONLINESHOP_B,
	FLASH_TYPE_SYSTEM_B,
	FLASH_TYPE_CHAR_B,
	FLASH_TYPE_PARTNER_B,
	FLASH_TYPE_MISSION_EVERYDAY_B,
	MAX_FLASH_TYPE,

};
CMenuHelpProcess::CMenuHelpProcess()
{
	m_headHelpItem = NULL;
	m_CurrentHelpItem = NULL;
	m_CurrentHelpLastItem = NULL;

	m_CurrContentID = 0;
	m_CurrContentPos = 0;
	
	m_SearchStartPos = 0;
	m_SearchStartID = 0;
	m_CurrIndex = 0;
	m_CurrContentLine = 0;
}

CMenuHelpProcess::~CMenuHelpProcess()
{
	Release();
}

void CMenuHelpProcess::SearchContent(PCSTRINGW text)
{
	BOOL FirstTime = FALSE;
	if (m_SearchStartText != text)
	{
		ResetSearch();
		m_SearchStartText = text;
		//FirstTime= TRUE;
	}

	for (UInt i = 0; i < m_MaxContentID; i ++)
	{
		UInt tempID = (m_CurrContentID + i) % (m_MaxContentID + 1);

		if (tempID == 0)
			tempID = 1;

		if (!FirstTime)
			tempID ++;

		const NewbieData * pNewbieData = gGlobal.m_resource->GetNewBieData(tempID);
		if (pNewbieData)
		{
			String temp = pNewbieData->description;
			size_t pos = temp.find(text, m_CurrContentPos + 1);
			if (pos != std::string::npos)
			{
				m_CurrContentID = tempID;
				m_CurrContentPos = pos;
				if (m_CurrContentPos == m_SearchStartPos &&
					m_CurrContentID == m_SearchStartID)
				{
					return gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HELP_SEARCH_ERROR_BACK_TO_START")));
				}
				else
				{
					if (m_SearchStartPos == 0)
						m_SearchStartPos = m_CurrContentPos;
					if (m_SearchStartID == 0)
						m_SearchStartID = m_CurrContentID;
					ShowContent(m_CurrContentID, m_CurrContentPos, (UInt)m_SearchStartText.size());
					return;
				}

			}
		}
	}
	
	return gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HELP_SEARCH_ERROR_NOT_FOUND")));
}

HelpItem * CMenuHelpProcess::MoveNextHelpItem(HelpItem * pItem)
{
	HelpItem * pTempItem = pItem;
	if (pTempItem->isOpen &&
		pTempItem->Leftchild)
		pTempItem = pTempItem->Leftchild;
	else if (pTempItem->right)
		pTempItem = pTempItem->right;
	else// if (pTempItem->parent)
	{
		while(pTempItem)
		{
			if (pTempItem->parent && pTempItem->parent->right)
			{
				pTempItem = pTempItem->parent->right;
				break;
			}
			pTempItem = pTempItem->parent;
		}
		//pTempItem = pTempItem->parent->right;
	}

	return pTempItem;
}

void CMenuHelpProcess::SetIndex(Int index)
{
	if (m_CurrIndex < (UInt32)index)
	{
		Int diff = index - m_CurrIndex;
		for (Int i = 0; i < diff; i++)
		{
			if (m_CurrentHelpItem)
			{
				HelpItem * pTempItem = MoveNextHelpItem(m_CurrentHelpLastItem);
				if (pTempItem)
				{
					m_CurrentHelpLastItem = pTempItem;
					m_CurrentHelpItem = MoveNextHelpItem(m_CurrentHelpItem);
					m_CurrIndex ++;
				}
				else
					break;
			}
		}
	}
	else
	{
		Int diff = m_CurrIndex - index;
		for (Int i = 0; i < diff; i++)
		{
			if (m_CurrentHelpItem)
			{
				HelpItem * pTempItem = MovePreHelpItem(m_CurrentHelpItem);
				if (pTempItem)
				{
					m_CurrentHelpItem = pTempItem;
					m_CurrentHelpLastItem = MovePreHelpItem(m_CurrentHelpLastItem);
					m_CurrIndex --;
				}	
				else
					break;
			}
		}
	}
}

void CMenuHelpProcess::CalOpenCount()
{
	m_openCount = 0;

	HelpItem * pTempItem = m_headHelpItem;
	while (pTempItem)
	{
		m_openCount ++;
		pTempItem = MoveNextHelpItem(pTempItem);
	}

	UIScrollBarV * pScroll = gGlobal.pUIManager->FindUIScrollBarV("LeftHelperPanelScroll");
	if (pScroll)
		pScroll->SetNumCount(m_openCount);
}
void CMenuHelpProcess::ShowContent(UInt16 contentID, size_t contentLinePos, UInt len)
{
	HelpItem * pTempItem = m_headHelpItem;
	m_CurrentHelpItem = m_headHelpItem;

	UInt index = 1;
	while (true)
	{
		if (pTempItem->Leftchild == NULL
			&& pTempItem->contentID == contentID)
		{
			RefreshMenu();
			ShowTextContent(index, contentLinePos, len);
			return;
		}
		else if(pTempItem->contentID >= contentID)
		{
			pTempItem->isOpen = true;
		}
		
		pTempItem = MoveNextHelpItem(pTempItem);

		if (index >= MAX_ITEM)
		{
			m_CurrentHelpItem = MoveNextHelpItem(m_CurrentHelpItem);
			m_CurrIndex ++;
		}
		else
			index ++;
	}
}

void CMenuHelpProcess::ConstructData()
{
	for (Int i = 1; ; i++)
	//while(pNewbieData != NILL)
	{
		
		const NewbieData * pNewbieData = gGlobal.m_resource->GetNewBieData(i);
		if (pNewbieData == NULL)
		{
			m_MaxContentID = i - 1;
			break;
		}

		HelpItem * pTemp = NULL;
		HelpItem * pLastItem = NULL;
		HelpItem * pHelpItemClass1 = NULL; 
		HelpItem * pHelpItemClass2 = NULL;
		HelpItem * pHelpItemClass3 = NULL;

		//find class one
		if (m_headHelpItem == NULL)
		{
			pHelpItemClass1 = SafeCreateObject(HelpItem);
			pHelpItemClass1->contentID = pNewbieData->id;
			pHelpItemClass1->itemName = pNewbieData->class1;
			pHelpItemClass1->level = 0;
			m_headHelpItem = pHelpItemClass1;
		}
		else
		{
			pTemp = m_headHelpItem;
			pLastItem = pTemp;
			while (pTemp != NULL)
			{
				if (pTemp->itemName == pNewbieData->class1)
				{
					pHelpItemClass1 = pTemp;
					pHelpItemClass1->contentID = pNewbieData->id;
					break;
				}
				pLastItem = pTemp;
				pTemp = pTemp->right;
			}

			if (pTemp == NULL)
			{
				pHelpItemClass1 = SafeCreateObject(HelpItem);
				pHelpItemClass1->contentID = pNewbieData->id;
				pHelpItemClass1->itemName = pNewbieData->class1;
				pLastItem->right = pHelpItemClass1;
				pHelpItemClass1->left = pLastItem;
				pHelpItemClass1->level = 0;
			}
		}

		if (wcsncmp(pNewbieData->class2, _T("0"), 1) == 0)
			continue;
		//find class two
		pHelpItemClass2 = pHelpItemClass1->Leftchild;
		if (pHelpItemClass2 == NULL)
		{
			pHelpItemClass2 = SafeCreateObject(HelpItem);
			pHelpItemClass2->contentID = pNewbieData->id;
			pHelpItemClass2->itemName = pNewbieData->class2;
			pHelpItemClass2->parent = pHelpItemClass1;
			pHelpItemClass2->level = 1;
			pHelpItemClass1->Leftchild = pHelpItemClass2;
		}
		else
		{
			pTemp = pHelpItemClass2;
			pLastItem = pTemp;
			while (pTemp != NULL)
			{
				if (pTemp->itemName == pNewbieData->class2)
				{
					pHelpItemClass2 = pTemp;
					pHelpItemClass2->contentID = pNewbieData->id;
					break;
				}
				pLastItem = pTemp;
				pTemp = pTemp->right;
			}

			if (pTemp == NULL)
			{
				pHelpItemClass2 = SafeCreateObject(HelpItem);
				pHelpItemClass2->contentID = pNewbieData->id;
				pHelpItemClass2->itemName = pNewbieData->class2;
				pHelpItemClass2->parent = pHelpItemClass1;
				pHelpItemClass2->level = 1;
				pLastItem->right = pHelpItemClass2;
				pHelpItemClass2->left = pLastItem;
			}
		}

		if (wcsncmp(pNewbieData->class3, _T("0"), 1) == 0)
			continue;
		//find class three
		pHelpItemClass3 = pHelpItemClass2->Leftchild;
		if (pHelpItemClass3 == NULL)
		{
			pHelpItemClass3 = SafeCreateObject(HelpItem);
			pHelpItemClass3->contentID = pNewbieData->id;
			pHelpItemClass3->itemName = pNewbieData->class3;
			pHelpItemClass3->parent = pHelpItemClass2;
			pHelpItemClass3->level = 2;
			pHelpItemClass2->Leftchild = pHelpItemClass3;
		}
		else
		{
			pTemp = pHelpItemClass3;
			pLastItem = pTemp;
			while (pTemp != NULL)
			{
				if (pTemp->itemName == pNewbieData->class3)
				{
					pHelpItemClass3 = pTemp;
					pHelpItemClass3->contentID = pNewbieData->id;
					break;
				}
				pLastItem = pTemp;
				pTemp = pTemp->right;
			}

			if (pTemp == NULL)
			{
				pHelpItemClass3 = SafeCreateObject(HelpItem);
				pHelpItemClass3->contentID = pNewbieData->id;
				pHelpItemClass3->itemName = pNewbieData->class3;
				pHelpItemClass3->parent = pHelpItemClass2;
				pHelpItemClass3->level = 2;
				pLastItem->right = pHelpItemClass3;
				pHelpItemClass3->left = pLastItem;
			}
		}
	}

	m_CurrentHelpItem = m_headHelpItem;

}
void CMenuHelpProcess::Release()
{
	FreeAllHelpItem(m_headHelpItem);
	m_headHelpItem = NULL;
	m_CurrentHelpItem = NULL;
	m_SearchStartText.Empty();
	m_finishHelpTipsID.clear();
	m_finishGuideID.clear();
}

void CMenuHelpProcess::FreeAllHelpItem(HelpItem * pHelpItem)
{
	if (pHelpItem)
	{
		if (pHelpItem->Leftchild)
			FreeAllHelpItem(pHelpItem->Leftchild);
		if (pHelpItem->right)
			FreeAllHelpItem(pHelpItem->right);
		SafeDeleteObject(pHelpItem);
	}
}

void CMenuHelpProcess::ShowTextContent(int index, size_t contentLinePos, UInt len)
{
	HelpItem * pTempItem = GetHelpItemByIndex(index);
	if (pTempItem)
	{
		if (pTempItem->Leftchild == NULL)
		{
			UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("HelperPanelContext");
			if (pChat)
			{
				pChat->ClearAllRow();
				const NewbieData * pNewbieData = gGlobal.m_resource->GetNewBieData(pTempItem->contentID);
				if (pNewbieData)
				{
					String text = pNewbieData->description;
					String final;
					String word;
					final = text.substr(0, contentLinePos);
					final += _T("\\(");
					word = text.substr(contentLinePos,len);
					final += word;
					final += _T("\\)");
					final += text.substr(contentLinePos + len);

					pChat->AddStringW(final.c_str());
					pChat->HomeRow();
					UInt Line = pChat->FindHighLightWord(0);
					pChat->SetRow(Line - 1, pChat->RowListCount());
					UIScrollBarV * pScroll = gGlobal.pUIManager->FindUIScrollBarV("RightHelperPanelScroll");
					if (pScroll)
					{
						pScroll->SetNumCount(pChat->RowListCount());
						pScroll->SetSelNum(Line - 1);
					}
				}
			}
			m_CurrContentID = pTempItem->contentID;
			m_CurrContentPos = 0;
		}
		else
		{
			UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("HelperPanelContext");
			if (pChat)
			{
				pChat->ClearAllRow();
				pChat->HomeRow();
				UIScrollBarV * pScroll = gGlobal.pUIManager->FindUIScrollBarV("RightHelperPanelScroll");
				if (pScroll)
					pScroll->SetNumCount(0);

			}
		}

		UIImage * pIm = gGlobal.pUIManager->FindUIImage("LeftHightLight");
		if (pIm)
		{
			pIm->SetOffsetXY(pIm->GetOffsetX(), (index - 1) * LINEHEIGHT + HELPLIGHTY);
			pIm->SetEnable(true);
		}
	}
}
HelpItem * CMenuHelpProcess::GetHelpItemByIndex(int index)
{
	HelpItem * pTempItem = m_CurrentHelpItem;
	for (Int i = 1; i <= MAX_ITEM; i++)
	{	
		if (pTempItem == NULL)
		{
			return NULL;
		}
		else
		{
			if (i == index)
			{
				return pTempItem;
			}

			pTempItem = MoveNextHelpItem(pTempItem);
		}
	}

	return NULL;
}
void CMenuHelpProcess::IsOpenChange(int index)
{
	HelpItem * pTempItem = GetHelpItemByIndex(index);
	if (pTempItem)
	{
		UICheckBox *pBox = NULL;
		StringA name;
		name.Format("HelperItem%d", index);
		pBox = gGlobal.pUIManager->FindUICheckBox(name.c_str());
		if (pBox)
		{
			pTempItem->isOpen = pBox->GetState();
			RefreshMenu();
			ResetSearch();
		}

		UIChatPanel * pChat = gGlobal.pUIManager->FindUIChatPanel("HelperPanelContext");
		if (pChat)
		{
			pChat->ClearAllRow();
			pChat->EndRow();
			UIScrollBarV * pScroll = gGlobal.pUIManager->FindUIScrollBarV("RightHelperPanelScroll");
			if (pScroll)
				pScroll->SetNumCount(0);

		}

		UIImage * pIm = gGlobal.pUIManager->FindUIImage("LeftHightLight");
		if (pIm)
		{
			pIm->SetOffsetXY(pIm->GetOffsetX(), (index - 1) * LINEHEIGHT + HELPLIGHTY);
			pIm->SetEnable(true);
		}

	}

	CalOpenCount();
}
void CMenuHelpProcess::RefreshMenu()
{
	HelpItem * pTempItem = m_CurrentHelpItem;
	for (Int i = 1; i <= 21; i++)
	{
		UICheckBox *pBox = NULL;
		UIImage *pIm = NULL;
		UIStaticText *pText = NULL;
		StringA name;
		name.Format("HelperItem%d", i);
		pBox = gGlobal.pUIManager->FindUICheckBox(name.c_str());

		name.Format("HelperItemName%d", i);
		pText = gGlobal.pUIManager->FindUIStaticText(name.c_str());

		name.Format("HelperItemLast%d", i);
		pIm = gGlobal.pUIManager->FindUIImage(name.c_str());
		if (pBox && pText && pIm)
		{
			if (pTempItem == NULL)
			{
				pBox->SetEnable(false);
				pText->SetEnable(false);
				pIm->SetEnable(false);
			}
			else
			{
				pBox->SetEnable(true);
				pText->SetEnable(true);
				pIm->SetEnable(false);
				
				if (pTempItem->Leftchild == NULL)
				{
					pBox->SetEnable(false);
					pIm->SetEnable(true);
				}
				else if (pTempItem->isOpen)
				{
					pBox->SetState(true);
				}
				else
					pBox->SetState(false);
				pBox->SetOffsetXY(14 + pTempItem->level * levelXDiff, pBox->GetOffsetY());
				pIm->SetOffsetXY(14 + pTempItem->level * levelXDiff, pBox->GetOffsetY());

				pText->SetText(pTempItem->itemName);
				pText->Redraw();
				pText->SetOffsetXY(30 + pTempItem->level * levelXDiff, pText->GetOffsetY());

				m_CurrentHelpLastItem = pTempItem;
				pTempItem = MoveNextHelpItem(pTempItem);

			}
		}
	}
}

HelpItem * CMenuHelpProcess::MovePreHelpItem(HelpItem * pItem)
{
	HelpItem * pTempItem = pItem;
	if (pTempItem->left)
		pTempItem = pTempItem->left;
	else
		pTempItem = pTempItem->parent;
	return pTempItem;
}
void CMenuHelpProcess::PageUp()
{
	if (m_CurrentHelpItem)
	{
		HelpItem * pTempItem = MovePreHelpItem(m_CurrentHelpItem);
		if (pTempItem)
		{
			m_CurrentHelpItem = pTempItem;
			m_CurrentHelpLastItem = MovePreHelpItem(m_CurrentHelpLastItem);
			m_CurrIndex --;
		}	
	}

	ResetSearch();
}
void CMenuHelpProcess::PageDown()
{
	if (m_CurrentHelpItem)
	{
		HelpItem * pTempItem = MoveNextHelpItem(m_CurrentHelpLastItem);
		if (pTempItem)
		{
			m_CurrentHelpLastItem = pTempItem;
			m_CurrentHelpItem = MoveNextHelpItem(m_CurrentHelpItem);
			m_CurrIndex ++;
		}
	}

	ResetSearch();
}

bool CMenuHelpProcess::ReadGuideFile()
{
	String temp;

	TCHAR szBuffer[_MAX_PATH];
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\HelpTips"), szBuffer);

	FILE* pFile = NULL;

	String tempfileName;
	tempfileName.Format(_T("%s\\%s.newguides"), temp.c_str(), gGlobal.g_pCharacter->GetCharName().c_str());
	pFile = _tfopen(tempfileName.c_str(), _T("rb"));
	ULONGLONG size = 0;
	if (pFile)
	{
		UInt16 count = 0;
		fread((char *)&count, sizeof(UInt16), 1, pFile);
		for (UInt i = 0; i < count; ++i)
		{
			UInt16 helpID = 0;
			fread((char *)&helpID, sizeof(UInt16), 1, pFile);
			m_finishGuideID.push_back(helpID);
		}
		fclose(pFile);
	}
	return true;
}

void CMenuHelpProcess::ReadHelpTipsFile()
{
	String temp;
	
	TCHAR szBuffer[_MAX_PATH];
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\HelpTips"), szBuffer);

	FILE* pFile = NULL;
	
	String tempfileName;
	tempfileName.Format(_T("%s\\%s.newbietips"), temp.c_str(), gGlobal.g_pCharacter->GetCharName().c_str());
	pFile = _tfopen(tempfileName.c_str(), _T("rb"));
	ULONGLONG size = 0;
	if (pFile)
	{
		UInt32 count = 0;
		fread((char *)&count, sizeof(UInt32),1,  pFile);
		for (UInt i = 0; i < count; i ++)
		{
			UInt16 helpID = 0;
			fread((char *)&helpID, sizeof(UInt16),1,  pFile);
			m_finishHelpTipsID.push_back(helpID);
		}
		fclose(pFile);
	}
}

void CMenuHelpProcess::WriteGuideFile()
{
	String temp;

	TCHAR szBuffer[_MAX_PATH];
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\HelpTips"), szBuffer);

	if (Reuben::Platform::File::IsExists(temp.c_str()) ||
		::CreateDirectory(temp.c_str(), NULL))

	{
		FILE* pFile = NULL;

		String tempfileName;
		tempfileName.Format(_T("%s\\%s.newguides"), temp.c_str(), gGlobal.g_pCharacter->GetCharName().c_str());
		pFile = _tfopen(tempfileName.c_str(), _T("wb"));
		ULONGLONG size = 0;
		if (pFile)
		{
			UInt16 count = (UInt16)m_finishGuideID.size();
			fwrite((char *)&count, sizeof(UInt16), 1, pFile);
			for (UInt i = 0; i < count; ++i)
			{
				fwrite((char *)&m_finishGuideID[i], sizeof(UInt16), 1, pFile);
			}
			fclose(pFile);
		}
	}
}

void CMenuHelpProcess::WriteHelpTipsFile()
{
	String temp;
	
	TCHAR szBuffer[_MAX_PATH];
	//::GetCurrentDirectory(_MAX_PATH, szBuffer);
	::GetModuleFileName(::GetModuleHandle(NULL), szBuffer, _MAX_PATH);
	*(::_tcsrchr(szBuffer, _T('\\'))) =_T('\0');

	temp.Format(_T("%s\\HelpTips"), szBuffer);

	if (Reuben::Platform::File::IsExists(temp.c_str()) ||
		::CreateDirectory(temp.c_str(), NULL))

	{
		FILE* pFile = NULL;

		String tempfileName;
		tempfileName.Format(_T("%s\\%s.newbietips"), temp.c_str(), gGlobal.g_pCharacter->GetCharName().c_str());
		pFile = _tfopen(tempfileName.c_str(), _T("wb"));
		ULONGLONG size = 0;
		if (pFile)
		{
			UInt32 count = (UInt32)m_finishHelpTipsID.size();
			fwrite((char *)&count, sizeof(UInt32),1,  pFile);
			for (UInt i = 0; i < count; i ++)
			{
				fwrite((char *)&m_finishHelpTipsID[i], sizeof(UInt16),1,  pFile);
			}
			fclose(pFile);
		}
	}
}

void CMenuHelpProcess::HandleGuideEvent(UInt16 ID)
{
	if ( ID == 10000 )
	{
		gGlobal.g_pScriptControl->OnCloseGuide();
		return;
	}
	else if ( ID == 10001 )
	{
		gGlobal.g_pScriptControl->OnCloseArrow();
		return;
	}
	else if ( ID == 10002 )
	{
		gGlobal.g_pScriptControl->OnCloseGuide();
		gGlobal.g_pScriptControl->OnCloseArrow();
		return;
	}
	const NewGuideData * pData = NULL;
	for(StlMultiMap<UInt16, const NewGuideData * >::iterator Itr = m_unFinishGuideID.find(ID);
		Itr != m_unFinishGuideID.end(); ++Itr)
	{
		if (ID == Itr->second->guide_id)
		{
			pData = Itr->second;
			break;
		}
	}

	if (pData != NULL)
	{
		gGlobal.g_pScriptControl->OnShowGuiPic(pData->context, pData->img1_name, pData->img1_X, pData->img1_Y, pData->img2_name, pData->img2_X, pData->img2_Y, pData->img3_name
											  ,pData->img3_X, pData->img3_Y, pData->img4_name, pData->img4_X, pData->img4_Y, pData->img5_name, pData->img5_X, pData->img5_Y);
		gGlobal.g_pScriptControl->OnShowArr(1, pData->ArrLeft, pData->ArrLeft_X, pData->ArrLeft_Y);
		gGlobal.g_pScriptControl->OnShowArr(2, pData->ArrRight, pData->ArrRight_X, pData->ArrRight_Y);
		gGlobal.g_pScriptControl->OnShowArr(3, pData->ArrDown, pData->ArrDown_X, pData->ArrDown_Y);
		gGlobal.g_pScriptControl->OnShowArr(4, pData->ArrLeft02, pData->ArrLeft02_X, pData->ArrLeft02_Y);		

		m_finishGuideID.push_back(pData->guide_id);
		CheckGuideEncounter();
		WriteGuideFile();
	}
}

void CMenuHelpProcess::HandleEvent(UInt8 event)
{
	UInt count = (UInt)m_unFinishHelpTipsID.count(event);
	UInt index = 0;
	const NewbieTipsData * pData = NULL;
	for (StlMultiMap<UInt16, const NewbieTipsData * >::iterator Itr = m_unFinishHelpTipsID.find(event);
		index < count && Itr != m_unFinishHelpTipsID.end(); ++Itr , ++index)
	{
		if (event == HELP_TIPS_LEVEL_UP)
		{
			if (gGlobal.g_pCharacter->GetLevel() + 1 >= Itr->second->level_up)
			{
				pData = Itr->second;
				break;
			}
		}
		else if (event == HELP_TIPS_ENTER_MAP)
		{
			if (gGlobal.g_pCharacter->GetCharNetObject()->GetMap_id() == Itr->second->map_id)
			{
				pData = Itr->second;
				break;
			}
		}
		else
		{
			pData = Itr->second;
			break;
		}
	}

	if (pData != NULL)
	{
		if (gGlobal.g_pCharacter->GetCharNetObject()->GetSex() == SEX_MALE)
		{
			for (UInt i = 0; i < (UInt)pData->girlstring.size(); i ++)
			{
				if (!pData->girlstring[i].Equals(_T("0")))
					gGlobal.g_pScriptControl->OnSayHandle(0, pData->girlstring[i], 0, true, pData->girlimages[i]);
			}
		}
		else
		{
			for (UInt i = 0; i < (UInt)pData->boystring.size(); i ++)
			{
				if (!pData->boystring[i].Equals(_T("0")))
					gGlobal.g_pScriptControl->OnSayHandle(0, pData->boystring[i], 0, true, pData->boyimages[i]);
			}
		}
		if (event != HELP_TIPS_EVERY_LOGIN)
			m_finishHelpTipsID.push_back(pData->id);
		CheckHelpTipsEncounter();
		WriteHelpTipsFile();
		UIButton * pBu = NULL;
		switch (pData->flash)
		{
		case FLASH_TYPE_NIL:
			break;
		case FLASH_TYPE_ATTACK_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBAttack");
			break;
		case FLASH_TYPE_TRADE_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBTrade");
			break;
		case FLASH_TYPE_GIVE_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBGive");
			break;
		case FLASH_TYPE_TEAM_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBParty");
			break;
		case FLASH_TYPE_SKILL_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBSkill");
			break;
		case FLASH_TYPE_ITEM_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBItem");
			break;
		case FLASH_TYPE_PET_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBPet");
			break;
		case FLASH_TYPE_MISSION_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBMission");
			break;
		case FLASH_TYPE_GUILD_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBHousen");
			break;
		case FLASH_TYPE_FRIEND_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBFriendShip");
			break;
		case FLASH_TYPE_ONLINESHOP_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBChargeShop");
			break;
		case FLASH_TYPE_SYSTEM_B:
			pBu = gGlobal.pUIManager->FindUIButton("DownBSystem");
			break;
		case FLASH_TYPE_CHAR_B:
			//pBu = gGlobal.pUIManager->FindUIButton("DownBAttack");
			break;
		case FLASH_TYPE_PARTNER_B:
			//pBu = gGlobal.pUIManager->FindUIButton("DownBAttack");
			break;
		case FLASH_TYPE_MISSION_EVERYDAY_B:
			pBu = gGlobal.pUIManager->FindUIButton("Mail");
			break;
		}

		if (pBu)
			pBu->SetNoticeState();
		
	}
}

void CMenuHelpProcess::CheckGuideEncounter()
{
	m_unFinishGuideID.clear();

	for (UInt helpID = 0 ; ; ++helpID)
	{
		const NewGuideData * pData = gGlobal.m_resource->GetNewGuideData(helpID);
		if (pData == NULL)
		{
			if (helpID == 0)
				continue;
			break;
		}

		bool IDFound = false;
		for (UInt i = 0; i < m_finishGuideID.size(); ++i)
		{
			if (m_finishGuideID[i] == helpID)
			{
				IDFound = true;
				break;
			}
		}
		if (!IDFound)
		{
			m_unFinishGuideID.insert(std::make_pair(pData->guide_id, pData));
		}
	}
}

void CMenuHelpProcess::CheckHelpTipsEncounter()
{
	m_unFinishHelpTipsID.clear();

	
	for (UInt helpID = 0; ;helpID ++)
	{
		const NewbieTipsData * pData = gGlobal.m_resource->GetNewBieTipsData(helpID);
		if (pData == NULL)
		{
			if (helpID == 0)
				continue;
			break;
		}
		
		if (pData->event >= MAX_HELP_TIPS_TYPE)
			continue;

		bool IDFound = false;
		for (UInt i = 0; i < m_finishHelpTipsID.size(); i++)
		{
			if (m_finishHelpTipsID[i] == helpID)
			{
				IDFound = true;
				break;
			}
		}
		if (!IDFound)
		{
			m_unFinishHelpTipsID.insert(std::make_pair(pData->event, pData));
		}
	}
}

void CMenuHelpProcess::CloseNewGuide(UInt8 flag)
{
	if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.g_pCharacter->GetCharNetObject()->ShutNewGuide(gGlobal.pPeerProxy, gGlobal.g_context, flag);
}