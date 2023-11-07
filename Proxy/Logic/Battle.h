#pragma once
#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "Proxy/Logic/Battle_Proxy.h"
#include "Proxy/Logic/BatActor.h"
#include "QGL/BattleControl.h"

class CBattle : public Battle_Proxy
{
public:
	DEFINE_CLASS(Battle_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_PlayTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const ByteArray &array, const UInt32 &totalSize);
	virtual RPCResult CB_PlayTurnMore(LPCPEER pPeer, RPCContext &context, const ByteArray &array, const UInt32 &totalSize);
	virtual RPCResult CB_NextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &turnTime, const UInt16 &autoCount, const Boolean &bInput, const Boolean &bPartnerInput);
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
	virtual void OnUpdateBattleType(const UInt8 &battleType);
	
	CBattle();
	virtual ~CBattle();
	INLINE void SetBattleView(CBattleControl * pGivBattleView) { m_pBattleView = pGivBattleView;}
	UInt8 GetCurTurn() { return m_curTurn; }

	enum EndEnum {
		END_WIN = 1,				// player team win
		END_LOSE,					// player team lose
		END_ESCAPED,				// player team escaped
		END_DRAW,					// no team win or lose 
		END_MONITOR,				// monitor end
	};

private:
	typedef Battle_Proxy Parent;
	CBattleControl * m_pBattleView;
	Buffer			m_turnBuffer;		// play turn buffer
	UInt8			m_curTurn;
};

#endif //_BATTLE_H_
