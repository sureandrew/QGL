//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/Party_proxy.h"


void Party_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(availMask))
	{
		buf.Read<Boolean>(m.avail);
	}
	if (m_updateMask.IsBit(lineupMask))
	{
		buf.Read<UInt32>(m.m_uLineupId);
	}
	if (m_updateMask.IsBit(lineupkeyposMask))
	{
		buf.Read<int>(m.m_iLineupKeyPos);
	}
	if (m_updateMask.IsBit(DescMask))
	{
		buf.Read<String>(m.desc);
	}
	if (m_updateMask.IsBit(LeaveTemporaryMask))
	{
		buf.ReadArray<Boolean>(m.m_bTempLeave, 5);
	}
}

void Party_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void Party_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(availMask)) OnUpdateAvail(m.avail);
	if (IsUpdated(lineupMask)) OnUpdateLineup(m.m_uLineupId);
	if (IsUpdated(lineupkeyposMask)) OnUpdateLineupkeypos(m.m_iLineupKeyPos);
	if (IsUpdated(DescMask)) OnUpdateDesc(m.desc);
	if (IsUpdated(LeaveTemporaryMask)) OnUpdateLeaveTemporary(m.m_bTempLeave[0], m.m_bTempLeave[1], m.m_bTempLeave[2], m.m_bTempLeave[3], m.m_bTempLeave[4]);
}

#pragma optimize("", off)
RPCResult Party_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_Party_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_Party_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_Party_CB_LeaderChanged:
		{
			UInt32 memberId;
			pBuf->Read<UInt32>(memberId);
			UInt8 memberIdx;
			pBuf->Read<UInt8>(memberIdx);
			Boolean shown;
			pBuf->Read<Boolean>(shown);
			_result = CB_LeaderChanged(pPeer, context, memberId, memberIdx, shown);
		}
		break;
	case RPC_Party_CB_Join:
		{
			NetObject* member;
			pBuf->Read<NetObject*>(member);
			Boolean shown;
			pBuf->Read<Boolean>(shown);
			_result = CB_Join(pPeer, context, member, shown);
		}
		break;
	case RPC_Party_CB_Leave:
		{
			NetObject* member;
			pBuf->Read<NetObject*>(member);
			bool kick;
			pBuf->Read<bool>(kick);
			Boolean shown;
			pBuf->Read<Boolean>(shown);
			_result = CB_Leave(pPeer, context, member, kick, shown);
		}
		break;
	case RPC_Party_CB_Destroy:
		{
			bool kick;
			pBuf->Read<bool>(kick);
			Boolean shown;
			pBuf->Read<Boolean>(shown);
			_result = CB_Destroy(pPeer, context, kick, shown);
		}
		break;
	case RPC_Party_CB_Regroup:
		{
			NetObject* member;
			pBuf->Read<NetObject*>(member);
			UInt8 memberIdx;
			pBuf->Read<UInt8>(memberIdx);
			_result = CB_Regroup(pPeer, context, member, memberIdx);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult Party_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_Party_UP_STATE:
			_result = RPCDecode<148968060>(pPeer, pBuf);
		break;
	case RPC_Party_CB_Error:
			_result = RPCDecode<861240151>(pPeer, pBuf);
		break;
	case RPC_Party_CB_LeaderChanged:
			_result = RPCDecode<154238635>(pPeer, pBuf);
		break;
	case RPC_Party_CB_Join:
			_result = RPCDecode<794261500>(pPeer, pBuf);
		break;
	case RPC_Party_CB_Leave:
			_result = RPCDecode<948005376>(pPeer, pBuf);
		break;
	case RPC_Party_CB_Destroy:
			_result = RPCDecode<809373378>(pPeer, pBuf);
		break;
	case RPC_Party_CB_Regroup:
			_result = RPCDecode<1355247430>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
