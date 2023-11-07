#pragma once
#ifndef _RELATIONSYSTEMPROCESS_H_
#define _RELATIONSYSTEMPROCESS_H_
#include <Reuben/Utility/Singleton.h>
#include "Common.h"
class CRelationSystem;
class CRelationSystemProcess:public Reuben::Utility::Singleton<CRelationSystemProcess>
{
public:
	CRelationSystemProcess();
	virtual ~CRelationSystemProcess();
	void SetRelationSystem(CRelationSystem* pRelationSystem);
	void OpenSetSiblingTitlePrefixPanel(const String &thirdWord);
	void OpenSetSiblingTitleSuffixPanel(const String &prefix);
	void SetSiblingTitlePrefix();
	void SetSiblingTitleSuffix();
	//marriage
public:
	typedef StlVector<POINT, ManagedAllocator<POINT> > ChooseMarriageTime;
	void ChooseTimeScroll(UInt16 scroll);
	void ShowChooseTime();
	void SelectChooseTime(int index);
	void EstablishChooseTime();
	void OpenSelectMarriageTimePanel();
private:
	void InitTime();
	POINT GetChooseTimeByIndex(UInt16 menuIndex);
private:
	CRelationSystem * m_pRelationSystem;
	UInt32 m_iStartIndex;
	ChooseMarriageTime m_chooseTime;
	UInt16 m_iSelectedIndex;
};


#endif // _RELATIONSYSTEMPROCESS_H_