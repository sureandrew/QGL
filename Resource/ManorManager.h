#pragma once
#ifndef _MANOR_MANAGER_H_
#define _MANOR_MANAGER_H_

struct ManorData;

class CManorManager
{
public:
	CManorManager(void);
	~CManorManager(void);

	const ManorData* GetManor(UInt8 manor_scale);	

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	void Release();

private:
	
	typedef StlMap<UInt8, ManorData*, std::less<UInt8>, ManagedAllocator<std::pair<UInt8, ManorData*> > > ManorDataMap;
	ManorDataMap	m_Manor;

};


#endif // _MANOR_MANAGER_H_