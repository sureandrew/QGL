//-- Common
#include "Common.h"
#include "Common/Procedure.h"
//-- Self
#include "Stub/Logic/GuildMember.h"
#include "Stub/Logic/Guild.h"
#include "Stub/Logic/Channel.h"
#include "Proxy/Data/GuildMemberData.h"
#include "Proxy/Data/MsgCharacterData.h"
#include "Stub/Service/MessageService.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Common/datacode.h"
#include "MessageService/Global.h"
#include "Stub/Logic/MessageCharacter.h"
#include "MessageService/MessageApplication.h"
#include "Proxy/Service/LogService.h"

//-- Resource
#include "Resource/ResManor.h"
#include "Resource/ServerResManager.h"

REGISTER_CLASSTYPE(CGuildMember, GuildMember_CLASSID);

#define WaitTime	2000
#define Max_Rule_Len	300
#define Max_Aim_Len	100
#define REMOVE_TRADER_POST_BY_NO_TRADE	30 * 60 * 1000	//30 minute
#define REMOVE_TRADER_POST_BY_NO_MONEY	15 * 60 * 1000	//15 minute
CGuildMember::CGuildMember()
{
	m_pGuild = NULL;
	m_requestMemberTime = 0;
	m_requestInfoTime = 0;
	m_donated = false;
}

CGuildMember::~CGuildMember()
{
	if (GetChar_id() != 0) 
		gGlobal.ReleaseMsgCharRef(GetChar_id());
}

void CGuildMember::ChangeDonate(Int change, UInt8 source)
{
	if ((Int)GetDonate() + change <= 0)
		SetDonate(0);
	else
		SetDonate(GetDonate() + change);

	if (change > 0)
		SetTotalDonate(GetTotalDonate() + change);

	LogService_Proxy::LogGuildDonate(gGlobal.pLogService->pPeer, RPCContext(), GetGuild_uid(), GetChar_id(),
									change,  GetTotalDonate(), source);

	if (m_pGuild)
		m_pGuild->SyncUpdate();
}

void CGuildMember::PreSend()
{
	RPCMask mask;
	mask.MaskBits(AllMask, -1);
	SetSendMask(mask);
}

void CGuildMember::WriteData(CGuildMemberData * pGuildMemberData)
{
	if (IsUpdated())
	{
		pGuildMemberData->SetKey(GetChar_id(), GetGuild_uid());
	}

	if (IsUpdated(donateMask))
	{
		pGuildMemberData->SetDonate(GetDonate());
	}

	if (IsUpdated(totalDonateMask))
	{
		pGuildMemberData->SetTotalDonate(GetTotalDonate());
	}

	if (IsUpdated(postMask))
	{
		pGuildMemberData->SetPost(GetPost());
	}

	if (IsUpdated(joinDateMask))
	{
		pGuildMemberData->SetJoinDate(GetJoinDate());
	}

	if (IsUpdated(businessMask))
	{
		pGuildMemberData->SetBusiness(GetBusinessCount(), GetBusinessProfit());
	}
}

void CGuildMember::SetData(CGuildMemberData * pGuildMemberData)
{
	if (pGuildMemberData->IsUpdated(CGuildMemberData::keyMask))
	{
		SetKey(pGuildMemberData->GetChar_id(), pGuildMemberData->GetGuild_uid());
	}

	if (pGuildMemberData->IsUpdated(CGuildMemberData::donateMask))
	{
		SetDonate(pGuildMemberData->GetDonate());
	}

	if (pGuildMemberData->IsUpdated(CGuildMemberData::totalDonateMask))
	{
		SetTotalDonate(pGuildMemberData->GetTotalDonate());
	}

	if (pGuildMemberData->IsUpdated(CGuildMemberData::postMask))
	{
		SetPost(pGuildMemberData->GetPost());
	}

	if (pGuildMemberData->IsUpdated(CGuildMemberData::joinDateMask))
	{
		SetJoinDate(pGuildMemberData->GetJoinDate());
	}

	if (pGuildMemberData->IsUpdated(CGuildMemberData::businessMask))
	{
		SetBusiness(pGuildMemberData->GetBusinessCount(), pGuildMemberData->GetBusinessProfit());
	}

	if (GetPost() == GUILD_POSITION_TRADER && m_pGuild && m_pGuild->GetAutoManageBusiness())
	{
		RaiseUniqEventID(EVT_REMOVE_TRADER_POST_BY_NO_TRADE, REMOVE_TRADER_POST_BY_NO_TRADE);
	}
	ClearUpdateMask();
}

