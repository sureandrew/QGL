#pragma once
#ifndef _MESSAGESERVICE_H_
#define _MESSAGESERVICE_H_
#include "Proxy/Service/MessageService_Proxy.h"

// public functions here

namespace MessageService_Proxy
{
	void AddFriend(UInt32 targetId, UInt8 group, PCSTRING targetName);
	void RemoveFriend(UInt32 targetId);
	void SendFriendMsg(UInt32 targetId, PCSTRING msg, bool bPrivate);
	void ShowFriends();
	void CreateGuild(PCSTRING guildName);
	void ApplyGuildMembership(UInt32 guildIndex);	
	void ApprovalGuildMembership(UInt32 applicantIndex);
	void RejectGuildMembership(UInt32 applicantIndex);
	void KickGuildMember(UInt32 memberIndex);
	void ChangeGuildPost(UInt32 memberIndex, UInt32 postIndex);
	void LeaveGuild();	
	void SetGuildAim(String msg);
	void SetGuildRule(String msg);
	void ChangeGroup(UInt16 targetId, UInt8 group);	
	RPCResult SearchFriend(RPCContext &context);


	
};

#endif //_MESSAGESERVICE_H_
