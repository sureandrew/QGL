#pragma once
#ifndef _GUILDAPPLICANT_H_
#define _GUILDAPPLICANT_H_

#include "Proxy/Logic/GuildApplicant_Proxy.h"

class CGuildApplicant : public GuildApplicant_Proxy
{
public:
	DEFINE_CLASS(GuildApplicant_CLASSID);

private:
	typedef GuildApplicant_Proxy Parent;

};

#endif //_GUILDAPPLICANT_H_
