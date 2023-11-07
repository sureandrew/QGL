#pragma once
#ifndef _MENUJOINGBATTLE_H_
#define _MENUJOINGBATTLE_H_


#include "Proxy/Logic/MapActor.h"
#include "Proxy/Logic/Character_Proxy.h"
#include "Proxy/Logic/Map.h"


class CParty;
class CCharacter;

class CMenuJoinBattleProcess
{
private:

	typedef StlVector<CCharacter*, ManagedAllocator<CCharacter*> > TargetList;
	TargetList		m_RequestList;
	UInt	m_StartIndex;

public:
	CMenuJoinBattleProcess();
	virtual ~CMenuJoinBattleProcess();

	VOID UpdateBattleJoinMenu();
	VOID ClearList();
	VOID AddCharacterToList(CCharacter * pChar);
	VOID RemoveCharacterFromList(CCharacter * pChar);

	VOID SendPrivateMessageTo(Int Index);
	VOID AcceptJoin(Int Index);
	VOID RejectJoin(Int Index);

	VOID ScrollUp();
	VOID ScrollDown();

};




#endif 