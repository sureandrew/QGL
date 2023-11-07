//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/SellYuanBaoData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboSellYuanBao.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"
#include "Stub/Data/CharacterData.h"
#include "Stub/Data/AccountData.h"

CdboSellYuanBao* &pdbSellYuanBao = gGlobal.gTables.pSellYuanBao;
StringVector CdboSellYuanBaoAccessor::m_params;
ThreadKey CSellYuanBaoData::m_key;
typedef WorkRequest<UInt32> SellYuanBaoRequest;

REGISTER_CLASSTYPE(CSellYuanBaoData, SellYuanBaoData_CLASSID);

////////////////////////////////////////////////////////////////////////
////						ChargeShop Data							////
////////////////////////////////////////////////////////////////////////

CSellYuanBaoData* NewSellYuanBaoData()
{
	CSellYuanBaoData *pSellYuanBaoData = NULL;
	UInt32 sellYuanBaoID = (UInt32) pdbSellYuanBao->m_sellYuanBao_id.LowPart;
	if (sellYuanBaoID > 0)
	{
		DataObject &dobj = gGlobal.sellYBIdMap[sellYuanBaoID];
		if (dobj.pobj == NULL) {
			pSellYuanBaoData = CSellYuanBaoData::NewInstance();
			pSellYuanBaoData->SetData(pdbSellYuanBao);			// assign values to sellYuanBao data
			dobj.pobj = pSellYuanBaoData;
		} else {
			pSellYuanBaoData = CastNetObject<CSellYuanBaoData>(dobj.pobj);
		}
	}

	return pSellYuanBaoData;
}

CSellYuanBaoData* FindSellYuanBao(UInt32 sellYuanBaoID)
{
	DataIdMap::iterator iter = gGlobal.sellYBIdMap.find(sellYuanBaoID);
	CSellYuanBaoData* pSellYuanBaoData = NULL;

	// found existing account in cache
	if (iter != gGlobal.sellYBIdMap.end())
		pSellYuanBaoData = CastNetObject<CSellYuanBaoData>(iter->second.pobj);

	return pSellYuanBaoData;
}

HRESULT RemoveSellYuanBao(UInt32 sellYuanBaoID)
{
	DataIdMap::iterator iter = gGlobal.sellYBIdMap.find(sellYuanBaoID);
	if (iter != gGlobal.sellYBIdMap.end())
	{
		//gGlobal.DequeueUpdate(&iter->second);
		CSellYuanBaoData* pSellYuanBaoData = CastNetObject<CSellYuanBaoData>(iter->second.pobj);
		if (pSellYuanBaoData) {			
			//gGlobal.m_deleteGroup.Add(pSellYuanBaoData);
			DeleteNetObject(pSellYuanBaoData);
		}
		gGlobal.sellYBIdMap.erase(iter);
	}
	return S_OK;
}

