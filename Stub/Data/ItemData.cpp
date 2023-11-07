//-- Common
#include "Common.h"
#include "Common/DataCode.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Data/ItemData.h"
//-- Library
#include "Stub/Service/ObjectService.h"
#include "ObjectService/dboItem.h"
#include "ObjectService/Global.h"
#include "ObjectService/DboSourcePool.h"
#include "Resource/ResItem.h"
#include "Resource/ServerResManager.h"
#include "Common/common_binary.h"

#define ELEMENT_TOTAL			6
#define ELEMENT_RANDOM			255

#define BAG_TYPE_COUNT		5
#define BAG_ITEM_COUNT		24
#define BAG_EQUIP_COUNT		9
#define BAG_MAX_ITEM		(BAG_TYPE_COUNT * BAG_ITEM_COUNT)
#define BAG_MAX_SIZE		(BAG_MAX_ITEM + BAG_EQUIP_COUNT)
#define BAG_CURRENT			0
#define BAG_BACKUP			1
#define BAG_BACKPACK		2
#define BAG_EQUIPMENT		5

REGISTER_CLASSTYPE(CItemData, ItemData_CLASSID);

//typedef WorkRequest<UInt32> ItemRequest;

struct ItemRequest : public WorkRequest<UInt32>
{
	UInt8 inWareHouse;
	ItemRequest(UInt32 id, LPCPEER pPeer, RPCContext &context)
		: WorkRequest<UInt32>(id, pPeer, context)
	{}
};

CItemData* NewItem();
CdboItem* &pdbItem = gGlobal.gTables.pItem;
StringVector CdboItemAccessor::m_params;
ThreadKey CItemData::m_key;

NetGroup* FindItemGroup(UInt32 char_id);
NetGroup* FindWareHouseItemGroup(UInt32 char_id);

////////////////////////////////////////////////////////////////////////
////						Item Data								////
////////////////////////////////////////////////////////////////////////
CItemData* NewItem()
{
	CItemData *pItemData = NULL;
	UInt32 item_uid = (UInt32) pdbItem->m_item_uid.LowPart;
	UInt32 owner_id = (UInt32) pdbItem->m_owner_id.LowPart;
	DataObject &dobj = gGlobal.itemIdMap[item_uid];

	if (dobj.pobj == NULL)
	{
		pItemData = CItemData::NewInstance();
		pItemData->SetData(pdbItem);			// assign values to character data
		dobj.pobj = pItemData;
	}
	else
	{
		pItemData = CastNetObject<CItemData>(dobj.pobj);
	}

	if (pItemData)
	{
		// found existing item group in cache
		NetGroup* itemGroup = NULL;
		if (pItemData->GetInWareHouse() == 0)
			itemGroup = FindItemGroup(owner_id);
		else
			itemGroup = FindWareHouseItemGroup(owner_id);

		if (itemGroup)
		{
			itemGroup->Add(pItemData);
		}
		else
		{
			if (pItemData->GetInWareHouse() == 0)
			{
				DataObject& dobj3 = gGlobal.itemGroupMap[owner_id];
				if (dobj3.pobj == NULL)
				{
					itemGroup = SafeCreateObject(NetGroup);
					dobj3.pobj = itemGroup;
				}
				else
					itemGroup = (NetGroup*) dobj3.pobj;
				
				itemGroup->Add(pItemData);
			}
			else
			{
				DataObject& dobj3 = gGlobal.wareHouseItemGroupMap[owner_id];
				if (dobj3.pobj == NULL)
				{
					itemGroup = SafeCreateObject(NetGroup);
					dobj3.pobj = itemGroup;
				}
				else
					itemGroup = (NetGroup*) dobj3.pobj;
				
				itemGroup->Add(pItemData);
			}
		}
	}

	return pItemData;
}

CItemData* FindItem(UInt32 item_uid)
{
	DataIdMap::iterator iter = gGlobal.itemIdMap.find(item_uid);
	CItemData* pItemData = NULL;

	// found existing account in cache
	if (iter != gGlobal.itemIdMap.end())
		pItemData = CastNetObject<CItemData>(iter->second.pobj);

	return pItemData;
}

NetGroup* FindItemGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.itemGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.itemGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

NetGroup* FindWareHouseItemGroup(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.wareHouseItemGroupMap.find(char_id);
	NetGroup* grp = NULL;

	// found existing account in cache
	if (iter != gGlobal.wareHouseItemGroupMap.end())
		grp = CastNetObject<NetGroup>(iter->second.pobj);

	return grp;
}

void RemoveItem(UInt32 item_uid)
{
	DataIdMap::iterator iter = gGlobal.itemIdMap.find(item_uid);
	
	if (iter != gGlobal.itemIdMap.end())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("item_uid %d"), item_uid);

		DataObject* dobj = &iter->second;
		CItemData* pItemData = CastNetObject<CItemData>(dobj->pobj);
		if (pItemData)
		{
			NetGroup *grp = NULL;
			grp = FindItemGroup(pItemData->GetOwner_id());
			if (grp)
				grp->Remove(pItemData);
			grp = FindWareHouseItemGroup(pItemData->GetOwner_id());
			if (grp)
				grp->Remove(pItemData);

			gGlobal.DequeueUpdate(dobj);			// remove from update queue
			gGlobal.m_deleteGroup.Add(pItemData);	// add to delete group
		}
		gGlobal.itemIdMap.erase(iter);
	}
}

void ReleaseItems(UInt32 char_id)
{
	GroupMap::iterator iter = gGlobal.itemGroupMap.find(char_id);

	if (iter != gGlobal.itemGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CItemData *pItemData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++)
		{
			pItemData = grp->GetChild<CItemData>(i);
			if (pItemData)
			{
				// remove from id map
				DataIdMap::iterator it = gGlobal.itemIdMap.find(pItemData->GetItem_uid());
				if (it != gGlobal.itemIdMap.end())
				{
					DataObject* dobj = &it->second;
					gGlobal.DequeueUpdate(dobj);		// remove from update queue
					gGlobal.itemIdMap.erase(it);
					// add to update group for DB
					if (pItemData->IsUpdated())
						gGlobal.m_releaseGroup.Add(pItemData);
					else
						DeleteNetObject(pItemData);
				}
			}
		}
		gGlobal.itemGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}

	iter = gGlobal.wareHouseItemGroupMap.find(char_id);

	if (iter != gGlobal.wareHouseItemGroupMap.end())
	{
		NetGroup* grp = CastNetObject<NetGroup>(iter->second.pobj);
		CItemData *pItemData;
	
		if (grp == NULL) return;
		for (UInt i = 0; i < grp->GetCount(); i++)
		{
			pItemData = grp->GetChild<CItemData>(i);
			if (pItemData)
			{
				// remove from id map
				DataIdMap::iterator it = gGlobal.itemIdMap.find(pItemData->GetItem_uid());
				if (it != gGlobal.itemIdMap.end())
				{
					DataObject* dobj = &it->second;
					gGlobal.DequeueUpdate(dobj);		// remove from update queue
					gGlobal.itemIdMap.erase(it);
					// add to update group for DB
					if (pItemData->IsUpdated())
						gGlobal.m_releaseGroup.Add(pItemData);
					else
						DeleteNetObject(pItemData);
				}
			}
		}
		gGlobal.wareHouseItemGroupMap.erase(iter);
		SafeDeleteObject(grp);
	}
}

