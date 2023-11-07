//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/HawkGroup.h"
#include "Proxy/Logic/HawkItem.h"
#include "common/common_binary.h"
#include "QGL/Global.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/Pet.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/PetGroup.h"
#include "QGL/CharacterControl.h"
#include "QGL/MenuHawkerProcess.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ResItem.h"

REGISTER_CLASSTYPE(CHawkGroup, HawkGroup_CLASSID);

CHawkGroup::CHawkGroup()
{
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();

	m_partnerSkillList.clear();
	m_petSkillList.clear();
}

CHawkGroup::~CHawkGroup()
{
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();

	m_partnerSkillList.clear();
	m_petSkillList.clear();
}

void CHawkGroup::ConstructHawkItem()
{
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		CHawkItem* hawkItem = GetChild<CHawkItem>(i);
		if (hawkItem)
		{
			switch (hawkItem->GetItem_type())
			{
			case HAWKER_ITEM:
				m_itemList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				break;
			case HAWKER_PARTNER:
				m_partnerList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				break;
			case HAWKER_PET:
				m_petList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				break;
			}
		}
	}
}

RPCResult CHawkGroup::CB_AddSellStartHawkItemInfo(LPCPEER pPeer, RPCContext &context,NetGroup* addHawkItem)
{
	for (Index i = C_INDEX(0); i < addHawkItem->GetCount(); ++i)
	{
		CHawkItem* HawkItem = addHawkItem->GetChild<CHawkItem>(i);
		if(HawkItem)
		{
			switch (HawkItem->GetItem_type())
			{
			case HAWKER_ITEM:
				m_itemList.insert(std::make_pair(HawkItem->GetItem_uid(), HawkItem));
				TRACE_ERRORDTL_1(GLOBAL_LOGGER,_F("my hawkItem->GetItem_uid() = %d"),HawkItem->GetItem_uid());
				break;
			case HAWKER_PARTNER:
				m_partnerList.insert(std::make_pair(HawkItem->GetItem_uid(), HawkItem));
				break;
			case HAWKER_PET:
				m_petList.insert(std::make_pair(HawkItem->GetItem_uid(), HawkItem));
				break;
			}
		}
	}
	DeleteNetGroup(addHawkItem);
	return RPC_RESULT_OK;
}

void CHawkGroup::ConstructHawkItem(NetGroup* itemGroup, NetGroup* partnerGroup, UInt16Vector partnerSkillID, NetGroup* petGroup, UInt16Vector petSkillID)
{
	ConstructHawkItem();

	for (Index i = C_INDEX(0); i < itemGroup->GetCount(); ++i)
	{
		CItem* item = itemGroup->GetChild<CItem>(i);
		if (item)
		{
			HawkItemMap::iterator itr = m_itemList.find(item->GetItem_uid());
			if (itr != m_itemList.end())
				itr->second->SetDataObject(item);
		}
	}

	UInt index = 0;
	for (Index i = C_INDEX(0); i < partnerGroup->GetCount(); ++i)
	{
		CPartner* partner = partnerGroup->GetChild<CPartner>(i);
		if (partner)
		{
			HawkItemMap::iterator itr = m_partnerList.find(partner->GetPartner_id());
			if (itr != m_partnerList.end())
				itr->second->SetDataObject(partner);

			UInt16Vector skillIDList;
			skillIDList.clear();
			for (UInt8 i = index; i < partnerSkillID.size(); ++i)
			{
				index++;
				if (partnerSkillID[i] == 0)
					break;
				skillIDList.push_back(partnerSkillID[i]);
			}
			m_partnerSkillList.insert(std::make_pair(partner->GetPartner_id(), skillIDList));
		}
	}

	index = 0;
	for (Index i = C_INDEX(0); i < petGroup->GetCount(); ++i)
	{
		CPet* pet = petGroup->GetChild<CPet>(i);
		if (pet)
		{
			HawkItemMap::iterator itr = m_petList.find(pet->GetPet_uid());
			if (itr != m_petList.end())
				itr->second->SetDataObject(pet);

			UInt16Vector skillIDList;
			skillIDList.clear();
			for (UInt8 i = index; i < petSkillID.size(); ++i)
			{
				index++;
				if (petSkillID[i] == 0)
					break;
				skillIDList.push_back(petSkillID[i]);
			}
			m_petSkillList.insert(std::make_pair(pet->GetPet_uid(), skillIDList));
		}
	}
}

