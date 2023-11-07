#pragma once
#ifndef _QUESTFLAGDATA_H_
#define _QUESTFLAGDATA_H_

#include "Stub/Data/QuestFlagData_Stub.h"

class CdboQuestFlag;
class CQuestFlagData : public QuestFlagData_Stub
{
public:
	DEFINE_CLASS(QuestFlagData_CLASSID);
	static CQuestFlagData * NewInstance();
	VOID SetData(CdboQuestFlag* dboQuestFlag);
	void PreSend();
	HRESULT UpdateDbo(CdboQuestFlag* dboQuestFlag);
	HRESULT DeleteDbo(CdboQuestFlag* dboQuestFlag);
	static VOID PreSendGroup(NetGroup * questFlagGroup);
	static CQuestFlagData* NewQuestFlag();
	static NetGroup* FindQuestFlagGroup(UInt32 owner_id);

private:
	typedef QuestFlagData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CQuestFlagData * CQuestFlagData::NewInstance()
{
	return CreateNetObject<CQuestFlagData>();
}

#endif //_QUESTFLAGDATA_H_
