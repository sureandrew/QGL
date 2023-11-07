//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuTradingProcess.h"
#include "Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "QGL/CharacterControl.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "MenuItemProcess.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Trading.h"
#include "Proxy/Logic/SkillGroup.h"
#include "Proxy/Logic/Skill.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "QGLApplication.h"
#include "MenuFriendProcess.h"
#include "Proxy/Logic/Messenger.h"
#include "MenuPartnerProcess.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "Resource/ResPet.h"
#include "MenuPetProcess.h"

CMenuTradingProcess::CMenuTradingProcess()
{
		m_partnerName.clear();
		m_petName.clear();
		for (Index i = C_INDEX(0); i < TRADINGITEM_MAX; ++i)
		{
			TradingItemInfo temp;
			temp.item_id = 0;
			temp.item_level = 0;
			temp.item_stack = 0;
			temp.item_uid = 0;
			m_itemList.push_back(temp);
		}

		for (Index i = C_INDEX(0); i < TRADINGPARTNER_MAX; ++i)
		{
			m_partnerList.push_back(0);
			m_partnerName.push_back(_T(""));
		}

		for (Index i = C_INDEX(0); i < TRADINGPET_MAX; ++i)
		{
			m_petList.push_back(0);
			m_petName.push_back(_T(""));
		}
		m_targetNetID = RPCNetID(); 

		m_bConfirmTrading = false;
		m_bLockTrading = false;
		
		m_iMoney = 0;
}


bool CMenuTradingProcess::hasTarget()
{
	if (m_targetNetID != RPCNetID() )
		return true;
	return false;
}

void CMenuTradingProcess::OpenRequestTradingBox()
{
	if (hasTarget())
	{	
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenReqTrading", String());
		CCharacter* target = FindNetObject<CCharacter>(m_targetNetID);
		UIStaticText* tradingMsg = gGlobal.pUIManager->FindUIStaticText("TradingReqMsg");
		UIStaticText* tradingWarningMsg = gGlobal.pUIManager->FindUIStaticText("TradingReqWarningMsg");
		if (target && tradingMsg && tradingWarningMsg)
		{
			String msg;
			msg.Format(_T("%s%s"), target->GetNickName().c_str(),gGlobal.GetStringTable()->Get(_T("MSG_TRADING_REQUEST")));
			tradingMsg->SetText( msg.c_str());
			tradingMsg->Redraw();
			
			CMessenger* messenger = gGlobal.g_pMenuFriendProcess->FindMessenger(m_iTargetCharID);
			if (messenger == NULL || messenger->GetFriend_relation() == 0 )
			{
				msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_NOT_FRIEND_REQ"));
				tradingWarningMsg->SetText(msg.c_str());
				tradingWarningMsg->Redraw();
				gGlobal.pUIManager->AddDirtyUIItem(tradingWarningMsg);
			}
			gGlobal.pUIManager->AddDirtyUIItem(tradingMsg);			
		}
	}
}

void CMenuTradingProcess::ConfirmReqTrading()
{
	if (hasTarget())
	{
		RPCContext context;
		gGlobal.g_pCharacter->GetCharNetObject()->ReplyReqTrading(gGlobal.pPeerProxy, context, TRUE, m_targetNetID);
	}
}

void CMenuTradingProcess::RejectReqTrading()
{
	if (hasTarget())
	{
		RPCContext context;
		gGlobal.g_pCharacter->GetCharNetObject()->ReplyReqTrading(gGlobal.pPeerProxy, context, FALSE, m_targetNetID);
		ResetTargetNetID();
	}
}

