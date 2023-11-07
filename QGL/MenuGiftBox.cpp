//-- Common
#include "Common.h"
#include "QGL.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "MenuGiftBox.h"

#include "Global.h"
#include "QGLApplication.h"
#include "Proxy/Logic/GiftBox.h"
#include "Proxy/Logic/Gift.h"
#include "LuaPush.h"
#include "Resource/ResPet.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "MenuItemProcess.h"
#include "MenuPartnerProcess.h"
#include "MenuPetProcess.h"
#include "Math.h"
#include "CharacterControl.h"


#define GIFT_PER_PAGE 48

CMenuGiftBox::CMenuGiftBox()
{
	m_iMenuIndex = GIFT_ITEM;
	m_iStartGiftBoxX = 0;
	m_iStartGiftBoxY = 0;
	m_iUpdateTime = 0;
}

CMenuGiftBox::~CMenuGiftBox()
{

}

void CMenuGiftBox::OpenGiftBoxInterface()
{
	m_iStartGiftBoxX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
	m_iStartGiftBoxY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenGiftBox", String());
}

void CMenuGiftBox::ShowGiftBoxItem()
{
	if (gGlobal.g_pGiftBox)
	{
		m_iMenuIndex = GIFT_ITEM;
		StringA iconName;
		UIAniIcon* pAniIcon = NULL;
		UIImage* pIconBg = NULL;
		UIStaticText* pText = NULL;
		for (Index i = C_INDEX(0); i < GIFT_PER_PAGE; ++i)
		{
			CGift* pGift = gGlobal.g_pGiftBox->GetGift(m_iMenuIndex, i);
			iconName.Format("GiftBox%03dBg", i);
			pIconBg = gGlobal.pUIManager->FindUIImage(iconName.c_str());
			iconName.Format("GiftBox%03dIcon", i);
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			iconName.Format("GiftBox%03dStack", i);
			pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());

			if (pAniIcon && pIconBg && pText)
			{
				pIconBg->SetEnable(false);
				pText->SetEnable(false);
				if (pGift )
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(pGift->GetItemID());
					if (pItemData)
					{
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
						pAniIcon->SetOffsetXY(pIconBg->GetOffsetX(), pIconBg->GetOffsetY());
						pAniIcon->SetEnable(true);

						if (pItemData->overlap > 1 && pGift->GetItemQuantity() > 1)
						{
							String stack;
							if (pItemData->overlap >= pGift->GetItemQuantity())
								stack.Format(_T("%d"), pGift->GetItemQuantity());
							else
								stack.Format(_T("%d"), pItemData->overlap);
							
							pText->SetText(stack.c_str());
							pText->Redraw();
							pText->SetEnable(true);
						}
					}
					else
						pAniIcon->SetEnable(false);
				}
				else
					pAniIcon->SetEnable(false);
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void CMenuGiftBox::ShowGiftBoxPartner()
{
	if (gGlobal.g_pGiftBox)
	{
		m_iMenuIndex = GIFT_PARTNER;
		StringA iconName;
		UIAniIcon* pAniIcon = NULL;
		UIImage* pIconBg = NULL;
		UIStaticText* pText = NULL;
		for (Index i = C_INDEX(0); i < GIFT_PER_PAGE; ++i)
		{
			CGift* pGift = gGlobal.g_pGiftBox->GetGift(m_iMenuIndex, i);
			iconName.Format("GiftBox%03dBg", i);
			pIconBg = gGlobal.pUIManager->FindUIImage(iconName.c_str());
			iconName.Format("GiftBox%03dIcon", i);
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			iconName.Format("GiftBox%03dStack", i);
			pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());

			if (pAniIcon && pIconBg && pText)
			{
				pIconBg->SetEnable(false);
				pText->SetEnable(false);
				if (pGift )
				{
					MobDB_Client * pMobData = gGlobal.LoadMobTemplate(pGift->GetItemID());
					if (pMobData)
					{
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(String(pMobData->m_basicinfo->mob_faceS).c_str());
						pAniIcon->SetOffsetXY(pIconBg->GetOffsetX() + 2, pIconBg->GetOffsetY() + 2);
						pAniIcon->SetEnable(true);
						pIconBg->SetEnable(true);
					}
					else
						pAniIcon->SetEnable(false);
				}
				else
					pAniIcon->SetEnable(false);
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void CMenuGiftBox::ShowGiftBoxPet()
{
	if (gGlobal.g_pGiftBox)
	{
		m_iMenuIndex = GIFT_PET;
		StringA iconName;
		UIAniIcon* pAniIcon = NULL;
		UIImage* pIconBg = NULL;
		UIStaticText* pText = NULL;
		for (Index i = C_INDEX(0); i < GIFT_PER_PAGE; ++i)
		{
			CGift* pGift = gGlobal.g_pGiftBox->GetGift(m_iMenuIndex, i);
			iconName.Format("GiftBox%03dBg", i);
			pIconBg = gGlobal.pUIManager->FindUIImage(iconName.c_str());
			iconName.Format("GiftBox%03dIcon", i);
			pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
			iconName.Format("GiftBox%03dStack", i);
			pText = gGlobal.pUIManager->FindUIStaticText(iconName.c_str());

			if (pAniIcon && pIconBg && pText)
			{
				pIconBg->SetEnable(false);
				pText->SetEnable(false);
				if (pGift )
				{
					const PetData * pPetData = gGlobal.m_resource->GetPetData(pGift->GetItemID());
					if (pPetData)
					{
						pAniIcon->ClearAniData();
						pAniIcon->SetAnimationData(String(pPetData->pet_faceS).c_str());
						pAniIcon->SetOffsetXY(pIconBg->GetOffsetX() + 2, pIconBg->GetOffsetY() + 2);
						pAniIcon->SetEnable(true);
						pIconBg->SetEnable(true);
					}
					else
						pAniIcon->SetEnable(false);
				}
				else
					pAniIcon->SetEnable(false);
				gGlobal.pUIManager->AddDirtyUIItem(pAniIcon);
			}
		}
	}
}

void CMenuGiftBox::ShowConfirmGetGiftName(int slot)
{
	if (slot >= 0 && gGlobal.g_pGiftBox)
	{
		String msg;
		CGift* pGift = gGlobal.g_pGiftBox->GetGift(m_iMenuIndex, slot);
		if (pGift)
		{
			switch(m_iMenuIndex)
			{
			case GIFT_ITEM:
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(pGift->GetItemID());
					if (pItemData)
						msg.Format(_T("%s%s?"),gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_GET_ALL")), String(pItemData->name).c_str());
				}
				break;
			case GIFT_PARTNER:
				{
					MobDB_Client * pMobData = gGlobal.LoadMobTemplate(pGift->GetItemID());
					if (pMobData)
						msg.Format(_T("%s%s?"),gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_GET_ALL")), String(pMobData->m_basicinfo->mid_name).c_str());
				}
				break;
			case GIFT_PET:
				{
					const PetData * pPetData = gGlobal.m_resource->GetPetData(pGift->GetItemID());
					if (pPetData)
						msg.Format(_T("%s%s?"),gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_GET_ALL")), String(pPetData->pet_name).c_str());
				}
				break;	
			}
			UIStaticText* text = gGlobal.pUIManager->FindUIStaticText("ConfirmGetGiftText");
			if (msg.length() > 0 && text)
			{
				text->SetText(msg.c_str());
				text->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(text);
			}
		}
	}
}

void CMenuGiftBox::ConfirmGetGift(int slot)
{
	if (slot >= 0 && gGlobal.g_pGiftBox)
	{
		CGift* pGift = gGlobal.g_pGiftBox->GetGift(m_iMenuIndex, slot);
		if (pGift)
		{
			gGlobal.g_pGiftBox->SetTakeGiftInfo(pGift->GetGiftID());
			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_WAITING")), false);
			gGlobal.g_pGiftBox->TakeGiftBox(gGlobal.pPeerProxy, gGlobal.g_context, pGift->GetGiftID());
		}
	}
}

void CMenuGiftBox::UpdateGiftBox()
{
	switch(m_iMenuIndex)
	{
	case GIFT_ITEM:
		ShowGiftBoxItem();
		break;
	case GIFT_PARTNER:
		ShowGiftBoxPartner();
		break;
	case GIFT_PET:
		ShowGiftBoxPet();
		break;
	}
}

void CMenuGiftBox::CloseGiftBox()
{
	if (gGlobal.g_pGiftBox)
	{
		if (gGlobal.pPeerProxy)
			gGlobal.g_pGiftBox->CloseGiftBox(gGlobal.pPeerProxy, RPCContext());
		DeleteNetGroup(gGlobal.g_pGiftBox, BOOLEAN_TRUE);
		gGlobal.g_pGiftBox = NULL;
	}
}

void CMenuGiftBox::ShowDetail(int slot)
{
	if (gGlobal.g_pGiftBox && slot >= 0)
	{
		CGift* pGift = gGlobal.g_pGiftBox->GetGift(m_iMenuIndex, slot);
		if (pGift)
		{
			switch(pGift->GetItemType())
			{
			case GIFT_ITEM:
				{
					UIPanelEx* pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
					if (pPanelEx == NULL)
					{
						XML_UI::GetInstance().Init();
						XML_UI::GetInstance().ReadFormFile("Data\\ItemDetail.xml", gGlobal.pUIManager);

						pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
						if (pPanelEx == NULL)
							return;		
					}
					gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);
					gGlobal.g_pItemProcess->ItemDetailPopUp(pGift->GetItemID(), true, NULL, false);

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
				break;
			case GIFT_PARTNER:
				{
					UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
					if (pPanel == NULL)
					{
						XML_UI::GetInstance().Init();
						XML_UI::GetInstance().ReadFormFile("Data\\PartnerDetail.xml", gGlobal.pUIManager);

						pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
						if (pPanel == NULL)
							return;		
					}
					gGlobal.g_pPartnerProcess->ShowMobDetail(pGift->GetItemID(), 1);
					gGlobal.pUIManager->SetPanel2Top(pPanel);
					Int32 posX = 0, posY = 0;

					if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
						posX = 800 - pPanel->GetWidth();
					else
						posX = gGlobal.pUIManager->GetCursorX();

					if ( ( gGlobal.pUIManager->GetCursorY() + pPanel->GetHeight() ) > 600 )
						posY = 600 - pPanel->GetHeight();
					else
						posY = gGlobal.pUIManager->GetCursorY();

					pPanel->SetPos(posX, posY);
					pPanel->SetEnable(true);
				}
				break;
			case GIFT_PET:
				{
					UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
					if (pPanel == NULL)
					{
						XML_UI::GetInstance().Init();
						XML_UI::GetInstance().ReadFormFile("Data\\PetDetail.xml", gGlobal.pUIManager);

						pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
						if (pPanel == NULL)
							return;		
					}
					gGlobal.g_pMenuPetProcess->ShowPetDetail(pGift->GetItemID(), 1);

					gGlobal.pUIManager->SetPanel2Top(pPanel);
					Int32 posX = 0, posY = 0;

					if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
						posX = 800 - pPanel->GetWidth();
					else
						posX = gGlobal.pUIManager->GetCursorX();

					if ( ( gGlobal.pUIManager->GetCursorY() + pPanel->GetHeight() ) > 600 )
						posY = 600 - pPanel->GetHeight();
					else
						posY = gGlobal.pUIManager->GetCursorY();

					pPanel->SetPos(posX, posY);
					pPanel->SetEnable(true);
				}
				break;
			}
		}
	}
}

void CMenuGiftBox::DisappearGiftBox()
{
	if (m_iStartGiftBoxX >  0 || m_iStartGiftBoxY > 0)
	{
		BlockPos currPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		BlockPos center;
		center.PosX = m_iStartGiftBoxX;
		center.PosY = m_iStartGiftBoxY;
		PixelDirection dummy;

		if (QGLMath::CalculateBlockPosDistance(currPos, center, dummy) > 400)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseGiftBox", String());
			m_iStartGiftBoxX = 0;
			m_iStartGiftBoxY = 0;
		}
	}
}

void CMenuGiftBox::ShowResultMessage(UInt msgIndex, UInt32 id, UInt type)
{
	switch(msgIndex)
	{
	case GIFTBOX_SUCCESS:
		{
			String msg;
			switch(type)
			{
			case GIFT_ITEM:
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(id);
					if (pItemData)
						msg.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_SUCCESS")), String(pItemData->name).c_str());
				}
				break;
			case GIFT_PARTNER:
				{
					MobDB_Client * pMobData = gGlobal.LoadMobTemplate(id);
					if (pMobData)
						msg.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_SUCCESS")), String(pMobData->m_basicinfo->mid_name).c_str());
				}
				break;
			case GIFT_PET:
				{
					const PetData * pPetData = gGlobal.m_resource->GetPetData(id);
					if (pPetData)
						msg.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_SUCCESS")), String(pPetData->pet_name).c_str());
				}
				break;
			}
			gGlobal.PrintPlayerMessage(msg);
		}
		break;
	case GIFTBOX_ERROR:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_GET_GIFT_FAIL")));
		break;
	case GIFTBOX_NO_ITEM_SLOT:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_SLOT_FAIL")));
		break;
	case GIFTBOX_NO_PARTNER_SLOT:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_PARTNER_FAIL")));
		break;
	case GIFTBOX_NO_PET_SLOT:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_PET_FAIL")));
		break;
	}
}

void CMenuGiftBox::UpdateGiftBoxFromServer()
{
	UInt32 diffTime = GetTickCount() - m_iUpdateTime;
	if (diffTime < 60000) //300000ms = 5 min
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_UPDATE_YUANBAO_TOO_FREQ")), (300 - diffTime / 1000));
		gGlobal.PrintPlayerMessage(text.c_str());
	}
	else if (gGlobal.g_pGiftBox)
	{
		m_iUpdateTime = GetTickCount();
		gGlobal.g_pCharacter->Stop();
		gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_QUESTLOG_NONAME_TYPE")), false);
		gGlobal.g_pGiftBox->UpdateGiftBox(gGlobal.pPeerProxy, gGlobal.g_context);
	}
}