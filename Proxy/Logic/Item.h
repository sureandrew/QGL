#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_

#include "Proxy/Logic/Item_Proxy.h"

struct ItemData;

class CItem : public Item_Proxy
{
public:
	DEFINE_CLASS(Item_CLASSID);
	CItem() : m_data(NULL), m_remain(0), m_stack(0), m_iGivenStack(0) {}
	virtual void OnUpdateId(const UInt32 &item_uid, const UInt32 &item_id);
	virtual void OnUpdateRemain(const UInt8 &remain);
	virtual void OnUpdateStack(const UInt8 &stack);
	INLINE UInt8 GetRemain() { return m_remain; }
	INLINE UInt8 GetStack() { return m_stack; }
	INLINE void SetRemain(UInt8 remain) { m_remain = remain; }
	INLINE void SetStack(UInt8 stack) { m_stack = stack; }

	Boolean IsExist() { return GetStack() != 0; }

	void SetGivenStack(UInt8 givenStack){ m_iGivenStack = givenStack;}
	UInt8 GetGivenStack(){ return m_iGivenStack;}
	bool IsGiven(){ return (m_iGivenStack > 0)?true:false;} 
	Boolean IsBound();
	Boolean CanUse(Boolean inMap);
	Boolean CanEquip();
	UInt8 GetItemType();
	UInt8 GetTargetType();

	//void ShowDetail();

	enum DataDetail {
		DETAIL_NONE = 0,
		DETAIL_INFO,
		DETAIL_MAP,
		DETAIL_BATTLE,
		DETAIL_ALL,
	};
	const ItemData *m_data;

private:
	Boolean AllowSexClass(UInt8 sex, UInt8 cclass);

	typedef Item_Proxy Parent;
	UInt8	m_remain;
	UInt8	m_stack;
	UInt8   m_iGivenStack;
};

#endif //_ITEM_H_
