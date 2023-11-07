//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/ChargeShop.h"
#include "Stub/Logic/ChargeShopItem.h"
#include "MapService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResChargeShop.h"
#include "Stub/Logic/Character.h"
#include "Proxy/Data/ChargeShopData.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Stub/Service/MapService.h"
#include "Stub/Logic/CharacterInfo.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Service/LogService.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Resource/ResPet.h"
#include "Stub/Logic/ScriptSystem.h"

REGISTER_CLASSTYPE(CChargeShop, ChargeShop_CLASSID);

CChargeShop::CChargeShop()
{
	m_AllItem.clear();
	m_limitedItem.clear();
	m_discountItem.clear();
	m_charIDList.clear();
}

CChargeShop::~CChargeShop()
{
	//DeleteNetChilds(this);
	//for (ChargeShopCategory::iterator itr = m_AllItem.begin(); 
	//	itr != m_AllItem.end(); ++itr)
	//	DeleteNetObject(itr->second);
	for (ChargeShopCategory::iterator itr = m_limitedItem.begin(); 
		itr != m_limitedItem.end(); ++itr)
		DeleteNetObject(itr->second);
	for (ChargeShopCategory::iterator itr = m_discountItem.begin(); 
		itr != m_discountItem.end(); ++itr)
		DeleteNetObject(itr->second);
	
	m_AllItem.clear();
	m_limitedItem.clear();
	m_discountItem.clear();
	m_charIDList.clear();
}


void CChargeShop::PreSend()
{
	CChargeShopItem* pItem = NULL;
	for(Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		pItem  = GetChild<CChargeShopItem>(i);
		if (pItem)
			pItem->PreSend();
	}
}


void CChargeShop::InitChargeShop()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CChargeShop, InitChargeShop, this, context)
}

