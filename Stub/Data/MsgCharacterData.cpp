//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/MsgCharacterData.h"
//-- Self
#include "Stub/Data/CharacterData.h"
#include "ObjectService/dboMsgCharacter.h"
#include "ObjectService/Global.h"
#include "Common/common_binary.h"
#include "ObjectService/DboSourcePool.h"
#include "Stub/Service/ObjectService.h"

REGISTER_CLASSTYPE(CMsgCharacterData, MsgCharacterData_CLASSID);

struct MsgCharRequest : public WorkRequest<UInt32>
{
	UInt32Vector dbCharIds;

	MsgCharRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
	{}
};

CMsgCharacterData* NewMsgCharacterData();
CdboMsgCharacter* &pdbMsgCharacter = gGlobal.gTables.pMsgCharacter;
StringVector CdboMsgCharacterAccessor::m_params;
ThreadKey CMsgCharacterData::m_key;

NetGroup* FindCharGroup(UInt32 acct_id);


CMsgCharacterData::CMsgCharacterData()
{
}

CMsgCharacterData::~CMsgCharacterData()
{
}

void ReleaseMsgChar(UInt32 char_id)
{
	DataIdMap::iterator iter = gGlobal.msgCharMap.find(char_id);
	CMsgCharacterData* pMsgCharData = NULL;
	if (iter != gGlobal.msgCharMap.end())
	{
		pMsgCharData = CastNetObject<CMsgCharacterData>(iter->second.pobj);
		gGlobal.DequeueUpdate(&iter->second);
		gGlobal.msgCharMap.erase(iter);
	}

	if (pMsgCharData)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char_id %d"), 
			pMsgCharData->GetChar_id());
		if (pMsgCharData->IsUpdated())				// write to db if updated
			gGlobal.m_releaseGroup.Add(pMsgCharData);
		else
			DeleteNetObject(pMsgCharData);
	}
}
////////////////////////////////////////////////////////////////////////
////						Character Data							////
////////////////////////////////////////////////////////////////////////
CMsgCharacterData* NewMsgCharacterData()
{
	CMsgCharacterData *pMsgCharData = NULL;
	UInt32 char_id = (UInt32) pdbMsgCharacter->m_char_id.LowPart;
	DataObject &dobj = gGlobal.msgCharMap[char_id];

	if (dobj.pobj == NULL) {
		pMsgCharData = CMsgCharacterData::NewInstance();
		pMsgCharData->SetData(pdbMsgCharacter);			// assign values to character data
		dobj.pobj = pMsgCharData;
	} else {
		pMsgCharData = CastNetObject<CMsgCharacterData>(dobj.pobj);
	}

	return pMsgCharData;
}

CMsgCharacterData* FindMsgChar(UInt32 char_id)
{
	DataIdMap::iterator iter = gGlobal.msgCharMap.find(char_id);
	CMsgCharacterData* pMsgCharData = NULL;

	// found existing character in cache
	if (iter != gGlobal.msgCharMap.end())
		pMsgCharData = CastNetObject<CMsgCharacterData>(iter->second.pobj);

	return pMsgCharData;
}

