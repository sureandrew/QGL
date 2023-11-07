//-- Common
#include "Common.h"
#include "common/common_binary.h"
//-- Self
#include "Proxy/Logic/HawkItem.h"
#include "QGL/Global.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/PetGroup.h"

REGISTER_CLASSTYPE(CHawkItem, HawkItem_CLASSID);

CHawkItem::CHawkItem()
{
	m_data = NULL;
}

CHawkItem::~CHawkItem()
{
	if (m_data)
	{
		bool needDelete = true;

		switch (GetItem_type())
		{
		case HAWKER_ITEM:
			if (gGlobal.m_pBag && gGlobal.m_pBag->HasItemByUID(GetItem_uid()))
				needDelete = false;
			break;
		case HAWKER_PARTNER:
			if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerGroup->GetPartner(GetItem_uid()))
				needDelete = false;
			break;
		case HAWKER_PET:
			if (gGlobal.g_PetLst && gGlobal.g_PetLst->GetPet(GetItem_uid()))
				needDelete = false;
			break;
		}

		if (needDelete)
			DeleteNetObject(m_data);
	}
}

void CHawkItem::OnUpdateIdInfo(const UInt32 &item_uid, const UInt32 &item_id)
{

}

void CHawkItem::OnUpdateStackInfo(const UInt8 &item_stack)
{

}

void CHawkItem::OnUpdatePriceInfo(const UInt32 &item_price)
{

}

void CHawkItem::OnUpdateItemTypeInfo(const UInt8 &item_type)
{

}

void CHawkItem::OnUpdateIndexInfo(const UInt8 &item_index)
{

}