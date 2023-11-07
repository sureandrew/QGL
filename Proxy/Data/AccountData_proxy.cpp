//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Data/AccountData_proxy.h"

void AccountData_Proxy::SetInfo(const UInt32 &acct_id, const String &loginName, const ByteArray &password_hash)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	if (m.loginName != loginName) { m.loginName = loginName; modified = true; }
	if (m.password_hash != password_hash) { m.password_hash = password_hash; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Proxy::SetAcct_id(const UInt32 &acct_id)
{
	bool modified = false;
	if (m.acct_id != acct_id) { m.acct_id = acct_id; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Proxy::SetLoginName(const String &loginName)
{
	bool modified = false;
	if (m.loginName != loginName) { m.loginName = loginName; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Proxy::SetPassword_hash(const ByteArray &password_hash)
{
	bool modified = false;
	if (m.password_hash != password_hash) { m.password_hash = password_hash; modified = true; }
	MarkUpdateAll(infoMask, modified);
}

void AccountData_Proxy::SetAddict(const UInt32 &disconnect_time, const UInt32 &offLnTime, const UInt32 &onLnTime)
{
	bool modified = false;
	if (m.disconnect_time != disconnect_time) { m.disconnect_time = disconnect_time; modified = true; }
	if (m.offLnTime != offLnTime) { m.offLnTime = offLnTime; modified = true; }
	if (m.onLnTime != onLnTime) { m.onLnTime = onLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Proxy::SetDisconnect_time(const UInt32 &disconnect_time)
{
	bool modified = false;
	if (m.disconnect_time != disconnect_time) { m.disconnect_time = disconnect_time; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Proxy::SetOffLnTime(const UInt32 &offLnTime)
{
	bool modified = false;
	if (m.offLnTime != offLnTime) { m.offLnTime = offLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Proxy::SetOnLnTime(const UInt32 &onLnTime)
{
	bool modified = false;
	if (m.onLnTime != onLnTime) { m.onLnTime = onLnTime; modified = true; }
	MarkUpdateAll(addictMask, modified);
}

void AccountData_Proxy::SetYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	if (m.yuanBaoTicket != yuanBaoTicket) { m.yuanBaoTicket = yuanBaoTicket; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void AccountData_Proxy::SetYuanBao(const UInt32 &yuanBao)
{
	bool modified = false;
	if (m.yuanBao != yuanBao) { m.yuanBao = yuanBao; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void AccountData_Proxy::SetYuanBaoTicket(const UInt32 &yuanBaoTicket)
{
	bool modified = false;
	if (m.yuanBaoTicket != yuanBaoTicket) { m.yuanBaoTicket = yuanBaoTicket; modified = true; }
	MarkUpdateAll(yuanBaoMask, modified);
}

void AccountData_Proxy::SetBanTime(const UInt32 &banTime)
{
	bool modified = false;
	if (m.banTime != banTime) { m.banTime = banTime; modified = true; }
	MarkUpdateAll(banTimeMask, modified);
}

void AccountData_Proxy::SetAddictType(const Boolean &addictType)
{
	bool modified = false;
	if (m.addictType != addictType) { m.addictType = addictType; modified = true; }
	MarkUpdateAll(addictTypeMask, modified);
}

void AccountData_Proxy::SetYuanBaoLock(const Boolean &yuanBaoLock)
{
	bool modified = false;
	if (m.yuanBaoLock != yuanBaoLock) { m.yuanBaoLock = yuanBaoLock; modified = true; }
	MarkUpdateAll(yuanBaoLockMask, modified);
}

void AccountData_Proxy::SetAssetLockPassword(const ByteArray &lockPassword)
{
	bool modified = false;
	if (m.lockPassword != lockPassword) { m.lockPassword = lockPassword; modified = true; }
	MarkUpdateAll(assetLockPasswordMask, modified);
}

void AccountData_Proxy::SetLockPassword(const ByteArray &lockPassword)
{
	bool modified = false;
	if (m.lockPassword != lockPassword) { m.lockPassword = lockPassword; modified = true; }
	MarkUpdateAll(assetLockPasswordMask, modified);
}

void AccountData_Proxy::SetUnlockTime(const UInt32 &unlock_time)
{
	bool modified = false;
	if (m.unlock_time != unlock_time) { m.unlock_time = unlock_time; modified = true; }
	MarkUpdateAll(unlockTimeMask, modified);
}

void AccountData_Proxy::SetUnlock_time(const UInt32 &unlock_time)
{
	bool modified = false;
	if (m.unlock_time != unlock_time) { m.unlock_time = unlock_time; modified = true; }
	MarkUpdateAll(unlockTimeMask, modified);
}

void AccountData_Proxy::SetInactive(const Boolean &inactive)
{
	bool modified = false;
	if (m.inactive != inactive) { m.inactive = inactive; modified = true; }
	MarkUpdateAll(inactiveMask, modified);
}


void AccountData_Proxy::ReadAttr(CONST Buffer &buf)
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

void AccountData_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
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

void AccountData_Proxy::OnUpdate()
{
	Parent::OnUpdate();
	if (IsUpdated(infoMask)) OnUpdateInfo(m.acct_id, m.loginName, m.password_hash);
	if (IsUpdated(addictMask)) OnUpdateAddict(m.disconnect_time, m.offLnTime, m.onLnTime);
	if (IsUpdated(yuanBaoMask)) OnUpdateYuanBao(m.yuanBao, m.yuanBaoTicket);
	if (IsUpdated(banTimeMask)) OnUpdateBanTime(m.banTime);
	if (IsUpdated(addictTypeMask)) OnUpdateAddictType(m.addictType);
	if (IsUpdated(yuanBaoLockMask)) OnUpdateYuanBaoLock(m.yuanBaoLock);
	if (IsUpdated(assetLockPasswordMask)) OnUpdateAssetLockPassword(m.lockPassword);
	if (IsUpdated(unlockTimeMask)) OnUpdateUnlockTime(m.unlock_time);
	if (IsUpdated(inactiveMask)) OnUpdateInactive(m.inactive);
}

#pragma optimize("", off)
RPCResult AccountData_Proxy::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_AccountData_UP_STATE:
		ReadAttr(*pBuf);
		if (IsUpdated()) OnUpdate();
		_result = RPC_RESULT_OK;
		break;
	default:
		_result = Parent::Process(rpcId, pPeer, context, pBuf);
	}
	return _result;
}

RPCResult AccountData_Proxy::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)
{
	RPCResult _result = RPC_RESULT_FAIL;
	switch(rpcId)
	{
	case 0:
		_result = RPC_RESULT_OK;
		break;
	case RPC_AccountData_UP_STATE:
			_result = RPCDecode<1209368035>(pPeer, pBuf);
		break;
	default:
		_result = Parent::Decode(rpcId, pPeer, pBuf);
	}
	return _result;
}
