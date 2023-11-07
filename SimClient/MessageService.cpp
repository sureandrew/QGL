//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
//-- Self
#include "SimClient/MessageService.h"
//-- Library
#include "SimClient/Character.h"
#include "SimClient/Map.h"
#include "SimClient/MessageCharacter.h"
#include "SimClient/Messenger.h"
#include "SimClient/Global.h"
#include "SimClient/Channel.h"
#include "SimClient/Guild.h"
#include "SimClient/GuildMember.h"
#include "SimClient/GuildApplicant.h"

// private function and namespace global variable here
namespace MessageService_Proxy
{
};

RPCResult MessageService_Proxy::CB_RequestApplicant(LPCPEER pPeer, RPCContext &context, 
													NetGroup* grp)
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_ownGuild)
	{
		gGlobal.m_pSimClient->m_ownGuild->RequestApplicant(grp);
		DeleteNetGroup(grp);
	}
	else
	{
		DeleteNetGroup(grp, BOOLEAN_TRUE);
	}

	return RPC_RESULT_OK; 
}

void MessageService_Proxy::AddFriend(UInt32 targetId, UInt8 group, PCSTRING targetName)
{
	CSimClient *pSimClient = gGlobal.m_pSimClient;
	CCharacter *pChar = gGlobal.m_pSimClient->m_pCharacter;
	UInt16Vector relations;

	relations.push_back(FRIEND_RELATION_FRIEND);	
	MessageService_Proxy::CreateMessenger(pSimClient->m_pProxyPeer, pSimClient->m_context,
		pChar->GetChar_id(), targetId, relations, group, 0, targetName);
	int friendNum = 1 + gGlobal.m_pSimClient->m_pMessengerGroup->GetCount();
	if (gGlobal.m_pSimClient->m_pMessengerGroup)
		TRACE_INFODTL_3(GLOBAL_LOGGER, 
			_T("AddFriend: added a friend, char_id %d, name %s, total friend %d"), 
			targetId, targetName, friendNum);
}

void MessageService_Proxy::RemoveFriend(UInt32 targetId)
{
	if (gGlobal.m_pSimClient->m_pMessengerGroup == NULL)
		return;

	// check if the given ID is matched to any ID currently on the friend list
	bool found = false;
	CMessenger* pMessenger;
	UInt16 msgCount = gGlobal.m_pSimClient->m_pMessengerGroup->GetCount();
	for (UInt16 i = 0; i < msgCount; ++i)
	{
		pMessenger = gGlobal.m_pSimClient->m_pMessengerGroup->GetChild<CMessenger>(i);
		// if the ID is found 
		if (pMessenger && (pMessenger->GetFriend_char_id() == targetId || targetId == 0))
		{
			targetId = pMessenger->GetFriend_char_id();	
			//pMessenger->RemoveMessenger(gGlobal.m_pSimClient->m_pProxyPeer, 
			//	gGlobal.m_pSimClient->m_context);
			MessageService_Proxy::RemoveMessenger(gGlobal.m_pSimClient->m_pProxyPeer, 
				gGlobal.m_pSimClient->m_context, pMessenger->GetPlayer_char_id(),
				pMessenger->GetFriend_char_id());
			gGlobal.m_pSimClient->m_pMessengerGroup->Remove(pMessenger);
			DeleteNetObject(pMessenger);	
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_T("RemoveFriend: removed char_id %d, remain friends %d"), 
				targetId, msgCount - 1);
			found = true;
			break;
		}
	}

	if (!found)		// if the ID cannot be found on the friend list
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("RemoveFriend Warning: character not found, char_id %d"), targetId);
}

void MessageService_Proxy::ChangeGroup(UInt16 targetId, UInt8 group)
{
	NetGroup *pMsgGrp = gGlobal.m_pSimClient->m_pMessengerGroup;
	if (targetId == 0 && pMsgGrp && pMsgGrp->GetCount() > 0)
	{
		UInt16 index = (targetId == 0)? GetRand(pMsgGrp->GetCount()) : targetId;
		CMessenger *pMessenger = pMsgGrp->GetChild<CMessenger>(index);
		if (pMessenger)
			pMessenger->MoveGroup(gGlobal.m_pSimClient->m_pProxyPeer, gGlobal.m_pSimClient->m_context, group);
	}

}

