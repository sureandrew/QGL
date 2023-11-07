//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuPartnerCultivationUpProcess.h"
//-- Library
#include "QGLApplication.h"
#include "Global.h"
#include "QGL/CharacterControl.h"
#include "QGL/Math.h"
#include "MiddleMap.h"
#include "Common/PathFinder.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "Common/CallLuaFunc.h"
#include "MenuItemProcess.h"

CMenuPartnerCultivationUpProcess::CMenuPartnerCultivationUpProcess()
{
	m_nType = 0;
	m_SrcPartnerID = 0;
	m_DestPartnerID = 0;
	//m_DestItemUid = 0;
	//m_DestItemSlot = 0;
	m_bCanStart = false;
	m_Rate = 0;
	m_ConsumeMoney = 0;

	m_iStartMixShopX = 0;
	m_iStartMixShopY = 0;
}

CMenuPartnerCultivationUpProcess::~CMenuPartnerCultivationUpProcess()
{
	//ResetPartnerCultivationUp();
}

void CMenuPartnerCultivationUpProcess::OpenPartnerCultivationUp(int type)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CMenuPartnerCultivationUpProcess::OpenPartnerCultivationUp(), type = %d"), type);

	m_nType = type;
	m_SrcPartnerID = 0;
	m_DestPartnerID = 0;
	//m_DestItemUid = 0;
	//m_DestItemSlot = 0;
	m_bCanStart = false;
	m_Rate = 0;
	m_ConsumeMoney = 0;

	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerCultivationUpPanel");
	if (pPanel == NULL)
	{
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile("Data\\PartnerCultivationUp.xml", gGlobal.pUIManager);

		pPanel = gGlobal.pUIManager->FindUIPanel("PartnerCultivationUpPanel");
		if (pPanel == NULL)
			return;		
	}

	pPanel->SetPos(80, 40);

	UpdatePartnerCultivationUp();

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenItemMenuForPartnerCultivationUp", String());

	if (gGlobal.g_pCharacter)
	{
		m_iStartMixShopX = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosX;
		m_iStartMixShopY = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos().PosY;
	}
}

void CMenuPartnerCultivationUpProcess::ClosePartnerCultivationUp()
{
	ResetPartnerCultivationUp();

	m_iStartMixShopX = 0;
	m_iStartMixShopY = 0;
}

