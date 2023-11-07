#pragma once
#ifndef _HAWKERHISTORYDATA_H_
#define _HAWKERHISTORYDATA_H_

#include "Stub/Data/HawkerHistoryData_Stub.h"

class CdboHawkerHistory;

class CHawkerHistoryData : public HawkerHistoryData_Stub
{
public:

	DEFINE_CLASS(HawkerHistoryData_CLASSID);
	static CHawkerHistoryData * NewInstance();
	void SetData(CdboHawkerHistory* dboHawkerHistory);
	void PreSend();
	HRESULT UpdateDbo(CdboHawkerHistory* dboHawkHist);
	HRESULT DeleteDbo(CdboHawkerHistory* dboHawkHist);

private:
	typedef HawkerHistoryData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CHawkerHistoryData * CHawkerHistoryData::NewInstance()
{
	return CreateNetObject<CHawkerHistoryData>();
}


#endif //_HAWKERHISTORYDATA_H_
