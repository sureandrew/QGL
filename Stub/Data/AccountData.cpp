//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/common_binary.h"
//-- Self
#include "Stub/Data/AccountData.h"
//-- Library
#include "Stub/Data/CharacterData.h"
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboAccount.h"
#include "ObjectService/dboCharacter.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"
#include "ObjectService/Configure.h"
#include <Reuben/Math/Hex.h>

#define BADNAME_WAIT_TIME		3000			// wait 3 second for bad account name

REGISTER_CLASSTYPE(CAccountData, AccountData_CLASSID);

CAccountData* NewAccount();
CdboAccount* &pdbAccount = gGlobal.gTables.pAccount;
StringVector CdboAccountAccessor::m_params;
ThreadKey CAccountData::m_key;
struct AccountRequest : public WorkRequest<String>
{
	Boolean			bReact;
	AccountRequest(const String &name, LPCPEER pPeer, RPCContext &context, Boolean bReact)
		: WorkRequest<String>(name, pPeer, context)
		, bReact(bReact)
	{}
};


////////////////////////////////////////////////////////////////////////
////						Account Data							////
////////////////////////////////////////////////////////////////////////
CAccountData* NewAccount()
{
	CAccountData *pAcctData = NULL;
	UInt32 acct_id = (UInt32) pdbAccount->m_acct_id.LowPart;
	DataObject &dobj = gGlobal.acctIdMap[acct_id];
	DataObject &dobj2 = gGlobal.acctNameMap[pdbAccount->m_loginName];

	if (dobj.pobj == NULL) {
		pAcctData = CAccountData::NewInstance();
		pAcctData->SetData(pdbAccount);			// assign values to character data
		dobj.pobj = pAcctData;
	}
	else
	{
		pAcctData = CastNetObject<CAccountData>(dobj.pobj);
	}
	dobj2 = dobj;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("pointer %d"), pAcctData);

	return pAcctData;
}

CAccountData::CAccountData()
{
	m_keepCharId = 0;
	m_bLoad = BOOLEAN_FALSE;
}

CAccountData::~CAccountData()
{
}

void CAccountData::AddCharId(UInt32 char_id)
{
	m_charIds.insert(char_id);
}

void CAccountData::RemoveCharId(UInt32 char_id)
{
	m_charIds.erase(char_id);
}

void CAccountData::KeepCharId(UInt32 char_id)
{
	m_keepCharId = char_id;
	if (char_id == 0)
		return;

	String s;
	for (CharIdSet::iterator it = m_charIds.begin(); it != m_charIds.end(); ++it)
	{
		if (*it == char_id)
			gGlobal.CacheKeepChar(*it);
		else
		{
			gGlobal.CacheReleaseChar(*it);
			s.AppendFormat(_T("%u, "), *it);
		}
	}

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("acct_id %d, char_id %d, release char_ids %s"),
		GetAcct_id(), char_id, s.c_str());
}

void CAccountData::ReleaseCharId(UInt32 char_id)
{
	// just reset keep character if released
	if (m_keepCharId == char_id)
		m_keepCharId = 0;
}

void CAccountData::ReleaseAllChar()
{
	// release all character except keep character
	String s;
	UInt32 char_id;
	for (CharIdSet::iterator it = m_charIds.begin(); it != m_charIds.end(); ++it)
	{
		char_id = *it;
		if (char_id != m_keepCharId)
		{
			gGlobal.CacheKeepChar(char_id);
			ReleaseCharacter(char_id);
			s.AppendFormat(_T("%u, "), char_id);
		}
	}
	m_charIds.clear();

	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_F("acct_id %d, keep char_id %d, char_ids %s"), 
		GetAcct_id(), m_keepCharId, s.c_str());
}

NetGroup* CAccountData::GetCharGroup()
{
	if (!m_bLoad)				// haven't load data from DB
		return NULL;

	bool avail = true;
	CCharacterData* pCharData;
	m_charGroup.Clear();

	for (CharIdSet::iterator it = m_charIds.begin(); it != m_charIds.end(); ++it)
	{
		pCharData = FindChar(*it);
		if (pCharData)
			m_charGroup.Add(pCharData);
		else
		{
			avail = false;
			break;
		}
	}

	if (avail)
	{
		// all character available
		// keep all character in account
		for (CharIdSet::iterator it = m_charIds.begin(); it != m_charIds.end(); ++it)
			gGlobal.CacheKeepChar(*it);
		return &m_charGroup;
	}
	else // return null when some characters are not available
		return NULL;
}