void CMenuPartnerCultivationUpProcess::UpdatePartnerCultivationUp()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CMenuPartnerCultivationUpProcess::UpdatePartnerCultivationUp(), m_nType = %d"), m_nType);

	UIButton* button = gGlobal.pUIManager->FindUIButton("PartnerCultivationUpByPartner");
	UIButton* button1 = gGlobal.pUIManager->FindUIButton("PartnerCultivationUpByItem");
	if (button && button1)
	{
		switch(m_nType)
		{
		case 0:
			{
				button->SetEnable(true);
				button1->SetEnable(false);
				if (m_bCanStart)
					button->SetNormalState();
				else
					button->SetDisableState();
			}
			break;
		case 1:
			{
				button->SetEnable(false);
				button1->SetEnable(true);
				if (m_bCanStart)
					button1->SetNormalState();
				else
					button1->SetDisableState();
			}
			break;
		}
	}
	else
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F(" can not find ui button."));
	}

	String text;
	StringA componentName, imageFilename;
	UIAniIcon* pAniIcon = NULL;
	UIImage* img = NULL;
	UIStaticText* pST = NULL;

	imageFilename.Format("PartnerCultivationUpPartnerBG");
	img = gGlobal.pUIManager->FindUIImage(imageFilename.c_str());
	if (img)
	{
		img->SetEnable(true);
	}

	componentName.Format("PartnerCultivationUpPartner");
	pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());
	if (pAniIcon)
	{
		if (m_SrcPartnerID)
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_SrcPartnerID);
			if (partner)
			{
				partner->SetGiven(true);

				MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(partner->GetMob_id());
				if (mobTemplate)
				{
					pAniIcon->ClearAniData();
					String pFace = String(mobTemplate->m_basicinfo->mob_faceS);
					pAniIcon->SetAnimationData(pFace.c_str());
					pAniIcon->SetEnable(true);
				}

				gGlobal.g_TempResult = 0;
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "GetPartnerCultivationUpRate", String(), partner->GetCultivation());
				m_Rate = (UInt)(gGlobal.g_TempResult);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" rate = %d"), m_Rate);

				gGlobal.g_TempResult = 0;
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "GetPartnerCultivationUpConsumeMoney", String(), m_nType, partner->GetCultivation());
				m_ConsumeMoney = (UInt32)gGlobal.g_TempResult;
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" m_ConsumeMoney = %d"), m_ConsumeMoney);
			}
		}
		else
		{
			pAniIcon->SetEnable(false);
		}
	}

	componentName.Format("PartnerCultivationUpRate");
	pST = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
	if (pST)
	{
		if (m_Rate > 0)
		{
			text.Format(_T("%d%%"), m_Rate);
			pST->SetText(text.c_str());
			pST->SetEnable(true);
			pST->Redraw();
		}
		else
			pST->SetEnable(false);
	}

	componentName.Format("PartnerCultivationUpMoney");
	pST = gGlobal.pUIManager->FindUIStaticText(componentName.c_str());
	if (pST)
	{
		if (m_ConsumeMoney > 0)
		{
			text.Format(_T("%d"), m_ConsumeMoney);
			pST->SetText(text.c_str());
			pST->SetEnable(true);
			pST->Redraw();
		}
		else
			pST->SetEnable(false);
	}

	imageFilename.Format("PartnerCultivationUpPartnerTipsBG");
	img = gGlobal.pUIManager->FindUIImage(imageFilename.c_str());
	if (img)
	{
		img->SetEnable(true);
	}

	if (m_nType == 0)
	{
		imageFilename.Format("PartnerCultivationUpPartnerGroup");
		img = gGlobal.pUIManager->FindUIImage(imageFilename.c_str());
		if (img)
		{
			img->SetEnable(true);
		}

		imageFilename.Format("PartnerCultivationUpUsePartnerBG");
		img = gGlobal.pUIManager->FindUIImage(imageFilename.c_str());
		if (img)
		{
			img->SetEnable(true);
		}

		imageFilename.Format("PartnerCultivationUpUsePartnerTipsBG");
		img = gGlobal.pUIManager->FindUIImage(imageFilename.c_str());
		if (img)
		{
			img->SetEnable(true);
		}

		componentName.Format("PartnerCultivationUpUsePartner");
		pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());
		if (pAniIcon)
		{
			if (m_DestPartnerID)
			{
				CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_DestPartnerID);
				if (partner)
				{
					partner->SetGiven(true);

					MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(partner->GetMob_id());
					if (mobTemplate)
					{
						pAniIcon->ClearAniData();
						String pFace = String(mobTemplate->m_basicinfo->mob_faceS);
						pAniIcon->SetAnimationData(pFace.c_str());
						pAniIcon->SetEnable(true);
					}
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
			}
		}
	}
	else // use item
	{
		imageFilename.Format("PartnerCultivationUpUseItemBG");
		img = gGlobal.pUIManager->FindUIImage(imageFilename.c_str());
		if (img)
		{
			img->SetEnable(true);
		}

		/*imageFilename.Format("PartnerCultivationUpUseItemTipsBG");
		img = gGlobal.pUIManager->FindUIImage(imageFilename.c_str());
		if (img)
		{
			img->SetEnable(true);
		}

		componentName.Format("PartnerCultivationUpUseItem");
		pAniIcon = (UIAniIcon*)gGlobal.pUIManager->GetUIItem(componentName.c_str());
		if (pAniIcon)
		{
			if (m_DestItemUid)
			{
				CItem* item = gGlobal.m_pBag->GetItemByUID(m_DestItemUid);
				if( item && item->m_data)
				{
					const ItemData* pItemData = item->m_data;
					if( pItemData )
					{
						pAniIcon->SetAnimationData(String(pItemData->icon2).c_str());
						pAniIcon->SetRenderStyle(0);
						pAniIcon->SetEnable(true);

						item->SetGivenStack(1);
					}
				}
				else
				{
					pAniIcon->SetEnable(false);
				}
			}
			else
			{
				pAniIcon->SetEnable(false);
			}
		}*/
	}
}

