//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/DataCode.h"
//-- Self
#include "Stub/Logic/Trading.h"
#include "Stub/Logic/Character.h"
#include "Resource/ResItem.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Proxy/Data/ItemData.h"
#include "Proxy/Data/PartnerData.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "MapService/Global.h"
#include "Proxy/Service/LogService.h"


REGISTER_CLASSTYPE(CTrading, Trading_CLASSID);

RPCResult CTrading::SetTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &itemBagIndex, const UInt8 &stack, const UInt8 &tradingPosition)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && tradingPosition < 6 && (!m_bChar1Confirm && !m_bChar2Confirm))
	{
		CItem* item = NULL;
		CCharacter* destination = NULL;
		
		if (pChar1->GetContext().sessionId == context.sessionId )
		{
			item = pChar1->m_pBag->GetItem(itemBagIndex);
			if (item)
			{	
				m_char1ItemVector[tradingPosition].itemBagIndex = itemBagIndex;
				m_char1ItemVector[tradingPosition].stack = stack;
				destination = pChar2;
			}
		}
		else if (pChar2->GetContext().sessionId == context.sessionId)
		{
			item = pChar2->m_pBag->GetItem(itemBagIndex);
			if (item)
			{				
				m_char2ItemVector[tradingPosition].itemBagIndex = itemBagIndex;
				m_char2ItemVector[tradingPosition].stack = stack;
				destination = pChar1;
			}
		}

		if (destination && item && destination->GetPeer())
		{
			CItem temp;
			CItemData tempData;
			
			item->SetUpdateMask(CItem::AllMask);
			item->WriteData(&tempData);
			item->ClearAllMask();
			
			temp.SetData(&tempData);
			temp.SetId(0, temp.GetItem_id());
			temp.SetStack(stack);
			temp.SetSendMask(CItem::AllMask);
			CB_SetItemSlot(destination->GetPeer(), destination->GetContext(), &temp, tradingPosition);

			TRACE_INFODTL_8(GLOBAL_LOGGER, _F("SetTradingItemSlot: netid %d:%d, sessionId %s, set item_id %d to char_id %d trading srvId %d and objId %d trading ptr %d"),
				GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str(), item->GetItem_uid(), destination->GetChar_id(), GetNetId().srvId, GetNetId().objId, this );

			ResetLock();
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::SetTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID, const UInt8 &position)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && position < 3  && (!m_bChar1Confirm && !m_bChar2Confirm))
	{
		CPartner* pPartner = NULL;
		CCharacter* source = NULL;
		CCharacter* destination = NULL;
		if (pChar1->GetContext().sessionId == context.sessionId )
		{
			pPartner = pChar1->m_pPartnerGroup->GetPartner(partnerID);
			if (pPartner)
			{	
				m_char1PartnerVector[position] = partnerID;
				source = pChar1;
				destination = pChar2;
			}
		}
		else if (pChar2->GetContext().sessionId == context.sessionId )
		{
			pPartner = pChar2->m_pPartnerGroup->GetPartner(partnerID);
			if (pPartner)
			{	
				m_char2PartnerVector[position] = partnerID;
				source = pChar2;
				destination = pChar1;
			}
		}

		if (source && destination && pPartner && destination->GetPeer())
		{
			CPartner temp;				
			pPartner->ClonePartner(&temp);
			ZeroMemory(&temp.m_AttriOfPartnerCollection, sizeof(AttriStruct2));
			temp.CalcAttrib();
			
			temp.SetId(0, temp.GetMob_id());
			temp.SetSendMask(CPartner::AllMask);
			UInt16Vector skillIDs;
			if (pPartner->GetSkillGroup())
			{
				for (UInt8 i = 0; i < pPartner->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = pPartner->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						skillIDs.push_back(skill->GetSkill_id());
				}
			}
			CB_SetPartnerSlot(destination->GetPeer(), destination->GetContext(), &temp, skillIDs, position);
			
			TRACE_INFODTL_9(GLOBAL_LOGGER, _F("SetTradingPartnerSlot: netid %d:%d, sessionId %s, set mob_id %d partner_id %d from char_id %d to char_id %d trading srvId %d and objId %d "),
				GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str(), pPartner->GetMob_id(), pPartner->GetPartner_id(), source->GetChar_id(), destination->GetChar_id(), GetNetId().srvId, GetNetId().objId );

			ResetLock();
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CTrading::SetTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt32 &petID, const UInt8 &position)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && position < 3 && (!m_bChar1Confirm && !m_bChar2Confirm))
	{
		CPet* pPet = NULL;
		CCharacter* source = NULL;
		CCharacter* destination = NULL;
		if (pChar1->GetContext().sessionId == context.sessionId)
		{
			pPet = pChar1->m_pPetGroup->GetPet(petID);
			if (pPet)
			{	
				m_char1PetVector[position] = petID;
				source = pChar1;
				destination = pChar2;
			}
		}
		else if (pChar2->GetContext().sessionId == context.sessionId)
		{
			pPet = pChar2->m_pPetGroup->GetPet(petID);
			if (pPet)
			{	
				m_char2PetVector[position] = petID;
				source = pChar2;
				destination = pChar1;
			}
		}

		if (source && destination && pPet && destination->GetPeer())
		{
			CPet temp;				
			pPet->ClonePet(&temp);
			
			temp.SetId(0, temp.GetPet_id());
			temp.SetSendMask(CPet::AllMask);
			UInt16Vector skillIDs;
			if (pPet->GetSkillGroup())
			{
				for (Index i = 0; i < pPet->GetSkillGroup()->GetCount(); ++i)
				{
					CSkill* skill = pPet->GetSkillGroup()->GetChild<CSkill>(i);
					if (skill)
						skillIDs.push_back(skill->GetSkill_id());
				}
			}
			CB_SetPetSlot(destination->GetPeer(), destination->GetContext(), &temp, skillIDs, position);

			TRACE_INFODTL_9(GLOBAL_LOGGER, _F("SetTradingPetSlot: netid %d:%d, sessionId %s, set pet_id %d, pet_uid %d from char_id %d to char_id %d trading srvId %d and objId %d"),
				GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str(), pPet->GetPet_id(), pPet->GetPet_uid(), source->GetChar_id(), destination->GetChar_id(), GetNetId().srvId, GetNetId().objId);

			ResetLock();
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CTrading::SetTradingLock(LPCPEER pPeer, RPCContext &context, const Boolean &lock)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && (!m_bChar1Confirm && !m_bChar2Confirm) )
	{
		if (lock)
		{
			if (pChar1->GetContext().sessionId == context.sessionId)
			{
				m_bChar1Lock = lock;
				CB_SetLock(pChar2->GetPeer(), pChar2->GetContext(), pChar1->GetChar_id(), lock);
			
				TRACE_INFODTL_6(GLOBAL_LOGGER, _F("SetTradingLock: netid %d:%d, sessionId %s, char_id %d set lock trading srvId %d and objId %d"),
					GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str(), pChar1->GetChar_id(), GetNetId().srvId, GetNetId().objId);
			}
			else if (pChar2->GetContext().sessionId == context.sessionId )
			{
				m_bChar2Lock = lock;
				CB_SetLock(pChar1->GetPeer(), pChar1->GetContext(), pChar2->GetChar_id(), lock);
			
				TRACE_INFODTL_6(GLOBAL_LOGGER, _F("SetTradingLock: netid %d:%d, sessionId %s, char_id %d set lock trading srvId %d and objId %d"),
					GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str(), pChar2->GetChar_id(), GetNetId().srvId, GetNetId().objId );
			}
		}
		else
			ResetLock();
	}
	return RPC_RESULT_OK;
}


