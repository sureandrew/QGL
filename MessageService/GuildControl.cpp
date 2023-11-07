//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/datacode.h"
//-- Self

#include "MessageService/GuildControl.h"
#include "MessageService/Global.h"
#include "Proxy/Data/GuildData.h"
#include "Proxy/Data/GuildMemberData.h"
#include "Proxy/Data/MsgCharacterData.h"
#include "Common/common_binary.h"
#include "Stub/Logic/Guild.h"
#include "Stub/Logic/GuildMember.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Stub/Service/MessageService.h"
#include "Resource/ResTitle.h"
#include "Resource/ServerResManager.h"
#include "Resource/GameStringTable.h"
#include "Proxy/Service/LogService.h"


CGuildControl::CGuildControl()
{
}

CGuildControl::~CGuildControl()
{
}

VOID CGuildControl::GetGuildByID(UInt32 guild_uid)
{
	RPCContext context;
	PROCEDURE_DO_METHOD1(CGuildControl, GetGuildByID, this, context, UInt32, guild_uid)
}

//only max 16 guild
VOID CGuildControl::GenerateGuildPvPGroup(LPCPEER pPeer)
{
	RPCContext context;
	PROCEDURE_DO_METHOD1(CGuildControl, GenerateGuildPvPGroup, this, context, LPCPEER, pPeer)
}

RPCResult CGuildControl::GenerateGuildPvPGroup( RPCContext &context)
{
	PROCEDURE_START_METHOD1(CGuildControl, GenerateGuildPvPGroup, this, context, LPCPEER, pPeer)

		ObjectService_Proxy::GetGuildByPvPBid(gGlobal.pObjectService->pPeer, context);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetGuilds, NetGroup *, guildGroup)
		TRACE_ENSURE(guildGroup != NULL);

		//UInt32Vector ids;
		for (UInt16 i = 0; i < guildGroup->GetCount(); i ++)
		{
			CGuildData *pGuildData = ((NetGroup*) guildGroup)->GetChild<CGuildData>(i);
			TRACE_ENSURE(pGuildData != NULL);

			GuildMap::iterator it = gGlobal.m_GuildMap.find(pGuildData->GetGuild_uid());
			CGuild * pGuild = NULL;
			if (it == gGlobal.m_GuildMap.end())			
			{
				pGuild = CGuild::NewInstance();
				pGuild->SetData(pGuildData);
				gGlobal.m_GuildMap.insert(std::make_pair(pGuild->GetGuild_uid(), pGuild));
			}
			//ids.push_back(pGuild->GetGuild_uid());
		}
		DeleteNetGroup(guildGroup, BOOLEAN_TRUE);	
		
		UInt8 positionAvail[MAX_GUILD_WAR_SIZE / 2];
		String groupString[MAX_GUILD_WAR_SIZE / 2];
		ZeroMemory(positionAvail, MAX_GUILD_WAR_SIZE / 2 * sizeof(UInt8));

		ObjectService_Proxy::ResetGuildGroup(gGlobal.pObjectService->pPeer, RPCContext());
		ObjectService_Proxy::ResetGuildBid(gGlobal.pObjectService->pPeer, RPCContext());
		//generate the group
		//take out the top 16 guild bid
		StlMultiMap<UInt32, CGuild *, std::greater<UInt32> > sorting;
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPBid() && Itr->second->GetGuildPvPBid() <= Itr->second->GetFund())
			{
				sorting.insert(std::make_pair(Itr->second->GetGuildPvPBid(), Itr->second));
			}
		}
		
		GuildMap tempMap = gGlobal.m_GuildMap;
		
		UInt maxGroup = MAX_GUILD_WAR_SIZE / 2;
		if (sorting.size() / 2 < maxGroup)
			maxGroup = (UInt)sorting.size() / 2;
		UInt indexForGorup = 0;				
		//form random group
		

		String msg = gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_GROUP_RUMER"));
		msg.append(_T("\\r"));
		
		for (StlMultiMap<UInt32, CGuild *, std::greater<UInt32> >::iterator Itr = sorting.begin();
			Itr != sorting.end() && indexForGorup < maxGroup * 2; ++Itr, ++indexForGorup)
		{
			Itr->second->SetFund(Itr->second->GetFund() - Itr->second->GetGuildPvPBid());
			Itr->second->SetGuildPvPBid(0);
			LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), Itr->second->GetGuild_uid(),  
									Itr->second->GetFund(), Itr->second->GetProsper(), Itr->second->GetManorScale(),
									LogService_Proxy::Guild_Fund::SOURCE_BID, 0);
			UInt8 rand = gGlobal.m_rand.GetRandom(maxGroup);
			UInt index = 0;
			while (positionAvail[rand % maxGroup] >= 2 && index < maxGroup * 2 )
			{
				rand ++;
				index ++;
			}
			
			if (index >= maxGroup * 2)
				continue;

			positionAvail[rand % maxGroup] ++;
			Itr->second->SetGuildPvPGroup(rand % maxGroup + 1);
			Itr->second->MarkUpdate(CGuild::guildPvPGroupMask);
			//find the same group guild
			UInt index2 = 0;	
			for (StlMultiMap<UInt32, CGuild *, std::greater<UInt32> >::iterator Itr2 = sorting.begin();
			Itr2 != sorting.end() && index2 < maxGroup * 2; ++Itr2, ++index2)
			{
				if (Itr2->second->GetGuildPvPGroup() == Itr->second->GetGuildPvPGroup() &&
					Itr2->second != Itr->second)
				{
					Itr->second->SetGroupPvPVSGuild(Itr2->second->GetName());
					Itr->second->SyncUpdate();
					Itr2->second->SetGroupPvPVSGuild(Itr->second->GetName());
					Itr2->second->SyncUpdate();
					break;
				}
			}



			Itr->second->SyncUpdate();
			msg.append(Itr->second->GetName().c_str());
			msg.append(_T("\\r"));
			GuildMap::iterator ItrGuild = tempMap.find(Itr->second->GetGuild_uid());
			if (ItrGuild != tempMap.end())
				tempMap.erase(ItrGuild);

			
		}

			
		if (maxGroup == 0)
		{
			msg =gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_GROUP_NOT_ENOUGH"));			
		}		

		MessageService_Stub::SendRumorMessage(pPeer, context, msg, 0, 1);

		for (GuildMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPBid())
			{				
				Itr->second->SetGuildPvPBid(0);
				Itr->second->SetGroupPvPVSGuild(String());
				Itr->second->SetGuildPvPGroup(0);
				Itr->second->SyncUpdate();
			}
		}


	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("GetAllGuild Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH
	
	return RPC_RESULT_OK; 
}
//only max 16 guild
VOID CGuildControl::GetGuildByPvPGroup()
{
	RPCContext context;
	PROCEDURE_DO_METHOD(CGuildControl, GetGuildByPvPGroup, this, context)
}

