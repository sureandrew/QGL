#include "Common.h"
#include "QGL.h"
//-- Self
#include "MenuGivenProcess.h"
//-- Library
#include "QGL/Global.h"
#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/ItemBag.h"
#include "MenuItemProcess.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "QGL/CharacterControl.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/PetGroup.h"
#include "QGL/ScriptControl.h"
#include "Proxy/Logic/ScriptSystem.h"

CMenuGivenProcess::CMenuGivenProcess()
{
	for (Index i = C_INDEX(0); i < GIVENITEM_MAX; ++i)
	{
		GivenItemInfo * temp = SafeCreateObject(GivenItemInfo);
		temp->item_id = 0;
		temp->item_level = 0;
		temp->item_stack = 0;
		temp->item_uid = 0;
		m_itemList.push_back(temp);
	}

	for (Index i = C_INDEX(0); i < GIVENPARTNER_MAX; ++i)
	{
		m_partnerList.push_back(0);
	}

	for (Index i = C_INDEX(0); i < GIVENPET_MAX; ++i)
	{
		m_petList.push_back(0);
	}

	m_iTargetCharID = 0;
	RPCNetID netId;
	m_iTargetCharNetID = netId;
	m_targetCharName = "";
	m_bNpcGiven = false;
	m_npcGivenIDList.clear();
	m_npcGivenTypeList.clear();
}

CMenuGivenProcess::~CMenuGivenProcess()
{
	for (Index i = C_INDEX(0); i < GIVENITEM_MAX; ++i)
	{
		GivenItemInfo * temp = m_itemList[i];
		SafeDeleteObject(temp);
	}
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();
	m_bNpcGiven = false;
	m_npcGivenIDList.clear();
	m_npcGivenTypeList.clear();
}

void CMenuGivenProcess::SetItemInfo(UInt8 index, UInt32 item_uid, UInt32 item_id, UInt8 item_stack, UInt8 item_level, UInt8 itemIndex_inbag)
{
	if (index < GIVENITEM_MAX )
	{
		m_itemList[index]->item_id = item_id;
		m_itemList[index]->item_level = item_level;
		m_itemList[index]->item_stack = item_stack;
		m_itemList[index]->item_uid = item_uid;
		m_itemList[index]->itemIndex_inbag = itemIndex_inbag;
	}
}

void CMenuGivenProcess::ResetAll()
{
	ResetItemPartnerPet();
	m_iTargetCharID = 0;
	RPCNetID netId;
	m_iTargetCharNetID = netId;
	m_targetCharName = "";

	if (m_bNpcGiven && gGlobal.g_pScriptControl)
	{
		UInt32Vector uidList;
		uidList.clear();
		UInt8Vector typeList;
		typeList.clear();
		gGlobal.g_pScriptControl->NpcGiven(uidList, typeList);
	}
	m_bNpcGiven = false;
	m_npcGivenIDList.clear();
	m_npcGivenTypeList.clear();
}

void CMenuGivenProcess::ResetItemPartnerPet()
{
	for (Index i = C_INDEX(0); i < GIVENITEM_MAX; ++i)
	{
		if (m_itemList[i]->item_id != 0)
		{
			CItem* item = gGlobal.m_pBag->GetItem(m_itemList[i]->itemIndex_inbag);
			if (item)
			{
				item->SetGivenStack(0);
				int oldItembag = m_itemList[i]->itemIndex_inbag / BAG_ITEM_COUNT;

				if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
				{
					int oldIndex = m_itemList[i]->itemIndex_inbag % BAG_ITEM_COUNT;
					gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
				}
			}
		}
		SetItemInfo(i, 0, 0, 0, 0, 0);
	}


	for (Index i = C_INDEX(0); i < GIVENPARTNER_MAX; ++i)
	{
		if (m_partnerList[i] != 0)
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[i]);
			if (partner)
				partner->SetGiven(false);
			m_partnerList[i] = 0;
		}
	}

	for (Index i = C_INDEX(0); i < GIVENPET_MAX; ++i)
	{
		if (m_petList[i] != 0)
		{
			CPet* pet = gGlobal.g_PetLst->GetPet(m_petList[i]);
			if (pet)
				pet->SetGiven(false);
			m_petList[i] = 0;
		}
	}
}

void CMenuGivenProcess::OpenGivenPanel(String targetName, UInt32 targetID, RPCNetID targetNetID)
{
	m_iTargetCharID = targetID;
	m_targetCharName = targetName;
	m_iTargetCharNetID = targetNetID;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenGivenPanel", String());
}