RPCResult CTrading::SetTradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && (!m_bChar1Confirm && !m_bChar2Confirm))
	{
		if (pChar1->GetContext().sessionId == context.sessionId )
		{
			m_char1Gmoney = gmoney;
			CB_SetMoney(pChar2->GetPeer(), pChar2->GetContext(), gmoney);

			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("SetTradingLock: netid %d:%d, sessionId %s, char_id %d set money %d "),
				GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str(), pChar1->GetChar_id(), gmoney);
		}
		else if (pChar2->GetContext().sessionId == context.sessionId )
		{
			m_char2Gmoney = gmoney;
			CB_SetMoney(pChar1->GetPeer(), pChar1->GetContext(), gmoney);
			
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("SetTradingLock: netid %d:%d, sessionId %s, char_id %d set money %d "),
				GetNetId().srvId, GetNetId().objId, context.sessionId.GetString().c_str(), pChar1->GetChar_id(), gmoney);
		}

		ResetLock();
	}
	return RPC_RESULT_OK;
}


RPCResult CTrading::RemoveTradingItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &tradingPosition)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && tradingPosition < 6 && (!m_bChar1Confirm && !m_bChar2Confirm))
	{
		if (pChar1->GetContext().sessionId == context.sessionId )
		{
			m_char1ItemVector[tradingPosition].itemBagIndex = NO_ITEM_INDEX;
			m_char1ItemVector[tradingPosition].stack = 0;

			CB_RemoveItemSlot(pChar2->GetPeer(), pChar2->GetContext(), tradingPosition);
		}
		else if (pChar2->GetContext().sessionId == context.sessionId)
		{
			m_char2ItemVector[tradingPosition].itemBagIndex = NO_ITEM_INDEX;
			m_char2ItemVector[tradingPosition].stack = 0;
			
			CB_RemoveItemSlot(pChar1->GetPeer(), pChar1->GetContext(), tradingPosition);
		}
		ResetLock();
	}
	return RPC_RESULT_OK;
}