void CMenuPartnerCultivationUpProcess::ResetPartnerCultivationUp()
{
	if (m_SrcPartnerID)
	{
		CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_SrcPartnerID);
		if (partner)
		{
			partner->SetGiven(false);
		}
	}

	if (m_DestPartnerID)
	{
		CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_DestPartnerID);
		if (partner)
		{
			partner->SetGiven(false);
		}
	}

	/*if (m_DestItemUid)
	{
		CItem* item = gGlobal.m_pBag->GetItemByUID(m_DestItemUid);
		if( item && item->m_data)
		{
			item->SetGivenStack(0);

			if (gGlobal.g_pItemProcess)
			{
				int oldItembag = m_DestItemSlot / BAG_ITEM_COUNT;

				if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
				{
					int oldIndex = m_DestItemSlot % BAG_ITEM_COUNT;
					gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
				}
			}
		}
	}*/

	m_SrcPartnerID = 0;
	m_DestPartnerID = 0;
	//m_DestItemUid = 0;
	//m_DestItemSlot = 0;
	m_bCanStart = false;
	m_Rate = 0;
	m_ConsumeMoney = 0;

	UpdatePartnerCultivationUp();
}

void CMenuPartnerCultivationUpProcess::SetSrcPartner(UInt32 partnerID)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CMenuPartnerCultivationUpProcess::SetSrcPartner(), PartnerID = %d"), partnerID);

	if (m_SrcPartnerID == 0 && IsSuitableSrcPartner(partnerID))
	{
		m_SrcPartnerID = partnerID;

		if (m_nType == 1)
		{
			m_bCanStart = true;
		}

		UpdatePartnerCultivationUp();
	}
}

void CMenuPartnerCultivationUpProcess::SetDestPartner(UInt32 partnerID)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CMenuPartnerCultivationUpProcess::SetDestPartner(), PartnerID = %d"), partnerID);

	if (m_DestPartnerID == 0 && IsSuitableDestPartner(partnerID))
	{
		m_DestPartnerID = partnerID;

		m_bCanStart = true;

		UpdatePartnerCultivationUp();
	}
}

/*void CMenuPartnerCultivationUpProcess::SetDestItem(int itemSlot)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CMenuPartnerCultivationUpProcess::SetDestItem(), itemSlot = %d"), itemSlot);

	if (m_DestItemUid == 0 && IsSuitableDestItem(itemSlot))
	{
		CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
		if( item && item->m_data)
		{
			m_DestItemUid = item->GetItem_uid();
			m_DestItemSlot = itemSlot;

			m_bCanStart = true;

			UpdatePartnerCultivationUp();
		}
	}
}*/

bool CMenuPartnerCultivationUpProcess::IsSuitableSrcPartner(UInt32 partnerID)
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
	if (partner)
	{
		if (!partner->GetLock())
		{
			MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(partner->GetMob_id());
			if (mobTemplate && mobTemplate->m_basicinfo)
			{
				if (mobTemplate->m_basicinfo->race == 2
					|| mobTemplate->m_basicinfo->race == 3
					|| mobTemplate->m_basicinfo->race == 4
					|| mobTemplate->m_basicinfo->race == 5
					|| mobTemplate->m_basicinfo->race == 11)
				{
					if (partner->GetCultivation() < 10)
					{
						return true;
					}
					else
					{
						ShowResult(PARTNER_CULTIVATION_UP_FAIL_MAX_CULTIVATION, 0, partnerID);
					}
				}
				else
				{
					ShowResult(PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_RACE, 0, partnerID);
				}
			}
		}
		else
		{
			ShowResult(PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_LOCK, 0, partnerID);
		}
	}
	else
	{
		ShowResult(PARTNER_CULTIVATION_UP_FAIL_NOT_PARTNER, 0, 0);
	}
	return false;
}

