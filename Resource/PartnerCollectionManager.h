#pragma once
#ifndef _PARTNERCOLLECTION_MANAGER_H_
#define _PARTNERCOLLECTION_MANAGER_H_

struct PartnerCollectionData;
typedef StlMap<UInt32,const PartnerCollectionData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, const PartnerCollectionData*> > > PartnerCollectionDataMap;

class CPartnerCollectionManager
{
public:
	CPartnerCollectionManager();
	~CPartnerCollectionManager();

	UInt16 GetPartnerCollectionBuffID(UInt16 Count, UInt8 Type);
	void GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type);
	void GetPartnerCollectionData(PartnerCollectionDataMap &PartnerCollectionList, UInt8 Type, UInt8 Group);
	const PartnerCollectionData* GetPartnerCollectionData(UInt32 collection_id);
	size_t GetSize(){return m_PartnerCollection.size();};

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:
	typedef StlMap<UInt32, PartnerCollectionData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, PartnerCollectionData*> > > PartnerCollectionMap;

	PartnerCollectionMap m_PartnerCollection;
};

#endif // _PARTNERCOLLECTION_MANAGER_H_
