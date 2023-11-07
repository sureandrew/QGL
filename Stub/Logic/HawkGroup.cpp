//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/DataCode.h"
//-- Self
#include "Stub/Logic/HawkGroup.h"
#include "Stub/Logic/HawkItem.h"
#include "Stub/Logic/Item.h"
#include "Proxy/Data/ItemData.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Partner.h"
#include "Proxy/Data/PartnerData.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Pet.h"
#include "Proxy/Data/PetData.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "MapService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "MapService/Configure.h"
#include "Proxy/Service/LogService.h"

REGISTER_CLASSTYPE(CHawkGroup, HawkGroup_CLASSID);


CHawkGroup::CHawkGroup()
{
	m_pHawker = NULL;
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();
	m_charContextList.clear();
	m_iStartTime = 0;
}

CHawkGroup::~CHawkGroup()
{
	for (CharContextList::iterator iter = m_charContextList.begin(); iter != m_charContextList.end(); ++iter)
	{
		UnsetOwnObject(iter->second.sessionId, this);
	}

	m_pHawker = NULL;
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();
	m_charContextList.clear();
	m_iStartTime = 0;
}

void CHawkGroup::SetHawkItem(HawkerItemInfoVector itemVector, HawkerItemInfoVector partnerVector, HawkerItemInfoVector petVector)
{
	CHawkItem* hawkItem = NULL;
	if (m_pHawker)
	{
		if (m_pHawker->m_pBag)
		{
			CItem*  item = NULL;
			for (Index i = C_INDEX(0); i < itemVector.size(); ++i)
			{
				item = m_pHawker->m_pBag->GetItemByUID(itemVector[i].item_uid);
				if (item)
				{
					hawkItem = CHawkItem::NewInstance();
					if (hawkItem)
					{
						hawkItem->SetHawkData(itemVector[i], HAWKER_ITEM);
						if (hawkItem->GetItem_index() != item->GetBagIndex())
							hawkItem->SetItem_index(item->GetBagIndex());												
							
						if (item->GetStack() < hawkItem->GetItem_stack())
							hawkItem->SetItem_stack(item->GetStack());
						
						hawkItem->SetDataObjectNetID(item->GetNetId());
						Add(hawkItem);
						m_itemList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
												
					}				
				}
			}
		}

		if (m_pHawker->m_pPartnerGroup)
		{
			CPartner* partner = NULL;
			for (Index i = C_INDEX(0); i < partnerVector.size(); ++i)
			{		
				partner = m_pHawker->m_pPartnerGroup->GetPartner(partnerVector[i].item_uid);
				if (partner)
				{
					hawkItem = CHawkItem::NewInstance();
					if (hawkItem)
					{
						hawkItem->SetHawkData(partnerVector[i], HAWKER_PARTNER);						
						hawkItem->SetDataObjectNetID(partner->GetNetId());
						Add(hawkItem);
						m_partnerList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
					}
				}								
			}
		}

		if (m_pHawker->m_pPetGroup)
		{
			CPet* pet = NULL;
			for (Index i = C_INDEX(0); i < petVector.size(); ++i)
			{		
				pet = m_pHawker->m_pPetGroup->GetPet(petVector[i].item_uid);
				if (pet)
				{
					hawkItem = CHawkItem::NewInstance();
					if (hawkItem)
					{
						hawkItem->SetHawkData(petVector[i], HAWKER_PET);						
						hawkItem->SetDataObjectNetID(pet->GetNetId());
						Add(hawkItem);
						m_petList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
					}
				}				
			}
		}
	}
}

