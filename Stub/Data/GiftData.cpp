//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/GiftData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboGift.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"

REGISTER_CLASSTYPE(CGiftData, GiftData_CLASSID);

CdboGift* &pdbGift = gGlobal.gTables.pGift;
StringVector CdboGiftAccessor::m_params;
ThreadKey CGiftData::m_key;
//typedef WorkRequest<UInt32> GiftRequest;

struct GiftRequest : public WorkRequest<UInt32>
{
	UInt32 giftID;
	GiftRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
	{}
};

#define MAX_GIFT_PER_TYPE 48

////////////////////////////////////////////////////////////////////////
////						Gift Data								////
////////////////////////////////////////////////////////////////////////


CGiftData* FindGift(UInt32 giftID)
{
	DataIdMap::iterator iter = gGlobal.giftIdMap.find(giftID);
	CGiftData* pGiftData = NULL;

	// found existing account in cache
	if (iter != gGlobal.giftIdMap.end())
		pGiftData = CastNetObject<CGiftData>(iter->second.pobj);

	return pGiftData;
}

NetGroup* FindGiftGroup(UInt8 type, UInt32 acct_id)
{
	NetGroup* grp = NULL;
	switch (type)
	{
	case GIFT_ITEM:
		{
			GroupMap::iterator iter = gGlobal.existItemGift.find(acct_id);	
			if (iter != gGlobal.existItemGift.end())
				grp = CastNetObject<NetGroup>(iter->second.pobj);
		}
		break;
	case GIFT_PARTNER:
		{
			GroupMap::iterator iter = gGlobal.existPartnerGift.find(acct_id);	
			if (iter != gGlobal.existPartnerGift.end())
				grp = CastNetObject<NetGroup>(iter->second.pobj);
		}
		break;
	case GIFT_PET:
		{
			GroupMap::iterator iter = gGlobal.existPetGift.find(acct_id);	
			if (iter != gGlobal.existPetGift.end())
				grp = CastNetObject<NetGroup>(iter->second.pobj);
		}
		break;
	}			
	
	return grp;
}

HRESULT RemoveGift(UInt32 giftID)
{
	DataIdMap::iterator iter = gGlobal.giftIdMap.find(giftID);
	if (iter != gGlobal.giftIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CGiftData* pGiftData = CastNetObject<CGiftData>(iter->second.pobj);
		if (pGiftData) {
			NetGroup *grp = FindGiftGroup(pGiftData->GetItemType(), pGiftData->GetAcctID());
			if (grp) 
				grp->Remove(pGiftData);
			gGlobal.giftDelMap.push_back(pGiftData->GetGiftID());
			gGlobal.m_deleteGroup.Add(pGiftData);
		}
		gGlobal.giftIdMap.erase(iter);
	}
	return S_OK;
}

void ReleaseGift(UInt32 acct_id)
{

	GroupMap::iterator iter;

	for (Index j = 0; j < 3; ++j)
	{
		if (j == 0)
		{
			iter = gGlobal.existItemGift.find(acct_id);
			if (iter == gGlobal.existItemGift.end())
				continue;
		}
		else if (j == 1)
		{
			iter = gGlobal.existPartnerGift.find(acct_id);
			if (iter == gGlobal.existPartnerGift.end())
				continue;
		}
		else if (j == 2)
		{
			iter = gGlobal.existPetGift.find(acct_id);
			if (iter == gGlobal.existPetGift.end())
				continue;
		}

		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CGiftData* pGiftData = NULL;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++) {
			pGiftData = grp->GetChild<CGiftData>(i);
			if (pGiftData) 
			{
				DataIdMap::iterator it = gGlobal.giftIdMap.find(pGiftData->GetGiftID());
				if (it != gGlobal.giftIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.giftIdMap.erase(it);
				}
				if (pGiftData->IsUpdated())
					gGlobal.m_releaseGroup.Add(pGiftData);
				else
					DeleteNetObject(pGiftData);
			}
		}

		if (j == 0)
			gGlobal.existItemGift.erase(iter);	
		else if (j == 1)
			gGlobal.existPartnerGift.erase(iter);
		else if (j == 2)
			gGlobal.existPetGift.erase(iter);
					
		SafeDeleteObject(grp);		
	}

	for (int i = 0; i < 3; ++i)
	{
		CGiftData* pGiftData = NULL;
		RemainGiftMap::iterator itr;
		if (i == 0)
		{
			itr = gGlobal.remainItemGift.find(acct_id);
			if (itr == gGlobal.remainItemGift.end())
				continue;
		}
		else if (i == 1)
		{
			itr = gGlobal.remainPartnerGift.find(acct_id);
			if (itr == gGlobal.remainPartnerGift.end())
				continue;
		}
		else if (i ==2)
		{
			itr = gGlobal.remainPetGift.find(acct_id);
			if (itr == gGlobal.remainPetGift.end())
				continue;
		}

		for (Index j = C_INDEX(0); j < itr->second.size(); ++j)
		{
			pGiftData = FindGift(itr->second[j]);
			if (pGiftData) 
			{
				DataIdMap::iterator it = gGlobal.giftIdMap.find(pGiftData->GetGiftID());
				if (it != gGlobal.giftIdMap.end()) {		// remove from update queue
					gGlobal.DequeueUpdate(&it->second);
					gGlobal.giftIdMap.erase(it);
				}
				if (pGiftData->IsUpdated())
					gGlobal.m_releaseGroup.Add(pGiftData);
				else
					DeleteNetObject(pGiftData);
			}
		}

		if (i == 0)
			gGlobal.remainItemGift.erase(itr);
		else if (i == 1)
			gGlobal.remainPartnerGift.erase(itr);
		else if (i == 2)
			gGlobal.remainPetGift.erase(itr);
	}	
}

