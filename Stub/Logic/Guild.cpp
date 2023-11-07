//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/common_binary.h"
//-- Self
#include "Stub/Service/MessageService.h"
#include "Stub/Logic/MessageCharacter.h"
#include "Stub/Logic/Guild.h"
#include "Stub/Logic/GuildMember.h"
#include "Stub/Logic/Channel.h"
#include "Proxy/Data/MsgCharacterData.h"
#include "Proxy/Data/GuildMemberData.h"
#include "Stub/Logic/GuildApplicant.h"
#include "Proxy/Data/GuildData.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "MessageService/Global.h"
#include "MessageService/GuildControl.h"
#include "MessageService/MessageApplication.h"
#include "MessageService/Configure.h"
#include "Resource/GameStringTable.h"
#include "Proxy/Service/LogService.h"

//-- Resource
#include "Resource/ResManor.h"
#include "Resource/ServerResManager.h"

#define MAX_GUILD_FUND	5000000


REGISTER_CLASSTYPE(CGuild, Guild_CLASSID);

const UInt16 GuildPostLimit[MAX_GUILD_POSITION] = {1, 1, 1, 1, 1, 4, 4, 10, 10, 4, 10000};
CGuild::CGuild()
{
	m_GuildMasterID = NULL;
	m_pChannel = NULL;

	time_t nowTime;
	time(&nowTime);
	struct tm * newtime = localtime(&nowTime);

	m_donateDay = newtime->tm_yday;
}

CGuild::~CGuild()
{
	for (GuildApplicantMap::iterator Itr = m_applicantMap.begin(); 
		Itr != m_applicantMap.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_applicantMap.clear();

	for (GuildMemberMap::iterator Itr = m_memberMap.begin(); 
		Itr != m_memberMap.end(); ++Itr)
	{
		DeleteNetObject(Itr->second);
	}
	m_memberMap.clear();
}

void CGuild::SetData(CGuildData * pData)
{
	if (pData->IsUpdated(CGuildData::idMask))
	{
		SetId(pData->GetGuild_uid());
	}
	if (pData->IsUpdated(CGuildData::nameMask))
	{
		SetName(pData->GetName());
	}

	if (pData->IsUpdated(CGuildData::creatorMask))
	{
		SetCreator(pData->GetCreator());		
	}

	if (pData->IsUpdated(CGuildData::masterMask))
	{
		SetMaster(pData->GetMaster());
	}

	if (pData->IsUpdated(CGuildData::allianceMask))
	{
		SetAlliance(pData->GetAlliance_id());
	}

	if (pData->IsUpdated(CGuildData::enemyMask))
	{
		SetEnemy(pData->GetEnemy_id());
	}

	if (pData->IsUpdated(CGuildData::secMasterMask))
	{
		SetSecMaster(pData->GetSecMasterName());
	}

	if (pData->IsUpdated(CGuildData::levelMask))
	{
		SetLevel(pData->GetLevel());
	}

	if (pData->IsUpdated(CGuildData::prosperMask))
	{
		SetProsper(pData->GetProsper(), pData->GetMax_prosper());
		
	}

	if (pData->IsUpdated(CGuildData::fundMask))
	{
		SetFund(pData->GetFund());
	}

	if (pData->IsUpdated(CGuildData::memberNumMask))
	{
		SetMemberNum(pData->GetTotalMember(), pData->GetMaxMember());
		
	}

	if (pData->IsUpdated(CGuildData::aimMask))
	{
		SetAim(pData->GetAim());
	}

	if (pData->IsUpdated(CGuildData::ruleMask))
	{
		SetRule(pData->GetRule());
	}

	if (pData->IsUpdated(CGuildData::guildPvPScoreMask))
	{
		SetGuildPvPScore(pData->GetGuildPvPScore());
	}

	if (pData->IsUpdated(CGuildData::guildPvPBidMask))
	{
		SetGuildPvPBid(pData->GetGuildPvPBid());
	}

	if (pData->IsUpdated(CGuildData::guildPvPGroupMask))
	{
		SetGuildPvPGroup(pData->GetGuildPvPGroup());
	}

	if (pData->IsUpdated(CGuildData::groupPvPVSGuildMask))
	{
		SetGroupPvPVSGuild(pData->GetGroupPvPVSGuild());
	}

	if (pData->IsUpdated(CGuildData::manorScaleMask))
	{
		SetManorScale(pData->GetManorScale());
	}

	if (pData->IsUpdated(CGuildData::manorRoomMask))
	{
		SetRoom(pData->GetRoom());
	}

	if (pData->IsUpdated(CGuildData::manorMaterialMask))
	{
		SetMaterial(pData->GetMaterial());
	}

	if (pData->IsUpdated(CGuildData::manorGuardMask))
	{
		SetGuard(pData->GetGuard());
	}

	if (pData->IsUpdated(CGuildData::manorTrainPlayerPowMask))
	{
		SetManorTrainPlayerPow(pData->GetTrainCharExPow(), pData->GetTrainCharInPow());
	}

	if (pData->IsUpdated(CGuildData::manorTrainPartnerPowMask))
	{
		SetManorTrainPartnerPow(pData->GetTrainPartnerExPow(), pData->GetTrainPartnerInPow());
	}

	if (pData->IsUpdated(CGuildData::manorRoomPropertyMask))
	{
		SetManorRoomProperty(pData->GetDrugLv(), pData->GetCookLv(), 
							 pData->GetWineLv(), pData->GetHpLv(), pData->GetSpLv());
	}

	if (pData->IsUpdated(CGuildData::manorMaintainNumMask))
	{
		SetMaintainNum(pData->GetMaintainNum());
	}

	if (pData->IsUpdated(CGuildData::lastMaintainDateMask))
	{
		SetLastMaintainDate(pData->GetLastMaintainDate());
	}

	if (pData->IsUpdated(CGuildData::manorAuraMask))
	{
		SetAura(pData->GetAura());
	}

	if (pData->IsUpdated(CGuildData::offlinetimeMask))
	{
		SetOfflinetime(pData->GetOfflinetime());
	}

	if (pData->IsUpdated(CGuildData::clearBusinessDateMask))
	{
		SetClearBusinessDate(pData->GetClearBusinessDate());

		UInt32 now = (UInt32) time(NULL);
		if (GetClearBusinessDate() == 0 || GetClearBusinessDate() <= now)
		{
			RaiseUniqEventID(EVT_CLEAR_BUSINESS_LOG);
		}
		else
		{
			RaiseUniqEventID(EVT_CLEAR_BUSINESS_LOG, (GetClearBusinessDate() - now) * 1000);
		}
	}

	if (pData->IsUpdated(CGuildData::autoManageBusinessMask))
	{
		SetAutoManageBusiness(pData->GetAutoManageBusiness());
	}


	ClearUpdateMask();
}

void CGuild::InsertGuildMember(CGuildMember* &pGuildMember)
{
	if (pGuildMember == NULL)
		return;

	UInt32 char_id = pGuildMember->GetChar_id();
	pGuildMember->m_pGuild = this;
	if (HasMember(char_id))
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("[member already exist], guild_uid %d, char_id %d"),
			GetGuild_uid(), char_id);
		pGuildMember->SetChar_id(0);	// prevent MsgChar reference release
		DeleteNetObject(pGuildMember);
		pGuildMember = GetMember(char_id);
	}
	else
	{
		m_memberMap.insert(std::make_pair(char_id, pGuildMember));
		if (pGuildMember->GetPost() == GUILD_POSITION_MASTER)
			m_GuildMasterID = char_id;
	}
}

