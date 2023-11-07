#pragma once
#ifndef _QUESTTYPE_H_
#define _QUESTTYPE_H_

#include "Stub/Logic/QuestType_Stub.h"

class CQuestTypeData;
class CQuestSystem;

class CQuestType : public QuestType_Stub
{
public:
	DEFINE_CLASS(QuestType_CLASSID);

	CQuestType();
	INLINE static CQuestType* NewInstance();

	void SetQuestSystem(CQuestSystem* pQuestSys) { m_pQuestSys = pQuestSys; }
	void SetData(CQuestTypeData *pData);
	void WriteData(CQuestTypeData *pData);
	void PreSend();			// prepare to send
	void Update();
	void ResetData();
	Boolean IsExist() { return GetStatus() != ST_DELETED; }
	void ChangeRound(Int change);
	void ChangeTotalRound(Int change);
	UInt16 GetTypeId() { return GetType_id(); }
	void SetClearTime();
	void SetTotalRound(const UInt16 &totalRound);

	enum StatusEnum
	{
		ST_NEW,
		ST_NORMAL,
		ST_DELETED,
	};

	enum ClearTypeEnum {
		TYPE_PER_DAY	= 1,
		TYPE_PER_WEEK,
	};

private:
	typedef QuestType_Stub Parent;

	CQuestSystem* m_pQuestSys;	
};

INLINE CQuestType* CQuestType::NewInstance()
{
	return CreateNetObject<CQuestType>();
}

#endif //_QUESTTYPE_H_
