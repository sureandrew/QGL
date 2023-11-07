#pragma once
#ifndef _GUILDAPPLICANT_H_
#define _GUILDAPPLICANT_H_

#include "Proxy/Logic/GuildApplicant_Proxy.h"

class CGuildApplicant : public GuildApplicant_Proxy
{
private:
	typedef GuildApplicant_Proxy Parent;

public:
	DEFINE_CLASS(GuildApplicant_CLASSID);
// OnUpdate can be implemented selectively
};

#endif //_GUILDAPPLICANT_H_
