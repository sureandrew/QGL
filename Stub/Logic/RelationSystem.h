#pragma once
#ifndef _RELATIONSYSTEM_H_
#define _RELATIONSYSTEM_H_

#include "Stub/Logic/RelationSystem_Stub.h"
class CCharacter;
class CSiblingData;
class CMarriageData;

class CRelationSystem : public RelationSystem_Stub
	,public Reuben::Simulation::IEventHandler<CRelationSystem>
{
public:
	enum StateEnum 
	{
		ST_NONE = 1,
		ST_CREATE_SIBLING,
		LAST_STATE,	
	};
	virtual RPCResult SetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &prefix);
	virtual RPCResult SetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &suffix);
	virtual RPCResult SelectMarriageTime(LPCPEER pPeer, RPCContext &context, const UInt32 &hour,const UInt32 &minute);

	CRelationSystem();
	virtual ~CRelationSystem();
	void SetOwner(CCharacter* pCharacter){ m_pOwner = pCharacter;}
	CCharacter* GetOwner(void){ return m_pOwner;};
	void SetState(UInt8 nState){m_state = nState;}
	UInt8 GetState(void){return m_state;}
	DEFINE_CLASS(RelationSystem_CLASSID);

	//master and student
	void CreateMasterAndStudent(UInt32 master_id,UInt32 student_id);
	void DestroyMasterAndStudent(UInt32 master_id,UInt32 student_id);
	void CheckMasterAndStudentDataValidate();
	RPCResult CheckMasterAndStudentDataValidate(RPCContext &context);

	//sibling
	void RequestSetSiblingTitlePrefix();
	void RequestSetSiblingTitleSuffix();
	void CreateSibling(UInt32 sibling_id1,UInt32 sibling_id2,UInt32 sibling_id3,UInt32 sibling_id4,UInt32 sibling_id5);
	void RemoveSibling(UInt32 sibling_id);
	void AddSibling(UInt32 sibling_id1,UInt32 sibling_id2,UInt32 sibling_id3,UInt32 sibling_id4);
	RPCResult CreateSibling(RPCContext &context);
	void SetSiblingData(CSiblingData *pSiblingData);
	CMarriageData* GetMarriageData(){return m_pMarriageData;}
	void UpdateSiblingTitle();
	void CheckSiblingDataValidate();
	String GetSiblingTitlePrefix();
	RPCResult CheckSiblingDataValidate(RPCContext &context);
	UInt32 GetSiblingRank(UInt32 sibling_id);
	//husband and wife
	//marriage Event
	enum EventEnum
	{
		EVT_WAIT1		= MAKE_EVENTID(RelationSystem_CLASSID,  1),
		EVT_WAIT2		= MAKE_EVENTID(RelationSystem_CLASSID,  2),
		EVT_WAIT3		= MAKE_EVENTID(RelationSystem_CLASSID,  3),
		EVT_WEDDING1	= MAKE_EVENTID(RelationSystem_CLASSID,  4),
		EVT_WEDDING2	= MAKE_EVENTID(RelationSystem_CLASSID,  5),
		EVT_WEDDING3	= MAKE_EVENTID(RelationSystem_CLASSID,  6),
		EVT_WEDDING4	= MAKE_EVENTID(RelationSystem_CLASSID,  7),
		EVT_RESET		= MAKE_EVENTID(RelationSystem_CLASSID,  8),
		EVT_WEDDING_STOP = MAKE_EVENTID(RelationSystem_CLASSID,  9),
	};

	virtual void HandleEvent(CONST Event& event);
	void RequestSelectMarriageTime(UInt32 husband_id,UInt32 wife_id);
	void CreateMarriage(UInt32 husband_id,UInt32 wife_id,UInt32 marriage_time);
	void DestroyMarriage(UInt32 husband_id,UInt32 wife_id);
	RPCResult CreateMarriage(RPCContext &context);
	void ExecuteMarriage(UInt32 husband_id,UInt32 wife_id);
	UInt32 GetMarriageTime();
	UInt32 GetRealMarriageTime();
	void SetMarriageData(CMarriageData *pMarriageData);
	void CheckHusbandAndWifeDataValidate();
	RPCResult CheckHusbandAndWifeDataValidate(RPCContext &context);
	UInt32 GetHusbandId();
	UInt32 GetWifeId();
	UInt32 GetMarriageId();
	//RPCResult SendMarriageRumor(RPCContext &context);
	void OnCompanionExit(UInt32 char_id);

	INLINE static CRelationSystem* NewInstance();

	typedef StlVector<UInt32, ManagedAllocator<UInt32> > TmpSiblingVec;
	typedef StlVector<UInt32, ManagedAllocator<UInt32> > SiblingIdVec;
	typedef StlVector<String, ManagedAllocator<String> > SiblingSuffixVec;

	bool hasSiblingData();
	
private:
	String GetSiblingTitlePrefixThirdWord();
	String GetSiblingTitlePrefixThirdWord(UInt32 count);
	String GetSiblingTitleSuffix();
	void GetSiblingIdAndSuffix(SiblingIdVec& siblingIdVec,SiblingSuffixVec &siblingSuffixVec);
	void SetSiblingIdAndSuffix(const SiblingIdVec& siblingIdVec,const SiblingSuffixVec &siblingSuffixVec);
	void UpdateAllMemberTitle();
	void CreateMarriageRelationship(UInt32 husband_id,UInt32 wife_id);
	void OnMarriageEvent(UInt32 state);
	void OnMarriageWaitEvent(UInt32 state);
	Boolean FindMarriageTime(time_t marriagetime);
private:
	typedef RelationSystem_Stub Parent;
	CCharacter* m_pOwner;
	TmpSiblingVec m_tmpSiblingVec;
	UInt8 m_state;
	CSiblingData* m_pSiblingData;
	CMarriageData* m_pMarriageData;
	UInt32 m_tmpHusbandId;
	UInt32 m_tmpWifeId;
	Boolean m_bWedding;
	static UInt32 m_allMarriageCount;
	RPCNetID m_siblingDataNetID;

};
INLINE CRelationSystem* CRelationSystem::NewInstance()
{
	return CreateNetObject<CRelationSystem>();
}
#endif //_RELATIONSYSTEM_H_
