//-- Common
#include "Common.h"
#include "common/common_binary.h"
//-- Self
#include "SimClient/GiftBox.h"
//-- Library
#include "SimClient/Gift.h"

REGISTER_CLASSTYPE(CGiftBox, GiftBox_CLASSID);

CGiftBox::CGiftBox()
{
}

CGiftBox::~CGiftBox()
{
}

RPCResult CGiftBox::CB_TakeGiftBox(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult CGiftBox::CB_UpdateGiftBox(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, NetGroup* updateGrp)
{
	return RPC_RESULT_OK;
}