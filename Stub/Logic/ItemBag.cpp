//-- Common
#include "Common.h"
#include "Common/DataCode.h"
#include "Common/Procedure.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/Item.h"
//-- Library
#include "Proxy/Data/ItemData.h"
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Service/LogService.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/Question.h"
#include "Stub/Logic/QuestSystem.h"
//#include "Stub/Logic/QuestSystem.h"
#include "MapService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResItem.h"
#include "Resource/ResShop.h"
#include "Resource/ResBuff.h"
#include "Resource/ResMixShop.h"
#include "Resource/ResRandTable.h"
#include "Resource/QGLCSVTReader.h"
#include "Resource/ResItemSet.h"
#include "Map.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Resource/GameStringTable.h"


REGISTER_CLASSTYPE(CItemBag, ItemBag_CLASSID);

CItemBag::CItemBag()
{
	m_pOwner = NULL;
	RPCNetID empty = RPCNetID();
	for (Index i = C_INDEX(0); i < BAG_MAX_SIZE; ++i)
		m_items.push_back(empty);

	m_dummyItem = CItem::NewInstance(); //for reserve
	m_bWareHouseUpdated = BOOLEAN_FALSE;
	m_iStartMakeTime = 0;
	m_EquipItemSetMap.clear();
	m_wareHouseItems.clear();
	m_FoodItemMap.clear();
	m_JpackItemVec.clear();
	m_WarehouseItemVec.clear();
}

CItemBag::~CItemBag()
{
	RPCNetID empty;
	CItem* item = NULL;
	
	for(Index i = C_INDEX(0); i < m_items.size(); ++i)
	{
		if (m_items[i] != empty && m_items[i] != m_dummyItem->GetNetId())
		{
			 item = FindNetObject<CItem>(m_items[i]);
			 if (item)
				 DeleteNetObject(item);
		}
	}
	DeleteNetObject(m_dummyItem);
	m_items.clear();

	for (UInt16 i = 0; i < m_delGroup.GetCount(); ++i)
	{
		CItem* item = m_delGroup.GetChild<CItem>(i);
		if (item)
			DeleteNetObject(item);
	}	
	ClearWareHouse();
	m_EquipItemSetMap.clear();
	m_FoodItemMap.clear();
	m_JpackItemVec.clear();
	m_WarehouseItemVec.clear();
}

Boolean CItemBag::IsBagEnabled(UInt8 bagType)
{
	if (m_pOwner && bagType < (BAG_BACKPACK + m_pOwner->GetAvailBackPack() ))
		return BOOLEAN_TRUE;
	return BOOLEAN_FALSE;
}

void CItemBag::SwapItemByIndex(UInt srcIndex, UInt desIndex)
{
	if (srcIndex >= m_items.size() || desIndex >= m_items.size())
		return;

	CItem* srcItem = GetItem(srcIndex);
	CItem* desItem = GetItem(desIndex);

	if (srcItem)
	{
		m_items[srcIndex] = RPCNetID();
		srcItem->SetBagIndex(desIndex);
		m_items[desIndex] = srcItem->GetNetId();

		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("srcItem owner_id %d, owner session_id %s, item_uid %d, item_id %d, from %d to %d"), 
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), srcItem->GetItem_uid(), srcItem->GetItem_id(), srcIndex, desIndex);
	
		if (desItem)
		{
			desItem->SetBagIndex(srcIndex);
			m_items[srcIndex] = desItem->GetNetId();
			TRACE_INFODTL_6(GLOBAL_LOGGER, _F("desItem owner_id %d, owner session_id %s, item_uid %d, item_id %d, from %d to %d"), 
				m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), desItem->GetItem_uid(), desItem->GetItem_id(), desIndex, srcIndex);
		}
	}
}

RPCResult CItemBag::MoveItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, 
							 const UInt8 &dstIdx)
{
	String errMsg;

	do {
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		if (srcIdx == dstIdx ||										// same bag index
			srcIdx >= BAG_MAX_SIZE || dstIdx >= BAG_MAX_SIZE)		// index out range			
		{
			errMsg = _T("MSG_SYS_BAG_MOVE_INVALID");
			break;
		}

		CItem* srcItem = GetItem(srcIdx);
		if (srcItem == NULL || !srcItem->IsExist())
		{
			errMsg = _T("MSG_SYS_BAG_MOVE_INVALID");
			break;
		}

		if (srcItem->m_data == NULL || srcItem->GetInWareHouse())
		{
			errMsg.Format(_T("Move invalid item, item_id=%d"), srcItem->GetItem_id());
			break;
		}

		UInt8 srcBag = srcIdx / BAG_ITEM_COUNT;
		UInt8 dstBag = dstIdx / BAG_ITEM_COUNT;

		// backpack not enabled
		if (!IsBagEnabled(dstBag))
		{
			errMsg = _T("MSG_SYS_BAG_DISABLE");		 // this backpack not enabled
			break;
		}

		// cannot move item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			errMsg = _T("MSG_SYS_BAG_SCRIPT_RUNNING");
			break;
		}

		// cannot move script item into other bags
		if (dstBag != BAG_CURRENT && srcItem && srcItem->m_data && 
			srcItem->IsExist() && srcItem->m_data->item_type == ITEMTYPE_SCRIPT)
		{
			errMsg = _T("MSG_SYS_BAG_MOVE_SCRIPT");	// cannot move script item into other bags
			break;
		}

		// cannot move business item or money into other bags
		if (dstBag != BAG_CURRENT && srcItem && srcItem->m_data && 
			srcItem->IsExist() && (srcItem->m_data->item_type == ITEMTYPE_BUSINESS_ITEM
			|| srcItem->m_data->item_type == ITEMTYPE_BUSINESS_MONEY))
		{
			errMsg = _T("MSG_SYS_BAG_MOVE_BUSINESS");	// cannot move business item or money into other bags
			break;
		}


		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("owner_id %d, owner session_id %s, item_uid %d, item_id %d, from %d to %d"), 
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), srcItem->GetItem_uid(), srcItem->GetItem_id(), srcIdx, dstIdx);

		// try stack up if possible
		srcItem->ClearUpdateMask();				// clear source item update mask before moving
		if (srcBag != dstBag)
		{
			bool swapItem = true;
			if (srcItem->m_data->overlap > 1) //if the item can stackable, then stack it first
			{
				StackItem(srcItem, dstBag);
				if (srcItem->IsUpdated(CItem::stackMask)) //if the stack mask is updated, there is item can stack then it need not to swap.
					swapItem = false;
			}
			
			if (swapItem)
			{
				if (GetFreeSlot(dstBag) == 0)
				{
					errMsg = _T("bag full");
					break;
				}
				UInt8 newIdx = 0;
				if (FindEmptyIndex(dstBag, newIdx))
					SwapItemByIndex(srcItem->GetBagIndex(), newIdx);
			}
		}
		else
		{
			bool swapItem = true;
			//if there are desItem and can stack then it will ony stack and will not move
			if (srcItem->m_data->overlap > 1)
			{
				CItem* dstItem = GetItem(dstIdx);
				if (dstItem && dstItem->IsExist() && dstItem->GetItem_id() == srcItem->GetItem_id())
				{
					UInt8 avail = dstItem->m_data->overlap - dstItem->GetStack();
					if (avail > 0)
					{
						// item stack will change
						if (srcItem->GetStack() <= avail)
						{
							// fully merged
							dstItem->SetStack(dstItem->GetStack() + srcItem->GetStack());
							srcItem->SetStack(0);							// source item change to non-exist
						}
						else
						{
							// partial merged
							dstItem->SetStack(dstItem->m_data->overlap);
							srcItem->SetStack(srcItem->GetStack() - avail);
						}
						dstItem->SetLock((srcItem->GetLock() || dstItem->GetLock()));	
						swapItem = false;
					}					
				}
			}					

			if (swapItem)
				SwapItemByIndex(srcItem->GetBagIndex(), dstIdx);
		}

		if (srcItem->GetStack() == 0)
			AddToDeleteGroup(srcItem->GetBagIndex());

		RaiseUpdate();					// raise update object event
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("%s"), errMsg.c_str());
		CB_Error(pPeer, context, 1, errMsg);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

RPCResult CItemBag::DropItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, 
							 const UInt8 &count)
{
	String errMsg;

	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		if (m_pOwner->IsHawking())
		{
			errMsg = _T("MSG_HAWK_CANT_USE_ITEM");
			break;
		}

		CItem *item = GetItem(bagIndex);
		if (bagIndex >= BAG_MAX_ITEM ||									// index out range
			item == NULL ||	!item->IsExist())		// source item not exist
		{
			errMsg = _T("MSG_SYS_ITEM_DROP_ERR");			// invalid item to drop
			break;
		}

		// cannot drop item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			errMsg = _T("MSG_SYS_BAG_SCRIPT_RUNNING");
			break;
		}

		if (item->m_data && item->m_data->dumpable == 0) // || item->GetItem_soulboundState())
		{
			errMsg = _T("MSG_DROP_ITEM_BOUND");		// invalid item to drop
			break;
		}

		if (item->GetLock())
		{
			errMsg = _T("MSG_DROP_ITEM_LOCK");		// item is locked, which can't be dropped.
			break;
		}

		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("owner_id %d, owner session_id %s, item_uid %d, item_id %d, drop count %d"), 
			m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), item->GetItem_uid(), item->GetItem_id(), count);

		if (count == 0 || count >= item->GetStack())
		{
			// drop item with all count
			LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, m_pOwner->GetContext(),
				m_pOwner->GetChar_id(), LogService_Proxy::Item::TARGET_UNKNOW, 0, 
				item->GetItem_uid(), item->GetItem_id(), item->GetStack(), 
				LogService_Proxy::Item::ACT_DROP);
			item->SetStack(0);
			AddToDeleteGroup(item->GetBagIndex());
		}
		else
		{
			// drop item with specify count
			LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, m_pOwner->GetContext(),
				m_pOwner->GetChar_id(), LogService_Proxy::Item::TARGET_UNKNOW, 0, 
				item->GetItem_uid(), item->GetItem_id(), count, 
				LogService_Proxy::Item::ACT_DROP);
			item->SetStack(item->GetStack() - count);
		}

		RaiseUpdate();					// raise update object event
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("DropItem Error: %s"), errMsg.c_str());
		CB_Error(pPeer, context, 1, errMsg);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

RPCResult CItemBag::UseItem(LPCPEER pPeer, RPCContext &context, const UInt8 &bagIndex, const UInt32 &itemId, 
									  const Boolean &bScript, const UInt32 &partnerID, const UInt32 &pet_uid)
{
	String errMsg;

	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		if (m_pOwner->IsHawking())
		{
			errMsg = _T("MSG_HAWK_CANT_USE_ITEM");
			break;
		}

		CItem* item = GetItem(bagIndex);

		if (item == NULL || !item->IsExist() || item->m_data == NULL)
		{
			errMsg = _T("MSG_SYS_ITEM_NOT_EXIST"); // item doest not exist
			break;
		}

		// cannot use item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			errMsg = _T("MSG_SYS_BAG_SCRIPT_RUNNING");
			break;
		}

		if ( item->GetItem_id() != itemId || item->GetItem_uid() == 0)
		{
			errMsg = _T("MSG_SYS_ITEM_ID_ERR");
			break;
		}

		if (item->GetLock()) // check if it is locked
		{
			errMsg = _T("MSG_ITEM_LOCK_CANT_USE");
			break;
		}

		if (item->m_data->Identify && item->GetItem_IdentifyState() == 0)
		{
			CheckIdentifyItemExist(item);
			errMsg = _T("MSG_ITEM_HAVENT_IDENTIFY");			
			break;
		}

		if ( ((item->m_data->script && (_tcsicmp(item->m_data->script, _T("")) != 0)) ^ bScript) != false )
		{
			errMsg = _T("MSG_SYS_ITEM_CANT_USE");
			break;
		}

		if (item->m_data->consume == 0 )
		{
			errMsg = _T("MSG_SYS_ITEM_CANT_USE");
			break;
		}

		

		if( (item->m_data->consume==1 || item->m_data->consume==2) && bagIndex > 23 )
		{
			errMsg = _T("MSG_SYS_ITEM_BAG_CANT_USE");
			break;
		}

		if (!(item->m_data->memberUse) && bScript && 
			m_pOwner->GetParty() && m_pOwner != m_pOwner->GetParty()->GetLeader())
		{
			errMsg = _T("MSG_SYS_ITEM_MEMBERCANNOTUSE");
			break;	
		}

		if (bScript && m_pOwner->GetScriptSystem() && 
			m_pOwner->GetScriptSystem()->IsRunning())
		{
			errMsg = _T("MSG_SYS_ITEM_SCRIPT_RUNNING");
			break;	
		}

		if(m_pOwner->m_pQuestion->GetState() == CQuestion::ST_ENTER || m_pOwner->m_pQuestion->GetState() == CQuestion::ST_QUESTION )
		{
			errMsg = _T("MSG_SYS_ITEM_CANT_USE");
			break;
		}


		// Use the item to different target

		if (partnerID == 0 && pet_uid == 0) // is not partner or pet
		{
			if (!item->UseOn(m_pOwner, m_pOwner, errMsg))
			{
				errMsg = _T("MSG_SYS_ITEM_CANT_USE"); // item cannot use on character
				break;
			}
			LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, m_pOwner->GetContext(),
				m_pOwner->GetChar_id(), LogService_Proxy::Item::TARGET_PLAYER, 
				m_pOwner->GetChar_id(), item->GetItem_uid(), item->GetItem_id(), 
				item->GetStack(), LogService_Proxy::Item::ACT_USE);
		}
		else if (partnerID > 0) // is partner
		{
			CPartner* partner = m_pOwner->m_pPartnerGroup->GetPartner(partnerID);
			if (partner == NULL || !item->UseOn(m_pOwner, partner, errMsg))
			{
				errMsg = _T("MSG_SYS_ITEM_CANT_USE"); // item cannot use on character
				break;
			}
			LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, m_pOwner->GetContext(),
				m_pOwner->GetChar_id(), LogService_Proxy::Item::TARGET_PARTNER, 
				partner->GetPartner_id(), item->GetItem_uid(), item->GetItem_id(), 
				item->GetStack(), LogService_Proxy::Item::ACT_USE);
		}
		else if (pet_uid > 0) // is pet
		{
			CPet* pet = m_pOwner->m_pPetGroup->GetPet(pet_uid);
			if (pet == NULL || !item->UseOn(m_pOwner, pet, errMsg))
			{
				errMsg = _T("MSG_SYS_ITEM_CANT_USE"); // item cannot use on character
				break;
			}
			LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, m_pOwner->GetContext(),
				m_pOwner->GetChar_id(), LogService_Proxy::Item::TARGET_PET, 
				pet->GetPet_uid(), item->GetItem_uid(), item->GetItem_id(), 
				item->GetStack(), LogService_Proxy::Item::ACT_USE);
		}

		// souldbound check
		if (item->GetItem_soulboundState() == SOULBOUND_USE_BOUND)
			item->SetSoulboundState(SOULBOUND_ALREADY_BOUND);

		//this checking can delete if the new method of deleting the item exist.
		if (item->GetStack() == 0)
			AddToDeleteGroup(item->GetBagIndex());

		RaiseUpdate();					// raise update object event
	}
	while (false);

	CB_ItemUseResult(pPeer, context, !errMsg.IsEmpty());
	if (!errMsg.IsEmpty())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UseItem Failed: %s"), errMsg.c_str());
		CB_Error(pPeer, context, 1, errMsg);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

// do equip and unequip item according to source bag index
RPCResult CItemBag::EquipItem(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIdx, 
							  const UInt8 &dstIdx)
{
	String errMsg;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		// if dstIdx == 255, then system will auto assign a position to it
		if (srcIdx == dstIdx ||														// same place
			srcIdx >= BAG_MAX_SIZE || (dstIdx != 255 && dstIdx >= BAG_MAX_SIZE) ||	// index out range
			(srcIdx >= BAG_MAX_ITEM && dstIdx != 255 && dstIdx >= BAG_MAX_ITEM) )	// internal exchange
		{
			errMsg = _T("MSG_SYS_ITEM_NOT_EXIST");		// invalid equip item
			break;
		}

		CItem * item = GetItem(srcIdx);
		if (item == NULL || item->m_data == NULL || !item->IsExist() || 
			item->m_data->item_type != ITEMTYPE_EQUIP ||				// not an equipment
			item->GetItem_uid() == 0)									// uninitialized UID
		{
			errMsg = _T("MSG_SYS_ITEM_NOT_EXIST");		// invalid equip item
			break;
		}

		// cannot equip item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			errMsg = _T("MSG_SYS_BAG_SCRIPT_RUNNING");
			break;
		}
		if (item->m_data->Identify && item->GetItem_IdentifyState() == 0)
		{
			CheckIdentifyItemExist(item);
			errMsg = _T("MSG_ITEM_HAVENT_IDENTIFY");			
			break;
		}

		UInt8 bagIndex;					// source item index
		UInt8 eqIndex;					// equipment index

		// swap srcIdx and dstIdx if dstIdx is an item
		if (srcIdx >= BAG_MAX_ITEM && dstIdx < BAG_MAX_ITEM && GetItem(dstIdx))
		{
			//if it is unequip but there is item in the dstIdx then make the dstIdx equip
			bagIndex = dstIdx;
			eqIndex = srcIdx;
			item = GetItem(bagIndex);
		}
		else
		{
			bagIndex = srcIdx;
			eqIndex = dstIdx;
		}
		
		// if do unequip and backpack not enabled and check it is auto find index or not
		if (bagIndex >= BAG_MAX_ITEM && eqIndex != 255 && !IsBagEnabled(eqIndex / BAG_ITEM_COUNT))
		{
			errMsg = _T("MSG_SYS_BAG_DISABLE");		// this backpack not enabled for unequiped item
			break;
		}

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("EquipItem: sessionId %s, char_id %d, srcIdx %d, dstIdx %d"), 
			context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), srcIdx, dstIdx);

		if (bagIndex < BAG_MAX_ITEM)
		{
			// do equip
			// allow to equip?
			if (!item->AllowSexClass(m_pOwner->GetSex(), m_pOwner->GetCclass()) || (item->m_data->reqLv > m_pOwner->GetLevel()) ) {
				errMsg = _T("MSE_SYS_EQUIP_CANT_USE"); // sex or class not match to equip
				break;
			}
			if (eqIndex == 255)
			{
				// find appropriate equipment slot
				eqIndex = item->GetEquipIndex();
				if (eqIndex == 255)
				{
					errMsg = _T("MSE_SYS_EQUIP_ERR"); // wrong equip index
					break;
				}				
			}
			else
			{
				// validate equipment placement
				if (eqIndex != item->GetEquipIndex())
				{
					errMsg = _T("MSE_SYS_EQUIP_ERR"); // invalid equip item type
					break;
				}
			}

			// change character outlook
			switch (eqIndex - BAG_MAX_ITEM)
			{
			case EQPLACE_WEAPON:
				m_pOwner->SetEqWeapon(item->GetItem_id(), item->GetEqColor());
				break;
			//case EQPLACE_GEAR:
			case EQPLACE_HEAD_ACCESSORY:
				m_pOwner->SetEqHead(item->GetItem_id(), item->GetEqColor());
				break;
			case EQPLACE_CLOTH:
				m_pOwner->SetEqCloth(item->GetItem_id(), item->GetEqColor());
				break;
			}

			if (item->GetItem_soulboundState() == SOULBOUND_EQ_BOUND)
				item->SetSoulboundState(SOULBOUND_ALREADY_BOUND);

			if (item->GetItem_expireDate() == 0 && item->m_data->item_TimeLimitType == ITEM_TIME_LIMIT_TYPE_EQUIP)
			{
				UInt32 now = (UInt32) time(NULL);
				UInt32 expire;
				expire = now + 3600 * 24 * item->m_data->item_TimeLimit;
				item->SetExpireDate(expire);
				item->RaiseUniqEventID(CItem::EVT_UPDATE_ITEM_EXPIRE, (now - expire) * 1000);
			}
		}
		else
		{						// do unequip
			if (eqIndex == 255)
			{
				// find empty slot in bag
				if (!FindEmptyIndex(BAG_CURRENT, eqIndex))
				{
					errMsg = _T("MSG_SYS_BAG_FULL"); // no place to unequip in current bag
					break;
				}
			}
			else if (!IsSlotFree(eqIndex))
			{
				errMsg = _T("MSE_SYS_UNEQUIP_ERR"); // cannot unequip to that place
				break;
			}
			// change character outlook
			switch (bagIndex - BAG_MAX_ITEM)
			{
			case EQPLACE_WEAPON:
				m_pOwner->SetEqWeapon(0, 0);
				break;
			//case EQPLACE_GEAR:
			case EQPLACE_HEAD_ACCESSORY:
				m_pOwner->SetEqHead(0, 0);
				break;
			case EQPLACE_CLOTH:
				m_pOwner->SetEqCloth(0, 0);
				break;
			}
		}
				
		//remove and add eq buff
		CItem* oldEq = NULL;
		CItem* newEq = NULL;
		if (bagIndex >= BAG_MAX_ITEM)
		{
			oldEq = GetItem(bagIndex);
			newEq = GetItem(eqIndex);
		}
		else
		{
			oldEq = GetItem(eqIndex);
			newEq = GetItem(bagIndex);
		}

		if (oldEq && oldEq->GetBuffID())
		{
			m_pOwner->RemoveBuff(oldEq->GetBuffID());
		}
		if (newEq && newEq->GetBuffID())
		{
			m_pOwner->AddBuff(newEq->GetBuffID(), newEq->GetBuffValue1(), newEq->GetBuffValue2(), newEq->GetBuffValue3(), 
				newEq->GetBuffCoolDown(), newEq->GetBuffPro());
		}
		// replace equipment items with new equipment

		SwapItemByIndex(bagIndex, eqIndex);

		//add and remove equip skill
		if (oldEq && oldEq->GetSkillID() > 0)
		{
			m_pOwner->RemoveSkillTemp(oldEq->GetSkillID());
		}
		if (newEq && newEq->GetSkillID() > 0 && newEq->GetEndure() > 0)
		{
			m_pOwner->AddSkillTemp(newEq->GetSkillID());
		}
		
		UpdateItemSet();		// check ItemSet

		m_pOwner->CalcAttrib();			// re-calculate after equipment changed

		if (m_pOwner->GetUpdateMask().IsBit(CCharacter::eqWeaponMask) ||
			m_pOwner->GetUpdateMask().IsBit(CCharacter::eqClothMask) ||
			m_pOwner->GetUpdateMask().IsBit(CCharacter::eqHeadMask))
		{
			m_pOwner->RaiseUpdate();			// update character to Object service
			m_pOwner->RaiseUniqEventID(CCharacter::EVT_MSGDATA_UPDATE);
		}

		RaiseUpdate();					// raise update object event
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("EquipItem Error: %s"), errMsg.c_str());
		CB_Error(pPeer, context, 1, errMsg);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