void CMenuGivenProcess::SetTargetCharIDNameToPanel()
{
	if (m_iTargetCharID > 0)
	{
		String temp;// = m_iTargetCharID;
		temp.Format(_T("%d"), m_iTargetCharID);
		gGlobal.SetUIStaticText("AcceptPersonID", temp);
		gGlobal.SetUIStaticText("AcceptPersonName", m_targetCharName);
		UIEditBox* pEditBox = gGlobal.pUIManager->FindUIEditBox("GivenMoney");
		if (pEditBox)
		{
			UInt32 money = gGlobal.g_pCharacter->GetMoney();
			UInt8 limit = 0;
			while (money != 0)
			{
				money /= 10;
				//if (money > 0)
				++limit;
			}
			pEditBox->SetEditLimit(limit);
		}
	}
}

void CMenuGivenProcess::SetItem(int givenIndex, int itemSlot, int stack)
{
	UInt itemIndex = gGlobal.g_pItemProcess->GetBagPage() * BAG_ITEM_COUNT + itemSlot;
	CItem* item = gGlobal.m_pBag->GetItem(itemIndex);
	if (item && givenIndex < GIVENITEM_MAX)
	{
		if (m_itemList[givenIndex]->item_id != 0)
		{
			CItem* item2 = gGlobal.m_pBag->GetItem(m_itemList[givenIndex]->itemIndex_inbag);
			if (item2)
				item2->SetGivenStack(0);

			int oldItembag = m_itemList[givenIndex]->itemIndex_inbag / BAG_ITEM_COUNT;

			if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
			{
				int oldIndex = m_itemList[givenIndex]->itemIndex_inbag % BAG_ITEM_COUNT;
				gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
			}
			
		}
		SetItemInfo(givenIndex, item->GetItem_uid(), item->GetItem_id(), stack, 0, itemIndex);
		item->SetGivenStack(stack);
		
		gGlobal.g_pItemProcess->UpdateItemSlot(itemSlot, true);

		//because I don't know why lua can't display the text, those code will remove if the lua set text is fixed
		{
			String temp;
			StringA textName;
			textName.Format("GivenItem00%dStack",givenIndex);
			if (stack > 1)
				temp.Format(_T("%d"), stack);
			gGlobal.SetUIStaticText(textName, temp, true);
			UIStaticText * tempText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
			if (tempText)
				tempText->SetEnable(true);
		}
	}
}

void CMenuGivenProcess::RemoveItem(int givenIndex)
{
	if (givenIndex >= 0)
	{
		CItem* item = gGlobal.m_pBag->GetItem(m_itemList[givenIndex]->itemIndex_inbag);
		if (item)
		{
			item->SetGivenStack(0);
			int oldItembag = m_itemList[givenIndex]->itemIndex_inbag / BAG_ITEM_COUNT;

			if (oldItembag == gGlobal.g_pItemProcess->GetBagPage())
			{
				int oldIndex = m_itemList[givenIndex]->itemIndex_inbag % BAG_ITEM_COUNT;
				gGlobal.g_pItemProcess->UpdateItemSlot(oldIndex, true);
			}
		}

		SetItemInfo(givenIndex, 0, 0, 0, 0, 0);
	}
}

void CMenuGivenProcess::RemovePartner(int givenIndex)
{
	if (givenIndex >= 0 && givenIndex < GIVENPARTNER_MAX)
	{
		CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[givenIndex]);
		if (partner)
			partner->SetGiven(false);
		m_partnerList[givenIndex] = 0;
	}
}

void CMenuGivenProcess::SetPartner(int givenIndex, UInt32 partnerID)
{
	if (givenIndex >= 0 && givenIndex < GIVENPARTNER_MAX && partnerID != 0)
	{
		CPartner* partner = NULL;

		if (m_partnerList[givenIndex] != 0)
		{
			partner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[givenIndex]);
			if (partner)
				partner->SetGiven(false);
		}

		partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
		if (partner)
		{
			partner->SetGiven(true);
			m_partnerList[givenIndex] = partnerID;
		}
	}
}

void CMenuGivenProcess::SetPet(int givenIndex, UInt32 petID)
{
	if (givenIndex >= 0 && givenIndex < GIVENPET_MAX && petID != 0)
	{
		CPet* pet = NULL;

		if (m_petList[givenIndex] != 0)
		{
			pet = gGlobal.g_PetLst->GetPet(m_petList[givenIndex]);
			if (pet)
				pet->SetGiven(false);
		}

		pet = gGlobal.g_PetLst->GetPet(petID);
		if (pet)
		{
			pet->SetGiven(true);
			m_petList[givenIndex] = petID;
		}
	}
}