RPCResult CTrading::RemoveTradingPartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && position < 3 && (!m_bChar1Confirm && !m_bChar2Confirm))
	{
		if (pChar1->GetContext().sessionId == context.sessionId )
		{
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("RemoveTradingPartnerSlot: (1) netid %d:%d, char_id %d, pet_uid %d, position %d"),
				GetNetId().srvId, GetNetId().objId, pChar1->GetChar_id(), m_char1PartnerVector[position], position);
			m_char1PartnerVector[position] = 0;
			
			CB_RemovePartnerSlot(pChar2->GetPeer(), pChar2->GetContext(), position);
		}
		else if (pChar2->GetContext().sessionId == context.sessionId)
		{
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("RemoveTradingPartnerSlot: (2) netid %d:%d, char_id %d, pet_uid %d, position %d"),
				GetNetId().srvId, GetNetId().objId, pChar2->GetChar_id(), m_char2PartnerVector[position], position);
			m_char2PartnerVector[position] = 0;
			
			CB_RemovePartnerSlot(pChar1->GetPeer(), pChar1->GetContext(), position);
		}
		ResetLock();
	}
	return RPC_RESULT_OK;
}

RPCResult CTrading::RemoveTradingPetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
	if (pChar1 && pChar2 && position < 3 && (!m_bChar1Confirm && !m_bChar2Confirm))
	{
		if (pChar1->GetContext().sessionId == context.sessionId )
		{
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("RemoveTradingPetSlot: (1) netid %d:%d, char_id %d, pet_uid %d, position %d"),
				GetNetId().srvId, GetNetId().objId, pChar1->GetChar_id(), m_char1PetVector[position], position);
			m_char1PetVector[position] = 0;
			
			CB_RemovePetSlot(pChar2->GetPeer(), pChar2->GetContext(), position);
		}
		else if (pChar2->GetContext().sessionId == context.sessionId )
		{
			TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("RemoveTradingPetSlot: (2) netid %d:%d, char_id %d, pet_uid %d, position %d"),
				GetNetId().srvId, GetNetId().objId, pChar2->GetChar_id(), m_char2PetVector[position], position);
			m_char2PetVector[position] = 0;
			
			CB_RemovePetSlot(pChar1->GetPeer(), pChar1->GetContext(), position);
		}
		ResetLock();
	}
	return RPC_RESULT_OK;
}


RPCResult CTrading::CancelTrading(LPCPEER pPeer, RPCContext &context)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);

	if (pChar1->GetContext().sessionId == context.sessionId)
	{
		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("CancelTrading: (1) netid %d:%d, char_id %d, trading ptr %d"),
			GetNetId().srvId, GetNetId().objId, pChar1->GetChar_id(), this);
		StopTrading(pChar1->GetChar_id());
	}
	else if (pChar2->GetContext().sessionId == context.sessionId)
	{
		TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F("CancelTrading: (2) netid %d:%d, char_id %d, trading ptr %d"),
			GetNetId().srvId, GetNetId().objId, pChar2->GetChar_id(), this);
		StopTrading(pChar2->GetChar_id());
	}
	
	return RPC_RESULT_OK;
}

void CTrading::TradingItem(CCharacter* source, CCharacter* target, 
						   ItemSlotVector sourceItemVector)
{
	UInt32Vector itemUIDs;
	UInt32Vector itemIDs;
	UInt32Vector changeOwnerItemUIDs;	//the item only chnage the owner, such as the weapon
	UInt32Vector changeOwnerItemIDs;
	UInt8Vector itemStacks;
	UInt8Vector itemTypes;

	for (Index i = 0; i < sourceItemVector.size(); ++i)
	{
		if (sourceItemVector[i].itemBagIndex != NO_ITEM_INDEX)
		{
			CItem* orgItem = source->m_pBag->GetItem(sourceItemVector[i].itemBagIndex);
			if (orgItem)
			{
				itemUIDs.push_back(orgItem->GetItem_uid());
				itemIDs.push_back(orgItem->GetItem_id());
				itemTypes.push_back(LogService_Proxy::Exchange::EXCHANGE_ITEM);

				if (orgItem->m_data->overlap == 1)	//
				{
					source->m_pBag->RemoveItem(orgItem, BOOLEAN_FALSE);
					target->m_pBag->AddItem(orgItem, 0);
					changeOwnerItemUIDs.push_back(orgItem->GetItem_uid());
					changeOwnerItemIDs.push_back(orgItem->GetItem_id());
					itemStacks.push_back(1);
				}
				else
				{						
					if (target->m_pBag->AddItem(orgItem, 0, sourceItemVector[i].stack))
					{							
						itemStacks.push_back(sourceItemVector[i].stack);
						if (orgItem->GetStack() == 0)
							source->m_pBag->AddToDeleteGroup(orgItem->GetBagIndex());
					}					
				}
			}
		}
	}

	if (itemIDs.size() > 0)
		CB_TradingItem(target->GetPeer(), target->GetContext(), itemIDs, itemStacks);
					
	if (changeOwnerItemUIDs.size() > 0)
	{
		source->m_pBag->ChangeOwnerID(changeOwnerItemUIDs, changeOwnerItemIDs, target->GetChar_id());
		source->m_pBag->CB_EqItemGiven(source->GetPeer(), source->GetContext(), changeOwnerItemUIDs);		
	}
	source->m_pBag->RaiseUpdate();
	target->m_pBag->RaiseUpdate();

	LogService_Proxy::LogExchangeForItemList(gGlobal.pLogService->pPeer, source->GetContext(), source->GetChar_id(), target->GetChar_id(), 
		LogService_Proxy::Exchange::PROCESS_TRADING, itemTypes, itemUIDs, itemIDs, itemStacks);
}