void MessageService_Proxy::SendFriendMsg(UInt32 targetId, PCSTRING msg, bool bPrivate)
{
	NetGroup *pMsgGrp = gGlobal.m_pSimClient->m_pMessengerGroup;
	String targetName = _T("anyone");
	if (targetId == 0 && pMsgGrp && pMsgGrp->GetCount() > 0)
	{
		UInt16 index = GetRand(pMsgGrp->GetCount());
		CMessenger *pMessenger = pMsgGrp->GetChild<CMessenger>(index);
		if (pMessenger)
            targetId = pMessenger->GetFriend_char_id();
	}
 
	if (targetId != 0)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER,_F("SendFriendMsg: char_id %d, private %d, %s"),
			targetId, bPrivate, msg);		
		MessageService_Proxy::SendPrivateMessage(gGlobal.m_pSimClient->m_pProxyPeer, 
			gGlobal.m_pSimClient->m_context, msg, 
			gGlobal.m_pSimClient->m_pCharacter->GetChar_id(), targetId, 
			targetName, bPrivate);
	}
		else
			TRACE_INFODTL_1(GLOBAL_LOGGER,_F("SendFriendMsg: friend not found, char_id %d"), targetId);	
}

void MessageService_Proxy::ShowFriends()
{
	CMessenger* pMessenger;
	UInt16 msgCount = gGlobal.m_pSimClient->m_pMessengerGroup->GetCount();
	CMap* pMap = gGlobal.m_pSimClient->m_pMap;
	PCSTRING friendName;
	bool nearby;
	UInt32 x, y;

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ShowFriends: total %d"), msgCount);
	for (UInt16 i = 0; i < msgCount; ++i)
	{
		pMessenger = gGlobal.m_pSimClient->m_pMessengerGroup->GetChild<CMessenger>(i);
		if (pMessenger == NULL)
			continue;
        UInt32 char_id = pMessenger->GetFriend_char_id();
		CCharacter *pFriend = pMap->FindCharacter(char_id);
		if (pFriend) // if the friend is currently on the same map as the player
		{
			friendName = pFriend->GetNickName().c_str();
			x = pFriend->GetCurPosX();
			y = pFriend->GetCurPosY();
			nearby = true;
		}
		else // if the friend is not on the same map or offline
		{
			friendName = NULL;
			x = 0;
			y = 0;
			nearby = false;
		}
		// output the friend information
		TRACE_INFODTL_6(GLOBAL_LOGGER, 
			_T("Friend %d:char_id %d, group %d, nearby %d, pos %d,%d"), i,
			pMessenger->GetFriend_char_id(), pMessenger->GetFriend_group(), nearby, x, y);
	}

}

RPCResult MessageService_Proxy::SearchFriend(RPCContext &context)
{
	// search if the friend is currently online, action will be taken in callback function
	PROCEDURE_START_FUNC2(MessageService_Proxy::SearchFriend, context, UInt8, group, 
							String, targetName)

		MessageService_Proxy::RequestInfo(gGlobal.m_pSimClient->m_pProxyPeer, 
			gGlobal.m_pSimClient->m_context, 0, targetName, FALSE, true);

	PROCEDURE_WAIT2(1, MessageService, CB_RequestInfo, NetObject*, msgCharData, bool, update)
		TRACE_ENSURE(msgCharData);
		CMessageCharacter* pMsgChar = CastNetObject<CMessageCharacter>(msgCharData);

		if (pMsgChar)
			AddFriend(pMsgChar->GetChar_id(), group, targetName.c_str());
		DeleteNetObject(msgCharData);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(MessageService, CB_Error, UInt32, err, String, errMsg)
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("SearchFriend Warning: %s"), errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetAvailService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveMessageChannel(LPCPEER pPeer, RPCContext &context, 
														 NetObject * channel, const BYTE& cType)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_ReceiveMessageChannel: channel %d,%d, type %d\n"), 
		channel->GetNetId().srvId, channel->GetNetId().objId, cType);

	if (cType >= MAX_CHANNELTYPE)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CB_ReceiveMessageChannel Error: invalid channel type %d"), 
			cType);
	};

	CChannel* pChannel = CastNetObject<CChannel>(channel);
	
	if (pChannel == NULL)
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CB_ReceiveMessageChannel: invalid channel"));
		return RPC_RESULT_FAIL;
	}

	TRACE_ENSURE(gGlobal.m_pSimClient);

	if (gGlobal.m_pSimClient->m_pChannel[cType] && 
		gGlobal.m_pSimClient->m_pChannel[cType] != pChannel)
	{
		DeleteNetObject(gGlobal.m_pSimClient->m_pChannel[cType]);
		gGlobal.m_pSimClient->m_pChannel[cType] = NULL;
	}

	gGlobal.m_pSimClient->m_pChannel[cType] = pChannel;

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SendHorseLightMessage(LPCPEER pPeer, RPCContext &context,  const BYTE &Result)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveSystemMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const SessionIDVector &listeners)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_CreateGuildMsg(LPCPEER pPeer, RPCContext &context, const String &guildName, const String &creator , const SessionIDVector &listeners)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceivePrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, 
														 const String &SrcName, const UInt32 &SrcChar_id, 
														 const String &TargetName, const UInt32 &TargetChar_id, const boolean &isFriend)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &count, const SessionIDVector &listeners)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ReceiveHorseLightMessage: count %d"), 
		count);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveRumorMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const BYTE & SubType, const SessionIDVector &listeners)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ReceiveRumorMessage: subType %d"), SubType);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SendSystemMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	return RPC_RESULT_OK;
}
RPCResult MessageService_Proxy::CB_SendRumorMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	return RPC_RESULT_OK;
}


