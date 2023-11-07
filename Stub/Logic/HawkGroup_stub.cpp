//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/HawkGroup_stub.h"

void HawkGroup_Stub::SetOwnerInfo(const UInt32 &ownerID)
{
	bool modified = false;
	if (m.ownerID != ownerID) { m.ownerID = ownerID; modified = true; }
	MarkUpdateAll(ownerInfoMask, modified);
}

void HawkGroup_Stub::SetOwnerID(const UInt32 &ownerID)
{
	bool modified = false;
	if (m.ownerID != ownerID) { m.ownerID = ownerID; modified = true; }
	MarkUpdateAll(ownerInfoMask, modified);
}

void HawkGroup_Stub::SetHawkNameInfo(const String &hawkName)
{
	bool modified = false;
	if (m.hawkName != hawkName) { m.hawkName = hawkName; modified = true; }
	MarkUpdateAll(hawkNameInfoMask, modified);
}

void HawkGroup_Stub::SetHawkName(const String &hawkName)
{
	bool modified = false;
	if (m.hawkName != hawkName) { m.hawkName = hawkName; modified = true; }
	MarkUpdateAll(hawkNameInfoMask, modified);
}



void HawkGroup_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void HawkGroup_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(ownerInfoMask))
	{
		buf.Write<UInt32>(m.ownerID);
	}
	if (mask.IsBit(hawkNameInfoMask))
	{
		buf.Write<String>(m.hawkName);
	}
}

RPCResult HawkGroup_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkGroup_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult HawkGroup_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_HawkGroup_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_HawkGroup_BuyHawkItem:
		{
			UInt32 itemID;
			pBuf->Read<UInt32>(itemID);
			UInt8 itemType;
			pBuf->Read<UInt8>(itemType);
			UInt8 itemStack;
			pBuf->Read<UInt8>(itemStack);
			_result = BuyHawkItem(pPeer, context, itemID, itemType, itemStack);
		}
		break;
	case RPC_HawkGroup_ChangeHawkName:
		{
			String hawkName;
			pBuf->Read<String>(hawkName);
			_result = ChangeHawkName(pPeer, context, hawkName);
		}
		break;
	case RPC_HawkGroup_CloseHawkForUser:
		{
			UInt32 charID;
			pBuf->Read<UInt32>(charID);
			_result = CloseHawkForUser(pPeer, context, charID);
		}
		break;
	case RPC_HawkGroup_StopHawk:
		{
			_result = StopHawk(pPeer, context);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult HawkGroup_Stub::CB_UpdateHawkItem(LPCPEER pPeer, RPCContext &context, const UInt32 &itemID, const UInt8 &itemType)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkGroup_CB_UpdateHawkItem);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(itemID);
	pBuf->Write<UInt8>(itemType);
	return RPCSend<665529049>(pPeer, pBuf);
}

RPCResult HawkGroup_Stub::CB_BuyHawkItemSuccess(LPCPEER pPeer, RPCContext &context, const Boolean &owner, const UInt32 &itemID, const UInt8 &itemType, const UInt8 &itemStack, const UInt32 &tax)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkGroup_CB_BuyHawkItemSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<Boolean>(owner);
	pBuf->Write<UInt32>(itemID);
	pBuf->Write<UInt8>(itemType);
	pBuf->Write<UInt8>(itemStack);
	pBuf->Write<UInt32>(tax);
	return RPCSend<435233807>(pPeer, pBuf);
}

RPCResult HawkGroup_Stub::CB_ChangeHawkName(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkGroup_CB_ChangeHawkName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<569115728>(pPeer, pBuf);
}

RPCResult HawkGroup_Stub::CB_CloseHawkForUser(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkGroup_CB_CloseHawkForUser);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<1991732788>(pPeer, pBuf);
}

RPCResult HawkGroup_Stub::CB_StopHawk(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkGroup_CB_StopHawk);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<189008725>(pPeer, pBuf);
}

RPCResult HawkGroup_Stub::CB_AddSellStartHawkItemInfo(LPCPEER pPeer, RPCContext &context, NetGroup* addHawkItem)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_HawkGroup_CB_AddSellStartHawkItemInfo);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(addHawkItem);
	return RPCSend<1838372492>(pPeer, pBuf);
}

