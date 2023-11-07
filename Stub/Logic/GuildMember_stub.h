///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILDMEMBER_STUB_H_
#define _RPC_GUILDMEMBER_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GuildMember_enum.h"

class GuildMember_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult ResponseApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const bool &accept);
	virtual RPCResult PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &newPost);
	virtual RPCResult ChangeAim(LPCPEER pPeer, RPCContext &context, const String &text);
	virtual RPCResult ChangeRule(LPCPEER pPeer, RPCContext &context, const String &text);
	virtual RPCResult ClearGuildBusinessLog(LPCPEER pPeer, RPCContext &context);
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
		nameMask	= Parent::NextFreeMask + 5,
		sexClassMask	= Parent::NextFreeMask + 6,
		levelMask	= Parent::NextFreeMask + 7,
		offlineTimeMask	= Parent::NextFreeMask + 8,
		businessMask	= Parent::NextFreeMask + 9,
		inactiveMask	= Parent::NextFreeMask + 10,
		NextFreeMask	= Parent::NextFreeMask + 11
	};
	virtual RPCResult ResponseApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const bool &accept) = 0;
	virtual RPCResult PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &newPost) = 0;
	virtual RPCResult ChangeAim(LPCPEER pPeer, RPCContext &context, const String &text) = 0;
	virtual RPCResult ChangeRule(LPCPEER pPeer, RPCContext &context, const String &text) = 0;
	virtual RPCResult ClearGuildBusinessLog(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position);
	const UInt32& GetChar_id() const { return m.char_id; }
	const UInt32& GetGuild_uid() const { return m.guild_uid; }
	const UInt32& GetDonate() const { return m.donate; }
	const UInt32& GetTotalDonate() const { return m.totalDonate; }
	const UInt8& GetPost() const { return m.post; }
	const UInt32& GetJoinDate() const { return m.joinDate; }
	const String& GetName() const { return m.name; }
	const UInt8& GetSexClass() const { return m.sexClass; }
	const UInt16& GetLevel() const { return m.level; }
	const UInt32& GetOfflineTime() const { return m.offlineTime; }
	const UInt16& GetBusinessCount() const { return m.businessCount; }
	const UInt32& GetBusinessProfit() const { return m.businessProfit; }
	const Boolean& GetInactive() const { return m.inactive; }
	void SetKey(const UInt32 &char_id, const UInt32 &guild_uid);
	void SetChar_id(const UInt32 &char_id);
	void SetGuild_uid(const UInt32 &guild_uid);
	void SetDonate(const UInt32 &donate);
	void SetTotalDonate(const UInt32 &totalDonate);
	void SetPost(const UInt8 &post);
	void SetJoinDate(const UInt32 &joinDate);
	void SetName(const String &name);
	void SetSexClass(const UInt8 &sexClass);
	void SetLevel(const UInt16 &level);
	void SetOfflineTime(const UInt32 &offlineTime);
	void SetBusiness(const UInt16 &businessCount, const UInt32 &businessProfit);
	void SetBusinessCount(const UInt16 &businessCount);
	void SetBusinessProfit(const UInt32 &businessProfit);
	void SetInactive(const Boolean &inactive);
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
	//name
	String name;
	//sexClass
	UInt8 sexClass;
	//level
	UInt16 level;
	//offlineTime
	UInt32 offlineTime;
	//business
	UInt16 businessCount;
	UInt32 businessProfit;
	//inactive
	Boolean inactive;
} m;

public:
	INLINE GuildMember_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_GUILDMEMBER_STUB_H_
