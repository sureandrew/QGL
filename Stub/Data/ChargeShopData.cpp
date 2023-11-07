//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/ChargeShopData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboChargeShop.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"


REGISTER_CLASSTYPE(CChargeShopData, ChargeShopData_CLASSID);

CChargeShopData* NewChargeShopItem();
CdboChargeShop* &pdbChargeShop = gGlobal.gTables.pChargeShop;
StringVector CdboChargeShopAccessor::m_params;
ThreadKey CChargeShopData::m_key;
typedef WorkRequest<UInt8> ChargeShopRequest;

////////////////////////////////////////////////////////////////////////
////						ChargeShop Data							////
////////////////////////////////////////////////////////////////////////

CChargeShopData* NewChargeShopItem()
{
	CChargeShopData *pChargeShopData = NULL;
	UInt32 chargeShopID = (UInt32) pdbChargeShop->m_ChargeShopID.LowPart;
	DataObject &dobj = gGlobal.chargeShopIdMap[chargeShopID];
	if (dobj.pobj == NULL) {
		pChargeShopData = CChargeShopData::NewInstance();
		pChargeShopData->SetData(pdbChargeShop);			// assign values to chargetShop data
		dobj.pobj = pChargeShopData;
	} else {
		pChargeShopData = CastNetObject<CChargeShopData>(dobj.pobj);
	}

	return pChargeShopData;
}

CChargeShopData* FindChargeShopItem(UInt32 chargeShopID)
{
	DataIdMap::iterator iter = gGlobal.chargeShopIdMap.find(chargeShopID);
	CChargeShopData* pChargeShopData = NULL;

	// found existing account in cache
	if (iter != gGlobal.chargeShopIdMap.end())
		pChargeShopData = CastNetObject<CChargeShopData>(iter->second.pobj);

	return pChargeShopData;
}

HRESULT RemoveChargeShopItem(UInt32 chargeShopID)
{
	DataIdMap::iterator iter = gGlobal.chargeShopIdMap.find(chargeShopID);
	if (iter != gGlobal.chargeShopIdMap.end())
	{
		CChargeShopData* pChargeShopData = CastNetObject<CChargeShopData>(iter->second.pobj);
		gGlobal.DequeueUpdate(&iter->second);
		if (pChargeShopData)
		{
			// directly remove coz of primary constrain
			//gGlobal.m_deleteGroup.Add(pChargeShopData);
			DeleteNetObject(pChargeShopData);
		}
		gGlobal.chargeShopIdMap.erase(iter);
	}

	return (pdbChargeShop->Delete(chargeShopID));
}

void ReleaseChargeShopItem(UInt32 chargeShopID)
{
	DataIdMap::iterator iter = gGlobal.chargeShopIdMap.find(chargeShopID);
	CChargeShopData* pChargeShopData = NULL;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ReleaseChargeShopItem: chargeShopID %d"), chargeShopID);

	// found existing account in cache
	if (iter != gGlobal.chargeShopIdMap.end())
	{
		pChargeShopData = CastNetObject<CChargeShopData>(iter->second.pobj);
		gGlobal.DequeueUpdate(&iter->second);
		gGlobal.chargeShopIdMap.erase(iter);
		if (pChargeShopData) {
			if (pChargeShopData->IsUpdated())
				gGlobal.m_releaseGroup.Add(pChargeShopData);
			else
				DeleteNetObject(pChargeShopData);
		}
	}
}

void CChargeShopData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

