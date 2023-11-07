#pragma once
#ifndef _SKILL_COMBINATION_MANAGER_H_
#define _SKILL_COMBINATION_MANAGER_H_

struct MobSkillCombination;
#ifdef CLIENT
class CClientResManager;
#endif

class CSkillCombinationManager
{
public:
	CSkillCombinationManager();
	~CSkillCombinationManager();

public:
	BOOL LoadCSV(LPCTSTR szfilename, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	MobSkillCombination* GetSkillCombination(String szeFileName);
	VOID SearchFolder();
#ifdef CLIENT
	VOID SearchClientFolder(CClientResManager* pResource);
#endif
	VOID Release();
private:

	typedef StlMap<String, MobSkillCombination*, std::less<String>, ManagedAllocator<std::pair<String, MobSkillCombination*> > > MobSkillCombTable;

	MobSkillCombTable m_MobSkillCombLst;

};

#endif // _SKILL_COMBINATION_MANAGER_H_