void CGuild::RemoveGuildApplicant(UInt32 char_id)
{
	GuildApplicantMap::iterator Itr = m_applicantMap.find(char_id);
	if (Itr == m_applicantMap.end())
		return;

	DeleteNetObject(Itr->second);
	m_applicantMap.erase(Itr);

	//sync to all member
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
		Itr != m_memberMap.end(); ++Itr)
	{
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(Itr->second->GetChar_id());
		if (pMsgInfo)
			CB_RemoveApplicant(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), char_id);
	}
}

void CGuild::GenerateNewGuildMaster()
{
	CGuildMember* pMember;
	CGuildMember* pMaster = NULL;

	// check if already have guild master
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
		Itr != m_memberMap.end(); ++Itr)
	{
		pMember = Itr->second;
		if (pMember && pMember->GetPost() == GUILD_POSITION_MASTER)
			return;
	}

	// if not, find a greatest second master
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
		Itr != m_memberMap.end(); ++Itr)
	{
		pMember = Itr->second;
		if (pMember && pMember->GetPost() == GUILD_POSITION_SECMASTER)
		{
			if (pMaster == NULL || (pMaster->GetPost() < pMember->GetPost()))
				pMaster = pMember;
		}
	}

	if (pMaster)
	{
		SetMaster(pMaster->GetName());
		pMaster->SetPost(GUILD_POSITION_MASTER);
		SyncUpdate();
	}
}

bool CGuild::NoMaster()
{
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
			Itr != m_memberMap.end(); ++Itr)
	{
		if (Itr->second->GetPost() == GUILD_POSITION_MASTER)
			return false;
	}
	return true;
}

void CGuild::SendGuildMessage(String msg)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SendGuildMessage: Send message to guild with UID = %d"), GetGuild_uid());

	String channelName;
	channelName.Format(_T("%s_%d"), ChannelPrefix[CHANNELTYPE_GUILD], GetGuild_uid());
	CChannel *pChannel = gGlobal.GetChannel(channelName);	
	if (pChannel == NULL){
		return;
	}

	//find one online member
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();Itr != m_memberMap.end(); ++Itr)
	{
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(Itr->second->GetChar_id());
		if (pMsgInfo && pMsgInfo->pMsgChar && !pMsgInfo->pMsgChar->IsOffline())
		{
			pChannel->SendGrpMessage(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), 
			msg, Itr->second->GetChar_id(), Boolean(BOOLEAN_TRUE));
			break;
		}
	}
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SendGuildMessage: Message to guild with UID = %d is sent"), GetGuild_uid());
}

void CGuild::RemoveAllGuildMember()
{
	UInt32Vector char_ids;		
	
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
		Itr != m_memberMap.end(); ++Itr)
	{
		char_ids.push_back(Itr->second->GetChar_id());
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(Itr->second->GetChar_id());		
		if (pMsgInfo)
		{
			if (pMsgInfo->pMsgChar)
				pMsgInfo->pMsgChar->SetGuild(0, String());
			CB_RemoveMember(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), 
				Itr->second->GetChar_id(), String());
		}
		DeleteNetObject(Itr->second);
	}

	//sync to map
	for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); 
		ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
	{		
		MapService_Proxy::GuildMemberRemove(*ItrMap, RPCContext(), char_ids);
	}


	m_memberMap.clear();
}
void CGuild::RemoveGuildMember(UInt32 char_id, String reason, UInt32 kicker_id)
{
	CGuildMember* pMember = NULL;
	GuildMemberMap::iterator Itr = m_memberMap.find(char_id);

	if (Itr != m_memberMap.end())
	{
		pMember = Itr->second;
		m_memberMap.erase(Itr);
		SetTotalMember((UInt16)m_memberMap.size());
	}

	if (pMember)
	{
		UInt8 post = pMember->GetPost();
		
		pMember->SetPost(GUILD_POSITION_NIL);
		if (post == GUILD_POSITION_MASTER)
		{
			SetMaster(_T(""));
			GenerateNewGuildMaster();
		}
		else if (post == GUILD_POSITION_SECMASTER)
		{
			SetSecMaster(_T(""));
		}
		String reasonFull;
		reasonFull.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_KICK_BYOTHER_OWN_MSG")), 
			reason.c_str());
		gGlobal.BlockWord(reasonFull);		

		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo && pMsgInfo->pMsgChar)
		{
			pMsgInfo->pMsgChar->SetGuild(0, String());
			gGlobal.OnUpdateCharStatus(pMsgInfo->pMsgChar);
			if (pMsgInfo->pProxyPeer) 
			{
				String channelName;
				channelName.Format(_T("%s_%d"), ChannelPrefix[CHANNELTYPE_GUILD], GetGuild_uid());
				MessageService_Stub::LeaveChannel(pMsgInfo->pProxyPeer, 
					RPCContext(pMsgInfo->SID), char_id, channelName);
				CB_RemoveMember(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), char_id, reason);
			}
			if (kicker_id != char_id)
			{
				CharMsgInfo* pMsgInfoKicker = gGlobal.GetMsgInfo(kicker_id);
				if (pMsgInfoKicker)
					MessageService_Stub::CB_ReceivePrivateMessage(pMsgInfoKicker->pProxyPeer, 
						RPCContext(pMsgInfoKicker->SID), reasonFull, 
						pMsgInfoKicker->pMsgChar->GetNickName(), 
						pMsgInfoKicker->pMsgChar->GetChar_id(), 
						pMsgInfo->pMsgChar->GetNickName(), 
						pMsgInfo->pMsgChar->GetChar_id(), true); 
			}
		}
		else
		{
			if (!pMember->GetInactive() && kicker_id != char_id)
			{
				CharMsgInfo* pMsgInfoKicker = gGlobal.GetMsgInfo(kicker_id);
				if (pMsgInfoKicker)
					gGlobal.AddOfflineMsg(char_id, kicker_id, 
						pMsgInfoKicker->pMsgChar->GetNickName(), reasonFull);
			}
		}

		//sync to map
		for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); 
			ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
		{
			UInt32Vector char_ids;
			char_ids.push_back(char_id);
			MapService_Proxy::GuildMemberRemove(*ItrMap, RPCContext(), char_ids);
		}

		//sync to all member
		CGuildMember* pMember2;
		CharMsgInfo* pSubMsgInfo;
		for (GuildMemberMap::iterator Itr = m_memberMap.begin();
			Itr != m_memberMap.end(); ++Itr)
		{
			pMember2 = Itr->second;
			pSubMsgInfo = gGlobal.GetMsgInfo(pMember2->GetChar_id());
			if (pSubMsgInfo)
				CB_RemoveMember(pSubMsgInfo->pProxyPeer, RPCContext(pSubMsgInfo->SID), 
					char_id, reason);
		}

		// delete it finally
		DeleteNetObject(pMember);
		SyncUpdate();
	}
}