RPCResult CItemBag::BuyItemFromShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, 
									const Int32 &shopId, const UInt8 &slot, const UInt16 &count)
{
	String errMsg;
	UInt8 msgIndex = 0;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		CONST ShopLstData* pShop = gGlobal.m_resource->GetShopInfo(shopId);
		if ( !pShop )
		{
			errMsg = _T("Can not find the shop id");
			break;
		}

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("BuyItemFromShop: sessionId %s, char_id %d, shopId %d, itemId %d"), 
			context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), shopId, itemId);

		UInt16 discount = 0;
		{
			if ( slot >= (UInt8)pShop->ItemLst.size() )
			{
				errMsg = _T("Item id is not in the shop list");
				break;
			}
			
			if ( pShop->ItemLst[slot] != itemId )
			{
				errMsg = _T("Item id invalid");
				break;
			}
			discount = pShop->BuyDiscount; 
		}
			
		{
			const ItemData* pItmData = gGlobal.m_resource->GetItemData(itemId);
			if ( !pItmData)
			{
				errMsg = _T("Item id is not exist");
				return RPC_RESULT_FAIL;
			}

			UInt32 eachPrice = (UInt32)(pItmData->g_price * (discount / 100.0)); // pervent the overflow of the product of multiplication
			
			UInt32 finalPrice = eachPrice * count;

			if ( m_pOwner->GetMoney() < finalPrice )
			{
				errMsg = _T("Not enough money to buy item");
				break;
			}

			// - Check Free Slot
			UInt8 totalUse = count / (pItmData->overlap);
			if ( ( count % (pItmData->overlap)) > 0 )
				totalUse += 1;
			if ( totalUse > GetTotalFreeSlot() )
			{
				errMsg = _T("Not enough free space");
				break;
			}

			// cannot buy item when the script is running
			if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
			{
				errMsg = _T("MSG_SYS_BAG_SCRIPT_RUNNING");
				break;
			}

			if (!CanCreateLimitItem(itemId, count))
			{
				errMsg = _T("reach the item limit");
				msgIndex = ITEM_MSG_REACH_LIMIT;
				break;
			}

			UInt16 remaincount = count;
			finalPrice = 0;
			do
			{
				UInt8 add = 0;
				if ( remaincount > (pItmData->overlap) )
					add = pItmData->overlap;
				else
					add = (UInt8) remaincount;

				CItem* pNewItem = m_pOwner->m_pBag->NewItem(itemId, add);
				if ( pNewItem )
				{
					Boolean bFinishBuy(BOOLEAN_FALSE);
					for ( UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
					{
						if (m_pOwner->m_pBag->AddItem(pNewItem, i))
						{
							finalPrice += (eachPrice * add);
							//finalPrice += (eachPrice * (pNewItem->GetStack()));
							bFinishBuy = BOOLEAN_TRUE;
							break;
						}
					}

					if ( !bFinishBuy )
					{
						DeleteNetObject((NetObject*&) pNewItem);
						//errMsg = _T("BuyItemFromShop Error: Add item failed");
						//break;
					}
				}
				
				remaincount -= add;
			}
			while(remaincount > 0);

			m_pOwner->ChangeMoney(-(Int32)finalPrice, LogService_Proxy::Money_And_Exp::SOURCE_SHOP_BUY);

			if (m_pOwner->IsUpdated())
			{
				CB_BuytItemSuccess(pPeer, context, itemId, finalPrice);
				m_pOwner->RaiseUpdate();
			}
		}
		RaiseUpdate();					// raise update object event
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("BuyItemFromShop Error: %s"), errMsg.c_str());
		CB_Error(pPeer, context, msgIndex, errMsg);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

RPCResult CItemBag::SellItemToShop(LPCPEER pPeer, RPCContext &context, const Int32 &shopId, const UInt8 &bagIndex, const UInt8 &count)
{
	String errMsg;

	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		// - Reduce Item Count
		CItem *item = GetItem(bagIndex);
		if (bagIndex >= BAG_MAX_ITEM ||									// index out range
			item == NULL || !item->IsExist() || item->m_data == NULL)		// source item not exist
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");				//invalid item to sell
			break;
		}

		// cannot sell item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			errMsg = _T("MSG_SYS_BAG_SCRIPT_RUNNING");
			break;
		}

		if (item->m_data->sellable == 0)		// source item not sellable
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");				//non-sellable item to sell
			break;
		}

		if (item->GetLock())		
		{
			errMsg = _T("MSG_ITEM_LOCK_SELL_ERR");			
			break;
		}

		if ( count == 0 || count > item->GetStack() )
		{
			errMsg = _T("Sell count is larger than the stack");
			break;
		}

		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("SellItemFromShop: sessionId %s, char_id %d, shopId %d, bagIndex %d"), 
			context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), shopId, bagIndex);

		CONST ShopLstData* pShop = gGlobal.m_resource->GetShopInfo(shopId);
		if ( !pShop )
		{
			errMsg = _T("Can not find the shop id");
			break;
		}

		if ( pShop->CanSell == 0 )
		{
			errMsg = _T("Can not sell item to shop");
			break;
		}

		item->SetStack(item->GetStack() - count);
		if (item->GetStack() == 0)
			AddToDeleteGroup(item->GetBagIndex());

		// prevent the overflow of the product of multiplication
		UInt32 finalprice = (UInt32)(item->m_data->g_price * (pShop->SellDiscount / 100.0));
		finalprice *= count;
		m_pOwner->ChangeMoney(finalprice, LogService_Proxy::Money_And_Exp::SOURCE_SHOP_SELL);

		if (m_pOwner->IsUpdated())
			m_pOwner->RaiseUpdate();
		CB_SelltItemSuccess(pPeer, context, item->GetItem_id(), finalprice);
		RaiseUpdate();					// raise update object event
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("SellItemToShop Error: %s"), errMsg.c_str());
		CB_Error(pPeer, context, 1, errMsg);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}


Boolean CItemBag::HasItem(UInt32 item_id)
{
	for (UInt8 i = BAG_CURRENT; i < BAG_MAX_SIZE; ++i)
	{
		CItem* pItem = GetItem(i);
		if (pItem && pItem->GetItem_id() == item_id && pItem->IsExist())
			return BOOLEAN_TRUE;
	}

	return BOOLEAN_FALSE;
}

Boolean CItemBag::IsSlotFree(UInt8 bagIndex)
{
	if (bagIndex < BAG_MAX_SIZE)
		return (m_items[bagIndex] == RPCNetID());
	else
		return BOOLEAN_FALSE;
}

void CItemBag::PreBattle()
{
	CItem*  item = NULL;
	// set try stack and remain before battle
	for (UInt8 i = 0; i < BAG_ITEM_COUNT; i++)
	{
		item = GetItem(i);
		if (item) {
			item->m_tryStack = item->GetStack();
			item->m_tryRemain = item->GetRemain();
		}
	}
}

CItem* CItemBag::Get(int index)
{
	if (index >= BAG_MAX_SIZE)						// index out range
		return NULL;

	int count = 0;
	CItem* item = NULL;
	for (int i = 0; i < BAG_MAX_SIZE; i++)
	{
		item = GetItem(i);
		if (item && item->IsExist())
		{
			if (count == index)
				return item;
			count++;
		}
	}

	return NULL;
}

CItem* CItemBag::GetItem(UInt8 bagIndex)
{
	if (bagIndex >= BAG_MAX_SIZE)						// index out range
		return NULL;

	if (m_items[bagIndex] == RPCNetID() || m_items[bagIndex] == m_dummyItem->GetNetId())
		return NULL;

	return FindNetObject<CItem>(m_items[bagIndex]);
}

CItem* CItemBag::GetItemByUID(UInt32 item_uid)
{
	CItem* item = NULL;
	for (int i = 0; i < BAG_MAX_ITEM; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() &&
			item->GetItem_uid() == item_uid)
			return item;
	}
	return NULL;
}

CItem* CItemBag::GetEquipByUID(UInt32 item_uid)
{
	CItem* item = NULL;
	for (int i = BAG_MAX_ITEM; i < BAG_MAX_SIZE; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() &&
			item->GetItem_uid() == item_uid)
			return item;
	}
	return NULL;
}

CItem* CItemBag::GetItemAndEquipByUID(UInt32 item_uid)
{
	CItem* item = NULL;
	for (int i = 0; i < BAG_MAX_SIZE; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() &&
			item->GetItem_uid() == item_uid)
			return item;
	}
	return NULL;
}

CItem* CItemBag::FindItemById(UInt32 item_id, UInt8 bagType, UInt8 startIdx)
{
	if (bagType >= BAG_TYPE_COUNT)
		return NULL;

	CItem* item = NULL;
	for (int i = (startIdx == 0 ? bagType * BAG_ITEM_COUNT : startIdx);	// start at give index or not
		i < (bagType + 1) * BAG_ITEM_COUNT; i++)
	{
		item = GetItem(i);
		if (item)
		{
			if (item->GetItem_id() == item_id && item->GetItem_uid() != 0)
				return item;
			else if (item->GetItem_uid() == 0)
				TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("(bag item uid is zero) pItem %d, item_id %d, bag index %d"), item, item->GetItem_id(), i);
		}
	}

	return NULL;
}

Boolean CItemBag::FindEmptyIndex(UInt8 bagType, UInt8 &bagIndex, UInt8 startIdx)
{
	if (bagType >= BAG_TYPE_COUNT)
		return BOOLEAN_FALSE;

	CItem* item = NULL;
	for (int i = (startIdx == 0 ? bagType * BAG_ITEM_COUNT : startIdx);	// start at give index or not
		i < (bagType + 1) * BAG_ITEM_COUNT; i++)
	{
		item = GetItem(i);
		if (IsSlotFree(i))
		{
			bagIndex = i;
			return BOOLEAN_TRUE;
		}
		else if (item && !item->IsExist())
		{
			AddToDeleteGroup(item->GetBagIndex());
			bagIndex = i;
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

Boolean CItemBag::StackItem(CItem* item, UInt8 bagType, UInt8 quality)
{
	if (item == NULL || bagType >= BAG_TYPE_COUNT)					// null item
		return BOOLEAN_FALSE;

	UInt8 tempQuality = quality;
	if (tempQuality == 0 || tempQuality > item->GetStack())
		tempQuality = item->GetStack();

	UInt8 avail = 0;
	UInt8 count = 0;
	CItem* bagItem = NULL;
	for (int i = bagType * BAG_ITEM_COUNT; i < (bagType + 1) * BAG_ITEM_COUNT; ++i)
	{
		bagItem = GetItem(i);
		if (bagItem && bagItem->GetItem_id() == item->GetItem_id() &&
			bagItem->GetStack() < bagItem->m_data->overlap)
		{
			if (m_pOwner->IsInBattle())
			{
				if (bagItem->m_tryStack + tempQuality > 99)
					bagItem->m_tryStack = 99;
				else
					bagItem->m_tryStack += tempQuality;
			}
			avail = bagItem->m_data->overlap - bagItem->GetStack();
			if (tempQuality <= avail)
			{
				// can merge all
				if (item->GetItem_uid() > 0)
					LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, 
						m_pOwner->GetContext(), m_pOwner->GetChar_id(), 
						LogService_Proxy::Item::TARGET_UNKNOW, 0, 
						item->GetItem_uid(), item->GetItem_id(), tempQuality, 
						LogService_Proxy::Item::ACT_STACK_OUT);
				LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, 
					m_pOwner->GetContext(), m_pOwner->GetChar_id(), 
					LogService_Proxy::Item::TARGET_UNKNOW, 0,
					bagItem->GetItem_uid(), bagItem->GetItem_id(), tempQuality,
					LogService_Proxy::Item::ACT_STACK_IN);
				count = bagItem->GetStack() + tempQuality;
				bagItem->SetStack(count);
				item->SetStack(item->GetStack() - tempQuality);
				tempQuality = 0;
			}
			else
			{
				// partial merge
				if (item->GetItem_uid() > 0)
					LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, 
						m_pOwner->GetContext(), m_pOwner->GetChar_id(), 
						LogService_Proxy::Item::TARGET_UNKNOW,
						0, item->GetItem_uid(), item->GetItem_id(), avail, 
						LogService_Proxy::Item::ACT_STACK_OUT);
				LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, 
					m_pOwner->GetContext(), m_pOwner->GetChar_id(), 
					LogService_Proxy::Item::TARGET_UNKNOW, 0, 
					bagItem->GetItem_uid(), bagItem->GetItem_id(), avail, 
					LogService_Proxy::Item::ACT_STACK_IN);
				count = bagItem->m_data->overlap;
				bagItem->SetStack(count);
				item->SetStack(item->GetStack() - avail);
				tempQuality -= avail;
			}
			bagItem->SetLock((bagItem->GetLock() || item->GetLock()));
			if (!item->IsExist())								// no more need to stack
				break;
		}
	}
	return (!item->IsExist() || tempQuality == 0);									// all stacked?
}

Boolean CItemBag::IsEquiped(CONST UInt32 item_id)
{
	const ItemData *itemData = gGlobal.m_resource->GetItemData(item_id);
	if ( !itemData || (itemData->item_type != ITEMTYPE_EQUIP) ) return BOOLEAN_FALSE;

	UInt8 ei = 255;

	if (itemData->eqType < EQTYPE_GEAR)
		ei = EQPLACE_WEAPON + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_GEAR)
		ei = EQPLACE_GEAR + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_ARMOR)
		ei = EQPLACE_ARMOR + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_SHOE)
		ei = EQPLACE_SHOE + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_BELT)
		ei = EQPLACE_BELT + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_RING)
		ei = EQPLACE_RING + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_AMULET)
		ei = EQPLACE_AMULET + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_CLOTH)
		ei = EQPLACE_CLOTH + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_HAT_WITH_HAIR)
		ei = EQPLACE_HEAD_ACCESSORY + BAG_MAX_ITEM;
	else if (itemData->eqType == EQTYPE_HAT_WITHOUT_HAIR)
		ei = EQPLACE_HEAD_ACCESSORY + BAG_MAX_ITEM;

	CItem *item = GetItem(ei);
	if ( item != NULL && item->GetItem_id() == item_id )
		return BOOLEAN_TRUE;

	return BOOLEAN_FALSE;
}

UInt8 CItemBag::GetFreeSlot(UInt8 bagIndex)
{
	if (IsBagEnabled(bagIndex) && bagIndex < BAG_TYPE_COUNT)
	{
		UInt8 freeSlot = 0;
		RPCNetID empty = RPCNetID();
		for (int i = bagIndex * BAG_ITEM_COUNT; 
			i < (bagIndex + 1) * BAG_ITEM_COUNT; ++i)
		{
			if (m_items[i] == empty)
				++freeSlot;
		}
		return freeSlot;
	}
	return 0;
}

UInt16 CItemBag::GetTotalFreeSlot()
{
	UInt16 totalFree = 0;
	for (UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i)
	{
		totalFree += (UInt16)GetFreeSlot(i);
	}
	return totalFree;
}

Boolean CItemBag::CanAddItem(UInt8 bagIndex, UInt32 item_id, UInt16 count)
{
	if (count == 0) return BOOLEAN_TRUE;

	UInt16 remain = count;
	if (IsBagEnabled(bagIndex))
	{
		if (!CanCreateLimitItem(item_id, count))
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag NewItem: item_id %d reach the generate limit"), 
			item_id);
			return BOOLEAN_FALSE;
		}

		if (bagIndex >= BAG_TYPE_COUNT)
			return BOOLEAN_FALSE;
		// stack check
		for (int i = (bagIndex * BAG_ITEM_COUNT); i < ((bagIndex + 1) * BAG_ITEM_COUNT); ++i)
		{
			CItem *item = GetItem(i);
			if (item && item->m_data && item_id == item->GetItem_id() &&
				item->GetStack() < item->m_data->overlap)
			{
				UInt32 avail = (item->m_data->overlap > item->GetStack()) ? item->m_data->overlap - item->GetStack() : 0;
				if (remain <= avail)							// can merge all
				{
					remain = 0;
					break;
				} else													// partial merge
				{
					remain -= avail;
				}
			}
		}

		// non stackable (non-overlap), check free slot
		if (remain != 0)
		{
			const ItemData *itemData = gGlobal.m_resource->GetItemData(item_id);
			if (itemData == NULL)
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CItemBag NewItem: item_id %d not found in resource"), 
					item_id);
				return BOOLEAN_FALSE;
			}
			UInt32 freeslot = (UInt32)GetFreeSlot(bagIndex);
			if (remain <= (UInt32)itemData->overlap * freeslot)
				remain = 0;
		}	
	}
	return (remain==0);
}

UInt16 CItemBag::AddItem(UInt32 item_id, UInt16 count, UInt16 quality, UInt8 bagIndex)
{
	CItem* item = NULL;
	UInt16 remain = count;
	UInt16 stack;
	while (remain > 0) {
		item = NewItem(item_id, (UInt8) MIN(remain, 255));
		if (item == NULL)
			break;
		stack = item->GetStack();
		item->SetQuality(quality);
		if (item && !AddItem(item, bagIndex) &&			// add to current first
			bagIndex == BAG_CURRENT && !AddItem(item, BAG_BACKUP))	// then add to backup
		{
			DeleteNetObject(item);
			break;
		}
		
		if (remain > stack)
			remain -= stack;
		else
			remain = 0;
	}
	return (count - remain);
}

