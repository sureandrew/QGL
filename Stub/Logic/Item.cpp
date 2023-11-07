//-- Common
#include "Common.h"
#include "Common/DataCode.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/Item.h"
//-- Library
#include "Proxy/Data/ItemData.h"
#include "Proxy/Service/ObjectService.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/BatMob.h"
#include "Stub/Logic/Battle.h"
#include "MapService/Global.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResItem.h"
#include "Stub/Logic/Partner.h"
//#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Pet.h"
//#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/ScriptSystem.h"
//#include "Stub/Logic/QuestSystem.h"
#include "Resource/ResBuff.h"
#include "MapService/Global.h"
#include "Stub/Logic/ItemBag.h"
#include "MapService/ViewManager.h"

//#define BAG_MAX_ITEM	120

REGISTER_CLASSTYPE(CItem, Item_CLASSID);

PROTO_LuaPush(CCharacter);
PROTO_LuaPush(CBatActor);
PROTO_LuaPush(CItem);

PCSTRINGA strMapUse = "MapUse";
PCSTRINGA strBatCan = "CanUse";
PCSTRINGA strBatUse = "BattleUse";

CItem::CItem()
{
	m_data = NULL;
	m_tryRemain = 0;
	m_tryStack = 0;
	m_buffData = NULL;
	m_BuffPreUpdateTime = NULL;	
	m_iCreateType = CItemBag::CREATE_BY_SYSTEM;
}

UInt8 CItem::GetEquipIndex()
{
	if (m_data == NULL || m_data->item_type != ITEMTYPE_EQUIP)
		return 255;

	UInt8 ei = 255;

	if (m_data->eqType < EQTYPE_GEAR)
		ei = EQPLACE_WEAPON + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_GEAR)
		ei = EQPLACE_GEAR + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_ARMOR)
		ei = EQPLACE_ARMOR + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_SHOE)
		ei = EQPLACE_SHOE + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_BELT)
		ei = EQPLACE_BELT + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_RING)
		ei = EQPLACE_RING + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_AMULET)
		ei = EQPLACE_AMULET + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_CLOTH)
		ei = EQPLACE_CLOTH + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_HAT_WITH_HAIR)
		ei = EQPLACE_HEAD_ACCESSORY + BAG_MAX_ITEM;
	else if (m_data->eqType == EQTYPE_HAT_WITHOUT_HAIR)
		ei = EQPLACE_HEAD_ACCESSORY + BAG_MAX_ITEM;
	

	return ei;
}

Boolean CItem::AllowSexClass(UInt8 sex, UInt8 cclass)
{
	if (!m_data) return BOOLEAN_FALSE;

	UInt8 sexClass = (cclass > 0 ? ((cclass - 1) * 2 + sex + 1) : 100 + sex);
	return (m_data->reqSexClass1 == SEXCLASS_NONE || 
			sex + 100 == m_data->reqSexClass1 || sexClass == m_data->reqSexClass1 ||
			sex + 100 == m_data->reqSexClass2 || sexClass == m_data->reqSexClass2);
}

Boolean CItem::CanUse(CCharacter* pUser, NetObject* pTarget, String &errMsg)
{
	if (pUser == NULL || m_data == NULL) {
		errMsg.Format(_T("item_uid %d null user or data"), GetItem_uid());
		return BOOLEAN_FALSE;
	}

	// is block?
	if (pUser->CheckBlockItem(m_data->item_type))
	{
		pUser->CB_Error(pUser->GetPeer(), pUser->GetContext(), BATTLE_ERROR_ITEMBLOCK, 
			String());
		return BOOLEAN_FALSE;
	}
	// can use in battle or map?
	if (m_data->usePlace != 0 && m_data->usePlace != FIELD_MAP) {
		errMsg.Format(_T("item_uid %d, item_id %d cannot use in battle"), GetItem_uid(),
			GetItem_id());
		return BOOLEAN_FALSE;
	}

	// use count > 0
	if (m_data->useCount == 0 || GetStack() == 0 || GetRemain() == 0) {
		errMsg.Format(_T("item_uid %d no more stack %d or remain %d"), GetItem_uid(),
			GetStack(), GetRemain());
		return BOOLEAN_FALSE;
	}

	// fulfill all requirement?
	if (m_data->reqFaction != 0 && m_data->reqFaction != pUser->GetFaction() ||	// req faction
		m_data->reqLv > pUser->GetLevel() ||						// req level
		!AllowSexClass(pUser->GetSex(), pUser->GetCclass()))		// req sex and class
	{
		errMsg.Format(_T("item_uid %d, item_id %d requirement not fulfill"), GetItem_uid(), 
			GetItem_id());
		return BOOLEAN_FALSE;
	}

	CPartner *partner = CastNetObject<CPartner>(pTarget);

	if ((m_data->targetType == TARGET_TYPE_SELF && pUser != pTarget) ||
		(m_data->targetType == TARGET_TYPE_ANY_PARNTER && (partner == NULL || partner->GetOwner_id() != pUser->GetChar_id())) ||
		(m_data->targetType == TARGET_TYPE_ANY_SAME_TEAM && !(pUser == pTarget || (partner && partner->GetOwner_id() == pUser->GetChar_id()) )))
	{
		errMsg.Format(_T("item_uid %d, item_id %d invalid target type %d on target classId %d"),
			GetItem_uid(), GetItem_id(), m_data->targetType, pTarget->GetClassID());
		return BOOLEAN_FALSE;
	}

	CPet *pet = CastNetObject<CPet>(pTarget);

	if ((m_data->targetType == TARGET_TYPE_SELF && pUser != pTarget) ||
		(m_data->targetType == TARGET_TYPE_ANY_PET && 
			(pet == NULL || pet->GetOwner_id() != pUser->GetChar_id())))
	{
		errMsg.Format(_T("item_uid %d, item_id %d invalid target type %d on target classId %d"),
			GetItem_uid(), GetItem_id(), m_data->targetType, pTarget->GetClassID());
		return BOOLEAN_FALSE;
	}	

	return BOOLEAN_TRUE;
}

