#ifndef _PARTY_CHAR_INFO_H_
#define _PARTY_CHAR_INFO_H_

#include "Character.h"
#include "Stub/Logic/PartyCharInfo_stub.h"

class CPartyCharInfo : public PartyCharInfo_Stub
{
public:
	DEFINE_CLASS(PartyCharInfo_CLASSID);
public:
	CPartyCharInfo() {};
	~CPartyCharInfo() {};
public:
	void SetData(const CCharacter* pChar);
};

#endif