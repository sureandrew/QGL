#pragma once
#ifndef _BATCHARACTER_H_
#define _BATCHARACTER_H_

#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/BatCharacter_Stub.h"

class CCharacter;
class CBatMob;
class CPetGroup;

class CBatCharacter : public BatCharacter_Stub, 
					public Reuben::Simulation::IEventHandler<CBatCharacter>
{
public:
	DEFINE_CLASS(BatCharacter_CLASSID);

	CBatCharacter();
	virtual ~CBatCharacter();
	INLINE static CBatCharacter* NewInstance();
	void SetData(CCharacter *pChar);			// set character by CCharacter
	void WriteData(CCharacter *pChar);			// write character to CCharacter
	virtual void WriteAction();
	virtual void WriteReaction();
	virtual void DoHelp();
	virtual void DoEscape();
	virtual void DoCatch();
	virtual void DoFriend();
	virtual void DoEvent();
	virtual void TurnClear();
	virtual void PreSend();					// prepare to send
	virtual void PreSendEnemy();
	virtual void CalcAttrib();			// calculate the final attributes
	void SetAuto(BatClient* pClient);
	void RaiseAutoEvent();
	void StopAutoEvent();
	virtual bool IsLeader();
	virtual void InitPrevCmd();
	virtual void SaveLastCmd();
	bool HasParty();
	virtual void HandleEvent(CONST Event &event);
	virtual UInt32 GetActorId() { return GetChar_id(); }
	CBatMob* GetCurPartner();
	void ClearAllPartnerItemBag();
	INLINE virtual CCharacter* GetOwner() { return m_pOwner; }

	Boolean	GetAttackBoss();

	enum EventEnum {
		EVT_AUTO_TIMEOUT = MAKE_EVENTID(BatCharacter_CLASSID, 1),
	};

	virtual void ChangeDP(Int change, bool show = true);
	virtual void ChangeWC(Int change, bool show = true);
	virtual void ChangeVP(Int change, bool show = true);
	virtual void ChangeTP(Int change, bool show = true);
	virtual void ChangeWcExp(Int change);

	virtual bool AllCmdBlock();

	virtual void ResetDpChange();
	virtual void ResetVPChange();

	CCharacter*		m_pOwner;				// character owner
	NetGroup*		m_partnerGroup;			// partner group
	UInt32			m_nextPartnerID;		// next partner id to call
	UInt32			m_escapeWait;			// animation time up to escape

private:
	typedef BatCharacter_Stub Parent;
};

INLINE CBatCharacter* CBatCharacter::NewInstance()
{
	return CreateNetObject<CBatCharacter>();
}


#endif //_BATCHARACTER_H_
