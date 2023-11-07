#pragma once
#ifndef _GLOBALVIEW_H_
#define _GLOBALVIEW_H_

//all global data
class GlobalView
{
public:
	virtual VOID NotifyEnterSuccess(LPCPEER pPeer, RPCContext &context, NetObject * map, NetObject * character) = 0;
	virtual VOID NotifyEnterBattle(LPCPEER pPeer, RPCContext &context, NetObject * battle, NetGroup * grp, const UInt8 & PlayerTeam
		,const BYTE & battleState, const UInt32 & npcId) = 0;
	virtual VOID NotifyReceiveMessageChannel(LPCPEER pPeer, RPCContext &context, NetObject * channel, const BYTE &Type) = 0;
	virtual VOID NotifyReceivePrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName
		, const String &TargetName) = 0;
	virtual VOID NotifyCB_SendPrivateMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const boolean &isFriend, const UInt32 &resultFlag) = 0;
	virtual VOID NotifyReceiveHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &count) = 0;
	virtual VOID NotifyCB_JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result) = 0;
	virtual VOID NotifyLeaveSuccess(Boolean bDelete) = 0;
};


extern GlobalView * g_pGlobalView;

#endif // _GLOBAL_H_