void CGuild::WriteData(CGuildData * pData)
{
	if (IsUpdated())
	{
		pData->SetId(GetGuild_uid());
	}
	if (IsUpdated(nameMask))
	{
		pData->SetName(GetName());
	}

	if (IsUpdated(creatorMask))
	{
		pData->SetCreator(GetCreator());		
	}

	if (IsUpdated(masterMask))
	{
		pData->SetMaster(GetMaster());
	}

	if (IsUpdated(allianceMask))
	{
		pData->SetAlliance(GetAlliance_id());
	}

	if (IsUpdated(enemyMask))
	{
		pData->SetEnemy(GetEnemy_id());
	}

	if (IsUpdated(secMasterMask))
	{
		pData->SetSecMaster(GetSecMasterName());
	}

	if (IsUpdated(levelMask))
	{
		pData->SetLevel(GetLevel());
	}

	if (IsUpdated(prosperMask))
	{
		pData->SetProsper(GetProsper(), GetMax_prosper());
	}

	if (IsUpdated(fundMask))
	{
		pData->SetFund(GetFund());
	}

	if (IsUpdated(memberNumMask))
	{
		pData->SetMemberNum(GetTotalMember(), GetMaxMember());
	}

	if (IsUpdated(aimMask))
	{
		pData->SetAim(GetAim());
	}

	if (IsUpdated(ruleMask))
	{
		pData->SetRule(GetRule());
	}

	if (IsUpdated(guildPvPScoreMask))
	{
		pData->SetGuildPvPScore(GetGuildPvPScore());
	}

	if (IsUpdated(guildPvPBidMask))
	{
		pData->SetGuildPvPBid(GetGuildPvPBid());
	}

	if (IsUpdated(guildPvPGroupMask))
	{
		pData->SetGuildPvPGroup(GetGuildPvPGroup());
	}

	if (IsUpdated(groupPvPVSGuildMask))
	{
		pData->SetGroupPvPVSGuild(GetGroupPvPVSGuild());
	}

	if (IsUpdated(manorScaleMask))
	{
		pData->SetManorScale(GetManorScale());
	}

	if (IsUpdated(manorRoomMask))
	{
		pData->SetRoom(GetRoom());
	}

	if (IsUpdated(manorMaterialMask))
	{
		pData->SetMaterial(GetMaterial());
	}

	if (IsUpdated(manorGuardMask))
	{
		pData->SetGuard(GetGuard());
	}

	if (IsUpdated(manorTrainPlayerPowMask))
	{
		pData->SetManorTrainPlayerPow(GetTrainCharExPow(), GetTrainCharInPow());
	}

	if (IsUpdated(manorTrainPartnerPowMask))
	{
		pData->SetManorTrainPartnerPow(GetTrainPartnerExPow(), GetTrainPartnerInPow());
	}

	if (IsUpdated(manorRoomPropertyMask))
	{
		pData->SetManorRoomProperty(GetDrugLv(), GetCookLv(), 
									GetWineLv(), GetHpLv(), GetSpLv());
	}

	if (IsUpdated(manorMaintainNumMask))
	{
		pData->SetMaintainNum(GetMaintainNum());
	}

	if (IsUpdated(lastMaintainDateMask))
	{
		pData->SetLastMaintainDate(GetLastMaintainDate());
	}

	if (IsUpdated(manorAuraMask))
	{
		pData->SetAura(GetAura());
	}

	if (IsUpdated(offlinetimeMask))
	{
		pData->SetOfflinetime(GetOfflinetime());
	}

	if (IsUpdated(clearBusinessDateMask))
	{
		pData->SetClearBusinessDate(GetClearBusinessDate());
	}

	if (IsUpdated(autoManageBusinessMask))
	{
		pData->SetAutoManageBusiness(GetAutoManageBusiness());
	}
}

void CGuild::ReleaseMembers()
{
	m_memberMap.clear();
}

void CGuild::CheckRelease()
{
	//check guild need to release from object service
	if (!OnlineMemberExist() && GetGuildPvPGroup() == 0)
	{
		UInt32 guild_uid = GetGuild_uid();
		
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Guild Release: guild_uid %d, member count %d"),
			guild_uid, GetTotalMember());

		CancelUniqEvent(EVT_MANOR_AURA_CHANGE);	
		CancelUniqEvent(EVT_MANOR_MAINTENANCE);	
		//release all guild member and guild
		ObjectService_Proxy::ReleaseGuildAndMem(gGlobal.pObjectService->pPeer, RPCContext(), 
			guild_uid);

		// remove guild
		gGlobal.RemoveGuild(this);
	}
}

void CGuild::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

CGuildMember* CGuild::GetMember(UInt32 char_id)
{
	GuildMemberMap::iterator it = m_memberMap.find(char_id);
	if (it != m_memberMap.end())
		return it->second;
	else
		return NULL;
}

RPCResult CGuild::DoGetMembers(RPCContext &context)
{
	PROCEDURE_START_METHOD(CGuild, DoGetMembers, this, context)

		ObjectService_Proxy::GetGuildMember(gGlobal.pObjectService->pPeer, 
			context, GetGuild_uid(), 0);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetGuildMembers, NetGroup *, guildMemGroup)
		TRACE_ENSURE(guildMemGroup != NULL);

		DeleteNetGroup(guildMemGroup, TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("DoGetMembers Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

void CGuild::PackGuildApplicant(NetGroup *grp)
{
	for (GuildApplicantMap::iterator Itr = m_applicantMap.begin(); Itr != m_applicantMap.end();
		++Itr)
	{
		Itr->second->PreSend();
		grp->Add(Itr->second);
	}
}
void CGuild::PackGuildMember(NetGroup *grp)
{
	for (GuildMemberMap::iterator Itr = m_memberMap.begin(); Itr != m_memberMap.end();
		++Itr)
	{
		Itr->second->PreSend();
		grp->Add(Itr->second);
	}
}

void CGuild::SyncUpdate()
{
	RaiseUniqEventID(EVT_SYNC_UPDATE);
}

void CGuild::DoSyncUpdate()
{
	if (!IsUpdated())
		return;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("DoSyncUpdate: Sync guild data to map and client, guild UID = %d"), GetGuild_uid());

	NetGroup guildGroup;
	NetGroup dataGroup;
	CGuildData* pData = CGuildData::NewInstance();
	UInt32Vector ids;
	RPCContext context;

	// bug fix on sequence causing 0 to send
	MarkUpdate(idMask);

	WriteData(pData);
	if (pData->IsUpdated())
	{
		pData->SetSendMask(pData->GetUpdateMask());
		dataGroup.Add(pData);
		ids.push_back(GetGuild_uid());
	}
	else
		DeleteNetObject(pData);

	SetSendMask(GetUpdateMask());
	ClearUpdateMask();
	guildGroup.Add(this);

	// sync guild to all map service
	for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
		it != gGlobal.m_mapPeers.end(); ++it)
	{
		MapService_Proxy::GuildUpdate(*it, context, &guildGroup);
	}

	if (dataGroup.GetCount() > 0)
	{
		ObjectService_Proxy::UpdateGuildList(gGlobal.pObjectService->pPeer, RPCContext(), 
			ids, &dataGroup, false);
		DeleteNetChilds(&dataGroup);
	}

	DoSyncUpdateMember(&guildGroup);
}