Boolean CItem::UseOn(CCharacter* pUser, NetObject* pTarget, String &errMsg)
{
	if (m_data == NULL || pUser == NULL || pTarget == NULL)
		return BOOLEAN_FALSE;

	if (!CanUse(pUser, pTarget, errMsg))
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("UseOn Error: char_id %d, %s"), 
			pUser->GetChar_id(), errMsg.c_str());
		return BOOLEAN_FALSE;
	}

	// apply item effect
	if (pTarget->GetClassID() == Character_CLASSID)
	{
		CCharacter *pchar = CastNetObject<CCharacter>(pTarget);
		if (pchar == NULL) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UseOn Error: invalid target classId %d"),
				pTarget->GetClassID());
			return BOOLEAN_FALSE;
		}

		if (!ApplyCharacter(pchar))
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("UseOn Error: char_id %d fail to apply item_id %d, type %d"),
				pUser->GetChar_id(), GetItem_id(), m_data->item_type);
			return BOOLEAN_FALSE;
		}

		if (m_data->effect_Atk != _T("0") || m_data->effect_Hurt != _T("0") || m_data->effect_Spark != _T("0"))
		{
			if (pUser->m_viewManager)
			{
				CharacterPtrList chars;
				pUser->m_viewManager->GetCharacters(chars);
				
				for (CharacterPtrList::iterator it = chars.begin(); it != chars.end(); ++it)
				{
					if (*it && !(*it)->IsInBattle() )
					{
						(*it)->CB_UseItemCharacter((*it)->GetPeer(), (*it)->GetContext(), m_data->item_id, pUser->GetChar_id(),pchar->GetChar_id());
					}
				}
			}

			if (pUser->GetPeer() && !pUser->IsInBattle())
				pUser->CB_UseItemCharacter(pUser->GetPeer(), pUser->GetContext(), m_data->item_id, pUser->GetChar_id(),pchar->GetChar_id());
		}
	}
	else if (pTarget->GetClassID() == Partner_CLASSID)
	{
		CPartner *partner = CastNetObject<CPartner>(pTarget);
		if (partner == NULL) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UseOn Error: invalid target partner classId %d"),
				pTarget->GetClassID());
			return BOOLEAN_FALSE;
		}

		if (!ApplyPartner(partner))
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("UseOn Error: partner_id %d fail to apply item_id %d, type %d"),
				pUser->GetChar_id(), GetItem_id(), m_data->item_type);
			return BOOLEAN_FALSE;
		}
	}
	else if (pTarget->GetClassID() == Pet_CLASSID)
	{
		CPet *pet = CastNetObject<CPet>(pTarget);
		if (pet == NULL) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UseOn Error: invalid target partner classId %d"),
				pTarget->GetClassID());
			return BOOLEAN_FALSE;
		}
		if (!ApplyPet(pet))
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _F("UseOn Error: item_type %d fail to apply item_id %d, type %d"),
				pUser->GetChar_id(), GetItem_id(), m_data->item_type);
			return BOOLEAN_FALSE;
		}
	}

	UInt8 remain = GetRemain();
	// determine to run script 
	if (m_data->script && m_data->script[0] != _T('\0') && pUser->GetScriptSystem())
	{
		CScriptSystem* pScript = pUser->GetScriptSystem();
		Boolean r = pScript->ExecuteItemScript(this, NULL, pTarget, strMapUse);
		if (!r)
			return r;
	}
	else if (m_data->consume == 1)
	{
		// reduce use count or stack count
		ReduceCount();
	}

	return BOOLEAN_TRUE;
}

