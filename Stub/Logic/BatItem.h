#pragma once
#ifndef _BATITEM_H_
#define _BATITEM_H_

#include "Stub/Logic/BatItem_Stub.h"

struct ItemData;
class CItemData;
class CBatActor;
class CBattle;

class CBatItem : public BatItem_Stub
{
private:
	typedef BatItem_Stub Parent;
	typedef StlVector<CBatActor*, ManagedAllocator<CBatActor*> > Actors;

public:
	const ItemData* m_data;
	UInt8	m_tryStack;						// stack after try
	UInt8	m_tryRemain;					// remain after try
	DEFINE_CLASS(BatItem_CLASSID);

	CBatItem();
	virtual ~CBatItem();
	INLINE static CBatItem* NewInstance();
	UInt8 GetEquipIndex();
	void SetData(CItemData *pData);			// set character by CItemData
	void WriteData(CItemData *pData);		// write character to CItemData
	void PreSend();							// prepare to send
	Boolean IsExist();						// is really exist?
	
	// user can use to target or not?
	Boolean CanUse(CBatActor* pUser, CBatActor* pTarget, String &errMsg);
	// user use on target (try or not)
	Boolean UseOn(CBatActor* pUser, CBatActor* pTarget, String &errMsg, 
					Boolean bTry=BOOLEAN_FALSE);
	Boolean AllowSexClass(UInt8 sex, UInt8 cclass);		// can use/equip for sex and class?
	void ReduceCount();
	int GetQuality();
};

INLINE CBatItem* CBatItem::NewInstance()
{
	return CreateNetObject<CBatItem>();
}

INLINE Boolean CBatItem::IsExist()
{
	return (GetStack() != 0);
}

#endif //_BATITEM_H_