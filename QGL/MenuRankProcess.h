#pragma once
#ifndef _MENURANKPROCESS_H_
#define _MENURANKPROCESS_H_

#include "Common.h"

class CRankCharacter;

class CMenuRankProcess
{
public:
	CMenuRankProcess();
	~CMenuRankProcess();
	void ConstructData(NetGroup * givenGrp, UInt8 rankType);
	void ConstructDataGuild(NetGroup * givenGrp);
	void Release();
	void RefreshMenu();
	void HideAllValue();
	void OpenMenu();
	void PageUp();
	void PageDown();
	void SetIndex(Int index);
	void RankScroll();
	void FindRank(PCSTRINGW name);
	void SetSelIndex(Int index);
	void SetRankSubType(Int index);
	void ConstructSubData();
	void InitMenu();
	void HideAllCharData();

	inline UInt GetRankSize(){return m_GroupEndIndex - m_GroupStartIndex;}

private:
	//typedef StlMultiMap<UInt32, CRankCharacter *, std::greater_equal<UInt32>, ManagedAllocator<std::pair<UInt32, CRankCharacter *> > > rankMap;
	//rankMap m_CurrDisplayRank;

	NetGroup	*	m_currGrp;

	UInt	m_StartIndex;

	UInt	m_CurRankType;

	UInt	m_CurRankSubType;

	Int	m_SelIndex;

	UInt	m_GroupStartIndex;
	UInt	m_GroupEndIndex;

};


#endif // _MENURANKPROCESS_H_