Boolean CAccountData::IsCharacterIDExist(UInt32 char_id)
{
	for (CharIdSet::iterator it = m_charIds.begin(); it != m_charIds.end(); ++it)
	{
		UInt32 temp = *it;
		if (char_id == temp)
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

void CAccountData::SetData(CdboAccount* dboAccount, bool inactive)
{
	Size size = 0;
	Reuben::Math::HexToBytes(NULL, size, dboAccount->m_password_hash, 64);
	ByteArray hashed_pw(size);
	Reuben::Math::HexToBytes(&hashed_pw, size, dboAccount->m_password_hash, 64);
	String login_name(dboAccount->m_loginName);

	SetInfo((UInt32) dboAccount->m_acct_id.LowPart, login_name.Trim().ToUpper(), hashed_pw); 
	time_t banTm;
	TimeConvert(dboAccount->m_banTime, banTm);
	SetBanTime((UInt32)banTm);
	if (inactive)
	{
		SetInactive(BOOLEAN_TRUE);
		ClearUpdateMask();
		return;
	}

	time_t disContime, unlockTime;         
	TimeConvert(dboAccount->m_disconnect_time, disContime);
	SetAddict((UInt32)disContime, (UInt32) dboAccount->m_offLnTime, (UInt32) dboAccount->m_onLnTime); 
	SetYuanBao((UInt32)dboAccount->m_yuanbao.LowPart, (UInt32)dboAccount->m_yuanbaoTicket.LowPart);
	SetAddictType((Boolean)(dboAccount->m_addictType == 1));
	SetYuanBaoLock((Boolean)(dboAccount->m_yuanbaoLock == 1)); 

	Size passSize = 0;
	Reuben::Math::HexToBytes(NULL, passSize, dboAccount->m_assetLockPassword, 64);
	ByteArray assetHashed_pw(passSize);
	Reuben::Math::HexToBytes(&assetHashed_pw, passSize, dboAccount->m_assetLockPassword, 64);
	SetAssetLockPassword(assetHashed_pw);
	TimeConvert(dboAccount->m_forceUnlockDate, unlockTime);	
	SetUnlockTime((UInt32)unlockTime);
	SetInactive(BOOLEAN_FALSE);

	ClearUpdateMask();
}

CAccountData* FindAccount(UInt32 acct_id)
{
	DataIdMap::iterator iter = gGlobal.acctIdMap.find(acct_id);
	CAccountData* pAcctData = NULL;

	// found existing account in cache
	if (iter != gGlobal.acctIdMap.end())
		pAcctData = CastNetObject<CAccountData>(iter->second.pobj);

	return pAcctData;
}

CAccountData* FindAccount(const String &name)
{
	DataNameMap::iterator iter = gGlobal.acctNameMap.find(name);
	CAccountData* pAcctData = NULL;

	// found existing account in cache
	if (iter != gGlobal.acctNameMap.end())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("pointer %d"), iter->second.pobj);
		pAcctData = CastNetObject<CAccountData>(iter->second.pobj);
	}

	return pAcctData;
}

void ReleaseAccount(UInt32 acct_id)
{
	DataIdMap::iterator iter = gGlobal.acctIdMap.find(acct_id);
	CAccountData* pAcctData = NULL;

	// found existing account in cache
	ReleaseGift(acct_id);

	if (iter != gGlobal.acctIdMap.end())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("acct_id %d"), acct_id);

		pAcctData = CastNetObject<CAccountData>(iter->second.pobj);
		gGlobal.acctIdMap.erase(iter);
		if (pAcctData)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_F("acct_id %d, pointer %d"), acct_id, pAcctData);
			DataNameMap::iterator itfind = gGlobal.acctNameMap.find(pAcctData->GetLoginName());
			if (itfind != gGlobal.acctNameMap.end())
				gGlobal.acctNameMap.erase(itfind);
			else
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
					_FE("cannot find the name, acct_id %d, pointer %d"),
					acct_id, pAcctData);
			}
			pAcctData->ReleaseAllChar();		// also release all characters at same time
			if (pAcctData->IsUpdated())
				gGlobal.m_releaseGroup.Add(pAcctData);
			else
				DeleteNetObject(pAcctData);
		}
	}
}

