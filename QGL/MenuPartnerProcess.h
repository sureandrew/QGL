#pragma once
#ifndef _MENUPARTNER_H_
#define _MENUPARTNER_H_


#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/PartnerGroup.h"
struct SkillData;
struct PartnerCollectionData;
struct AttriStruct2;

const UInt8 PartnerCollectionRace[11] ={
	0,		//not use
		2,	//green partner
		3,	//blue partner
		4,	//purple partner
		5,	//golden partner
		11,	//red partner
		7,	//green animal partner,not use
		8,	//blue animal partner,not use
		9,	//purple animal partner,not use
		10,	//golden animal partner,not use
		0,	//red animal partner,not use
};
typedef StlMap<StringA, UInt16, std::less<StringA>, ManagedAllocator<std::pair<StringA, UInt16> > > PartnerSkillButtonMap;
typedef StlMap<UInt32,const PartnerCollectionData*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, const PartnerCollectionData*> > > PartnerCollectionDataMap;

class CMenuPartnerProcess
{
private :
	CPartner* m_pOnShowPartner;
	UInt8 m_iStartIndex;
	UInt8 m_iSelectedIndex;
	bool m_bRemoving;
	UInt m_iSTRAdd;
	UInt m_iCONAdd;
	UInt m_iSTAAdd;
	UInt m_iAGIAdd;
	UInt m_iSPRAdd;
	UInt16 m_iTempSelSkillID;
	const SkillData* m_pTempSelSkillData;
	
	PartnerSkillButtonMap m_SkillButtonMap;

	StringA m_ShortCutStr[10];
	//partner collection
	PartnerCollectionDataMap m_PartnerCollectionMap;
	UInt8	m_iPage;
	
public:
	CMenuPartnerProcess();
	~CMenuPartnerProcess();
	void UpdatePartnerMenu();
	void SetOnShowPartner(CPartner* partner);
	void SetNumField(StringA fieldName, Int32 fieldValue, UInt16 addedValue = 0);
	void UpdatePartnerNameList();
	void UpdateBar(StringA barNamePref, UInt32 nowValue, UInt32 MaxValue); 
	void DeleteOnShowPartner();
	bool IsRemoving();
	INLINE void SetRemoving(bool remove){ m_bRemoving = remove;}
	void ResetValue();
	void AttrUp(int index);
	void AttrDown(int index);
	void AddConfirm();
	void UpdateAttrib1();
	void UpdateAttrPoint();
	void UpdateAttrib2();
	void UpdateLevel();
	void UpdateFavor();
	void UpdateGoodness();
	void UpdateHP();
	void UpdateSP();
	void UpdateExp();
	void SelectPartner(int index);
	void ChangePartnerName();
	void SetPartnerState(int state); //0: standby 1: fight 2:rest
	void ScrollPartner(int index); // 0: up 1: down 2: scollmove
	void SetFollow();
	void UpdateStateButton();
	void UpdateHealth();
	void SetButtonState(StringA buttonName, bool state); //true : normal state  false: Disable state
	void UpdateAllButtonState();
	void ShowRemovePartnerName();
	void UpdateSmallIcon(bool show, UInt32 partnerID = 0);
	void UpdateSmallHP(UInt32 partnerID);
	void UpdateSmallSP(UInt32 partnerID);
	//void ShowPartnerButton();
	bool IsOnShowPartner(UInt32 partnerID);
	void UpdateSkillMenu(CPartner* partner);
	void ShowSkillDetail();
	void UpdateBattleSkillMenu();
	bool SelectSkill();
	bool ShortCutSelectSkill(UInt32 Index);
	void UpdateNative();
	void UpdateSoulBoundAndUnique();
	void UpdateReqLevelAndRemain();
	
	bool IsBattleSkillExist();
	void UpdatePartnerDesc(CPartner* partner);
	void UpdatePartnerModel();

	void ShowPartnerDetail(CPartner* pPartner, UInt16Vector skillIDs);
	void ShowPublicPartnerDetail(UInt32 char_id, UInt32 partner_id);
	
	void ShowMobDetail(UInt32 mobID, UInt8 mobLvl = 0);

	const SkillData* GetSelSkillData(){ return m_pTempSelSkillData;}
	CPartner* GetOnShowPartner(){ return m_pOnShowPartner;}
	UInt8 GetSelectIndex(){ return m_iSelectedIndex;};
	bool HasPartner();

	void UpdateRaceType();

	//partner collection
	void SetPartnerCollectionList(const UInt32Vector &PartnerIDs);
	void UpdatePartnerCollectionMenu(UInt8 race, UInt8 page);
	void ShowPartnerCollectionDetail(UInt8 race);
	void PartnerCollectionNextPage(UInt8 race, bool bNext);
	void SetNumField(StringA fieldName, Int32 fieldValue, UInt16 addedValue, Int16 PartnerCollectionValue);
	void GetPartnerCollectionAttri(AttriStruct2 & Attri);
	void PartnerCollectionUseBuffTemp(bool bUse);
	void DragPartner();
	void AddPartnerCollection();

	void UpdateCultivation();
	void UpdatePractice();

	//eat fast food
	void PartnerEatFastFood(UInt8 eatType);
};

#endif 