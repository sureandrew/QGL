#pragma once
#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include "Common.h"

class CFileManager
{
public:
	CFileManager();
	~CFileManager();

	BYTE* Load(String filename, UInt64 & size);
	BOOL Free(String filename);

private:
	std::map<String, BYTE*> m_fileDataList;
	std::map<String, UInt64> m_fileSizeList;
	std::map<String, UInt32> m_reservedSizeList;
};

#endif //_FILE_MANAGER_H_