UInt16 CItemBag::GetItemCount(UInt32 item_id)
{
	UInt16 total = 0;

	CItem* item = NULL;
	for (UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
	{
		if (IsBagEnabled(i))
		{
			for (int j = (i * BAG_ITEM_COUNT); j < ((i + 1) * BAG_ITEM_COUNT); ++j)
			{
				item = GetItem(j);
				if (item && item->GetItem_id() == item_id)
					total += (UInt16)(item->GetStack());
			}
		}
	}
	return total;
}

UInt16 CItemBag::GetUnlockItemCount(UInt32 item_id)
{
	UInt16 total = 0;

	CItem* item = NULL;
	for (UInt8 i = BAG_CURRENT; i < BAG_TYPE_COUNT; ++i )
	{
		if (IsBagEnabled(i))
		{
			for (int j = (i * BAG_ITEM_COUNT); j < ((i + 1) * BAG_ITEM_COUNT); ++j)
			{
				item = GetItem(j);
				if (item && item->GetItem_id() == item_id && !item->GetLock())
					total += (UInt16)(item->GetStack());
			}
		}
	}
	return total;
}

void CItemBag::RemoveBuffAfterOffline()
{
	CItem * pItem = NULL;
	//check weapon buff
	for (UInt i = 0; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++) 
	{
		pItem = GetItem(i);
		if (pItem && pItem->GetBuffID() && pItem->IsRemoveAfterOffline())// remove the buff from item bag
			pItem->RemoveBuff();
		if (i < (UInt)(m_pOwner->GetAvailItemWareHouse() * BAG_ITEM_COUNT))// remove the buff from warehouse
		{
			CItem* tempItem = GetItemFromWareHouse(i);
			if (tempItem && tempItem->GetBuffID() && tempItem->IsRemoveAfterOffline())
			{
				m_bWareHouseUpdated = BOOLEAN_TRUE;
				tempItem->RemoveBuff();
			}
		}
	}
}

void CItemBag::UpdateTimeBuff()
{
	CItem * pItem = NULL;
	//check weapon buff
	for (UInt i = 0; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	{
		pItem = GetItem(i);
		if (pItem && pItem->GetBuffID())
			pItem->UpdateBuff();		
	}
}
void CItemBag::UpdateTurnBuff()
{
	//check weapon buff
	for (UInt i = 0; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	{
		CItem * pItem = GetItem(i);
		if (pItem && pItem->GetBuffID() && pItem->m_buffData)
		{
			if (pItem->m_buffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END &&
				pItem->m_buffData->Buff_period_type == BUFF_PERIOD_TYPE_TURN)
			{
				if (pItem->GetBuffCoolDown() == 0)
				{
					pItem->RemoveBuff();
				}
				else
					pItem->SetBuffCoolDown(pItem->GetBuffCoolDown() - 1);
			}
		}
	}
}
RPCResult CItemBag::LoadItems(RPCContext &context)
{
	if (m_pOwner == NULL)
	{
		PROCEDURE_ERROR_OUTSIDE(RPC_RESULT_FAIL)
	}

	PROCEDURE_START_METHOD(CItemBag, LoadItems, this, context)

		SetOwner_id(m_pOwner->GetChar_id());

		// get items from object service
		ObjectService_Proxy::GetItemsByOwner(gGlobal.pObjectService->pPeer, context, 
			GetOwner_id(), 0, CItemData::DETAIL_MAP);

	PROCEDURE_WAIT2(1, ObjectService, CB_GetItemGroup, NetGroup*, items, UInt8, detail)

		TRACE_ENSURE(items);
		for (Index i = C_INDEX(0); i < items->GetCount(); i++)
		{
			CItemData *pItemData = items->GetChild<CItemData>(i);
			if (pItemData && pItemData->GetStack() > 0) {				// not null or empty item
				UInt8 bagIndex = pItemData->GetBagIndex();
				CItem* pItem = GetItem(bagIndex);
				// strange item exist in bag index
				if (pItem == NULL) {									// empty index
					pItem = CItem::NewInstance();
					m_items[bagIndex] = pItem->GetNetId();
				} else if (pItem->GetItem_uid() == pItemData->GetItem_uid()) {	// skip same item
					continue;
				}
				pItem->SetData(pItemData, false);
				pItem->SetSendMask(pItem->GetUpdateMask());
				pItem->ClearUpdateMask();
			}
			else
			{
				TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CItemBag LoadItems Error: invalid item"));
			}
		}

		UpdateItemSet();

		//check weapon buff
		for (UInt i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
		{
			CItem * pItem = GetItem(i);
			if (pItem && pItem->GetBuffID())
			{
				if (m_pOwner)
					m_pOwner->AddBuff(pItem->GetBuffID(), pItem->GetBuffValue1(), pItem->GetBuffValue2(), pItem->GetBuffValue3(), 
					pItem->GetBuffCoolDown() / 60, pItem->GetBuffPro(), false);
			}
		}

		ValidateItems(_T("LoadItems"));
		DeleteNetGroup(items, BOOLEAN_TRUE);
		
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CItemBag::UpdateItems(RPCContext &context)
{
	PROCEDURE_START_METHOD(CItemBag, UpdateItems, this, context)

		TRACE_ENSURE(m_pOwner);

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("UpdateItems: sessionId %s, char_id %d"), 
			context.sessionId.GetString().c_str(), m_pOwner->GetChar_id());

		ValidateItems(_T("UpdateItems"));

		NetGroup grp;
		CItemData *pItemData;
		UInt32Vector item_ids;
		UInt8Vector bagIndices, wareHouseBagIndices;
		RPCNetIDVector delItemNetIDList;
		// delGroup add to grp
		for (UInt16 i = 0; i < m_delGroup.GetCount(); ++i)
		{
			CItem* item = m_delGroup.GetChild<CItem>(i);
			if (item)
			{
				item_ids.push_back(item->GetItem_uid());
				pItemData = CreateNetObject<CItemData>();
				TRACE_ENSURE(pItemData);
				item->WriteData(pItemData);
				grp.Add(pItemData);
				delItemNetIDList.push_back(item->GetNetId());
				
				TRACE_INFOL_4(GLOBAL_LOGGER, _F("m_delGroup item_id = %d, item_uid = %d, bag index = %d, owner_id %d"), 
					pItemData->GetItem_id(), pItemData->GetItem_uid(), pItemData->GetBagIndex(), m_pOwner->GetChar_id());
			}
		}

		for (int i = 0; i < BAG_MAX_SIZE; i++)
		{
			CItem* item = GetItem(i);
			if (item == NULL ) 
				continue;
			if (item->GetItem_id() == 0)
			{
				// Alert here. that may need to do notifaction to object service for real delete the items in database.
				TRACE_INFOL_4(GLOBAL_LOGGER, _F("item_id = %d, item_uid = %d, bag index = %d, owner_id %d"), 
					item->GetItem_id(), item->GetItem_uid(), item->GetBagIndex(), m_pOwner->GetChar_id());
				RemoveItem(item);
			}
			else if (item->IsUpdated())
			{
				bagIndices.push_back((UInt8)i);
				item_ids.push_back(item->GetItem_uid());
				pItemData = CreateNetObject<CItemData>();
				TRACE_ENSURE(pItemData);
				item->WriteData(pItemData);
				grp.Add(pItemData);
				
				TRACE_INFOL_4(GLOBAL_LOGGER, _F("item updated item_id = %d, item_uid = %d, bag index = %d, owner_id = %d"), 
					pItemData->GetItem_id(), pItemData->GetItem_uid(), pItemData->GetBagIndex(), m_pOwner->GetChar_id());
			}
		}

		if (m_bWareHouseUpdated)
		{
			for (Index i = C_INDEX(0); i < BAG_MAX_WAREHOUSE_ITEM; ++i)
			{
				CItem* item = GetItemFromWareHouse(i);
				if (item == NULL ) 
					continue;
				
				if (item->IsUpdated())
				{
					wareHouseBagIndices.push_back((UInt8)i);
					item_ids.push_back(item->GetItem_uid());
					pItemData = CreateNetObject<CItemData>();
					TRACE_ENSURE(pItemData);
					item->WriteData(pItemData);
					grp.Add(pItemData);
					
					TRACE_INFOL_4(GLOBAL_LOGGER, _F("warehouse item updated item_id = %d, item_uid = %d, bag index = %d, owner_id = %d"), 
						pItemData->GetItem_id(), pItemData->GetItem_uid(), pItemData->GetBagIndex(), m_pOwner->GetChar_id());
				}
			}
			m_bWareHouseUpdated = BOOLEAN_FALSE;
		}

		if (item_ids.empty())
		{
			// nothing to update
			PROCEDURE_RETURN
		}

		TRACE_INFOL_1(GLOBAL_LOGGER, _F("Item Count=%d"), item_ids.size());

		PROCEDURE_STORE_VALUE3(UInt8Vector, bagIndices, UInt8Vector, wareHouseBagIndices, RPCNetIDVector, delItemNetIDList)
		m_delGroup.Clear();

		// update to object service
		ObjectService_Proxy::UpdateItemList(gGlobal.pObjectService->pPeer, context, item_ids, m_pOwner->GetChar_id(), &grp, BOOLEAN_TRUE);
		DeleteNetChilds(&grp);

		TRACE_INFOL(GLOBAL_LOGGER, _F("UpdateItemList sent"));

	PROCEDURE_WAIT1(1, ObjectService, CB_UpdateItemList, NetGroup*, newItems)

		for (UInt8 i = 0; i < newItems->GetCount(); i++)
		{
			CItemData *pItemData = newItems->GetChild<CItemData>(i);
			if (pItemData)
			{
				CItem *item = NULL;
				if (pItemData->GetInWareHouse() == 0)
					item = GetItem(pItemData->GetBagIndex());
				else
					item = GetItemFromWareHouse(pItemData->GetBagIndex());
				if (item)
				{
					item->SetData(pItemData, false);
					switch (item->GetCreateType())
					{
						case CREATE_BY_SYSTEM:
							LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, 
								m_pOwner->GetContext(), m_pOwner->GetChar_id(), 
								LogService_Proxy::Item::TARGET_UNKNOW,
								0, item->GetItem_uid(), item->GetItem_id(), item->GetStack(), 
								LogService_Proxy::Item::ACT_CREATE);
							break;
						case CREATE_BY_EXCHANGE:
							LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, 
								m_pOwner->GetContext(), m_pOwner->GetChar_id(), 
								LogService_Proxy::Item::TARGET_UNKNOW,
								0, item->GetItem_uid(), item->GetItem_id(), item->GetStack(), 
								LogService_Proxy::Item::ACT_CREATE_TRANSFER);
							break;
						case CREATE_BY_WAREHOUSE:
							LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, 
								m_pOwner->GetContext(), m_pOwner->GetChar_id(), 
								LogService_Proxy::Item::TARGET_UNKNOW,
								0, item->GetItem_uid(), item->GetItem_id(), item->GetStack(), 
								LogService_Proxy::Item::ACT_CREATE_WAREHOUSE);							
							break;
					}
					item->ResetCreateType();					
				}
			}
		}
		DeleteNetGroup(newItems, BOOLEAN_TRUE);

		// Update successfully
		PROCEDURE_RESTORE_VALUE3(UInt8Vector, bagIndices, UInt8Vector, wareHouseBagIndices, RPCNetIDVector, delItemNetIDList)

		// update to client
		NetGroup itemGroup;
		NetGroup delGroup;
		CItem* item;
		// delGroup add to itemGroup
		for (UInt16 i = 0; i < delItemNetIDList.size(); ++i )
		{
			item = FindNetObject<CItem>(delItemNetIDList[i]);
			if (item == NULL) {
				TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CItemBag UpdateItems: null item after object updated"));
				continue;
			}
			item->SetSendMask(item->GetUpdateMask());
			item->ClearUpdateMask();
			itemGroup.Add(item);
			delGroup.Add(item);
		}

		for (UInt8Vector::iterator it = bagIndices.begin(); it != bagIndices.end(); it++) {
			item = GetItem(*it);
			if (item == NULL) {
				TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CItemBag UpdateItems: null item after object updated"));
				continue;
			}
			if (item->IsUpdated(CItem::owner_idMask))
				item->SetSendMask(CItem::AllMask);
			else
				item->SetSendMask(item->GetUpdateMask());
			item->ClearUpdateMask();
			itemGroup.Add(item);
		}

		for (Index i = C_INDEX(0); i < wareHouseBagIndices.size(); ++i) 
		{
			item = GetItemFromWareHouse(wareHouseBagIndices[i]);
			if (item == NULL) {
				TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("null item after object updated"));
				continue;
			}
			if (item->IsUpdated(CItem::owner_idMask))
				item->SetSendMask(CItem::AllMask);
			else
				item->SetSendMask(item->GetUpdateMask());
			item->ClearUpdateMask();
			itemGroup.Add(item);
		}

		CB_SyncItems(m_pOwner->GetPeer(), m_pOwner->GetContext(), &itemGroup);

		for (UInt8Vector::iterator it = bagIndices.begin(); it != bagIndices.end(); it++)
		{
			CItem *item = GetItem(*it);
			if (item && !item->IsExist())
			{
				m_items[*it] = RPCNetID();
				DeleteNetObject(item);
			}
		}
		// delete item that is not exist
		DeleteNetChilds(&delGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdateItems Error: ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CItemBag::SetOwner(CCharacter* pchar)
{
	m_pOwner = pchar;
	if (pchar)
		SetOwner_id(pchar->GetChar_id());
	else
		SetOwner_id(0);
}

CItem* CItemBag::NewItem(UInt32 item_id, UInt8 count, bool needCheck)
{
	if (count == 0)
		return NULL;

	const ItemData *itemData = gGlobal.m_resource->GetItemData(item_id);
	if (itemData == NULL)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CItemBag NewItem: item_id %d not found in resource"), 
			item_id);
		return NULL;
	}

	if (needCheck && itemData->item_GenerateLimit && !CanCreateLimitItem(item_id, count))
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag NewItem: item_id %d reach the generate limit"), 
			item_id);
		
		return NULL;
	}

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("owner_id %d, owner session_id %s, item_id %d, count %d"), 
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), item_id, count);

	CItem* pItem = CItem::NewInstance();
	TRACE_ENSURE(pItem);
	pItem->m_data = itemData;
	pItem->SetId(0, item_id);
	pItem->SetStack(MIN(count, itemData->overlap));
	if (itemData->useCount == 255)
		pItem->SetRemain(255);
	else
		pItem->SetRemain(MIN(count, itemData->useCount));

	if (needCheck && itemData->item_GenerateLimit)
	{
		ItemLimitMap::iterator itr = gGlobal.m_itemLimitMap.find(item_id);
		time_t nowTime;
		time(&nowTime);
		UInt limitPeriod = itemData->item_GenerateLimit_Time * 60; //(3600s = 1 hour)
		if (itr != gGlobal.m_itemLimitMap.end() )
		{
			if (itr->second.endTime > (UInt32)nowTime)
				itr->second.stack = itr->second.stack + pItem->GetStack();
			else
			{
				itr->second.stack = pItem->GetStack();
				UInt32 diffTime = (UInt32)nowTime - itr->second.endTime;
				UInt period = diffTime / limitPeriod + 1;
				itr->second.endTime = itr->second.endTime + period * limitPeriod;
			}			
		}
		else
		{
			ItemLimit itemlim;
			itemlim.item_id = item_id;
			itemlim.stack = pItem->GetStack();
			itemlim.endTime = (UInt32)nowTime + limitPeriod; 
			gGlobal.m_itemLimitMap.insert(std::make_pair(item_id, itemlim));
		}
	}

	return pItem;
}

//if quality == 0 mean that a whole item will move to this bag
Boolean CItemBag::AddItem(CItem* item, UInt8 bagType, UInt8 quality)
{
	// 1 is a address for reserved the dummy item 
	if (item == DUMMY_ITEM_ADDRESS)
	{
		if (!IsBagEnabled(bagType))	// backpack not enable
			return BOOLEAN_FALSE;
		
		UInt8 bagIndex = 0;
		if (!FindEmptyIndex(bagType, bagIndex, bagType * BAG_ITEM_COUNT))
			return BOOLEAN_FALSE;

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Add dummy item in bagIndex %d"), bagIndex);
		
		m_items[bagIndex] = m_dummyItem->GetNetId();
		return BOOLEAN_TRUE;
	}

	if (item == NULL || !item->IsExist())			// null item
		return BOOLEAN_FALSE;

	if (!IsBagEnabled(bagType))	// backpack not enable
		return BOOLEAN_FALSE;

	UInt8 remainStack = item->GetStack();
	if (StackItem(item, bagType, quality))
	{
		if (item->GetItem_uid() == 0)
			DeleteNetObject(item);
		RaiseUpdate();

		return BOOLEAN_TRUE;
	}

	remainStack = quality - (remainStack - item->GetStack());

	// remain not stacked
	UInt8 bagIndex = 0;
	if (!FindEmptyIndex(bagType, bagIndex, bagType * BAG_ITEM_COUNT))
		return BOOLEAN_FALSE;

	if (item->GetItem_uid() != 0 && quality != 0)
	{
		UInt remain = item->GetRemain();
		item->SetStack(item->GetStack() - remainStack);
		item = NewItem(item->GetItem_id(), remainStack);
		if (item)
		{
			item->SetRemain(remain);
			item->SetCreateType(CItemBag::CREATE_BY_EXCHANGE);
		}
	}

	if (item)
	{
		item->SetOwner_id(GetOwner_id());
		item->SetBagIndex(bagIndex);
		m_items[bagIndex] = item->GetNetId();
	}
	RaiseUpdate();

	return BOOLEAN_TRUE;
}

CItem* CItemBag::AddRandItem(const String &csvFile)
{
	CONST CSVItemData* itemList = gGlobal.m_resource->GetCSVItemInfo(csvFile);
	if (itemList == NULL)
		return NULL;

	UInt32 totalRange = 0;
	CSVItemRow* pRow;
	for (UInt16 i = 0; i < itemList->RowLst.size(); ++i)
	{
		pRow = itemList->RowLst[i];
		if (pRow)
			totalRange += pRow->uRange;
	}
	UInt32 range = GetRand((int)totalRange);
	UInt32 item_id = 0;
	CItem* pItem = NULL;
	for (UInt16 i = 0; i < itemList->RowLst.size(); ++i)
	{
		pRow = itemList->RowLst[i];
		if (pRow && !pRow->ItemLst.empty()) {
			if (range < pRow->uRange) {
				item_id = pRow->ItemLst[GetRand((int)pRow->ItemLst.size())];
				AddItem(item_id, 1, BAG_CURRENT);
				break;
			}
		}
		if (pRow->uRange < range)
			range -= pRow->uRange;
		else
			range = 0;
	}

	return pItem;
}

Boolean CItemBag::RemoveItem(CItem* item, Boolean needDelete)
{
	if (item == NULL)
		return BOOLEAN_FALSE;

	if (m_items[item->GetBagIndex()] == item->GetNetId())
	{
		m_items[item->GetBagIndex()] = RPCNetID();
		if (needDelete)
			DeleteNetObject(item);
		return BOOLEAN_TRUE;
	}
	else
	{
		for (int i = 0; i < BAG_MAX_ITEM; i++)
		{
			if (m_items[i] == item->GetNetId())
			{
				if (needDelete)
					DeleteNetObject(item);
				m_items[i] = RPCNetID();
				return BOOLEAN_TRUE;
			}
		}
	}
	return BOOLEAN_FALSE;
}

Boolean CItemBag::RemoveItem(UInt32 item_id, UInt16 count)
{
	if (item_id == 0)
		return BOOLEAN_FALSE;

	bool updated = false;
	CItem* item = NULL;
	for (Index i = C_INDEX(0); i < BAG_MAX_ITEM; i++)
	{
		item = GetItem(i);
		if (item && item->GetItem_id() == item_id)
		{
			UInt8 minCount = MIN(item->GetStack(), count);
			item->SetStack(item->GetStack() - minCount);
			if (item->GetStack() == 0)
				AddToDeleteGroup(item->GetBagIndex());
			count -= minCount;
			updated = true;
			if (count == 0)
				break;
		}
	}

	if (updated)
		RaiseUpdate();

	return updated;
}

Boolean CItemBag::RemoveUnLockItem(UInt32 item_id, UInt16 count)
{
	if (item_id == 0)
		return BOOLEAN_FALSE;

	bool updated = false;
	CItem* item = NULL;
	for (Index i = C_INDEX(0); i < BAG_MAX_ITEM; i++)
	{
		item = GetItem(i);
		if (item && (item->GetItem_id() == item_id) && !item->GetLock())
		{
			UInt8 minCount = MIN(item->GetStack(), count);
			item->SetStack(item->GetStack() - minCount);
			if (item->GetStack() == 0)
				AddToDeleteGroup(item->GetBagIndex());
			count -= minCount;
			updated = true;
			if (count == 0)
				break;
		}
	}

	if (updated)
		RaiseUpdate();

	return updated;
}


Boolean CItemBag::RemoveItemByUID(UInt32 item_uid, UInt16 count)
{
	if (item_uid == 0)
		return BOOLEAN_FALSE;

	CItem* pItem = GetItemByUID(item_uid);
	if (pItem) {
		UInt8 minCount = MIN(pItem->GetStack(), count);
		pItem->SetStack(pItem->GetStack() - minCount);
		if (pItem->GetStack() == 0)
			AddToDeleteGroup(pItem->GetBagIndex());
		RaiseUpdate();
		return BOOLEAN_TRUE;
	} else {
		return BOOLEAN_FALSE;
	}
}

void CItemBag::SendAll()
{
	// send item bag to client
	if (m_pOwner)
	{
		NetGroup itemGroup;
		PreSend(&itemGroup);
		CB_SyncItems(m_pOwner->GetPeer(), m_pOwner->GetContext(), &itemGroup);
	}
}

void CItemBag::PreSend(NetGroup *itemGroup, int count)
{
	RPCMask mask;
	mask.MaskBits(owner_idMask, -1);
	SetSendMask(mask);

	CItem* item = NULL;
	for (int i = 0; i < count; ++i)
	{
		item = GetItem(i);
		if (item)
		{
			item->PreSend();
			itemGroup->Add(item);
		}
	}
}

void CItemBag::SyncTryCount()
{
	CItem *item = NULL;

	for (UInt8 i = 0; i < BAG_ITEM_COUNT; i++)
	{
		item = GetItem(i);
		if (item && (item->m_tryStack != item->GetStack() || 
				item->m_tryRemain != item->GetRemain()))
		{
			item->m_tryStack = item->GetStack();
			item->m_tryRemain = item->GetRemain();
			if (m_pOwner)
				CB_ItemCount(m_pOwner->GetPeer(), m_pOwner->GetContext(), i, 
					item->m_tryStack, item->m_tryRemain);
		}
	}
}

void CItemBag::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_OBJECT:				// update all partner to object service
		if (m_pOwner)
		{
			PROCEDURE_DO_METHOD(CItemBag, UpdateItems, this, m_pOwner->GetContext())
		}
		break;
	}
}

RPCResult CItemBag::MarkMixStartTime(LPCPEER pPeer, RPCContext &context, 
									 const Boolean &startOrCancel)
{
	//startOrCancel: start = true, cancel = false
	if (startOrCancel)
	{
		time_t nowTime;
		time(&nowTime);
		m_iStartMakeTime = (UInt32)nowTime;
	}
	else
	{
		m_iStartMakeTime = 0;
	}
	TRACE_INFOL_3(GLOBAL_LOGGER, _F("owner_id %d, owner session_id %s, start time is %d"), 
		m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(),  m_iStartMakeTime);

	CB_MarkMixStartTime(pPeer, context, startOrCancel);
	return RPC_RESULT_OK;
}