void CTrading::TradingPartner(CCharacter* source, CCharacter* target, UInt32Vector sourcePartnerVector)
{
	UInt32Vector changeCharPartnerIDs;
	NetGroup partnerGroups;
	NetGroup skillGroups;
	UInt32Vector modIDs;
	UInt8Vector stacks;
	UInt8Vector type;

	for (Index i = 0; i < sourcePartnerVector.size(); ++i)
	{
		if (sourcePartnerVector[i] != 0)
		{
			CPartner* partner = source->m_pPartnerGroup->GetPartner(sourcePartnerVector[i]);
			if (partner)
			{
				if (partner->GetPartner_id() == source->GetPartner_id())
					source->SetPartner(0, 0, _T(""), 0);

				TRACE_VERBOSEDTL_6(GLOBAL_LOGGER, _F("TradingPartner: netid %d:%d, mob_id %d, partner_id %d, from char_id %d to char_id %d"), 
					GetNetId().srvId, GetNetId().objId, partner->GetMob_id(), partner->GetPartner_id(), partner->GetOwner_id(), target->GetChar_id());

				changeCharPartnerIDs.push_back(partner->GetPartner_id());
				modIDs.push_back(partner->GetMob_id());
				stacks.push_back(1);
				type.push_back(LogService_Proxy::Exchange::EXCHANGE_PARTNER);

				partner->MarkSend(CPartner::AllMask);
				partnerGroups.Add(partner);
				skillGroups.Add(partner->GetSkillGroup());
			}
		}
	}

	if (partnerGroups.GetCount() > 0)
	{
		source->m_pPartnerGroup->ChangeOwnerID(changeCharPartnerIDs, target->GetChar_id());
		for (Index i = 0; i < partnerGroups.GetCount(); ++i)
		{
			CPartner* partner = partnerGroups.GetChild<CPartner>(i);
			if (partner)
			{
				partner->RemoveBuffSchool(PARTNER_COLLECTION_BUFF_SCHOOL_ID);
				ZeroMemory(&partner->m_AttriOfPartnerCollection, sizeof(AttriStruct2));
				partner->CalcAttrib();
				target->m_pPartnerGroup->Add(partner);
			}
		}
		target->ResetPartnerCollectionBuff();

		target->m_pPartnerGroup->CB_AddPartnerGroup(target->GetPeer(), target->GetContext(), 
			&partnerGroups, &skillGroups);
		if (changeCharPartnerIDs.size() > 0)
			source->m_pPartnerGroup->CB_DeleteListSuccess(source->GetPeer(), source->GetContext(), changeCharPartnerIDs, FALSE);	

		CB_TradingPartner(target->GetPeer(), target->GetContext(), changeCharPartnerIDs);
		
		LogService_Proxy::LogExchangeForItemList(gGlobal.pLogService->pPeer, source->GetContext(), source->GetChar_id(), target->GetChar_id(), 
		LogService_Proxy::Exchange::PROCESS_TRADING, type, changeCharPartnerIDs, modIDs, stacks);
	}
}

void CTrading::TradingPet(CCharacter* source, CCharacter* target, UInt32Vector sourcePetVector)
{
	UInt32Vector changeCharPetIDs;
	NetGroup petGroups;
	NetGroup petSkillGroups;
	UInt32Vector petIDs;
	UInt8Vector stacks;
	UInt8Vector type;

	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("TradingPet: pet_uid %s"), GetVectorString(sourcePetVector).c_str());

	for (Index i = 0; i < sourcePetVector.size(); ++i)
	{
		if (sourcePetVector[i] != 0)
		{
			CPet* pet = source->m_pPetGroup->GetPet(sourcePetVector[i]);
			if (pet)
			{
				if (pet->GetPet_uid() == source->GetPet_uid())
					source->SetPet(0, 0, _T(""), 0);

				TRACE_VERBOSEDTL_6(GLOBAL_LOGGER, _F("TradingPet: netid %d:%d, pet_id %d, pet_uid %d, from char_id %d to char_id %d"), 
					GetNetId().srvId, GetNetId().objId, pet->GetPet_id(), pet->GetPet_uid(), pet->GetOwner_id(), target->GetChar_id());

				pet->MarkSend(CPet::AllMask);
				petGroups.Add(pet);
				petSkillGroups.Add(pet->GetSkillGroup());

				changeCharPetIDs.push_back(pet->GetPet_uid());
				petIDs.push_back(pet->GetPet_id());
				stacks.push_back(1);
				type.push_back(LogService_Proxy::Exchange::EXCHANGE_PET);

				TRACE_INFODTL_6(GLOBAL_LOGGER, _F("TradingPet: from char_id %d to char_id %d and pet_uid %d, trading srvId %d and objId %d, trading ptr %d"), 
					source->GetChar_id(), target->GetChar_id(), pet->GetPet_uid(), GetNetId().srvId, GetNetId().objId, this);
			}
		}
	}

	if (petGroups.GetCount() > 0)
	{
		source->m_pPetGroup->ChangeOwnerID(changeCharPetIDs, target->GetChar_id());
		for (Index i = C_INDEX(0); i < petGroups.GetCount(); ++i)
			target->m_pPetGroup->Add(petGroups.GetChild<CPet>(i));

		target->m_pPetGroup->CB_AddPets(target->GetPeer(), target->GetContext(), 
			&petGroups, &petSkillGroups);
		if (changeCharPetIDs.size() > 0)
			source->m_pPetGroup->CB_RemovePets(source->GetPeer(), source->GetContext(), changeCharPetIDs);	

		CB_TradingPet(target->GetPeer(), target->GetContext(), changeCharPetIDs);
		
		LogService_Proxy::LogExchangeForItemList(gGlobal.pLogService->pPeer, source->GetContext(), source->GetChar_id(), target->GetChar_id(), 
		LogService_Proxy::Exchange::PROCESS_TRADING, type, changeCharPetIDs, petIDs, stacks);
	}
}


