//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "SimClient/Item.h"
//-- Library
#include "SimClient/Global.h"
#include "SimClient/Character.h"
#include "Common/DataCode.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"

REGISTER_CLASSTYPE(CItem, Item_CLASSID);

void CItem::OnUpdateId(const UInt32 &item_uid, const UInt32 &item_id)
{
	if (gGlobal.m_resource)
		m_data = gGlobal.m_resource->GetItemData(item_id);
}

void CItem::OnUpdateRemain(const UInt8 &remain)
{
	m_remain = remain;
}

void CItem::OnUpdateStack(const UInt8 &stack)
{
	m_stack = stack;
}

Boolean CItem::AllowSexClass(UInt8 sex, UInt8 cclass)
{
	if (!m_data) return BOOLEAN_FALSE;

	UInt8 sexClass = (cclass > 0 ? ((cclass - 1) * 2 + sex + 1) : 100 + sex);
	return (m_data->reqSexClass1 == SEXCLASS_NONE || 
			sex + 100 == m_data->reqSexClass1 || sexClass == m_data->reqSexClass1 ||
			sex + 100 == m_data->reqSexClass2 || sexClass == m_data->reqSexClass2);
}

Boolean CItem::IsBound()
{
	return GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND;
}

Boolean CItem::CanUse(Boolean inMap)
{
	if (gGlobal.m_pSimClient == NULL || 
		gGlobal.m_pSimClient->m_pCharacter == NULL || m_data == NULL)
		return false;

	// can use in battle or map?
	if (m_data->usePlace != FIELD_ANY &&
		(inMap && m_data->usePlace != FIELD_MAP ||
		!inMap && m_data->usePlace != FIELD_BATTLE))
		return false;

	// use count > 0
	if (m_data->useCount == 0 || GetStack() == 0)
		return false;

	// fulfill all requirement?
	if (m_data->reqFaction != 0 && 
		m_data->reqFaction != gGlobal.m_pSimClient->m_pCharacter->GetFaction() || // req faction
		m_data->reqLv > gGlobal.m_pSimClient->m_pCharacter->GetLevel() ||		// req level
		!AllowSexClass(gGlobal.m_pSimClient->m_pCharacter->GetSex(), 
						gGlobal.m_pSimClient->m_pCharacter->GetCclass()))	// req sex and class
	{
		return false;
	}

	return true;
}

Boolean CItem::CanEquip()
{
	if (gGlobal.m_pSimClient == NULL || 
		gGlobal.m_pSimClient->m_pCharacter == NULL || m_data == NULL)
		return false;

	// is equipable?
	if (m_data->item_type != ITEMTYPE_EQUIP)
		return false;

	// fulfill all requirement?
	if (m_data->reqFaction != 0 && 
		m_data->reqFaction != gGlobal.m_pSimClient->m_pCharacter->GetFaction() || // req faction
		m_data->reqLv > gGlobal.m_pSimClient->m_pCharacter->GetLevel() ||		// req level
		!AllowSexClass(gGlobal.m_pSimClient->m_pCharacter->GetSex(), 
						gGlobal.m_pSimClient->m_pCharacter->GetCclass()))	// req sex and class
	{
		return false;
	}

	return true;
}

void CItem::ShowDetail()
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Item Detail:"));
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("  item_uid %d, item_id %d, owner_id %d, bagIndex %d"), 
		GetItem_uid(), GetItem_id(), GetOwner_id(), GetBagIndex());
	if (m_data && m_data->item_type == ITEMTYPE_EQUIP)
	{
		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("  eq_color %d, eq_element %d, endure %d/%d, grade %d, upg_exp %d"), 
			GetEqColor(), GetEq_element(), GetEndure(), GetMEndure(), GetGrade(), GetUpg_exp());
		TRACE_INFODT_9(GLOBAL_LOGGER, _F("  STA %d+%d+%d, SPR %d+%d+%d, STR %d+%d+%d, "),
			GetBase_STA(), GetUpg_STA(), GetInlay_STA(), 
			GetBase_SPR(), GetUpg_SPR(), GetInlay_SPR(), 
			GetBase_STR(), GetUpg_STR(), GetInlay_STR());
		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("CON %d+%d+%d, AGI %d+%d+%d"), 
			GetBase_CON(), GetUpg_CON(), GetInlay_CON(), 
			GetBase_AGI(), GetUpg_AGI(), GetInlay_AGI());
		TRACE_INFODT_9(GLOBAL_LOGGER, _F("  ATK %d+%d+%d, DEF %d+%d+%d, HIT %d+%d+%d, "),
			GetBase_ATK(), GetUpg_ATK(), GetInlay_ATK(), 
			GetBase_DEF(), GetUpg_DEF(), GetInlay_DEF(), 
			GetBase_HIT(), GetUpg_HIT(), GetInlay_HIT());
		TRACE_INFODTL_9(GLOBAL_LOGGER, _F("EVA %d+%d+%d, POW %d+%d+%d, SPD %d+%d+%d"),
			GetBase_EVA(), GetUpg_EVA(), GetInlay_EVA(), 
			GetBase_POW(), GetUpg_POW(), GetInlay_POW(), 
			GetBase_SPD(), GetUpg_SPD(), GetInlay_SPD());
		TRACE_INFODTL_9(GLOBAL_LOGGER, _F("  maxHP %d+%d+%d, maxSP %d+%d+%d, maxVP %d+%d+%d"),
			GetBase_maxHP(), GetUpg_maxHP(), GetInlay_maxHP(), 
			GetBase_maxSP(), GetUpg_maxSP(), GetInlay_maxSP(), 
			GetBase_maxVP(), GetUpg_maxVP(), GetInlay_maxVP());
		TRACE_INFODT_9(GLOBAL_LOGGER, _F("  Gems(%d): %d, %d, %d, %d, %d, %d, %d, %d, "),
			GetEq_gem(), GetInlay_gem(0), GetInlay_gem(1), GetInlay_gem(2), 
			GetInlay_gem(3), GetInlay_gem(4), GetInlay_gem(5), GetInlay_gem(6), 
			GetInlay_gem(7));
		//TRACE_INFODTL_4(GLOBAL_LOGGER, _F("%d, %d, addon %d, buff %s"), GetInlay_gem(8), GetInlay_gem(9), GetAddon(), GetBuff().c_str());
	}
	else
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F(", stack %d, remain %d"), 
		GetStack(), GetRemain());
	}
}

String CItem::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", item_id %d, item_uid %d, owner_id %d, bagIndex %d, stack %d"), 
		GetItem_id(), GetItem_uid(), GetOwner_id(), GetBagIndex(), GetStack());
	return s;
}