RPCResult CItemBag::MakeItemMix(LPCPEER pPeer, RPCContext &context,
								const UInt32 &mixProductID, const UInt32 &mixShopID, 
								const UInt32Vector &itemUids, const UInt8Vector &stacks)
{
	Boolean success = BOOLEAN_FALSE;
	UInt8 msgCode = MIXSHOP_SUCCESS;
	const MixShopData* mixShop = NULL;
	const ItemMixData* itemMix = NULL;
	UInt8Vector reduceStacks;  
	float quality = 0.0f;
	
	reduceStacks.clear();
	for (Index i = C_INDEX(0); i < stacks.size(); ++i)
		reduceStacks.push_back(0);

	TRACE_INFOL_1(GLOBAL_LOGGER, _F("MakeItemMix: start make item for char_id = %d"), m_pOwner->GetChar_id());
	do 
	{
		if (itemUids.size() != stacks.size())
		{
			msgCode = MIXSHOP_ERROR;
			break;
		}
	
		if (mixProductID == 0 || mixShopID == 0)
		{
			msgCode = MIXSHOP_ERROR;
			break;
		}

		for (Index i = C_INDEX(0); i < itemUids.size(); ++i)
		{
			CItem* checkItem = GetItemByUID(itemUids[i]);
			if (checkItem == NULL || (checkItem->GetStack() < stacks[i]) || checkItem->GetLock())
			{
				msgCode = MIXSHOP_ERROR;
				break;
			}
		}

		if (msgCode != MIXSHOP_SUCCESS)
			break;

		//check the bag is full or not
		if (GetTotalFreeSlot() == 0)
		{
			msgCode = MIXSHOP_NO_ITEMSLOT;
			break;
		}
	
		mixShop = gGlobal.m_resource->GetMixShopData(mixShopID);
		if (mixShop)
			itemMix = gGlobal.m_resource->GetItemMixData(String(mixShop->mixItemTable), mixProductID);
			 
		if (mixShop == NULL || itemMix == NULL || m_iStartMakeTime == 0)
		{
			msgCode = MIXSHOP_ERROR;
			break;
		}
			
		time_t nowTime;
		time(&nowTime);

		UInt32 diffTime = (UInt32)nowTime - m_iStartMakeTime; //in second
		if (diffTime > (itemMix->consumeTime * 9 / 10) )
		{			
			UInt8Vector stoneProb;
			stoneProb.clear();
			bool hasLightStone = false;
			CItem* eqItem = NULL;
						
			//check the material and the number is correct.
			typedef StlMap<UInt32, UInt8, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, UInt8> > > ItemIDStack;
			ItemIDStack temp;
			temp.clear();
			for (UInt8 i = 0; i < itemMix->materials.size(); ++i)
			{
				if (itemMix->materials[i] > 0)
					temp.insert(std::make_pair(itemMix->materials[i], itemMix->numOfMaterials[i]));
			}

			for (UInt i = 0; i < itemUids.size(); ++i)
			{
				CItem* item = GetItemByUID(itemUids[i]);
				if (item && item->m_data)
				{
					ItemIDStack::iterator itr =temp.end();
					if (item->m_data->item_type == ITEMTYPE_STONE)
						itr = temp.find(item->m_data->root_id);
					else
						itr = temp.find(item->GetItem_id());

					if (itr != temp.end())
					{
						if (itr->second >= stacks[i])
						{
							itr->second = itr->second - stacks[i];
							reduceStacks[i] = stacks[i];
						}
						else
						{
							reduceStacks[i] = itr->second;
							itr->second = 0;
						}
					}
					if (stoneProb.size() == 0 && item->m_data->item_type == ITEMTYPE_STONE)
					{
						bool bSpeicalRate = false;
						if (mixShop->shopID >= 26 && mixShop->shopID <= 34)
						{
							if (m_pOwner && m_pOwner->GetQuests())
							{
								UInt16 FlagId = 50;
								UInt32 flag = m_pOwner->GetQuests()->GetFlag(FlagId);
								TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" flagid : %d = %d."), FlagId, flag);
								if (flag > 1000)
								{
									gGlobal.m_resource->GetStoneProbList(item->GetItem_id() + 70, stoneProb);
									bSpeicalRate = true;
								}
							}
						}
						if (!bSpeicalRate)
						{
							gGlobal.m_resource->GetStoneProbList(item->GetItem_id(), stoneProb);
						}
					}
					else if (item->m_data->item_type == ITEMTYPE_LIGHTSTONE)
						hasLightStone = true;
					else if (item->m_data->item_type == ITEMTYPE_EQUIP && eqItem == NULL)
						eqItem = item;
				}
			}

			if (hasLightStone)
			{
				if (eqItem == NULL)
				{
					msgCode = MIXSHOP_ERROR;
					break;
				}

				if (eqItem->GetItem_IdentifyState())
				{
					msgCode = MIXSHOP_ERROR;
					break;
				}
			}

			for (ItemIDStack::iterator itr = temp.begin(); itr != temp.end(); ++itr)
			{
				if (itr->second != 0)
				{
					msgCode = MIXSHOP_ERROR;
					break;
				}
			}

			if (msgCode != MIXSHOP_SUCCESS)
				break;

			msgCode = GetMixShopConsumeMsgIndex(itemMix->consumeType, itemMix->consumeAmount);

			if (msgCode != MIXSHOP_SUCCESS)
				break;
			UInt logIndex = LogService_Proxy::Money_And_Exp::SOURCE_UNKNOWN;
			Boolean bFinishMake = BOOLEAN_FALSE;
			UInt32 itemID = 0;
			UInt outputNum = 0;
			if (msgCode == MIXSHOP_SUCCESS  )
			{				
				switch (mixShop->type)
				{
				case MIX_WEAPONMAKING:
				case MIX_CLOTHMAKING:
				case MIX_ORNAMENTMAKING:
					{
						if (stoneProb.size() > 0)
						{
							int prob = GetRand(1, 100);
							int count = 100;
							if (mixShop->shopID >= 26 && mixShop->shopID <= 34)
							{
								prob = GetRand(1, 200);
								count = 200;
							}
							TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" prob = %d."), prob);
							for (UInt i = 0; i < (UInt)stoneProb.size(); i++) // test
							{
								TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" stoneProb[%d] = %d."), i, stoneProb[i]);
							}
							for (UInt i = ((UInt)stoneProb.size() - 1); i > 0; --i)
							{
								count -= stoneProb[i];
								if ( prob > count )
								{
									//stoneProb[0] is "false" colume, but output[0] is already a success therefore
									//stoneProb[1] to stoneProb[5] match output[0] to output[4]
									itemID = itemMix->output[i-1]; 
									outputNum = itemMix->outputNum[i-1];
									if (itemID == 0 && i > 0)
									{
										for (UInt j = i - 1; j > 0; --j)
										{
											itemID = itemMix->output[j];
											outputNum = itemMix->outputNum[j];
											if (itemID > 0)
												break;
										}
									}
									break;
								}
							}							
						}
						else
						{
							itemID = itemMix->output[0];
							outputNum = itemMix->outputNum[0];
						}
						logIndex = LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE;
					}
					break;
				case MIX_COOKING:
				case MIX_HERMETIC:
				case MIX_MAKEWINE:
					{
						if (itemMix->Quality == 1)
						{
							UInt16 j = m_pOwner->GetSkillLv(itemMix->liveSkillID);
							if(itemMix->productType == ITEMTYPE_FOOD)
							{
								float iRand = GetRand(2.5f, 3.0f);								
								quality = iRand * ( static_cast<float>(j - 50) );
							}
							else if (itemMix->productType == ITEMTYPE_DRUG)
							{
								float iRand = GetRand(2.5f, 3.0f);
								quality = iRand * ( static_cast<float>(j - 50) );
							}
							else if (itemMix->productType == ITEMTYPE_WINE)
							{
								float iRand = GetRand(1.2f, 1.5f) ;
								quality = iRand * static_cast<float>(j) ;
							}
						}
						itemID = itemMix->output[0];
						outputNum = itemMix->outputNum[0];
						logIndex = LogService_Proxy::Money_And_Exp::SOURCE_LIVESKILL_MAKE;
					}
					break;
				case MIX_IRONMAKE:
					itemID = itemMix->output[0];
					outputNum = itemMix->outputNum[0];
					logIndex = LogService_Proxy::Money_And_Exp::SOURCE_LIVESKILL_MAKE;
					break;
				}

				if (itemID > 0 && outputNum > 0)
				{
					if (eqItem && hasLightStone)
					{
						eqItem->SetItem_id(itemID);
						eqItem->m_data = gGlobal.m_resource->GetItemData(itemID);
						bFinishMake = BOOLEAN_TRUE;
					}
					else if (CanCreateLimitItem(itemID, outputNum))
					{
						TRACE_INFOL_3(GLOBAL_LOGGER, _F("MakeItemMix: success to make for char_id = %d, item_id = %d and output number = %d"), 
							m_pOwner->GetChar_id(), itemID, outputNum);
						CItem* pNewItem = NewItem(itemID, outputNum);
						if ( pNewItem )
						{
							pNewItem->SetMaker(m_pOwner->GetNickName());
							if(itemMix->Quality == 1)
								pNewItem->SetQuality(quality);
							for ( UInt8 j = BAG_CURRENT; j < BAG_TYPE_COUNT; ++j )
							{
								if (AddItem(pNewItem, j))
								{
									bFinishMake = BOOLEAN_TRUE;
									break;
								}
							}
							if (!bFinishMake)
								DeleteNetObject(pNewItem);
						}
					}
					else
						msgCode = MIXSHOP_ITEM_REACH_LIMIT;
				}
				else
					msgCode = MIXSHOP_FAIL;
			}
			
			if (bFinishMake)
			{
				if (mixShop->shopID >= 26 && mixShop->shopID <= 34)
				{
					if (m_pOwner && m_pOwner->GetQuests())
					{
						UInt16 FlagId = 50;
						UInt32 flag = m_pOwner->GetQuests()->GetFlag(FlagId);
						m_pOwner->GetQuests()->SetFlag(FlagId, flag + 1);

						TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" Flag %d : %d -> %d."), 
							FlagId, flag, m_pOwner->GetQuests()->GetFlag(50));
					}
				}

				TRACE_INFOL_1(GLOBAL_LOGGER, _F("MakeItemMix: Finish for char_id %d"), m_pOwner->GetChar_id());
				ReduceMixShopConsume(itemMix->consumeType, itemMix->consumeAmount, 1, logIndex);

				UInt32Vector logSourceItemId;
				for (UInt8 k = 0; k < itemUids.size(); ++k)
				{
					CItem* item = GetItemByUID(itemUids[k]);
					if (item)
					{
						if (!hasLightStone || (hasLightStone && item != eqItem))
						{
							item->SetStack(item->GetStack() - reduceStacks[k]);
							if (item->GetStack() == 0)
								AddToDeleteGroup(item->GetBagIndex());
						}
						logSourceItemId.push_back(item->GetItem_id());
					}
				}
				LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), mixShop->type, 
					itemID, outputNum, itemUids, logSourceItemId, reduceStacks);
				RaiseUpdate();
				success = BOOLEAN_TRUE;
				//for checking that it can done another time
				switch (mixShop->type)
				{
				case MIX_COOKING:
				case MIX_HERMETIC:
				case MIX_MAKEWINE:
					msgCode = GetMixShopConsumeMsgIndex(itemMix->consumeType, itemMix->consumeAmount);
					break;
				}
			}
			else
				msgCode = MIXSHOP_FAIL;
		}		
	}
	while(false);

	m_iStartMakeTime = 0;

	CB_MakeItemMix(pPeer, context, success, msgCode, itemUids, reduceStacks);
	return RPC_RESULT_OK;
}

