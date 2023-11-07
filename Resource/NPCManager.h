#pragma once
#ifndef _NPC_MANAGER_H_
#define _NPC_MANAGER_H_

struct NpcData;
struct NPCDB;

class CNpcManager
{
public:
	CNpcManager();
	~CNpcManager();
	typedef StlVector<String, ManagedAllocator<String> > NameMap;
	const NpcData* GetNpc(UInt32 npcid);

	BOOL LoadCSV(const TCHAR* baseFile, const TCHAR* mobFile, const TCHAR* dropItemFile);
	BOOL LoadDAT(const TCHAR* filename);
//	BOOL SaveCSV(const TCHAR* filename);
	BOOL SaveDAT(const TCHAR* filename);
	BOOL LoadRandomNameCSV(const TCHAR* maleFile, const TCHAR* femaleFile, const TCHAR* preNameFile);	

	String GetNPCFstName(UInt16 id);
	String GetMaleNPCSndName(UInt16 id);
	String GetFemaleNPCSndName(UInt16 id);

	UInt16 GetTotalNPCFstName();
	UInt16 GetTotalNPCMaleSndName();
	UInt16 GetTotalNPCFemaleSndName();

	
	VOID Release();
private:
	typedef StlMap<UInt32, NpcData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, NpcData*> > > NpcIdMap;
	
	NpcIdMap m_idMap;
	NameMap m_fstName;
	NameMap m_maleSndName;
	NameMap m_femaleSndName;
	NameMap m_npcPreName;
    
};

#endif // _NPC_MANAGER_H_
