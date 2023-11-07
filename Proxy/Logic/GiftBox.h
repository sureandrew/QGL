#pragma once
#ifndef _GIFTBOX_H_
#define _GIFTBOX_H_

#include "Proxy/Logic/GiftBox_Proxy.h"

class CGift;

class CGiftBox : public GiftBox_Proxy
{
private:
	typedef GiftBox_Proxy Parent;
	//typedef StlMap<UInt32, CGift*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CGift*> > > GiftMap;
	typedef StlVector<CGift*, ManagedAllocator<CGift*> > GiftList;
	GiftList m_itemList;
	GiftList m_partnerList;
	GiftList m_petList;
	UInt m_iTakeGiftID;
	
public:
	DEFINE_CLASS(GiftBox_CLASSID);

	CGiftBox();
	virtual ~CGiftBox();
	void ConstructGiftBox();
	UInt GetGiftBoxSize(UInt type);
	CGift* GetGift(UInt type, UInt slot);
	virtual RPCResult CB_TakeGiftBox(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex);
	inline void SetTakeGiftInfo(UInt giftID){	m_iTakeGiftID = giftID;	}
	virtual RPCResult CB_UpdateGiftBox(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, NetGroup* updateGrp);

// OnUpdate can be implemented selectively
};

#endif //_GIFTBOX_H_
