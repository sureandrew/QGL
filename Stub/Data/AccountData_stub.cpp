//-- Common
#include "Common.h"
//-- Self
#include "Stub/Data/AccountData_stub.h"

void AccountData_Stub::SetInfo(const UInt32 &acct_id, const String &loginName, const ByteArray &password_hash)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.loginName != loginName) { m.loginName = loginName; modified = true; }
	if (m.password_hash != password_hash) { m.password_hash = password_hash; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Stub::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Stub::SetLoginName(const String &loginName)
{
	bool modified = false;
	if (m.loginName != loginName) { m.loginName = loginName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Stub::SetPassword_hash(const ByteArray &password_hash)
{
	bool modified = false;
	if (m.password_hash != password_hash) { m.password_hash = password_hash; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Stub::SetAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime)
{
	bool modified = false;
	if (m.disconnect_time != disconnect_time) { m.disconnect_time = disconnect_time; modified = true; }
	if (m.offLnTime != offLnTime) { m.offLnTime = offLnTime; modified = true; }
	if (m.onLnTime != onLnTime) { m.onLnTime = onLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Stub::SetDisconnect_time(const UInt32 &disconnect_time)
{
	bool modified = false;
	if (m.disconnect_time != disconnect_time) { m.disconnect_time = disconnect_time; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Stub::SetOffLnTime(const UInt32 &offLnTime)
{
	bool modified = false;
	if (m.offLnTime != offLnTime) { m.offLnTime = offLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Stub::SetOnLnTime(const UInt32 &onLnTime)
{
	bool modified = false;
	if (m.onLnTime != onLnTime) { m.onLnTime = onLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Stub::SetYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	if (m.yuanBaoTicket != yuanBaoTicket) { m.yuanBaoTicket = yuanBaoTicket; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void AccountData_Stub::SetYuanBao(const UInt32 &yuanBao)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void AccountData_Stub::SetYuanBaoTicket(const UInt32 &yuanBaoTicket)
{
	bool modified = false;
	if (m.yuanBaoTicket != yuanBaoTicket) { m.yuanBaoTicket = yuanBaoTicket; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void AccountData_Stub::SetBanTime(const UInt32 &banTime)
{
	bool modified = false;
	if (m.banTime != banTime) { m.banTime = banTime; modified = true; }
	MarkUpdateAll(banTimeMask, modified);
}

void AccountData_Stub::SetAddictType(const Boolean &addictType)
{
	bool modified = false;
	if (m.addictType != addictType) { m.addictType = addictType; modified = true; }
	MarkUpdateAll(addictTypeMask, modified);
}

void AccountData_Stub::SetYuanBaoLock(const Boolean &yuanBaoLock)
{
	bool modified = false;
	if (m.yuanBaoLock != yuanBaoLock) { m.yuanBaoLock = yuanBaoLock; modified = true; }
	MarkUpdateAll(yuanBaoLockMask, modified);
}

void AccountData_Stub::SetAssetLockPassword(const ByteArray &lockPassword)
{
	bool modified = false;
	if (m.lockPassword != lockPassword) { m.lockPassword = lockPassword; modified = true; }
	MarkUpdateAll(assetLockPasswordMask, modified);
}

void AccountData_Stub::SetLockPassword(const ByteArray &lockPassword)
{
	bool modified = false;
	if (m.lockPassword != lockPassword) { m.lockPassword = lockPassword; modified = true; }
	MarkUpdateAll(assetLockPasswordMask, modified);
}

void AccountData_Stub::SetUnlockTime(const UInt32 &unlock_time)
{
	bool modified = false;
	if (m.unlock_time != unlock_time) { m.unlock_time = unlock_time; modified = true; }
	MarkUpdateAll(unlockTimeMask, modified);
}

void AccountData_Stub::SetUnlock_time(const UInt32 &unlock_time)
{
	bool modified = false;
	if (m.unlock_time != unlock_time) { m.unlock_time = unlock_time; modified = true; }
	MarkUpdateAll(unlockTimeMask, modified);
}

void AccountData_Stub::SetInactive(const Boolean &inactive)
{
	bool modified = false;
	if (m.inactive != inactive) { m.inactive = inactive; modified = true; }
	MarkUpdateAll(inactiveMask, modified);
}


void AccountData_Stub::Clone(AccountData_Stub* obj, const RPCMask &mask)
{
	if (obj == NULL) return;

	Parent::Clone(obj, mask);
	if (mask.IsBit(infoMask) && !(obj->m.acct_id == m.acct_id)) {
		m.acct_id = obj->m.acct_id;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.loginName == m.loginName)) {
		m.loginName = obj->m.loginName;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(infoMask) && !(obj->m.password_hash == m.password_hash)) {
		m.password_hash = obj->m.password_hash;
		MarkUpdateAll(infoMask, true);
	}
	if (mask.IsBit(addictMask) && !(obj->m.disconnect_time == m.disconnect_time)) {
		m.disconnect_time = obj->m.disconnect_time;
		MarkUpdateAll(addictMask, true);
	}
	if (mask.IsBit(addictMask) && !(obj->m.offLnTime == m.offLnTime)) {
		m.offLnTime = obj->m.offLnTime;
		MarkUpdateAll(addictMask, true);
	}
	if (mask.IsBit(addictMask) && !(obj->m.onLnTime == m.onLnTime)) {
		m.onLnTime = obj->m.onLnTime;
		MarkUpdateAll(addictMask, true);
	}
	if (mask.IsBit(yuanBaoMask) && !(obj->m.yuanBao == m.yuanBao)) {
		m.yuanBao = obj->m.yuanBao;
		MarkUpdateAll(yuanBaoMask, true);
	}
	if (mask.IsBit(yuanBaoMask) && !(obj->m.yuanBaoTicket == m.yuanBaoTicket)) {
		m.yuanBaoTicket = obj->m.yuanBaoTicket;
		MarkUpdateAll(yuanBaoMask, true);
	}
	if (mask.IsBit(banTimeMask) && !(obj->m.banTime == m.banTime)) {
		m.banTime = obj->m.banTime;
		MarkUpdateAll(banTimeMask, true);
	}
	if (mask.IsBit(addictTypeMask) && !(obj->m.addictType == m.addictType)) {
		m.addictType = obj->m.addictType;
		MarkUpdateAll(addictTypeMask, true);
	}
	if (mask.IsBit(yuanBaoLockMask) && !(obj->m.yuanBaoLock == m.yuanBaoLock)) {
		m.yuanBaoLock = obj->m.yuanBaoLock;
		MarkUpdateAll(yuanBaoLockMask, true);
	}
	if (mask.IsBit(assetLockPasswordMask) && !(obj->m.lockPassword == m.lockPassword)) {
		m.lockPassword = obj->m.lockPassword;
		MarkUpdateAll(assetLockPasswordMask, true);
	}
	if (mask.IsBit(unlockTimeMask) && !(obj->m.unlock_time == m.unlock_time)) {
		m.unlock_time = obj->m.unlock_time;
		MarkUpdateAll(unlockTimeMask, true);
	}
	if (mask.IsBit(inactiveMask) && !(obj->m.inactive == m.inactive)) {
		m.inactive = obj->m.inactive;
		MarkUpdateAll(inactiveMask, true);
	}
}

void AccountData_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
	if (m_updateMask.IsBit(infoMask))
	{
		buf.Read<UInt32>(m.acct_id);
		buf.Read<String>(m.loginName);
		buf.Read<ByteArray>(m.password_hash);
	}
	if (m_updateMask.IsBit(addictMask))
	{
		buf.Read<UInt32>(m.disconnect_time);
		buf.Read<UInt32>(m.offLnTime);
		buf.Read<UInt32>(m.onLnTime);
	}
	if (m_updateMask.IsBit(yuanBaoMask))
	{
		buf.Read<UInt32>(m.yuanBao);
		buf.Read<UInt32>(m.yuanBaoTicket);
	}
	if (m_updateMask.IsBit(banTimeMask))
	{
		buf.Read<UInt32>(m.banTime);
	}
	if (m_updateMask.IsBit(addictTypeMask))
	{
		buf.Read<Boolean>(m.addictType);
	}
	if (m_updateMask.IsBit(yuanBaoLockMask))
	{
		buf.Read<Boolean>(m.yuanBaoLock);
	}
	if (m_updateMask.IsBit(assetLockPasswordMask))
	{
		buf.Read<ByteArray>(m.lockPassword);
	}
	if (m_updateMask.IsBit(unlockTimeMask))
	{
		buf.Read<UInt32>(m.unlock_time);
	}
	if (m_updateMask.IsBit(inactiveMask))
	{
		buf.Read<Boolean>(m.inactive);
	}
}

void AccountData_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(infoMask))
	{
		buf.Write<UInt32>(m.acct_id);
		buf.Write<String>(m.loginName);
		buf.Write<ByteArray>(m.password_hash);
	}
	if (mask.IsBit(addictMask))
	{
		buf.Write<UInt32>(m.disconnect_time);
		buf.Write<UInt32>(m.offLnTime);
		buf.Write<UInt32>(m.onLnTime);
	}
	if (mask.IsBit(yuanBaoMask))
	{
		buf.Write<UInt32>(m.yuanBao);
		buf.Write<UInt32>(m.yuanBaoTicket);
	}
	if (mask.IsBit(banTimeMask))
	{
		buf.Write<UInt32>(m.banTime);
	}
	if (mask.IsBit(addictTypeMask))
	{
		buf.Write<Boolean>(m.addictType);
	}
	if (mask.IsBit(yuanBaoLockMask))
	{
		buf.Write<Boolean>(m.yuanBaoLock);
	}
	if (mask.IsBit(assetLockPasswordMask))
	{
		buf.Write<ByteArray>(m.lockPassword);
	}
	if (mask.IsBit(unlockTimeMask))
	{
		buf.Write<UInt32>(m.unlock_time);
	}
	if (mask.IsBit(inactiveMask))
	{
		buf.Write<Boolean>(m.inactive);
	}
}

RPCResult AccountData_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_AccountData_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult AccountData_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_AccountData_REQ_STATE:
		{
			RPCMask mask;
			pBuf->Read<RPCMask>(mask);
			_result = SendAttr(pPeer, context, mask);
		}
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}
#pragma optimize("", on)

