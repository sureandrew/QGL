#pragma once
#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "Stub/Logic/Channel_Stub.h"
#include "Stub/Logic/Character.h"
#include "Common/ChannelType.h"

struct ChannelCharData;
class CMessageCharacter;

class CChannel : public Channel_Stub
{
private:
	typedef Channel_Stub Parent;
	typedef StlMap<UInt32, ChannelCharData, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, ChannelCharData> > > ChannelCharMap;

	ChannelCharMap m_MapCharsContextByID;

public:
	DEFINE_CLASS(Channel_CLASSID);

	CChannel();
	virtual ~CChannel();
	INLINE static CChannel* NewInstance();

	VOID InsertCharContext(RPCContext context, CMessageCharacter * msgChar);

	virtual RPCResult SendGrpMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId, const Boolean &guildSysMsg);
	virtual RPCResult SendGrpFlushMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId);
	virtual RPCResult SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen, const UInt32 &char_Id);
	virtual RPCResult LeaveChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult SendEmotion(LPCPEER pPeer, RPCContext &context, const UInt32 &emo_id, const UInt32 &SourceId, const UInt32 &TargetID, const bool &isPet);


	UInt NumberOfPlayer();
	VOID ClearSession(SessionID &sid);
	VOID RemoveCharFromChannel(UInt32 char_id);
	void SendShutdownMsg(UInt32 downTime);

	String m_channelName;
};

INLINE CChannel* CChannel::NewInstance()
{
	return CreateNetObject<CChannel>();
}

#endif //_CHANNEL_H_