void CMenuTradingProcess::SetItem(int tradingIndex, int itemSlot, int stack)
{
	UInt itemIndex = gGlobal.g_pItemProcess->GetBagPage() * BAG_ITEM_COUNT + itemSlot;
	CItem* item = gGlobal.m_pBag->GetItem(itemIndex);
	if (item && tradingIndex < TRADINGITEM_MAX)
	{
		if (m_itemList[tradingIndex].item_id != 0)
		{			
			CItem* item2 = gGlobal.m_pBag->GetItem(m_itemList[tradingIndex].itemIndex_inbag);
			if (item2)
				item2->SetGivenStack(0);

			int oldItembag = m_itemList[tradingIndex].itemIndex_inbag / BAG_ITEM_COUNT;
			if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
			{
				int oldIndex = m_itemList[tradingIndex].itemIndex_inbag % BAG_ITEM_COUNT;
				gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
			}
			
		}

		m_itemList[tradingIndex].item_id = item->GetItem_id();
		m_itemList[tradingIndex].item_stack = stack;
		item->SetGivenStack(stack);
		m_itemList[tradingIndex].item_uid = item->GetItem_uid();
		m_itemList[tradingIndex].itemIndex_inbag = itemIndex;

		if (gGlobal.g_pTrading)
		{
			gGlobal.g_pTrading->SetTradingItemSlot(gGlobal.pPeerProxy, RPCContext(), 
				itemIndex, stack,  tradingIndex);
		}

		gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot, true);

		//because I don't know why lua can't display the text, those code will remove if the lua set text is fixed
		{
			String temp;
			StringA textName;
			textName.Format("TradingItem00%dStack",tradingIndex);
			if (stack > 1)
				temp.Format(_T("%d"), stack);
			gGlobal.SetUIStaticText(textName, temp, true);
			UIStaticText * tempText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
			if (tempText)
				tempText->SetEnable(true);
		}
	}
}


void CMenuTradingProcess::RemoveItem(int tradingIndex)
{
	if (tradingIndex >= 0)
	{
		CItem* item = gGlobal.m_pBag->GetItem(m_itemList[tradingIndex].itemIndex_inbag);
		if (item)
		{
			item->SetGivenStack(0);
			int oldItembag = m_itemList[tradingIndex].itemIndex_inbag / BAG_ITEM_COUNT;
			if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
			{
				int oldIndex = m_itemList[tradingIndex].itemIndex_inbag % BAG_ITEM_COUNT;
				gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
			}
		}

		m_itemList[tradingIndex].item_id = 0;
		m_itemList[tradingIndex].item_stack = 0;
		m_itemList[tradingIndex].item_uid = 0;
		m_itemList[tradingIndex].itemIndex_inbag = 0;
		m_itemList[tradingIndex].item_level = 0;

		if (gGlobal.g_pTrading)
			gGlobal.g_pTrading->RemoveTradingItemSlot(gGlobal.pPeerProxy, RPCContext(), tradingIndex);
	}
}

void CMenuTradingProcess::RemovePartner(int tradingIndex)
{
	if (tradingIndex >= 0 && tradingIndex < TRADINGPARTNER_MAX)
	{
		CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[tradingIndex]);
		if (partner)
			partner->SetGiven(false);

		m_partnerList[tradingIndex] = 0;
		m_partnerName[tradingIndex] = _T("");

		if (gGlobal.g_pTrading)
			gGlobal.g_pTrading->RemoveTradingPartnerSlot(gGlobal.pPeerProxy, RPCContext(), tradingIndex);		
	}
}

void CMenuTradingProcess::SetPartner(int tradingIndex, UInt32 partnerID)
{
	if (tradingIndex >= 0 && tradingIndex < TRADINGPARTNER_MAX && partnerID != 0)
	{
		CPartner* partner = NULL;
		if (m_partnerList[tradingIndex] != 0)
		{
			partner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[tradingIndex]);
			if (partner)
				partner->SetGiven(false);
		}

		partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
		if (partner)
		{
			partner->SetGiven(true);
			m_partnerName[tradingIndex] = partner->GetRaname();
			m_partnerList[tradingIndex] = partnerID;
			if (gGlobal.g_pTrading)
				gGlobal.g_pTrading->SetTradingPartnerSlot(gGlobal.pPeerProxy, RPCContext(), partnerID,  tradingIndex);
		}		
	}
}

void CMenuTradingProcess::SetPet(int tradingIndex, UInt32 petID)
{
	if (tradingIndex >= 0 && tradingIndex < TRADINGPET_MAX && petID != 0)
	{
		CPet* pet = NULL;
		if (m_petList[tradingIndex] != 0)
		{
			pet = gGlobal.g_PetLst->GetPet(m_petList[tradingIndex]);
			if (pet)
				pet->SetGiven(false);
		}

		pet = gGlobal.g_PetLst->GetPet(petID);
		if (pet)
		{
			pet->SetGiven(true);
			m_petList[tradingIndex] = petID;
			m_petName[tradingIndex] = pet->GetName();
			if (gGlobal.g_pTrading)
				gGlobal.g_pTrading->SetTradingPetSlot(gGlobal.pPeerProxy, RPCContext(), petID,  tradingIndex);			
		}		
	}
}