void CMsgCharacterData::SetData(CdboMsgCharacter* dboMsgChar, bool inactive)
{
	String nickName(dboMsgChar->m_nickName);

	SetId((UInt32) dboMsgChar->m_char_id.LowPart);

	if (inactive)
	{
		SetNickName(nickName.Trim());
		SetInactive(BOOLEAN_TRUE);
		ClearUpdateMask();
		return;
	}

	SetInfo(nickName.Trim(), 
			(UInt8) dboMsgChar->m_sex, 
			(UInt8) dboMsgChar->m_cclass,
			(UInt8) dboMsgChar->m_faction);

	
	SetLevel((UInt16) dboMsgChar->m_level);
	SetTitle(dboMsgChar->m_title);
	SetStyle((UInt16) dboMsgChar->m_photo_id,
			(UInt16) dboMsgChar->m_hairStyle1,
			(UInt16) dboMsgChar->m_hairStyle2,
			(UInt16) dboMsgChar->m_hairColor);

	SetEqWeapon((UInt32) dboMsgChar->m_eq_weapon.LowPart,
				(UInt16) dboMsgChar->m_eq_weaponColor);
	SetEqHead((UInt32) dboMsgChar->m_eq_headBand.LowPart,
				(UInt16) dboMsgChar->m_eq_headColor);
	SetEqCloth((UInt32) dboMsgChar->m_eq_cloth.LowPart,
				(UInt16) dboMsgChar->m_eq_clothColor);
	
	SetOrgHairStyle((UInt16)dboMsgChar->m_org_hairStyle1, (UInt16)dboMsgChar->m_org_hairColor);
	SetOrgEqCloth((UInt32)dboMsgChar->m_org_eq_cloth.LowPart, (UInt16)dboMsgChar->m_org_eq_clothColor);
	SetFriend_status((UInt8)dboMsgChar->m_friend_status);
	SetFriend_icon((UInt8)dboMsgChar->m_friend_icon);
	SetFriend_about(dboMsgChar->m_friend_about);
	SetFriend_reply(dboMsgChar->m_friend_onlineAutoReply, dboMsgChar->m_friend_offlineAutoReply);
	SetSpeakCount((UInt16) dboMsgChar->m_speakCount);
	
	time_t uTime;
	TimeConvert(dboMsgChar->m_speakBlock, uTime);
	SetSpeakBlock((UInt32)uTime);

	TimeConvert(dboMsgChar->m_offlineTime, uTime);
	SetOfflineTime((UInt32)uTime);
	SetGuild_uid((UInt32)dboMsgChar->m_guild_uid.LowPart);
	SetMoney((UInt32)dboMsgChar->m_money.LowPart);
	SetUiHisGenAcc((UInt32)dboMsgChar->m_HistoryGenAccmulation.LowPart);
	SetInactive(BOOLEAN_FALSE);

	ClearUpdateMask();
}

void CMsgCharacterData::SetData(CCharacterData* pData, BOOL setAll)
{
	
	if (pData->IsUpdated(CCharacterData::idMask) || setAll)
	{
		SetId(pData->GetChar_id());
	}

	if (pData->IsUpdated(CCharacterData::infoMask) || setAll)
	{
		SetInfo(pData->GetNickName(), pData->GetSex(), pData->GetCclass(), pData->GetFaction()); 
	}

	if (pData->IsUpdated(CCharacterData::levelInfoMask) || setAll)
	{
		SetLevel(pData->GetLevel());
	}

	if (pData->IsUpdated(CCharacterData::titleMask) || setAll)
	{
		SetTitle(pData->GetTitle());
	}
	
	if (pData->IsUpdated(CCharacterData::styleMask) || setAll)
	{
		SetStyle(pData->GetPhoto_id(), pData->GetHairStyle1(), pData->GetHairStyle2(), 
			pData->GetHairColor());
	}

	if (pData->IsUpdated(CCharacterData::styleMask) || setAll)
	{
		SetStyle(pData->GetPhoto_id(), pData->GetHairStyle1(), pData->GetHairStyle2(), 
			pData->GetHairColor());
	}

	if (pData->IsUpdated(CCharacterData::eqWeaponMask) || setAll)
	{
		SetEqWeapon(pData->GetEq_weapon(), pData->GetEq_weaponColor());
	}

	if (pData->IsUpdated(CCharacterData::eqHeadMask) || setAll)
	{
		SetEqHead(pData->GetEq_headBand(), pData->GetEq_headColor());
	}

	if (pData->IsUpdated(CCharacterData::eqClothMask) || setAll)
	{
		SetEqCloth(pData->GetEq_cloth(), pData->GetEq_clothColor());
	}

	if (pData->IsUpdated(CCharacterData::orgEqClothMask) || setAll)
	{
		SetOrgEqCloth(pData->GetOrg_eq_cloth(), pData->GetOrg_eq_clothColor());
	}

	if (pData->IsUpdated(CCharacterData::orgHairStyleMask) || setAll)
	{
		SetOrgHairStyle(pData->GetOrg_hairStyle1(), pData->GetOrg_hairColor());
	}

	
	if (pData->IsUpdated(CCharacterData::speakCountMask) || setAll)
	{
		SetSpeakCount(pData->GetSpeakCount());
	}

	if (pData->IsUpdated(CCharacterData::speakBlockMask) || setAll)
	{
		SetSpeakBlock(pData->GetSpeakBlock());
	}

	if (pData->IsUpdated(CCharacterData::offlineTimeMask) || setAll)
	{
		SetOfflineTime(pData->GetOfflineTime());
	}

	if (pData->IsUpdated(CCharacterData::moneyMask) || setAll)
	{
		SetMoney(pData->GetMoney());
	}

	if (pData->IsUpdated(CCharacterData::GenerationAccMask) || setAll)
	{
		SetUiHisGenAcc(pData->GetIHisGenAcc());
	}
}