DWORD WINAPI QueueGetAccount(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	AccountRequest* pRequest = (AccountRequest*)lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.acctDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup acctGroup;
		CAccountData* pAcctData;
		CdboAccount* dboAccount = SafeCreateObject(CdboAccount);

		acctGroup.SetValidate(BOOLEAN_FALSE);
		dboAccount->SetDataSource(dbSrc);
		if (pRequest->bReact)
		{
			// try to reactivate account using gameDb
			CDboSource* dbGameSrc = gGlobal.gameDbPool.GetAvailSource();
			dboAccount->SetDataSource(dbGameSrc);
			hr = dboAccount->Reactivate(gConfigure.dbAccount.strDatabase.c_str(), 
				gConfigure.dbGame.strDatabase.c_str(), 
				gConfigure.dbAccount.strInactiveDb.c_str(),
				gConfigure.dbGame.strInactiveDb.c_str(), pRequest->key.c_str());
			gGlobal.gameDbPool.ReleaseSource(dbGameSrc);
			dboAccount->SetDataSource(dbSrc);		// back to account db source
		}
		// try to get account or inactive account
		if (hr == S_OK && DB_SUCCEEDED(hr = dboAccount->RSByName(pRequest->key.c_str())))
		{
			hr = dboAccount->MoveFirst();
			if (hr == S_OK)
			{
				pAcctData = SafeCreateObject(CAccountData);
				pAcctData->SetData(dboAccount);
				pAcctData->SetSendMask(CAccountData::AllMask);
				acctGroup.Add(pAcctData);
			}
		}
		if (DB_SUCCEEDED(hr))
		{
			TRACE_INFODTL_3(GLOBAL_LOGGER, 
				_F("retrieve account success, sessionId %s, pid %d:%d"),
				pRequest->context.sessionId.GetString().c_str(), 
				pRequest->context.pcdId.srvId, 
				pRequest->context.pcdId.subId);
		}
		else if (hr == DB_S_ENDOFROWSET && !pRequest->bReact)
		{
			dboAccount->Close();
			// account not found, try find inactive account
			if (DB_SUCCEEDED(hr = dboAccount->RSByInactiveName(pRequest->key.c_str())) &&
				DB_SUCCEEDED(hr = dboAccount->MoveFirst()))
			{
				RPCMask mask;
				pAcctData = SafeCreateObject(CAccountData);
				pAcctData->SetData(dboAccount, true);
				pAcctData->MarkSend(CAccountData::infoMask);
				pAcctData->MarkSend(CAccountData::banTimeMask);
				pAcctData->MarkSend(CAccountData::inactiveMask);
				acctGroup.Add(pAcctData);
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("retrieve inactive account, sessionId %s, pid %d:%d"),
					pRequest->context.sessionId.GetString().c_str(), 
					pRequest->context.pcdId.srvId, 
					pRequest->context.pcdId.subId);
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetAccount"), hr, dboAccount->GetErrorMsg(hr), 
			pRequest->context, &acctGroup);
		dboAccount->Close();
		SafeDeleteObject(dboAccount);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("cannot get data source, sessionId %s, pid %d:%d"),
			pRequest->context.sessionId.GetString().c_str(), 
			pRequest->context.pcdId.srvId, pRequest->context.pcdId.subId);
	}

	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("profile, utime %d"), dt);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