DWORD WINAPI QueueGetItems(LPVOID lpParam)
{
	HRESULT hr = S_OK;
	UInt32 curTime = ::GetTickCount();

	ItemRequest* pRequest = (ItemRequest*) lpParam;
	TRACE_ENSURE(pRequest);
	CDboSourcePool& dbPool = gGlobal.gameDbPool;
	CDboSource* dbSrc = dbPool.GetAvailSource();
	if (dbSrc)
	{
		NetGroup itemGroup;
		CItemData* pItemData;
		CdboItem* dboItem = SafeCreateObject(CdboItem);

		itemGroup.SetValidate(BOOLEAN_FALSE);
		dboItem->SetDataSource(dbSrc);
		if (DB_SUCCEEDED(hr = dboItem->RSByOwnerID(pRequest->key, pRequest->inWareHouse)))
		{
			hr = dboItem->MoveFirst();
			while (hr == S_OK)
			{
				pItemData = SafeCreateObject(CItemData);
				pItemData->SetData(dboItem);
				pItemData->SetDetail(CItemData::DETAIL_ALL);
				itemGroup.Add(pItemData);
				hr = dboItem->MoveNext();
			}
		}
		gGlobal.CB_QueueNetGroup(_T("QueueGetItems"), hr, dboItem->GetErrorMsg(hr), 
			pRequest->context, &itemGroup);
		dboItem->Close();
		SafeDeleteObject(dboItem);
		dbPool.ReleaseSource(dbSrc);
	}
	else
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("cannot get data source, session_id %s, owner_id %d"),
			pRequest->context.sessionId.GetString().c_str(), pRequest->key);
	}
	// show query time
	UInt32 dt = ::GetTickCount() - curTime;
	if (dt > 1000)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("profile, utime %d, owner_id %d"), 
			dt, pRequest->key);
	}
	SafeDeleteObject(pRequest);
	gGlobal.WorkQueueEnd();

	return 0;
}

void CItemData::SetData(CdboItem* dboItem)
{
	SetId((UInt32) dboItem->m_item_uid.LowPart,	(UInt32) dboItem->m_item_id.LowPart);
	SetOwner_id((UInt32) dboItem->m_owner_id.LowPart);
	SetBagIndex((UInt8) dboItem->m_bagIndex); 
	SetMaker(dboItem->m_maker);
	SetEq_element((UInt8) dboItem->m_eq_element);
	SetEndure((UInt16) dboItem->m_endure);
	SetMEndure((UInt16) dboItem->m_mEndure);
	SetGrade((UInt8) dboItem->m_grade);
	SetUpgExp((UInt16) dboItem->m_upg_exp);
	SetRemain((UInt8) dboItem->m_remain);
	SetStack((UInt8) dboItem->m_stack);
	SetBaseAttr((Int16) dboItem->m_base_STA, 
				(Int16) dboItem->m_base_SPR, 
				(Int16) dboItem->m_base_STR, 
				(Int16) dboItem->m_base_CON, 
				(Int16) dboItem->m_base_AGI,
				(Int16) dboItem->m_base_ATK,
				(Int16) dboItem->m_base_DEF,
				(Int16) dboItem->m_base_HIT,
				(Int16) dboItem->m_base_EVA,
				(Int16) dboItem->m_base_POW,
				(Int16) dboItem->m_base_SPD,
				(Int16) dboItem->m_base_maxHP,
				(Int16) dboItem->m_base_maxSP,
				(Int16) dboItem->m_base_maxVP);
	SetUpgAttr((Int16) dboItem->m_upg_STA, 
				(Int16) dboItem->m_upg_SPR, 
				(Int16) dboItem->m_upg_STR, 
				(Int16) dboItem->m_upg_CON, 
				(Int16) dboItem->m_upg_AGI,
				(Int16) dboItem->m_upg_ATK,
				(Int16) dboItem->m_upg_DEF,
				(Int16) dboItem->m_upg_HIT,
				(Int16) dboItem->m_upg_EVA,
				(Int16) dboItem->m_upg_POW,
				(Int16) dboItem->m_upg_SPD,
				(Int16) dboItem->m_upg_maxHP,
				(Int16) dboItem->m_upg_maxSP,
				(Int16) dboItem->m_upg_maxVP);
	SetInlayGem((UInt8) dboItem->m_eq_gem, 
				(UInt16) dboItem->m_inlay_gem0, 
				(UInt16) dboItem->m_inlay_gem1, 
				(UInt16) dboItem->m_inlay_gem2, 
				(UInt16) dboItem->m_inlay_gem3, 
				(UInt16) dboItem->m_inlay_gem4, 
				(UInt16) dboItem->m_inlay_gem5, 
				(UInt16) dboItem->m_inlay_gem6, 
				(UInt16) dboItem->m_inlay_gem7, 
				(UInt16) dboItem->m_inlay_gem8, 
				(UInt16) dboItem->m_inlay_gem9);
	SetInlayAttr((Int16) dboItem->m_inlay_STA, 
					(Int16) dboItem->m_inlay_SPR, 
					(Int16) dboItem->m_inlay_STR, 
					(Int16) dboItem->m_inlay_CON, 
					(Int16) dboItem->m_inlay_AGI,
					(Int16) dboItem->m_inlay_ATK,
					(Int16) dboItem->m_inlay_DEF,
					(Int16) dboItem->m_inlay_HIT,
					(Int16) dboItem->m_inlay_EVA,
					(Int16) dboItem->m_inlay_POW,
					(Int16) dboItem->m_inlay_SPD,
					(Int16) dboItem->m_inlay_maxHP,
					(Int16) dboItem->m_inlay_maxSP,
					(Int16) dboItem->m_inlay_maxVP);
	SetAddon((UInt16) dboItem->m_addon);
	SetEqColor((UInt16) dboItem->m_eqColor);
	SetQuality((UInt16) dboItem->m_quality);
	SetSoulboundState((UInt8) dboItem->m_soulboundState);
	SetIdentifyState((UInt8) dboItem->m_IdentifyState);

	time_t uTime;
	TimeConvert(dboItem->m_ExpireDate, uTime);
	SetExpireDate((UInt32)uTime);

	SetValue((UInt16)dboItem->m_Value1, (UInt16)dboItem->m_Value2, (UInt16)dboItem->m_Value3);

	SetBuffID((UInt16)dboItem->m_buff_id);
	SetBuffValue1((UInt32)dboItem->m_buff_Value1.LowPart);
	SetBuffValue2((UInt32)dboItem->m_buff_Value2.LowPart); 
	SetBuffValue3((UInt32)dboItem->m_buff_Value3.LowPart);
	SetBuffCoolDown((UInt32)dboItem->m_buff_CoolDown.LowPart);
	SetBuffPro((UInt8)dboItem->m_buff_Pro);
	SetInWareHouse((UInt8)dboItem->m_inWareHouse);
	SetLock((Boolean)(dboItem->m_lock == 1));
	SetSkillID((UInt16)dboItem->m_skill_id);
	ClearUpdateMask();
}

