#pragma once
#ifndef _GUILD_H_
#define _GUILD_H_

#include "Stub/Logic/Guild_Stub.h"

class CGuildData;
class CGuildMember;
class CMessageCharacter;
class CGuildApplicant;
class CChannel;
struct CharMsgInfo;

typedef StlMap<UInt32, CGuildMember*, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, CGuildMember*> > > GuildMemberMap;
typedef StlMap<UInt32, CGuildApplicant*, std::less<UInt32>, 
		ManagedAllocator< std::pair<UInt32, CGuildApplicant*> > > GuildApplicantMap;
#define MAX_MEMBER 60
#define MIN_MANOR_DONATE 5000000
#define MAX_FAIL_MANOR_MAINTAIN 200

class CGuild : public Guild_Stub,
				public Reuben::Simulation::IEventHandler<CGuild>
{
public:

	CGuild();
	virtual ~CGuild();
	
	DEFINE_CLASS(Guild_CLASSID);

	static CGuild* NewInstance();
	void SetData(CGuildData * pGuildData);
	void WriteData(CGuildData * pGuildData);
	RPCResult DoGetMembers(RPCContext &context);
	void PreSend();
	void PackGuildMember(NetGroup *grp);
	void PackGuildApplicant(NetGroup *grp);
	void InsertGuildMember(CGuildMember* &pGuildMember);
	void InsertApplicant(CMessageCharacter * pMsgChar);
	void RemoveGuildMember(UInt32 char_id, String reason, UInt32 kicker_id);
	void RemoveGuildApplicant(UInt32 char_id);
	void RemoveAllGuildMember();
	void SyncUpdate();
	void SyncToMap(LPCPEER pPeer=NULL);
	bool HasMember(UInt32 char_id);
	void MemberOnline(UInt32 char_id, Boolean bOnline);
	bool CheckPostAvialble(UInt8 post);
	CGuildMember* GetMember(UInt32 char_id);

	RPCResult LoadMembers(RPCContext &context);
	RPCResult AddMember(RPCContext &context);
	RPCResult KickMember(RPCContext &context);
	void GenerateNewGuildMaster();
	inline UInt32 GetMemberSize() { return (UInt32)m_memberMap.size(); }
	void SendGuild(CharMsgInfo* pMsgInfo, CGuildMember* pMember);

	bool IsApplicantExist(UInt32 char_id);
	bool NoMaster();

	bool OnlineMemberExist();
	void ReleaseMembers();
	void CheckRelease();

	void ChangeFund(Int32 change, UInt8 source = 5);
	void ChangePvPScore(Int32 chnage);
	void ChangePvPBid(Int32 chnage);
	void SendGuildMessage(String msg);
	void InitManor();
	UInt32 GetOweMaintainPayNum();
	void UpdateManorScale(UInt8 scale);	
	UInt32 FindNextResetAuraTime(UInt32 fromTime);

	virtual void HandleEvent(CONST Event &event);
	virtual String DebugInfo();

	void ChangeProsper(Int32 change, UInt8 source = 2);
	void ChangeMaterial(Int32 change);
	void ChangeRoom(Int8 change);
	void ChangeTrainCharEx(Int8 change);
	void ChangeTrainCharIn(Int8 change);
	void ChangeTrainPartnerEx(Int8 change);
	void ChangeTrainPartnerIn(Int8 change);
	void ChangeDrug(Int8 change);
	void ChangeCook(Int8 change);
	void ChangeWine(Int8 change);
	void ChangeManorHP(Int8 change);	
	void ChangeManorSP(Int8 change);

	void ChangeAutoManageBusiness(Boolean bOpen);

	UInt32	m_GuildMasterID;
	GuildMemberMap		m_memberMap;
	CChannel * m_pChannel;
	UInt16 m_donateDay;

	enum EventEnum
	{
		EVT_SYNC_UPDATE				= MAKE_EVENTID(Guild_CLASSID,  1),
		EVT_MANOR_MAINTENANCE		= MAKE_EVENTID(Guild_CLASSID,  2),
		EVT_MANOR_AURA_CHANGE		= MAKE_EVENTID(Guild_CLASSID,  3),	
		EVT_CLEAR_BUSINESS_LOG		= MAKE_EVENTID(Guild_CLASSID, 4),	
	};

private:
	void DoSyncUpdate();
	void DoSyncUpdateMember(NetGroup* guildGroup=NULL);

	typedef Guild_Stub Parent;
	
	GuildApplicantMap	m_applicantMap;
};

INLINE CGuild* CGuild::NewInstance()
{
	return CreateNetObject<CGuild>();
}

#endif //_GUILD_H_
