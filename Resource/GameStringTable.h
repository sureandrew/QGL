#pragma once
#ifndef _GAMESTRINGTABLE_H_
#define _GAMESTRINGTABLE_H_

#include <Reuben\System\Reuben.h>

typedef StlMap<String, String, std::less<String>, ManagedAllocator<std::pair<String, String> > > MapString;

class CGameStringTable
{
public:
	CGameStringTable();
	~CGameStringTable();
	PCSTRING Get(PCSTRING pMsgName);

	VOID Load(String filename);
	VOID LoadFromBuffer(BYTE* pData, UInt32 pDataLength);

	inline void ClearStringTable(){m_stringTable.clear();}

private:
	MapString m_stringTable;
};

#endif // _GAMESTRINGTABLE_H_