Boolean CItem::ApplyCharacter(CCharacter* pTarget)
{
	if (pTarget == NULL)
		return BOOLEAN_FALSE;

	Boolean used = BOOLEAN_FALSE;
	RPCMask mask;

	switch (m_data->item_type)
	{
	case ITEMTYPE_DRUG:
	case ITEMTYPE_FOOD:
	case ITEMTYPE_FAST_FOOD:
		if (m_data->restore_HP > 0) {
			UInt16 hp = pTarget->GetHP() + m_data->restore_HP;
			pTarget->SetHP(MIN(hp, pTarget->GetMHP()), pTarget->GetMHP(), pTarget->GetMaxHP());
			mask.MaskBit(CCharacter::HPMask);
		}
		if (m_data->restore_mHP > 0) {
			UInt16 mhp = pTarget->GetMHP() + m_data->restore_mHP;
			pTarget->SetHP(pTarget->GetHP(), MIN(mhp, pTarget->GetMaxHP()), pTarget->GetMaxHP());
			mask.MaskBit(CCharacter::HPMask);
		}
		if (m_data->restore_SP > 0) {
			UInt16 SP = pTarget->GetSP() + m_data->restore_SP;
			pTarget->SetSP(MIN(SP, pTarget->GetMSP()), pTarget->GetMSP(), pTarget->GetMaxSP());
			mask.MaskBit(CCharacter::SPMask);
		}
		if (m_data->restore_mSP > 0) {
			UInt16 mSP = pTarget->GetMSP() + m_data->restore_mSP;
			pTarget->SetSP(pTarget->GetSP(), MIN(mSP, pTarget->GetMaxSP()), pTarget->GetMaxSP());
			mask.MaskBit(CCharacter::SPMask);
		}

		if (m_data->restore_VP > 0) {
			UInt16 VP = pTarget->GetVP() + m_data->restore_VP;
			pTarget->SetVP(MIN(VP, pTarget->GetMaxVP()), pTarget->GetMaxVP());
			mask.MaskBit(CCharacter::VPMask);
		}
		used = BOOLEAN_TRUE;
		break;
	}

	if (used)
		pTarget->RaiseUpdate();

	if ( m_data && m_data->script && m_data->consume != 0 )
		return BOOLEAN_TRUE;

	return used;
}

Boolean CItem::ApplyPartner(CPartner* pTarget)
{
	if (pTarget == NULL)
		return BOOLEAN_FALSE;

	Boolean used = BOOLEAN_FALSE;
	RPCMask mask;

	switch (m_data->item_type)
	{
	case ITEMTYPE_DRUG:
	case ITEMTYPE_FOOD:
	case ITEMTYPE_FAST_FOOD:
		if (m_data->restore_HP > 0) {
			UInt16 hp = pTarget->GetHP() + m_data->restore_HP;
			pTarget->SetHP(MIN(hp, pTarget->GetMaxHP()), pTarget->GetMaxHP());
			mask.MaskBit(CPartner::HPMask);
		}
		
		if (m_data->restore_SP > 0) {
			UInt16 SP = pTarget->GetSP() + m_data->restore_SP;
			pTarget->SetSP(MIN(SP, pTarget->GetMaxSP()), pTarget->GetMaxSP());
			mask.MaskBit(CPartner::SPMask);
		}
		
		used = BOOLEAN_TRUE;
		break;
	case ITEMTYPE_WINE:
		if (m_data->restore_favor > 0)
		{
			float favor = pTarget->GetFavor() + m_data->restore_favor;
			if (favor > 100.0f)
				favor = 100.0f;
			pTarget->SetFavor(favor);
			mask.MaskBit(CPartner::favorMask);
		}
		used = BOOLEAN_TRUE;
		break;
	case ITEMTYPE_SKILLBOOK:
	case ITEMTYPE_SCRIPT:
	case ITEMTYPE_SPECIAL:
		{
			used = BOOLEAN_TRUE;
		}
		break;
	}

	if (used)
		pTarget->RaiseUpdate();

	return used;
}
Boolean CItem::ApplyPet(CPet* pTarget)
{
	if (pTarget == NULL)
		return BOOLEAN_FALSE;

	Boolean used = BOOLEAN_FALSE;
	RPCMask mask;

	if (m_data->restore_VP)
	{
		pTarget->ChangeVP(m_data->restore_VP);

		//random add favor
		UInt add = rand() % 10 + 1;

		pTarget->ChangeHappiness(add);
		used = TRUE;
	}
	else if (m_data->restore_SP)
	{
		pTarget->SetSpirit(pTarget->GetSpirit() + m_data->restore_SP);

		//random add favor
		UInt add = rand() % 10 + 1;

		if (pTarget->GetHappiness() + add > MAX_PET_HAPPINESS)
			pTarget->SetHappiness(MAX_PET_HAPPINESS);
		else
			pTarget->SetHappiness(pTarget->GetHappiness() + add);
		
		used = TRUE;
	}

	if (m_data->item_type == ITEMTYPE_SPECIAL)
	{
		used = BOOLEAN_TRUE;
	}

	if (used)
		pTarget->RaiseUpdate();

	return used;
}