RPCResult CGuildControl::GetGuildByPvPGroup( RPCContext &context)
{
	PROCEDURE_START_METHOD(CGuildControl, GetGuildByPvPGroup, this, context)

		ObjectService_Proxy::GetGuildByPvPGroup(gGlobal.pObjectService->pPeer, context);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetGuilds, NetGroup *, guildGroup)
		TRACE_ENSURE(guildGroup != NULL);

		//UInt32Vector ids;
		for (UInt16 i = 0; i < guildGroup->GetCount(); i ++)
		{
			CGuildData *pGuildData = ((NetGroup*) guildGroup)->GetChild<CGuildData>(i);
			TRACE_ENSURE(pGuildData != NULL);

			GuildMap::iterator it = gGlobal.m_GuildMap.find(pGuildData->GetGuild_uid());
			CGuild * pGuild = NULL;
			if (it == gGlobal.m_GuildMap.end())			
			{
				pGuild = CGuild::NewInstance();
				pGuild->SetData(pGuildData);
				gGlobal.m_GuildMap.insert(std::make_pair(pGuild->GetGuild_uid(), pGuild));

				if (pGuild->GetManorScale() > 0)				
					pGuild->InitManor(); // initizlize the manor if any 	
				PROCEDURE_DO_METHOD(CGuild, LoadMembers, pGuild, context)				
				
			}
			//ids.push_back(pGuild->GetGuild_uid());
		}
		DeleteNetGroup(guildGroup, BOOLEAN_TRUE);	
		

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("GetAllGuild Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH
	
	return RPC_RESULT_OK; 
}

