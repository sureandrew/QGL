#pragma once
#ifndef _MOB_MANAGER_H_
#define _MOB_MANAGER_H_

struct MobData;

typedef StlVector<const MobData*, ManagedAllocator<const MobData* > > MobDataList;

class CMobManager
{
public:
	CMobManager();
	~CMobManager();

	const MobData* GetMob(UInt32 Mobid);
	void GetMobList(MobDataList &mobList);
	UInt32 GetMobSize();

	BOOL LoadCSV(const TCHAR* filename);
	BOOL LoadDAT(const TCHAR* filename);
//	BOOL SaveCSV(const TCHAR* filename);
	BOOL SaveDAT(const TCHAR* filename);

	VOID Release();

private:
	typedef StlMap<UInt32, MobData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, MobData*> > > MobMapInt;

	MobMapInt m_mobint;
};

#endif // _MOB_MANAGER_H_
