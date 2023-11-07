///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_RELATIONSYSTEM_STUB_H_
#define _RPC_RELATIONSYSTEM_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/RelationSystem_enum.h"

class RelationSystem_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult SetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &prefix);
	virtual RPCResult SetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &suffix);
	virtual RPCResult SelectMarriageTime(LPCPEER pPeer, RPCContext &context, const UInt32 &hour, const UInt32 &minute);
***/

protected:
	typedef NetObject Parent;

public:
	virtual RPCResult CB_RequestSetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &thirdWord);
	virtual RPCResult SetSiblingTitlePrefix(LPCPEER pPeer, RPCContext &context, const String &prefix) = 0;
	virtual RPCResult CB_RequestSetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &prefix);
	virtual RPCResult SetSiblingTitleSuffix(LPCPEER pPeer, RPCContext &context, const String &suffix) = 0;
	virtual RPCResult CB_RequestSelectMarriageTime(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult SelectMarriageTime(LPCPEER pPeer, RPCContext &context, const UInt32 &hour, const UInt32 &minute) = 0;
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
} m;

public:
	INLINE RelationSystem_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_RELATIONSYSTEM_STUB_H_