DWORD WINAPI QueueGetSellYuanBao(LPVOID lpParam)
{
	HRESULT hr = S_OK;

	SellYuanBaoRequest* pRequest = (SellYuanBaoRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource(); 
	if (dbSrc)
	{
		NetGroup sellYuanBaoGroup;
		CSellYuanBaoData* pSellYuanBaoData = NULL;
		CdboSellYuanBao* dboSellYuanBao = SafeCreateObject(CdboSellYuanBao);

		sellYuanBaoGroup.SetValidate(BOOLEAN_FALSE);
		dboSellYuanBao->SetDataSource(dbSrc);
		
		hr = dboSellYuanBao->GetAll();
		
		if (DB_SUCCEEDED(hr))
		{
			hr = dboSellYuanBao->MoveFirst();
			while (hr == S_OK)
			{
				pSellYuanBaoData = SafeCreateObject(CSellYuanBaoData);
				pSellYuanBaoData->SetData(dboSellYuanBao);
				pSellYuanBaoData->SetSendMask(CSellYuanBaoData::AllMask);
				sellYuanBaoGroup.Add(pSellYuanBaoData);
				hr = dboSellYuanBao->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetSellYuanBao"), hr, dboSellYuanBao->GetErrorMsg(hr), 
			pRequest->context, &sellYuanBaoGroup);
		dboSellYuanBao->Close();
		SafeDeleteObject(dboSellYuanBao);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s"),
			pRequest->context.sessionId.GetString().c_str());
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CSellYuanBaoData::SetData(CdboSellYuanBao* dboSellYuanBao)
{	
	String charName(dboSellYuanBao->m_charName);
	time_t uTime;
	TimeConvert(dboSellYuanBao->m_expireDate, uTime);

	SetId((UInt32)dboSellYuanBao->m_sellYuanBao_id.LowPart);
	SetCharInfo((UInt32)dboSellYuanBao->m_char_id.LowPart, charName.Trim());
	SetSellInfo((UInt8)dboSellYuanBao->m_sellType, (UInt32)dboSellYuanBao->m_totalYBT.LowPart, (UInt32)dboSellYuanBao->m_moneyPerYBT.LowPart, (UInt32)uTime);
	SetSoldYBT((UInt32)dboSellYuanBao->m_soldYBT.LowPart);

	ClearUpdateMask();
}

void CSellYuanBaoData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

RPCResult ObjectService_Stub::GetAllSellYuanBao(LPCPEER pPeer, RPCContext &context)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC(ObjectService, GetAllSellYuanBao, NULL)

		TRACE_ENSURE(pdbSellYuanBao != NULL);

		TRACE_INFODTL(GLOBAL_LOGGER, _F("start"));

		gGlobal.WorkQueueBegin();
		SellYuanBaoRequest *pRequest = SafeCreateObject3(SellYuanBaoRequest, 0, pPeer, context);
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetSellYuanBao, pRequest, DB_WORKER_FLAG);
#else
			QueueGetSellYuanBao(pRequest);
#endif
		
		PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, sellYuanBaoGroup)

			CSellYuanBaoData* pSellYuanBaoData = NULL;
			NetGroup sendGrp;
			
			for (Index i = C_INDEX(0); i < sellYuanBaoGroup->GetCount(); ++i)
			{
				pSellYuanBaoData = sellYuanBaoGroup->GetChild<CSellYuanBaoData>(i);
				if (pSellYuanBaoData)
				{
					pSellYuanBaoData->PreSend();
					DataObject &dobj = gGlobal.sellYBIdMap[pSellYuanBaoData->GetSellYuanBao_id()];
					if (dobj.pobj) {
						DeleteNetObject(pSellYuanBaoData);
						pSellYuanBaoData = CastNetObject<CSellYuanBaoData>(dobj.pobj);
					} else {
						LocalRPCManager::GetInstance().Register(pSellYuanBaoData);
						pSellYuanBaoData->ClearUpdateMask();
						dobj.pobj = pSellYuanBaoData;
					}
					sendGrp.Add(pSellYuanBaoData);
				}
			}

			CB_GetSellYuanBao(pPeer, context, &sendGrp, YB_MSG_NONE);
			DeleteNetGroup(sellYuanBaoGroup);			

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &sellType, const UInt32 &totalSell, const UInt32 &moneyPerUnit, const ByteArray &password)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" char_id %d, session_id %s"), char_id,
		context.sessionId.GetString().c_str());

	CCharacterData* pCharData = NULL;
	CAccountData* pAccountData = NULL;
	CSellYuanBaoData* pSellYBData = NULL;
	NetGroup sendGrp;

	UInt8 msgIndex = SELL_YB_ERROR;

	pCharData = FindChar(char_id);
	if (pCharData)
		pAccountData = FindAccount(pCharData->GetAcct_id());

	if (pCharData != NULL && pAccountData != NULL && pAccountData->GetPassword_hash() == password)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("correct password char_id %d, session_id %s"), char_id,
			context.sessionId.GetString().c_str());
		DBTIMESTAMP dbtime;
		UInt32 now = (UInt32) time(NULL);
		UInt32 expire;
		expire = now + 3600 * 8; //8 hours later
		TimeConvert((time_t) expire, dbtime);
		HRESULT hr = pdbSellYuanBao->Insert(pCharData->GetAcct_id(), char_id, pCharData->GetNickName(), sellType, totalSell, moneyPerUnit, dbtime);
		if (hr == S_OK && DB_SUCCEEDED(hr = pdbSellYuanBao->MoveFirst()))
		{			
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" can insert char_id %d, session_id %s"), char_id,
				context.sessionId.GetString().c_str());
			pSellYBData = NewSellYuanBaoData();
			if (pSellYBData)
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" have sell record char_id %d, session_id %s"), char_id,
					context.sessionId.GetString().c_str());
				pAccountData->SetYuanBaoTicket(pAccountData->GetYuanBaoTicket() - totalSell);
				pSellYBData->PreSend();
				sendGrp.Add(pSellYBData);
				msgIndex = SELL_YB_ADD_RECORD_SUCCESS;
			}
		}
		else if (DB_FAILED(hr))
			pdbSellYuanBao->ShowError(hr);
		pdbSellYuanBao->Close();
	}
	else
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" wrong password char_id %d, session_id %s"), char_id,
			context.sessionId.GetString().c_str());
		msgIndex = YB_MSG_WRONG_PASSWORD;
	}

	CB_GetSellYuanBao(pPeer, context, &sendGrp, msgIndex);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt32 &buyerAcct_id, const UInt32 &sellYuanBaoID, const UInt32 &YBT, const ByteArray &password)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" buyerAcct_id %d, session_id %s"), buyerAcct_id,
		context.sessionId.GetString().c_str());
	CSellYuanBaoData* pSellYBData = FindSellYuanBao(sellYuanBaoID);
	CAccountData* pAccountData = FindAccount(buyerAcct_id);

	UInt8 msgIndex = BUY_YB_ERROR;

	if (pSellYBData && pAccountData && pAccountData->GetPassword_hash() == password)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" correct password ownerChar_id %d, session_id %s"), pSellYBData->GetChar_id(),
			context.sessionId.GetString().c_str());
		HRESULT hr = pdbSellYuanBao->BuyYBT(buyerAcct_id, sellYuanBaoID, YBT);
		if (hr == S_OK && DB_SUCCEEDED(hr = pdbSellYuanBao->MoveFirst()))
		{			
			if (pdbSellYuanBao->m_success == 1)
			{
				pSellYBData->SetSoldYBT((UInt32)pdbSellYuanBao->m_soldYBT.LowPart);
				pAccountData->SetYuanBaoTicket(pAccountData->GetYuanBaoTicket() + YBT);
				pAccountData->UnmarkUpdate(CAccountData::yuanBaoMask);
				msgIndex = BUY_YB_SUCCESS;
			}
			else
			{
				msgIndex = BUY_YB_SOLD_ALL;
			}
		}
		else if (DB_FAILED(hr))
			pdbSellYuanBao->ShowError(hr);
		pdbSellYuanBao->Close();
	}
	else
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F(" wrong password buyerAcct_id %d, session_id %s"), buyerAcct_id,
			context.sessionId.GetString().c_str());
		msgIndex = YB_MSG_WRONG_PASSWORD;
	}

	CB_BuySellYuanBao(pPeer, context, msgIndex);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetTempBank(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const ByteArray &password)
{
	UInt32Vector sellYuanBaoIDs;
	UInt32 totalMoney = 0;
	UInt32 remainYBT = 0;

	CAccountData* pAccountData = NULL;
	CCharacterData* pCharData = FindChar(char_id);
	if (pCharData)
		pAccountData = FindAccount(pCharData->GetAcct_id());

	if (pAccountData && pAccountData->GetPassword_hash() == password)
	{
		HRESULT hr = pdbSellYuanBao->GetTempBank(char_id);
		if (hr == S_OK && DB_SUCCEEDED(hr = pdbSellYuanBao->MoveFirst()))
		{
			while (hr == S_OK)
			{
				if (pdbSellYuanBao->m_sellYuanBao_id.LowPart > 0)
				{
					sellYuanBaoIDs.push_back(pdbSellYuanBao->m_sellYuanBao_id.LowPart);
					totalMoney += pdbSellYuanBao->m_soldMoney.LowPart;
					remainYBT += pdbSellYuanBao->m_remainYBT.LowPart;

					RemoveSellYuanBao(pdbSellYuanBao->m_sellYuanBao_id.LowPart);
				}
				hr = pdbSellYuanBao->MoveNext();
			}
		}
		else if (DB_FAILED(hr))
			pdbSellYuanBao->ShowError(hr);

		if (remainYBT > 0)
		{
			pAccountData->SetYuanBaoTicket(pAccountData->GetYuanBaoTicket() + remainYBT);
			pAccountData->UnmarkUpdate(CAccountData::yuanBaoMask);
		}
		pdbSellYuanBao->Close();
	}

	CB_GetTempBank(pPeer, context, sellYuanBaoIDs, remainYBT, totalMoney);

	return RPC_RESULT_OK;
}
