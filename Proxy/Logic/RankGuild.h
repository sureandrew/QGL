#pragma once
#ifndef _RANKGUILD_H_
#define _RANKGUILD_H_

#include "Proxy/Logic/RankGuild_Proxy.h"

class CRankGuild : public RankGuild_Proxy
{
public:
	// OnUpdate can be implemented selectively

	DEFINE_CLASS(RankGuild_CLASSID);

private:
	typedef RankGuild_Proxy Parent;

};

#endif //_RANKGUILD_H_
