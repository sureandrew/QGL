#pragma once
#ifndef _MESSENGERDATA_H_
#define _MESSENGERDATA_H_

#include "Stub/Data/MessengerData_Stub.h"

class CdboMessenger;
class CMessengerData : public MessengerData_Stub
{
public:
	CMessengerData();
	virtual ~CMessengerData();
	DEFINE_CLASS(MessengerData_CLASSID);
	static CMessengerData * NewInstance();
	void SetData(CdboMessenger* dboMessenger);
	void PreSend();
	HRESULT UpdateDbo(CdboMessenger* dboMessenger);
	HRESULT DeleteDbo(CdboMessenger* dboMessenger);

private:
	typedef MessengerData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CMessengerData * CMessengerData::NewInstance()
{
	return CreateNetObject<CMessengerData>();
}
NetGroup* FindMessengerGroup(UInt32 char_id);
#endif //_MESSENGERDATA_H_