RPCResult CItemBag::MixItem(LPCPEER pPeer, RPCContext &context, const UInt32 &mixShopID, const UInt32Vector &itemUids, const UInt8Vector &stacks, const UInt8 &removeGemIndex)
{
	Boolean success = BOOLEAN_FALSE;
	UInt8 msgCode = MIXSHOP_SUCCESS;
	const MixShopData* mixShop = NULL;

	do
	{				
		mixShop = gGlobal.m_resource->GetMixShopData(mixShopID); 
		if (mixShop == NULL || m_iStartMakeTime == 0 || itemUids.size() == 0 || stacks.size() == 0)
		{
			msgCode = MIXSHOP_ERROR;
			break;
		}

		for (Index i = C_INDEX(0); i < itemUids.size(); ++i)
		{
			CItem* checkItem = GetItemByUID(itemUids[i]);
			if (checkItem == NULL || (checkItem->GetStack() < stacks[i]) || checkItem->GetLock())
			{
				msgCode = MIXSHOP_ERROR;
				break;
			}
		}

		if (msgCode != MIXSHOP_SUCCESS)
			break;

		if (mixShop->slotSwitch01 == 1 )//MIX_SWITCH_ITEM)
		{
			CItem* tempItem = NULL;
			bool haveItem = false;
			for (Index i = C_INDEX(0); i < itemUids.size(); ++i)
			{
				tempItem = GetItemByUID(itemUids[i]);
				if (tempItem && tempItem->m_data && 
					tempItem->m_data->item_type == mixShop->slotItemType01)
				{
					haveItem = true;
					break;
				}
			}
			if (!haveItem)
			{
				msgCode = MIXSHOP_ERROR;
				break;
			}
		}
			
		time_t nowTime;
		time(&nowTime);

		UInt32 diffTime = (UInt32)nowTime - m_iStartMakeTime; //in second
		if (diffTime > (mixShop->consumeTime * 9 / 10) )
		{
			switch(mixShop->type)
			{
			case MIX_EQUIPREPAIR:
			case MIX_IRONREPAIR:
				{
					msgCode = EquipRepair(itemUids, mixShop->consumeType, mixShop->consumeAmount);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
					case MIXSHOP_SUCCESS_REDUCE_MENDURING:
					case MIXSHOP_SUCCESS_REBUILD:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_STONEMIX:
				{
					msgCode = StoneMix(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_UNSOULBOUND:
				{
					msgCode = UnSoulBound(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount, mixShop);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_GEMMIX:
				{
					msgCode = GemMix(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_GEMINLAY:
				{
					msgCode = GemInlay(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_GEMREMOVE:
				{
					msgCode = GemRemove(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount, removeGemIndex);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_GEMMELTING:
				{
					msgCode = GemMelting(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_GEMPOLISHING:
				{
					msgCode = GemPolishing(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			case MIX_COLORSTONEMIX:
				{
					msgCode = ColorStoneMix(itemUids, stacks, mixShop->consumeType, mixShop->consumeAmount);
					switch (msgCode)
					{
					case MIXSHOP_SUCCESS:
						success = BOOLEAN_TRUE;
						break;
					}
				}
				break;
			}
		}
	}
	while(false);

	m_iStartMakeTime = 0;
	CB_MixItem(pPeer, context, success, msgCode);
	return RPC_RESULT_OK;
}

void CItemBag::ReduceEqEnduring()
{
	CItem* item = NULL;
	bool needUpdate = false;
	for (UInt8 i = BAG_MAX_ITEM; i < BAG_MAX_SIZE; ++i)
	{
		item = GetItem(i);
		if (item && item->GetEndure() > 0 && GetRand(1, 100) < 15)
		{
			item->SetEndure(item->GetEndure() - 1);
			needUpdate = true;

			if (item->GetEndure() == 0 && item->GetBuffID())
				m_pOwner->RemoveBuff(item->GetBuffID());

			if (item->GetEndure() == 0 && item->GetSkillID())
				m_pOwner->RemoveSkillTemp(item->GetSkillID());
		}
	}

	if (needUpdate)
		RaiseUpdate();
}

void CItemBag::AddToDeleteGroup(UInt8 itemIndex, UInt8 inWareHouse)
{
	if (inWareHouse == 0)
	{
		CItem* item = GetItem(itemIndex);
		if (item && !item->IsExist())
		{
			m_delGroup.Add(item);
			m_items[itemIndex] = RPCNetID();
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("in normal bag owner_id %d, owner session_id %s, item_uid %d, item_id %d, bagIndex %d "), 
				m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), item->GetItem_uid(), item->GetItem_id(), itemIndex);
		}		
	}
	else
	{
		CItem* item = GetItemFromWareHouse(itemIndex);
		if (item && !item->IsExist())
		{
			m_delGroup.Add(item);
			m_wareHouseItems[itemIndex] = RPCNetID();
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("in warehouse owner_id %d, owner session_id %s, item_uid %d, item_id %d, bagIndex %d "), 
				m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), item->GetItem_uid(), item->GetItem_id(), itemIndex);
		}
		
	}
}

void CItemBag::ValidateItems(const String &header)
{
	String warnMsg;

	for (int i = 0; i < BAG_MAX_SIZE; i++)
	{
		CItem* item = GetItem(i);
		if (item == NULL)
		{
			if (m_items[i].IsValid())
			{
				RPCNetID netId = m_items[i];
				warnMsg.Format(_T("invalid item netId %d:%d"), netId.srvId, netId.objId);
				m_items[i] = RPCNetID();
			}
		}
		else if (item->GetItem_id() == 0)
		{
			warnMsg.Format(_T("invalid item id"));
		}
		else if (item->GetOwner_id() != m_pOwner->GetChar_id())
		{
			warnMsg.Format(_T("invalid owner id"));
		}
		else if (item->GetBagIndex() != i)
		{
			warnMsg.Format(_T("invalid bagIndex"));
		}
		if (!warnMsg.IsEmpty())
		{
			if (item)
			{
				TRACE_WARNDTL_8(GLOBAL_LOGGER, 
					_T("%s Validate Warning: [%s], char_id %d, item_uid %d, item_id %d, bagIndex %d(%d), owner_id %d"), 
					header.c_str(), warnMsg.c_str(), m_pOwner->GetChar_id(), 
					item->GetItem_uid(), item->GetItem_id(), item->GetBagIndex(), i,
					item->GetOwner_id());
				RemoveItem(item);
			}
			else
			{
				TRACE_WARNDTL_4(GLOBAL_LOGGER, 
					_T("%s Validate Warning: [%s], char_id %d, null item, bagIndex %d"), 
					header.c_str(), warnMsg.c_str(), m_pOwner->GetChar_id(), i);
			}
			warnMsg.Empty();
		}
	}
}

RPCResult CItemBag::IdentifyItem(LPCPEER pPeer, RPCContext &context, const UInt8 &ItemSlot, const UInt8 &IdentifyItemSlot)
{
	UInt8 msgIndex = IDENTIFY_SUCCESS;
	do
	{
		if (ItemSlot >= BAG_ITEM_COUNT || IdentifyItemSlot >= BAG_ITEM_COUNT)
		{
			msgIndex = IDENTIFY_ERROR_ITEM;
			break;
		}

		CItem* item = GetItem(ItemSlot);
		CItem* identifyItem = GetItem(IdentifyItemSlot);

		if (item == NULL || identifyItem == NULL || 
			item->m_data == NULL || identifyItem->m_data == NULL || identifyItem->GetLock())
		{
			msgIndex = IDENTIFY_NO_ITEM;
			break;
		}

		if (item->GetItem_IdentifyState())
		{
			msgIndex = IDENTIFY_ALREADY_IDENTIFY;
			break;
		}

		bool notSuitable = true;
		switch(identifyItem->m_data->eqType)
		{
		case EQTYPE_ALL_WEAPON:
			{
				switch (item->m_data->eqType)
				{
				case EQTYPE_SPEAR:
				case EQTYPE_WIPE:
				case EQTYPE_FAN:
				case EQTYPE_PEN:
				case EQTYPE_SWORD:
				case EQTYPE_DBSWORD:
				case EQTYPE_BLADE:
				case EQTYPE_OTHER:
				case EQTYPE_HANDRING:
				case EQTYPE_CLAW:
					notSuitable = false;
				}
			}
			break;
		case EQTYPE_ALL_CLOTH:
			{
				switch (item->m_data->eqType)
				{
				case EQTYPE_GEAR:
				case EQTYPE_ARMOR:
				case EQTYPE_SHOE:
				case EQTYPE_CLOTH:
					notSuitable = false;
				}
			}
			break;
		case EQTYPE_ALL_ORNAMENT:
			{
				switch (item->m_data->eqType)
				{
				case EQTYPE_BELT:
				case EQTYPE_RING:				
				case EQTYPE_HAT_WITH_HAIR:
				case EQTYPE_HAT_WITHOUT_HAIR:
					notSuitable = false;
				}
			}
			break;
		case EQTYPE_AMULET:
			{
				if (item->m_data->eqType == EQTYPE_AMULET)
					notSuitable = false;
			}
			break;
		}

		if (notSuitable)
		{
			msgIndex = IDENTIFY_FAIL_REQ;
			break;
		}

		if (identifyItem->m_data->item_type == ITEMTYPE_IDENTIFY && item->m_data->reqLv <= identifyItem->m_data->reqLv)
		{
			item->SetIdentifyState(1);
			identifyItem->SetStack(identifyItem->GetStack() - 1);
			if (identifyItem->GetStack() == 0)
				AddToDeleteGroup(identifyItem->GetBagIndex());
			RaiseUpdate();
		}
		else
		{
			msgIndex = IDENTIFY_FAIL_REQ;
			break;
		}
	}while(false);

	CB_IdentifyItem(pPeer, context, msgIndex, ItemSlot, IdentifyItemSlot);

	return RPC_RESULT_OK;
}

UInt CItemBag::GetMixShopConsumeMsgIndex(const UInt8Vector &consumeType, const UInt32Vector &consumeAmount, UInt price)
{
	
	for (Index i = C_INDEX(0); i < consumeType.size(); ++i)
	{
		if (consumeType[i] == MIXSHOP_CONSUME_NONE )
			continue;

		switch (consumeType[i])
		{
		case MIXSHOP_CONSUME_VP:
			if (m_pOwner->GetVP() < consumeAmount[i])
				return MIXSHOP_NO_VP;
			break;
		case MIXSHOP_CONSUME_EXP:
			if (m_pOwner->GetExp() < consumeAmount[i])
				return MIXSHOP_NO_EXP;
			break;
		case MIXSHOP_CONSUME_MONEY:
			if (m_pOwner->GetMoney() < price * consumeAmount[i])
				return MIXSHOP_NO_GMONEY;
			break;
		case MIXSHOP_CONSUME_YUANBAO:
			if (m_pOwner->GetYuanBao() < consumeAmount[i])
				return MIXSHOP_NO_MONEY;
			break;
		case MIXSHOP_CONSUME_HP:
			if (m_pOwner->GetHP() < consumeAmount[i])
				return MIXSHOP_NO_HP;
			break;
		case MIXSHOP_CONSUME_MHP:
			if (m_pOwner->GetMHP() < consumeAmount[i])
				return MIXSHOP_NO_MHP;
			break;
		case MIXSHOP_CONSUME_SP:
			if (m_pOwner->GetSP() < consumeAmount[i])
				return MIXSHOP_NO_SP;
			break;
		case MIXSHOP_CONSUME_MSP:
			if (m_pOwner->GetMSP() < consumeAmount[i])
				return MIXSHOP_NO_MSP;
			break;
		case MIXSHOP_CONSUME_GOODNESS:
			//if (m_pOwner->GetGoodness() < itemMix->consumeAmount[i])
			//	msgCode = MIXSHOP_NO_GOODNESS;
			break;
		case MIXSHOP_CONSUME_HONOR:
			if (m_pOwner->GetHonor() < consumeAmount[i])
				return MIXSHOP_NO_HONOR;
			break;
		case MIXSHOP_CONSUME_FACTIONDONATE:
			if (m_pOwner->GetFDonate() < consumeAmount[i])
				return MIXSHOP_NO_FACTIONDONATE;
			break;
		case MIXSHOP_CONSUME_MANORDONATE:
			if (m_pOwner->GetMDonate() < consumeAmount[i])
				return MIXSHOP_NO_MANORDONATE;
			break;
		}
	}

	return MIXSHOP_SUCCESS;
}

void CItemBag::ReduceMixShopConsume(const UInt8Vector &consumeType, const UInt32Vector &consumeAmount, UInt price, UInt16 logtype)
{
	for (UInt8 i = 0; i < consumeType.size(); ++i)
	{
		if (consumeType[i] == MIXSHOP_CONSUME_NONE )
			continue;

		switch (consumeType[i])
		{
		case MIXSHOP_CONSUME_VP:
			m_pOwner->SetVP(m_pOwner->GetVP() - consumeAmount[i], m_pOwner->GetMaxVP());
			break;
		case MIXSHOP_CONSUME_EXP:
			m_pOwner->ChangeExp(-(Int32)consumeAmount[i], logtype);
			break;
		case MIXSHOP_CONSUME_MONEY:
			m_pOwner->ChangeMoney(-(Int32)(price * consumeAmount[i]), logtype);
			break;
		//case MIXSHOP_CONSUME_YUANBAO:
		//	m_pOwner->SetYuanBao(m_pOwner->GetYuanBao() - consumeAmount[i]);
		//	break;
		case MIXSHOP_CONSUME_HP:
			m_pOwner->SetHP(m_pOwner->GetHP() - consumeAmount[i], m_pOwner->GetMHP(), m_pOwner->GetMaxHP());
			break;
		case MIXSHOP_CONSUME_MHP:
			m_pOwner->SetHP( m_pOwner->GetHP(), m_pOwner->GetMHP() - consumeAmount[i], m_pOwner->GetMaxHP());
			break;
		case MIXSHOP_CONSUME_SP:
			m_pOwner->SetSP(m_pOwner->GetSP() - consumeAmount[i], m_pOwner->GetMSP(), m_pOwner->GetMaxSP());
			break;
		case MIXSHOP_CONSUME_MSP:
			m_pOwner->SetSP(m_pOwner->GetSP(), m_pOwner->GetMSP() - consumeAmount[i], m_pOwner->GetMaxSP());
			break;
		case MIXSHOP_CONSUME_GOODNESS:
			//m_pOwner->SetGoodness(m_pOwner->GetGoodness() - consumeAmount[i]);
			break;
		case MIXSHOP_CONSUME_HONOR:
			m_pOwner->ChangeHonor(-(Int32)consumeAmount[i]);
			break;
		case MIXSHOP_CONSUME_FACTIONDONATE:
			m_pOwner->SetDonate(m_pOwner->GetFDonate() - consumeAmount[i], m_pOwner->GetTotalFDonate(), m_pOwner->GetMDonate(), m_pOwner->GetTotalMDonate());
			break;
		case MIXSHOP_CONSUME_MANORDONATE:
			m_pOwner->SetDonate(m_pOwner->GetFDonate(), m_pOwner->GetTotalFDonate(), m_pOwner->GetMDonate() - consumeAmount[i], m_pOwner->GetTotalMDonate());
			break;
		}
	}
	m_pOwner->RaiseUpdate();
}

UInt8 CItemBag::EquipRepair(UInt32Vector itemUids, UInt8Vector consumeType, UInt32Vector consumeAmount)
{
	UInt8 msgCode = MIXSHOP_SUCCESS;
	CItem* eqItem = NULL;
	typedef StlVector<CItem*, ManagedAllocator<CItem> > HelpRepairItemList;
	HelpRepairItemList itemList;
	UInt8 helpItemType = 0; //0: none, 1:have help repair item, 2:have help rebuild item
	UInt price = 0;

	for (UInt8 i = 0; i < itemUids.size(); ++i)
	{
		CItem* temp = GetItemByUID(itemUids[i]);
		if (temp && temp->m_data)
		{
			if (temp->m_data->item_type == ITEMTYPE_EQUIP)
				eqItem = temp;
			else if (temp->m_data->item_type == ITEMTYPE_REPAIR)
			{
				helpItemType = 1;
				itemList.push_back(temp);
			}
			else if (temp->m_data->item_type == ITEMTYPE_REBUILD)
			{
				helpItemType = 2;
				itemList.push_back(temp);
			}	
			else if (temp->m_data->item_type == ITEMTYPE_SCROLL)
			{
				helpItemType = 3;
				itemList.push_back(temp);
			}
		}
	}

	if (eqItem == NULL || eqItem->m_data == NULL)
		return MIXSHOP_NO_EQ;

	//Cal price
	if (helpItemType == 2)
	{
		if (eqItem->GetMEndure() == eqItem->m_data->eq_maxEndure)
			return MIXSHOP_NO_NEED_REBUILD;

		price = eqItem->m_data->g_price;
	}
	else if (helpItemType == 3)
	{
		if (eqItem->GetMEndure() == 0)
			return MIXSHOP_NO_MENDURING;
		else if (eqItem->GetMEndure() == eqItem->GetEndure())
			return MIXSHOP_NO_NEED_REPAIR;
	}
	else
	{
		if (eqItem->GetMEndure() == 0)
			return MIXSHOP_NO_MENDURING;
		else if (eqItem->GetMEndure() == eqItem->GetEndure())
			return MIXSHOP_NO_NEED_REPAIR;
			
		price = eqItem->m_data->g_price * (eqItem->GetMEndure() - eqItem->GetEndure()) / eqItem->GetMEndure();
	}

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount, price);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;
	
	if (helpItemType == 2)
	{
		UInt16 enduring = eqItem->GetMEndure() + (UInt16)(0.2f * eqItem->m_data->eq_maxEndure);
		if (enduring > eqItem->m_data->eq_maxEndure)
			enduring = eqItem->m_data->eq_maxEndure;

		eqItem->SetMEndure(enduring);	
		msgCode = MIXSHOP_SUCCESS_REBUILD;
	}
	else if (helpItemType == 3)
	{
		eqItem->SetEndure(eqItem->GetMEndure());
	}
	else
	{
		if (helpItemType == 0 && GetRand(0, 100) < 40)
		{
			UInt16 tempValue = (UInt16)(0.1f * eqItem->m_data->eq_maxEndure);
			if (tempValue > eqItem->GetMEndure())
				eqItem->SetMEndure(0);
			else
				eqItem->SetMEndure(eqItem->GetMEndure() - tempValue);
			msgCode = MIXSHOP_SUCCESS_REDUCE_MENDURING;
		}
		
		eqItem->SetEndure(eqItem->GetMEndure());
	}

	ReduceMixShopConsume(consumeType, consumeAmount, price, LogService_Proxy::Money_And_Exp::SOURCE_EQ_REPAIR);
	for (UInt8 k = 0; k < itemList.size(); ++k)
	{
		itemList[k]->SetStack(itemList[k]->GetStack() - 1);
		if (itemList[k]->GetStack() == 0)
			AddToDeleteGroup(itemList[k]->GetBagIndex());
	}
	itemList.clear();
	RaiseUpdate();
	return msgCode;
}

UInt8 CItemBag::StoneMix(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount)
{
	UInt8 msgCode = MIXSHOP_SUCCESS;

	if (itemUids.size() == 0 || stacks.size() == 0)
		return MIXSHOP_ERROR;

	if (itemUids.size() != stacks.size())
		return MIXSHOP_ERROR;

	if (GetTotalFreeSlot() == 0)
		return MIXSHOP_NO_ITEMSLOT;

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	UInt32 stoneID = 0;
	UInt totalStacks = 0;
	UInt maxStack = 0;
	UInt minStack = 0;
	UInt resultID = 0;

	CItem* tempItem = GetItemByUID(itemUids[0]);
	if (tempItem == NULL)
		return MIXSHOP_ERROR;

	for (Index i = C_INDEX(0); i < itemUids.size(); ++i)
	{
		CItem* item = GetItemByUID(itemUids[i]);
		if (item == NULL || item->GetStack() < stacks[i])
			return MIXSHOP_ERROR;		
	}

	stoneID = tempItem->GetItem_id();
	resultID = gGlobal.m_resource->GetResultStoneID(stoneID);
	if (resultID == 0)
		return MIXSHOP_NO_HIGHER_RESULT;

	maxStack = gGlobal.m_resource->GetMaxStoneMixNum(stoneID);
	minStack = gGlobal.m_resource->GetMinStoneMixNum(stoneID);

	for (Index i = C_INDEX(0); i < stacks.size(); ++i)
		totalStacks += stacks[i];

	if (maxStack == 0 || minStack == 0 || totalStacks > maxStack )
		return MIXSHOP_ERROR;

	if (!CanCreateLimitItem(resultID, 1))
		return MIXSHOP_ITEM_REACH_LIMIT;

	if (totalStacks >= minStack && totalStacks <= maxStack)
	{
		UInt prob = gGlobal.m_resource->GetStoneMixProb(stoneID, totalStacks);
		if ( prob >= (UInt)GetRand(0, 100) )
		{
			TRACE_INFOL_2(GLOBAL_LOGGER, _F("StoneMix: success to make for char_id = %d, item_id = %d"), m_pOwner->GetChar_id(), resultID);
			bool bFinishMake = false;
			CItem* pNewItem = NewItem(resultID, 1);
			if ( pNewItem )
			{
				for ( Index j = C_INDEX(BAG_CURRENT); j < BAG_TYPE_COUNT; ++j )
				{
					if (AddItem(pNewItem, j))
					{
						bFinishMake = true;
						break;
					}
				}
				if (!bFinishMake)
				{
					DeleteNetObject(pNewItem);
					if (stacks[0] > 0)
						stacks[0] = stacks[0] - 1;
				}
			}

			if (bFinishMake)
				ReduceMixShopConsume(consumeType, consumeAmount, 1, LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);
		}
		else
		{
			if (stacks[0] > 0)
				stacks[0] = stacks[0] - 1;
			msgCode = MIXSHOP_FAIL;
		}
		
		UInt32Vector logSourceItemId;
		for (Index k = C_INDEX(0); k < itemUids.size(); ++k)
		{
			CItem* item = GetItemByUID(itemUids[k]);
			if (item)
			{
				item->SetStack(item->GetStack() - stacks[k]);
				if (item->GetStack() == 0)
					AddToDeleteGroup(item->GetBagIndex());
				logSourceItemId.push_back(item->GetItem_id());
			}
		}
		if (msgCode == MIXSHOP_FAIL)
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
						resultID, 0, itemUids, logSourceItemId, stacks);
		}
		else
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
						resultID, 1, itemUids, logSourceItemId, stacks);
		}
		RaiseUpdate();
	}
	return msgCode;
}

Boolean CItemBag::CanCreateLimitItem(UInt32 itemID, UInt16 stack)
{
	const ItemData* pItemData = gGlobal.m_resource->GetItemData(itemID);
	if (pItemData == NULL)
		return BOOLEAN_FALSE;

	if (!pItemData->item_GenerateLimit)
		return BOOLEAN_TRUE;

	if (stack > pItemData->item_GenerateLimit_Count)
		return BOOLEAN_FALSE;

	ItemLimitMap::iterator itr = gGlobal.m_itemLimitMap.find(itemID);
	if (itr != gGlobal.m_itemLimitMap.end() )
	{
		time_t nowTime;
		time(&nowTime);
		if (itr->second.endTime > (UInt32)nowTime && (itr->second.stack + stack) > pItemData->item_GenerateLimit_Count)
			return BOOLEAN_FALSE;

		TRACE_INFOL_4(GLOBAL_LOGGER, _F("CanCreateLimitItem: endTime = %d, nowTime = %d, createdStack = %d, limit_count = %d"), 
			itr->second.endTime, nowTime, itr->second.stack, pItemData->item_GenerateLimit_Count);

	}
	return BOOLEAN_TRUE;
}

void CItemBag::RemoveDummyItem(UInt noOfDummyItem)
{
	for (Index i = C_INDEX(BAG_CURRENT); i < m_items.size(); ++i)
	{
		if (m_items[i] == m_dummyItem->GetNetId() )
		{
			m_items[i] = RPCNetID();
			--noOfDummyItem;

			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("owner_id %d, owner session_id, %s, bagIndex %d"), 
				m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), i);
		}
		if (noOfDummyItem == 0)
			break;
	}
}

bool CItemBag::SwapFromDummy(CItem* item)
{
	for (Index i = C_INDEX(BAG_CURRENT); i < m_items.size(); ++i)
	{
		if (m_items[i] == m_dummyItem->GetNetId())
		{
			m_items[i] = item->GetNetId();
			item->SetOwner_id(GetOwner_id());
			item->SetBagIndex(i);
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("owner_id %d, owner session_id, %s, item_id %d, bagIndex %d"), 
				m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str(), item->GetItem_id(), i);
			return true;
		}
	}
	return false;
}

void CItemBag::ChangeOwnerID(const UInt32Vector &itemUIDs, const UInt32Vector &itemIDs, UInt32 targetOwnerID)
{
	PROCEDURE_DO_METHOD3(CItemBag, ChangeOwnerID, this, m_pOwner->GetContext(),
				UInt32Vector, itemUIDs, UInt32Vector, itemIDs, UInt32, targetOwnerID)
}

RPCResult CItemBag::ChangeOwnerID(RPCContext &context)
{
	PROCEDURE_START_METHOD3(CItemBag, ChangeOwnerID, this, context, UInt32Vector, itemUIDs, UInt32Vector, itemIDs, UInt32, targetOwnerID)

		NetGroup grp;

		for (Index i = C_INDEX(0); i < itemUIDs.size(); ++i)
		{
			CItemData* pItemData = CreateNetObject<CItemData>();
			TRACE_ENSURE(pItemData);
			pItemData->SetId(itemUIDs[i], itemIDs[i]);
			pItemData->SetOwner_id(targetOwnerID);	
			pItemData->SetSendMask(pItemData->GetUpdateMask());
			grp.Add(pItemData);

			TRACE_INFOL_4(GLOBAL_LOGGER, _F("item_uid %d item_id %d change from owner_id %d to target_id %d"), itemUIDs[i], itemIDs[i], m_pOwner->GetChar_id(), targetOwnerID);
		}

		ObjectService_Proxy::UpdateItemList(gGlobal.pObjectService->pPeer, context, 
			itemUIDs, m_pOwner->GetChar_id(), &grp, BOOLEAN_TRUE);
		
		DeleteNetChilds(&grp);

	PROCEDURE_WAIT1(1, ObjectService, CB_UpdateItemList, NetGroup*, newItems)

		DeleteNetGroup(newItems, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CPetGroup ChangeOwner Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

Boolean CItemBag::ChangeItemLockStatus(const UInt32 &bagIndex, const Boolean &lock, UInt8 &msg)
{
	CItem* pitem = NULL;
	pitem = GetItem(bagIndex);
	if (!pitem)
	{
		msg = ASSET_INVALID_ERR;
		return BOOLEAN_FALSE;
	}
	if (pitem->GetLock() == lock)
	{
		if (lock)
			msg = ASSET_LOCK_ALREADY;	
		else
			msg = ASSET_UNLOCK_ALREADY;	
			
		return BOOLEAN_FALSE;
	}
	pitem->SetLock(lock);
	RaiseUpdate();
	msg = lock? ASSET_LOCK_SUCCESS : ASSET_UNLOCK_SUCCESS;	

	return BOOLEAN_TRUE;
}


void CItemBag::CheckIdentifyItemExist(CItem* item)
{
	CItem* identifyItem = NULL;
	for (Index i = C_INDEX(0); i < BAG_ITEM_COUNT; ++i)
	{
		CItem* tempItem = GetItem(i);
		if (tempItem == NULL || tempItem->m_data == NULL)
			continue;

		if (tempItem->GetLock())
			continue;	

		if (tempItem->m_data->item_type != ITEMTYPE_IDENTIFY)
			continue;

		if (item->m_data->reqLv > tempItem->m_data->reqLv)
			continue;

		bool notSuitable = true;
		switch(tempItem->m_data->eqType)
		{
		case EQTYPE_ALL_WEAPON:
			{
				switch (item->m_data->eqType)
				{
				case EQTYPE_SPEAR:
				case EQTYPE_WIPE:
				case EQTYPE_FAN:
				case EQTYPE_PEN:
				case EQTYPE_SWORD:
				case EQTYPE_DBSWORD:
				case EQTYPE_BLADE:
				case EQTYPE_OTHER:
				case EQTYPE_HANDRING:
				case EQTYPE_CLAW:
					notSuitable = false;
				}
			}
			break;
		case EQTYPE_ALL_CLOTH:
			{
				switch (item->m_data->eqType)
				{
				case EQTYPE_GEAR:
				case EQTYPE_ARMOR:
				case EQTYPE_SHOE:
				case EQTYPE_CLOTH:
					notSuitable = false;
				}
			}
			break;
		case EQTYPE_ALL_ORNAMENT:
			{
				switch (item->m_data->eqType)
				{
				case EQTYPE_BELT:
				case EQTYPE_RING:				
				case EQTYPE_HAT_WITH_HAIR:
				case EQTYPE_HAT_WITHOUT_HAIR:
					notSuitable = false;
				}
			}
			break;
		case EQTYPE_AMULET:
			{
				if (item->m_data->eqType == EQTYPE_AMULET)
					notSuitable = false;
			}
			break;
		}

		if (notSuitable)
			continue;

		if (identifyItem && identifyItem->m_data)
		{
			if (identifyItem->m_data->reqLv > tempItem->m_data->reqLv)
				identifyItem = tempItem;
		}
		else						
			identifyItem = tempItem;			
	}
	if (identifyItem)
		CB_IdentifyItem(m_pOwner->GetPeer(), m_pOwner->GetContext(), IDENTIFY_CAN_IDENTIFY, item->GetBagIndex(), identifyItem->GetBagIndex());
	else
		CB_IdentifyItem(m_pOwner->GetPeer(), m_pOwner->GetContext(), IDENTIFY_NO_IDENTIFY_ITEM, item->GetBagIndex(), 0);
}

void CItemBag::SetWareHouseItem(NetGroup* grp)
{
	m_wareHouseItems.clear();
	RPCNetID empty;
	for (Index i = C_INDEX(0); i < BAG_MAX_WAREHOUSE_ITEM; ++i)
		m_wareHouseItems.push_back(empty);
	
	if (grp != NULL)
	{
		for (Index i = C_INDEX(0); i < grp->GetCount(); i++)
		{
			CItemData *pItemData = grp->GetChild<CItemData>(i);
			if (pItemData && pItemData->GetStack() > 0) 
			{				// not null or empty item
				UInt8 bagIndex = pItemData->GetBagIndex();
				CItem* pItem = GetItemFromWareHouse(bagIndex);
				// strange item exist in bag index
				if (pItem == NULL) 
				{									// empty index
					pItem = CItem::NewInstance();
					m_wareHouseItems[bagIndex] = pItem->GetNetId();
				} 
				else if (pItem->GetItem_uid() == pItemData->GetItem_uid()) 
				{	// skip same item
					continue;
				}
				pItem->SetData(pItemData, false);
				pItem->SetSendMask(pItem->GetUpdateMask());
				pItem->ClearUpdateMask();
			}
		}
	}
}

CItem* CItemBag::GetItemFromWareHouse(UInt bagIndex)
{
	if (bagIndex >= BAG_MAX_WAREHOUSE_ITEM || bagIndex >= m_wareHouseItems.size() || m_wareHouseItems[bagIndex] == RPCNetID())
		return NULL;

	return FindNetObject<CItem>(m_wareHouseItems[bagIndex]);
}

void CItemBag::GetAllWareHouse(NetGroup* grp)
{
	if (grp != NULL)
	{
		CItem* item = NULL;
		for (Index i = C_INDEX(0); i < BAG_MAX_WAREHOUSE_ITEM; ++i)
		{
			item = GetItemFromWareHouse(i);
			if (item)
			{
				item->SetSendMask(CItem::AllMask);
				grp->Add(item);
			}
		}
	}
}

void CItemBag::ClearWareHouse()
{
	CItem* item = NULL;
	for (Index i = C_INDEX(0); i < BAG_MAX_WAREHOUSE_ITEM; ++i)
	{
		item = GetItemFromWareHouse(i);
		if (item)
			DeleteNetObject(item);
	}
	m_wareHouseItems.clear();
}

RPCResult CItemBag::PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &item_uid, const UInt8 &stack, const UInt8 &desIndex)
{
	UInt8 msgIndex = WAREHOUSE_SUCCESS;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			msgIndex = WAREHOUSE_NOT_IN_MAP;
			break;
		}

		// cannot move item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			msgIndex = WAREHOUSE_RUNNING_SCIRPT;
			break;
		}

		if (m_pOwner->IsTrading())
		{
			msgIndex = WAREHOUSE_CANNOT_TRADING;
			break;
		}

		CItem* item = GetItemByUID(item_uid);
		if (item == NULL || item->m_data == NULL || !item->IsExist() || m_wareHouseItems.size() == 0)
		{
			msgIndex = WAREHOUSE_NO_ITEM;
			break;
		}

		// cannot move script item into other bags
		if (item->m_data->item_type == ITEMTYPE_SCRIPT || item->m_data->item_type == ITEMTYPE_BUSINESS_ITEM || item->m_data->item_type == ITEMTYPE_BUSINESS_MONEY)
		{
			msgIndex = WAREHOUSE_QUEST_ITEM;
			break;
		}

		UInt realDesIndex = desIndex;
		UInt realStack = stack;

		if (realDesIndex == 255) //desIndex == 255, that mean auto find a suitable place
		{
			realStack = item->GetStack(); 
			CItem* tempItem = NULL;
			for (UInt i = 0; i < (UInt)(m_pOwner->GetAvailItemWareHouse() * BAG_ITEM_COUNT); ++i)
			{
				tempItem = GetItemFromWareHouse(i);
				if (tempItem == NULL)
				{
					if (realDesIndex == 255)
						realDesIndex = i;
				}
				else if (tempItem->m_data && tempItem->GetItem_id() == item->GetItem_id() && 
					tempItem->GetStack() < tempItem->m_data->overlap)
				{
					realDesIndex = i;
					break;
				}					
			}
		}

		if (realDesIndex == 255)
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if ( realDesIndex >= (UInt)(m_pOwner->GetAvailItemWareHouse() * BAG_ITEM_COUNT))
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if (realStack > item->GetStack())
		{
			msgIndex = WAREHOUSE_ITEM_STACK_WRONG;
			break;
		}
		
		TRACE_INFOL_5(GLOBAL_LOGGER, _F("item_uid %d stack %d move to wareHouse index %d, owner_id %d, session_id %s "), 
			item_uid, stack, desIndex, m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());

		CItem* desItem = GetItemFromWareHouse(realDesIndex);

		//if the desIndex no item then put srcItem to the empty index
		if (desItem == NULL)	
		{
			if (item->GetStack() == realStack)
			{
				RemoveItem(item, false);
				item->SetInWareHouse(1);
				item->SetBagIndex(realDesIndex);
				m_wareHouseItems[realDesIndex] = item->GetNetId();
			}
			else
			{
				CItem* newItem = NewItem(item->GetItem_id(), realStack, false);
				if (newItem)
				{
					newItem->SetInWareHouse(1);
					newItem->SetOwner_id(m_pOwner->GetChar_id());
					newItem->SetBagIndex(realDesIndex);
					newItem->SetLock(item->GetLock());
					newItem->SetCreateType(CItemBag::CREATE_BY_WAREHOUSE);
					m_wareHouseItems[realDesIndex] = newItem->GetNetId();
					item->SetStack(item->GetStack() - realStack);
				}
			}
		}
		else
		{
			if (desItem->m_data && (desItem->m_data->item_type == ITEMTYPE_BUSINESS_ITEM || desItem->m_data->item_type == ITEMTYPE_BUSINESS_MONEY))
			{
				msgIndex = WAREHOUSE_QUEST_ITEM;
				break;
			}
			if (desItem->GetItem_id() != item->GetItem_id() || item->m_data->overlap == 1)
			{	
				//swap item from warehouse to item bag
				UInt8 itemBagIndex = item->GetBagIndex();
				UInt8 wareHouseIndex = desItem->GetBagIndex();
				
				RemoveItem(item, false);
				item->SetInWareHouse(1);
				item->SetBagIndex(wareHouseIndex);
				m_wareHouseItems[wareHouseIndex] = item->GetNetId();

				desItem->SetInWareHouse(0);
				desItem->SetBagIndex(itemBagIndex);
				m_items[itemBagIndex] = desItem->GetNetId();
			}
			else
			{			
				bool keepDoing = false;
				UInt wareHouseStartIndex = 0;
				UInt emptyIndex = 255;
				do
				{
					keepDoing = false;
					UInt avail = desItem->m_data->overlap - desItem->GetStack();
					if (avail > realStack)
						avail = realStack;	
					desItem->SetStack(desItem->GetStack() + avail);
					desItem->SetLock((desItem->GetLock()||item->GetLock()));
					item->SetStack(item->GetStack() - avail);
					realStack -= avail;
					if (item->GetStack() == 0)
					{
						AddToDeleteGroup(item->GetBagIndex());
						item = NULL;
						realStack = 0;
					}						
					else if (realStack > 0)
					{
						CItem* tempItem = NULL;
						for (UInt i = wareHouseStartIndex; i < (UInt)(m_pOwner->GetAvailItemWareHouse() * BAG_ITEM_COUNT); ++i)
						{
							tempItem = GetItemFromWareHouse(i);
							if (tempItem && tempItem->GetItem_id() == item->GetItem_id() && tempItem->m_data && 
								tempItem->GetStack() < tempItem->m_data->overlap)
							{
								desItem = tempItem;
								wareHouseStartIndex = i;
								keepDoing = true;
								break;
							}
							else if (tempItem == NULL && emptyIndex == 255)
								emptyIndex = i;
						}
					}
				}
				while(keepDoing);

				if (realStack > 0 && item && item->GetStack() > 0 && emptyIndex < (UInt)(m_pOwner->GetAvailItemWareHouse() * BAG_ITEM_COUNT))
				{
					if (realStack >= item->GetStack())
					{
						RemoveItem(item, false);
						item->SetInWareHouse(1);
						item->SetBagIndex(emptyIndex);					
						m_wareHouseItems[emptyIndex] = item->GetNetId();
					}
					else
					{
						CItem* newItem = NewItem(item->GetItem_id(), realStack, false);
						if (newItem)
						{
							newItem->SetInWareHouse(1);
							newItem->SetOwner_id(m_pOwner->GetChar_id());
							newItem->SetBagIndex(emptyIndex);
							newItem->SetCreateType(CItemBag::CREATE_BY_WAREHOUSE);
							m_wareHouseItems[emptyIndex] = newItem->GetNetId();
							item->SetStack(item->GetStack() - realStack);
						}
					}
				}					
			}		
		}
	} while(false);

	if (msgIndex == WAREHOUSE_SUCCESS)
	{
		m_bWareHouseUpdated = BOOLEAN_TRUE;
		RaiseUpdate();
	}

	CB_WareHouse(pPeer, context, WAREHOUSE_PROCESS_PUT_ITEM, msgIndex);

	return RPC_RESULT_OK;

}

