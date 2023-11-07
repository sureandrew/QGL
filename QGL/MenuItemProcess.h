#pragma once
#ifndef _MENUITEM_H_
#define _MENUITEM_H_

#include "Common.h"
#include <Reuben/Math/Hex.h>

#define MAX_PET_PARTNER_ONE_PAGE 6

struct ExtractedBuffData;
class CItem;

class CMenuItemProcess
	//: public Reuben::Simulation::Object
{
	private:
		int m_iSoundIndex;
		int m_iBag;
		UInt8 m_iMenuIndex;			//0: character, 1:partner, 2:pet
		UInt8 m_iStartIndex;
		
		UInt8 m_iSelectedPartnerIndex;
		UInt8 m_iSelectedPetIndex;

		UInt m_iIdentifyItemSlot;
		UInt m_iIdentifyToolItemSlot;
		StringA m_Text;
		ByteArray m_hashed_empty_pw;

	public:

		//DEFINE_CLASS(0);

		enum ItemSound
		{
			ITEM_TAKE = 0,
			ITEM_PUT,
			ITEM_EQUIP_WP,
			ITEM_OFF_WP,
			ITEM_EQUIP_CLOTH,
			ITEM_OFF_CLOTH,
			ITEM_EQUIP_OTHER,
			ITEM_OFF_OTHER,
			ITEM_USE,
			ITEM_CANT_USE_OR_EQUIP,
			ITEM_DROP,
		};

		CMenuItemProcess()
		{ 
			m_Text = "";
			m_iSoundIndex = -1;
			m_iBag = 0;
			m_iMenuIndex = 0;
			m_iStartIndex = 0;
					
			m_iSelectedPartnerIndex = 0;
			m_iSelectedPetIndex = 0;
			m_iIdentifyItemSlot = 255;
			m_iIdentifyToolItemSlot = 255;
			TCHAR emptyPass[65];
			for (int i = 0; i < 65; ++i)
				emptyPass[i] = 32;	
			emptyPass[64] = NULL;
			Size size = 0;
			Reuben::Math::HexToBytes(NULL, size, emptyPass, 64);
			m_hashed_empty_pw.SetSize(size);
			Reuben::Math::HexToBytes(&m_hashed_empty_pw, size, emptyPass, 64);
			m_bUnlockPassEnter = BOOLEAN_FALSE;	
		}

		virtual ~CMenuItemProcess(){m_Text.Empty();}

		void UpdateShortCutSlot(int itemPos);
		void WriteToShortCut(int itemPos, int targetPos, bool NetUpdate = true);
		void CopyToShortCut(int itemPos, int targetPos);

		void MoveEquipToBag(int itemPos, int targetPos);		
		void MoveToBag(int itemPos, int bag, int target = -1);
		
		void DropItem(int itemPos, int dropSize);
		void EquipItemUse(int itemPos);
		
		void ShowMessage(StringA message="");
		void ItemUse(int itemPos);
		
		void UpdateItemMenu(int bag = -1);
		void PlaySound();

		INLINE void SetSoundIndex(int index){	m_iSoundIndex = index;	}

		INLINE void SetBag(int bag){	m_iBag = bag;}
		INLINE Int GetBagPage() { return m_iBag; };

		void UpdateItemDetail(int itemPos);
		void ItemDetailPopUp(UInt32 ItemId, bool bShop = false, CItem* item = NULL, bool showPrice = true);

		void UpdateItemPartner(int partnerIndex=-1);
		void UpdateItemPet(UInt8 petIndex=0);
		void UpdateItemEq();

		void NewUpdateItemMenu(int bag = -1);
		void NewUpdateItemCharacter();
		void SetIconOff(int iconPos, bool eq);   //true: eqItem, false: normalItem
		void UpdateCharacterEnt();
		void Scroll(int up); //0:up, 1:down, 2:scroll
		void ResetScroll();

		void UpdateItemPartnerNameList();
		void UpdateItemPetNameList();
		void SetUIItemVerticalPosAndEnable(StringA component, UInt curY, bool enabled);
		void SetItemRowEnable(StringA prefix, bool show, UInt curY = 0, String showText = _T(""), Color32 textColor = 0);
		void SetItemText(StringA componentName, String text, Color32 textColor = 0, bool addDirty = false);

		void ChangeWeapon(String name);
		void UpdateCharWeaponEnt(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);

		void UpdateHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
		void ChangeHeadAccessory(String name, String colorFile);
		void ChangeBody(UInt16 eqCloth);
		void UpdateBody(const UInt32 &eq_cloth);
		void UpdateItemSlot(UInt8 slot, bool addDirty = false);

		void DragPartner();
		void DragPet();
		UInt32 GetSelectedPartnerID();
		UInt32 GetSelectedPetID();
		bool IsGivenPartner();
		bool IsGivenPet();
		INLINE UInt8 GetMenuIndex(){ return m_iMenuIndex;}
		INLINE UInt8 GetMenuSelectedPartnerIndex(){ return m_iSelectedPartnerIndex;}
		INLINE UInt8 GetMenuSelectedPetIndex(){ return m_iSelectedPetIndex;}
		INLINE UInt8 GetMenuStartIndex(){ return m_iStartIndex;}
		UInt8 GetItemStack(UInt8 index);
		bool IsItemBind(UInt8 index);
		bool IsItemGiven(UInt8 index);
		bool IsItemSent(UInt8 index);
		bool NeedRedBox(UInt8 index);
		void BindCantTrading(UInt8 type = 0); // 0: trading, 1:given

		void ShowPublicItemDetail(UInt32 char_id, UInt32 item_uid);
		bool IsItemSellable(UInt8 index);

		void JpackBag(UInt8 bagType);

        inline void ResetIdentify()
		{
			m_iIdentifyItemSlot = 255;
			m_iIdentifyToolItemSlot = 255;
		}

		inline void SetIdentify(UInt identifyItemSlot, UInt identifyToolSlot)
		{
			m_iIdentifyItemSlot = identifyItemSlot;
			m_iIdentifyToolItemSlot = identifyToolSlot;
		}

		void ShowIdentifyDiag(UInt identifyItemSlot, UInt identifyToolSlot);
		void IdentifyItem();

		void LockItem(UInt32 index, UInt32 assetType);
		void UnlockItem(UInt32 bagIndex, UInt32 assetType);	
		void OpenUnlockItemPanel(UInt32 index, UInt32 assetType);
		Boolean CanPerformUnlockORLock(UInt32 index, UInt32 assetType, Boolean locktype);
		void ConfirmAssetLockPassword(Int lockType);	
		void ConfirmAssetUnLockPassword(Int index, UInt32 assetType);
		bool IsPartnerLocked(UInt32 partnerID);
		bool IsPetLocked(UInt32 petID);	
		void ForceResetAssetPassword();
		Boolean m_bUnlockPassEnter;

	

};



#endif 
