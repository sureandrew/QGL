//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/ChannelType.h"
//-- Self
#include "Stub/Logic/Party.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/QuestSystem.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "Proxy/Service/MessageService_proxy.h"
#include <algorithm>
#include "Battle.h"
#include "Stub/Logic/ItemBag.h"

REGISTER_CLASSTYPE(CParty, Party_CLASSID);

CParty::CParty()
{
	m_pLeader = NULL;
	SetAvail(BOOLEAN_TRUE);
	SetSendMask(AllMask);
	ClearUpdateMask();
	SetLineup(13);
	m_leaderNetId = RPCNetID();

	m_nJoinPartyLevelDown = 0;
	m_nJoinPartyLevelUp = 0;

	SetLeaveTemporary(BOOLEAN_FALSE,BOOLEAN_FALSE,BOOLEAN_FALSE,BOOLEAN_FALSE,BOOLEAN_FALSE);

}

CParty::~CParty()
{
	CCharacter* member;

	CancelAllEvent();
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		member->SetParty(NULL);
		member->StopFollow();
		LeaveTeamChannel(member);
		CB_Destroy(member->GetPeer(), member->GetContext(), BOOLEAN_FALSE, BOOLEAN_FALSE);
	}
}

RPCResult CParty::Promote(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL || psess->pchar == NULL) {
		CB_Error(pPeer, context, 1, _T("CParty Promote Error: Client session or character not registered."));
		return (RPC_RESULT_FAIL);
	}

	if (psess->pchar != m_pLeader) {
		CB_Error(pPeer, context, 1, _T("CParty Promote Error: No right to promote"));
		return (RPC_RESULT_FAIL);
	}

	CCharacter* member = FindNetObject<CCharacter>(memberId);
	if (member == NULL) {
		CB_Error(pPeer, context, 1, _T("CParty Promote Error: member character is not registered."));
		return (RPC_RESULT_FAIL);
	}
	if (member->GetParty() != this && member->GetTempParty() != this) {
		CB_Error(pPeer, context, 1, _T("CParty Promote Error: member character is not in this party"));
		return (RPC_RESULT_FAIL);
	}
	if (member == m_pLeader) {
		CB_Error(pPeer, context, 1, _T("CParty Promote Error: leader cannot promote again"));
		return (RPC_RESULT_FAIL);
	}
	if (m_pLeader->IsState(CCharacter::ST_BATTLE))
	{
		CB_Error(pPeer, context, 1, _T("CParty Promote Error: cannot promote inside the battle"));
		return (RPC_RESULT_FAIL);
	}
	if (!Promote(member))
	{
		CB_Error(pPeer, context, 1, _T("CParty Promote Error: promote failed"));
		return (RPC_RESULT_FAIL);
	}
#ifdef LINEUP_SYSTEM_ON
	SetLineup(13);
	SetLineupkeypos(0);
	SendToMembers();
#endif

	return RPC_RESULT_OK;
}

RPCResult CParty::Kick(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL || psess->pchar == NULL)
	{
		CB_Error(pPeer, context, 1, _T("CParty Kick Error: Client session or character not registered."));
		return (RPC_RESULT_FAIL);
	}
	if (psess->pchar != m_pLeader)
	{
		CB_Error(pPeer, context, 1, _T("CParty Kick Error: No right to kick"));
		return (RPC_RESULT_FAIL);
	}
	if (m_pLeader->IsState(CCharacter::ST_BATTLE))
	{
		CB_Error(pPeer, context, 1, _T("CParty Kick Error: Cannot kick when inside a battle"));
		return (RPC_RESULT_FAIL);
	}

	CCharacter* member = FindNetObject<CCharacter>(memberId);
	if (member == NULL)
	{
		CB_Error(pPeer, context, 1, _T("CParty Kick Error: member character is not registered."));
		return (RPC_RESULT_FAIL);
	}
	if (member->GetParty() != this && member->GetTempParty() != this)
	{
		CB_Error(pPeer, context, 1, _T("CParty Kick Error: member character is not in this party"));
		return (RPC_RESULT_FAIL);
	}

	if (!Leave(member, true) && psess->pchar)
	{
		psess->pchar->CB_Error(pPeer, context, 1, _T("MSG_PARTY_CANNOT_LEAVE"));
		return (RPC_RESULT_FAIL);
	}
	return (RPC_RESULT_OK);
}

RPCResult CParty::Leave(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	CCharacter* pchar = psess ? psess->pchar : NULL;
	if (psess == NULL || pchar == NULL)
	{
		CB_Error(pPeer, context, 1, _FE("client session or character not registered."));
		
		return (RPC_RESULT_FAIL);
	}
	if (pchar->GetParty() == NULL && NULL == pchar->GetTempParty())
	{
		CB_Error(pPeer, context, 1, _FE("no party"));
		return (RPC_RESULT_FAIL);
	}
	if (pchar->GetParty() != this && pchar->GetTempParty() != this)
	{
		CB_Error(pPeer, context, 1, _FE("character is not in this party"));
		return (RPC_RESULT_FAIL);
	}
	if (!Leave(pchar))
	{
		pchar->CB_Error(pchar->GetPeer(), pchar->GetContext(), 1, _T("MSG_PARTY_CANNOT_LEAVE"));
		return (RPC_RESULT_FAIL);
	}

	return (RPC_RESULT_OK);
}

