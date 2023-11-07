#pragma once
#ifndef _MESSAGECHARACTER_H_
#define _MESSAGECHARACTER_H_

#include "Stub/Logic/MessageCharacter_Stub.h"
#include "Common/ChannelType.h"
#include "Common/common_binary.h"

class CCharacterData;
class CMsgCharacterData;
class CCharacter;

#define REQUEST_GUILD_COUNT	30
#define REQUEST_GUILD_TIME	2000

class CMessageCharacter : public MessageCharacter_Stub,
							public Reuben::Simulation::IEventHandler<CMessageCharacter>
{
public:
	CMessageCharacter();
	virtual ~CMessageCharacter();
	DEFINE_CLASS(MessageCharacter_CLASSID);
	INLINE static CMessageCharacter* NewInstance();
	Boolean HasGuild() { return GetGuild_uid() != 0; }
	Boolean IsOnline() { return GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE; }
	Boolean IsAway() { return GetFriend_status() == FRIENC_CHAT_STATUS_ONLINE_AWAY; }
	Boolean IsOffline() { return GetFriend_status() == FRIENC_CHAT_STATUS_OFFLINE; }
	void SetData(CMsgCharacterData * pData, bool bClear = true);
private:
	void WriteData(CMsgCharacterData * pData);
public:
	void ChangeStatus(UInt8 status);
	void UpdateObject();
	void DoUpdateObject();
	void PreSend();
	RPCResult UpdateInfo(RPCContext &context);

	virtual void HandleEvent(CONST Event &event);
	virtual RPCResult CMessageCharacter::SelfSettingChange(LPCPEER pPeer, RPCContext &context, const String &about, const UInt8 &OnlineState, const String &Reply);

	void SyncGuild();
	
	enum EventEnum
	{
		EVT_SYNC_GUILD		= MAKE_EVENTID(MessageCharacter_CLASSID,  1),
		EVT_UPDATE			= MAKE_EVENTID(MessageCharacter_CLASSID,  2),
	};

	bool			m_speakRights[MAX_CHANNELTYPE];
	UInt32Vector	m_requestGuildList;
	BOOL			m_firstRequest;
	UInt32			m_refCount;

private:
	typedef MessageCharacter_Stub Parent;
};

INLINE CMessageCharacter* CMessageCharacter::NewInstance()
{
	return CreateNetObject<CMessageCharacter>();
}
#endif //_MESSAGECHARACTER_H_
