//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/MsgCharacterData_stub.h"

void MsgCharacterData_Stub::SetId(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void MsgCharacterData_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void MsgCharacterData_Stub::SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &faction)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	if (m.sex != sex) { m.sex = sex; modified = true; }
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MsgCharacterData_Stub::SetNickName(const String &nickName)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MsgCharacterData_Stub::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MsgCharacterData_Stub::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MsgCharacterData_Stub::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MsgCharacterData_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void MsgCharacterData_Stub::SetTitle(const String &title)
{
	bool modified = false;
	if (m.title != title) { m.title = title; modified = true; }
	MarkUpdateAll(titleMask, modified);
}

void MsgCharacterData_Stub::SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MsgCharacterData_Stub::SetPhoto_id(const UInt16 &photo_id)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MsgCharacterData_Stub::SetHairStyle1(const UInt16 &hairStyle1)
{
	bool modified = false;
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MsgCharacterData_Stub::SetHairStyle2(const UInt16 &hairStyle2)
{
	bool modified = false;
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MsgCharacterData_Stub::SetHairColor(const UInt16 &hairColor)
{
	bool modified = false;
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MsgCharacterData_Stub::SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void MsgCharacterData_Stub::SetEq_weapon(const UInt32 &eq_weapon)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void MsgCharacterData_Stub::SetEq_weaponColor(const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void MsgCharacterData_Stub::SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void MsgCharacterData_Stub::SetEq_headBand(const UInt32 &eq_headBand)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void MsgCharacterData_Stub::SetEq_headColor(const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void MsgCharacterData_Stub::SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void MsgCharacterData_Stub::SetEq_cloth(const UInt32 &eq_cloth)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void MsgCharacterData_Stub::SetEq_clothColor(const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void MsgCharacterData_Stub::SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void MsgCharacterData_Stub::SetOrg_hairStyle1(const UInt16 &org_hairStyle1)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void MsgCharacterData_Stub::SetOrg_hairColor(const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void MsgCharacterData_Stub::SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void MsgCharacterData_Stub::SetOrg_eq_cloth(const UInt32 &org_eq_cloth)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void MsgCharacterData_Stub::SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void MsgCharacterData_Stub::SetFriend_status(const UInt8 &friend_status)
{
	bool modified = false;
	if (m.friend_status != friend_status) { m.friend_status = friend_status; modified = true; }
	MarkUpdateAll(friend_statusMask, modified);
}

void MsgCharacterData_Stub::SetFriend_icon(const UInt16 &friend_icon)
{
	bool modified = false;
	if (m.friend_icon != friend_icon) { m.friend_icon = friend_icon; modified = true; }
	MarkUpdateAll(friend_iconMask, modified);
}

void MsgCharacterData_Stub::SetFriend_about(const String &friend_about)
{
	bool modified = false;
	if (m.friend_about != friend_about) { m.friend_about = friend_about; modified = true; }
	MarkUpdateAll(friend_aboutMask, modified);
}

void MsgCharacterData_Stub::SetFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply)
{
	bool modified = false;
	if (m.friend_onlineAutoReply != friend_onlineAutoReply) { m.friend_onlineAutoReply = friend_onlineAutoReply; modified = true; }
	if (m.friend_offlineAutoReply != friend_offlineAutoReply) { m.friend_offlineAutoReply = friend_offlineAutoReply; modified = true; }
	MarkUpdateAll(friend_replyMask, modified);
}

void MsgCharacterData_Stub::SetFriend_onlineAutoReply(const String &friend_onlineAutoReply)
{
	bool modified = false;
	if (m.friend_onlineAutoReply != friend_onlineAutoReply) { m.friend_onlineAutoReply = friend_onlineAutoReply; modified = true; }
	MarkUpdateAll(friend_replyMask, modified);
}

void MsgCharacterData_Stub::SetFriend_offlineAutoReply(const String &friend_offlineAutoReply)
{
	bool modified = false;
	if (m.friend_offlineAutoReply != friend_offlineAutoReply) { m.friend_offlineAutoReply = friend_offlineAutoReply; modified = true; }
	MarkUpdateAll(friend_replyMask, modified);
}

void MsgCharacterData_Stub::SetLine(const UInt8 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(lineMask, modified);
}

void MsgCharacterData_Stub::SetLine_id(const UInt8 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(lineMask, modified);
}

void MsgCharacterData_Stub::SetSpeakCount(const UInt16 &globalSpeakCount)
{
	bool modified = false;
	if (m.globalSpeakCount != globalSpeakCount) { m.globalSpeakCount = globalSpeakCount; modified = true; }
	MarkUpdateAll(speakCountMask, modified);
}

void MsgCharacterData_Stub::SetGlobalSpeakCount(const UInt16 &globalSpeakCount)
{
	bool modified = false;
	if (m.globalSpeakCount != globalSpeakCount) { m.globalSpeakCount = globalSpeakCount; modified = true; }
	MarkUpdateAll(speakCountMask, modified);
}

void MsgCharacterData_Stub::SetSpeakBlock(const UInt32 &speakBlock)
{
	bool modified = false;
	if (m.speakBlock != speakBlock) { m.speakBlock = speakBlock; modified = true; }
	MarkUpdateAll(speakBlockMask, modified);
}

void MsgCharacterData_Stub::SetOfflineTime(const UInt32 &offlineTime)
{
	bool modified = false;
	if (m.offlineTime != offlineTime) { m.offlineTime = offlineTime; modified = true; }
	MarkUpdateAll(offlineTimeMask, modified);
}

void MsgCharacterData_Stub::SetBattleInfo(const Boolean &EnablePK)
{
	bool modified = false;
	if (m.EnablePK != EnablePK) { m.EnablePK = EnablePK; modified = true; }
	MarkUpdateAll(BattleInfoMask, modified);
}

void MsgCharacterData_Stub::SetEnablePK(const Boolean &EnablePK)
{
	bool modified = false;
	if (m.EnablePK != EnablePK) { m.EnablePK = EnablePK; modified = true; }
	MarkUpdateAll(BattleInfoMask, modified);
}

void MsgCharacterData_Stub::SetSettingInfo(const Boolean &EnableBlockMakeFriend, const Boolean &EnableBlockMsg)
{
	bool modified = false;
	if (m.EnableBlockMakeFriend != EnableBlockMakeFriend) { m.EnableBlockMakeFriend = EnableBlockMakeFriend; modified = true; }
	if (m.EnableBlockMsg != EnableBlockMsg) { m.EnableBlockMsg = EnableBlockMsg; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void MsgCharacterData_Stub::SetEnableBlockMakeFriend(const Boolean &EnableBlockMakeFriend)
{
	bool modified = false;
	if (m.EnableBlockMakeFriend != EnableBlockMakeFriend) { m.EnableBlockMakeFriend = EnableBlockMakeFriend; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void MsgCharacterData_Stub::SetEnableBlockMsg(const Boolean &EnableBlockMsg)
{
	bool modified = false;
	if (m.EnableBlockMsg != EnableBlockMsg) { m.EnableBlockMsg = EnableBlockMsg; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void MsgCharacterData_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(guild_uidMask, modified);
}

void MsgCharacterData_Stub::SetInactive(const Boolean &inactive)
{
	bool modified = false;
	if (m.inactive != inactive) { m.inactive = inactive; modified = true; }
	MarkUpdateAll(inactiveMask, modified);
}

void MsgCharacterData_Stub::SetMoney(const UInt32 &money)
{
	bool modified = false;
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(moneyMask, modified);
}

void MsgCharacterData_Stub::SetGenerationAcc(const UInt32 &uiHisGenAcc)
{
	bool modified = false;
	if (m.uiHisGenAcc != uiHisGenAcc) { m.uiHisGenAcc = uiHisGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void MsgCharacterData_Stub::SetUiHisGenAcc(const UInt32 &uiHisGenAcc)
{
	bool modified = false;
	if (m.uiHisGenAcc != uiHisGenAcc) { m.uiHisGenAcc = uiHisGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}


void MsgCharacterData_Stub::Clone(MsgCharacterData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idMask) && !(obj->m.char_id == m.char_id)) {
		m.char_id = obj->m.char_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.nickName == m.nickName)) {
		m.nickName = obj->m.nickName;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.sex == m.sex)) {
		m.sex = obj->m.sex;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.cclass == m.cclass)) {
		m.cclass = obj->m.cclass;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.faction == m.faction)) {
		m.faction = obj->m.faction;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(levelMask) && !(obj->m.level == m.level)) {
		m.level = obj->m.level;
		MarkUpdateAll(levelMask, true);
	}
	if (mask.IsBit(titleMask) && !(obj->m.title == m.title)) {
		m.title = obj->m.title;
		MarkUpdateAll(titleMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.photo_id == m.photo_id)) {
		m.photo_id = obj->m.photo_id;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.hairStyle1 == m.hairStyle1)) {
		m.hairStyle1 = obj->m.hairStyle1;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.hairStyle2 == m.hairStyle2)) {
		m.hairStyle2 = obj->m.hairStyle2;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(styleMask) && !(obj->m.hairColor == m.hairColor)) {
		m.hairColor = obj->m.hairColor;
		MarkUpdateAll(styleMask, true);
	}
	if (mask.IsBit(eqWeaponMask) && !(obj->m.eq_weapon == m.eq_weapon)) {
		m.eq_weapon = obj->m.eq_weapon;
		MarkUpdateAll(eqWeaponMask, true);
	}
	if (mask.IsBit(eqWeaponMask) && !(obj->m.eq_weaponColor == m.eq_weaponColor)) {
		m.eq_weaponColor = obj->m.eq_weaponColor;
		MarkUpdateAll(eqWeaponMask, true);
	}
	if (mask.IsBit(eqHeadMask) && !(obj->m.eq_headBand == m.eq_headBand)) {
		m.eq_headBand = obj->m.eq_headBand;
		MarkUpdateAll(eqHeadMask, true);
	}
	if (mask.IsBit(eqHeadMask) && !(obj->m.eq_headColor == m.eq_headColor)) {
		m.eq_headColor = obj->m.eq_headColor;
		MarkUpdateAll(eqHeadMask, true);
	}
	if (mask.IsBit(eqClothMask) && !(obj->m.eq_cloth == m.eq_cloth)) {
		m.eq_cloth = obj->m.eq_cloth;
		MarkUpdateAll(eqClothMask, true);
	}
	if (mask.IsBit(eqClothMask) && !(obj->m.eq_clothColor == m.eq_clothColor)) {
		m.eq_clothColor = obj->m.eq_clothColor;
		MarkUpdateAll(eqClothMask, true);
	}
	if (mask.IsBit(orgHairStyleMask) && !(obj->m.org_hairStyle1 == m.org_hairStyle1)) {
		m.org_hairStyle1 = obj->m.org_hairStyle1;
		MarkUpdateAll(orgHairStyleMask, true);
	}
	if (mask.IsBit(orgHairStyleMask) && !(obj->m.org_hairColor == m.org_hairColor)) {
		m.org_hairColor = obj->m.org_hairColor;
		MarkUpdateAll(orgHairStyleMask, true);
	}
	if (mask.IsBit(orgEqClothMask) && !(obj->m.org_eq_cloth == m.org_eq_cloth)) {
		m.org_eq_cloth = obj->m.org_eq_cloth;
		MarkUpdateAll(orgEqClothMask, true);
	}
	if (mask.IsBit(orgEqClothMask) && !(obj->m.org_eq_clothColor == m.org_eq_clothColor)) {
		m.org_eq_clothColor = obj->m.org_eq_clothColor;
		MarkUpdateAll(orgEqClothMask, true);
	}
	if (mask.IsBit(friend_statusMask) && !(obj->m.friend_status == m.friend_status)) {
		m.friend_status = obj->m.friend_status;
		MarkUpdateAll(friend_statusMask, true);
	}
	if (mask.IsBit(friend_iconMask) && !(obj->m.friend_icon == m.friend_icon)) {
		m.friend_icon = obj->m.friend_icon;
		MarkUpdateAll(friend_iconMask, true);
	}
	if (mask.IsBit(friend_aboutMask) && !(obj->m.friend_about == m.friend_about)) {
		m.friend_about = obj->m.friend_about;
		MarkUpdateAll(friend_aboutMask, true);
	}
	if (mask.IsBit(friend_replyMask) && !(obj->m.friend_onlineAutoReply == m.friend_onlineAutoReply)) {
		m.friend_onlineAutoReply = obj->m.friend_onlineAutoReply;
		MarkUpdateAll(friend_replyMask, true);
	}
	if (mask.IsBit(friend_replyMask) && !(obj->m.friend_offlineAutoReply == m.friend_offlineAutoReply)) {
		m.friend_offlineAutoReply = obj->m.friend_offlineAutoReply;
		MarkUpdateAll(friend_replyMask, true);
	}
	if (mask.IsBit(lineMask) && !(obj->m.line_id == m.line_id)) {
		m.line_id = obj->m.line_id;
		MarkUpdateAll(lineMask, true);
	}
	if (mask.IsBit(speakCountMask) && !(obj->m.globalSpeakCount == m.globalSpeakCount)) {
		m.globalSpeakCount = obj->m.globalSpeakCount;
		MarkUpdateAll(speakCountMask, true);
	}
	if (mask.IsBit(speakBlockMask) && !(obj->m.speakBlock == m.speakBlock)) {
		m.speakBlock = obj->m.speakBlock;
		MarkUpdateAll(speakBlockMask, true);
	}
	if (mask.IsBit(offlineTimeMask) && !(obj->m.offlineTime == m.offlineTime)) {
		m.offlineTime = obj->m.offlineTime;
		MarkUpdateAll(offlineTimeMask, true);
	}
	if (mask.IsBit(BattleInfoMask) && !(obj->m.EnablePK == m.EnablePK)) {
		m.EnablePK = obj->m.EnablePK;
		MarkUpdateAll(BattleInfoMask, true);
	}
	if (mask.IsBit(SettingInfoMask) && !(obj->m.EnableBlockMakeFriend == m.EnableBlockMakeFriend)) {
		m.EnableBlockMakeFriend = obj->m.EnableBlockMakeFriend;
		MarkUpdateAll(SettingInfoMask, true);
	}
	if (mask.IsBit(SettingInfoMask) && !(obj->m.EnableBlockMsg == m.EnableBlockMsg)) {
		m.EnableBlockMsg = obj->m.EnableBlockMsg;
		MarkUpdateAll(SettingInfoMask, true);
	}
	if (mask.IsBit(guild_uidMask) && !(obj->m.guild_uid == m.guild_uid)) {
		m.guild_uid = obj->m.guild_uid;
		MarkUpdateAll(guild_uidMask, true);
	}
	if (mask.IsBit(inactiveMask) && !(obj->m.inactive == m.inactive)) {
		m.inactive = obj->m.inactive;
		MarkUpdateAll(inactiveMask, true);
	}
	if (mask.IsBit(moneyMask) && !(obj->m.money == m.money)) {
		m.money = obj->m.money;
		MarkUpdateAll(moneyMask, true);
	}
	if (mask.IsBit(GenerationAccMask) && !(obj->m.uiHisGenAcc == m.uiHisGenAcc)) {
		m.uiHisGenAcc = obj->m.uiHisGenAcc;
		MarkUpdateAll(GenerationAccMask, true);
	}
}

void MsgCharacterData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.char_id);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.nickName);
		buf.Read<UInt8>(m.sex);
		buf.Read<UInt8>(m.cclass);
		buf.Read<UInt8>(m.faction);
	}
	if (m_updateMask.IsBit(levelMask))
	{
		buf.Read<UInt16>(m.level);
	}
	if (m_updateMask.IsBit(titleMask))
	{
		buf.Read<String>(m.title);
	}
	if (m_updateMask.IsBit(styleMask))
	{
		buf.Read<UInt16>(m.photo_id);
		buf.Read<UInt16>(m.hairStyle1);
		buf.Read<UInt16>(m.hairStyle2);
		buf.Read<UInt16>(m.hairColor);
	}
	if (m_updateMask.IsBit(eqWeaponMask))
	{
		buf.Read<UInt32>(m.eq_weapon);
		buf.Read<UInt16>(m.eq_weaponColor);
	}
	if (m_updateMask.IsBit(eqHeadMask))
	{
		buf.Read<UInt32>(m.eq_headBand);
		buf.Read<UInt16>(m.eq_headColor);
	}
	if (m_updateMask.IsBit(eqClothMask))
	{
		buf.Read<UInt32>(m.eq_cloth);
		buf.Read<UInt16>(m.eq_clothColor);
	}
	if (m_updateMask.IsBit(orgHairStyleMask))
	{
		buf.Read<UInt16>(m.org_hairStyle1);
		buf.Read<UInt16>(m.org_hairColor);
	}
	if (m_updateMask.IsBit(orgEqClothMask))
	{
		buf.Read<UInt32>(m.org_eq_cloth);
		buf.Read<UInt16>(m.org_eq_clothColor);
	}
	if (m_updateMask.IsBit(friend_statusMask))
	{
		buf.Read<UInt8>(m.friend_status);
	}
	if (m_updateMask.IsBit(friend_iconMask))
	{
		buf.Read<UInt16>(m.friend_icon);
	}
	if (m_updateMask.IsBit(friend_aboutMask))
	{
		buf.Read<String>(m.friend_about);
	}
	if (m_updateMask.IsBit(friend_replyMask))
	{
		buf.Read<String>(m.friend_onlineAutoReply);
		buf.Read<String>(m.friend_offlineAutoReply);
	}
	if (m_updateMask.IsBit(lineMask))
	{
		buf.Read<UInt8>(m.line_id);
	}
	if (m_updateMask.IsBit(speakCountMask))
	{
		buf.Read<UInt16>(m.globalSpeakCount);
	}
	if (m_updateMask.IsBit(speakBlockMask))
	{
		buf.Read<UInt32>(m.speakBlock);
	}
	if (m_updateMask.IsBit(offlineTimeMask))
	{
		buf.Read<UInt32>(m.offlineTime);
	}
	if (m_updateMask.IsBit(BattleInfoMask))
	{
		buf.Read<Boolean>(m.EnablePK);
	}
	if (m_updateMask.IsBit(SettingInfoMask))
	{
		buf.Read<Boolean>(m.EnableBlockMakeFriend);
		buf.Read<Boolean>(m.EnableBlockMsg);
	}
	if (m_updateMask.IsBit(guild_uidMask))
	{
		buf.Read<UInt32>(m.guild_uid);
	}
	if (m_updateMask.IsBit(inactiveMask))
	{
		buf.Read<Boolean>(m.inactive);
	}
	if (m_updateMask.IsBit(moneyMask))
	{
		buf.Read<UInt32>(m.money);
	}
	if (m_updateMask.IsBit(GenerationAccMask))
	{
		buf.Read<UInt32>(m.uiHisGenAcc);
	}
}

void MsgCharacterData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.char_id);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<String>(m.nickName);
		buf.Write<UInt8>(m.sex);
		buf.Write<UInt8>(m.cclass);
		buf.Write<UInt8>(m.faction);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.level);
	}
	if (mask.IsBit(titleMask))
	{
		buf.Write<String>(m.title);
	}
	if (mask.IsBit(styleMask))
	{
		buf.Write<UInt16>(m.photo_id);
		buf.Write<UInt16>(m.hairStyle1);
		buf.Write<UInt16>(m.hairStyle2);
		buf.Write<UInt16>(m.hairColor);
	}
	if (mask.IsBit(eqWeaponMask))
	{
		buf.Write<UInt32>(m.eq_weapon);
		buf.Write<UInt16>(m.eq_weaponColor);
	}
	if (mask.IsBit(eqHeadMask))
	{
		buf.Write<UInt32>(m.eq_headBand);
		buf.Write<UInt16>(m.eq_headColor);
	}
	if (mask.IsBit(eqClothMask))
	{
		buf.Write<UInt32>(m.eq_cloth);
		buf.Write<UInt16>(m.eq_clothColor);
	}
	if (mask.IsBit(orgHairStyleMask))
	{
		buf.Write<UInt16>(m.org_hairStyle1);
		buf.Write<UInt16>(m.org_hairColor);
	}
	if (mask.IsBit(orgEqClothMask))
	{
		buf.Write<UInt32>(m.org_eq_cloth);
		buf.Write<UInt16>(m.org_eq_clothColor);
	}
	if (mask.IsBit(friend_statusMask))
	{
		buf.Write<UInt8>(m.friend_status);
	}
	if (mask.IsBit(friend_iconMask))
	{
		buf.Write<UInt16>(m.friend_icon);
	}
	if (mask.IsBit(friend_aboutMask))
	{
		buf.Write<String>(m.friend_about);
	}
	if (mask.IsBit(friend_replyMask))
	{
		buf.Write<String>(m.friend_onlineAutoReply);
		buf.Write<String>(m.friend_offlineAutoReply);
	}
	if (mask.IsBit(lineMask))
	{
		buf.Write<UInt8>(m.line_id);
	}
	if (mask.IsBit(speakCountMask))
	{
		buf.Write<UInt16>(m.globalSpeakCount);
	}
	if (mask.IsBit(speakBlockMask))
	{
		buf.Write<UInt32>(m.speakBlock);
	}
	if (mask.IsBit(offlineTimeMask))
	{
		buf.Write<UInt32>(m.offlineTime);
	}
	if (mask.IsBit(BattleInfoMask))
	{
		buf.Write<Boolean>(m.EnablePK);
	}
	if (mask.IsBit(SettingInfoMask))
	{
		buf.Write<Boolean>(m.EnableBlockMakeFriend);
		buf.Write<Boolean>(m.EnableBlockMsg);
	}
	if (mask.IsBit(guild_uidMask))
	{
		buf.Write<UInt32>(m.guild_uid);
	}
	if (mask.IsBit(inactiveMask))
	{
		buf.Write<Boolean>(m.inactive);
	}
	if (mask.IsBit(moneyMask))
	{
		buf.Write<UInt32>(m.money);
	}
	if (mask.IsBit(GenerationAccMask))
	{
		buf.Write<UInt32>(m.uiHisGenAcc);
	}
}

RPCResult MsgCharacterData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MsgCharacterData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult MsgCharacterData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MsgCharacterData_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