RPCResult CParty::Destroy(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL || psess->pchar == NULL)
	{
		CB_Error(pPeer, context, 1, _FE("client session or character not registered."));
		return (RPC_RESULT_FAIL);
	}
	if (psess->pchar->GetParty() == NULL)
	{
		CB_Error(pPeer, context, 1, _FE("no party"));
		return (RPC_RESULT_FAIL);
	}
	if (psess->pchar->GetParty() != this)
	{
		CB_Error(pPeer, context, 1, _FE("character is not in this party"));
		return (RPC_RESULT_FAIL);
	}
	if (psess->pchar != m_pLeader)
	{
		CB_Error(pPeer, context, 1, _FE("is not leader"));
		return (RPC_RESULT_FAIL);
	}
	if (psess->pchar->IsStartBattle())
	{
		CB_Error(pPeer, context, 1, _FE("battle starting"));
		return (RPC_RESULT_FAIL);
	}
	if (psess->pchar->IsState(CCharacter::ST_BATTLE))
	{
		CB_Error(pPeer, context, 1, _FE("insdie a battle"));
		return (RPC_RESULT_FAIL);
	}

	Destroy();
	return (RPC_RESULT_OK);
}

CCharacter* CParty::FindMember(UInt char_id)
{
	CCharacter * pchar;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		pchar = GetChild<CCharacter>(i);
		if (pchar && pchar->GetChar_id() == char_id)
			return pchar;
	}
	return NULL;
}

Boolean CParty::IsMember(CONST RPCNetID &netId)
{
	NetObject *obj;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		obj = GetChild(i);
		if (obj && obj->GetNetId() == netId)
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

Boolean CParty::IsMember(CCharacter *pchar)
{
	NetObject *obj;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		obj = GetChild(i);
		if (obj && obj == pchar)
			return BOOLEAN_TRUE;
	}
	return BOOLEAN_FALSE;
}

Boolean CParty::Join(CCharacter* pchar, Boolean bNotify, Boolean bForce, Boolean shown)
{
	if (pchar == NULL || pchar->HasParty() || !GetAvail() || GetCount() >= PARTY_MAX_MEMBER)
		return (BOOLEAN_FALSE);

	// can't join party in different map
	if (m_pLeader && (pchar->GetMap_id() != m_pLeader->GetMap_id() ||
		pchar->GetMap()->GetLine_id() != m_pLeader->GetMap()->GetLine_id() ||
		pchar->GetMap_ownerId() != m_pLeader->GetMap_ownerId()))
	{
		return BOOLEAN_FALSE;
	}

	// can't join party when leader or joining character running script
	if (m_pLeader && m_pLeader->GetScript()->IsRunning() && 
		!m_pLeader->GetScript()->IsItemScript() ||
		pchar->GetScript()->IsRunning())
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, 
			_FW("script running, session_id %s, char_id %d, script running %d, leader script running %d"),
			pchar->GetSessionId().GetString().c_str(), pchar->GetChar_id(), 
			pchar->GetScript()->IsRunning(), m_pLeader ? m_pLeader->GetScript()->IsRunning() : 0); 
		return BOOLEAN_FALSE;
	}

	if (bForce == BOOLEAN_FALSE && (m_pLeader && (!m_pLeader->IsInMap() || m_pLeader->IsStartBattle())))
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("party inside battle, leader %d(%s), requester %d(%s)"),
			m_pLeader->GetChar_id(), m_pLeader->GetSessionId().GetString().c_str(), pchar->GetChar_id(), pchar->GetSessionId().GetString().c_str());
		return (BOOLEAN_FALSE);
	}

	if (bForce == BOOLEAN_FALSE && (pchar->IsState(CCharacter::ST_BATTLE) || pchar->m_BattleNetID != RPCNetID() || pchar->IsStartBattle()))
	{
		TRACE_WARNDTL_5(GLOBAL_LOGGER, _FW("requester inside battle, char_id %d(%s), battle state %d, battle %d, starting battle %d"),
			pchar->GetChar_id(), pchar->GetSessionId().GetString().c_str(), pchar->GetBattleStateID(), pchar->m_BattleNetID, pchar->IsStartBattle()); 
		return BOOLEAN_FALSE;
	}

	if (bForce == BOOLEAN_FALSE && pchar->IsState(CCharacter::ST_MONITOR) &&
		(m_pLeader == NULL || !m_pLeader->IsInMap() || pchar->m_BattleNetID != m_pLeader->m_BattleNetID))
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("in diff battle, char_id %d"), pchar->GetChar_id()); 
		return BOOLEAN_FALSE;
	}

	if (pchar->GetBag() && pchar->GetBag()->HaveBusinessItemOrMoney())
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("have business item or money, char_id %d"), pchar->GetChar_id()); 
		return BOOLEAN_FALSE;
	}

	if (GetCount() == 0)
	{
		// no one in party
		//m_pLeader = pchar;
		SetLeader(pchar);
	}
	else
	{
		pchar->FollowTarget(m_pLeader);
		if (shown && pchar->GetQuests())
			pchar->GetQuests()->LeaderChanged();
	}
	pchar->CancelAllRequests();			// cancel all request after joined
	
	//refresh temporarily leave info
	CPartyCharInfo cpci;
	cpci.SetData(pchar);
	cpci.SetU16Index(GetCount());
	RefreshPartyCharInfo(&cpci,BOOLEAN_FALSE);
	SetM_bTempLeave(GetCount(),BOOLEAN_FALSE);
	// notify existing members
	CCharacter* member;
	RPCMask mask(CCharacter::m_initMask);
	mask.MaskBits(CCharacter::idMask, CCharacter::followIdMask, CCharacter::partyInfoMask, 
		CCharacter::HPMask, CCharacter::SPMask, -1);
	pchar->SetSendMask(mask);
	
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		// set member concern mask when joined
		member->SetConcernMask(pchar, CCharacter::m_memberMask);
		pchar->SetConcernMask(member, CCharacter::m_memberMask);
		if (bNotify && member->GetPeer())
		{
			SendAttr(member->GetPeer(),member->GetContext(),CParty::LeaveTemporaryMask);
			CB_Join(member->GetPeer(), member->GetContext(), pchar, shown);
			if (GetM_bTempLeave(i)) 
			{
				cpci.SetData(member);
				cpci.SetU16Index(i);
				cpci.SetSendMask(CPartyCharInfo::AllMask);
				pchar->CB_RefreshPartyCharInfo(pchar->GetPeer(),pchar->GetContext(),&cpci);
			}
		}
	}

	Add(pchar);
	pchar->SetParty(this, pchar == m_pLeader);
	SetOwnObject(pchar->GetContext().sessionId, this);
	
	// cancel all invite and request when full party
	if (m_pLeader && GetCount() >= PARTY_MAX_MEMBER)
		m_pLeader->CancelAllRequests();

	JoinTeamChannel(pchar);
	CalAvgLevel();

	TRACE_VERBOSEDTL_6(GLOBAL_LOGGER, _F(
		"Party Join: char_id %d, party netId %d:%d, count %d, leader char_id %d, bForce %d"), 
		pchar->GetChar_id(), GetNetId().srvId, GetNetId().objId, GetCount(),
		m_pLeader->GetChar_id(), (bForce)? 1 : 0 );

	ShowMember();

	return BOOLEAN_TRUE;
}

