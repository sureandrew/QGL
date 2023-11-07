//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/Party_stub.h"

void Party_Stub::SetAvail(const Boolean &avail)
{
	bool modified = false;
	if (m.avail != avail) { m.avail = avail; modified = true; }
	MarkUpdateAll(availMask, modified);
}

void Party_Stub::SetLineup(const UInt32 &m_uLineupId)
{
	bool modified = false;
	if (m.m_uLineupId != m_uLineupId) { m.m_uLineupId = m_uLineupId; modified = true; }
	MarkUpdateAll(lineupMask, modified);
}

void Party_Stub::SetM_uLineupId(const UInt32 &m_uLineupId)
{
	bool modified = false;
	if (m.m_uLineupId != m_uLineupId) { m.m_uLineupId = m_uLineupId; modified = true; }
	MarkUpdateAll(lineupMask, modified);
}

void Party_Stub::SetLineupkeypos(const int &m_iLineupKeyPos)
{
	bool modified = false;
	if (m.m_iLineupKeyPos != m_iLineupKeyPos) { m.m_iLineupKeyPos = m_iLineupKeyPos; modified = true; }
	MarkUpdateAll(lineupkeyposMask, modified);
}

void Party_Stub::SetM_iLineupKeyPos(const int &m_iLineupKeyPos)
{
	bool modified = false;
	if (m.m_iLineupKeyPos != m_iLineupKeyPos) { m.m_iLineupKeyPos = m_iLineupKeyPos; modified = true; }
	MarkUpdateAll(lineupkeyposMask, modified);
}

void Party_Stub::SetDesc(const String &desc)
{
	bool modified = false;
	if (m.desc != desc) { m.desc = desc; modified = true; }
	MarkUpdateAll(DescMask, modified);
}

void Party_Stub::SetLeaveTemporary(const Boolean &m_bTempLeave0, const Boolean &m_bTempLeave1, const Boolean &m_bTempLeave2, const Boolean &m_bTempLeave3, const Boolean &m_bTempLeave4)
{
	bool modified = false;
	if (m.m_bTempLeave[0] != m_bTempLeave0) { m.m_bTempLeave[0] = m_bTempLeave0; modified = true; }
	if (m.m_bTempLeave[1] != m_bTempLeave1) { m.m_bTempLeave[1] = m_bTempLeave1; modified = true; }
	if (m.m_bTempLeave[2] != m_bTempLeave2) { m.m_bTempLeave[2] = m_bTempLeave2; modified = true; }
	if (m.m_bTempLeave[3] != m_bTempLeave3) { m.m_bTempLeave[3] = m_bTempLeave3; modified = true; }
	if (m.m_bTempLeave[4] != m_bTempLeave4) { m.m_bTempLeave[4] = m_bTempLeave4; modified = true; }
	MarkUpdateAll(LeaveTemporaryMask, modified);
}

void Party_Stub::SetM_bTempLeave(const UInt8 i, const Boolean &m_bTempLeave)
{
	bool modified = false;
	if (m.m_bTempLeave[i] != m_bTempLeave) { m.m_bTempLeave[i] = m_bTempLeave; modified = true; }
	MarkUpdateAll(LeaveTemporaryMask, modified);
}



void Party_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void Party_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(availMask))
	{
		buf.Write<Boolean>(m.avail);
	}
	if (mask.IsBit(lineupMask))
	{
		buf.Write<UInt32>(m.m_uLineupId);
	}
	if (mask.IsBit(lineupkeyposMask))
	{
		buf.Write<int>(m.m_iLineupKeyPos);
	}
	if (mask.IsBit(DescMask))
	{
		buf.Write<String>(m.desc);
	}
	if (mask.IsBit(LeaveTemporaryMask))
	{
		buf.WriteArray<Boolean>(m.m_bTempLeave, 5);
	}
}

RPCResult Party_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult Party_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Party_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_Party_Promote:
		{
			RPCNetID memberId;
			pBuf->Read<RPCNetID>(memberId);
			_result = Promote(pPeer, context, memberId);
		}
		break;
	case RPC_Party_Kick:
		{
			RPCNetID memberId;
			pBuf->Read<RPCNetID>(memberId);
			_result = Kick(pPeer, context, memberId);
		}
		break;
	case RPC_Party_Leave:
		{
			_result = Leave(pPeer, context);
		}
		break;
	case RPC_Party_Destroy:
		{
			_result = Destroy(pPeer, context);
		}
		break;
	case RPC_Party_JoinChannel:
		{
			UInt32 char_id;
			pBuf->Read<UInt32>(char_id);
			_result = JoinChannel(pPeer, context, char_id);
		}
		break;
	case RPC_Party_ChangeDesc:
		{
			String desc;
			pBuf->Read<String>(desc);
			_result = ChangeDesc(pPeer, context, desc);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult Party_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_CB_Error);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(errCode);
	pBuf->Write<String>(errMsg);
	return RPCSend<861240151>(pPeer, pBuf);
}

RPCResult Party_Stub::CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, const UInt32 &memberId, const UInt8 &memberIdx, const Boolean &shown)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_CB_LeaderChanged);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(memberId);
	pBuf->Write<UInt8>(memberIdx);
	pBuf->Write<Boolean>(shown);
	return RPCSend<154238635>(pPeer, pBuf);
}

RPCResult Party_Stub::CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, const Boolean &shown)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_CB_Join);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(member);
	pBuf->Write<Boolean>(shown);
	return RPCSend<794261500>(pPeer, pBuf);
}

RPCResult Party_Stub::CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject* member, const bool &kick, const Boolean &shown)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_CB_Leave);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(member);
	pBuf->Write<bool>(kick);
	pBuf->Write<Boolean>(shown);
	return RPCSend<948005376>(pPeer, pBuf);
}

RPCResult Party_Stub::CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, const Boolean &shown)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_CB_Destroy);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<bool>(kick);
	pBuf->Write<Boolean>(shown);
	return RPCSend<809373378>(pPeer, pBuf);
}

RPCResult Party_Stub::CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, const UInt8 &memberIdx)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Party_CB_Regroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(member);
	pBuf->Write<UInt8>(memberIdx);
	return RPCSend<1355247430>(pPeer, pBuf);
}

