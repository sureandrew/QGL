//-- Common
#include "Common.h"
#include "Common/common_binary.h"
//-- Self
#include "SimClient/Guild.h"
//-- Library
#include "SimClient/Global.h"
#include "SimClient/GuildMember.h"
#include "SimClient/Guild.h"
#include "SimClient/GuildApplicant.h"
#include "SimClient/Character.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"

PROTO_LuaPush(CGuild)
REGISTER_CLASSTYPE(CGuild, Guild_CLASSID);

CGuild::~CGuild()
{
	for (GuildMemberMap::iterator it = m_memberMap.begin(); it != m_memberMap.end(); ++it)
		DeleteNetObject(it->second);
	for (GuildApplicantMap::iterator it = m_applicantMap.begin(); it != m_applicantMap.end(); ++it)
		DeleteNetObject(it->second);
	m_memberMap.clear();
}

RPCResult CGuild::CB_AddMember(LPCPEER pPeer, RPCContext &context, NetObject* pMember)
{
	CGuildMember * pGuildMember = CastNetObject<CGuildMember>(pMember);
	if (pGuildMember)
	{
		GuildMemberMap::iterator it = m_memberMap.find(pGuildMember->GetChar_id());
		if (it == m_memberMap.end())
		{
			m_memberMap.insert(std::make_pair(pGuildMember->GetChar_id(), pGuildMember));
			TRACE_INFODTL_2(GLOBAL_LOGGER,
				_T("CB_AddMember: member join, guild %d, char_id %d"),
				pGuildMember->GetGuild_uid(), pGuildMember->GetChar_id());
		}
		else if (pGuildMember != it->second)
			DeleteNetObject(pGuildMember);
	}
	else
		DeleteNetObject(pMember);    

	return RPC_RESULT_OK;
}

RPCResult CGuild::CB_RemoveMember(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &reason)
{
	GuildMemberMap::iterator it = m_memberMap.find(char_id);
	if (it != m_memberMap.end())
	{
		if (it->second == gGlobal.m_pSimClient->m_ownGuildMember)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CB_RemoveMember: you leave guild %d"), 
				GetGuild_uid());
			gGlobal.m_pSimClient->m_ownGuildMember = NULL;
			DeleteNetObject(gGlobal.m_pSimClient->m_ownGuild);
		} else {
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_RemoveMember: member left, guild %d, char_id %d"),
				GetGuild_uid(), char_id);
			DeleteNetObject(it->second);
			m_memberMap.erase(it);
		}
	}
	return RPC_RESULT_OK;
}

void CGuild::RemoveAllMember()
{
	for (GuildMemberMap::iterator it = m_memberMap.begin(); it != m_memberMap.end(); ++it)
	{
		DeleteNetObject(it->second);
	}
	m_memberMap.clear();
}


RPCResult CGuild::CB_AddApplicant(LPCPEER pPeer, RPCContext &context, NetObject* pObj)
{
	CGuildApplicant * pGuildApplicant = CastNetObject<CGuildApplicant>(pObj);
	if (pGuildApplicant)
	{
		GuildApplicantMap::iterator it = m_applicantMap.find(pGuildApplicant->GetChar_id());
		if (it == m_applicantMap.end())
		{
			m_applicantMap.insert(std::make_pair(pGuildApplicant->GetChar_id(), pGuildApplicant));
			TRACE_INFODTL_1(GLOBAL_LOGGER,
				_T("CB_AddApplicant: char_id %d waiting for member approval"),
				pGuildApplicant->GetChar_id());
		}
		else if (pGuildApplicant != it->second)
			DeleteNetObject(pGuildApplicant);
	}
	else
		DeleteNetObject(pObj);	

//	DeleteNetObject(pObj);
	return RPC_RESULT_OK;
}


RPCResult CGuild::CB_RemoveApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	GuildApplicantMap::iterator it = m_applicantMap.find(char_id);
	if (it != m_applicantMap.end())
	{
		DeleteNetObject(it->second);
		m_applicantMap.erase(it);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_RemoveApplicant: char_id %d leave guild"),
			char_id); 	
	}	
	return RPC_RESULT_OK;
}

void CGuild::ShowDetail()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Show Guild %d"), GetGuild_uid());

	CGuildMember* pMember;
	int i = 0;
	for (GuildMemberMap::iterator it = m_memberMap.begin(); it != m_memberMap.end(); ++it, ++i)
	{
		pMember = it->second;
		if (pMember)
		{
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Member %d: char_id %d, post %d"),
				i, pMember->GetChar_id(), pMember->GetPost());
		}
	}
}