void CGuild::DoSyncUpdateMember(NetGroup* guildGroup)
{
	NetGroup memberGroup;
	NetGroup dataGroup;
	UInt32Vector memberIds;
	CGuildMemberData* pData;
	CGuildMember* pMember;
	RPCContext context;

	for (GuildMemberMap::iterator Itr = m_memberMap.begin(); 
		Itr != m_memberMap.end(); ++Itr)
	{
		pMember = Itr->second;
		if (pMember && pMember->IsUpdated())
		{
			pData = CGuildMemberData::NewInstance();
			pMember->WriteData(pData);
			if (pData->IsUpdated())
			{
				pData->SetSendMask(pData->GetUpdateMask());
				dataGroup.Add(pData);
				memberIds.push_back(pMember->GetChar_id());
			}
			else
				DeleteNetObject(pData);
			pMember->SetSendMask(pMember->GetUpdateMask());
			pMember->ClearUpdateMask();
			memberGroup.Add(pMember);
		}
	}

	if (dataGroup.GetCount() > 0)
	{
		UInt32Vector guildIds;
		guildIds.push_back(GetGuild_uid());
		ObjectService_Proxy::UpdateGuildMemberList(gGlobal.pObjectService->pPeer, context, 
			memberIds, guildIds, &dataGroup, false);
		DeleteNetChilds(&dataGroup);
	}

	bool guildUpdated = (guildGroup && guildGroup->GetCount() > 0);
	bool memberUpdated = (memberGroup.GetCount() > 0);

	// check if no update
	if (!guildUpdated && !memberUpdated)
		return;

	// callback update to all online members
	for (GuildMemberMap::iterator Itr = m_memberMap.begin(); 
		Itr != m_memberMap.end(); ++Itr)
	{
		pMember = Itr->second;
		if (pMember)
		{
			CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(pMember->GetChar_id());
			if (pMsgInfo && pMsgInfo->pProxyPeer)
			{
				if (guildUpdated)
					MessageService_Stub::CB_Guilds(pMsgInfo->pProxyPeer, 
						RPCContext(pMsgInfo->SID), guildGroup, false);
				
				if (memberUpdated)
					MessageService_Stub::CB_GuildMembers(pMsgInfo->pProxyPeer, 
						RPCContext(pMsgInfo->SID), &memberGroup);
			}
		}
	}

	// sync member to all map service
	if (memberUpdated)
	{
		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
			it != gGlobal.m_mapPeers.end(); ++it)
		{
			MapService_Proxy::GuildMemberUpdate(*it, context, &memberGroup);
		}
	}
}

void CGuild::SyncToMap(LPCPEER pPeer)
{
	RPCContext context;
	NetGroup guildGrp;
	NetGroup guildMemberGrp;

	if (NoMaster())
		GenerateNewGuildMaster();

	PreSend();
	guildGrp.Add(this);
	PackGuildMember(&guildMemberGrp);

	if (pPeer)
	{
		// sync to one map service only
		MapService_Proxy::GuildUpdate(pPeer, context, &guildGrp);
		MapService_Proxy::GuildMemberUpdate(pPeer, context, &guildMemberGrp);
	}
	else
	{
		// sync to all map service
		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
			it != gGlobal.m_mapPeers.end(); ++it)
		{
			MapService_Proxy::GuildUpdate(*it, context, &guildGrp);
			MapService_Proxy::GuildMemberUpdate(*it, context, &guildMemberGrp);
		}
	}
}

void CGuild::InsertApplicant(CMessageCharacter * pMsgChar)
{
	static const int MAX_APPLICANT = 50;
	CGuildApplicant * pApplicant = CGuildApplicant::NewInstance();

	pApplicant->SetData(pMsgChar);
	pApplicant->SetApplyDate((UInt32)time(NULL));

	if (m_applicantMap.size() >= MAX_APPLICANT)
	{
		GuildApplicantMap::iterator rmItr = m_applicantMap.end();
		UInt32 earliest = 0xFFFFFFFF;
		for (GuildApplicantMap::iterator Itr = m_applicantMap.begin(); 
			Itr != m_applicantMap.end(); ++Itr)
		{
			if (Itr->second->GetApplyDate() <= earliest)
			{
				rmItr = Itr;
				earliest = Itr->second->GetApplyDate();
			}
		}

		if (rmItr != m_applicantMap.end())
		{
			DeleteNetObject(rmItr->second);
			m_applicantMap.erase(rmItr);
		}
	}
	m_applicantMap.insert(std::make_pair(pApplicant->GetChar_id(), pApplicant));

	pApplicant->PreSend();
	//sync to all member
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
		Itr != m_memberMap.end(); ++Itr)
	{
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(Itr->second->GetChar_id());
		if (pMsgInfo)
			CB_AddApplicant(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), pApplicant);
	}

}

bool CGuild::CheckPostAvialble(UInt8 post)
{
	if (post == GUILD_POSITION_MASTER)
		return true;
	//if (post <= GUILD_POSITION_DEPTHEAD)
	{
		UInt16 count = 0;
		for (GuildMemberMap::iterator Itr = m_memberMap.begin();
			Itr != m_memberMap.end(); ++Itr)
		{
			if (Itr->second->GetPost() == post)
				count ++;
		}

		if (count >= GuildPostLimit[post])
			return false;
	}
	return true;
}

bool CGuild::OnlineMemberExist()
{
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
		Itr != m_memberMap.end(); ++Itr)
	{
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(Itr->second->GetChar_id());
		if (pMsgInfo)
			return true;
	}

	return false;
}

bool CGuild::HasMember(UInt32 char_id)
{
	return (m_memberMap.find(char_id) != m_memberMap.end());
}

void CGuild::MemberOnline(UInt32 char_id, Boolean bOnline)
{
	CGuildMember* pMember = GetMember(char_id);
	bool oldOnline = (pMember && pMember->GetOfflineTime() == 0);
	if (pMember && oldOnline != bOnline)
	{
		if (bOnline)
			pMember->SetOfflineTime(0);
		else
			pMember->SetOfflineTime((UInt32) ::time(NULL));
		SyncUpdate();
	}
}

RPCResult CGuild::KickMember(RPCContext &context)
{
	PROCEDURE_START_METHOD3(CCharacter, KickMember, this, context, UInt32, char_id, 
		String, reason, UInt32, kicker_id)

		UInt32Vector char_ids;
		char_ids.push_back(char_id);
		ObjectService_Proxy::DeleteGuildMemberList(gGlobal.pObjectService->pPeer, context, char_ids);

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteGuildMemberList)
		RemoveGuildMember(char_id, reason, kicker_id);
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("KickMember:char_id %d leave the guild with uid %d"), 
				char_id, GetGuild_uid());


		if (GetManorScale() > 0) // if the guild has manor, also kick the member out of manor instance if he is still located there.
		{
			UInt32Vector charIDs;	
			charIDs.push_back(char_id);
			for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
                MapService_Proxy::ManorForceOut(*ItrMap, RPCContext(), GetGuild_uid(), charIDs, BOOLEAN_TRUE);
		}
		
	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CGuild::ChangeFund(Int32 change, UInt8 source)
{
	if ((Int64)GetFund() + change < 0)
		SetFund(0);
	else
		SetFund(GetFund() + change);

	LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), GetGuild_uid(),  GetFund(), GetProsper(), GetManorScale(),
		                       source, 0);

}
void CGuild::ChangePvPScore(Int32 change)
{
	if ((Int64)GetGuildPvPScore() + change < 0)
		SetGuildPvPScore(0);
	else
		SetGuildPvPScore(GetGuildPvPScore() + change);
}

void CGuild::ChangePvPBid(Int32 change)
{
	if ((Int64)GetGuildPvPBid() + change < 0)
		SetGuildPvPBid(0);
	else
		SetGuildPvPBid(GetGuildPvPBid() + change);
}

