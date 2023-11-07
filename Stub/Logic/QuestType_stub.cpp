//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/QuestType_stub.h"

void QuestType_Stub::SetId(const UInt32 &char_id, const UInt16 &Type_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestType_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestType_Stub::SetType_id(const UInt16 &Type_id)
{
	bool modified = false;
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestType_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void QuestType_Stub::SetRound(const UInt16 &round)
{
	bool modified = false;
	if (m.round != round) { m.round = round; modified = true; }
	MarkUpdateAll(roundMask, modified);
}

void QuestType_Stub::SetMaxRound(const UInt16 &maxRound)
{
	bool modified = false;
	if (m.maxRound != maxRound) { m.maxRound = maxRound; modified = true; }
	MarkUpdateAll(maxRoundMask, modified);
}

void QuestType_Stub::SetTotalRound(const UInt16 &totalRound)
{
	bool modified = false;
	if (m.totalRound != totalRound) { m.totalRound = totalRound; modified = true; }
	MarkUpdateAll(totalRoundMask, modified);
}

void QuestType_Stub::SetLimitRound(const UInt16 &limitRound)
{
	bool modified = false;
	if (m.limitRound != limitRound) { m.limitRound = limitRound; modified = true; }
	MarkUpdateAll(limitRoundMask, modified);
}

void QuestType_Stub::SetLimitType(const UInt16 &limitType)
{
	bool modified = false;
	if (m.limitType != limitType) { m.limitType = limitType; modified = true; }
	MarkUpdateAll(limitTypeMask, modified);
}

void QuestType_Stub::SetLastClearTime(const UInt32 &lastClearTime)
{
	bool modified = false;
	if (m.lastClearTime != lastClearTime) { m.lastClearTime = lastClearTime; modified = true; }
	MarkUpdateAll(lastClearTimeMask, modified);
}

void QuestType_Stub::SetStatus(const UInt8 &status)
{
	bool modified = false;
	if (m.status != status) { m.status = status; modified = true; }
	MarkUpdateAll(statusMask, modified);
}



void QuestType_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void QuestType_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.char_id);
		buf.Write<UInt16>(m.Type_id);
	}
	if (mask.IsBit(nameMask))
	{
		buf.Write<String>(m.name);
	}
	if (mask.IsBit(roundMask))
	{
		buf.Write<UInt16>(m.round);
	}
	if (mask.IsBit(maxRoundMask))
	{
		buf.Write<UInt16>(m.maxRound);
	}
	if (mask.IsBit(totalRoundMask))
	{
		buf.Write<UInt16>(m.totalRound);
	}
	if (mask.IsBit(limitRoundMask))
	{
		buf.Write<UInt16>(m.limitRound);
	}
	if (mask.IsBit(limitTypeMask))
	{
		buf.Write<UInt16>(m.limitType);
	}
	if (mask.IsBit(lastClearTimeMask))
	{
		buf.Write<UInt32>(m.lastClearTime);
	}
	if (mask.IsBit(statusMask))
	{
		buf.Write<UInt8>(m.status);
	}
}

RPCResult QuestType_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestType_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult QuestType_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestType_REQ_STATE:
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

