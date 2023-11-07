///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILDAPPLICANT_STUB_H_
#define _RPC_GUILDAPPLICANT_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GuildApplicant_enum.h"

class GuildApplicant_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		nameMask	= Parent::NextFreeMask + 1,
		applyDateMask	= Parent::NextFreeMask + 2,
		levelMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt32& GetChar_id() const { return m.char_id; }
	const String& GetName() const { return m.name; }
	const UInt32& GetApplyDate() const { return m.ApplyDate; }
	const UInt16& GetLevel() const { return m.level; }
	void SetId(const UInt32 &char_id);
	void SetChar_id(const UInt32 &char_id);
	void SetName(const String &name);
	void SetApplyDate(const UInt32 &ApplyDate);
	void SetLevel(const UInt16 &level);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 char_id;
	//name
	String name;
	//applyDate
	UInt32 ApplyDate;
	//level
	UInt16 level;
} m;

public:
	INLINE GuildApplicant_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_GUILDAPPLICANT_STUB_H_
