//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/HawkGroup_proxy.h"


void HawkGroup_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(ownerInfoMask))
	{
		buf.Read<UInt32>(m.ownerID);
	}
	if (m_updateMask.IsBit(hawkNameInfoMask))
	{
		buf.Read<String>(m.hawkName);
	}
}

void HawkGroup_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void HawkGroup_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(ownerInfoMask)) OnUpdateOwnerInfo(m.ownerID);
	if (IsUpdated(hawkNameInfoMask)) OnUpdateHawkNameInfo(m.hawkName);
}

#pragma optimize("", off)
RPCResult HawkGroup_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_HawkGroup_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_HawkGroup_CB_UpdateHawkItem:
		{
			UInt32 itemID;
			pBuf->Read<UInt32>(itemID);
			UInt8 itemType;
			pBuf->Read<UInt8>(itemType);
			_result = CB_UpdateHawkItem(pPeer, context, itemID, itemType);
		}
		break;
	case RPC_HawkGroup_CB_BuyHawkItemSuccess:
		{
			Boolean owner;
			pBuf->Read<Boolean>(owner);
			UInt32 itemID;
			pBuf->Read<UInt32>(itemID);
			UInt8 itemType;
			pBuf->Read<UInt8>(itemType);
			UInt8 itemStack;
			pBuf->Read<UInt8>(itemStack);
			UInt32 tax;
			pBuf->Read<UInt32>(tax);
			_result = CB_BuyHawkItemSuccess(pPeer, context, owner, itemID, itemType, itemStack, tax);
		}
		break;
	case RPC_HawkGroup_CB_ChangeHawkName:
		{
			_result = CB_ChangeHawkName(pPeer, context);
		}
		break;
	case RPC_HawkGroup_CB_CloseHawkForUser:
		{
			_result = CB_CloseHawkForUser(pPeer, context);
		}
		break;
	case RPC_HawkGroup_CB_StopHawk:
		{
			_result = CB_StopHawk(pPeer, context);
		}
		break;
	case RPC_HawkGroup_CB_AddSellStartHawkItemInfo:
		{
			NetGroup* addHawkItem;
			pBuf->Read<NetGroup*>(addHawkItem);
			_result = CB_AddSellStartHawkItemInfo(pPeer, context, addHawkItem);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult HawkGroup_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_HawkGroup_UP_STATE:
			_result = RPCDecode<408894990>(pPeer, pBuf);
		break;
	case RPC_HawkGroup_CB_UpdateHawkItem:
			_result = RPCDecode<665529049>(pPeer, pBuf);
		break;
	case RPC_HawkGroup_CB_BuyHawkItemSuccess:
			_result = RPCDecode<435233807>(pPeer, pBuf);
		break;
	case RPC_HawkGroup_CB_ChangeHawkName:
			_result = RPCDecode<569115728>(pPeer, pBuf);
		break;
	case RPC_HawkGroup_CB_CloseHawkForUser:
			_result = RPCDecode<1991732788>(pPeer, pBuf);
		break;
	case RPC_HawkGroup_CB_StopHawk:
			_result = RPCDecode<189008725>(pPeer, pBuf);
		break;
	case RPC_HawkGroup_CB_AddSellStartHawkItemInfo:
			_result = RPCDecode<1838372492>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
