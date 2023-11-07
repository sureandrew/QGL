#pragma once
#ifndef _GUILDMEMBERDATA_H_
#define _GUILDMEMBERDATA_H_

#include "Proxy/Data/GuildMemberData_Proxy.h"

class CGuildMemberData : public GuildMemberData_Proxy
{
private:
	typedef GuildMemberData_Proxy Parent;

public:
	DEFINE_CLASS(GuildMemberData_CLASSID);
// OnUpdate can be implemented selectively
	static CGuildMemberData * NewInstance();
};


inline CGuildMemberData * CGuildMemberData::NewInstance()
{
	return CreateNetObject<CGuildMemberData>();
}
#endif //_GUILDMEMBERDATA_H_
