///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_ACCOUNTDATA_PROXY_H_
#define _RPC_ACCOUNTDATA_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/AccountData_enum.h"

class AccountData_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateInfo(const UInt32 &acct_id, const String &loginName, const ByteArray &password_hash);
	virtual void OnUpdateAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime);
	virtual void OnUpdateYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket);
	virtual void OnUpdateBanTime(const UInt32 &banTime);
	virtual void OnUpdateAddictType(const Boolean &addictType);
	virtual void OnUpdateYuanBaoLock(const Boolean &yuanBaoLock);
	virtual void OnUpdateAssetLockPassword(const ByteArray &lockPassword);
	virtual void OnUpdateUnlockTime(const UInt32 &unlock_time);
	virtual void OnUpdateInactive(const Boolean &inactive);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		infoMask	= Parent::NextFreeMask + 0,
		addictMask	= Parent::NextFreeMask + 1,
		yuanBaoMask	= Parent::NextFreeMask + 2,
		banTimeMask	= Parent::NextFreeMask + 3,
		addictTypeMask	= Parent::NextFreeMask + 4,
		yuanBaoLockMask	= Parent::NextFreeMask + 5,
		assetLockPasswordMask	= Parent::NextFreeMask + 6,
		unlockTimeMask	= Parent::NextFreeMask + 7,
		inactiveMask	= Parent::NextFreeMask + 8,
		NextFreeMask	= Parent::NextFreeMask + 9
	};
	const UInt32& GetAcct_id() const { return m.acct_id; }
	const String& GetLoginName() const { return m.loginName; }
	const ByteArray& GetPassword_hash() const { return m.password_hash; }
	const UInt32& GetDisconnect_time() const { return m.disconnect_time; }
	const UInt32& GetOffLnTime() const { return m.offLnTime; }
	const UInt32& GetOnLnTime() const { return m.onLnTime; }
	const UInt32& GetYuanBao() const { return m.yuanBao; }
	const UInt32& GetYuanBaoTicket() const { return m.yuanBaoTicket; }
	const UInt32& GetBanTime() const { return m.banTime; }
	const Boolean& GetAddictType() const { return m.addictType; }
	const Boolean& GetYuanBaoLock() const { return m.yuanBaoLock; }
	const ByteArray& GetLockPassword() const { return m.lockPassword; }
	const UInt32& GetUnlock_time() const { return m.unlock_time; }
	const Boolean& GetInactive() const { return m.inactive; }
	virtual void OnUpdateInfo(const UInt32 &acct_id, const String &loginName, const ByteArray &password_hash) {};
	virtual void OnUpdateAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime) {};
	virtual void OnUpdateYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket) {};
	virtual void OnUpdateBanTime(const UInt32 &banTime) {};
	virtual void OnUpdateAddictType(const Boolean &addictType) {};
	virtual void OnUpdateYuanBaoLock(const Boolean &yuanBaoLock) {};
	virtual void OnUpdateAssetLockPassword(const ByteArray &lockPassword) {};
	virtual void OnUpdateUnlockTime(const UInt32 &unlock_time) {};
	virtual void OnUpdateInactive(const Boolean &inactive) {};
	void SetInfo(const UInt32 &acct_id, const String &loginName, const ByteArray &password_hash);
	void SetAcct_id(const UInt32 &acct_id);
	void SetLoginName(const String &loginName);
	void SetPassword_hash(const ByteArray &password_hash);
	void SetAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime);
	void SetDisconnect_time(const UInt32 &disconnect_time);
	void SetOffLnTime(const UInt32 &offLnTime);
	void SetOnLnTime(const UInt32 &onLnTime);
	void SetYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket);
	void SetYuanBao(const UInt32 &yuanBao);
	void SetYuanBaoTicket(const UInt32 &yuanBaoTicket);
	void SetBanTime(const UInt32 &banTime);
	void SetAddictType(const Boolean &addictType);
	void SetYuanBaoLock(const Boolean &yuanBaoLock);
	void SetAssetLockPassword(const ByteArray &lockPassword);
	void SetLockPassword(const ByteArray &lockPassword);
	void SetUnlockTime(const UInt32 &unlock_time);
	void SetUnlock_time(const UInt32 &unlock_time);
	void SetInactive(const Boolean &inactive);
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
	ByteArray password_hash;
	//addict
	UInt32 disconnect_time;
	UInt32 offLnTime;
	UInt32 onLnTime;
	//yuanBao
	UInt32 yuanBao;
	UInt32 yuanBaoTicket;
	//banTime
	UInt32 banTime;
	//addictType
	Boolean addictType;
	//yuanBaoLock
	Boolean yuanBaoLock;
	//assetLockPassword
	ByteArray lockPassword;
	//unlockTime
	UInt32 unlock_time;
	//inactive
	Boolean inactive;
} m;

public:
	INLINE AccountData_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult AccountData_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_AccountData_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<174461286>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1209368035)
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(174461286)
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_ACCOUNTDATA_PROXY_H_