void CItemData::SetDetail(UInt8 detail)
{
	RPCMask mask;

	switch (detail)
	{
	case DETAIL_NONE:
		break;

	case DETAIL_INFO:
		mask.MaskBits(idMask, owner_idMask, bagIndexMask, stackMask, eqColorMask, -1);
		break;

	case DETAIL_MAP:
		mask.MaskBits(idMask, owner_idMask, bagIndexMask, makerMask, gradeMask, 
			eq_elementMask, endureMask, mEndureMask, upgExpMask, 
			remainMask, stackMask, baseAttrMask, upgAttrMask, 
			inlayGemMask, inlayAttrMask, addonMask, buffIDMask, buffValue1Mask, buffValue2Mask, buffValue3Mask, buffCoolDownMask, buffProMask, eqColorMask,
			qualityMask, soulboundStateMask, IdentifyStateMask, ExpireDateMask, ValueMask, inWareHouseMask, assetLockMask, skillIDMask, -1);
		break;

	case DETAIL_ALL:
		mask.low = 0xFFFFFFFF;
		mask.high = 0xFFFFFFFF;
		break;
	}

	SetSendMask(mask);
}

// set item data detail in group
void CItemData::SetGroupDetail(NetGroup *itemGroup, UInt8 detail)
{
	if (itemGroup == NULL) return;

	for (UInt16 i = 0; i < itemGroup->GetCount(); i++)
	{
		CItemData *pItemData = itemGroup->GetChild<CItemData>(i);
		if (pItemData)
			pItemData->SetDetail(detail);
		else
			itemGroup->Replace(i, NULL);
	}
}

