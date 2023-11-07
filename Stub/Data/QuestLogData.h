#pragma once
#ifndef _QUESTLOGDATA_H_
#define _QUESTLOGDATA_H_

#include "Stub/Data/QuestLogData_Stub.h"

class CdboQuestLog;
class CQuestLogData : public QuestLogData_Stub
{
public:
	DEFINE_CLASS(QuestLogData_CLASSID);
	static CQuestLogData * NewInstance();
	VOID SetData(CdboQuestLog* dboQuestLog);
	VOID PreSend();
	void SetDetail(UInt8 detail);
	HRESULT UpdateDbo(CdboQuestLog* dboQuestLog);
	HRESULT DeleteDbo(CdboQuestLog* dboQuestLog);
	static VOID PreSendGroup(NetGroup * questLogGroup);
	static CQuestLogData* NewQuestLog();
	static NetGroup* FindQuestLogGroup(UInt32 owner_id);

	enum DataDetail {
		DETAIL_NONE = 0,
		DETAIL_INFO,
		DETAIL_ALL,
	};

	enum StatusEnum
	{
		ST_NEW,
		ST_NORMAL,
		ST_DELETED,
	};

private:
	typedef QuestLogData_Stub Parent;
	UInt8	m_detail;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CQuestLogData * CQuestLogData::NewInstance()
{
	return CreateNetObject<CQuestLogData>();
}

#endif //_QUESTLOGDATA_H_
