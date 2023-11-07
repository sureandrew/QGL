#pragma once
#ifndef _DBO_LOG_GENERATION_ACC_H_
#define _DBO_LOG_GENERATION_ACC_H_
#include "Dbo.h"

class CdboLogGenAccAccessor : public CdboAccessor
{
public:
	DEFINE_DBOCLASS(CdboLogGenAccAccessor)

		DEFINE_ENTRY(uiTargetId, LARGE_INTEGER)
		DEFINE_ENTRY(uiRelatedId, LARGE_INTEGER)
		DEFINE_ENTRY(uiAcc, LARGE_INTEGER)
		DEFINE_ENTRY(ui16TargetLevel,int)
		DEFINE_ENTRY(ui16RelatedLevel,int)
		DEFINE_ENTRY(uiItemUId,LARGE_INTEGER)
		DEFINE_ENTRY(uiItemId,LARGE_INTEGER)

		BEGIN_PARAM_MAP_FUNC(_Insert)
		COLUMN_ENTRY(1, m_uiTargetId)
		COLUMN_ENTRY(2, m_uiRelatedId)
		COLUMN_ENTRY(3, m_uiAcc)
		COLUMN_ENTRY(4, m_ui16TargetLevel)
		COLUMN_ENTRY(5,m_ui16RelatedLevel)
		COLUMN_ENTRY(6,m_uiItemUId)
		COLUMN_ENTRY(7,m_uiItemId)
		END_PARAM_MAP_FUNC()

		DEFINE_COMMAND_EX(CdboLogMoneyAccessor, L"SELECT * FROM LogGenerationAcc")
};

class CdboLogGenAcc : public Cdbo<CCommand<CAccessor<CdboLogGenAccAccessor> > >
{
public:

	HRESULT Log(UInt32 uiTargetId, UInt32 uiRelatedId, UInt32 uiAcc,UInt16 ui16TargetLevel, UInt16 ui16RelatedLevel,UInt32 uiItemUId,UInt32 uiItemId)
	{
		SetGetBindFunc(NULL);
		SetGetParamsFunc(_Insert);
		m_uiTargetId.QuadPart = uiTargetId;
		m_uiRelatedId.QuadPart = uiRelatedId;
		m_uiAcc.QuadPart = uiAcc;
		m_ui16TargetLevel = ui16TargetLevel;
		m_ui16RelatedLevel = ui16RelatedLevel;
		m_uiItemUId.QuadPart = uiItemUId;
		m_uiItemId.QuadPart = uiItemId;
		return OpenCommand(_T("INSERT INTO LogGenerationAcc (target_id, related_id, accumulation, target_level, related_level,item_uid,item_id,create_time) VALUES (?, ?, ?, ?, ?, ?, ?,GetDate())"));
	}
};

#endif // _DBO_LOG_GENERATION_ACC_H_
