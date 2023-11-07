#pragma once
#ifndef RPC_GIFTBOX_ENUM
#define RPC_GIFTBOX_ENUM

enum RPC_GiftBox_Enum
{
	GiftBox_CLASSID	= 53000,
	RPC_GiftBox_ENUM_ST	= 53000,
	RPC_GiftBox_UP_STATE	= 53426,
	RPC_GiftBox_REQ_STATE	= 53456,
	RPC_GiftBox_TakeGiftBox	= 53179,
	RPC_GiftBox_CloseGiftBox	= 53920,
	RPC_GiftBox_CB_TakeGiftBox	= 53043,
	RPC_GiftBox_UpdateGiftBox	= 53447,
	RPC_GiftBox_CB_UpdateGiftBox	= 53678,
	RPC_GiftBox_ENUM_ED	= 53999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_GiftBox()
{
	RPC_ENUM_MAP[RPC_GiftBox_UP_STATE] = 429473798;
	RPC_ENUM_MAP[RPC_GiftBox_REQ_STATE] = 653534320;
	RPC_ENUM_MAP[RPC_GiftBox_TakeGiftBox] = 1396441818;
	RPC_ENUM_MAP[RPC_GiftBox_CloseGiftBox] = 892937215;
	RPC_ENUM_MAP[RPC_GiftBox_CB_TakeGiftBox] = 1442668117;
	RPC_ENUM_MAP[RPC_GiftBox_UpdateGiftBox] = 679484234;
	RPC_ENUM_MAP[RPC_GiftBox_CB_UpdateGiftBox] = 1125544751;
	return 0;
}
static int _addrpc_GiftBox = _AddRpcEnum_GiftBox();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_GiftBox()
{
	RPC_NONBLOCK_MAP[RPC_GiftBox_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_GiftBox_CB_TakeGiftBox] = true;
	RPC_NONBLOCK_MAP[RPC_GiftBox_CB_UpdateGiftBox] = true;
	return 0;
}
static int _nonblock_GiftBox = _NonblockMap_GiftBox();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_GIFTBOX_ENUM