VOID CParty::GetMemberIds(UInt32Vector &memberIds)
{
	memberIds.clear();
	CCharacter *member;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member) {
			memberIds.push_back(member->GetChar_id());
		}
	}
}

VOID CParty::GetSessionIds(SessionIDVector &sessIds)
{
	sessIds.clear();
	CCharacter *member;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member) {
			sessIds.push_back(member->GetSessionId());
		}
	}
}

RPCResult CParty::JoinChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	CCharacter * pChar = FindMember(char_id);
	if (pChar)
		JoinTeamChannel(pChar);
	return RPC_RESULT_OK;
}
//join the message channel for this team
VOID CParty::JoinTeamChannel(CCharacter * pChar)
{
	if (m_pLeader == NULL) return;

	if (gGlobal.pMessageService)
	{
		String NewChannelName;

		NewChannelName.Format(_T("%s%s%d"), ChannelPrefix[CHANNELTYPE_PARTY], 
			Del, m_pLeader->GetChar_id());

		if (gGlobal.pMessageService->pPeer)
			MessageService_Proxy::JoinMessageChannel(gGlobal.pMessageService->pPeer, 
				pChar->GetContext(), pChar->GetChar_id(), 
				pChar->GetNickName(), NewChannelName, 
				CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);
	}
}

//leave team channel
VOID CParty::LeaveTeamChannel(CCharacter * pChar)
{
	if (m_pLeader == NULL) return;

	if (gGlobal.pMessageService)
	{
		String NewChannelName;

		NewChannelName.Format(_T("%s%s%d"), ChannelPrefix[CHANNELTYPE_PARTY], 
			Del, m_pLeader->GetChar_id());

		if (gGlobal.pMessageService->pPeer)
			MessageService_Proxy::LeaveChannel(gGlobal.pMessageService->pPeer, 
				pChar->GetContext(), pChar->GetChar_id(), NewChannelName);
	}
}

