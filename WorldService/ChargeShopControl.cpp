//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "WorldService/ChargeShopControl.h"
#include "WorldService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResChargeShop.h"
#include "Proxy/Data/ChargeShopData.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Stub/Service/WorldService.h"
#include "Proxy/Service/LogService.h"

REGISTER_CLASSTYPE(CChargeShopControl, CChargeShopControl_CLASSID);

CChargeShopControl::CChargeShopControl()
{	
	m_limitItemList.clear();
	m_boughtLimitedCharID.clear();
}

void CChargeShopControl::InitChargeShop()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CChargeShopControl, InitChargeShop, this, context)
}

RPCResult CChargeShopControl::InitChargeShop(RPCContext &context)
{
	PROCEDURE_START_METHOD(CChargeShopControl, InitChargeShop, this, context)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("start GetChargeShop:"));

		ChargeShopDataList totalItemList = gGlobal.m_resource->GetChargeShopList();
		NetGroup addOrUpdate;

		for (UInt32 i = 0; i < totalItemList.size(); ++i)
		{
			const ChargeShopData* tempData = totalItemList[i];
			if (tempData)
			{
				CChargeShopData* itemData = CreateNetObject<CChargeShopData>();
				itemData->SetIdInfo(tempData->chargeShop_ID);
				itemData->SetItemInfo(String(tempData->chargeShop_ItemName), 
					tempData->chargeShop_Type, tempData->chargeShop_ItemType, tempData->chargeShop_ItemID, 
					tempData->chargeShop_Mmoney, tempData->chargeShop_LV, tempData->chargeShop_PreviewType);
				itemData->SetActiveInfo(tempData->chargeShop_Active);
				itemData->SetDiscountInfo(tempData->chargeShop_Discount);
				itemData->SetStockInfo(tempData->chargeShop_TotalStock);
				itemData->SetLimitedInfo(tempData->chargeShop_LimitType, tempData->chargeShop_Restore);
				itemData->SetRemainInfo(tempData->chargeShop_TotalStock);
				String startTimeStr = String(tempData->chargeShop_StartTime);
				String endTimeStr = String(tempData->chargeShop_EndTime);

				if ((itemData->GetShopType() == DISCOUNT || itemData->GetShopType() == LIMITED) &&
					(startTimeStr.length() == 14 && endTimeStr.length() == 14) )
				{
					time_t startTime, endTime;
					
					startTime = ConvertStringToTime(startTimeStr);
					endTime = ConvertStringToTime(endTimeStr);

					itemData->SetExistTimeInfo((UInt32)startTime, (UInt32)endTime);
				}
				else
					itemData->SetExistTimeInfo(0, 0);
				
				itemData->SetLastOrderInfo(0);
				itemData->SetSellInfo(0);
				itemData->SetSendMask(CChargeShopData::AllMask);
				addOrUpdate.Add(itemData);
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CChargeShopData Class ID: %d"), itemData->GetClassID());
				if (addOrUpdate.GetCount() > 20)
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F("start UpdateChargeShop:"));
					ObjectService_Proxy::UpdateChargeShop(gGlobal.pObjectService->pPeer, context,&addOrUpdate, BOOLEAN_FALSE);
					DeleteNetChilds(&addOrUpdate);
				}
			}
		}

		if (addOrUpdate.GetCount() > 0)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("start UpdateChargeShop:"));
			ObjectService_Proxy::UpdateChargeShop(gGlobal.pObjectService->pPeer, context,&addOrUpdate, BOOLEAN_TRUE);
		}
		else
		{
			PROCEDURE_GO_STEP(2)
		}

		DeleteNetChilds(&addOrUpdate);

	PROCEDURE_WAIT(2, ObjectService, CB_UpdateChargeShopSuccess)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("finish update and  GetChargeShop again:"));
		ObjectService_Proxy::GetCacheChargeShop(gGlobal.pObjectService->pPeer, context, GETLIMITONLY);

	PROCEDURE_WAIT1(3, ObjectService, CB_GetChargeShop, NetGroup*, chargeShopDataGroup)

		TRACE_INFODTL(GLOBAL_LOGGER, _F("GetChargeShop and divide it to different list:"));
		for (UInt32 i = 0; i < chargeShopDataGroup->GetCount(); ++i)
		{
			CChargeShopData* pChargeShopData = chargeShopDataGroup->GetChild<CChargeShopData>(i);
			if (pChargeShopData)
			{	
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CChargeShopData ClassID %d, ChargeShopID %d "), 
					pChargeShopData->GetClassID(), pChargeShopData->GetChargeShopID());
				LimitItemList::iterator itfind = m_limitItemList.find(pChargeShopData->GetChargeShopID());
				if (itfind != m_limitItemList.end()) {
					DeleteNetObject(itfind->second);
					m_limitItemList.erase(itfind);
				}
				m_limitItemList.insert(std::make_pair(pChargeShopData->GetChargeShopID(), pChargeShopData));//.push_back(item);
			}
		}
		
		time_t nowTime;
		time(&nowTime);
		struct tm *newtime = localtime(&nowTime);
		if (newtime)
		{
			UInt32 ms = (60 - newtime->tm_min) * 60 * 1000;
			RaiseUniqEventID(EVT_UPDATE_LIMITEDITEM, ms);
			UInt32 hour = 23 - newtime->tm_hour;

			RaiseUniqEventID(EVT_UPDATE_CLEAR_BOUGHT_LIMITED_CHAR_ID, (hour * 3600 * 1000 + ms)); 
		}
		else
		{
			RaiseUniqEventID(EVT_UPDATE_LIMITEDITEM, 3600000);
			RaiseUniqEventID(EVT_UPDATE_CLEAR_BOUGHT_LIMITED_CHAR_ID, 86400000); //86400000 ms = 1 day
		}
		
		DeleteNetGroup(chargeShopDataGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CChargeShopControl InitChargeShop Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

time_t CChargeShopControl::ConvertStringToTime(String timeStr)
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

UInt8 CChargeShopControl::CanBuyLimitedItem(UInt32 chargeShopID, UInt16 stack, UInt32 char_id)
{
	if (find(m_boughtLimitedCharID.begin(), m_boughtLimitedCharID.end(), char_id) != m_boughtLimitedCharID.end())
		return CHARGESHOP_BOUGHT_LIMIT_ALREADY;

	LimitItemList::iterator itr = m_limitItemList.find(chargeShopID);
	if (itr != m_limitItemList.end() && itr->second->GetRemain() > 0 && 
		(itr->second->GetRemain() + itr->second->GetPreBuy()) >= stack )
	{
		CChargeShopData* temp = itr->second;
		itr->second->AddPreBuy(stack);
		return CHARGESHOP_BUY_SUCCESS;
	}

	return CHARGESHOP_NO_REMAIN;
}

void CChargeShopControl::ReduceLimitItem(UInt32 chargeShopID, Boolean buySuccess, UInt16 reduceStack, UInt32 char_id)
{
	LimitItemList::iterator itr = m_limitItemList.find(chargeShopID);
	if (itr != m_limitItemList.end())
	{
		CChargeShopData* temp = itr->second;
		itr->second->ReducePreBuy(reduceStack);
		if (buySuccess)
		{
			m_boughtLimitedCharID.push_back(char_id);
			itr->second->SetRemain(itr->second->GetRemain() - reduceStack);
			time_t nowTime;
			time(&nowTime);
			
			itr->second->SetSellInfo(itr->second->GetTotalSell() + reduceStack);
			itr->second->SetLastOrderDate((UInt32)nowTime);

			CChargeShopData* pData = CreateNetObject<CChargeShopData>();
			pData->SetSellInfo(itr->second->GetTotalSell());
			pData->SetLastOrderInfo(itr->second->GetLastOrderDate());
			pData->SetRemainInfo(itr->second->GetRemain());
			pData->SetChargeShopID(itr->second->GetChargeShopID());

			pData->SetSendMask(pData->GetUpdateMask());
			NetGroup grp;
			grp.Add(pData);

			RPCContext context;
			ObjectService_Proxy::UpdateChargeShop(gGlobal.pObjectService->pPeer, context, &grp, BOOLEAN_FALSE);

			LPCPEER pPeer;
			for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); it != gGlobal.m_mapPeers.end(); ++it)
			{
				pPeer = *it;
				MapService_Proxy::UpdateLimitedItemRemain(pPeer, context, &grp);
			}

			DeleteNetChilds(&grp);
		}
	}
}

