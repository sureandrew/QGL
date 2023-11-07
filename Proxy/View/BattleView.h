#pragma once
#ifndef _BATTLEVIEW_H_
#define _BATTLEVIEW_H_

class CBattleView
{
public:
	virtual VOID NotifyPlayTurn(LPCPEER pPeer, RPCContext &context, Buffer &buffer, UInt32 totalSize) = 0;
	virtual VOID NotifyLoseBattle(LPCPEER pPeer, RPCContext &context) = 0;
	virtual VOID NotifyNextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &timeLeft, const UInt16 &autoCount,
		const Boolean &bInput, const Boolean &bPartnerInput) = 0;
	virtual VOID NotifyEscapeBattle(LPCPEER pPeer, RPCContext &context) = 0;
	virtual VOID NotifyWinBattle(LPCPEER pPeer, RPCContext &context) = 0;
	virtual VOID NotifyDrawBattle(LPCPEER pPeer, RPCContext &context) = 0;

	virtual VOID NotifyControlCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx) = 0;
	virtual VOID NotifyControlPartner(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx, const UInt8 &mobIdx) = 0;
	virtual VOID NotifyAddActors(LPCPEER pPeer, RPCContext &context, NetGroup* actors, const UInt8 &Team) = 0;
	virtual VOID NotifyMartialResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &money, const UInt8 &point) = 0;
};

#endif