DWORD WINAPI QueueGetChargeShops(LPVOID lpParam)
{
	HRESULT hr = S_OK;

	ChargeShopRequest* pRequest = (ChargeShopRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup shopGroup;
		CChargeShopData* pChargeShopData = NULL;
		CdboChargeShop* dboChargeShop = SafeCreateObject(CdboChargeShop);

		shopGroup.SetValidate(BOOLEAN_FALSE);
		dboChargeShop->SetDataSource(dbSrc);
		if (pRequest->key == GETALL)
			hr = dboChargeShop->RSByAll();
		else if (pRequest->key == GETACTIVEONLY)
			hr = dboChargeShop->RSByActive();

		if (DB_SUCCEEDED(hr))
		{
			hr = dboChargeShop->MoveFirst();
			while (hr == S_OK)
			{
				pChargeShopData = SafeCreateObject(CChargeShopData);
				pChargeShopData->SetData(dboChargeShop);
				pChargeShopData->SetSendMask(CChargeShopData::AllMask);
				shopGroup.Add(pChargeShopData);
				hr = dboChargeShop->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetChargeShops"), hr, dboChargeShop->GetErrorMsg(hr), 
			pRequest->context, &shopGroup);
		dboChargeShop->Close();
		SafeDeleteObject(dboChargeShop);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, key %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CChargeShopData::SetData(CdboChargeShop* dboChargeShop)
{
	SetIdInfo((UInt32)dboChargeShop->m_ChargeShopID.LowPart);
	SetItemInfo( dboChargeShop->m_Name, (UInt8)dboChargeShop->m_Type,
		(UInt8)dboChargeShop->m_ItemType, (UInt32)dboChargeShop->m_ItemID.LowPart, (UInt32)dboChargeShop->m_YuanBao.LowPart,
		(UInt8)dboChargeShop->m_LV, (UInt8)dboChargeShop->m_PreviewType);
	SetActiveInfo((UInt8)dboChargeShop->m_Active);
	SetDiscountInfo((UInt8)dboChargeShop->m_Discount);
	SetStockInfo((UInt16)dboChargeShop->m_TotalStock);
	SetRemainInfo((UInt16)dboChargeShop->m_Remain);
	time_t startTime, endTime;
	TimeConvert(dboChargeShop->m_StartTime, startTime);
	TimeConvert(dboChargeShop->m_EndTime, endTime);
	SetExistTimeInfo((UInt32)startTime, (UInt32)endTime);
	SetLimitedInfo((UInt8)dboChargeShop->m_LimitType, (UInt8)dboChargeShop->m_NeedRestore);
	time_t uTime;
	TimeConvert(dboChargeShop->m_LastOrderDate, uTime);
	if (uTime < 0)
		uTime = 0;
	SetLastOrderInfo((UInt32)uTime);
	SetSellInfo((UInt32)dboChargeShop->m_TotalSell.LowPart);
	ClearUpdateMask();
}

HRESULT CChargeShopData::UpdateDbo(CdboChargeShop* dboChargeShop)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	//TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CChargeShopData UpdateDbo: ChargeShopID %d updateMask %d,%d"), 
	//	GetChargeShopID(), m_updateMask.low, m_updateMask.high);

	dboChargeShop->ClearParam();

	if (IsUpdated(idInfoMask))
	{
		COLUMN_UPDATE_LARGE(dboChargeShop, ChargeShopID, GetChargeShopID());
	}

	if (IsUpdated(itemInfoMask))
	{
		
		COLUMN_UPDATE_STRING(dboChargeShop, Name, GetName());
		COLUMN_UPDATE(dboChargeShop, Type, GetShopType());
		COLUMN_UPDATE(dboChargeShop, ItemType, GetItemType());
		COLUMN_UPDATE_LARGE(dboChargeShop, ItemID, GetItemId());
		COLUMN_UPDATE_LARGE(dboChargeShop, YuanBao, GetYuanBao());
		COLUMN_UPDATE(dboChargeShop, LV, GetLV());
		COLUMN_UPDATE(dboChargeShop, PreviewType, GetPreviewType());
	}

	if (IsUpdated(activeInfoMask))
	{
		COLUMN_UPDATE(dboChargeShop, Active, GetActive());
	}

	if (IsUpdated(stockInfoMask))
	{
		COLUMN_UPDATE(dboChargeShop, TotalStock, GetTotalStock());
	}

	if (IsUpdated(remainInfoMask))
	{
		COLUMN_UPDATE(dboChargeShop, Remain, GetRemain());
	}

	if (IsUpdated(discountInfoMask))
	{
		COLUMN_UPDATE(dboChargeShop, Discount, GetDiscount());
	}

	if (IsUpdated(limitedInfoMask))
	{
		COLUMN_UPDATE(dboChargeShop, LimitType, GetLimitType());
		COLUMN_UPDATE(dboChargeShop, NeedRestore, GetRestore());
	}

	if (IsUpdated(existTimeInfoMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetStartTime(), dbtime);
		COLUMN_UPDATE(dboChargeShop, StartTime, dbtime);
		TimeConvert((time_t) GetEndTime(), dbtime);
		COLUMN_UPDATE(dboChargeShop, EndTime, dbtime);
	}

	if (IsUpdated(lastOrderInfoMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetLastOrderDate(), dbtime);
		COLUMN_UPDATE(dboChargeShop, LastOrderDate, dbtime);
	}

	if (IsUpdated(CChargeShopData::sellInfoMask))
	{
		COLUMN_UPDATE_LARGE(dboChargeShop, TotalSell, GetTotalSell());
	}

	HRESULT hr;
	if (DB_FAILED(hr = dboChargeShop->UpdateByAny(GetChargeShopID())))
		dboChargeShop->ShowError(hr);
	else
		ClearUpdateMask();
	dboChargeShop->ClearParam();
	dboChargeShop->Close();

	return hr;
}

HRESULT CChargeShopData::DeleteDbo(CdboChargeShop* dboChargeShop)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboChargeShop->Delete(GetChargeShopID())))
		dboChargeShop->ShowError(hr);
	dboChargeShop->Close();

	return hr;
}

