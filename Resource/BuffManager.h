#pragma once
#ifndef _BUFF_MANAGER_H_
#define _BUFF_MANAGER_H_

typedef StlVector<const BuffData*, ManagedAllocator<const BuffData* > > BuffDataList;

class CBuffManager
{
public:
	CBuffManager();
	~CBuffManager();

public:
	typedef StlMap<UInt32, BuffData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, BuffData*> > > BuffMap;
	
	const BuffData* GetBuff(UInt32 Buff_id);
	void GetBuffList(BuffDataList &buffList);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:

	BuffMap m_buffs;
};

#endif // _SKILL_MANAGER_H_
