#ifndef _PARTY_CHAR_INFO_H_
#define _PARTY_CHAR_INFO_H_

#include "Character.h"
#include "Proxy/Logic/PartyCharInfo_proxy.h"

class CPartyCharInfo : public PartyCharInfo_Proxy
{
public:
	DEFINE_CLASS(PartyCharInfo_CLASSID);
public:
	CPartyCharInfo() {};
	~CPartyCharInfo() {};
};

#endif