#pragma once
#ifndef _PET_MANAGER_H_
#define _PET_MANAGER_H_

struct PetData;

class CPetManager
{
public:
	CPetManager();
	~CPetManager();

	const PetData* GetPet(UInt32 pet_id);
	
	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0);

	void Release();

private:
	
	typedef StlMap<UInt16, PetData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, PetData*> > > PetDataMap;
	PetDataMap	m_Pet;
	
};

#endif // _PET_MANAGER_H_