void CMenuGivenProcess::RemovePet(int givenIndex)
{
	if (givenIndex >= 0 && givenIndex < GIVENPET_MAX)
	{
		CPet* pet = gGlobal.g_PetLst->GetPet(m_petList[givenIndex]);
		if (pet)
			pet->SetGiven(false);
		m_petList[givenIndex] = 0;
	}
}

void CMenuGivenProcess::ConfirmGiven()
{

	/*CCharacter* target = FindNetObject<CCharacter>(m_iTargetCharNetID);
	if (target == NULL )
	{
		//gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_DIS_FAIL"));
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_DIS_FAIL"),CHANNELTYPE_SYSTEM);
		return;
	}*/

	if (m_bNpcGiven)
	{
		
		UInt32Vector uidList;
		UInt8Vector typeList;
		uidList.clear();
		typeList.clear();

		for (Index i = C_INDEX(0); i < m_itemList.size(); ++i)
		{
			if (m_itemList[i]->item_id > 0 && m_itemList[i]->item_uid > 0)
			{
				uidList.push_back(m_itemList[i]->item_uid);
				typeList.push_back(CScriptSystem::GIVEN_ITEM);
			}
		}

		for (Index i = C_INDEX(0); i < m_partnerList.size(); ++i)
		{
			if (m_partnerList[i] > 0)
			{
				CPartner * tempPartner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[i]);
				if (tempPartner)
				{
					uidList.push_back(tempPartner->GetPartner_id());
					typeList.push_back(CScriptSystem::GIVEN_PARTNER);
				}
			}
		}

		for (Index i = C_INDEX(0); i < m_petList.size(); ++i)
		{
			if (m_petList[i] > 0)
			{
				CPet * tempPet = gGlobal.g_PetLst->GetPet(m_petList[i]);
				if (tempPet)
				{
					uidList.push_back(tempPet->GetPet_uid());
					typeList.push_back(CScriptSystem::GIVEN_PET);
				}
			}
		}

		if (uidList.size() == m_npcGivenIDList.size())
		{
			gGlobal.g_pScriptControl->NpcGiven(uidList, typeList);
		}
	}
	else
	{
		UIEditBox* editbox = gGlobal.pUIManager->FindUIEditBox("GivenMoney");
		int money = editbox->GetInt();
		if (money > (int)gGlobal.g_pCharacter->GetCharNetObject()->GetMoney())
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_MONEY_FAIL")),CHANNELTYPE_SYSTEM);
			return;
		}

		bool hasItem = false;
		bool hasPartner = false;
		bool hasPet = false;

		for (Index i = C_INDEX(0); i < m_itemList.size(); ++i)
		{
			if (m_itemList[i]->item_id != 0)
			{
				hasItem = true;
				break;
			}
		}

		for (Index i = C_INDEX(0); i < m_partnerList.size(); ++i)
		{
			if (m_partnerList[i] != 0)
			{
				hasPartner = true;
				break;
			}
		}

		for (Index i = C_INDEX(0); i < m_petList.size(); ++i)
		{
			if (m_petList[i] != 0)
			{
				hasPet = true;
				break;
			}
		}

		if (hasItem || hasPartner || hasPet || money > 0)
		{
			UInt8Vector givenItemIndex, givenItemStack;
			UInt32Vector givenPartnerID, givenPetID;

			for (Index i = C_INDEX(0); i < m_itemList.size(); ++i)
			{
				if (m_itemList[i]->item_id != 0)
				{
					givenItemIndex.push_back(m_itemList[i]->itemIndex_inbag);
					givenItemStack.push_back(m_itemList[i]->item_stack);
				}
			}

			for (Index i = C_INDEX(0); i < m_partnerList.size(); ++i)
			{
				if (m_partnerList[i] != 0)
					givenPartnerID.push_back(m_partnerList[i]);
			}

			for (Index i = C_INDEX(0); i < m_petList.size(); ++i)
			{
				if (m_petList[i] != 0)
					givenPetID.push_back(m_petList[i]);
			}

			gGlobal.g_pCharacter->GetCharNetObject()->GivenProcess(gGlobal.pPeerProxy, gGlobal.g_context, m_iTargetCharNetID, 
				givenItemIndex, givenItemStack, givenPartnerID, givenPetID, money);

			gGlobal.AddAlertBox(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PROCESS")), false);

		}
		else
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_GIVE_NOTHING")),CHANNELTYPE_SYSTEM);
		}
	}
}

