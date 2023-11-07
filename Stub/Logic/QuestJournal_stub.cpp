//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/QuestJournal_stub.h"

void QuestJournal_Stub::SetId(const UInt32 &journal_id)
{
	bool modified = false;
	if (m.journal_id != journal_id) { m.journal_id = journal_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestJournal_Stub::SetJournal_id(const UInt32 &journal_id)
{
	bool modified = false;
	if (m.journal_id != journal_id) { m.journal_id = journal_id; modified = true; }
	MarkUpdateAll(idMask, modified);
}

void QuestJournal_Stub::SetName(const String &name)
{
	bool modified = false;
	if (m.name != name) { m.name = name; modified = true; }
	MarkUpdateAll(nameMask, modified);
}

void QuestJournal_Stub::SetLevel(const UInt16 &minLv, const UInt16 &maxLv)
{
	bool modified = false;
	if (m.minLv != minLv) { m.minLv = minLv; modified = true; }
	if (m.maxLv != maxLv) { m.maxLv = maxLv; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void QuestJournal_Stub::SetMinLv(const UInt16 &minLv)
{
	bool modified = false;
	if (m.minLv != minLv) { m.minLv = minLv; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void QuestJournal_Stub::SetMaxLv(const UInt16 &maxLv)
{
	bool modified = false;
	if (m.maxLv != maxLv) { m.maxLv = maxLv; modified = true; }
	MarkUpdateAll(levelMask, modified);
}

void QuestJournal_Stub::SetDescript(const String &descript)
{
	bool modified = false;
	if (m.descript != descript) { m.descript = descript; modified = true; }
	MarkUpdateAll(descriptMask, modified);
}

void QuestJournal_Stub::SetTeamDescript(const String &teamDescript)
{
	bool modified = false;
	if (m.teamDescript != teamDescript) { m.teamDescript = teamDescript; modified = true; }
	MarkUpdateAll(teamDescriptMask, modified);
}

void QuestJournal_Stub::SetTimeStr(const String &timeStr)
{
	bool modified = false;
	if (m.timeStr != timeStr) { m.timeStr = timeStr; modified = true; }
	MarkUpdateAll(timeStrMask, modified);
}

void QuestJournal_Stub::SetNpc(const String &npcName, const String &mapName)
{
	bool modified = false;
	if (m.npcName != npcName) { m.npcName = npcName; modified = true; }
	if (m.mapName != mapName) { m.mapName = mapName; modified = true; }
	MarkUpdateAll(npcMask, modified);
}

void QuestJournal_Stub::SetNpcName(const String &npcName)
{
	bool modified = false;
	if (m.npcName != npcName) { m.npcName = npcName; modified = true; }
	MarkUpdateAll(npcMask, modified);
}

void QuestJournal_Stub::SetMapName(const String &mapName)
{
	bool modified = false;
	if (m.mapName != mapName) { m.mapName = mapName; modified = true; }
	MarkUpdateAll(npcMask, modified);
}

void QuestJournal_Stub::SetRound(const UInt16 &Type_id, const UInt16 &limitRound)
{
	bool modified = false;
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	if (m.limitRound != limitRound) { m.limitRound = limitRound; modified = true; }
	MarkUpdateAll(roundMask, modified);
}

void QuestJournal_Stub::SetType_id(const UInt16 &Type_id)
{
	bool modified = false;
	if (m.Type_id != Type_id) { m.Type_id = Type_id; modified = true; }
	MarkUpdateAll(roundMask, modified);
}

void QuestJournal_Stub::SetLimitRound(const UInt16 &limitRound)
{
	bool modified = false;
	if (m.limitRound != limitRound) { m.limitRound = limitRound; modified = true; }
	MarkUpdateAll(roundMask, modified);
}

void QuestJournal_Stub::SetEnable(const Boolean &enable)
{
	bool modified = false;
	if (m.enable != enable) { m.enable = enable; modified = true; }
	MarkUpdateAll(enableMask, modified);
}

void QuestJournal_Stub::SetDBClickScript(const String &DBClickScript)
{
	bool modified = false;
	if (m.DBClickScript != DBClickScript) { m.DBClickScript = DBClickScript; modified = true; }
	MarkUpdateAll(DBClickScriptMask, modified);
}



void QuestJournal_Stub::ReadAttr(CONST Buffer &buf)
{
	Parent::ReadAttr(buf);
}

void QuestJournal_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST
{
	Parent::WriteAttr(buf, mask);
	if (mask.IsBit(idMask))
	{
		buf.Write<UInt32>(m.journal_id);
	}
	if (mask.IsBit(nameMask))
	{
		buf.Write<String>(m.name);
	}
	if (mask.IsBit(levelMask))
	{
		buf.Write<UInt16>(m.minLv);
		buf.Write<UInt16>(m.maxLv);
	}
	if (mask.IsBit(descriptMask))
	{
		buf.Write<String>(m.descript);
	}
	if (mask.IsBit(teamDescriptMask))
	{
		buf.Write<String>(m.teamDescript);
	}
	if (mask.IsBit(timeStrMask))
	{
		buf.Write<String>(m.timeStr);
	}
	if (mask.IsBit(npcMask))
	{
		buf.Write<String>(m.npcName);
		buf.Write<String>(m.mapName);
	}
	if (mask.IsBit(roundMask))
	{
		buf.Write<UInt16>(m.Type_id);
		buf.Write<UInt16>(m.limitRound);
	}
	if (mask.IsBit(enableMask))
	{
		buf.Write<Boolean>(m.enable);
	}
	if (mask.IsBit(DBClickScriptMask))
	{
		buf.Write<String>(m.DBClickScript);
	}
}

RPCResult QuestJournal_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)
{
	if (pPeer == NULL) return RPC_RESULT_FAIL;

	Buffer* pBuf = SafeCreateObject(Buffer);
	Reuben::Network::RPCHeader header(RPC_QuestJournal_UP_STATE);
	pBuf->Write<Reuben::Network::RPCHeader>(header);
	pBuf->Write<RPCContext>(context);
	pBuf->Write<RPCNetID>(m_netId);
	WriteAttr(*pBuf, mask);

	return RPCSend(pPeer, pBuf);
}

#pragma optimize("", off)
RPCResult QuestJournal_Stub::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)
{
	RPCResult _result = RPC_NOT_ACCESS;
	switch(rpcId)
	{
	case 0:
		break;
	case RPC_QuestJournal_REQ_STATE:
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

