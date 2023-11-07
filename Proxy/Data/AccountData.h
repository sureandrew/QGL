#pragma once
#ifndef _ACCOUNTDATA_H_
#define _ACCOUNTDATA_H_

#include "Proxy/Data/AccountData_Proxy.h"

class CAccountData : public AccountData_Proxy
{
private:
	typedef AccountData_Proxy Parent;

public:
	DEFINE_CLASS(AccountData_CLASSID);

	static CAccountData * NewInstance();
	//virtual void OnUpdateInfo(const UInt32 &acct_id, const String &loginName, const String &password);
	//virtual void OnUpdateAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime);

};

inline CAccountData * CAccountData::NewInstance()
{
	return CreateNetObject<CAccountData>();
}

#endif //_ACCOUNTDATA_H_