Boolean CParty::Leave(CCharacter* pchar, Boolean bKick, Boolean shown)
{
	if (pchar == NULL || m_pLeader == NULL || !GetAvail() || !IsMember(pchar))
		return BOOLEAN_FALSE;

	// can't leave party when running script except kicking
	if (!bKick && pchar->GetScript()->IsRunning())
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, 
			_FW("script running, session_id %s, char_id %d, script running %d"),
			pchar->GetSessionId().GetString().c_str(), pchar->GetChar_id(), 
			pchar->GetScript()->IsRunning()); 
		return BOOLEAN_FALSE;
	}

	if (pchar->IsState(CCharacter::ST_BATTLE) || pchar->IsStartBattle())
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, 
			_FW("inside battle, session_id %s, char_id %d, battle_state %d, starting battle %d"),
			pchar->GetSessionId().GetString().c_str(), pchar->GetChar_id(), 
			pchar->GetBattleStateID(), pchar->IsStartBattle()); 
		return BOOLEAN_FALSE;
	}

	if (m_pLeader->IsStartBattle())
	{
		TRACE_WARNDTL_6(GLOBAL_LOGGER, 
			_FW("leader inside battle, char_id %d(%s), leader %d(%s), battle_state %d, starting battle %d"),
			pchar->GetChar_id(), pchar->GetSessionId().GetString().c_str(), 
			m_pLeader->GetChar_id(), m_pLeader->GetSessionId().GetString().c_str(),
			m_pLeader->GetBattleStateID(), m_pLeader->IsStartBattle()); 
		return BOOLEAN_FALSE;
	}

	if (pchar == m_pLeader)
	{
		if (GetCount() > 2)
		{
			CCharacter* pchar2 = NULL;
			for (int iCnt = 1;iCnt < GetCount();iCnt++) 
			{
				pchar2 = GetChild<CCharacter>(iCnt);
				if (NULL != pchar2)
					break;
			}
#ifdef LINEUP_SYSTEM_ON
			SetLineup(13);
			SetLineupkeypos(0);
			SendToMembers();
#endif
			Promote(pchar2, BOOLEAN_TRUE, shown);
		}
		else
		{
			Destroy();							// character destroy party
			return BOOLEAN_TRUE;
		}
	}
	if (NULL != pchar->GetParty()) 
		pchar->StopFollow();

	// character leave party
	CCharacter* member;
	pchar->ClearSendMask();
	Boolean bNeedChangeState =BOOLEAN_FALSE;
	for (UInt16 j = 0; j < GetCount(); ++j)
	{
		member = GetChild<CCharacter>(j);
		if (member == NULL) continue;
		if (member == pchar)
		{
			SetM_bTempLeave(j,BOOLEAN_FALSE);
			if (j < GetCount()-1) 
				bNeedChangeState =BOOLEAN_TRUE;
		}
		else if (bNeedChangeState && j > 1) 
			SetM_bTempLeave(j-1,GetM_bTempLeave(j));
	}
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		if (member == pchar)
			CB_Destroy(member->GetPeer(), member->GetContext(), bKick, shown);
		else {
			// unset member concern mask when left
			member->SetConcernMask(pchar, CCharacter::m_charMask);
			pchar->SetConcernMask(member, CCharacter::m_charMask);
			pchar->SetSendMask(CCharacter::m_charMask);
			SendAttr(member->GetPeer(),member->GetContext(),CParty::LeaveTemporaryMask);
			CB_Leave(member->GetPeer(), member->GetContext(), pchar, bKick, shown);
		}
	}
	pchar->SetParty(NULL);
	pchar->SetTempParty();
	Remove(pchar);
	UnsetOwnObject(pchar->GetContext().sessionId, this);
	LeaveTeamChannel(pchar);
	CalAvgLevel();

	TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, 
		_T("Party Leave: char_id %d, party netId %d:%d, count %d, leader char_id %d"), 
		pchar->GetChar_id(), GetNetId().srvId, GetNetId().objId, GetCount(),
		m_pLeader->GetChar_id());
	ShowMember();

	return BOOLEAN_TRUE;
}

VOID CParty::Destroy(Boolean shown)
{
	CCharacter* member;

	if (m_pLeader)
	{
		m_pLeader->CancelAllRequests();
	}

	TRACE_INFODT_1(GLOBAL_LOGGER, _F("members %d, "), GetCount());

	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;

		TRACE_INFO_2(GLOBAL_LOGGER, _F("idx %d(%s), "),
			i, member->GetSessionId().GetString().c_str());

		member->SetParty(NULL);
		if (!GetM_bTempLeave(i)) 
			member->StopFollow();
		member->SetTempParty();
		// notify quest system leader changed
		if (shown && GetCount() > 1 && member->GetQuests())
			member->GetQuests()->LeaderChanged();
		LeaveTeamChannel(member);
		UnsetOwnObject(member->GetContext().sessionId, this);
		if (member->GetPeer())
			CB_Destroy(member->GetPeer(), member->GetContext(), false, shown);
		for (UInt16 j = i + 1; j < GetCount(); ++j)
		{
			CCharacter* pchar = GetChild<CCharacter>(j);
			// unset member concern mask when left
			if (pchar)
			{
				member->SetConcernMask(pchar, CCharacter::m_charMask);
				pchar->SetConcernMask(member, CCharacter::m_charMask);
			}
		}
	}
	
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));

	//m_pLeader = NULL;
	SetLeader(NULL);
	Clear();
	CParty* party = this;
	DeleteNetGroup(party);
}

