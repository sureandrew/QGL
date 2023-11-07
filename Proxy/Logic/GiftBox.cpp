//-- Common
#include "Common.h"
#include "common/common_binary.h"
//-- Self
#include "Proxy/Logic/GiftBox.h"
#include "Proxy/Logic/Gift.h"
#include "QGL/MenuGiftBox.h"
#include "QGL/Global.h"
#include "Common/CallLuaFunc.h"

REGISTER_CLASSTYPE(CGiftBox, GiftBox_CLASSID);

CGiftBox::CGiftBox()
{
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();
	m_iTakeGiftID = 0;
}

CGiftBox::~CGiftBox()
{
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();

}

void CGiftBox::ConstructGiftBox()
{
	for (Index i = C_INDEX(0); i < GetCount(); ++i)
	{
		CGift* pGift = GetChild<CGift>(i);
		if (pGift)
		{
			switch(pGift->GetItemType())
			{
			case GIFT_ITEM:
				m_itemList.push_back(pGift);
				break;
			case GIFT_PARTNER:
				m_partnerList.push_back(pGift);
				break;
			case GIFT_PET:
				m_petList.push_back(pGift);
				break;
			}
		}
	}
}

UInt CGiftBox::GetGiftBoxSize(UInt type)
{
	switch(type)
	{
	case GIFT_ITEM:
		return (UInt)m_itemList.size();
		break;
	case GIFT_PARTNER:
		return (UInt)m_partnerList.size();
		break;
	case GIFT_PET:
		return (UInt)m_petList.size();
		break;	
	}
	return 0;
}

CGift* CGiftBox::GetGift(UInt type, UInt slot)
{
	switch(type)
	{
	case GIFT_ITEM:
		{
			if (slot < m_itemList.size())
				return m_itemList[slot];
		}
		break;
	case GIFT_PARTNER:
		{
			if (slot < m_partnerList.size())
				return m_partnerList[slot];
		}
		break;
	case GIFT_PET:
		{
			if (slot < m_petList.size())
				return m_petList[slot];
		}
		break;	
	}
	return NULL;
}

RPCResult CGiftBox::CB_TakeGiftBox(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex)
{
	if (m_iTakeGiftID > 0)
	{
		for (Index i = C_INDEX(0); i < GetCount(); ++i)
		{
			CGift* pGift = GetChild<CGift>(i);
			if (pGift && pGift->GetGiftID() == m_iTakeGiftID)
			{
				gGlobal.g_pGiftBoxProcess->ShowResultMessage(msgIndex, pGift->GetItemID(), pGift->GetItemType());
				if (success)
				{
					switch(pGift->GetItemType())
					{
					case GIFT_ITEM:
						m_itemList.erase(remove(m_itemList.begin(), m_itemList.end(), pGift));
						break;
					case GIFT_PARTNER:
						m_partnerList.erase(remove(m_partnerList.begin(), m_partnerList.end(), pGift));
						break;
					case GIFT_PET:
						m_petList.erase(remove(m_petList.begin(), m_petList.end(), pGift));
						break;
					}
					Remove(pGift);
					DeleteNetObject(pGift);
				}
				else
				{

				}
				break;
			}
		}
	}

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	m_iTakeGiftID = 0;
	gGlobal.g_pGiftBoxProcess->UpdateGiftBox();
	return RPC_RESULT_OK;
}

RPCResult CGiftBox::CB_UpdateGiftBox(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex,  NetGroup* updateGrp)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	if (updateGrp && msgIndex == GIFTBOX_UDPATE_SUCCESS)
	{
		for (Index i = C_INDEX(0); i < updateGrp->GetCount(); ++i)
		{
			CGift* pGift = updateGrp->GetChild<CGift>(i);
			if (pGift)
			{
				Add(pGift);
				switch(pGift->GetItemType())
				{
				case GIFT_ITEM:		m_itemList.push_back(pGift);	break;
				case GIFT_PARTNER:	m_partnerList.push_back(pGift);	break;
				case GIFT_PET:		m_petList.push_back(pGift);		break;
				}
			}
		}		
		gGlobal.g_pGiftBoxProcess->UpdateGiftBox();
	}

	String msg;
	switch (msgIndex)
	{
	case GIFTBOX_UDPATE_SUCCESS:
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GIFTBOX_UPDATE_SUCCESS"));
		break;
	case GIFTBOX_UDPATE_FALSE:
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GIFTBOX_UPDATE_FAIL"));
		break;
	case GIFTBOX_UDPATE_NOT_IN_MAP:
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GIFTBOX_NOT_IN_MAP"));
		break;
	case GIFTBOX_UDPATE_SCRIPT_RUNNING:
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GIFTBOX_UPDATE_SCRIPT_RUNNING"));
		break;
	case GIFTBOX_UDPATE_TRADING:
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GIFTBOX_UPDATE_TRADING"));
		break;
	case GIFTBOX_UDPATE_TIMEOUT:

		break;
	}

	if (msg.length() > 0)
	{
		gGlobal.PrintPlayerMessage(msg);
	}
	return RPC_RESULT_OK;
}