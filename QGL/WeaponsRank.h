#pragma once
#ifndef _WEAPONSRANK_H_
#define _WEAPONSRANK_H_

#include <vector>

struct WeaponsRankInfo
{
	String ChiOwnerName;
	String EngOwnerName;
	String WeaponsName;
	String WeaponsDepict;
};

class CWeaponsRank
{
public:
	CWeaponsRank();
	~CWeaponsRank();

	void ShowWeaponsRank();
	void SelectWeaponsRank(int index);
	void DBClickWeaponsRank(int index);
	void WeaponsRankScroll(int scroll); //0: up, 1:down, 2: scroll
	void WeaponsRankSearchList(int lang);//0: eng, 1: chinese word

	void AddWeaponsRankList();
	bool FindWeaponsRank(UInt16 index, String keyword, int lang);

	inline void CloseWeaponsRank()
	{
		m_iStartIndex = 0;
	}

private:

	UInt m_iStartIndex;

	//StlVector<WeaponsRankInfo, ManagedAllocator<WeaponsRankInfo> >	m_WeaponsRankInfoLst;

	int m_iSelectedIndex;
};

#endif _WEAPONSRANK_H_