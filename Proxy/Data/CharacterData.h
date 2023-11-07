#pragma once
#ifndef _CHARACTERDATA_H_
#define _CHARACTERDATA_H_

#include "Proxy/Data/CharacterData_Proxy.h"

class CCharacterData : public CharacterData_Proxy
{
public:
	enum DataDetail {
		DETAIL_NONE		= 0,
		DETAIL_INFO,
		DETAIL_LOC,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_MESSAGE,
		DETAIL_BATTLE_SYNC,
		DETAIL_FOLLOW,
		DETAIL_PARTY,
		DETAIL_RANK,
		DETAIL_ALL,
	};

private:
	typedef CharacterData_Proxy Parent;

public:
	DEFINE_CLASS(CharacterData_CLASSID);
	static CCharacterData * NewInstance();
};

inline CCharacterData * CCharacterData::NewInstance()
{
	return CreateNetObject<CCharacterData>();
}

#endif //_CHARACTERDATA_H_
