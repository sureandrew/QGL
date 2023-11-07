#pragma once
#ifndef _RES_SKILL_COMBINATION_H_
#define _RES_SKILL_COMBINATION_H_


struct MobSkillDef
{
	UInt32 skillRate;
	UInt16	skillId;
	UInt8	skillType;
};


struct MobSkillCombination
{
	typedef StlVector<MobSkillDef*, ManagedAllocator<MobSkillDef*> > CombinationLst;
	CombinationLst  skillLst;
	
	MobSkillCombination()
	{
		skillLst.clear();
	};
	
	~MobSkillCombination()
	{
		for ( CombinationLst::iterator itr = skillLst.begin(); itr != skillLst.end(); ++itr )
			SafeDeallocate((*itr));
		skillLst.clear();
	};
};

#endif // _RES_SKILL_COMBINATION_H_