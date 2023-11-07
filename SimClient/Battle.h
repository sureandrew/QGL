#pragma once
#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "Proxy/Logic/Battle_Proxy.h"

class CBatActor;
class CBatCharacter;
class CBatMob;

class CBattle : public Battle_Proxy
{
public:
	DEFINE_CLASS(Battle_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_PlayTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const ByteArray &array, const UInt32 &totalSize);
	virtual RPCResult CB_PlayTurnMore(LPCPEER pPeer, RPCContext &context, const ByteArray &array, const UInt32 &totalSize);
	virtual RPCResult CB_NextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &turnTime, const UInt16 &autoCount, const Boolean &bInput, const Boolean &bPartnerInput);
	virtual RPCResult CB_ResetCounter(LPCPEER pPeer, RPCContext &context, const UInt32 &turnTime);
	virtual RPCResult CB_EndBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &endState);
	virtual RPCResult CB_AddActors(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &Team);
	virtual RPCResult CB_AddPartners(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &Team);
	virtual RPCResult CB_ControlCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx);
	virtual RPCResult CB_ControlPartner(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx, const UInt8 &mobIdx);
	virtual RPCResult CB_Reward(LPCPEER pPeer, RPCContext &context, const Boolean &levelUp, const UInt32 &exp, const UInt32Vector &gotItems, const UInt32Vector &discardItems, const UInt32 &partnerExp, const Boolean &partnerLvlUp, const UInt32 &boundsExp, const UInt32 &boundsPartnerExp);
	virtual RPCResult CB_Penalty(LPCPEER pPeer, RPCContext &context, const UInt32 &exp, const UInt32 &money, const UInt32 &honor);
	virtual RPCResult CB_NeedHelp(LPCPEER pPeer, RPCContext &context, const Boolean &bNeed);
	virtual RPCResult CB_WaitTalk(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &turnTime);
	virtual RPCResult CB_Choose(LPCPEER pPeer, RPCContext &context, const UInt8 &askerType, const UInt32 &askerId, const Boolean &bLeft, const UInt8 &total, const StringVector &msgList);
	virtual RPCResult CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, const int &imageType, const Boolean &bLeft, const UInt32 &npc_id);
	virtual RPCResult CB_CancelTalk(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_DisableCmd(LPCPEER pPeer, RPCContext &context, const UInt32 &blockCmd);
	virtual RPCResult CB_HideCmd(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const Boolean &hide);
	virtual RPCResult CB_MartialResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &money, const UInt8 &point);

	CBattle() : m_needHelp(BOOLEAN_FALSE) {}
	virtual ~CBattle();
	void ShowAllDetail();
	void ShowBattle();
	void ClearContent();
	CBatActor* GetActor(UInt8 index);
	CBatCharacter* GetCharacter(UInt8 index);
	CBatMob* GetMob(UInt8 index);
	UInt16 GetActorCount() { return ((UInt16) m_actors.size()); }
	UInt16 GetTeamCount(UInt8 team);
	Boolean IsNeedHelp() { return m_needHelp; }
	void PlayAction(LPCPEER pPeer, RPCContext &context, UInt32 totalSize);
	void CheckEnd(UInt8 endState);
	
	enum EndEnum {
		END_WIN = 1,				// player team win
		END_LOSE,					// player team lose
		END_ESCAPED,				// player team escaped
		END_DRAW,					// no team win or lose 
		END_MONITOR,				// monitor end
	};
	typedef StlVector<CBatActor*, ManagedAllocator<CBatActor*> > Actors;
	Actors		m_actors;			// actor group contain mobs and characters
	Boolean		m_needHelp;			// team need help
	Buffer		m_turnBuffer;		// play turn buffer

private:
	typedef Battle_Proxy Parent;

};

INLINE CBatActor* CBattle::GetActor(UInt8 index)
{
	if (index < (UInt8) m_actors.size())
		return m_actors[index];
	else
		return NULL;
}

#endif //_BATTLE_H_
