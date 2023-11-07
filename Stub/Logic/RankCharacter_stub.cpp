//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/RankCharacter_stub.h"

void RankCharacter_Stub::SetId(const UInt32 &acct_id, const UInt32 &char_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void RankCharacter_Stub::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void RankCharacter_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void RankCharacter_Stub::SetInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	if (m.sex != sex) { m.sex = sex; modified = true; }
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void RankCharacter_Stub::SetNickName(const String &nickName)
{
	bool modified = false;
	if (m.nickName != nickName) { m.nickName = nickName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void RankCharacter_Stub::SetSex(const UInt8 &sex)
{
	bool modified = false;
	if (m.sex != sex) { m.sex = sex; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void RankCharacter_Stub::SetCclass(const UInt8 &cclass)
{
	bool modified = false;
	if (m.cclass != cclass) { m.cclass = cclass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void RankCharacter_Stub::SetSexClass(const UInt8 &sexClass)
{
	bool modified = false;
	if (m.sexClass != sexClass) { m.sexClass = sexClass; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void RankCharacter_Stub::SetFaction(const UInt8 &faction)
{
	bool modified = false;
	if (m.faction != faction) { m.faction = faction; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void RankCharacter_Stub::SetLevel(const UInt16 &level)
{
	bool modified = false;
	if (m.level != level) { m.level = level; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void RankCharacter_Stub::SetMoney(const UInt32 &money)
{
	bool modified = false;
	if (m.money != money) { m.money = money; modified = true; }
	MarkUpdateAll(moneyMask, modified);
}

void RankCharacter_Stub::SetPKInfo(const UInt16 &PK_Value, const UInt32 &wantedReward)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	if (m.wantedReward != wantedReward) { m.wantedReward = wantedReward; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void RankCharacter_Stub::SetPK_Value(const UInt16 &PK_Value)
{
	bool modified = false;
	if (m.PK_Value != PK_Value) { m.PK_Value = PK_Value; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void RankCharacter_Stub::SetWantedReward(const UInt32 &wantedReward)
{
	bool modified = false;
	if (m.wantedReward != wantedReward) { m.wantedReward = wantedReward; modified = true; }
	MarkUpdateAll(PKInfoMask, modified);
}

void RankCharacter_Stub::SetHonor(const UInt32 &honor)
{
	bool modified = false;
	if (m.honor != honor) { m.honor = honor; modified = true; }
	MarkUpdateAll(honorMask, modified);
}

void RankCharacter_Stub::SetBankMoney(const UInt32 &bankMoney)
{
	bool modified = false;
	if (m.bankMoney != bankMoney) { m.bankMoney = bankMoney; modified = true; }
	MarkUpdateAll(bankMoneyMask, modified);
}

void RankCharacter_Stub::SetPvPScoreSingle(const UInt32 &PvPScoreSingle)
{
	bool modified = false;
	if (m.PvPScoreSingle != PvPScoreSingle) { m.PvPScoreSingle = PvPScoreSingle; modified = true; }
	MarkUpdateAll(PvPScoreSingleMask, modified);
}



void RankCharacter_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void RankCharacter_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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
		buf.Write<UInt8>(m.sexClass);
		buf.Write<UInt8>(m.faction);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.level);
	}
	if (mask.IsBit(moneyMask))
	{
		buf.Write<UInt32>(m.money);
	}
	if (mask.IsBit(PKInfoMask))
	{
		buf.Write<UInt16>(m.PK_Value);
		buf.Write<UInt32>(m.wantedReward);
	}
	if (mask.IsBit(honorMask))
	{
		buf.Write<UInt32>(m.honor);
	}
	if (mask.IsBit(bankMoneyMask))
	{
		buf.Write<UInt32>(m.bankMoney);
	}
	if (mask.IsBit(PvPScoreSingleMask))
	{
		buf.Write<UInt32>(m.PvPScoreSingle);
	}
}

RPCResult RankCharacter_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RankCharacter_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult RankCharacter_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_RankCharacter_REQ_STATE:
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

