#pragma once
#ifndef _MENUTARGETEQUIP_H_
#define _MENUTARGETEQUIP_H_

class CItem;
struct ItemSetData;
struct EquipItemSetInfo;
class CMenuTargetEquipProcess
{
//private:
    	
public :
	CMenuTargetEquipProcess();
	virtual ~CMenuTargetEquipProcess();

	void UpdateTargetEquip();
	void SetIconOff(int iconPos);
	void SetItemText(StringA componentName, String text, Color32 textColor, bool addDirty);

	void RequestTargetEquip();
	void TargetEquipReply(NetGroup * itemGroup);
	void FreeTargetEquip();

	void UpdateTargetEquipDetail(int bagIndex);
	bool HasTargetEquip(int bagIndex);

	// target equip
	CItem** m_items;

	UInt32 m_char_id;
	String m_char_name;

	// itemset
	typedef StlMap<UInt32,EquipItemSetInfo, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, EquipItemSetInfo> > > EquipItemSetInfoMap;
	EquipItemSetInfoMap		m_EquipItemSetInfoMap;

	void UpdateItemSet();
	EquipItemSetInfo* GetItemSetInfo(UInt32 itemset_id);

};


#endif 