Boolean CParty::Promote(CCharacter* pchar, Boolean bLeft, Boolean shown)
{
	if (pchar == NULL || m_pLeader == NULL || pchar == m_pLeader || !GetAvail() || !IsMember(pchar))
		return BOOLEAN_FALSE;

	// can't promote other to leader when running script
	if (m_pLeader->GetScript()->IsRunning())
	{
		TRACE_WARNDTL_3(GLOBAL_LOGGER, 
			_FW("leader script running, session_id %s, char_id %d, script running %d"),
			m_pLeader->GetSessionId().GetString().c_str(), m_pLeader->GetChar_id(), 
			m_pLeader->GetScript()->IsRunning()); 
		return BOOLEAN_FALSE;
	}

	CCharacter *member;
	UInt8 oldIndex = 0;

	// all leave team channel
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL)
			continue;
		if (member == pchar)
			oldIndex = (UInt8) i;
		LeaveTeamChannel(member);
	}

	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("CParty [Promote Leader Successful] old leader char_id=%d, new leader char_id=%d"), m_pLeader->GetChar_id(), pchar->GetChar_id());
	
	m_pLeader->CancelAllRequests();			// cancel all join requests and invitations
	CCharacter* pPrevLeader = m_pLeader;
	if (GetM_bTempLeave(oldIndex))
		SetTempLeaveAll();
	m_pLeader->SetParty(this, FALSE);		// set the party info
	pchar->SetParty(this, TRUE);
	pchar->SetTempParty();
	Replace(oldIndex, m_pLeader);			// swap leader and new promote character
	Replace(0, pchar);						// new leader should be index 0
	//m_pLeader = pchar;
	SetLeader(pchar);
	if (GetM_bTempLeave(oldIndex))
	{
		pPrevLeader->SetParty(NULL);
		pPrevLeader->SetTempParty(this);
	}
	// join new team channel and follow new leader
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		if (member != pchar)
		{
			if (!GetM_bTempLeave(i))
				member->FollowTarget(pchar);
		}
		else
			member->StopFollow();
		// notify quest system leader changed
		if (shown && member->GetQuests())
			member->GetQuests()->LeaderChanged();
		if (!bLeft || i != oldIndex)
		{
			if (member->GetPeer())
			{
				CB_LeaderChanged(member->GetPeer(), member->GetContext(), m_pLeader->GetChar_id(), oldIndex, shown);
			}
			JoinTeamChannel(member);
		}
	}
	
	return BOOLEAN_TRUE;
}

Boolean CParty::Regroup(CCharacter* pchar, const UInt32Vector &memberIdList)
{
	if (pchar == NULL || pchar->HasParty() || GetAvail() || GetCount() >= PARTY_MAX_MEMBER)
		return (BOOLEAN_FALSE);

	// cannot regroup party in different map
	if (m_pLeader && (pchar->GetMap_id() != m_pLeader->GetMap_id() ||
		pchar->GetMap()->GetLine_id() != m_pLeader->GetMap()->GetLine_id() ||
		pchar->GetMap_ownerId() != m_pLeader->GetMap_ownerId()))
	{
		return BOOLEAN_FALSE;
	}

	// check membership and find index position for this character in current party
	bool found = false;
	UInt8 index = 0;				// member index
	for (UInt32Vector::const_iterator it = memberIdList.begin(); it != memberIdList.end(); ++it)
	{
		if (*it == pchar->GetChar_id())
		{
			found = true;
			break;
		}
		else if (FindMember(*it))
			++index;
	}
	if (!found)					// not a party member
		return BOOLEAN_FALSE;

	if (GetCount() == 0)
	{
		// no one in party
		//m_pLeader = pchar;
		SetLeader(pchar);
	}
	else if (index == 0)
	{
		// become new temp leader first
		if (m_pLeader)
			m_pLeader->SetParty(this, FALSE);
		//m_pLeader = pchar;
		SetLeader(pchar);
	}
	Insert(pchar, (UInt16) index);

	// notify existing members
	bool bComplete = (GetCount() >= (UInt16) memberIdList.size());
	CCharacter* member;
	RPCMask mask(CCharacter::m_initMask);
	mask.MaskBits(CCharacter::idMask, CCharacter::followIdMask, CCharacter::partyInfoMask, 
		CCharacter::HPMask, CCharacter::SPMask, -1);
	pchar->SetSendMask(mask);
	if (bComplete != GetAvail())
		SetAvail(bComplete);

	// send party regroup info and rebuild follower list
	m_pLeader->ClearFollower();
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		if (i == 0)
			member->StopFollow();
		else
			member->FollowTarget(m_pLeader);
		if (i != index)
		{
			// set member concern mask when joined
			member->SetConcernMask(pchar, CCharacter::m_memberMask);
			pchar->SetConcernMask(member, CCharacter::m_memberMask);
			member->SetSendMask(mask);
			if (IsUpdated())
				SendAttr(member->GetPeer(), member->GetContext(), GetUpdateMask());
			CB_Regroup(member->GetPeer(), member->GetContext(), pchar, index);
		}
	}
	ClearUpdateMask();

	pchar->SetParty(this, pchar == m_pLeader);
	SetOwnObject(pchar->GetContext().sessionId, this);
	JoinTeamChannel(pchar);
	CalAvgLevel();

	// send party to character
	pchar->CB_JoinParty(pchar->GetPeer(), pchar->GetContext(), this);

	TRACE_VERBOSEDTL_6(GLOBAL_LOGGER, _F(
		"char_id %d, party netId %d:%d, count %d, need %d, leader char_id %d"), 
		pchar->GetChar_id(), GetNetId().srvId, GetNetId().objId, GetCount(),
		memberIdList.size(), m_pLeader->GetChar_id());
	ShowMember();

	return BOOLEAN_TRUE;
}

