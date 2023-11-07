#pragma once
#ifndef _START_CHARACTER_MANAGER_H_
#define _START_CHARACTER_MANAGER_H_

#include "Common.h"
#include "Common/common_binary.h"

struct StartCharData;

class CStartCharManager
{
public:
	CStartCharManager();
	~CStartCharManager();

	const StartCharData* GetStartChar(UInt32 sexClass);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	void Release();

private:
	typedef StlMap<UInt32, StartCharData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, StartCharData*> > > StartCharMap;

	StartCharMap m_StartChar;
};

#endif // _CHAR_MANAGER_H_
