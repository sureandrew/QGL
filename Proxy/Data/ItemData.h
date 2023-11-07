#pragma once
#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_

#include "Proxy/Data/ItemData_Proxy.h"

class CItemData : public ItemData_Proxy
{
public:
	enum DataDetail {
		DETAIL_NONE = 0,
		DETAIL_INFO,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_ALL,
	};
	UInt8	m_detail;

private:
	typedef ItemData_Proxy Parent;

public:
	DEFINE_CLASS(ItemData_CLASSID);
// OnUpdate can be implemented selectively
};

#endif //_ITEMDATA_H_
