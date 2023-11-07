#pragma once
#ifndef _PARTNER_H_
#define _PARTNER_H_


#include "Proxy/Data/PartnerData.h"
#include "Stub/Logic/Partner_Stub.h"
#include "Common/common_binary.h"

class CPartnerGroup;
class CSkill;
class CSkillGroup;
class CSkillData;
class CBuff;
class CBatMob;
struct ExtractedBuffData;
struct MobData;

class CPartner : public Partner_Stub,
				public Reuben::Simulation::IEventHandler<CPartner>
{
public:
	virtual RPCResult RaiseAttrib(LPCPEER pPeer, RPCContext &context, const UInt16 &sta, const UInt16 &spr, const UInt16 &str, const UInt16 &con, const UInt16 &agi);
	virtual RPCResult ChangePartnerName(LPCPEER pPeer, RPCContext &context, const String &newName);
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt8 &state);

	DEFINE_CLASS(Partner_CLASSID);
	CPartner();
	virtual ~CPartner();
	INLINE static CPartner* NewInstance();
	void PreSend(NetGroup* skillGroup=NULL);
	void CalcAttrib();							// calculate final attribute values
	void SetData(CPartnerData* pPartnerData);	// set partner by CPartnerData
	void CalcAttrib2();							// calculate temp values
	//RPCResult UpdateToObject(RPCContext &context);	// update partner to object service

	void WriteData(CPartnerData* pPartnerData);
	Boolean CanLevelUp();
	void LevelUp();
	void SetPartnerGroup(CPartnerGroup* ownerGroup);
	LPCPEER GetOwnerPeer();
	RPCContext &GetOwnerContext();

	UInt8 GetPartnerType();
	UInt16 GetReqLv();
	UInt8 GetRace();
	void ChangeState(UInt8 state);
	void ChangeExp(Int32 change, Int32 exp_type = 0);
	void ChangeFavor(float change);
	void ChangeHealth(Int change);
	void ChangeHP(Int change);
	void ChangeSP(Int change);
	void ChangeHP_native(Int change);
	void ChangeSP_native(Int change); 
	void ChangeATK_native(Int change);
	void ChangeDEF_native(Int change);
	void ChangeSPD_native(Int change);
	void ChangePOW_native(Int change);
	void ChangeHIT_native(Int change);
	void ChangeEVA_native(Int change);
	void ChangeGrow(float change);
	void SetFull(bool HP=true, bool SP=true, bool favor=true);
	void ResetLevel(CONST UInt16 lv);
	void ResetPotential();
	void ResetCultivation();
	void ResetPartner();
	Boolean NeedUpdate();						// need update to database?
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event

	void ChangeCultivation(UInt16 change);
	void ChangePractice(UInt16 change);
	UInt8 CalcGrowLevel();

	RPCResult RegenSkill(RPCContext &context);
	
	// Event & State
	virtual void HandleEvent(CONST Event &event);

	RPCResult DoLearnSkill(RPCContext &context);
	RPCResult DoRemoveSkill(RPCContext &context);


	//void SetSkillGroup(NetGroup * SGrp);
	NetGroup* GetSkillGroup(bool forSend = false);
	void MakeLearnSkillLst(bool forNewPartner=false, bool toObject=false);
	UInt16Vector GetLearnSkillLst(){ return m_LearnSkillID; }
	void ClearLearnSkillLst(){ m_LearnSkillID.clear(); }

	void ClearSkillMap();
	bool HasSkill(UInt16 skillID);
	Boolean AddSkill(CSkill* pSkill);
	void AddSkill(UInt16 skill_id);
	void RemoveSkill(UInt16 skillId);
	UInt16	GetRandomSkillID(UInt16 Type);
	INLINE void SetHasAddSkill(bool hasAdd){ m_bHasAddSkill = hasAdd; };
	INLINE bool HasAddSkill(){ return m_bHasAddSkill;}
	UInt8 GetNormalSkillCount();
	UInt8 GetSpecialSkillCount();
	void SetSkillCoolDown(UInt16 ID, DWORD countDown);

	void ChangeOutlookColor(UInt16 colorIndex);

	void ClonePartner(CPartner* pPartner);

	Boolean ReverseAttrib(UInt8 attrType, UInt16 point);
	

	//////////////////////////////////////
	//			Buff methods			//
	//////////////////////////////////////
	void SetBuffGroup(NetGroup * BGrp);
	bool AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro);
	void RemoveBuff(UInt16 Buff_id);
	void RemoveBuffSchool(UInt16 school_id);
	bool HasBuffSchool(UInt16 school_id);
	bool HasBuff(UInt16 Buff_id);
	ExtractedBuffData * FindBuff(UInt16 Buff_id);
	void ExtractBuffString();
	void ConstructBuffString();
	void ExtractBuffCacheString();
	void ConstructBuffCacheString();

	const UInt16 GetSTA();
	const UInt16 GetSPR();
	const UInt16 GetSTR();
	const UInt16 GetCON();
	const UInt16 GetAGI();

	bool IsUsePartnerCollection();

	enum StateEnum
	{
		ST_STANDBY = 0,
		ST_FIGHT = 1,
		ST_REST = 2,
	};

	enum EventEnum
	{
		//EVT_UPDATE_OBJECT	= MAKE_EVENTID(Partner_CLASSID, 1),	// update object
		//EVT_UPDATE_PARTNER_EXPIRE = MAKE_EVENTID(Partner_CLASSID, 2),
		EVT_UPDATE_PARTNER_EXPIRE = MAKE_EVENTID(Partner_CLASSID, 1),
	};

	typedef StlMap<UInt16, CSkill*, std::less<UInt16>, ManagedAllocator<std::pair<UInt16, CSkill*> > > SkillMap;

	const MobData*	m_data;				// MobData struct
	CSkillGroup*	m_pSkillGroup;
	SkillMap		m_skillMap;
	BuffMap			m_BuffMap;
	BuffMap			m_BuffCacheMap;
	UInt16			m_iNewSkillID;
	CBatMob*		m_pMob;
	UInt8			m_cmd;				// battle command
	UInt32			m_sub_type;			// battle sub type
	UInt32			m_additional;		// battle additional
	UInt32			m_cmdTarget;		// saved target id
	UInt8			m_cmdTargetType;	// BatActor target type
	AttriStruct2	m_AttriOfPartnerCollection;// saved the Sta, Spr, Str, Con, Agi attri change by the partner collection

private:
	typedef Partner_Stub Parent;
	CPartnerGroup*	m_pOwnerGroup;

	UInt16Vector m_LearnSkillID;
	bool		m_bHasAddSkill;
};

INLINE CPartner* CPartner::NewInstance()
{
	return CreateNetObject<CPartner>();
}

#endif //_PARTNER_H_