HRESULT CItemData::UpdateDbo(CdboItem* dboItem)
{
	if (!IsUpdated()) return S_OK;			// no need update

	ThreadGuard guard(m_key);
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("item_uid %d, owner_id %d, update_mask %d:%d"), 
		GetItem_uid(), GetOwner_id(), m_updateMask.low, m_updateMask.high);

	dboItem->ClearParam();

	if (IsUpdated(idMask))
	{
		COLUMN_UPDATE_LARGE(dboItem, item_id, GetItem_id());
	}
	if (IsUpdated(owner_idMask)) {
		COLUMN_UPDATE_LARGE(dboItem, owner_id, GetOwner_id());
	}
	if (IsUpdated(bagIndexMask)) {
		COLUMN_UPDATE(dboItem, bagIndex, GetBagIndex());
	}
	if (IsUpdated(makerMask)) {
		COLUMN_UPDATE_STRING(dboItem, maker, GetMaker());
	}
	if (IsUpdated(eq_elementMask)) {
		COLUMN_UPDATE(dboItem, eq_element, GetEq_element());
	}
	if (IsUpdated(endureMask)) {
		COLUMN_UPDATE(dboItem, endure, GetEndure());
	}
	if (IsUpdated(mEndureMask)) {
		COLUMN_UPDATE(dboItem, mEndure, GetMEndure());
	}
	if (IsUpdated(gradeMask)) {
		COLUMN_UPDATE(dboItem, grade, GetGrade());
	}
	if (IsUpdated(upgExpMask)) {
		COLUMN_UPDATE(dboItem, upg_exp, GetUpg_exp());
	}
	if (IsUpdated(remainMask)) {
		COLUMN_UPDATE(dboItem, remain, GetRemain());
	}
	if (IsUpdated(stackMask)) {
		COLUMN_UPDATE(dboItem, stack, GetStack());
	}
	if (IsUpdated(baseAttrMask)) {
		COLUMN_UPDATE(dboItem, base_STA, GetBase_STA());
		COLUMN_UPDATE(dboItem, base_SPR, GetBase_SPR());
		COLUMN_UPDATE(dboItem, base_STR, GetBase_STR());
		COLUMN_UPDATE(dboItem, base_CON, GetBase_CON());
		COLUMN_UPDATE(dboItem, base_AGI, GetBase_AGI());
		COLUMN_UPDATE(dboItem, base_ATK, GetBase_ATK());
		COLUMN_UPDATE(dboItem, base_DEF, GetBase_DEF());
		COLUMN_UPDATE(dboItem, base_HIT, GetBase_HIT());
		COLUMN_UPDATE(dboItem, base_EVA, GetBase_EVA());
		COLUMN_UPDATE(dboItem, base_POW, GetBase_POW());
		COLUMN_UPDATE(dboItem, base_SPD, GetBase_SPD());
		COLUMN_UPDATE(dboItem, base_maxHP, GetBase_maxHP());
		COLUMN_UPDATE(dboItem, base_maxSP, GetBase_maxSP());
		COLUMN_UPDATE(dboItem, base_maxVP, GetBase_maxVP());
	}
	if (IsUpdated(upgAttrMask)) {
		COLUMN_UPDATE(dboItem, upg_STA, GetUpg_STA());
		COLUMN_UPDATE(dboItem, upg_SPR, GetUpg_SPR());
		COLUMN_UPDATE(dboItem, upg_STR, GetUpg_STR());
		COLUMN_UPDATE(dboItem, upg_CON, GetUpg_CON());
		COLUMN_UPDATE(dboItem, upg_AGI, GetUpg_AGI());
		COLUMN_UPDATE(dboItem, upg_ATK, GetUpg_ATK());
		COLUMN_UPDATE(dboItem, upg_DEF, GetUpg_DEF());
		COLUMN_UPDATE(dboItem, upg_HIT, GetUpg_HIT());
		COLUMN_UPDATE(dboItem, upg_EVA, GetUpg_EVA());
		COLUMN_UPDATE(dboItem, upg_POW, GetUpg_POW());
		COLUMN_UPDATE(dboItem, upg_SPD, GetUpg_SPD());
		COLUMN_UPDATE(dboItem, upg_maxHP, GetUpg_maxHP());
		COLUMN_UPDATE(dboItem, upg_maxSP, GetUpg_maxSP());
		COLUMN_UPDATE(dboItem, upg_maxVP, GetUpg_maxVP());
	}
	if (IsUpdated(inlayGemMask)) {
		COLUMN_UPDATE(dboItem, eq_gem, GetEq_gem());
		COLUMN_UPDATE(dboItem, inlay_gem0, GetInlay_gem(0));
		COLUMN_UPDATE(dboItem, inlay_gem1, GetInlay_gem(1));
		COLUMN_UPDATE(dboItem, inlay_gem2, GetInlay_gem(2));
		COLUMN_UPDATE(dboItem, inlay_gem3, GetInlay_gem(3));
		COLUMN_UPDATE(dboItem, inlay_gem4, GetInlay_gem(4));
		COLUMN_UPDATE(dboItem, inlay_gem5, GetInlay_gem(5));
		COLUMN_UPDATE(dboItem, inlay_gem6, GetInlay_gem(6));
		COLUMN_UPDATE(dboItem, inlay_gem7, GetInlay_gem(7));
		COLUMN_UPDATE(dboItem, inlay_gem8, GetInlay_gem(8));
		COLUMN_UPDATE(dboItem, inlay_gem9, GetInlay_gem(9));
	}
	if (IsUpdated(inlayAttrMask)) {
		COLUMN_UPDATE(dboItem, inlay_STA, GetInlay_STA());
		COLUMN_UPDATE(dboItem, inlay_SPR, GetInlay_SPR());
		COLUMN_UPDATE(dboItem, inlay_STR, GetInlay_STR());
		COLUMN_UPDATE(dboItem, inlay_CON, GetInlay_CON());
		COLUMN_UPDATE(dboItem, inlay_AGI, GetInlay_AGI());
		COLUMN_UPDATE(dboItem, inlay_ATK, GetInlay_ATK());
		COLUMN_UPDATE(dboItem, inlay_DEF, GetInlay_DEF());
		COLUMN_UPDATE(dboItem, inlay_HIT, GetInlay_HIT());
		COLUMN_UPDATE(dboItem, inlay_EVA, GetInlay_EVA());
		COLUMN_UPDATE(dboItem, inlay_POW, GetInlay_POW());
		COLUMN_UPDATE(dboItem, inlay_SPD, GetInlay_SPD());
		COLUMN_UPDATE(dboItem, inlay_maxHP, GetInlay_maxHP());
		COLUMN_UPDATE(dboItem, inlay_maxSP, GetInlay_maxSP());
		COLUMN_UPDATE(dboItem, inlay_maxVP, GetInlay_maxVP());
	}
	if (IsUpdated(addonMask)) {
		COLUMN_UPDATE(dboItem, addon, GetAddon());
	}
	if (IsUpdated(buffIDMask)) {
		//COLUMN_UPDATE_STRING(pdbItem, buff, GetBuff());
		COLUMN_UPDATE(dboItem, buff_id, GetBuffID());
	}

	if (IsUpdated(eqColorMask))
	{
		COLUMN_UPDATE(dboItem, eqColor, GetEqColor());
	}

	if (IsUpdated(buffValue1Mask))
		COLUMN_UPDATE_LARGE(dboItem, buff_Value1, GetBuffValue1());
	if (IsUpdated(buffValue2Mask))
		COLUMN_UPDATE_LARGE(dboItem, buff_Value2, GetBuffValue2());
	if (IsUpdated(buffValue3Mask))
		COLUMN_UPDATE_LARGE(dboItem, buff_Value3, GetBuffValue3());
	if (IsUpdated(buffCoolDownMask))
		COLUMN_UPDATE_LARGE(dboItem, buff_CoolDown, GetBuffCoolDown());
	if (IsUpdated(buffProMask))
		COLUMN_UPDATE(dboItem, buff_Pro, GetBuffPro());

	if (IsUpdated(qualityMask)) {
		COLUMN_UPDATE(dboItem, quality, GetItem_quality());
	}

	if (IsUpdated(soulboundStateMask)) {
		COLUMN_UPDATE(dboItem, soulboundState, GetItem_soulboundState());
	}

	if (IsUpdated(IdentifyStateMask)) {
		COLUMN_UPDATE(dboItem, IdentifyState, GetItem_IdentifyState());
	}

	if (IsUpdated(ExpireDateMask)) {
		DBTIMESTAMP dbtime;
		TimeConvert((time_t) GetItem_expireDate(), dbtime);
		COLUMN_UPDATE(dboItem, ExpireDate, dbtime);
	}

	if (IsUpdated(ValueMask)) {
		COLUMN_UPDATE(dboItem, Value1, GetItemValue1());
		COLUMN_UPDATE(dboItem, Value2, GetItemValue2());
		COLUMN_UPDATE(dboItem, Value3, GetItemValue3());
	}
	if (IsUpdated(inWareHouseMask))
	{
		COLUMN_UPDATE(dboItem, inWareHouse, GetInWareHouse());
	}

	if (IsUpdated(assetLockMask)) {
		UInt8 locked = GetLock()? 1:0;
		COLUMN_UPDATE(dboItem, lock, locked);
	}

	if (IsUpdated(skillIDMask)) {
		COLUMN_UPDATE(dboItem, skill_id, GetSkillID());
	}

	

	HRESULT hr;
	if (DB_FAILED(hr = dboItem->UpdateByAny(GetItem_uid())))
		dboItem->ShowError(hr);
	else
		ClearUpdateMask();
	dboItem->ClearParam();
	dboItem->Close();

	return hr;
}

HRESULT CItemData::DeleteDbo(CdboItem* dboItem)
{
	HRESULT hr;

	if (DB_FAILED(hr = dboItem->Delete(GetItem_uid())))
		dboItem->ShowError(hr);
	dboItem->Close();

	return hr;
}