bool CGuild::IsApplicantExist(UInt32 char_id)
{
	return m_applicantMap.find(char_id) != m_applicantMap.end();
}

RPCResult CGuild::LoadMembers(RPCContext &context)
{
	PROCEDURE_START_METHOD(CGuild, LoadMembers, this, context)
		
		ObjectService_Proxy::GetGuildMember(gGlobal.pObjectService->pPeer, context, 
			GetGuild_uid(), 0);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetGuildMembers, NetGroup *, guildMemGroup)

		UInt32Vector char_ids;
		CMessageCharacter* pMsgChar;
		CGuildMemberData *pGuildMemberData;
		CGuildMember* pGuildMember;

		TRACE_ENSURE(guildMemGroup != NULL);
		for (UInt16 i = 0; i < guildMemGroup->GetCount(); i ++)
		{
			pGuildMemberData = guildMemGroup->GetChild<CGuildMemberData>(i);
			if (pGuildMemberData)
			{
				pGuildMember = CGuildMember::NewInstance();
				pGuildMember->SetData(pGuildMemberData);
				InsertGuildMember(pGuildMember);
				pMsgChar = gGlobal.GetMsgCharRef(pGuildMemberData->GetChar_id(), true);
				if (pMsgChar == NULL)
				{
					// get MsgCharacter from object service if not exist
					char_ids.push_back(pGuildMember->GetChar_id());
				}
				else
				{
					// directly use current MsgCharacter
					pGuildMember->SetData(pMsgChar);
					if (pMsgChar->GetGuild_uid() != GetGuild_uid())
					{
						pMsgChar->SetGuild(GetGuild_uid(), GetName());
						pMsgChar->UpdateObject();
					}
				}
			} 
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_GetGuildMembers: guild_uid %d, char_id %d"), 
				pGuildMemberData->GetGuild_uid(), pGuildMemberData->GetChar_id());
		}
		DeleteNetGroup(guildMemGroup, TRUE);

		if (char_ids.empty())
		{
			NetGroup grp;
			PROCEDURE_GO_STEP1(2, NetGroup*, &grp);
		}
		else
		{
			ObjectService_Proxy::GetMessageCharacters(gGlobal.pObjectService->pPeer, 
				context, char_ids, BOOLEAN_FALSE);
		}

	PROCEDURE_WAIT1(2, ObjectService, CB_GetMsgCharacterGroup, NetGroup *, charGroup)

		TRACE_ENSURE(charGroup != NULL);
		for (UInt16 i = 0; i < charGroup->GetCount(); i ++)
		{
			CMsgCharacterData *pMsgCharData = charGroup->GetChild<CMsgCharacterData>(i);
			if (pMsgCharData)
			{
				gGlobal.AddMsgCharRef(pMsgCharData->GetChar_id(), pMsgCharData);
				CGuildMember* pMember = GetMember(pMsgCharData->GetChar_id());
				if (pMember)
					pMember->SetData(pMsgCharData);
			}
		}

		//send to messengers all map service 
		NetGroup guildGrp;
		NetGroup guildMemberGrp;
	
		if (NoMaster())
			GenerateNewGuildMaster();

		// check total members
		if (GetTotalMember() != (UInt16) m_memberMap.size())
		{
			SetTotalMember((UInt16) m_memberMap.size());
			SyncUpdate();
		}

		PreSend();
		guildGrp.Add(this);
		PackGuildMember(&guildMemberGrp);

		for (PeerSet::iterator it = gGlobal.m_mapPeers.begin(); 
			it != gGlobal.m_mapPeers.end(); ++it)
		{
			MapService_Proxy::GuildUpdate(*it, RPCContext(), &guildGrp);
			MapService_Proxy::GuildMemberUpdate(*it, RPCContext(), &guildMemberGrp);
		}
		DeleteNetGroup(charGroup, TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH
	
		return RPC_RESULT_OK; 
}

RPCResult CGuild::AddMember(RPCContext &context)
{
	PROCEDURE_START_METHOD2(CCharacter, AddMember, this, context, UInt32, master_id, 
		UInt32, char_id)

		CMessageCharacter* pMsgChar = gGlobal.GetMsgCharRef(char_id);
		if (pMsgChar && pMsgChar->HasGuild() || HasMember(char_id))
		{
			LPCPEER pPeer = GetProxy(context.sessionId.serviceId);
			MessageService_Stub::CB_Error(pPeer, context, 0 , "MSG_ALREADY_TARGET_HAVE_GUILD");
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		ObjectService_Proxy::CreateGuildMember(gGlobal.pObjectService->pPeer, context, char_id, 
			GetGuild_uid(), GUILD_POSITION_MEMBER);
	
	PROCEDURE_WAIT1(1, ObjectService, CB_CreateGuildMember, NetObject*, guildMemData)

		TRACE_ENSURE(guildMemData);

		if (HasMember(char_id))
		{
			LPCPEER pPeer = GetProxy(context.sessionId.serviceId);
			MessageService_Stub::CB_Error(pPeer, context, 0, "MSG_ALREADY_TARGET_HAVE_GUILD");
			DeleteNetObject(guildMemData);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CGuildMemberData *pGuildMemberData = CastNetObject<CGuildMemberData>(guildMemData);
		CGuildMember* pMember = CGuildMember::NewInstance();
		pMember->SetData(pGuildMemberData);
		DeleteNetObject(pGuildMemberData);
		InsertGuildMember(pMember);
		// increase member number
		if (GetTotalMember() != (UInt16)m_memberMap.size())
		{
			SetTotalMember((UInt16)m_memberMap.size());
			SyncUpdate();
		}

		//find client
		CMessageCharacter* pMsgChar = gGlobal.GetMsgCharRef(char_id, true);
		if (pMsgChar)
		{
			NetGroup dummy;
			PROCEDURE_GO_STEP1(2, NetGroup*, &dummy)
		}
		else
		{
			//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GetMessageCharacterByIdList: char_id %d, group count %d"), char_id, char_ids.size());
			UInt32Vector char_ids;
			char_ids.push_back(char_id);
			ObjectService_Proxy::GetMessageCharacters(gGlobal.pObjectService->pPeer, context,
				char_ids, BOOLEAN_FALSE);
		}
	
	PROCEDURE_WAIT1(2, ObjectService, CB_GetMsgCharacterGroup, NetGroup *, charGroup)

		TRACE_ENSURE(charGroup != NULL);
		CGuildMember* pMember = GetMember(char_id);
		if (pMember == NULL)
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("[member not found], guild_uid %d, char_id %d"),
				GetGuild_uid(), char_id);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CMsgCharacterData *pMsgCharData = NULL;
		CMessageCharacter *pMsgChar = NULL;
		if (charGroup->GetCount() > 0)
		{
			pMsgCharData = charGroup->GetChild<CMsgCharacterData>(0);
			if (pMsgCharData)
				pMsgChar = gGlobal.AddMsgCharRef(char_id, pMsgCharData);
		}

		if (pMsgChar == NULL)
			pMsgChar = gGlobal.GetMsgCharRef(char_id);

		if (pMsgChar)
		{
			pMsgChar->SetGuild(GetGuild_uid(), GetName());
			pMsgChar->UpdateObject();
		}

		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo && pMsgInfo->pMsgChar)
		{
			pMember->SetData(pMsgInfo->pMsgChar);
			pMember->PreSend();
			SetOwnObject(pMsgInfo->SID, pMember);
			SendGuild(pMsgInfo, pMember);
		}
		else if (pMsgCharData)
		{
			pMember->SetData(pMsgCharData);
			pMember->PreSend();
		}
		else
		{
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("[msgChar not found], guild_uid %d, char_id %d"),
				GetGuild_uid(), char_id);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		//sync to all member
		for (GuildMemberMap::iterator Itr = m_memberMap.begin();
			Itr != m_memberMap.end(); ++Itr)
		{
			CharMsgInfo* pSubMsgInfo = gGlobal.GetMsgInfo(Itr->first);
			if (pSubMsgInfo)
				CB_AddMember(pSubMsgInfo->pProxyPeer, RPCContext(pSubMsgInfo->SID), pMember);
		}

		//sync to map
		for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); 
			ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
		{
			NetGroup Grp;
			pMember->PreSend();
			Grp.Add(pMember);
			MapService_Proxy::GuildMemberUpdate(*ItrMap, RPCContext(), &Grp);
		}

		DeleteNetGroup(charGroup, TRUE);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			//CB_Error(pchar->GetPeer(), context, err, errMsg);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

