#pragma once
#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_

#include "Stub/Data/ItemData_Stub.h"

class CdboItem;
class CItemData : public ItemData_Stub
{
public:
	DEFINE_CLASS(ItemData_CLASSID);
	static CItemData * NewInstance();
	void SetData(CdboItem* dboItem);
	void SetDetail(UInt8 detail);
	HRESULT UpdateDbo(CdboItem* dboItem);
	HRESULT DeleteDbo(CdboItem* dboItem);
	static void SetGroupDetail(NetGroup *itemGroup, UInt8 detail);

	enum DataDetail {
		DETAIL_NONE = 0,
		DETAIL_INFO,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_ALL,
	};

private:
	typedef ItemData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CItemData * CItemData::NewInstance()
{
	return CreateNetObject<CItemData>();
}

#endif //_ITEMDATA_H_
