#pragma once
#ifndef _SKILL_MANAGER_H_
#define _SKILL_MANAGER_H_

struct SkillModData;
struct SkillData;
struct SkillMap;

typedef StlVector<const SkillData*, ManagedAllocator<const SkillData* > > SkillDataList;

class CSkillManager
{
public:
	CSkillManager();
	~CSkillManager();

private:
	VOID ConvertChr2Mod(TCHAR* src, SkillModData& target);

public:
	typedef StlMap<UInt16, SkillData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, SkillData*> > > SkillMap;
	typedef StlMultiMap<UInt16, SkillData*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, SkillData*> > > SkillRelMap;

	const SkillData* GetSkill(UInt16 skill_id);
	void GetSkillList(SkillDataList &skillList);
	void GetSkillLink(UInt16 skill_id, SkillDataList &skillList);
	void GetSkillLink(UInt16 skill_id, UInt16Vector &skillIds);

	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
//	BOOL SaveCSV(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);

	VOID Release();

private:

	SkillMap m_skills;
	SkillRelMap m_skillRels;
};

#endif // _SKILL_MANAGER_H_