void CGuild::SendGuild(CharMsgInfo* pMsgInfo, CGuildMember* pMember)
{
	if (pMsgInfo == NULL || pMsgInfo->pMsgChar == NULL || pMsgInfo->pMsgChar->IsOffline())
		return;

	RPCContext context(pMsgInfo->SID);
	NetGroup grp;

	PreSend();
	grp.Add(this);
	MessageService_Stub::CB_Guilds(pMsgInfo->pProxyPeer, context, &grp, false);

	grp.Clear();
	PackGuildMember(&grp);
	MessageService_Stub::CB_GuildMembers(pMsgInfo->pProxyPeer, context, &grp);

	if (pMember && pMember->GetPost() <= GUILD_POSITION_SECMASTER)
	{
		grp.Clear();
		PackGuildApplicant(&grp);
		MessageService_Stub::CB_RequestApplicant(pMsgInfo->pProxyPeer, context, &grp);
	}

	String channelName;
	channelName.Format(_T("%s_%d"), ChannelPrefix[CHANNELTYPE_GUILD], GetGuild_uid());

	MessageService_Stub::JoinMessageChannel(pMsgInfo->pProxyPeer, context,
		pMsgInfo->pMsgChar->GetChar_id(), pMsgInfo->pMsgChar->GetNickName(),
		channelName, CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);

}

void CGuild::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_SYNC_UPDATE:				// update guild and guild members
		if (IsUpdated())
			DoSyncUpdate();
		else
			DoSyncUpdateMember();
		break;
	case EVT_MANOR_MAINTENANCE:			
		{
			const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
			if (!manorData)
				return;	
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Guild Manor Maintenance Event before recalculation: guild_uid %d, manor scale %d, failed payment=%d"), GetGuild_uid(), GetManorScale(), GetOweMaintainPayNum());
			Boolean mainSuccess = BOOLEAN_FALSE;
			if (GetFund() >= MIN_MANOR_DONATE)// Step 1: deduct the maintenance fee if possible, then reset the last paymenet date
			{
				SetFund(GetFund() - manorData->manor_maintainCost);
				SetLastMaintainDate((UInt32) time(NULL));
				SetMaintainNum(MIN(GetMaintainNum() + 1, manorData->manor_maxMaintain)); // check if how to finds the max number maintenance
				SetProsper(MIN(GetProsper() +  manorData->manor_prosperIncome, manorData->manor_maxProsper));	
				mainSuccess = BOOLEAN_TRUE;
				LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), GetGuild_uid(),  GetFund(), GetProsper(), GetManorScale(),
					              LogService_Proxy::Guild_Fund::SOURCE_MANOR, LogService_Proxy::Guild_Prosper::SOURCE_MANOR);
			}
			if (GetOweMaintainPayNum() >= MAX_FAIL_MANOR_MAINTAIN) // Step 2: check if it is needed for manor scale down
			{
				if (GetManorScale() >= 1)
					UpdateManorScale(GetManorScale() - 1);
				else
					UpdateManorScale(0);	
				SetLastMaintainDate((UInt32) time(NULL));	
			}
			if (GetManorScale() > 0)
			{
				SetFailMantainPayNum(MIN(GetOweMaintainPayNum(), MAX_FAIL_MANOR_MAINTAIN));
				RaiseUniqEventID(EVT_MANOR_MAINTENANCE, 1000*gConfigure.maintainTime);	
			}
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Guild Manor Maintenance Event after recalculation: guild_uid %d, manor scale %d, failed payment=%d"), GetGuild_uid(), GetManorScale(), GetOweMaintainPayNum());
			if (mainSuccess)
			{
				String msg;
				msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MANOR_MAINTAIN_SUCCESS")), ((GetProsper() < manorData->manor_maxProsper)? manorData->manor_prosperIncome : 0));
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("EVT_MANOR_MAINTENANCE Event: Send the manor maintanence to guild UID = %d"), GetGuild_uid());
				SendGuildMessage(msg);
			}
			else
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("EVT_MANOR_MAINTENANCE Event: Send the manor maintanence to guild UID = %d"), GetGuild_uid());
				SendGuildMessage(gGlobal.GetStringTable()->Get(_T("MSG_MANOR_MAINTAIN_FAIL")));
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("EVT_MANOR_MAINTENANCE Event: Finish manor maintenance, now syn to map and client, guildUID = %d"), GetGuild_uid());
			SyncUpdate();
			
		}
		break;
	case EVT_MANOR_AURA_CHANGE:
		{
			SetAura(gGlobal.m_rand.GetRandom(5));
			RaiseUniqEventID(EVT_MANOR_AURA_CHANGE, 1000*(24*60*60));
			SyncUpdate();	
		}
		break;
	case EVT_CLEAR_BUSINESS_LOG:			
		{
			for (GuildMemberMap::iterator Itr = m_memberMap.begin();
				Itr != m_memberMap.end(); ++Itr)
			{
				if (Itr->second->GetBusinessCount() > 0)
				{
					Itr->second->SetBusiness(0, 0);
				}
			}
			UInt32 uTime = (UInt32) time(NULL) + 86400 * 8;
			time_t ttime = (time_t)uTime;
			struct tm *newtime = localtime(&ttime);
			newtime->tm_hour	= 0;
			newtime->tm_min		= 0;
			newtime->tm_sec		= 0;
			uTime = (UInt32)mktime(newtime);
			SetClearBusinessDate(uTime);
			RaiseUniqEventID(EVT_CLEAR_BUSINESS_LOG, uTime - (UInt32) time(NULL));
			SyncUpdate();
		}
		break;
	default:
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("[wrong event id], guild_uid %d, event_id %d"),
				GetGuild_uid(), event.GetID());
		}
	}
}

String CGuild::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", guild_uid %d, total member %d"), GetGuild_uid(), GetTotalMember());
	return s;
}