RPCResult CItemBag::GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &item_uid, const UInt8 &stack, const UInt8 &desIndex)
{
	UInt msgIndex = WAREHOUSE_SUCCESS;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			msgIndex = WAREHOUSE_NOT_IN_MAP;
			break;
		}

		// cannot move item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			msgIndex = WAREHOUSE_RUNNING_SCIRPT;
			break;
		}

		if (m_pOwner->IsTrading())
		{
			msgIndex = WAREHOUSE_CANNOT_TRADING;
			break;
		}

		CItem* item = NULL;

		for(UInt i = 0; i < (UInt)(m_pOwner->GetAvailItemWareHouse() * BAG_ITEM_COUNT); ++i)
		{
			CItem* tempItem = GetItemFromWareHouse(i);
			if (tempItem && tempItem->GetItem_uid() == item_uid)
			{
				item = tempItem;
				break;
			}
		}

		if (item == NULL || item->m_data == NULL || !item->IsExist() || m_wareHouseItems.size() == 0)
		{
			msgIndex = WAREHOUSE_NO_ITEM;
			break;
		}

		UInt realDesIndex = desIndex;
		UInt realStack = stack;

		if (realDesIndex == 255)
		{
			realStack = item->GetStack();
			CItem* tempItem = NULL;
			for (UInt i = 0; i < BAG_ITEM_COUNT; ++i)
			{
				tempItem = GetItem(i);
				if (tempItem == NULL)
				{
					if (realDesIndex == 255)
						realDesIndex = i;
				}
				else if (tempItem->m_data && tempItem->GetItem_id() == item->GetItem_id() && 
					tempItem->GetStack() < tempItem->m_data->overlap)
				{
					realDesIndex = i;
					break;
				}					
			}
		}

		if (realDesIndex == 255)
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if ( realDesIndex >= (UInt)BAG_ITEM_COUNT)
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if (realStack > item->GetStack())
		{
			msgIndex = WAREHOUSE_ITEM_STACK_WRONG;
			break;
		}
		
		TRACE_INFOL_5(GLOBAL_LOGGER, _F("item_uid %d stack %d move to itemBag index %d, owner_id %d, session_id %s "), 
			item_uid, stack, desIndex, m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());
		
		//if the desIndex no item then put srcItem to the empty index
		if (IsSlotFree(realDesIndex))
		{
			if (item->GetStack() == realStack)
			{
				m_wareHouseItems[item->GetBagIndex()] = RPCNetID();
				item->SetInWareHouse(0);
				item->SetBagIndex(realDesIndex);
				m_items[realDesIndex] = item->GetNetId();
				
			}
			else
			{
				CItem* newItem = NewItem(item->GetItem_id(), realStack, false);
				if (newItem)
				{
					newItem->SetInWareHouse(0);
					newItem->SetOwner_id(m_pOwner->GetChar_id());
					newItem->SetBagIndex(realDesIndex);
					newItem->SetLock(item->GetLock());
					newItem->SetCreateType(CItemBag::CREATE_BY_WAREHOUSE);
					m_items[realDesIndex] = newItem->GetNetId();
					item->SetStack(item->GetStack() - realStack);
				}
			}
		}
		else
		{
			CItem* desItem = GetItem(realDesIndex);
			if (desItem && desItem->m_data && (desItem->m_data->item_type == ITEMTYPE_BUSINESS_ITEM || desItem->m_data->item_type == ITEMTYPE_BUSINESS_MONEY))
			{
				msgIndex = WAREHOUSE_QUEST_ITEM;
				break;
			}
			if (desItem && (desItem->GetItem_id() != item->GetItem_id() || item->m_data->overlap == 1))
			{	
				//swap item from warehouse to item bag
				UInt8 itemBagIndex = desItem->GetBagIndex();
				UInt8 wareHouseIndex = item->GetBagIndex();
				
				RemoveItem(desItem, false);
				desItem->SetInWareHouse(1);
				desItem->SetBagIndex(wareHouseIndex);
				m_wareHouseItems[wareHouseIndex] = desItem->GetNetId();

				item->SetInWareHouse(0);
				item->SetBagIndex(itemBagIndex);
				m_items[itemBagIndex] = item->GetNetId();
			}
			else if (desItem)
			{
				bool keepDoing = false;
				UInt startIndex = 0;
				UInt emptyIndex = 255;
				do
				{
					keepDoing = false;
					UInt avail = desItem->m_data->overlap - desItem->GetStack();
					if (avail > realStack)
						avail = realStack;	
					desItem->SetStack(desItem->GetStack() + avail);
					desItem->SetLock((desItem->GetLock()||item->GetLock()));
					item->SetStack(item->GetStack() - avail);
					realStack -= avail;
					if (item->GetStack() == 0)
					{
						AddToDeleteGroup(item->GetBagIndex(), item->GetInWareHouse());
						item = NULL;
						realStack = 0;
					}						
					else if (realStack > 0)
					{
						CItem* tempItem = NULL;
						for (UInt i = startIndex; i < (UInt)BAG_ITEM_COUNT; ++i)
						{
							tempItem = GetItem(i);
							if (tempItem && tempItem->GetItem_id() == item->GetItem_id() && tempItem->m_data && 
								tempItem->GetStack() < tempItem->m_data->overlap)
							{
								desItem = tempItem;
								startIndex = i;
								keepDoing = true;
								break;
							}
							else if (tempItem == NULL && emptyIndex == 255)
								emptyIndex = i;
						}
					}
				}
				while(keepDoing);

				if (realStack > 0 && item && item->GetStack() > 0 && emptyIndex < (UInt)BAG_ITEM_COUNT)
				{
					if (realStack >= item->GetStack())
					{
						m_wareHouseItems[item->GetBagIndex()] = RPCNetID();
						item->SetInWareHouse(0);
						item->SetBagIndex(emptyIndex);
						m_items[emptyIndex] = item->GetNetId();
					}
					else
					{
						CItem* newItem = NewItem(item->GetItem_id(), realStack, false);
						if (newItem)
						{
							newItem->SetInWareHouse(0);
							newItem->SetOwner_id(m_pOwner->GetChar_id());
							newItem->SetBagIndex(emptyIndex);
							newItem->SetCreateType(CItemBag::CREATE_BY_WAREHOUSE);
							m_items[emptyIndex] = newItem->GetNetId();
							item->SetStack(item->GetStack() - realStack);
						}
					}
				}			
			}
		}
	}
	while (false);

	if (msgIndex == WAREHOUSE_SUCCESS)
	{
		m_bWareHouseUpdated = BOOLEAN_TRUE;
		RaiseUpdate();
	}

	CB_WareHouse(pPeer, context, WAREHOUSE_PROCESS_GET_ITEM, msgIndex);

	return RPC_RESULT_OK;
}

RPCResult CItemBag::WareHouseItemMove(LPCPEER pPeer, RPCContext &context, const UInt8 &srcIndex, const UInt8 &desIndex)
{
	UInt msgIndex = WAREHOUSE_MOVE_SUCCESS;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			msgIndex = WAREHOUSE_NOT_IN_MAP;
			break;
		}

		// cannot move item when the script is running
		if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
		{
			msgIndex = WAREHOUSE_RUNNING_SCIRPT;
			break;
		}

		if ( desIndex >= (UInt)(m_pOwner->GetAvailItemWareHouse() * BAG_ITEM_COUNT))
		{
			msgIndex = WAREHOUSE_FULL;
			break;
		}

		if (m_pOwner->IsTrading())
		{
			msgIndex = WAREHOUSE_CANNOT_TRADING;
			break;
		}

		CItem* srcItem = GetItemFromWareHouse(srcIndex);
		if (srcItem == NULL)
		{
			msgIndex = WAREHOUSE_NO_ITEM;
			break;
		}

		bool swapItem = true;
			//if there are desItem and can stack then it will ony stack and will not move
		if (srcItem->m_data->overlap > 1)
		{
			CItem* dstItem = GetItemFromWareHouse(desIndex);
			if (dstItem && dstItem->IsExist() && dstItem->GetItem_id() == srcItem->GetItem_id())
			{
				UInt8 avail = dstItem->m_data->overlap - dstItem->GetStack();
				if (avail > 0)
				{
					// item stack will change
					if (srcItem->GetStack() <= avail)
					{
						// fully merged
						dstItem->SetStack(dstItem->GetStack() + srcItem->GetStack());
						srcItem->SetStack(0);							// source item change to non-exist
					}
					else
					{
						// partial merged
						dstItem->SetStack(dstItem->m_data->overlap);
						srcItem->SetStack(srcItem->GetStack() - avail);
					}
					dstItem->SetLock((dstItem->GetLock() || srcItem->GetLock())); // set the destination item to lock status if either destItem or srcItem is locked 
					swapItem = false;
				}					
			}
		}					

		if (swapItem)
		{
			CItem* desItem = GetItemFromWareHouse(desIndex);
			if (desItem)
			{
				desItem->SetBagIndex(srcIndex);
				m_wareHouseItems[srcIndex] = desItem->GetNetId();
			}
			else
				m_wareHouseItems[srcIndex] = RPCNetID();
			
			srcItem->SetBagIndex(desIndex);
			m_wareHouseItems[desIndex] = srcItem->GetNetId();
		}

		if (srcItem->GetStack() == 0)
			AddToDeleteGroup(srcItem->GetBagIndex(), srcItem->GetInWareHouse());

	}
	while(false);

	if (msgIndex == WAREHOUSE_MOVE_SUCCESS)
	{
		m_bWareHouseUpdated = BOOLEAN_TRUE;
		RaiseUpdate();
	}

	CB_WareHouse(pPeer, context, WAREHOUSE_PROCESS_MOVE_ITEM, msgIndex);

	return RPC_RESULT_OK;
}

void CItemBag::ActivateEquipedSkill()
{
	for (UInt8 ei=EQPLACE_WEAPON + BAG_MAX_ITEM; ei <= EQPLACE_ANY + BAG_MAX_ITEM; ei++)
	{
		CItem *item = GetItem(ei);
		if (item && item->GetSkillID() > 0 && item->GetEndure() > 0)
		{
			m_pOwner->AddSkillTemp(item->GetSkillID());
		}
	}
}

UInt8 CItemBag::UnSoulBound(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount, const MixShopData* pShop)
{
	if (itemUids.size() != stacks.size())
		return MIXSHOP_ERROR;

	UInt8 msgCode = MIXSHOP_SUCCESS;
	CItem* eqItem = NULL;
	typedef StlVector<CItem*, ManagedAllocator<CItem> > HelpRepairItemList;
	HelpRepairItemList itemList;
	UInt8 helpItemType = 0; //0: none, 1:have help repair item, 2:have help rebuild item
	UInt price = 0;

	UInt8 count(0);
	UInt32 unSoulBoundItemId(0);
	for (UInt8 i = 0; i < itemUids.size(); ++i)
	{
		CItem* temp = GetItemByUID(itemUids[i]);
		if (temp == NULL || temp->GetStack() < stacks[i])
			return MIXSHOP_ERROR;
		if (temp && temp->m_data)
		{
			if (temp->m_data->item_type == ITEMTYPE_EQUIP)
			{
				eqItem = temp;
				CallLuaFunc(gGlobal.g_pLuaState, "GetUnSoulBoundStoneID", String(), eqItem->m_data->rare);
				unSoulBoundItemId = (UInt32)gGlobal.g_TempResult;
			}
			else if (temp->m_data->item_type == ITEMTYPE_UNSOULBOUND)
			{
				if (unSoulBoundItemId != temp->m_data->item_id)
				{
					return MIXSHOP_ERROR;
				}
				count += stacks[i];
				itemList.push_back(temp);
			}				
		}
	}

	if (eqItem == NULL || eqItem->m_data == NULL)
		return MIXSHOP_NO_EQ;


	{
		float lv = (float)eqItem->m_data->reqLv;
		CallLuaFunc(gGlobal.g_pLuaState, "GetUnSoulBoundStoneNum", String(), lv);
		UInt8 needCount = (UInt8)gGlobal.g_TempResult;
		if (count != needCount || needCount < 1)
		{
			return MIXSHOP_ERROR;
		}
	}


	{
		if (eqItem->GetEq_gem() > 0 && 
			(eqItem->GetInlay_gem(0) > 0 ||
			eqItem->GetInlay_gem(1) > 0 ||
			eqItem->GetInlay_gem(2) > 0 ||
			eqItem->GetInlay_gem(3) > 0 ||
			eqItem->GetInlay_gem(4) > 0 ||
			eqItem->GetInlay_gem(5) > 0 ||
			eqItem->GetInlay_gem(6) > 0 ||
			eqItem->GetInlay_gem(7) > 0 ||
			eqItem->GetInlay_gem(8) > 0 ||
			eqItem->GetInlay_gem(9) > 0)
			)
		{
			return MIXSHOP_ERROR;
		}
	}

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount, price);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	TRACE_INFOL_3(GLOBAL_LOGGER, _F("UnSoulBound item_uid %d, owner_id %d, session_id %s "), 
		eqItem->GetItem_uid(), m_pOwner->GetChar_id(), m_pOwner->GetSessionId().GetString().c_str());

	eqItem->SetItem_soulboundState(SOULBOUND_EQ_BOUND);

	ReduceMixShopConsume(consumeType, consumeAmount, price, LogService_Proxy::Money_And_Exp::SOURCE_EQ_REPAIR);
	for (Index k = C_INDEX(0); k < itemList.size() && (k+1) < stacks.size(); ++k)
	{
		itemList[k]->SetStack(itemList[k]->GetStack() - stacks[k+1]);
		if (itemList[k]->GetStack() == 0)
			AddToDeleteGroup(itemList[k]->GetBagIndex());
	}
	itemList.clear();
	RaiseUpdate();
	return msgCode;
}


// ItemSet
void CItemBag::UpdateItemSet()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag::UpdateItemSet(), char_id %d"), m_pOwner->GetChar_id());

	CItemBag::EquipItemSetCountMap tmpEquipItemSetCountMap;
	tmpEquipItemSetCountMap.clear();

	for (UInt i = BAG_MAX_ITEM; i < BAG_MAX_ITEM + BAG_EQUIP_COUNT; i++)
	{
		CItem *item = GetItem(i);
		if (item && item->GetEndure() > 0 && item->m_data)
		{
			UInt32 itemset_id = item->m_data->itemset_id; // if item has itemset ...
			if (itemset_id > 0)
			{
				CItemBag::EquipItemSetCountMap::iterator itr = tmpEquipItemSetCountMap.find(itemset_id);
				if (itr == tmpEquipItemSetCountMap.end())
				{
					tmpEquipItemSetCountMap.insert(std::make_pair(itemset_id, 1));
				}
				else
				{
					(itr)->second++;
				}
			}
		}
	}

	if (tmpEquipItemSetCountMap.size() > 0)
	{
		CItemBag::EquipItemSetCountMap::iterator itr = tmpEquipItemSetCountMap.begin();
		while (itr != tmpEquipItemSetCountMap.end())
		{
			UInt32 itemset_id = itr->first;
			UInt8 count = itr->second;

			TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" itemset_id: %d, count: %d"), itemset_id, count);

			const ItemSetData * pItemSetData = gGlobal.m_resource->GetItemSetData(itemset_id);
			if (pItemSetData)
			{
				if (count >= pItemSetData->count)
				{
					if (m_EquipItemSetMap.find(itemset_id) == m_EquipItemSetMap.end())
					{
						// add temp skill ...
						for (Index i = 0; i < pItemSetData->skill_ids.size(); i++)
						{
							m_pOwner->AddSkillTemp(pItemSetData->skill_ids[i]);
						}

						m_EquipItemSetMap.insert(std::make_pair(itemset_id, pItemSetData));
					}
				}
				else
				{
					CItemBag::ItemSetDataMap::iterator itr_itemset = m_EquipItemSetMap.find(itemset_id);
					if (itr_itemset != m_EquipItemSetMap.end())
					{
						// remove temp skill ...
						for (Index i = 0; i < pItemSetData->skill_ids.size(); i++)
						{
							m_pOwner->RemoveSkillTemp(pItemSetData->skill_ids[i]);
						}

						m_EquipItemSetMap.erase(itr_itemset);
					}
				}
			}
			else
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F(" can not get item set data."));
			}

			itr++;
		}
	}

	if (m_EquipItemSetMap.size())
	{
		CItemBag::ItemSetDataMap::iterator itr = m_EquipItemSetMap.begin();
		while (itr != m_EquipItemSetMap.end())
		{
			UInt32 itemset_id = itr->first;

			if (tmpEquipItemSetCountMap.find(itemset_id) == tmpEquipItemSetCountMap.end())
			{
				// remove temp skill ...
				const ItemSetData* pItemSetData = itr->second;
				for (Index i = 0; i < pItemSetData->skill_ids.size(); i++)
				{
					m_pOwner->RemoveSkillTemp(pItemSetData->skill_ids[i]);
				}

				itr = m_EquipItemSetMap.erase(itr);
			}
			else
				itr++;
		}
	}
}

void CItemBag::CalcItemSetBasicAttrib(Int16& sta, Int16& spr, Int16& str, Int16& con, Int16& agi)
{
	// increate STA, SPR, STR, CON, AGI by ItemSet
	if (m_EquipItemSetMap.size() > 0)
	{
		CItemBag::ItemSetDataMap::iterator itr = m_EquipItemSetMap.begin();
		while (itr != m_EquipItemSetMap.end())
		{
			const ItemSetData* pItemSetData = (itr)->second;
			if (pItemSetData)
			{
				sta += pItemSetData->STA;
				spr += pItemSetData->SPR;
				str += pItemSetData->STR;
				con += pItemSetData->CON;
				agi += pItemSetData->AGI;
			}

			itr++;
		}
	}
}

void CItemBag::CalcItemSetBatAttrib(float& atk, float& def, float& hit, float& eva, float& pow, float& spd, UInt16& eq_maxHp, UInt16& eq_maxSp)
{
	if (m_EquipItemSetMap.size() > 0)
	{
		CItemBag::ItemSetDataMap::iterator itr = m_EquipItemSetMap.begin();
		while (itr != m_EquipItemSetMap.end())
		{
			const ItemSetData* pItemSetData = (itr)->second;
			if (pItemSetData)
			{
				atk += pItemSetData->ATK;
				def += pItemSetData->DEF;
				hit += pItemSetData->HIT;
				eva += pItemSetData->EVA;
				pow += pItemSetData->POW;
				spd += pItemSetData->SPD;

				eq_maxHp += pItemSetData->MaxHP;
				eq_maxSp += pItemSetData->MaxSP;

				UInt8 bat_attrib = pItemSetData->bat_attrib;
				float ratio = ((float) pItemSetData->ratio + 100.0f) / 100.0f;
				if (bat_attrib > 0 && ratio > 1.0f)
				{
					switch(bat_attrib)
					{
					case 1:	atk *= ratio;	break;
					case 2:	def *= ratio;	break;
					//case 3:	eq_maxHp = UInt16(float(maxHp) * ratio);	break;
					//case 4:	eq_maxSp = UInt16(float(maxSp) * ratio);	break;
					case 5:	pow *= ratio;	break;
					case 6:	spd *= ratio;	break;
					}
				}
			}

			itr++;
		}
	}
}

void CItemBag::CalcItemSetBatAttribMaxHpSp(float& maxHp, float& maxSp)
{
	if (m_EquipItemSetMap.size() > 0)
	{
		CItemBag::ItemSetDataMap::iterator itr = m_EquipItemSetMap.begin();
		while (itr != m_EquipItemSetMap.end())
		{
			const ItemSetData* pItemSetData = (itr)->second;
			if (pItemSetData)
			{
				UInt8 bat_attrib = pItemSetData->bat_attrib;
				float ratio = ((float) pItemSetData->ratio + 100.0f) / 100.0f;
				if (bat_attrib > 0 && ratio > 1.0f)
				{
					switch(bat_attrib)
					{
					//case 1:	atk *= ratio;	break;
					//case 2:	def *= ratio;	break;
					case 3:	maxHp *= ratio;	break;
					case 4:	maxSp *= ratio;	break;
					//case 5:	pow *= ratio;	break;
					//case 6:	spd *= ratio;	break;
					}
				}
			}

			itr++;
		}
	}
}

UInt8 CItemBag::GemMix(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag::GemMix(), char_id %d"), m_pOwner->GetChar_id());

	UInt8 msgCode = MIXSHOP_SUCCESS;

	if (itemUids.size() == 0 || stacks.size() == 0)
		return MIXSHOP_ERROR;

	if (GetTotalFreeSlot() == 0)
		return MIXSHOP_NO_ITEMSLOT;

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	UInt32 gemID = 0;
	UInt totalStacks = 0;
	UInt maxStack = 0;
	UInt minStack = 0;
	UInt resultID = 0;

	CItem* tempItem = GetItemByUID(itemUids[0]);
	if (tempItem == NULL)
		return MIXSHOP_ERROR;
	gemID = tempItem->GetItem_id();

	UInt32 GemMixFirstItemId = 0;

	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, "CheckGemMixItemId", String(), GemMixFirstItemId, tempItem->GetItem_id(), tempItem->GetLock());
	UInt Result = (UInt32)gGlobal.g_TempResult;
	if (Result != 0)
		return MIXSHOP_ERROR;
	GemMixFirstItemId = tempItem->GetItem_id();

	for (Index i = C_INDEX(0); i < itemUids.size(); ++i)
	{
		CItem* item = GetItemByUID(itemUids[i]);
		if (item == NULL || item->GetStack() < stacks[i])
			return MIXSHOP_ERROR;
		if (item->GetItem_id() > gemID) // find bound item id
			gemID = item->GetItem_id();

		gGlobal.g_TempResult = 1;
		CallLuaFunc(gGlobal.g_pLuaState, "CheckGemMixItemId", String(), GemMixFirstItemId, item->GetItem_id(), item->GetLock());
		UInt Result = (UInt)gGlobal.g_TempResult;
		if (Result != 0)
			return MIXSHOP_ERROR;
	}

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "GetGemMixConsumeMoney", String(), GemMixFirstItemId);
	Int32 ConsumeMoney = (Int32)gGlobal.g_TempResult;
	if ((Int32)m_pOwner->GetMoney() < ConsumeMoney)
		return MIXSHOP_NO_GMONEY;

	resultID = gGlobal.m_resource->GetResultStoneID(gemID);
	if (resultID == 0)
		return MIXSHOP_NO_HIGHER_RESULT;

	maxStack = gGlobal.m_resource->GetMaxStoneMixNum(gemID);
	minStack = gGlobal.m_resource->GetMinStoneMixNum(gemID);

	for (Index i = C_INDEX(0); i < stacks.size(); ++i)
		totalStacks += stacks[i];

	TRACE_INFODTL_5(GLOBAL_LOGGER, _F(" gemID = %d, resultID = %d, maxStack = %d, minStack = %d, totalStacks = %d."),
		gemID, resultID, maxStack, minStack, totalStacks);

	if (maxStack == 0 || minStack == 0 || totalStacks > maxStack )
		return MIXSHOP_ERROR;

	if (!CanCreateLimitItem(resultID, 1))
		return MIXSHOP_ITEM_REACH_LIMIT;

	if (totalStacks >= minStack && totalStacks <= maxStack)
	{
		UInt prob = gGlobal.m_resource->GetStoneMixProb(gemID, totalStacks);
		if ( prob >= (UInt)GetRand(0, 100) )
		{
			TRACE_INFOL_2(GLOBAL_LOGGER, _F("GemMix: success to make for char_id = %d, item_id = %d"), m_pOwner->GetChar_id(), resultID);
			bool bFinishMake = false;
			CItem* pNewItem = NewItem(resultID, 1);
			if ( pNewItem )
			{
				for ( Index j = C_INDEX(BAG_CURRENT); j < BAG_TYPE_COUNT; ++j )
				{
					if (AddItem(pNewItem, j))
					{
						bFinishMake = true;
						break;
					}
				}
				if (!bFinishMake)
				{
					DeleteNetObject(pNewItem);
					if (stacks[0] > 0)
						stacks[0] = stacks[0] - 1;
				}
			}

			if (bFinishMake)
			{
				ReduceMixShopConsume(consumeType, consumeAmount, 1, LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

				m_pOwner->ChangeMoney(-(Int32)(ConsumeMoney), LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

				if (pNewItem)
				{
					gGlobal.g_TempResult = 0;
					CallLuaFunc(gGlobal.g_pLuaState, "GetGemItemLevel", String(), resultID);
					UInt GemLevel = (UInt)gGlobal.g_TempResult;
					if (GemLevel > 4)
					{
						String msg("");
						msg.Format(gGlobal.GetStringTable()->Get(_T("MIXSHOP_GEMMIX_RUMOR_LEVEL4")), 
							m_pOwner->GetCharNameLink(), pNewItem->m_data->name);
						MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
							RPCContext(), msg, 0, 1);
					}
				}
			}
		}
		else
		{
			if (stacks[0] > 0)
				stacks[0] = stacks[0] - 1;
			msgCode = MIXSHOP_FAIL;
		}

		UInt32Vector logSourceItemId;
		for (Index k = C_INDEX(0); k < itemUids.size(); ++k)
		{
			CItem* item = GetItemByUID(itemUids[k]);
			if (item)
			{
				item->SetStack(item->GetStack() - stacks[k]);
				if (item->GetStack() == 0)
					AddToDeleteGroup(item->GetBagIndex());
				logSourceItemId.push_back(item->GetItem_id());
			}
		}
		if (msgCode == MIXSHOP_FAIL)
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultID, 0, itemUids, logSourceItemId, stacks);
		}
		else
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultID, 1, itemUids, logSourceItemId, stacks);
		}
		RaiseUpdate();
	}
	return msgCode;
}