void CMenuTradingProcess::RemovePet(int tradingIndex)
{
	if (tradingIndex >= 0 && tradingIndex < TRADINGPET_MAX)
	{
		CPet* pet = gGlobal.g_PetLst->GetPet(m_petList[tradingIndex]);
		if (pet)
			pet->SetGiven(false);

		m_petList[tradingIndex] = 0;
		m_petName[tradingIndex] = _T("");

		if (gGlobal.g_pTrading)
			gGlobal.g_pTrading->RemoveTradingPetSlot(gGlobal.pPeerProxy, RPCContext(), tradingIndex);
	}
}

void CMenuTradingProcess::DisplayTargetItemSlot(UInt32 itemID, UInt8 stack, UInt8 position)
{
	StringA iconName, stackTextName, lvlTextName ;
	iconName.Format("TradingTargetItem%03dIcon", position);
	stackTextName.Format("TradingTargetItem%03dStack", position);
	lvlTextName.Format("TradingTargetItem%03dLvl", position);

	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(iconName.c_str());
	UIStaticText* stackText = gGlobal.pUIManager->FindUIStaticText(stackTextName.c_str());
	UIStaticText* lvlText = gGlobal.pUIManager->FindUIStaticText(lvlTextName.c_str());

	if (pAniIcon && stackText && lvlText)
	{
		if (itemID == 0)
		{
			pAniIcon->SetEnable(false);
			stackText->SetEnable(false);
			lvlText->SetEnable(false);
		}
		else
		{
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(itemID);
			if (pItemData)
			{
				pAniIcon->ClearAniData();
				String itemIconName(pItemData->icon2);
				pAniIcon->SetAnimationData(itemIconName.c_str());
				pAniIcon->SetEnable(true);
			}

			if(stack > 1)
			{
				String tempStack;
				tempStack.Format(_T("%d"), stack);
				stackText->SetText(tempStack.c_str());
				stackText->Redraw();
				stackText->SetEnable(true);
			}
			else
				stackText->SetEnable(false);

			lvlText->SetEnable(false);
		}
	}
}

void CMenuTradingProcess::DisplayTargetPartnerSlot(UInt32 mobID, UInt8 position)
{
	StringA partnerIconName, lvlTextName;
	partnerIconName.Format("TradingTargetPartner%03dIcon", position);
	lvlTextName.Format("TradingTargetPartner%03dLvl", position);
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(partnerIconName.c_str());
	UIStaticText* pLvlText = gGlobal.pUIManager->FindUIStaticText(lvlTextName.c_str());

	if (pAniIcon && pLvlText)
	{
		if (mobID == 0)
		{
			pAniIcon->SetEnable(false);
			pLvlText->SetEnable(false);
		}
		else
		{
			MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(mobID);
			
			if (mobTemplate)
			{
				pAniIcon->ClearAniData();
				String pFace = String(mobTemplate->m_basicinfo->mob_faceS);
				pAniIcon->SetAnimationData(pFace.c_str());
				pAniIcon->SetEnable(true);
			}

			pLvlText->SetEnable(false);
		}
	}
}

void CMenuTradingProcess::DisplayTargetPetSlot(UInt32 petID, UInt8 position)
{
	StringA petIconName, lvlTextName;
	petIconName.Format("TradingTargetPet%03dIcon", position);
	lvlTextName.Format("TradingTargetPet%03dLvl", position);
	UIAniIcon* pAniIcon = gGlobal.pUIManager->FindUIAniIcon(petIconName.c_str());
	UIStaticText* pLvlText = gGlobal.pUIManager->FindUIStaticText(lvlTextName.c_str());

	if (pAniIcon && pLvlText)
	{
		if (petID == 0)
		{
			pAniIcon->SetEnable(false);
			pLvlText->SetEnable(false);
		}
		else
		{
			const PetData* pPetData = gGlobal.m_resource->GetPetData(petID);
			
			if (pPetData)
			{
				pAniIcon->ClearAniData();
				String pFace = String(pPetData->pet_faceS);
				pAniIcon->SetAnimationData(pFace.c_str());
				pAniIcon->SetEnable(true);
			}

			pLvlText->SetEnable(false);
		}
	}
}

void CMenuTradingProcess::ResetTradingProcess()
{
	ResetTargetNetID();
	ResetItemPartnerPet();
	
	m_bConfirmTrading = false;
	m_bLockTrading = false;
	
	m_iMoney = 0;
}

