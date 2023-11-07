//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Gift.h"
#include "Proxy/Data/GiftData.h"

REGISTER_CLASSTYPE(CGift, Gift_CLASSID);

void CGift::SetData(CGiftData* gData)
{
	SetIdInfo(gData->GetGiftID(), gData->GetAcctID());
	SetGiftInfo(gData->GetItemID(), gData->GetItemType(), gData->GetItemQuantity());
	SetSerialNoInfo(gData->GetSerialNo());

	ClearAllMask();
}

