//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/MessageCharacter_proxy.h"


void MessageCharacter_Proxy::ReadAttr(CONST Buffer &buf)
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
		buf.Read<UInt8>(m.sexClass);
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
	if (m_updateMask.IsBit(BattleInfoMask))
	{
		buf.Read<Boolean>(m.EnablePK);
	}
	if (m_updateMask.IsBit(SettingInfoMask))
	{
		buf.Read<Boolean>(m.EnableBlockMakeFriend);
		buf.Read<Boolean>(m.EnableBlockMsg);
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
	if (m_updateMask.IsBit(guildMask))
	{
		buf.Read<UInt32>(m.guild_uid);
		buf.Read<String>(m.guild_name);
	}
	if (m_updateMask.IsBit(moneyMask))
	{
		buf.Read<UInt32>(m.money);
	}
	if (m_updateMask.IsBit(inactiveMask))
	{
		buf.Read<Boolean>(m.inactive);
	}
	if (m_updateMask.IsBit(GenerationAccMask))
	{
		buf.Read<UInt32>(m.uiHisGenAcc);
	}
}

void MessageCharacter_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void MessageCharacter_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.char_id);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.nickName, m.sex, m.cclass, m.sexClass, m.faction);
	if (IsUpdated(levelMask)) OnUpdateLevel(m.level);
	if (IsUpdated(titleMask)) OnUpdateTitle(m.title);
	if (IsUpdated(styleMask)) OnUpdateStyle(m.photo_id, m.hairStyle1, m.hairStyle2, m.hairColor);
	if (IsUpdated(eqWeaponMask)) OnUpdateEqWeapon(m.eq_weapon, m.eq_weaponColor);
	if (IsUpdated(eqHeadMask)) OnUpdateEqHead(m.eq_headBand, m.eq_headColor);
	if (IsUpdated(eqClothMask)) OnUpdateEqCloth(m.eq_cloth, m.eq_clothColor);
	if (IsUpdated(BattleInfoMask)) OnUpdateBattleInfo(m.EnablePK);
	if (IsUpdated(SettingInfoMask)) OnUpdateSettingInfo(m.EnableBlockMakeFriend, m.EnableBlockMsg);
	if (IsUpdated(orgHairStyleMask)) OnUpdateOrgHairStyle(m.org_hairStyle1, m.org_hairColor);
	if (IsUpdated(orgEqClothMask)) OnUpdateOrgEqCloth(m.org_eq_cloth, m.org_eq_clothColor);
	if (IsUpdated(friend_statusMask)) OnUpdateFriend_status(m.friend_status);
	if (IsUpdated(friend_iconMask)) OnUpdateFriend_icon(m.friend_icon);
	if (IsUpdated(friend_aboutMask)) OnUpdateFriend_about(m.friend_about);
	if (IsUpdated(friend_replyMask)) OnUpdateFriend_reply(m.friend_onlineAutoReply, m.friend_offlineAutoReply);
	if (IsUpdated(lineMask)) OnUpdateLine(m.line_id);
	if (IsUpdated(speakCountMask)) OnUpdateSpeakCount(m.globalSpeakCount);
	if (IsUpdated(speakBlockMask)) OnUpdateSpeakBlock(m.speakBlock);
	if (IsUpdated(offlineTimeMask)) OnUpdateOfflineTime(m.offlineTime);
	if (IsUpdated(guildMask)) OnUpdateGuild(m.guild_uid, m.guild_name);
	if (IsUpdated(moneyMask)) OnUpdateMoney(m.money);
	if (IsUpdated(inactiveMask)) OnUpdateInactive(m.inactive);
	if (IsUpdated(GenerationAccMask)) OnUpdateGenerationAcc(m.uiHisGenAcc);
}

#pragma optimize("", off)
RPCResult MessageCharacter_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_MessageCharacter_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult MessageCharacter_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_MessageCharacter_UP_STATE:
			_result = RPCDecode<444148693>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