RPCResult ObjectService_Stub::CreateAccount(LPCPEER pPeer, RPCContext &context, const String &loginName, const ByteArray &password_hash)
{
	TRACE_ENSURE(pdbAccount != NULL);
	HRESULT hr;

	String strHashPw;
	Reuben::Math::BytesToHex(strHashPw, &password_hash, password_hash.GetSize());

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("session_id %s, password_hash = %s"),
		context.sessionId.GetString().c_str(), strHashPw.c_str());

	// check name exist in InactiveAccount
	if (DB_SUCCEEDED(hr = pdbAccount->RSByInactiveName(loginName.c_str())))
	{
		if (DB_SUCCEEDED(hr = pdbAccount->MoveFirst()))
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _FW("[inactive name exist], acct_id %d, name %s"), 
				pdbAccount->m_acct_id.LowPart, loginName.c_str());
			CB_Error(pPeer, context, hr, _FW("LOGINNAME_EXIST"));
			pdbAccount->Close();
			return RPC_RESULT_FAIL;
		}
	}
	pdbAccount->Close();

	DBTIMESTAMP timeNow;
	TimeConvert(time(NULL), timeNow);

	if (DB_FAILED(hr = pdbAccount->Insert(loginName.c_str(), strHashPw.c_str(), timeNow)))
	{
		pdbAccount->ShowError(hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
	}
	pdbAccount->Close();
	if (DB_FAILED(hr))
		return RPC_RESULT_FAIL;

	// also create account object
	gGlobal.badAcctNames.erase(loginName.c_str());		// remove any bad name before
	CAccountData* pAcctData = NewAccount();

	// send account data to peer
	pAcctData->SetSendMask(NetObject::AllMask);
	CB_GetAccountData(pPeer, context, pAcctData);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::DeleteAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	TRACE_ENSURE(pdbAccount != NULL);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("session_id %s, acct_id %d"),
		context.sessionId.GetString().c_str(), acct_id);

	// remove in account id and name map
	DataIdMap::iterator iter = gGlobal.acctIdMap.find(acct_id);
	if (iter != gGlobal.acctIdMap.end())
	{
		gGlobal.DequeueUpdate(&iter->second);
		CAccountData* pAcctData = CastNetObject<CAccountData>(iter->second.pobj);
		if (pAcctData)
		{
			gGlobal.acctNameMap.erase(pAcctData->GetLoginName());
			gGlobal.acctIdMap.erase(iter);
			gGlobal.m_deleteGroup.Add(pAcctData);
		}
		else
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("acct data NULL"));
		}
	}

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetAccountByName(LPCPEER pPeer, RPCContext &context, 
											   const String &loginName, const Boolean &bReact)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC2(ObjectService, GetAccountByName, NULL, String, loginName, Boolean, bReact)

		TRACE_ENSURE(pdbAccount != NULL);
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("sessionId %s, pid %d:%d"),
			context.sessionId.GetString().c_str(), context.pcdId.srvId, context.pcdId.subId);

		HRESULT hr = S_OK;
		CAccountData* pAcctData = FindAccount(loginName);
		bool badName = false;

		if (pAcctData) // keep the account if found
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("disconnect time from existing account data object %d"), pAcctData->GetDisconnect_time());		
			gGlobal.CacheKeepAcct(pAcctData->GetAcct_id());
			pAcctData->SetSendMask(NetObject::AllMask);
			CB_GetAccountData(pPeer, context, pAcctData);
			PROCEDURE_RETURN
		}
		else
		{
			if (gGlobal.badAcctNames.find(loginName) != gGlobal.badAcctNames.end())
			{
				NetGroup grp;
				PROCEDURE_GO_STEP1(1, NetGroup*, &grp)
			}
			else // wait for thread pool to process	
			{
				gGlobal.WorkQueueBegin();
				AccountRequest *pRequest = SafeCreateObject4(AccountRequest, loginName, 
					pPeer, context, bReact);
#ifdef DB_THREAD_POOL
				gGlobal.QueueUserWorkItem(QueueGetAccount, pRequest, DB_WORKER_FLAG);
#else // DB_THREAD_POOL
				QueueGetAccount(pRequest);
#endif // DB_THREAD_POOL
			}
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, acctGroup)

		TRACE_ENSURE(acctGroup);

		if (acctGroup->GetCount() > 0)
		{
			// have that account
			CAccountData* pAcctData = acctGroup->GetChild<CAccountData>(0);
			TRACE_ENSURE(pAcctData);
			// need to register for new acct data
			TRACE_ERRORDTL_IF_4(GLOBAL_LOGGER, loginName != pAcctData->GetLoginName(),
				_FE("login name not match, session_id %s, acct_id %d, pid %d:%d"),
				context.sessionId.GetString().c_str(), pAcctData->GetAcct_id(), 
				context.pcdId.srvId, context.pcdId.subId);
			// 

			if (pAcctData->GetInactive())
			{	// inactive account
				TRACE_INFODTL_5(GLOBAL_LOGGER, 
					_F("inactive account, session_id %s, acct_id %d, pointer %d, pid %d:%d"),
					context.sessionId.GetString().c_str(), pAcctData->GetAcct_id(), 
					pAcctData, context.pcdId.srvId, context.pcdId.subId);
				pAcctData->SetSendMask(pAcctData->GetUpdateMask());
				CB_GetAccountData(pPeer, context, pAcctData);
				DeleteNetGroup(acctGroup, BOOLEAN_TRUE);
				PROCEDURE_RETURN
			}

			TRACE_INFODTL_5(GLOBAL_LOGGER, 
				_F("new account, session_id %s, acct_id %d, pointer %d, pid %d:%d"),
				context.sessionId.GetString().c_str(), pAcctData->GetAcct_id(), 
				pAcctData, context.pcdId.srvId, context.pcdId.subId);
			DataObject &dobj = gGlobal.acctIdMap[pAcctData->GetAcct_id()];
			if (dobj.pobj)
			{
				DeleteNetObject(pAcctData);
				pAcctData = CastNetObject<CAccountData>(dobj.pobj);
				TRACE_INFODTL_1(GLOBAL_LOGGER, 
					_F("disconnect time from old account data object %d"), 
					pAcctData->GetDisconnect_time());
			}
			else
			{
				DataObject &dobj2 = gGlobal.acctNameMap[pAcctData->GetLoginName()];
				LocalRPCManager::GetInstance().Register(pAcctData);
				pAcctData->ClearUpdateMask();
				dobj.pobj = pAcctData;
				dobj2.pobj = pAcctData;
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("disconnect time from db %d"), 
					pAcctData->GetDisconnect_time());
			}
			if (pAcctData)
			{
				pAcctData->SetSendMask(NetObject::AllMask);
				gGlobal.CacheKeepAcct(pAcctData->GetAcct_id());
			}
			CB_GetAccountData(pPeer, context, pAcctData);
			DeleteNetGroup(acctGroup);
			PROCEDURE_RETURN
		}
		DeleteNetGroup(acctGroup);
		// wait timeout for bad name

	PROCEDURE_WAIT_TIMEOUT(2, BADNAME_WAIT_TIME)

		TRACE_WARNDTL_3(GLOBAL_LOGGER, _FW("account not found, session_id %s, pid %d:%d"),
			context.sessionId.GetString().c_str(), context.pcdId.srvId, context.pcdId.subId);
		CB_Error(pPeer, context, 1, _FW("account not found"));
		gGlobal.badAcctNames.insert(loginName);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