UInt8 CItemBag::GemInlay(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag::GemInlay(), char_id %d"), m_pOwner->GetChar_id());

	UInt8 msgCode = MIXSHOP_SUCCESS;

	if (itemUids.size() < 3 || stacks.size() != itemUids.size())
		return MIXSHOP_ERROR;

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	UInt32 gemID = 0;
	UInt totalStacks = 0;
	UInt maxStack = 0;
	UInt minStack = 0;
	UInt resultID = 0;

	CItem* eqItem = GetItemByUID(itemUids[0]);
	CItem* gemItem = GetItemByUID(itemUids[1]);
	CItem* consumeItem = GetItemByUID(itemUids[2]);

	if (eqItem == NULL || consumeItem == NULL || gemItem == NULL)
		return MIXSHOP_ERROR;

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "GetGemInlayConsumeMoney", String(), eqItem->m_data->reqLv);
	Int32 ConsumeMoney = (Int32)gGlobal.g_TempResult;
	if ((Int32)m_pOwner->GetMoney() < ConsumeMoney)
		return MIXSHOP_NO_GMONEY;

	UInt8 eq_gem_count = eqItem->GetEq_gem();
	if (eq_gem_count < 10)
	{
		// find empty slot
		UInt8 eq_gem_index = -1;
		for (UInt8 i = 0; i < 10; i++)
		{
			if (eqItem->GetInlay_gem(i) == 0)
			{
				eq_gem_index = i;
				break;
			}
		}

		if (eq_gem_index >= 0 && eq_gem_index < 10)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" find empty slot : %d."), eq_gem_index);

			bool bFinishMake = false;

			eqItem->SetInlay_gem(eq_gem_index, gemItem->GetItem_id());

			eq_gem_count++;
			eqItem->SetEq_gem(eq_gem_count);

			if (gemItem->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND)
				eqItem->SetSoulboundState(SOULBOUND_ALREADY_BOUND);

			eqItem->SetInlay_ATK(eqItem->GetInlay_ATK() + gemItem->GetATK());
			eqItem->SetInlay_DEF(eqItem->GetInlay_DEF() + gemItem->GetDEF());
			eqItem->SetInlay_HIT(eqItem->GetInlay_HIT() + gemItem->GetHIT());
			eqItem->SetInlay_EVA(eqItem->GetInlay_EVA() + gemItem->GetEVA());
			eqItem->SetInlay_POW(eqItem->GetInlay_POW() + gemItem->GetPOW());
			eqItem->SetInlay_SPD(eqItem->GetInlay_SPD() + gemItem->GetSPD());
			eqItem->SetInlay_maxHP(eqItem->GetInlay_maxHP() + gemItem->GetMaxHP());
			eqItem->SetInlay_maxSP(eqItem->GetInlay_maxSP() + gemItem->GetMaxSP());
			eqItem->SetInlay_maxVP(eqItem->GetInlay_maxVP() + gemItem->GetMaxVP());

			bFinishMake = true;

			if (bFinishMake)
			{
				ReduceMixShopConsume(consumeType, consumeAmount, 1, LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

				m_pOwner->ChangeMoney(-(Int32)(ConsumeMoney), LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);
			}

			UInt32Vector logSourceItemId;
			gemItem->SetStack(gemItem->GetStack() - stacks[1]);
			if (gemItem->GetStack() == 0)
				AddToDeleteGroup(gemItem->GetBagIndex());
			logSourceItemId.push_back(gemItem->GetItem_id());

			consumeItem->SetStack(consumeItem->GetStack() - stacks[2]);
			if (consumeItem->GetStack() == 0)
				AddToDeleteGroup(consumeItem->GetBagIndex());
			logSourceItemId.push_back(consumeItem->GetItem_id());
			if (msgCode == MIXSHOP_FAIL)
			{
				LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
					0, 0, itemUids, logSourceItemId, stacks);
			}
			else
			{
				LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
					0, 1, itemUids, logSourceItemId, stacks);
			}
			RaiseUpdate();
		}
		else
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" can not find empty slot for gem inlay."));

			msgCode = MIXSHOP_FAIL;
		}
	}
	else
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F(" max gem inlay count."));

		msgCode = MIXSHOP_FAIL;
	}

	return msgCode;
}

UInt8 CItemBag::GemRemove(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount, const UInt8 removeGemIndex)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CItemBag::GemRemove(), char_id %d, removeGemIndex = %d."), m_pOwner->GetChar_id(), removeGemIndex);

	UInt8 msgCode = MIXSHOP_SUCCESS;

	if (itemUids.size() != 2 || stacks.size() != itemUids.size())
		return MIXSHOP_ERROR;

	if (GetTotalFreeSlot() == 0)
		return MIXSHOP_NO_ITEMSLOT;

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" eqItem: %d, consumeItem: %d, removeGemIndex: %d."), 
		itemUids[0], itemUids[1], removeGemIndex);

	CItem* eqItem = GetItemByUID(itemUids[0]);
	CItem* consumeItem = GetItemByUID(itemUids[1]);
	if (eqItem == NULL || consumeItem == NULL || removeGemIndex < 0 || removeGemIndex >= 2)
		return MIXSHOP_ERROR;

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "GetGemRemoveConsumeMoney", String(), eqItem->m_data->reqLv);
	Int32 ConsumeMoney = (Int32)gGlobal.g_TempResult;
	if ((Int32)m_pOwner->GetMoney() < ConsumeMoney)
		return MIXSHOP_NO_GMONEY;

	bool bFinishMake = false;

	UInt16 Inlay_gem = eqItem->GetInlay_gem(removeGemIndex);
	if (Inlay_gem > 0)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" find inlay gem id : %d."), Inlay_gem);

		gGlobal.g_TempResult = 0;
		CallLuaFunc(gGlobal.g_pLuaState, "GetSoulBoundGemItemId", String(), Inlay_gem);
		Inlay_gem = (Int32)gGlobal.g_TempResult;

		const ItemData* pItemData = gGlobal.m_resource->GetItemData(Inlay_gem);
		if (pItemData)
		{
			if (!CanCreateLimitItem(Inlay_gem, 1))
				return MIXSHOP_ITEM_REACH_LIMIT;

			eqItem->SetInlay_gem(removeGemIndex, 0);

			UInt8 eq_gem_count = eqItem->GetEq_gem();
			if( eq_gem_count!=0 )
				eq_gem_count--;
			eqItem->SetEq_gem(eq_gem_count);

			Int16 Inlay_ATK = eqItem->GetInlay_ATK() - pItemData->ATK_d;
			if (Inlay_ATK < 0)
				Inlay_ATK = 0;
			Int16 Inlay_DEF = eqItem->GetInlay_DEF() - pItemData->DEF_d;
			if (Inlay_DEF < 0)
				Inlay_DEF = 0;
			Int16 Inlay_HIT = eqItem->GetInlay_HIT() - pItemData->HIT_d;
			if (Inlay_HIT < 0)
				Inlay_HIT = 0;
			Int16 Inlay_EVA = eqItem->GetInlay_EVA() - pItemData->EVA_d;
			if (Inlay_EVA < 0)
				Inlay_EVA = 0;
			Int16 Inlay_POW = eqItem->GetInlay_POW() - pItemData->POW_d;
			if (Inlay_POW < 0)
				Inlay_POW = 0;
			Int16 Inlay_SPD = eqItem->GetInlay_SPD() - pItemData->SPD_d;
			if (Inlay_SPD < 0)
				Inlay_SPD = 0;
			Int16 Inlay_maxHP = eqItem->GetInlay_maxHP() - pItemData->maxHP_d;
			if (Inlay_maxHP < 0)
				Inlay_maxHP = 0;
			Int16 Inlay_maxSP = eqItem->GetInlay_maxSP() - pItemData->maxSP_d;
			if (Inlay_maxSP < 0)
				Inlay_maxSP = 0;
			Int16 Inlay_maxVP = eqItem->GetInlay_maxVP() - pItemData->maxVP_d;
			if (Inlay_maxVP < 0)
				Inlay_maxVP = 0;

			eqItem->SetInlay_ATK(Inlay_ATK);
			eqItem->SetInlay_DEF(Inlay_DEF);
			eqItem->SetInlay_HIT(Inlay_HIT);
			eqItem->SetInlay_EVA(Inlay_EVA);
			eqItem->SetInlay_POW(Inlay_POW);
			eqItem->SetInlay_SPD(Inlay_SPD);
			eqItem->SetInlay_maxHP(Inlay_maxHP);
			eqItem->SetInlay_maxSP(Inlay_maxSP);
			eqItem->SetInlay_maxVP(Inlay_maxVP);

			CItem* pNewItem = NewItem(Inlay_gem, 1);
			if ( pNewItem )
			{
				for ( Index j = C_INDEX(BAG_CURRENT); j < BAG_TYPE_COUNT; ++j )
				{
					if (AddItem(pNewItem, j))
					{
						bFinishMake = true;
						break;
					}
				}
				if (!bFinishMake)
				{
					DeleteNetObject(pNewItem);
				}
			}
		}
		else
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F(" can not find gem item data."));

			msgCode = MIXSHOP_FAIL;
		}
	}

	if (bFinishMake)
	{
		ReduceMixShopConsume(consumeType, consumeAmount, 1, LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

		m_pOwner->ChangeMoney(-(Int32)(ConsumeMoney), LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);
	}

	UInt32Vector logSourceItemId;
	consumeItem->SetStack(consumeItem->GetStack() - stacks[1]);
	if (consumeItem->GetStack() == 0)
		AddToDeleteGroup(consumeItem->GetBagIndex());
	logSourceItemId.push_back(consumeItem->GetItem_id());

	if (msgCode == MIXSHOP_FAIL)
	{
		LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
			0, 0, itemUids, logSourceItemId, stacks);
	}
	else
	{
		LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
			0, 1, itemUids, logSourceItemId, stacks);
	}
	RaiseUpdate();

	return msgCode;
}

UInt8 CItemBag::GemMelting(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag::GemMelting(), char_id %d."), m_pOwner->GetChar_id());

	UInt8 msgCode = MIXSHOP_SUCCESS;

	if (itemUids.size() != 1 || stacks.size() != itemUids.size())
		return MIXSHOP_ERROR;

	if (GetTotalFreeSlot() == 0)
		return MIXSHOP_NO_ITEMSLOT;

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" GemItemUid : %d."), itemUids[0]);

	CItem* gemItem = GetItemByUID(itemUids[0]);
	if (gemItem == NULL)
		return MIXSHOP_ERROR;

	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, "CheckGemMeltingGemItem", String(), gemItem->GetItem_id(), gemItem->GetLock());
	UInt Result = (UInt)gGlobal.g_TempResult;
	if (Result != 0)
		return MIXSHOP_ERROR;

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "GetGemMeltingConsumeMoney", String(), gemItem->GetItem_id());
	Int32 ConsumeMoney = (Int32)gGlobal.g_TempResult;
	if ((Int32)m_pOwner->GetMoney() < ConsumeMoney)
		return MIXSHOP_NO_GMONEY;

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "GetGemItemLevel", String(), gemItem->GetItem_id());
	UInt GemLevel = (UInt)gGlobal.g_TempResult;

	UInt32 resultItemID = 61056;
	Int8 resultItemCount = 0;
	if(GemLevel == 1)
		resultItemCount = 1;
	else if(GemLevel == 2)
		resultItemCount = 5;
	else
		return MIXSHOP_ERROR;

	if (!CanCreateLimitItem(resultItemID, resultItemCount))
		return MIXSHOP_ITEM_REACH_LIMIT;

	bool bFinishMake = false;
	CItem* pNewItem = NewItem(resultItemID, resultItemCount);
	if ( pNewItem )
	{
		for ( Index j = C_INDEX(BAG_CURRENT); j < BAG_TYPE_COUNT; ++j )
		{
			if (AddItem(pNewItem, j))
			{
				bFinishMake = true;
				break;
			}
		}
		if (!bFinishMake)
		{
			DeleteNetObject(pNewItem);
			if (stacks[0] > 0)
				stacks[0] = stacks[0] - 1;
		}
	}

	if (bFinishMake)
	{
		ReduceMixShopConsume(consumeType, consumeAmount, 1, LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

		m_pOwner->ChangeMoney(-(Int32)(ConsumeMoney), LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

		UInt32Vector logSourceItemId;
		for (Index k = C_INDEX(0); k < itemUids.size(); ++k)
		{
			CItem* item = GetItemByUID(itemUids[k]);
			if (item)
			{
				item->SetStack(item->GetStack() - stacks[k]);
				if (item->GetStack() == 0)
					AddToDeleteGroup(item->GetBagIndex());
				logSourceItemId.push_back(item->GetItem_id());
			}
		}
		if (msgCode == MIXSHOP_FAIL)
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultItemID, 0, itemUids, logSourceItemId, stacks);
		}
		else
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultItemID, 1, itemUids, logSourceItemId, stacks);
		}
		RaiseUpdate();
	}
	return msgCode;
}

UInt8 CItemBag::GemPolishing(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag::GemPolishing(), char_id %d."), m_pOwner->GetChar_id());

	UInt8 msgCode = MIXSHOP_SUCCESS;

	if (itemUids.size() != 2 || stacks.size() != itemUids.size())
		return MIXSHOP_ERROR;

	if (GetTotalFreeSlot() == 0)
		return MIXSHOP_NO_ITEMSLOT;

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" GemItemUid : %d, ConsumeItemUid : %d."), 
		itemUids[0], itemUids[1]);

	CItem* gemItem = GetItemByUID(itemUids[0]);
	CItem* consumeItem = GetItemByUID(itemUids[1]);
	if (gemItem == NULL || consumeItem == NULL)
		return MIXSHOP_ERROR;

	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, "CheckGemPolishingGemItem", String(), gemItem->GetItem_id(), gemItem->GetLock());
	UInt Result = (UInt)gGlobal.g_TempResult;
	if (Result != 0)
		return MIXSHOP_ERROR;

	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, "CheckGemPolishingConsumeItem", String(), gemItem->GetItem_id(), consumeItem->GetItem_id());
	Result = (UInt)gGlobal.g_TempResult;
	if (Result != 0)
		return MIXSHOP_ERROR;

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "GetGemPolishingConsumeMoney", String(), gemItem->GetItem_id());
	Int32 ConsumeMoney = (Int32)gGlobal.g_TempResult;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" ConsumeMoney : %d."), ConsumeMoney);
	if ((Int32)m_pOwner->GetMoney() < ConsumeMoney)
		return MIXSHOP_NO_GMONEY;

	bool bIsBound = false;
	if (gemItem->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND)
		bIsBound = true;
	else if(consumeItem->GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND)
		bIsBound = true;
	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "GetGemPolishingResultGemItemId", String(), gemItem->GetItem_id(), bIsBound);
	UInt32 resultItemID = (UInt32)gGlobal.g_TempResult;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" resultItemID : %d."), resultItemID);
	if (resultItemID == 0)
		return MIXSHOP_ERROR;

	bool bFinishMake = false;
	CItem* pNewItem = NewItem(resultItemID, 1);
	if ( pNewItem )
	{
		for ( Index j = C_INDEX(BAG_CURRENT); j < BAG_TYPE_COUNT; ++j )
		{
			if (AddItem(pNewItem, j))
			{
				bFinishMake = true;
				break;
			}
		}
		if (!bFinishMake)
		{
			DeleteNetObject(pNewItem);
			if (stacks[0] > 0)
				stacks[0] = stacks[0] - 1;
		}
	}

	if (bFinishMake)
	{
		ReduceMixShopConsume(consumeType, consumeAmount, 1, LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

		m_pOwner->ChangeMoney(-(Int32)(ConsumeMoney), LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

		UInt32Vector logSourceItemId;
		for (Index k = C_INDEX(0); k < itemUids.size(); ++k)
		{
			CItem* item = GetItemByUID(itemUids[k]);
			if (item)
			{
				item->SetStack(item->GetStack() - stacks[k]);
				if (item->GetStack() == 0)
					AddToDeleteGroup(item->GetBagIndex());
				logSourceItemId.push_back(item->GetItem_id());
			}
		}
		if (msgCode == MIXSHOP_FAIL)
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultItemID, 0, itemUids, logSourceItemId, stacks);
		}
		else
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultItemID, 1, itemUids, logSourceItemId, stacks);
		}
		RaiseUpdate();
	}
	return msgCode;
}

UInt8 CItemBag::ColorStoneMix(UInt32Vector itemUids, UInt8Vector stacks, UInt8Vector consumeType, UInt32Vector consumeAmount)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CItemBag::ColorStoneMix(), char_id %d."), m_pOwner->GetChar_id());

	UInt8 msgCode = MIXSHOP_SUCCESS;

	if (itemUids.size() != 2 || stacks.size() != itemUids.size())
		return MIXSHOP_ERROR;

	if (GetTotalFreeSlot() == 0)
		return MIXSHOP_NO_ITEMSLOT;

	msgCode = GetMixShopConsumeMsgIndex(consumeType, consumeAmount);
	if (msgCode != MIXSHOP_SUCCESS)
		return msgCode;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" Item1 : %d, Item2 : %d."), 
		itemUids[0], itemUids[1]);

	CItem* stoneItem = GetItemByUID(itemUids[0]);
	CItem* consumeItem = GetItemByUID(itemUids[1]);
	if (stoneItem == NULL || consumeItem == NULL)
		return MIXSHOP_ERROR;

	if (stoneItem->m_data->reqLv != consumeItem->m_data->reqLv)
		return MIXSHOP_ERROR;

	UInt resultID = gGlobal.m_resource->GetResultStoneID(stoneItem->GetItem_id());
	if (resultID == 0)
		return MIXSHOP_NO_HIGHER_RESULT;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F(" resultID : %d."), resultID);

	bool bFinishMake = false;
	CItem* pNewItem = NewItem(resultID, 1);
	if ( pNewItem )
	{
		for ( Index j = C_INDEX(BAG_CURRENT); j < BAG_TYPE_COUNT; ++j )
		{
			if (AddItem(pNewItem, j))
			{
				bFinishMake = true;
				break;
			}
		}
		if (!bFinishMake)
		{
			DeleteNetObject(pNewItem);
			if (stacks[0] > 0)
				stacks[0] = stacks[0] - 1;
		}
	}

	if (bFinishMake)
	{
		ReduceMixShopConsume(consumeType, consumeAmount, 1, LogService_Proxy::Money_And_Exp::SOURCE_EQ_MAKE);

		UInt32Vector logSourceItemId;
		for (Index k = C_INDEX(0); k < itemUids.size(); ++k)
		{
			CItem* item = GetItemByUID(itemUids[k]);
			if (item)
			{
				item->SetStack(item->GetStack() - stacks[k]);
				if (item->GetStack() == 0)
					AddToDeleteGroup(item->GetBagIndex());
				logSourceItemId.push_back(item->GetItem_id());
			}
		}
		if (msgCode == MIXSHOP_FAIL)
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultID, 0, itemUids, logSourceItemId, stacks);
		}
		else
		{
			LogService_Proxy::LogMix(gGlobal.pLogService->pPeer, m_pOwner->GetContext(), m_pOwner->GetChar_id(), LogService_Proxy::Mix::MIX_STONEMIX, 
				resultID, 1, itemUids, logSourceItemId, stacks);
		}
		RaiseUpdate();
	}
	return msgCode;
}

