#pragma once
#ifndef _PARTNERGROUP_H_
#define _PARTNERGROUP_H_

#include "Stub/Logic/PartnerGroup_Stub.h"
#include "Stub/Logic/Partner.h"

#define PARTNER_MAX_WAREHOUSE	8

class CCharacter;

class CPartnerGroup : public PartnerGroup_Stub,
						public Reuben::Simulation::IEventHandler<CPartnerGroup>
{
public:
	virtual RPCResult DeletePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt8 &state);
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID);
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID);
	virtual RPCResult PartnerCultivationUpByPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_partnerID);
	virtual RPCResult PartnerCultivationUpByItem(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_itemUid);

	DEFINE_CLASS(PartnerGroup_CLASSID);
	CPartnerGroup();
	virtual ~CPartnerGroup();
	INLINE static CPartnerGroup* NewInstance();
	void PreSend(NetGroup* skillGroup);
	void SendAll();
	void Destroy();
	CPartner* GetPartner(UInt32 partner_id);
	CPartner* GetPartnerByMobId(UInt32 mob_id);
	CPartner* GetPartnerByID(UInt32 partner_id);
	CPartner* GetPartnerByIndex(UInt16 index);
	CPartner* GetFightPartner();
	CPartner* Get(int index) { return GetPartnerByIndex(index); }
	UInt16 GetCount(bool withNewPartner = false);
	UInt32 GetFightPartnerID();
	void SetOwner(CCharacter* owner){ m_pOwner = owner;}
	CCharacter* GetOwner(){return m_pOwner;}
	RPCContext& GetOwnerContext();
	Boolean AddNewPartner(CPartner* partner);
	
	UInt32 GetPartnerID(UInt16 index);
	UInt16 GetPartnerCount(UInt32 modId);
	void GetPartnerIdList(UInt32Vector& idList);

	Boolean Add(NetObject *obj);
	Boolean Remove(NetObject *obj);
	void AddPartner(UInt32 mob_id, UInt8 level=0);
	Boolean RemovePartner(UInt32 mob_id);
	Boolean RemovePartner(CPartner* partner);
	Boolean RemovePartnerByUID(UInt32 partner_id);

	RPCResult LoadPartners(RPCContext &context);		// load partners from object service
	RPCResult AddPartner(RPCContext &context);
	//RPCResult DeletePartnerByMobID(RPCContext &context);
	RPCResult AddAllPartnerSkill(RPCContext &context);
	RPCResult UpdateAllPartner(RPCContext &context);
	
	void SetAllFull(bool HP=true, bool SP=true, bool favor=true);
	void ChangeFavor(UInt8 targetType, float value);
	void SetAllPartnerSkills(NetGroup* skillDataGroups, NetGroup *sendGroup, 
							bool forNewOrLvlUpPartner=false, UInt inWareHouse = 0);

	virtual void HandleEvent(CONST Event &event);
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event

	void InitPartnerOwnObject();
	void ChangeOwnerID(const UInt32Vector &partnerIDs, UInt32 targetOwnerID);
	RPCResult ChangeOwnerID(RPCContext &context);

	void SetWareHousePartner(NetGroup* partnerGroup);
	void GetWareHousePartnerID(UInt32Vector& idVector);
	CPartner* GetWareHousePartnerByIndex(UInt index);
	CPartner* GetWareHousePartner(UInt32 partnerID);
	void ClearWareHouse();
	void GetAllWareHousePartner(NetGroup* partnerGroup, NetGroup* skillGroup);
	Boolean ChangePartnerLockStatus(const UInt32 &bagIndex, const Boolean &lock, UInt8 &errMsg);	
	void UpdatePartnerCollection();

	enum EventEnum
	{
		EVT_UPDATE_OBJECT		= MAKE_EVENTID(PartnerGroup_CLASSID,  1),
	};

private:
	typedef PartnerGroup_Stub Parent;
	CCharacter* m_pOwner;
	NetGroup	m_newPartners;
	NetGroup	m_delGroup;
	RPCNetIDVector m_wareHousePartners;
};

INLINE CPartnerGroup* CPartnerGroup::NewInstance()
{
	return CreateNetObject<CPartnerGroup>();
}

INLINE void CPartnerGroup::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

INLINE void CPartnerGroup::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}

INLINE Boolean CPartnerGroup::AddNewPartner(CPartner* partner)
{
	return m_newPartners.Add(partner);
}

#endif //_PARTNERGROUP_H_