HRESULT CGiftData::UpdateDbo(CdboGift* dboGift)
{
	return S_OK;
}

HRESULT CGiftData::DeleteDbo(CdboGift* dboGift)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	gGlobal.giftDelMap.erase(remove(gGlobal.giftDelMap.begin(), gGlobal.giftDelMap.end(), GetGiftID()));

	if (DB_FAILED(hr = dboGift->Delete(GetGiftID())))
		dboGift->ShowError(hr);
	dboGift->Close();

	return hr;
}

// set gift data detail in group
void PreSendGiftGroup(NetGroup * giftGroup)
{
	if (giftGroup == NULL) return;

	for (UInt16 i = 0; i < giftGroup->GetCount(); i++)
	{
		CGiftData *pGiftData = giftGroup->GetChild<CGiftData>(i);
		if (pGiftData)
			pGiftData->PreSend();
		else
			giftGroup->Replace(i, NULL);
	}
}

void PutRemainGiftToExist(UInt32 acct_id, NetGroup* sendGrp)
{
	for (Index j = C_INDEX(0); j < 3; ++j)
	{
		NetGroup* giftGroup = NULL;
		RemainGiftMap::iterator itr;
		RemainGiftMap::iterator endItr;
		switch (j)
		{
		case 0:
			{
				giftGroup = FindGiftGroup(GIFT_ITEM, acct_id);
				itr = gGlobal.remainItemGift.find(acct_id);
				endItr = gGlobal.remainItemGift.end();
			}
			break;
		case 1:
			{
				giftGroup = FindGiftGroup(GIFT_PARTNER, acct_id);
				itr = gGlobal.remainPartnerGift.find(acct_id);
				endItr = gGlobal.remainPartnerGift.end();
			}
			break;
		case 2:
			{
				giftGroup = FindGiftGroup(GIFT_PET, acct_id);
				itr = gGlobal.remainPetGift.find(acct_id);
				endItr = gGlobal.remainPetGift.end();
			}
			break;
		}
		if (giftGroup && giftGroup->GetCount() < MAX_GIFT_PER_TYPE && itr != endItr && itr->second.size() > 0)
		{
			UInt32Vector getList;
			UInt i = 0;
			CGiftData* pGiftData = NULL;
			for (Index i = C_INDEX(0); i < itr->second.size(); ++i)
			{
				pGiftData = FindGift(itr->second[i]);
				if (pGiftData)
				{
					giftGroup->Add(pGiftData);
					if (sendGrp)
					{
						pGiftData->PreSend();
						sendGrp->Add(pGiftData);
					}
					getList.push_back(pGiftData->GetGiftID());				
				}
				if (giftGroup->GetCount() >= MAX_GIFT_PER_TYPE)
					break;
			}

			for (Index i = C_INDEX(0); i < getList.size(); ++i)
				itr->second.erase(remove(itr->second.begin(), itr->second.end(), getList[i]));
		}
	}
}