bool CMenuPartnerCultivationUpProcess::IsSuitableDestPartner(UInt32 partnerID)
{
	if (m_SrcPartnerID == 0)
	{
		return false;
	}

	CPartner* src_partner = gGlobal.g_pPartnerGroup->GetPartner(m_SrcPartnerID);
	if (!src_partner)
	{
		return false;
	}

	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
	if (partner)
	{
		if (!partner->GetLock())
		{
			MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(partner->GetMob_id());
			if (mobTemplate && mobTemplate->m_basicinfo)
			{
				if (mobTemplate->m_basicinfo->race == 1)
				{
					if (partner->m_data->m_basicinfo->req_lv >= src_partner->m_data->m_basicinfo->req_lv)
					{
						if (partner->GetPractice() > src_partner->GetCultivation())
						{
							return true;
						}
						else
						{
							ShowResult(PARTNER_CULTIVATION_UP_FAIL_PRACTICE, src_partner->GetCultivation(), partnerID);
						}
					}
					else
					{
						ShowResult(PARTNER_CULTIVATION_UP_FAIL_REQLV, 0, partnerID);
					}
				}
				else
				{
					ShowResult(PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_RACE, 0, partnerID);
				}
			}
		}
		else
		{
			ShowResult(PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_RACE, 0, partnerID);
		}
	}
	else
	{
		ShowResult(PARTNER_CULTIVATION_UP_FAIL_NOT_PARTNER, 0, partnerID);
	}
	return false;
}

/*bool CMenuPartnerCultivationUpProcess::IsSuitableDestItem(int itemSlot)
{
	if (m_SrcPartnerID == 0)
	{
		return false;
	}

	CPartner* src_partner = gGlobal.g_pPartnerGroup->GetPartner(m_SrcPartnerID);
	if (!src_partner)
	{
		return false;
	}

	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if (item && item->m_data)
	{
		if (!item->GetLock())
		{
			gGlobal.g_TempResult = 0;
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CheckPartnerCultivationUpItem", String(), item->GetItem_id(), src_partner->GetCultivation());
			UInt32 result = (UInt32)gGlobal.g_TempResult;

			if (result == 0)
			{
				return true;
			}
			else
			{
				if (result == 1)
				{
					ShowResult(PARTNER_CULTIVATION_UP_FAIL_ITEMTYPE, 0, 0);
				}
				else
				{
					ShowResult(PARTNER_CULTIVATION_UP_FAIL_ITEMLEVEL, 0, 0);
				}
			}
		}
		else
		{
			ShowResult(PARTNER_CULTIVATION_UP_FAIL_ITEMLOCK, 0, 0);
		}
	}

	return false;
}*/

UInt32 CMenuPartnerCultivationUpProcess::GetSuitableDestItem()
{
	if (m_SrcPartnerID == 0)
	{
		return 0;
	}

	CPartner* src_partner = gGlobal.g_pPartnerGroup->GetPartner(m_SrcPartnerID);
	if (!src_partner)
	{
		return 0;
	}

	for (UInt8 i = 0; i < BAG_ITEM_COUNT; i++) // check current item bag.
	{
		CItem* item = gGlobal.m_pBag->GetItem(i);
		if (item && item->m_data)
		{
			if (!item->GetLock())
			{
				gGlobal.g_TempResult = 0;
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CheckPartnerCultivationUpItem", String(), item->GetItem_id(), src_partner->GetCultivation());
				UInt32 result = (UInt32)gGlobal.g_TempResult;

				if (result == 0)
				{
					return item->GetItem_uid();
				}
				else
				{
					if (result == 1)
					{
						//ShowResult(PARTNER_CULTIVATION_UP_FAIL_ITEMTYPE, 0, 0);
					}
					else
					{
						//ShowResult(PARTNER_CULTIVATION_UP_FAIL_ITEMLEVEL, 0, 0);
					}
				}
			}
			else
			{
				//ShowResult(PARTNER_CULTIVATION_UP_FAIL_ITEMLOCK, 0, 0);
			}
		}
	}

	return 0;
}