RPCResult CChargeShop::InitChargeShop(RPCContext &context)
{
	PROCEDURE_START_METHOD(CChargeShop, InitChargeShop, this, context)

		ObjectService_Proxy::GetCacheChargeShop(gGlobal.pObjectService->pPeer, context, GETALL);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetChargeShop, NetGroup*, chargeShopDataGroup)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("GetChargeShop and divide it to different list:"));
		for (UInt32 i = 0; i < chargeShopDataGroup->GetCount(); ++i)
		{
			CChargeShopData* pChargeShopData = chargeShopDataGroup->GetChild<CChargeShopData>(i);
			CChargeShopItem* item = CChargeShopItem::NewInstance();
			if (pChargeShopData && item)
			{	
				item->SetData(pChargeShopData);
				switch(item->GetShopType())
				{
				case LIMITED:
					m_limitedItem.insert(std::make_pair(item->GetChargeShopID(), item));//.push_back(item);
					break;
				case DISCOUNT:
					m_discountItem.insert(std::make_pair(item->GetChargeShopID(), item));//.push_back(item);
					break;
				default:
					Add(item);
					break;
				}
				m_AllItem.insert(std::make_pair(item->GetChargeShopID(), item));
			}
		}
		DeleteNetGroup(chargeShopDataGroup, BOOLEAN_TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CChargeShop InitChargeShop Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

time_t CChargeShop::ConvertStringToTime(String timeStr)
{
	tm fillTime;
	
	fillTime.tm_isdst = 0;
	fillTime.tm_wday = 0;
	fillTime.tm_yday = 0;

	String temp;
	temp = timeStr.substr(0, 4);
	fillTime.tm_year = _ttoi(temp.c_str()) - 1900;

	temp = timeStr.substr(4, 2);
	fillTime.tm_mon = _ttoi(temp.c_str()) - 1;

	temp = timeStr.substr(6, 2);
	fillTime.tm_mday = _ttoi(temp.c_str());

	temp = timeStr.substr(8, 2);
	fillTime.tm_hour = _ttoi(temp.c_str());

	temp = timeStr.substr(10, 2);
	fillTime.tm_min = _ttoi(temp.c_str());

	temp = timeStr.substr(12,2);
	fillTime.tm_sec = _ttoi(temp.c_str());

	return mktime(&fillTime);
}

NetGroup CChargeShop::GetDiscountItem()
{
	NetGroup discountGroup;

	time_t nowTime;
	time(&nowTime);

	ChargeShopCategory haveTimeRangeItem, noTimeRangeItem;
	for (ChargeShopCategory::iterator itr = m_discountItem.begin(); itr != m_discountItem.end(); ++itr)
	{	
		if (itr->second->GetStartTime() > 0 && itr->second->GetEndTime() > 0)
		{
			if (itr->second->GetStartTime() < (UInt32)nowTime && itr->second->GetEndTime() > (UInt32)nowTime)
				haveTimeRangeItem.insert(std::make_pair(itr->second->GetChargeShopID(), itr->second));					
		}
		else
			noTimeRangeItem.insert(std::make_pair(itr->second->GetChargeShopID(), itr->second));
	}

	for (ChargeShopCategory::iterator itr = haveTimeRangeItem.begin(); itr != haveTimeRangeItem.end(); ++itr)
	{
		itr->second->PreSend();
		discountGroup.Add(itr->second);
		if (discountGroup.GetCount() == 3)
			break;
	}

	if (discountGroup.GetCount() != 3)
	{
		for (ChargeShopCategory::iterator itr = noTimeRangeItem.begin(); itr != noTimeRangeItem.end(); ++itr)
		{
			itr->second->PreSend();
			discountGroup.Add(itr->second);
			if (discountGroup.GetCount() == 3)
				break;
		}		
	}

	haveTimeRangeItem.clear();
	noTimeRangeItem.clear();
	
	return discountGroup;
}

CChargeShopItem* CChargeShop::GetLimitedItem()
{
	if (m_limitedItem.size() > 0)
	{
		time_t nowTime;
		time(&nowTime);
		struct tm *newtime = localtime(&nowTime);
		ChargeShopCategory haveTimeRangeItem, noTimeRangeItem;
		for (ChargeShopCategory::iterator itr = m_limitedItem.begin(); itr != m_limitedItem.end(); ++itr)
		{
			if (itr->second->IsOnSaleDay() )
			{
				if (itr->second->GetStartTime() > 0 && itr->second->GetEndTime() > 0)
				{
					if (itr->second->GetStartTime() < (UInt32)nowTime && itr->second->GetEndTime() > (UInt32)nowTime)
						haveTimeRangeItem.insert(std::make_pair(itr->second->GetChargeShopID(), itr->second));					
				}
				else
					noTimeRangeItem.insert(std::make_pair(itr->second->GetChargeShopID(), itr->second));
			}
		}

		CChargeShopItem* temp = NULL;
		CChargeShopItem* temp1 = NULL;
		CChargeShopItem* temp2 = NULL;
		if (haveTimeRangeItem.size() > 0)
		{
			for (ChargeShopCategory::iterator itr = haveTimeRangeItem.begin(); itr != haveTimeRangeItem.end(); ++itr)
			{
				if (itr->second->GetLimitType() == ONE_TIME_ONLY)
				{
					if (temp == NULL)
					{
						temp = itr->second;
						break;
					}
				}
				else if (itr->second->GetLimitType() == newtime->tm_wday || (itr->second->GetLimitType() == EVERY_SUNDAY && newtime->tm_wday == 0))
				{
					if (temp1 == NULL)
						temp1 = itr->second;
				}
				else if (itr->second->GetLimitType() == EVERYDAY)
				{
					if (temp2 == NULL)
						temp2 = itr->second;
				}
			}						
		}
		else if (noTimeRangeItem.size() > 0)
		{
			for (ChargeShopCategory::iterator itr = noTimeRangeItem.begin(); itr != noTimeRangeItem.end(); ++itr)
			{
				if (itr->second->GetLimitType() == ONE_TIME_ONLY)
				{
					if (temp == NULL)
					{
						temp = itr->second;
						break;
					}
				}
				else if (itr->second->GetLimitType() == newtime->tm_wday || (itr->second->GetLimitType() == EVERY_SUNDAY && newtime->tm_wday == 0))
				{
					if (temp1 == NULL)
						temp1 = itr->second;
				}
				else if (itr->second->GetLimitType() == EVERYDAY)
				{
					if (temp2 == NULL)
						temp2 = itr->second;
				}
			}
		}

		haveTimeRangeItem.clear();
		noTimeRangeItem.clear();
		
		if (temp)
			return temp;
		else if (temp1)
			return temp1;
		else if (temp2)
			return temp2;
	}
	return NULL;
}

void CChargeShop::AddCharID(UInt32 charID, RPCContext context)
{
	CharContextList::iterator itr = m_charIDList.find(charID);
	if (itr == m_charIDList.end())
		m_charIDList.insert(std::make_pair(charID, context));
	else
		itr->second.sessionId = context.sessionId;
}

void CChargeShop::RemoveCharID(UInt32 charID)
{
	CharContextList::iterator itr = m_charIDList.find(charID);
	if (itr != m_charIDList.end())
		m_charIDList.erase(itr);
}

UInt8 CChargeShop::CheckCharCanBuy(CCharacter* pChar, CChargeShopItem* item, UInt16 stack, bool needReserve)
{
	if (pChar->GetScriptSystem()->IsRunning())
		return CHARGESHOP_RUN_SCRIPT;
	
	if (pChar->IsInBattle())
		return CHARGESHOP_CHAR_IN_BATTLE;

	if (item->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
	{
		const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItemId());
		if ( pItemData == NULL)
			return CHARGESHOP_NO_SELL_ITEM;

		if (!pChar->m_pBag->CanCreateLimitItem(item->GetItemId(), stack))
			return CHARGESHOP_ITEM_REACH_LIMIT;
		
		UInt noOfItem = stack / pItemData->overlap;
		UInt mod = stack % pItemData->overlap;
		if (pItemData->overlap > 1 && mod != 0)
			++noOfItem;
		
		if(noOfItem > pChar->m_pBag->GetTotalFreeSlot())
			return CHARGESHOP_NO_ITEM_SLOT;

		if (needReserve)
		{
			for (Index i = C_INDEX(0); i < noOfItem; ++i)
			{
				Boolean bFinishBuy = BOOLEAN_FALSE;
				for ( Index j = C_INDEX(BAG_CURRENT); j < BAG_TYPE_COUNT; ++j )
				{
					if (pChar->m_pBag->AddItem(DUMMY_ITEM_ADDRESS, j))
					{
						bFinishBuy = BOOLEAN_TRUE;
						break;
					}
				}
				if (!bFinishBuy)
				{
					pChar->m_pBag->RemoveDummyItem(i + 1);
					return CHARGESHOP_NO_ITEM_SLOT;
				}
			}
		}
	}
	else if (item->GetItemType() == CHARGE_ITEM_TYPE_PARTNER)
	{
		if (pChar->GetMaxPartner() <= pChar->m_pPartnerGroup->GetCount(true))
			return CHARGESHOP_NO_PARTNER_SLOT;
		//make sure the mob is existed
		const MobData* pData = gGlobal.m_resource->GetMobData(item->GetItemId());
		if (pData == NULL)
			return CHARGESHOP_NO_SELL_ITEM;
	}
	else if (item->GetItemType() == CHARGE_ITEM_TYPE_PET)
	{
		if (pChar->GetMaxPet() <= pChar->m_pPetGroup->GetCount(true))
			return CHARGESHOP_NO_PET_SLOT;		
		//make sure the pet is existed
		const PetData* pData = gGlobal.m_resource->GetPetData(item->GetItemId());
		if (pData == NULL)
			return CHARGESHOP_NO_SELL_ITEM;					
	}
	else
		return CHARGESHOP_NO_SELL_ITEM;

	return CHARGESHOP_BUY_SUCCESS;
}

RPCResult CChargeShop::BuyItemInChargeShop(LPCPEER pPeer, RPCContext &context, const UInt32 &chargeShopID, const UInt16 &stack)
{
	PROCEDURE_START_RPC2(ChargeShop, BuyItemInChargeShop, this, UInt32, chargeShopID, UInt16, stack)

		PROCEDURE_WAIT_TIMEOUT(1, 2000) //delay 2 second
		
		time_t nowTime;
		time(&nowTime);
		UInt8 msgCode = CHARGESHOP_BUY_SUCCESS;
		ChargeShopCategory::iterator itr = m_AllItem.find(chargeShopID);
		bool needCheck = false;
		if (itr != m_AllItem.end())
		{
			if (itr->second->GetShopType() == DISCOUNT)
			{
				if (itr->second->GetStartTime() > 0 && itr->second->GetEndTime() > 0 && 
					(itr->second->GetStartTime() > (UInt32)nowTime || itr->second->GetEndTime() < (UInt32)nowTime))
				{
					msgCode = CHARGESHOP_NOT_SALEDAY;
				}
			}
			else if (itr->second->GetShopType() == LIMITED )
			{
				if ((itr->second->GetStartTime() > 0 && itr->second->GetEndTime() > 0 && 
					(itr->second->GetStartTime() > (UInt32)nowTime || itr->second->GetEndTime() < (UInt32)nowTime)) || 
					(!itr->second->IsOnSaleDay()) )
				{
					msgCode = CHARGESHOP_NOT_SALEDAY;
				}				
				else
					needCheck = true;
			}
		}
		else
			msgCode = CHARGESHOP_NO_SELL_ITEM;

		Session *psess = GetSession(context.sessionId);
		if (psess == NULL || psess->pchar == NULL)
			msgCode = CHARGESHOP_CAHR_ERROR;
		else
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("BuyItem: session %s char_id %d start time %d"), context.sessionId.GetString().c_str(), psess->pchar->GetChar_id(), (UInt32)nowTime);	

		if (psess && psess->pchar) // check if the yuanbao is locked
		{
			CCharacter *pchar = psess->pchar;
			if (pchar->GetYuanbaoLock())
			{
				msgCode = CHARGESHOP_YUANBAO_LOCK_ERROR;
			}
		}
		

		if (needCheck && msgCode == CHARGESHOP_BUY_SUCCESS)
			WorldService_Proxy::CheckCanBuyLimitedItem(gGlobal.pWorldService->pPeer, context, chargeShopID, stack, psess->pchar->GetChar_id());
		else
		{
			PROCEDURE_GO_STEP1(2, UInt8, msgCode)
		}

	PROCEDURE_WAIT1(2, WorldService, CB_CheckCanBuyLimitedItem, UInt8, msgCode)

		ChargeShopCategory::iterator itr = m_AllItem.find(chargeShopID);
		UInt32 totalPrice = 0;
		UInt32 acct_id = 0;
		UInt32 char_id = 0;
		bool isLimited = false;
		if (msgCode == CHARGESHOP_BUY_SUCCESS )
		{
			do
			{
				Session *psess = GetSession(context.sessionId);
				if (psess == NULL || psess->pchar == NULL)
				{
					msgCode = CHARGESHOP_CAHR_ERROR;
					break;
				}

				CCharacter* pChar = psess->pchar;
				if (itr == m_AllItem.end())
				{
					msgCode = CHARGESHOP_NO_SELL_ITEM;
					break;
				}

				if (itr->second->GetShopType() == LIMITED)
					isLimited = true;

				msgCode = CheckCharCanBuy(pChar, itr->second, stack, true);

				if (msgCode == CHARGESHOP_BUY_SUCCESS)
				{
					totalPrice = stack * itr->second->GetYuanBao();
					if (itr->second->GetShopType() == DISCOUNT)
						totalPrice = stack * (itr->second->GetYuanBao() * itr->second->GetDiscount() / 100);
					TRACE_INFODTL_5(GLOBAL_LOGGER, _F("BuyChargeShopItem: sessionId %s, acct_id %d, char_id %d, exist yuanbao %d, used %d"), 
						context.sessionId.GetString().c_str(), pChar->GetAcct_id(), pChar->GetChar_id(), pChar->GetYuanBao(), totalPrice);
					acct_id = pChar->GetAcct_id();
					char_id = pChar->GetChar_id();
				}
			}while(false);

			
			if (isLimited && msgCode != CHARGESHOP_BUY_SUCCESS) //to reduce the pre buy therefore no need use a char_id
				WorldService_Proxy::ReduceLimitItem(gGlobal.pWorldService->pPeer, context, chargeShopID, BOOLEAN_FALSE, stack, 0); 
		}

		if (msgCode != CHARGESHOP_BUY_SUCCESS || acct_id == 0)
		{
			CB_BuyItemInChargeShop(pPeer, context, BOOLEAN_FALSE, msgCode, chargeShopID, 0, stack, 0);
			PROCEDURE_RETURN
		}
		else
		{
			ObjectService_Proxy::UpdateYuanBao(gGlobal.pObjectService->pPeer, context, acct_id, -(Int)totalPrice);
		}

		UInt32 finish = ::timeGetTime();
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("BuyItem: reduce yuanbao session %s char_id %d start time %d"), context.sessionId.GetString().c_str(), char_id, finish / 1000);	


	PROCEDURE_WAIT2(3, ObjectService, CB_UpdateYuanBao, UInt32, yuanbao, Boolean, buySuccess)

		Session *psess = GetSession(context.sessionId);
		CCharacter* pChar = NULL;
		CChargeShopItem* item = NULL;
		UInt8 msgCode = CHARGESHOP_BUY_SUCCESS;
		ChargeShopCategory::iterator itr = m_AllItem.find(chargeShopID);
		if (psess && (pChar = psess->pchar) && itr != m_AllItem.end())
		{
			UInt32 finish = ::timeGetTime();
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("BuyItem: reduce yuanbao service session %s char_id %d end time %d"), context.sessionId.GetString().c_str(), pChar->GetChar_id(), finish / 1000);	

			item = itr->second;
			pChar->SetYuanBao(yuanbao);
			if (buySuccess)
			{
				UInt32 totalPrice = stack * item->GetYuanBao();
				if (item->GetShopType() == DISCOUNT)
					totalPrice = stack * (item->GetYuanBao() * item->GetDiscount() / 100);
				LogService_Proxy::LogYuanBao(gGlobal.pLogService->pPeer, context, 
					pChar->GetAcct_id(), pChar->GetChar_id(), 
					LogService_Proxy::YuanBao::USEIN_CHARGESHOP, totalPrice);
				BuyItem(pChar, item, stack);

				time_t nowTime;
				time(&nowTime);
				
				item->SetSellInfo(itr->second->GetTotalSell() + stack);
				item->SetLastOrderInfo((UInt32)nowTime);
				RaiseUpdate();

				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("charID %d buy the chargeShopItemType %d and itemID %d "), 
					pChar->GetChar_id(), item->GetItemType(), item->GetItemId() );
				
				LogService_Proxy::LogChargeShop(gGlobal.pLogService->pPeer, context, 
					pChar->GetAcct_id(), pChar->GetChar_id(), item->GetItemId(), 
					stack, item->GetItemType(), totalPrice);
				CB_BuyItemInChargeShop(pPeer, context, BOOLEAN_TRUE, CHARGESHOP_BUY_SUCCESS, 
					item->GetItemId(), item->GetItemType(), stack, totalPrice);
			}
			else
			{
				ReleaseReservedItem(pChar, item, stack);
				msgCode = CHARGESHOP_NO_YUANBAO;
			}

			if (item->GetShopType() == LIMITED)
				WorldService_Proxy::ReduceLimitItem(gGlobal.pWorldService->pPeer, context, item->GetChargeShopID(), buySuccess, stack, pChar->GetChar_id());
		}
		else
			msgCode = CHARGESHOP_CAHR_ERROR;

		if (msgCode != CHARGESHOP_BUY_SUCCESS )
			CB_BuyItemInChargeShop(pPeer, context, BOOLEAN_FALSE, msgCode, chargeShopID, 0, stack, 0);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("BuyChargeShopItem Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

			Session *psess = GetSession(context.sessionId);
			CCharacter* pChar = NULL;
			CChargeShopItem* item = NULL;
			ChargeShopCategory::iterator itr = m_AllItem.find(chargeShopID);
			if (psess && (pChar = psess->pchar) && itr != m_AllItem.end())
			{
				item = itr->second;
				ReleaseReservedItem(pChar, item, stack);
				CB_BuyItemInChargeShop(pPeer, context, BOOLEAN_FALSE, CHARGESHOP_NO_SELL_ITEM, chargeShopID, 0, stack, 0);
			}

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

void CChargeShop::BuyItem(CCharacter* pChar, CChargeShopItem* item, UInt16 stack)
{
	if (pChar && item)
	{
		if (item->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
		{
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItemId());
			if (pItemData)
			{
				UInt noOfItem = stack / pItemData->overlap;
				UInt mod = stack % pItemData->overlap;
				if (pItemData->overlap > 1 && mod != 0)
					++noOfItem;
				
				UInt remaincount = stack;
				for (Index i = C_INDEX(0); i < noOfItem; ++i)
				{
					UInt add = 0;
					if ( remaincount > pItemData->overlap )
						add = pItemData->overlap;
					else
						add = remaincount;

					CItem* pNewItem = pChar->m_pBag->NewItem(item->GetItemId(), add, false);
					if ( pNewItem )
					{
						pChar->m_pBag->RemoveDummyItem(1);
						bool canAdd = false;
						for (Index i = C_INDEX(0); i < BAG_TYPE_COUNT; ++i)
						{
							if (pChar->m_pBag->AddItem(pNewItem, i))
							{
								canAdd = true;
								break;
							}
						}
						if (!canAdd)
							DeleteNetObject(pNewItem);
					}
					remaincount -= add;
				}
				pChar->m_pBag->RaiseUpdate();
			}
		}
		else if (item->GetItemType() == CHARGE_ITEM_TYPE_PARTNER)
		{
			const MobData* pData = gGlobal.m_resource->GetMobData(item->GetItemId());
			if (pData)
			{
				CPartner* partner = CPartner::NewInstance();
				partner->SetId(0, item->GetItemId());
				if (pData->partner_type == LEVEL_NOT_CHANGE_FOLLOW_OWNER)
					partner->SetLevel(pChar->GetLevel() + 5);
				else if (pData->partner_type == LEVEL_FOLLOW_OWNER)
					partner->SetLevel(pChar->GetLevel());
				else
					partner->SetLevel(item->GetLV());
				partner->SetOwner(pChar->GetChar_id());
				if (pChar->m_pPartnerGroup->AddNewPartner(partner)) 
					pChar->m_pPartnerGroup->RaiseUpdate();
				else
					DeleteNetObject(partner);	
			}
		}
		else if (item->GetItemType() == CHARGE_ITEM_TYPE_PET)
		{
			const PetData* pData = gGlobal.m_resource->GetPetData(item->GetItemId());
			if (pData)
			{
				CPet* pet = CPet::NewInstance();
				pet->SetId(0, item->GetItemId());
				pet->SetLevel(item->GetLV());
				pet->SetOwner_id(pChar->GetChar_id());
				pet->SetFirstMaster(pChar->GetNickName());
				if (pChar->m_pPetGroup->AddNewPet(pet)) 
					pChar->m_pPetGroup->RaiseUpdate();
				else
					DeleteNetObject(pet);	
			}
		}
	}
}

void CChargeShop::UpdateLimitedItem(CChargeShopData* pData)
{
	if (pData)
	{
		ChargeShopCategory::iterator itr = m_limitedItem.find(pData->GetChargeShopID());
		if (itr != m_limitedItem.end())
		{
			itr->second->SetData(pData, false);
			if (itr->second->IsUpdated(CChargeShopItem::remainInfoMask) && itr->second->IsOnSaleDay())
			{
				UInt32Vector removeCharID;
				for (CharContextList::iterator iter = m_charIDList.begin(); iter != m_charIDList.end(); ++iter)
				{
					Session *psess = GetSession(iter->second.sessionId);
					if (psess && psess->pchar && psess->pchar->GetPeer())
						itr->second->SendAttr(psess->pchar->GetPeer(), psess->pchar->GetContext(), CChargeShopItem::remainInfoMask);
					else
					{
						removeCharID.push_back(iter->first);
						UnsetOwnObject(iter->second.sessionId, this);
					}
				}

				for(Index i = C_INDEX(0); i < removeCharID.size(); ++i)
					RemoveCharID(removeCharID[i]);
			}
		}
	}
}

void CChargeShop::ReleaseReservedItem(CCharacter* pChar, CChargeShopItem* item, UInt16 stack)
{
	if (pChar && item)
	{
		if (item->GetItemType() == CHARGE_ITEM_TYPE_ITEM)
		{
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(item->GetItemId());
			if (pItemData)
			{
				UInt noOfItem = stack / pItemData->overlap;
				UInt mod = stack % pItemData->overlap;
				if (pItemData->overlap > 1 && mod != 0)
					++noOfItem;
				pChar->m_pBag->RemoveDummyItem(noOfItem);
			}
		}
	}
}

void CChargeShop::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_OBJECT:
		{
			PROCEDURE_DO_METHOD(CChargeShop, UpdateToObject, this, RPCContext())
		}
		break;
	}
}

