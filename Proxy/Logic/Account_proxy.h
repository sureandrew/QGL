///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_ACCOUNT_PROXY_H_
#define _RPC_ACCOUNT_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/Account_enum.h"

class Account_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &status, const Int32Vector &lineCounts, const UInt16Vector &lineState);
	virtual RPCResult CB_GetOwnCharacters(LPCPEER pPeer, RPCContext &context, NetGroup* charInfos);
	virtual RPCResult CB_SelectCharacter(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_LogoutSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_CreateSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_RemoveCharSuccess(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CB_CheckCharacterName(LPCPEER pPeer, RPCContext &context, const UInt8 &index);
	virtual RPCResult CB_RecoverCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo);
	virtual RPCResult CB_FreezeCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo);
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt32 &acct_id, const String &loginName);
	virtual void OnUpdateAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime);
	virtual void OnUpdateBanTime(const UInt32 &banTime);
	virtual void OnUpdateAddictType(const Boolean &addictType);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		addictMask	= Parent::NextFreeMask + 1,
		banTimeMask	= Parent::NextFreeMask + 2,
		addictTypeMask	= Parent::NextFreeMask + 3,
		NextFreeMask	= Parent::NextFreeMask + 4
	};
	virtual RPCResult GetOwnCharacters(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CheckCharacterName(LPCPEER pPeer, RPCContext &context, const String &nickName);
	virtual RPCResult SelectCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id);
	virtual RPCResult Logout(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult AddCharacter(LPCPEER pPeer, RPCContext &context, const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &hairStyle, const UInt16 &hairColor, const UInt16 &clothStyle, const UInt16 &clothColor);
	virtual RPCResult RemoveCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult FreezeCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult RecoverCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg) = 0;
	virtual RPCResult CB_CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &status, const Int32Vector &lineCounts, const UInt16Vector &lineState) = 0;
	virtual RPCResult CB_GetOwnCharacters(LPCPEER pPeer, RPCContext &context, NetGroup* charInfos) = 0;
	virtual RPCResult CB_SelectCharacter(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_LogoutSuccess(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_CreateSuccess(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_RemoveCharSuccess(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult CB_CheckCharacterName(LPCPEER pPeer, RPCContext &context, const UInt8 &index) = 0;
	virtual RPCResult CB_RecoverCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo) = 0;
	virtual RPCResult CB_FreezeCharSuccess(LPCPEER pPeer, RPCContext &context, NetObject* charInfo) = 0;
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const String& GetLoginName() const { return m.loginName; }
	const UInt32& GetDisconnect_time() const { return m.disconnect_time; }
	const UInt32& GetOffLnTime() const { return m.offLnTime; }
	const UInt32& GetOnLnTime() const { return m.onLnTime; }
	const UInt32& GetBanTime() const { return m.banTime; }
	const Boolean& GetAddictType() const { return m.addictType; }
	virtual void OnUpdateInfo(const UInt32 &acct_id, const String &loginName) {};
	virtual void OnUpdateAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime) {};
	virtual void OnUpdateBanTime(const UInt32 &banTime) {};
	virtual void OnUpdateAddictType(const Boolean &addictType) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//info
	UInt32 acct_id;
	String loginName;
	//addict
	UInt32 disconnect_time;
	UInt32 offLnTime;
	UInt32 onLnTime;
	//banTime
	UInt32 banTime;
	//addictType
	Boolean addictType;
} m;

public:
	INLINE Account_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult Account_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<2014207959>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::GetOwnCharacters(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_GetOwnCharacters);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<160132587>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::CheckCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CheckCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1201502748>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::CheckCharacterName(LPCPEER pPeer, RPCContext &context, const String &nickName)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_CheckCharacterName);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(nickName);
	return RPCSend<188553919>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::SelectCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &line_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_SelectCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	pBuf->Write<UInt8>(line_id);
	return RPCSend<507605114>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::Logout(LPCPEER pPeer, RPCContext &context)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_Logout);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	return RPCSend<2091078578>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::AddCharacter(LPCPEER pPeer, RPCContext &context, const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt16 &hairStyle, const UInt16 &hairColor, const UInt16 &clothStyle, const UInt16 &clothColor)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_AddCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<String>(nickName);
	pBuf->Write<UInt8>(sex);
	pBuf->Write<UInt8>(cclass);
	pBuf->Write<UInt16>(hairStyle);
	pBuf->Write<UInt16>(hairColor);
	pBuf->Write<UInt16>(clothStyle);
	pBuf->Write<UInt16>(clothColor);
	return RPCSend<317921466>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::RemoveCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_RemoveCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<188507704>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::FreezeCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_FreezeCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1376462057>(pPeer, pBuf);
}

INLINE RPCResult Account_Proxy::RecoverCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_Account_RecoverCharacter);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
		pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<UInt32>(char_id);
	return RPCSend<1617326016>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(97800821)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(2014207959)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(160132587)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1201502748)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(188553919)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(507605114)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(2091078578)
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(317921466)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(188507704)
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1376462057)
	AES_SUB_BYTES
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_ENCRYPT_HEAD(1617326016)
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
AES_ENCRYPT_TAIL

AES_DECRYPT_HEAD(1474721487)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1339323336)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(310063790)
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1728582379)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1944146759)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1890454112)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(417215886)
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1423646894)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(237001887)
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_DECRYPT_HEAD(1536905089)
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_ACCOUNT_PROXY_H_
