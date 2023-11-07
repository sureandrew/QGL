#pragma once
#ifndef _RESOURCE_SKILLSHOP_H_
#define _RESOURCE_SKILLSHOP_H_

#include <Reuben\System\Reuben.h>

struct SkillShopData
{
	UInt16	id;
	StlVector<UInt16, ManagedAllocator<UInt16> >	SkillShopIDLst;
	UInt16	SkillBaseType;
	UInt16	UseSmoney;
	UInt16	UseMoney;
	UInt16	UseEXP;
	UInt16	UseHonor;
	UInt16	UseDonateFaction;
	UInt16	UseDonateManor;
	UInt16	UseYuanBao;

	SkillShopData()
		: id(C_UINT16(0))
		, SkillBaseType(C_UINT16(0))
		, UseSmoney(C_UINT16(0))
		, UseMoney(C_UINT16(0))
		, UseEXP(C_UINT16(0))
		, UseHonor(C_UINT16(0))
		, UseDonateFaction(C_UINT16(0))
		, UseDonateManor(C_UINT16(0))
		, UseYuanBao(C_UINT16(0))
	{}

	~SkillShopData()	{SkillShopIDLst.clear();}
};

#endif // _RESOURCE_SKILL_H_

