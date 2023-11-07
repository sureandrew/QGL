#pragma once
#ifndef _SELLYUANBAODATA_H_
#define _SELLYUANBAODATA_H_

#include "Stub/Data/SellYuanBaoData_Stub.h"

class CdboSellYuanBao;

class CSellYuanBaoData : public SellYuanBaoData_Stub
{
public:

	DEFINE_CLASS(SellYuanBaoData_CLASSID);

	static CSellYuanBaoData * NewInstance();
	void SetData(CdboSellYuanBao* dboSellYuanBao);
	void PreSend();
	//HRESULT UpdateDbo(CdboSellYuanBao* dboSellYuanBao);	

private:
	typedef SellYuanBaoData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CSellYuanBaoData * CSellYuanBaoData::NewInstance()
{
	return CreateNetObject<CSellYuanBaoData>();
}

#endif //_SELLYUANBAODATA_H_
