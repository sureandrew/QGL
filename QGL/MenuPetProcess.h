#pragma once
#ifndef _MENUPETPROCESS_H_
#define _MENUPETPROCESS_H_

#include "Common.h"

class CPet;
class CMenuPetProcess
{
public:
	CMenuPetProcess();
	~CMenuPetProcess();

	void ResetMenu();
	void RefreshMenu();
	void PageUp();
	void PageDown();
	void Scroll();
	void HideSelectInfoItem();
	void HideRightUpItem();
	void ShowSelectedInfo();
	void SetSelIndex(Int index);
	void UsePet();
	void PetRest();
	void DumpPet();
	void RefreshRightUpPanel();
	void ChangePetName();
	void ShowRemovePetName();
	void OpenPetSkillDetail(int index);
	void ClosePetSkillDetail();

	void ShowPetDetail(CPet* pPet, UInt16Vector skillIDs);
	void ShowPetDetail(UInt32 petID, UInt8 petLvl = 0);
	void ShowPublicPetDetail(UInt32 char_id, UInt32 pet_id);

	inline Int GetPetSelectIndex() {return m_SelIndex;}
	bool HasPet();

	bool PetCanUseItem(UInt16 itemSlot);

	void SetUseFoodItem(UInt16 itemSlot);
	bool CanEatFood();
	void ReSetFoodUseItem();
	void PetUseFood();
	void SetUseFoodMsg(bool isSucceed) {m_useFoodMsg = isSucceed;}
	bool GetUseFoodMsg(){return m_useFoodMsg;}
	void InitUsePetFoodPanel();


	UInt16 m_StartIndex;
	Int m_SelIndex;

	UInt32	m_currPetUid;

	UInt16 m_useItemSlot;
	bool m_useFoodMsg;
};


#endif // _MENUSHOPPROCESS_H_