///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_RANKGUILD_STUB_H_
#define _RPC_RANKGUILD_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/RankGuild_enum.h"

class RankGuild_Stub : public NetObject
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
		masterMask	= Parent::NextFreeMask + 2,
		guildPvPScoreMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	const UInt32& GetGuild_uid() const { return m.guild_uid; }
	const String& GetName() const { return m.name; }
	const String& GetMaster() const { return m.master; }
	const UInt32& GetGuildPvPScore() const { return m.guildPvPScore; }
	void SetId(const UInt32 &guild_uid);
	void SetGuild_uid(const UInt32 &guild_uid);
	void SetName(const String &name);
	void SetMaster(const String &master);
	void SetGuildPvPScore(const UInt32 &guildPvPScore);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 guild_uid;
	//name
	String name;
	//master
	String master;
	//guildPvPScore
	UInt32 guildPvPScore;
} m;

public:
	INLINE RankGuild_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_RANKGUILD_STUB_H_