HRESULT CAccountData::UpdateDbo(CdboAccount* dboAcct)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_F("acct_id %d update_mask %d:%d"), 
		GetAcct_id(), m_updateMask.low, m_updateMask.high);

	dboAcct->ClearParam();
	if (IsUpdated(addictMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetDisconnect_time(), dbtime);
		COLUMN_UPDATE(dboAcct, disconnect_time, dbtime);	
		COLUMN_UPDATE(dboAcct, offLnTime, GetOffLnTime());	
		COLUMN_UPDATE(dboAcct, onLnTime, GetOnLnTime());	
	}

	if (IsUpdated(banTimeMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetBanTime(), dbtime);
		COLUMN_UPDATE(dboAcct, banTime, dbtime);
	}


	if (!dboAcct->m_params.empty())
	{
		HRESULT hr;
		if (DB_FAILED(hr = dboAcct->UpdateByAny(GetAcct_id())))
			dboAcct->ShowError(hr);
		else
			ClearUpdateMask();
	}
	else
		ClearUpdateMask();
	dboAcct->ClearParam();
	dboAcct->Close();

	return S_OK;
}

HRESULT CAccountData::DeleteDbo(CdboAccount* dboAcct)
{
	HRESULT hr;
	//ThreadGuard guard(m_key);

	if (DB_FAILED(hr = dboAcct->Delete(GetAcct_id())))
		dboAcct->ShowError(hr);
	else
	{
		// delete all characters for that account
		CCharacterData* pCharData;
		for (CharIdSet::iterator it = m_charIds.begin(); it != m_charIds.end(); ++it)
		{
			pCharData = FindChar(*it);
			if (pCharData)
				gGlobal.m_deleteGroup.Add(pCharData);
		}
	}
	dboAcct->Close();

	return hr;
}

RPCResult ObjectService_Stub::ReleaseAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("session_id %s, acct_id %d"),
		context.sessionId.GetString().c_str(), acct_id);

	CAccountData* pAcctData = FindAccount(acct_id);
	if (pAcctData)
		gGlobal.CacheReleaseAcct(acct_id);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateAccount(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, NetObject *acctdata)
{
	//////////////////////////////////////////////////////
	CAccountData *pNewData = CastNetObject<CAccountData>(acctdata);
	TRACE_ENSURE(pdbAccount && pNewData);

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("acct_id %d"), acctID);

	// **NOTE: The accountdata object will be deleted in "ReduceCache" after player enter the game for around 2 minute .
	// Therefore, the vaule of "CAccountData* pAcctData = FindAccount(acctID)" maybe NULL or not depends when the player leave the game
	// As a result, the following code may be changed depends on when the account data is updated.

	CAccountData* pAcctData = FindAccount(acctID);	
	if (pAcctData == NULL)
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("acct_id not found, acct_id %d"),
			acctID);
	}
	else
	{						
		DataIdMap::iterator iter = gGlobal.acctIdMap.find(acctID);
		// change later when 
		pAcctData->Clone(pNewData, pNewData->GetUpdateMask());
		if (iter != gGlobal.acctIdMap.end())
			gGlobal.EnqueueUpdate(&iter->second);
	}
	DeleteNetObject(pNewData);	

	CB_UpdateAccountSuccess(pPeer, context);
	return RPC_RESULT_OK;
} 

