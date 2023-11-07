///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_GUILDMEMBERDATA_PROXY_H_
#define _RPC_GUILDMEMBERDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/GuildMemberData_enum.h"

class GuildMemberData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateKey(const UInt32 &char_id, const UInt32 &guild_uid);
	virtual void OnUpdateDonate(const UInt32 &donate);
	virtual void OnUpdateTotalDonate(const UInt32 &totalDonate);
	virtual void OnUpdatePost(const UInt8 &post);
	virtual void OnUpdateJoinDate(const UInt32 &joinDate);
	virtual void OnUpdateBusiness(const UInt16 &businessCount, const UInt32 &businessProfit);
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
	virtual void OnUpdateKey(const UInt32 &char_id, const UInt32 &guild_uid) {};
	virtual void OnUpdateDonate(const UInt32 &donate) {};
	virtual void OnUpdateTotalDonate(const UInt32 &totalDonate) {};
	virtual void OnUpdatePost(const UInt8 &post) {};
	virtual void OnUpdateJoinDate(const UInt32 &joinDate) {};
	virtual void OnUpdateBusiness(const UInt16 &businessCount, const UInt32 &businessProfit) {};
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
	//business
	UInt16 businessCount;
	UInt32 businessProfit;
} m;

public:
	INLINE GuildMemberData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult GuildMemberData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_GuildMemberData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<22637123>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(759827620)
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(22637123)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_GUILDMEMBERDATA_PROXY_H_