void CGuild::InitManor()
{
	if (GetManorScale() == 0)
		return;
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	/// Part A => deduct the maintanence payment
	///(I) find how many maintenance payment date that the Guild missed during offline time
    // Step 1: find the next payment date after offline
	UInt32 now = (UInt32) time(NULL);
	// Step 2: find next pay date right after offtime
	UInt32 nextDate;
	Boolean abnormalOffline = BOOLEAN_TRUE;
	if (GetLastMaintainDate() < GetOfflinetime())
	{
        nextDate = GetOfflinetime() + 
				   gConfigure.maintainTime  - (GetOfflinetime() - GetLastMaintainDate())%gConfigure.maintainTime; 
		abnormalOffline = BOOLEAN_FALSE;
	}
	else
		nextDate = GetLastMaintainDate() + gConfigure.maintainTime; 
	if (abnormalOffline)
		TRACE_INFODTL(GLOBAL_LOGGER, _F("InitManor: The last maintain date is greater than offlinetime. Last maintain date is used for initialization"));


	UInt32 noPayDays = 0;
	if (now >= nextDate) // if already exceed the first maintenance payment date right after the guild become offline
		noPayDays = ((now - nextDate)/gConfigure.maintainTime) + 1; // calculate how many payment the guild has missed during offline

	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("InitManor start: guild_uid %d, manor scale %d, payment missing(noPayDays)=%d, failed payment=%d, fund=%d"), 
										GetGuild_uid(), GetManorScale(), noPayDays, GetOweMaintainPayNum(), GetFund());

	///(II) if some payment date have been missed during offtime, calculate the max number of the missed payment that player can afford to settle, 
	/// and set the last payment date + settle the payment
	if ((noPayDays > 0) && (GetFund() >= MIN_MANOR_DONATE))
	{
		UInt32 maxDaysCanPay = (GetFund() - MIN_MANOR_DONATE)/manorData->manor_maintainCost + 1; 
		UInt32 actualPayDays = (noPayDays > maxDaysCanPay)? maxDaysCanPay : noPayDays;
		UInt32 lastMaintainDate = (actualPayDays - 1)*gConfigure.maintainTime + nextDate;
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("InitManor: guild_uid %d, max payments the guild can pay = %d, actual payment need to pay = %d, Total Guild Fund = %d"), GetGuild_uid(), maxDaysCanPay, actualPayDays, GetFund());	
		SetLastMaintainDate(lastMaintainDate);
		SetFund(GetFund() - actualPayDays*manorData->manor_maintainCost);
		SetProsper(MIN(GetProsper() + actualPayDays*manorData->manor_prosperIncome, manorData->manor_maxProsper)); 
		SetMaintainNum(MIN(GetMaintainNum() + actualPayDays, manorData->manor_maxMaintain)); //must check if the max number of success maintenance payment comes form csv
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitManor: guild_uid %d, fund after the payment=%d, "), GetGuild_uid(), GetFund());
		LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), GetGuild_uid(),  GetFund(), GetProsper(), GetManorScale(),
							       LogService_Proxy::Guild_Fund::SOURCE_MANOR, LogService_Proxy::Guild_Prosper::SOURCE_MANOR);
	}
	///(III) check if any scale down is needed
	UInt16 numScaleDown = GetOweMaintainPayNum()/MAX_FAIL_MANOR_MAINTAIN;
	if (numScaleDown > 0)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitManor: guild_uid %d, need to downgrade scales=%d"), GetGuild_uid(), numScaleDown);	
		if (numScaleDown <= GetManorScale())
		{
			UInt32 lastScaleDownDate = GetLastMaintainDate() + MAX_FAIL_MANOR_MAINTAIN * numScaleDown * gConfigure.maintainTime;
			SetLastMaintainDate(lastScaleDownDate); // set new maintenance pay date == the date of last manor scale down
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitManor: guild_uid %d, the number of failed payment after downgrade=%d"), GetGuild_uid(), GetOweMaintainPayNum());		
			UpdateManorScale(GetManorScale() - numScaleDown);
		}
		else
			UpdateManorScale(0);	
	}
	///(IV) recalculate how many payment that a player have missed.
	if (GetManorScale() > 0)
	{
		SetFailMantainPayNum(GetOweMaintainPayNum());

		/// Part B => set the event 
		if (now < GetLastMaintainDate())
			return;
		// Step 1: raise the event of maintenance
		UInt32 nextMaintainTime = gConfigure.maintainTime -
								(now - GetLastMaintainDate())%gConfigure.maintainTime; 
		RaiseUniqEventID(EVT_MANOR_MAINTENANCE, 1000*nextMaintainTime);
		// Step 2: raise the event of assigning the "Aura" value
		if (now >= FindNextResetAuraTime(GetOfflinetime()))
			SetAura(gGlobal.m_rand.GetRandom(5));
		RaiseUniqEventID(EVT_MANOR_AURA_CHANGE, 1000 * (FindNextResetAuraTime(now) - now));
	}
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("InitManor Result: guild_uid %d, the number of failed payment=%d, scale=%d "), GetGuild_uid(), GetOweMaintainPayNum(), GetManorScale());		
	SyncUpdate();

}

UInt32 CGuild::GetOweMaintainPayNum()
{

	UInt32 now = (UInt32) time(NULL);
	if ((GetLastMaintainDate() != 0) && (now >= GetLastMaintainDate()))
		return ((now - GetLastMaintainDate())/gConfigure.maintainTime);
	else
		return 0;
}

