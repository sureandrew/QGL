#include "Common.h"
#include "Common/Procedure.h"
#include "Proxy/Data/MsgCharacterData.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Stub/Logic/Channel.h"
#include "Stub/Logic/Messenger.h"
#include "Stub/Logic/Guild.h"
#include "Stub/Logic/GuildMember.h"
#include "Stub/Logic/MessageCharacter.h"
#include "Stub/Service/MessageService.h"
#include "MessageService/Global.h"
#include "MessageService/MessageApplication.h"
#include "MessageService/GuildControl.h"
#include "Resource/ServerResManager.h"
#include "Resource/GameStringTable.h"
#include "BusinessShopControl.h"

#include "Configure.h"

#define FILENAME_LENGTH				1024
#define CONFIGURATION_INI_FILE		_T(".\\Data\\Setting\\fileconfig.ini")
#define GAMESTRINGTABLE_LOCALIZATION_FILENAME _T("Data\\CSV\\localization.csv")

VOID Session::Release(CONST SessionID &sid)
{
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("Session Deleted: session %s"), 
		sid.GetString().c_str());

	ProcedureManager::GetInstance().RemoveBySession(sid);
}

Boolean Global::Initialize(VOID)
{
	m_rand.Randomize();
	//create global channel
	CChannel *pChannelGlobal = CChannel::NewInstance();
	pChannelGlobal->SetInfo(CHANNELTYPE_GLOBAL);
	pChannelGlobal->m_channelName = ChannelPrefix[CHANNELTYPE_GLOBAL];
	m_MapChannel.insert(std::make_pair(ChannelPrefix[CHANNELTYPE_GLOBAL], pChannelGlobal));

	SysHorseLightMessage[0] = "testing message number one";
	SysHorseLightMessage[1] = "testing message number two";

	m_SysHLMsgIndex = 0;
	m_NumOfClientHLMsg = 0;
	m_PreHLMsgUpdateTime = 0;

	if (m_resource == NULL)
		m_resource = SafeCreateObject(CServerResManager);
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading Block Word..."));
	m_resource->LoadBlockWordCSV();
	m_resource->LoadManorCSV();	

	m_guildControl = SafeCreateObject(CGuildControl);

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading localization..."));
	m_pStringTable = SafeCreateObject(CGameStringTable);
	m_pStringTable->Load(GAMESTRINGTABLE_LOCALIZATION_FILENAME);

	TRACE_INFODTL(GLOBAL_LOGGER, _F("Initialization: Reading BusinessShop..."));
	m_resource->LoadBusinessShopCSV();
	CBusinessShopControl::GetInstance().Initialize();

	return (BOOLEAN_TRUE);
}

VOID Global::Uninitialize(VOID)
{
	DestroyGameContext();

	SafeDeleteObject(m_resource);
}

