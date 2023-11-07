#pragma once
#ifndef _BLOCKWORD_MANAGER_H_
#define _BLOCKWORD_MANAGER_H_

#include "Common.h"
#include "Common/common_binary.h"

struct BlockWordData;

typedef StlVector<String, ManagedAllocator<String > > BlockWordDataList;

class CBlockWordManager
{
public:
	CBlockWordManager();
	~CBlockWordManager();

	enum BLOCKTYPE
	{
		BLOCK_BOTH = 0,
		BLOCK_MSG_ONLY,
		BLOCK_NAME_ONLY,
	};

	const BlockWordData* GetBlockWordData(UInt32 index);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	VOID GetBlockWordList(BlockWordDataList &blockWordList);

	BlockWordDataList* GetMsgBlockWordList();
	BlockWordDataList* GetNameBlockWordList();

	void Release();

private:
	typedef StlMap<UInt32, BlockWordData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, BlockWordData*> > > BlockWordMap;

	BlockWordMap m_BlockWordMap;
	BlockWordDataList m_BlockNameWordList;
	BlockWordDataList m_BlockMsgWordList;
};

#endif // _BLOCKWORD_MANAGER_H_
