#pragma once
#ifndef _NEWBIETIPS_MANAGER_H_
#define _NEWBIETIPS_MANAGER_H_

struct NewbieTipsData;
struct NewbieTipsMap;

class CNewbieTipsManager
{
public:
	typedef StlMap<UInt16, NewbieTipsData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, NewbieTipsData*> > > NewbieTipsMap;

	CNewbieTipsManager();
	~CNewbieTipsManager();

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

	const NewbieTipsData* GetNewbieTipsData(UInt16 NewbieTips_id);
private:

	NewbieTipsMap m_NewbieTips;
};

#endif // _SKILL_MANAGER_H_
