#pragma once
#ifndef _BATMOB_H_
#define _BATMOB_H_

#include "Stub/Logic/BatActor.h"
#include "Stub/Logic/BatMob_Stub.h"

struct BatClient;
struct MobData;

class CPartner;

class CBatMob : public BatMob_Stub
{
public:
	DEFINE_CLASS(BatMob_CLASSID);
	CBatMob();
	virtual ~CBatMob();
	INLINE static CBatMob* NewInstance();

	virtual UInt8 GetSex();
	UInt8 GetRace();
	void SetData(UInt32 mob_id, UInt16 level = 0);
	void SetCommandByAI();
	virtual void PreSend();					// prepare to send
	virtual void PreSendEnemy();
	virtual void SetState(const UInt8 &state);
	void SetPartnerData(CPartner* partner);		// set batmob using partner as data
	void WritePartnerData(CPartner*partner);	// write partner mob to CPartner
	//virtual void DoEscape(Buffer &buffer);
	void DoPartnerLeft();
	void UpdateHealthAndFavor();

	INLINE bool IsNew() { return m_bNew; }
	INLINE void SetNew(bool isNew) { m_bNew = isNew; }
	INLINE void SetGuest(bool bGuest) { m_bGuest = bGuest; }

	virtual void CalcAttrib();			// calculate the final attributes
	INLINE virtual UInt32 GetActorId() { return GetMob_id(); }
	INLINE virtual bool IsParnter() { return GetPartner_id() > 0; }
	INLINE virtual bool IsGuest() { return m_bGuest; }
	virtual bool IsBoss();
	virtual bool AllCmdBlock();
	virtual void InitPrevCmd();
	virtual void SaveLastCmd();
	void MobLearnSkill();
	float GetFavor();
	UInt16 GetHealth();
	void ChangeFavor(float change);
	void ChangeHealth(Int change);

	const MobData*	m_data;				// MobData struct
	CPartner*		m_pPartner;			// assiocated partner
	bool			m_bNew;				// new mob
	bool			m_bGuest;			// guest npc mob

private:
	typedef BatMob_Stub Parent;
};

INLINE CBatMob* CBatMob::NewInstance()
{
	return CreateNetObject<CBatMob>();
}

#endif //_BATMOB_H_