void Global::DestroyGameContext()
{
	for (MapIndexList::iterator it = m_mapIndices.begin();
		it!=m_mapIndices.end(); ++it)
	{
		if (it->second)
		{
			SafeRelease(it->second);
		}
	}
	m_mapIndices.clear();

	for (ChannelNameMap::iterator Itr = m_MapChannel.begin(); Itr != m_MapChannel.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_MapChannel.clear();

	for (Int i = 0; i < MaxHorseLightMess; i++)
		SysHorseLightMessage[i].Empty();
	ClientHorseLightMessageQuene.clear();
	m_CurrHorseLightMsg.Empty();

	for (MsgCharMap::iterator Itr = m_msgCharMap.begin(); Itr != m_msgCharMap.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_msgCharMap.clear();

	for (OfflineMsgMap::iterator Itr = m_AllOfflineMsg.begin(); Itr != m_AllOfflineMsg.end();
		++Itr)
	{
		Itr->second.msg.Empty();
		Itr->second.name.Empty();
	}
	m_AllOfflineMsg.clear();
	
	if (m_guildControl)
		SafeDeleteObject(m_guildControl);

	for (GuildMap::iterator Itr = m_GuildMap.begin(); Itr != m_GuildMap.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_GuildMap.clear();

	CharMsgInfo* pMsgInfo;
	for (AllCharMap::iterator itCharMap = m_AllCharMap.begin(); itCharMap != m_AllCharMap.end(); ++itCharMap)
	{
		pMsgInfo = itCharMap->second;
		if (pMsgInfo)
		{
			if (pMsgInfo->messengers)
			{
				DeleteNetGroup(pMsgInfo->messengers, BOOLEAN_TRUE);
				pMsgInfo->messengers = NULL;
			}
			SafeDeleteObject(pMsgInfo);
		}
	}
	m_AllCharMap.clear();

	m_mapPeers.clear();
	//m_AllCharWatchMap.clear();
	g_MsgScheduleMap.clear();
	m_guildApplyLock.clear();
	m_proxySidMap.clear();
	m_watcherMap.clear();
	m_charNameMap.clear();
	SafeDeleteObject(m_pStringTable);
}

VOID Global::LockCharGuild(UInt32 char_id)
{
	m_guildApplyLock.insert(std::make_pair(char_id, char_id));
}

VOID Global::UnLockCharGuild(UInt32 char_id)
{
	CharGuildMap::iterator Itr = m_guildApplyLock.find(char_id);
	if (Itr != m_guildApplyLock.end())
	{
		m_guildApplyLock.erase(Itr);
	}
}

BOOL Global::IsLockCharGuild(UInt32 char_id)
{
	return m_guildApplyLock.find(char_id) != m_guildApplyLock.end();
}

CChannel* Global::GetChannel(String ChannelName)
{
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetChannel: ChannelName address 0x%08x Msg: %s"), ChannelName.c_str(), ChannelName.c_str());
	ChannelNameMap::iterator Itr = m_MapChannel.find(ChannelName);
	if (Itr != m_MapChannel.end())
		return Itr->second;
	else
		return NULL;
}

CGuild* Global::GetGuild(UInt32 guild_uid)
{
	if (guild_uid == 0)
		return NULL;

	GuildMap::iterator Itr = m_GuildMap.find(guild_uid);
	if (Itr != m_GuildMap.end())
		return Itr->second;
	else
		return NULL;
}

CGuildMember* Global::GetGuildMember(UInt32 guild_uid, UInt32 char_id)
{
	if (guild_uid == 0 || char_id == 0)
		return NULL;

	GuildMap::iterator Itr = m_GuildMap.find(guild_uid);
	if (Itr != m_GuildMap.end() && Itr->second)
		return Itr->second->GetMember(char_id);
	else
		return NULL;
}

void Global::RemoveGuild(CGuild* pGuild)
{
	if (pGuild)
	{
		gGlobal.m_GuildMap.erase(pGuild->GetGuild_uid());
		DeleteNetObject(pGuild);
	}
}

VOID Global::LeaveAllChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &SourceId)
{
	ChannelNameMap::iterator Itr = m_MapChannel.begin();
	while (Itr != m_MapChannel.end())
	{
		Itr->second->LeaveChannel(pPeer, context, SourceId);
		Itr++;
	}
}

VOID Global::ReleaseChar(UInt32 char_id)
{
	//for (AllCharMap::iterator it = m_AllCharMap.begin(); it != m_AllCharMap.end(); it++)
	CharMsgInfo* pMsgInfo = GetMsgInfo(char_id);
	UnLockCharGuild(char_id);
	if (pMsgInfo && pMsgInfo->pMsgChar)
	{
		CMessageCharacter* pMsgChar = pMsgInfo->pMsgChar;

		for (ChannelNameMap::iterator Itr = m_MapChannel.begin(); 
			Itr != m_MapChannel.end(); ++Itr)
		{
			Itr->second->RemoveCharFromChannel(char_id);
		}

		OnUpdateCharStatus(pMsgChar);	
		UInt32 guild_uid = pMsgChar->GetGuild_uid();
		RemoveMsgInfo(char_id);	
		if (guild_uid != 0)
		{
			CGuild* pGuild = GetGuild(guild_uid);
			if (pGuild)
				pGuild->CheckRelease();				// check if need to release whole guild
		}
	}
}

VOID Global::BlockColorWord(String &Msg)
{
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("BlockColorWord address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());
}

bool Global::IsBlockName(String &Msg)
{
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("BlockWord: BlockWord address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());
	BlockWordDataList* blockList = m_resource->GetNameBlockWordList();
	if (blockList == NULL)
		return true;

	String space = _T(" ");
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("BlockWord: space address 0x%08x Msg: %s"), space.c_str(), space.c_str());
	for (UInt32 i = 0; i < blockList->size(); ++i)
	{
		int temp = (int)Msg.find(blockList->at(i).c_str());
		if (temp >= 0)
			return true;
	}
	return false;
}
VOID Global::BlockWord(String &Msg)
{
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("BlockWord: BlockWord address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());
	BlockWordDataList* blockList = m_resource->GetMsgBlockWordList();
	String space = _T(" ");
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("BlockWord: space address 0x%08x Msg: %s"), space.c_str(), space.c_str());

	if (blockList == NULL)
		return;

	UInt8Vector link1Pos, link2Pos;
	UInt8Vector checkOpenClose;
	for (Index i = C_INDEX(0); i < Msg.size(); ++i)
	{
		if (Msg[i] == 15)
			checkOpenClose.push_back(i);
		else if (Msg[i] == 16 && checkOpenClose.size() > 0)
		{
			link1Pos.push_back(checkOpenClose.back());
			link2Pos.push_back(i);
			checkOpenClose.pop_back();
		}
	}
	for (UInt32 i = 0; i < blockList->size(); ++i)
	{
		int temp = (int)Msg.find(blockList->at(i).c_str());
		UInt32 j = 0;
		UInt32 msgSize = (UInt32)Msg.size();
		while (temp >= 0 && j < msgSize )
		{
			UInt pos = 0;
			bool ignor = false;
			for (Index a = C_INDEX(0); a < link1Pos.size(); ++a)
			{
				if (link1Pos[a] < temp && link2Pos[a] > temp)
				{
					ignor = true;
					pos = a;
					break;
				}
			}

			if (!ignor)
				Msg.replace(temp, blockList->at(i).length(), space.c_str());
			else
				temp += link2Pos[pos];
			temp = (int)Msg.find(blockList->at(i).c_str(), temp + 1);
			++j;
		}
	}
}

VOID Global::InsertCharWatch(UInt32 viewer, UInt32 target)
{
	m_watcherMap[target].insert(viewer);
}

VOID Global::RemoveGuildApplicant(UInt32 char_id)
{
	for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin();
		Itr != gGlobal.m_GuildMap.end(); ++Itr)
	{
		Itr->second->RemoveGuildApplicant(char_id);
	}
}


VOID Global::RemoveCharWatch(UInt32 viewer, UInt32 target)
{
	m_watcherMap[target].erase(viewer);
}

bool Global::IsBlockList(UInt32 listener, UInt32 speaker)
{
	CharMsgInfo* pMsgInfo = GetMsgInfo(listener);
	if (pMsgInfo)
	{
		if (pMsgInfo->messengers)
		{
			for (int i = 0; i < pMsgInfo->messengers->GetCount(); ++i)
			{
				CMessenger * pMessengerData = pMsgInfo->messengers->GetChild<CMessenger>(i);
				if (pMessengerData && pMessengerData->GetFriend_char_id() == speaker)
				{
					return pMessengerData->GetFriend_group() == FRIEND_GROUP_BLACK;
				}
			}
		}

		if (pMsgInfo->pMsgChar->GetEnableBlockMsg())
			return true;
	}
	return false;
}

VOID Global::OnUpdateCharStatus(CMessageCharacter* pMsgChar)
{
	if (pMsgChar == NULL || !pMsgChar->IsUpdated())
		return;

	RPCContext context;
	WatcherSet &watcherList = m_watcherMap[pMsgChar->GetChar_id()];
	NetGroup grp;

	pMsgChar->SetSendMask(pMsgChar->GetUpdateMask());
	grp.Add(pMsgChar);

	for (WatcherSet::iterator it = watcherList.begin(); it != watcherList.end();)
	{
		CharMsgInfo* pMsgInfo = GetMsgInfo(*it);
		if (pMsgInfo)
		{
			context.sessionId = pMsgInfo->SID;
			MessageService_Stub::CB_UpdateMsgCharInfo(pMsgInfo->pProxyPeer, context, &grp);
			++it;
		}
		else
		{
			// watcher does not exist, remove it from watcher list
			it = watcherList.erase(it);
		}
	}

	// update guild member if have guild
	if (pMsgChar->HasGuild() && pMsgChar->IsUpdated(CMessageCharacter::friend_statusMask))
	{
		CGuild* pGuild = GetGuild(pMsgChar->GetGuild_uid());
		if (pGuild)
			pGuild->MemberOnline(pMsgChar->GetChar_id(), !pMsgChar->IsOffline());
	}
	pMsgChar->ClearUpdateMask();
}

VOID Global::RemoveMessagerByOwnerID(UInt32 char_id)
{
	UInt32Vector ownerID;
	UInt32Vector targetID;

	CharMsgInfo* pMsgInfo = GetMsgInfo(char_id);
	if (pMsgInfo == NULL)
		return;

	CMessenger* pMessenger;
	RPCContext context;
	NetGroup* pMsgGrp;

	//remove all messenger friend_id == char_id
	WatcherSet &watcherList = m_watcherMap[char_id];
	for (WatcherSet::iterator itWatch = watcherList.begin(); 
		itWatch != watcherList.end(); ++itWatch)
	{
		CharMsgInfo* pSubMsgInfo = GetMsgInfo(*itWatch);
		if (pSubMsgInfo && pSubMsgInfo->messengers && pSubMsgInfo->pProxyPeer)
		{
			context.sessionId = pSubMsgInfo->SID;
			pMsgGrp = pSubMsgInfo->messengers;
			for (int j = 0; j < pMsgGrp->GetCount(); ++j)
			{
				pMessenger = pMsgGrp->GetChild<CMessenger>(j);
				if (pMessenger && pMessenger->GetFriend_char_id() == char_id)
				{
					pMsgGrp->Remove(pMessenger);
					MessageService_Stub::CB_RemoveMessengers(pSubMsgInfo->pProxyPeer, context, 
						pMessenger->GetPlayer_char_id(), pMessenger->GetFriend_char_id());
					MessageService_Stub::CB_UpdateMessengers(pSubMsgInfo->pProxyPeer, context, pMsgGrp);
					ownerID.push_back(pMessenger->GetPlayer_char_id());
					targetID.push_back(pMessenger->GetFriend_char_id());
					DeleteNetObject(pMessenger);
					break;
				}
			}
		}
	}
	// remove all watcher for this character
	m_watcherMap.erase(char_id);

	//remove all messenger player_id == char_id
	if (pMsgInfo->messengers)
	{
		for (int j = 0; j < pMsgInfo->messengers->GetCount(); ++j)
		{
			CMessenger * pMessenger = pMsgInfo->messengers->GetChild<CMessenger>(j);
			if (pMessenger)
			{
				ownerID.push_back(pMessenger->GetPlayer_char_id());
				targetID.push_back(pMessenger->GetFriend_char_id());
			}
		}
	}
	pMsgInfo->messengers = NULL;
	
	ChannelNameMap::iterator Itr = m_MapChannel.begin();
	while (Itr != m_MapChannel.end())
	{
		Itr->second->RemoveCharFromChannel(pMsgInfo->pMsgChar->GetChar_id());
		Itr++;
	}

	ReleaseMsgCharRef(char_id);
}

VOID Global::RemoveMessager(CMessenger * pMessenger)
{
	if (pMessenger == NULL)
		return;

	CharMsgInfo* pMsgInfo = GetMsgInfo(pMessenger->GetPlayer_char_id());
	if (pMsgInfo && pMsgInfo->messengers)
	{
		if (pMessenger->GetFriend_group() < MAX_FRIEND_GROUP_TYPE)
		{
		if (pMsgInfo->messengerCount[pMessenger->GetFriend_group()] > 0)
			pMsgInfo->messengerCount[pMessenger->GetFriend_group()] --;
		}
		pMsgInfo->messengers->Remove(pMessenger);

		MessageService_Stub::CB_RemoveMessengers(pMsgInfo->pProxyPeer, 
			RPCContext(pMsgInfo->SID), pMessenger->GetPlayer_char_id(),
			pMessenger->GetFriend_char_id());
		MessageService_Stub::CB_UpdateMessengers(pMsgInfo->pProxyPeer, 
			RPCContext(pMsgInfo->SID), pMsgInfo->messengers);

		gGlobal.RemoveCharWatch(pMessenger->GetPlayer_char_id(), pMessenger->GetFriend_char_id());
		
		UInt32Vector ownerID;
		UInt32Vector targetID;
		ownerID.push_back(pMessenger->GetPlayer_char_id());
		targetID.push_back(pMessenger->GetFriend_char_id());

		UnsetOwnObject(pMsgInfo->SID, pMessenger);
	}
}

Boolean Global::AddOfflineMsg(UInt32 targetId, UInt32 srcID, const String &name, const String &msg)
{
	if ((UInt16) m_AllOfflineMsg.count(targetId) >= gConfigure.maxOffMsg)
		return BOOLEAN_FALSE;

	OfflineMsg offMsg(srcID, name, msg);
	m_AllOfflineMsg.insert(std::make_pair(targetId, offMsg));

	return BOOLEAN_TRUE;
}

CharMsgInfo* Global::GetMsgInfo(UInt32 char_id)
{
	AllCharMap::iterator it = m_AllCharMap.find(char_id);
	if (it != m_AllCharMap.end())
		return it->second;
	else
		return NULL;
}

CharMsgInfo* Global::GetMsgInfo(const String& name)
{
	CharNameMap::iterator it = m_charNameMap.find(name);
	if (it != m_charNameMap.end())
		return GetMsgInfo(it->second);
	else
		return NULL;
}

CharMsgInfo* Global::AddMsgInfo(UInt32 char_id, CMsgCharacterData* pData, SessionID &sid)
{
	AllCharMap::iterator it = m_AllCharMap.find(char_id);
	if (it != m_AllCharMap.end())
		return it->second;

	CharMsgInfo* pMsgInfo = SafeCreateObject(CharMsgInfo);
	
	pMsgInfo->SID = sid;
	pMsgInfo->pMsgChar = AddMsgCharRef(char_id, pData);
	gGlobal.m_AllCharMap[char_id] = pMsgInfo;

	return pMsgInfo;
}

void Global::RemoveMsgInfo(UInt32 char_id)
{
	AllCharMap::iterator it = m_AllCharMap.find(char_id);
	if (it != m_AllCharMap.end())
	{
		CharMsgInfo* pMsgInfo = it->second;
		m_AllCharMap.erase(it);				// remove from AllCharMap for msg char ref checking
		if (pMsgInfo)
		{
			UInt32 mcount = pMsgInfo->messengers ? pMsgInfo->messengers->GetCount() : 0;
			TRACE_INFODTL_3(GLOBAL_LOGGER, 
				_T("RemoveMsgInfo: char_id %d, sessionId %s, messengers %d"),
				char_id, pMsgInfo->SID.GetString().c_str(), mcount);
			if (pMsgInfo->pMsgChar)
			{
				WatcherMap::iterator it2 = m_watcherMap.find(char_id);
				if (it2 != m_watcherMap.end())
					m_watcherMap.erase(it2);
				ReleaseMsgCharRef(char_id);
			}
			DeleteNetGroup(pMsgInfo->messengers, BOOLEAN_TRUE);
			SafeDeleteObject(pMsgInfo);
		}
	}
}

CMessageCharacter* Global::AddMsgCharRef(UInt32 char_id, CMsgCharacterData* pData)
{
	MsgCharMap::iterator it = m_msgCharMap.find(char_id);
	CMessageCharacter* pMsgChar;

	if (it != m_msgCharMap.end())
	{
		// found message character
		pMsgChar = it->second;
	}
	else
	{
		// not found, create new one
		pMsgChar = CMessageCharacter::NewInstance();
		TRACE_ENSURE(pMsgChar);
		m_msgCharMap[char_id] = pMsgChar;
	}

	if (pMsgChar)
	{
		if (pData)
			pMsgChar->SetData(pData);
		pMsgChar->SetChar_id(char_id);
		pMsgChar->SetFriend_status(FRIENC_CHAT_STATUS_OFFLINE);
		pMsgChar->ClearUpdateMask();
		pMsgChar->m_refCount++;		// add reference

		if (!pMsgChar->GetNickName().IsEmpty())
		{
			const String &name = pMsgChar->GetNickName();
			CharNameMap::iterator itfind = m_charNameMap.find(name);
			if (itfind == m_charNameMap.end())
				m_charNameMap[name] = char_id;
		}

		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("AddMsgRef: char_id %d, count %d"), 
			char_id, pMsgChar->m_refCount);
	}

	return pMsgChar;
}

CMessageCharacter* Global::GetMsgCharRef(UInt32 char_id, bool bAdd)
{
	CMessageCharacter* pMsgChar = NULL;
	MsgCharMap::iterator it = m_msgCharMap.find(char_id);
	if (it != m_msgCharMap.end())
		pMsgChar = it->second;

	if (pMsgChar && bAdd)
	{
		pMsgChar->m_refCount++;
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("AddMsgRef: char_id %d, count %d"), 
			char_id, pMsgChar->m_refCount);
	}

	return pMsgChar;
}

