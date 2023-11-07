#pragma once
#ifndef _MENU_HAWKER_PROCESS_H_
#define _MENU_HAWKER_PROCESS_H_

#define MAX_HAWKER_ITEM 24
#define MAX_HAWKER_PARTNER 24
#define MAX_HAWKER_PET 24
#define HAWK_SLOT_PER_PAGE 12

#include "Proxy/Logic/Character.h"
#include <list>

class CItem;
//struct HawkerItemInfo;
class CHawkItem;
//struct HawkerHistory;

class CMenuHawkerProcess
{
private:

	struct HawkerHistoryInfo
	{
		String itemName;
		HawkerHistory data;

		HawkerHistoryInfo()
		{
			itemName.Empty();
		}

		~HawkerHistoryInfo()
		{
			itemName.Empty();
		}

		BOOL operator==(HawkerHistoryInfo second)
		{
			bool eq = true;
			if (data.historyId != second.data.historyId)
				eq = false;
			return eq;
		};
	};

	UInt8 m_iBagIndex;
	UInt8 m_iMenuIndex;
	UInt8 m_iStartIndex;
	UInt8 m_iSelectedPartnerIndex;
	UInt8 m_iSelectedPetIndex;
	UInt8 m_iHawkMenuIndex;
	UInt8 m_iHawkPageIndex;
	bool m_bOpenHawk, m_bStartHawk;
	bool m_bSetConcernTarget;
	UInt32 m_targetId;
	Boolean m_allHawkEnable;

	typedef StlVector<HawkerItemInfo, ManagedAllocator<HawkerItemInfo> > HawkerItemList;
	typedef StlVector<CHawkItem*, ManagedAllocator<CHawkItem*> > HawkItemList;

	//UInt8 itemIndex, UInt8 stack, UInt32 price)
	HawkerItemList m_hawkerItems;
	HawkerItemList m_hawkerPartners;
	HawkerItemList m_hawkerPets;

	HawkerItemList m_InithawkerItems;
	HawkerItemList m_InithawkerPartners;
	HawkerItemList m_InithawkerPets;

	HawkItemList m_hawkItemList;
	HawkItemList m_hawkPartnerList;
	HawkItemList m_hawkPetList;

	std::list<UInt32 > m_concernCharIdList;
	std::list<UInt32 > m_startHawkCharIdList;

	typedef StlDeque<HawkerHistoryInfo, ManagedAllocator<HawkerHistoryInfo> > HawkerHistoryInfoDeque;

	HawkerHistoryInfoDeque m_hawkerHistory;

	enum HawkerMessageIndex
	{
		HAWK_CANT_DO_ANYTHING = 0,
		HAWK_CANT_HAWK,
		HAWK_CANT_FOR_SELL,
		HAWK_STOP_HAWK,
	};


public:

	CMenuHawkerProcess();
	~CMenuHawkerProcess();

	void OpenHawkerOwnerMenu();

	void UpdateOwnerItem(int itemBag = -1);
	void UpdateOwnerItemSlot(UInt8 itemSlot, bool addDirty = false);

	void UpdateOwnerEqItem();

	void UpdateOwnerModel();
	void UpdateOwner();
	void UpdateOwnerPartner(int index = -1);
	void UpdateOwnerPet(int petIndex = -1);
	void SetIconOff(int iconPos, bool eq);
	void UpdateOwnerPartnerNameList();
	void UpdateOwnerPetNameList();
	INLINE UInt8 GetMenuIndex(){return m_iMenuIndex;}

	void ChangeWeapon(String name);
	void UpdateCharWeaponEnt(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);

	void UpdateHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	void ChangeHeadAccessory(String name, String colorFile);
	void ChangeBody(UInt16 eqCloth);
	void UpdateBody(const UInt32 &eq_cloth);

	void SetItemHawk(UInt8 itemIndex, UInt8 stack, UInt32 price);
	void SetPartnerHawk(UInt32 price);
	void SetPetHawk(UInt32 price);

	void SelectHawkItem();
	void SelectHawkPartner();
	void SelectHawkPet();
	void SelectHawkExchangeRecord();

	void UpdateHawkItem(UInt8 page);
	void UpdateHawkPartner(UInt8 page);
	void UpdateHawkPet(UInt8 page);
	void UpdateHawkExchangeRecord(UInt8 page);

	void TurnPage(bool next); //true: next, false:prev

	void SelectPartnerOrPet(UInt8 index);

	bool NotForSellPartner(bool showMsg = false);
	bool NotForSellPet(bool showMsg = false);

	void ResetHawkerOwner();
	void ResetItemPartnerPet();

	bool IsOpenHawkMenu(){return m_bOpenHawk;}

	void DragPartner();
	void DragPet();
	void ResetSellItem(UInt8 index);

	UInt8 GetHawkMenuIndex();
	void StartHawk();
	void StopHawk();

	bool IsStartHawk();

	void ClickHawkAlertPanel();

	void SelectHawkSellItem();
	void SelectHawkSellPartner();
	void SelectHawkSellPet();

	void ShowHawkMessage(UInt8 index);

	void ChangeHawkName();
	void CloseHawkFromTarget();

	int GetMaxItemStackForHawk(UInt8 index);
	int GetItemPriceForHawk(UInt8 index);
	void ShowItemPriceForHawk(UInt8 index, UInt8 stack);

	void BuyHawkItem(UInt8 itemIndex, UInt8 itemMenuIndex, UInt8 itemStack);

	void UpdateHawkerItemList(UInt32 item_uid, UInt8 remainStack);
	void UpdateHawkerPartnerList(UInt32 partner_id);
	void UpdateHawkerPetList(UInt32 pet_id);

	void SetMajorPanelToTop();
	void UpdateUserMoneyDisplay();

	void ShowHawkSelltemDetail();
	void UpdateHawkerTitle();

	void ResumeHawkerOwner();
	INLINE void SetStartHawkFlag(bool start){ m_bStartHawk = start;}

	void HawkerOwnerNameListScroll(UInt8 index); //0: up, 1:down, 2:scroll
	void ResetOwnerScroll();
	void UpdateHawkerTime();

	void DisappearHawkPanel();

	void ClosePanel();
	void AddHawkerHistory(HawkerHistory data, String itemName, bool pushback = true); //true: push in back, false: push in front
	void RemoveHawkerHistory(UInt32Vector delHistoryID);

	void SetItemMoneyColour();

	void InitItemPartnerPet();

	void SetConcernTarget();

	void InitTargetConcernState(UInt32 targetId);

	Boolean IsConcernTarget(UInt32 targetId);

	void SetAllHawkEnable(Boolean enable);

	Boolean GetAllHawkEnable();

	void AddStartHawkInAllList(UInt32 targetId);

	void ReSetConcernTarget(UInt32 targetId);

	void ReSetHawkInAllList(UInt32 targetId);
};



#endif 