/*
HRESULT DoCreateItem(UInt32 item_id, UInt32 owner_id, UInt8 bagIndex, UInt8 stack, 
					 CItemData* &pItemData, String maker, UInt8 inWareHouse, 
					 UInt16 quality, Boolean lock)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CreateItem: item_id %d, owner_id %d, bagIndex %d"),
		item_id, owner_id, bagIndex);

	HRESULT hr = S_OK;
	// get item data from resource
	const ItemData* data = gGlobal.m_resource->GetItemData(item_id);
	if (data == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CreateItem DB_FAILED: item_id %d not found in resource"), item_id);
		return S_FALSE;
	}

	UInt8 useCount = 1;
	// random element if necessary
	UInt8 element = (data->eq_baseElement == ELEMENT_RANDOM) ? 
						GetRand(ELEMENT_TOTAL) : data->eq_baseElement;
	UInt16 endure = 0;
	UInt8 soulBound = data->soulBound;
	UInt8 identify = !data->Identify;
	UInt16 base_STA = 0;
	UInt16 base_SPR = 0;
	UInt16 base_STR = 0;
	UInt16 base_CON = 0;
	UInt16 base_AGI = 0;
	UInt16 base_ATK = 0;
	UInt16 base_DEF = 0;
	UInt16 base_HIT = 0;
	UInt16 base_EVA = 0;
	UInt16 base_POW = 0;
	UInt16 base_SPD = 0;
	UInt16 base_maxHP = 0;
	UInt16 base_maxSP = 0;
	UInt16 base_maxVP = 0;
	UInt16 eqColor = 0;
	UInt16 skill_id = 0;

	pItemData = NULL;
	if (data->soulBound == SOULBOUND_GET_BOUND)
		soulBound = SOULBOUND_ALREADY_BOUND;
	if (data->item_type == ITEMTYPE_EQUIP)
	{
		int extraValue[5] = {0, 0, 0, 0, 0}; 
		int hasValueCount = 0;
		if (data->STA_d != 0 ||  data->STA_u != 0)
		{
			extraValue[0] = GetRand(data->STA_d, data->STA_u);
			if (extraValue[0] > 0)
				++hasValueCount;
		}
		if (data->SPR_d != 0 ||  data->SPR_u != 0)
		{
			extraValue[1] = GetRand(data->SPR_d, data->SPR_u);
			if (extraValue[1] > 0)
				++hasValueCount;
		}
		if (data->STR_d != 0 ||  data->STR_u != 0)
		{
			extraValue[2] = GetRand(data->STR_d, data->STR_u);
			if (extraValue[2] > 0)
				++hasValueCount;
		}
		if (data->CON_d != 0 ||  data->CON_u != 0)
		{
			extraValue[3] = GetRand(data->CON_d, data->CON_u);
			if (extraValue[3] > 0)
				++hasValueCount;
		}
		if (data->AGI_d != 0 ||  data->AGI_u != 0)
		{
			extraValue[4] = GetRand(data->AGI_d, data->AGI_u);
			if (extraValue[4] > 0)
				++hasValueCount;
		}

		if(data->eqType != EQTYPE_CLOTH)
		{
			int extra = 0;
			switch (data->color)
			{
			case ItemColorIndex_Grey:		 extra = 0;				 break;
			case ItemColorIndex_LightGreen:	 extra = GetRand(0, 2);	 break;
			case ItemColorIndex_LightBlue:	 extra = GetRand(0, 3);	 break;
			case ItemColorIndex_LightPurple: extra = GetRand(0, 4);	 break;
			case ItemColorIndex_LightGolden: extra = GetRand(0, 5);	 break;
			case ItemColorIndex_GodGolden:	 extra = 5;				 break;
			}			
		
			if (extra < hasValueCount)
			{
				int setZero = hasValueCount - extra;
				while (setZero > 0)
				{
					UInt randIndex = GetRand(0, 4);
					if (extraValue[randIndex] > 0)
					{
						extraValue[randIndex] = 0;
						--setZero;
					}
					bool allClear = true;
					for (Index j = C_INDEX(0); j < 5; ++j)
					{
						if (extraValue[j] > 0)
						{
							allClear = false;
							break;
						}
					}
					if (allClear)
						break;
				}
			}
		}

		if (data->skill_ids.size() > 0 && data->skill_ids[0] > 0 && data->overlap == 1 &&
			data->probability >= (UInt)GetRand(1, 100))
		{
			skill_id = data->skill_ids[GetRand(0,(UInt)data->skill_ids.size()-1)];
		}

		endure = data->eq_maxEndure;
		base_STA = extraValue[0];
		base_SPR = extraValue[1];
		base_STR = extraValue[2];
		base_CON = extraValue[3];
		base_AGI = extraValue[4];
		base_ATK = GetRand(data->ATK_d, data->ATK_u);
		base_DEF = GetRand(data->DEF_d, data->DEF_u);
		base_HIT = GetRand(data->HIT_d, data->HIT_u);
		base_EVA = GetRand(data->EVA_d, data->EVA_u);
		base_POW = GetRand(data->POW_d, data->POW_u);
		base_SPD = GetRand(data->SPD_d, data->SPD_u);
		base_maxHP = GetRand(data->maxHP_d, data->maxHP_u);
		base_maxSP = GetRand(data->maxSP_d, data->maxSP_u);
		base_maxVP = GetRand(data->maxVP_d, data->maxVP_u);
		eqColor = data->eq_Color;
	}
	else
	{
		useCount = data->useCount;
	}

	if (data->item_TimeLimitType == ITEM_TIME_LIMIT_TYPE_CREATE)
	{
		DBTIMESTAMP dbtime;
		UInt32 now = (UInt32) time(NULL);
		UInt32 expire;
		expire = now + 3600 * 24 * data->item_TimeLimit;
		TimeConvert((time_t) expire, dbtime);
		hr = pdbItem->Insert(item_id, owner_id, bagIndex, useCount, stack, 
			element, endure, base_STA, base_SPR, base_STR, base_CON, base_AGI,
			base_ATK, base_DEF, base_HIT, base_EVA, base_POW, base_SPD, base_maxHP,
			base_maxSP, base_maxVP, soulBound, identify, maker, inWareHouse, 
			eqColor, quality, skill_id, lock, &dbtime);
	}
	else 
	{
		hr = pdbItem->Insert(item_id, owner_id, bagIndex, useCount, stack, 
			element, endure, base_STA, base_SPR, base_STR, base_CON, base_AGI,
			base_ATK, base_DEF, base_HIT, base_EVA, base_POW, base_SPD, base_maxHP,
			base_maxSP, base_maxVP, soulBound, identify, maker, inWareHouse, 
			eqColor, quality, skill_id, lock, NULL);
	}

	// insert new item into DB
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbItem->MoveFirst()))
	{
		// create Item object in object service
		pItemData = NewItem();
		if (pItemData->GetMaker().length() > 0)
			pItemData->SetMaker(pItemData->GetMaker());
	}
	pdbItem->Close();

	return hr;
}
*/