Boolean CItem::CanUse(CBatActor* pUser, CBatActor* pTarget, String &errMsg)
{
	if (pUser == NULL || m_data == NULL) {
		errMsg.Format(_T("item_uid %d null user or data"), GetItem_uid());
		return BOOLEAN_FALSE;
	}

	if (pTarget == NULL || !pTarget->IsValidTarget(BOOLEAN_FALSE)) {
		errMsg.Format(_T("item_uid %d null or invalid target"), GetItem_uid());
		return BOOLEAN_FALSE;
	}

	// is block?
	if (pUser->CheckBlockItem(m_data->item_type))
	{
		pUser->SendBattleMsg(BATTLE_ERROR_ITEMBLOCK, _T(""), pUser->GetSubType());
		return BOOLEAN_FALSE;
	}
	// can use in battle?
	if (m_data->usePlace != 0 && m_data->usePlace != FIELD_BATTLE) {
		errMsg.Format(_T("item_uid %d, item_id %d cannot use in battle"), GetItem_uid(),
			GetItem_id());
		return BOOLEAN_FALSE;
	}

	// use count > 0
	if (m_data->useCount == 0 || GetStack() == 0 || GetRemain() == 0) {
		errMsg.Format(_T("item_uid %d no more stack %d or remain %d"), GetItem_uid(),
			GetStack(), GetRemain());
		return BOOLEAN_FALSE;
	}

	

	// valid target type
	if (!pUser->IsValidTargetType(m_data->targetType, pTarget))
	{
		errMsg.Format(_T("item_uid %d, item_id %d invalid target type %d, target %d"), GetItem_uid(), 
			GetItem_id(), m_data->targetType, pTarget->GetIndex());
		return BOOLEAN_FALSE;
	}
	
	// fulfill all requirement?
	if (m_data->reqFaction != 0 && m_data->reqFaction != pUser->GetFaction() ||	// req faction
		m_data->reqLv > pUser->GetLevel() ||						// req level
		!AllowSexClass(pUser->GetSex(), pUser->GetCclass()))		// req sex and class
	{
		errMsg.Format(_T("item_uid %d, item_id %d requirement not fulfill"), GetItem_uid(), 
			GetItem_id());
		return BOOLEAN_FALSE;
	}

	//check lua
	if (m_data && m_data->script && m_data->script[0] != _T('\0'))
	{
		CScriptSystem* pScript = pUser->GetBattle()->GetScript();		
		if (pScript && pScript->IsNewScript()) {
			Boolean r = pScript->ExecuteItemScript(this, pUser, pTarget, strBatCan);
			if (!r)
				return r;

			if (pUser->GetReturnType() > 0)
				return BOOLEAN_FALSE;
		} 
	}
	return BOOLEAN_TRUE;
}

Boolean CItem::UseOn(CBatActor* pUser, CBatActor* pTarget, String &errMsg,
						Boolean bTry)
{
	if (!CanUse(pUser, pTarget, errMsg))
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Item UseOn Warning: %s"), errMsg.c_str());
		return BOOLEAN_FALSE;
	}

	if (bTry && m_tryRemain != 255)
	{
		// mark try use and reduce use count or stack count
		m_tryRemain--;							// reduce use count
		if (m_tryRemain == 0)
		{
			m_tryStack--;						// reduce stack
			if (m_tryStack > 0)
			{
				// still here after use
				m_tryRemain = m_data->useCount;	// reset use count
			}
		}
		return BOOLEAN_TRUE;
	}

	if (m_data && m_data->script && m_data->script[0] != _T('\0'))
	{
		CScriptSystem* pScript = pUser->GetBattle()->GetScript();
		pUser->m_Msg.Empty();
		pUser->ClearReturnType();
		if (pScript && pScript->IsNewScript())
		{
			Boolean r = pScript->ExecuteItemScript(this, pUser, pTarget, strBatUse);
			if (!r)
				return BOOLEAN_FALSE;
		}
		else
		{
			StringA luaName = m_data->script;
			size_t pos = luaName.find(".");
			if (pos != std::string::npos)
			{
				luaName.Format("%s%s", luaName.substr(0, pos).c_str(), "Use");
				CallLuaFunc(gGlobal.g_pLuaState, luaName.c_str(), String(), pUser, pTarget, this);
			}
		}
		if (!pUser->m_Msg.IsEmpty())
		{
			if (pUser->m_ReturnType > 0)
				pUser->SendBattleMsg(BATTLE_ERROR_MSG_GIVEN, pUser->m_Msg, pUser->GetSubType());
			else
				pUser->SendBattleMsg(BATTLE_USE_MSG_GIVEN, pUser->m_Msg, pUser->GetSubType());
		}
	}
	else
	{
		// apply item effect
		switch (m_data->item_type)
		{
		case ITEMTYPE_DRUG:
			if (m_data->restore_HP > 0)
			{
				pTarget->ChangeHP(m_data->restore_HP);
			}
			if (m_data->restore_mHP > 0)
			{
				pTarget->ChangeMHP(m_data->restore_mHP);
			}
			if (m_data->restore_SP > 0)
			{
				pTarget->ChangeSP(m_data->restore_SP);
			}
			if (m_data->restore_mSP > 0)
			{
				pTarget->ChangeMSP(m_data->restore_mSP);
				
			}
			if (m_data->restore_VP > 0) 
			{
				pTarget->ChangeVP(m_data->restore_VP);
			}
			break;
		}
	}

	// reduce use count or stack count
	ReduceCount();

	return BOOLEAN_TRUE;
}

