#pragma once
#ifndef _GIFTDATA_H_
#define _GIFTDATA_H_

#include "Stub/Data/GiftData_Stub.h"

class CdboGift;

class CGiftData : public GiftData_Stub
{
public:
	DEFINE_CLASS(GiftData_CLASSID);

	static CGiftData * NewInstance();
	void SetData(CdboGift* dboGift);
	void PreSend();
	HRESULT UpdateDbo(CdboGift* dboGift);
	HRESULT DeleteDbo(CdboGift* dboGift);

	//HRESULT UpdateDbo();

private:
	typedef GiftData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CGiftData * CGiftData::NewInstance()
{
	return CreateNetObject<CGiftData>();
}

#endif //_GIFTDATA_H_
