#pragma once
#ifndef _MENUGIFTBOX_H_
#define _MENUGIFTBOX_H_

class CMenuGiftBox
{
private:
	UInt m_iMenuIndex;
	UInt m_iStartGiftBoxX;
	UInt m_iStartGiftBoxY;
	UInt m_iUpdateTime;
	
public:
	CMenuGiftBox();
	~CMenuGiftBox();

	void OpenGiftBoxInterface();
	void ShowGiftBoxItem();
	void ShowGiftBoxPartner();
	void ShowGiftBoxPet();
	void ShowConfirmGetGiftName(int slot);
	void ConfirmGetGift(int slot);
	void UpdateGiftBox();
	void CloseGiftBox();
	void ShowDetail(int slot);
	void DisappearGiftBox();
	void ShowResultMessage(UInt msgIndex, UInt32 id, UInt type);
	void UpdateGiftBoxFromServer();
};

#endif
