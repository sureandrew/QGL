//-- Common
#include "Common.h"
//-- Self
#include "Resource/SkillManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/BuffReader.h>
#include "Resource/ResSkill.h"
#include "Resource/QGLCSVTReader.h"

#define SKILL_DATA_VER		1

typedef Reuben::Platform::BuffReader File;
typedef Reuben::Platform::FileException FileException;

////////////// Skill Manager //////////////////////////////////

CSkillManager::CSkillManager()
{
	m_skills.clear();
}

CSkillManager::~CSkillManager()
{
	Release();
}

VOID CSkillManager::Release()
{
	for (SkillMap::iterator it = m_skills.begin(); it != m_skills.end(); it++)
		SafeDeleteObject(it->second);
	m_skills.clear();
	m_skillRels.clear();
}

const SkillData* CSkillManager::GetSkill(UInt16 skill_id)
{
	SkillMap::iterator it = m_skills.find(skill_id);
	if( it!=m_skills.end() )
		return it->second;
	else
		return NULL;
}

void CSkillManager::GetSkillList(SkillDataList &skillList)
{
	for (SkillMap::iterator it = m_skills.begin(); it != m_skills.end(); it++)
		skillList.push_back(it->second);
}

void CSkillManager::GetSkillLink(UInt16 skill_id, SkillDataList &skillList)
{
	SkillRelMap::iterator Itr = m_skillRels.find(skill_id);
	Int count = (Int)m_skillRels.count(skill_id);
	for (Int i = 0; i < count; i++, ++Itr)
		skillList.push_back(Itr->second);
}

void CSkillManager::GetSkillLink(UInt16 skill_id, UInt16Vector &skillIds)
{
	SkillRelMap::iterator Itr = m_skillRels.find(skill_id);
	Int count = (Int)m_skillRels.count(skill_id);

	skillIds.push_back(skill_id);
	for (Int i = 0; i < count; i++, ++Itr)
		skillIds.push_back(Itr->second->id);
}

