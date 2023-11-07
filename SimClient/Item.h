#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_

#include "Proxy/Logic/Item_Proxy.h"

struct ItemData;

class CItem : public Item_Proxy
{
public:
	enum DataDetail {
		DETAIL_NONE = 0,
		DETAIL_INFO,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_ALL,
	};
	const ItemData *m_data;

	DEFINE_CLASS(Item_CLASSID);
	CItem() : m_data(NULL), m_remain(0), m_stack(0) {}
	virtual void OnUpdateId(const UInt32 &item_uid, const UInt32 &item_id);
	virtual void OnUpdateRemain(const UInt8 &remain);
	virtual void OnUpdateStack(const UInt8 &stack);
	INLINE UInt8 GetRemain() { return m_remain; }
	INLINE UInt8 GetStack() { return m_stack; }
	INLINE void SetRemain(UInt8 remain) { m_remain = remain; }
	INLINE void SetStack(UInt8 stack) { m_stack = stack; }

	Boolean IsExist() { return GetStack() != 0; }
	Boolean IsBound();
	Boolean CanUse(Boolean inMap);
	Boolean CanEquip();
	void ShowDetail();
	virtual String DebugInfo();

private:
	typedef Item_Proxy Parent;
	UInt8	m_remain;
	UInt8	m_stack;

	Boolean AllowSexClass(UInt8 sex, UInt8 cclass);
};

#endif //_ITEM_H_
