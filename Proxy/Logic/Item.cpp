//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Item.h"
//-- Library
#include "QGL/CharacterControl.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"

#ifdef QGL
#include "QGL/Global.h"
#endif // QGL

REGISTER_CLASSTYPE(CItem, Item_CLASSID);

void CItem::OnUpdateId(const UInt32 &item_uid, const UInt32 &item_id)
{
#ifdef QGL
	if (gGlobal.m_resource)
		m_data = gGlobal.m_resource->GetItemData(item_id);
#endif // QGL
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
	return (GetItem_soulboundState() == SOULBOUND_ALREADY_BOUND);
}

Boolean CItem::CanUse(Boolean inMap)
{
#ifdef QGL
	if (gGlobal.g_pCharacter == NULL || m_data == NULL)
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
		m_data->reqFaction != gGlobal.g_pCharacter->GetFaction() || // req faction
		m_data->reqLv > gGlobal.g_pCharacter->GetLevel() ||		// req level
		!AllowSexClass(gGlobal.g_pCharacter->GetSex(), 
						gGlobal.g_pCharacter->GetCclass()))	// req sex and class
	{
		return false;
	}
#endif
	return true;
}

Boolean CItem::CanEquip()
{
#ifdef QGL
	if (gGlobal.g_pCharacter == NULL || m_data == NULL)
		return false;

	// is equipable?
	if (m_data->item_type != ITEMTYPE_EQUIP)
		return false;

	// fulfill all requirement?
	if (m_data->reqFaction != 0 && 
		m_data->reqFaction != gGlobal.g_pCharacter->GetFaction() || // req faction
		m_data->reqLv > gGlobal.g_pCharacter->GetLevel() ||		// req level
		!AllowSexClass(gGlobal.g_pCharacter->GetSex(), 
						gGlobal.g_pCharacter->GetCclass()))	// req sex and class
	{
		return false;
	}
#endif
	return true;
}

UInt8 CItem::GetItemType()
{
	return (m_data ? m_data->item_type : 0);
}

UInt8 CItem::GetTargetType()
{
	return (m_data ? m_data->targetType : 0);
}