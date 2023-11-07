//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/CharacterInfo_stub.h"

void CharacterInfo_Stub::SetId(const UInt32 &acct_id, const UInt32 &char_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterInfo_Stub::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterInfo_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void CharacterInfo_Stub::SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &level, const UInt32 &exp, const UInt8 &faction, const UInt32 &money)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	if (m.sex != sex) { m.sex = sex; modified = true; }
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.level != level) { m.level = level; modified = true; }
	if (m.exp != exp) { m.exp = exp; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetNickName(const String &nickName)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetExp(const UInt32 &exp)
{
	bool modified = false;
	if (m.exp != exp) { m.exp = exp; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetMoney(const UInt32 &money)
{
	bool modified = false;
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void CharacterInfo_Stub::SetMap(const UInt16 &map_id, const UInt8 &line_id, const UInt32 &map_ownerId, const UInt16 &map_x, const UInt16 &map_y)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterInfo_Stub::SetMap_id(const UInt16 &map_id)
{
	bool modified = false;
	if (m.map_id != map_id) { m.map_id = map_id; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterInfo_Stub::SetLine_id(const UInt8 &line_id)
{
	bool modified = false;
	if (m.line_id != line_id) { m.line_id = line_id; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterInfo_Stub::SetMap_ownerId(const UInt32 &map_ownerId)
{
	bool modified = false;
	if (m.map_ownerId != map_ownerId) { m.map_ownerId = map_ownerId; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterInfo_Stub::SetMap_x(const UInt16 &map_x)
{
	bool modified = false;
	if (m.map_x != map_x) { m.map_x = map_x; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterInfo_Stub::SetMap_y(const UInt16 &map_y)
{
	bool modified = false;
	if (m.map_y != map_y) { m.map_y = map_y; modified = true; }
	MarkUpdateAll(mapMask, modified);
}

void CharacterInfo_Stub::SetReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterInfo_Stub::SetReborn_map(const UInt16 &reborn_map)
{
	bool modified = false;
	if (m.reborn_map != reborn_map) { m.reborn_map = reborn_map; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterInfo_Stub::SetReborn_x(const UInt16 &reborn_x)
{
	bool modified = false;
	if (m.reborn_x != reborn_x) { m.reborn_x = reborn_x; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterInfo_Stub::SetReborn_y(const UInt16 &reborn_y)
{
	bool modified = false;
	if (m.reborn_y != reborn_y) { m.reborn_y = reborn_y; modified = true; }
	MarkUpdateAll(rebornMask, modified);
}

void CharacterInfo_Stub::SetStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterInfo_Stub::SetPhoto_id(const UInt16 &photo_id)
{
	bool modified = false;
	if (m.photo_id != photo_id) { m.photo_id = photo_id; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterInfo_Stub::SetHairStyle1(const UInt16 &hairStyle1)
{
	bool modified = false;
	if (m.hairStyle1 != hairStyle1) { m.hairStyle1 = hairStyle1; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterInfo_Stub::SetHairStyle2(const UInt16 &hairStyle2)
{
	bool modified = false;
	if (m.hairStyle2 != hairStyle2) { m.hairStyle2 = hairStyle2; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterInfo_Stub::SetHairColor(const UInt16 &hairColor)
{
	bool modified = false;
	if (m.hairColor != hairColor) { m.hairColor = hairColor; modified = true; }
	MarkUpdateAll(styleMask, modified);
}

void CharacterInfo_Stub::SetEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterInfo_Stub::SetEq_weapon(const UInt32 &eq_weapon)
{
	bool modified = false;
	if (m.eq_weapon != eq_weapon) { m.eq_weapon = eq_weapon; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterInfo_Stub::SetEq_weaponColor(const UInt16 &eq_weaponColor)
{
	bool modified = false;
	if (m.eq_weaponColor != eq_weaponColor) { m.eq_weaponColor = eq_weaponColor; modified = true; }
	MarkUpdateAll(eqWeaponMask, modified);
}

void CharacterInfo_Stub::SetEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterInfo_Stub::SetEq_headBand(const UInt32 &eq_headBand)
{
	bool modified = false;
	if (m.eq_headBand != eq_headBand) { m.eq_headBand = eq_headBand; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterInfo_Stub::SetEq_headColor(const UInt16 &eq_headColor)
{
	bool modified = false;
	if (m.eq_headColor != eq_headColor) { m.eq_headColor = eq_headColor; modified = true; }
	MarkUpdateAll(eqHeadMask, modified);
}

void CharacterInfo_Stub::SetEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterInfo_Stub::SetEq_cloth(const UInt32 &eq_cloth)
{
	bool modified = false;
	if (m.eq_cloth != eq_cloth) { m.eq_cloth = eq_cloth; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterInfo_Stub::SetEq_clothColor(const UInt16 &eq_clothColor)
{
	bool modified = false;
	if (m.eq_clothColor != eq_clothColor) { m.eq_clothColor = eq_clothColor; modified = true; }
	MarkUpdateAll(eqClothMask, modified);
}

void CharacterInfo_Stub::SetStatus(const UInt8 &status)
{
	bool modified = false;
	if (m.status != status) { m.status = status; modified = true; }
	MarkUpdateAll(statusMask, modified);
}

void CharacterInfo_Stub::SetOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterInfo_Stub::SetOrg_hairStyle1(const UInt16 &org_hairStyle1)
{
	bool modified = false;
	if (m.org_hairStyle1 != org_hairStyle1) { m.org_hairStyle1 = org_hairStyle1; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterInfo_Stub::SetOrg_hairColor(const UInt16 &org_hairColor)
{
	bool modified = false;
	if (m.org_hairColor != org_hairColor) { m.org_hairColor = org_hairColor; modified = true; }
	MarkUpdateAll(orgHairStyleMask, modified);
}

void CharacterInfo_Stub::SetOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterInfo_Stub::SetOrg_eq_cloth(const UInt32 &org_eq_cloth)
{
	bool modified = false;
	if (m.org_eq_cloth != org_eq_cloth) { m.org_eq_cloth = org_eq_cloth; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterInfo_Stub::SetOrg_eq_clothColor(const UInt16 &org_eq_clothColor)
{
	bool modified = false;
	if (m.org_eq_clothColor != org_eq_clothColor) { m.org_eq_clothColor = org_eq_clothColor; modified = true; }
	MarkUpdateAll(orgEqClothMask, modified);
}

void CharacterInfo_Stub::SetDeleteDate(const UInt32 &delete_date)
{
	bool modified = false;
	if (m.delete_date != delete_date) { m.delete_date = delete_date; modified = true; }
	MarkUpdateAll(deleteDateMask, modified);
}

void CharacterInfo_Stub::SetDelete_date(const UInt32 &delete_date)
{
	bool modified = false;
	if (m.delete_date != delete_date) { m.delete_date = delete_date; modified = true; }
	MarkUpdateAll(deleteDateMask, modified);
}



void CharacterInfo_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void CharacterInfo_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.acct_id);
		buf.Write<UInt32>(m.char_id);
	}
	if (mask.IsBit(infoMask))
	{
		buf.Write<String>(m.nickName);
		buf.Write<UInt8>(m.sex);
		buf.Write<UInt8>(m.cclass);
		buf.Write<UInt16>(m.level);
		buf.Write<UInt32>(m.exp);
		buf.Write<UInt8>(m.faction);
		buf.Write<UInt32>(m.money);
	}
	if (mask.IsBit(mapMask))
	{
		buf.Write<UInt16>(m.map_id);
		buf.Write<UInt8>(m.line_id);
		buf.Write<UInt32>(m.map_ownerId);
		buf.Write<UInt16>(m.map_x);
		buf.Write<UInt16>(m.map_y);
	}
	if (mask.IsBit(rebornMask))
	{
		buf.Write<UInt16>(m.reborn_map);
		buf.Write<UInt16>(m.reborn_x);
		buf.Write<UInt16>(m.reborn_y);
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
	if (mask.IsBit(statusMask))
	{
		buf.Write<UInt8>(m.status);
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
	if (mask.IsBit(deleteDateMask))
	{
		buf.Write<UInt32>(m.delete_date);
	}
}

RPCResult CharacterInfo_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_CharacterInfo_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult CharacterInfo_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_CharacterInfo_REQ_STATE:
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

