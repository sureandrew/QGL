//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/MessageCharacter_stub.h"

void MessageCharacter_Stub::SetId(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void MessageCharacter_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void MessageCharacter_Stub::SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	if (m.sex != sex) { m.sex = sex; modified = true; }
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MessageCharacter_Stub::SetNickName(const String &nickName)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MessageCharacter_Stub::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MessageCharacter_Stub::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MessageCharacter_Stub::SetSexClass(const UInt8 &sexClass)
{
	bool modified = false;
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MessageCharacter_Stub::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void MessageCharacter_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void MessageCharacter_Stub::SetTitle(const String &title)
{
	bool modified = false;
	if (m.title != title) { m.title = title; modified = true; }
	MarkUpdateAll(titleMask, modified);
}

void MessageCharacter_Stub::SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MessageCharacter_Stub::SetPhoto_id(const UInt16 &photo_id)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MessageCharacter_Stub::SetHairStyle1(const UInt16 &hairStyle1)
{
	bool modified = false;
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MessageCharacter_Stub::SetHairStyle2(const UInt16 &hairStyle2)
{
	bool modified = false;
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MessageCharacter_Stub::SetHairColor(const UInt16 &hairColor)
{
	bool modified = false;
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void MessageCharacter_Stub::SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void MessageCharacter_Stub::SetEq_weapon(const UInt32 &eq_weapon)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void MessageCharacter_Stub::SetEq_weaponColor(const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void MessageCharacter_Stub::SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void MessageCharacter_Stub::SetEq_headBand(const UInt32 &eq_headBand)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void MessageCharacter_Stub::SetEq_headColor(const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void MessageCharacter_Stub::SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void MessageCharacter_Stub::SetEq_cloth(const UInt32 &eq_cloth)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void MessageCharacter_Stub::SetEq_clothColor(const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void MessageCharacter_Stub::SetBattleInfo(const Boolean &EnablePK)
{
	bool modified = false;
	if (m.EnablePK != EnablePK) { m.EnablePK = EnablePK; modified = true; }
	MarkUpdateAll(BattleInfoMask, modified);
}

void MessageCharacter_Stub::SetEnablePK(const Boolean &EnablePK)
{
	bool modified = false;
	if (m.EnablePK != EnablePK) { m.EnablePK = EnablePK; modified = true; }
	MarkUpdateAll(BattleInfoMask, modified);
}

void MessageCharacter_Stub::SetSettingInfo(const Boolean &EnableBlockMakeFriend, const Boolean &EnableBlockMsg)
{
	bool modified = false;
	if (m.EnableBlockMakeFriend != EnableBlockMakeFriend) { m.EnableBlockMakeFriend = EnableBlockMakeFriend; modified = true; }
	if (m.EnableBlockMsg != EnableBlockMsg) { m.EnableBlockMsg = EnableBlockMsg; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void MessageCharacter_Stub::SetEnableBlockMakeFriend(const Boolean &EnableBlockMakeFriend)
{
	bool modified = false;
	if (m.EnableBlockMakeFriend != EnableBlockMakeFriend) { m.EnableBlockMakeFriend = EnableBlockMakeFriend; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void MessageCharacter_Stub::SetEnableBlockMsg(const Boolean &EnableBlockMsg)
{
	bool modified = false;
	if (m.EnableBlockMsg != EnableBlockMsg) { m.EnableBlockMsg = EnableBlockMsg; modified = true; }
	MarkUpdateAll(SettingInfoMask, modified);
}

void MessageCharacter_Stub::SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void MessageCharacter_Stub::SetOrg_hairStyle1(const UInt16 &org_hairStyle1)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void MessageCharacter_Stub::SetOrg_hairColor(const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void MessageCharacter_Stub::SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void MessageCharacter_Stub::SetOrg_eq_cloth(const UInt32 &org_eq_cloth)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void MessageCharacter_Stub::SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void MessageCharacter_Stub::SetFriend_status(const UInt8 &friend_status)
{
	bool modified = false;
	if (m.friend_status != friend_status) { m.friend_status = friend_status; modified = true; }
	MarkUpdateAll(friend_statusMask, modified);
}

void MessageCharacter_Stub::SetFriend_icon(const UInt16 &friend_icon)
{
	bool modified = false;
	if (m.friend_icon != friend_icon) { m.friend_icon = friend_icon; modified = true; }
	MarkUpdateAll(friend_iconMask, modified);
}

void MessageCharacter_Stub::SetFriend_about(const String &friend_about)
{
	bool modified = false;
	if (m.friend_about != friend_about) { m.friend_about = friend_about; modified = true; }
	MarkUpdateAll(friend_aboutMask, modified);
}

void MessageCharacter_Stub::SetFriend_reply(const String &friend_onlineAutoReply, const String &friend_offlineAutoReply)
{
	bool modified = false;
	if (m.friend_onlineAutoReply != friend_onlineAutoReply) { m.friend_onlineAutoReply = friend_onlineAutoReply; modified = true; }
	if (m.friend_offlineAutoReply != friend_offlineAutoReply) { m.friend_offlineAutoReply = friend_offlineAutoReply; modified = true; }
	MarkUpdateAll(friend_replyMask, modified);
}

void MessageCharacter_Stub::SetFriend_onlineAutoReply(const String &friend_onlineAutoReply)
{
	bool modified = false;
	if (m.friend_onlineAutoReply != friend_onlineAutoReply) { m.friend_onlineAutoReply = friend_onlineAutoReply; modified = true; }
	MarkUpdateAll(friend_replyMask, modified);
}

void MessageCharacter_Stub::SetFriend_offlineAutoReply(const String &friend_offlineAutoReply)
{
	bool modified = false;
	if (m.friend_offlineAutoReply != friend_offlineAutoReply) { m.friend_offlineAutoReply = friend_offlineAutoReply; modified = true; }
	MarkUpdateAll(friend_replyMask, modified);
}

void MessageCharacter_Stub::SetLine(const UInt8 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(lineMask, modified);
}

void MessageCharacter_Stub::SetLine_id(const UInt8 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(lineMask, modified);
}

void MessageCharacter_Stub::SetSpeakCount(const UInt16 &globalSpeakCount)
{
	bool modified = false;
	if (m.globalSpeakCount != globalSpeakCount) { m.globalSpeakCount = globalSpeakCount; modified = true; }
	MarkUpdateAll(speakCountMask, modified);
}

void MessageCharacter_Stub::SetGlobalSpeakCount(const UInt16 &globalSpeakCount)
{
	bool modified = false;
	if (m.globalSpeakCount != globalSpeakCount) { m.globalSpeakCount = globalSpeakCount; modified = true; }
	MarkUpdateAll(speakCountMask, modified);
}

void MessageCharacter_Stub::SetSpeakBlock(const UInt32 &speakBlock)
{
	bool modified = false;
	if (m.speakBlock != speakBlock) { m.speakBlock = speakBlock; modified = true; }
	MarkUpdateAll(speakBlockMask, modified);
}

void MessageCharacter_Stub::SetOfflineTime(const UInt32 &offlineTime)
{
	bool modified = false;
	if (m.offlineTime != offlineTime) { m.offlineTime = offlineTime; modified = true; }
	MarkUpdateAll(offlineTimeMask, modified);
}

void MessageCharacter_Stub::SetGuild(const UInt32 &guild_uid, const String &guild_name)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	if (m.guild_name != guild_name) { m.guild_name = guild_name; modified = true; }
	MarkUpdateAll(guildMask, modified);
}

void MessageCharacter_Stub::SetGuild_uid(const UInt32 &guild_uid)
{
	bool modified = false;
	if (m.guild_uid != guild_uid) { m.guild_uid = guild_uid; modified = true; }
	MarkUpdateAll(guildMask, modified);
}

void MessageCharacter_Stub::SetGuild_name(const String &guild_name)
{
	bool modified = false;
	if (m.guild_name != guild_name) { m.guild_name = guild_name; modified = true; }
	MarkUpdateAll(guildMask, modified);
}

void MessageCharacter_Stub::SetMoney(const UInt32 &money)
{
	bool modified = false;
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(moneyMask, modified);
}

void MessageCharacter_Stub::SetInactive(const Boolean &inactive)
{
	bool modified = false;
	if (m.inactive != inactive) { m.inactive = inactive; modified = true; }
	MarkUpdateAll(inactiveMask, modified);
}

void MessageCharacter_Stub::SetGenerationAcc(const UInt32 &uiHisGenAcc)
{
	bool modified = false;
	if (m.uiHisGenAcc != uiHisGenAcc) { m.uiHisGenAcc = uiHisGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}

void MessageCharacter_Stub::SetUiHisGenAcc(const UInt32 &uiHisGenAcc)
{
	bool modified = false;
	if (m.uiHisGenAcc != uiHisGenAcc) { m.uiHisGenAcc = uiHisGenAcc; modified = true; }
	MarkUpdateAll(GenerationAccMask, modified);
}



void MessageCharacter_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void MessageCharacter_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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
		buf.Write<UInt8>(m.sexClass);
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
	if (mask.IsBit(BattleInfoMask))
	{
		buf.Write<Boolean>(m.EnablePK);
	}
	if (mask.IsBit(SettingInfoMask))
	{
		buf.Write<Boolean>(m.EnableBlockMakeFriend);
		buf.Write<Boolean>(m.EnableBlockMsg);
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
	if (mask.IsBit(guildMask))
	{
		buf.Write<UInt32>(m.guild_uid);
		buf.Write<String>(m.guild_name);
	}
	if (mask.IsBit(moneyMask))
	{
		buf.Write<UInt32>(m.money);
	}
	if (mask.IsBit(inactiveMask))
	{
		buf.Write<Boolean>(m.inactive);
	}
	if (mask.IsBit(GenerationAccMask))
	{
		buf.Write<UInt32>(m.uiHisGenAcc);
	}
}

RPCResult MessageCharacter_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_MessageCharacter_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult MessageCharacter_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MessageCharacter_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_MessageCharacter_SelfSettingChange:
		{
			String about;
			pBuf->Read<String>(about);
			UInt8 OnlineState;
			pBuf->Read<UInt8>(OnlineState);
			String Reply;
			pBuf->Read<String>(Reply);
			_result = SelfSettingChange(pPeer, context, about, OnlineState, Reply);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

