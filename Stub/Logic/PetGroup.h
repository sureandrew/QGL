#pragma once
#ifndef _PETGROUP_H_
#define _PETGROUP_H_

#include "Stub/Logic/PetGroup_Stub.h"
#include "Stub/Logic/Pet.h"

#define PET_MAX_WAREHOUSE	8

class CCharacter;

class CPetGroup : public PetGroup_Stub,
				public Reuben::Simulation::IEventHandler<CPetGroup>
{
public:
	DEFINE_CLASS(PetGroup_CLASSID);
	CPetGroup();
	virtual ~CPetGroup();
	INLINE static CPetGroup* NewInstance();
	virtual RPCResult DeletePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid);
	virtual RPCResult ChangePetState(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid, const UInt8 &state);
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petID);
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petID);

	LPCPEER GetOwnerPeer();
	RPCContext& GetOwnerContext();
	CCharacter* GetOwner() { return m_pCharOwner; }
	void SetOwner(CCharacter *pChar) { m_pCharOwner = pChar; }
	virtual void HandleEvent(CONST Event &event);
	void RaiseUpdate();							// raise update event
	void CancelUpdate();						// cancel update event

	void InitCharPetState();
	void SetAllPetSkills(NetGroup* skillDataGroups, NetGroup *sendGroup,
										bool forNewOrLvlUpPartner=false, UInt inWareHouse = 0);

	Boolean Add(NetObject *obj);
	Boolean Remove(NetObject *obj);

	void AddPet(UInt32 pet_id);
	void RemovePet(UInt32 pet_id);
	void RemovePet(CPet* pPet);
	void RemovePetByUID(UInt32 pet_uid);
	CPet* GetPetByIndex(UInt16 index);
	CPet* Get(int index) { return GetPetByIndex(index); }
	CPet* GetPet(UInt32 pet_uid);
	CPet* GetCurPet();
	void GetPetIdList(UInt32Vector& idList);
	void InitPetOwnObject();
	void PreSend(NetGroup* skillGroup);
	void SendAll();
	UInt16 GetIdCount(UInt32 pet_id);
	Boolean AddNewPet(CPet* pet);
	UInt16 GetCount(bool withNewPet = false);

	RPCResult LoadPets(RPCContext &context);		// load pets from object service
	RPCResult AddPet(RPCContext &context);
	RPCResult AddAllPetSkill(RPCContext &context);
	RPCResult UpdateAllPet(RPCContext &context);

	void ChangeOwnerID(const UInt32Vector &petIDs, UInt32 targetOwnerID);
	RPCResult ChangeOwnerID(RPCContext &context);

	void SetWareHousePet(NetGroup* petGroup);
	void GetWareHousePetUID(UInt32Vector& idVector);
	CPet* GetWareHousePetByIndex(UInt index);
	CPet* GetWareHousePet(UInt32 petUID);
	void ClearWareHouse();
	void GetAllWareHousePet(NetGroup* petGroup, NetGroup* skillGroup);
	Boolean ChangePetLockStatus(const UInt32 &bagIndex, const Boolean &lock, UInt8 &errMsg);	

	enum EventEnum
	{
		EVT_UPDATE_OBJECT	= MAKE_EVENTID(PetGroup_CLASSID, 1),	// update object
	};

private:
	typedef PetGroup_Stub Parent;
	CCharacter*		m_pCharOwner;
	NetGroup		m_newPets;
	NetGroup		m_delGroup;
	RPCNetIDVector m_wareHousePets;
	UInt8			m_nowUsePetType;
	CPet*			m_pTwoUsePet;
};

INLINE CPetGroup* CPetGroup::NewInstance()
{
	return CreateNetObject<CPetGroup>();
}

INLINE void CPetGroup::RaiseUpdate()
{
	RaiseUniqEventID(EVT_UPDATE_OBJECT);
}

INLINE void CPetGroup::CancelUpdate()
{
	CancelUniqEvent(EVT_UPDATE_OBJECT);
}

INLINE Boolean CPetGroup::AddNewPet(CPet* pet)
{
	return m_newPets.Add(pet);
}

#endif //_PETGROUP_H_
