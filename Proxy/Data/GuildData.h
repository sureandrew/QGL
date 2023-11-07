#pragma once
#ifndef _GUILDDATA_H_
#define _GUILDDATA_H_

#include "Proxy/Data/GuildData_Proxy.h"

class CGuildData : public GuildData_Proxy
{
private:
	typedef GuildData_Proxy Parent;

public:
	DEFINE_CLASS(GuildData_CLASSID);
// OnUpdate can be implemented selectively
	static CGuildData * NewInstance();
	
};

inline CGuildData * CGuildData::NewInstance()
{
	return CreateNetObject<CGuildData>();
}
#endif //_GUILDDATA_H_
