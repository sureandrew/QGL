///////////////////////////////////////////////////////////
///        Generated RPC Stub header by IDL             ///
///        (DO NOT modify this file)                    ///
///////////////////////////////////////////////////////////
#pragma once
#ifndef _RPC_QUESTJOURNAL_STUB_H_
#define _RPC_QUESTJOURNAL_STUB_H_

#include "Common/RPCFunc.h"
#include "RPCEnum/QuestJournal_enum.h"

class QuestJournal_Stub : public NetObject
{
/*** virtual method for fast copy and paste in real class header ***
public:
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
	void SetId(const UInt32 &journal_id);
	void SetJournal_id(const UInt32 &journal_id);
	void SetName(const String &name);
	void SetLevel(const UInt16 &minLv, const UInt16 &maxLv);
	void SetMinLv(const UInt16 &minLv);
	void SetMaxLv(const UInt16 &maxLv);
	void SetDescript(const String &descript);
	void SetTeamDescript(const String &teamDescript);
	void SetTimeStr(const String &timeStr);
	void SetNpc(const String &npcName, const String &mapName);
	void SetNpcName(const String &npcName);
	void SetMapName(const String &mapName);
	void SetRound(const UInt16 &Type_id, const UInt16 &limitRound);
	void SetType_id(const UInt16 &Type_id);
	void SetLimitRound(const UInt16 &limitRound);
	void SetEnable(const Boolean &enable);
	void SetDBClickScript(const String &DBClickScript);
	virtual void ReadAttr(CONST Buffer &buf);
	virtual void WriteAttr(Buffer &buf, const RPCMask &mask) CONST;
	virtual void ClearAttr() { Parent::ClearAttr(); _fields _m = {}; m = _m; }
	virtual RPCResult SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask);
	virtual RPCResult Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf);

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
	INLINE QuestJournal_Stub()
	{ _fields _m = {}; m = _m; }

};

#endif //_RPC_QUESTJOURNAL_STUB_H_
