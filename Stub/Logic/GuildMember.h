#pragma once
#ifndef _GUILDMEMBER_H_
#define _GUILDMEMBER_H_

#include "Stub/Logic/GuildMember_Stub.h"

class CGuildMemberData;
class CMsgCharacterData;
class CGuild;
class CMessageCharacter;

class CGuildMember : public GuildMember_Stub,
						public Reuben::Simulation::IEventHandler<CGuildMember>
{
public:
	virtual RPCResult ResponseApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const bool &accept);
	virtual RPCResult PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &newPost);
	virtual RPCResult ChangeAim(LPCPEER pPeer, RPCContext &context, const String &text);
	virtual RPCResult ChangeRule(LPCPEER pPeer, RPCContext &context, const String &text);
	virtual RPCResult ClearGuildBusinessLog(LPCPEER pPeer, RPCContext &context);

	DEFINE_CLASS(GuildMember_CLASSID);

	CGuildMember();
	virtual ~CGuildMember();
	static CGuildMember* NewInstance();

	void SetData(CGuildMemberData * pGuildMemberData);
	Boolean SetData(CMsgCharacterData * pMsgCharData, bool bClear = true);
	void SetData(CMessageCharacter * pMsgChar, bool bClear=true);

	void WriteData(CGuildMemberData * pGuildMemberData);
	void PreSend();
	void ChangeDonate(Int change, UInt8 source = 0);

	virtual RPCResult RequestGuildInfo(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RequestMember(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult RequestApplicant(LPCPEER pPeer, RPCContext &context);

	virtual void HandleEvent(CONST Event &event);
	CGuild * m_pGuild;

	UInt32	m_requestMemberTime;
	UInt32	m_requestInfoTime;
	bool	m_donated;

	void RaiseNoTradeEvent();
	void RaiseNoMoneyEvent();
	void CancelNoMoneyEvent();
	enum EventEnum
	{
		EVT_REMOVE_TRADER_POST_BY_NO_TRADE	= MAKE_EVENTID(GuildMember_CLASSID,  1),
		EVT_REMOVE_TRADER_POST_BY_NO_MONEY	= MAKE_EVENTID(GuildMember_CLASSID,  2),
	};

private:
	typedef GuildMember_Stub Parent;

};


INLINE CGuildMember* CGuildMember::NewInstance()
{
	return CreateNetObject<CGuildMember>();
}
#endif //_GUILDMEMBER_H_
