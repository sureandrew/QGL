#include "Common.h"
#include "Common/Procedure.h"
#include "Proxy/Data/CharacterData.h"
#include "Proxy/Logic/Character.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Proxy/Data/MsgCharacterData.h"
#include "Stub/Logic/Guild.h"
#include "Stub/Logic/MessageCharacter.h"
#include "Stub/Logic/GuildMember.h"
#include "Stub/Service/MessageService.h"
#include "Common/datacode.h"

#include "MessageService/Global.h"
#include "MessageService/MessageApplication.h"
#include "Reuben/Simulation/ScheduleManager.h"

REGISTER_CLASSTYPE(CMessageCharacter, MessageCharacter_CLASSID);


CMessageCharacter::CMessageCharacter()
{
	for (Int i = 0; i < MAX_CHANNELTYPE; i++)
	{
		m_speakRights[i] = true;
	}

	SetEnableBlockMsg(FALSE);
	m_firstRequest = FALSE;
	m_refCount = 0;
}

CMessageCharacter::~CMessageCharacter()
{
	m_requestGuildList.clear();
}

void CMessageCharacter::WriteData(CMsgCharacterData * pData)
{
	SetId(GetChar_id());

	if (IsModified(friend_statusMask))
	{
		pData->SetFriend_status(GetFriend_status());
	}

	if (IsModified(friend_iconMask))
	{
		pData->SetFriend_icon(GetFriend_icon());
	}

	if (IsModified(friend_aboutMask))
	{
		pData->SetFriend_about(GetFriend_about());
	}

	if (IsModified(friend_replyMask))
	{
		pData->SetFriend_reply(GetFriend_onlineAutoReply(), GetFriend_offlineAutoReply());
	}

	if (IsModified(speakCountMask))
	{
		pData->SetSpeakCount(GetGlobalSpeakCount());
	}

	if (IsModified(speakBlockMask))
	{
		pData->SetSpeakBlock(GetSpeakBlock());
	}

	if (IsModified(titleMask))
	{
		pData->SetTitle(GetTitle());
	}

	if (IsModified(guildMask))
	{
		pData->SetGuild_uid(GetGuild_uid());
	}
	
	if (IsModified(GenerationAccMask))
	{
		pData->SetUiHisGenAcc(GetUiHisGenAcc());
	}
}

