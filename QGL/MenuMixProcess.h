#pragma once
#ifndef _MENUMIX_H_
#define _MENUMIX_H_


struct MixShopData;
struct ItemMixData;
class CItem; 

#define MIX_SHOP_NONE 50
#define MIX_SHOP_NO_ITEM 255
#define MIX_SHOP_MAKE_DISPLAY_SOURCE 4
#define MIX_SHOP_ITEM_DISPLAY_SOURCE 3

//for weapon, cloth, making shop
enum ItemMixSiwtch
{
	MIX_SWITCH_OFF = 0,
	MIX_SWITCH_ITEM,
	MIX_SWITCH_STONE,
	MIX_SWITCH_LIGHT_STONE,
};


class CMenuMixProcess
{
public:
	CMenuMixProcess();
	~CMenuMixProcess();
	void OpenMixShop(UInt8 shopID);
	
	void SetSelectedIndex(int index);
	void ResetMixShopMake();	
	void ResetMixShop();
	void MixShopMakeProductScroll(UInt8 up);
	void MixShopMakeSourceScroll(UInt8 up);

	void UpdateTimeLine(UInt32 currTime);
	void StartServerTime();
	void StartTimeLine();

	bool HaveSelectedItemMix();
	bool IsSuitableItemMix(int picIndex, int itemSlot);
	void SetSourceItem(int srcIconIndex, int itemSlot);
	void ShowMixShopResult(Boolean success, UInt8 msgCode, UInt32Vector item_uids, UInt8Vector reduceStacks);

	void ResetItemMix();
	void ResetAllMixItem();
	
	void ResetProductScrollBar();
	bool IsStartMaking();
	void CancelMake();

	void RemoveMakeItem(int sourceIndex);
	void RemoveMixItem(int picIndex);
	void UpdateProbText(bool enable, UInt8 index, UInt8 value = 0);
	
	bool IsSuitableMixShopItem(int picIndex, int itemSlot);
	void SetMixShopItem(int srcIconIndex, int itemSlot, int stack);
	void SetMakeEqSourceItem(int srcIconIndex, int itemSlot);
	void SetLiveSkillSourceItem(int srcIconIndex, int itemSlot);

	void SetRemainItemCount(int remainCount);
	void ReduceLiveSkillItemMix(UInt32Vector item_uids, UInt8Vector reduceStacks);
	inline void SetMakeAll(bool makeAll){ m_bMakeAll = makeAll;}
	bool IsRepairShop();
	bool IsStoneMixShop();
	int GetMaxStoneNumByItemSlot(int itemSlot);
	bool HasResultStone(int itemSlot, bool showErrMessage);
	
	void UpdateMixItemDetail(int itemPos);
	void SetMixItemText(StringA componentName, String text, Color32 textColor = 0, bool addDirty = false);
	void SetUIMixItemVerticalPosAndEnable(StringA component, UInt curY, bool enabled);
	void SetMixItemRowEnable(StringA prefix, bool show, UInt curY = 0, String showText = _T(""), Color32 textColor = 0);
	//void ResetAllGemItem();
	//void ResetMixShopGem(){}

	bool IsUnSoulBoundShop();
	int GetUnSoulBoundStoneNum();
	void DisappearMixShop();

	bool IsSuitableGemMixShopItem(int picIndex, int itemSlot);
	bool IsGemMixShop();
	void SetGemMixShopItem(int srcIconIndex, int itemSlot, int stack);
	bool IsGemInlayShop();
	bool IsGemRemoveShop();
	bool IsGemMeltingShop();
	bool IsGemPolishingShop();
	bool IsColorStoneMixShop();
	void SetGemRemoveIndex(int index);
	Boolean GetMixShopState(){return m_isOpenMixShop;}
	bool isShowItem(int index);

	void SetIronShopItem(int srcIconIndex, int itemSlot, int stack);
	bool IsSuitableIronShopItem(int picIndex, int itemSlot);
	bool IsIronShopMake();
	bool IsIronShopRepair();
	void ResetIronShop();
	void ResetIronItem();
	void RemoveIronItem(int picIndex);
private:

