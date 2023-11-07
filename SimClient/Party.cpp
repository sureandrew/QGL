//-- Common
#include "Common.h"
//-- Self
#include "SimClient/Party.h"
//-- Library
#include "SimClient/Global.h"
#include "SimClient/Character.h"

REGISTER_CLASSTYPE(CParty, Party_CLASSID);

CParty::~CParty()
{
	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("CParty %d,%d deleted"), m_netId.srvId, m_netId.objId);
}

RPCResult CParty::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CParty Error: errcode %d, %s"), errCode, errMsg.c_str());
	return RPC_RESULT_OK;
}

RPCResult CParty::CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, 
								   const UInt32 &memberId, const UInt8 &memberIdx,
								   const Boolean &shown)
{
	TRACE_VERBOSEDTL(GLOBAL_LOGGER, _F("CParty::CB_LeaderChanged"));

	if (memberIdx == 0 || memberIdx >= GetCount() || GetCount() < 1)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty CB_LeaderChanged Error: member index out range"));
		return RPC_RESULT_FAIL;
	}

	CCharacter* oldLeader = GetChild<CCharacter>(0);
	CCharacter* newLeader = GetChild<CCharacter>(memberIdx);
	if (oldLeader == NULL || newLeader == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CParty CB_LeaderChanged Error: invalid character"));
		return RPC_RESULT_FAIL;
	}
	Replace(0, newLeader);
	Replace(memberIdx, oldLeader);

	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Party Leader changed: char_id %d"), newLeader->GetChar_id());

	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, 
						  const Boolean &shown)
{
	CCharacter *pchar = CastNetObject<CCharacter>(member);
	TRACE_ENSURE(pchar);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CParty::CB_Join: char_id %d"), pchar->GetChar_id());

	Add(member);
	ShowMember();
	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject * member, 
						   const bool &kick, const Boolean &shown)
{
	CCharacter *pchar = CastNetObject<CCharacter>(member);
	TRACE_ENSURE(pchar);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CParty::CB_Leave: char_id %d"), pchar->GetChar_id());

	Remove(member);
	ShowMember();
	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, 
							 const Boolean &shown)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::CB_Destroy"));

	TRACE_ENSURE(gGlobal.m_pSimClient);
	Clear();
	DeleteNetGroup(gGlobal.m_pSimClient->m_pCharacter->m_pParty);
	gGlobal.m_pSimClient->m_lastMoveX = 0;
	gGlobal.m_pSimClient->m_lastMoveY = 0;

	return RPC_RESULT_OK;
}

RPCResult CParty::CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, 
							 const UInt8 &memberIdx)
{
	CCharacter *pchar = CastNetObject<CCharacter>(member);
	TRACE_ENSURE(pchar);
	TRACE_INFODTL_3(GLOBAL_LOGGER, 
		_T("CParty::CB_Regroup: char_id %d, index %d, avail %d"),
		pchar->GetChar_id(), memberIdx, GetAvail());

	Insert(member, memberIdx);
	ShowMember();

	return RPC_RESULT_OK;
}

/*
UInt CParty::GetMemberCount()
{
	return (UInt)m_member.size();
}

BOOL CParty::AddMember(CCharacter* newmember)
{
	if( newmember )
	{
        if( FindMember(newmember->GetChar_id()) )
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::AddMember, new member is added already"));
		else
		{
			m_member.push_back(newmember);
			TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::AddMember, new member is added successfully"));
			return TRUE;
		}
	}
	else
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::AddMember, new member is invalid"));
	}
	return FALSE;
}

BOOL CParty::RemoveMember(CCharacter* member)
{
	if( member )
	{
		for( MemberVec::iterator itr = m_member.begin(); itr!=m_member.end(); ++itr )
		{
			if( member->GetChar_id()==(*itr)->GetChar_id() )
			{
				DeleteNetObject(*itr);
				m_member.erase(itr);
				TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::RemoveMember, member is removedsuccessfully"));
				return true;
			}
		}
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CParty::RemoveMember, member not found in party"));
	}
	else
	{
		TRACE_INFODLT(GLOBAL_LOGGER, _F("CParty::RemoveMember, member is invalid"));
	}
	return FALSE;
}
*/

CCharacter* CParty::GetMember(UInt16 index)
{
	if (index < GetCount())
		return (GetChild<CCharacter >(index));
	else
		return NULL;
}

CCharacter* CParty::FindMember(UInt32 char_id)
{
	CCharacter* pchar;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		 pchar = GetChild<CCharacter>(i);
		 if (pchar && pchar->GetChar_id() == char_id)
			 return pchar;
	}
	return NULL;
}

VOID CParty::ShowMember()
{
	CCharacter *pchar;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		 pchar = GetChild<CCharacter>(i);
		if (pchar)
		{
			TRACE_INFODTL_8(GLOBAL_LOGGER, _F("Member %d: char_id %d, hp %d:%d:%d, sp %d:%d:%d"),
				i, pchar->GetChar_id(),
				pchar->GetHP(), pchar->GetMHP(), pchar->GetMaxHP(),
				pchar->GetSP(), pchar->GetMSP(), pchar->GetMaxSP());
		}
	}
}
