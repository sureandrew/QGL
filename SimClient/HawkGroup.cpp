//-- Common
#include "Common.h"
//-- Self
#include "SimClient/HawkGroup.h"
#include "SimClient/HawkItem.h"
#include "common/common_binary.h"
#include "SimClient/Global.h"
#include "SimClient/Item.h"
#include "SimClient/Partner.h"
#include "SimClient/Pet.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"



PROTO_LuaPush(CHawkGroup)

REGISTER_CLASSTYPE(CHawkGroup, HawkGroup_CLASSID);

CHawkGroup::CHawkGroup()
{
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();
}

CHawkGroup::~CHawkGroup()
{
	m_itemList.clear();
	m_partnerList.clear();
	m_petList.clear();

}

void CHawkGroup::ConstructHawkItem()
{
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CHawkItem* hawkItem = GetChild<CHawkItem>(i);
		if (hawkItem)
		{
			switch (hawkItem->GetItem_type())
			{
			case HAWKER_ITEM:
				m_itemList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				break;
			case HAWKER_PARTNER:
				m_partnerList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				break;
			case HAWKER_PET:
				m_petList.insert(std::make_pair(hawkItem->GetItem_uid(), hawkItem));
				break;
			}
		}
	}
}

void CHawkGroup::ConstructHawkItem(NetGroup* itemGroup, NetGroup* partnerGroup, UInt16Vector partnerSkillID, NetGroup* petGroup, UInt16Vector petSkillID)
{

}

RPCResult CHawkGroup::CB_CloseHawkForUser(LPCPEER pPeer, RPCContext &context)
{
	 
	if (gGlobal.m_pSimClient->m_pTargetHawkGroup)
	{
		CHawkGroup* temp = gGlobal.m_pSimClient->m_pTargetHawkGroup;
		gGlobal.m_pSimClient->m_pTargetHawkGroup = NULL;
		TRACE_INFODTL(GLOBAL_LOGGER, _F("You have successfully stop dealing with a hawker"));	
		DeleteNetGroup(temp, BOOLEAN_TRUE);
	}

	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_ChangeHawkName(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_UpdateHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType)
{
			
		CHawkItem* item = NULL;
		switch(itemType)
		{
		case HAWKER_ITEM:
			{
				HawkItemMap::iterator itr = m_itemList.find(itemID);
				if (itr != m_itemList.end())
				{			
					UInt8 remainStack = itr->second->GetItem_stack();// will be deleted after testing
					if (itr->second->GetItem_stack() == 0)
					{
						item = itr->second;
						m_itemList.erase(itr);
						Remove(item);
						DeleteNetObject(item);
					}
	
				}
			}
			break;
		case HAWKER_PARTNER:
			{
				HawkItemMap::iterator itr = m_partnerList.find(itemID);
				if (itr != m_partnerList.end())
				{
					if (itr->second->GetItem_stack() == 0)
					{
						item = itr->second;
						m_partnerList.erase(itr);
						Remove(item);
						DeleteNetObject(item);
					}
				}
			}
			break;
		case HAWKER_PET:
			{
				HawkItemMap::iterator itr = m_petList.find(itemID);
				if (itr != m_petList.end())
				{
					if (itr->second->GetItem_stack() == 0)
					{
						item = itr->second;
						m_petList.erase(itr);
						Remove(item);
						DeleteNetObject(item);
					}

				}
			}
			break;
		}
		TRACE_INFODTL(GLOBAL_LOGGER, _F("You have successfully purchased a thing from hawker"));

		if (gGlobal.m_pSimClient == NULL)
			return RPC_RESULT_OK;	
		if (gGlobal.m_pSimClient->m_bRobot)
			CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "StopShopOverHawker", String());	
				
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_StopHawk(LPCPEER pPeer, RPCContext &context)
{
	// delete the hawkgroup from myself 
	if (gGlobal.m_pSimClient->m_pMyHawkGroup)
	{
		CHawkGroup *temp = gGlobal.m_pSimClient->m_pMyHawkGroup;
		gGlobal.m_pSimClient->m_pMyHawkGroup = NULL;
		TRACE_INFODTL(GLOBAL_LOGGER, _F("You have successfully close your hawking business"));
		DeleteNetGroup(temp, BOOLEAN_TRUE);
	}


	return RPC_RESULT_OK;
}

UInt16Vector CHawkGroup::GetPartnerSkillList(UInt32 partnerID)
{

	UInt16Vector dummy;
	return dummy;
}

UInt16Vector CHawkGroup::GetPetSkillList(UInt32 petID)
{

	UInt16Vector dummy;
	return dummy;
}

RPCResult CHawkGroup::CB_BuyHawkItemSuccess(LPCPEER pPeer, RPCContext &context, const Boolean &owner, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack, const UInt32 &tax)
{
	return RPC_RESULT_OK;
}

RPCResult CHawkGroup::CB_AddSellStartHawkItemInfo(LPCPEER pPeer, RPCContext &context,NetGroup* addHawkItem)
{
	return RPC_RESULT_OK;
}