void CGuildMember::SetData(CMessageCharacter * pMsgChar, bool bClear)
{
	if (pMsgChar == NULL)
		return;

	UInt8 SexClass = (pMsgChar->GetCclass() > 0 ? 
			((pMsgChar->GetCclass() - 1) * MAX_SEXTYPE + pMsgChar->GetSex() + 1) : 0);

	if (!pMsgChar->GetInactive())
	{
		// member active
		SetName(pMsgChar->GetNickName());
		SetSexClass(SexClass);
		SetLevel(pMsgChar->GetLevel());
		if (pMsgChar->IsOffline())
			SetOfflineTime(pMsgChar->GetOfflineTime());
		SetInactive(BOOLEAN_FALSE);
	}
	else
	{
		// member inactive
		SetName(pMsgChar->GetNickName());
		SetOfflineTime(UINT32_MAX_VALUE);
		SetInactive(BOOLEAN_TRUE);
	}

	if (bClear)
		ClearUpdateMask();
}

Boolean CGuildMember::SetData(CMsgCharacterData * pMsgCharData, bool bClear)
{
	Boolean modified = BOOLEAN_FALSE; 
	if (pMsgCharData->IsUpdated(CMsgCharacterData::infoMask))
	{
		UInt8 SexClass = (pMsgCharData->GetCclass() > 0 ? 
			((pMsgCharData->GetCclass() - 1) * MAX_SEXTYPE + pMsgCharData->GetSex() + 1) : 0);
		SetName(pMsgCharData->GetNickName());
		SetSexClass(SexClass);
		modified = BOOLEAN_TRUE; 
	}

	if (pMsgCharData->IsUpdated(CMsgCharacterData::levelMask))
	{
		SetLevel(pMsgCharData->GetLevel());
		modified = BOOLEAN_TRUE;
	}
	if (pMsgCharData->IsUpdated(CMsgCharacterData::offlineTimeMask))
	{
		SetOfflineTime(pMsgCharData->GetOfflineTime());
		modified = BOOLEAN_TRUE;
	}

	if (bClear)
        ClearUpdateMask();
	return modified; 
}

RPCResult CGuildMember::RequestGuildInfo(LPCPEER pPeer, RPCContext &context)
{
	if (m_requestInfoTime + WaitTime > ::timeGetTime())
		return RPC_RESULT_OK;
	else
		m_requestInfoTime = ::timeGetTime();
	if (m_pGuild)
	{
		m_pGuild->PreSend();
		NetGroup grp;
		grp.Add(m_pGuild);
		MessageService_Stub::CB_Guilds(pPeer, context, &grp, false);
	}

	return RPC_RESULT_OK;
}
RPCResult CGuildMember::RequestMember(LPCPEER pPeer, RPCContext &context)
{
	if (m_requestMemberTime + WaitTime > ::timeGetTime())
		return RPC_RESULT_OK;
	else
		m_requestMemberTime = ::timeGetTime();
	if (m_pGuild)
	{
		NetGroup grp;
		m_pGuild->PackGuildMember(&grp);
		MessageService_Stub::CB_GuildMembers(pPeer, context, &grp);
	}
	return RPC_RESULT_OK;
}

RPCResult CGuildMember::RequestApplicant(LPCPEER pPeer, RPCContext &context)
{
	if (m_pGuild)
	{
		NetGroup grp;
		m_pGuild->PackGuildApplicant(&grp);
		MessageService_Stub::CB_RequestApplicant(pPeer, context, &grp);
	}
	return RPC_RESULT_OK;
}

RPCResult CGuildMember::ResponseApplicant(LPCPEER pPeer, RPCContext &context, 
										  const UInt32 &char_id, const bool &accept)
{
	if (m_pGuild == NULL || GetPost() > GUILD_POSITION_ELDER || m_pGuild->HasMember(char_id))
		return RPC_RESULT_OK;

	if (m_pGuild->IsApplicantExist(char_id))
	{
		UInt16 extraMember = 0;
		if (m_pGuild->GetManorScale() > 0)
		{
			const ManorData *manorData;
			manorData = gGlobal.m_resource->GetManorData(m_pGuild->GetManorScale());
			if (!manorData)
				return RPC_RESULT_OK;
			extraMember = 10 * m_pGuild->GetRoom();
		}
		if (m_pGuild->GetTotalMember() < MAX_MEMBER + extraMember)
		{
			if (accept)
			{
				PROCEDURE_DO_METHOD2(CGuild, AddMember, m_pGuild, context, 
					UInt32, GetChar_id(), UInt32, char_id)
			}
			m_pGuild->RemoveGuildApplicant(char_id);
		}
		else
			MessageService_Stub::CB_Error(pPeer, context, 0 , "MSG_GUILD_MEMBER_FULL");
	}
	else
		MessageService_Stub::CB_Error(pPeer, context, 0 , "MSG_ALREADY_TARGET_NOT_IN_LIST");

	return RPC_RESULT_OK;
}

