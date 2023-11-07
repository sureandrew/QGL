///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_MARRIAGEDATA_STUB_H_
#define _RPC_MARRIAGEDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/MarriageData_enum.h"

class MarriageData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		marriage_idMask	= Parent::NextFreeMask + 0,
		husband_idMask	= Parent::NextFreeMask + 1,
		wife_idMask	= Parent::NextFreeMask + 2,
		marriagetimeMask	= Parent::NextFreeMask + 3,
		stateMask	= Parent::NextFreeMask + 4,
		NextFreeMask	= Parent::NextFreeMask + 5
	};
	const UInt32& GetMarriage_id() const { return m.marriage_id; }
	const UInt32& GetHusband_id() const { return m.husband_id; }
	const UInt32& GetWife_id() const { return m.wife_id; }
	const UInt32& GetMarriage_time() const { return m.marriage_time; }
	const UInt32& GetState() const { return m.state; }
	void SetMarriage_id(const UInt32 &marriage_id);
	void SetHusband_id(const UInt32 &husband_id);
	void SetWife_id(const UInt32 &wife_id);
	void SetMarriagetime(const UInt32 &marriage_time);
	void SetMarriage_time(const UInt32 &marriage_time);
	void SetState(const UInt32 &state);
	void Clone(MarriageData_Stub* obj, const RPCMask &mask);
	friend void MarriageData_Stub::Clone(MarriageData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//marriage_id
	UInt32 marriage_id;
	//husband_id
	UInt32 husband_id;
	//wife_id
	UInt32 wife_id;
	//marriagetime
	UInt32 marriage_time;
	//state
	UInt32 state;
} m;

public:
	INLINE MarriageData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_MARRIAGEDATA_STUB_H_
