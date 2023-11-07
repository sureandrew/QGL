#pragma once
#ifndef _GIFTBOX_H_
#define _GIFTBOX_H_

#include "Stub/Logic/GiftBox_Stub.h"

class CGift;
class CCharacter;

class CGiftBox : public GiftBox_Stub
{
private:
	typedef GiftBox_Stub Parent;
	typedef StlMap<UInt32, CGift*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CGift*> > > GiftMap;
	GiftMap m_giftList;
	CCharacter* m_pOwner;
	UInt m_iUpdateTime;

public:
	DEFINE_CLASS(GiftBox_CLASSID);
	CGiftBox();
	virtual ~CGiftBox();
	INLINE static CGiftBox* NewInstance();
	void SetGiftBox(NetGroup* giftbox, bool sendToClient = false, NetGroup* sendGrp = NULL);
	virtual RPCResult TakeGiftBox(LPCPEER pPeer, RPCContext &context, const UInt32 &giftID);
	virtual RPCResult CloseGiftBox(LPCPEER pPeer, RPCContext &context);
	inline void SetOwner(CCharacter* owner){ m_pOwner = owner;}
	virtual RPCResult UpdateGiftBox(LPCPEER pPeer, RPCContext &context);

};

INLINE CGiftBox* CGiftBox::NewInstance()
{
	return CreateNetObject<CGiftBox>();
}

#endif //_GIFTBOX_H_