void CMessageCharacter::SetData(CMsgCharacterData * pData, bool bClear)
{
	if (pData->IsUpdated(CMsgCharacterData::idMask))
	{
		SetId(pData->GetChar_id());
	}

	if (pData->IsUpdated(CMsgCharacterData::infoMask))
	{
		UInt8 SexClass = (pData->GetCclass() > 0 ? 
			((pData->GetCclass() - 1) * MAX_SEXTYPE + pData->GetSex() + 1) : 0);
		SetInfo(pData->GetNickName(), pData->GetSex(), pData->GetCclass(), SexClass, pData->GetFaction()); 
	}

	if (pData->IsUpdated(CMsgCharacterData::levelMask))
	{
		SetLevel(pData->GetLevel());
	}

	if (pData->IsUpdated(CMsgCharacterData::titleMask))
	{
		SetTitle(pData->GetTitle());
	}
	
	if (pData->IsUpdated(CMsgCharacterData::styleMask))
	{
		SetStyle(pData->GetPhoto_id(), pData->GetHairStyle1(), pData->GetHairStyle2(), 
			pData->GetHairColor());
	}

	if (pData->IsUpdated(CMsgCharacterData::styleMask))
	{
		SetStyle(pData->GetPhoto_id(), pData->GetHairStyle1(), pData->GetHairStyle2(), 
			pData->GetHairColor());
	}

	if (pData->IsUpdated(CMsgCharacterData::eqWeaponMask)) {
		SetEqWeapon(pData->GetEq_weapon(), pData->GetEq_weaponColor());
	}
	if (pData->IsUpdated(CMsgCharacterData::eqHeadMask)) {
		SetEqHead(pData->GetEq_headBand(), pData->GetEq_headColor());
	}
	if (pData->IsUpdated(CMsgCharacterData::eqClothMask)) {
		SetEqCloth(pData->GetEq_cloth(), pData->GetEq_clothColor());
	}

	if (pData->IsUpdated(CMsgCharacterData::orgEqClothMask))
	{
		SetOrgEqCloth(pData->GetOrg_eq_cloth(), pData->GetOrg_eq_clothColor());
	}
	if (pData->IsUpdated(CMsgCharacterData::orgHairStyleMask))
	{
		SetOrgHairStyle(pData->GetOrg_hairStyle1(), pData->GetOrg_hairColor());
	}
	if (pData->IsUpdated(CMsgCharacterData::friend_statusMask))
	{
		SetFriend_status(pData->GetFriend_status());
	}

	if (pData->IsUpdated(CMsgCharacterData::friend_iconMask))
	{
		SetFriend_icon(pData->GetFriend_icon());
	}

	if (pData->IsUpdated(CMsgCharacterData::friend_aboutMask))
	{
		SetFriend_about(pData->GetFriend_about());
	}

	if (pData->IsUpdated(CMsgCharacterData::friend_replyMask))
	{
		SetFriend_reply(pData->GetFriend_onlineAutoReply(), pData->GetFriend_offlineAutoReply());
	}

	if (pData->IsUpdated(CMsgCharacterData::speakCountMask))
	{
		SetSpeakCount(pData->GetGlobalSpeakCount());
	}

	if (pData->IsUpdated(CMsgCharacterData::speakBlockMask))
	{
		time_t now = time(NULL);
		if ((UInt32)now >= pData->GetSpeakBlock())
			SetSpeakBlock(0);
		else
			SetSpeakBlock(pData->GetSpeakBlock());
	}

	if (pData->IsUpdated(CMsgCharacterData::offlineTimeMask))
	{
		SetOfflineTime(pData->GetOfflineTime());
	}

	if (pData->IsUpdated(CMsgCharacterData::guild_uidMask))
	{
		SetGuild_uid(pData->GetGuild_uid());
	}

	if (pData->IsUpdated(CMsgCharacterData::lineMask))
		SetLine(pData->GetLine_id());

	if (pData->IsUpdated(CMsgCharacterData::BattleInfoMask))
		SetBattleInfo(pData->GetEnablePK());

	if (pData->IsUpdated(CMsgCharacterData::SettingInfoMask))
		SetSettingInfo(pData->GetEnableBlockMakeFriend(), pData->GetEnableBlockMsg());

	if (pData->IsUpdated(CMsgCharacterData::moneyMask))
		SetMoney(pData->GetMoney());

	if (pData->IsUpdated(CMsgCharacterData::inactiveMask))
		SetInactive(pData->GetInactive());

	if (pData->IsUpdated(CMsgCharacterData::GenerationAccMask))
		SetUiHisGenAcc(pData->GetUiHisGenAcc());

	if (bClear)
		ClearUpdateMask();
}

RPCResult CMessageCharacter::SelfSettingChange(LPCPEER pPeer, RPCContext &context, const String &about, const UInt8 &OnlineState, const String &Reply)
{
	if (about.size() <= 80)
	{
		String modifyAbout(about);
		RemoveLink(modifyAbout);
		SetFriend_about(modifyAbout);
	}
	if (Reply.size() <= 80)
	{
		String modifyReply(Reply);
		RemoveLink(modifyReply);
		SetFriend_reply(modifyReply, modifyReply);
	}
	SetFriend_status(OnlineState);

	SendAttr(pPeer, context, GetUpdateMask());
	
	UpdateObject();
	
	//ClearUpdateMask();
	return RPC_RESULT_OK;
}

void CMessageCharacter::PreSend()
{
	if (GetInactive())
	{
		RPCMask mask;
		mask.MaskBits(idMask, infoMask, inactiveMask);
		SetSendMask(mask);
	}
	else
	{
		SetSendMask(CMessageCharacter::AllMask);
	}
}

