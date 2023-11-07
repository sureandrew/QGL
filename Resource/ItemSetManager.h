#pragma once
#ifndef _ITEMSET_MANAGER_H_
#define _ITEMSET_MANAGER_H_

struct ItemSetData;
typedef StlMap<UInt32,const ItemSetData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, const ItemSetData*> > > ItemSetDataMap;

class CItemSetManager
{
public:
	CItemSetManager();
	~CItemSetManager();

	const ItemSetData* GetItemSetData(UInt32 itemset_id);
	size_t GetSize(){return m_ItemSet.size();};

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:
	typedef StlMap<UInt32, ItemSetData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, ItemSetData*> > > ItemSetMap;

	ItemSetMap m_ItemSet;
};

#endif // _ITEMSET_MANAGER_H_