RPCResult CTrading::ConfirmTrading(LPCPEER pPeer, RPCContext &context)
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);

	if (pChar1 && pChar2 && m_bChar1Lock && m_bChar2Lock && (!m_bChar1Confirm || !m_bChar2Confirm))
	{
		if (pChar1->GetContext().sessionId == context.sessionId)
		{
			m_bChar1Confirm = TRUE;
			if (!m_bChar2Confirm)
			{
				pChar2->CB_TradingMessage(pChar2->GetPeer(), pChar2->GetContext(), TRUE, TRADING_CONFIRM);
				CB_ConfirmTrading(pChar2->GetPeer(), pChar2->GetContext(), pChar1->GetChar_id());
			}
		}
		else if (pChar2->GetContext().sessionId == context.sessionId)
		{
			m_bChar2Confirm = TRUE;
			if (!m_bChar1Confirm)
			{
				pChar1->CB_TradingMessage(pChar1->GetPeer(), pChar1->GetContext(), TRUE, TRADING_CONFIRM);
				CB_ConfirmTrading(pChar1->GetPeer(), pChar1->GetContext(), pChar2->GetChar_id());
			}
		}

		if ( (m_bChar1Confirm  && m_bChar2Confirm) )
		{
			String msg;
			UInt32 errorCharID = 0;

			do
			{
				// check script is running or not
				if ( (pChar1->GetScriptSystem() && pChar1->GetScriptSystem()->IsRunning() ) ||
					(pChar2->GetScriptSystem() && pChar2->GetScriptSystem()->IsRunning() ))
				{
					if (pChar1->GetScriptSystem()->IsRunning())
						errorCharID = pChar1->GetChar_id();
					else if (pChar2->GetScriptSystem()->IsRunning())
						errorCharID = pChar2->GetChar_id();
					msg = _T("MSG_SCRIPT_TRADING_CANT");
					break;
				}

				if (!pChar1->IsInMap() || !pChar2->IsInMap())
				{
					if (!pChar1->IsInMap())
						errorCharID = pChar1->GetChar_id();
					else if (!pChar2->IsInMap())
						errorCharID = pChar2->GetChar_id();
					msg = _T("TRADING_FAIL_CHAR_NOT_FIND");
					break;
				}
			
				if (pChar1->IsInBattle() || pChar2->IsInBattle() )
				{
					if (pChar1->IsInBattle())
						errorCharID = pChar1->GetChar_id();
					else if (pChar2->IsInBattle())
						errorCharID = pChar2->GetChar_id();

					msg = _T("MSG_TRADING_BATTLE_FALSE");
					break;
				}

				UInt x = 0;
				if (pChar1->GetCurPosX() > pChar2->GetCurPosX())
					x = pChar1->GetCurPosX() - pChar2->GetCurPosX();
				else
					x = pChar2->GetCurPosX() - pChar1->GetCurPosX();

				x *= x;

				UInt y = 0;
				if (pChar1->GetCurPosY() > pChar2->GetCurPosY())
					y = pChar1->GetCurPosY() - pChar2->GetCurPosY();
				else
					y = pChar2->GetCurPosY() - pChar1->GetCurPosY();

				y *= y;

				if ( (x + y) > 2008)
				{
					msg = _T("MSG_TRADING_DIS_FAIL");
					break;
				}

				TRACE_INFODTL_5(GLOBAL_LOGGER, _F("ConfirmTrading: char_id %d and char_id %d start trading srvId %d and objId %d, trading ptr %d"), 
					pChar1->GetChar_id(), pChar2->GetChar_id(), GetNetId().srvId, GetNetId().objId, this);

				UInt8 tempBagIndex = 0;
				//for m_pChar1 to m_pChar2
				for (Index i = C_INDEX(0); i < m_char1ItemVector.size(); ++i)
				{
					if (m_char1ItemVector[i].itemBagIndex != NO_ITEM_INDEX)
					{
						CItem* orgItem = pChar1->m_pBag->GetItem(m_char1ItemVector[i].itemBagIndex);
						if (orgItem == NULL || orgItem->m_data == NULL )
						{
							msg = _T("MSG_TRADING_TRADE_NOTHING");
							errorCharID = pChar1->GetChar_id();
							break;
						}
						if (orgItem->GetLock()) // check if the item is locked
						{
							msg = _T("MSG_TRADING_TRADE_ITEM_LOCK");
							errorCharID = pChar1->GetChar_id();
							break;
						}
						if ( orgItem->GetStack() < m_char1ItemVector[i].stack)
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_STACK_FAIL");
							break;
						}
						if (orgItem->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND)
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_ITEM_BOUND");
							break;
						}
							
						if (orgItem->m_data->item_type == ITEMTYPE_FLOWER )
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_ITEM_FLOWER");
							break;
						}

						if (orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_ITEM_WEAPON_MODEL");
							break;
						}

						if (!pChar2->m_pBag->FindEmptyIndex(0, tempBagIndex, tempBagIndex))
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_SLOT_FAIL");
							break;
						}
						++tempBagIndex;
					}
				}
				if (!msg.IsEmpty())
						break;

				//for m_pChar2 to m_pChar1
				tempBagIndex = 0;
				for (Index i = C_INDEX(0); i < m_char2ItemVector.size(); ++i)
				{
					if (m_char2ItemVector[i].itemBagIndex != NO_ITEM_INDEX)
					{
						CItem* orgItem = pChar2->m_pBag->GetItem(m_char2ItemVector[i].itemBagIndex);
						if (orgItem == NULL || orgItem->m_data == NULL )
						{
							msg = _T("MSG_TRADING_TRADE_NOTHING");
							errorCharID = pChar2->GetChar_id();
							break;
						}
						if (orgItem->GetLock()) // check if the item is locked
						{
							msg = _T("MSG_TRADING_TRADE_ITEM_LOCK");
							errorCharID = pChar2->GetChar_id();
							break;
						}
						if ( orgItem->GetStack() < m_char2ItemVector[i].stack)
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_STACK_FAIL");
							break;
						}
						if (orgItem->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND)
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_ITEM_BOUND");
							break;
						}

						if (orgItem->m_data->item_type == ITEMTYPE_FLOWER )
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_ITEM_FLOWER");
							break;
						}

						if (orgItem->m_data->item_type == ITEMTYPE_WEAPON_MODEL)
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_ITEM_WEAPON_MODEL");
							break;
						}
													
						if (!pChar1->m_pBag->FindEmptyIndex(0, tempBagIndex, tempBagIndex))
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_SLOT_FAIL");
							break;
						}
						++tempBagIndex;
					}
				}
				if (!msg.IsEmpty())
						break;
				

				//for m_pChar1 to m_pChar2
				UInt8 partnerCount = 0;
				for (Index i = C_INDEX(0); i < m_char1PartnerVector.size(); ++i)
				{
					if (m_char1PartnerVector[i] != 0)
					{
						CPartner* partner = pChar1->m_pPartnerGroup->GetPartner(m_char1PartnerVector[i]);
						if (partner && partner->GetSoulboundState() == SOULBOUND_ALREADY_BOUND)
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_PARTNER_BOUND");
							break;
						}
						if (partner && partner->GetLock()) // check if partner is locked
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_PARTNER_LOCK");
							break;
						}
						++partnerCount;
					}
				}
				if (!msg.IsEmpty())
						break;
				if ((pChar2->GetMaxPartner() - pChar2->m_pPartnerGroup->GetCount()) < partnerCount)
				{
					errorCharID = pChar2->GetChar_id();
					msg = _T("MSG_TRADING_PARTNER_FAIL");
					break;
				}

				//for m_pChar2 to m_pChar1
				partnerCount = 0;
				for (Index i = C_INDEX(0); i < m_char2PartnerVector.size(); ++i)
				{
					if (m_char2PartnerVector[i] != 0)
					{
						CPartner* partner = pChar2->m_pPartnerGroup->GetPartner(m_char2PartnerVector[i]);
						if (partner && partner->GetSoulboundState() == SOULBOUND_ALREADY_BOUND)
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_PARTNER_BOUND");
							break;
						}
						if (partner && partner->GetLock())// check if partner is locked
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_PARTNER_LOCK");
							break;
						}
						++partnerCount;
					}
				}
				if (!msg.IsEmpty())
						break;
				if ((pChar1->GetMaxPartner() - pChar1->m_pPartnerGroup->GetCount()) < partnerCount)
				{
					errorCharID = pChar1->GetChar_id();
					msg = _T("MSG_TRADING_PARTNER_FAIL");
					break;
				}

				//for m_pChar1 to m_pChar2
				UInt8 petCount = 0;
				for (Index i = C_INDEX(0); i < m_char1PetVector.size(); ++i)
				{
					if (m_char1PetVector[i] != 0)
					{
						CPet* pet = pChar1->m_pPetGroup->GetPet(m_char1PetVector[i]);
						if (pet && pet->GetSoulbound() == SOULBOUND_ALREADY_BOUND)
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_PET_BOUND");
							break;
						}
						if (pet && pet->GetLock())// checking if pet is locked
						{
							errorCharID = pChar1->GetChar_id();
							msg = _T("MSG_TRADING_PET_LOCK");
							break;
						}
						++petCount;
					}
				}
				if (!msg.IsEmpty())
						break;
				if ((pChar2->GetMaxPet() - pChar2->m_pPetGroup->GetCount()) < petCount)
				{
					errorCharID = pChar2->GetChar_id();
					msg = _T("MSG_TRADING_PET_FAIL");
					break;
				}

				//for m_pChar2 to m_pChar1
				petCount = 0;
				for (Index i = C_INDEX(0); i < m_char2PetVector.size(); ++i)
				{
					if (m_char2PetVector[i] != 0)
					{
						CPet* pet = pChar2->m_pPetGroup->GetPet(m_char2PetVector[i]);
						if (pet && pet->GetSoulbound() == SOULBOUND_ALREADY_BOUND)
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_PET_BOUND");
							break;
						}
						if (pet && pet->GetLock()) // checking if pet is locked
						{
							errorCharID = pChar2->GetChar_id();
							msg = _T("MSG_TRADING_PET_LOCK");
							break;
						}
						++petCount;
					}
				}
				if (!msg.IsEmpty())
						break;
				if ((pChar1->GetMaxPet() - pChar1->m_pPetGroup->GetCount()) < petCount)
				{
					errorCharID = pChar1->GetChar_id();
					msg = _T("MSG_TRADING_PET_FAIL");
					break;
				}


				//for check m_pChar1
				if (m_char1Gmoney > pChar1->GetMoney())
				{
					errorCharID = pChar1->GetChar_id();
					msg = _T("MSG_TRADING_MONEY_FAIL");
					break;
				}

				//for check m_pChar2
				if (m_char2Gmoney > pChar2->GetMoney())
				{
					errorCharID = pChar2->GetChar_id();
					msg = _T("MSG_TRADING_MONEY_FAIL");
					break;
				}

				//for m_pChar1 to m_pChar2
				TradingItem(pChar1, pChar2, m_char1ItemVector);				
				//for m_pChar2 to m_pChar1
				TradingItem(pChar2, pChar1, m_char2ItemVector);
				
				//for m_pChar1 to m_pChar2
				TradingPartner(pChar1, pChar2, m_char1PartnerVector);
				//for m_pChar2 to m_pChar1
				TradingPartner(pChar2, pChar1, m_char2PartnerVector);

				//for m_pChar1 to m_pChar2
				TradingPet(pChar1, pChar2, m_char1PetVector);
				//for m_pChar2 to m_pChar1				
				TradingPet(pChar2, pChar1, m_char2PetVector);
				
				if (m_char1Gmoney > 0 || m_char2Gmoney > 0)
				{
					if (m_char1Gmoney > 0)
					{
						//for m_pChar1 to m_pChar2
						pChar1->ChangeMoney(-(Int32)m_char1Gmoney);
						pChar2->ChangeMoney(m_char1Gmoney);
						CB_TradingMoney(pChar2->GetPeer(), pChar2->GetContext(), m_char1Gmoney);
						LogService_Proxy::LogExchange(gGlobal.pLogService->pPeer, pChar1->GetContext(), 
							pChar1->GetChar_id(), pChar2->GetChar_id(), LogService_Proxy::Exchange::PROCESS_TRADING, 
							LogService_Proxy::Exchange::EXCHANGE_MONEY, 0, 0, 0, m_char1Gmoney, 0);
					}

					if (m_char2Gmoney > 0)
					{
						//for m_pChar2 to m_pChar1
						pChar2->ChangeMoney(-(Int32)m_char2Gmoney);
						pChar1->ChangeMoney(m_char2Gmoney);
						CB_TradingMoney(pChar1->GetPeer(), pChar1->GetContext(), m_char2Gmoney);
						LogService_Proxy::LogExchange(gGlobal.pLogService->pPeer, pChar2->GetContext(), 
							pChar2->GetChar_id(), pChar1->GetChar_id(), LogService_Proxy::Exchange::PROCESS_TRADING, 
							LogService_Proxy::Exchange::EXCHANGE_MONEY, 0, 0, 0, m_char2Gmoney, 0);
					}

					pChar1->RaiseUpdate();
					pChar2->RaiseUpdate();
				}
			}
			while (false);

			Boolean success = BOOLEAN_FALSE;
			if (msg.IsEmpty())
			{
				success = BOOLEAN_TRUE;
				msg = _T("MSG_TRADING_SUCCESS");
			}

			CB_TradingProcess(pChar1->GetPeer(), pChar1->GetContext(), success, msg, errorCharID);
			CB_TradingProcess(pChar2->GetPeer(), pChar2->GetContext(), success, msg, errorCharID);
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("ConfirmTrading: char_id %d and char_id %d finish trading and stop trading srvId %d and objId %d, trading ptr %d "), 
				pChar1->GetChar_id(), pChar2->GetChar_id(), GetNetId().srvId, GetNetId().objId, this);
			StopTrading();
		}
	}

	return RPC_RESULT_OK;
}

