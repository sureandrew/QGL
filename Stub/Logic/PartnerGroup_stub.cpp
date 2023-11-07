//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/PartnerGroup_stub.h"



void PartnerGroup_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void PartnerGroup_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

RPCResult PartnerGroup_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult PartnerGroup_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PartnerGroup_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	case RPC_PartnerGroup_DeletePartner:
		{
			UInt32 partner_id;
			pBuf->Read<UInt32>(partner_id);
			_result = DeletePartner(pPeer, context, partner_id);
		}
		break;
	case RPC_PartnerGroup_ChangePartnerState:
		{
			UInt32 partner_id;
			pBuf->Read<UInt32>(partner_id);
			UInt8 state;
			pBuf->Read<UInt8>(state);
			_result = ChangePartnerState(pPeer, context, partner_id, state);
		}
		break;
	case RPC_PartnerGroup_PutToWareHouse:
		{
			UInt32 partnerID;
			pBuf->Read<UInt32>(partnerID);
			_result = PutToWareHouse(pPeer, context, partnerID);
		}
		break;
	case RPC_PartnerGroup_GetFromWareHouse:
		{
			UInt32 partnerID;
			pBuf->Read<UInt32>(partnerID);
			_result = GetFromWareHouse(pPeer, context, partnerID);
		}
		break;
	case RPC_PartnerGroup_PartnerCultivationUpByPartner:
		{
			UInt32 src_partnerID;
			pBuf->Read<UInt32>(src_partnerID);
			UInt32 dest_partnerID;
			pBuf->Read<UInt32>(dest_partnerID);
			_result = PartnerCultivationUpByPartner(pPeer, context, src_partnerID, dest_partnerID);
		}
		break;
	case RPC_PartnerGroup_PartnerCultivationUpByItem:
		{
			UInt32 src_partnerID;
			pBuf->Read<UInt32>(src_partnerID);
			UInt32 dest_itemUid;
			pBuf->Read<UInt32>(dest_itemUid);
			_result = PartnerCultivationUpByItem(pPeer, context, src_partnerID, dest_itemUid);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

RPCResult PartnerGroup_Stub::CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerIDs, const bool &showDelMsg)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_DeleteListSuccess);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32Vector>(partnerIDs);
	pBuf->Write<bool>(showDelMsg);
	return RPCSend<1305766728>(pPeer, pBuf);
}

RPCResult PartnerGroup_Stub::CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, NetGroup* skillGroups)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_AddPartnerGroup);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(partnerGroup);
	pBuf->Write<NetGroup*>(skillGroups);
	return RPCSend<1607038446>(pPeer, pBuf);
}

RPCResult PartnerGroup_Stub::CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_ChangePartnerState);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<251283557>(pPeer, pBuf);
}

RPCResult PartnerGroup_Stub::CB_AddPartner(LPCPEER pPeer, RPCContext &context, NetObject* partner, NetGroup* skillGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_AddPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetObject*>(partner);
	pBuf->Write<NetGroup*>(skillGroup);
	return RPCSend<362050677>(pPeer, pBuf);
}

RPCResult PartnerGroup_Stub::CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_UpdateAllPartner);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(partnerGroup);
	return RPCSend<1209149290>(pPeer, pBuf);
}

RPCResult PartnerGroup_Stub::CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_AddSkills);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<NetGroup*>(skillGrp);
	return RPCSend<1252151694>(pPeer, pBuf);
}

RPCResult PartnerGroup_Stub::CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_WareHouse);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(processIndex);
	pBuf->Write<UInt8>(msgIndex);
	return RPCSend<1840984601>(pPeer, pBuf);
}

RPCResult PartnerGroup_Stub::CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_PartnerGroup_CB_PartnerCultivationUp);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	if (context.bCallback && context.pcdId.subId != 0)
		pBuf->Write<RPCContext>(GetCbContext(context));
	else
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt8>(msgIndex);
	pBuf->Write<UInt8>(msgValue);
	pBuf->Write<UInt32>(src_partnerID);
	return RPCSend<677792347>(pPeer, pBuf);
}

