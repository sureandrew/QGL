///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_ACCOUNTDATA_STUB_H_
#define _RPC_ACCOUNTDATA_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/AccountData_enum.h"

class AccountData_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void Clone(AccountData_Stub* obj, const RPCMask &mask);
	friend void AccountData_Stub::Clone(AccountData_Stub* obj, const RPCMask &mask);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE AccountData_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_ACCOUNTDATA_STUB_H_
