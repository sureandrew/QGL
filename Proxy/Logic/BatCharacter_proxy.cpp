//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/BatCharacter_proxy.h"


void BatCharacter_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt32>(m.acct_id);
		buf.Read<UInt32>(m.char_id);
		buf.Read<String>(m.nickName);
		buf.Read<UInt8>(m.sex);
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
	if (m_updateMask.IsBit(PKInfoMask))
	{
		buf.Read<UInt16>(m.PK_Value);
	}
	if (m_updateMask.IsBit(maxPartnerMask))
	{
		buf.Read<UInt8>(m.maxPartner);
	}
	if (m_updateMask.IsBit(TPMask))
	{
		buf.Read<UInt16>(m.TP);
		buf.Read<UInt32>(m.Model_mobid);
	}
}

void BatCharacter_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void BatCharacter_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.acct_id, m.char_id, m.nickName, m.sex);
	if (IsUpdated(styleMask)) OnUpdateStyle(m.photo_id, m.hairStyle1, m.hairStyle2, m.hairColor);
	if (IsUpdated(eqWeaponMask)) OnUpdateEqWeapon(m.eq_weapon, m.eq_weaponColor);
	if (IsUpdated(eqHeadMask)) OnUpdateEqHead(m.eq_headBand, m.eq_headColor);
	if (IsUpdated(eqClothMask)) OnUpdateEqCloth(m.eq_cloth, m.eq_clothColor);
	if (IsUpdated(PKInfoMask)) OnUpdatePKInfo(m.PK_Value);
	if (IsUpdated(maxPartnerMask)) OnUpdateMaxPartner(m.maxPartner);
	if (IsUpdated(TPMask)) OnUpdateTP(m.TP, m.Model_mobid);
}

#pragma optimize("", off)
RPCResult BatCharacter_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_BatCharacter_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult BatCharacter_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_BatCharacter_UP_STATE:
			_result = RPCDecode<1983910738>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