void CHawkGroup::AddStartHawkItemInfo(HawkerItemInfo itemInfo,StartHawkAddType addType)
{
	CHawkItem* hawkItem = NULL;
	if (m_pHawker)
	{
		if (m_pHawker->m_pBag)
		{
			CItem*  item =  m_pHawker->m_pBag->GetItemByUID(itemInfo.item_uid);
			if (item && addType == START_HAWK_ADD_ITEM)
			{
				hawkItem = CHawkItem::NewInstance();
				if (hawkItem)
				{
					hawkItem->SetHawkData(itemInfo, HAWKER_ITEM);
					if (hawkItem->GetItem_index() != item->GetBagIndex())
						hawkItem->SetItem_index(item->GetBagIndex());												

					if (item->GetStack() < hawkItem->GetItem_stack())
						hawkItem->SetItem_stack(item->GetStack());

					hawkItem->SetDataObjectNetID(item->GetNetId());
					Add(hawkItem);
					m_itemList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
					TRACE_ERRORDTL_1(GLOBAL_LOGGER,_F("my Server hawkItem->GetItem_uid() = %d"),hawkItem->GetItem_uid());
				}				
			}
		}

		if (m_pHawker->m_pPartnerGroup)
		{
			CPartner* partner = m_pHawker->m_pPartnerGroup->GetPartner(itemInfo.item_uid);
			if (partner && addType == START_HAWK_ADD_PARTNER)
			{
				hawkItem = CHawkItem::NewInstance();
				if (hawkItem)
				{
					hawkItem->SetHawkData(itemInfo, HAWKER_PARTNER);						
					hawkItem->SetDataObjectNetID(partner->GetNetId());
					Add(hawkItem);
					m_partnerList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				}
			}								

		}

		if (m_pHawker->m_pPetGroup && addType == START_HAWK_ADD_PET)
		{
			CPet* pet = m_pHawker->m_pPetGroup->GetPet(itemInfo.item_uid);

			if (pet)
			{
				hawkItem = CHawkItem::NewInstance();
				if (hawkItem)
				{
					hawkItem->SetHawkData(itemInfo, HAWKER_PET);						
					hawkItem->SetDataObjectNetID(pet->GetNetId());
					Add(hawkItem);
					m_petList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				}
			}				

		}
	}
	if (hawkItem != NULL)
		hawkItem->PreSend();
	NetGroup grp;
	grp.Add(hawkItem);
	CB_AddSellStartHawkItemInfo(m_pHawker->GetPeer(), m_pHawker->GetContext(),&grp);
}

void CHawkGroup::GetItemNetGroup(NetGroup* itemGroup)
{
	for (HawkItemMap::iterator itr = m_itemList.begin(); itr != m_itemList.end(); ++itr)
	{
		CItem* item = FindNetObject<CItem>(itr->second->GetDataObjectNetID());
		if (item)
		{
			item->SetSendMask(CItem::AllMask);
			itemGroup->Add(item);
		}		
	}
}
	
void CHawkGroup::GetPartnerNetGroup(NetGroup* partnerGroup, UInt16Vector& partnerSkillID)
{
	for (HawkItemMap::iterator itr = m_partnerList.begin(); itr != m_partnerList.end(); ++itr)
	{
		CPartner* partner = FindNetObject<CPartner>(itr->second->GetDataObjectNetID());
		if (partner)
		{
			CPartner* pTempPartner = CPartner::NewInstance();
			partner->ClonePartner(pTempPartner);
			pTempPartner->CalcAttrib2();
			pTempPartner->SetSendMask(CPartner::AllMask);
			partnerGroup->Add(pTempPartner);

			if (partner->GetSkillGroup())
			{
				for (UInt8 i = 0; i < partner->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = partner->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						partnerSkillID.push_back(skill->GetSkill_id());
				}
				partnerSkillID.push_back(0); //for separate the skill set for each partner
			}
		}
	}
}
	
void CHawkGroup::GetPetNetGroup(NetGroup* petGroup, UInt16Vector& petSkillID)
{
	for (HawkItemMap::iterator itr = m_petList.begin(); itr != m_petList.end(); ++itr)
	{
		CPet* pet = FindNetObject<CPet>(itr->second->GetDataObjectNetID());
		if (pet)
		{
			pet->SetSendMask(CPet::AllMask);
			petGroup->Add(pet);

			if (pet->GetSkillGroup())
			{
				for (UInt8 i = 0; i < pet->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = pet->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						petSkillID.push_back(skill->GetSkill_id());
				}
				petSkillID.push_back(0);
			}
		}		
	}
}

void CHawkGroup::PreSend()
{
	SetSendMask(CHawkGroup::AllMask);
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		CHawkItem* hawkItem = GetChild<CHawkItem>(i);
		if (hawkItem)
			hawkItem->PreSend();
	}
}

