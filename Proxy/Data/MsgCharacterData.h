#pragma once
#ifndef _MSGCHARACTERDATA_H_
#define _MSGCHARACTERDATA_H_

#include "Proxy/Data/MsgCharacterData_Proxy.h"

class CMsgCharacterData : public MsgCharacterData_Proxy
{
private:
	typedef MsgCharacterData_Proxy Parent;

public:
	DEFINE_CLASS(MsgCharacterData_CLASSID);
// OnUpdate can be implemented selectively
	INLINE static CMsgCharacterData* NewInstance();
};


inline CMsgCharacterData * CMsgCharacterData::NewInstance()
{
	return CreateNetObject<CMsgCharacterData>();
}
#endif //_MSGCHARACTERDATA_H_
