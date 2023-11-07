#pragma once
#ifndef _HAWKITEM_H_
#define _HAWKITEM_H_

#include "Stub/Logic/HawkItem_Stub.h"
#include "Stub/Logic/Character.h"

class CHawkItem : public HawkItem_Stub
{
private:
	typedef HawkItem_Stub Parent;
	RPCNetID m_dataNetID;

	UInt m_iPreBuyStack;
public:
	DEFINE_CLASS(HawkItem_CLASSID);
	CHawkItem() { m_iPreBuyStack = 0; }
	virtual ~CHawkItem(){};
	INLINE static CHawkItem* NewInstance();

	void SetHawkData(HawkerItemInfo data, UInt8 type);
	INLINE void SetDataObjectNetID(RPCNetID netID){ m_dataNetID = netID;}
	INLINE RPCNetID GetDataObjectNetID(){ return m_dataNetID;}
	INLINE void PreSend(){ SetSendMask(CHawkItem::AllMask);}
	INLINE void SetPreBuyStack(UInt preBuyStack){ m_iPreBuyStack = preBuyStack; }
	INLINE UInt GetPreBuyStack(){ return m_iPreBuyStack;}
};

INLINE CHawkItem* CHawkItem::NewInstance()
{
	return CreateNetObject<CHawkItem>();
}


#endif //_HAWKITEM_H_
