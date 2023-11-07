#pragma once
#ifndef _PARTNERCOLLECTIONDATA_H_
#define _PARTNERCOLLECTIONDATA_H_

#include "Stub/Data/PartnerCollectionData_Stub.h"

class CdboPartnerCollection;
class CPartnerCollectionData : public PartnerCollectionData_Stub
{
public:

	DEFINE_CLASS(PartnerCollectionData_CLASSID);
	static CPartnerCollectionData * NewInstance();
	void SetData(CdboPartnerCollection* dboPartnerCollection);
	void PreSend();
	HRESULT UpdateDbo(CdboPartnerCollection* dboPartnerCollection);
	HRESULT DeleteDbo(CdboPartnerCollection* dboPartnerCollection);

private:
	typedef PartnerCollectionData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access

};

inline CPartnerCollectionData * CPartnerCollectionData::NewInstance()
{
	return CreateNetObject<CPartnerCollectionData>();
}

#endif //_PARTNERCOLLECTIONDATA_H_
