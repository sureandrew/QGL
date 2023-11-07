#pragma once
#ifndef _SKILLDATA_H_
#define _SKILLDATA_H_

#include "Proxy/Data/SkillData_Proxy.h"

class CSkillData : public SkillData_Proxy
{
private:
	typedef SkillData_Proxy Parent;

public:
	DEFINE_CLASS(SkillData_CLASSID);
};

#endif //_SKILLDATA_H_
