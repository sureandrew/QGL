///////////////////////////////////////////////////////////
///        Generated RPC Proxy header by IDL            ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTJOURNAL_PROXY_H_
#define _RPC_QUESTJOURNAL_PROXY_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestJournal_enum.h"

class QuestJournal_Proxy : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
	// OnUpdate can be implemented selectively
	virtual void OnUpdateId(const UInt32 &journal_id);
	virtual void OnUpdateName(const String &name);
	virtual void OnUpdateLevel(const UInt16 &minLv, const UInt16 &maxLv);
	virtual void OnUpdateDescript(const String &descript);
	virtual void OnUpdateTeamDescript(const String &teamDescript);
	virtual void OnUpdateTimeStr(const String &timeStr);
	virtual void OnUpdateNpc(const String &npcName, const String &mapName);
	virtual void OnUpdateRound(const UInt16 &Type_id, const UInt16 &limitRound);
	virtual void OnUpdateEnable(const Boolean &enable);
	virtual void OnUpdateDBClickScript(const String &DBClickScript);
***/

protected:
	typedef NetObject Parent;

public:
	enum UpdateMask {
		idMask	= Parent::NextFreeMask + 0,
		nameMask	= Parent::NextFreeMask + 1,
		levelMask	= Parent::NextFreeMask + 2,
		descriptMask	= Parent::NextFreeMask + 3,
		teamDescriptMask	= Parent::NextFreeMask + 4,
		timeStrMask	= Parent::NextFreeMask + 5,
		npcMask	= Parent::NextFreeMask + 6,
		roundMask	= Parent::NextFreeMask + 7,
		enableMask	= Parent::NextFreeMask + 8,
		DBClickScriptMask	= Parent::NextFreeMask + 9,
		NextFreeMask	= Parent::NextFreeMask + 10
	};
	const UInt32& GetJournal_id() const { return m.journal_id; }
	const String& GetName() const { return m.name; }
	const UInt16& GetMinLv() const { return m.minLv; }
	const UInt16& GetMaxLv() const { return m.maxLv; }
	const String& GetDescript() const { return m.descript; }
	const String& GetTeamDescript() const { return m.teamDescript; }
	const String& GetTimeStr() const { return m.timeStr; }
	const String& GetNpcName() const { return m.npcName; }
	const String& GetMapName() const { return m.mapName; }
	const UInt16& GetType_id() const { return m.Type_id; }
	const UInt16& GetLimitRound() const { return m.limitRound; }
	const Boolean& GetEnable() const { return m.enable; }
	const String& GetDBClickScript() const { return m.DBClickScript; }
	virtual void OnUpdateId(const UInt32 &journal_id) {};
	virtual void OnUpdateName(const String &name) {};
	virtual void OnUpdateLevel(const UInt16 &minLv, const UInt16 &maxLv) {};
	virtual void OnUpdateDescript(const String &descript) {};
	virtual void OnUpdateTeamDescript(const String &teamDescript) {};
	virtual void OnUpdateTimeStr(const String &timeStr) {};
	virtual void OnUpdateNpc(const String &npcName, const String &mapName) {};
	virtual void OnUpdateRound(const UInt16 &Type_id, const UInt16 &limitRound) {};
	virtual void OnUpdateEnable(const Boolean &enable) {};
	virtual void OnUpdateDBClickScript(const String &DBClickScript) {};
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual	void OnUpdate();
	virtual RPCResult SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);
	static RPCResult Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf);

private:
struct _fields {
	//id
	UInt32 journal_id;
	//name
	String name;
	//level
	UInt16 minLv;
	UInt16 maxLv;
	//descript
	String descript;
	//teamDescript
	String teamDescript;
	//timeStr
	String timeStr;
	//npc
	String npcName;
	String mapName;
	//round
	UInt16 Type_id;
	UInt16 limitRound;
	//enable
	Boolean enable;
	//DBClickScript
	String DBClickScript;
} m;

public:
	INLINE QuestJournal_Proxy()
			{ _fields _m = {}; m = _m; }

};

INLINE RPCResult QuestJournal_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;
	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestJournal_REQ_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	pBuf->Write<RPCMask>(mask);

	return RPCSend<28716839>(pPeer, pBuf);
}


#ifdef _REUBEN_MATH_AES_H_
AES_DECRYPT_HEAD(1715610412)
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_IMIX_COLUMNS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISHIFT_ROWS
	AES_ISUB_BYTES
	AES_ISHIFT_ROWS
	AES_IADD_ROUNDKEY
	AES_IMIX_COLUMNS
AES_DECRYPT_TAIL

AES_ENCRYPT_HEAD(28716839)
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SHIFT_ROWS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_ADD_ROUNDKEY
	AES_SHIFT_ROWS
	AES_SHIFT_ROWS
	AES_MIX_COLUMNS
	AES_SUB_BYTES
	AES_SHIFT_ROWS
	AES_ADD_ROUNDKEY
	AES_SUB_BYTES
AES_ENCRYPT_TAIL

#endif // _REUBEN_MATH_AES_H_

#endif //_RPC_QUESTJOURNAL_PROXY_H_