HRESULT DoCreateItem(UInt32 item_id, UInt32 owner_id, UInt8 bagIndex, UInt8 stack, 
					 CItemData* &pItemData, String maker, UInt8 inWareHouse, 
					 UInt16 quality, Boolean lock)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CreateItem: item_id %d, owner_id %d, bagIndex %d"),
		item_id, owner_id, bagIndex);

	HRESULT hr = S_OK;
	// get item data from resource
	const ItemData* data = gGlobal.m_resource->GetItemData(item_id);
	if (data == NULL)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CreateItem DB_FAILED: item_id %d not found in resource"), item_id);
		return S_FALSE;
	}

	// random element if necessary
	UInt8 element = (data->eq_baseElement == ELEMENT_RANDOM) ? GetRand(ELEMENT_TOTAL) : data->eq_baseElement;
	
	pItemData = NULL;
	UInt8 soulBound = data->soulBound;
	UInt8 identify = !data->Identify;

	if (data->soulBound == SOULBOUND_GET_BOUND)
		soulBound = SOULBOUND_ALREADY_BOUND;
	if (data->item_type == ITEMTYPE_EQUIP || data->item_type == ITEMTYPE_GEM)
	{
		int extraValue[5] = {0, 0, 0, 0, 0}; 
		int hasValueCount = 0;
		if (data->STA_d != 0 ||  data->STA_u != 0)
		{
			extraValue[0] = GetRand(data->STA_d, data->STA_u);
			if (extraValue[0] > 0)
				++hasValueCount;
		}
		if (data->SPR_d != 0 ||  data->SPR_u != 0)
		{
			extraValue[1] = GetRand(data->SPR_d, data->SPR_u);
			if (extraValue[1] > 0)
				++hasValueCount;
		}
		if (data->STR_d != 0 ||  data->STR_u != 0)
		{
			extraValue[2] = GetRand(data->STR_d, data->STR_u);
			if (extraValue[2] > 0)
				++hasValueCount;
		}
		if (data->CON_d != 0 ||  data->CON_u != 0)
		{
			extraValue[3] = GetRand(data->CON_d, data->CON_u);
			if (extraValue[3] > 0)
				++hasValueCount;
		}
		if (data->AGI_d != 0 ||  data->AGI_u != 0)
		{
			extraValue[4] = GetRand(data->AGI_d, data->AGI_u);
			if (extraValue[4] > 0)
				++hasValueCount;
		}

		if(data->eqType != EQTYPE_CLOTH && data->eqType != EQTYPE_HAT_WITH_HAIR && data->eqType != EQTYPE_HAT_WITHOUT_HAIR)
		{
			int extra = 0;
			switch (data->color)
			{
			case ItemColorIndex_Grey:		 extra = 0;				 break;
			case ItemColorIndex_LightGreen:	 extra = GetRand(0, 2);	 break;
			case ItemColorIndex_LightBlue:	 extra = GetRand(0, 3);	 break;
			case ItemColorIndex_LightPurple: extra = GetRand(0, 4);	 break;
			case ItemColorIndex_LightGolden: extra = GetRand(0, 5);	 break;
			case ItemColorIndex_GodGolden:	 extra = 5;				 break;
			}			
		
			if (extra < hasValueCount)
			{
				int setZero = hasValueCount - extra;
				while (setZero > 0)
				{
					UInt randIndex = GetRand(0, 4);
					if (extraValue[randIndex] > 0)
					{
						extraValue[randIndex] = 0;
						--setZero;
					}
					bool allClear = true;
					for (Index j = C_INDEX(0); j < 5; ++j)
					{
						if (extraValue[j] > 0)
						{
							allClear = false;
							break;
						}
					}
					if (allClear)
						break;
				}
			}
		}

		int skill_id(0);
		if (data->skill_ids.size() > 0 && data->skill_ids[0] > 0 && data->overlap == 1 && data->probability >= (UInt)GetRand(1, 1000))
		{
			skill_id = data->skill_ids[GetRand(0,(UInt)data->skill_ids.size()-1)];
		}

		if (data->item_TimeLimitType == ITEM_TIME_LIMIT_TYPE_CREATE)
		{

			DBTIMESTAMP dbtime;
			UInt32 now = (UInt32) time(NULL);
			UInt32 expire;
			expire = now + 3600 * 24 * data->item_TimeLimit;
			TimeConvert((time_t) expire, dbtime);
			hr = pdbItem->InsertEqWithTimeLimit(item_id, owner_id, bagIndex, element, data->eq_maxEndure,
								extraValue[0], //GetRand(data->STA_d, data->STA_u),
								extraValue[1], //GetRand(data->SPR_d, data->SPR_u),
								extraValue[2], //GetRand(data->STR_d, data->STR_u),
								extraValue[3], //GetRand(data->CON_d, data->CON_u),
								extraValue[4], //GetRand(data->AGI_d, data->AGI_u),
								GetRand(data->ATK_d, data->ATK_u),
								GetRand(data->DEF_d, data->DEF_u),
								GetRand(data->HIT_d, data->HIT_u),
								GetRand(data->EVA_d, data->EVA_u),
								GetRand(data->POW_d, data->POW_u),
								GetRand(data->SPD_d, data->SPD_u),
								GetRand(data->maxHP_d, data->maxHP_u),
								GetRand(data->maxSP_d, data->maxSP_u),
								GetRand(data->maxVP_d, data->maxVP_u),
								soulBound, identify,
								maker, dbtime, inWareHouse, data->eq_Color, quality,skill_id, lock);
		}
		else
		{
			hr = pdbItem->InsertEq(item_id, owner_id, bagIndex, element, data->eq_maxEndure,
								extraValue[0], //GetRand(data->STA_d, data->STA_u),
								extraValue[1], //GetRand(data->SPR_d, data->SPR_u),
								extraValue[2], //GetRand(data->STR_d, data->STR_u),
								extraValue[3], //GetRand(data->CON_d, data->CON_u),
								extraValue[4], //GetRand(data->AGI_d, data->AGI_u),
								GetRand(data->ATK_d, data->ATK_u),
								GetRand(data->DEF_d, data->DEF_u),
								GetRand(data->HIT_d, data->HIT_u),
								GetRand(data->EVA_d, data->EVA_u),
								GetRand(data->POW_d, data->POW_u),
								GetRand(data->SPD_d, data->SPD_u),
								GetRand(data->maxHP_d, data->maxHP_u),
								GetRand(data->maxSP_d, data->maxSP_u),
								GetRand(data->maxVP_d, data->maxVP_u),
								soulBound, identify, maker, inWareHouse, data->eq_Color, quality, skill_id, lock);
		}
	}
	else
	{
		if (data->item_TimeLimitType == ITEM_TIME_LIMIT_TYPE_CREATE)
		{

			DBTIMESTAMP dbtime;
			UInt32 now = (UInt32) time(NULL);
			UInt32 expire;
			expire = now + 3600 * 24 * data->item_TimeLimit;
			TimeConvert((time_t) expire, dbtime);
			hr = pdbItem->InsertWithTimeLimit(item_id, owner_id, bagIndex, element, data->useCount, stack, soulBound, identify, maker, dbtime, inWareHouse, quality, lock);
		}
		else
		{
			hr = pdbItem->Insert(item_id, owner_id, bagIndex, element, data->useCount, stack, soulBound, identify, maker, inWareHouse, quality, lock);
		}
	}

	// insert new item into DB
	if (DB_SUCCEEDED(hr) && DB_SUCCEEDED(hr = pdbItem->MoveFirst()))
	{
		// create Item object in object service
		pItemData = NewItem();
		if (pItemData->GetMaker().length() > 0)
			pItemData->SetMaker(pItemData->GetMaker());
	}
	pdbItem->Close();

	return hr;
}

