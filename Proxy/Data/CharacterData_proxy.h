///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_CHARACTERDATA_PROXY_H_
#define _RPC_CHARACTERDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/CharacterData_enum.h"

class CharacterData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id);
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &faction);
	virtual void OnUpdateLevelInfo(const UInt16 &level);
	virtual void OnUpdateExp(const UInt32 &exp);
	virtual void OnUpdateAttrib(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	virtual void OnUpdateAttrPoint(const UInt16 &attrPoint);
	virtual void OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP);
	virtual void OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP);
	virtual void OnUpdateDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP);
	virtual void OnUpdateWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp);
	virtual void OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP);
	virtual void OnUpdateGoodness(const Int32 &goodness);
	virtual void OnUpdateHonor(const UInt32 &honor);
	virtual void OnUpdateTitle(const String &title);
	virtual void OnUpdateManor(const UInt32 &manor);
	virtual void OnUpdateDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate);
	virtual void OnUpdateMoney(const UInt32 &money);
	virtual void OnUpdateBankMoney(const UInt32 &bankMoney);
	virtual void OnUpdateSMoney(const UInt32 &SMoney);
	virtual void OnUpdatePartner(const UInt32 &partner);
	virtual void OnUpdatePet(const UInt32 &pet);
	virtual void OnUpdateMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet);
	virtual void OnUpdateMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt32 &map_ownerId);
	virtual void OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y);
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	virtual void OnUpdateState(const UInt8 &state);
	virtual void OnUpdateBuff(const String &buff);
	virtual void OnUpdateShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9);
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value);
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	virtual void OnUpdateSpeakCount(const UInt16 &speakCount);
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime);
	virtual void OnUpdateTP(const UInt16 &TP);
	virtual void OnUpdateSpeakBlock(const UInt32 &speakBlock);
	virtual void OnUpdateHawkerTime(const UInt32 &hawkerTime);
	virtual void OnUpdateAvailBackPack(const UInt8 &availBackPack);
	virtual void OnUpdateAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse);
	virtual void OnUpdatePvPScoreSingle(const UInt32 &PvPScoreSingle);
	virtual void OnUpdateDeleteTime(const UInt32 &delete_time);
	virtual void OnUpdateBankLock(const Boolean &bankLock);
	virtual void OnUpdateSiblingId(const UInt32 &sibling_id);
	virtual void OnUpdateMark(const UInt32 &mark);
	virtual void OnUpdateWantedReward(const UInt32 &wantedReward);
	virtual void OnUpdateGuild_uid(const UInt32 &guild_uid);
	virtual void OnUpdateGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		levelInfoMask	= Parent::NextFreeMask + 2,
		expMask	= Parent::NextFreeMask + 3,
		attribMask	= Parent::NextFreeMask + 4,
		attrPointMask	= Parent::NextFreeMask + 5,
		HPMask	= Parent::NextFreeMask + 6,
		SPMask	= Parent::NextFreeMask + 7,
		DPMask	= Parent::NextFreeMask + 8,
		WCMask	= Parent::NextFreeMask + 9,
		VPMask	= Parent::NextFreeMask + 10,
		goodnessMask	= Parent::NextFreeMask + 11,
		honorMask	= Parent::NextFreeMask + 12,
		titleMask	= Parent::NextFreeMask + 13,
		manorMask	= Parent::NextFreeMask + 14,
		donateMask	= Parent::NextFreeMask + 15,
		moneyMask	= Parent::NextFreeMask + 16,
		bankMoneyMask	= Parent::NextFreeMask + 17,
		SMoneyMask	= Parent::NextFreeMask + 18,
		partnerMask	= Parent::NextFreeMask + 19,
		petMask	= Parent::NextFreeMask + 20,
		maxOtherMask	= Parent::NextFreeMask + 21,
		mapMask	= Parent::NextFreeMask + 22,
		rebornMask	= Parent::NextFreeMask + 23,
		styleMask	= Parent::NextFreeMask + 24,
		eqWeaponMask	= Parent::NextFreeMask + 25,
		eqHeadMask	= Parent::NextFreeMask + 26,
		eqClothMask	= Parent::NextFreeMask + 27,
		stateMask	= Parent::NextFreeMask + 28,
		buffMask	= Parent::NextFreeMask + 29,
		shortcutMask	= Parent::NextFreeMask + 30,
		PKInfoMask	= Parent::NextFreeMask + 31,
		orgHairStyleMask	= Parent::NextFreeMask + 32,
		orgEqClothMask	= Parent::NextFreeMask + 33,
		speakCountMask	= Parent::NextFreeMask + 34,
		offlineTimeMask	= Parent::NextFreeMask + 35,
		TPMask	= Parent::NextFreeMask + 36,
		speakBlockMask	= Parent::NextFreeMask + 37,
		hawkerTimeMask	= Parent::NextFreeMask + 38,
		availBackPackMask	= Parent::NextFreeMask + 39,
		availWareHouseMask	= Parent::NextFreeMask + 40,
		PvPScoreSingleMask	= Parent::NextFreeMask + 41,
		deleteTimeMask	= Parent::NextFreeMask + 42,
		bankLockMask	= Parent::NextFreeMask + 43,
		SiblingIdMask	= Parent::NextFreeMask + 44,
		markMask	= Parent::NextFreeMask + 45,
		wantedRewardMask	= Parent::NextFreeMask + 46,
		guild_uidMask	= Parent::NextFreeMask + 47,
		GenerationAccMask	= Parent::NextFreeMask + 48,
		NextFreeMask	= Parent::NextFreeMask + 49
	};
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetNickName() const { return m.nickName; }
	const UInt8& GetSex() const { return m.sex; }
	const UInt8& GetCclass() const { return m.cclass; }
	const UInt8& GetFaction() const { return m.faction; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetExp() const { return m.exp; }
	const UInt16& GetSTA() const { return m.STA; }
	const UInt16& GetSPR() const { return m.SPR; }
	const UInt16& GetSTR() const { return m.STR; }
	const UInt16& GetCON() const { return m.CON; }
	const UInt16& GetAGI() const { return m.AGI; }
	const UInt16& GetAttrPoint() const { return m.attrPoint; }
	const UInt16& GetHP() const { return m.HP; }
	const UInt16& GetMHP() const { return m.mHP; }
	const UInt16& GetMaxHP() const { return m.maxHP; }
	const UInt16& GetSP() const { return m.SP; }
	const UInt16& GetMSP() const { return m.mSP; }
	const UInt16& GetMaxSP() const { return m.maxSP; }
	const UInt16& GetDP() const { return m.DP; }
	const UInt16& GetMDP() const { return m.mDP; }
	const UInt16& GetMaxDP() const { return m.maxDP; }
	const UInt16& GetWC() const { return m.WC; }
	const UInt16& GetMaxWC() const { return m.maxWC; }
	const UInt16& GetWcExp() const { return m.WcExp; }
	const UInt16& GetVP() const { return m.VP; }
	const UInt16& GetMaxVP() const { return m.maxVP; }
	const Int32& GetGoodness() const { return m.goodness; }
	const UInt32& GetHonor() const { return m.honor; }
	const String& GetTitle() const { return m.title; }
	const UInt32& GetManor() const { return m.manor; }
	const UInt32& GetFDonate() const { return m.fDonate; }
	const UInt32& GetTotalFDonate() const { return m.totalFDonate; }
	const UInt32& GetMDonate() const { return m.mDonate; }
	const UInt32& GetTotalMDonate() const { return m.totalMDonate; }
	const UInt32& GetMoney() const { return m.money; }
	const UInt32& GetBankMoney() const { return m.bankMoney; }
	const UInt32& GetSMoney() const { return m.SMoney; }
	const UInt32& GetPartner() const { return m.partner; }
	const UInt32& GetPet() const { return m.pet; }
	const UInt8& GetMaxPartner() const { return m.maxPartner; }
	const UInt8& GetMaxPet() const { return m.maxPet; }
	const UInt16& GetMap_id() const { return m.map_id; }
	const UInt16& GetMap_x() const { return m.map_x; }
	const UInt16& GetMap_y() const { return m.map_y; }
	const UInt32& GetMap_ownerId() const { return m.map_ownerId; }
	const UInt16& GetReborn_map() const { return m.reborn_map; }
	const UInt16& GetReborn_x() const { return m.reborn_x; }
	const UInt16& GetReborn_y() const { return m.reborn_y; }
	const UInt16& GetPhoto_id() const { return m.photo_id; }
	const UInt16& GetHairStyle1() const { return m.hairStyle1; }
	const UInt16& GetHairStyle2() const { return m.hairStyle2; }
	const UInt16& GetHairColor() const { return m.hairColor; }
	const UInt32& GetEq_weapon() const { return m.eq_weapon; }
	const UInt16& GetEq_weaponColor() const { return m.eq_weaponColor; }
	const UInt32& GetEq_headBand() const { return m.eq_headBand; }
	const UInt16& GetEq_headColor() const { return m.eq_headColor; }
	const UInt32& GetEq_cloth() const { return m.eq_cloth; }
	const UInt16& GetEq_clothColor() const { return m.eq_clothColor; }
	const UInt8& GetState() const { return m.state; }
	const String& GetBuff() const { return m.buff; }
	const UInt16& GetShortcut(const UInt8 i) const { return m.shortcut[i]; }
	const UInt8& GetShortcuttype(const UInt8 i) const { return m.shortcuttype[i]; }
	const UInt16& GetPK_Value() const { return m.PK_Value; }
	const UInt16& GetOrg_hairStyle1() const { return m.org_hairStyle1; }
	const UInt16& GetOrg_hairColor() const { return m.org_hairColor; }
	const UInt32& GetOrg_eq_cloth() const { return m.org_eq_cloth; }
	const UInt16& GetOrg_eq_clothColor() const { return m.org_eq_clothColor; }
	const UInt16& GetSpeakCount() const { return m.speakCount; }
	const UInt32& GetOfflineTime() const { return m.offlineTime; }
	const UInt16& GetTP() const { return m.TP; }
	const UInt32& GetSpeakBlock() const { return m.speakBlock; }
	const UInt32& GetHawkerTime() const { return m.hawkerTime; }
	const UInt8& GetAvailBackPack() const { return m.availBackPack; }
	const UInt8& GetAvailItemWareHouse() const { return m.availItemWareHouse; }
	const UInt8& GetAvailPartnerWareHouse() const { return m.availPartnerWareHouse; }
	const UInt8& GetAvailPetWareHouse() const { return m.availPetWareHouse; }
	const UInt32& GetPvPScoreSingle() const { return m.PvPScoreSingle; }
	const UInt32& GetDelete_time() const { return m.delete_time; }
	const Boolean& GetBankLock() const { return m.bankLock; }
	const UInt32& GetSibling_id() const { return m.sibling_id; }
	const UInt32& GetMark() const { return m.mark; }
	const UInt32& GetWantedReward() const { return m.wantedReward; }
	const UInt32& GetGuild_uid() const { return m.guild_uid; }
	const UInt32& GetIHisGenAcc() const { return m.iHisGenAcc; }
	const UInt32& GetUiCurGenAcc() const { return m.uiCurGenAcc; }
	virtual void OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id) {};
	virtual void OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &faction) {};
	virtual void OnUpdateLevelInfo(const UInt16 &level) {};
	virtual void OnUpdateExp(const UInt32 &exp) {};
	virtual void OnUpdateAttrib(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI) {};
	virtual void OnUpdateAttrPoint(const UInt16 &attrPoint) {};
	virtual void OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP) {};
	virtual void OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP) {};
	virtual void OnUpdateDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP) {};
	virtual void OnUpdateWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp) {};
	virtual void OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP) {};
	virtual void OnUpdateGoodness(const Int32 &goodness) {};
	virtual void OnUpdateHonor(const UInt32 &honor) {};
	virtual void OnUpdateTitle(const String &title) {};
	virtual void OnUpdateManor(const UInt32 &manor) {};
	virtual void OnUpdateDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate) {};
	virtual void OnUpdateMoney(const UInt32 &money) {};
	virtual void OnUpdateBankMoney(const UInt32 &bankMoney) {};
	virtual void OnUpdateSMoney(const UInt32 &SMoney) {};
	virtual void OnUpdatePartner(const UInt32 &partner) {};
	virtual void OnUpdatePet(const UInt32 &pet) {};
	virtual void OnUpdateMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet) {};
	virtual void OnUpdateMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt32 &map_ownerId) {};
	virtual void OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y) {};
	virtual void OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor) {};
	virtual void OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor) {};
	virtual void OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor) {};
	virtual void OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor) {};
	virtual void OnUpdateState(const UInt8 &state) {};
	virtual void OnUpdateBuff(const String &buff) {};
	virtual void OnUpdateShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9) {};
	virtual void OnUpdatePKInfo(const UInt16 &PK_Value) {};
	virtual void OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor) {};
	virtual void OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor) {};
	virtual void OnUpdateSpeakCount(const UInt16 &speakCount) {};
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime) {};
	virtual void OnUpdateTP(const UInt16 &TP) {};
	virtual void OnUpdateSpeakBlock(const UInt32 &speakBlock) {};
	virtual void OnUpdateHawkerTime(const UInt32 &hawkerTime) {};
	virtual void OnUpdateAvailBackPack(const UInt8 &availBackPack) {};
	virtual void OnUpdateAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse) {};
	virtual void OnUpdatePvPScoreSingle(const UInt32 &PvPScoreSingle) {};
	virtual void OnUpdateDeleteTime(const UInt32 &delete_time) {};
	virtual void OnUpdateBankLock(const Boolean &bankLock) {};
	virtual void OnUpdateSiblingId(const UInt32 &sibling_id) {};
	virtual void OnUpdateMark(const UInt32 &mark) {};
	virtual void OnUpdateWantedReward(const UInt32 &wantedReward) {};
	virtual void OnUpdateGuild_uid(const UInt32 &guild_uid) {};
	virtual void OnUpdateGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc) {};
	void SetId(const UInt32 &acct_id, const UInt32 &char_id);
	void SetAcct_id(const UInt32 &acct_id);
	void SetChar_id(const UInt32 &char_id);
	void SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &faction);
	void SetNickName(const String &nickName);
	void SetSex(const UInt8 &sex);
	void SetCclass(const UInt8 &cclass);
	void SetFaction(const UInt8 &faction);
	void SetLevelInfo(const UInt16 &level);
	void SetLevel(const UInt16 &level);
	void SetExp(const UInt32 &exp);
	void SetAttrib(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI);
	void SetSTA(const UInt16 &STA);
	void SetSPR(const UInt16 &SPR);
	void SetSTR(const UInt16 &STR);
	void SetCON(const UInt16 &CON);
	void SetAGI(const UInt16 &AGI);
	void SetAttrPoint(const UInt16 &attrPoint);
	void SetHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP);
	void SetHP(const UInt16 &HP);
	void SetMHP(const UInt16 &mHP);
	void SetMaxHP(const UInt16 &maxHP);
	void SetSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP);
	void SetSP(const UInt16 &SP);
	void SetMSP(const UInt16 &mSP);
	void SetMaxSP(const UInt16 &maxSP);
	void SetDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP);
	void SetDP(const UInt16 &DP);
	void SetMDP(const UInt16 &mDP);
	void SetMaxDP(const UInt16 &maxDP);
	void SetWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp);
	void SetWC(const UInt16 &WC);
	void SetMaxWC(const UInt16 &maxWC);
	void SetWcExp(const UInt16 &WcExp);
	void SetVP(const UInt16 &VP, const UInt16 &maxVP);
	void SetVP(const UInt16 &VP);
	void SetMaxVP(const UInt16 &maxVP);
	void SetGoodness(const Int32 &goodness);
	void SetHonor(const UInt32 &honor);
	void SetTitle(const String &title);
	void SetManor(const UInt32 &manor);
	void SetDonate(const UInt32 &fDonate, const UInt32 &totalFDonate, const UInt32 &mDonate, const UInt32 &totalMDonate);
	void SetFDonate(const UInt32 &fDonate);
	void SetTotalFDonate(const UInt32 &totalFDonate);
	void SetMDonate(const UInt32 &mDonate);
	void SetTotalMDonate(const UInt32 &totalMDonate);
	void SetMoney(const UInt32 &money);
	void SetBankMoney(const UInt32 &bankMoney);
	void SetSMoney(const UInt32 &SMoney);
	void SetPartner(const UInt32 &partner);
	void SetPet(const UInt32 &pet);
	void SetMaxOther(const UInt8 &maxPartner, const UInt8 &maxPet);
	void SetMaxPartner(const UInt8 &maxPartner);
	void SetMaxPet(const UInt8 &maxPet);
	void SetMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt32 &map_ownerId);
	void SetMap_id(const UInt16 &map_id);
	void SetMap_x(const UInt16 &map_x);
	void SetMap_y(const UInt16 &map_y);
	void SetMap_ownerId(const UInt32 &map_ownerId);
	void SetReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y);
	void SetReborn_map(const UInt16 &reborn_map);
	void SetReborn_x(const UInt16 &reborn_x);
	void SetReborn_y(const UInt16 &reborn_y);
	void SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor);
	void SetPhoto_id(const UInt16 &photo_id);
	void SetHairStyle1(const UInt16 &hairStyle1);
	void SetHairStyle2(const UInt16 &hairStyle2);
	void SetHairColor(const UInt16 &hairColor);
	void SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor);
	void SetEq_weapon(const UInt32 &eq_weapon);
	void SetEq_weaponColor(const UInt16 &eq_weaponColor);
	void SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor);
	void SetEq_headBand(const UInt32 &eq_headBand);
	void SetEq_headColor(const UInt16 &eq_headColor);
	void SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor);
	void SetEq_cloth(const UInt32 &eq_cloth);
	void SetEq_clothColor(const UInt16 &eq_clothColor);
	void SetState(const UInt8 &state);
	void SetBuff(const String &buff);
	void SetShortcut(const UInt16 &shortcut0, const UInt16 &shortcut1, const UInt16 &shortcut2, const UInt16 &shortcut3, const UInt16 &shortcut4, const UInt16 &shortcut5, const UInt16 &shortcut6, const UInt16 &shortcut7, const UInt16 &shortcut8, const UInt16 &shortcut9, const UInt8 &shortcuttype0, const UInt8 &shortcuttype1, const UInt8 &shortcuttype2, const UInt8 &shortcuttype3, const UInt8 &shortcuttype4, const UInt8 &shortcuttype5, const UInt8 &shortcuttype6, const UInt8 &shortcuttype7, const UInt8 &shortcuttype8, const UInt8 &shortcuttype9);
	void SetShortcut(const UInt8 i, const UInt16 &shortcut);
	void SetShortcuttype(const UInt8 i, const UInt8 &shortcuttype);
	void SetPKInfo(const UInt16 &PK_Value);
	void SetPK_Value(const UInt16 &PK_Value);
	void SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor);
	void SetOrg_hairStyle1(const UInt16 &org_hairStyle1);
	void SetOrg_hairColor(const UInt16 &org_hairColor);
	void SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor);
	void SetOrg_eq_cloth(const UInt32 &org_eq_cloth);
	void SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor);
	void SetSpeakCount(const UInt16 &speakCount);
	void SetOfflineTime(const UInt32 &offlineTime);
	void SetTP(const UInt16 &TP);
	void SetSpeakBlock(const UInt32 &speakBlock);
	void SetHawkerTime(const UInt32 &hawkerTime);
	void SetAvailBackPack(const UInt8 &availBackPack);
	void SetAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse);
	void SetAvailItemWareHouse(const UInt8 &availItemWareHouse);
	void SetAvailPartnerWareHouse(const UInt8 &availPartnerWareHouse);
	void SetAvailPetWareHouse(const UInt8 &availPetWareHouse);
	void SetPvPScoreSingle(const UInt32 &PvPScoreSingle);
	void SetDeleteTime(const UInt32 &delete_time);
	void SetDelete_time(const UInt32 &delete_time);
	void SetBankLock(const Boolean &bankLock);
	void SetSiblingId(const UInt32 &sibling_id);
	void SetSibling_id(const UInt32 &sibling_id);
	void SetMark(const UInt32 &mark);
	void SetWantedReward(const UInt32 &wantedReward);
	void SetGuild_uid(const UInt32 &guild_uid);
	void SetGenerationAcc(const UInt32 &iHisGenAcc, const UInt32 &uiCurGenAcc);
	void SetIHisGenAcc(const UInt32 &iHisGenAcc);
	void SetUiCurGenAcc(const UInt32 &uiCurGenAcc);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 acct_id;
	UInt32 char_id;
	//info
	String nickName;
	UInt8 sex;
	UInt8 cclass;
	UInt8 faction;
	//levelInfo
	UInt16 level;
	//exp
	UInt32 exp;
	//attrib
	UInt16 STA;
	UInt16 SPR;
	UInt16 STR;
	UInt16 CON;
	UInt16 AGI;
	//attrPoint
	UInt16 attrPoint;
	//HP
	UInt16 HP;
	UInt16 mHP;
	UInt16 maxHP;
	//SP
	UInt16 SP;
	UInt16 mSP;
	UInt16 maxSP;
	//DP
	UInt16 DP;
	UInt16 mDP;
	UInt16 maxDP;
	//WC
	UInt16 WC;
	UInt16 maxWC;
	UInt16 WcExp;
	//VP
	UInt16 VP;
	UInt16 maxVP;
	//goodness
	Int32 goodness;
	//honor
	UInt32 honor;
	//title
	String title;
	//manor
	UInt32 manor;
	//donate
	UInt32 fDonate;
	UInt32 totalFDonate;
	UInt32 mDonate;
	UInt32 totalMDonate;
	//money
	UInt32 money;
	//bankMoney
	UInt32 bankMoney;
	//SMoney
	UInt32 SMoney;
	//partner
	UInt32 partner;
	//pet
	UInt32 pet;
	//maxOther
	UInt8 maxPartner;
	UInt8 maxPet;
	//map
	UInt16 map_id;
	UInt16 map_x;
	UInt16 map_y;
	UInt32 map_ownerId;
	//reborn
	UInt16 reborn_map;
	UInt16 reborn_x;
	UInt16 reborn_y;
	//style
	UInt16 photo_id;
	UInt16 hairStyle1;
	UInt16 hairStyle2;
	UInt16 hairColor;
	//eqWeapon
	UInt32 eq_weapon;
	UInt16 eq_weaponColor;
	//eqHead
	UInt32 eq_headBand;
	UInt16 eq_headColor;
	//eqCloth
	UInt32 eq_cloth;
	UInt16 eq_clothColor;
	//state
	UInt8 state;
	//buff
	String buff;
	//shortcut
	UInt16 shortcut[10];
	UInt8 shortcuttype[10];
	//PKInfo
	UInt16 PK_Value;
	//orgHairStyle
	UInt16 org_hairStyle1;
	UInt16 org_hairColor;
	//orgEqCloth
	UInt32 org_eq_cloth;
	UInt16 org_eq_clothColor;
	//speakCount
	UInt16 speakCount;
	//offlineTime
	UInt32 offlineTime;
	//TP
	UInt16 TP;
	//speakBlock
	UInt32 speakBlock;
	//hawkerTime
	UInt32 hawkerTime;
	//availBackPack
	UInt8 availBackPack;
	//availWareHouse
	UInt8 availItemWareHouse;
	UInt8 availPartnerWareHouse;
	UInt8 availPetWareHouse;
	//PvPScoreSingle
	UInt32 PvPScoreSingle;
	//deleteTime
	UInt32 delete_time;
	//bankLock
	Boolean bankLock;
	//SiblingId
	UInt32 sibling_id;
	//mark
	UInt32 mark;
	//wantedReward
	UInt32 wantedReward;
	//guild_uid
	UInt32 guild_uid;
	//GenerationAcc
	UInt32 iHisGenAcc;
	UInt32 uiCurGenAcc;
} m;

public:
	INLINE CharacterData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult CharacterData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_CharacterData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<1303466261>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(592655900)
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(1303466261)
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_CHARACTERDATA_PROXY_H_
