#pragma once
#ifndef _BIGMAP_MANAGER_H_
#define _BIGMAP_MANAGER_H_

struct BigMapData;
struct BigMapBelongData;

class CBigMapManager
{
public:
	CBigMapManager();
	~CBigMapManager();

	const BigMapData* GetBigMap(UInt16 map_id);
	const BigMapData* GetBigMapByIndex(UInt32 index);
	const BigMapBelongData* GetBigMapBelong(UInt16 map_id);
	UInt32 GetMapCount();

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	BOOL loadBelongCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	void Release();

private:
	typedef StlMap<UInt16, BigMapBelongData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BigMapBelongData*> > > BigMapBelongDataMap;
	typedef StlMap<UInt16, BigMapData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, BigMapData*> > > BigMapDataMap;
	typedef StlVector<BigMapData*, ManagedAllocator<BigMapData*> > BigMapVector;

	BigMapDataMap	m_BigMap;
	BigMapVector	m_BigMapVector;
	BigMapBelongDataMap	m_BigMapBelong;
};

#endif // _BIGMAP_MANAGER_H_
