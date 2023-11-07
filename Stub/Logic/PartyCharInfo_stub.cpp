//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/PartyCharInfo_stub.h"

void PartyCharInfo_Stub::SetPartyIndex(const UInt16 &u16Index)
{
	bool modified = false;
	if (m.u16Index != u16Index) { m.u16Index = u16Index; modified = true; }
	MarkUpdateAll(PartyIndexMask, modified);
}

void PartyCharInfo_Stub::SetU16Index(const UInt16 &u16Index)
{
	bool modified = false;
	if (m.u16Index != u16Index) { m.u16Index = u16Index; modified = true; }
	MarkUpdateAll(PartyIndexMask, modified);
}

void PartyCharInfo_Stub::SetCharBaseInfo(const UInt32 &u32CharId, const RPCNetID &stNetId, const UInt16 &u16MapId, const UInt16 &u16XPos, const UInt16 &u16YPos, const UInt16 &u16Level, const UInt8 &u8SexClass, const UInt8 &u8Faction, const UInt16 &u16Hp, const UInt16 &u16MHp, const UInt16 &u16MaxHp, const UInt16 &u16Sp, const UInt16 &u16MSp, const UInt16 &u16MaxSp, const String &strNickName)
{
	bool modified = false;
	if (m.u32CharId != u32CharId) { m.u32CharId = u32CharId; modified = true; }
	if (m.stNetId != stNetId) { m.stNetId = stNetId; modified = true; }
	if (m.u16MapId != u16MapId) { m.u16MapId = u16MapId; modified = true; }
	if (m.u16XPos != u16XPos) { m.u16XPos = u16XPos; modified = true; }
	if (m.u16YPos != u16YPos) { m.u16YPos = u16YPos; modified = true; }
	if (m.u16Level != u16Level) { m.u16Level = u16Level; modified = true; }
	if (m.u8SexClass != u8SexClass) { m.u8SexClass = u8SexClass; modified = true; }
	if (m.u8Faction != u8Faction) { m.u8Faction = u8Faction; modified = true; }
	if (m.u16Hp != u16Hp) { m.u16Hp = u16Hp; modified = true; }
	if (m.u16MHp != u16MHp) { m.u16MHp = u16MHp; modified = true; }
	if (m.u16MaxHp != u16MaxHp) { m.u16MaxHp = u16MaxHp; modified = true; }
	if (m.u16Sp != u16Sp) { m.u16Sp = u16Sp; modified = true; }
	if (m.u16MSp != u16MSp) { m.u16MSp = u16MSp; modified = true; }
	if (m.u16MaxSp != u16MaxSp) { m.u16MaxSp = u16MaxSp; modified = true; }
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU32CharId(const UInt32 &u32CharId)
{
	bool modified = false;
	if (m.u32CharId != u32CharId) { m.u32CharId = u32CharId; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetStNetId(const RPCNetID &stNetId)
{
	bool modified = false;
	if (m.stNetId != stNetId) { m.stNetId = stNetId; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16MapId(const UInt16 &u16MapId)
{
	bool modified = false;
	if (m.u16MapId != u16MapId) { m.u16MapId = u16MapId; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16XPos(const UInt16 &u16XPos)
{
	bool modified = false;
	if (m.u16XPos != u16XPos) { m.u16XPos = u16XPos; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16YPos(const UInt16 &u16YPos)
{
	bool modified = false;
	if (m.u16YPos != u16YPos) { m.u16YPos = u16YPos; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16Level(const UInt16 &u16Level)
{
	bool modified = false;
	if (m.u16Level != u16Level) { m.u16Level = u16Level; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU8SexClass(const UInt8 &u8SexClass)
{
	bool modified = false;
	if (m.u8SexClass != u8SexClass) { m.u8SexClass = u8SexClass; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU8Faction(const UInt8 &u8Faction)
{
	bool modified = false;
	if (m.u8Faction != u8Faction) { m.u8Faction = u8Faction; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16Hp(const UInt16 &u16Hp)
{
	bool modified = false;
	if (m.u16Hp != u16Hp) { m.u16Hp = u16Hp; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16MHp(const UInt16 &u16MHp)
{
	bool modified = false;
	if (m.u16MHp != u16MHp) { m.u16MHp = u16MHp; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16MaxHp(const UInt16 &u16MaxHp)
{
	bool modified = false;
	if (m.u16MaxHp != u16MaxHp) { m.u16MaxHp = u16MaxHp; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16Sp(const UInt16 &u16Sp)
{
	bool modified = false;
	if (m.u16Sp != u16Sp) { m.u16Sp = u16Sp; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16MSp(const UInt16 &u16MSp)
{
	bool modified = false;
	if (m.u16MSp != u16MSp) { m.u16MSp = u16MSp; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetU16MaxSp(const UInt16 &u16MaxSp)
{
	bool modified = false;
	if (m.u16MaxSp != u16MaxSp) { m.u16MaxSp = u16MaxSp; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetStrNickName(const String &strNickName)
{
	bool modified = false;
	if (m.strNickName != strNickName) { m.strNickName = strNickName; modified = true; }
	MarkUpdateAll(CharBaseInfoMask, modified);
}

void PartyCharInfo_Stub::SetCharOrgEq(const UInt32 &u32OrgCloth, const UInt16 &u16OrgClothColor, const UInt16 &u16OrgHairStyle, const UInt16 &u16OrgHairColor, const UInt16 &u16HairStyle1, const UInt16 &u16HairColor)
{
	bool modified = false;
	if (m.u32OrgCloth != u32OrgCloth) { m.u32OrgCloth = u32OrgCloth; modified = true; }
	if (m.u16OrgClothColor != u16OrgClothColor) { m.u16OrgClothColor = u16OrgClothColor; modified = true; }
	if (m.u16OrgHairStyle != u16OrgHairStyle) { m.u16OrgHairStyle = u16OrgHairStyle; modified = true; }
	if (m.u16OrgHairColor != u16OrgHairColor) { m.u16OrgHairColor = u16OrgHairColor; modified = true; }
	if (m.u16HairStyle1 != u16HairStyle1) { m.u16HairStyle1 = u16HairStyle1; modified = true; }
	if (m.u16HairColor != u16HairColor) { m.u16HairColor = u16HairColor; modified = true; }
	MarkUpdateAll(CharOrgEqMask, modified);
}

void PartyCharInfo_Stub::SetU32OrgCloth(const UInt32 &u32OrgCloth)
{
	bool modified = false;
	if (m.u32OrgCloth != u32OrgCloth) { m.u32OrgCloth = u32OrgCloth; modified = true; }
	MarkUpdateAll(CharOrgEqMask, modified);
}

void PartyCharInfo_Stub::SetU16OrgClothColor(const UInt16 &u16OrgClothColor)
{
	bool modified = false;
	if (m.u16OrgClothColor != u16OrgClothColor) { m.u16OrgClothColor = u16OrgClothColor; modified = true; }
	MarkUpdateAll(CharOrgEqMask, modified);
}

void PartyCharInfo_Stub::SetU16OrgHairStyle(const UInt16 &u16OrgHairStyle)
{
	bool modified = false;
	if (m.u16OrgHairStyle != u16OrgHairStyle) { m.u16OrgHairStyle = u16OrgHairStyle; modified = true; }
	MarkUpdateAll(CharOrgEqMask, modified);
}

void PartyCharInfo_Stub::SetU16OrgHairColor(const UInt16 &u16OrgHairColor)
{
	bool modified = false;
	if (m.u16OrgHairColor != u16OrgHairColor) { m.u16OrgHairColor = u16OrgHairColor; modified = true; }
	MarkUpdateAll(CharOrgEqMask, modified);
}

void PartyCharInfo_Stub::SetU16HairStyle1(const UInt16 &u16HairStyle1)
{
	bool modified = false;
	if (m.u16HairStyle1 != u16HairStyle1) { m.u16HairStyle1 = u16HairStyle1; modified = true; }
	MarkUpdateAll(CharOrgEqMask, modified);
}

void PartyCharInfo_Stub::SetU16HairColor(const UInt16 &u16HairColor)
{
	bool modified = false;
	if (m.u16HairColor != u16HairColor) { m.u16HairColor = u16HairColor; modified = true; }
	MarkUpdateAll(CharOrgEqMask, modified);
}

void PartyCharInfo_Stub::SetCharEq(const UInt32 &u32Cloth, const UInt16 &u16ClothColor, const UInt16 &u16HeadBand, const UInt32 &u32Weapon)
{
	bool modified = false;
	if (m.u32Cloth != u32Cloth) { m.u32Cloth = u32Cloth; modified = true; }
	if (m.u16ClothColor != u16ClothColor) { m.u16ClothColor = u16ClothColor; modified = true; }
	if (m.u16HeadBand != u16HeadBand) { m.u16HeadBand = u16HeadBand; modified = true; }
	if (m.u32Weapon != u32Weapon) { m.u32Weapon = u32Weapon; modified = true; }
	MarkUpdateAll(CharEqMask, modified);
}

void PartyCharInfo_Stub::SetU32Cloth(const UInt32 &u32Cloth)
{
	bool modified = false;
	if (m.u32Cloth != u32Cloth) { m.u32Cloth = u32Cloth; modified = true; }
	MarkUpdateAll(CharEqMask, modified);
}

void PartyCharInfo_Stub::SetU16ClothColor(const UInt16 &u16ClothColor)
{
	bool modified = false;
	if (m.u16ClothColor != u16ClothColor) { m.u16ClothColor = u16ClothColor; modified = true; }
	MarkUpdateAll(CharEqMask, modified);
}

void PartyCharInfo_Stub::SetU16HeadBand(const UInt16 &u16HeadBand)
{
	bool modified = false;
	if (m.u16HeadBand != u16HeadBand) { m.u16HeadBand = u16HeadBand; modified = true; }
	MarkUpdateAll(CharEqMask, modified);
}

void PartyCharInfo_Stub::SetU32Weapon(const UInt32 &u32Weapon)
{
	bool modified = false;
	if (m.u32Weapon != u32Weapon) { m.u32Weapon = u32Weapon; modified = true; }
	MarkUpdateAll(CharEqMask, modified);
}



void PartyCharInfo_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void PartyCharInfo_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(PartyIndexMask))
	{
		buf.Write<UInt16>(m.u16Index);
	}
	if (mask.IsBit(CharBaseInfoMask))
	{
		buf.Write<UInt32>(m.u32CharId);
		buf.Write<RPCNetID>(m.stNetId);
		buf.Write<UInt16>(m.u16MapId);
		buf.Write<UInt16>(m.u16XPos);
		buf.Write<UInt16>(m.u16YPos);
		buf.Write<UInt16>(m.u16Level);
		buf.Write<UInt8>(m.u8SexClass);
		buf.Write<UInt8>(m.u8Faction);
		buf.Write<UInt16>(m.u16Hp);
		buf.Write<UInt16>(m.u16MHp);
		buf.Write<UInt16>(m.u16MaxHp);
		buf.Write<UInt16>(m.u16Sp);
		buf.Write<UInt16>(m.u16MSp);
		buf.Write<UInt16>(m.u16MaxSp);
		buf.Write<String>(m.strNickName);
	}
	if (mask.IsBit(CharOrgEqMask))
	{
		buf.Write<UInt32>(m.u32OrgCloth);
		buf.Write<UInt16>(m.u16OrgClothColor);
		buf.Write<UInt16>(m.u16OrgHairStyle);
		buf.Write<UInt16>(m.u16OrgHairColor);
		buf.Write<UInt16>(m.u16HairStyle1);
		buf.Write<UInt16>(m.u16HairColor);
	}
	if (mask.IsBit(CharEqMask))
	{
		buf.Write<UInt32>(m.u32Cloth);
		buf.Write<UInt16>(m.u16ClothColor);
		buf.Write<UInt16>(m.u16HeadBand);
		buf.Write<UInt32>(m.u32Weapon);
	}
}

RPCResult PartyCharInfo_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartyCharInfo_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult PartyCharInfo_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PartyCharInfo_REQ_STATE:
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

