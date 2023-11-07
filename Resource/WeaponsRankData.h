#pragma once
#ifndef _WEAPONSRANKDATA_H_
#define _WEAPONSRANKDATA_H_

struct WeaponsRankInfo
{
	String ChiOwnerName;
	String EngOwnerName;
	String WeaponsName;
	String WeaponsDepict;
};

class CWeaponsRankData
{
public:
	CWeaponsRankData();
	~CWeaponsRankData();

	BOOL LoadCSV(const TCHAR* filename,const BYTE* pData = NULL, UInt32 pDataLen = 0);
	BOOL LoadDAT(const TCHAR* filename);
	BOOL SaveDAT(const TCHAR* filename);

	WeaponsRankInfo * GetWeaponsRankData(UInt32 weaponsrank_id);

	VOID Release();

private:
	typedef StlMap<UInt16, WeaponsRankInfo*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, WeaponsRankInfo*> > > WeaponsRankInfoMap;

	WeaponsRankInfoMap m_mapWeaponsRank;
};

#endif // _WEAPONSRANKDATA_H_
