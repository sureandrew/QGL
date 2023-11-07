#pragma once
#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

struct ItemData;

typedef StlVector<const ItemData*, ManagedAllocator<const ItemData* > > ItemDataList;

class CItemManager
{
public:
	CItemManager();
	~CItemManager();

	const ItemData* GetItem(UInt32 item_id);
	void GetItemList(ItemDataList &itemList, UInt8 filterType = 0, UInt8 item_type = 0);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:
	typedef StlMap<UInt32, ItemData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, ItemData*> > > ItemMap;

	ItemMap m_items;
};

#endif // _ITEM_MANAGER_H_