RPCResult ObjectService_Stub::GetChargeShop(LPCPEER pPeer, RPCContext &context, const UInt8 &index)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetChargeShop, NULL, UInt8, index)

		TRACE_ENSURE(pdbChargeShop != NULL);
		// wait for thread pool to process
		gGlobal.WorkQueueBegin();
		ChargeShopRequest *pRequest = SafeCreateObject3(ChargeShopRequest, index, pPeer, context);
#ifdef DB_THREAD_POOL
		gGlobal.QueueUserWorkItem(QueueGetChargeShops, pRequest, DB_WORKER_FLAG);
#else
		QueueGetChargeShops(pRequest);
#endif

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, chargeShopGroup)

		TRACE_ENSURE(chargeShopGroup);

		// store new chargeShop group
		CChargeShopData* pChargeShopData;
		NetGroup sendGrp;
		for (UInt16 i = 0; i < chargeShopGroup->GetCount(); i++)
		{
			pChargeShopData = chargeShopGroup->GetChild<CChargeShopData>(i);
			if (pChargeShopData) {
				// need to register for new chargeShop data
				pChargeShopData->SetSendMask(CChargeShopData::AllMask);
				DataObject &dobj = gGlobal.chargeShopIdMap[pChargeShopData->GetChargeShopID()];
				if (dobj.pobj) {
					DeleteNetObject(pChargeShopData);
					pChargeShopData = CastNetObject<CChargeShopData>(dobj.pobj);
				} else {
					LocalRPCManager::GetInstance().Register(pChargeShopData);
					pChargeShopData->ClearUpdateMask();
					dobj.pobj = pChargeShopData;
				}
				sendGrp.Add(pChargeShopData);
			}
		}

		// send charge shop data to peer
		CB_GetChargeShop(pPeer, context, &sendGrp);
		DeleteNetGroup(chargeShopGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_T("GetChargeShop Error: sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH
		
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetCacheChargeShop(LPCPEER pPeer, RPCContext &context, const UInt8 &index)
{
	NetGroup shopGroup;
	for (DataIdMap::iterator itr = gGlobal.chargeShopIdMap.begin(); itr != gGlobal.chargeShopIdMap.end(); ++itr)
	{
		CChargeShopData* pChargeShopData = CastNetObject<CChargeShopData>(itr->second.pobj);
		if (pChargeShopData->GetActive())
		{
			if (index == GETLIMITONLY && pChargeShopData->GetShopType() != LIMITED)
				continue;

			pChargeShopData->SetSendMask(CChargeShopData::AllMask);
			shopGroup.Add(pChargeShopData);
		}
	}
	CB_GetChargeShop(pPeer, context, &shopGroup);

	return RPC_RESULT_OK;
}

HRESULT DoCreateChargeShopData(CChargeShopData* pChargeShopData)
{
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateChargeShop: chargeShop_id %d, item_id %d"), 
	//	pChargeShopData->GetChargeShopID(),	pChargeShopData->GetItemId());

	HRESULT hr = S_OK;

	//pdbChargeShop->m_ChargeShopID.QuadPart = pChargeShopData->GetChargeShopID();
	//COLUMN_STRING_COPY(pdbChargeShop->m_Name, pChargeShopData->GetName().c_str() );
	pdbChargeShop->m_Type = pChargeShopData->GetShopType();
	pdbChargeShop->m_ItemType = pChargeShopData->GetItemType();
	pdbChargeShop->m_ItemID.QuadPart = pChargeShopData->GetItemId();
	pdbChargeShop->m_YuanBao.QuadPart = pChargeShopData->GetYuanBao();
	pdbChargeShop->m_LV = pChargeShopData->GetLV();
	pdbChargeShop->m_PreviewType = pChargeShopData->GetPreviewType();
	pdbChargeShop->m_Active = pChargeShopData->GetActive();
	pdbChargeShop->m_Discount = pChargeShopData->GetDiscount();
	pdbChargeShop->m_TotalStock = pChargeShopData->GetTotalStock();
	pdbChargeShop->m_Remain = pChargeShopData->GetTotalStock();
	TimeConvert((time_t) pChargeShopData->GetStartTime(), pdbChargeShop->m_StartTime);
	TimeConvert((time_t) pChargeShopData->GetEndTime(), pdbChargeShop->m_EndTime);
	
	pdbChargeShop->m_LimitType = pChargeShopData->GetLimitType();
	pdbChargeShop->m_NeedRestore = pChargeShopData->GetRestore();

	hr = pdbChargeShop->Insert(pChargeShopData->GetChargeShopID(), pChargeShopData->GetName().c_str());
	
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbChargeShop->MoveFirst()))
	{
		// create partner object in object service
		NewChargeShopItem();
	}
	pdbChargeShop->Close();

	return hr;
}

