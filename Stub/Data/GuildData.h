#pragma once
#ifndef _GUILDDATA_H_
#define _GUILDDATA_H_

#include "Stub/Data/GuildData_Stub.h"

class CdboGuild;

class CGuildData : public GuildData_Stub
{
public:
	DEFINE_CLASS(GuildData_CLASSID);

	static CGuildData * NewInstance();
	void SetData(CdboGuild* dboGuild);
	HRESULT UpdateDbo(CdboGuild* dboGuild);
	HRESULT DeleteDbo(CdboGuild* dboGuild);
	void PreSend();

private:
	typedef GuildData_Stub Parent;
	static ThreadKey m_key;					// safe thread key for multiple db access
};

inline CGuildData * CGuildData::NewInstance()
{
	return CreateNetObject<CGuildData>();
}
#endif //_GUILDDATA_H_