CMessageCharacter* Global::GetMsgCharRef(const String& name)
{
	CharNameMap::iterator it = m_charNameMap.find(name);
	if (it != m_charNameMap.end())
		return GetMsgCharRef(it->second, false);
	else
		return NULL;
}

void Global::ReleaseMsgCharRef(UInt32 char_id)
{
	CMessageCharacter* pMsgChar = NULL;
	MsgCharMap::iterator it = m_msgCharMap.find(char_id);

	if (it != m_msgCharMap.end())
		pMsgChar = it->second;

	if (pMsgChar)
	{
		if (pMsgChar->m_refCount > 0)
			pMsgChar->m_refCount--;
		TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("RemoveMsgRef: char_id %d, count %d"), 
			char_id, pMsgChar->m_refCount);
		// safely prevent to remove msg char ref when MsgInfo exist
		if (pMsgChar->m_refCount == 0 && GetMsgInfo(pMsgChar->GetChar_id()) != NULL)
			pMsgChar->m_refCount = 1;

		if (pMsgChar->m_refCount == 0)
		{
			// real release character when no more reference
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ReleaseMsgRef: release char_id %d"), char_id);
			ObjectService_Proxy::ReleaseMsgCharacter(gGlobal.pObjectService->pPeer, 
				RPCContext(), char_id);
			m_charNameMap.erase(pMsgChar->GetNickName());
			DeleteNetObject(pMsgChar);
			m_msgCharMap.erase(it);
		}
	}
}