VOID CGuildControl::DeleteGuildByID(UInt32 guild_uid)
{
	RPCContext context;
	PROCEDURE_DO_METHOD1(CGuildControl, DeleteGuildByID, this, context, UInt32, guild_uid)
}
RPCResult CGuildControl::DeleteGuildByID( RPCContext &context)
{
	PROCEDURE_START_METHOD1(CGuildControl, DeleteGuildByID, this, context, UInt32, guild_uid)
	//remove the member first
	UInt32Vector char_ids;
	CGuild* pGuild = gGlobal.GetGuild(guild_uid);
	if (pGuild)
	{
		for (GuildMemberMap::iterator Itr = pGuild->m_memberMap.begin(); 
			Itr != pGuild->m_memberMap.end(); ++Itr)
			char_ids.push_back(Itr->second->GetChar_id());
	}
		ObjectService_Proxy::DeleteGuildMemberList(gGlobal.pObjectService->pPeer, context, 
			char_ids);

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteGuildMemberList)
	
		CGuild* pGuild = gGlobal.GetGuild(guild_uid);
		if (pGuild)
		{
			pGuild->RemoveAllGuildMember();			
		}

		// delete guild in object service
		UInt32Vector guild_uids;
		guild_uids.push_back(guild_uid);
		ObjectService_Proxy::DeleteGuildList(gGlobal.pObjectService->pPeer, context, 
			guild_uids);

	PROCEDURE_WAIT(2, ObjectService, CB_DeleteGuildList)
		
		CGuild* pGuild = gGlobal.GetGuild(guild_uid);
		if (pGuild)
		{
			String msg;
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_DELETE_MSG")), 
				pGuild->GetName().c_str());
			Peer temp;
			MessageService_Stub::SendRumorMessage(&temp, RPCContext(), msg, 0, 0);
			
			gGlobal.RemoveGuild(pGuild);			
		}
		

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("GetAllGuild Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH
}
RPCResult CGuildControl::GetGuildByID(RPCContext &context)
{
	PROCEDURE_START_METHOD1(CGuildControl, GetGuildByID, this, context, UInt32, guild_uid)
		
		ObjectService_Proxy::GetGuildByID(gGlobal.pObjectService->pPeer, context, guild_uid);

	PROCEDURE_WAIT1(1, ObjectService, CB_GetGuilds, NetGroup *, guildGroup)
		TRACE_ENSURE(guildGroup != NULL);

		//UInt32Vector ids;
		CGuild* pGuild = NULL;
		for (UInt16 i = 0; i < guildGroup->GetCount(); i ++)
		{
			CGuildData *pGuildData = ((NetGroup*) guildGroup)->GetChild<CGuildData>(i);
			TRACE_ENSURE(pGuildData != NULL);

			GuildMap::iterator it = gGlobal.m_GuildMap.find(pGuildData->GetGuild_uid());
			if (it != gGlobal.m_GuildMap.end())
			{
				pGuild = it->second;	
				pGuild->SetData(pGuildData);
			} 
			else
			{
				pGuild = CGuild::NewInstance();
				pGuild->SetData(pGuildData);
				gGlobal.m_GuildMap.insert(std::make_pair(pGuild->GetGuild_uid(), pGuild));
			}
			//ids.push_back(pGuild->GetGuild_uid());
		}
		DeleteNetGroup(guildGroup, BOOLEAN_TRUE);
		if (pGuild)
		{
			if (pGuild->GetManorScale() > 0)
                pGuild->InitManor(); // initizlize the manor if any 	
			PROCEDURE_DO_METHOD(CGuild, LoadMembers, pGuild, context)
		}
		else
		{
			TRACE_WARNDTL_1(GLOBAL_LOGGER, _FW("[no guild exist], guild_uid %d"),
				guild_uid);
			PROCEDURE_GO_STEP(2)
		}

	PROCEDURE_WAIT_RETURN(2)

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_T("GetAllGuild Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());

	PROCEDURE_END_CATCH
	
		return RPC_RESULT_OK; 
}