void CTrading::StopTrading(UInt32 owner_id)
{
	if (owner_id > 0)
	{
		CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
		CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);
		if (pChar1 && pChar1->GetPeer())
			CB_CancelTrading(pChar1->GetPeer(), pChar1->GetContext(), owner_id);

		if (pChar2 && pChar2->GetPeer())
			CB_CancelTrading(pChar2->GetPeer(), pChar2->GetContext(), owner_id);

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("StopTrading: netid %d:%d, char_id %d stop trading, trading ptr %d "), GetNetId().srvId, GetNetId().objId, owner_id, this);
	}
		
	DeleteTrading();
}

void CTrading::DeleteTrading()
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);

	if (pChar1)
	{
		pChar1->SetTradingObject(NULL);
		pChar1->ResetTradingTarget();
		UnsetOwnObject(pChar1->GetContext().sessionId, this);
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("DeleteTrading: netid %d:%d, delete trading and UnsetOwnObject char1 char_id %d, trading ptr %d"), GetNetId().srvId, GetNetId().objId, pChar1->GetChar_id(), this);
	}

	if (pChar2)
	{	
		pChar2->SetTradingObject(NULL);
		pChar2->ResetTradingTarget();
		UnsetOwnObject(pChar2->GetContext().sessionId, this);
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("DeleteTrading: netid %d:%d, delete trading and UnsetOwnObject char2 char_id %d trading ptr %d"), GetNetId().srvId, GetNetId().objId, pChar2->GetChar_id(), this);
	}

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("DeleteTrading: objId %d, netid %d:%d trading ptr %d"), this, GetNetId().srvId, GetNetId().objId, this);

	m_CharNetID1 = RPCNetID();
	m_CharNetID2 = RPCNetID();
	
	m_char1Gmoney = 0;
	m_char2Gmoney = 0;

	m_char1ItemVector.clear();
	m_char2ItemVector.clear();

	m_char1PartnerVector.clear();
	m_char2PartnerVector.clear();

	m_char1PetVector.clear();
	m_char2PetVector.clear();

	m_bChar1Lock = FALSE;
	m_bChar2Lock = FALSE;
	
	m_bChar1Confirm = FALSE;
	m_bChar2Confirm = FALSE;

	CTrading* trading = this;
	DeleteNetObject(trading);
}



