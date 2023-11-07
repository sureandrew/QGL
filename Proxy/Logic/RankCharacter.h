#pragma once
#ifndef _RANKCHARACTER_H_
#define _RANKCHARACTER_H_

#include "Proxy/Logic/RankCharacter_Proxy.h"

class CRankCharacter : public RankCharacter_Proxy
{
private:
	typedef RankCharacter_Proxy Parent;
// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &char_id);
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction);
	virtual void OnUpdateLevel(const UInt16 &level);
public:
	DEFINE_CLASS(RankCharacter_CLASSID);
};

#endif //_RANKCHARACTER_H_