RPCResult CGuildMember::PositionChange(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt8 &newPost)
{
	if (GetPost() <= GUILD_POSITION_DEPTHEAD && (GetPost() < newPost || 
		(GetPost() == GUILD_POSITION_MASTER && newPost == GUILD_POSITION_MASTER) ))
	{
		CGuildMember * pMember = m_pGuild->GetMember(char_id);
		if (pMember && GetPost() <= pMember->GetPost())
		{
			if (pMember != this)
			{
				if (m_pGuild && m_pGuild->CheckPostAvialble(newPost))
				{
					if (newPost == GUILD_POSITION_SECMASTER)
					{
						if (m_pGuild)
						{
							m_pGuild->SetSecMaster(pMember->GetName());

						}
					}
					else if (newPost == GUILD_POSITION_MASTER)
					{
						if ((pMember->GetLevel() < 30) || (GetPost() != GUILD_POSITION_MASTER))
						{
							return RPC_RESULT_OK;
						}
						if (m_pGuild)
						{
							m_pGuild->SetMaster(pMember->GetName());
						}
						SetPost(GUILD_POSITION_MEMBER);

					}

					pMember->SetPost(newPost);

					if (newPost == GUILD_POSITION_TRADER && m_pGuild->GetAutoManageBusiness())
					{
						pMember->RaiseUniqEventID(EVT_REMOVE_TRADER_POST_BY_NO_TRADE, REMOVE_TRADER_POST_BY_NO_TRADE);
					}

					if (m_pGuild->NoMaster())
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("No Master: guild_uid: %d"),
							m_pGuild->GetGuild_uid());
					}

					m_pGuild->SyncUpdate();
				}
				else
				{
					MessageService_Stub::CB_Error(pPeer, context, 0, _T("MSG_GUILD_POSITION_FULL"));
				}
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult CGuildMember::ChangeAim(LPCPEER pPeer, RPCContext &context, const String &text)
{
	if (text.GetLength() > Max_Aim_Len || GetPost() > GUILD_POSITION_ADMIN02)
		return RPC_RESULT_OK;

	if (m_pGuild)
	{
		m_pGuild->SetAim(text);
		m_pGuild->SyncUpdate();
	}
	
	return RPC_RESULT_OK;
}

RPCResult CGuildMember::ChangeRule(LPCPEER pPeer, RPCContext &context, const String &text)
{
	if (text.GetLength() > Max_Rule_Len || GetPost() > GUILD_POSITION_SECMASTER)
		return RPC_RESULT_OK;

	if (m_pGuild)
	{
		m_pGuild->SetRule(text);
		m_pGuild->SyncUpdate();
	}
	return RPC_RESULT_OK;
}

RPCResult CGuildMember::ClearGuildBusinessLog(LPCPEER pPeer, RPCContext &context)
{
	if (GetPost() > GUILD_POSITION_MASTER)
		return RPC_RESULT_OK;
	if (m_pGuild)
	{
		m_pGuild->RaiseUniqEventID(CGuild::EVT_CLEAR_BUSINESS_LOG);
	}
	return RPC_RESULT_OK;
}

void CGuildMember::HandleEvent(CONST Event &event)
{
	switch (event.GetID())
	{
	case EVT_REMOVE_TRADER_POST_BY_NO_TRADE:
		if (m_pGuild && m_pGuild->GetAutoManageBusiness() && GetPost() == GUILD_POSITION_TRADER)
		{
			SetPost(GUILD_POSITION_MEMBER);
			m_pGuild->SyncUpdate();
		}
		CancelUniqEvent(EVT_REMOVE_TRADER_POST_BY_NO_MONEY);
		break;
	case EVT_REMOVE_TRADER_POST_BY_NO_MONEY:
		if (m_pGuild && m_pGuild->GetAutoManageBusiness() && GetPost() == GUILD_POSITION_TRADER)
		{
			SetPost(GUILD_POSITION_MEMBER);
			m_pGuild->SyncUpdate();
		}
		CancelUniqEvent(EVT_REMOVE_TRADER_POST_BY_NO_TRADE);
		break;
	}
}

void CGuildMember::RaiseNoTradeEvent()
{
	RaiseUniqEventID(EVT_REMOVE_TRADER_POST_BY_NO_TRADE, REMOVE_TRADER_POST_BY_NO_TRADE);
}

void CGuildMember::RaiseNoMoneyEvent()
{
	RaiseUniqEventID(EVT_REMOVE_TRADER_POST_BY_NO_MONEY, REMOVE_TRADER_POST_BY_NO_MONEY);
}

void CGuildMember::CancelNoMoneyEvent()
{
	CancelUniqEvent(EVT_REMOVE_TRADER_POST_BY_NO_MONEY);
}