Boolean CItem::ReduceCount()
{
	// reduce use count or stack count
	if (GetRemain() != 255 && GetRemain() > 0 && GetStack() > 0)
	{
		SetRemain(GetRemain() - 1);				// reduce use count
		if (GetRemain() == 0)
		{
			SetStack(GetStack() - 1);			// reduce stack
			if (GetStack() > 0)
			{
				// still here after use
				SetRemain(m_data->useCount);	// reset use count
			}
		}
		return BOOLEAN_TRUE;
	}
	else
	{
		return BOOLEAN_FALSE;
	}
}

void CItem::SetData(CItemData *pData, bool bClear)
{
	TRACE_ENSURE(pData);

	if (pData->IsUpdated(CItemData::idMask))
	{
		SetId(pData->GetItem_uid(), pData->GetItem_id());
		m_data = gGlobal.m_resource->GetItemData(GetItem_id());
		if (m_data == NULL)
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CItem Warning: cannot find item_id %d in resource"), 
				GetItem_id());
		SetDetailLv(CItemData::DETAIL_INFO);			// set brief detail info level
	}
	if (pData->IsUpdated(CItemData::owner_idMask))
	{
		SetOwner_id(pData->GetOwner_id());
	}
	if (pData->IsUpdated(CItemData::bagIndexMask))
	{
		SetBagIndex(pData->GetBagIndex());
	}
	if (pData->IsUpdated(CItemData::makerMask))
	{
		SetMaker(pData->GetMaker());
	}
	if (pData->IsUpdated(CItemData::eq_elementMask))
	{
		SetEq_element(pData->GetEq_element());
	}
	if (pData->IsUpdated(CItemData::endureMask))
	{
		SetEndure(pData->GetEndure());
	}
	if (pData->IsUpdated(CItemData::mEndureMask))
	{
		SetMEndure(pData->GetMEndure());
	}
	if (pData->IsUpdated(CItemData::gradeMask))
	{
		SetGrade(pData->GetGrade());
	}
	if (pData->IsUpdated(CItemData::upgExpMask))
	{
		SetUpgExp(pData->GetUpg_exp());
	}
	if (pData->IsUpdated(CItemData::remainMask))
	{
		SetRemain(pData->GetRemain());
	}
	if (pData->IsUpdated(CItemData::stackMask))
	{
		SetStack(pData->GetStack());
	}
	if (pData->IsUpdated(CItemData::baseAttrMask))
	{
		SetBaseAttr(pData->GetBase_STA(), pData->GetBase_SPR(), pData->GetBase_STR(),
					pData->GetBase_CON(), pData->GetBase_AGI(), pData->GetBase_ATK(),
					pData->GetBase_DEF(), pData->GetBase_HIT(), pData->GetBase_EVA(),
					pData->GetBase_POW(), pData->GetBase_SPD(), pData->GetBase_maxHP(),
					pData->GetBase_maxSP(), pData->GetBase_maxVP());
		SetDetailLv(CItemData::DETAIL_MAP);			// set detail info level
	}
	if (pData->IsUpdated(CItemData::upgAttrMask))
	{
		SetUpgAttr(pData->GetUpg_STA(), pData->GetUpg_SPR(), pData->GetUpg_STR(),
					pData->GetUpg_CON(), pData->GetUpg_AGI(), pData->GetUpg_ATK(),
					pData->GetUpg_DEF(), pData->GetUpg_HIT(), pData->GetUpg_EVA(),
					pData->GetUpg_POW(), pData->GetUpg_SPD(), pData->GetUpg_maxHP(),
					pData->GetUpg_maxSP(), pData->GetUpg_maxVP());
	}
	if (pData->IsUpdated(CItemData::inlayGemMask))
	{
		SetInlayGem(pData->GetEq_gem(), pData->GetInlay_gem(0), pData->GetInlay_gem(1),
			pData->GetInlay_gem(2), pData->GetInlay_gem(3), pData->GetInlay_gem(4), 
			pData->GetInlay_gem(5), pData->GetInlay_gem(6), pData->GetInlay_gem(7),
			pData->GetInlay_gem(8), pData->GetInlay_gem(9));
	}
	if (pData->IsUpdated(CItemData::inlayAttrMask))
	{
		SetInlayAttr(pData->GetInlay_STA(), pData->GetInlay_SPR(), pData->GetInlay_STR(),
					pData->GetInlay_CON(), pData->GetInlay_AGI(), pData->GetInlay_ATK(),
					pData->GetInlay_DEF(), pData->GetInlay_HIT(), pData->GetInlay_EVA(),
					pData->GetInlay_POW(), pData->GetInlay_SPD(), pData->GetInlay_maxHP(),
					pData->GetInlay_maxSP(), pData->GetInlay_maxVP());
	}
	if (pData->IsUpdated(CItemData::addonMask))
	{
		SetAddon(pData->GetAddon());
	}
	if (pData->IsUpdated(CItemData::buffIDMask))
	{
		SetBuffID(pData->GetBuffID());
		m_buffData = gGlobal.m_resource->GetBuffData(pData->GetBuffID());
	}
	if (pData->IsUpdated(CItemData::buffValue1Mask))
		SetBuffValue1(pData->GetBuffValue1());
	if (pData->IsUpdated(CItemData::buffValue2Mask))
		SetBuffValue2(pData->GetBuffValue2());
	if (pData->IsUpdated(CItemData::buffValue3Mask))
		SetBuffValue3(pData->GetBuffValue3());
	if (pData->IsUpdated(CItemData::buffCoolDownMask))
		SetBuffCoolDown(pData->GetBuffCoolDown());
	if (pData->IsUpdated(CItemData::buffProMask))
		SetBuffPro(pData->GetBuffPro());
	if (pData->IsUpdated(CItemData::eqColorMask))
	{
		SetEqColor(pData->GetEqColor());
	}

	if (pData->IsUpdated(CItemData::qualityMask))
	{
		SetQuality(pData->GetItem_quality());
	}

	if (pData->IsUpdated(CItemData::soulboundStateMask))
	{
		SetSoulboundState(pData->GetItem_soulboundState());
	}

	if (pData->IsUpdated(CItemData::ExpireDateMask))
	{
		SetExpireDate(pData->GetItem_expireDate());
		if (m_data && m_data->item_TimeLimitType != ITEM_TIME_LIMIT_TYPE_NONE &&
			GetItem_expireDate() != 0)
		{
			UInt32 now = (UInt32) time(NULL);
			if (GetItem_expireDate() > (UInt32)now)
				RaiseUniqEventID(EVT_UPDATE_ITEM_EXPIRE, (GetItem_expireDate() - (UInt32)now) * 1000);
			else
				RaiseUniqEventID(EVT_UPDATE_ITEM_EXPIRE);
		}
	}

	if (pData->IsUpdated(CItemData::IdentifyStateMask))
	{
		SetIdentifyState(pData->GetItem_IdentifyState());
	}

	if (pData->IsUpdated(CItemData::ValueMask))
	{
		SetValue(pData->GetItemValue1(), pData->GetItemValue2(), pData->GetItemValue3());
	}
	if (pData->IsUpdated(CItemData::inWareHouseMask))
	{
		SetInWareHouse(pData->GetInWareHouse());
	}
	if (pData->IsUpdated(CItemData::assetLockMask))
	{
		SetLock(pData->GetLock());
	}
	if (pData->IsUpdated(CItemData::skillIDMask))
	{
		SetSkillID(pData->GetSkillID());
	}

	if (bClear)
		ClearAllMask();
}

