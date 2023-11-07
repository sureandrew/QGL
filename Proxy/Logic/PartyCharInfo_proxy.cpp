//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/PartyCharInfo_proxy.h"


void PartyCharInfo_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(PartyIndexMask))
	{
		buf.Read<UInt16>(m.u16Index);
	}
	if (m_updateMask.IsBit(CharBaseInfoMask))
	{
		buf.Read<UInt32>(m.u32CharId);
		buf.Read<RPCNetID>(m.stNetId);
		buf.Read<UInt16>(m.u16MapId);
		buf.Read<UInt16>(m.u16XPos);
		buf.Read<UInt16>(m.u16YPos);
		buf.Read<UInt16>(m.u16Level);
		buf.Read<UInt8>(m.u8SexClass);
		buf.Read<UInt8>(m.u8Faction);
		buf.Read<UInt16>(m.u16Hp);
		buf.Read<UInt16>(m.u16MHp);
		buf.Read<UInt16>(m.u16MaxHp);
		buf.Read<UInt16>(m.u16Sp);
		buf.Read<UInt16>(m.u16MSp);
		buf.Read<UInt16>(m.u16MaxSp);
		buf.Read<String>(m.strNickName);
	}
	if (m_updateMask.IsBit(CharOrgEqMask))
	{
		buf.Read<UInt32>(m.u32OrgCloth);
		buf.Read<UInt16>(m.u16OrgClothColor);
		buf.Read<UInt16>(m.u16OrgHairStyle);
		buf.Read<UInt16>(m.u16OrgHairColor);
		buf.Read<UInt16>(m.u16HairStyle1);
		buf.Read<UInt16>(m.u16HairColor);
	}
	if (m_updateMask.IsBit(CharEqMask))
	{
		buf.Read<UInt32>(m.u32Cloth);
		buf.Read<UInt16>(m.u16ClothColor);
		buf.Read<UInt16>(m.u16HeadBand);
		buf.Read<UInt32>(m.u32Weapon);
	}
}

void PartyCharInfo_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void PartyCharInfo_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(PartyIndexMask)) OnUpdatePartyIndex(m.u16Index);
	if (IsUpdated(CharBaseInfoMask)) OnUpdateCharBaseInfo(m.u32CharId, m.stNetId, m.u16MapId, m.u16XPos, m.u16YPos, m.u16Level, m.u8SexClass, m.u8Faction, m.u16Hp, m.u16MHp, m.u16MaxHp, m.u16Sp, m.u16MSp, m.u16MaxSp, m.strNickName);
	if (IsUpdated(CharOrgEqMask)) OnUpdateCharOrgEq(m.u32OrgCloth, m.u16OrgClothColor, m.u16OrgHairStyle, m.u16OrgHairColor, m.u16HairStyle1, m.u16HairColor);
	if (IsUpdated(CharEqMask)) OnUpdateCharEq(m.u32Cloth, m.u16ClothColor, m.u16HeadBand, m.u32Weapon);
}

#pragma optimize("", off)
RPCResult PartyCharInfo_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PartyCharInfo_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult PartyCharInfo_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_PartyCharInfo_UP_STATE:
			_result = RPCDecode<602479523>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
