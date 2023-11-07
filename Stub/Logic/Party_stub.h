///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_PARTY_STUB_H_
#define _RPC_PARTY_STUB_H_

#include "Common/RPCFunc.h"
#include "Reuben/Network/NetGroup.h"
#include "RPCEnum/Party_enum.h"

class Party_Stub : public NetGroup
{
/*** virtual method for fast copy and paste in real class header ***
public:
	virtual RPCResult Promote(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult Kick(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId);
	virtual RPCResult Leave(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult Destroy(LPCPEER pPeer, RPCContext &context);
	virtual RPCResult JoinChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id);
	virtual RPCResult ChangeDesc(LPCPEER pPeer, RPCContext &context, const String &desc);
***/

protected:
	typedef NetGroup Parent;

public:
	enum UpdateMask {
		availMask	= Parent::NextFreeMask + 0,
		lineupMask	= Parent::NextFreeMask + 1,
		lineupkeyposMask	= Parent::NextFreeMask + 2,
		DescMask	= Parent::NextFreeMask + 3,
		LeaveTemporaryMask	= Parent::NextFreeMask + 4,
		NextFreeMask	= Parent::NextFreeMask + 5
	};
	virtual RPCResult Promote(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId) = 0;
	virtual RPCResult Kick(LPCPEER pPeer, RPCContext &context, const RPCNetID &memberId) = 0;
	virtual RPCResult Leave(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult Destroy(LPCPEER pPeer, RPCContext &context) = 0;
	virtual RPCResult JoinChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id) = 0;
	virtual RPCResult ChangeDesc(LPCPEER pPeer, RPCContext &context, const String &desc) = 0;
	virtual RPCResult CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg);
	virtual RPCResult CB_LeaderChanged(LPCPEER pPeer, RPCContext &context, const UInt32 &memberId, const UInt8 &memberIdx, const Boolean &shown);
	virtual RPCResult CB_Join(LPCPEER pPeer, RPCContext &context, NetObject* member, const Boolean &shown);
	virtual RPCResult CB_Leave(LPCPEER pPeer, RPCContext &context, NetObject* member, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Destroy(LPCPEER pPeer, RPCContext &context, const bool &kick, const Boolean &shown);
	virtual RPCResult CB_Regroup(LPCPEER pPeer, RPCContext &context, NetObject* member, const UInt8 &memberIdx);
	const Boolean& GetAvail() const { return m.avail; }
	const UInt32& GetM_uLineupId() const { return m.m_uLineupId; }
	const int& GetM_iLineupKeyPos() const { return m.m_iLineupKeyPos; }
	const String& GetDesc() const { return m.desc; }
	const Boolean& GetM_bTempLeave(const UInt8 i) const { return m.m_bTempLeave[i]; }
	void SetAvail(const Boolean &avail);
	void SetLineup(const UInt32 &m_uLineupId);
	void SetM_uLineupId(const UInt32 &m_uLineupId);
	void SetLineupkeypos(const int &m_iLineupKeyPos);
	void SetM_iLineupKeyPos(const int &m_iLineupKeyPos);
	void SetDesc(const String &desc);
	void SetLeaveTemporary(const Boolean &m_bTempLeave0, const Boolean &m_bTempLeave1, const Boolean &m_bTempLeave2, const Boolean &m_bTempLeave3, const Boolean &m_bTempLeave4);
	void SetM_bTempLeave(const UInt8 i, const Boolean &m_bTempLeave);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

private:
struct _fields {
	//avail
	Boolean avail;
	//lineup
	UInt32 m_uLineupId;
	//lineupkeypos
	int m_iLineupKeyPos;
	//Desc
	String desc;
	//LeaveTemporary
	Boolean m_bTempLeave[5];
} m;

public:
	INLINE Party_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_PARTY_STUB_H_
