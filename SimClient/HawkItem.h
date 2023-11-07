#pragma once
#ifndef _HAWKITEM_H_
#define _HAWKITEM_H_

#include "Proxy/Logic/HawkItem_Proxy.h"

class CHawkItem : public HawkItem_Proxy
{
private:
	typedef HawkItem_Proxy Parent;
	NetObject* m_data;

public:
	DEFINE_CLASS(HawkItem_CLASSID);
// OnUpdate can be implemented selectively
	virtual void OnUpdateIdInfo(const UInt32 &item_uid, const UInt32 &item_id);
	virtual void OnUpdateStackInfo(const UInt8 &item_stack);
	virtual void OnUpdatePriceInfo(const UInt32 &item_price);
	virtual void OnUpdateItemTypeInfo(const UInt8 &item_type);
	virtual void OnUpdateIndexInfo(const UInt8 &item_index);
	INLINE void SetDataObject(NetObject* data){ m_data = data;}
	INLINE NetObject* GetDataObject(){ return m_data;}
	CHawkItem();
	~CHawkItem();
};

#endif //_HAWKITEM_H_
