#pragma once
#ifndef _PARTY_H_
#define _PARTY_H_

#include "Proxy/Logic/Party_Proxy.h"
class CCharacter;
class CCharacterControl;

typedef StlVector<CCharacter*, ManagedAllocator<CCharacter*> > MemberVec;

class CParty : public Party_Proxy
{
private:
	typedef Party_Proxy Parent;

public:

	//CCharacterControl * m_TeamLeader;
	//StlMap<UInt32, CCharacterControl *> m_MemberList;

	CParty();
	virtual ~CParty();
	DEFINE_CLASS(Party_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, const UInt32 &memberId, const UInt8 &memberIdx, const Boolean &shown);
	virtual RPCResult CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, const Boolean &shown);
	virtual RPCResult CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject* member, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, const UInt8 &memberIdx);
	
	BOOL IsLeader();
	CCharacter* GetMember(UInt16 index);
	CCharacter* FindMember(UInt32 char_id);
	UInt32 FindMemberPos(String SrcName);
	VOID ShowMember();
	UInt16 GetRealMemNum();
	CCharacter* GetInPartyMember(UInt16 u16Index);

	/*VOID RemoveFromList(UInt32 Char_id);
	VOID AddToList(UInt32 Char_id);
	*/

//	UInt GetMemberCount();
	VOID RedrawUI();
	VOID SetUILeader(bool display, bool myself, UInt16 otherPos);
	VOID SetUIMemberPanel(bool display, UInt16 otherPos);
	VOID SetUIMemberImage(UInt16 otherPos, PCSTRING imageName, PCSTRING charName);
	VOID SetUIMemberName(UInt16 otherPos, PCSTRING charName);
	VOID SetUIMemberHPBar(UInt16 pos, UInt16 hp, UInt16 mhp, UInt16 maxhp);
	VOID SetUIMemberSPBar(UInt16 pos, UInt16 sp, UInt16 msp, UInt16 maxsp);

	VOID ResetNameColor();

	int SetChangePos(UInt8 ui8PreIndex,UInt8 ui8DestIndex);
};

#endif //_PARTY_H_
