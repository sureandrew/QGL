#include "Common.h"
#include "QGL.h"
#include "UI.h"
#include "Global.h"
#include "CharacterControl.h"
#include "Proxy/Logic/Character.h"
#include "Resource/ResPointsShop.h"
#include "PointsShopProcess.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResItem.h"
#include "QGLApplication.h"
#include "MenuItemProcess.h"

CPointsShopProcess::CPointsShopProcess()
	:m_ui32StarIndex(0)
	,m_uiCurPage(0)
	,m_uiSelIndex(0)
	,m_uiTotalRecord(0)
	,m_uiTotalPoints(0)
	,m_uiPointsShopID(-1)
	,m_uiPointsShopTypeID(-1)
{

}
CPointsShopProcess::~CPointsShopProcess()
{
}

void CPointsShopProcess::ShowPointsShop()
{	
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PointsShopPanel");
	if ( pPanel == NULL )
	XML_UI::GetInstance().AddFormFile("Data\\PointsShopUi.xml", gGlobal.pUIManager);
	pPanel = gGlobal.pUIManager->FindUIPanel("PointsShopPanel");
	if (NULL == pPanel)
		return;

	UInt32 uiItemSize = (UInt32)m_itemLst.size()/*gGlobal.m_resource->GetPointsShopItemDataSize()*/;
	UInt32 uiRestPage = uiItemSize % POINTSSHOPMAXITEMPERPAGE;
	UInt32 uiMaxPage = uiItemSize/POINTSSHOPMAXITEMPERPAGE;
	if (uiRestPage > 0)
		uiMaxPage++;
	if ((m_uiCurPage+1) == uiMaxPage && uiMaxPage > 0)
		m_uiTotalRecord = uiRestPage;
	else
		m_uiTotalRecord = POINTSSHOPMAXITEMPERPAGE;

	StringA strIconName;
	strIconName.Format("PointsShopPageDesc");
	UIStaticText* pAccText = gGlobal.pUIManager->FindUIStaticText(strIconName.c_str());
	if (NULL != pAccText) {
		String strPageDesc;
		strPageDesc.Format(_T("%d/%d"),m_uiCurPage+1,uiMaxPage);
		pAccText->SetText(strPageDesc);
		pAccText->Redraw();
	}

	strIconName.Format("PointsShopCurAcc");
	pAccText = gGlobal.pUIManager->FindUIStaticText(strIconName.c_str());
	if (NULL != pAccText) {
				String strPageDesc;
				strPageDesc.Format(_T("%d"), m_uiTotalPoints);
				pAccText->SetText(strPageDesc);
				pAccText->Redraw();
	}

	UIButton* pBtn = gGlobal.pUIManager->FindUIButton("PointsShopPrevPageBtn");
	if (NULL != pBtn)
	{
		if (0 == m_uiCurPage) 
			pBtn->SetEnable(false);
		else
			pBtn->SetEnable(true);
	}

	pBtn = gGlobal.pUIManager->FindUIButton("PointsShopNextPageBtn");
	if (NULL != pBtn)
	{
		if (uiMaxPage == (m_uiCurPage+1)) 
			pBtn->SetEnable(false);
		else
			pBtn->SetEnable(true);
	}

	StringA str;
	UInt32 nMax = MIN((Reuben::System::UInt32)m_itemLst.size(), POINTSSHOPMAXITEMPERPAGE);
	for (UInt32 i=0;i < nMax; i++) {
		if (i < m_uiTotalRecord) {
			const POINTSSHOPITEM* pPointsItem = gGlobal.m_resource->GetPointsShopData(m_itemLst[m_ui32StarIndex+i]);
			if (NULL != pPointsItem) {
				const ItemData* pItem = gGlobal.m_resource->GetItemData(pPointsItem->uiItemId);
				if (NULL != pItem) {
					StringA strPointsItemName;
					strPointsItemName.Format("PointsShopItem%02d",i);
					UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(strPointsItemName.c_str());
					if (pAniIcon)
					{
						String temp(pItem->icon2);
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(temp.c_str());
						pAniIcon->SetEnable(true);
					}

					if (i == m_uiSelIndex) {
						UIImage* pSelImage = gGlobal.pUIManager->FindUIImage("PointsShopSelectedRect");
						if (NULL != pSelImage) {
							pSelImage->SetOffsetXY(pAniIcon->GetOffsetX()-3,pAniIcon->GetOffsetY()-3);
						}
					}
					strPointsItemName.Format("PointsShopItemName%02d",i);
					UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(strPointsItemName.c_str());
					if (NULL != pText) {
						pText->SetText(pItem->name);
						pText->Redraw();
					}
					strPointsItemName.Format("PointsShopItemCost%02d",i);
					pText = gGlobal.pUIManager->FindUIStaticText(strPointsItemName.c_str());
					if (NULL != pText) {
						String strPrice;
						strPrice.Format(gGlobal.GetStringTable()->Get(_T("MSG_POINTS_SHOP_PRICE")),pPointsItem->uiPointsCost);
						pText->SetText(strPrice);
						pText->Redraw();
					}
				}
			}
		}
		else{
			StringA strPointsItemName;
			strPointsItemName.Format("PointsShopItem%02d",i);
			UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(strPointsItemName.c_str());
			if (pAniIcon)
			{
				pAniIcon->ClearAniData();
				pAniIcon->SetEnable(false);
			}
			strPointsItemName.Format("PointsShopItemName%02d",i);
			UIStaticText* pText = gGlobal.pUIManager->FindUIStaticText(strPointsItemName.c_str());
			if (NULL != pText) {
				pText->SetText(_T(""));
				pText->Redraw();
			}
			strPointsItemName.Format("PointsShopItemCost%02d",i);
			pText = gGlobal.pUIManager->FindUIStaticText(strPointsItemName.c_str());
			if (NULL != pText) {
				pText->SetText(_T(""));
				pText->Redraw();
			}
		}
	}

	gGlobal.pUIManager->AddDirtyUIItem(pPanel);
}
void CPointsShopProcess::SelectPointsShopItem(int index)
{
	if (index > -1 && index < (int)m_uiTotalRecord) {
		m_uiSelIndex = (UInt32)index;
		ShowPointsShop();
	}
}
void CPointsShopProcess::PointsShopPrevPage()
{
	if (m_uiCurPage > 0) {
		m_uiCurPage--;
		m_ui32StarIndex = m_uiCurPage*POINTSSHOPMAXITEMPERPAGE;
		ShowPointsShop();
	}
}
void CPointsShopProcess::PointsShopNextPage()
{
	UInt32 uiItemSize = (UInt32)m_itemLst.size()/*gGlobal.m_resource->GetPointsShopItemDataSize()*/;
	UInt32 uiRestPage = uiItemSize % POINTSSHOPMAXITEMPERPAGE;
	UInt32 uiMaxPage = uiItemSize/POINTSSHOPMAXITEMPERPAGE;
	if (uiRestPage > 0)
		uiMaxPage++;
	uiMaxPage-=1;
	if (m_uiCurPage < uiMaxPage) {
		m_uiCurPage++;
		m_ui32StarIndex = m_uiCurPage*POINTSSHOPMAXITEMPERPAGE;
		ShowPointsShop();
	}
}
void CPointsShopProcess::PointsShopBuyItem()
{
	if (NULL != gGlobal.g_pCharacter) {
		CCharacter* pChar= gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) {
			if ((m_ui32StarIndex + m_uiSelIndex) < m_itemLst.size())
			{
				pChar->BuyPointsShopItem(gGlobal.pPeerProxy,gGlobal.g_context, m_itemLst[m_ui32StarIndex+m_uiSelIndex], m_uiPointsShopID, m_uiPointsShopTypeID);
			}
		}
	}
}
void CPointsShopProcess::OnMouseMoveOver(UInt32 uiIndex)
{
	if (uiIndex < POINTSSHOPMAXITEMPERPAGE && (m_ui32StarIndex + uiIndex) < m_itemLst.size()) {
		const POINTSSHOPITEM* pPointsItem = gGlobal.m_resource->GetPointsShopData(m_itemLst[m_ui32StarIndex+uiIndex]);
		if (NULL != pPointsItem) {
			UIPanelEx* pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
			if (pPanelEx == NULL)
			{
				XML_UI::GetInstance().Init();
				XML_UI::GetInstance().ReadFormFile("Data\\ItemDetail.xml", gGlobal.pUIManager);

				pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
				if (pPanelEx == NULL)
					return;
				gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);
				gGlobal.g_pItemProcess->ItemDetailPopUp(pPointsItem->uiItemId, true, NULL, false);

				Int32 posX = 0, posY = 0;

				UInt32 uiOffsetX = 20;
				UInt32 uiOffsetY = 20;

				if ( ( gGlobal.pUIManager->GetCursorX() + pPanelEx->GetWidth() ) > 800 )
					posX = 800 - pPanelEx->GetWidth() - uiOffsetX;
				else
					posX = gGlobal.pUIManager->GetCursorX() + uiOffsetX;

				if ( ( gGlobal.pUIManager->GetCursorY() + pPanelEx->GetHeight() ) > 600 )
					posY = 600 - pPanelEx->GetHeight() - uiOffsetY;
				else
					posY = gGlobal.pUIManager->GetCursorY() + uiOffsetY;

				pPanelEx->SetPos(posX, posY);
				pPanelEx->SetEnable(true);
			}
		}
	}
}

void CPointsShopProcess::SetTotalPoints( UInt32 uiPoints )
{
	m_uiTotalPoints = uiPoints;

	StringA strIconName;
	strIconName.Format("PointsShopCurAcc");
	UIStaticText* pAccText  = gGlobal.pUIManager->FindUIStaticText(strIconName.c_str());
	if (NULL != pAccText) {
				String strPageDesc;
				strPageDesc.Format(_T("%d"), m_uiTotalPoints);
				pAccText->SetText(strPageDesc);
				pAccText->Redraw();
		}
}

void CPointsShopProcess::SetData( UInt32 uiPointsShopID,UInt32 uiPointsShopTypeID, UInt32Vector& Lst )
{
	m_uiPointsShopID = uiPointsShopID;
	m_uiPointsShopTypeID = uiPointsShopTypeID;
	m_itemLst = Lst;
}