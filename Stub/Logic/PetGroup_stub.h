///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PETGROUP_STUB_H_
#define _RPC_PETGROUP_STUB_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/PetGroup_enum.h"

class PetGroup_Stub : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult DeletePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid);
	virtual RPCResult ChangePetState(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid, const UInt8 &state);
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID);
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID);
***/

protected:
	typedef NetGroup Parent;

public:
	enum UpdateMask {
		ownerMask	= Parent::NextFreeMask + 0,
		NextFreeMask	= Parent::NextFreeMask + 1
	};
	virtual RPCResult DeletePet(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid) = 0;
	virtual RPCResult ChangePetState(LPCPEER pPeer, RPCContext &context, const UInt32 &pet_uid, const UInt8 &state) = 0;
	virtual RPCResult PutToWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID) = 0;
	virtual RPCResult GetFromWareHouse(LPCPEER pPeer, RPCContext &context, const UInt32 &petUID) = 0;
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_AddSkills(LPCPEER pPeer, RPCContext &context, NetGroup* skillGrp);
	virtual RPCResult CB_AddPets(LPCPEER pPeer, RPCContext &context, NetGroup* petGrp, NetGroup* skillGroup);
	virtual RPCResult CB_RemovePets(LPCPEER pPeer, RPCContext &context, const UInt32Vector &pet_ids);
	virtual RPCResult CB_UpdateAllPets(LPCPEER pPeer, RPCContext &context, NetGroup* petlGrp);
	virtual RPCResult CB_WareHouse(LPCPEER pPeer, RPCContext &context, const UInt8 &processIndex, const UInt8 &msgIndex);
	const UInt32& GetOwner_id() const { return m.owner_id; }
	void SetOwner(const UInt32 &owner_id);
	void SetOwner_id(const UInt32 &owner_id);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//owner
	UInt32 owner_id;
} m;

public:
	INLINE PetGroup_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PETGROUP_STUB_H_
