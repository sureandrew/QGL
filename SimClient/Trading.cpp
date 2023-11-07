//-- Common
#include "Common.h"
//-- Self
#include "SimClient/Trading.h"
//-- Library
#include "SimClient/Item.h"
#include "SimClient/Partner.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/Partner.h"
#include "SimClient/Global.h"
#include "SimClient/Pet.h"
#include "SimClient/PetGroup.h"
#include "SimClient/Character.h"
#include "SimClient/CharacterInfo.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "Common/CallLuaFunc.h"

	
REGISTER_CLASSTYPE(CTrading, Trading_CLASSID);

PROTO_LuaPush(CTrading)	


CTrading::~CTrading()
{
	ResetAllData();	

}

RPCResult CTrading::CB_SetItemSlot(LPCPEER pPeer, RPCContext &context, NetObject* item, const UInt8 &position)
{
	CItem *pItem =  CastNetObject<CItem>(item);	
	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;

	if (pItem)
	{
		////////////////////////The code for SimClient use////////////////////////

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Item %d is set by Player ID: %d"),
			pItem->GetItem_id(), gGlobal.m_pSimClient->m_TarTradeID);

		typedef StlMap<UInt16, UInt32, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, UInt32> > > itemMap;

		itemMap::iterator iter = m_oppItem.find(position);
		if (iter != m_oppItem.end())
		{
			iter->second = pItem->GetItem_id();
		}
		else
		{
			m_oppItem.insert(std::make_pair(position,pItem->GetItem_id()));
		}

		DeleteNetObject(pItem);	

	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetPartnerSlot(LPCPEER pPeer, RPCContext &context, NetObject* partner, 
									  const UInt16Vector &skillID, const UInt8 &position)
{
	CPartner* pPartner =  CastNetObject<CPartner>(partner);
	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;
    
	if (pPartner)
	{
	//////////////////////////////code for the simclient use ///////////////////////

		if (gGlobal.m_pSimClient->m_pCharacter == NULL)
			return RPC_RESULT_OK;
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Partner %d is set by Player ID: %d"),
			pPartner->GetPartner_id(), gGlobal.m_pSimClient->m_pCharacter->GetChar_id());

		typedef StlMap<UInt16, UInt32, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, UInt32> > > partnerMap;

		partnerMap::iterator iter = m_oppPartner.find(position);
		if (iter != m_oppPartner.end())
		{
			iter->second = pPartner->GetPartner_id();
		}
		else
		{
			m_oppPartner.insert(std::make_pair(position, pPartner->GetPartner_id()));
		}

		DeleteNetObject(pPartner);	
	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetPetSlot(LPCPEER pPeer, RPCContext &context, NetObject* pet, const UInt16Vector &skillID, const UInt8 &position)
{
	CPet* pPet =  CastNetObject<CPet>(pet);

	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;

	if (pPet)
	{

		if (gGlobal.m_pSimClient->m_pCharacter == NULL)
			return RPC_RESULT_OK;
	
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Pet %d:%d is set by Player ID: %d"),
			pPet->GetPet_uid(), pPet->GetPet_id(), gGlobal.m_pSimClient->m_pCharacter->GetChar_id());

	
	//////////////////////////////code for the simclient use ///////////////////////
		typedef StlMap<UInt16, UInt32, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, UInt32> > > petMap;

		petMap::iterator iter = m_oppPet.find(position);
		if (iter != m_oppPet.end())
		{
			iter->second = pPet->GetPet_id();
		}
		else
		{
			m_oppPet.insert(std::make_pair(position, pPet->GetPet_id()));
		}

		DeleteNetObject(pPet);	
	}
    
	return RPC_RESULT_OK;
}


RPCResult CTrading::CB_RemoveItemSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{

	typedef StlMap<UInt16, UInt32, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, UInt32> > > itemMap;
	itemMap::iterator iter = m_oppItem.find(position);
	if (iter != m_oppItem.end())
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Item %d is removed from sales slot by opposite party"),
			iter->second);
	}

	RemoveItem(m_oppItem, position);	

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_RemovePartnerSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{
	typedef StlMap<UInt16, UInt32, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, UInt32> > > partnerMap;
	partnerMap::iterator iter = m_oppPartner.find(position);
	if (iter != m_oppPartner.end())
	{
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Partner %d is removed from sales slot by opposite party"),	
				iter->second); 
	}
	RemoveItem(m_oppPartner, position);	
	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_RemovePetSlot(LPCPEER pPeer, RPCContext &context, const UInt8 &position)
{

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Pet in slot %d is removed by opposite party"),	
			position); 
	RemoveItem(m_oppPartner, position);	


	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetLock(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Boolean &lock)
{

	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_pCharacter == NULL)
		return RPC_RESULT_OK;

	if (gGlobal.m_pSimClient->m_pCharacter->GetChar_id() != char_id)
	{
		m_bTargetLock = lock;
		gGlobal.m_pSimClient->m_btarlock = lock;
		if (lock)
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Trade is locked by the other side"));
		}
		else
		{
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Trade is unlocked by the other side"));
		}
		if ((gGlobal.m_pSimClient->m_bRobot))
		{
	//          gGlobal.m_pSimClient->m_pTrading->SetTradingLock(gGlobal.m_pSimClient->m_pProxyPeer,
	//		                         gGlobal.m_pSimClient->m_context, TRUE);
	//			TRACE_INFODTL(GLOBAL_LOGGER, _F("Trade is automatically locked"));
	//			gGlobal.m_pSimClient->m_btradelock = TRUE;	
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_CancelTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;


	gGlobal.m_pSimClient->m_bTradReqCfm = FALSE; 
    gGlobal.m_pSimClient->m_TarTradeID = 0; 
	gGlobal.m_pSimClient->m_getTradReq = FALSE;
	gGlobal.m_pSimClient->m_btarlock = FALSE;
	gGlobal.m_pSimClient->m_btradelock = FALSE;

    TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Trading has been cancel. trading netId %d,%d"),
		GetNetId().srvId, GetNetId().objId);
	ResetAllData();
	if (gGlobal.m_pSimClient->m_pTrading == this)
		DeleteNetObject(gGlobal.m_pSimClient->m_pTrading);
	else
	    TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_T("CB_CancelTrading Warning: [trading object mismatch], trading netId %d,%d"),
			GetNetId().srvId, GetNetId().objId);

	return RPC_RESULT_OK;
}

