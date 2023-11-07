#pragma once
#ifndef _GIFTBOX_H_
#define _GIFTBOX_H_

#include "Proxy/Logic/GiftBox_Proxy.h"

class CGift;

class CGiftBox : public GiftBox_Proxy
{
public:
	virtual RPCResult CB_TakeGiftBox(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex);
	virtual RPCResult CB_UpdateGiftBox(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, NetGroup* updateGrp);
	DEFINE_CLASS(GiftBox_CLASSID);
	CGiftBox();
	virtual ~CGiftBox();

private:
	typedef GiftBox_Proxy Parent;
};

#endif //_GIFTBOX_H_
