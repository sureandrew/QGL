//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/RankGuild.h"
#include "Proxy/Data/GuildData.h"

REGISTER_CLASSTYPE(CRankGuild, RankGuild_CLASSID);


void CRankGuild::SetData(CGuildData *pGuildData)
{
	if (pGuildData->IsUpdated(CGuildData::idMask)) {
		SetId(pGuildData->GetGuild_uid());
	}

	if (pGuildData->IsUpdated(CGuildData::nameMask)) {
		SetName(pGuildData->GetName());
	}

	if (pGuildData->IsUpdated(CGuildData::guildPvPScoreMask)) {
		SetGuildPvPScore(pGuildData->GetGuildPvPScore());
	}

	if (pGuildData->IsUpdated(CGuildData::masterMask)) {
		SetMaster(pGuildData->GetMaster());
	}
	
}

void CRankGuild::PreSend()
{
	SetSendMask(AllMask);
}