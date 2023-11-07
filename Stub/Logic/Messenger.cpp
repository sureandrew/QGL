#include "Common.h"
#include "Common/Procedure.h"
#include "MessageService/MessageApplication.h"
#include "Stub/Logic/Messenger.h"
#include "Stub/Logic/MessageCharacter.h"
#include "Proxy/Data/MessengerData.h"
#include "MessageService/Global.h"
#include "Stub/Service/MessageService.h"
#include "Proxy/Data/MessengerData.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService.h"

REGISTER_CLASSTYPE(CMessenger, Messenger_CLASSID);

CMessenger::CMessenger()
{
}

CMessenger::~CMessenger()
{
	if (GetFriend_char_id() != 0) 
		gGlobal.ReleaseMsgCharRef(GetFriend_char_id());
}

void CMessenger::SetData(CMessengerData * pMData)
{
	if (pMData == NULL)
		return;
	if (pMData->IsUpdated(CMessengerData::keyMask))
		SetKey(pMData->GetPlayer_char_id(), pMData->GetFriend_char_id());
	if (pMData->IsUpdated(CMessengerData::relationMask))
	{
		SetRelation(pMData->GetFriend_relation());
		//TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CMessenger::SetData SetRelation %d"),pMData->GetFriend_relation());
	}
	if (pMData->IsUpdated(CMessengerData::groupMask))
	{
		SetGroup(pMData->GetFriend_group());
	}

	if (pMData->IsUpdated(CMessengerData::friendlyMask))
	{
		SetFriendly(pMData->GetFriend_friendly());
	}

	ClearUpdateMask();
}

void CMessenger::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

void CMessenger::WriteData(CMessengerData * pMData)
{
	if (pMData == NULL)
		return;

	pMData->ClearUpdateMask();

	if (IsUpdated(keyMask))
		pMData->SetKey(GetPlayer_char_id(), GetFriend_char_id());

	if (IsUpdated(relationMask))
		pMData->SetRelation(GetFriend_relation());

	//always mark update (no need now)
	if (IsUpdated(groupMask))
		pMData->SetGroup(GetFriend_group());

	if (IsUpdated(friendlyMask))
		pMData->SetFriendly(GetFriend_friendly());
}

void CMessenger::UpdateToObject()
{
	CMessengerData Data;
	WriteData(&Data);

	if (Data.IsUpdated())
	{
		Data.SetSendMask(Data.GetUpdateMask());
		ObjectService_Proxy::UpdateMessenger(gGlobal.pObjectService->pPeer, RPCContext(), GetPlayer_char_id(), GetFriend_char_id(), &Data);
		ClearUpdateMask();
	}
}

RPCResult CMessenger::MoveGroup(LPCPEER pPeer, RPCContext &context, const UInt8 &group)
{
	if (group >= MAX_FRIEND_GROUP_TYPE)
		return RPC_RESULT_OK;

	if ((group == FRIEND_GROUP_TEMP || group == FRIEND_GROUP_BLACK) && 
		(GetFriend_relation() != 0 && GetFriend_relation() != (1 << FRIEND_RELATION_FRIEND)))
		return RPC_RESULT_OK;

	if (group == FRIEND_GROUP_TEMP)
	{
		return MessageService_Stub::RemoveMessenger(pPeer, context, GetPlayer_char_id(),
			GetFriend_char_id());
	}

	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(GetPlayer_char_id());
	if (pMsgInfo)
	{
		if ((group == FRIEND_GROUP_TEMP && pMsgInfo->messengerCount[group] >= MAX_MEMBER_EACH_GROUP) ||
			(group != FRIEND_GROUP_TEMP && pMsgInfo->messengerCount[group] >= MAX_MEMBER_EACH_GROUP))
		{
			MessageService_Stub::CB_Error(pPeer, context, 0 , _T("MSG_ERROR_MOVE_GROUP_FULL"));
			return RPC_RESULT_OK;
		}

		bool addFriend = false;
		if (group != FRIEND_GROUP_TEMP && 
			(GetFriend_group() == FRIEND_GROUP_TEMP || GetFriend_group() == FRIEND_GROUP_BLACK))
		{
			UInt relation = 0;
			//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("MoveGroup: relation address 0x%08x Msg: %s"), relation.c_str(), relation.c_str());
			if (group != FRIEND_GROUP_BLACK)
			{
				CharMsgInfo* pSubMsgInfo = gGlobal.GetMsgInfo(GetFriend_char_id());
				if (pSubMsgInfo)
				{
					if (pSubMsgInfo->pMsgChar->GetEnableBlockMakeFriend())
					{
						MessageService_Stub::CB_Error(pPeer, context, 0, _T("MSG_BLOCK_FRIEND"));
						return RPC_RESULT_OK;
					}
				}
				else
				{
					MessageService_Stub::CB_Error(pPeer, context, 0, 
						_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_FOUND_TARGET"));
					return RPC_RESULT_OK;
				}
				relation = 1;
			}
			SetRelation(relation);
			SetFriendly(0);
			addFriend = true;
		}
		else if (group == FRIEND_GROUP_TEMP)
		{
			SetRelation(0);
			SetFriendly(0);
		}
		else if (group == GetFriend_group())
		{
			return RPC_RESULT_FAIL;
		}

		if (GetFriend_group() < MAX_FRIEND_GROUP_TYPE)
		{
			if (pMsgInfo->messengerCount[GetFriend_group()] > 0)
				pMsgInfo->messengerCount[GetFriend_group()]--;
		}
		pMsgInfo->messengerCount[group]++;
		SetGroup(group);
		SetSendMask(GetUpdateMask());
		UpdateToObject();
		if (pMsgInfo->pProxyPeer) 
		{
			MessageService_Stub::CB_UpdateMessengers(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), pMsgInfo->messengers);

			if (addFriend && group != FRIEND_GROUP_TEMP && group != FRIEND_GROUP_BLACK)
			{
				CharMsgInfo* pSubMsgInfo = gGlobal.GetMsgInfo(GetFriend_char_id());
				if (pSubMsgInfo)
				{
					MessageService_Stub::CB_MessageReturn(pMsgInfo->pProxyPeer, 
						RPCContext(pMsgInfo->SID), _T("MSG_FRIEND_ADD_SUCCESS"), 
						pSubMsgInfo->pMsgChar->GetNickName());
					MessageService_Stub::CB_MessageReturn(pSubMsgInfo->pProxyPeer, 
						RPCContext(pSubMsgInfo->SID), _T("MSG_FRIEND_IS_ADDED_SUCCESS"), 
						pMsgInfo->pMsgChar->GetNickName());
				}
			}
		}
	}
	
	return RPC_RESULT_OK;
}

bool CMessenger::IsRelation(UInt16 relMask)
{
	return (GetFriend_relation() & (1 << relMask)) > 0 ;
}

RPCResult CMessenger::ModifyRelation(LPCPEER pPeer, RPCContext &context, const UInt8Vector &relations)
{
	return RPC_RESULT_OK;
}