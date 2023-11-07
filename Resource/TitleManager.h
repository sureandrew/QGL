#pragma once
#ifndef _TITLE_MANAGER_H_
#define _TITLE_MANAGER_H_

struct TitleData;

class CTitleManager
{
public:
	CTitleManager();
	~CTitleManager();

	const TitleData* GetTitle(UInt32 title_id);
	
	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	void Release();

	const TitleData* GetTitleByRank(UInt8	Title_RankType, UInt8	Title_RankSubType, UInt8	Title_StartRank, UInt8	Title_EndRank);
	const TitleData* GetTitleDataByType(UInt8	Title_Type, UInt8	Title_RankSubType, UInt8	Title_StartRank, UInt8	Title_EndRank);

private:
	
	typedef StlMap<UInt16, TitleData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, TitleData*> > > TitleDataMap;
	TitleDataMap	m_Title;
	
};

#endif // _PET_MANAGER_H_