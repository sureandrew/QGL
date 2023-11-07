#pragma once
#ifndef _MOBDESCMANAGER_H_
#define _MOBDESCMANAGER_H_

class CMobDescManager
{
public:
	CMobDescManager(void);
	~CMobDescManager(void);
public:
	VOID Release();
	Boolean LoadCSV(LPCTSTR szfilename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	String GetDesc(UInt32 ref_id);
private:
	typedef StlMap<UInt32, String, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, String> > > MobDescMap;

	MobDescMap	m_MobDescList;
};

#endif // _MOBDESCMANAGER_H_