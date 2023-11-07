//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/QuestTypeData_stub.h"

void QuestTypeData_Stub::SetId(const UInt32 &char_id, const UInt16 &Type_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestTypeData_Stub::SetChar_id(const UInt32 &char_id)
{
	bool modified = false;
	if (m.char_id != char_id) { m.char_id = char_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestTypeData_Stub::SetType_id(const UInt16 &Type_id)
{
	bool modified = false;
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestTypeData_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void QuestTypeData_Stub::SetRound(const UInt16 &round)
{
	bool modified = false;
	if (m.round != round) { m.round = round; modified = true; }
	MarkUpdateAll(roundMask, modified);
}

void QuestTypeData_Stub::SetMaxRound(const UInt16 &maxRound)
{
	bool modified = false;
	if (m.maxRound != maxRound) { m.maxRound = maxRound; modified = true; }
	MarkUpdateAll(maxRoundMask, modified);
}

void QuestTypeData_Stub::SetTotalRound(const UInt16 &totalRound)
{
	bool modified = false;
	if (m.totalRound != totalRound) { m.totalRound = totalRound; modified = true; }
	MarkUpdateAll(totalRoundMask, modified);
}

void QuestTypeData_Stub::SetLimitRound(const UInt16 &limitRound)
{
	bool modified = false;
	if (m.limitRound != limitRound) { m.limitRound = limitRound; modified = true; }
	MarkUpdateAll(limitRoundMask, modified);
}

void QuestTypeData_Stub::SetLimitType(const UInt16 &limitType)
{
	bool modified = false;
	if (m.limitType != limitType) { m.limitType = limitType; modified = true; }
	MarkUpdateAll(limitTypeMask, modified);
}

void QuestTypeData_Stub::SetLastClearTime(const UInt32 &lastClearTime)
{
	bool modified = false;
	if (m.lastClearTime != lastClearTime) { m.lastClearTime = lastClearTime; modified = true; }
	MarkUpdateAll(lastClearTimeMask, modified);
}

void QuestTypeData_Stub::SetStatus(const UInt8 &status)
{
	bool modified = false;
	if (m.status != status) { m.status = status; modified = true; }
	MarkUpdateAll(statusMask, modified);
}


void QuestTypeData_Stub::Clone(QuestTypeData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(idMask) && !(obj->m.char_id == m.char_id)) {
		m.char_id = obj->m.char_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(idMask) && !(obj->m.Type_id == m.Type_id)) {
		m.Type_id = obj->m.Type_id;
		MarkUpdateAll(idMask, true);
	}
	if (mask.IsBit(nameMask) && !(obj->m.name == m.name)) {
		m.name = obj->m.name;
		MarkUpdateAll(nameMask, true);
	}
	if (mask.IsBit(roundMask) && !(obj->m.round == m.round)) {
		m.round = obj->m.round;
		MarkUpdateAll(roundMask, true);
	}
	if (mask.IsBit(maxRoundMask) && !(obj->m.maxRound == m.maxRound)) {
		m.maxRound = obj->m.maxRound;
		MarkUpdateAll(maxRoundMask, true);
	}
	if (mask.IsBit(totalRoundMask) && !(obj->m.totalRound == m.totalRound)) {
		m.totalRound = obj->m.totalRound;
		MarkUpdateAll(totalRoundMask, true);
	}
	if (mask.IsBit(limitRoundMask) && !(obj->m.limitRound == m.limitRound)) {
		m.limitRound = obj->m.limitRound;
		MarkUpdateAll(limitRoundMask, true);
	}
	if (mask.IsBit(limitTypeMask) && !(obj->m.limitType == m.limitType)) {
		m.limitType = obj->m.limitType;
		MarkUpdateAll(limitTypeMask, true);
	}
	if (mask.IsBit(lastClearTimeMask) && !(obj->m.lastClearTime == m.lastClearTime)) {
		m.lastClearTime = obj->m.lastClearTime;
		MarkUpdateAll(lastClearTimeMask, true);
	}
	if (mask.IsBit(statusMask) && !(obj->m.status == m.status)) {
		m.status = obj->m.status;
		MarkUpdateAll(statusMask, true);
	}
}

void QuestTypeData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(idMask))
	{
		buf.Read<UInt32>(m.char_id);
		buf.Read<UInt16>(m.Type_id);
	}
	if (m_updateMask.IsBit(nameMask))
	{
		buf.Read<String>(m.name);
	}
	if (m_updateMask.IsBit(roundMask))
	{
		buf.Read<UInt16>(m.round);
	}
	if (m_updateMask.IsBit(maxRoundMask))
	{
		buf.Read<UInt16>(m.maxRound);
	}
	if (m_updateMask.IsBit(totalRoundMask))
	{
		buf.Read<UInt16>(m.totalRound);
	}
	if (m_updateMask.IsBit(limitRoundMask))
	{
		buf.Read<UInt16>(m.limitRound);
	}
	if (m_updateMask.IsBit(limitTypeMask))
	{
		buf.Read<UInt16>(m.limitType);
	}
	if (m_updateMask.IsBit(lastClearTimeMask))
	{
		buf.Read<UInt32>(m.lastClearTime);
	}
	if (m_updateMask.IsBit(statusMask))
	{
		buf.Read<UInt8>(m.status);
	}
}

void QuestTypeData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

RPCResult QuestTypeData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestTypeData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult QuestTypeData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestTypeData_REQ_STATE:
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

