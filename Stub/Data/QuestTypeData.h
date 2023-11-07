#pragma once
#ifndef _QUESTTYPEDATA_H_
#define _QUESTTYPEDATA_H_

#include "Stub/Data/QuestTypeData_Stub.h"

class CdboQuestType;
class CQuestTypeData : public QuestTypeData_Stub
{
public:
	DEFINE_CLASS(QuestTypeData_CLASSID);
	static CQuestTypeData * NewInstance();
	VOID SetData(CdboQuestType* dboQuestType);
	VOID PreSend();
	HRESULT UpdateDbo(CdboQuestType* dboQuestType);
	HRESULT DeleteDbo(CdboQuestType* dboQuestType);

	static VOID PreSendGroup(NetGroup * questTypeGroup);
	static CQuestTypeData* NewQuestType();
	static NetGroup* FindQuestTypeGroup(UInt32 owner_id);

	enum StatusEnum
	{
		ST_NEW,
		ST_NORMAL,
		ST_DELETED,
	};

private:
	typedef QuestTypeData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CQuestTypeData * CQuestTypeData::NewInstance()
{
	return CreateNetObject<CQuestTypeData>();
}

#endif //_QUESTTYPEDATA_H_