RPCResult ObjectService_Stub::CreateItemList(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32Vector &item_ids, 
	const UInt32 &owner_id, 
	const UInt8Vector &bagIndices, 
	const UInt8 &detail)
{
	TRACE_ENSURE(pdbItem != NULL);

	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_F("owner_id %d, item_ids %s, bagIndices %s"), owner_id,
		GetVectorString(item_ids).c_str(), GetVectorString(bagIndices).c_str());

	if (item_ids.size() != bagIndices.size())
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("unmatch size of id list and bagIndex list"));
		return CB_Error(pPeer, context, 1, _FE("unmatch size of id list and bagIndex list\n"));
	}

	HRESULT hr = S_OK;
	NetGroup grp;

	for (int i = 0; i < (int) item_ids.size(); ++i)
	{
		const UInt32 &item_id = item_ids[i];
		const UInt8 &bagIndex = bagIndices[i];

		CItemData *pItemData = NULL;
		hr = DoCreateItem(item_id, owner_id, bagIndex, 1, pItemData, _T(""), 0, 0, BOOLEAN_FALSE);
		if (DB_FAILED(hr)) break;
		if (pItemData)
		{
			pItemData->SetDetail(detail);
			grp.Add(pItemData);
		}
	}

	if (DB_FAILED(hr))
	{
		pdbItem->ShowError(hr);
		CB_Error(pPeer, context, hr, _FE("DB_FAILED"));
		return RPC_RESULT_FAIL;
	}

	if (detail == CItemData::DETAIL_NONE)
		CB_CreateItemDone(pPeer, context);
	else
		CB_GetItemGroup(pPeer, context, &grp, detail);

	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::UpdateItemList(
	LPCPEER pPeer, RPCContext &context,
	const UInt32Vector &itemList, const UInt32 &owner_id,
	NetGroup* grp, const Boolean &separated)
{
	TRACE_ENSURE(pdbItem && grp);

	HRESULT hr = S_OK;
	UInt32Vector new_uids;
	NetGroup newItems;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("item_ids %s"), GetVectorString(itemList).c_str());

	for (Index idx = C_INDEX(0); idx < itemList.size(); ++idx)
	{
		const UInt32 &item_uid = itemList[idx];

		DataIdMap::iterator iter = gGlobal.itemIdMap.end();
		CItemData* pItemData = NULL;
		CItemData* pNewData = NULL;
		NetGroup* itemGroup = NULL;

		if (item_uid != 0)
		{
			iter = gGlobal.itemIdMap.find(item_uid);
			if (iter != gGlobal.itemIdMap.end())
			{
				// found existing Item in cache
				pItemData = CastNetObject<CItemData>(iter->second.pobj);
				TRACE_ENSURE(pItemData);
				if (itemGroup == NULL)
				{
					if (pItemData->GetInWareHouse() == 0)
						itemGroup = FindItemGroup(pItemData->GetOwner_id());
					else
						itemGroup = FindWareHouseItemGroup(pItemData->GetOwner_id());
				}
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("item_uid not found, item_uid %d"), item_uid);
				continue;
			}
		}

		if (pNewData == NULL || separated)
		{
			// separated data
			if (idx < grp->GetCount())
			{
				pNewData = grp->GetChild<CItemData>(idx);
			}
			else
			{
				TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("no more data in NetGroup, count %d, item_uid %d"), grp->GetCount(), item_uid);
				continue;
			}
			if (pNewData == NULL)
			{
				// skip if null data
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("invalid item data, item_uid %d"), item_uid);
				continue;
			}
		}

		if (item_uid != pNewData->GetItem_uid())
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("item_uid not match new data item_uid, item_uid %d, new data item_uid %d"),
				item_uid, pNewData->GetItem_uid());
			continue;
		}
 	
		// remove item if stack = 0
		if (pNewData->IsUpdated(CItemData::stackMask) && pNewData->GetStack() == 0)
		{
			RemoveItem(item_uid);
		}
		else if (item_uid == 0)
		{
			// create item if item_uid == 0
			pItemData = pNewData;
			String maker = _T("");
			if (pNewData->GetMaker().length() > 0)
				maker = pNewData->GetMaker();
			hr = DoCreateItem(pItemData->GetItem_id(), owner_id, pItemData->GetBagIndex(), 
				pItemData->GetStack(), pItemData, maker, pItemData->GetInWareHouse(), pItemData->GetItem_quality(), pItemData->GetLock());
			if (DB_FAILED(hr))
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("item create failed, hr 0x%08X"), hr);
				pdbItem->ShowError(hr);
				continue;
			}
			pItemData->SetDetail(CItemData::DETAIL_MAP);
			newItems.Add(pItemData);
		}
		else // else update item
		{
			//change the owner of the item
			if (pNewData->IsUpdated(CItemData::owner_idMask) && 
				pItemData->GetOwner_id() != pNewData->GetOwner_id())
			{
				NetGroup* tempItemGroup = FindItemGroup(pItemData->GetOwner_id());
				if (tempItemGroup)
					tempItemGroup->Remove(pItemData);

				tempItemGroup = FindItemGroup(pNewData->GetOwner_id());
				if (tempItemGroup == NULL)
				{
					DataObject& dobj3 = gGlobal.itemGroupMap[pNewData->GetOwner_id()];
					if (dobj3.pobj)
					{
						tempItemGroup = (NetGroup*) dobj3.pobj;
					}
					else
					{
						tempItemGroup = SafeCreateObject(NetGroup);
						dobj3.pobj = tempItemGroup;
					}
				}

				pItemData->Clone(pNewData, pNewData->GetUpdateMask());
				tempItemGroup->Add(pItemData);
			}
			else if (pNewData->IsUpdated(CItemData::inWareHouseMask) &&
				pItemData->GetInWareHouse() != pNewData->GetInWareHouse())
			{
				NetGroup* srcGrp = NULL;
				NetGroup* desGrp = NULL;
				
				if (pItemData->GetInWareHouse() == 0)
				{
					srcGrp = FindItemGroup(pItemData->GetOwner_id());
					desGrp = FindWareHouseItemGroup(pItemData->GetOwner_id());
				}
				else
				{
					srcGrp = FindWareHouseItemGroup(pItemData->GetOwner_id());
					desGrp = FindItemGroup(pItemData->GetOwner_id());					
				}

				if (srcGrp && desGrp)
				{
					srcGrp->Remove(pItemData);
					desGrp->Add(pItemData);
				}				
				pItemData->Clone(pNewData, pNewData->GetUpdateMask());
			}
			else
				pItemData->Clone(pNewData, pNewData->GetUpdateMask());
			gGlobal.EnqueueUpdate(&iter->second);
		}
	}

	DeleteNetGroup(grp, BOOLEAN_TRUE);
	pdbItem->Close();

	if (DB_FAILED(hr))
	{
		pdbItem->ShowError(hr);
		DeleteNetChilds(&newItems);
	}

	CB_UpdateItemList(pPeer, context, &newItems);
	return RPC_RESULT_OK;
}

