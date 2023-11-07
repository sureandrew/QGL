///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHANNEL_STUB_H_
#define _RPC_CHANNEL_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Channel_enum.h"

class Channel_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult SendGrpMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const Boolean &guildSysMsg);
	virtual RPCResult SendGrpFlushMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId);
	virtual RPCResult SendEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const UInt32 &SourceId, const UInt32 &TargetID, const bool &isPet);
	virtual RPCResult SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen, const UInt32 &char_Id);
	virtual RPCResult LeaveChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		NextFreeMask	= Parent::NextFreeMask + 1
	};
	virtual RPCResult SendGrpMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const Boolean &guildSysMsg) = 0;
	virtual RPCResult SendGrpFlushMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId) = 0;
	virtual RPCResult SendEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const UInt32 &SourceId, const UInt32 &TargetID, const bool &isPet) = 0;
	virtual RPCResult CB_ReceiveMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName, const SessionIDVector &listeners);
	virtual RPCResult CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result, const UInt32 &resultFlag);
	virtual RPCResult CB_ReceiveEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const String &SrcName, const String &TargetName, const bool &isPet);
	virtual RPCResult SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen, const UInt32 &char_Id) = 0;
	virtual RPCResult CB_SetListen(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult LeaveChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult CB_LeaveChannel(LPCPEER pPeer, RPCContext &context);
	const BYTE& GetChannelType() const { return m.ChannelType; }
	void SetInfo(const BYTE &ChannelType);
	void SetChannelType(const BYTE &ChannelType);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//info
	BYTE ChannelType;
} m;

public:
	INLINE Channel_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_CHANNEL_STUB_H_
