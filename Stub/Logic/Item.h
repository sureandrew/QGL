#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_

#include "Stub/Logic/Item_Stub.h"

struct ItemData;
class CItemData;
class CMapActor;
class CBatActor;
class CCharacter;
class CPartner;
class CPet;
class CItemBag;
struct ExtractedBuffData;
struct BuffData;

class CItem : public Item_Stub,
			public Reuben::Simulation::IEventHandler<CItem>
{
public:
	DEFINE_CLASS(Item_CLASSID);

	CItem();
	INLINE static CItem* NewInstance();
	virtual String DebugInfo();
	UInt8 GetEquipIndex();
	void SetData(CItemData *pData, bool bClear=true);		// set character by CItemData
	void WriteData(CItemData *pData);			// write character to CItemData
	void PreSend(UInt8 detail=0);			// prepare to send
	Boolean IsExist();						// is really exist?
	// for map
	Boolean CanUse(CCharacter* pUser, NetObject* pTarget, String &errMsg);	// self or partner can use?
	Boolean UseOn(CCharacter* pUser, NetObject* pTarget, String &errMsg);	// use self or partner?

	// for battle
	Boolean CanUse(CBatActor* pUser, CBatActor* pTarget, String &errMsg);
	Boolean UseOn(CBatActor* pUser, CBatActor* pTarget, String &errMsg, 
					Boolean bTry=BOOLEAN_FALSE);
	Boolean AllowSexClass(UInt8 sex, UInt8 cclass);		// can use/equip for sex and class?
	Boolean ReduceCount();

	INLINE UInt16 GetSTA() { return GetBase_STA() + GetUpg_STA() + GetInlay_STA(); }
	INLINE UInt16 GetSPR() { return GetBase_SPR() + GetUpg_SPR() + GetInlay_SPR(); }
	INLINE UInt16 GetSTR() { return GetBase_STR() + GetUpg_STR() + GetInlay_STR(); }
	INLINE UInt16 GetCON() { return GetBase_CON() + GetUpg_CON() + GetInlay_CON(); }
	INLINE UInt16 GetAGI() { return GetBase_AGI() + GetUpg_AGI() + GetInlay_AGI(); }
	INLINE UInt16 GetATK() { return GetBase_ATK() + GetUpg_ATK() + GetInlay_ATK(); }
	INLINE UInt16 GetDEF() { return GetBase_DEF() + GetUpg_DEF() + GetInlay_DEF(); }
	INLINE UInt16 GetHIT() { return GetBase_HIT() + GetUpg_HIT() + GetInlay_HIT(); }
	INLINE UInt16 GetEVA() { return GetBase_EVA() + GetUpg_EVA() + GetInlay_EVA(); }
	INLINE UInt16 GetPOW() { return GetBase_POW() + GetUpg_POW() + GetInlay_POW(); }
	INLINE UInt16 GetSPD() { return GetBase_SPD() + GetUpg_SPD() + GetInlay_SPD(); }
	INLINE UInt16 GetMaxHP() { return GetBase_maxHP() + GetUpg_maxHP() + GetInlay_maxHP(); }
	INLINE UInt16 GetMaxSP() { return GetBase_maxSP() + GetUpg_maxSP() + GetInlay_maxSP(); }
	INLINE UInt16 GetMaxVP() { return GetBase_maxVP() + GetUpg_maxVP() + GetInlay_maxVP(); }

	void SetItemValue(int order, UInt16 value);
	UInt16 GetItemValue(int order);
	UInt8 GetItemUserTypeId();

	virtual void HandleEvent(CONST Event &event);
	//////////////////////////////////////
	//			Buff methods			//
	//////////////////////////////////////

	void AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void RemoveBuff();
	void UpdateBuff();
	bool IsRemoveAfterOffline();
	UInt8 GetItemColor();

	UInt32 GetEqGemID(UInt8 nIndex);
	
	const BuffData * m_buffData;		//item only has one buff
	const ItemData* m_data;
	
	UInt8	m_tryStack;						// stack after try
	UInt8	m_tryRemain;					// remain after try
	UInt32	m_BuffPreUpdateTime;

	enum EventEnum
	{
		EVT_UPDATE_BUFF			= MAKE_EVENTID(Item_CLASSID,  1),
		EVT_UPDATE_ITEM_EXPIRE	= MAKE_EVENTID(Item_CLASSID, 2),
	};

	//////////////////////////////////////
	//			Skill methods			//
	//////////////////////////////////////
	bool AddSkill();
	void RemoveSkill();

	UInt m_iCreateType;
	void SetCreateType(UInt createType) { m_iCreateType = createType;}
	void ResetCreateType();
	UInt GetCreateType() { return m_iCreateType; }

private:
	typedef Item_Stub Parent;
	Boolean ApplyCharacter(CCharacter* pTarget);
	Boolean ApplyPartner(CPartner* pTarget);
	Boolean ApplyPet(CPet* pPet);
};

INLINE CItem* CItem::NewInstance()
{
	return CreateNetObject<CItem>();
}

INLINE Boolean CItem::IsExist()
{
	return (GetStack() != 0);
}
#endif //_ITEM_H_
