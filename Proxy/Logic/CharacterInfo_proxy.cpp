//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/CharacterInfo_proxy.h"


void CharacterInfo_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.acct_id);
		buf.Read<UInt32>(m.char_id);
	}
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<String>(m.nickName);
		buf.Read<UInt8>(m.sex);
		buf.Read<UInt8>(m.cclass);
		buf.Read<UInt16>(m.level);
		buf.Read<UInt32>(m.exp);
		buf.Read<UInt8>(m.faction);
		buf.Read<UInt32>(m.money);
	}
	if (m_updateMask.IsBit(mapMask))
	{
		buf.Read<UInt16>(m.map_id);
		buf.Read<UInt8>(m.line_id);
		buf.Read<UInt32>(m.map_ownerId);
		buf.Read<UInt16>(m.map_x);
		buf.Read<UInt16>(m.map_y);
	}
	if (m_updateMask.IsBit(rebornMask))
	{
		buf.Read<UInt16>(m.reborn_map);
		buf.Read<UInt16>(m.reborn_x);
		buf.Read<UInt16>(m.reborn_y);
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
	if (m_updateMask.IsBit(statusMask))
	{
		buf.Read<UInt8>(m.status);
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
	if (m_updateMask.IsBit(deleteDateMask))
	{
		buf.Read<UInt32>(m.delete_date);
	}
}

void CharacterInfo_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void CharacterInfo_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(idMask)) OnUpdateId(m.acct_id, m.char_id);
	if (IsUpdated(infoMask)) OnUpdateInfo(m.nickName, m.sex, m.cclass, m.level, m.exp, m.faction, m.money);
	if (IsUpdated(mapMask)) OnUpdateMap(m.map_id, m.line_id, m.map_ownerId, m.map_x, m.map_y);
	if (IsUpdated(rebornMask)) OnUpdateReborn(m.reborn_map, m.reborn_x, m.reborn_y);
	if (IsUpdated(styleMask)) OnUpdateStyle(m.photo_id, m.hairStyle1, m.hairStyle2, m.hairColor);
	if (IsUpdated(eqWeaponMask)) OnUpdateEqWeapon(m.eq_weapon, m.eq_weaponColor);
	if (IsUpdated(eqHeadMask)) OnUpdateEqHead(m.eq_headBand, m.eq_headColor);
	if (IsUpdated(eqClothMask)) OnUpdateEqCloth(m.eq_cloth, m.eq_clothColor);
	if (IsUpdated(statusMask)) OnUpdateStatus(m.status);
	if (IsUpdated(orgHairStyleMask)) OnUpdateOrgHairStyle(m.org_hairStyle1, m.org_hairColor);
	if (IsUpdated(orgEqClothMask)) OnUpdateOrgEqCloth(m.org_eq_cloth, m.org_eq_clothColor);
	if (IsUpdated(deleteDateMask)) OnUpdateDeleteDate(m.delete_date);
}

#pragma optimize("", off)
RPCResult CharacterInfo_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_CharacterInfo_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult CharacterInfo_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_CharacterInfo_UP_STATE:
			_result = RPCDecode<1730051561>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
