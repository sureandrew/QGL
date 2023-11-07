#pragma once
#ifndef _LINEUP_MANAGER_H_
#define _LINEUP_MANAGER_H_

struct LINEUPINFO;

enum ErrorCode_Lineup
{
	LINEUP_OK = 0,
	LINEUP_INVILAD_PARAM,
	LINEUP_FILE_NOT_EXIST,
	LINEUP_FILE_CONTEXT_ERROR,
};

class CLineupManager
{
public:
	CLineupManager() { m_Lineups.clear();}
	~CLineupManager() { Release();}
public:
	int LoadCSV(const TCHAR *szFileName, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	VOID Release();
	LINEUPINFO* GetLineupData(UInt32 uiLineupId);
	size_t GetLineupDataSize();
private:
	typedef StlMap<UInt32, LINEUPINFO*, std::less<UInt32>,ManagedAllocator<std::pair<UInt32, LINEUPINFO*> > > LineupMap;
	LineupMap m_Lineups;
};

#endif