//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Trading.h"
#include "QGL/Global.h"
#include "QGL/MenuTradingProcess.h"
#include "QGL/CharacterControl.h"
#include "QGL/LuaPush.h"
#include "QGL/CallLuaFunc.h"
#include "Proxy/Logic/Item.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "Proxy/Logic/PartnerGroup.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/Pet.h"

REGISTER_CLASSTYPE(CTrading, Trading_CLASSID);


RPCResult CTrading::CB_SetItemSlot(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt8 &position)
{
	CItem* pItem =  CastNetObject<CItem>(item);

	if (pItem)
	{
		TargetItemList::iterator itr = m_targetItemList.find(position);
		if (itr != m_targetItemList.end())
		{
			CItem* temp = itr->second;
			if (pItem != temp)
			{
				DeleteNetObject(temp);
				itr->second =  pItem;
			}
		}
		else
			m_targetItemList.insert(std::make_pair(position, pItem));
		
		if (gGlobal.g_pTradingProcess)
			gGlobal.g_pTradingProcess->DisplayTargetItemSlot(pItem->GetItem_id(), pItem->GetStack(), position);
	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetPartnerSlot(LPCPEER pPeer, RPCContext &context, NetObject* partner, const UInt16Vector &skillID, const UInt8 &position)
{
	CPartner* pPartner =  CastNetObject<CPartner>(partner);

	if (pPartner)
	{
		TargetPartnerList::iterator itr = m_targetPartnerList.find(position);
		TargetSkillList::iterator itr2 = m_targetPartnerSkillList.find(position);
		if (itr != m_targetPartnerList.end() && itr2 != m_targetPartnerSkillList.end())
		{
			CPartner* temp = itr->second;
			if (pPartner != temp)
			{
				DeleteNetObject(temp);
				itr->second =  pPartner;
			}
			itr2->second.clear();
			m_targetPartnerSkillList.erase(itr2);
			m_targetPartnerSkillList.insert(std::make_pair(position, skillID));
			
		}
		else
		{
			m_targetPartnerList.insert(std::make_pair(position, pPartner));
			m_targetPartnerSkillList.insert(std::make_pair(position, skillID));
		}
		
		if (gGlobal.g_pTradingProcess)
			gGlobal.g_pTradingProcess->DisplayTargetPartnerSlot(pPartner->GetMob_id(), position);
	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetPetSlot(LPCPEER pPeer, RPCContext &context, NetObject* pet, const UInt16Vector &skillID, const UInt8 &position)
{
	CPet* pPet =  CastNetObject<CPet>(pet);

	if (pPet)
	{
		TargetPetList::iterator itr = m_targetPetList.find(position);
		TargetSkillList::iterator itr2 = m_targetPetSkillList.find(position);
		if (itr != m_targetPetList.end() && itr2 != m_targetPetSkillList.end())
		{
			CPet* temp = itr->second;
			if (pPet != temp)
			{
				DeleteNetObject(temp);
				itr->second =  pPet;
			}
			itr2->second.clear();
			m_targetPetSkillList.erase(itr2);
			m_targetPetSkillList.insert(std::make_pair(position, skillID));
			
		}
		else
		{
			m_targetPetList.insert(std::make_pair(position, pPet));
			m_targetPetSkillList.insert(std::make_pair(position, skillID));
		}
		
		if (gGlobal.g_pTradingProcess)
			gGlobal.g_pTradingProcess->DisplayTargetPetSlot(pPet->GetPet_id(), position);
	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_RemoveItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	TargetItemList::iterator itr = m_targetItemList.find(position);
	if (itr != m_targetItemList.end())
	{
		CItem* temp = itr->second;
		if (temp)
		{
			DeleteNetObject(temp);
		}
		m_targetItemList.erase(itr);
	}
	if (gGlobal.g_pTradingProcess)
		gGlobal.g_pTradingProcess->DisplayTargetItemSlot(0, 0, position);

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_RemovePartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	TargetPartnerList::iterator itr = m_targetPartnerList.find(position);
	if (itr != m_targetPartnerList.end())
	{
		CPartner* temp = itr->second;
		if (temp)
		{
			DeleteNetObject(temp);
		}
		m_targetPartnerList.erase(itr);
	}

	TargetSkillList::iterator itr1 = m_targetPartnerSkillList.find(position);
	if (itr1 != m_targetPartnerSkillList.end())
	{
		itr1->second.clear();
		m_targetPartnerSkillList.erase(itr1);
	}

	if (gGlobal.g_pTradingProcess)
		gGlobal.g_pTradingProcess->DisplayTargetPartnerSlot(0, position);

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_RemovePetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	TargetPetList::iterator itr = m_targetPetList.find(position);
	if (itr != m_targetPetList.end())
	{
		CPet* temp = itr->second;
		if (temp)
			DeleteNetObject(temp);
		
		m_targetPetList.erase(itr);
	}

	TargetSkillList::iterator itr1 = m_targetPetSkillList.find(position);
	if (itr1 != m_targetPetSkillList.end())
	{
		itr1->second.clear();
		m_targetPetSkillList.erase(itr1);
	}

	if (gGlobal.g_pTradingProcess)
		gGlobal.g_pTradingProcess->DisplayTargetPetSlot(0, position);

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Boolean &lock)
{
	if (char_id != gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id() )
	{
		m_bTargetLock = lock;
		UIImage* img = gGlobal.pUIManager->FindUIImage("TargetTradingLockedImage");
		UIImage* imgCheck = gGlobal.pUIManager->FindUIImage("TradingTargetLock");
		UIImage* imgUnCheck = gGlobal.pUIManager->FindUIImage("TradingTargetUnLock");
		UIButton* confirmButton = gGlobal.pUIManager->FindUIButton("TradingYes");
		if (img && imgCheck && imgUnCheck && confirmButton)
		{
			img->SetEnable(lock);
			imgCheck->SetEnable(lock);
			imgUnCheck->SetEnable(!lock);
		
			if (lock)
			{
				if (gGlobal.g_pTradingProcess && gGlobal.g_pTradingProcess->IsLocked() )
				{
					confirmButton->SetNormalState();
					gGlobal.pUIManager->AddDirtyUIItem(confirmButton);
				}
			}
			else
			{
				confirmButton->SetDisableState();
				gGlobal.pUIManager->AddDirtyUIItem(confirmButton);
			}
		}
	}
	else
	{
		UIButton* confirmButton = gGlobal.pUIManager->FindUIButton("TradingYes");
		UICheckBox* tradingLock = gGlobal.pUIManager->FindUICheckBox("TradingLock");
		UIImage* lockedImage = gGlobal.pUIManager->FindUIImage("TradingLockedImage");
		if (gGlobal.g_pTradingProcess->IsLocked() && !lock && confirmButton && tradingLock && lockedImage)
		{
			gGlobal.g_pTradingProcess->ResetLock();
			tradingLock->SetState(false);
			lockedImage->SetEnable(false);
			confirmButton->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(confirmButton);

		}
	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	if (gGlobal.g_pTradingProcess)
		gGlobal.g_pTradingProcess->DisplayTargetMoney(gmoney);
	
	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_CancelTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	String message;
	if (owner_id == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id() )
		message = gGlobal.GetStringTable()->Get(_T("MSG_TRADING_CANCEL_BY_MYSELF"));
	else
		message = gGlobal.GetStringTable()->Get(_T("MSG_TRADING_CANCEL_BY_OTHER"));
	
	if (gGlobal.g_pTradingProcess)
	{
		message.Format(message.c_str(), gGlobal.g_pTradingProcess->GetTargetName().c_str());
		gGlobal.PrintMessage(message.c_str(), CHANNELTYPE_SYSTEM);
		gGlobal.g_pTradingProcess->ResetTradingProcess();
	}

	ResetAllData();

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "DeleteTradingPanel", String());
	if (gGlobal.g_pTrading)
		DeleteNetObject(gGlobal.g_pTrading);

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_ConfirmTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	if (owner_id != gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
		m_bTargetConfirm = true;
	return RPC_RESULT_OK;
}

void CTrading::ResetAllData()
{
	while(m_targetItemList.size() > 0)
	{
		TargetItemList::iterator itr = m_targetItemList.begin();
		if (itr != m_targetItemList.end() )
		{
			CItem* temp = itr->second;
			if (temp)
				DeleteNetObject(temp);
			m_targetItemList.erase(itr);
		}
	}

	while(m_targetPartnerList.size() > 0)
	{
		TargetPartnerList::iterator itr = m_targetPartnerList.begin();
		if (itr != m_targetPartnerList.end() )
		{
			CPartner* temp = itr->second;
			if (temp)
				DeleteNetObject(temp);
			m_targetPartnerList.erase(itr);
		}
	}
	
	while(m_targetPartnerSkillList.size() > 0)
	{
		TargetSkillList::iterator itr = m_targetPartnerSkillList.begin();
		if (itr != m_targetPartnerSkillList.end() )
		{
			itr->second.clear();
			m_targetPartnerSkillList.erase(itr);
		}
	}

	while(m_targetPetList.size() > 0)
	{
		TargetPetList::iterator itr = m_targetPetList.begin();
		if (itr != m_targetPetList.end() )
		{
			CPet* temp = itr->second;
			if (temp)
				DeleteNetObject(temp);
			m_targetPetList.erase(itr);
		}
	}

	while(m_targetPetSkillList.size() > 0)
	{
		TargetSkillList::iterator itr = m_targetPetSkillList.begin();
		if (itr != m_targetPetSkillList.end() )
		{
			itr->second.clear();
			m_targetPetSkillList.erase(itr);
		}
	}
}

RPCResult CTrading::CB_TradingProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const String &message, const UInt32 &errorCharID)
{
	if (!success)
	{
		String errorMsg;
		if (errorCharID == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id())
			errorMsg.Format(_T("%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_SINGLEWORD_YOU")), gGlobal.GetStringTable()->Get(message.c_str()));
		else if (errorCharID == gGlobal.g_pTradingProcess->GetTargetCharID())
			errorMsg.Format(_T("%s%s"), gGlobal.g_pTradingProcess->GetTargetName().c_str(), gGlobal.GetStringTable()->Get(message.c_str()));
		else
			errorMsg = gGlobal.GetStringTable()->Get(message.c_str());
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(message.c_str()),CHANNELTYPE_SYSTEM);
	}
	else
	{
		if (gGlobal.g_pTradingProcess)
			gGlobal.g_pTradingProcess->ShowTradingResultText();	
	}

	if (gGlobal.g_pTradingProcess)
		gGlobal.g_pTradingProcess->ResetTradingProcess();

	ResetAllData();

	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "DeleteTradingPanel", String());
	if (gGlobal.g_pTrading)
		DeleteNetObject(gGlobal.g_pTrading);
	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingItem(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemID, const UInt8Vector &itemStacks)
{
	if (itemID.size() > 0)
	{
		String msg;
		msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_GET"));

		for (Index i = C_INDEX(0); i < itemID.size(); ++i)
		{
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(itemID[i]);
			if (pItemData)
				msg.Format(_T("%s%d%s%s,"), msg.c_str(), itemStacks[i],gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_ITEM")), String(pItemData->name).c_str());
		}

		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingPartner(LPCPEER pPeer, RPCContext &context,  const UInt32Vector &partnerID)
{
	if (partnerID.size() > 0)
	{
		String msg;
		msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_GET"));
		for (Index i = C_INDEX(0); i < partnerID.size(); ++i)
		{
			CPartner* partner = gGlobal.g_pPartnerGroup->GetPartner(partnerID[i]);
			if (partner)
				msg.Format(_T("%s%s,"), msg.c_str(), partner->GetRaname().c_str());
		}
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}

	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingPet(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petID)
{
	if (petID.size() > 0)
	{
		String msg;
		msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_GET"));
		for (Index i = C_INDEX(0); i < petID.size(); ++i)
		{
			CPet* pet = gGlobal.g_PetLst->GetPet(petID[i]);
			if (pet)
				msg.Format(_T("%s%s,"), msg.c_str(), pet->GetName().c_str());
		}
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	if (gmoney > 0)
	{
		String msg;
		msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_GET"));
		msg.Format(_T("%s%d%s"), msg.c_str(), gmoney,gGlobal.GetStringTable()->Get(_T("MSG_ITEM_PRICE_UNIT2")));
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
	return RPC_RESULT_OK;
}

CItem* CTrading::GetTargetItem(UInt8 index)
{
	CItem* item = NULL;

	TargetItemList::iterator itr = m_targetItemList.find(index);
	if (itr != m_targetItemList.end())
		item = itr->second;

	return item;
}

CPartner* CTrading::GetTargetPartner(UInt8 index)
{
	CPartner* partner = NULL;
	TargetPartnerList::iterator itr = m_targetPartnerList.find(index);
	if (itr != m_targetPartnerList.end())
		partner = itr->second;
	return partner;
}

UInt16Vector CTrading::GetTargetPartnerSkill(UInt8 index)
{
	UInt16Vector skillIDs;
	TargetSkillList::iterator itr = m_targetPartnerSkillList.find(index);
	if (itr != m_targetPartnerSkillList.end())
		skillIDs = itr->second;
	
	return skillIDs;
}

CPet* CTrading::GetTargetPet(UInt8 index)
{
	CPet* pet = NULL;
	TargetPetList::iterator itr = m_targetPetList.find(index);
	if (itr != m_targetPetList.end())
		pet = itr->second;
	
	return pet;
}

UInt16Vector CTrading::GetTargetPetSkill(UInt8 index)
{
	UInt16Vector skillIDs;
	TargetSkillList::iterator itr = m_targetPetSkillList.find(index);
	if (itr != m_targetPetSkillList.end())
		skillIDs = itr->second;
	
	return skillIDs;
}

CTrading::~CTrading()
{
	ResetAllData();
}