VOID CParty::SetAvail(Boolean avail)
{
	Parent::SetAvail(avail);
	if (avail)
		CancelUniqEvent(EVT_REGROUP_TIMEOUT);
	else
		RaiseUniqEventID(EVT_REGROUP_TIMEOUT, gConfigure.partyRegroupTime);
}

VOID CParty::PreSend(Boolean bClear)
{
	RPCMask mask;
	CCharacter *member;

	if (!bClear)
		mask.MaskBits(CCharacter::idMask, CCharacter::infoMask, 
			CCharacter::HPMask, CCharacter::SPMask, CCharacter::partyInfoMask, -1);

	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		member->SetSendMask(mask);
	}

	SetSendMask(NetObject::AllMask);
}

VOID CParty::SendToMembers()
{
	RPCMask mask;
	CCharacter *member;

	SetSendMask(NetObject::AllMask);
	mask.MaskBits(CCharacter::HPMask, CCharacter::SPMask, -1);

	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		member->SetSendMask(mask);
	}

	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		if (member->GetPeer())
			member->CB_JoinParty(member->GetPeer(), member->GetContext(), this);
	}
}

VOID CParty::CalAvgLevel()
{
	CCharacter *member;
	UInt16 totalMember = 0;
	m_avgLevel = 0;

	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member && !GetM_bTempLeave(i)) {
			++totalMember;
			m_avgLevel += member->GetLevel();
		}
	}
	m_avgLevel /= totalMember;
}

UInt8 CParty::GetSexCount(int sex)
{
	UInt8 num = 0;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CCharacter* member = GetChild<CCharacter>(i);
		if (member && member->GetSex() == sex)
			++num;
	}

	return num;
}

UInt8 CParty::GetJobCount(int job)
{
	UInt8 num = 0;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CCharacter* member = GetChild<CCharacter>(i);
		if (member && member->GetCclass() == job)
			++num;
	}

	return num;
}

Boolean CParty::IsAllEntered()
{
	if (!GetAvail())
		return BOOLEAN_FALSE;

	// check all members has finished entering
	CCharacter* pchar;
	for (UInt8 i = 0; i < GetCount(); ++i)
	{
		pchar = GetChild<CCharacter>(i);
		if (pchar && !pchar->IsEnterDone())
			return BOOLEAN_FALSE;
	}
	return BOOLEAN_TRUE;
}

VOID CParty::ShowMember()
{
	CCharacter *pchar;

	TRACE_VERBOSEDTL_4(GLOBAL_LOGGER, _F(
		"Party ShowMember: id %d, netId %d:%d, count %d: "), 
		GetID(), GetNetId().srvId, GetNetId().objId, GetCount());

	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		pchar = GetChild<CCharacter>(i);
		if (pchar)
		{
			TRACE_VERBOSEDTL_7(GLOBAL_LOGGER, _F(
				"Member %d: char_id %d, hasParty %d, followId %d, map_id %d, curPos %d:%d"),
				i, pchar->GetChar_id(), pchar->GetHasParty(), pchar->GetFollowId(),
				pchar->GetMap_id(), pchar->GetCurPosX(), pchar->GetCurPosY());
		}
	}
}

void CParty::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_REGROUP_TIMEOUT:				// regroup timeout, make party avail
		if (!GetAvail())
		{
			Parent::SetAvail(BOOLEAN_TRUE);
			m_pLeader = GetLeader();
			if(m_pLeader == NULL)
			{
				TRACE_ERRORDTL(GLOBAL_LOGGER, _F("m_pLeader == NULL id %d, netId %d:%d, count %d: "), 
					GetID(), GetNetId().srvId, GetNetId().objId, GetCount());
				return;
			}
			CCharacter* member = NULL;
			m_pLeader->ClearSendMask();
			for (UInt16 i = 0; i < GetCount(); ++i)
			{
				member = GetChild<CCharacter>(i);
				if (member)
				{
					member->CB_Error(member->GetPeer(), member->GetContext(), 1, 
						_T("MSG_PARTY_REGROUP_FAIL"));
					SendAttr(member->GetPeer(), member->GetContext(), AllMask);
				}
			}
			ClearUpdateMask();
			// try enter instance for this party
			CMap* pMap = m_pLeader->GetMap();
			if (pMap && pMap->GetInst_id() != 0)
			{
				m_pLeader->OnEnterInst(pMap->GetInst_id(), pMap->GetOwner_id());
			}
		}
		break;
	}
}

