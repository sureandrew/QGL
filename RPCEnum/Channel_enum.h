#pragma once
#ifndef RPC_CHANNEL_ENUM
#define RPC_CHANNEL_ENUM

enum RPC_Channel_Enum
{
	Channel_CLASSID	= 27000,
	RPC_Channel_ENUM_ST	= 27000,
	RPC_Channel_UP_STATE	= 27302,
	RPC_Channel_REQ_STATE	= 27448,
	RPC_Channel_SendGrpMessage	= 27940,
	RPC_Channel_SendGrpFlushMessage	= 27028,
	RPC_Channel_SendEmotion	= 27687,
	RPC_Channel_CB_ReceiveMessage	= 27646,
	RPC_Channel_CB_SendGrpMessage	= 27109,
	RPC_Channel_CB_ReceiveEmotion	= 27817,
	RPC_Channel_SetListen	= 27939,
	RPC_Channel_CB_SetListen	= 27039,
	RPC_Channel_LeaveChannel	= 27770,
	RPC_Channel_CB_LeaveChannel	= 27284,
	RPC_Channel_ENUM_ED	= 27999
};

#ifdef RPC_ENUM_MAP
INLINE int _AddRpcEnum_Channel()
{
	RPC_ENUM_MAP[RPC_Channel_UP_STATE] = 684878080;
	RPC_ENUM_MAP[RPC_Channel_REQ_STATE] = 1650603450;
	RPC_ENUM_MAP[RPC_Channel_SendGrpMessage] = 1310921912;
	RPC_ENUM_MAP[RPC_Channel_SendGrpFlushMessage] = 1063456507;
	RPC_ENUM_MAP[RPC_Channel_SendEmotion] = 172715196;
	RPC_ENUM_MAP[RPC_Channel_CB_ReceiveMessage] = 953438141;
	RPC_ENUM_MAP[RPC_Channel_CB_SendGrpMessage] = 1809463618;
	RPC_ENUM_MAP[RPC_Channel_CB_ReceiveEmotion] = 651116148;
	RPC_ENUM_MAP[RPC_Channel_SetListen] = 1911187010;
	RPC_ENUM_MAP[RPC_Channel_CB_SetListen] = 1449986532;
	RPC_ENUM_MAP[RPC_Channel_LeaveChannel] = 642415681;
	RPC_ENUM_MAP[RPC_Channel_CB_LeaveChannel] = 1267335454;
	return 0;
}
static int _addrpc_Channel = _AddRpcEnum_Channel();
#endif // RPC_ENUM_MAP

#ifdef RPC_NONBLOCK_MAP
INLINE int _NonblockMap_Channel()
{
	RPC_NONBLOCK_MAP[RPC_Channel_UP_STATE] = true;
	RPC_NONBLOCK_MAP[RPC_Channel_CB_ReceiveMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Channel_CB_SendGrpMessage] = true;
	RPC_NONBLOCK_MAP[RPC_Channel_CB_ReceiveEmotion] = true;
	RPC_NONBLOCK_MAP[RPC_Channel_CB_SetListen] = true;
	RPC_NONBLOCK_MAP[RPC_Channel_CB_LeaveChannel] = true;
	return 0;
}
static int _nonblock_Channel = _NonblockMap_Channel();
#endif // RPC_NONBLOCK_MAP

#endif //RPC_CHANNEL_ENUM
