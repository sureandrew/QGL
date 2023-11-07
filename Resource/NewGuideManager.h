#pragma once
#ifndef _NEWGUIDE_MANAGER_H_
#define _NEWGUIDE_MANAGER_H_

struct NewGuideData;
struct NewGuideMap;

class CNewGuideManager
{
public:
	typedef StlMap<UInt16, NewGuideData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, NewGuideData*> > > NewGuideMap;

	CNewGuideManager();
	~CNewGuideManager();

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

	const NewGuideData* GetNewGuideData(UInt16 NewGuide_id);
private:
	NewGuideMap m_NewGuideMap;
};

#endif // _NEWGUIDE_MANAGER_H_