RPCResult MessageService_Proxy::CB_SendPrivateMessage(LPCPEER pPeer, RPCContext &context,  const BYTE &Result, const boolean &isFriend, const UInt32 &resultFlag)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_InitMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SetListen(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_JoinMessageChannel:"));

	return RPC_RESULT_OK;
}

RPCResult  MessageService_Proxy::CB_UpdateMessengers(LPCPEER pPeer, RPCContext &context, NetGroup* messengers)
{
	if (gGlobal.m_pSimClient)
	{
		// remove old messengers
		if (gGlobal.m_pSimClient->m_pMessengerGroup && 
			gGlobal.m_pSimClient->m_pMessengerGroup != messengers)
		{
			NetObject *obj1, *obj2;
			bool found = false;
			for (UInt16 i = 0; i < gGlobal.m_pSimClient->m_pMessengerGroup->GetCount(); ++i)
			{
				found = false;
				obj1 = gGlobal.m_pSimClient->m_pMessengerGroup->GetChild(i);
				for (UInt16 j = 0; j < messengers->GetCount(); ++j)
				{
					obj2 = messengers->GetChild(j);
					if (obj1 == obj2)
					{
						found = true;
						break;
					}
				}
				if (!found)
					DeleteNetObject(obj1);
			}
			DeleteNetGroup(gGlobal.m_pSimClient->m_pMessengerGroup);	
		}
		gGlobal.m_pSimClient->m_pMessengerGroup = messengers;
	}
	else
		DeleteNetGroup(messengers, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_RemoveMessengers(LPCPEER pPeer, RPCContext &context, 
													const UInt32 &owner_id, const UInt32 &target_id)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_UpdateMsgCharInfo(LPCPEER pPeer, RPCContext &context, NetGroup* magCharInfoGrp)
{
	if (gGlobal.m_pSimClient)
	{
	//	gGlobal.m_pSimClient->m_pMsgCharGroup = magCharInfoGrp;
		if (magCharInfoGrp)
            DeleteNetGroup(magCharInfoGrp, BOOLEAN_TRUE);
	}
	else
		DeleteNetGroup(magCharInfoGrp, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_RequestInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgCharData, const bool &update)
{

	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;	

	DeleteNetObject(msgCharData);	


	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_BattleEndUpdate(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_MessageReturn(LPCPEER pPeer, RPCContext &context, const String &msg, const String &name)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_RemoveMessengersByOwner(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt8 &result)
{
	return RPC_RESULT_OK; 
}

void MessageService_Proxy::CreateGuild(PCSTRING guildName)
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_pCharacter)
		gGlobal.m_pSimClient->m_pCharacter->CreateGuild(gGlobal.m_pSimClient->m_pProxyPeer,
			gGlobal.m_pSimClient->m_context, guildName, false);
	
	TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CreateGuild: create a new guild, name %s"), guildName);
}

void MessageService_Proxy::ApplyGuildMembership(UInt32 guildIndex)
{
	UInt i = 0;
	for (GuildMap::iterator it = gGlobal.m_pSimClient->m_guildMap.begin();
		it != gGlobal.m_pSimClient->m_guildMap.end(); ++it, i++)
	{
		if (i == guildIndex)
		{
			MessageService_Proxy::ApplyGuild(gGlobal.m_pSimClient->m_pProxyPeer, 
				gGlobal.m_pSimClient->m_context, 
				gGlobal.m_pSimClient->m_pCharacter->GetChar_id(), it->second->GetGuild_uid());
		}
	}
}

void MessageService_Proxy::ApprovalGuildMembership(UInt32 applicantIndex)
{
	if (gGlobal.m_pSimClient == NULL ||
		gGlobal.m_pSimClient->m_ownGuildMember == NULL || 
		gGlobal.m_pSimClient->m_ownGuild == NULL)
		return;

	if (gGlobal.m_pSimClient->m_ownGuildMember->GetPost() != GUILD_POSITION_MASTER)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("ApprovalGuildMembership: no right"));
		return;
	}

	gGlobal.m_pSimClient->m_ownGuild->ApprovalMembership(applicantIndex, true);
}

void MessageService_Proxy::RejectGuildMembership(UInt32 applicantIndex)
{
	if (gGlobal.m_pSimClient == NULL ||
		gGlobal.m_pSimClient->m_ownGuildMember == NULL || 
		gGlobal.m_pSimClient->m_ownGuild == NULL)
		return;

	if (gGlobal.m_pSimClient->m_ownGuildMember->GetPost() != GUILD_POSITION_MASTER)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("RejectGuildMembership: no right"));
		return;
	}

	gGlobal.m_pSimClient->m_ownGuild->ApprovalMembership(applicantIndex, false);
}

