///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTTYPE_PROXY_H_
#define _RPC_QUESTTYPE_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestType_enum.h"

class QuestType_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &char_id, const UInt16 &Type_id);
	virtual void OnUpdateName(const String &name);
	virtual void OnUpdateRound(const UInt16 &round);
	virtual void OnUpdateMaxRound(const UInt16 &maxRound);
	virtual void OnUpdateTotalRound(const UInt16 &totalRound);
	virtual void OnUpdateLimitRound(const UInt16 &limitRound);
	virtual void OnUpdateLimitType(const UInt16 &limitType);
	virtual void OnUpdateLastClearTime(const UInt32 &lastClearTime);
	virtual void OnUpdateStatus(const UInt8 &status);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		nameMask	= Parent::NextFreeMask + 1,
		roundMask	= Parent::NextFreeMask + 2,
		maxRoundMask	= Parent::NextFreeMask + 3,
		totalRoundMask	= Parent::NextFreeMask + 4,
		limitRoundMask	= Parent::NextFreeMask + 5,
		limitTypeMask	= Parent::NextFreeMask + 6,
		lastClearTimeMask	= Parent::NextFreeMask + 7,
		statusMask	= Parent::NextFreeMask + 8,
		NextFreeMask	= Parent::NextFreeMask + 9
	};
	const UInt32& GetChar_id() const { return m.char_id; }
	const UInt16& GetType_id() const { return m.Type_id; }
	const String& GetName() const { return m.name; }
	const UInt16& GetRound() const { return m.round; }
	const UInt16& GetMaxRound() const { return m.maxRound; }
	const UInt16& GetTotalRound() const { return m.totalRound; }
	const UInt16& GetLimitRound() const { return m.limitRound; }
	const UInt16& GetLimitType() const { return m.limitType; }
	const UInt32& GetLastClearTime() const { return m.lastClearTime; }
	const UInt8& GetStatus() const { return m.status; }
	virtual void OnUpdateId(const UInt32 &char_id, const UInt16 &Type_id) {};
	virtual void OnUpdateName(const String &name) {};
	virtual void OnUpdateRound(const UInt16 &round) {};
	virtual void OnUpdateMaxRound(const UInt16 &maxRound) {};
	virtual void OnUpdateTotalRound(const UInt16 &totalRound) {};
	virtual void OnUpdateLimitRound(const UInt16 &limitRound) {};
	virtual void OnUpdateLimitType(const UInt16 &limitType) {};
	virtual void OnUpdateLastClearTime(const UInt32 &lastClearTime) {};
	virtual void OnUpdateStatus(const UInt8 &status) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 char_id;
	UInt16 Type_id;
	//name
	String name;
	//round
	UInt16 round;
	//maxRound
	UInt16 maxRound;
	//totalRound
	UInt16 totalRound;
	//limitRound
	UInt16 limitRound;
	//limitType
	UInt16 limitType;
	//lastClearTime
	UInt32 lastClearTime;
	//status
	UInt8 status;
} m;

public:
	INLINE QuestType_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult QuestType_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestType_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<346048650>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(169762045)
	AES_IADD_ROUNDKEY
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
	AES_ISUB_BYTES
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(346048650)
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_MIX_COLUMNS
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_QUESTTYPE_PROXY_H_