void CItem::WriteData(CItemData *pData)
{
	TRACE_ENSURE(pData);

	pData->ClearUpdateMask();
	if (IsUpdated(owner_idMask))
	{
		pData->SetOwner_id(GetOwner_id());
	}
	if (IsUpdated(bagIndexMask))
	{
		pData->SetBagIndex(GetBagIndex());
	}
	if (IsUpdated(makerMask))
	{
		pData->SetMaker(GetMaker());
	}
	if (IsUpdated(eq_elementMask))
	{
		pData->SetEq_element(GetEq_element());
	}
	if (IsUpdated(endureMask))
	{
		pData->SetEndure(GetEndure());
	}
	if (IsUpdated(mEndureMask))
	{
		pData->SetMEndure(GetMEndure());
	}
	if (IsUpdated(gradeMask))
	{
		pData->SetGrade(GetGrade());
	}
	if (IsUpdated(upgExpMask))
	{
		pData->SetUpgExp(GetUpg_exp());
	}
	if (IsUpdated(remainMask))
	{
		pData->SetRemain(GetRemain());
	}
	if (IsUpdated(stackMask))
	{
		pData->SetStack(GetStack());
	}
	if (IsUpdated(baseAttrMask))
	{
		pData->SetBaseAttr(GetBase_STA(), GetBase_SPR(), GetBase_STR(),
					GetBase_CON(), GetBase_AGI(), GetBase_ATK(),
					GetBase_DEF(), GetBase_HIT(), GetBase_EVA(),
					GetBase_POW(), GetBase_SPD(), GetBase_maxHP(),
					GetBase_maxSP(), GetBase_maxVP());
	}
	if (IsUpdated(upgAttrMask))
	{
		pData->SetUpgAttr(GetUpg_STA(), GetUpg_SPR(), GetUpg_STR(),
					GetUpg_CON(), GetUpg_AGI(), GetUpg_ATK(),
					GetUpg_DEF(), GetUpg_HIT(), GetUpg_EVA(),
					GetUpg_POW(), GetUpg_SPD(), GetUpg_maxHP(),
					GetUpg_maxSP(), GetUpg_maxVP());
	}
	if (IsUpdated(inlayGemMask))
	{
		pData->SetInlayGem(GetEq_gem(), GetInlay_gem(0), GetInlay_gem(1),
			GetInlay_gem(2), GetInlay_gem(3), GetInlay_gem(4), 
			GetInlay_gem(5), GetInlay_gem(6), GetInlay_gem(7),
			GetInlay_gem(8), GetInlay_gem(9));
	}
	if (IsUpdated(inlayAttrMask))
	{
		pData->SetInlayAttr(GetInlay_STA(), GetInlay_SPR(), GetInlay_STR(),
					GetInlay_CON(), GetInlay_AGI(), GetInlay_ATK(),
					GetInlay_DEF(), GetInlay_HIT(), GetInlay_EVA(),
					GetInlay_POW(), GetInlay_SPD(), GetInlay_maxHP(),
					GetInlay_maxSP(), GetInlay_maxVP());
	}
	if (IsUpdated(addonMask))
	{
		pData->SetAddon(GetAddon());
	}
	if (IsUpdated(buffIDMask))
	{
		pData->SetBuffID(GetBuffID());
	} 

	if (IsUpdated(buffValue1Mask))
	{
		pData->SetBuffValue1(GetBuffValue1());
	}
	if (IsUpdated(buffValue2Mask))
	{
		pData->SetBuffValue2(GetBuffValue2());
	}
	if (IsUpdated(buffValue3Mask))
	{
		pData->SetBuffValue3(GetBuffValue3());
	}
	if (IsUpdated(buffCoolDownMask))
	{
		pData->SetBuffCoolDown(GetBuffCoolDown());
	}
	if (IsUpdated(buffProMask))
	{
		pData->SetBuffPro(GetBuffPro());
	}
	if (IsUpdated(eqColorMask))
	{
		pData->SetEqColor(GetEqColor());
	}

	if (IsUpdated(qualityMask))
	{
		pData->SetQuality(GetItem_quality());
	}

	if (IsUpdated(soulboundStateMask))
	{
		pData->SetSoulboundState(GetItem_soulboundState());
	}

	if (IsUpdated(ExpireDateMask))
	{
		pData->SetExpireDate(GetItem_expireDate());
	}

	if (IsUpdated(ValueMask))
	{
		pData->SetValue(GetItemValue1(), GetItemValue2(), GetItemValue3());
	}
	if (IsUpdated(IdentifyStateMask))
	{
		pData->SetIdentifyState(GetItem_IdentifyState());
	}
	if (IsUpdated(inWareHouseMask))
	{
		pData->SetInWareHouse(GetInWareHouse());
	}
	if (IsUpdated(skillIDMask))
	{
		pData->SetSkillID(GetSkillID());
	} 

	if (IsUpdated(assetLockMask))
	{
		pData->SetLock(GetLock());
	} 

	if (pData->IsUpdated() || IsUpdated(idMask)) {
		pData->SetId(GetItem_uid(), GetItem_id());
		pData->SetSendMask(pData->GetUpdateMask());
	}
}