HRESULT CMsgCharacterData::UpdateDbo(CdboMsgCharacter* dboMsgChar)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CCharacterData UpdateDbo: char_id %d, updateMask %d,%d"), 
		GetChar_id(), m_updateMask.low, m_updateMask.high);

	dboMsgChar->ClearParam();

	bool update = false;
	if (IsUpdated(friend_statusMask))
	{
		COLUMN_UPDATE(dboMsgChar, friend_status, GetFriend_status());
		update = true;
	}

	if (IsUpdated(friend_iconMask))
	{
		COLUMN_UPDATE(dboMsgChar, friend_icon, GetFriend_icon());
		update = true;
	}

	if (IsUpdated(friend_aboutMask))
	{
		COLUMN_UPDATE_STRING(dboMsgChar, friend_about, GetFriend_about());
		update = true;
	}

	if (IsUpdated(friend_replyMask))
	{
		COLUMN_UPDATE_STRING(dboMsgChar, friend_onlineAutoReply, GetFriend_onlineAutoReply());
		COLUMN_UPDATE_STRING(dboMsgChar, friend_offlineAutoReply, GetFriend_offlineAutoReply());
		update = true;
	}

	if (IsUpdated(speakCountMask))
	{
		COLUMN_UPDATE(dboMsgChar, speakCount, GetGlobalSpeakCount());
		update = true;
	}

	if (IsUpdated(titleMask))
	{
		COLUMN_UPDATE_STRING(dboMsgChar, title, GetTitle());
		update = true;
	}

	if (IsUpdated(speakBlockMask))
	{
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetSpeakBlock(), dbtime);
		COLUMN_UPDATE(dboMsgChar, speakBlock, dbtime);
		update = true;
	}

	if (IsUpdated(guild_uidMask))
	{
		COLUMN_UPDATE_LARGE(dboMsgChar, guild_uid, GetGuild_uid());
		update = true;
	}

	// money not need to be updated to DB

	HRESULT hr = S_OK;
	if (update && DB_FAILED(hr = dboMsgChar->UpdateByAny(GetChar_id())))
		dboMsgChar->ShowError(hr);
	else
		ClearUpdateMask();
	dboMsgChar->ClearParam();
	dboMsgChar->Close();

	return hr;
}

HRESULT CMsgCharacterData::DeleteDbo(CdboMsgCharacter* dboMsgChar)
{
	return S_OK;
}

void CMsgCharacterData::PreSend()
{
	if (GetInactive())
	{
		RPCMask mask;
		mask.MaskBits(idMask, infoMask, inactiveMask);
		SetSendMask(mask);
	}
	else
		SetSendMask(AllMask);
}

HRESULT GetDbMsgCharacter(UInt32 char_id, CMsgCharacterData* &pMsgCharData)
{
	TRACE_ENSURE(pdbMsgCharacter != NULL);
	HRESULT hr = S_OK;

	if (DB_SUCCEEDED(hr = pdbMsgCharacter->RSByID(char_id)))
		if (DB_SUCCEEDED(hr = pdbMsgCharacter->MoveFirst()))
			pMsgCharData = NewMsgCharacterData();
	pdbMsgCharacter->Close();

	return hr;
}

