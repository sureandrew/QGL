#pragma once
#ifndef _GUILDMEMBERDATA_H_
#define _GUILDMEMBERDATA_H_

#include "Stub/Data/GuildMemberData_Stub.h"

class CdboGuildMember;

class CGuildMemberData : public GuildMemberData_Stub
{
public:
	static CGuildMemberData * NewInstance();
	DEFINE_CLASS(GuildMemberData_CLASSID);
	void SetData(CdboGuildMember* dboGuildMember);
	void PreSend();
	HRESULT UpdateDbo(CdboGuildMember* dboGuildMember);
	HRESULT DeleteDbo(CdboGuildMember* dboGuildMember);

private:
	typedef GuildMemberData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CGuildMemberData * CGuildMemberData::NewInstance()
{
	return CreateNetObject<CGuildMemberData>();
}

UInt32 IdentifyGuild(UInt32 char_id);		// identify guild by char_id
HRESULT RemoveGuildMembers(UInt32 guild_id);
HRESULT ReleaseGuildMembers(UInt32 guild_id);
HRESULT DoCreateGuildMember(const UInt32 &player_char_id, const UInt32 &guild_id, 
							const UInt8 &guildPost, CGuildMemberData* &pGuildMemberData);

#endif //_GUILDMEMBERDATA_H_
