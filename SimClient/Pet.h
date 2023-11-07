#pragma once
#ifndef _PET_H_
#define _PET_H_

#include "Proxy/Logic/Pet_Proxy.h"

class CSkill;
class CSkillGroup;
class CPet : public Pet_Proxy
{
public:
	DEFINE_CLASS(Pet_CLASSID);
	CSkillGroup * m_pSkillGrp;
	bool m_bGiven;
	CPet();
	virtual ~CPet();
	void SetSkillGroup(CSkillGroup *grp);

	INLINE void SetGiven(bool given){ m_bGiven = given;}
	INLINE bool IsGiven(){return m_bGiven;}

private:
	typedef Pet_Proxy Parent;
	virtual RPCResult CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
// OnUpdate can be implemented selectively
	virtual RPCResult CB_LearnSkillMessage(LPCPEER pPeer, RPCContext &context, const UInt16 &skillID);
	virtual RPCResult CB_ChangePetName(LPCPEER pPeer, RPCContext &context, const bool &success);
};

#endif //_PET_H_