DWORD WINAPI QueueGetGiftBoxs(LPVOID lpParam)
{
	HRESULT hr = S_OK;

	GiftRequest* pRequest = (GiftRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.acctDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource(); 
	if (dbSrc)
	{
		NetGroup giftGroup;
		CGiftData* pGiftData;
		CdboGift* dboGift = SafeCreateObject(CdboGift);

		giftGroup.SetValidate(BOOLEAN_FALSE);
		dboGift->SetDataSource(dbSrc);
		
		hr = dboGift->RSByAcctID(pRequest->key, pRequest->giftID);
		
		if (DB_SUCCEEDED(hr))
		{
			hr = dboGift->MoveFirst();
			while (hr == S_OK)
			{
				pGiftData = SafeCreateObject(CGiftData);
				pGiftData->SetData(dboGift);
				pGiftData->SetSendMask(CGiftData::AllMask);
				giftGroup.Add(pGiftData);
				hr = dboGift->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetGifts"), hr, dboGift->GetErrorMsg(hr), 
			pRequest->context, &giftGroup);
		dboGift->Close();
		SafeDeleteObject(dboGift);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, acct_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CGiftData::SetData(CdboGift* dboGift)
{
	SetIdInfo((UInt32) dboGift->m_gift_id.LowPart, (UInt32)dboGift->m_acct_id.LowPart);
	SetGiftInfo((UInt32)dboGift->m_item_id, (UInt8)dboGift->m_itemtype, (UInt8)dboGift->m_quantity);
	SetSerialNoInfo(dboGift->m_serialNo);
	ClearUpdateMask();
}

void CGiftData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

RPCResult ObjectService_Stub::GetGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, GetGiftBoxByAcctID, NULL, UInt32, acct_id)

		TRACE_ENSURE(pdbGift != NULL);

		NetGroup* itemGiftGroup = FindGiftGroup(GIFT_ITEM, acct_id);
		NetGroup* partnerGiftGroup = FindGiftGroup(GIFT_PARTNER, acct_id);
		NetGroup* petGiftGroup = FindGiftGroup(GIFT_PET, acct_id);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetGiftBoxByAcctID: acct_id %d"), acct_id);

		if (itemGiftGroup || partnerGiftGroup || petGiftGroup)
		{
			UInt giftCount = 0;
			if (itemGiftGroup)
				giftCount += itemGiftGroup->GetCount();
			if (partnerGiftGroup)
				giftCount += partnerGiftGroup->GetCount();
			if (petGiftGroup)
				giftCount += petGiftGroup->GetCount();

			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetGiftBoxByAcctID: gift group found acct_id %d, count = %d"), acct_id, giftCount);
			
			PutRemainGiftToExist(acct_id, NULL);			
			NetGroup temp;
			NetGroup* giftGroup = NULL;

			for (Index j = C_INDEX(0); j < 3; ++j)
			{
				switch (j)
				{
				case 0: giftGroup = itemGiftGroup;		break;
				case 1: giftGroup = partnerGiftGroup;	break;
				case 2: giftGroup = petGiftGroup;		break;
				}
				if (giftGroup)
				{
					for (Index i = C_INDEX(0); i < giftGroup->GetCount(); ++i)
					{
						CGiftData* pGiftData = giftGroup->GetChild<CGiftData>(i);
						if (pGiftData)
						{
							pGiftData->PreSend();
							temp.Add(pGiftData);
						}
					}
				}
			}			
			
			CB_GetGiftBoxByAcctID(pPeer, context, &temp);
			PROCEDURE_RETURN
		}
		else
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("GetGiftBoxByAcctID: need retrive giftbox group from db acct_id %d"), acct_id);

			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			GiftRequest *pRequest = SafeCreateObject3(GiftRequest, acct_id, pPeer, context);
			if (pRequest)
				pRequest->giftID = 0;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetGiftBoxs, pRequest, DB_WORKER_FLAG);
#else
			QueueGetGiftBoxs(pRequest);
#endif
		}

		PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, giftGroup)

		TRACE_ENSURE(giftGroup);
		// store new chargeShop group
		DataObject& dobj3 = gGlobal.existItemGift[acct_id];
		NetGroup* itemGroup = NULL;
		if (dobj3.pobj) {
			itemGroup = (NetGroup*) dobj3.pobj;
		} else {
			itemGroup = SafeCreateObject(NetGroup);
			dobj3.pobj = itemGroup;
		}

		DataObject& dobj4 = gGlobal.existPartnerGift[acct_id];
		NetGroup* partnerGroup = NULL;
		if (dobj4.pobj) {
			partnerGroup = (NetGroup*) dobj4.pobj;
		} else {
			partnerGroup = SafeCreateObject(NetGroup);
			dobj4.pobj = partnerGroup;
		}

		DataObject& dobj5 = gGlobal.existPetGift[acct_id];
		NetGroup* petGroup = NULL;
		if (dobj5.pobj) {
			petGroup = (NetGroup*) dobj5.pobj;
		} else {
			petGroup = SafeCreateObject(NetGroup);
			dobj5.pobj = petGroup;
		}

		UInt itemCount = 0;
		UInt partnerCount = 0;
		UInt petCount = 0;
		NetGroup temp;
		for (Index i = C_INDEX(0); i < giftGroup->GetCount(); ++i)
		{
			CGiftData* pGiftData = giftGroup->GetChild<CGiftData>(i);
			if (pGiftData) {
				
				if (std::find(gGlobal.giftDelMap.begin(), gGlobal.giftDelMap.end(), 
					pGiftData->GetGiftID()) != gGlobal.giftDelMap.end())
					continue;

				// need to register for new chargeShop data
				DataObject &dobj = gGlobal.giftIdMap[pGiftData->GetGiftID()];
				if (dobj.pobj) {
					DeleteNetObject(pGiftData);
					pGiftData = CastNetObject<CGiftData>(dobj.pobj);
				} else {
					LocalRPCManager::GetInstance().Register(pGiftData);
					pGiftData->ClearUpdateMask();
					dobj.pobj = pGiftData;
				}				

				switch (pGiftData->GetItemType())
				{
				case GIFT_ITEM:
					if (itemCount < MAX_GIFT_PER_TYPE)
					{
						pGiftData->PreSend();
						temp.Add(pGiftData);
						itemGroup->Add(pGiftData);
						++itemCount;
					}
					else
					{
						RemainGiftMap::iterator itr = gGlobal.remainItemGift.find(acct_id);
						if (itr != gGlobal.remainItemGift.end())
							itr->second.push_back(pGiftData->GetGiftID());
						else
						{
							UInt32Vector tempVector;
							tempVector.push_back(pGiftData->GetGiftID());
							gGlobal.remainItemGift.insert(std::make_pair(acct_id, tempVector));
						}
					}
					break;
				case GIFT_PARTNER:
					if (partnerCount < MAX_GIFT_PER_TYPE)
					{
						pGiftData->PreSend();
						temp.Add(pGiftData);
						partnerGroup->Add(pGiftData);
						++partnerCount;
					}
					else
					{
						RemainGiftMap::iterator itr = gGlobal.remainPartnerGift.find(acct_id);
						if (itr != gGlobal.remainPartnerGift.end())
							itr->second.push_back(pGiftData->GetGiftID());
						else
						{
							UInt32Vector tempVector;
							tempVector.push_back(pGiftData->GetGiftID());
							gGlobal.remainPartnerGift.insert(std::make_pair(acct_id, tempVector));
						}
					}
					break;
				case GIFT_PET:
					if (petCount < MAX_GIFT_PER_TYPE)
					{
						pGiftData->PreSend();
						temp.Add(pGiftData);
						petGroup->Add(pGiftData);
						++petCount;
					}
					else
					{
						RemainGiftMap::iterator itr = gGlobal.remainPetGift.find(acct_id);
						if (itr != gGlobal.remainPetGift.end())
							itr->second.push_back(pGiftData->GetGiftID());
						else
						{
							UInt32Vector tempVector;
							tempVector.push_back(pGiftData->GetGiftID());
							gGlobal.remainPetGift.insert(std::make_pair(acct_id, tempVector));
						}
					}
					break;
				}
			}
		}

		// send charge shop data to peer
		CB_GetGiftBoxByAcctID(pPeer, context, &temp);
		DeleteNetGroup(giftGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_T("GetGiftBoxByAcctID Error: sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH
		
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32Vector &giftBoxID)
{
	HRESULT hr;

	TRACE_ENSURE(pdbGift != NULL);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("DeleteGiftBox: sessionId %s, gift_ids %s"),
		context.sessionId.GetString().c_str(), GetVectorString(giftBoxID).c_str());

	for (Index i = C_INDEX(0); i < giftBoxID.size(); ++i)
		hr = RemoveGift(giftBoxID[i]);

	CB_DeleteGiftBox(pPeer, context);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateGiftBoxByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC1(ObjectService, UpdateGiftBoxByAcctID, NULL, UInt32, acct_id)

		NetGroup* itemGiftGroup = FindGiftGroup(GIFT_ITEM, acct_id);
		NetGroup* partnerGiftGroup = FindGiftGroup(GIFT_PARTNER, acct_id);
		NetGroup* petGiftGroup = FindGiftGroup(GIFT_PET, acct_id);

		if (itemGiftGroup && itemGiftGroup->GetCount() >= MAX_GIFT_PER_TYPE &&
			partnerGiftGroup && partnerGiftGroup->GetCount() >= MAX_GIFT_PER_TYPE &&
			petGiftGroup && petGiftGroup->GetCount() >= MAX_GIFT_PER_TYPE)
		{
			NetGroup temp;
			CB_GetGiftBoxByAcctID(pPeer, context, &temp);
			PROCEDURE_RETURN
		}
		else
		{
			UInt32 maxGiftID = 0;
			RemainGiftMap::iterator itr;
			RemainGiftMap::iterator endItr;

			for (Index i = C_INDEX(0); i < 3; ++i)
			{
				NetGroup* tempGrp = NULL;
				switch (i)
				{
				case 0:
					{
						itr = gGlobal.remainItemGift.find(acct_id);
						endItr = gGlobal.remainItemGift.end();
						tempGrp = itemGiftGroup;
					}
					break;
				case 1:
					{
						itr = gGlobal.remainPartnerGift.find(acct_id);
						endItr = gGlobal.remainPartnerGift.end();
						tempGrp = partnerGiftGroup;
					}
					break;
				case 2:
					{
						itr = gGlobal.remainPetGift.find(acct_id);
						endItr = gGlobal.remainPetGift.end();
						tempGrp = petGiftGroup;
					}
					break;
				}
				
				if (itr != endItr && itr->second.size() > 0)
				{
					UInt tempNum = *std::max_element(itr->second.begin(), itr->second.end());
					if (maxGiftID < tempNum)
						maxGiftID = tempNum;
				}
				else if (tempGrp)
				{
					CGiftData* pGiftData = tempGrp->GetChild<CGiftData>(tempGrp->GetCount() - 1);
					if (pGiftData && maxGiftID < pGiftData->GetGiftID())
						maxGiftID = pGiftData->GetGiftID();
				}
			}

			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			GiftRequest *pRequest = SafeCreateObject3(GiftRequest, acct_id, pPeer, context);
			if (pRequest)
				pRequest->giftID = maxGiftID;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetGiftBoxs, pRequest, DB_WORKER_FLAG);
#else
			QueueGetGiftBoxs(pRequest);
#endif
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, giftGroup)
		
		NetGroup temp;
		for (Index i = C_INDEX(0); i < giftGroup->GetCount(); ++i)
		{
			CGiftData* pGiftData = giftGroup->GetChild<CGiftData>(i);
			if (pGiftData) 
			{				
				if (std::find(gGlobal.giftDelMap.begin(), gGlobal.giftDelMap.end(), 
					pGiftData->GetGiftID()) != gGlobal.giftDelMap.end())
					continue;

				// need to register for new chargeShop data
				DataObject &dobj = gGlobal.giftIdMap[pGiftData->GetGiftID()];
				if (dobj.pobj) {
					continue;
				} else {
					LocalRPCManager::GetInstance().Register(pGiftData);
					pGiftData->ClearUpdateMask();
					dobj.pobj = pGiftData;
					pGiftData->PreSend();
					temp.Add(pGiftData);
				}		

				switch (pGiftData->GetItemType())
				{
				case GIFT_ITEM:					
					{
						RemainGiftMap::iterator itr = gGlobal.remainItemGift.find(acct_id);
						if (itr != gGlobal.remainItemGift.end())
							itr->second.push_back(pGiftData->GetGiftID());
						else
						{
							UInt32Vector tempVector;
							tempVector.push_back(pGiftData->GetGiftID());
							gGlobal.remainItemGift.insert(std::make_pair(acct_id, tempVector));
						}
					}
					break;
				case GIFT_PARTNER:					
					{
						RemainGiftMap::iterator itr = gGlobal.remainPartnerGift.find(acct_id);
						if (itr != gGlobal.remainPartnerGift.end())
							itr->second.push_back(pGiftData->GetGiftID());
						else
						{
							UInt32Vector tempVector;
							tempVector.push_back(pGiftData->GetGiftID());
							gGlobal.remainPartnerGift.insert(std::make_pair(acct_id, tempVector));
						}
					}
					break;
				case GIFT_PET:					
					{
						RemainGiftMap::iterator itr = gGlobal.remainPetGift.find(acct_id);
						if (itr != gGlobal.remainPetGift.end())
							itr->second.push_back(pGiftData->GetGiftID());
						else
						{
							UInt32Vector tempVector;
							tempVector.push_back(pGiftData->GetGiftID());
							gGlobal.remainPetGift.insert(std::make_pair(acct_id, tempVector));
						}
					}
					break;
				}
			}
		}
		
		PutRemainGiftToExist(acct_id, &temp);
		CB_GetGiftBoxByAcctID(pPeer, context, &temp);
		DeleteNetGroup(giftGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}