void CMenuTradingProcess::CancelTrading()
{
	if (gGlobal.g_pTrading)
		gGlobal.g_pTrading->CancelTrading(gGlobal.pPeerProxy, RPCContext() );
}

void CMenuTradingProcess::SetLock(bool lock)
{
	if (gGlobal.g_pTrading)
		gGlobal.g_pTrading->SetTradingLock(gGlobal.pPeerProxy, RPCContext(), lock);
		
	m_bLockTrading = lock;

	UIButton* confirmButton = gGlobal.pUIManager->FindUIButton("TradingYes");
	if (confirmButton)
	{
		if (lock)
		{
			if (gGlobal.g_pTrading && gGlobal.g_pTrading->IsTargetLocked() )
			{
				confirmButton->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(confirmButton);
			}
		}
		else
		{
			confirmButton->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(confirmButton);
		}
	}
}

void CMenuTradingProcess::SetTargetNetID(RPCNetID targetNetID)
{ 
	m_targetNetID = targetNetID; 
	CCharacter* target = FindNetObject<CCharacter>(m_targetNetID);
	if (target)
	{
		m_targetName = target->GetNickName();
		m_iTargetCharID = target->GetChar_id();
	}
	else
	{
		m_targetName = _T("");
		m_iTargetCharID = 0;
	}
}

void CMenuTradingProcess::OpenTradingPanel()
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenTradingPanel", String());
	CCharacter* target = FindNetObject<CCharacter>(m_targetNetID);
	UIStaticText* nameText = gGlobal.pUIManager->FindUIStaticText("TradingTargetName");
	UIStaticText* idText = gGlobal.pUIManager->FindUIStaticText("TradingTargetID");

	if (target && nameText && idText)
	{
		nameText->SetText(target->GetNickName().c_str());
		nameText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(nameText);

		String idString;
		idString.Format(_T("%d"), target->GetChar_id());
		idText->SetText(idString.c_str());
		idText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(idText);

		String msg;
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_TRADING_WITH_TARGET")), target->GetNickName().c_str());
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
}

void CMenuTradingProcess::SetMoney()
{
	UIEditBox* gmoneyBox = gGlobal.pUIManager->FindUIEditBox("TradingMoney");
	if (gmoneyBox)
	{
		int gmoney = gmoneyBox->GetInt();
		if (gmoney <= (int)gGlobal.g_pCharacter->GetCharNetObject()->GetMoney())
		{
			if (gGlobal.g_pTrading)
				gGlobal.g_pTrading->SetTradingMoney(gGlobal.pPeerProxy, RPCContext(), gmoney);
	
			m_iMoney = gmoney;
		}
	}
}

void CMenuTradingProcess::DisplayTargetMoney(UInt32 gmoney)
{
	UIStaticText* gmoneyTextBox = gGlobal.pUIManager->FindUIStaticText("TradingTargetMoney");
	if (gmoneyTextBox)
	{
		String strGmoney;
		strGmoney.Format(_T("%d"), gmoney);

		Color32 textColor = gGlobal.GetDisplayMoneyColor(gmoney);

		if (textColor != 0)
			gmoneyTextBox->SetColor(textColor);

		gmoneyTextBox->SetText(strGmoney.c_str());
		gmoneyTextBox->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(gmoneyTextBox);
	}
}

void CMenuTradingProcess::CheckMoney()
{
	UIEditBox* gmoneyBox = gGlobal.pUIManager->FindUIEditBox("TradingMoney");
	if (gmoneyBox)
	{
		int gmoney = gmoneyBox->GetInt();
		if (gmoney > (int)gGlobal.g_pCharacter->GetCharNetObject()->GetMoney())
		{
			gmoney /= 10;
			String strGmoney;
			strGmoney.Format(_T("%d"), gmoney);
			gmoneyBox->SetText(strGmoney.c_str());
			gmoneyBox->Redraw();
			gGlobal.pUIManager->AddDirtyUIItem(gmoneyBox);
		}
	}
}

void CMenuTradingProcess::ConfirmTrading()
{
	if (gGlobal.g_pTrading)
	{
		m_bConfirmTrading = true;
		gGlobal.g_pTrading->ConfirmTrading(gGlobal.pPeerProxy, RPCContext());	
	}	
}

