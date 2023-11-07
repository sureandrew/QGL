//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/HawkItem.h"

REGISTER_CLASSTYPE(CHawkItem, HawkItem_CLASSID);

void CHawkItem::SetHawkData(HawkerItemInfo data, UInt8 type)
{
	SetIdInfo(data.item_uid, data.item_id);
	SetPriceInfo(data.item_price);
	SetIndexInfo(data.item_index);
	SetStackInfo(data.item_stack);
	SetItemTypeInfo(type);

	ClearAllMask();
}