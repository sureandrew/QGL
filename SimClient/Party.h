#pragma once
#ifndef _PARTY_H_
#define _PARTY_H_

#include "Proxy/Logic/Party_Proxy.h"

class CCharacter;

typedef StlVector<CCharacter*, ManagedAllocator<CCharacter*> > MemberVec;

class CParty : public Party_Proxy
{
private:
	typedef Party_Proxy Parent;

public:
	DEFINE_CLASS(Party_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, const UInt32 &memberId, const UInt8 &memberIdx, const Boolean &shown);
	virtual RPCResult CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, const Boolean &shown);
	virtual RPCResult CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject* member, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, const UInt8 &memberIdx);

	virtual ~CParty();
	CCharacter* GetMember(UInt16 index);
	CCharacter* FindMember(UInt32 char_id);
	VOID ShowMember();
//	BOOL AddMember(CCharacter* member);
//	BOOL RemoveMember(CCharacter* member);
};

#endif //_PARTY_H_