void CMenuPartnerCultivationUpProcess::PartnerCultivationUpByPartner()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuPartnerCultivationUpProcess::PartnerCultivationUpByPartner()"));

	if (m_nType == 0)
	{
		if (m_SrcPartnerID && m_DestPartnerID)
		{
			if (gGlobal.g_pCharacter->GetMoney() >= m_ConsumeMoney)
			{
				gGlobal.g_pPartnerGroup->PartnerCultivationUpByPartner(gGlobal.pPeerProxy, gGlobal.g_context, m_SrcPartnerID, m_DestPartnerID); 

				UIButton* button = gGlobal.pUIManager->FindUIButton("PartnerCultivationUpByPartner");
				if (button)
				{
					button->SetDisableState();
				}
			}
			else
			{
				ShowResult(PARTNER_CULTIVATION_UP_FAIL_MONEY, 0, 0);
			}
		}
	}
}

void CMenuPartnerCultivationUpProcess::PartnerCultivationUpByItem()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CMenuPartnerCultivationUpProcess::PartnerCultivationUpByItem()"));

	if (m_nType == 1)
	{
		if (m_SrcPartnerID)
		{
			if (gGlobal.g_pCharacter->GetMoney() >= m_ConsumeMoney)
			{
				UInt32 item_uid = GetSuitableDestItem();
				if (item_uid != 0)
				{
					gGlobal.g_pPartnerGroup->PartnerCultivationUpByItem(gGlobal.pPeerProxy, gGlobal.g_context, m_SrcPartnerID, item_uid); 

					UIButton* button = gGlobal.pUIManager->FindUIButton("PartnerCultivationUpByItem");
					if (button)
					{
						button->SetDisableState();
					}
				}
				else
				{
					ShowResult(PARTNER_CULTIVATION_UP_FAIL_ITEMTYPE, 0, 0);
				}
			}
			else
			{
				ShowResult(PARTNER_CULTIVATION_UP_FAIL_MONEY, 0, 0);
			}
		}
	}
}

void CMenuPartnerCultivationUpProcess::DisappearPartnerCultivationUp()
{
	//for disappear
	if (m_iStartMixShopX >  0 || m_iStartMixShopY > 0)
	{
		BlockPos currPos = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();
		BlockPos center;
		center.PosX = m_iStartMixShopX;
		center.PosY = m_iStartMixShopY;
		PixelDirection dummy;

		if (QGLMath::CalculateBlockPosDistance(currPos, center, dummy) > 400)
		{
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ClosePartnerCultivationUp", String());
		}
	}

}

void CMenuPartnerCultivationUpProcess::PartnerCultivationUpReply(const UInt8& msgIndex, const UInt8& msgValue, const UInt32& src_partnerID)
{
	ShowResult(msgIndex, msgValue, src_partnerID);

	UInt32 LastSrcPartnerID = m_SrcPartnerID;

	ResetPartnerCultivationUp();

	if (m_nType == 1)
	{
		SetSrcPartner(LastSrcPartnerID);
	}
}

void CMenuPartnerCultivationUpProcess::ShowResult(const UInt8& msgIndex, const UInt8& msgValue, const UInt32& src_partnerID)
{
	String temp;
	switch(msgIndex)
	{
	case PARTNER_CULTIVATION_UP_SUCCESS:
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(src_partnerID);
			if (partner)
			{
				temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_SUCCESS")), partner->GetRaname());
			}
		}
		break;
	case PARTNER_CULTIVATION_UP_FAIL:
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(src_partnerID);
			if (partner)
			{
				temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL")), partner->GetRaname(), msgValue);
			}
		}
		break;
	case PARTNER_CULTIVATION_UP_FAIL_NOT_PARTNER:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_PARTNERNOTFOUND"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_RACE:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_SRC_RACE"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_SRC_PARTNER_LOCK:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_SRC_LOCK"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_RACE:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_DEST_RACE"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_DEST_PARTNER_LOCK:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_DEST_LOCK"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_PRACTICE:
		temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_PARCTICE")), msgValue);
		break;
	case PARTNER_CULTIVATION_UP_FAIL_MONEY:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_MONEY"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_MAX_CULTIVATION:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_MAXCULTIVATION"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_REQLV:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_REQLV"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_ITEMTYPE:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_ITEMTYPE"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_ITEMLOCK:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_ITEMLOCK"));
		break;
	case PARTNER_CULTIVATION_UP_FAIL_ITEMLEVEL:
		temp = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_CULTVATION_UP_FAIL_ITEMLEVEL"));
		break;
	}
	gGlobal.PrintPlayerMessage(temp);
}