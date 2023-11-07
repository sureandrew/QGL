///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTNERGROUP_STUB_H_
#define _RPC_PARTNERGROUP_STUB_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/PartnerGroup_enum.h"

class PartnerGroup_Stub : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult DeletePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id);
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt8 &state);
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID);
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID);
	virtual RPCResult PartnerCultivationUpByPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_partnerID);
	virtual RPCResult PartnerCultivationUpByItem(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_itemUid);
***/

protected:
	typedef NetGroup Parent;

public:
	virtual RPCResult DeletePartner(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id) = 0;
	virtual RPCResult ChangePartnerState(LPCPEER pPeer, RPCContext &context, const UInt32 &partner_id, const UInt8 &state) = 0;
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID) = 0;
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &partnerID) = 0;
	virtual RPCResult PartnerCultivationUpByPartner(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_partnerID) = 0;
	virtual RPCResult PartnerCultivationUpByItem(LPCPEER pPeer, RPCContext &context, const UInt32 &src_partnerID, const UInt32 &dest_itemUid) = 0;
	virtual RPCResult CB_DeleteListSuccess(LPCPEER pPeer, RPCContext &context, const UInt32Vector &partnerIDs, const bool &showDelMsg);
	virtual RPCResult CB_AddPartnerGroup(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup, NetGroup* skillGroups);
	virtual RPCResult CB_ChangePartnerState(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_AddPartner(LPCPEER pPeer, RPCContext &context, NetObject* partner, NetGroup* skillGroup);
	virtual RPCResult CB_UpdateAllPartner(LPCPEER pPeer, RPCContext &context, NetGroup* partnerGroup);
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	virtual RPCResult CB_PartnerCultivationUp(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const UInt8 &msgValue, const UInt32 &src_partnerID);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE PartnerGroup_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PARTNERGROUP_STUB_H_