void CMenuGivenProcess::OpenNpcGivenPanel(UInt32 npcID, UInt32Vector idList, UInt8Vector typeList, String npcMessage)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseGivenPanel", String());

	m_npcGivenIDList.resize(idList.size());
	m_npcGivenTypeList.reserve(typeList.size());

	std::copy(idList.begin(), idList.end(), m_npcGivenIDList.begin());
	std::copy(typeList.begin(), typeList.end(), m_npcGivenTypeList.begin());
	m_bNpcGiven = true;


	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenGivenPanel", String());
	
	UIEditBox* editBox = gGlobal.pUIManager->FindUIEditBox("GivenMoney");
	UIImage* img = gGlobal.pUIManager->FindUIImage("GivenMoneyBg");
	UIStaticText* npcText = gGlobal.pUIManager->FindUIStaticText("NpcGivenText");

	if (editBox && img && npcText)
	{
		editBox->SetEnable(false);
		img->SetEnable(false);

		npcText->SetText(npcMessage.c_str());
		npcText->Redraw();
		npcText->SetEnable(true);

	}
}

bool CMenuGivenProcess::IsSuitableNpcGivenItem(int itemSlot)
{
	CItem* item = gGlobal.m_pBag->GetItem(itemSlot);
	if (item && m_bNpcGiven)
	{
		UInt count1 = 0;
		UInt count2 = 0;
		for (Index i = C_INDEX(0); i < m_npcGivenIDList.size(); ++i)
		{
			if (item->GetItem_id() == m_npcGivenIDList[i] && m_npcGivenTypeList[i] == CScriptSystem::GIVEN_ITEM)
				++count1;
		}
		if (count1 == 0)
			return false;

		for (Index i = C_INDEX(0); i < m_itemList.size(); ++i)
		{
			if (item->GetItem_id() == m_itemList[i]->item_id)
				++count2;
		}
		if (count2 < count1)
			return true;
	}
	return false;
}

bool CMenuGivenProcess::IsSuitableNpcGivenPartner(UInt32 partnerID)
{
	CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID);
	if (partner)
	{
		if (partner->GetLock())
			return false;
		UInt count1 = 0;
		UInt count2 = 0;
		for (Index i = C_INDEX(0); i < m_npcGivenIDList.size(); ++i)
		{
			if (partner->GetMob_id() == m_npcGivenIDList[i] && m_npcGivenTypeList[i] == CScriptSystem::GIVEN_PARTNER)
				++count1;
		}
		if (count1 == 0)
			return false;

		for (Index i = C_INDEX(0); i < m_partnerList.size(); ++i)
		{
			if (m_partnerList[i] > 0)
			{
				CPartner* tempPartner = gGlobal.g_pPartnerGroup->GetPartner(m_partnerList[i]);
				if (tempPartner && tempPartner->GetMob_id() == partner->GetMob_id())
					++count2;
			}
		}

		if (count2 < count1)
			return true;
	}
	return false;
}

bool CMenuGivenProcess::IsSuitableNpcGivenPet(UInt32 petID)
{
	CPet* pet = gGlobal.g_PetLst->GetPet(petID);
	if (pet)
	{
		if (pet->GetLock())
			return false;

		UInt count1 = 0;
		UInt count2 = 0;
		for (Index i = C_INDEX(0); i < m_npcGivenIDList.size(); ++i)
		{
			if (pet->GetPet_id() == m_npcGivenIDList[i] && m_npcGivenTypeList[i] == CScriptSystem::GIVEN_PET)
				++count1;
		}
		if (count1 == 0)
			return false;

		for (Index i = C_INDEX(0); i < m_petList.size(); ++i)
		{
			if (m_petList[i] > 0)
			{
				CPet* tempPet = gGlobal.g_PetLst->GetPet(m_petList[i]);
				if (tempPet && tempPet->GetPet_id() == pet->GetPet_id())
					++count2;
			}
		}
		if (count2 < count1)
			return true;
	}
	return false;
}

void CMenuGivenProcess::ShowMoneyColour()
{
	UIEditBox* gmoneyBox = gGlobal.pUIManager->FindUIEditBox("GivenMoney");	
	if(gmoneyBox)
		SetEditBoxMoneyColor(gmoneyBox);
}