void CItem::PreSend(UInt8 detail)
{
	m_sendMask.Clear();
	
	if (detail == 0)
		m_sendMask.MaskBits(idMask, owner_idMask, bagIndexMask, remainMask, 
		makerMask, stackMask, baseAttrMask, soulboundStateMask, endureMask, 
		mEndureMask, buffIDMask, buffValue1Mask, buffValue2Mask, buffValue3Mask, 
		buffCoolDownMask, buffProMask, IdentifyStateMask, ExpireDateMask, inWareHouseMask, qualityMask, assetLockMask, skillIDMask, inlayGemMask, inlayAttrMask,ValueMask, -1);
	else
		m_sendMask.MaskBits(makerMask, eq_elementMask, endureMask, 
			mEndureMask, gradeMask, upgExpMask, remainMask, stackMask, 
			baseAttrMask, upgAttrMask, inlayGemMask, inlayAttrMask,
			addonMask, buffIDMask, buffValue1Mask, buffValue2Mask, buffValue3Mask, 
			buffCoolDownMask, buffProMask, soulboundStateMask, IdentifyStateMask, 
			ExpireDateMask, inWareHouseMask, qualityMask, assetLockMask, skillIDMask,ValueMask, -1);
	
}

String CItem::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", item_id %d, item_uid %d, owner_id %d, bagIndex %d, stack %d"), 
		GetItem_id(), GetItem_uid(), GetOwner_id(), GetBagIndex(), GetStack());
	return s;
}

void CItem::AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		RemoveBuff();
		m_buffData = gGlobal.m_resource->GetBuffData(Buff_id);
		if (pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			Buff_CoolDown *= 60;

		SetBuffID(Buff_id);
		SetBuffValue1(Buff_Value1);
		SetBuffValue2(Buff_Value2);
		SetBuffValue3(Buff_Value3);
		SetBuffCoolDown(Buff_CoolDown);
		SetBuffPro(Pro);
		m_BuffPreUpdateTime = ::timeGetTime();

		CCharacter * pChar =gGlobal.m_charToSession.FindCharacter(GetOwner_id());
		if (pChar)
		{
			if (GetBagIndex() >= BAG_MAX_ITEM)
			{
				pChar->AddBuff(Buff_id, Buff_Value1, Buff_Value2, Buff_Value3, Buff_CoolDown / 60, Pro);
			}
			if (pChar->m_pBag)
				pChar->m_pBag->RaiseUpdate();
		}

		RaiseUniqEventID(EVT_UPDATE_BUFF, GetBuffCoolDown() * 1000);
	}
}
void CItem::RemoveBuff()
{
	if (GetBuffID())
	{
		UInt16 Id = GetBuffID();
		CancelUniqEvent(EVT_UPDATE_BUFF);
		SetBuffID(0);
		SetBuffValue1(0);
		SetBuffValue2(0);
		SetBuffValue3(0);
		SetBuffCoolDown(0);
		SetBuffPro(0);
		m_buffData = NULL;
		
		//find back the character
		CCharacter * pChar =gGlobal.m_charToSession.FindCharacter(GetOwner_id());
		if (pChar)
		{
			if (GetBagIndex() >= BAG_MAX_ITEM)
			{
				pChar->RemoveBuff(Id);
			}
			if (pChar->m_pBag)
				pChar->m_pBag->RaiseUpdate();
		}
	}
}
bool CItem::IsRemoveAfterOffline()
{
	if (m_buffData)
	{
		return m_buffData->Buff_RemoveAftOffline > 0;
	}
	return false;
}

