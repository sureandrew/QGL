///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILDMEMBERDATA_STUB_H_
#define _RPC_GUILDMEMBERDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GuildMemberData_enum.h"

class GuildMemberData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		keyMask	= Parent::NextFreeMask + 0,
		donateMask	= Parent::NextFreeMask + 1,
		totalDonateMask	= Parent::NextFreeMask + 2,
		postMask	= Parent::NextFreeMask + 3,
		joinDateMask	= Parent::NextFreeMask + 4,
		businessMask	= Parent::NextFreeMask + 5,
		NextFreeMask	= Parent::NextFreeMask + 6
	};
	const UInt32& GetChar_id() const { return m.char_id; }
	const UInt32& GetGuild_uid() const { return m.guild_uid; }
	const UInt32& GetDonate() const { return m.donate; }
	const UInt32& GetTotalDonate() const { return m.totalDonate; }
	const UInt8& GetPost() const { return m.post; }
	const UInt32& GetJoinDate() const { return m.joinDate; }
	const UInt16& GetBusinessCount() const { return m.businessCount; }
	const UInt32& GetBusinessProfit() const { return m.businessProfit; }
	void SetKey(const UInt32 &char_id, const UInt32 &guild_uid);
	void SetChar_id(const UInt32 &char_id);
	void SetGuild_uid(const UInt32 &guild_uid);
	void SetDonate(const UInt32 &donate);
	void SetTotalDonate(const UInt32 &totalDonate);
	void SetPost(const UInt8 &post);
	void SetJoinDate(const UInt32 &joinDate);
	void SetBusiness(const UInt16 &businessCount, const UInt32 &businessProfit);
	void SetBusinessCount(const UInt16 &businessCount);
	void SetBusinessProfit(const UInt32 &businessProfit);
	void Clone(GuildMemberData_Stub* obj, const RPCMask &mask);
	friend void GuildMemberData_Stub::Clone(GuildMemberData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//key
	UInt32 char_id;
	UInt32 guild_uid;
	//donate
	UInt32 donate;
	//totalDonate
	UInt32 totalDonate;
	//post
	UInt8 post;
	//joinDate
	UInt32 joinDate;
	//business
	UInt16 businessCount;
	UInt32 businessProfit;
} m;

public:
	INLINE GuildMemberData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_GUILDMEMBERDATA_STUB_H_
