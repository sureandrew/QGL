#pragma once
#ifndef _FACTION_MANAGER_H_
#define _FACTION_MANAGER_H_

struct FactionData;


class CFactionManager
{
public:
	CFactionManager();
	~CFactionManager();

private:

public:
	typedef StlMap<UInt16, FactionData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, FactionData*> > > FactionMap;

	const FactionData* GetFaction(UInt16 Faction_id);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:

	FactionMap m_Factions;

};

#endif // _SKILL_MANAGER_H_
