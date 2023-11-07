#pragma once
#ifndef _BATCHARACTER_H_
#define _BATCHARACTER_H_

#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/BatCharacter_Proxy.h"

class CBatCharacter : public BatCharacter_Proxy
{
private:
	typedef BatCharacter_Proxy Parent;

public:
	DEFINE_CLASS(BatCharacter_CLASSID);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual void OnUpdateInfo(const UInt32 &acct_id, const UInt32 &char_id, const String &nickName, const UInt8 &sex);
	virtual VOID OnUpdateExp(const UInt32 &exp);
	virtual VOID OnUpdateMDP(const UInt16 &mDP);
	virtual VOID OnUpdateMWC(const UInt16 &mWC);
	virtual VOID OnUpdateMVP(const UInt16 &mVP);
	virtual VOID OnUpdateDP(const UInt16 &DP);
	virtual VOID OnUpdateWC(const UInt16 &WC);
	virtual VOID OnUpdateVP(const UInt16 &VP);
	virtual VOID OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor, const UInt16 &clothType, const UInt16 &clothColor);
	virtual VOID OnUpdateEquipment(const UInt32 &eq_weapon, const UInt32 &eq_head, const UInt32 &eq_headBand, const UInt32 &eq_body, const UInt32 &eq_foot, const UInt32 &eq_tinker1, const UInt32 &eq_tinker2);
};

#endif //_BATCHARACTER_H_
