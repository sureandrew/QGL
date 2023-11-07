#pragma once
#ifndef _MENUPARTY_H_
#define _MENUPARTY_H_


#include "Proxy/Logic/MapActor.h"
#include "Proxy/Logic/Character_Proxy.h"
#include "Proxy/Logic/Map.h"


class CParty;
class CCharacter;

class CMenuPartyProcess
{
private:
	//typedef StlVector<CCharacter*, ManagedAllocator<CCharacter*> > TargetList;
	typedef StlVector<CParty*, ManagedAllocator<CParty*> > PartyList;

	typedef StlMap<RPCNetID, CCharacter*, std::less<RPCNetID>, ManagedAllocator<std::pair<RPCNetID, CCharacter*> > > NetIDCharMap;

	NetIDCharMap	m_ReqJoinList;

	//TargetList		m_RequestList;

	PartyList		m_PartyList;
	bool			m_bInviting;
	bool			m_bJoining;
	bool			m_bAutoAccept;
	int				m_iPartyIndex;
	int				m_iRequestIndex;
	UInt8           m_iPreChangeIndex;

public:
	CMenuPartyProcess()
	{
		m_bInviting = false;
		m_bJoining = false;
		m_bAutoAccept = true;
		m_iPartyIndex = 0;
		m_iRequestIndex = 0;
		m_ReqJoinList.clear();
		m_iPreChangeIndex = UCHAR_MAX;
	}

	virtual ~CMenuPartyProcess();

	void AddPartyList(CParty* pParty);
	void ClearPartyList();
	INLINE void SetInviting(bool invite){m_bInviting = invite;}
	bool IsInviting(){return m_bInviting;};
	void InviteComing(CParty* pParty);

	void UpdatePartyPanel(CParty* pParty);
	void ShowPartyByIndex(int index=-1);
	void InviteAccept();
	void InviteReject();
	void ResetPartyPanel();

	int	 GetPartyListSize()	{return (int)m_PartyList.size();}
	void RemoveParty(RPCNetID netId);


	void AddRequestList(CCharacter* pTarget);
	void JoinRequest(CCharacter* pTarget);
	bool IsJoinRequestExist(){return m_bJoining;};

	INLINE void SetJoining(bool joinRequest){m_bJoining = joinRequest;}
	int GetRequestListSize(){return (int)m_ReqJoinList.size();}//m_RequestList.size();}
	void UpdateApplicantMenu();
	void JoinAccept(int index);
	void JoinReject(int index);

	void ResetApplicantPanel();
	void RemoveRequest(RPCNetID netId);
	void ClearAllRequest();

	void ClearAllInvite();

	void ShowOtherParty(int next); //1: show next party, 0: show prev party
	void ApplicantPanelScroll(int scroll); //1: scroll up, 0: scroll down

	void ShowAllPanel();
	void HideAllPanel();

	void ShowPartyButton();
	void ResetPartyButton();
	void ApplicantPrivateCall(int index);

	void TeammatePrivateCall(int index);
	void TeammateKick(int index);
	void TeammatePromote(int index);

	bool IsInRequestList(RPCNetID netID);
	bool IsInInviteList(UInt32 char_id);

	void JoinRejectByChar(RPCNetID netID);
	void InviteRejectByCharID(UInt32 char_id);

	void InsertCharToOtherParty(RPCNetID partyID, CCharacter* pChar);
	void ShowTeammatePopUp(UInt8 index);
	void AutoAcceptCheckBox(bool on);
	void SelectTeammatePopUp(UInt8 popSelect, UInt8 index);
    
	void SetChangePos(UInt8 iIndex);
	void ChangePartyDesc();
};




#endif 