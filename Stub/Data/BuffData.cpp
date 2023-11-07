#include "Stub/Data/BuffData.h"
#include "Stub/Service/ObjectService.h"
#include "Stub/Data/BuffData.h"
#include "ObjectService/dboBuff.h"
#include "ObjectService/Global.h"

REGISTER_CLASSTYPE(CBuffData, BuffData_CLASSID);

CBuffData* NewBuff();
CdboBuff* &pdbBuff = gGlobal.gTables.pBuff;
StringVector CdboBuffAccessor::m_params;

////////////////////////////////////////////////////////////////////////
////						Buff Data							////
////////////////////////////////////////////////////////////////////////
CBuffData* NewBuff()
{
	CBuffData *pBuffData = NULL;
	DataObject *dobj = NULL;
	BuffDataKey BuffKey((UInt16)pdbBuff->m_buff_id, (UInt32)pdbBuff->m_owner_id.LowPart, (UInt8)pdbBuff->m_owner_type);
	//BuffKey.owner_id = pdbBuff->m_owner_id.LowPart;
	//BuffKey.buff_id = (UInt16)pdbBuff->m_buff_id;
	//BuffKey.owner_type = pdbBuff->m_owner_type;
	BuffDataMap::iterator iId = gGlobal.buffMap.find(BuffKey);

	// release network object if already exist
	if (iId != gGlobal.buffMap.end()) {
		dobj = iId->second;
	} else {
		dobj = SafeCreateObject(DataObject);
		dobj->pobj = NULL;
	}

	TRACE_ENSURE(dobj != NULL);
	if (dobj->pobj == NULL)
		pBuffData = CBuffData::NewInstance();
	else
		pBuffData = dynamic_cast<CBuffData *>(dobj->pobj);

	// assign values to Buff data
	TRACE_ENSURE(pBuffData != NULL);
	pBuffData->SetData();
	
	dobj->srcId = 0;
	dobj->dataId = 0;
	dobj->bExist = true;
	dobj->timeout = 0;
	dobj->pobj = (NetObject *) pBuffData;
	gGlobal.buffMap[BuffKey] = dobj;

	return pBuffData;
}

void ShowBuffError(HRESULT hr)
{
	String errMsg;
	pdbBuff->GetError(errMsg);
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _T("Buff DB Error: code %d: %s"), hr, errMsg.c_str());
}

void CBuffData::SetData()
{
	SetInfo((UInt16) pdbBuff->m_buff_id);
	SetOwner_info((UInt32)pdbBuff->m_owner_id.LowPart, 
		(UInt8)pdbBuff->m_owner_type);
	SetValue((UInt16) pdbBuff->m_buff_value);
	SetCoolDown((UInt32) pdbBuff->m_cool_down.LowPart);
}

