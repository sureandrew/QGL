//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/RelationSystem_stub.h"



void RelationSystem_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void RelationSystem_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

RPCResult RelationSystem_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult RelationSystem_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_RelationSystem_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_RelationSystem_SetSiblingTitlePrefix:
		{
			String prefix;
			pBuf->Read<String>(prefix);
			_result = SetSiblingTitlePrefix(pPeer, context, prefix);
		}
		break;
	case RPC_RelationSystem_SetSiblingTitleSuffix:
		{
			String suffix;
			pBuf->Read<String>(suffix);
			_result = SetSiblingTitleSuffix(pPeer, context, suffix);
		}
		break;
	case RPC_RelationSystem_SelectMarriageTime:
		{
			UInt32 hour;
			pBuf->Read<UInt32>(hour);
			UInt32 minute;
			pBuf->Read<UInt32>(minute);
			_result = SelectMarriageTime(pPeer, context, hour, minute);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult RelationSystem_Stub::CB_RequestSetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &thirdWord)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_CB_RequestSetSiblingTitlePrefix);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(thirdWord);
	return RPCSend<634673698>(pPeer, pBuf);
}

RPCResult RelationSystem_Stub::CB_RequestSetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &prefix)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_CB_RequestSetSiblingTitleSuffix);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(prefix);
	return RPCSend<921392118>(pPeer, pBuf);
}

RPCResult RelationSystem_Stub::CB_RequestSelectMarriageTime(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_RelationSystem_CB_RequestSelectMarriageTime);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<665868924>(pPeer, pBuf);
}

