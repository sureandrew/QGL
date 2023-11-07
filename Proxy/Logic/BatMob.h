#pragma once
#ifndef _BATMOB_H_
#define _BATMOB_H_

#include "Proxy/Logic/BatActor.h"
#include "Proxy/Logic/BatMob_Proxy.h"

class CBatMob : public BatMob_Proxy
{
private:
	typedef BatMob_Proxy Parent;

public:
	DEFINE_CLASS(BatMob_CLASSID);
	virtual void OnUpdateInfo(const UInt32 &mob_id, const String &raname);
	virtual void OnUpdateOwner(const UInt32 &owner_id);
	virtual void OnUpdateFightState(const UInt8 &fightstate);
};

#endif //_BATMOB_H_
