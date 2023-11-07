#pragma once
#ifndef _RANKGUILD_H_
#define _RANKGUILD_H_

#include "Stub/Logic/RankGuild_Stub.h"

class CGuildData;
class CRankGuild : public RankGuild_Stub
{
public:

	DEFINE_CLASS(RankGuild_CLASSID);
	inline static CRankGuild* NewInstance();
	void PreSend();
	void SetData(CGuildData *pGuildData);

private:
	typedef RankGuild_Stub Parent;

};


inline CRankGuild* CRankGuild::NewInstance()
{
	return CreateNetObject<CRankGuild>();
}
#endif //_RANKGUILD_H_