UInt8 CItem::GetItemColor()
{
	if (m_data)
	{
		return m_data->color;
	}
	return 0;
}

UInt32 CItem::GetEqGemID(UInt8 nIndex)
{
	UInt8 nNum = 0;
	for (UInt8 i = 0; i < 10; i++)
	{
		UInt32 InlayItemId = GetInlay_gem(i);
		if (InlayItemId > 0)
		{
			nNum++;
			if(nIndex == nNum)
				return InlayItemId;
		}
	}
	return 0;
}

void CItem::UpdateBuff()
{
	UInt32 curTime = ::timeGetTime();
	if (m_buffData && m_buffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END &&
		m_buffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
	{
		{
			UInt32 delTime = 0 ;
			if (curTime > m_BuffPreUpdateTime)
			{
				delTime = (curTime - m_BuffPreUpdateTime) / 1000;
				if (delTime > 0)
					m_BuffPreUpdateTime += delTime * 1000;
			}
			if (GetBuffCoolDown() > delTime)
			{
				SetBuffCoolDown(GetBuffCoolDown() - delTime);
				RaiseUniqEventID(EVT_UPDATE_BUFF, GetBuffCoolDown() * 1000);
			}
			else
			{
				SetBuffCoolDown(0);
				RemoveBuff();
			}
				
		}

	}
}

void CItem::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_UPDATE_BUFF:				// update all partner to object service
		UpdateBuff();
		break;
	case EVT_UPDATE_ITEM_EXPIRE:
		{
			CCharacter * pChar =gGlobal.m_charToSession.FindCharacter(GetOwner_id());
			if (m_data && m_data->item_TimeLimitType != ITEM_TIME_LIMIT_TYPE_NONE &&
				GetItem_expireDate() != 0 && pChar)
			{
				time_t now;
				time(&now);
				if (GetItem_expireDate() <= (UInt32)now)
				{
					if (GetBagIndex() >= BAG_MAX_ITEM && GetBagIndex() < BAG_MAX_SIZE && GetInWareHouse() == 0)
					{
						switch (GetBagIndex() - BAG_MAX_ITEM)
						{
						case EQPLACE_WEAPON:
							pChar->SetEqWeapon(0, 0);
							break;
						case EQPLACE_HEAD_ACCESSORY:
							pChar->SetEqHead(0, 0);
							break;
						case EQPLACE_CLOTH:
							pChar->SetEqCloth(0, 0);
							break;
						}
						pChar->RaiseUpdate();
					}
					SetStack(0);
					pChar->m_pBag->AddToDeleteGroup(GetBagIndex(), GetInWareHouse());
					pChar->m_pBag->RaiseUpdate();
				}
				else
					RaiseUniqEventID(EVT_UPDATE_ITEM_EXPIRE, (GetItem_expireDate() - (UInt32)now) * 1000);
				
			}
		}
		break;
	}
}

bool CItem::AddSkill()
{
	if (m_data && m_data->skill_ids.size() > 0 && m_data->skill_ids[0] > 0 && m_data->overlap == 1)
	{
		RemoveSkill();
		if(m_data->probability < (UInt32)rand() % 100 + 1)
			return false;
		UInt16 Id = m_data->skill_ids[rand() % m_data->skill_ids.size()];
		SetSkillID(Id);

		CCharacter * pChar =gGlobal.m_charToSession.FindCharacter(GetOwner_id());
		if (pChar)
		{
			if (GetBagIndex() >= BAG_MAX_ITEM && GetEndure() > 0)
			{
				pChar->AddSkill(Id);
			}
			if (pChar->m_pBag)
				pChar->m_pBag->RaiseUpdate();
		}
		return true;
	}
	return false;
}

void CItem::RemoveSkill()
{
	if (GetSkillID())
	{
		UInt16 Id = GetSkillID();
		SetSkillID(0);
		CCharacter * pChar =gGlobal.m_charToSession.FindCharacter(GetOwner_id());
		if (pChar)
		{
			if (GetBagIndex() >= BAG_MAX_ITEM)
			{
				pChar->RemoveSkill(Id);
			}
			if (pChar->m_pBag)
				pChar->m_pBag->RaiseUpdate();
		}
	}
}

void CItem::ResetCreateType()
{
	m_iCreateType = CItemBag::CREATE_NONE;
}