RPCResult CTrading::CB_ConfirmTrading(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id)
{
    TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Trading has confirmed. trading netId %d,%d"),
		GetNetId().srvId, GetNetId().objId);

	return RPC_RESULT_OK;
}

void CTrading::ResetAllData()
{
///////clean up the items/pets/partners traded by the opposite side
	while(m_oppItem.size() > 0)
	{
		oppMap::iterator iter = m_oppItem.begin();
		if (iter != m_oppItem.end())
		{
			m_oppItem.erase(iter);
		}

	}

	while(m_oppPartner.size() > 0)
	{
		oppMap::iterator iter = m_oppPartner.begin();
		if (iter != m_oppPartner.end())
		{
			m_oppPartner.erase(iter);
		}

	}

	while(m_oppPet.size() > 0)
	{
		oppMap::iterator iter = m_oppPet.begin();
		if (iter != m_oppPet.end())
		{
			m_oppPet.erase(iter);
		}

	}
///////////clean up the items/pets/partners traded by your side
	while(m_tradeItemPos.size() > 0)
	{
		oppMap::iterator iter = m_tradeItemPos.begin();
		if (iter != m_tradeItemPos.end())
		{
			m_tradeItemPos.erase(iter);
		}

	}

	while(m_tradePartnerPos.size() > 0)
	{
		oppMap::iterator iter = m_tradePartnerPos.begin();
		if (iter != m_tradePartnerPos.end())
		{
			m_tradePartnerPos.erase(iter);
		}

	}

	while(m_tradePetPos.size() > 0)
	{
		oppMap::iterator iter = m_tradePetPos.begin();
		if (iter != m_tradePetPos.end())
		{
			m_tradePetPos.erase(iter);
		}

	}


}

RPCResult CTrading::CB_TradingProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const String &message, const UInt32 &errorCharID)
{
	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_FAIL;

	if (success)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, 
			_T("CB_TradingProcess: trade success, trading netId %d,%d"), 
			GetNetId().srvId, GetNetId().objId);
	}
	else
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, 
			_F("trading netId %d,%d"), 
			GetNetId().srvId, GetNetId().objId);
	}
		
	gGlobal.m_pSimClient->m_bTradReqCfm = FALSE;	
	gGlobal.m_pSimClient->m_TarTradeID = 0;
	gGlobal.m_pSimClient->m_getTradReq = FALSE;
	gGlobal.m_pSimClient->m_btradelock = FALSE;	
	gGlobal.m_pSimClient->m_btarlock = FALSE;

	ResetAllData();			

	if (gGlobal.m_pSimClient->m_pTrading == this)
		DeleteNetObject(gGlobal.m_pSimClient->m_pTrading);
	else
	    TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_T("CB_TradingProcess Warning: [trading object mismatch], trading netId %d,%d"),
			GetNetId().srvId, GetNetId().objId);

	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingItem(LPCPEER pPeer, RPCContext &context, const UInt32Vector &itemID, const UInt8Vector &itemStacks)
{
	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingPartner(LPCPEER pPeer, RPCContext &context,  const UInt32Vector &partnerID)
{
	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingPet(LPCPEER pPeer, RPCContext &context, const UInt32Vector &petID)
{
	return RPC_RESULT_OK;
}
	
RPCResult CTrading::CB_TradingMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &gmoney)
{
	return RPC_RESULT_OK;
}

void CTrading::RemoveItem(oppMap &itemList, UInt32 pos)
{
	//////////////////////////////code for the simclient use ///////////////////////
	oppMap::iterator iter = itemList.find(pos);
	if (iter != itemList.end())
	{
		itemList.erase(iter);
	}
 

}