RPCResult ObjectService_Stub::GetItemsByOwner(
	LPCPEER pPeer, RPCContext &context, 
	const UInt32 &owner_id, const UInt8 &inWareHouse,
	const UInt8 &detail)
{
	PROCEDURE_SET_NONBLOCK
	PROCEDURE_START_RPC3(ObjectService, GetItemsByOwner, NULL, UInt32, owner_id, UInt8, inWareHouse, UInt8, detail)

		TRACE_ENSURE(pdbItem != NULL);
		CItemData* pItemData = NULL;
		NetGroup* itemGroup = NULL;
		if (inWareHouse == 0)
			itemGroup = FindItemGroup(owner_id);
		else
			itemGroup = FindWareHouseItemGroup(owner_id);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("owner_id %d"), owner_id);

		if (itemGroup)
		{
			// directly send back object group
			CItemData::SetGroupDetail(itemGroup, detail);
			CB_GetItemGroup(pPeer, context, itemGroup, detail);
			PROCEDURE_RETURN
		}
		else
		{
			// wait for thread pool to process
			gGlobal.WorkQueueBegin();
			ItemRequest *pRequest = SafeCreateObject3(ItemRequest, owner_id, pPeer, context);
			if (pRequest)
				pRequest->inWareHouse = inWareHouse;
#ifdef DB_THREAD_POOL
			gGlobal.QueueUserWorkItem(QueueGetItems, pRequest, DB_WORKER_FLAG);
#else // DB_THREAD_POOL
			QueueGetItems(pRequest);
#endif // DB_THREAD_POOL
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_QueueNetGroup, NetGroup*, itemGroup)

		TRACE_ENSURE(itemGroup);

		// store new item group
		CItemData* pItemData;
		NetGroup* newGroup = NULL;

		if (inWareHouse == 0)
		{
			DataObject& dobj3 = gGlobal.itemGroupMap[owner_id];
			if (dobj3.pobj)
				newGroup = (NetGroup*) dobj3.pobj;
			else
			{
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
		}
		else
		{
			DataObject& dobj3 = gGlobal.wareHouseItemGroupMap[owner_id];
			if (dobj3.pobj)
				newGroup = (NetGroup*) dobj3.pobj;
			else
			{
				newGroup = SafeCreateObject(NetGroup);
				dobj3.pobj = newGroup;
			}
		}
		for (UInt16 i = 0; i < itemGroup->GetCount(); i++)
		{
			pItemData = itemGroup->GetChild<CItemData>(i);
			if (pItemData && pItemData->GetOwner_id() == owner_id)
			{
				DataObject &dobj = gGlobal.itemIdMap[pItemData->GetItem_uid()];
				if (dobj.pobj)
				{
					// use old item data
					DeleteNetObject(pItemData);
					pItemData = CastNetObject<CItemData>(dobj.pobj);
					if (pItemData->GetOwner_id() != owner_id)
					{
						TRACE_WARNDTL_4(GLOBAL_LOGGER, 
							_FW("not owned, item_uid %d, item_id %d, owner_id %d, new data owner_id %d"),
							pItemData->GetItem_uid(), pItemData->GetItem_id(), 
							owner_id, pItemData->GetOwner_id());
						pItemData = NULL;
					}
				}
				else
				{
					// need to register for new item data
					LocalRPCManager::GetInstance().Register(pItemData);
					pItemData->ClearUpdateMask();
					dobj.pobj = pItemData;
				}
				if (pItemData)
					newGroup->Add(pItemData);
			}
		}

		// send item data to peer
		CItemData::SetGroupDetail(newGroup, detail);
		CB_GetItemGroup(pPeer, context, newGroup, detail);
		DeleteNetGroup(itemGroup);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_3(GLOBAL_LOGGER, 
				_FE("session_id %s, errcode %d, %s"),
				context.sessionId.GetString().c_str(), err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}


RPCResult ObjectService_Stub::SetItemLock(LPCPEER pPeer, RPCContext &context, const UInt32 &itemUID, const Boolean &lock)
{
	HRESULT hr = S_OK;
	TRACE_ENSURE(pdbItem);	
	DataIdMap::iterator itfind = gGlobal.itemIdMap.find(itemUID);
	if (itfind != gGlobal.itemIdMap.end())
	{
		CItemData* pData = CastNetObject<CItemData>(itfind->second.pobj);
		if (pData)
			pData->SetLock(lock);
	}
	Boolean success = DB_SUCCEEDED(hr = pdbItem->SetLock(itemUID, lock));
	pdbItem->Close();
    
	CB_SetAssetLock(pPeer, context, success);
	
	return RPC_RESULT_OK;
}
