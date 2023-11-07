#pragma once
#ifndef _GUILDAPPLICANT_H_
#define _GUILDAPPLICANT_H_

#include "Stub/Logic/GuildApplicant_Stub.h"

class CMessageCharacter;
class CGuildApplicant : public GuildApplicant_Stub
{
private:
	typedef GuildApplicant_Stub Parent;

public:
	DEFINE_CLASS(GuildApplicant_CLASSID);

	static CGuildApplicant* NewInstance();

	void SetData(CMessageCharacter * pMsgChar);
	void PreSend();
};

INLINE CGuildApplicant* CGuildApplicant::NewInstance()
{
	return CreateNetObject<CGuildApplicant>();
}
#endif //_GUILDAPPLICANT_H_