CTrading::CTrading()
{
	m_char1Gmoney = 0;
	m_char2Gmoney = 0;

	m_CharNetID1 = RPCNetID();
	m_CharNetID2 = RPCNetID();

	
	for (Index i = 0; i < 6; ++i)
	{
		ItemSlot temp1;
		m_char1ItemVector.push_back(temp1);

		ItemSlot temp2;
		m_char2ItemVector.push_back(temp2);

		m_char1PartnerVector.push_back(0);
		m_char2PartnerVector.push_back(0);

		m_char1PetVector.push_back(0);
		m_char2PetVector.push_back(0);
	}

	m_bChar1Lock = FALSE;
	m_bChar2Lock = FALSE;
	
	m_bChar1Confirm = FALSE;
	m_bChar2Confirm = FALSE;
}

void CTrading::ResetLock()
{
	CCharacter* pChar1 = FindNetObject<CCharacter>(m_CharNetID1);
	CCharacter* pChar2 = FindNetObject<CCharacter>(m_CharNetID2);


	if ( pChar1 && pChar2 && (m_bChar1Lock || m_bChar2Lock) )
	{
		if(m_bChar1Lock)
		{
			m_bChar1Lock = !m_bChar1Lock;
			CB_SetLock(pChar1->GetPeer(), pChar1->GetContext(), pChar1->GetChar_id(), m_bChar1Lock);
			CB_SetLock(pChar2->GetPeer(), pChar2->GetContext(), pChar1->GetChar_id(), m_bChar1Lock);
		}
		if(m_bChar2Lock)
		{
			m_bChar2Lock = !m_bChar2Lock;
			CB_SetLock(pChar1->GetPeer(), pChar1->GetContext(), pChar2->GetChar_id(), m_bChar2Lock);
			CB_SetLock(pChar2->GetPeer(), pChar2->GetContext(), pChar2->GetChar_id(), m_bChar2Lock);
		} 
	}
}

CTrading::~CTrading()
{
	m_CharNetID1 = RPCNetID();
	m_CharNetID2 = RPCNetID();
	
	m_char1Gmoney = 0;
	m_char2Gmoney = 0;

	m_char1ItemVector.clear();
	m_char2ItemVector.clear();

	m_char1PartnerVector.clear();
	m_char2PartnerVector.clear();

	m_char1PetVector.clear();
	m_char2PetVector.clear();
}