	const MixShopData* m_pMixShop;
	UInt16 m_iStartProductIndex;  //for MixShopProductScrollBar and MixShopMixOrReplace bar
	int m_iSelectedIndex;
	UInt16 m_iShowLineCount;
	const ItemMixData* m_pSelectedItemMix;
	UInt m_iStartSourceIndex; //for MixShopDragItemScrollBar
	UInt m_iRemainItemCount;
	bool m_bMakeAll;
	bool m_bHasLightStone;
	UInt m_iStartMixShopX;
	UInt m_iStartMixShopY;

	UInt32 m_iStartTime;
	CItem* m_pEqItem; //use in repair
	   
	Boolean m_isOpenMixShop;
	//Use for making weapon, cloth
	typedef StlVector<const ItemMixData*, ManagedAllocator<const ItemMixData* > > ItemMixDataList;
	struct ItemMixType
	{
		bool isOpen;
		ItemMixDataList dataList;
		ItemMixType()
		{
			isOpen = false;
			dataList.clear();
		}

		~ItemMixType()
		{
			isOpen = false;
			dataList.clear();
		}
	};

	struct ShowItem
	{
		UInt8 type;
		const ItemMixData* item;
		ShowItem()
		{
			ZeroMemory(this, sizeof(ShowItem));
			type = 0;
			item = NULL;
		}

		~ShowItem()
		{
			type = 0;
			item = NULL;
		}
	};

	struct ItemMixSource
	{
		UInt32 sourceRootID;
		UInt8 maxStack;
		UInt8 nowStack;
		UInt8Vector nowStacks;
		UInt8Vector itemSlots;
		UInt32Vector item_uids;
		UInt8 itemSwitch;
		
		ItemMixSource()
		{
			sourceRootID = 0;
			itemSwitch = 0;
			maxStack = 0;
			nowStack = 0;
			nowStacks.clear();
			itemSlots.clear();
			item_uids.clear();
		}

		~ItemMixSource()
		{
			sourceRootID = 0;
			itemSwitch = 0;
			maxStack = 0;
			nowStack = 0;
			nowStacks.clear();
			itemSlots.clear();
			item_uids.clear();
		}
	};

	enum MixShopGemColorIndex
	{
		GEM_FOR_WEAPON = 0,
		GEM_FOR_HEAD_ASS,
		GEM_FOR_CLOTH,
		GEM_FOR_BELT,
		GEM_FOR_RING,
		GEM_FOR_SHOE,
	};
	typedef StlMap<UInt8, ItemMixType*, std::less<UInt8>, ManagedAllocator<std::pair<UInt8, ItemMixType*> > > ItemMixTypeList;
	typedef StlVector<ShowItem, ManagedAllocator<ShowItem > > ShowDataList;
	typedef StlVector<ItemMixSource, ManagedAllocator<ItemMixSource > > ItemMixSourceList;
	ItemMixTypeList m_typeItemList;
	ShowDataList m_showList;
	ItemMixSourceList m_sourceList;
	ItemMixSourceList m_mixShopItem;
	ItemMixSourceList m_mixShopGem;

	void OpenMixOrRepair();
	void UpdateMixOrRepairItem();

	void OpenMixShopEqMaking(); //for weapon, cloth or other making shop
	void UpdateSelectItemList();

	void OpenIronMakeOrRepair();
	void UpdateIronMakeOrRepair();

	UInt16 GetShowLineExist();
	void UpdateItemMixSource();
	void MakeSourceList();

	bool IsConsumeAccept(UInt8Vector consumeType, UInt32Vector consumerAmount, bool showMsg = false);

	void ShowMakeConsume(bool show);
	void ShowMixConsume(bool show);
	void OpenMixShopLiveSkill();
	void ClearItemMixSourceList(ItemMixSourceList &list, UInt index);

	UInt32 m_GemMixFirstItemId;
	bool m_bNeedSecondCheck;
	int m_iGemRemoveSelectIndex;
	UInt32 m_GemRemoveSelectGemId;

	void OpenGem();
	//void ShowGemConsume(bool show);
	void UpdateGem();
};

#endif