RPCResult CChargeShop::UpdateToObject(RPCContext &context)
{
	PROCEDURE_START_METHOD(CChargeShop, UpdateToObject, this, context)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("CChargeShop UpdateToObject:"));		

		NetGroup grp;
		CChargeShopData* pData = NULL;
		CChargeShopItem* pItem = NULL;
		for (ChargeShopCategory::iterator itr = m_AllItem.begin(); itr != m_AllItem.end(); ++itr)
		{
			pItem = itr->second;
			if (pItem && pItem->IsUpdated() && pItem->GetShopType() != LIMITED)
			{
				pData = CreateNetObject<CChargeShopData>();
				pItem->WriteData(pData);
				if (pData->IsUpdated())
				{
					pData->SetSendMask(pData->GetUpdateMask());
					grp.Add(pData);
				}
			}
		}

		if (grp.GetCount() == 0)
		{
			PROCEDURE_GO_STEP(1)
		}

		ObjectService_Proxy::UpdateChargeShop(gGlobal.pObjectService->pPeer, context, &grp, BOOLEAN_TRUE);

		DeleteNetChilds(&grp);			// delete data object after send

		PROCEDURE_WAIT(1, ObjectService, CB_UpdateChargeShopSuccess)


	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CQuestSystem::UpdateQuests Error: ObjectService:CB_Error, errcode %d, %s"),
			err, errMsg.c_str());

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}
