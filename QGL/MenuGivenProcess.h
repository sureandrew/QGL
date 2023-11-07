#pragma once
#ifndef _MENUGIVEN_H_
#define _MENUGIVEN_H_

struct GivenItemInfo
{
	UInt32 item_uid;
	UInt32 item_id;
	UInt8 item_stack;
	UInt8 item_level;
	UInt8 itemIndex_inbag;

	GivenItemInfo()
	{
		item_uid = 0;
		item_id = 0;
		item_stack = 0;
		item_level = 0;
		itemIndex_inbag = 0;
	}
};

typedef StlVector<GivenItemInfo*, ManagedAllocator<GivenItemInfo*> > GivenItemList;


#define GIVENITEM_MAX		6
#define GIVENPARTNER_MAX	3
#define GIVENPET_MAX		3

class CMenuGivenProcess
{
private:
	GivenItemList m_itemList;
	String m_targetCharName;
	UInt32	m_iTargetCharID;
	RPCNetID	m_iTargetCharNetID;
	UInt32Vector m_partnerList;
	UInt32Vector m_petList;
	bool m_bNpcGiven;
	UInt32Vector m_npcGivenIDList;
	UInt8Vector m_npcGivenTypeList;
public:
	CMenuGivenProcess();
	virtual ~CMenuGivenProcess();
	void SetItemInfo(UInt8 index, UInt32 item_uid, UInt32 item_id, UInt8 item_stack, UInt8 item_level, UInt8 item_index_inbag);
	void ResetAll();
	void OpenGivenPanel(String targetName, UInt32 targetID, RPCNetID targetNetID);
	INLINE String GetTargetCharName(){ return m_targetCharName;}
	INLINE UInt32 GetTargetCharID(){ return m_iTargetCharID;}
	
	void SetTargetCharIDNameToPanel();
	void SetItem(int givenIndex, int itemSlot, int stack);
	void RemoveItem(int givenIndex);
	void SetPartner(int givenIndex, UInt32 partnerID);
	void RemovePartner(int givenIndex);
	void SetPet(int givenIndex, UInt32 petID);
	void RemovePet(int givenIndex);
	void ConfirmGiven();
	void ResetItemPartnerPet();
	inline bool IsNpcGiven() { return m_bNpcGiven;}
	void OpenNpcGivenPanel(UInt32 npcID, UInt32Vector idList, UInt8Vector typeList, String npcMessage);
	bool IsSuitableNpcGivenItem(int itemSlot);
	bool IsSuitableNpcGivenPartner(UInt32 partnerID);
	bool IsSuitableNpcGivenPet(UInt32 petID);
	void SetIsNpcGiven(bool isNpcGiven) { m_bNpcGiven = isNpcGiven;}
	void ShowMoneyColour();
};



#endif 