RPCResult CItemBag::BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count)
{
	PROCEDURE_START_RPC3(ItemBag, BuyItemFromBusinessShop, this, UInt32, itemId, Int16, shopId, UInt16, count)
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("BuyItemFromBusinessShop: sessionId %s, char_id %d, shopId %d, itemId %d ,count %d"),
		context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), shopId, itemId,count);
	String errMsg;
	UInt8 msgIndex = 0;
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		if ( !m_pOwner->HasGuild())
		{
			errMsg = _T("you are not in a guild");
			break;
		}

		if (IsBusinessMoneyEnough())
		{
			errMsg = _T("MSG_BUSINESS_ALREADY_ENOUGH");
			break;
		}

		UInt16 MaxCount = GetBusinessItemCount() + count;
		if (MaxCount > BUSINESSSHOP_MAX_ITEM_COUNT)
		{
			errMsg = _T("MSG_BUSINESSSHOP_TOO_MORE_ITEM");
			break;
		}

		const ItemData* pItmData = gGlobal.m_resource->GetItemData(itemId);
		if ( !pItmData)
		{
			errMsg = _T("MSG_SHOP_NO_ITEM_ID");
			break;
		}

		if (ITEMTYPE_BUSINESS_ITEM != pItmData->item_type)
		{
			errMsg = _T("Item type is not business item");
			break;
		}

		UInt8 totalUse = MaxCount / (pItmData->overlap);
		if ( ( MaxCount % (pItmData->overlap)) > 0 )
			totalUse += 1;
		if ( totalUse > GetFreeSlot(BAG_CURRENT) )
		{
			errMsg = _T("MSG_SHOP_NO_SPACE");
			break;
		}
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("BuyItemFromBusinessShop Error: %s"), errMsg.c_str());
		CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), msgIndex, errMsg);
		PROCEDURE_RETURN
	}
	MessageService_Proxy::BuyItemFromBusinessShop(gGlobal.pMessageService->pPeer, context, itemId, shopId, count, GetBusinessMoney(), m_pOwner->GetGuildUid(), m_pOwner->GetChar_id());

	PROCEDURE_WAIT3(1, MessageService, CB_BuyItemFromBusinessShop, Boolean, success, UInt16, finalCount, UInt32, finalPrice)
		String errMsg;
	UInt8 msgIndex = 0;
	do
	{
		if (!success)
		{
			errMsg = _T("MSG_BUSINESSSHOP_BUY_FAIL");
			break;
		}

		UInt16 remaincount = finalCount;
		const ItemData* pItmData = gGlobal.m_resource->GetItemData(itemId);
		do
		{
			UInt8 add = 0;
			if ( remaincount > (pItmData->overlap) )
				add = pItmData->overlap;
			else
				add = (UInt8) remaincount;

			CItem* pNewItem = NewItem(itemId, add);
			if ( pNewItem )
			{
				Boolean bFinishBuy(BOOLEAN_FALSE);
				for ( UInt8 i = BAG_CURRENT; i < BAG_BACKUP; ++i )
				{
					if (AddItem(pNewItem, i))
					{
						bFinishBuy = BOOLEAN_TRUE;
						break;
					}
				}

				if ( !bFinishBuy )
				{
					DeleteNetObject((NetObject*&) pNewItem);
				}
			}
			remaincount -= add;
		}
		while(remaincount > 0);

		ChangeBusinessMoney(-(Int32)finalPrice);
		CB_BuyItemFromBusinessShop(m_pOwner->GetPeer(), m_pOwner->GetContext(), itemId, finalPrice);
		RaiseUpdate();					// raise update object event
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("BuyItemFromBusinessShop Error: %s"), errMsg.c_str());
		CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), msgIndex, errMsg);
		PROCEDURE_RETURN
	}
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("BuyItemFromBusinessShop Error: MessageService:CB_Error, errcode %d, %s"), 
		err, errMsg.c_str());

	PROCEDURE_EXCEPTION_DEFAULT

		PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}

RPCResult CItemBag::SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const Int16 &shopId, const UInt8 &bagIndex, const UInt16 &count)
{
	PROCEDURE_START_RPC3(ItemBag, SellItemToBusinessShop, this, Int16, shopId, UInt8, bagIndex, UInt16, count)
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("SellItemToBusinessShop: sessionId %s, char_id %d, shopId %d, bagIndex %d ,count %d"),
		context.sessionId.GetString().c_str(), m_pOwner->GetChar_id(), shopId, bagIndex,count);
	String errMsg;
	UInt8 msgIndex = 0;
	CItem *item(NULL);
	do
	{
		if (m_pOwner == NULL || !m_pOwner->IsInMap())
		{
			errMsg = _T("cannot use item while not in map");
			break;
		}

		if ( !m_pOwner->HasGuild())
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");
			break;
		}

		item = GetItem(bagIndex);
		if (bagIndex >= BAG_MAX_ITEM ||									// index out range
			item == NULL || !item->IsExist() || item->m_data == NULL)		// source item not exist
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");				//invalid item to sell
			break;
		}


		if (ITEMTYPE_BUSINESS_ITEM != item->m_data->item_type)		// source item not sellable
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");				//non-sellable item to sell
			break;
		}

		if ( count == 0 || count > item->GetStack() )
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");//Sell count is larger than the stack
			break;
		}
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("SellItemToBusinessShop Error: %s"), errMsg.c_str());
		CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), msgIndex, errMsg);
		PROCEDURE_RETURN
	}
	MessageService_Proxy::SellItemToBusinessShop(gGlobal.pMessageService->pPeer, context, item->GetItem_id(), shopId, count, m_pOwner->GetGuildUid(), m_pOwner->GetChar_id());

	PROCEDURE_WAIT2(1, MessageService, CB_SellItemToBusinessShop, Boolean, success, UInt32, finalPrice)
		String errMsg;
	UInt8 msgIndex = 0;
	do
	{
		if (!success)
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");
			break;
		}

		CItem *item = GetItem(bagIndex);
		if (bagIndex >= BAG_MAX_ITEM ||									// index out range
			item == NULL || !item->IsExist() || item->m_data == NULL)		// source item not exist
		{
			errMsg = _T("MSG_SYS_ITEM_SELL_ERR");				//invalid item to sell
			break;
		}
		item->SetStack(item->GetStack() - count);
		if (item->GetStack() == 0)
			AddToDeleteGroup(item->GetBagIndex());

		ChangeBusinessMoney(finalPrice);
		CB_SellItemToBusinessShop(m_pOwner->GetPeer(), m_pOwner->GetContext(), item->GetItem_id(), finalPrice, IsBusinessMoneyEnough());
		RaiseUpdate();					// raise update object event
	}
	while (false);

	if (!errMsg.IsEmpty())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("BuyItemFromBusinessShop Error: %s"), errMsg.c_str());
		CB_Error(m_pOwner->GetPeer(), m_pOwner->GetContext(), msgIndex, errMsg);
		PROCEDURE_RETURN
	}
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("BuyItemFromBusinessShop Error: MessageService:CB_Error, errcode %d, %s"), 
		err, errMsg.c_str());

	PROCEDURE_EXCEPTION_DEFAULT

		PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}

UInt16 CItemBag::GetBusinessItemCount()
{
	UInt16 TotalCount(0);
	CItem* item = NULL;
	for (int i = 0; i < BAG_ITEM_COUNT; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() && item->m_data && ITEMTYPE_BUSINESS_ITEM == item->m_data->item_type)
			TotalCount += item->GetStack();
	}
	return TotalCount;
}

void CItemBag::RemoveAllBusinessItem()
{
	bool updated = false;
	for (UInt8 i = 0; i < BAG_ITEM_COUNT; ++i)
	{
		CItem* pItem = GetItem(i);
		if (pItem && pItem->IsExist() && pItem->m_data &&
			(ITEMTYPE_BUSINESS_ITEM == pItem->m_data->item_type || ITEMTYPE_BUSINESS_MONEY == pItem->m_data->item_type))
		{
			updated = true;
			pItem->SetStack(0);
			AddToDeleteGroup(pItem->GetBagIndex());
		}
	}
	if (updated)
		RaiseUpdate();
}

UInt32 CItemBag::GetBusinessMoney()
{
	UInt32 BusinessMoney(0);
	CItem* item = NULL;
	for (int i = 0; i < BAG_ITEM_COUNT; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() && item->m_data && ITEMTYPE_BUSINESS_MONEY == item->m_data->item_type)
		{
			if( item->m_data->rare > 3 )
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("rare >3 not handled, item_id %d, rare %d"),
					item->m_data->item_id, item->m_data->rare);
			}

			BusinessMoney += (item->GetItemValue1() * 10000 + item->GetItemValue2());
			if (item->GetItemValue3() == 0)
			{
				BusinessMoney = BusinessMoneyStart[item->m_data->rare] + BusinessMoney;
			}
			else
			{
				BusinessMoney = BusinessMoneyStart[item->m_data->rare] - BusinessMoney;
			}
			break;
		}
	}
	return BusinessMoney;
}

Boolean CItemBag::IsBusinessMoneyEnough()
{
	UInt32 BusinessMoney(0);
	CItem* item = NULL;
	for (int i = 0; i < BAG_ITEM_COUNT; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() && item->m_data && ITEMTYPE_BUSINESS_MONEY == item->m_data->item_type)
		{
			if( item->m_data->rare > 3 )
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("rare >3 not handled, item_id %d, rare %d"),
					item->m_data->item_id, item->m_data->rare);
			}

			BusinessMoney += (item->GetItemValue1() * 10000 + item->GetItemValue2());
			if (item->GetItemValue3() == 0)
			{
				BusinessMoney = BusinessMoneyStart[item->m_data->rare] + BusinessMoney;
			}
			else
			{
				BusinessMoney = BusinessMoneyStart[item->m_data->rare] - BusinessMoney;
			}
			if (BusinessMoney >= BusinessMoneyFinish[item->m_data->rare])
			{
				return BOOLEAN_TRUE;
			}
			return BOOLEAN_FALSE;
		}
	}
	return BOOLEAN_FALSE;
}

Boolean CItemBag::HaveBusinessItemOrMoney()
{
	UInt32 BusinessMoney(0);
	CItem* item = NULL;
	for (int i = 0; i < BAG_ITEM_COUNT; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() && item->m_data && (ITEMTYPE_BUSINESS_ITEM == item->m_data->item_type || ITEMTYPE_BUSINESS_MONEY == item->m_data->item_type))
		{
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

Boolean CItemBag::ChangeBusinessMoney(Int32 nChange)
{
	CItem* item = NULL;
	for (int i = 0; i < BAG_ITEM_COUNT; ++i) 
	{
		item = GetItem(i);
		if (item && item->IsExist() && item->m_data && ITEMTYPE_BUSINESS_MONEY == item->m_data->item_type)
		{
			if( item->m_data->rare > 3 )
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("rare >3 not handled, item_id %d, rare %d"),
					item->m_data->item_id, item->m_data->rare);
			}

			UInt32 money = GetBusinessMoney();
			if (money < BusinessMoneyFinish[item->m_data->rare]
			&& (money + nChange) >= BusinessMoneyFinish[item->m_data->rare])
			{
				item->SetItem_id(item->GetItem_id() + 1);
			}
			money += nChange;
			if (money >= BusinessMoneyStart[item->m_data->rare])
			{
				money -= BusinessMoneyStart[item->m_data->rare];
				item->SetItemValue1(money / 10000);
				item->SetItemValue2(money % 10000);
				item->SetItemValue3(0);
			}
			else
			{
				money = BusinessMoneyStart[item->m_data->rare] - money;
				item->SetItemValue1(money / 10000);
				item->SetItemValue2(money % 10000);
				item->SetItemValue3(1);
			}
			return BOOLEAN_TRUE;
		}
	}
	return BOOLEAN_FALSE;
}

void CItemBag::FindFoodItemLstByType(UInt8 itemType,UInt8 useTpye)
{
	m_FoodItemMap.clear();
	for (int i = 0; i < BAG_ITEM_COUNT; ++i) 
	{
		CItem* item = GetItem(i);
		if(item && item->IsExist() && item->m_data)
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Item bagindex = %d,Item type = %d,Item restore_HP = %d, Item restore_SP = %d"), 
			i,item->m_data->item_type,item->m_data->restore_HP,item->m_data->restore_SP);
		if (item && item->IsExist() && item->m_data && itemType == item->m_data->item_type)
		{
			if (useTpye == USE_FAST_FOOD_HP && item->m_data->restore_HP > 0 && !item->GetLock())
				m_FoodItemMap.insert(std::make_pair(i, item));
			else if (useTpye == USE_FAST_FOOD_SP && item->m_data->restore_SP > 0  && !item->GetLock())
				m_FoodItemMap.insert(std::make_pair(i, item));
		}
	}
}

UInt8 CItemBag::FindMinItemInBagIndex()
{
	UInt8 item_bagindex = 0;
	Int16 item_add = ADD_MAX;
	if (m_FoodItemMap.size())
	{
		CItemBag::FoodItemMap::iterator itr = m_FoodItemMap.begin();
		while (itr != m_FoodItemMap.end())
		{
			const CItem* pItem = itr->second;
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("FindMinItemInBagIndex Item bagindex = %d,Item type = %d,Item restore_HP = %d, Item restore_SP = %d"), 
				itr->first,pItem->m_data->item_type,pItem->m_data->restore_HP,pItem->m_data->restore_SP);
			if ((pItem->m_data->restore_HP > 0 && item_add > pItem->m_data->restore_HP)||(pItem->m_data->restore_SP > 0 && item_add > pItem->m_data->restore_SP) )
			{
				if (pItem->m_data->restore_HP > 0)
					item_add = pItem->m_data->restore_HP;
				else
					item_add = pItem->m_data->restore_SP;		
				item_bagindex = itr->first;
			}
			itr++;
		}
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("return item_bagindex = %d"),item_bagindex);
		return item_bagindex;
	}
	return 0;
}

RPCResult CItemBag::UseFastFoodItem(LPCPEER pPeer, RPCContext &context,const UInt8 &useType,const UInt32& partnerID)
{
	FindFoodItemLstByType(ITEMTYPE_FAST_FOOD,useType);
	CPartner* partner = NULL;
	if(partnerID > 0)
	{
		partner = m_pOwner->m_pPartnerGroup->GetPartner(partnerID);
		while (partner && ((useType == USE_FAST_FOOD_HP && partner->GetHP() < partner->GetMaxHP())|| 
			(useType == USE_FAST_FOOD_SP && partner->GetSP() < partner->GetMaxSP())))
		{
			if (m_FoodItemMap.size() <= 0)
				return RPC_RESULT_OK;
			UInt8 item_bagindex = FindMinItemInBagIndex();
			if (item_bagindex == 0)
				return RPC_RESULT_OK;
			CItem* item = GetItem(item_bagindex);
			if(item)
			{
				TRACE_INFODTL_4(GLOBAL_LOGGER, _F("UseItem Item bagindex = %d,Item type = %d,Item restore_HP = %d, Item restore_SP = %d"), 
					item_bagindex,item->m_data->item_type,item->m_data->restore_HP,item->m_data->restore_SP);
				if(RPC_RESULT_FAIL == UseItem(m_pOwner->GetPeer(),m_pOwner->GetContext(),item_bagindex,item->GetItem_id(),false,partnerID,0))
					break;
			}
			else
			{	
				CItemBag::FoodItemMap::iterator itr_itemuse = m_FoodItemMap.find(item_bagindex);
				if (itr_itemuse != m_FoodItemMap.end())
					m_FoodItemMap.erase(itr_itemuse);
				TRACE_INFODTL(GLOBAL_LOGGER, _F("remove foodlist"));
			}
			if (partner)
			{
				TRACE_INFODTL_4(GLOBAL_LOGGER, _F("partner hp = %d,maxhp = %d,sp = %d, maxsp = %d"), 
					partner->GetHP(),partner->GetMaxHP(),partner->GetSP(),partner->GetMaxSP());
			}
		}
	}
	else
	{
		while((useType == USE_FAST_FOOD_HP && m_pOwner->GetHP() < m_pOwner->GetMHP())||
			(useType == USE_FAST_FOOD_SP && m_pOwner->GetSP() < m_pOwner->GetMSP()))
		{
			if (m_FoodItemMap.size() <= 0)
				return RPC_RESULT_OK;
			UInt8 item_bagindex = FindMinItemInBagIndex();
			CItem* item = GetItem(item_bagindex);
			if(item)
			{
				TRACE_INFODTL_4(GLOBAL_LOGGER, _F("UseItem Item bagindex = %d,Item type = %d,Item restore_HP = %d, Item restore_SP = %d"), 
					item_bagindex,item->m_data->item_type,item->m_data->restore_HP,item->m_data->restore_SP);
				if(RPC_RESULT_FAIL == UseItem(m_pOwner->GetPeer(),m_pOwner->GetContext(),item_bagindex,item->GetItem_id(),false,0,0))
					break;
			}
			else
			{	
				CItemBag::FoodItemMap::iterator itr_itemuse = m_FoodItemMap.find(item_bagindex);
				if (itr_itemuse != m_FoodItemMap.end())
					m_FoodItemMap.erase(itr_itemuse);
				TRACE_INFODTL(GLOBAL_LOGGER, _F("remove foodlist"));
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CItemBag::Jpack(LPCPEER pPeer, RPCContext &context,const UInt8 &bagType)
{

	m_JpackItemVec.clear();
	CItem* item = NULL;
	for (int i = bagType*BAG_ITEM_COUNT; i < (bagType+1)*BAG_ITEM_COUNT; i++)
	{
		item = GetItem(i);
		if(item && item->IsExist() && item->m_data)
			m_JpackItemVec.push_back(item);

	}
	if(m_JpackItemVec.size() <= 0)
		return RPC_RESULT_OK;
	JpackItemSort();
	for(int i = 0; i < m_JpackItemVec.size() ; i++)
	{
		if(m_JpackItemVec[i] && m_JpackItemVec[i]->m_data && m_JpackItemVec[i]->GetBagIndex() != bagType*BAG_ITEM_COUNT+i )
		{
			SwapItemByIndex(m_JpackItemVec[i]->GetBagIndex(),bagType*BAG_ITEM_COUNT+i);
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Change ItemID = %d,Item bagindex = %d,Item new Index = %d"), 
				m_JpackItemVec[i]->GetItem_id(),m_JpackItemVec[i]->GetBagIndex(),bagType*BAG_ITEM_COUNT+i);
		}
	}
	RaiseUpdate();
	m_JpackItemVec.clear();
	return RPC_RESULT_OK;
}

RPCResult CItemBag::CleanWarehouse(LPCPEER pPeer, RPCContext &context,const UInt8 &page)
{
	m_WarehouseItemVec.clear();
	CItem* item = NULL;
	for (int i = page*BAG_MAX_WAREHOUSE_ITEM_PER_PAGE; i < (page+1)*BAG_MAX_WAREHOUSE_ITEM_PER_PAGE; i++)
	{
		CItem* item = GetItemFromWareHouse(i);
		if (item && item->IsExist() && item->m_data)
			m_WarehouseItemVec.push_back(item);
	}
	if (m_WarehouseItemVec.size() <= 0)
		return RPC_RESULT_OK;
	JpackItemSort();
	for(int i = 0; i < m_WarehouseItemVec.size() ; i++)
	{
		if(m_WarehouseItemVec[i] && m_WarehouseItemVec[i]->m_data &&
			m_WarehouseItemVec[i]->GetBagIndex() != page*BAG_MAX_WAREHOUSE_ITEM_PER_PAGE+i)
			WareHouseItemMove(m_pOwner->GetPeer(),m_pOwner->GetContext(),m_WarehouseItemVec[i]->GetBagIndex(),page*BAG_MAX_WAREHOUSE_ITEM_PER_PAGE+i);
	}
	RaiseUpdate();
	m_WarehouseItemVec.clear();
	return RPC_RESULT_OK;
}

void CItemBag::JpackItemSort()
{
	if(m_JpackItemVec.size() > 0)
	{
		for(int i = 0; i < m_JpackItemVec.size() ; i++)
		{
			int Index = 0;
			for(int j = 1; j < m_JpackItemVec.size() - i; j++)
			{
				if(m_JpackItemVec[Index]->m_data->item_type < m_JpackItemVec[j]->m_data->item_type)
					Index = j;
				else if (m_JpackItemVec[Index]->m_data->item_type == m_JpackItemVec[j]->m_data->item_type &&
					m_JpackItemVec[Index]->GetItem_id() < m_JpackItemVec[j]->GetItem_id())
				{
					Index = j;
				}
				else if (m_JpackItemVec[Index]->GetItem_id() == m_JpackItemVec[j]->GetItem_id())
				{
					CItem* dstItem = m_JpackItemVec[Index];
					CItem* srcItem = m_JpackItemVec[j];
					if (srcItem->m_data->overlap > 1)
					{
						if (dstItem && dstItem->IsExist() && dstItem->GetItem_id() == srcItem->GetItem_id() &&
							dstItem->GetStack() != dstItem->m_data->overlap &&
							srcItem->GetStack() != srcItem->m_data->overlap)
						{
							UInt8 avail = dstItem->m_data->overlap - dstItem->GetStack();
							if (avail > 0)
							{
								// item stack will change
								if (srcItem->GetStack() <= avail)
								{
									// fully merged
									dstItem->SetStack(dstItem->GetStack() + srcItem->GetStack());
									srcItem->SetStack(0);							// source item change to non-exist
									ItemVec::iterator itr = m_JpackItemVec.begin();
									itr += j;
									m_JpackItemVec.erase(itr);
									j--;
									AddToDeleteGroup(srcItem->GetBagIndex());
								}
								else
								{
									// partial merged
									dstItem->SetStack(dstItem->m_data->overlap);
									srcItem->SetStack(srcItem->GetStack() - avail);
									RaiseUpdate();
								}
								dstItem->SetLock((dstItem->GetLock() || srcItem->GetLock()));
							}					
						}
					}		
				}
			}
			std::swap(m_JpackItemVec[Index], m_JpackItemVec[m_JpackItemVec.size()-i-1]);
		}
	}
	else if(m_WarehouseItemVec.size() > 0)
	{
		for(int i = 0; i < m_WarehouseItemVec.size() ; i++)
		{
			int Index = 0;
			for(int j = 1; j < m_WarehouseItemVec.size() - i; j++)
			{
				if(m_WarehouseItemVec[Index]->m_data->item_type < m_WarehouseItemVec[j]->m_data->item_type)
					Index = j;
				else if (m_WarehouseItemVec[Index]->m_data->item_type == m_WarehouseItemVec[j]->m_data->item_type &&
					m_WarehouseItemVec[Index]->GetItem_id() < m_WarehouseItemVec[j]->GetItem_id())
				{
					Index = j;
				}
				else if (m_WarehouseItemVec[Index]->GetItem_id() == m_WarehouseItemVec[j]->GetItem_id())
				{
					if (m_WarehouseItemVec[Index]->m_data->overlap > 1)
					{
						CItem* dstItem = m_WarehouseItemVec[Index];
						CItem* srcItem = m_WarehouseItemVec[j];
						bool swapItem = true;
						if (dstItem && dstItem->IsExist() && dstItem->GetItem_id() == srcItem->GetItem_id() &&
							dstItem->GetStack() != dstItem->m_data->overlap &&
							srcItem->GetStack() != srcItem->m_data->overlap)
						{
							UInt8 avail = dstItem->m_data->overlap - dstItem->GetStack();
							if (avail > 0)
							{
								// item stack will change
								if (srcItem->GetStack() <= avail)
								{
									// fully merged
									dstItem->SetStack(dstItem->GetStack() + srcItem->GetStack());
									srcItem->SetStack(0);							// source item change to non-exist
									ItemVec::iterator itr = m_WarehouseItemVec.begin();
									itr += j;
									m_WarehouseItemVec.erase(itr);
									j--;
									AddToDeleteGroup(srcItem->GetBagIndex(), srcItem->GetInWareHouse());
								}
								else
								{
									// partial merged
									dstItem->SetStack(dstItem->m_data->overlap);
									srcItem->SetStack(srcItem->GetStack() - avail);
									RaiseUpdate();
								}
								dstItem->SetLock((dstItem->GetLock() || srcItem->GetLock()));
							}					
						}

					}	

				}
			}
			std::swap(m_WarehouseItemVec[Index], m_WarehouseItemVec[m_WarehouseItemVec.size()-i-1]);
		}
	}
}