RPCResult ObjectService_Stub::UpdateAccountAddictInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, const UInt32 &disconnectTime, const UInt32 &offTime, const UInt32 &onTime)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("acct_id %d"), acctID);

	// The accountdata object will be deleted in "ReduceCache" after player enter the game for around 2 minute .
	// Therefore, the vaule of "CAccountData* pAcctData = FindAccount(acctID)" maybe NULL or not depends when the player leave the game.
	// It will update the DB no matter if the accountdata object in objectService exist or not.

	CAccountData* pAcctData = FindAccount(acctID);
	if (pAcctData) // but in case the accountdata object still hasn't been erased by ReduceCache yet, update it anyway because ObjectService will
	{              // sink this data to worldservice if it exists, otherwise it will create a new one based on the information on the DB and send it to worldservice. 
		pAcctData->SetAddict(disconnectTime, offTime, onTime);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("addiction info update to existing account data object, acct_id %d, disconn_time %d"), acctID, disconnectTime);	
	}
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("addiction info update to db, acct_id %d, disconn_time %d"), acctID, disconnectTime);	

	DBTIMESTAMP dbtime;	
	TimeConvert((time_t) disconnectTime, dbtime);		
	pdbAccount->UpdateAddictGameInfo(offTime, onTime, dbtime, acctID); 		
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("write game addiction info to db with acct_id %d"), acctID);	

	CB_UpdateAccountSuccess(pPeer, context);
	return RPC_RESULT_OK;
}

void CAccountData::GetDbCharId(LPCPEER pPeer, RPCContext &context)
{
	HRESULT hr;
	CdboCharacter* &pdbCharacter = gGlobal.gTables.pCharacter;

	if (DB_SUCCEEDED(hr = pdbCharacter->RSIDByAccountID(GetAcct_id())))
	{
		String s;

		m_charIds.clear();
		hr = pdbCharacter->MoveFirst();
		while (hr == S_OK)
		{
			UInt32 char_id = (UInt32) pdbCharacter->m_char_id.LowPart;
			m_charIds.insert(char_id);
			s.AppendFormat(_T("%u, "), char_id);
			hr = pdbCharacter->MoveNext();
		}
		pdbCharacter->Close();
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("acct_id %u, char_ids %s"),
			GetAcct_id(), s.c_str());
	}
	else
	{
		pdbCharacter->ShowError(hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
	}
}