RPCResult CHawkGroup::CB_CloseHawkForUser(LPCPEER pPeer, RPCContext &context)
{
	if (gGlobal.g_pTargetHawkGroup)
	{
		CHawkGroup* temp = gGlobal.g_pTargetHawkGroup;
		gGlobal.g_pTargetHawkGroup = NULL;
		gGlobal.pUIManager->DeletePanel("HawkPanel");
		gGlobal.pUIManager->DeletePanel("HawkItemCountPanel");
		DeleteNetGroup(temp, BOOLEAN_TRUE);
	}
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_ChangeHawkName(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_UpdateHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType)
{
			
		CHawkItem* item = NULL;
		switch(itemType)
		{
		case HAWKER_ITEM:
			{
				HawkItemMap::iterator itr = m_itemList.find(itemID);
				if (itr != m_itemList.end())
				{			
					UInt8 remainStack = itr->second->GetItem_stack();
					if (itr->second->GetItem_stack() == 0)
					{
						item = itr->second;
						m_itemList.erase(itr);
						Remove(item);
						DeleteNetObject(item);
					}

					if (GetOwnerID() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
						gGlobal.g_pHawkerProcess->UpdateHawkerItemList(itemID, remainStack);
					else
					{
						if (gGlobal.g_pHawkerProcess->GetHawkMenuIndex() == HAWKER_ITEM)
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SelectHawkSellItem", String());//gGlobal.g_pHawkerProcess->SelectHawkSellItem();
					}
					
				}
			}
			break;
		case HAWKER_PARTNER:
			{
				HawkItemMap::iterator itr = m_partnerList.find(itemID);
				if (itr != m_partnerList.end())
				{
					if (itr->second->GetItem_stack() == 0)
					{
						item = itr->second;
						m_partnerList.erase(itr);
						Remove(item);
						DeleteNetObject(item);
					}
				
					if (GetOwnerID() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
						gGlobal.g_pHawkerProcess->UpdateHawkerPartnerList(itemID);
					else
					{
						if (gGlobal.g_pHawkerProcess->GetHawkMenuIndex() == HAWKER_PARTNER)
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SelectHawkSellPartner", String());//gGlobal.g_pHawkerProcess->SelectHawkSellPartner();
					}
				}
			}
			break;
		case HAWKER_PET:
			{
				HawkItemMap::iterator itr = m_petList.find(itemID);
				if (itr != m_petList.end())
				{
					if (itr->second->GetItem_stack() == 0)
					{
						item = itr->second;
						m_petList.erase(itr);
						Remove(item);
						DeleteNetObject(item);
					}
					
					if (GetOwnerID() == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
						gGlobal.g_pHawkerProcess->UpdateHawkerPetList(itemID);
					else
					{
						if (gGlobal.g_pHawkerProcess->GetHawkMenuIndex() == HAWKER_PET)
							CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "SelectHawkSellPet", String());//gGlobal.g_pHawkerProcess->SelectHawkSellPet();
					}
				}
			}
			break;
		}
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_StopHawk(LPCPEER pPeer, RPCContext &context)
{
#ifdef QGL
	UIButton* button = gGlobal.pUIManager->FindUIButton("HawkerOwnerHawk");
	UIButton* button1 = gGlobal.pUIManager->FindUIButton("HawkerOwnerStopHawk");

	if (button && button1)
	{
		button1->SetEnable(false);
		button->SetNormalState();
		button->SetEnable(true);
	}

	if (gGlobal.g_pMyHawkGroup)
	{
		DeleteNetGroup(gGlobal.g_pMyHawkGroup, BOOLEAN_TRUE);
		gGlobal.g_pMyHawkGroup = NULL;
	}

	gGlobal.g_pHawkerProcess->StopHawk();
#endif
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_BuyHawkItemSuccess(LPCPEER pPeer, RPCContext &context, const Boolean &owner, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack, const UInt32 &tax)
{
	switch(itemType)
	{
	case HAWKER_ITEM:
		{
			HawkItemMap::iterator itr = m_itemList.find(itemID);
			if (itr != m_itemList.end())
			{			
				
				CItem* item = NULL;
				if (owner)
					item = gGlobal.m_pBag->GetItem(itr->second->GetItem_index());
				else if (itr->second->GetDataObject())
					item = CastNetObject<CItem>(itr->second->GetDataObject());

				if (item)
				{
					String msg;
					String countText;
					UInt32 price = itemStack * itr->second->GetItem_price();
					countText.Format(_T("%d%s%s"), itemStack,gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_ITEM")), String(item->m_data->name).c_str());
					if (owner)
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_OWNER")), countText.c_str(), price, tax);						
					else
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_BUYER")), countText.c_str(), price, countText.c_str());
					gGlobal.PrintPlayerMessage(msg);
				}
			}
		}
		break;
	case HAWKER_PARTNER:
		{
			HawkItemMap::iterator itr = m_partnerList.find(itemID);
			if (itr != m_partnerList.end())
			{
				CPartner* pPartner = NULL;
				
				if (owner)
					pPartner = gGlobal.g_pPartnerGroup->GetPartner(itemID);
				else if (itr->second->GetDataObject())
					pPartner = CastNetObject<CPartner>(itr->second->GetDataObject());

				if (pPartner)
				{
					String msg;
					if (owner)
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_OWNER")), pPartner->GetRaname().c_str(), itr->second->GetItem_price(), tax);
					else
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_BUYER")), pPartner->GetRaname().c_str(), itr->second->GetItem_price(), pPartner->GetRaname().c_str());
					
					gGlobal.PrintPlayerMessage(msg);
				}
			}
		}
		break;
	case HAWKER_PET:
		{
			HawkItemMap::iterator itr = m_petList.find(itemID);
			if (itr != m_petList.end())
			{
				CPet* pPet = NULL;
				if (owner)
					pPet = gGlobal.g_PetLst->GetPet(itemID);
				else if (itr->second->GetDataObject())
					pPet = CastNetObject<CPet>(itr->second->GetDataObject());

				if (pPet)
				{
					String msg;
					if (owner)
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_OWNER")), pPet->GetName().c_str(), itr->second->GetItem_price(), tax);
					else
						msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_BUY_SUCCESS_FOR_BUYER")), pPet->GetName().c_str(), itr->second->GetItem_price(), pPet->GetName().c_str());
					
					gGlobal.PrintPlayerMessage(msg);
				}
			}
		}
		break;
	}
	return RPC_RESULT_OK;
}

UInt16Vector CHawkGroup::GetPartnerSkillList(UInt32 partnerID)
{
	PPSkillMap::iterator itr = m_partnerSkillList.find(partnerID);
	if (itr != m_partnerSkillList.end())
		return itr->second;

	UInt16Vector dummy;
	return dummy;
}

UInt16Vector CHawkGroup::GetPetSkillList(UInt32 petID)
{
	PPSkillMap::iterator itr = m_petSkillList.find(petID);
	if (itr != m_petSkillList.end())
		return itr->second;

	UInt16Vector dummy;
	return dummy;
}

void CHawkGroup::OnUpdateHawkNameInfo(const String &hawkName)
{
	if (GetOwnerID() != gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id() )
			gGlobal.g_pHawkerProcess->UpdateHawkerTitle();
}