void MessageService_Proxy::KickGuildMember(UInt32 memberIndex)
{
	if (gGlobal.m_pSimClient == NULL ||
		gGlobal.m_pSimClient->m_ownGuildMember == NULL || 
		gGlobal.m_pSimClient->m_ownGuild == NULL)
		return;

	if (gGlobal.m_pSimClient->m_ownGuildMember->GetPost() != GUILD_POSITION_MASTER)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("KickGuildMember: no right"));
		return;
	}

	gGlobal.m_pSimClient->m_ownGuild->KickMember(memberIndex);
}

void MessageService_Proxy::ChangeGuildPost(UInt32 memberIndex, UInt32 postIndex)
{
	if (gGlobal.m_pSimClient == NULL ||
		gGlobal.m_pSimClient->m_ownGuildMember == NULL || 
		gGlobal.m_pSimClient->m_ownGuild == NULL)
		return;

	if ((gGlobal.m_pSimClient->m_ownGuildMember->GetPost() > GUILD_POSITION_DEPTHEAD) ||
		(gGlobal.m_pSimClient->m_ownGuildMember->GetPost() > postIndex))
	{
		TRACE_INFODTL(GLOBAL_LOGGER,_F("ChangeGuildPost: no right"));
		return;
	}

	gGlobal.m_pSimClient->m_ownGuild->ChangeGuildPost(memberIndex, postIndex);
}

void MessageService_Proxy::LeaveGuild()
{
	if (gGlobal.m_pSimClient->m_pCharacter)
		gGlobal.m_pSimClient->m_pCharacter->KickMember(
			gGlobal.m_pSimClient->m_pProxyPeer,
			gGlobal.m_pSimClient->m_context, 
			gGlobal.m_pSimClient->m_pCharacter->GetChar_id(), String());
}

void MessageService_Proxy::SetGuildAim(String msg)
{
	if (gGlobal.m_pSimClient->m_ownGuildMember == NULL)
		return;

	if (gGlobal.m_pSimClient->m_ownGuildMember->GetPost() > GUILD_POSITION_SECMASTER)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("SetGuildAim: no right"));
		return;
	}
	gGlobal.m_pSimClient->m_ownGuildMember->ChangeAim(gGlobal.m_pSimClient->m_pProxyPeer, 
		gGlobal.m_pSimClient->m_context, msg);
}

