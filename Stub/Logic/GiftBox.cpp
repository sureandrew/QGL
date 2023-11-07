//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/GiftBox.h"
#include "Stub/Logic/Gift.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "Proxy/Data/GiftData.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/Character.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Service/LogService.h"
#include "Stub/Logic/ScriptSystem.h"

REGISTER_CLASSTYPE(CGiftBox, GiftBox_CLASSID);

CGiftBox::CGiftBox()
{
	m_giftList.clear();
	m_iUpdateTime = 0;
}

void CGiftBox::SetGiftBox(NetGroup* giftbox, bool sendToClient, NetGroup* sendGrp )
{
	if (giftbox)
	{
		for (Index i = C_INDEX(0); i < giftbox->GetCount(); ++i)
		{
			CGiftData* pGiftData = giftbox->GetChild<CGiftData>(i);
			if (pGiftData)
			{
				CGift* pGift = CGift::NewInstance();
				if (pGift)
				{
					pGift->SetData(pGiftData);
					Add(pGift);
					m_giftList.insert(std::make_pair(pGift->GetGiftID(), pGift));
					if (sendToClient)
						pGift->PreSend();
					if (sendGrp)
						sendGrp->Add(pGift);
				}
			}
		}
	}
}

RPCResult CGiftBox::TakeGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32 &giftID)
{
	PROCEDURE_START_RPC1(GiftBox, TakeGiftBox, this, UInt32, giftID)

		UInt8 msgIndex = GIFTBOX_ERROR;
		GiftMap::iterator itr =  m_giftList.find(giftID);
		if (itr == m_giftList.end() || m_pOwner == NULL)
		{
			CB_TakeGiftBox(pPeer, context, BOOLEAN_FALSE, msgIndex);
			PROCEDURE_RETURN
		}

		switch(itr->second->GetItemType())
		{
		case GIFT_ITEM:
			{
				UInt8 emptyBagIndex = 0;
				if (m_pOwner->m_pBag->FindEmptyIndex(0, emptyBagIndex))
				{
					//if (m_pOwner->m_pBag->CanCreateLimitItem(itr->second->GetItemID(), itr->second->GetItemQuantity())
					//{
						CItem* pNewItem = m_pOwner->m_pBag->NewItem(itr->second->GetItemID(), itr->second->GetItemQuantity(), false);
						if (pNewItem && m_pOwner->m_pBag->AddItem(pNewItem, 0))
						{
							m_pOwner->m_pBag->RaiseUpdate();
							msgIndex = GIFTBOX_SUCCESS;
						}
					/*}
					else
						msgIndex = GIFTBOX_ITEM_REACH_LIMIT;*/
				}
				else
					msgIndex = GIFTBOX_NO_ITEM_SLOT;
			}
			break;
		case GIFT_PARTNER:
			{
				if (m_pOwner->GetMaxPartner() > m_pOwner->m_pPartnerGroup->GetCount())
				{
					const MobData* pData = gGlobal.m_resource->GetMobData(itr->second->GetItemID());
					if (pData)
					{
						CPartner* partner = CPartner::NewInstance();
						if (partner)
						{
							partner->SetId(0, itr->second->GetItemID());
							if (pData->partner_type == LEVEL_NOT_CHANGE_FOLLOW_OWNER)
								partner->SetLevel(m_pOwner->GetLevel() + 5);
							else if (pData->partner_type == LEVEL_FOLLOW_OWNER)
								partner->SetLevel(m_pOwner->GetLevel());
							else
								partner->SetLevel(1);

							partner->SetOwner(m_pOwner->GetChar_id());
							if (m_pOwner->m_pPartnerGroup->AddNewPartner(partner)) 
							{	
								m_pOwner->m_pPartnerGroup->RaiseUpdate();
								msgIndex = GIFTBOX_SUCCESS;
							}
							else
								DeleteNetObject(partner);	
						}
					}
				}
				else
					msgIndex = GIFTBOX_NO_PARTNER_SLOT;
			}
			break;
		case GIFT_PET:
			{
				if (m_pOwner->GetMaxPet() > m_pOwner->m_pPetGroup->GetCount())
				{
					//make sure the mob is existed
					const PetData* pData = gGlobal.m_resource->GetPetData(itr->second->GetItemID());
					if (pData)
					{
						CPet* pet = CPet::NewInstance();
						if (pet)
						{
							pet->SetId(0, itr->second->GetItemID());
							pet->SetLevel(1);
							pet->SetOwner_id(m_pOwner->GetChar_id());
							pet->SetFirstMaster(m_pOwner->GetNickName());
							if (m_pOwner->m_pPetGroup->AddNewPet(pet)) 
							{	
								m_pOwner->m_pPetGroup->RaiseUpdate();							
								msgIndex = GIFTBOX_SUCCESS;
							}
							else
								DeleteNetObject(pet);	
						}
					}					
				}
				else
					msgIndex = GIFTBOX_NO_PET_SLOT;
			}
			break;
		}

		if (msgIndex == GIFTBOX_SUCCESS)
		{
			UInt32Vector giftList;
			giftList.clear();
			giftList.push_back(itr->second->GetGiftID());
			ObjectService_Proxy::DeleteGiftBox(gGlobal.pObjectService->pPeer, context, giftList);
		}
		else
		{
			CB_TakeGiftBox(pPeer, context, BOOLEAN_FALSE, msgIndex);
			PROCEDURE_RETURN
		}

		
	PROCEDURE_WAIT(1, ObjectService, CB_DeleteGiftBox)
		
		GiftMap::iterator itr =  m_giftList.find(giftID);
		if (itr != m_giftList.end())
		{
			CGift* pGift = itr->second;
			if (pGift)
			{
				UInt quantity = 0;
				switch(pGift->GetItemType())				
				{
				case GIFT_ITEM:
					{
						const ItemData* pItemData = gGlobal.m_resource->GetItemData(pGift->GetItemID());
						if (pItemData)
						{
							if (pItemData->overlap >= pGift->GetItemQuantity())
								quantity = pGift->GetItemQuantity();
							else
								quantity = pItemData->overlap;
						}
					}
					break;
				case GIFT_PARTNER:
				case GIFT_PET:
					quantity = 1;
					break;
				}
				if (m_pOwner)
					LogService_Proxy::LogGiftBox(gGlobal.pLogService->pPeer, context, 
						m_pOwner->GetAcct_id(), m_pOwner->GetChar_id(), 
						pGift->GetItemID(), pGift->GetItemType(), 
						quantity, pGift->GetSerialNo());
				m_giftList.erase(itr);
				Remove(pGift);
				DeleteNetObject(pGift);
			}
		}
		
		CB_TakeGiftBox(pPeer, context, BOOLEAN_TRUE, GIFTBOX_SUCCESS);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CGiftBox TakeGiftBox Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());
		CB_TakeGiftBox(pPeer, context, BOOLEAN_FALSE, GIFTBOX_ERROR);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult CGiftBox::CloseGiftBox(LPCPEER pPeer, RPCContext &context)
{
	CGiftBox* pGiftBox = this;
	UnsetOwnObject(m_pOwner->GetContext().sessionId, pGiftBox);
	m_pOwner->m_pGiftBox = NULL;
	DeleteNetGroup(pGiftBox, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

CGiftBox::~CGiftBox()
{
	m_giftList.clear();
}

RPCResult CGiftBox::UpdateGiftBox(LPCPEER pPeer, RPCContext &context)
{
	PROCEDURE_START_RPC(GiftBox, UpdateGiftBox, this)

		PROCEDURE_WAIT_TIMEOUT(1, 2000) //delay 2 second	

		UInt msgIndex = GIFTBOX_UDPATE_SUCCESS;
		do
		{
			if (m_pOwner == NULL)
			{
				msgIndex = GIFTBOX_UDPATE_FALSE;
				break;
			}

			if (!m_pOwner->IsInMap())
			{
				msgIndex = GIFTBOX_UDPATE_NOT_IN_MAP;
				break;
			}

			if (GetTickCount() - m_iUpdateTime < gConfigure.uGiftBoxRefreshTime)
			{
				msgIndex = GIFTBOX_UDPATE_TIMEOUT;
				break;
			}

			if (m_pOwner->IsTrading())
			{
				msgIndex = GIFTBOX_UDPATE_TRADING;
				break;
			}

			if (m_pOwner->GetScriptSystem() && m_pOwner->GetScriptSystem()->IsRunning())
			{
				msgIndex = GIFTBOX_UDPATE_SCRIPT_RUNNING;
				break;
			}
		}
		while(false);

		if (msgIndex != GIFTBOX_UDPATE_SUCCESS)
		{
			NetGroup temp;
			CB_UpdateGiftBox(pPeer, context, msgIndex, &temp);
			PROCEDURE_RETURN
		}
		else
		{
			ObjectService_Proxy::UpdateGiftBoxByAcctID(gGlobal.pObjectService->pPeer, context, m_pOwner->GetAcct_id());
		}

	PROCEDURE_WAIT1(2, ObjectService, CB_GetGiftBoxByAcctID, NetGroup*, updateGrp)

		NetGroup sendGrp;
		SetGiftBox(updateGrp, true, &sendGrp);
		m_iUpdateTime = GetTickCount();
		CB_UpdateGiftBox(pPeer, context, GIFTBOX_UDPATE_SUCCESS, &sendGrp);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());
		NetGroup temp;
		CB_UpdateGiftBox(pPeer, context, GIFTBOX_UDPATE_FALSE, &temp);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}