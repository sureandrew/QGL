//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/PartnerGroup_proxy.h"


void PartnerGroup_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void PartnerGroup_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void PartnerGroup_Proxy::OnUpdate()
{
	Parent::OnUpdate();
}

#pragma optimize("", off)
RPCResult PartnerGroup_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PartnerGroup_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_PartnerGroup_CB_DeleteListSuccess:
		{
			UInt32Vector partnerIDs;
			pBuf->Read<UInt32Vector>(partnerIDs);
			bool showDelMsg;
			pBuf->Read<bool>(showDelMsg);
			_result = CB_DeleteListSuccess(pPeer, context, partnerIDs, showDelMsg);
		}
		break;
	case RPC_PartnerGroup_CB_AddPartnerGroup:
		{
			NetGroup* partnerGroup;
			pBuf->Read<NetGroup*>(partnerGroup);
			NetGroup* skillGroups;
			pBuf->Read<NetGroup*>(skillGroups);
			_result = CB_AddPartnerGroup(pPeer, context, partnerGroup, skillGroups);
		}
		break;
	case RPC_PartnerGroup_CB_ChangePartnerState:
		{
			_result = CB_ChangePartnerState(pPeer, context);
		}
		break;
	case RPC_PartnerGroup_CB_AddPartner:
		{
			NetObject* partner;
			pBuf->Read<NetObject*>(partner);
			NetGroup* skillGroup;
			pBuf->Read<NetGroup*>(skillGroup);
			_result = CB_AddPartner(pPeer, context, partner, skillGroup);
		}
		break;
	case RPC_PartnerGroup_CB_UpdateAllPartner:
		{
			NetGroup* partnerGroup;
			pBuf->Read<NetGroup*>(partnerGroup);
			_result = CB_UpdateAllPartner(pPeer, context, partnerGroup);
		}
		break;
	case RPC_PartnerGroup_CB_AddSkills:
		{
			NetGroup* skillGrp;
			pBuf->Read<NetGroup*>(skillGrp);
			_result = CB_AddSkills(pPeer, context, skillGrp);
		}
		break;
	case RPC_PartnerGroup_CB_WareHouse:
		{
			UInt8 processIndex;
			pBuf->Read<UInt8>(processIndex);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_WareHouse(pPeer, context, processIndex, msgIndex);
		}
		break;
	case RPC_PartnerGroup_CB_PartnerCultivationUp:
		{
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			UInt8 msgValue;
			pBuf->Read<UInt8>(msgValue);
			UInt32 src_partnerID;
			pBuf->Read<UInt32>(src_partnerID);
			_result = CB_PartnerCultivationUp(pPeer, context, msgIndex, msgValue, src_partnerID);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult PartnerGroup_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_PartnerGroup_UP_STATE:
			_result = RPCDecode<852316315>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_DeleteListSuccess:
			_result = RPCDecode<1305766728>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_AddPartnerGroup:
			_result = RPCDecode<1607038446>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_ChangePartnerState:
			_result = RPCDecode<251283557>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_AddPartner:
			_result = RPCDecode<362050677>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_UpdateAllPartner:
			_result = RPCDecode<1209149290>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_AddSkills:
			_result = RPCDecode<1252151694>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_WareHouse:
			_result = RPCDecode<1840984601>(pPeer, pBuf);
		break;
	case RPC_PartnerGroup_CB_PartnerCultivationUp:
			_result = RPCDecode<677792347>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