DWORD WINAPI QueueGetMsgCharacters(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	MsgCharRequest* pRequest = (MsgCharRequest*) lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		pRequest->context.bCallback = BOOLEAN_FALSE;
		NetGroup charGroup;
		CdboMsgCharacter* dboMsgChar = SafeCreateObject(CdboMsgCharacter);
		String idStr = GetVectorString(pRequest->dbCharIds);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char_ids %s"),
			idStr.c_str());
		charGroup.SetValidate(BOOLEAN_FALSE);
		dboMsgChar->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboMsgChar->RSByIdStr(idStr.c_str())))
		{
			UInt32Vector::iterator it;
			hr = dboMsgChar->MoveFirst();
			while (hr == S_OK)
			{
				// remove normal char_id from dbCharIds
				it = std::find(pRequest->dbCharIds.begin(), pRequest->dbCharIds.end(), 
					(UInt32) dboMsgChar->m_char_id.LowPart);
				if (it != pRequest->dbCharIds.end())
					pRequest->dbCharIds.erase(it);
				CMsgCharacterData* pMsgCharData = SafeCreateObject(CMsgCharacterData);
				pMsgCharData->SetData(dboMsgChar);
				pMsgCharData->PreSend();
				charGroup.Add(pMsgCharData);
				hr = dboMsgChar->MoveNext();
			}
		}
		if (!pRequest->dbCharIds.empty())
		{
			// not all char_id find, someone may be inactive
			idStr = GetVectorString(pRequest->dbCharIds);
			if (DB_SUCCEEDED(hr = dboMsgChar->InactiveIdStr(idStr.c_str())))
			{
				hr = dboMsgChar->MoveFirst();
				while (hr == S_OK)
				{
					CMsgCharacterData* pMsgCharData = SafeCreateObject(CMsgCharacterData);
					pMsgCharData->SetData(dboMsgChar, BOOLEAN_TRUE);
					pMsgCharData->PreSend();
					charGroup.Add(pMsgCharData);
					hr = dboMsgChar->MoveNext();
				}
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetMsgCharacters"), hr, dboMsgChar->GetErrorMsg(hr), 
			pRequest->context, &charGroup);
		dboMsgChar->Close();
		SafeDeleteObject(dboMsgChar);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s"),
			pRequest->context.sessionId.GetString().c_str());
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _F("QueueGetMsgCharacters Profile: utime %d, char_ids %s"), 
			dt, GetVectorString(pRequest->dbCharIds).c_str());
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

RPCResult ObjectService_Stub::GetMessageCharacters(LPCPEER pPeer, RPCContext &context, 
												   const UInt32Vector &charList,
												   const Boolean &bReactive)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC2(ObjectService, GetMessageCharacters, NULL, 
						UInt32Vector, charList, Boolean, bReactive)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetMessageCharacters: session %s, char_ids %s"), 
			context.sessionId.GetString().c_str(), GetVectorString(charList).c_str());
		PROCEDURE_STORE_VALUE1(UInt32, ::GetTickCount())

		HRESULT hr = S_OK;
		NetGroup charGroup;

		CMsgCharacterData* pMsgCharData;
		NetGroup msgCharGroup;
		UInt32 char_id;
		UInt32Vector dbCharIds;

		for (UInt32Vector::const_iterator it = charList.begin(); it != charList.end(); ++it)
		{
			char_id = *it;
			pMsgCharData = FindMsgChar(char_id);
			if (pMsgCharData)
			{
				if (bReactive && pMsgCharData->GetInactive())
				{
					TRACE_INFODTL_2(GLOBAL_LOGGER, 
						_T("GetMessageCharacters: reactive character, session %s, char_id %d"), 
						context.sessionId.GetString().c_str(), pMsgCharData->GetChar_id());
					// found inactive character to reactive
					ReleaseMsgChar(pMsgCharData->GetChar_id());
					pMsgCharData = NULL;
				}
				else
				{
					// find normal or inactive character if not reactive
					//gGlobal.CacheExtendMsgChar(char_id);
					gGlobal.CacheKeepMsgChar(char_id);
					pMsgCharData->PreSend();
					msgCharGroup.Add(pMsgCharData);
				}
			}
			if (pMsgCharData == NULL)
			{
				// character not cached, prevent char_id duplication
				if (std::find(dbCharIds.begin(), dbCharIds.end(), char_id) == dbCharIds.end())
					dbCharIds.push_back(char_id);
			}
		}

		if (dbCharIds.empty()) {			// no db char ids, all found
			// directly send back object group
			CB_GetMsgCharacterGroup(pPeer, context, &msgCharGroup);
			PROCEDURE_RETURN
		} else {
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			MsgCharRequest *pRequest = SafeCreateObject3(MsgCharRequest, 0, pPeer, context);
			pRequest->dbCharIds.swap(dbCharIds);	// swap dbCharIds
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetMsgCharacters, pRequest, DB_WORKER_FLAG);
#else
			QueueGetMsgCharacters(pRequest);