VOID Global::OnMapServiceClose(LPCPEER pPeer)
{
	for (MapIndexList::iterator it = m_mapIndices.begin(); it != m_mapIndices.end(); ++it)
	{
		if (it->second == pPeer)
		{
			SafeRelease(it->second);
			it = m_mapIndices.erase(it);
		}
	}
	m_mapPeers.erase(pPeer);
}

void Global::RegisterMap(const MapIndex& idx, LPCPEER pPeer)
{
	if (pPeer == NULL) return;

	UnregisterMap(idx);				// unregister previous one if found
	m_mapIndices[idx] = pPeer->AddRef();
	m_mapPeers.insert(pPeer);
}

void Global::UnregisterMap(const MapIndex& idx)
{
	MapIndexList::iterator itfind = m_mapIndices.find(idx);
	if (itfind != m_mapIndices.end())
	{
		SafeRelease(itfind->second);
		m_mapIndices.erase(itfind);
	}
}

LPCPEER Global::GetMapPeer(const MapIndex& idx)
{
	MapIndexList::iterator iter = m_mapIndices.find(idx);
	if (iter != m_mapIndices.end())
		return (iter->second);
	return (NULL);
}

void Global::AddProxySid(SessionID& sid)
{
	LPCPEER pPeer = GetProxy(sid.serviceId);
	SessionIDVector &sidList = m_proxySidMap[pPeer];

	SessionIDVector::iterator it = std::find(sidList.begin(), sidList.end(), sid);
	if (it == sidList.end())
		sidList.push_back(sid);
}

