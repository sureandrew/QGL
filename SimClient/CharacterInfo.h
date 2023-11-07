#pragma once
#ifndef _CHARACTERINFO_H_
#define _CHARACTERINFO_H_

#include "Proxy/Logic/CharacterInfo_Proxy.h"

class CCharacterInfo : public CharacterInfo_Proxy
{
public:
	DEFINE_CLASS(CharacterInfo_CLASSID);

	enum CharStatus {
		ST_LEAVE = 1,
		ST_ENTER,
		ST_DISCONNECT,
		ST_NOT_FOUND,
		ST_NOT_AVAIL,
	};

private:
	typedef CharacterInfo_Proxy Parent;
};

#endif //_CHARACTERINFO_H_
