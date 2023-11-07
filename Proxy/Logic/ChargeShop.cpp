//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "Proxy/Logic/ChargeShop.h"

#include "Resource/ResItem.h"
#include "Resource/ResMob.h"
#include "Resource/ResPet.h"
#include "Resource/ClientResManager.h"
#include "Common/CallLuaFunc.h"
#include "QGL/Global.h"

REGISTER_CLASSTYPE(CChargeShop, ChargeShop_CLASSID);


RPCResult CChargeShop::CB_BuyItemInChargeShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgCode, 
													 const UInt32 &chargeItemID, const UInt8 &itemType, const UInt32 &stack, const UInt32 &money)
{
#ifdef QGL
	String text;
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	switch(msgCode)
	{
	case CHARGESHOP_BUY_SUCCESS:
		{
			text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_BUY_SUCCESS"));
			String itemName;
			switch(itemType)
			{
			case CHARGE_ITEM_TYPE_ITEM:
				{
					const ItemData* pItemData = gGlobal.m_resource->GetItemData(chargeItemID);
					if (pItemData)
						itemName = String(pItemData->name);
				}
				break;
			case CHARGE_ITEM_TYPE_PARTNER:
				{
					MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(chargeItemID);
					if (mobTemplate)
						itemName = String(mobTemplate->m_basicinfo->mid_name);
				}
				break;
			case CHARGE_ITEM_TYPE_PET:
				{
					const PetData* pPetData = gGlobal.m_resource->GetPetData(chargeItemID);
					if (pPetData)
						itemName = String(pPetData->pet_name);
				}
				break;
			}
			if (itemName.length() > 0)
			{
				UInt32 totalCost = money;
				String temp, temp2;
				temp = gGlobal.GetStringTable()->Get(_T("MSG_SCRIPT_ADDITEM2"));
				temp2 = gGlobal.GetStringTable()->Get(_T("MSG_SPEND_YUANBAO"));
				temp.Format(temp.c_str(), itemName.c_str());
				temp2.Format(temp2.c_str(), totalCost);
				text.Format(_T("%s,%s,%s"), text.c_str(), temp.c_str(), temp2.c_str());
			}
		}
		break;
	case CHARGESHOP_NO_SELL_ITEM:
		text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_NO_SELL_ITEM"));
		break;
	case CHARGESHOP_NO_ITEM_SLOT:
		text =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_SLOT_FAIL"));
		break;
	case CHARGESHOP_NO_PARTNER_SLOT:
		text =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_PARTNER_FAIL"));
		break;
	case CHARGESHOP_NO_PET_SLOT:
		text =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_PET_FAIL"));
		break;
	case CHARGESHOP_RUN_SCRIPT:
		text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_RUN_SCRIPT"));
		break;
	case CHARGESHOP_CHAR_IN_BATTLE:
		text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_IN_BATTLE"));
		break;
	case CHARGESHOP_NO_YUANBAO:
		text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_NO_YUANBAO"));
		break;
	case CHARGESHOP_NOT_SALEDAY:
		text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_NOT_SALEDAY"));
		break;
	case CHARGESHOP_NO_REMAIN:
		text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_NO_REMAIN"));
		break;
	case CHARGESHOP_BOUGHT_LIMIT_ALREADY:
		text =gGlobal.GetStringTable()->Get(_T("MSG_CHARGE_SHOP_BOUGHT_LIMITED_ALREADY"));
		break;
	case CHARGESHOP_ITEM_REACH_LIMIT:
		text =gGlobal.GetStringTable()->Get(_T("MSG_ITEM_REACH_LIMIT"));
		break;
	case CHARGESHOP_YUANBAO_LOCK_ERROR:
		text = gGlobal.GetStringTable()->Get(_T("MSG_CHARGESHOP_YUANBAO_LOCK_ERROR"));
		break;
	}

	if (text.length() > 0)
	{
		gGlobal.PrintPlayerMessage(text);
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
	}
#endif
	return RPC_RESULT_OK; 
}