void CGuild::RequestApplicant(NetGroup* grp)
{
	CGuildApplicant * pGuildApp;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		pGuildApp = CastNetObject<CGuildApplicant>(grp->GetChild(i));
		if (pGuildApp)
		{
			GuildApplicantMap::iterator it = m_applicantMap.find(pGuildApp->GetChar_id());
			if (it != m_applicantMap.end())
			{
				if (it->second == pGuildApp)
				{
					pGuildApp = NULL;
				} 
				else
				{
					DeleteNetObject(it->second);
					m_applicantMap.erase(it);
				}
			}
			if (pGuildApp)
			{
				m_applicantMap.insert(std::make_pair(pGuildApp->GetChar_id(), pGuildApp));
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Guild Applicant %d: char_id %d"), i, 
					pGuildApp->GetChar_id());
			}
		}
	}
}

void CGuild::ApprovalMembership(UInt32 applIndex, bool allow)
{
	UInt32 i = 0;
	for (GuildApplicantMap::iterator it = m_applicantMap.begin();
		it != m_applicantMap.end(); ++it, ++i)
	{
		if (i == applIndex && gGlobal.m_pSimClient->m_ownGuildMember)
		{
			gGlobal.m_pSimClient->m_ownGuildMember->ResponseApplicant(
				gGlobal.m_pSimClient->m_pProxyPeer, gGlobal.m_pSimClient->m_context, 
				it->second->GetChar_id(), allow);
		}
	}
}

void CGuild::KickMember(UInt32 memberIndex)
{
	UInt32 i = 0;
	for (GuildApplicantMap::iterator it = m_applicantMap.begin();
		it != m_applicantMap.end(); ++it, ++i)
	{
		if (i == memberIndex && gGlobal.m_pSimClient->m_ownGuildMember)
		{
			gGlobal.m_pSimClient->m_pCharacter->KickMember(
				gGlobal.m_pSimClient->m_pProxyPeer, gGlobal.m_pSimClient->m_context, 
				it->second->GetChar_id(), String());
		}
	}
}

void CGuild::ChangeGuildPost(UInt32 memberIndex, UInt32 postIndex)
{
	if (gGlobal.m_pSimClient == NULL || gGlobal.m_pSimClient->m_ownGuildMember == NULL)
		return;

	UInt i = 0;
	for (GuildMemberMap::iterator it = m_memberMap.begin();
		it != m_memberMap.end(); ++it, i++)
	{
		if (i == memberIndex)
		{
			if (it->second->GetLevel() < 30 && postIndex == GUILD_POSITION_MASTER)
			{
				TRACE_INFODTL(GLOBAL_LOGGER, _F("ChangeGuildPost: not enough level to promote"));
			}
			else 
			{
				gGlobal.m_pSimClient->m_ownGuildMember->PositionChange(
					gGlobal.m_pSimClient->m_pProxyPeer, gGlobal.m_pSimClient->m_context, 
					it->second->GetChar_id(), postIndex);
			}
		}
	}
}

void CGuild::SetMembers(NetGroup* grp)
{
	if (gGlobal.m_pSimClient == NULL)
	{
		DeleteNetGroup(grp, BOOLEAN_TRUE);
		return;
	}

	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		CGuildMember* pGuildMember = CastNetObject<CGuildMember>(grp->GetChild(i));
		if (pGuildMember)
		{
			if (pGuildMember->GetGuild_uid() != GetGuild_uid())
			{
				DeleteNetObject(pGuildMember);
				continue;
			}
			GuildMemberMap::iterator it = m_memberMap.find(pGuildMember->GetChar_id());
			if (it != m_memberMap.end())
			{
				if (it->second != pGuildMember)
				{
					DeleteNetObject(it->second);
					m_memberMap.erase(it);
				}
			}		
			m_memberMap[pGuildMember->GetChar_id()] = pGuildMember;
			if (gGlobal.m_pSimClient->m_pCharacter &&
				pGuildMember->GetChar_id() == gGlobal.m_pSimClient->m_pCharacter->GetChar_id())
				gGlobal.m_pSimClient->m_ownGuildMember = pGuildMember;
		}
	}
	DeleteNetGroup(grp);
	ShowDetail();
}

UInt32 CGuild::GetGuildBidAmount()
{
	if (!gGlobal.m_pSimClient || !gGlobal.m_pSimClient->m_ownGuild)
		return 0;
	else
		return gGlobal.m_pSimClient->m_ownGuild->GetGuildPvPBid();
}

UInt32 CGuild::GetGuildPosition()
{
	if (!gGlobal.m_pSimClient || !gGlobal.m_pSimClient->m_ownGuildMember)
		return 255;
	else
		return gGlobal.m_pSimClient->m_ownGuildMember->GetPost();
}

UInt32 CGuild::GetGuildFund()
{
	if (!gGlobal.m_pSimClient || !gGlobal.m_pSimClient->m_ownGuild)
		return 0;
	else
		return gGlobal.m_pSimClient->m_ownGuild->GetFund();

}

UInt8 CGuild::GetGuildWarGroup()
{
	if (!gGlobal.m_pSimClient || !gGlobal.m_pSimClient->m_ownGuild)
		return 255;
	else
		return gGlobal.m_pSimClient->m_ownGuild->GetGuildPvPGroup();

}
