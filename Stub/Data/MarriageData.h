#pragma once
#ifndef _MARRIAGEDATA_H_
#define _MARRIAGEDATA_H_

#include "Stub/Data/MarriageData_Stub.h"
class CdboMarriage;
class CMarriageData : public MarriageData_Stub
{
public:

	DEFINE_CLASS(MarriageData_CLASSID);
	static CMarriageData * NewInstance();
	void SetData(CdboMarriage* dboMarriage);
	void PreSend();
	HRESULT UpdateDbo(CdboMarriage* dboMarriage);
	HRESULT DeleteDbo(CdboMarriage* dboMarriage);


	static HRESULT DoCreateMarriage(const UInt32 & husband_id, const UInt32 & wife_id,const time_t &marriage_time,const UInt32 & state,CMarriageData* &pMarriageData);
	static DWORD WINAPI QueueGetMarriage(LPVOID lpParam);
	static CMarriageData* FindMarriage(UInt32 marriage_id);
	static CMarriageData* FindMarriageByHusbandId(UInt32 husband_id);
	static CMarriageData* FindMarriageByWifeId(UInt32 wife_id);
	static HRESULT RemoveMarriage(UInt32 marriage_id);
	static CMarriageData* NewMarriage();
	static void ReleaseMarriage(UInt32 char_id);

private:
	typedef MarriageData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access

};
inline CMarriageData * CMarriageData::NewInstance()
{
	return CreateNetObject<CMarriageData>();
}
#endif //_MARRIAGEDATA_H_