void Global::RemoveProxySid(SessionID& sid)
{
	LPCPEER pPeer = GetProxy(sid.serviceId);
	SessionIDVector &sidList = m_proxySidMap[pPeer];

	SessionIDVector::iterator it = std::find(sidList.begin(), sidList.end(), sid);
	if (it != sidList.end())
		sidList.erase(it);
}

void Global::UnregisterProxy(LPCPEER pPeer)
{
	CharMsgInfo* pMsgInfo;
	for (AllCharMap::iterator it = m_AllCharMap.begin(); it != m_AllCharMap.end(); ++it)
	{
		pMsgInfo = it->second;
		if (pMsgInfo && pMsgInfo->pProxyPeer == pPeer)
			pMsgInfo->pProxyPeer = NULL;
	}
}

UInt Global::NoCodeStringSize(String &Text)
{
	Int		Result = 0;
	bool	HideNow = false;

	for(Int i = 0; i < (Int)Text.size(); ++i)
	{
		if(Text[i] == 15)
		{
			HideNow = true;
			continue;
		}
		else
		if(Text[i] == 16)
		{
			HideNow = false;
			continue;
		}
		else
		if(Text[i] <= 26 || HideNow)
		{
			continue;
		}
		
		++Result;
	}

	return Result;
}

void Global::ShutdownAt(UInt32 downTime)
{
	// use global channel to send shutdown message
	ChannelNameMap::iterator itfind = m_MapChannel.find(ChannelPrefix[CHANNELTYPE_GLOBAL]);
	if (itfind == m_MapChannel.end())
		return;

	CChannel* pChannel = itfind->second;
	if (pChannel == NULL)
		return;

	m_downTime = downTime;
	pChannel->SendShutdownMsg(downTime);
}

