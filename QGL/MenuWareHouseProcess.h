#pragma once
#ifndef _MENUWAREHOUSEPROCESS_H_
#define _MENUWAREHOUSEPROCESS_H_

class CItem;
class CPartner;
class CPet;

class CMenuWareHouseProcess
{
private:
	RPCNetIDVector m_wareHouseItems;
	RPCNetIDVector m_wareHousePartners;
	RPCNetIDVector m_wareHousePets;
	UInt m_iStartWareHouseX;
	UInt m_iStartWareHouseY;
	UInt m_iCountDown;
	UInt m_iMenuIndex;

public:
	CMenuWareHouseProcess();
	~CMenuWareHouseProcess();

	void InitWareHouse(NetGroup* itemGroup, NetGroup* partnerGroup, NetGroup* petGroup, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup);
	void OpenWareHouse();
	void CloseWareHouse();
	void ResetWareHouse();
	void ShowWareHouseItem(int pageNo = 0);
	void ShowWareHousePartner();
	void ShowWareHousePet();
	void PutToWareHouse(int desIndex, int srcIndex, int stack);
	void RemoveItem(CItem* item, bool needDelete = false);
	void AddItem(CItem* item);
	void UpdateWareHouse();
	int GetMaxStack(int index);
	void GetFromWareHouse(int desIndex, int srcIndex, int stack);
	bool CheckCanSwap(int itemBagIndex, int wareHouseIndex);
	bool AvailSlot(int wareHouseIndex);
	void ShowItemDetail();
	void ShowPartnerDetail();
	void ShowPetDetail();
	void DisappearWareHouse();
	void MoveItem(int srcIndex, int desIndex);
	void ShowResult(UInt processType, UInt msgIndex);
	
	void PutPartnerToWareHouse(UInt32 partnerID);
	void AddPartner(CPartner* partner);
	void RemovePartner(CPartner* partner, bool needDelete = false);
	void GetPartnerFromWareHouse(int wareHouseIndex);
	CPartner* GetPartner(UInt32 partnerID);

	void PutPetToWareHouse(UInt32 petUID);
	void AddPet(CPet* pet);
	void RemovePet(CPet* pet, bool needDelete = false);
	void GetPetFromWareHouse(int wareHouseIndex);
	CPet* GetPet(UInt32 petUID);

	void JpackWareHouse(UInt8 itemPage);
};




#endif 