RPCResult ObjectService_Stub::UpdateChargeShop(LPCPEER pPeer, RPCContext &context, 
											   NetGroup* chargeShop, 
											   const Boolean &needCallBack)
{
	TRACE_ENSURE(pdbChargeShop != NULL);

	HRESULT hr = S_OK;
	CChargeShopData* pNewData = NULL;
	DataIdMap::iterator iter;

	for (UInt32 i = 0; i < chargeShop->GetCount(); ++i)
	{
		pNewData = chargeShop->GetChild<CChargeShopData>(i);
		if (pNewData)
		{
			CChargeShopData* pChargeShopData = FindChargeShopItem(pNewData->GetChargeShopID());
			if (pChargeShopData == NULL)
			{
				hr = pdbChargeShop->RSByID(pNewData->GetChargeShopID());

				if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbChargeShop->MoveFirst()))
				{
					pChargeShopData = NewChargeShopItem();
					if (pChargeShopData)
					{
						if (pChargeShopData->GetItemId() == pNewData->GetItemId())
						{
							RPCMask mask;
							mask.MaskBits( CChargeShopData::sellInfoMask, CChargeShopData::lastOrderInfoMask, CChargeShopData::remainInfoMask , -1);
							pNewData->UnmarkUpdate(mask);
						}
						pChargeShopData->Clone(pNewData, pNewData->GetUpdateMask());
						iter = gGlobal.chargeShopIdMap.find(pChargeShopData->GetChargeShopID());

						if (iter != gGlobal.chargeShopIdMap.end())
							gGlobal.EnqueueUpdate(&iter->second);
					}
				}
				else
				{
					pdbChargeShop->Close();
					hr = DoCreateChargeShopData(pNewData) ;
					if (DB_FAILED(hr)) {
						TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UpdatePChargeShop DB_FAILED: chargeShopItem create failed, hr=%X"), hr);
						pdbChargeShop->ShowError(hr);
						CB_Error(pPeer, context, hr, _T("UpdateChargeShop DB_FAILED: chargeShopItem create failed"));
						break;
					}
				}	
				pdbChargeShop->Close();
			}
			else
			{
				pChargeShopData->Clone(pNewData, pNewData->GetUpdateMask());
				iter = gGlobal.chargeShopIdMap.find(pChargeShopData->GetChargeShopID());

				if (iter != gGlobal.chargeShopIdMap.end())
					gGlobal.EnqueueUpdate(&iter->second);
			}
		}
	}

	DeleteNetGroup(chargeShop, BOOLEAN_TRUE);

	if (needCallBack)
		CB_UpdateChargeShopSuccess(pPeer, context);

	return RPC_RESULT_OK;
}
