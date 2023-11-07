#pragma once
#ifndef _RANKCHARACTER_H_
#define _RANKCHARACTER_H_

#include "Stub/Logic/RankCharacter_Stub.h"

class CCharacterData;
class CRankCharacter : public RankCharacter_Stub
{
public:
	DEFINE_CLASS(RankCharacter_CLASSID);
	inline static CRankCharacter* NewInstance();
	void SetData(CCharacterData *pCharData);	// set character by CCharacterData
	CRankCharacter();
	virtual ~CRankCharacter();
	void PreSend();

private:
	typedef RankCharacter_Stub Parent;
};

inline CRankCharacter* CRankCharacter::NewInstance()
{
	return CreateNetObject<CRankCharacter>();
}

#endif //_RANKCHARACTER_H_
