//-- Common
#include "Common.h"
#include "common/common_binary.h"
//-- Self
#include "SimClient/HawkItem.h"
#include "SimClient/ItemBag.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/PetGroup.h"

REGISTER_CLASSTYPE(CHawkItem, HawkItem_CLASSID);

CHawkItem::CHawkItem()
{
	m_data = NULL;
}

CHawkItem::~CHawkItem()
{

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