UInt8 CParty::GetCharactorIndexById(UInt32 uiCharId)
{
	CCharacter *pchar;
	for (UInt8 i = 0; i < GetCount(); ++i)
	{
		pchar = GetChild<CCharacter>(i);
		if (NULL != pchar && pchar->GetChar_id() == uiCharId && !GetM_bTempLeave(i))
		{
			return i;
		}
	}

	return TEAMMATE_MAX;
}

int CParty::SetChangePos(UInt8 ui8PreIndex,UInt8 ui8DestIndex)
{
	int iRetCode = -3;
	UInt16 iCurMemNum = GetCount();
	if (ui8PreIndex != 0 
		&& ui8PreIndex != ui8DestIndex 
		&& ui8DestIndex != 0
		&& ui8PreIndex < iCurMemNum
		&& ui8DestIndex < iCurMemNum) {
			CCharacter* pPreChar = GetChild<CCharacter>(ui8PreIndex);
			CCharacter* pDestChar = GetChild<CCharacter>(ui8DestIndex);
			if (NULL != pPreChar && NULL != pDestChar) {
				Replace(ui8DestIndex, pPreChar);
				Replace(ui8PreIndex, pDestChar);
				Boolean bState = GetM_bTempLeave(ui8PreIndex);
				SetM_bTempLeave(ui8PreIndex,GetM_bTempLeave(ui8DestIndex));
				SetM_bTempLeave(ui8DestIndex,bState);
			}
			iRetCode = 0;
			for (UInt16 i=0;i<iCurMemNum;i++) {
				CCharacter* pChar = GetChild<CCharacter>(i);
				if (NULL != pChar) {
					SendAttr(pChar->GetPeer(),pChar->GetContext(),CParty::LeaveTemporaryMask);
					pChar->CB_SetChangePos(pChar->GetPeer(),
										pChar->GetContext(),
										iRetCode,ui8PreIndex,ui8DestIndex);
				}
			}
	}
	return iRetCode;
}

RPCResult CParty::ChangeDesc(LPCPEER pPeer, RPCContext &context, const String &desc)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL || psess->pchar == NULL)
	{
		CB_Error(pPeer, context, 1, _T("CParty Description Error: Client session or character not registered."));
		return (RPC_RESULT_FAIL);
	}
	if (psess->pchar != m_pLeader)
	{
		CB_Error(pPeer, context, 1, _T("CParty Description Error: No right to kick"));
		return (RPC_RESULT_FAIL);
	}
	SetDesc(desc);

	CCharacter *member;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		if (member->GetPeer())
		{
			member->CB_ChangeDesc(member->GetPeer(), member->GetContext(), this);
		}
	}

	return RPC_RESULT_OK;
}
Boolean CParty::SetTempLeave(UInt32 uId)
{
	Boolean bFind = BOOLEAN_FALSE;
	CCharacter* pLeaveTempChar = NULL;
	for (UInt16 i=0;i<GetCount();i++) 
	{
		CCharacter* pChar= GetChild<CCharacter>(i);
		if (NULL != pChar) 
		{
			if (pChar->GetChar_id() == uId) 
			{
				pLeaveTempChar = pChar;
				pLeaveTempChar->StopFollow();
				SetM_bTempLeave(i,BOOLEAN_TRUE);
				bFind = BOOLEAN_TRUE;
				break;
			}
		}
	}

	if (bFind) 
	{
		pLeaveTempChar->ClearSendMask();
		for (UInt16 i=0;i<GetCount();i++) 
		{
			CCharacter* pChar= GetChild<CCharacter>(i);
			if (NULL != pChar) 
			{
				if (pLeaveTempChar != pChar) 
				{
					pChar->SetConcernMask(pLeaveTempChar, CCharacter::m_charMask);
					pLeaveTempChar->SetConcernMask(pChar, CCharacter::m_charMask);
				}
				SendAttr(pChar->GetPeer(),pChar->GetContext(),CParty::LeaveTemporaryMask);
				pChar->CB_LeaveTeamTemporarily(pChar->GetPeer(),pChar->GetContext(),0,uId);
			}
		}
		CalAvgLevel();
	}
	return bFind;
}
void CParty::SetTempLeaveAll()
{
	for (UInt16 i=0;i<GetCount();i++) 
	{
		CCharacter* pChar= GetChild<CCharacter>(i);
		if (NULL != pChar) 
		{
			if (!pChar->IsLeader()) 
			{
				pChar->LeaveTeamTemporarily(pChar->GetPeer(),pChar->GetContext());
			}
		}
	}
}
UInt16 CParty::GetRealMemNum()
{
	UInt16 uRet = 0;
	for (UInt8 i=0;i<GetCount();i++) 
	{
		if (!GetM_bTempLeave(i))
			uRet++;
	}
	return uRet;
}