void CHawkGroup::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_NAME:				
		{
			UInt32Vector removeCharID;
			for (CharContextList::iterator iter = m_charContextList.begin(); iter != m_charContextList.end(); ++iter)
			{
				Session *psess = GetSession(iter->second.sessionId);
				if (psess && psess->pchar && psess->pchar->GetPeer())
					SendAttr(psess->pchar->GetPeer(), psess->pchar->GetContext(), CHawkGroup::hawkNameInfoMask );
				else
					removeCharID.push_back(iter->first);

			}
			for(UInt32 i = 0; i < removeCharID.size(); ++i)
				RemoveCharID(removeCharID[i]);
		}
		break;
	case EVT_UPDATE_TIMEUP:
		{
			if (m_pHawker)
			{
				m_pHawker->SetHawkerTime(0);
				if (m_pHawker->GetPeer())
					m_pHawker->CB_HawkMessage(m_pHawker->GetPeer(), m_pHawker->GetContext(), HAWK_TIMEUP);
			}
			StopHawk();
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("HawkerTimeUp: owner_id %d hawker timeup"),	GetOwnerID());
		}
		break;
	}
}

void CHawkGroup::AddCharContext(UInt32 charID,  RPCContext context)
{ 
	CharContextList::iterator itr = m_charContextList.find(charID);
	if (itr == m_charContextList.end())
	{
		m_charContextList.insert(std::make_pair(charID, context));
		SetOwnObject(context.sessionId, this);
	}
	else
	{
		UnsetOwnObject(itr->second.sessionId, this);
		itr->second.sessionId = context.sessionId;
		SetOwnObject(context.sessionId, this);
	}
}

void CHawkGroup::RemoveCharID(UInt32 charID)
{
	CharContextList::iterator itr = m_charContextList.find(charID);
	if (itr != m_charContextList.end())
	{
		UnsetOwnObject(itr->second.sessionId, this);
		m_charContextList.erase(itr);
	}
}

void CHawkGroup::StopHawk()
{
	for (CharContextList::iterator iter = m_charContextList.begin(); iter != m_charContextList.end(); ++iter)
	{
		Session *psess = GetSession(iter->second.sessionId);
		if (psess && psess->pchar && psess->pchar->GetPeer())
		{
			CB_CloseHawkForUser(psess->pchar->GetPeer(), psess->pchar->GetContext());
			UnsetOwnObject(iter->second.sessionId, this);
		}
	}
	m_charContextList.clear();


	/*UInt32 diff = ::timeGetTime() - m_iStartTime;
	diff = diff / (1000 * 60);
	if (m_pHawker->GetHawkerTime() > diff)
		m_pHawker->SetHawkerTime(m_pHawker->GetHawkerTime() - diff);
	else
		m_pHawker->SetHawkerTime(0);*/

	m_pHawker->SetHawkInfo(_T(""));

	m_pHawker->SetAction(CCharacter::ST_STOP, ::timeGetTime());
	m_pHawker->m_pHawkGroup = NULL;
	m_iStartTime = 0;

	m_pHawker->RaiseUpdate();

	if (m_pHawker->GetPeer() == NULL)
		m_pHawker->ScheduleLogout(0);
	else
		CB_StopHawk(m_pHawker->GetPeer(), m_pHawker->GetContext());

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("StopHawk: owner_id %d stop hawker"),	GetOwnerID());
	CHawkGroup* hawkGroup = this;
	DeleteNetGroup(hawkGroup, BOOLEAN_TRUE);	
}