void CChargeShopControl::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_LIMITEDITEM:				// update limitedItem every hour to object service
		{
			UpdateLimitedItem();
			RaiseUniqEventID(EVT_UPDATE_LIMITEDITEM, 3600000); //3600000 ms = 1 hour
		}
		break;
	case EVT_UPDATE_CLEAR_BOUGHT_LIMITED_CHAR_ID:
		{
			m_boughtLimitedCharID.clear();
			RaiseUniqEventID(EVT_UPDATE_CLEAR_BOUGHT_LIMITED_CHAR_ID, 86400000); //86400000 ms = 1 day
		}
		break;
	}
}

void CChargeShopControl::UpdateLimitedItem()
{
	NetGroup grp;
	time_t nowTime;
	time(&nowTime);
	struct tm *newtime = localtime(&nowTime);
	
	if (newtime)
	{
		int day = newtime->tm_mday;

		for (LimitItemList::iterator itr = m_limitItemList.begin(); itr != m_limitItemList.end(); ++itr)
		{
			if (itr->second->GetRestore() == 1 && itr->second->GetRemain() != itr->second->GetTotalStock() 
				&& itr->second->GetLimitType() != ONE_TIME_ONLY)
			{
				time_t temptime	= itr->second->GetLastOrderDate();
				struct tm *lasttime = localtime(&temptime);
				if (lasttime)
				{
					if ( ( itr->second->GetLimitType() == day || 
						(itr->second->GetLimitType() == EVERY_SUNDAY && day == 0) || 
						itr->second->GetLimitType() == EVERYDAY ) && 
						day != lasttime->tm_mday)
					{
						itr->second->SetRemainInfo(itr->second->GetTotalStock());
						itr->second->ResetPreBuy();
						CChargeShopData* pData = CreateNetObject<CChargeShopData>();						
						pData->SetRemainInfo(itr->second->GetRemain());
						pData->SetChargeShopID(itr->second->GetChargeShopID());
						pData->SetSendMask(pData->GetUpdateMask());
						grp.Add(pData);

					}
				}
			}
		}
	}

	if (grp.GetCount() > 0)
	{
		RPCContext context;
		ObjectService_Proxy::UpdateChargeShop(gGlobal.pObjectService->pPeer, context, &grp, BOOLEAN_FALSE);

		LPCPEER pPeer;
		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); it != gGlobal.m_mapPeers.end(); ++it)
		{
			pPeer = *it;
			MapService_Proxy::UpdateLimitedItemRemain(pPeer, context, &grp);
		}

		DeleteNetChilds(&grp);
	}
}

CChargeShopControl::~CChargeShopControl()
{
	for (LimitItemList::iterator itr = m_limitItemList.begin(); itr != m_limitItemList.end(); ++itr)
	{
		DeleteNetObject(itr->second);
	}
	m_limitItemList.clear();
	m_boughtLimitedCharID.clear();
}