BOOL CSkillManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: SkillManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		SkillData* pSkill = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				TCHAR buf[512];

				pSkill = SafeCreateObject(SkillData);
				reader.Read(_T("skill_id"),		pSkill->id);
				reader.Read(_T("skill_type"),	pSkill->type);
				reader.ReadString(_T("skill_name"),	pSkill->name, 11);
				reader.Read(_T("skill_faction"),	pSkill->faction);
				reader.Read(_T("skill_class"),	pSkill->class_skill);
				reader.Read(_T("skill_reqBaseSkill"),	pSkill->reqBaseSkill);
				reader.Read(_T("skill_reqBaseSkillRank"),	pSkill->reqBaseSkillRank);
				reader.Read(_T("skill_reqLv"),	pSkill->reqLv);
				reader.Read(_T("skill_Req_Exp"),	pSkill->Req_Exp);
				reader.Read(_T("skill_maxRank"),	pSkill->maxRank);
				reader.Read(_T("skill_active"),	pSkill->active);
				reader.Read(_T("skill_battleorField"),	pSkill->battleorField);
				reader.Read(_T("skill_form"),	pSkill->form);
				reader.Read(_T("skill_targetType"),	pSkill->targetType);
				reader.Read(_T("skill_UseElement"),	pSkill->skill_UseElement);
				reader.Read(_T("skill_Script_Type"), pSkill->skill_Script_Type);
				reader.Read(_T("skill_cooldownType"),	pSkill->skill_cooldownType);
				reader.Read(_T("skill_mod_atk"),	pSkill->mod_atk);
				reader.Read(_T("skill_mod_Def"),	pSkill->mod_Def);
				reader.Read(_T("skill_mod_Spd"),	pSkill->mod_Spd);
				reader.Read(_T("skill_mod_Pow"),	pSkill->mod_Pow);
				reader.Read(_T("skill_mod_Hit"),	pSkill->mod_Hit);
				reader.Read(_T("skill_mod_Eva"),	pSkill->mod_Eva);
				reader.Read(_T("skill_mod_maxHP"),	pSkill->mod_maxHP);
				reader.Read(_T("skill_mod_maxSP"),	pSkill->mod_maxSP);
				reader.Read(_T("skill_mod_maxDP"),	pSkill->mod_maxDP);
				reader.Read(_T("skill_mod_maxWc"),	pSkill->mod_maxWc);
				reader.Read(_T("Skill_PreSkill"),	pSkill->preSkill);
				reader.Read(_T("Skill_Animation_Motion"),	pSkill->animation_Motion);
				reader.Read(_T("Skill_Animation_Type"),	pSkill->animation_Type);
				reader.Read(_T("Skill_ThrowItemType"), pSkill->Skill_ThrowItemType);
				reader.ReadString(_T("skill_Effect_Atk"), pSkill->effect_Atk, 99);
				reader.ReadString(_T("skill_Effect_Hurt"), pSkill->effect_Hurt, 99);
				reader.ReadString(_T("skill_Effect_Spark"), pSkill->effect_Spark, 99);
				reader.ReadString(_T("skill_Sound_Atk"), pSkill->sound_Atk, 99);
				reader.ReadString(_T("skill_Sound_Hurt"), pSkill->sound_Hurt, 99);
				reader.ReadString(_T("skill_Sound_Spark"), pSkill->sound_Spark, 99);
				reader.ReadString(_T("skill_icon"), pSkill->icon, 99);
				reader.ReadString(_T("Skill_description"), pSkill->skill_description, 99);
				reader.ReadString(_T("Skill_functionDes"), pSkill->skill_functionDes, 99);
				reader.Read(_T("Skill_AutoSwitch"),	pSkill->Skill_AutoSwitch);
				

				TCHAR *delim = _T("|");
				TCHAR *cmd = NULL; 
				reader.ReadString(_T("Skill_AutoAddBuff"),	buf, 511);
				cmd = _tcstok(buf, delim);
				while (cmd != NULL)
				{
					UInt16 value = _ttoi(cmd);
					pSkill->AutoAdd_Buffs.push_back(value);
					cmd = _tcstok(NULL, delim);
				}
				//reader.ReadString(_T("Skill_AutoAddBuff"), pSkill->skill_functionDes, 101);
				

				SkillMap::iterator Itr = m_skills.find(pSkill->id);
				if (Itr != m_skills.end())
				{
					SkillData* oldSkill = Itr->second;
					// relationship handling
					if( oldSkill->reqBaseSkill )
					{
						for( SkillRelMap::iterator relItr = m_skillRels.find(oldSkill->reqBaseSkill);
							relItr != m_skillRels.end();
							)
						{
							if( relItr->second==oldSkill )
								relItr = m_skillRels.erase(relItr);
							else
								++relItr;
						}
					}
					SafeDeleteObject(oldSkill);
					m_skills.erase(Itr);
				}

				m_skills.insert(std::make_pair(pSkill->id, pSkill));
				if (pSkill->reqBaseSkill > 0)
					m_skillRels.insert(std::make_pair(pSkill->reqBaseSkill, pSkill));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i skill(s) read"), (UInt)m_skills.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: SkillManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CSkillManager::LoadDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_READ_EXISTING);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			file.Read( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numSkill = 0;
			file.Read( (BYTE*)&numSkill, sizeof(UInt32) );

			SkillData* pSkill;
			for (UInt32 i = 0; i < numSkill; i++)
			{
				pSkill = SafeCreateObject(SkillData);
				file.Read( (BYTE*)pSkill, sizeof(SkillData) );
				m_skills[pSkill->id] = pSkill;
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CSkillManager::LoadDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();

	return TRUE;
}

BOOL CSkillManager::SaveDAT(const TCHAR *filename)
{
	File file;

	try
	{
		file.Open(filename, File::MODE_WRITE);
		if (file.IsOpened())
		{
			DefaultInfoHeader header;
			header.version = SKILL_DATA_VER;
			file.Write( (BYTE*)&header, sizeof(DefaultInfoHeader) );
			UInt32 numSkill = (UInt32)m_skills.size();
			file.Write( (BYTE*)&numSkill, sizeof(UInt32) );

			for (SkillMap::iterator it = m_skills.begin(); it != m_skills.end(); it++)
			{
				file.Write( (BYTE*)it->second, sizeof(SkillData) );
			}
		}
	}
	catch(FileException Er)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: CSkillManager::SaveDAT, open error, %s"), filename);
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}


VOID CSkillManager::ConvertChr2Mod(TCHAR* src, SkillModData& target)
{
	String tempstring = src;
	tempstring.Trim();
	if( tempstring.Right(1).Compare(_T("%"))==0 )
	{
		target.type = SKILL_MOD_TYPE_PERCENT;
		target.value = (Real32)_tstof(tempstring.Left(tempstring.GetLength()-1).c_str());
	}else
	{
		target.type = SKILL_MOD_TYPE_NUM_PER_LEVEL;
		target.value = (Real32)_tstof(tempstring.c_str());
	}
}