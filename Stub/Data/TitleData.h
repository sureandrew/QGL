#pragma once
#ifndef _TITLEDATA_H_
#define _TITLEDATA_H_

#include "Stub/Data/TitleData_Stub.h"

class CdboTitle;
class CTitleData : public TitleData_Stub
{
public:
	DEFINE_CLASS(TitleData_CLASSID);
	static CTitleData * NewInstance();
	void SetData(CdboTitle* dboTitle);
	void PreSend();
	HRESULT UpdateDbo(CdboTitle* dboTitle);
	HRESULT DeleteDbo(CdboTitle* dboTitle);

private:
	typedef TitleData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CTitleData * CTitleData::NewInstance()
{
	return CreateNetObject<CTitleData>();
}

#endif //_TITLEDATA_H_