void CGuild::UpdateManorScale(UInt8 targetScale)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("UpdateManorScale: guild_uid %d, initial scale=%d, failed payment =%d"), GetGuild_uid(), GetManorScale(), GetOweMaintainPayNum());			
	if (GetManorScale() == targetScale)
		return;
	UInt8 iniScale = GetManorScale();
	Boolean downScale = BOOLEAN_FALSE;
	if (GetManorScale() > targetScale) // if down scale
	{	
		const ManorData *manorData = NULL;
		if (targetScale > 0 && targetScale <=5)
		{
			manorData = gGlobal.m_resource->GetManorData(targetScale);
			if (!manorData)
				return;
		}
		UInt8 maxTargetRoom = manorData? manorData->manor_maxRoomNum : 0;
		UInt32 maxPropsper = manorData? manorData->manor_maxProsper : 0;	
		UInt8 maxCharEx = manorData? manorData->manor_maxCharEx : 0;
		UInt8 maxCharIn = manorData? manorData->manor_maxCharIn : 0;
		UInt8 maxParEx = manorData? manorData->manor_maxPartnerEx : 0;
		UInt8 maxParIn = manorData? manorData->manor_maxPartnerIn : 0;
		UInt8 maxDrug = manorData? manorData->manor_maxDrug : 0;
		UInt8 maxCook = manorData? manorData->manor_maxCook : 0;
		UInt8 maxWine = manorData? manorData->manor_maxWine : 0;
		UInt8 maxHp = manorData? manorData->manor_maxHp : 0;
		UInt8 maxSp = manorData? manorData->manor_maxSp : 0;
		UInt8 maxFund = manorData? manorData->manor_maxFund : MAX_GUILD_FUND;
		UInt8 maxGuard = manorData? manorData->manor_maxGuard : 0;
		// reduce the manor room
		SetManorRoom(MIN(GetRoom(), maxTargetRoom));
		// reduce the prosper
		SetProsper(MIN(GetProsper(), maxPropsper)); 
		SetTrainCharExPow(MIN(GetTrainCharExPow(), maxCharEx));
		SetTrainCharInPow(MIN(GetTrainCharInPow(), maxCharIn));
		SetTrainPartnerExPow(MIN(GetTrainPartnerExPow(), maxParEx));
		SetTrainPartnerInPow(MIN(GetTrainPartnerInPow(), maxParIn));
		SetDrugLv(MIN(GetDrugLv(), maxDrug));
		SetCookLv(MIN(GetCookLv(), maxCook));
		SetWineLv(MIN(GetWineLv(), maxWine));
		SetHpLv(MIN(GetSpLv(), maxHp));
		SetSpLv(MIN(GetSpLv(), maxSp));
		SetFund(MIN(GetFund(), maxFund));
		SetGuard(MIN(GetGuard(), maxGuard));
		downScale = BOOLEAN_TRUE;
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UpdateManorScale: guild_uid %d has reset its value after downgrade"), GetGuild_uid());			
	}
	else
	{
		String msg;
		if (targetScale != 1)
            msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MANOR_RUMOR_UPGRADE")), GetName().c_str(), targetScale);
		else
		{
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MANOR_CREATE")), GetName().c_str());
			SetAura(gGlobal.m_rand.GetRandom(5));
		}
		Peer temp;
		MessageService_Stub::SendRumorMessage(&temp, RPCContext(), msg, 0, 0);
	}
	SetManorScale(targetScale);
	SetMaintainNum(0);
	// cancel the event if the manor is downgrade to 0
	if ((targetScale == 0) || (targetScale > 5))
	{
		CancelUniqEvent(EVT_MANOR_AURA_CHANGE);	
		CancelUniqEvent(EVT_MANOR_MAINTENANCE);		
	}
	// force out all online guild members from current manor instance
	UInt32Vector charIDs;	
	for (GuildMemberMap::iterator Itr = m_memberMap.begin();
		Itr != m_memberMap.end(); ++Itr)
	{
		if (gGlobal.GetMsgInfo(Itr->second->GetChar_id()) != NULL)
            charIDs.push_back(Itr->second->GetChar_id());
	}
	// call map service to kick people out from manor
	if (charIDs.size() > 0)
	{
		for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
			MapService_Proxy::ManorForceOut(*ItrMap, RPCContext(), GetGuild_uid(), charIDs, BOOLEAN_FALSE);
	}
	if (downScale)
	{
        SendGuildMessage(String(gGlobal.GetStringTable()->Get(_T("MSG_MANOR_DOWNGRADE"))));
	}
	else
	{
		String upMsg, gradeMsg;
		gradeMsg.Format(_T("MSG_MANOR%d"), targetScale); 
		upMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_MANOR_UPGRADE")), gGlobal.GetStringTable()->Get(gradeMsg.c_str()));
		SendGuildMessage(upMsg);
	}
	UInt8 fundSource = downScale? LogService_Proxy::Guild_Fund::SOURCE_MANOR : 0;
	UInt8 prosperSource = downScale? LogService_Proxy::Guild_Prosper::SOURCE_MANOR : 0;

	LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), GetGuild_uid(),  GetFund(), GetProsper(), targetScale,
						       fundSource, prosperSource);

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("UpdateManorScale: guild_uid %d, result scale=%d, failed payment =%d"), GetGuild_uid(), GetManorScale(), GetOweMaintainPayNum());			

}

UInt32 CGuild::FindNextResetAuraTime(UInt32 fromTime)
{
	UInt32 resetPoint = gConfigure.auraTime; // the time chosen for reseting "Aura" values 
	struct tm *newtime = localtime((time_t*)&fromTime);
	UInt32 currentTime = newtime->tm_hour * 60 * 60 + newtime->tm_min * 60 + newtime->tm_sec;
	// find the next reset point (in exact date with year, month, hour ...etc)
	if (currentTime >= resetPoint)
		resetPoint = ((60*60*24 - currentTime) + resetPoint) + fromTime;
	else
		resetPoint = (resetPoint - currentTime) + fromTime; 
	
	return resetPoint;
}

void CGuild::ChangeProsper(Int32 change, UInt8 source)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;		
	if ((Int64)GetProsper() + change < 0)
		SetProsper(0);
	else
		SetProsper(MIN(GetProsper() + change, manorData->manor_maxProsper));
	
	LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(),GetGuild_uid(),  GetFund(), GetProsper(), GetManorScale(),
		                       0, source);

}

void CGuild::ChangeMaterial(Int32 change)
{
	if ((Int64)GetMaterial() + change < 0)
		SetManorMaterial(0);
	else
		SetManorMaterial(GetMaterial() + change);
}

void CGuild::ChangeRoom(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetRoom() + change < 0)
		SetRoom(0);
	else
		SetRoom(MIN(GetRoom() + change,manorData->manor_maxRoomNum));
}

void CGuild::ChangeTrainCharEx(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetTrainCharExPow() + change < 0)
		SetTrainCharExPow(0);
	else
		SetTrainCharExPow(MIN(GetTrainCharExPow() + change, manorData->manor_maxCharEx));

}
void CGuild::ChangeTrainCharIn(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetTrainCharInPow() + change < 0)
		SetTrainCharInPow(0);
	else
		SetTrainCharInPow(MIN(GetTrainCharInPow() + change, manorData->manor_maxCharIn));
}

void CGuild::ChangeTrainPartnerEx(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetTrainPartnerExPow() + change < 0)
		SetTrainPartnerExPow(0);
	else
		SetTrainPartnerExPow(MIN(GetTrainPartnerExPow() + change, manorData->manor_maxPartnerEx));

}

void CGuild::ChangeTrainPartnerIn(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetTrainPartnerInPow() + change < 0)
		SetTrainPartnerInPow(0);
	else
		SetTrainPartnerInPow(MIN(GetTrainPartnerInPow() + change, manorData->manor_maxPartnerIn));

}

void CGuild::ChangeDrug(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetDrugLv() + change < 0)
		SetDrugLv(0);
	else
		SetDrugLv(MIN(GetDrugLv() + change, manorData->manor_maxDrug));

}

void CGuild::ChangeCook(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetCookLv() + change < 0)
		SetCookLv(0);
	else
		SetCookLv(MIN(GetCookLv() + change, manorData->manor_maxCook));

}

void CGuild::ChangeWine(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetWineLv() + change < 0)
		SetWineLv(0);
	else
		SetWineLv(MIN(GetWineLv() + change, manorData->manor_maxWine));

}

void CGuild::ChangeManorHP(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetHpLv() + change < 0)
		SetHpLv(0);
	else
		SetHpLv(MIN(GetHpLv() + change, manorData->manor_maxHp));

}

void CGuild::ChangeManorSP(Int8 change)
{
	const ManorData *manorData = gGlobal.m_resource->GetManorData(GetManorScale());
	if (!manorData)
		return;	
	if (GetSpLv() + change < 0)
		SetSpLv(0);
	else
		SetSpLv(MIN(GetSpLv() + change,manorData->manor_maxSp));

}

void CGuild::ChangeAutoManageBusiness(Boolean bOpen)
{
	if (GetAutoManageBusiness() != bOpen)
	{
		SetAutoManageBusiness(bOpen);
		if (bOpen)
		{
			for (GuildMemberMap::iterator Itr = m_memberMap.begin(); Itr != m_memberMap.end(); ++Itr)
			{
				if(Itr->second->GetPost() == GUILD_POSITION_TRADER)
				{
					Itr->second->SetPost(GUILD_POSITION_NIL);
					Itr->second->SetPost(GUILD_POSITION_TRADER);
					Itr->second->RaiseNoTradeEvent();
				}
			}
		}
		SyncUpdate();
	}
}



   


