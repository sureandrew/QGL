#include "Common.h"
#include "QGL.h"
#include "UI.h"
#include "Global.h"
#include "CharacterControl.h"
#include "Proxy/Logic/Character.h"
#include "Resource/ResAccmulationShop.h"
#include "AccShopProcess.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResItem.h"
#include "QGLApplication.h"
#include "MenuItemProcess.h"

CAccShopProcess::CAccShopProcess()
	:m_ui32StarIndex(0)
	,m_uiCurPage(0)
	,m_uiSelIndex(0)
	,m_uiTotalRecord(0)
{

}
CAccShopProcess::~CAccShopProcess()
{
}
void CAccShopProcess::ShowAccShop()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("AccShopPanel");
	if (NULL == pPanel)
		return;
	UInt32 uiItemSize = (UInt32)gGlobal.m_resource->GetAccShopItemDataSize();
	UInt32 uiRestPage = uiItemSize % ACCSHOPMAXITEMPERPAGE;
	UInt32 uiMaxPage = uiItemSize/ACCSHOPMAXITEMPERPAGE;
	if (uiRestPage > 0)
		uiMaxPage++;
	if ((m_uiCurPage+1) == uiMaxPage && uiMaxPage > 0)
		m_uiTotalRecord = uiRestPage;
	else
		m_uiTotalRecord = ACCSHOPMAXITEMPERPAGE;

	StringA strIconName;
	strIconName.Format("AccShopPageDesc");
	UIStaticText* pAccText = gGlobal.pUIManager->FindUIStaticText(strIconName.c_str());
	if (NULL != pAccText) {
		String strPageDesc;
		strPageDesc.Format(_T("%d/%d"),m_uiCurPage+1,uiMaxPage);
		pAccText->SetText(strPageDesc);
		pAccText->Redraw();
	}

	strIconName.Format("AccShopCurAcc");
	pAccText = gGlobal.pUIManager->FindUIStaticText(strIconName.c_str());
	if (NULL != pAccText) {
		if (NULL != gGlobal.g_pCharacter) {
			CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
			if (NULL != pChar) {
				String strPageDesc;
				strPageDesc.Format(_T("%d"),pChar->GetCurAcc());
				pAccText->SetText(strPageDesc);
				pAccText->Redraw();
			}
		}
	}

	UIButton* pBtn = gGlobal.pUIManager->FindUIButton("AccShopPrevPageBtn");
	if (NULL != pBtn)
	{
		if (0 == m_uiCurPage) 
			pBtn->SetEnable(false);
		else
			pBtn->SetEnable(true);
	}

	pBtn = gGlobal.pUIManager->FindUIButton("AccShopNextPageBtn");
	if (NULL != pBtn)
	{
		if (uiMaxPage == (m_uiCurPage+1)) 
			pBtn->SetEnable(false);
		else
			pBtn->SetEnable(true);
	}

	for (UInt32 i=0;i < ACCSHOPMAXITEMPERPAGE;i++) {
		if (i < m_uiTotalRecord) {
			const ACCSHOPITEM* pAccItem = gGlobal.m_resource->GetAccShopData(m_ui32StarIndex+i);
			if (NULL != pAccItem) {
				const ItemData* pItem = gGlobal.m_resource->GetItemData(pAccItem->uiItemId);
				if (NULL != pItem) {
					StringA strAccItemName;
					strAccItemName.Format("AccShopItem%02d",i);
					UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(strAccItemName.c_str());
					if (pAniIcon)
					{
						String temp(pItem->icon2);
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(temp.c_str());
						pAniIcon->SetEnable(true);
					}

					if (i == m_uiSelIndex) {
						UIImage* pSelImage = gGlobal.pUIManager->FindUIImage("AccShopSelectedRect");
						if (NULL != pSelImage) {
							pSelImage->SetOffsetXY(pAniIcon->GetOffsetX()-3,pAniIcon->GetOffsetY()-3);
						}
					}
					strAccItemName.Format("AccShopItemName%02d",i);
					UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(strAccItemName.c_str());
					if (NULL != pText) {
						pText->SetText(pItem->name);
						pText->Redraw();
					}
					strAccItemName.Format("AccShopItemCost%02d",i);
					pText = gGlobal.pUIManager->FindUIStaticText(strAccItemName.c_str());
					if (NULL != pText) {
						String strPrice;
						strPrice.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_PRICE")),pAccItem->uiAccmulationCost);
						pText->SetText(strPrice);
						pText->Redraw();
					}
				}
			}
		}
		else{
			StringA strAccItemName;
			strAccItemName.Format("AccShopItem%02d",i);
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(strAccItemName.c_str());
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetEnable(false);
			}
			strAccItemName.Format("AccShopItemName%02d",i);
			UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(strAccItemName.c_str());
			if (NULL != pText) {
				pText->SetText(_T(""));
				pText->Redraw();
			}
			strAccItemName.Format("AccShopItemCost%02d",i);
			pText = gGlobal.pUIManager->FindUIStaticText(strAccItemName.c_str());
			if (NULL != pText) {
				pText->SetText(_T(""));
				pText->Redraw();
			}
		}
	}

	gGlobal.pUIManager->AddDirtyUIItem(pPanel);
}
void CAccShopProcess::SelectAccShopItem(int index)
{
	if (index > -1 && index < (int)m_uiTotalRecord) {
		m_uiSelIndex = (UInt32)index;
		ShowAccShop();
	}
}
void CAccShopProcess::AccShopPrevPage()
{
	if (m_uiCurPage > 0) {
		m_uiCurPage--;
		m_ui32StarIndex = m_uiCurPage*ACCSHOPMAXITEMPERPAGE;
		ShowAccShop();
	}
}
void CAccShopProcess::AccShopNextPage()
{
	UInt32 uiItemSize = (UInt32)gGlobal.m_resource->GetAccShopItemDataSize();
	UInt32 uiRestPage = uiItemSize % ACCSHOPMAXITEMPERPAGE;
	UInt32 uiMaxPage = uiItemSize/ACCSHOPMAXITEMPERPAGE;
	if (uiRestPage > 0)
		uiMaxPage++;
	uiMaxPage-=1;
	if (m_uiCurPage < uiMaxPage) {
		m_uiCurPage++;
		m_ui32StarIndex = m_uiCurPage*ACCSHOPMAXITEMPERPAGE;
		ShowAccShop();
	}
}
void CAccShopProcess::AccShopBuyItem()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) {
			pChar->BuyAccShopItem(gGlobal.pPeerProxy,gGlobal.g_context,m_ui32StarIndex+m_uiSelIndex);
		}
	}
}
void CAccShopProcess::OnMouseMoveOver(UInt32 uiIndex)
{
	if (uiIndex < ACCSHOPMAXITEMPERPAGE) {
		const ACCSHOPITEM* pAccItem = gGlobal.m_resource->GetAccShopData(m_ui32StarIndex+uiIndex);
		if (NULL != pAccItem) {
			UIPanelEx* pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
			if (pPanelEx == NULL)
			{
				XML_UI::GetInstance().Init();
				XML_UI::GetInstance().ReadFormFile("Data\\ItemDetail.xml", gGlobal.pUIManager);

				pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
				if (pPanelEx == NULL)
					return;
				gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);
				gGlobal.g_pItemProcess->ItemDetailPopUp(pAccItem->uiItemId, true, NULL, false);

				Int32 posX = 0, posY = 0;

				if ( ( gGlobal.pUIManager->GetCursorX() + pPanelEx->GetWidth() ) > 800 )
					posX = 800 - pPanelEx->GetWidth();
				else
					posX = gGlobal.pUIManager->GetCursorX();

				if ( ( gGlobal.pUIManager->GetCursorY() + pPanelEx->GetHeight() ) > 600 )
					posY = 600 - pPanelEx->GetHeight();
				else
					posY = gGlobal.pUIManager->GetCursorY();

				pPanelEx->SetPos(posX, posY);
				pPanelEx->SetEnable(true);
			}
		}
	}
}