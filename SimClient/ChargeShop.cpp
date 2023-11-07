//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "SimClient/ChargeShop.h"
#include "SimClient/Global.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"
#include "SimClient/SimClient.h"
#include "SimClient/Character.h"

PROTO_LuaPush(CChargeShop)
REGISTER_CLASSTYPE(CChargeShop, ChargeShop_CLASSID);

RPCResult CChargeShop::CB_BuyItemInChargeShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode
													 , const UInt32 &chargeItemID, const UInt8 &itemType, const UInt32 &stack, const UInt32 &money)
{
	switch(msgCode)
	{
	case CHARGESHOP_BUY_SUCCESS:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_CHARGE_SHOP_BUY_SUCCESS"));	
		break;
	case CHARGESHOP_NO_SELL_ITEM:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_CHARGE_SHOP_NO_SELL_ITEM"));		
		break;
	case CHARGESHOP_NO_ITEM_SLOT:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_TRADING_SLOT_FAIL"));			
		break;
	case CHARGESHOP_NO_PARTNER_SLOT:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_TRADING_PARTNER_FAIL"));		
		break;
	case CHARGESHOP_NO_PET_SLOT:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_TRADING_PET_FAIL"));		
		break;
	case CHARGESHOP_RUN_SCRIPT:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_CHARGE_SHOP_RUN_SCRIPT"));			
		break;
	case CHARGESHOP_CHAR_IN_BATTLE:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_CHARGE_SHOP_IN_BATTLE"));			
		break;
	case CHARGESHOP_NO_YUANBAO:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_CHARGE_SHOP_NO_YUANBAO"));		
		break;
	case CHARGESHOP_NOT_SALEDAY:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_CHARGE_SHOP_NOT_SALEDAY"));		
		break;
	case CHARGESHOP_NO_REMAIN:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("MSG_CHARGE_SHOP_NO_REMAIN"));		
		break;
	}

	if (gGlobal.m_pSimClient->m_bRobot)
        CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "CloseShop", String());	

	if (gGlobal.m_pSimClient->m_pCharacter)
		gGlobal.m_pSimClient->m_pCharacter->m_bIsBuyingChargeItem = false;
	return RPC_RESULT_OK; 
}