void MessageService_Proxy::SetGuildRule(String msg)
{
	if (gGlobal.m_pSimClient->m_ownGuildMember == NULL)
		return;

	if (gGlobal.m_pSimClient->m_ownGuildMember->GetPost() > GUILD_POSITION_SECMASTER)
	{
		TRACE_INFODTL(GLOBAL_LOGGER,_F("SetGuildRule: no right"));
		return;
	}
	gGlobal.m_pSimClient->m_ownGuildMember->ChangeRule(gGlobal.m_pSimClient->m_pProxyPeer, 
		gGlobal.m_pSimClient->m_context, msg);
}


RPCResult MessageService_Proxy::CB_Guilds(LPCPEER pPeer, RPCContext &context, 
										  NetGroup* GuildGrp, const bool &openApply)
{
	for (UInt16 i = 0; i < GuildGrp->GetCount(); i ++)
	{
		CGuild * pGuild = CastNetObject<CGuild>(GuildGrp->GetChild(i));
		if (pGuild)
		{
			GuildMap::iterator it = gGlobal.m_pSimClient->m_guildMap.find(pGuild->GetGuild_uid());
			if (it != gGlobal.m_pSimClient->m_guildMap.end())
			{
				if (it->second != pGuild)
				{
					DeleteNetObject(it->second);
					gGlobal.m_pSimClient->m_guildMap.erase(it);
				}
			}	
			gGlobal.m_pSimClient->m_guildMap[pGuild->GetGuild_uid()] = pGuild;
			if (!openApply)
				gGlobal.m_pSimClient->m_ownGuild = pGuild;
		}
	}
	DeleteNetGroup(GuildGrp);

	return RPC_RESULT_OK; 
}

RPCResult MessageService_Proxy::CB_GuildMembers(LPCPEER pPeer, RPCContext &context, 
												NetGroup* members)
{
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_ownGuild)
		gGlobal.m_pSimClient->m_ownGuild->SetMembers(members);
	else
		DeleteNetGroup(members, BOOLEAN_TRUE);

	return RPC_RESULT_OK; 
}

RPCResult MessageService_Proxy::CB_SendShutdownMessage(LPCPEER pPeer, RPCContext &context, 
													   const UInt32 &downTime)
{
	gGlobal.m_downTime = downTime;
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &addedGDonate, const UInt8 &returnFlag, const UInt32 &addedGMoney)
{
	return RPC_RESULT_OK; 
}

RPCResult MessageService_Proxy::CB_ApplyGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid)
{
	CGuild *pGuild = NULL;	
	GuildMap::iterator it = gGlobal.m_pSimClient->m_guildMap.find(guild_uid);
	if (it != gGlobal.m_pSimClient->m_guildMap.end())
	{
		pGuild = it->second;
		if (pGuild)
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ApplyGuild: waiting guild %d for approval"),
			pGuild->GetGuild_uid()); 
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const Boolean &canChange)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32Vector &messgers)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetMessengersName(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const StringVector &messgers)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &friendly)
{
	return RPC_RESULT_OK;
}
RPCResult MessageService_Proxy::CB_IsFriend(LPCPEER pPeer, RPCContext &context, const UInt32 &isFriend)
{
	return RPC_RESULT_OK;
}
RPCResult MessageService_Proxy::CB_GetTargetFromHawk(LPCPEER pPeer, RPCContext &context,const bool& isonline,const String& char_name)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt16 &finalCount, const UInt32 &finalPrice)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt32 &finalPrice)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, const Boolean &sell, const UInt16 &shop_id, const UInt8 &shopName, const UInt32 &SynTime, const UInt32 &BusinessMoney, const BusinessShopItemInfoVector &ItemInfoList)
{
#ifdef QGL
#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const BusinessShopItemInfoVector &ItemInfoList)
{
#ifdef QGL
#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetTargetInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgTargetData)
{
	DeleteNetObject(msgTargetData);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CItem &item)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPartner &partner, const UInt16Vector &skillIDs)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPet &pet, const UInt16Vector &skillIDs)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveFriendMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ReceiveMessage from %s: %s"), SrcName.c_str(), Msg.c_str());
#ifdef QGL
	//gGlobal.NotifyReceiveMessage(pPeer, context, Msg, SrcName);
	gGlobal.PrintMessage(Msg,CHANNELTYPE_FRIEND, SrcName);
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ChangeFriendly(LPCPEER pPeer, RPCContext &context)
{	
	return RPC_RESULT_OK;
}