PCSTRINGA ConvAnsiString(const String& s)
{
	static char buf[1024];
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), (int)s.length(), 
			buf, 1024, 0, 0);
	if (len > 0)
		buf[len] = '\0';
	else
		buf[0] = '\0';

	return buf;
}

Boolean OpenLogger(Logger& logger)
{
	String strLogFile = GLOBAL_CONFIG.GetString(GLOBAL_NAME, logger.GetName().c_str(), NULL);
	if (strLogFile.GetLength() == 0)
		return (BOOLEAN_FALSE);
	Int size = GLOBAL_CONFIG.GetInt(GLOBAL_NAME, _T("LogSize"), 0);
	if (size == 0)
		GLOBAL_CONFIG.GetInt(_T("General"), _T("LogSize"), 200);
	String strArchive = GLOBAL_CONFIG.GetString(GLOBAL_NAME, _T("LogArchive"), NULL);
	if (strArchive.GetLength() == 0)
		strArchive = GLOBAL_CONFIG.GetString(_T("General"), _T("LogArchive"), NULL);
	return (logger.Open(strLogFile.c_str(),
		BOOLEAN_FALSE, BOOLEAN_FALSE, BOOLEAN_TRUE, Logger::LEVEL_VERBOSE,
		size, strArchive.c_str()));
}

