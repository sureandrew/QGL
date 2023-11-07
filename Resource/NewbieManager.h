#pragma once
#ifndef _NEWBIE_MANAGER_H_
#define _NEWBIE_MANAGER_H_

struct NewbieData;
struct NewBieMap;

class CNewBieManager
{
public:
	typedef StlMap<UInt16, NewbieData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, NewbieData*> > > NewbieMap;

	CNewBieManager();
	~CNewBieManager();

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

	const NewbieData* GetNewBieData(UInt16 NewBie_id);
private:

	NewbieMap m_newbies;
};

#endif // _SKILL_MANAGER_H_