void CMenuTradingProcess::ResetItemPartnerPet()
{
	for (Index i = C_INDEX(0); i < TRADINGITEM_MAX; ++i)
	{
		if (m_itemList[i].item_id != 0)
		{
			CItem* item = gGlobal.m_pBag->GetItem(m_itemList[i].itemIndex_inbag);
			if (item)
			{
				item->SetGivenStack(0);
				int oldItembag = m_itemList[i].itemIndex_inbag / BAG_ITEM_COUNT;

				if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
				{
					int oldIndex = m_itemList[i].itemIndex_inbag % BAG_ITEM_COUNT;
					gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
				}
			}
		}
		m_itemList[i].item_id = 0;
		m_itemList[i].item_level = 0;
		m_itemList[i].item_stack = 0;
		m_itemList[i].item_uid = 0;
		m_itemList[i].itemIndex_inbag = 0;
	}


	for (Index i = C_INDEX(0); i < TRADINGPARTNER_MAX; ++i)
	{
		if (m_partnerList[i] != 0)
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[i]);
			if (partner)
				partner->SetGiven(false);

			m_partnerList[i] = 0;
			m_partnerName[i] = _T("");
		}
	}

	for (Index i = C_INDEX(0); i < TRADINGPET_MAX; ++i)
	{
		if (m_petList[i] != 0)
		{
			CPet* pet = gGlobal.g_PetLst->GetPet(m_petList[i]);
			if (pet)
				pet->SetGiven(false);

			m_petList[i] = 0;
			m_petName[i] = _T("");
		}
	}
}

void CMenuTradingProcess::ShowItemDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	if (pPanel && gGlobal.g_pTrading)
	{
		Int posX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX();  
		Int posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY();
		if (posX > 0 && posY > 0 && posX < (Int)pPanel->GetWidth() && posY < (Int)pPanel->GetHeight())
		{
			Int tposX = posX - 29;
			Int tposY = posY - 99;

			Int mposX = posX - 29;
			Int mposY = posY - 270;

			CItem* item = NULL;
			if (tposX > 0 && tposY > 0 && tposX < 246 && tposY < 46)
			{
				UInt index = tposX / 43;
				item = gGlobal.g_pTrading->GetTargetItem(index);
			}
			else if (mposX > 0 && mposY > 0 && mposX < 246 && mposY < 46)
			{
				UInt index = mposX / 43;
				if (m_itemList.size() > index && m_itemList[index].item_id > 0)
					item = gGlobal.m_pBag->GetItem(m_itemList[index].itemIndex_inbag);
			}
			
			if (item)
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
				gGlobal.g_pItemProcess->ItemDetailPopUp(item->GetItem_id(), false, item);

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
	
void CMenuTradingProcess::ShowPartnerDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	if (pPanel && gGlobal.g_pTrading)
	{
		Int posX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX();  
		Int posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY();
		if (posX > 0 && posY > 0 && posX < (Int)pPanel->GetWidth() && posY < (Int)pPanel->GetHeight())
		{
			Int tposX = posX - 25;
			Int tposY = posY - 165;

			Int mposX = posX - 25;
			Int mposY = posY - 336;
			UInt index = 0;

			CPartner* pPartner = NULL;
			UInt16Vector tempSkillList;
			if (tposX > 0 && tposY > 0 && tposX < 129 && tposY < 39)
			{
				index = tposX / 43;
				pPartner = gGlobal.g_pTrading->GetTargetPartner(index);
				tempSkillList = gGlobal.g_pTrading->GetTargetPartnerSkill(index);
			}
			else if (mposX > 0 && mposY > 0 && mposX < 129 && mposY < 39)
			{
				index = mposX / 43;
				if (m_partnerList.size() && m_partnerList[index] > 0)
					pPartner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[index]);

				
				if (pPartner->m_pSkillGrp)
				{
					for (UInt32 i = 0; i < pPartner->m_pSkillGrp->GetCount(); ++i)
					{
						CSkill* pSkill = pPartner->m_pSkillGrp->GetChild<CSkill>(i);
						if (pSkill)
							tempSkillList.push_back(pSkill->GetSkill_id());
					}
				}
			}

			if (pPartner)
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
				gGlobal.g_pPartnerProcess->ShowPartnerDetail(pPartner, tempSkillList);
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
		}
	}
}

