///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTNERCOLLECTIONDATA_STUB_H_
#define _RPC_PARTNERCOLLECTIONDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/PartnerCollectionData_enum.h"

class PartnerCollectionData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		keyMask	= Parent::NextFreeMask + 0,
		infoMask	= Parent::NextFreeMask + 1,
		NextFreeMask	= Parent::NextFreeMask + 2
	};
	const UInt32& GetCollection_uid() const { return m.collection_uid; }
	const UInt32& GetCollection_id() const { return m.collection_id; }
	const UInt32& GetChar_id() const { return m.char_id; }
	void SetKey(const UInt32 &collection_uid);
	void SetCollection_uid(const UInt32 &collection_uid);
	void SetInfo(const UInt32 &collection_id, const UInt32 &char_id);
	void SetCollection_id(const UInt32 &collection_id);
	void SetChar_id(const UInt32 &char_id);
	void Clone(PartnerCollectionData_Stub* obj, const RPCMask &mask);
	friend void PartnerCollectionData_Stub::Clone(PartnerCollectionData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//key
	UInt32 collection_uid;
	//info
	UInt32 collection_id;
	UInt32 char_id;
} m;

public:
	INLINE PartnerCollectionData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PARTNERCOLLECTIONDATA_STUB_H_
