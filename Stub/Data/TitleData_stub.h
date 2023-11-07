///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_TITLEDATA_STUB_H_
#define _RPC_TITLEDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/TitleData_enum.h"

class TitleData_Stub : public NetObject
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
		ownerMask	= Parent::NextFreeMask + 2,
		expireDateMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt32& GetTitle_uid() const { return m.title_uid; }
	const UInt16& GetTitle_id() const { return m.title_id; }
	const UInt32& GetOwner_id() const { return m.owner_id; }
	const UInt32& GetExpireDate() const { return m.expireDate; }
	void SetKey(const UInt32 &title_uid);
	void SetTitle_uid(const UInt32 &title_uid);
	void SetInfo(const UInt16 &title_id);
	void SetTitle_id(const UInt16 &title_id);
	void SetOwner(const UInt32 &owner_id);
	void SetOwner_id(const UInt32 &owner_id);
	void SetExpireDate(const UInt32 &expireDate);
	void Clone(TitleData_Stub* obj, const RPCMask &mask);
	friend void TitleData_Stub::Clone(TitleData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//key
	UInt32 title_uid;
	//info
	UInt16 title_id;
	//owner
	UInt32 owner_id;
	//expireDate
	UInt32 expireDate;
} m;

public:
	INLINE TitleData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_TITLEDATA_STUB_H_