RPCResult CHawkGroup::CloseHawkForUser(LPCPEER pPeer, RPCContext &context, const UInt32 &charID)
{
	RemoveCharID(charID);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("session_id %s, char_id %d close hawk owner_id %d session_id %s"),
		context.sessionId.GetString().c_str(), charID, m_pHawker->GetChar_id(), m_pHawker->GetSessionId().GetString().c_str());
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::ChangeHawkName(LPCPEER pPeer, RPCContext &context, const String &hawkName)
{
	if (m_pHawker->GetContext().sessionId == context.sessionId)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ChangeHawkName: sessionId %s, char_id %d close hawk"),
			context.sessionId.GetString().c_str(), GetOwnerID());
		SetHawkName(hawkName);
		m_pHawker->SetHawkInfo(hawkName);
		RaiseHawkNameUpdate();
		
	}
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::BuyHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack)
{
	Session *psess = GetSession(context.sessionId);
	if (m_pHawker && m_pHawker->GetContext().sessionId != context.sessionId && psess && psess->pchar && itemStack > 0 && itemID > 0)
	{
		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("sessionId %s, char_id %d buy hawk item item_uid %d, itemType %d, stack %d, hawker owner_id %d"),
				context.sessionId.GetString().c_str(), psess->pchar->GetChar_id(), itemID, itemType, itemStack, m_pHawker->GetChar_id());

		CCharacter* buyer = psess->pchar;
		CHawkItem* hawkItem = NULL;
		UInt8 msgIndex = HAWK_NO_MESSAGE;
		UInt tax = 0;
		UInt taxedPrice = 0;
		UInt itemPrice = 0;
		UInt32 uid = 0;
		String name = _T("");

		bool error = false;
		
		switch (itemType)
		{
		case HAWKER_ITEM:
			{
				bool needReduce = false;
				do
				{
					HawkItemMap::iterator itr = m_itemList.find(itemID);
					if (itr == m_itemList.end() || itr->second == NULL)
					{
						error = true;
						msgIndex = HAWK_NO_ITEM;
						break;
					}

					hawkItem = itr->second;

					tax = hawkItem->GetItem_price() * itemStack * gConfigure.iHawkerTaxRate / 10000;
					taxedPrice = hawkItem->GetItem_price() * itemStack - tax;
					itemPrice = hawkItem->GetItem_price() * itemStack;

					if (itemPrice > buyer->GetMoney())
					{
						error = true;
						msgIndex = HAWK_NO_MONEY;
						break;
					}
					
					CItem* orgItem = FindNetObject<CItem>(hawkItem->GetDataObjectNetID());
					if (orgItem == NULL || orgItem->m_data == NULL || orgItem->GetOwner_id() != m_pHawker->GetChar_id())
					{
						error = true;
						msgIndex = HAWK_NO_ITEM;
						break;
					}

					if (orgItem->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND)
					{
						error = true;
						msgIndex = HAWK_ITEM_BIND;
						break;
					}

					if ((orgItem->m_data->item_type == ITEMTYPE_FLOWER || orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)&&orgItem->GetItemValue1() )
					{
						error = true;
						msgIndex = HAWK_ITEM_FLOWER_MODEL;
						break;

					}

					if (hawkItem->GetItem_stack() < (itemStack + hawkItem->GetPreBuyStack() ) || orgItem->GetStack() < (itemStack + hawkItem->GetPreBuyStack()) )
					{
						error = true;
						msgIndex = HAWK_NOT_ENOUGH_HAWKITEM;
						break;
					}

					needReduce = true;
					hawkItem->SetPreBuyStack(hawkItem->GetPreBuyStack() + itemStack);

					UInt noOfItem = itemStack / orgItem->m_data->overlap;
					UInt mod = itemStack % orgItem->m_data->overlap;
					if (orgItem->m_data->overlap > 1 && mod != 0)
						++noOfItem;

					if(noOfItem > buyer->m_pBag->GetTotalFreeSlot())
					{
						error = true;
						msgIndex = HAWK_NO_ITEM_SLOT;
						break;
					}
					
					if (orgItem->m_data->overlap == 1)	//
					{
						m_pHawker->m_pBag->RemoveItem(orgItem, BOOLEAN_FALSE);
						for (UInt8 i = 0; i < BAG_TYPE_COUNT; ++i)
						{
							if (buyer->m_pBag->AddItem(orgItem, i))
								break;
						}
					}
					else
					{						
						bool canAdd = false;
						UInt8 tempItemStack = itemStack;
						UInt8 remainStack = orgItem->GetStack();
						for (Index i = C_INDEX(0); i < BAG_TYPE_COUNT; ++i)
						{
							if (buyer->m_pBag->AddItem(orgItem, i, tempItemStack))
							{									
								if (orgItem->GetStack() == 0)
									m_pHawker->m_pBag->AddToDeleteGroup(orgItem->GetBagIndex());
								canAdd = true;
								break;
							}
							else if (remainStack != orgItem->GetStack())
							{
								tempItemStack -= (remainStack - orgItem->GetStack());
								remainStack = orgItem->GetStack();
							}
						}
						if (!canAdd)
						{								
							error = true;
							msgIndex = HAWK_NO_ITEM;
							break;
						}
					}					
					
					hawkItem->SetItem_stack(hawkItem->GetItem_stack() - itemStack);
					buyer->ChangeMoney(-(Int32)(hawkItem->GetItem_price() * itemStack));
					m_pHawker->ChangeMoney(taxedPrice);

					LogService_Proxy::LogMoney(gGlobal.pLogService->pPeer, 
						m_pHawker->GetContext(), m_pHawker->GetAcct_id(), 
						m_pHawker->GetChar_id(), 
						LogService_Proxy::Money_And_Exp::SOURCE_HAWK_TAX, -(Int32)tax, m_pHawker->GetMoney() + m_pHawker->GetBankMoney());
					name.Format(_T("%s"), orgItem->m_data->name);

					//tell client to remove the item which is selled to buyer
					if (orgItem->GetStack() == 0 || orgItem->GetOwner_id() != m_pHawker->GetChar_id())
					{
						UInt32Vector removeItemList;
						UInt32Vector removeItemIDList;
						removeItemList.push_back(hawkItem->GetItem_uid());
						removeItemIDList.push_back(hawkItem->GetItem_id());

						if (orgItem->GetOwner_id() != m_pHawker->GetChar_id())
							m_pHawker->m_pBag->ChangeOwnerID(removeItemList, removeItemIDList, buyer->GetChar_id());
						
						if (m_pHawker->GetPeer())
						{
							CB_BuyHawkItemSuccess(m_pHawker->GetPeer(), m_pHawker->GetContext(), BOOLEAN_TRUE, itemID, itemType, itemStack, tax);
							m_pHawker->m_pBag->CB_EqItemGiven(m_pHawker->GetPeer(), m_pHawker->GetContext(), removeItemList);
						}
					}					
					m_pHawker->RaiseUpdate();
					m_pHawker->m_pBag->RaiseUpdate();
					buyer->RaiseUpdate();
				}while(false);

				if (needReduce && hawkItem)
					hawkItem->SetPreBuyStack(hawkItem->GetPreBuyStack() - itemStack);
			}
			break;
		case HAWKER_PARTNER:
			{
				do
				{
					HawkItemMap::iterator itr = m_partnerList.find(itemID);
					if (itr == m_partnerList.end() || itr->second == NULL)
					{
						error = true;
						msgIndex = HAWK_NO_ITEM;
						break;
					}
					
					hawkItem = itr->second;

					tax = hawkItem->GetItem_price() * gConfigure.iHawkerTaxRate / 10000;
					taxedPrice = hawkItem->GetItem_price() - tax;
					itemPrice = hawkItem->GetItem_price();
					
					if (itemPrice > buyer->GetMoney())
					{
						error = true;
						msgIndex = HAWK_NO_MONEY;
						break;
					}

					if (buyer->m_pPartnerGroup->GetCount() >= buyer->GetMaxPartner())
					{
						error = true;
						msgIndex = HAWK_NO_PARTNER_SLOT;
						break;
					}

					CPartner* partner = FindNetObject<CPartner>(itr->second->GetDataObjectNetID());
					if (partner == NULL || partner->GetOwner_id() != m_pHawker->GetChar_id())
					{
						error = true;
						msgIndex = HAWK_NO_ITEM;
						break;
					}

					if (partner->GetSoulboundState() == SOULBOUND_ALREADY_BOUND)
					{
						error = true;
						msgIndex = HAWK_PARTNER_BIND;
						break;
					}

					if (partner->GetPartner_id() == m_pHawker->GetPartner_id())
						m_pHawker->SetPartner(0, 0, _T(""), 0);
					
					UInt32Vector tempPartnerIDs;
					tempPartnerIDs.push_back(partner->GetPartner_id());
					m_pHawker->m_pPartnerGroup->ChangeOwnerID(tempPartnerIDs, buyer->GetChar_id());					
					buyer->m_pPartnerGroup->Add(partner);
					
					partner->MarkSend(CPartner::AllMask);
					NetGroup partnerGroups, skillGroups;
					partnerGroups.Add(partner);
					skillGroups.Add(partner->GetSkillGroup());

					buyer->ChangeMoney(-(Int32)hawkItem->GetItem_price());
					m_pHawker->ChangeMoney(taxedPrice);
					LogService_Proxy::LogMoney(gGlobal.pLogService->pPeer, 
						m_pHawker->GetContext(), m_pHawker->GetAcct_id(), 
						m_pHawker->GetChar_id(), 
						LogService_Proxy::Money_And_Exp::SOURCE_HAWK_TAX, -(Int32)tax, m_pHawker->GetMoney() + m_pHawker->GetBankMoney());
					buyer->ResetPartnerCollectionBuff();
					buyer->RaiseUpdate();
					m_pHawker->RaiseUpdate();
					hawkItem->SetItem_stack(hawkItem->GetItem_stack() - 1);

					name.Format(_T("%s(%s)"), partner->GetRaname().c_str(), partner->m_data->mid_name);

					if (partnerGroups.GetCount() > 0 )
					{
						if (buyer->GetPeer())
							buyer->m_pPartnerGroup->CB_AddPartnerGroup(buyer->GetPeer(), buyer->GetContext(), 
								&partnerGroups, &skillGroups);							
												
						if (m_pHawker->GetPeer())
						{
							UInt32Vector changeCharPartnerIDs;
							changeCharPartnerIDs.push_back(partner->GetPartner_id());
							CB_BuyHawkItemSuccess(m_pHawker->GetPeer(), m_pHawker->GetContext(), BOOLEAN_TRUE, itemID, itemType, itemStack, tax);
							m_pHawker->m_pPartnerGroup->CB_DeleteListSuccess(m_pHawker->GetPeer(), m_pHawker->GetContext(), changeCharPartnerIDs, FALSE);	
						}
					}
				}while(false);
			}
			break;
		case HAWKER_PET:
			{
				do
				{
					HawkItemMap::iterator itr = m_petList.find(itemID);
					if (itr == m_petList.end() || itr->second == NULL)
					{
						error = true;
						msgIndex = HAWK_NO_ITEM;
						break;
					}
					
					hawkItem = itr->second;

					tax = hawkItem->GetItem_price() * gConfigure.iHawkerTaxRate / 10000;
					taxedPrice = hawkItem->GetItem_price() - tax;
					itemPrice = hawkItem->GetItem_price();
					
					if (itemPrice > buyer->GetMoney())
					{
						error = true;
						msgIndex = HAWK_NO_MONEY;
						break;
					}
					if (buyer->m_pPetGroup->GetCount() >= buyer->GetMaxPet())
					{
						error = true;
						msgIndex = HAWK_NO_PET_SLOT;
						break;
					}

					CPet* pet = FindNetObject<CPet>(itr->second->GetDataObjectNetID());
					if (pet == NULL || pet->GetOwner_id() != m_pHawker->GetChar_id())
					{
						error = true;
						msgIndex = HAWK_NO_ITEM;
						break;
					}
					if (pet->GetSoulbound() == SOULBOUND_ALREADY_BOUND)
					{
						error = true;
						msgIndex = HAWK_PET_BIND;
						break;
					}

					if (pet->GetPet_uid() == m_pHawker->GetPet_uid())
						m_pHawker->SetPet(0, 0, _T(""), 0);

					UInt32Vector tempPetIDs;
					tempPetIDs.push_back(pet->GetPet_uid());
					m_pHawker->m_pPetGroup->ChangeOwnerID(tempPetIDs, buyer->GetChar_id());					
					buyer->m_pPetGroup->Add(pet);
					
					NetGroup petGroups, petSkillGroups;
					pet->MarkSend(CPet::AllMask);
					petGroups.Add(pet);
					petSkillGroups.Add(pet->GetSkillGroup());

					buyer->ChangeMoney(-(Int32)hawkItem->GetItem_price());
					m_pHawker->ChangeMoney(taxedPrice);
					LogService_Proxy::LogMoney(gGlobal.pLogService->pPeer, 
						m_pHawker->GetContext(), m_pHawker->GetAcct_id(), 
						m_pHawker->GetChar_id(), 
						LogService_Proxy::Money_And_Exp::SOURCE_HAWK_TAX, -(Int32)tax, m_pHawker->GetMoney() + m_pHawker->GetBankMoney());
					hawkItem->SetItem_stack(hawkItem->GetItem_stack() - 1);

					buyer->RaiseUpdate();
					m_pHawker->RaiseUpdate();
					
					name.Format(_T("%s(%s)"), pet->GetName().c_str(), pet->m_pPetData->pet_name);
					if (buyer->GetPeer())
						buyer->m_pPetGroup->CB_AddPets(buyer->GetPeer(), buyer->GetContext(), 
							&petGroups, &petSkillGroups);
											
					if (m_pHawker->GetPeer())
					{
						UInt32Vector changeCharPetIDs;
						changeCharPetIDs.push_back(pet->GetPet_uid());
						CB_BuyHawkItemSuccess(m_pHawker->GetPeer(), m_pHawker->GetContext(), BOOLEAN_TRUE, itemID, itemType, itemStack, tax);
						m_pHawker->m_pPetGroup->CB_RemovePets(m_pHawker->GetPeer(), m_pHawker->GetContext(), changeCharPetIDs);
					}
				}while(false);
			}
			break;
		}
			
		if (hawkItem && !error)
		{
			TRACE_INFODTL_6(GLOBAL_LOGGER, _F("buy success sessionId %s, char_id %d buy hawk item item_uid %d, itemType %d, stack %d, hawker owner_id %d"),
				context.sessionId.GetString().c_str(), psess->pchar->GetChar_id(), itemID, itemType, itemStack, m_pHawker->GetChar_id());

			if (buyer->GetPeer())
				CB_BuyHawkItemSuccess(buyer->GetPeer(), buyer->GetContext(), BOOLEAN_FALSE, itemID, itemType, itemStack, tax);
				
			m_pHawker->AddHistory(name, itemType, itemStack, itemPrice, tax);
			LogService_Proxy::LogExchange(gGlobal.pLogService->pPeer, context, 
				m_pHawker->GetChar_id(), buyer->GetChar_id(), 
				LogService_Proxy::Exchange::PROCESS_HAWKER, itemType, 
				hawkItem->GetItem_uid(), hawkItem->GetItem_id(), itemStack, itemPrice, tax);

			UInt32Vector removeCharID;
			for (CharContextList::iterator iter = m_charContextList.begin(); iter != m_charContextList.end(); ++iter)
			{
				Session *psess = GetSession(iter->second.sessionId);
				if (psess && psess->pchar && psess->pchar->GetPeer())
				{
					hawkItem->SendAttr(psess->pchar->GetPeer(), psess->pchar->GetContext(), CHawkItem::stackInfoMask);
					CB_UpdateHawkItem(psess->pchar->GetPeer(), psess->pchar->GetContext(), itemID, itemType);
				}
				else
					removeCharID.push_back(iter->first);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("BuyHawkItem: buyer buy success and send update the client of this session %s"),
					iter->second.sessionId.GetString().c_str());
			}
			for(UInt32 i = 0; i < removeCharID.size(); ++i)
				RemoveCharID(removeCharID[i]);

			if (hawkItem->GetItem_stack() == 0)
			{
				switch(hawkItem->GetItem_type())
				{
				case HAWKER_ITEM:
					{
						HawkItemMap::iterator itr = m_itemList.find(hawkItem->GetItem_uid());
						if (itr != m_itemList.end())
							m_itemList.erase(itr);
					}
					break;
				case HAWKER_PARTNER:
					{
						HawkItemMap::iterator itr = m_partnerList.find(hawkItem->GetItem_uid());
						if (itr != m_partnerList.end())
							m_partnerList.erase(itr);
					}
					break;
				case HAWKER_PET:
					{
						HawkItemMap::iterator itr = m_petList.find(hawkItem->GetItem_uid());
						if (itr != m_petList.end())
							m_petList.erase(itr);
					}
					break;
				}
				Remove(hawkItem);
				DeleteNetObject(hawkItem);
			}
		}
		else
		{
			if (msgIndex != HAWK_NO_MESSAGE && buyer->GetPeer())
				buyer->CB_HawkMessage(buyer->GetPeer(), buyer->GetContext(), msgIndex);
		}
	}	
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::StopHawk(LPCPEER pPeer, RPCContext &context)
{
	if (context.sessionId == m_pHawker->GetContext().sessionId)
	{
		CancelHawkNameUpdate();
		StopHawk();
	}
	return RPC_RESULT_OK;
}


void CHawkGroup::StartOfflineCount()
{
	if (m_pHawker)
	{
		m_iStartTime = ::timeGetTime();
		RaiseHawkTimeUpUpdate(m_pHawker->GetHawkerTime() * 1000 * 60);
	}
}

void CHawkGroup::StopOfflineCount()
{
	if (m_pHawker)
	{
		UInt diff = (::timeGetTime() - m_iStartTime) / (1000 * 60);
		if (m_pHawker->GetHawkerTime() > diff)
			m_pHawker->SetHawkerTime(m_pHawker->GetHawkerTime() - diff);
		else
			m_pHawker->SetHawkerTime(0);
		m_pHawker->RaiseUpdate();
		m_iStartTime = 0;
	}
}