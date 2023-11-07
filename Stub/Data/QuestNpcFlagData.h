#pragma once
#ifndef _QUESTNPCFLAGDATA_H_
#define _QUESTNPCFLAGDATA_H_

#include "Stub/Data/QuestNpcFlagData_Stub.h"
class CdboQuestNpcFlag;
class CQuestNpcFlagData : public QuestNpcFlagData_Stub
{
public:

	DEFINE_CLASS(QuestNpcFlagData_CLASSID);

	static CQuestNpcFlagData * NewInstance();

	void SetData(CdboQuestNpcFlag* dboQuestNpcFlag);
	void PreSend();
	HRESULT UpdateDbo(CdboQuestNpcFlag* dboQuestNpcFlag);
	HRESULT DeleteDbo(CdboQuestNpcFlag* dboQuestNpcFlag);

	static HRESULT DoCreateNpcFlag(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id, const UInt16 &flag);
	static DWORD WINAPI QueueGetNpcFlag(LPVOID lpParam);
	static CQuestNpcFlagData* FindNpcFlag(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id);
	static HRESULT RemoveNpcFlag(const UInt32 &npc_id, const UInt16 &line_id, const UInt32 &owner_id, const UInt16 &flag_id);
	static CQuestNpcFlagData* NewNpcFlag();
	static void ReleaseNpcFlag();

private:
	typedef QuestNpcFlagData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access

};

inline CQuestNpcFlagData * CQuestNpcFlagData::NewInstance()
{
	return CreateNetObject<CQuestNpcFlagData>();
}


#endif //_QUESTNPCFLAGDATA_H_
