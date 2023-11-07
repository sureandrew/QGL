///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILDMEMBER_PROXY_H_
#define _RPC_GUILDMEMBER_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GuildMember_enum.h"

class GuildMember_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateKey(const UInt32 &char_id, const UInt32 &guild_uid);
	virtual void OnUpdateDonate(const UInt32 &donate);
	virtual void OnUpdateTotalDonate(const UInt32 &totalDonate);
	virtual void OnUpdatePost(const UInt8 &post);
	virtual void OnUpdateJoinDate(const UInt32 &joinDate);
	virtual void OnUpdateName(const String &name);
	virtual void OnUpdateSexClass(const UInt8 &sexClass);
	virtual void OnUpdateLevel(const UInt16 &level);
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime);
	virtual void OnUpdateBusiness(const UInt16 &businessCount, const UInt32 &businessProfit);
	virtual void OnUpdateInactive(const Boolean &inactive);
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
	virtual RPCResult ResponseApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const bool &accept);
	virtual RPCResult PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &newPost);
	virtual RPCResult ChangeAim(LPCPEER pPeer, RPCContext &context, const String &text);
	virtual RPCResult ChangeRule(LPCPEER pPeer, RPCContext &context, const String &text);
	virtual RPCResult ClearGuildBusinessLog(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg) = 0;
	virtual RPCResult CB_PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &src, const UInt32 &target, const UInt8 &position) = 0;
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
	virtual void OnUpdateKey(const UInt32 &char_id, const UInt32 &guild_uid) {};
	virtual void OnUpdateDonate(const UInt32 &donate) {};
	virtual void OnUpdateTotalDonate(const UInt32 &totalDonate) {};
	virtual void OnUpdatePost(const UInt8 &post) {};
	virtual void OnUpdateJoinDate(const UInt32 &joinDate) {};
	virtual void OnUpdateName(const String &name) {};
	virtual void OnUpdateSexClass(const UInt8 &sexClass) {};
	virtual void OnUpdateLevel(const UInt16 &level) {};
	virtual void OnUpdateOfflineTime(const UInt32 &offlineTime) {};
	virtual void OnUpdateBusiness(const UInt16 &businessCount, const UInt32 &businessProfit) {};
	virtual void OnUpdateInactive(const Boolean &inactive) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

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
	INLINE GuildMember_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult GuildMember_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<2085039862>(pPeer, pBuf);
}

INLINE RPCResult GuildMember_Proxy::ResponseApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const bool &accept)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_ResponseApplicant);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<bool>(accept);
	return RPCSend<1299078484>(pPeer, pBuf);
}

INLINE RPCResult GuildMember_Proxy::PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &newPost)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_PositionChange);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(newPost);
	return RPCSend<254953419>(pPeer, pBuf);
}

INLINE RPCResult GuildMember_Proxy::ChangeAim(LPCPEER pPeer, RPCContext &context, const String &text)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_ChangeAim);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(text);
	return RPCSend<832781680>(pPeer, pBuf);
}

INLINE RPCResult GuildMember_Proxy::ChangeRule(LPCPEER pPeer, RPCContext &context, const String &text)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_ChangeRule);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(text);
	return RPCSend<971653655>(pPeer, pBuf);
}

INLINE RPCResult GuildMember_Proxy::ClearGuildBusinessLog(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMember_ClearGuildBusinessLog);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<2104772248>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1010581609)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2085039862)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1299078484)
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(254953419)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(832781680)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(971653655)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2104772248)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(986982435)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(944125522)
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_GUILDMEMBER_PROXY_H_