RPCResult ObjectService_Stub::GetYaunBaoByAcctID(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id)
{
	TRACE_ENSURE(pdbAccount != NULL);
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("session_id %s, acct_id %d"),
		context.sessionId.GetString().c_str(), acct_id);

	HRESULT hr = S_OK;
	CAccountData* pAcctData = FindAccount(acct_id);

	UInt32 yuanbao = 0, yuanbaoTicket = 0;
	Boolean yuanbaoLock = BOOLEAN_FALSE;
	time_t unlockDate = 0;
	
	if (DB_SUCCEEDED(hr = pdbAccount->GetYuaoBaoByID(acct_id)))
	{
		if (DB_SUCCEEDED(hr = pdbAccount->MoveFirst()))
		{
			yuanbao = pdbAccount->m_yuanbao.LowPart;
			yuanbaoLock = (Boolean) (pdbAccount->m_yuanbaoLock == 1); 
			yuanbaoTicket = pdbAccount->m_yuanbaoTicket.LowPart;
			TimeConvert(pdbAccount->m_forceUnlockDate, unlockDate);	
		}
	}
	pdbAccount->Close();	
	CB_GetYuanBaoByAcctID(pPeer, context, yuanbao, yuanbaoTicket, yuanbaoLock, (UInt32)unlockDate);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::IsAssetProtected(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID)
{
	TRACE_ENSURE(pdbAccount != NULL);
	HRESULT hr = S_OK;
	Boolean bPassProtect = BOOLEAN_TRUE, bNeedResetPass = BOOLEAN_FALSE;
	if (DB_SUCCEEDED(hr = pdbAccount->GetAssetPasswordByID(acctID)))
	{
		if (DB_SUCCEEDED(hr = pdbAccount->MoveFirst()))
		{
			Size size = 0;
			Reuben::Math::HexToBytes(NULL, size, pdbAccount->m_assetLockPassword, 64);
			ByteArray hashed_pw(size);
			Reuben::Math::HexToBytes(&hashed_pw, size, pdbAccount->m_assetLockPassword, 64);
			if (hashed_pw != gGlobal.m_hashed_empty_pw)
			{
				UInt32 timeNow = (UInt32) time(NULL);
				time_t timeToUnlock;         
				TimeConvert(pdbAccount->m_forceUnlockDate, timeToUnlock);	
				if ((timeToUnlock != 0) && (timeNow >= ((UInt32)timeToUnlock)))
					bNeedResetPass = BOOLEAN_TRUE;	
			}
			else
				bPassProtect = BOOLEAN_FALSE;

			CB_IsAssetProtected(pPeer, context, bPassProtect, bNeedResetPass);
		}
		else
            CB_IsAssetProtected(pPeer, context, BOOLEAN_FALSE, BOOLEAN_TRUE);	
	}
	else
		CB_IsAssetProtected(pPeer, context, BOOLEAN_FALSE, BOOLEAN_TRUE);
	pdbAccount->Close();
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::StoreAssetPassword(LPCPEER pPeer, RPCContext &context, const ByteArray &password, const UInt32 &acct_id)
{
	TRACE_ENSURE(pdbAccount != NULL);
	HRESULT hr = S_OK;
	CAccountData* pAcctData = FindAccount(acct_id);
	TCHAR inputpass[65];
	Size len = 64;
	Reuben::Math::BytesToHex(inputpass, len, &password, password.GetSize());
	Boolean success = DB_SUCCEEDED(hr = pdbAccount->SetAssetPassword(acct_id, inputpass));
	pdbAccount->Close();	
	if (success && pAcctData)
	{
		pAcctData->SetAssetLockPassword(password);	
		pAcctData->SetUnlock_time(0);	
	}
	CB_StoreAssetPassword(pPeer, context, success);	
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::SetAssetPassResetDate(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const UInt32 &resetDate)
{
	TRACE_ENSURE(pdbAccount != NULL);
	HRESULT hr = S_OK;	
 	CAccountData* pAcctData = FindAccount(acct_id);
	Boolean  success = DB_SUCCEEDED(hr = pdbAccount->ResetAssetDate(acct_id, resetDate));
	pdbAccount->Close();	
	if ((success) && (pAcctData))
	{
		pAcctData->SetUnlock_time(resetDate);	
	}
	CB_ResetAssetPassDate(pPeer, context, success);	
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetAssetPassword(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID)
{
	TRACE_ENSURE(pdbAccount != NULL);
	HRESULT hr = S_OK;
	if (DB_SUCCEEDED(hr = pdbAccount->GetAssetPasswordByID(acctID)))
	{
		if (DB_SUCCEEDED(hr = pdbAccount->MoveFirst()))
		{
			Size size = 0;
			Reuben::Math::HexToBytes(NULL, size, pdbAccount->m_assetLockPassword, 64);
			ByteArray hashed_pw(size);
			Reuben::Math::HexToBytes(&hashed_pw, size, pdbAccount->m_assetLockPassword, 64);
			if (hashed_pw != gGlobal.m_hashed_empty_pw)
				CB_GetAssetPassword(pPeer, context, hashed_pw);
			else
				CB_GetAssetPassword(pPeer, context, ByteArray());
		}
	}
	else
		CB_GetAssetPassword(pPeer, context, ByteArray());
	pdbAccount->Close();	

	return RPC_RESULT_OK;	
}


RPCResult ObjectService_Stub::UpdateYuanBao(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &acct_id, const Int32 &change)
{
	HRESULT hr = S_OK;
	CAccountData* pAcctData = FindAccount(acct_id);
	
	UInt32 yuanbao = 0;
	UInt8 success = 0;
#ifdef DB_MYSQL
	if (DB_SUCCEEDED(hr = pdbAccount->GetYuaoBaoByID(acct_id)))
	{
		if (DB_SUCCEEDED(hr = pdbAccount->MoveFirst()))
		{
			yuanbao = pdbAccount->m_yuanbao.LowPart;
			pdbAccount->Close();
		}
		if ((Int32) yuanbao + change >= 0 && 
			DB_SUCCEEDED(hr = pdbAccount->UpdateYaunBao(acct_id, change)))
		{
			yuanbao = (UInt32)((Int32) yuanbao + change);
			success = 1;
		}
		else
			success = 0;
	}
#else
	if (DB_SUCCEEDED(hr = pdbAccount->UpdateYaunBao(acct_id, change)))
	{
		if (DB_SUCCEEDED(hr = pdbAccount->MoveFirst()))
		{
			yuanbao = pdbAccount->m_yuanbao.LowPart;
			success = pdbAccount->m_canupdate;
		}
	}
#endif // DB_MYSQL
	if (DB_SUCCEEDED(hr))
	{
		if (pAcctData)
		{
			pAcctData->SetYuanBao(yuanbao);
			pAcctData->UnmarkUpdate(CAccountData::yuanBaoMask);
		}
		if (success == 0)
			CB_UpdateYuanBao(pPeer, context, yuanbao, BOOLEAN_FALSE);
		else
			CB_UpdateYuanBao(pPeer, context, yuanbao, BOOLEAN_TRUE);
	}
	else
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
			_FE("session_id %s, acct_id %d, change %d"),
			context.sessionId.GetString().c_str(), acct_id, change);
		pdbAccount->ShowError(hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
	}
	pdbAccount->Close();
	
	if (DB_SUCCEEDED(hr))
	{
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_F("session_id %s, acct_id %d, change %d, yuanbao %d"),
			context.sessionId.GetString().c_str(), acct_id, change, yuanbao);
	}

	return RPC_RESULT_OK;
}


RPCResult ObjectService_Stub::SetYuanBaoLock(LPCPEER pPeer, RPCContext &context, const UInt32 &acctID, const Boolean &lock)
{
	HRESULT hr = S_OK;
	TRACE_ENSURE(pdbAccount != NULL);	

	Boolean success;
	success = DB_SUCCEEDED(hr = pdbAccount->SetYuanbaoLock(acctID, lock));
	pdbAccount->Close();	

	CB_SetAssetLock(pPeer, context, success);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateYuanBaoTicket(LPCPEER pPeer, RPCContext &context, const UInt32 &acct_id, const Int32 &change, const ByteArray &password)
{
	HRESULT hr = S_OK;
	CAccountData* pAcctData = FindAccount(acct_id);
	
	UInt32 yuanbao = 0;
	UInt32 yuanbaoticket = 0;
	UInt8 success = 0;

	if (pAcctData == NULL || pAcctData->GetPassword_hash() != password)
	{
		CB_UpdateYuanBaoTicket(pPeer, context, yuanbao, yuanbaoticket, CHANGE_YB_WRONGPASSWORD);
		return RPC_RESULT_OK;
	}
	
	if (DB_SUCCEEDED(hr = pdbAccount->UpdateYaunBaoTicket(acct_id, change, gConfigure.ratioPerYBT)))
	{
		if (DB_SUCCEEDED(hr = pdbAccount->MoveFirst()))
		{
			yuanbao = pdbAccount->m_yuanbao.LowPart;
			yuanbaoticket = pdbAccount->m_yuanbaoTicket.LowPart;
			success = pdbAccount->m_canupdate;
		}
	}
	if (DB_SUCCEEDED(hr))
	{
		if (pAcctData)
		{
			pAcctData->SetYuanBao(yuanbao);
			pAcctData->SetYuanBaoTicket(yuanbaoticket);
			pAcctData->UnmarkUpdate(CAccountData::yuanBaoMask);
		}
		if (success == 0)
			CB_UpdateYuanBaoTicket(pPeer, context, yuanbao, yuanbaoticket, CHANGE_YB_FAIL);
		else
			CB_UpdateYuanBaoTicket(pPeer, context, yuanbao, yuanbaoticket, CHANGE_YB_SUCCESS);
	}
	else
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
			_FE("session_id %s, acct_id %d, change %d"),
			context.sessionId.GetString().c_str(), acct_id, change);
		pdbAccount->ShowError(hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
	}
	pdbAccount->Close();
	
	if (DB_SUCCEEDED(hr))
	{
		TRACE_INFODTL_5(GLOBAL_LOGGER, 
			_F("session_id %s, acct_id %d, change %d, yuanbao %d, yuanbaoTicket %d"),
			context.sessionId.GetString().c_str(), acct_id, change, yuanbao, yuanbaoticket );
	}

	return RPC_RESULT_OK;
}