void CBuffData::UpdateDbo()
{
	pdbBuff->ClearParam();
	//if (IsUpdated(idMask)) 
	//{
	//	COLUMN_UPDATE_LARGE(pdbItem, item_uid, GetItem_uid());
	//	COLUMN_UPDATE_LARGE(pdbItem, item_id, GetItem_id());
	//}
	if (IsUpdated(owner_infoMask)) {
		COLUMN_UPDATE_LARGE_QUA(pdbBuff, owner_id, GetOwner_id());
		COLUMN_UPDATE(pdbBuff, owner_type, GetOwner_type());
	}
	if (IsUpdated(infoMask)) {
		COLUMN_UPDATE(pdbBuff, buff_id, GetBuff_id());
	}

	if (IsUpdated(ValueMask)) {
		COLUMN_UPDATE(pdbBuff, buff_value, GetBuff_value());
	}

	if (IsUpdated(CoolDownMask)) {
		COLUMN_UPDATE_LARGE_QUA(pdbBuff, cool_down, GetCool_down());
	}
}
RPCResult ObjectService_Stub::CreateBuffList(const Peer &peer, RPCContext &context, const UInt32 &owner_id, const UInt16Vector &buff_ids, 
											 const UInt8 &owner_type, const UInt16Vector &buff_values, const UInt32Vector &buff_CoolDowns)
{
	BuffGroupKey BuffGKey(owner_id, owner_type);
	NetGroup* BuffGroup = NULL;

	BuffGroupMap::iterator Itr = gGlobal.buffGroupMap.find(BuffGKey);
	if (Itr != gGlobal.buffGroupMap.end())
	{
		BuffGroup = Itr->second;
	}
	else
	{
		BuffGroup = SafeCreateObject(NetGroup);
		gGlobal.buffGroupMap[BuffGKey] = BuffGroup;
	}

	NetGroup NewBuffs;

	for (Int i = 0; i < (Int)buff_ids.size(); i++)
	{
		if (i >= (Int)buff_values.size() ||
			i >= (Int)buff_CoolDowns.size())
			break;

		HRESULT hr = S_OK;

		if (DB_FAILED(hr = pdbBuff->Insert(buff_ids[i], owner_id, owner_type, buff_values[i], buff_CoolDowns[i])))
		{
			ShowBuffError(hr);
			//return CB_Error(peer, context, hr, _T("DeleteItem DB_FAILED:"));
		}

		pdbBuff->Close();

		CBuffData * pBuffData = NewBuff();
		BuffGroup->Add(pBuffData);

		pBuffData->PreSend();
		NewBuffs.Add(pBuffData);
	}

	CBuffData::PreSendGroup(&NewBuffs);
	CB_GetBuffLst(peer, context, &NewBuffs);

	return RPC_RESULT_OK;
}
RPCResult ObjectService_Stub::CreateBuff(const Peer &peer, RPCContext &context, const UInt32 &owner_id, const UInt16 &buff_id, 
										 const UInt8 &owner_type, const UInt16 &buff_value, const UInt32 &buff_CoolDown)
{
	HRESULT hr = S_OK;

	if (DB_FAILED(hr = pdbBuff->Insert(buff_id, owner_id, owner_type, buff_value, buff_CoolDown)))
	{
		ShowBuffError(hr);
		return CB_Error(peer, context, hr, _T("CreateBuff DB_FAILED:"));
	}

	pdbBuff->Close();

	CBuffData * pBuffData = NewBuff();

	BuffGroupKey BuffGKey(owner_id, owner_type);
	NetGroup* BuffGroup = NULL;

	BuffGroupMap::iterator Itr = gGlobal.buffGroupMap.find(BuffGKey);
	if (Itr != gGlobal.buffGroupMap.end())
	{
		BuffGroup = Itr->second;
	}
	else
	{
		BuffGroup = SafeCreateObject(NetGroup);
		gGlobal.buffGroupMap[BuffGKey] = BuffGroup;
	}

	BuffGroup->Add(pBuffData);

	pBuffData->PreSend();
	CB_CreateBuffDone(peer, context, pBuffData);
	
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Stub::UpdateBuff(const Peer &peer, RPCContext &context, const UInt32 &owner_id, 
										  const UInt16 &buff_id, const UInt8 &owner_type, Buffer & buffer)
{
	HRESULT hr = S_OK;

	BuffDataKey BuffKey(buff_id, owner_id, owner_type);

	CBuffData* pBuffData = NULL;
	BuffDataMap::iterator iter = gGlobal.buffMap.find(BuffKey);
	if (iter != gGlobal.buffMap.end())
	{
		// found existing Item in cache
		pBuffData = (CBuffData *) iter->second->pobj;
		//pBuffData->SetData();
		//pBuffData->
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _T("UpdateBuff DB_FAILED:, hr=%X"), hr);
		return CB_Error(peer, context, hr, _T("UpdateBuff DB_FAILED:"));
	}

	Size size = buffer.GetSize();
	buffer.ResetCursor();
	pBuffData->ClearUpdateMask();		// clear update mask for further update synchronize
	pBuffData->ReadAttr(buffer);
	pBuffData->UpdateDbo();

	if (DB_FAILED(hr = pdbBuff->UpdateByAny(buff_id, owner_id, owner_type)))
	{
		ShowBuffError(hr);
		return CB_Error(peer, context, hr, _T("UpdateBuff DB_FAILED:"));
	}

	pdbBuff->Close();

	CB_UpdateBuff(peer, context);
	return RPC_RESULT_OK;
}
RPCResult ObjectService_Stub::DeleteBuff(const Peer &peer, RPCContext &context, const UInt32 &owner_id, const UInt16 &buff_id, const UInt8 &owner_type)
{
	BuffGroupKey BuffGKey(owner_id, owner_type);
	BuffDataKey BuffKey(buff_id, owner_id, owner_type);
	HRESULT hr = S_OK;
	CBuffData* pBuffData = NULL;
	NetGroup* BuffGroup = NULL;
	BuffDataMap::iterator iter = gGlobal.buffMap.find(BuffKey);
	if (iter != gGlobal.buffMap.end())
	{
		// found existing Item in cache
		pBuffData = (CBuffData *) iter->second->pobj;
		BuffGroupMap::iterator it2 = gGlobal.buffGroupMap.find(BuffGKey);
		if (it2 != gGlobal.buffGroupMap.end())
		{
			// found existing in ItemGroup cache
			it2->second->Remove(pBuffData);
		}
		gGlobal.buffMap.erase(iter);
		DeleteNetObject(pBuffData);
	}
	

	if (DB_FAILED(hr = pdbBuff->Delete(buff_id, owner_id, owner_type)))
	{
		ShowBuffError(hr);
		return CB_Error(peer, context, hr, _T("DeleteBuff DB_FAILED:"));
	}

	pdbBuff->Close();

	CB_DeleteBuffDone(peer, context);

	return RPC_RESULT_OK;
}
RPCResult ObjectService_Stub::DeleteBuffList(const Peer &peer, RPCContext &context, const UInt32 &owner_id, const UInt16Vector &buff_ids, const UInt8 &owner_type)
{
	BuffGroupKey BuffGKey(owner_id, owner_type);

	for (Int i = 0; i < (Int)buff_ids.size(); i++)
	{
		BuffDataKey BuffKey(buff_ids[i], owner_id, owner_type);
		HRESULT hr = S_OK;
		CBuffData* pBuffData = NULL;
		NetGroup* BuffGroup = NULL;
		BuffDataMap::iterator iter = gGlobal.buffMap.find(BuffKey);
		if (iter != gGlobal.buffMap.end())
		{
			// found existing Item in cache
			pBuffData = (CBuffData *) iter->second->pobj;
			BuffGroupMap::iterator it2 = gGlobal.buffGroupMap.find(BuffGKey);
			if (it2 != gGlobal.buffGroupMap.end())
			{
				// found existing in ItemGroup cache
				it2->second->Remove(pBuffData);
			}
			gGlobal.buffMap.erase(iter);
			DeleteNetObject(pBuffData);
		}

		if (DB_FAILED(hr = pdbBuff->Delete(buff_ids[i], owner_id, owner_type)))
		{
			ShowBuffError(hr);
			continue;
		}
		pdbBuff->Close();
	}

	CB_DeleteBuffDone(peer, context);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetBuffLst(const Peer &peer, RPCContext &context, const UInt32 &owner_id, const UInt8 &owner_type)
{
	BuffGroupKey BuffGKey(owner_id, owner_type);

	BuffGroupMap::iterator Itr = gGlobal.buffGroupMap.find(BuffGKey);

	NetGroup* BuffGroup = NULL;
	CBuffData* pBuffData = NULL;
	if (Itr != gGlobal.buffGroupMap.end())
	{
		BuffGroup = Itr->second;
	}
	else
	{
		HRESULT hr = S_OK;

		BuffGroup = SafeCreateObject(NetGroup);

		TRACE_ENSURE(pdbBuff != NULL && BuffGroup != NULL);
		if (DB_SUCCEEDED(hr = pdbBuff->RSByID(owner_id, owner_type)))
		{
			hr = pdbBuff->MoveFirst();
			while (hr == S_OK)
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _T("GetBuff Buff_id: %ld, Buff_value: %ld\n"), 
					pdbBuff->m_buff_id, pdbBuff->m_buff_value);
				pBuffData = NewBuff();
				BuffGroup->Add(pBuffData);
				hr = pdbBuff->MoveNext();
			}
			gGlobal.buffGroupMap[BuffGKey] = BuffGroup;
		}
		pdbBuff->Close();
		if (DB_FAILED(hr) && hr != DB_S_ENDOFROWSET)
		{
			ShowBuffError(hr);
			CB_Error(peer, context, hr, _T("GetItem Error."));
			DeleteNetGroup(BuffGroup);
			return RPC_RESULT_FAIL;
		}
	}
	
	if (BuffGroup)
	{
		CBuffData::PreSendGroup(BuffGroup);
		CB_GetBuffLst(peer, context, BuffGroup);
	}

	return RPC_RESULT_OK;
}

// set item data detail in group
void CBuffData::PreSendGroup(NetGroup * buffGroup)
{
	if (buffGroup == NULL) return;

	for (UInt16 i = 0; i < buffGroup->GetCount(); i++) {
		CBuffData *pBuffData = dynamic_cast<CBuffData*>(buffGroup->GetChild(i));
		if (pBuffData)
			pBuffData->PreSend();
	}
}

void CBuffData::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}
