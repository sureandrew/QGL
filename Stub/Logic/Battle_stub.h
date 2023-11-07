///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_BATTLE_STUB_H_
#define _RPC_BATTLE_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Battle_enum.h"

class Battle_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult EndTurn(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult NextTalk(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult ChooseResult(LPCPEER pPeer, RPCContext &context, const UInt8 &ans);
	virtual RPCResult SendBattleMessage(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const String &Msg);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		battleTypeMask	= Parent::NextFreeMask + 0,
		NextFreeMask	= Parent::NextFreeMask + 1
	};
	virtual RPCResult EndTurn(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult NextTalk(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult ChooseResult(LPCPEER pPeer, RPCContext &context, const UInt8 &ans) = 0;
	virtual RPCResult SendBattleMessage(LPCPEER pPeer, RPCContext &context, const RPCNetID &charNetID, const String &Msg) = 0;
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
	const UInt8& GetBattleType() const { return m.battleType; }
	void SetBattleType(const UInt8 &battleType);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//battleType
	UInt8 battleType;
} m;

public:
	INLINE Battle_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_BATTLE_STUB_H_
