#pragma once
#ifndef _GUILD_H_
#define _GUILD_H_

#include "Proxy/Logic/Guild_Proxy.h"

class CGuildMember;
class CGuildApplicant;

class CGuild : public Guild_Proxy
{
public:
	virtual RPCResult CB_AddMember(LPCPEER pPeer, RPCContext &context, NetObject* pMember);
	virtual RPCResult CB_RemoveMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason);
	virtual RPCResult CB_RemoveApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_AddApplicant(LPCPEER pPeer, RPCContext &context, NetObject* pObj);

	DEFINE_CLASS(Guild_CLASSID);
	virtual ~CGuild();
	void RemoveAllMember();
	void ShowDetail();
	void RequestApplicant(NetGroup* grp);
	void ApprovalMembership(UInt32 applIndex, bool allow);
	void KickMember(UInt32 memberIndex);
	void ChangeGuildPost(UInt32 memberIndex, UInt32 postIndex);
	void SetMembers(NetGroup* grp);
	UInt32 GetMemberCount() { return (UInt32) m_memberMap.size(); }
	UInt32 GetApplicantCount() { return (UInt32) m_applicantMap.size(); }
	// get the guild bid if any
	UInt32 GetGuildBidAmount();	
	// get the guild position	
	UInt32 GetGuildPosition();
	UInt32 GetGuildFund();
	UInt8 GetGuildWarGroup();	

private:
	typedef Guild_Proxy Parent;
	typedef StlMap<UInt32, CGuildMember*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, CGuildMember*> > > GuildMemberMap;
	typedef StlMap<UInt32, CGuildApplicant*, std::less<UInt32>, 
		ManagedAllocator<std::pair<UInt32, CGuildApplicant*> > > GuildApplicantMap;

	GuildMemberMap		m_memberMap;
	GuildApplicantMap	m_applicantMap;
};

#endif //_GUILD_H_
