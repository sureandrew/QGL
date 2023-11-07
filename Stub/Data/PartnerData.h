#pragma once
#ifndef _PARTNERDATA_H_
#define _PARTNERDATA_H_

#include "Stub/Data/PartnerData_Stub.h"

class CdboPartner;
class CPartnerData : public PartnerData_Stub
{
public:
	DEFINE_CLASS(PartnerData_CLASSID);
	CPartnerData() : m_detail(DETAIL_NONE) {}
	static CPartnerData * NewInstance();
	void SetData(CdboPartner* dboPartner);
	void SetDetail(UInt8 detail);
	HRESULT UpdateDbo(CdboPartner* dboPartner);
	HRESULT DeleteDbo(CdboPartner* dboPartner);
	static void SetGroupDetail(NetGroup *partnerGroup, UInt8 detail);

	enum DataDetail {
		DETAIL_NONE = 0,
		DETAIL_INFO,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_ALL,
	};

private:
	typedef PartnerData_Stub Parent;
	UInt8	m_detail;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CPartnerData * CPartnerData::NewInstance()
{
	return CreateNetObject<CPartnerData>();
}

#endif //_PARTNERDATA_H_