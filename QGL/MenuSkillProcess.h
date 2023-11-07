#pragma once
#ifndef _MENUSKILL_H_
#define _MENUSKILL_H_

#include "QGL.h"
#include "common_binary.h"

#define MAX_MAIN_SKILL	5
#define MAX_SUB_SKILL	7

struct SkillData;
class CSkill;

struct SkillComData
{
	const SkillData * pSkillData;
	CSkill * pSkill;
	SkillComData()
	{
		pSkillData = NULL;
		pSkill = NULL;
	}
};

class CMenuSkillProcess
{
public:
	CMenuSkillProcess();
	virtual ~CMenuSkillProcess();

	void UpdateSkillMenu();
	void UpdateSubSkillMenu(Int Index);
	void UpdateMainSkillMenu(Int Index);
	void SkillMenuMainUp();
	void SkillMenuMainDown();
	void SkillMenuMainSubUp();
	void SkillMenuMainSubDown();
	void SkillMenuMainScroll();
	void SkillMenuSubScroll();
	void WriteToShortCut(Int ID, int targetPos);
	Int32 GetUseSkillID(Int Index);
	bool UseSkillProc();
	bool UseSkill(Int Index);
	bool UseSkillID(Int ID);
	void RefreshMainSkillMenu();
	void RefreshSubSkillMenu();

	void HideAllSkillButton();

	void ConstructData();

	void ConstructDataAll();

	void ConstructByShopID(UInt32 shopID);

	BYTE FindSkillTypeByBase(BYTE Base);

	void HideAllSubSkillButton();

	void UpdateSkillData(CSkill * pSkill);

	CSkill * FindSkill(Int ID);

	const SkillData * GetSubSkillData(Int Index);
	const SkillData * GetMainSkillData(Int Index);

	void SetSkillDetailIndex(int Index);

	void OpenSkillDetail();

	const SkillData * m_pTempSkillData;

	const SkillData * m_pTempSkillDetailData;

	inline bool GetIsShop(){return m_SkillShopID > 0;}

	inline WORD GetShopID(){return m_SkillShopID;}
	void SetSkillShop(int Value);

	CSkill * GetSkill(UInt16 skill_id);

	void RefreshShopDetail();
	void RefreshShopDetailItem();

	void ShopLearn();

	void ResetPosition();


private:
	BYTE	m_CurrSkillType;

	WORD	m_MainSkillStartIndex;
	WORD	m_SubSkillStartIndex;

	WORD	m_MainSkillSelIndex;
	WORD	m_SubSkillSelIndex;

	WORD	m_totalSubSkill;

	StlMap<WORD, SkillComData> m_SkillGroups[MAX_SKILL_TYPE];

	StlMultiMap<WORD, SkillComData> m_SkillBaseChildRel;

	const SkillData * m_MainSkillCurLst[MAX_MAIN_SKILL];
	const SkillData * m_SubSkillCurLst[MAX_SUB_SKILL];

	WORD	m_SkillShopID;
};




#endif 
