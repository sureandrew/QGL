#pragma once
#ifndef _SKILLSHOP_MANAGER_H_
#define _SKILLSHOP_MANAGER_H_

struct SkillShopData;

class CSkillShopManager
{
public:
	CSkillShopManager();
	~CSkillShopManager();

public:
	typedef StlMap<UInt32, SkillShopData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, SkillShopData*> > > SkillShopMap;
	
	const SkillShopData* GetSkillShop(UInt32 skillShop_id);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:

	SkillShopMap m_skillShops;
};

#endif // _SKILL_MANAGER_H_