void CMenuTradingProcess::ShowPetDetail()
{
	UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("TradingPanel");
	if (pPanel && gGlobal.g_pTrading)
	{
		Int posX = gGlobal.pUIManager->GetCursorX() - pPanel->GetX();  
		Int posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetY();
		if (posX > 0 && posY > 0 && posX < (Int)pPanel->GetWidth() && posY < (Int)pPanel->GetHeight())
		{
			Int tposX = posX - 159;
			Int tposY = posY - 165;

			Int mposX = posX - 159;
			Int mposY = posY - 336;

			UInt index = 0;
			CPet* pPet = NULL;
			if (tposX > 0 && tposY > 0 && tposX < 129 && tposY < 39)
			{
				index = tposX / 43;
				pPet = gGlobal.g_pTrading->GetTargetPet(index);	
			}
			else if (mposX > 0 && mposY > 0 && mposX < 129 && mposY < 39)
			{
				index = mposX / 43;
				if (m_petList.size() && m_petList[index] > 0)
					pPet = gGlobal.g_PetLst->GetPet(m_petList[index]);				
			}

			if (pPet)
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
				gGlobal.g_pMenuPetProcess->ShowPetDetail(pPet, gGlobal.g_pTrading->GetTargetPetSkill(index));
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
		}
	}
}

void CMenuTradingProcess::ShowTradingResultText()
{
	String itemText, partnerText, petText, moneyText;

	if (m_itemList.size() > 0)
	{
		bool hasItem = false;
		for (Index i = C_INDEX(0); i < m_itemList.size(); ++i)
		{
			if (m_itemList[i].item_id > 0)
			{
				hasItem = true;
				break;
			}
		}	

		if (hasItem)
		{
			String msg;
			msg.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_TO")), m_targetName.c_str());
			
			for (Index i = C_INDEX(0); i < m_itemList.size(); ++i)
			{
				if (m_itemList[i].item_id != 0)
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(m_itemList[i].item_id);
					if (pItemData)
						msg.Format(_T("%s%d%s%s,"), msg.c_str(), m_itemList[i].item_stack,gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_ITEM")), String(pItemData->name).c_str());					
				}
			}
			gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
		}
	}

	if (m_partnerName.size() > 0)
	{
		bool hasPartner = false;
		for (Index i = C_INDEX(0); i < m_partnerList.size(); ++i)
		{
			if (m_partnerList[i] > 0)
			{
				hasPartner = true;
				break;
			}
		}

		if (hasPartner)
		{
			String msg;			
			msg.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE3")));
			for (Index i = C_INDEX(0); i < m_partnerName.size(); ++i)
			{				
				if (!m_partnerName[i].Equals(_T("")))
					msg.Format(_T("%s%s,"), msg.c_str(), m_partnerName[i].c_str());
			}
			msg.Format(_T("%s%s%s"), msg.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE4")), m_targetName.c_str());
		
			gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
		}
	}

	if (m_petName.size() > 0)
	{
		bool hasPet = false;
		for (Index i = C_INDEX(0); i < m_petList.size(); ++i)
		{
			if (m_petList[i] > 0)
			{
				hasPet = true;
				break;
			}
		}

		if (hasPet)
		{
			String msg;
			
			msg.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PET_GIVE3")));
			for (Index i = C_INDEX(0); i < m_petName.size(); ++i)
			{				
				if (!m_petName[i].Equals(_T("")))
					msg.Format(_T("%s%s,"), msg.c_str(), m_partnerName[i].c_str());
			}
			msg.Format(_T("%s%s%s"), msg.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE4")), m_targetName.c_str());
		
			gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
		}
	}

	if (m_iMoney > 0)
	{
		String msg;
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_MONEY")), m_iMoney);
		msg.Format(_T("%s%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_TO")), m_targetName.c_str(), msg.c_str());
	
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
}

bool CMenuTradingProcess::IsConfirm()
{
	return (m_bConfirmTrading || (gGlobal.g_pTrading && gGlobal.g_pTrading->IsTargetConfirmed() ) );
}

CMenuTradingProcess::~CMenuTradingProcess()
{
	ResetTargetNetID();
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();
	m_partnerName.clear();
	m_petName.clear();
	m_iMoney = 0;
}

void CMenuTradingProcess::ShowMoneyColour()
{
	UIEditBox* TradingMoneyBox = gGlobal.pUIManager->FindUIEditBox("TradingMoney");	
	if(TradingMoneyBox)
		SetEditBoxMoneyColor(TradingMoneyBox);
}