#pragma once
#ifndef _SKILLDATA_H_
#define _SKILLDATA_H_

#include "Stub/Data/SkillData_Stub.h"

class CdboSkill;
class CSkillData : public SkillData_Stub
{
public:
	DEFINE_CLASS(SkillData_CLASSID);
	static CSkillData * NewInstance();
	void SetData(CdboSkill* dboSkill);
	void PreSend();
	HRESULT UpdateDbo(CdboSkill* dboSkill);
	HRESULT DeleteDbo(CdboSkill* dboSkill);

private:
	typedef SkillData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CSkillData * CSkillData::NewInstance()
{
	return CreateNetObject<CSkillData>();
}

HRESULT RemoveSkills(UInt32 owner_id, UInt8 owner_type);

#endif //_SKILLDATA_H_
