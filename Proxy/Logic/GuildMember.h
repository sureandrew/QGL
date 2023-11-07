#pragma once
#ifndef _GUILDMEMBER_H_
#define _GUILDMEMBER_H_

#include "Proxy/Logic/GuildMember_Proxy.h"

class CGuildMember : public GuildMember_Proxy
{
private:
	typedef GuildMember_Proxy Parent;
	UInt8	m_post;

public:
	DEFINE_CLASS(GuildMember_CLASSID);
	CGuildMember();
// OnUpdate can be implemented selectively
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position);
	

	virtual void OnUpdateDonate(const UInt32 &donate);
	virtual void OnUpdateTotalDonate(const UInt32 &totalDonate);
	virtual void OnUpdatePost(const UInt8 &post);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime);
	INLINE bool	IsActive()		{ return (!GetInactive()); }
};

#endif //_GUILDMEMBER_H_