void WriteMsgLog(RPCContext &context, UInt channelType, UInt32 senderID, String &senderName, UInt32Vector &receiveIDs, String guildName, String &msg)
{
	time_t nowTime;
	time(&nowTime);
	struct tm *newtime = localtime(&nowTime);
	String timeStr;
	timeStr.Format(_T("[%04d-%02d-%02d %02d.%02d.%02d]"), newtime->tm_year + 1900, newtime->tm_mon + 1, 
		newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

	switch(channelType)
	{
	case CHANNELTYPE_PARTY:
		{
			static Logger logger(_T("LogPartyMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_6(logger,
					_T("%s session_id %s, char_id %d to member %s, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, GetVectorString(receiveIDs).c_str(), String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_GANG:
		{
			static Logger logger(_T("LogGangMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_5(logger,
					_T("%s session_id %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_GUILD:
		{
			static Logger logger(_T("LogGuildMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen && guildName.length() > 0)
			{									
				TRACE_ALWAYSL_6(logger,
					_T("%s session_id %s, guildName %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), guildName.c_str(), senderID, String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;		
	case CHANNELTYPE_GLOBAL:
		{
			static Logger logger(_T("LogGlobalMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_5(logger,
					_T("%s session_id %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;		
	case CHANNELTYPE_SYSTEM:
		{
			static Logger logger(_T("LogSystemMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_3(logger,
					_T("%s session_id %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_RUMOR:
		{
			static Logger logger(_T("LogRumorMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_3(logger,
					_T("%s session_id %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_OFFICIAL:
		{
			static Logger logger(_T("LogOfficialMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_5(logger,
					_T("%s session_id %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_CHATROOM:
		{
			static Logger logger(_T("LogChatroomMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_5(logger,
					_T("%s session_id %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_NOTICE:
		{
			static Logger logger(_T("LogNoticeMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_5(logger,
					_T("%s session_id %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_SPEAKER:
		{
			static Logger logger(_T("LogSpeakerMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen)
			{									
				TRACE_ALWAYSL_5(logger,
					_T("%s session_id %s, char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_PRIVATE:
		{
			static Logger logger(_T("LogPrivateMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen && receiveIDs.size() > 0)
			{							
				
				TRACE_ALWAYSL_6(logger,
					_T("%s session_id %s, char_id %d send to char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, receiveIDs[0], String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	case CHANNELTYPE_FRIEND:
		{
			static Logger logger(_T("LogFriendMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen && receiveIDs.size() > 0)
			{							

				TRACE_ALWAYSL_6(logger,
					_T("%s session_id %s, char_id %d send to char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, receiveIDs[0], String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
	case CHANNELTYPE_INSTANCE:
		{
			static Logger logger(_T("LogInstanceMessage"));
			static Boolean bOpen = OpenLogger(logger);
			if (bOpen && receiveIDs.size() > 0)
			{							

				TRACE_ALWAYSL_6(logger,
					_T("%s session_id %s, char_id %d send to char_id %d, %s : %s"),
					timeStr.c_str(), context.sessionId.GetString().c_str(), senderID, receiveIDs[0], String(ConvAnsiString(senderName.c_str())).c_str(), 
					String(ConvAnsiString(msg.c_str())).c_str());
			}
		}
		break;
	}
}