CCharacter* CParty::GetInPartyMember(UInt16 u16Index)
{
	if (u16Index > GetRealMemNum()) 
		return NULL;
	UInt16 j=0;
	CCharacter* pChar= NULL;
	for (UInt16 i=0;i<GetCount();i++) 
	{
		if (!GetM_bTempLeave(i))
		{
			if (j == u16Index) 
			{
				pChar = GetChild<CCharacter>(i);
				break;
			}
			j++;
		}
	}
	return pChar;
}

UInt32 CParty::ReturnParty(CCharacter* pChar)
{
	if (IsMember(pChar) && m_pLeader != pChar) 
	{
		// can't join party in different map
		if (m_pLeader && (m_pLeader->GetMap() != NULL) && (pChar->GetMap() != NULL) 
			&& (pChar->GetMap_id() != m_pLeader->GetMap_id() ||
			pChar->GetMap()->GetLine_id() != m_pLeader->GetMap()->GetLine_id() ||
			pChar->GetMap_ownerId() != m_pLeader->GetMap_ownerId()))
		{
			return 2;
		}

		// can't join party when leader or joining character running script
		if (m_pLeader && m_pLeader->GetScript()->IsRunning() && 
			!m_pLeader->GetScript()->IsItemScript() ||
			pChar->GetScript()->IsRunning())
		{
			return 3;
		}

		if (m_pLeader && (!m_pLeader->IsInMap() || m_pLeader->IsStartBattle()))
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("party inside battle, leader %d(%s), requester %d(%s)"),
				m_pLeader->GetChar_id(), m_pLeader->GetSessionId().GetString().c_str(), pChar->GetChar_id(), pChar->GetSessionId().GetString().c_str());
			return 4;
		}

		if (pChar->IsState(CCharacter::ST_MONITOR) &&
			(m_pLeader == NULL || !m_pLeader->IsInMap() || pChar->m_BattleNetID != m_pLeader->m_BattleNetID))
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("in diff battle, char_id %d"), pChar->GetChar_id()); 
			return 5;
		}

		if (pChar->GetBag() && pChar->GetBag()->HaveBusinessItemOrMoney())
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("have business item or money, char_id %d"), pChar->GetChar_id()); 
			return 6;
		}
		
		Int x = 0;		
		x = pChar->GetCurPosX() - m_pLeader->GetCurPosX();		
		x *= x;

		Int y = 0;
		y = pChar->GetCurPosY() - m_pLeader->GetCurPosY();
		y *= y;

		if ( (x + y) > 576)
		{
			return 7;
		}

		pChar->FollowTarget(m_pLeader);
		if (pChar->GetQuests())
			pChar->GetQuests()->LeaderChanged();
		pChar->CancelAllRequests();			// cancel all request after joined

		// notify existing members
		CCharacter* member;
		RPCMask mask(CCharacter::m_initMask);
		mask.MaskBits(CCharacter::idMask, CCharacter::followIdMask, CCharacter::partyInfoMask, 
			CCharacter::HPMask, CCharacter::SPMask, -1);
		pChar->SetSendMask(mask);

		for (UInt16 i = 0; i < GetCount(); ++i)
		{
			member = GetChild<CCharacter>(i);
			if (member == NULL) continue;
			if (member == pChar)
			{
				SetM_bTempLeave(i,BOOLEAN_FALSE);
				break;
			}
		}

		for (UInt16 j=0;j<GetCount();j++) 
		{
			member = GetChild<CCharacter>(j);
			if (member == NULL) continue;
			if (member->GetPeer())
			{
				SendAttr(member->GetPeer(),member->GetContext(),CParty::LeaveTemporaryMask);
				pChar->SetSendMask(mask);
				member->CB_ReturnParty(member->GetPeer(),member->GetContext(),0,pChar);
			}
			if (member != pChar) 
			{
				member->SetConcernMask(pChar, CCharacter::m_memberMask);
				pChar->SetConcernMask(member, CCharacter::m_memberMask);
				member->SendAttr(pChar->GetPeer(),pChar->GetContext(),mask);
			}
		}

		CalAvgLevel();
	}
	return 0;
}
VOID CParty::RefreshPartyCharInfo(CPartyCharInfo* pInfo,Boolean bTempLeave)
{
	if (NULL == pInfo)
		return;
	for (UInt16 i = 0; i < GetCount(); ++i)
	{
		CCharacter* member = GetChild<CCharacter>(i);
		if (member == NULL) continue;
		// set member concern mask when joined
		if (member->GetChar_id() != pInfo->GetU32CharId())
		{
			pInfo->SetSendMask(CPartyCharInfo::AllMask);
			if (bTempLeave) 
				member->CB_RefreshPartyCharInfo(member->GetPeer(),member->GetContext(),pInfo);
			else if (GetM_bTempLeave(i)) 
				member->CB_RefreshPartyCharInfo(member->GetPeer(),member->GetContext(),pInfo);
		}
	}
}