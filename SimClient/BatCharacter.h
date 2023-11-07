#pragma once
#ifndef _BATCHARACTER_H_
#define _BATCHARACTER_H_

#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/BatCharacter_Proxy.h"

class CBatCharacter : public BatCharacter_Proxy
{
public:
	DEFINE_CLASS(BatCharacter_CLASSID);
	void ShowDetail();

private:
	typedef BatCharacter_Proxy Parent;
};

#endif //_BATCHARACTER_H_
