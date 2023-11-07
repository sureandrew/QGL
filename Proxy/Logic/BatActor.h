#pragma once
#ifndef _BATACTOR_H_
#define _BATACTOR_H_

#include "Proxy/Logic/BatActor_Proxy.h"

#ifdef QGL
#include "QGL/CharacterControl.h"
#endif

#ifdef SIMCLIENT
#include "Common/StateCode.h"
#endif

class CBatActor : public BatActor_Proxy
{
private:
	typedef BatActor_Proxy Parent;

#ifdef QGL
protected:
	CCharacterControl * pCharView;
#endif

public:
	DEFINE_CLASS(BatActor_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_CommandSuccess(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd);
	virtual RPCResult CB_Inputed(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd);
	virtual RPCResult CB_AutoCount(LPCPEER pPeer, RPCContext &context, const UInt16 &count);
	virtual RPCResult CB_BattleMsg(LPCPEER pPeer, RPCContext &context, const UInt8 &MsgType, const String &Msg, const UInt16 &subType);

#ifdef QGL
	virtual void OnUpdateHP(const UInt16 &HP, const Int16 &dHP, const Int16 &dShowHP);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdateBuffCache(const String &buffCache);
	virtual void OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD);

	CBatActor();
	inline void SetCharacterView(CCharacterControl * pGivCharView){ pCharView = pGivCharView;}

	bool IsPlayer();
	bool IsMob();
	bool IsParnter();
	bool IsBoss();
	
#endif // QGL

#ifdef SIMCLIENT
	CBatActor();
	virtual RPCResult SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, const UInt8 &target, const UInt32 &sub_type, const UInt32 &additional);
	Boolean IsValidTarget();

	Boolean m_commandOk;
#endif // SIMCLIENT
};

#ifdef SIMCLIENT
INLINE Boolean CBatActor::IsValidTarget()
{
	return (!GetLeftBattle() && GetState() != CHAR_ST_DEAD && GetState() != CHAR_ST_ESCAPED &&
			GetState() != CHAR_ST_BACKUP && GetState() != CHAR_ST_CATCHED);
}
#endif // SIMCLIENT

#endif //_BATACTOR_H_
