#pragma once
#ifndef _GUILDMEMBER_H_
#define _GUILDMEMBER_H_

#include "Proxy/Logic/GuildMember_Proxy.h"

class CGuildMember : public GuildMember_Proxy
{
public:
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	
	virtual RPCResult CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position);

	DEFINE_CLASS(GuildMember_CLASSID);

private:
	typedef GuildMember_Proxy Parent;

};

#endif //_GUILDMEMBER_H_