void CMessageCharacter::ChangeStatus(UInt8 status)
{
	if (status != GetFriend_status())
	{
		SetFriend_status(status);
		if (status == FRIENC_CHAT_STATUS_OFFLINE)
		{
			SetOfflineTime((UInt32) ::time(NULL));
			UpdateObject();
		}
	}
}

void CMessageCharacter::UpdateObject()
{
	RaiseUniqEventID(EVT_UPDATE);
}

void CMessageCharacter::DoUpdateObject()
{
	if (IsModified())
	{
		//only update speak count
		CMsgCharacterData Data;
		WriteData(&Data);
		Data.SetSendMask(Data.GetUpdateMask());

		ObjectService_Proxy::UpdateMsgCharacter(gGlobal.pObjectService->pPeer, RPCContext(), 
		GetChar_id(), &Data);
		//sync to map
		NetGroup Grp;
		SetSendMask(GetUpdateMask());
		Grp.Add(this);
		for (PeerSet::iterator ItrMap = gGlobal.m_mapPeers.begin(); ItrMap != gGlobal.m_mapPeers.end(); ++ItrMap)
		{
			MapService_Proxy::MsgCharacterUpdate(*ItrMap, RPCContext(), &Grp);
		}

		ClearModifyMask();
	}
}

RPCResult CMessageCharacter::UpdateInfo(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CMessageCharacter, UpdateInfo, this, context, NetObject*, pData)

		TRACE_ENSURE(pData);
		CMsgCharacterData* pMsgData = CastNetObject<CMsgCharacterData>(pData);
		if (pMsgData)
		{
			/// syn with guild member data if any ////
			GuildMap::iterator Itr = gGlobal.m_GuildMap.find(GetGuild_uid());
			if ((Itr != gGlobal.m_GuildMap.end()) && Itr->second != NULL) 
			{
				CGuild* pGuild = Itr->second;
				if (pGuild)
				{
					CGuildMember* pMember = pGuild->GetMember(GetChar_id());
                    if (pMember)
					{
                        if (pMember->SetData(pMsgData, false))
                            pGuild->SyncUpdate();
					}
				}
			}
			/////
			SetData(pMsgData, false);
			UpdateObject();
			gGlobal.OnUpdateCharStatus(this);
		}
		DeleteNetObject(pData);

	PROCEDURE_END

	return RPC_RESULT_OK;
}

void CMessageCharacter::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_SYNC_GUILD:	
		{
			UInt i = 0;

			NetGroup grp;
			UInt32Vector::iterator Itr = m_requestGuildList.begin();
			while(i < REQUEST_GUILD_COUNT && Itr != m_requestGuildList.end())
			{
				GuildMap::iterator ItrGuild = gGlobal.m_GuildMap.find(*Itr);
				if (ItrGuild != gGlobal.m_GuildMap.end())
				{
					RPCMask mask;
					mask.MaskBits(CGuild::idMask, CGuild::nameMask, CGuild::creatorMask, 
						CGuild::masterMask, CGuild::allianceMask, CGuild::enemyMask, 
						CGuild::secMasterMask, CGuild::levelMask, CGuild::prosperMask, 
						CGuild::fundMask, CGuild::memberNumMask, CGuild::manorRoomMask, -1);
					ItrGuild->second->SetSendMask(mask);
					grp.Add(ItrGuild->second);
				}
				i ++;
				Itr = m_requestGuildList.erase(Itr);
			}

			if (grp.GetCount() > 0)
			{
				CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(GetChar_id());
				if (pMsgInfo)
					MessageService_Stub::CB_Guilds(pMsgInfo->pProxyPeer, 
						RPCContext(pMsgInfo->SID), &grp, m_firstRequest);
			}

			if (m_requestGuildList.size() > 0)
				RaiseUniqEventID(EVT_SYNC_GUILD, REQUEST_GUILD_TIME);

			m_firstRequest = FALSE;
		}
		break;

	case EVT_UPDATE:	
		DoUpdateObject();
		break;
	}
}

void CMessageCharacter::SyncGuild()
{
	RaiseUniqEventID(EVT_SYNC_GUILD, 0);
}