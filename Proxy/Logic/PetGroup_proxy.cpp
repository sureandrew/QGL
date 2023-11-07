//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/PetGroup_proxy.h"


void PetGroup_Proxy::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(ownerMask))
	{
		buf.Read<UInt32>(m.owner_id);
	}
}

void PetGroup_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
}

void PetGroup_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(ownerMask)) OnUpdateOwner(m.owner_id);
}

#pragma optimize("", off)
RPCResult PetGroup_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_PetGroup_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	case RPC_PetGroup_CB_Error:
		{
			UInt32 errCode;
			pBuf->Read<UInt32>(errCode);
			String errMsg;
			pBuf->Read<String>(errMsg);
			_result = CB_Error(pPeer, context, errCode, errMsg);
		}
		break;
	case RPC_PetGroup_CB_AddSkills:
		{
			NetGroup* skillGrp;
			pBuf->Read<NetGroup*>(skillGrp);
			_result = CB_AddSkills(pPeer, context, skillGrp);
		}
		break;
	case RPC_PetGroup_CB_AddPets:
		{
			NetGroup* petGrp;
			pBuf->Read<NetGroup*>(petGrp);
			NetGroup* skillGroup;
			pBuf->Read<NetGroup*>(skillGroup);
			_result = CB_AddPets(pPeer, context, petGrp, skillGroup);
		}
		break;
	case RPC_PetGroup_CB_RemovePets:
		{
			UInt32Vector pet_ids;
			pBuf->Read<UInt32Vector>(pet_ids);
			_result = CB_RemovePets(pPeer, context, pet_ids);
		}
		break;
	case RPC_PetGroup_CB_UpdateAllPets:
		{
			NetGroup* petlGrp;
			pBuf->Read<NetGroup*>(petlGrp);
			_result = CB_UpdateAllPets(pPeer, context, petlGrp);
		}
		break;
	case RPC_PetGroup_CB_WareHouse:
		{
			UInt8 processIndex;
			pBuf->Read<UInt8>(processIndex);
			UInt8 msgIndex;
			pBuf->Read<UInt8>(msgIndex);
			_result = CB_WareHouse(pPeer, context, processIndex, msgIndex);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult PetGroup_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_PetGroup_UP_STATE:
			_result = RPCDecode<256533608>(pPeer, pBuf);
		break;
	case RPC_PetGroup_CB_Error:
			_result = RPCDecode<1109594468>(pPeer, pBuf);
		break;
	case RPC_PetGroup_CB_AddSkills:
			_result = RPCDecode<1188848268>(pPeer, pBuf);
		break;
	case RPC_PetGroup_CB_AddPets:
			_result = RPCDecode<1415327286>(pPeer, pBuf);
		break;
	case RPC_PetGroup_CB_RemovePets:
			_result = RPCDecode<2006324838>(pPeer, pBuf);
		break;
	case RPC_PetGroup_CB_UpdateAllPets:
			_result = RPCDecode<1222004327>(pPeer, pBuf);
		break;
	case RPC_PetGroup_CB_WareHouse:
			_result = RPCDecode<1412698051>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