#endif
		}
	
	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, charGroup)

		TRACE_ENSURE(charGroup);

		NetGroup newGroup;

		// create new msg characters
		CMsgCharacterData* pMsgCharData;
		for (Index idx = C_INDEX(0); idx < charGroup->GetCount(); ++idx)
		{
			pMsgCharData = charGroup->GetChild<CMsgCharacterData>(idx);
			if (pMsgCharData)
			{
				// need to register for new char data
				DataObject &dobj = gGlobal.msgCharMap[pMsgCharData->GetChar_id()];
				if (dobj.pobj)
				{
					DeleteNetObject(pMsgCharData);
					pMsgCharData = CastNetObject<CMsgCharacterData>(dobj.pobj);
				}
				else
				{
					LocalRPCManager::GetInstance().Register(pMsgCharData);
					pMsgCharData->ClearUpdateMask();
					dobj.pobj = pMsgCharData;
				}
				if (pMsgCharData)
					//gGlobal.CacheExtendMsgChar(pMsgCharData->GetChar_id());
					gGlobal.CacheKeepMsgChar(pMsgCharData->GetChar_id()); 
			}
		}

		// store new character group
		UInt32 char_id;
		for (UInt32Vector::const_iterator it = charList.begin(); it != charList.end(); ++it)
		{
			char_id = *it;
			pMsgCharData = FindMsgChar(char_id);
			if (pMsgCharData)
			{
				pMsgCharData->PreSend();
				newGroup.Add(pMsgCharData);
			}
		}

		// send char data to peer
		CB_GetMsgCharacterGroup(pPeer, context, &newGroup);
		DeleteNetGroup(charGroup);

		PROCEDURE_RESTORE_VALUE1(UInt32, lastTime)
		UInt32 dt = ::GetTickCount() - lastTime;
		if (dt >= 1000)
		{
			TRACE_WARNDTL_3(GLOBAL_LOGGER, 
				_T("GetMessageCharacters Profile: utime %d, session %s, char_ids %s"), 
				dt, context.sessionId.GetString().c_str(), GetVectorString(charList).c_str());
		}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_T("GetMessageCharacters Error: sessionId %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::KeepMsgCharacter(LPCPEER pPeer, RPCContext &context, 
											const UInt32 &char_id)
{
	CMsgCharacterData* pMsgCharData = FindMsgChar(char_id);

	if (pMsgCharData)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("KeepMsgCharacter: try keep char_id %d"), char_id);
		gGlobal.CacheKeepMsgChar(char_id);
	}

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::ReleaseMsgCharacter(LPCPEER pPeer, RPCContext &context, 
											   const UInt32 &char_id)
{
	CMsgCharacterData* pMsgCharData = FindMsgChar(char_id);

	if (pMsgCharData)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ReleaseMsgCharacter: try release char_id %d"), char_id);
		gGlobal.CacheReleaseMsgChar(char_id);
	}

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateMsgCharacter(LPCPEER pPeer, RPCContext &context, 
											  const UInt32 &char_id, NetObject *msgCharData)
{
	CMsgCharacterData* pNewData = CastNetObject<CMsgCharacterData>(msgCharData);
	TRACE_ENSURE(pdbMsgCharacter && pNewData);

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("UpdateMsgCharacter: sessionId %s, char_id %d"),
		context.sessionId.GetString().c_str(), char_id);

	CMsgCharacterData* pCharData = FindMsgChar(char_id);

	if (pCharData == NULL)
	{
		// not found in cache
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UpdateCharacter Error : character not found sessionId %s, char_id %d"),
			context.sessionId.GetString().c_str(), char_id);
		CB_Error(pPeer, context, 1, _T("UpdateCharacter Error : character not found"));
		return RPC_RESULT_FAIL;
	}

	DataIdMap::iterator iter = gGlobal.msgCharMap.find(char_id);
	pCharData->Clone(pNewData, pNewData->GetUpdateMask());

	if (iter != gGlobal.msgCharMap.end())
		gGlobal.EnqueueUpdate(&iter->second);
	DeleteNetObject(pNewData);

	CB_UpdateMsgCharacter(pPeer, context);
	return RPC_RESULT_OK;
}
