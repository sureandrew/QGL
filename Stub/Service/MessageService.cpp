//-- Common
#include "Common.h"
#include "Common/ChannelType.h"
#include "Common/Procedure.h"
#include "Common/common_binary.h"
//-- Self
#include "Stub/Service/MessageService.h"
//-- Library
#include "Proxy/Data/MessengerData.h"
#include "Proxy/Service/ObjectService_proxy.h"
#include "Proxy/Service/MapService_proxy.h"
#include "Stub/Logic/Channel.h"
#include "Stub/Logic/Messenger.h"
#include "Stub/Logic/MessageCharacter.h"
#include "Stub/Data/CharacterData.h"
#include "Stub/Data/MsgCharacterData.h"
#include "MessageService/Global.h"
#include "MessageService/Configure.h"
#include "MessageService/MessageApplication.h"
#include "Stub/Logic/Guild.h"
#include "Stub/Logic/GuildMember.h"
#include "Proxy/Data/GuildData.h"
#include "Proxy/Data/GuildMemberData.h"
#include "MessageService/GuildControl.h"
#include "Resource/GameStringTable.h"
#include "MessageService/BusinessShopControl.h"
#include "Proxy/Service/LogService.h"

//-- Resource
#include "Resource/ResManor.h"
#include "Resource/ServerResManager.h"

typedef StlMap<LPCPEER, SessionIDVector, std::less<LPCPEER>, 
ManagedAllocator< std::pair<LPCPEER, SessionIDVector> > > ProxyListenersMap;

#define MAX_GUILD_NAME_SIZE 6
#define MAX_GUILD_MONEY	5000000

// private function and namespace global variable here
namespace MessageService_Stub
{
};

RPCResult MessageService_Stub::RegisterMap(LPCPEER pPeer, RPCContext &context, 
										   const ServiceID &serviceId, const UInt16 &map_id,
										   const UInt8 &line_id, const UInt32 &owner_id)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("serviceId %d, map_id %d, line_id %d, owner_id %d"),
		serviceId, map_id, line_id, owner_id);

	bool syncGuild = (gGlobal.m_mapPeers.find(pPeer) == gGlobal.m_mapPeers.end());
	gGlobal.RegisterMap(MapIndex(map_id, line_id, owner_id), pPeer);

	// sync the guild info to map if that map service haven't registered before
	// send to messengers all map service 
	if (syncGuild)
	{
		NetGroup guildGrp;
		CGuild* pGuild;

		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); 
			Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			pGuild = Itr->second;
			if (pGuild)
			{
				pGuild->PreSend();
				guildGrp.Add(Itr->second);
			}
		}
		MapService_Proxy::GuildUpdate(pPeer, RPCContext(), &guildGrp);

		for (UInt16 i = 0; i < guildGrp.GetCount(); ++i)
		{
			pGuild = guildGrp.GetChild<CGuild>(i);
			pGuild->SyncToMap(pPeer);
		}
	}

	return RPC_RESULT_OK;
}
//cw
RPCResult MessageService_Stub::GenerateGuildGroupCW(LPCPEER pPeer, RPCContext &context,const UInt8 &guildWarState)
{
	if (/*gGlobal.m_guildWarStatus*/GuildGroupCW::GetInstance().GetGuildWarStatus() == guildWarState)
	{
		//not to start again in same status
		return RPC_RESULT_OK;
	}

	/*gGlobal.m_guildWarStatus = guildWarState;*/
	GuildGroupCW::GetInstance().SetGuildWarStatus(guildWarState);

	UInt8 positionAvail[MAX_GUILD_WAR_SIZE / 2];
	String groupString[MAX_GUILD_WAR_SIZE / 2];
	ZeroMemory(positionAvail, MAX_GUILD_WAR_SIZE / 2 * sizeof(UInt8));

	for (UInt i = 0; i < MAX_GUILD_WAR_SIZE / 2; i++)
	{
		String msgGroup;
		msgGroup.Format(_T("MSG_GUILDWAR_GROUP%d"), i);
		groupString[i] = gGlobal.GetStringTable()->Get(msgGroup.c_str());
	}

	if (/*guildWarState*/GuildGroupCW::GetInstance().GetGuildWarStatus() == GUILD_WARCW_STATE_BID_END)
	{		
		gGlobal.m_guildControl->GenerateGuildPvPGroup(pPeer);
	}
	else if (/*guildWarState*/GuildGroupCW::GetInstance().GetGuildWarStatus() == GUILD_WARCW_STATE_WAR_FINISH_CONTINUE)
	{		
		StlMultiMap<UInt8, CGuild *> guildPvPGrp;
		UInt preMaxGroup = 0;
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPGroup())
			{
				guildPvPGrp.insert(std::make_pair(Itr->second->GetGuildPvPGroup(), Itr->second));
				positionAvail[Itr->second->GetGuildPvPGroup() - 1] ++;

				if (preMaxGroup < Itr->second->GetGuildPvPGroup())
					preMaxGroup = Itr->second->GetGuildPvPGroup();
			}
		}

		

		UInt maxGroup = (UInt)guildPvPGrp.size() / 2;
		if (maxGroup == 0)
			return RPC_RESULT_OK;

		StlVector<StlMultiMap<UInt8, CGuild *>::iterator > removeGuild;
		for (UInt index = 0; index < MAX_GUILD_WAR_SIZE; index ++)
		{
			//remove single group
			if (positionAvail[index] == 1)
			{
				StlMultiMap<UInt8, CGuild *>::iterator ItrRemove = guildPvPGrp.find(index + 1);
				if (ItrRemove != guildPvPGrp.end())
				{
					removeGuild.push_back(ItrRemove);
					//shift the next guild Group	
					StlMultiMap<UInt8, CGuild *>::iterator ItrShiftRemove = ItrRemove;
					ItrShiftRemove ++;
					for (; ItrShiftRemove != guildPvPGrp.end(); ItrShiftRemove ++)
					{
						ItrShiftRemove->second->SetGuildPvPGroup(ItrShiftRemove->second->GetGuildPvPGroup() - 1);
						ItrShiftRemove->second->MarkUpdate(CGuild::guildPvPGroupMask);
					}
				}
			}
		}

		//remove the guild
		for (UInt i = 0; i < removeGuild.size(); i++)
		{
			guildPvPGrp.erase(removeGuild[i]);
			
		}
	

		maxGroup = (UInt)guildPvPGrp.size() / 2;
		ZeroMemory(positionAvail, MAX_GUILD_WAR_SIZE / 2 * sizeof(UInt8));


		if (maxGroup > 0)
		{
			UInt rand1 = gGlobal.m_rand.GetRandom(maxGroup );
			UInt rand2 = gGlobal.m_rand.GetRandom(maxGroup);
			if (rand1 == rand2)
				rand2 = (rand2 + 1) % (maxGroup);

			UInt inc = rand1;
			UInt i = 0;
			StlMultiMap<UInt8, CGuild *>::iterator ItrGuild = guildPvPGrp.begin();
			for (; ItrGuild != guildPvPGrp.end() && i < maxGroup * 2; ++ItrGuild, ++i)
			{
				ItrGuild->second->SetGuildPvPGroup((ItrGuild->second->GetGuildPvPGroup() + inc) % maxGroup + 1);
				ItrGuild->second->MarkUpdate(CGuild::guildPvPGroupMask);

				if (inc == rand1)
					inc = rand2;
				else
					inc = rand1;

				ItrGuild->second->SyncUpdate();

				positionAvail[ItrGuild->second->GetGuildPvPGroup() - 1] ++;
				if (positionAvail[ItrGuild->second->GetGuildPvPGroup() - 1] == 1)
				{
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(ItrGuild->second->GetName().c_str());
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(_T(" VS "));
				}
				else
				{
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(ItrGuild->second->GetName().c_str());
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(_T("\\r"));
				}

				//find the same group guild
				UInt index2 = 0;	
				for (StlMultiMap<UInt8, CGuild *>::iterator Itr2 = guildPvPGrp.begin();
					Itr2 != guildPvPGrp.end() && index2 < maxGroup * 2; ++Itr2, ++index2)
				{
					if (Itr2->second->GetGuildPvPGroup() == ItrGuild->second->GetGuildPvPGroup() &&
						Itr2->second != ItrGuild->second)
					{
						ItrGuild->second->SetGroupPvPVSGuild(Itr2->second->GetName());
						ItrGuild->second->SyncUpdate();
						Itr2->second->SetGroupPvPVSGuild(ItrGuild->second->GetName());
						Itr2->second->SyncUpdate();
						break;
					}
				}
			}		

		}
		//add back some remove guild		
		for (UInt i = 0; i < removeGuild.size(); i++)
		{
			if (i % 2 == 0)
				maxGroup ++;
			removeGuild[i]->second->SetGuildPvPGroup(maxGroup);
			removeGuild[i]->second->MarkUpdate(CGuild::guildPvPGroupMask);

			removeGuild[i]->second->SyncUpdate();

				positionAvail[removeGuild[i]->second->GetGuildPvPGroup() - 1] ++;
				if (positionAvail[removeGuild[i]->second->GetGuildPvPGroup() - 1] == 1)
				{
					groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(removeGuild[i]->second->GetName().c_str());
					groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(_T(" VS "));
				}
				else
				{
					groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(removeGuild[i]->second->GetName().c_str());
					groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(_T("\\r"));
				}

				//find the same group guild
				UInt index2 = 0;	
				for (StlMultiMap<UInt8, CGuild *>::iterator Itr2 = guildPvPGrp.begin();
					Itr2 != guildPvPGrp.end() && index2 < maxGroup * 2; ++Itr2, ++index2)
				{
					if (Itr2->second->GetGuildPvPGroup() == removeGuild[i]->second->GetGuildPvPGroup() &&
						Itr2->second != removeGuild[i]->second)
					{
						removeGuild[i]->second->SetGroupPvPVSGuild(Itr2->second->GetName());
						removeGuild[i]->second->SyncUpdate();
						Itr2->second->SetGroupPvPVSGuild(removeGuild[i]->second->GetName());
						Itr2->second->SyncUpdate();
						break;
					}
				}
		}

		String msg;
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_END_FIRST_CONTINUE"));
		msg.append(_T("\\r"));
		for (UInt i = 0; i < maxGroup; i ++)
		{
			msg.append(groupString[i].c_str());
		}

		SendRumorMessage(pPeer, context, msg, 0, 1);
	}
	else if (GuildGroupCW::GetInstance().GetGuildWarStatus() == GUILD_WAR_STATE_WAR_PREPARE)
	{
// /*
// 		void GuildWarPrepare()
// 		{
// 			TRACE_INFODTL(GLOBAL_LOGGER, _F("Call Lua:GuildWarPrepare"));
// 			gGlobal.g_GuildWarState = GUILD_WAR_STATE_WAR_PREPARE;
// 		}
// */
// 
// 		//check guild war map
// 		if (pmap->GetMapInfo() && pmap->GetMapInfo()->m_basicinfo->m_maptype == MAP_TYPE_GUILD_WAR)
// 		{
// 			if (!bLocal || (gGlobal.g_GuildWarState != GUILD_WAR_STATE_IN_WAR && gGlobal.g_GuildWarState != GUILD_WAR_STATE_WAR_PREPARE))
// 			{
// 				UInt8 line_id = pmap->GetLine_id();					
// 				JumpMap(gConfigure.guildWarRebornMapID, gConfigure.guildWarRebornMapX, gConfigure.guildWarRebornMapY, line_id);
// 				bJumped = true;
// 			}
// 			else
// 				ChangeGuildWarTitle();
// 			
// 			// cli enter
// 		}

	}
	else if (guildWarState == GUILD_WARCW_STATE_WAR_FINISH_END)
	{
		ObjectService_Proxy::ResetGuildGroup(gGlobal.pObjectService->pPeer, RPCContext());
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPGroup())
			{
				Itr->second->SetGuildPvPGroup(0);
				Itr->second->SyncUpdate();
			}
		}
	}
	else if (guildWarState == GUILD_WARCW_STATE_SEND_RUMOR_GROUP)
	{		
		UInt maxGroup = 0;
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPGroup())
			{
				positionAvail[Itr->second->GetGuildPvPGroup() - 1] ++;
				if (positionAvail[Itr->second->GetGuildPvPGroup() - 1] == 1)
				{
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(Itr->second->GetName().c_str());
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(_T(" VS "));
				}
				else
				{
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(Itr->second->GetName().c_str());
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(_T("\\r"));
				}
				if (Itr->second->GetGuildPvPGroup() > maxGroup )
					maxGroup = Itr->second->GetGuildPvPGroup();
			}
		}
		String msg;
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_GROUP_UPPER_MSG"));
		msg.append(_T("\\r"));
		for (UInt i = 0; i < maxGroup; i ++)
		{
			msg.append(groupString[i].c_str());
		}
		msg.append(gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_GROUP_LOWER_MSG")));

		SendRumorMessage(pPeer, context, msg, 0, 1);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GenerateGuildGroup(LPCPEER pPeer, RPCContext &context,const UInt8 &guildWarState)
{
	if (gGlobal.m_guildWarStatus == guildWarState)
	{
		//not to start again in same status
		return RPC_RESULT_OK;
	}

	gGlobal.m_guildWarStatus = guildWarState;

	UInt8 positionAvail[MAX_GUILD_WAR_SIZE / 2];
	String groupString[MAX_GUILD_WAR_SIZE / 2];
	ZeroMemory(positionAvail, MAX_GUILD_WAR_SIZE / 2 * sizeof(UInt8));

	for (UInt i = 0; i < MAX_GUILD_WAR_SIZE / 2; i++)
	{
		String msgGroup;
		msgGroup.Format(_T("MSG_GUILDWAR_GROUP%d"), i);
		groupString[i] = gGlobal.GetStringTable()->Get(msgGroup.c_str());
	}

	if (guildWarState == GUILD_WARCW_STATE_BID_END) // 1 . bang pai ru xuan gong gao
	{		
		gGlobal.m_guildControl->GenerateGuildPvPGroup(pPeer);
	}
	else if (guildWarState == GUILD_WARCW_STATE_SEND_RUMOR_GROUP) // 2. fen zhu xing xi
	{		
		UInt maxGroup = 0;
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPGroup())
			{
				positionAvail[Itr->second->GetGuildPvPGroup() - 1] ++;
				if (positionAvail[Itr->second->GetGuildPvPGroup() - 1] == 1)
				{
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(Itr->second->GetName().c_str());
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(_T(" VS "));
				}
				else
				{
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(Itr->second->GetName().c_str());
					groupString[Itr->second->GetGuildPvPGroup() - 1].append(_T("\\r"));
				}
				if (Itr->second->GetGuildPvPGroup() > maxGroup )
					maxGroup = Itr->second->GetGuildPvPGroup();
			}
		}
		String msg;
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_GROUP_UPPER_MSG"));
		msg.append(_T("\\r"));
		for (UInt i = 0; i < maxGroup; i ++)
		{
			msg.append(groupString[i].c_str());
		}
		msg.append(gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_GROUP_LOWER_MSG")));

		SendRumorMessage(pPeer, context, msg, 0, 1);
	}
	else if (guildWarState == GUILD_WAR_STATE_WAR_FINISH_CONTINUE)
	{		
		StlMultiMap<UInt8, CGuild *> guildPvPGrp;
		UInt preMaxGroup = 0;
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPGroup())
			{
				guildPvPGrp.insert(std::make_pair(Itr->second->GetGuildPvPGroup(), Itr->second));
				positionAvail[Itr->second->GetGuildPvPGroup() - 1] ++;

				if (preMaxGroup < Itr->second->GetGuildPvPGroup())
					preMaxGroup = Itr->second->GetGuildPvPGroup();
			}
		}



		UInt maxGroup = (UInt)guildPvPGrp.size() / 2;
		if (maxGroup == 0)
			return RPC_RESULT_OK;

		StlVector<StlMultiMap<UInt8, CGuild *>::iterator > removeGuild;
		for (UInt index = 0; index < MAX_GUILD_WAR_SIZE; index ++)
		{
			//remove single group
			if (positionAvail[index] == 1)
			{
				StlMultiMap<UInt8, CGuild *>::iterator ItrRemove = guildPvPGrp.find(index + 1);
				if (ItrRemove != guildPvPGrp.end())
				{
					removeGuild.push_back(ItrRemove);
					//shift the next guild Group	
					StlMultiMap<UInt8, CGuild *>::iterator ItrShiftRemove = ItrRemove;
					ItrShiftRemove ++;
					for (; ItrShiftRemove != guildPvPGrp.end(); ItrShiftRemove ++)
					{
						ItrShiftRemove->second->SetGuildPvPGroup(ItrShiftRemove->second->GetGuildPvPGroup() - 1);
						ItrShiftRemove->second->MarkUpdate(CGuild::guildPvPGroupMask);
					}
				}
			}
		}

		//remove the guild
		for (UInt i = 0; i < removeGuild.size(); i++)
		{
			guildPvPGrp.erase(removeGuild[i]);

		}


		maxGroup = (UInt)guildPvPGrp.size() / 2;
		ZeroMemory(positionAvail, MAX_GUILD_WAR_SIZE / 2 * sizeof(UInt8));


		if (maxGroup > 0)
		{
			UInt rand1 = gGlobal.m_rand.GetRandom(maxGroup );
			UInt rand2 = gGlobal.m_rand.GetRandom(maxGroup);
			if (rand1 == rand2)
				rand2 = (rand2 + 1) % (maxGroup);

			UInt inc = rand1;
			UInt i = 0;
			StlMultiMap<UInt8, CGuild *>::iterator ItrGuild = guildPvPGrp.begin();
			for (; ItrGuild != guildPvPGrp.end() && i < maxGroup * 2; ++ItrGuild, ++i)
			{
				ItrGuild->second->SetGuildPvPGroup((ItrGuild->second->GetGuildPvPGroup() + inc) % maxGroup + 1);
				ItrGuild->second->MarkUpdate(CGuild::guildPvPGroupMask);

				if (inc == rand1)
					inc = rand2;
				else
					inc = rand1;

				ItrGuild->second->SyncUpdate();

				positionAvail[ItrGuild->second->GetGuildPvPGroup() - 1] ++;
				if (positionAvail[ItrGuild->second->GetGuildPvPGroup() - 1] == 1)
				{
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(ItrGuild->second->GetName().c_str());
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(_T(" VS "));
				}
				else
				{
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(ItrGuild->second->GetName().c_str());
					groupString[ItrGuild->second->GetGuildPvPGroup() - 1].append(_T("\\r"));
				}

				//find the same group guild
				UInt index2 = 0;	
				for (StlMultiMap<UInt8, CGuild *>::iterator Itr2 = guildPvPGrp.begin();
					Itr2 != guildPvPGrp.end() && index2 < maxGroup * 2; ++Itr2, ++index2)
				{
					if (Itr2->second->GetGuildPvPGroup() == ItrGuild->second->GetGuildPvPGroup() &&
						Itr2->second != ItrGuild->second)
					{
						ItrGuild->second->SetGroupPvPVSGuild(Itr2->second->GetName());
						ItrGuild->second->SyncUpdate();
						Itr2->second->SetGroupPvPVSGuild(ItrGuild->second->GetName());
						Itr2->second->SyncUpdate();
						break;
					}
				}
			}		

		}
		//add back some remove guild		
		for (UInt i = 0; i < removeGuild.size(); i++)
		{
			if (i % 2 == 0)
				maxGroup ++;
			removeGuild[i]->second->SetGuildPvPGroup(maxGroup);
			removeGuild[i]->second->MarkUpdate(CGuild::guildPvPGroupMask);

			removeGuild[i]->second->SyncUpdate();

			positionAvail[removeGuild[i]->second->GetGuildPvPGroup() - 1] ++;
			if (positionAvail[removeGuild[i]->second->GetGuildPvPGroup() - 1] == 1)
			{
				groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(removeGuild[i]->second->GetName().c_str());
				groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(_T(" VS "));
			}
			else
			{
				groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(removeGuild[i]->second->GetName().c_str());
				groupString[removeGuild[i]->second->GetGuildPvPGroup() - 1].append(_T("\\r"));
			}

			//find the same group guild
			UInt index2 = 0;	
			for (StlMultiMap<UInt8, CGuild *>::iterator Itr2 = guildPvPGrp.begin();
				Itr2 != guildPvPGrp.end() && index2 < maxGroup * 2; ++Itr2, ++index2)
			{
				if (Itr2->second->GetGuildPvPGroup() == removeGuild[i]->second->GetGuildPvPGroup() &&
					Itr2->second != removeGuild[i]->second)
				{
					removeGuild[i]->second->SetGroupPvPVSGuild(Itr2->second->GetName());
					removeGuild[i]->second->SyncUpdate();
					Itr2->second->SetGroupPvPVSGuild(removeGuild[i]->second->GetName());
					Itr2->second->SyncUpdate();
					break;
				}
			}
		}

		String msg;
		msg = gGlobal.GetStringTable()->Get(_T("MSG_GUILDWAR_END_FIRST_CONTINUE"));
		msg.append(_T("\\r"));
		for (UInt i = 0; i < maxGroup; i ++)
		{
			msg.append(groupString[i].c_str());
		}

		SendRumorMessage(pPeer, context, msg, 0, 1);
	}
	else if (guildWarState == GUILD_WAR_STATE_WAR_FINISH_END)
	{
		ObjectService_Proxy::ResetGuildGroup(gGlobal.pObjectService->pPeer, RPCContext());
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			if (Itr->second->GetGuildPvPGroup())
			{
				Itr->second->SetGuildPvPGroup(0);
				Itr->second->SyncUpdate();
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ApplyChange(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, 
										   const RPCMask &changeMask, const Int32Vector &updatedata, const UInt8 &source)
{
	GuildMap::iterator Itr = gGlobal.m_GuildMap.find(guild_uid);
	if ((Itr != gGlobal.m_GuildMap.end()) && Itr->second != NULL) 
	{
		UInt i = 0;
		do
		{
			if (updatedata.size() == i)
				break;
			if (changeMask.IsBit(CGuild::fundMask))
			{
				Itr->second->ChangeFund(updatedata[i], source);
				if (updatedata.size() == i)
					break;
			}

			/*if (changeMask.IsBit(CGuild::guildPvPCWMoraleMask))
			{
				Itr->second->ChangePvPCWMorale(source);
				if (updatedata.size() == i)
					break;
			}*/

			if (changeMask.IsBit(CGuild::guildPvPScoreMask))
			{
				Itr->second->ChangePvPScore(updatedata[i]);
				if (updatedata.size() == i)
					break;
			}

			if (changeMask.IsBit(CGuild::guildPvPBidMask))
			{
				Itr->second->ChangePvPBid(updatedata[i]);
				if (updatedata.size() == i)
					break;
			}

			if (changeMask.IsBit(CGuild::prosperMask))
			{
				Itr->second->ChangeProsper(updatedata[i], source);
				if (updatedata.size() == i)
					break;
			}
			if (changeMask.IsBit(CGuild::manorRoomMask))
			{
				Itr->second->ChangeRoom(updatedata[i]);
				if (updatedata.size() == i)
					break;
			}
			if (changeMask.IsBit(CGuild::manorMaterialMask))
			{
				Itr->second->ChangeMaterial(updatedata[i]);
				if (updatedata.size() == i)
					break;
			}
			if (changeMask.IsBit(CGuild::manorAuraMask))
			{
				if (updatedata[i] >= 0 && updatedata[i] <= 4)
                    Itr->second->SetAura(updatedata[i]);
				if (updatedata.size() == i)
					break;
			}
			if (changeMask.IsBit(CGuild::manorTrainPlayerPowMask))
			{
				if (updatedata[i] != 0){
                    if (i == 0){
                        Itr->second->ChangeTrainCharEx(updatedata[i]);
					}
					else if (i == 1){
						Itr->second->ChangeTrainCharIn(updatedata[i]);
					}
				}
				if (updatedata.size() == i)
					break;
			}
			if (changeMask.IsBit(CGuild::manorTrainPartnerPowMask))
			{
				if (updatedata[i] != 0){
                    if (i == 0){
                        Itr->second->ChangeTrainPartnerEx(updatedata[i]);
					}
					else if (i == 1){
						Itr->second->ChangeTrainPartnerIn(updatedata[i]);
					}
				}
				if (updatedata.size() == i)
					break;
			}
			if (changeMask.IsBit(CGuild::manorRoomPropertyMask))
			{
				if (updatedata[i] != 0){
					if (i == 0){
						Itr->second->ChangeDrug(updatedata[i]);
					}
					else if (i == 1){
						Itr->second->ChangeCook(updatedata[i]);
					}
					else if (i == 2){
						Itr->second->ChangeWine(updatedata[i]);
					}
					else if (i == 3){
						Itr->second->ChangeManorHP(updatedata[i]);
					}
					else if (i == 4){
						Itr->second->ChangeManorSP(updatedata[i]);
					}
				}
				if (updatedata.size() == i)
					break;
			}
			if (changeMask.IsBit(CGuild::autoManageBusinessMask))
			{
				Itr->second->ChangeAutoManageBusiness(updatedata[i]);
				if (updatedata.size() == i)
					break;
			}
			i ++;
		}while(i < updatedata.size());

		if (i > 0)
			Itr->second->SyncUpdate();
	}
	return RPC_RESULT_OK;
}
RPCResult MessageService_Stub::UnregisterMap(LPCPEER pPeer, RPCContext &context, 
											 const ServiceID &serviceId, const UInt16 &map_id,
											 const UInt8 &line_id, const UInt32 &owner_id)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("serviceId %d, map_id %d, line_id %d, owner_id %d"),
		serviceId, map_id, line_id, owner_id);

	gGlobal.UnregisterMap(MapIndex(map_id, line_id, owner_id));

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::RegisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("RegisterProxy: serviceId %d"), serviceId);

	MessageApplication::GetInstance().RegisterProxy(pPeer, serviceId);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::UnregisterProxy(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UnregisterProxy: serviceId %d"), serviceId);

	MessageApplication::GetInstance().UnregisterProxy(serviceId);
	gGlobal.UnregisterProxy(pPeer);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ClientDisconnect(LPCPEER pPeer, RPCContext &context)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL)
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("[session not found], sessionId %d"),
			context.sessionId.GetString().c_str());
		return RPC_RESULT_FAIL;
	}

	if (psess->pMsgChar && 
		psess->pMsgChar == FindNetObject<CMessageCharacter>(psess->msgCharNetId))
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("sessionId %d, char_id %d"),
			context.sessionId.GetString().c_str(), psess->pMsgChar->GetChar_id());
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(psess->pMsgChar->GetChar_id());
		if (pMsgInfo)
			pMsgInfo->pProxyPeer = NULL;
	}

	// check if lock too long, then timeout fail the locked procedure
	ProcedureManager::GetInstance().CheckLockTimeout(context.sessionId);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ReleaseSession(LPCPEER pPeer, RPCContext &context,
											  const SessionID &sid,
											  const Boolean &bDelete)
{
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL)
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_T("ReleaseSession Warning: [session not find], sessionId %s, sid %s"),
			context.sessionId.GetString().c_str(), sid.GetString().c_str());
	}

	UInt32 char_id = 0;
	if (psess && psess->pMsgChar)
		char_id = psess->pMsgChar->GetChar_id();

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ReleaseSession: sessionId %s, char_id %d"), 
		context.sessionId.GetString().c_str(), char_id);

	CChannel *pChannel;
	for (ChannelNameMap::iterator it = gGlobal.m_MapChannel.begin(); 
		it != gGlobal.m_MapChannel.end(); ++it)
	{
		pChannel = it->second;
		if (pChannel)
			pChannel->ClearSession(context.sessionId);
	}
	//for (AllCharMap::iterator it = gGlobal.m_AllCharMap.begin(); it != gGlobal.m_AllCharMap.end(); it++)
	//{
	//	if (it->second.SID == context.sessionId)
	//	{
	//		gGlobal.ReleaseChar(it->second.pMsgChar->GetChar_id());
	//		break;
	//	}
	//}

	// change message character to offline status
	if (psess)
	{
		if (psess->pMsgChar && 
			psess->pMsgChar == FindNetObject<CMessageCharacter>(psess->msgCharNetId))
		{
			CMessageCharacter* pMsgChar = psess->pMsgChar;
			pMsgChar->ChangeStatus(FRIENC_CHAT_STATUS_OFFLINE);
			gGlobal.OnUpdateCharStatus(pMsgChar);
			UnsetOwnObject(context.sessionId, pMsgChar);
			if (pMsgChar->HasGuild())
			{
				CGuild* pGuild = gGlobal.GetGuild(pMsgChar->GetGuild_uid());
				if (pGuild)
				{
					CGuildMember* pMember = pGuild->GetMember(char_id);
					UnsetOwnObject(context.sessionId, pMember);
				}
			}
		}
		psess->msgCharNetId.srvId = INVALID_SERVICE_ID;
		psess->pMsgChar = NULL;
	}
	else
	{
		ProcedureManager::GetInstance().RemoveBySession(context.sessionId);
	}

	gGlobal.RemoveProxySid(context.sessionId);
	RemoveSession(context.sessionId);

	return RPC_RESULT_OK;
}


RPCResult MessageService_Stub::ResumeMessageChannel(LPCPEER pPeer, RPCContext &context, 
													const UInt32 &char_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ResumeMessageChannel: session %s, char_id %d"), 
		context.sessionId.GetString().c_str(), char_id);

	LPCPEER pProxyPeer = GetProxy(context.sessionId.serviceId);
	if (pProxyPeer == NULL)			// proxy service not found
		return RPC_RESULT_OK;

	// do InitMessageChannel if not found message character
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo == NULL)
	{
		PROCEDURE_DO_RPC1(MessageService, InitMessageChannel, NULL, pPeer, context, UInt32, char_id)
		return RPC_RESULT_OK;
	}
		
	CMessageCharacter* pMsgChar = pMsgInfo->pMsgChar;
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL)
		psess = AddSession(context.sessionId, Session());
	gGlobal.AddProxySid(context.sessionId);

	// change message character to online status
	pMsgInfo->SID = context.sessionId;
	pMsgInfo->pProxyPeer = pProxyPeer;
	psess->pMsgChar = pMsgChar;
	psess->msgCharNetId = pMsgChar->GetNetId();
	SetOwnObject(context.sessionId, pMsgChar);
	pMsgChar->ChangeStatus(FRIENC_CHAT_STATUS_ONLINE);

	// join back channels
	CB_InitMessageChannel(pProxyPeer, context, MSG_RETURN_SUCCESS);
	//send global channel and global channel to players
	JoinMessageChannel(pProxyPeer, context, pMsgChar->GetChar_id(), pMsgChar->GetNickName(),
		ChannelPrefix[CHANNELTYPE_GLOBAL], CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);
	//send speaker channel and speaker channel to players
	JoinMessageChannel(pProxyPeer, context, pMsgChar->GetChar_id(), pMsgChar->GetNickName(),
		ChannelPrefix[CHANNELTYPE_SPEAKER], CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);
	// send shutdown message when services shutting down
	if (gGlobal.m_downTime > 0)
		CB_SendShutdownMessage(pProxyPeer, context, gGlobal.m_downTime);

	if (pMsgChar->GetFaction() > 0)
	{
		String channelName;
		channelName.Format(_T("%s_%d"), ChannelPrefix[CHANNELTYPE_GANG], pMsgChar->GetFaction());
		JoinMessageChannel(pProxyPeer, context, pMsgChar->GetChar_id(),  
			pMsgChar->GetNickName(), channelName, CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);
	}

	// send message characters, messengers NetObjects
	NetGroup msgChars;
	pMsgChar->SetSendMask(CMessageCharacter::AllMask);
	msgChars.Add(pMsgChar);
	CMessenger* pMessenger;
	CMessageCharacter* pMsgChar2;
	NetGroup* messengers = pMsgInfo->messengers;
	if (messengers) {
		for (UInt16 i = 0; i < messengers->GetCount(); ++i)
		{
			pMessenger = messengers->GetChild<CMessenger>(i);
			if (pMessenger)
			{
				SetOwnObject(context.sessionId, pMessenger);
				pMessenger->PreSend();
				pMsgChar2 = gGlobal.GetMsgCharRef(pMessenger->GetFriend_char_id());
				if (pMsgChar2)
				{
					pMsgChar2->SetSendMask(CMessageCharacter::AllMask);
					msgChars.Add(pMsgChar2);
				}
			}
		}
		CB_UpdateMsgCharInfo(pProxyPeer, context, &msgChars);
		CB_UpdateMessengers(pProxyPeer, context, messengers);
	}

	if (pMsgChar->HasGuild())
	{
		// send guild and guild member NetObjects
		CGuild* pGuild = gGlobal.GetGuild(pMsgChar->GetGuild_uid());
		if (pGuild)			// if guild found
		{
			CGuildMember* pMember = pGuild->GetMember(char_id);
			if (pMember)
			{
				SetOwnObject(context.sessionId, pMember);
				pGuild->SendGuild(pMsgInfo, pMember);
			}
		}
	}

	gGlobal.OnUpdateCharStatus(pMsgChar);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ReleaseMsgCharacter(LPCPEER pPeer, RPCContext &context, 
												   const UInt32 &char_id)
{
	// release character only when no session found
	Session* psess = GetSession(context.sessionId);
	if (psess == NULL)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ReleaseMsgCharacter: session %s, char_id %d"), 
			context.sessionId.GetString().c_str(), char_id);
		gGlobal.ReleaseChar(char_id);
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GetAvailService(LPCPEER pPeer, RPCContext &context)
{
	CB_GetAvailService(pPeer, context, gConfigure.serviceId);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ApplyGuild(LPCPEER pPeer, RPCContext &context, 
										  const UInt32 &char_id, const UInt32 &guild_uid)
{
	if (!gGlobal.IsLockCharGuild(char_id))
	{
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo && !pMsgInfo->pMsgChar->HasGuild())
		{
			GuildMap::iterator Itr = gGlobal.m_GuildMap.find(guild_uid);
			if (Itr != gGlobal.m_GuildMap.end())
			{
				Itr->second->InsertApplicant(pMsgInfo->pMsgChar);
				CB_ApplyGuild(pPeer, context, guild_uid);
			}
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::InitMessageChannel(LPCPEER pPeer, RPCContext &context, 
												  const UInt32 &char_id)
{
	PROCEDURE_START_RPC1(MessageService, InitMessageChannel, NULL, UInt32, char_id)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitMessageChannel: session %s, char_id %d"), 
			context.sessionId.GetString().c_str(), char_id);
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_STORE_VALUE2(UInt32, curTime, UInt32, curTime)

		CMessageCharacter* pMsgChar = gGlobal.GetMsgCharRef(char_id);
		if (pMsgChar && !pMsgChar->GetInactive())
		{
			// normal message character found
			NetGroup dummy;
			PROCEDURE_GO_STEP1(1, NetGroup*, &dummy);
		}
		else
		{
			UInt32Vector charList;
			charList.push_back(char_id);
			ObjectService_Proxy::GetMessageCharacters(gGlobal.pObjectService->pPeer, 
				context, charList, BOOLEAN_TRUE);
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_GetMsgCharacterGroup, NetGroup *, charGroup)

		TRACE_ENSURE(charGroup);
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("InitMessageChannel (1): session %s, char_id %d, charGroup %d, utime %d"), 
			context.sessionId.GetString().c_str(), char_id, charGroup->GetCount(),
			curTime - lastTime);
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		
		CMessageCharacter* pMsgChar = gGlobal.GetMsgCharRef(char_id);
		if (charGroup->GetCount() == 0 && pMsgChar == NULL) {
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, 
				_T("InitMessageChannel Error: character not found, char_id %d"), char_id);
			DeleteNetGroup(charGroup);
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		CMsgCharacterData *pCharData = NULL;
		if (charGroup->GetCount() > 0)
			pCharData = charGroup->GetChild<CMsgCharacterData>(0);
			
		Session* psess = GetSession(context.sessionId);
		if (psess == NULL)
			psess = AddSession(context.sessionId, Session());

		gGlobal.AddProxySid(context.sessionId);
		bool infoExist = false;
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo)
		{
			pMsgInfo->SID = context.sessionId;
			pMsgChar = pMsgInfo->pMsgChar;
			if (pMsgChar->GetInactive())
				pMsgChar->SetData(pCharData);
			infoExist = true;
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_T("InitMessageChannel: [MsgInfo already exist], session %s, char_id %d"), 
				context.sessionId.GetString().c_str(), char_id);
		}
		else
		{
			pMsgInfo = gGlobal.AddMsgInfo(char_id, pCharData, context.sessionId);
			pMsgChar = pMsgInfo->pMsgChar;
		}

		// set session message character
		psess->pMsgChar = pMsgChar;
		psess->msgCharNetId = pMsgChar->GetNetId();
		SetOwnObject(context.sessionId, pMsgChar);

		LPCPEER pProxyPeer = GetProxy(context.sessionId.serviceId);
		pMsgInfo->pProxyPeer = pProxyPeer;
		if (pProxyPeer) 
		{
			pMsgChar->ChangeStatus(FRIENC_CHAT_STATUS_ONLINE);
			//connect client to message service
			CB_InitMessageChannel(pProxyPeer, context, MSG_RETURN_SUCCESS);
			//send global channel and global channel to players
			JoinMessageChannel(pProxyPeer, context, char_id, pMsgChar->GetNickName(),
				ChannelPrefix[CHANNELTYPE_GLOBAL], CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);
			//send speaker channel and speaker channel to players
			JoinMessageChannel(pProxyPeer, context, char_id, pMsgChar->GetNickName(),
				ChannelPrefix[CHANNELTYPE_SPEAKER], CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST);
			// send shutdown message when services shutting down
			if (gGlobal.m_downTime > 0)
				CB_SendShutdownMessage(pProxyPeer, context, gGlobal.m_downTime);

			if (pMsgChar->GetFaction() > 0)
			{
				String channelName;
				channelName.Format(_T("%s_%d"), ChannelPrefix[CHANNELTYPE_GANG], 
					pMsgChar->GetFaction());
				JoinMessageChannel(pProxyPeer, context, char_id, pMsgChar->GetNickName(), 
					channelName, CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST );
			}
		}
		if (pCharData)
			DeleteNetObject(pCharData);
		DeleteNetGroup(charGroup);

		if (infoExist)
		{
			NetGroup dummy;
			PROCEDURE_GO_STEP1(2, NetGroup*, &dummy);
		}
		else
			ObjectService_Proxy::GetMessengerLst(gGlobal.pObjectService->pPeer, context, char_id);

	PROCEDURE_WAIT1(2, ObjectService, CB_GetMessengerLst, NetGroup *, messengers)

		TRACE_ENSURE(messengers != NULL);
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("InitMessageChannel (2): session %s, char_id %d, messengers %d, utime %d"), 
			context.sessionId.GetString().c_str(), char_id, messengers->GetCount(),
			curTime - lastTime);
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		UInt32Vector char_idList;
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo)
		{
			if (pMsgInfo->messengers == NULL)
				pMsgInfo->messengers = CreateNetGroup();

			for (int i = 0; i < messengers->GetCount(); ++i)
			{
				CMessengerData * pMessengerData = messengers->GetChild<CMessengerData>(i);
				if (pMessengerData)
				{
					if (pMessengerData->GetFriend_group() < MAX_FRIEND_GROUP_TYPE)
						pMsgInfo->messengerCount[pMessengerData->GetFriend_group()] ++;
					CMessenger * pMessenger = CMessenger::NewInstance();
					UInt32 friend_id = pMessengerData->GetFriend_char_id();
					pMessenger->SetData(pMessengerData);
					SetOwnObject(context.sessionId, pMessenger);
					gGlobal.InsertCharWatch(char_id, friend_id);	// add friend to watcher list
					if (gGlobal.GetMsgCharRef(friend_id, true) == NULL)
						char_idList.push_back(friend_id);

					pMessenger->PreSend();
					pMsgInfo->messengers->Add(pMessenger);

					gGlobal.InsertCharWatch(friend_id, char_id);
				}
			}
		}
		DeleteNetGroup(messengers, BOOLEAN_TRUE);

		LPCPEER pPeer = GetProxy(context.sessionId.serviceId);
		if (pPeer) 
			CB_UpdateMessengers(pPeer, context, pMsgInfo->messengers);
		//find back the unfound character

		if (char_idList.size() == 0)
		{
			NetGroup dummy;
			PROCEDURE_GO_STEP1(3, NetGroup*, &dummy)
		}
		else
		{
			ObjectService_Proxy::GetMessageCharacters(gGlobal.pObjectService->pPeer, context,
				char_idList, BOOLEAN_FALSE);
		}

	PROCEDURE_WAIT1(3, ObjectService, CB_GetMsgCharacterGroup, NetGroup*, charGroup)

		TRACE_ENSURE(charGroup != NULL);
		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("InitMessageChannel (3): session %s, char_id %d, charGroup %d, utime %d"), 
			context.sessionId.GetString().c_str(), char_id, charGroup->GetCount(),
			curTime - lastTime);
		PROCEDURE_STORE_VALUE2(UInt32, firstTime, UInt32, curTime)
		UInt32 guild_uid = 0;
		CGuild* pGuild = NULL;
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo)
		{
			CMsgCharacterData *pCharData;
			CMessageCharacter* pMsgChar;

			if (pMsgInfo->pMsgChar)
			{
				guild_uid = pMsgInfo->pMsgChar->GetGuild_uid();
				if (guild_uid != 0)
					pGuild = gGlobal.GetGuild(guild_uid);
				
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitMessageChannel: char_id %d belongs to the guild with uid %d"), 
				char_id, guild_uid);
			}

			for (int i = 0; i < charGroup->GetCount(); ++i)
			{
				pCharData = ((NetGroup*) charGroup)->GetChild<CMsgCharacterData>(i);
				if (pCharData)
					pMsgChar = gGlobal.AddMsgCharRef(pCharData->GetChar_id(), pCharData);
			}
			if (pMsgInfo->pProxyPeer) 
			{
				NetGroup grp;
				//send the group to client
				if (pMsgInfo->messengers)
				{
					for (int i = 0; i < pMsgInfo->messengers->GetCount(); ++i)
					{
						CMessenger * pMessenger = pMsgInfo->messengers->GetChild<CMessenger>(i);
						if (pMessenger)
						{
							pMsgChar = gGlobal.GetMsgCharRef(pMessenger->GetFriend_char_id());
							if (pMsgChar)
							{
								pMsgChar->PreSend();
								grp.Add(pMsgChar);
							}
						}
					}
				}

				pMsgInfo->pMsgChar->SetSendMask(CMessageCharacter::AllMask);
				grp.Add(pMsgInfo->pMsgChar);
				CB_UpdateMsgCharInfo(pMsgInfo->pProxyPeer, context, &grp);

				//call back all offline message
				for (OfflineMsgMap::iterator it = gGlobal.m_AllOfflineMsg.find(char_id);
					it != gGlobal.m_AllOfflineMsg.end() && it->first == char_id;)
				{
					CB_ReceivePrivateMessage(pMsgInfo->pProxyPeer, context, it->second.msg, 
						it->second.name, it->second.srcID, pMsgInfo->pMsgChar->GetNickName() ,
						char_id, true); 
					it = gGlobal.m_AllOfflineMsg.erase(it);
				}
			}
		}
		DeleteNetGroup(charGroup, BOOLEAN_TRUE);

		if (guild_uid == 0 || pGuild) {		// character no guild or guild already exist
			PROCEDURE_GO_STEP(4)
		} else {
			
			PROCEDURE_DO_METHOD1(CGuildControl, GetGuildByID, gGlobal.m_guildControl, context,
				UInt32, guild_uid)
		}

	PROCEDURE_WAIT_RETURN(4)

		UInt32 curTime = ::GetTickCount();
		PROCEDURE_RESTORE_VALUE2(UInt32, firstTime, UInt32, lastTime)
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_T("InitMessageChannel (End): session %s, char_id %d, utime %d, total time %d"), 
			context.sessionId.GetString().c_str(), char_id, curTime - lastTime, 
			curTime - firstTime);
		
		//guild stuff
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo)
		{
			if (pMsgInfo->pMsgChar->HasGuild())
			{
				UInt32 guild_uid = pMsgInfo->pMsgChar->GetGuild_uid();
				CGuild* pGuild = gGlobal.GetGuild(guild_uid);
				if (pGuild)
				{
					CGuildMember* pMember = pGuild->GetMember(char_id);
					if (pMember)
					{
						// valid in guild
						SetOwnObject(context.sessionId, pMember);
						pMsgInfo->pMsgChar->SetGuild(pGuild->GetGuild_uid(), pGuild->GetName());
						if (pMember->GetInactive() && !pMsgInfo->pMsgChar->GetInactive())
						{
							// member reactivated
							pMember->SetData(pMsgInfo->pMsgChar, false);
							pGuild->SyncUpdate();
						}
						pGuild->SendGuild(pMsgInfo, pMember);
						TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitMessageChannel: char_id %d successfully get his own guild with uid %d"), 
										char_id, guild_uid);
					}
					else
					{
						// already kicked
						pMsgInfo->pMsgChar->SetGuild(0, _T(""));
						pGuild->CheckRelease();
						TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitMessageChannel: char_id %d already being kicked from guild with uid %d"), 
										char_id, guild_uid);
					}
				}
				else
				{
					// guild already deleted
					pMsgInfo->pMsgChar->SetGuild(0, _T(""));
				}
			} 
			gGlobal.OnUpdateCharStatus(pMsgInfo->pMsgChar);
		}

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			gGlobal.RemoveProxySid(context.sessionId);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::JoinMessageChannel(LPCPEER pPeer, RPCContext &context, 
												  const UInt32 &char_id, const String &char_Name, 
												  const String &ChannelName, const BYTE &JoinMode)
{
	BYTE Result = CHANNEL_NO_ERR;
	CChannel * pChannel = gGlobal.GetChannel(ChannelName);
	//if found
	if (pChannel)
	{
		if (JoinMode == CHANNEL_JOIN_MODE_NOT_CREATE_IF_EXIST)
			Result = CHANNELERR_CHANNEL_EXIST;
	}
	//if not found
	else
	{
		//create channel
		if (JoinMode == CHANNEL_JOIN_MODE_CREATE_IF_NOT_EXIST || 
			JoinMode == CHANNEL_JOIN_MODE_NOT_CREATE_IF_EXIST)
		{
			//create a new channel 
			String strType = ChannelName;
			//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendGrpFlushMessage: strType address 0x%08x Msg: %s"), strType.c_str(), strType.c_str());
			size_t pos = ChannelName.find_first_of(Del);
			if (pos != std::string::npos)
			{
				strType = ChannelName.substr(0, pos);
			}
			for (BYTE Type = CHANNELTYPE_MAP; Type < MAX_CHANNELTYPE; Type ++)
			{
				if (strType == ChannelPrefix[Type])
				{
					pChannel = CChannel::NewInstance();
					pChannel->SetInfo(Type);
					pChannel->m_channelName = ChannelName;
					pChannel->SetSendMask(pChannel->GetUpdateMask());
					gGlobal.m_MapChannel.insert(std::make_pair(ChannelName, pChannel));
					break;
				}
			}
		}
		else if (JoinMode == CHANNEL_JOIN_MODE_JOIN_ONLY_EXIST)
		{
			Result = CHANNELERR_NOT_FOUND;
		}
	}


	if (pChannel && Result == CHANNEL_NO_ERR)
	{
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo)
		{
			pChannel->InsertCharContext(context, pMsgInfo->pMsgChar); 
			CB_ReceiveMessageChannel(pMsgInfo->pProxyPeer, context, pChannel, 
				pChannel->GetChannelType());
		}
	}

	CB_JoinMessageChannel(pPeer, context, Result);

	return RPC_RESULT_OK;
}


RPCResult MessageService_Stub::SendPrivateMessage(LPCPEER pPeer, RPCContext &context, 
												  const String &Msg, const UInt32 &SourceId, 
												  const UInt32 &TargetID, 
												  const String &TargetName, 
												  const boolean &isFriend)
{
	if (Msg.IsEmpty())
		return RPC_RESULT_OK;

	UInt sizeofMsg = NoCodeStringSize(String(Msg));
	if (sizeofMsg > MaxMsgLongLen)
	{
		CB_Error(pPeer, context, 0, _T("MSG_MESSAGE_TOO_LONG"));
		return RPC_RESULT_OK;
	}
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("SendPrivateMessage: From %d to %d or %s"), SourceId, TargetID, TargetName.c_str());

	UInt32 CurrTime = (UInt32)time(NULL);
	BYTE Result = MSG_RETURN_FAIL_NOT_FOUND;
	UInt32 resultFlag = 0;
	CharMsgInfo* pSrcMsgInfo = gGlobal.GetMsgInfo(SourceId);
	if (pSrcMsgInfo == NULL && SourceId != 0)
		return RPC_RESULT_FAIL;

	UInt32 srcId = SourceId;
	String srcName = SourceId == 0?String("system"):pSrcMsgInfo->pMsgChar->GetNickName();
	UInt32 dstId = TargetID;
	String dstName = TargetName;
	String blockedMsg = Msg;
	gGlobal.BlockWord(blockedMsg);
	if (pSrcMsgInfo != NULL && (CurrTime - pSrcMsgInfo->PreSpeakTime < SpeakRestTime[CHANNELTYPE_PRIVATE]))
	{
		resultFlag = SpeakRestTime[CHANNELTYPE_PRIVATE] + pSrcMsgInfo->PreSpeakTime - CurrTime;
		Result = MSG_RETURN_FAIL_TOO_FREQUENT;
	}
	else
	{	
		if(pSrcMsgInfo != NULL)
			pSrcMsgInfo->PreSpeakTime = CurrTime;
		CharMsgInfo* pDstMsgInfo = gGlobal.GetMsgInfo(TargetID);
		if (pDstMsgInfo == NULL)
			pDstMsgInfo = gGlobal.GetMsgInfo(TargetName);
		if (pDstMsgInfo)
		{
			dstName = pDstMsgInfo->pMsgChar->GetNickName();
			dstId = pDstMsgInfo->pMsgChar->GetChar_id();
			if (dstId == srcId)
				return RPC_RESULT_FAIL;

			UInt32Vector tempList;
			tempList.push_back(dstId);
			String emptyText;
			WriteMsgLog(context, CHANNELTYPE_PRIVATE, srcId, srcName, tempList, emptyText, blockedMsg);

			if(pSrcMsgInfo != NULL)
			{
				CB_ReceivePrivateMessage(pSrcMsgInfo->pProxyPeer, RPCContext(pSrcMsgInfo->SID),
					blockedMsg, srcName, srcId, dstName, dstId, isFriend); 
			}
			//check offline
			if (pDstMsgInfo->pMsgChar->IsOffline())
			{
				if (gGlobal.AddOfflineMsg(dstId, srcId, srcName, blockedMsg))
					Result = MSG_RETURN_SUCCESS;
				else
					Result = MSG_RETURN_FAIL_TOO_MUCH;
			}
			else
			{	
				if (pDstMsgInfo->pProxyPeer) 
				{
					if (isFriend && pDstMsgInfo->pMsgChar->IsAway() &&
						!pDstMsgInfo->pMsgChar->GetFriend_onlineAutoReply().IsEmpty())
					{
						if(pSrcMsgInfo != NULL)
						{
							CB_ReceivePrivateMessage(pSrcMsgInfo->pProxyPeer, 
									RPCContext(pSrcMsgInfo->SID), 
									pDstMsgInfo->pMsgChar->GetFriend_onlineAutoReply(), 
									dstName, dstId, srcName, srcId, isFriend); 
						}
					}

					//check black list
					bool isBlock = gGlobal.IsBlockList(dstId, srcId);

					if (!isBlock && (isFriend || (!isFriend && pDstMsgInfo->Listen)))
					{
						CB_ReceivePrivateMessage(pDstMsgInfo->pProxyPeer, 
							RPCContext(pDstMsgInfo->SID), blockedMsg, 
							srcName, srcId, dstName, dstId, isFriend);
					}
				}
				Result = MSG_RETURN_SUCCESS;
			}
		}
		else if (isFriend && TargetID != 0)
		{
			CMessageCharacter* pMsgInfo = gGlobal.GetMsgCharRef(TargetID);
			if (pMsgInfo == NULL)
				pMsgInfo = gGlobal.GetMsgCharRef(TargetName);
			if (pMsgInfo == NULL)
			{
				Result = MSG_RETURN_FAIL_NOT_FOUND;
			}
			else if (pMsgInfo->GetInactive())
			{
				Result = MSG_RETURN_FAIL_INACTIVE;
			}
			else
			{
				if (gGlobal.AddOfflineMsg(TargetID, srcId, srcName, blockedMsg))
				{
					Result = MSG_RETURN_SUCCESS;
					if(pSrcMsgInfo != NULL)
					{
						CB_ReceivePrivateMessage(pSrcMsgInfo->pProxyPeer, RPCContext(pSrcMsgInfo->SID),
							blockedMsg, srcName, srcId, dstName, dstId, isFriend); 
					}
				}
				else
					Result = MSG_RETURN_FAIL_TOO_MUCH;
			}
		}
	}
	if(pSrcMsgInfo != NULL)
	{
		CB_SendPrivateMessage(pSrcMsgInfo->pProxyPeer, RPCContext(pSrcMsgInfo->SID), Result, 
			isFriend, resultFlag); 
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::SendHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &SourceId,
													 const UInt32 & count)
{
	if (Msg.IsEmpty())
		return RPC_RESULT_OK;

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendHorseLightMessage: Msg address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());

	AllCharMap::iterator it = gGlobal.m_AllCharMap.begin();
	String blockedMsg = Msg;
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendHorseLightMessage: blockedMsg address 0x%08x Msg: %s"), blockedMsg.c_str(), blockedMsg.c_str());
	//gGlobal.BlockWord(blockedMsg);

	for (ProxySidMap::iterator it = gGlobal.m_proxySidMap.begin(); 
		it != gGlobal.m_proxySidMap.end(); ++it)
		CB_ReceiveHorseLightMessage(it->first, RPCContext(), blockedMsg, count, it->second);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::SendRumorMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, 
												const UInt32 &SourceId, const BYTE &SubType)
{
	if (Msg.IsEmpty())
		return RPC_RESULT_OK;


	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendRumorMessage: Msg address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());

	AllCharMap::iterator it = gGlobal.m_AllCharMap.begin();
	String blockedMsg = Msg;

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendRumorMessage: blockedMsg address 0x%08x Msg: %s"), blockedMsg.c_str(), blockedMsg.c_str());
	//gGlobal.BlockWord(blockedMsg);

	for (ProxySidMap::iterator it = gGlobal.m_proxySidMap.begin(); 
		it != gGlobal.m_proxySidMap.end(); ++it)
		CB_ReceiveRumorMessage(it->first, RPCContext(), blockedMsg, SubType, it->second);

	BYTE Result = MSG_RETURN_SUCCESS;
	CB_SendRumorMessage(pPeer, context, Result);
	UInt32Vector temp;
	String empty;
	WriteMsgLog(context, CHANNELTYPE_RUMOR, 0, empty, temp, empty, blockedMsg);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::SendSystemMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, 
												 const UInt32 &SourceId)
{
	if (Msg.IsEmpty())
		return RPC_RESULT_OK;


	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendRumorMessage: Msg address 0x%08x Msg: %s"), Msg.c_str(), Msg.c_str());

	AllCharMap::iterator it = gGlobal.m_AllCharMap.begin();
	String blockedMsg = Msg;
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendRumorMessage: blockedMsg address 0x%08x Msg: %s"), blockedMsg.c_str(), blockedMsg.c_str());
	//gGlobal.BlockWord(blockedMsg);

	for (ProxySidMap::iterator it = gGlobal.m_proxySidMap.begin(); 
		it != gGlobal.m_proxySidMap.end(); ++it)
		CB_ReceiveSystemMessage(it->first, RPCContext(), blockedMsg, it->second);


	BYTE Result = MSG_RETURN_SUCCESS;
	CB_SendSystemMessage(pPeer, context, Result);
	UInt32Vector temp;
	String empty;
	WriteMsgLog(context, CHANNELTYPE_SYSTEM, 0, empty, temp, empty, blockedMsg);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::CharacterLeave(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CharacterLeave: session %s, char_id %d"), 
		context.sessionId.GetString().c_str(), char_id);
	gGlobal.LeaveAllChannel(pPeer, context, char_id);
	gGlobal.ReleaseChar(char_id);
		
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::LeaveAllChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	gGlobal.LeaveAllChannel(pPeer, context, char_id);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::LeaveChannel(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &ChannelName)
{
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("LeaveChannel: ChannelName address 0x%08x Msg: %s"), ChannelName.c_str(), ChannelName.c_str());
	CChannel * pChannel = gGlobal.GetChannel(ChannelName);
	if (pChannel)
		pChannel->LeaveChannel(pPeer, context, char_id);
	return RPC_RESULT_OK;
}


RPCResult MessageService_Stub::SetListen(LPCPEER pPeer, RPCContext &context, const BOOL &IsListen, const UInt32 &char_id)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo)
	{
		String Name = pMsgInfo->pMsgChar->GetNickName();
		//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SetListen: Name address 0x%08x Msg: %s"), Name.c_str(), Name.c_str());
		pMsgInfo->Listen = IsListen;
	}
	CB_SetListen(pPeer, context); 
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::RemoveMessenger(LPCPEER pPeer, RPCContext &context, 
											   const UInt32 &owner_id, const UInt32 &target_id)
{
	PROCEDURE_START_RPC2(MessageService, RemoveMessenger, NULL, UInt32, owner_id, 
		UInt32, target_id)

		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("RemoveMessenger: owner_id %d, target_id %d"), 
				owner_id, target_id);
			
		//add by taojianping do not remove if has other relation
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
		if (pMsgInfo && pMsgInfo->messengers)
		{
			for (UInt i = 0; i < pMsgInfo->messengers->GetCount(); ++i )
			{
				CMessenger * pMessenger = pMsgInfo->messengers->GetChild<CMessenger>(i);
				if (pMessenger && pMessenger->GetFriend_char_id() == target_id)
				{
					if (pMessenger->GetFriend_relation() != 0 &&
						pMessenger->GetFriend_relation() != (1 << FRIEND_RELATION_FRIEND))
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER,
							_T("RemoveMessenger: [can't remove with other relation], relation %d"),
							pMessenger->GetFriend_relation());
						PROCEDURE_RETURN;
					}
					break;
				}
			}
		}

		ObjectService_Proxy::DeleteMessenger(gGlobal.pObjectService->pPeer, context, owner_id, target_id);

	PROCEDURE_WAIT(1, ObjectService, CB_DeleteMessenger)

	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		CMessenger* pMessenger;
		for (UInt i = 0; i < pMsgInfo->messengers->GetCount(); ++i )
		{
			pMessenger = pMsgInfo->messengers->GetChild<CMessenger>(i);
			if (pMessenger && pMessenger->GetFriend_char_id() == target_id)
			{
				if (pMessenger->GetFriend_group() < MAX_FRIEND_GROUP_TYPE)
				{
					if (pMsgInfo->messengerCount[pMessenger->GetFriend_group()] > 0)
						pMsgInfo->messengerCount[pMessenger->GetFriend_group()] --;
				}
				pMsgInfo->messengers->Remove(pMessenger);

				MessageService_Stub::CB_RemoveMessengers(pMsgInfo->pProxyPeer, 
					RPCContext(pMsgInfo->SID), owner_id, target_id);
				//MessageService_Stub::CB_UpdateMessengers(pMsgInfo->pProxyPeer, 
				//	RPCContext(pMsgInfo->SID), pMsgInfo->messengers);

				gGlobal.RemoveCharWatch(owner_id, target_id);
				UnsetOwnObject(pMsgInfo->SID, pMessenger);
				DeleteNetObject(pMessenger);
				break;
			}
		}
	}
	
	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RemoveMessenger Error: ObjectService:CB_Error, errcode %d, %s"),
				err, errMsg.c_str());
			CB_Error(pPeer, context, err, errMsg);
	
	PROCEDURE_END_CATCH
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::CreateMessenger(LPCPEER pPeer, RPCContext &context, 
											   const UInt32 &owner_id, const UInt32 &target_id,
											   const UInt16Vector &relations, 
											   const UInt8 &group, const UInt16 &friendly,
											   const String &targetName)
{
	PROCEDURE_START_RPC6(MessageService, CreateMessenger, NULL, UInt32, owner_id, 
		UInt32, target_id, UInt16Vector, relations, UInt8, group, 
		UInt16, friendly, String, targetName)

		if (owner_id == target_id)
		{
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger: targetName address 0x%08x Msg: %s"), targetName.c_str(), targetName.c_str());
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger: owner_id %d, target_id %d"), owner_id, target_id);

		if (group >= MAX_FRIEND_GROUP_TYPE)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger: (group not right) owner_id %d, group %d"), owner_id, group);
			CB_Error(pPeer, context, 0, _T("group not right"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (group == FRIEND_GROUP_TEMP)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger: (can't create temp group) owner_id %d, group %d"), owner_id, group);
			CB_Error(pPeer, context, 0, _T("can't create temp group"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt8 relGroup = group;
		//find whether the player exist
		UInt32 FinalTargetID = target_id;
		bool targetFound = false;
		CharMsgInfo* pDstMsgInfo = gGlobal.GetMsgInfo(target_id);
		if (pDstMsgInfo == NULL)
		{
			pDstMsgInfo = gGlobal.GetMsgInfo(targetName);
			if (pDstMsgInfo)
				FinalTargetID = pDstMsgInfo->pMsgChar->GetChar_id();
		}

		if (pDstMsgInfo && pDstMsgInfo->pMsgChar)
		{
			if (relGroup != FRIEND_GROUP_TEMP && relGroup != FRIEND_GROUP_BLACK)
			{
				if (pDstMsgInfo->pMsgChar->GetEnableBlockMakeFriend())
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateMessenger: (MSG_BLOCK_FRIEND) owner_id %d"), owner_id);
					CB_Error(pPeer, context, 0, _T("MSG_BLOCK_FRIEND"));
					PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
				}
			}
			targetFound = true;
		}

		if (!targetFound && group != FRIEND_GROUP_BLACK)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, 
				_T("CreateMessenger: (MSG_PK_REQUEST_RESULT_FAIL_NOT_FOUND_TARGET) owner_id %d"),
				owner_id);
			CB_Error(pPeer, context, 0, _T("MSG_PK_REQUEST_RESULT_FAIL_NOT_FOUND_TARGET"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}
		// add target msg char reference first
		CMessageCharacter* pMsgChar = gGlobal.GetMsgCharRef(target_id, true);

		//find if there is space in group
		CharMsgInfo* pSrcMsgInfo = gGlobal.GetMsgInfo(owner_id);
		if (pSrcMsgInfo == NULL)
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, 
				_T("CreateMessenger: (MSG_CHAT_MSG_ERROR_NOT_ONLINE) owner_id %d"), owner_id);
			CB_Error(pPeer, context, 0, _T("MSG_CHAT_MSG_ERROR_NOT_ONLINE"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		UInt32 groupCount = pSrcMsgInfo->messengerCount[relGroup];
		if ((relGroup == FRIEND_GROUP_TEMP && 
			groupCount >= MAX_MEMBER_TEMP_GROUP) ||
			(relGroup == FRIEND_GROUP_BLACK && 
			groupCount >= MAX_MEMBER_TEMP_GROUP))
		{
			TRACE_INFODTL_1(GLOBAL_LOGGER, 
				_T("CreateMessenger: (MSG_FRIEND_ADD_FAIL_FULL) owner_id %d"), owner_id);
			CB_Error(pPeer, context, 0, _T("MSG_FRIEND_ADD_FAIL_FULL"));
			PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
		}

		if (groupCount >= MAX_MEMBER_EACH_GROUP)
		{
			relGroup = MAX_FRIEND_GROUP_TYPE;
			for (int i = FRIEND_GROUP_ONE; i <= FRIEND_GROUP_FOUR; i ++)
			{
				if (pSrcMsgInfo->messengerCount[i] < MAX_MEMBER_EACH_GROUP)
				{
					relGroup = i;
					break;
				}
			}

			if (relGroup > FRIEND_GROUP_FOUR)
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, 
					_T("CreateMessenger: (group full) owner_id %d"), owner_id);
				CB_Error(pPeer, context, 0, _T("MSG_FRIEND_ADD_FAIL_FULL"));
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			}
		}

		//find whether the relation exist
		if (pSrcMsgInfo->messengers)
		{
			for(Int i = 0; i < pSrcMsgInfo->messengers->GetCount(); i++)
			{
				CMessenger * pMessenger = CastNetObject<CMessenger>(pSrcMsgInfo->messengers->GetChild(i));
				if (pMessenger && pMessenger->GetFriend_char_id() == FinalTargetID)
				{
					UInt16 rels = 0;
					for (UInt8 index = 0 ; index < relations.size(); index ++)
					{						
						if (!pMessenger->IsRelation(relations[index]))
							rels += (1 << relations[index]);
					
					}
					if (rels == 0)
					{
					
						TRACE_INFODTL_2(GLOBAL_LOGGER, 
							_T("CreateMessenger: relation found, owner_id %d, target_id %d"), 
							owner_id, target_id);

						CB_Error(pPeer, context, 0, _T("MSG_FRIEND_ALREADY_FRIEND"));
						PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
					}
					else
					{							
						pMessenger->SetRelation(pMessenger->GetFriend_relation() + rels);
						pMessenger->PreSend();
						CB_UpdateMessengers(pSrcMsgInfo->pProxyPeer, 
							RPCContext(pSrcMsgInfo->SID), pSrcMsgInfo->messengers);
						pMessenger->UpdateToObject();
						PROCEDURE_RETURN
					}
				}
			}
		}

		/*UInt count = (UInt)gGlobal.m_AllCharWatchMap.count(FinalTargetID);
		UInt i = 0;
		for (AllCharWatchMap::iterator Itr = gGlobal.m_AllCharWatchMap.find(FinalTargetID);
			Itr != gGlobal.m_AllCharWatchMap.end() && i < count; ++Itr, ++i)
		{
			if (Itr->second == owner_id)
			{
				TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger Fail: pair found owner_id %d, target_id %d"), owner_id, target_id);
				CB_Error(pPeer, context, 0, _T("MSG_FRIEND_ALREADY_FRIEND"));
				PROCEDURE_ERROR_RETURN(RPC_RESULT_FAIL)
			}
		}*/

		UInt16 relationList = 0;
		for (UInt8 index = 0 ; index < relations.size(); index ++)
			relationList += (1 << relations[index]);
		//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger: relationList address 0x%08x Msg: %s"), relationList.c_str(), relationList.c_str());
		/*TCHAR *valueDelim = _T(",");
		//for (UInt8Vector::iterator Itr = relations.begin(); Itr != relations.end(); ++ Itr)
		for (Int i = 0; i < (Int)relations.size(); i++)
		{
		String relation;
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger: relation address 0x%08x Msg: %s"), relation.c_str(), relation.c_str());
		relation.Format(_T("%d%s"), relations[i], valueDelim);
		relationList += relation.c_str();
		relation.Empty();
		}*/

		if (relGroup == FRIEND_GROUP_TEMP)
		{
			CMessengerData dummy;
			dummy.SetKey(owner_id, FinalTargetID);
			dummy.SetGroup(FRIEND_GROUP_TEMP);
			dummy.SetSendMask(dummy.GetUpdateMask());
			PROCEDURE_GO_STEP1(1, NetObject*, &dummy)
		}
		else
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CreateMessenger Send to Object Service: owner_id %d, target_id %d"), owner_id, target_id);

			ObjectService_Proxy::CreateMessenger(gGlobal.pObjectService->pPeer, context, 
				owner_id, FinalTargetID, relationList, relGroup, friendly);
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_CreateMessenger, NetObject *, messenger)

		CMessengerData *pMessengerData = CastNetObject<CMessengerData>(messenger);
		TRACE_ENSURE(pMessengerData != NULL);
		gGlobal.InsertCharWatch(pMessengerData->GetPlayer_char_id(), 
			pMessengerData->GetFriend_char_id());

		if (pMessengerData->GetFriend_char_id() != target_id)
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("owner_id %dtarget_id %d DB return Player_char_id %d Friend_char_id %d not equal"), 
				owner_id, target_id, pMessengerData->GetPlayer_char_id(), pMessengerData->GetFriend_char_id());
			CB_Error(pPeer, context, 0, _T("MSG_CHAT_MSG_ERROR_NOT_ONLINE"));
			DeleteNetObject(pMessengerData);
			PROCEDURE_RETURN
		}
		CharMsgInfo* pSrcMsgInfo = gGlobal.GetMsgInfo(owner_id);
		if (pSrcMsgInfo)
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_CreateMessenger: owner_id %d, target_id %d"), 
				owner_id, target_id);

			CMessenger *pMessenger = NULL;
			if (pSrcMsgInfo->messengers == NULL)
				pSrcMsgInfo->messengers = CreateNetGroup();			
			pMessenger = CMessenger::NewInstance();
			pMessenger->SetData(pMessengerData);
			pMessenger->PreSend();
			pSrcMsgInfo->messengers->Add(pMessenger);
			SetOwnObject(pSrcMsgInfo->SID, pMessenger);
			if (pSrcMsgInfo->pProxyPeer && pSrcMsgInfo->pMsgChar)
			{
				NetGroup grp;
				//send the group to client

				if (pMessenger->GetFriend_group() != FRIEND_GROUP_TEMP && 
					pMessenger->GetFriend_group() != FRIEND_GROUP_BLACK)
				{
					CharMsgInfo* pSubMsgInfo = gGlobal.GetMsgInfo(target_id);
					if (pSubMsgInfo)
					{
						NetGroup grp2Sub;
						pSrcMsgInfo->pMsgChar->PreSend();
						grp2Sub.Add(pSrcMsgInfo->pMsgChar);
						CB_UpdateMsgCharInfo(pSubMsgInfo->pProxyPeer, RPCContext(pSubMsgInfo->SID), &grp2Sub);

						pSubMsgInfo->pMsgChar->PreSend();
						grp.Add(pSubMsgInfo->pMsgChar);	
						CB_MessageReturn(pSubMsgInfo->pProxyPeer, RPCContext(pSubMsgInfo->SID),
							_T("MSG_FRIEND_IS_ADDED_SUCCESS"), 
							pSrcMsgInfo->pMsgChar->GetNickName());
						CB_MessageReturn(pSrcMsgInfo->pProxyPeer, RPCContext(pSrcMsgInfo->SID),
							_T("MSG_FRIEND_ADD_SUCCESS"), pSubMsgInfo->pMsgChar->GetNickName());
					}
				}
				CB_UpdateMsgCharInfo(pSrcMsgInfo->pProxyPeer, RPCContext(pSrcMsgInfo->SID), &grp);				
				CB_UpdateMessengers(pSrcMsgInfo->pProxyPeer, RPCContext(pSrcMsgInfo->SID), pSrcMsgInfo->messengers);
			}
		}

		DeleteNetObject(pMessengerData);

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_4(GLOBAL_LOGGER, _F("CreateMessenger Error: ObjectService:CB_Error, errcode %d, %s, owner_id %d, target_id %d"),
			err, errMsg.c_str(), owner_id, target_id);
			CB_Error(pPeer, context, err, errMsg);
			// remove msg char reference when messenger creation failed
			gGlobal.ReleaseMsgCharRef(target_id);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::UpdateCharInfo(LPCPEER pPeer, RPCContext &context, NetGroup* grp)
{
	TRACE_ENSURE(grp);

	CMsgCharacterData *pData;
	CMessageCharacter *pMsgChar;
	RPCContext context2;

	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		pData = grp->GetChild<CMsgCharacterData>(i);
		if (pData) {
			CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(pData->GetChar_id());
			if (pMsgInfo)
			{
				pMsgChar = pMsgInfo->pMsgChar;
				context2.sessionId = pMsgInfo->SID;
				PROCEDURE_DO_METHOD1(CMessageCharacter, UpdateInfo, pMsgChar, 
					context2, NetObject*, pData)
			}
		}
	}
	DeleteNetGroup(grp, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::RequestInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &name, const bool &update, const bool &tip)
{
	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("RequestInfo: name address 0x%08x Msg: %s"), name.c_str(), name.c_str());
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo == NULL && !name.IsEmpty())
		pMsgInfo = gGlobal.GetMsgInfo(name);
	if (pMsgInfo)
	{
		if (pMsgInfo->pMsgChar->IsOffline())
		{
			if (tip)
			{
				if (update)
					CB_Error(pPeer, context, 1, "MSG_CHAT_MSG_ERROR_NOT_ONLINE_UPDATE");
				else
					CB_Error(pPeer, context, 1, "MSG_CHAT_MSG_ERROR_NOT_ONLINE");
			}
			else
				CB_Error(pPeer, context, 1, "");
		}
		else
		{
			pMsgInfo->pMsgChar->PreSend();
			CB_RequestInfo(pPeer, context, pMsgInfo->pMsgChar, update);
		}
	}
	else
	{
		if (tip)
		{
			if (update)
				CB_Error(pPeer, context, 1, "MSG_CHAT_MSG_ERROR_NOT_ONLINE_UPDATE");
			else
				CB_Error(pPeer, context, 1, "MSG_CHAT_MSG_ERROR_NOT_ONLINE");
		}
		else
			CB_Error(pPeer, context, 1, "");
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::BattleEndUpdate(LPCPEER pPeer, RPCContext &context, const UInt32Vector &char_idList)
{
	for (Int i = 0; i < (Int)char_idList.size(); i++)
	{
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_idList[i]);
		if (pMsgInfo && pMsgInfo->messengers)
		{
			for(Int j = 0; j < pMsgInfo->messengers->GetCount(); j++)
			{
				bool update = false;
				CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(j));
				if (pMessenger)
				{
					for (Int k = 0; k < (Int)char_idList.size(); k ++)
					{
						if (k != i && 
							pMessenger->GetFriend_char_id() == char_idList[k])
						{
							if(pMessenger->GetFriend_friendly() < MAX_FRIENDLY)//add by taojianping 2009/02/25
							{
								pMessenger->SetFriendly(pMessenger->GetFriend_friendly() + 1);
								pMessenger->SetSendMask(pMessenger->GetUpdateMask());
								CMessengerData Data;
								pMessenger->WriteData(&Data);
								Data.SetSendMask(Data.GetUpdateMask());

								ObjectService_Proxy::UpdateMessenger(gGlobal.pObjectService->pPeer, context, pMessenger->GetPlayer_char_id(), pMessenger->GetFriend_char_id(), &Data);
								update = true;
							}
						}
					}
				}

				if (update)
					MessageService_Stub::CB_UpdateMessengers(pMsgInfo->pProxyPeer, 
						RPCContext(pMsgInfo->SID), pMsgInfo->messengers);
			}
		}
	}

	CB_BattleEndUpdate(pPeer, context);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::AddSpeakCount(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const Int16 &count)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo)
	{
		if (count < 0 && pMsgInfo->pMsgChar->GetGlobalSpeakCount() < count * -1)
			pMsgInfo->pMsgChar->SetSpeakCount(0);
		else
			pMsgInfo->pMsgChar->SetSpeakCount(pMsgInfo->pMsgChar->GetGlobalSpeakCount() + count);
		pMsgInfo->pMsgChar->UpdateObject();
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ChannelsBlockHandle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, 
												   const UInt8Vector &bkChannels, const UInt8Vector &unBkChannels)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo)
	{
		for (UInt i = 0; i < (UInt) bkChannels.size(); i ++)
		{
			if (bkChannels[i] < MAX_CHANNELTYPE)
				pMsgInfo->pMsgChar->m_speakRights[bkChannels[i]] = false;
		}

		for (UInt i = 0; i < (UInt) unBkChannels.size(); i ++)
		{
			if (unBkChannels[i] < MAX_CHANNELTYPE)
				pMsgInfo->pMsgChar->m_speakRights[unBkChannels[i]] = true;
		}	
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::RemoveMessengersByOwner(LPCPEER pPeer, RPCContext &context, 
													   const UInt32 &char_id)
{
	PROCEDURE_START_RPC1(MessageService, RemoveMessengersByOwner, NULL, UInt32, char_id)

		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
		if (pMsgInfo && pMsgInfo->pMsgChar)
		{
			NetGroup dummy;
			PROCEDURE_STORE_VALUE1(UInt32, pMsgInfo->pMsgChar->GetGuild_uid())
			PROCEDURE_GO_STEP1(1, NetGroup*, &dummy);
		}
		else
		{
			UInt32Vector charList;
			charList.push_back(char_id);
			ObjectService_Proxy::GetMessageCharacters(gGlobal.pObjectService->pPeer, 
				context, charList, BOOLEAN_FALSE);
		}

	PROCEDURE_WAIT1(1, ObjectService, CB_GetMsgCharacterGroup, NetGroup *, charGroup)

		TRACE_ENSURE(charGroup);
		if (charGroup->GetCount() > 0)
		{
			CMsgCharacterData* pData = charGroup->GetChild<CMsgCharacterData>(0);
			if (pData)
			{
				PROCEDURE_STORE_VALUE1(UInt32, pData->GetGuild_uid())
			}
			else
			{
				PROCEDURE_STORE_VALUE1(UInt32, 0)
			}
		}

		ObjectService_Proxy::DeleteMessengerList(gGlobal.pObjectService->pPeer, context, 
			char_id);

	PROCEDURE_WAIT(2, ObjectService, CB_DeleteMessengerList)

		//find
		gGlobal.RemoveMessagerByOwnerID(char_id);

		//never delete guild
		PROCEDURE_GO_STEP(4)

		PROCEDURE_RESTORE_VALUE1(UInt32, guild_uid);
		if (guild_uid == 0)
		{
			// if not joined guild, just return
			PROCEDURE_GO_STEP(4)
		}

		//then remove related guild
		UInt32Vector char_ids;
		char_ids.push_back(char_id);
		ObjectService_Proxy::DeleteGuildMemberList(gGlobal.pObjectService->pPeer, context, 
			char_ids);

	PROCEDURE_WAIT(3, ObjectService, CB_DeleteGuildMemberList)

		PROCEDURE_RESTORE_VALUE1(UInt32, guild_uid);
	
		//never delete guild
		PROCEDURE_GO_STEP(4)
		CGuild* pGuild = gGlobal.GetGuild(guild_uid);
		if (pGuild)
		{
			pGuild->RemoveGuildMember(char_id, _T(""), char_id);
			if (pGuild->GetMemberSize() > 0)
			{
				// guild still have member, don't delete it
				PROCEDURE_GO_STEP(4)
			}
			gGlobal.RemoveGuild(pGuild);
		}

		// delete guild in object service
		UInt32Vector guild_uids;
		guild_uids.push_back(guild_uid);
		ObjectService_Proxy::DeleteGuildList(gGlobal.pObjectService->pPeer, context, 
			guild_uids);

	PROCEDURE_WAIT(4, ObjectService, CB_DeleteGuildList)
		
		CB_RemoveMessengersByOwner(pPeer, context);

	PROCEDURE_CATCH
		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("RemoveMessengersByOwner Error: ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());
	CB_Error(pPeer, context, err, errMsg);

	PROCEDURE_END_CATCH
		return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::AddMsgSchedule(LPCPEER pPeer, RPCContext &context, const UInt16 &id, const UInt32 &startTime, const UInt16 &periodCount, 
											  const UInt16 &count, const UInt32 &period, const UInt8 &msgType, const String &msg, const UInt32 &char_id)
{
	if (period == 0)
		return RPC_RESULT_OK;
	MsgSchedule msgSchedule;
	msgSchedule.msg = msg;
	msgSchedule.startTime = startTime * 60000;
	msgSchedule.periodCount = periodCount;
	msgSchedule.count = count;
	msgSchedule.period = period * 60000;
	msgSchedule.msgType = msgType;
	msgSchedule.srcID = char_id;
	msgSchedule.sendTimeCount = -1;

	MsgScheduleMap::iterator Itr = gGlobal.g_MsgScheduleMap.find(id);
	if (Itr != gGlobal.g_MsgScheduleMap.end())
	{
		gGlobal.g_MsgScheduleMap.erase(Itr);
	}

	gGlobal.g_MsgScheduleMap.insert(std::make_pair(id, msgSchedule));
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::RemoveMsgSchedule(LPCPEER pPeer, RPCContext &context, const UInt16 &id)
{
	MsgScheduleMap::iterator Itr = gGlobal.g_MsgScheduleMap.find(id);
	if (Itr != gGlobal.g_MsgScheduleMap.end())
	{
		gGlobal.g_MsgScheduleMap.erase(Itr);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::RemoveAllGuildApplicant(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	gGlobal.RemoveGuildApplicant(char_id);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::CreateGuild(LPCPEER pPeer, RPCContext &context, 
										   const UInt32 &char_id, const String &guild_name, 
										   const String &guild_creator, const String &guild_master)
{
	PROCEDURE_START_RPC4(MessageService, CreateGuild, NULL, UInt32, char_id, 
						String, guild_name, String, guild_creator, String, guild_master)

		//if offline, return
		bool success = true;
		String errMsg;
		CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);

		do {
			TRACE_INFODTL(GLOBAL_LOGGER, _F("do-while start..."));

			if (pMsgInfo == NULL || pMsgInfo->pMsgChar == NULL)
			{
				success = false;
				break;
			}

			TRACE_INFODTL(GLOBAL_LOGGER, _F("check if guild exists..."));
			//check whether has guild
			if (pMsgInfo->pMsgChar->HasGuild() || gGlobal.IsLockCharGuild(char_id))
			{
				success = false;
				errMsg = _T("MSG_ALREADY_HAVE_GUILD");
				break;
			}

			TRACE_INFODTL(GLOBAL_LOGGER, _F("check duplicate name..."));
			//cheat duplicate name
			for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); 
				success && Itr != gGlobal.m_GuildMap.end(); ++Itr)
			{
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("guildId %d..."), Itr->first);
				if (Itr->second->GetName() == guild_name)
				{
					TRACE_INFODTL(GLOBAL_LOGGER, _F("found"));
					success = false;
					errMsg = _T("MSG_CREATE_GUILD_FAIL_NAME_DUP");
					break;
				}
			}

			TRACE_INFODTL(GLOBAL_LOGGER, _F("Check name block and size..."));
			String name = guild_name;
			if (gGlobal.IsBlockName(name) || guild_name.empty() || 
				guild_name.size() > MAX_GUILD_NAME_SIZE)
			{
				success = false;
				errMsg = _T("MSG_CREATE_GUILD_FAIL_NAME_INVALID");
				break;
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateGuild: char_id %d is checked"), char_id);	

		} while (false);

		TRACE_INFODTL(GLOBAL_LOGGER, _F("check if success..."));
		if (!success) {
			TRACE_INFODTL(GLOBAL_LOGGER, _F("call CB_CreateGuild..."));
			CB_CreateGuild(pPeer, context, 1);
			//find client
			if (!errMsg.IsEmpty() && pMsgInfo)
				CB_Error(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), 0, errMsg);
			PROCEDURE_RETURN
		}
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateGuild: char_id %d"), char_id);

		gGlobal.LockCharGuild(char_id);
		ObjectService_Proxy::CreateGuild(gGlobal.pObjectService->pPeer, context, char_id, guild_name,
			GUILD_POSITION_MASTER, guild_creator, guild_master);

	PROCEDURE_WAIT2(1, ObjectService, CB_CreateGuild, NetObject*, guildData, NetObject*, guildMember)
		CGuildData *pGuildData = CastNetObject<CGuildData>(guildData);
		CGuildMemberData *pGuildMemberData = CastNetObject<CGuildMemberData>(guildMember);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateGuild: return from objectservice, char_id %d"), char_id);	

		if (pGuildData && pGuildMemberData)
		{
			CGuild * pGuild = gGlobal.GetGuild(pGuildData->GetGuild_uid());
			if (pGuild == NULL)
			{
				pGuild = CGuild::NewInstance();
				pGuild->SetData(pGuildData);
				gGlobal.m_GuildMap.insert(std::make_pair(pGuild->GetGuild_uid(), pGuild));
			}
			DeleteNetObject(pGuildData);

			CGuildMember* pMember = CGuildMember::NewInstance();
			pMember->SetData(pGuildMemberData);
			pGuild->InsertGuildMember(pMember);
			gGlobal.UnLockCharGuild(char_id);

			//find client
			CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
			if (pMsgInfo && pMsgInfo->pMsgChar)
			{
				pMsgInfo->pMsgChar->SetGuild(pGuild->GetGuild_uid(), pGuild->GetName());
				pMsgInfo->pMsgChar->UpdateObject();
				pMember->SetData(pMsgInfo->pMsgChar);
				pMember->SetOfflineTime(0);
				SetOwnObject(pMsgInfo->SID, pMember);
				pGuild->SendGuild(pMsgInfo, pMember);
				CB_Error(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), 0,
					"MSG_CREATE_GUILD_FINISH");
			}

			for (ProxySidMap::iterator it = gGlobal.m_proxySidMap.begin(); 
				it != gGlobal.m_proxySidMap.end(); ++it)
				CB_CreateGuildMsg(it->first, RPCContext(), pGuild->GetName(), 
					pMember->GetName(), it->second);

			pGuild->SyncToMap();
			CB_CreateGuild(pPeer, context, 0);
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateGuild: char_id %d has created his guild"), char_id);	
		}
		else
		{
			CB_CreateGuild(pPeer, context, 1);
		}

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CreateGuild: char_id %d successfully finished the guild creation"), char_id);

		gGlobal.UnLockCharGuild(char_id);
		DeleteNetObject(guildData);
		DeleteNetObject(guildMember);

	PROCEDURE_CATCH

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_FE("ObjectService:CB_Error, errcode %d, %s"), err, errMsg.c_str());
				CB_Error(pPeer, context, err, errMsg);

		PROCEDURE_EXCEPTION2(MapService, CB_Error, UInt32, err, String, errMsg)

			TRACE_ERRORDTL_2(GLOBAL_LOGGER, 
				_FE("MapService:CB_Error, errcode %d, %s"), err, errMsg.c_str());
				CB_Error(pPeer, context, err, errMsg);

		PROCEDURE_EXCEPTION_DEFAULT
			TRACE_ERRORDTL(GLOBAL_LOGGER, _FE("Add Exception"));
			gGlobal.UnLockCharGuild(char_id);
			CB_CreateGuild(pPeer, context, 1);

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::DonateMoney(LPCPEER pPeer, RPCContext &context, 
										   const UInt32 &guild_uid, const UInt32 &char_id, 
										   const UInt32 &money)
{
	UInt32 donate = 0;
	CGuild* pGuild = gGlobal.GetGuild(guild_uid);

	UInt32 modifiedMoney = 0;
	UInt8 success = GUILD_DONATE_RESULT_SUCC;
	if (pGuild)
	{
		//check donate day
		time_t nowTime;
		time(&nowTime);
		struct tm *newtime = localtime(&nowTime);
//		if (newtime->tm_yday != pGuild->m_donateDay)
		{
			pGuild->m_donateDay = newtime->tm_yday;
			//reset donated
			for (GuildMemberMap::iterator Itr = pGuild->m_memberMap.begin(); 
				Itr != pGuild->m_memberMap.end(); ++Itr)
			{
				Itr->second->m_donated = false;
			}
		}

		CGuildMember* pMember = pGuild->GetMember(char_id);
		if (pMember && !pMember->m_donated)
		{
			UInt32 maxFund = MAX_GUILD_MONEY;
			if (pGuild->GetManorScale() > 0)
			{
				const ManorData *manorData = gGlobal.m_resource->GetManorData(pGuild->GetManorScale());
				if (manorData)
					maxFund = manorData->manor_maxFund;
			}
			if (pGuild->GetFund() >= maxFund * 0.8)
			{
				success = GUILD_DONATE_RESULT_FAIL_MONEY_ENOUGH;
			}
			else
			{				
				modifiedMoney = money;
				if (pGuild->GetFund() + money > maxFund)
					modifiedMoney = (maxFund >= pGuild->GetFund())? maxFund - pGuild->GetFund() : 0;


				donate = modifiedMoney / 10000;

				//check donate msg
				if (modifiedMoney >= 100000)
				{
					String msg;
					msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_MUCH_DONATE_MSG")), 
						pMember->GetName().c_str(), modifiedMoney);
					pGuild->SendGuildMessage(msg);				
				}

				pGuild->SetFund(pGuild->GetFund() + modifiedMoney);
				
				LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), pGuild->GetGuild_uid(),  
				                        pGuild->GetFund(), pGuild->GetProsper(), pGuild->GetManorScale(),
										 LogService_Proxy::Guild_Fund::SOURCE_DONATE, 0);
				
				if (modifiedMoney > 0)
					pGuild->SyncUpdate();	
				if (donate > 0)
				{				
					pMember->ChangeDonate(donate, LogService_Proxy::Guild_Donate::SOURCE_MEMBER_DONATE);
				}
				pMember->m_donated = true;
				success = GUILD_DONATE_RESULT_SUCC;
			}	
		}
		else
			success = GUILD_DONATE_RESULT_FAIL_ONCE_ADAY;

	}

	CB_DonateMoney(pPeer, context, donate, success, modifiedMoney);	

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::RequestGuildAim(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid)
{
	GuildMap::iterator Itr = gGlobal.m_GuildMap.find(guild_uid); 
	if (Itr != gGlobal.m_GuildMap.end())
	{
		Itr->second->SendAttr(pPeer, context, CGuild::aimMask);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::OpenGuildApply(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	//find client
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo)
	{
		for (GuildMap::iterator Itr = gGlobal.m_GuildMap.begin(); 
		Itr != gGlobal.m_GuildMap.end(); ++Itr)
		{
			pMsgInfo->pMsgChar->m_requestGuildList.push_back(Itr->second->GetGuild_uid());			
		}
		pMsgInfo->pMsgChar->m_firstRequest = pMsgInfo->pMsgChar->m_requestGuildList.size() > 0;
		pMsgInfo->pMsgChar->SyncGuild();
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, 
											 const UInt32 &char_id, const UInt16 &iconID, 
											 const Boolean &change)
{
	//find client
	bool canChange = BOOLEAN_FALSE;
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo)
	{
		if (pMsgInfo->pMsgChar->GetFriend_icon() != iconID)
		{
			canChange = true;
			if (change)
			{
				pMsgInfo->pMsgChar->SetFriend_icon(iconID);
				pMsgInfo->pMsgChar->SendAttr(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID),
					CMessageCharacter::friend_iconMask);
				pMsgInfo->pMsgChar->UpdateObject();
			}
		}
	}

	CB_ChangeMsgIcon(pPeer, context, canChange);

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ChangeGDonate(LPCPEER pPeer, RPCContext &context, 
											 const UInt32 &guid_uid, const UInt32 &char_id, 
											 const Int &change, const UInt8 &source)
{
	CGuildMember* pMember = gGlobal.GetGuildMember(guid_uid, char_id);
	if (pMember)
		pMember->ChangeDonate(change, source);		

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GetMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &relation)
{
	UInt32Vector messagers;
	UInt32Vector char_idList;
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for(Int i = 0; i < pMsgInfo->messengers->GetCount(); i++)
		{
			CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(i));
			if (pMessenger && pMessenger->IsRelation(relation))
			{
				CharMsgInfo* pMsg = gGlobal.GetMsgInfo(pMessenger->GetFriend_char_id());
				if (pMsg && pMsg->pMsgChar)
				{
					messagers.push_back(pMessenger->GetFriend_char_id());
				}
				else
				{
					char_idList.push_back(pMessenger->GetFriend_char_id());
				}
			}
		}
	}
	UInt32Vector::iterator iter = char_idList.begin();
	UInt32Vector::iterator iterend = char_idList.end();
	for(; iter != iterend; ++iter)
	{
		messagers.push_back(*iter);
	}


	CB_GetMessengers(pPeer,context,owner_id,messagers);
	TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CB_GetMessengers:%d"),messagers.size());
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GetMessengersName(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &relation)
{
	PROCEDURE_START_RPC2(MessageService, GetMessengersName, NULL, UInt32, owner_id,UInt16,relation)
	StringVector messagers;
	UInt32Vector char_idList;
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for(Int i = 0; i < pMsgInfo->messengers->GetCount(); i++)
		{
			CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(i));
			if (pMessenger && pMessenger->IsRelation(relation))
			{
				CharMsgInfo* pMsg = gGlobal.GetMsgInfo(pMessenger->GetFriend_char_id());
				if (pMsg && pMsg->pMsgChar)
				{
					messagers.push_back(pMsg->pMsgChar->GetNickName());
				}
				else
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CB_GetMessengersName:%d"),messagers.size());
					char_idList.push_back(pMessenger->GetFriend_char_id());
				}
			}
		}
	}

	if(char_idList.empty())
	{
		CB_GetMessengersName(pPeer,context,owner_id,messagers);
		PROCEDURE_RETURN;
	}
	PROCEDURE_STORE_VALUE1(StringVector, messagers)

	ObjectService_Proxy::GetMessageCharacters(gGlobal.pObjectService->pPeer, context, char_idList, BOOLEAN_TRUE);
	
	PROCEDURE_WAIT1(1, ObjectService, CB_GetMsgCharacterGroup, NetGroup*, charGroup)
	TRACE_ENSURE(charGroup);
	PROCEDURE_RESTORE_VALUE1(StringVector, messagers)
	for (int i = 0; i < charGroup->GetCount(); ++i)
	{
		CMsgCharacterData *pCharData = charGroup->GetChild<CMsgCharacterData>(i);
		if(pCharData != NULL)
			messagers.push_back(pCharData->GetNickName());
	}
	DeleteNetGroup(charGroup,BOOLEAN_TRUE);

	CB_GetMessengersName(pPeer,context,owner_id,messagers);
	TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CB_GetMessengersName:%d"),messagers.size());

	PROCEDURE_CATCH 

		PROCEDURE_EXCEPTION2(ObjectService, CB_Error, UInt32, err, String, errMsg)
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FE("ObjectService:CB_Error, errcode %d, %s"),
		err, errMsg.c_str());

	PROCEDURE_END_CATCH

	return RPC_RESULT_OK;
}


RPCResult MessageService_Stub::RemoveRelation(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const UInt16 &relation)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RemoveRelation: owner_id %d, target_id %d,relation %d"), owner_id, target_id,relation);

	bool bMidfied = false;
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for (UInt i = 0; i < pMsgInfo->messengers->GetCount(); ++i )
		{
			CMessenger * pMessenger = pMsgInfo->messengers->GetChild<CMessenger>(i);
			if (pMessenger && pMessenger->GetFriend_char_id() == target_id)
			{
				UINT16 rel = pMessenger->GetFriend_relation();
				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RemoveRelation: owner_id %d, target_id %d,pre modfied relation %d"), owner_id, target_id,rel);
				rel = rel & ~( 1<< relation );
				pMessenger->SetFriend_relation(rel);
				pMessenger->UpdateToObject();

				TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RemoveRelation: owner_id %d, target_id %d,modfied relation %d"), owner_id, target_id,rel);

				MessageService_Stub::CB_UpdateMessengers(pMsgInfo->pProxyPeer, 
					RPCContext(pMsgInfo->SID), pMsgInfo->messengers);
				bMidfied = true;
				break;
				//return RPC_RESULT_OK;
			}
		}
	}

	if(!bMidfied)
	{

		ObjectService_Proxy::RemoveRelation(gGlobal.pObjectService->pPeer, context,owner_id,target_id,relation);
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("RemoveRelation charater offline: owner_id %d, target_id %d,relation %d"), owner_id, target_id,relation);
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GetFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	UInt32 friendly = 0;
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for (UInt i = 0; i < pMsgInfo->messengers->GetCount(); ++i )
		{
			CMessenger * pMessenger = pMsgInfo->messengers->GetChild<CMessenger>(i);
			if (pMessenger && pMessenger->GetFriend_char_id() == target_id)
			{
				friendly = pMessenger->GetFriend_friendly();
				break;
			}
		}
	}

	//pMsgInfo = gGlobal.GetMsgInfo(target_id);
	//if (pMsgInfo && pMsgInfo->messengers)
	//{
	//	for (UInt i = 0; i < pMsgInfo->messengers->GetCount(); ++i )
	//	{
	//		CMessenger * pMessenger = pMsgInfo->messengers->GetChild<CMessenger>(i);
	//		if (pMessenger && pMessenger->GetFriend_char_id() == owner_id)
	//		{
	//			friendly = friendly > pMessenger->GetFriend_friendly()?pMessenger->GetFriend_friendly():friendly;
	//			break;
	//		}
	//	}
	//}

	CB_GetFriendly(pPeer,context,friendly);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ChangeFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id, const Int32 &changed)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F(" owner_id = %d, target_id = %d,changed = %d"), owner_id, target_id, changed);
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for(Int i = 0; i < pMsgInfo->messengers->GetCount(); i++)
		{
			bool update = false;
			CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(i));
			if (pMessenger 
				&& pMessenger->GetFriend_char_id() == target_id 
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_TEMP 
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_BLACK)
			{
				if(pMessenger->GetFriend_friendly() + changed <= MAX_FRIENDLY)
				{
					pMessenger->SetFriendly(pMessenger->GetFriend_friendly() + changed);
					pMessenger->SetSendMask(pMessenger->GetUpdateMask());
					CMessengerData Data;
					pMessenger->WriteData(&Data);
					Data.SetSendMask(Data.GetUpdateMask());

					ObjectService_Proxy::UpdateMessenger(gGlobal.pObjectService->pPeer, context, pMessenger->GetPlayer_char_id(), pMessenger->GetFriend_char_id(), &Data);
					update = true;
				}
			}
			if (update)
			{
				MessageService_Stub::CB_UpdateMessengers(pMsgInfo->pProxyPeer, 
				RPCContext(pMsgInfo->SID), pMsgInfo->messengers);
				break;
			}
		}
	}

	pMsgInfo = gGlobal.GetMsgInfo(target_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for(Int i = 0; i < pMsgInfo->messengers->GetCount(); i++)
		{
			bool update = false;
			CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(i));
			if (pMessenger 
				&& pMessenger->GetFriend_char_id() == owner_id
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_TEMP 
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_BLACK)
			{
				if(pMessenger->GetFriend_friendly() + changed <= MAX_FRIENDLY)
				{
					pMessenger->SetFriendly(pMessenger->GetFriend_friendly() + changed);
					pMessenger->SetSendMask(pMessenger->GetUpdateMask());
					CMessengerData Data;
					pMessenger->WriteData(&Data);
					Data.SetSendMask(Data.GetUpdateMask());

					ObjectService_Proxy::UpdateMessenger(gGlobal.pObjectService->pPeer, context, pMessenger->GetPlayer_char_id(), pMessenger->GetFriend_char_id(), &Data);
					update = true;
				}
			}
			if (update)
			{
				MessageService_Stub::CB_UpdateMessengers(pMsgInfo->pProxyPeer, 
				RPCContext(pMsgInfo->SID), pMsgInfo->messengers);
				break;
			}
		}
	}

	CB_ChangeFriendly(pPeer,context);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::IsFriend(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	Boolean isFriendOwner = false;
	Boolean isFriendTarget = false;

	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for(Int i = 0; i < pMsgInfo->messengers->GetCount(); i++)
		{
			CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(i));
			if (pMessenger 
				&& pMessenger->GetFriend_char_id() == target_id 
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_TEMP 
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_BLACK)
			{
				isFriendOwner = true;
				break;
			}
		}
	}

	pMsgInfo = gGlobal.GetMsgInfo(target_id);
	if (pMsgInfo && pMsgInfo->messengers)
	{
		for(Int i = 0; i < pMsgInfo->messengers->GetCount(); i++)
		{
			CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(i));
			if (pMessenger 
				&& pMessenger->GetFriend_char_id() == owner_id
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_TEMP 
				&& pMessenger->GetFriend_group() != FRIEND_GROUP_BLACK)
			{
				isFriendTarget = true;
				break;
			}
		}
	}

	CB_IsFriend(pPeer,context,(isFriendOwner&&isFriendTarget)?1:0);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GuildWarResult(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid1, const UInt32 &guild_uid2, const UInt32 &win_guild_uid)
{
	GuildMap::iterator ItrGuild1 = gGlobal.m_GuildMap.find(guild_uid1);
	GuildMap::iterator ItrGuild2 = gGlobal.m_GuildMap.find(guild_uid2);
	if (ItrGuild1 != gGlobal.m_GuildMap.end() &&
		ItrGuild2 != gGlobal.m_GuildMap.end())
	{
		String msg;
		if (win_guild_uid == 0)
		{
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_WAR_DRAW")), ItrGuild1->second->GetName().c_str(), ItrGuild2->second->GetName().c_str());
			ItrGuild1->second->SetGuildPvPScore(ItrGuild1->second->GetGuildPvPScore() + 3);
			ItrGuild2->second->SetGuildPvPScore(ItrGuild2->second->GetGuildPvPScore() + 3);
		}
		if (guild_uid1 == win_guild_uid)
		{
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_WAR_WIN")), ItrGuild1->second->GetName().c_str(), ItrGuild2->second->GetName().c_str());
			ItrGuild1->second->SetGuildPvPScore(ItrGuild1->second->GetGuildPvPScore() + 5);
			ItrGuild1->second->SetFund(ItrGuild1->second->GetFund() + 300000);
			ItrGuild2->second->SetGuildPvPScore(ItrGuild2->second->GetGuildPvPScore() + 1);
			LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), ItrGuild1->second->GetGuild_uid(),  ItrGuild1->second->GetFund(), 
				ItrGuild1->second->GetProsper(), ItrGuild1->second->GetManorScale(),
				 LogService_Proxy::Guild_Fund::SOURCE_GUILDWAR, 0);
		}
		else if (guild_uid2 == win_guild_uid)
		{
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_WAR_WIN")), ItrGuild2->second->GetName().c_str(), ItrGuild1->second->GetName().c_str());
			ItrGuild1->second->SetGuildPvPScore(ItrGuild1->second->GetGuildPvPScore() + 1);
			ItrGuild2->second->SetGuildPvPScore(ItrGuild2->second->GetGuildPvPScore() + 5);
			ItrGuild2->second->SetFund(ItrGuild2->second->GetFund() + 300000);
			LogService_Proxy::LogManor(gGlobal.pLogService->pPeer, RPCContext(), ItrGuild2->second->GetGuild_uid(),  ItrGuild2->second->GetFund(), 
				ItrGuild2->second->GetProsper(), ItrGuild2->second->GetManorScale(),
					 LogService_Proxy::Guild_Fund::SOURCE_GUILDWAR, 0);
		}
		ItrGuild1->second->SyncUpdate();
		ItrGuild2->second->SyncUpdate();
		SendRumorMessage(pPeer, context, msg, 0, 0);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::KickMember(LPCPEER pPeer, RPCContext &context, 
										  const UInt32 &guild_uid, const UInt32 &kicker_id,
										  const UInt32 &member_id, const String &reason)
{
	CGuild* pGuild = gGlobal.GetGuild(guild_uid);
	if (pGuild)
	{
		CGuildMember* pKicker = pGuild->GetMember(kicker_id);
		CGuildMember* pMember = pGuild->GetMember(member_id);
		if (pKicker && pMember && pMember->GetPost() != GUILD_POSITION_MASTER && 
			(pKicker->GetPost() <= GUILD_POSITION_SECMASTER || kicker_id == member_id))
		{
			if (pMember->GetPost() == GUILD_POSITION_SECMASTER)
			{
				pGuild->SetSecMaster(String());
				pGuild->SyncUpdate();
			}
			PROCEDURE_DO_METHOD3(CGuild, KickMember, pGuild, context, UInt32, member_id, 
				String, reason, UInt32, kicker_id)
		}
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ChangeTitle(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const String &title)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo && pMsgInfo->pMsgChar)
	{
		pMsgInfo->pMsgChar->SetTitle(title);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::SetMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &money)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->pMsgChar)
	{
		pMsgInfo->pMsgChar->SetMoney(money);
		pMsgInfo->pMsgChar->UnmarkUpdate(CMessageCharacter::moneyMask);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GetTargetFromHawk(LPCPEER pPeer, RPCContext &context, const UInt32& target_id)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(target_id);
	if(pMsgInfo && pMsgInfo->pMsgChar)
	{
		if (pMsgInfo && pMsgInfo->pProxyPeer)
			CB_GetTargetFromHawk(pPeer,context,true,pMsgInfo->pMsgChar->GetNickName());
		else
			CB_GetTargetFromHawk(pPeer,context,false,pMsgInfo->pMsgChar->GetNickName());
	}
	else
	{
		if (pMsgInfo && pMsgInfo->pProxyPeer)
			CB_GetTargetFromHawk(pPeer,context,true,_T(""));
		else
			CB_GetTargetFromHawk(pPeer,context,false,_T(""));
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count, const UInt32 &money, const UInt32 &guild_uid, const UInt32 &char_id)
{
	CGuild* pGuild = gGlobal.GetGuild(guild_uid);
	if (pGuild && pGuild->GetAutoManageBusiness())
	{
		CGuildMember* pMember = pGuild->GetMember(char_id);
		if (pMember)
		{
			pMember->RaiseNoTradeEvent();
		}
	}
	CBusinessShopControl::GetInstance().BuyItem(pPeer, context, itemId, shopId, count, money);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const UInt32 &itemId, const Int16 &shopId, const UInt16 &count, const UInt32 &guild_uid, const UInt32 &char_id)
{
	CGuild* pGuild = gGlobal.GetGuild(guild_uid);
	if (pGuild && pGuild->GetAutoManageBusiness())
	{
		CGuildMember* pMember = pGuild->GetMember(char_id);
		if (pMember)
		{
			pMember->RaiseNoTradeEvent();
		}
	}
	CBusinessShopControl::GetInstance().SellItem(pPeer, context, itemId, shopId, count);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt16 &shop_id, const UInt32 &money)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(owner_id);
	if (pMsgInfo && pMsgInfo->pProxyPeer)
	{
		CBusinessShopControl::GetInstance().OpenBusinessShopMenu(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), shop_id, money);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const UInt16 &shop_id)
{
	CBusinessShopControl::GetInstance().UpdateBusinessShopList(pPeer, context, shop_id);
	return RPC_RESULT_OK;
}


RPCResult MessageService_Stub::ChangeBusinessProfit(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, const UInt32 &char_id, const Int32 &profit)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ChangeBusinessProfit: guild_uid %d, char_id %d,profit %d"), guild_uid, char_id, profit);
	CGuild* pGuild = gGlobal.GetGuild(guild_uid);
	if (pGuild)
	{
		CGuildMember* pMember = pGuild->GetMember(char_id);
		if (pMember)
		{
			if (profit < 0)
			{
				pMember->CancelNoMoneyEvent();
				return RPC_RESULT_OK;
			}
			if (pGuild->GetAutoManageBusiness())
			{
				pMember->RaiseNoMoneyEvent();
			}
			if (profit != 0)
			{
				Int32 businessProfit = pMember->GetBusinessProfit() + profit;
				if (businessProfit < 0)
				{
					businessProfit = 0;
				}
				pMember->SetBusiness(pMember->GetBusinessCount() + 1, (UInt32)businessProfit);
				pGuild->SyncUpdate();
			}
		}
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ManorScaleChange(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid, 
												const UInt8 &scale)
{
	GuildMap::iterator Itr = gGlobal.m_GuildMap.find(guild_uid);
	if ((Itr != gGlobal.m_GuildMap.end()) && Itr->second != NULL) 
	{
		if (Itr->second->GetManorScale() != scale && scale >= 0 && scale <= 5)
		{
			Itr->second->SetLastMaintainDate((UInt32)time(NULL));
            Itr->second->UpdateManorScale(scale);
			Itr->second->SetFailMantainPayNum(MIN(Itr->second->GetOweMaintainPayNum(), MAX_FAIL_MANOR_MAINTAIN));
			Itr->second->CancelUniqEvent(CGuild::EVT_MANOR_MAINTENANCE);
			if (scale >0 && scale <= 5)
                Itr->second->RaiseUniqEventID(CGuild::EVT_MANOR_MAINTENANCE, 1000*gConfigure.maintainTime);	
			Itr->second->SyncUpdate();
		}
	}

	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ApplyTrader(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid , const UInt32 &char_id)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("ApplyTrader: session %s, guild_uid %d, char_id %d"), 
		context.sessionId.GetString().c_str(), guild_uid, char_id);
	CGuild* pGuild = gGlobal.GetGuild(guild_uid);
	if (pGuild && pGuild->GetAutoManageBusiness())
	{
		CGuildMember* pMember = pGuild->GetMember(char_id);
		if (pMember && pMember->GetPost() != GUILD_POSITION_TRADER && pMember->GetPost() != GUILD_POSITION_MASTER)
		{
			if (pGuild->CheckPostAvialble(GUILD_POSITION_TRADER))
			{
				pMember->SetPost(GUILD_POSITION_TRADER);
				pMember->RaiseNoTradeEvent();
				pGuild->SyncUpdate();
			}
			else
			{
				CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
				if (pMsgInfo && pMsgInfo->pProxyPeer)
				{
					CB_Error(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), 0, _T("MSG_APPLY_TRADER_POST_FULL"));
				}
			}
		}
		else
			TRACE_INFODTL(GLOBAL_LOGGER, _F("ApplyTrader: can't find member"));
	}
	else
		TRACE_INFODTL(GLOBAL_LOGGER, _F("ApplyTrader: can't find guild or this guild not open AutoManageBusiness"));
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::TransmitGuildMsg(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &guildUID)
{
	if (Msg.IsEmpty())
		return RPC_RESULT_OK;
	GuildMap::iterator Itr = gGlobal.m_GuildMap.find(guildUID);
	if ((Itr != gGlobal.m_GuildMap.end()) && Itr->second != NULL) 
	{
		Itr->second->SendGuildMessage(Msg);
	}

	return RPC_RESULT_OK;

}
RPCResult MessageService_Stub::GetGenerationChars(LPCPEER pPeer, RPCContext &context, const UInt32Vector &uiVtRelatedIds)
{
	NetGroup ntOnlineIds;
	for (UInt32 i=0;i<uiVtRelatedIds.size();i++) 
	{
		CMessageCharacter* pCharMsg = gGlobal.GetMsgCharRef(uiVtRelatedIds[i]);
		if (NULL != pCharMsg) 
		{
			if (pCharMsg->IsOnline()) 
			{
				pCharMsg->SetSendMask(CMessageCharacter::AllMask);
				ntOnlineIds.Add(pCharMsg);
			}
		}
	}

	if (ntOnlineIds.GetCount() > 0) 
	{
		MapService_Proxy::GetGenenrationCharsResult(pPeer,context,&ntOnlineIds);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::GetTargetInfo(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo && pMsgInfo->pMsgChar)
	{
		pMsgInfo->pMsgChar->PreSend();
		CB_GetTargetInfo(pPeer, context, pMsgInfo->pMsgChar);
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &item_uid, const UInt32 &char_idTo)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo && pMsgInfo->pMsgChar && pMsgInfo->pProxyPeer)
	{
		UInt8 uLineId(pMsgInfo->pMsgChar->GetLine_id());
		LPCPEER pMapPeer(NULL);
		for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin(); it != gGlobal.m_mapIndices.end(); ++it)
		{
			if (it->first.line_id == uLineId)
			{
				pMapPeer = it->second;
				break;
			}
		}
		if (pMapPeer)
		{
			MapService_Proxy::ShowPublicItemDetailFromOtherLine(pMapPeer, context, char_id, item_uid, char_idTo);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &partner_id, const UInt32 &char_idTo)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo && pMsgInfo->pMsgChar && pMsgInfo->pProxyPeer)
	{
		UInt8 uLineId(pMsgInfo->pMsgChar->GetLine_id());
		LPCPEER pMapPeer(NULL);
		for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin(); it != gGlobal.m_mapIndices.end(); ++it)
		{
			if (it->first.line_id == uLineId)
			{
				pMapPeer = it->second;
				break;
			}
		}
		if (pMapPeer)
		{
			MapService_Proxy::ShowPublicPartnerDetailFromOtherLine(pMapPeer, context, char_id, partner_id, char_idTo);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id, const UInt32 &pet_id, const UInt32 &char_idTo)
{
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(char_id);
	if (pMsgInfo && pMsgInfo->pMsgChar && pMsgInfo->pProxyPeer)
	{
		UInt8 uLineId(pMsgInfo->pMsgChar->GetLine_id());
		LPCPEER pMapPeer(NULL);
		for (MapIndexList::iterator it = gGlobal.m_mapIndices.begin(); it != gGlobal.m_mapIndices.end(); ++it)
		{
			if (it->first.line_id == uLineId)
			{
				pMapPeer = it->second;
				break;
			}
		}
		if (pMapPeer)
		{
			MapService_Proxy::ShowPublicPetDetailFromOtherLine(pMapPeer, context, char_id, pet_id, char_idTo);
		}
	}
	return RPC_RESULT_OK;
}

RPCResult MessageService_Stub::SendFriendMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const UInt32 &Char_id)
{

	if (Msg.IsEmpty())
		return RPC_RESULT_OK;

	UInt sizeofMsg = NoCodeStringSize(String(Msg));
	if (sizeofMsg > MaxMsgLongLen)
	{
		CB_Error(pPeer, context, 0, _T("MSG_MESSAGE_TOO_LONG"));
		return RPC_RESULT_OK;
	}
	BYTE Result = MSG_RETURN_FAIL_NOT_FOUND;
	UInt32 resultFlag = 0;
	UInt32 CurrTime = (UInt32)time(NULL);
	UInt32Vector FriendVec;
	CharMsgInfo* pMsgInfo = gGlobal.GetMsgInfo(Char_id);
	if (CurrTime - pMsgInfo->PreSpeakTime < SpeakRestTime[CHANNELTYPE_FRIEND])
	{
		resultFlag = SpeakRestTime[CHANNELTYPE_FRIEND] + pMsgInfo->PreSpeakTime - CurrTime;
		Result = MSG_RETURN_FAIL_TOO_FREQUENT;
	}
	else
	{
		if (pMsgInfo && pMsgInfo->messengers)
		{
			for(Int i = 0; i < pMsgInfo->messengers->GetCount(); i++)
			{
				CMessenger * pMessenger = CastNetObject<CMessenger>(pMsgInfo->messengers->GetChild(i));
				if (pMessenger 
					&& pMessenger->GetFriend_group() != FRIEND_GROUP_TEMP 
					&& pMessenger->GetFriend_group() != FRIEND_GROUP_BLACK
					&& pMessenger->GetFriend_char_id() != Char_id)
				{
					FriendVec.push_back(pMessenger->GetFriend_char_id());
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SendFriendMessage: push friend ID =  %d"), pMessenger->GetFriend_char_id());
				}
			}
		}

		if(FriendVec.size() > 0)
		{
			UInt32Vector::iterator Itr = FriendVec.begin();
			while(Itr != FriendVec.end())
			{
				CharMsgInfo* pFriendMsgInfo = gGlobal.GetMsgInfo(*Itr);
				Boolean isFriend = false;
				if (pFriendMsgInfo && pFriendMsgInfo->pProxyPeer)
				{
					if (pFriendMsgInfo && pFriendMsgInfo->messengers)
					{
						for(Int i = 0; i < pFriendMsgInfo->messengers->GetCount(); i++)
						{
							CMessenger * pMessenger = CastNetObject<CMessenger>(pFriendMsgInfo->messengers->GetChild(i));
							if (pMessenger 
								&& pMessenger->GetFriend_char_id() == Char_id
								&& pMessenger->GetFriend_group() != FRIEND_GROUP_TEMP 
								&& pMessenger->GetFriend_group() != FRIEND_GROUP_BLACK)
							{
								isFriend = true;
								TRACE_INFODTL_2(GLOBAL_LOGGER, _F("SendFriendMessage: friend ID = %d ,find My ID = %d"),
									pFriendMsgInfo->pMsgChar->GetChar_id(), pMessenger->GetFriend_char_id());
								break;
							}
						}
					}
					if(!isFriend)
					{
						TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SendFriendMessage:friend not find me so remove friend ID = %d"),
							*Itr);
						FriendVec.erase(Itr);
					}
					else
						Itr++;
				}
				else
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("SendFriendMessage:friend not online so remove friend ID = %d"),
						*Itr);
					FriendVec.erase(Itr);
				}
			}
		}
	}
	if (FriendVec.size() > 0)
	{
		Result = MSG_RETURN_SUCCESS;
		String blockedMsg = Msg;
		gGlobal.BlockWord(blockedMsg);
		UInt32Vector tempList;
		String emptyText;
		String srcName = pMsgInfo->pMsgChar->GetNickName();
		for (UInt32 i = 0; i < FriendVec.size(); i++)
		{
			CharMsgInfo* pDstMsgInfo = gGlobal.GetMsgInfo(FriendVec[i]);
			tempList.push_back(FriendVec[i]);
			if (pDstMsgInfo && pDstMsgInfo->pProxyPeer && pDstMsgInfo->Listen)
			CB_ReceiveFriendMessage(pDstMsgInfo->pProxyPeer, 
				RPCContext(pDstMsgInfo->SID), blockedMsg, pMsgInfo->pMsgChar->GetNickName());
		}
		if (pMsgInfo && pMsgInfo->pProxyPeer)
		CB_ReceiveFriendMessage(pMsgInfo->pProxyPeer, 
			RPCContext(pMsgInfo->SID), blockedMsg, pMsgInfo->pMsgChar->GetNickName());
		WriteMsgLog(context, CHANNELTYPE_FRIEND, Char_id, srcName, tempList, emptyText, blockedMsg);
	}
	else
		Result = MSG_RETURN_FAIL_NO_FRIEND_ONLINE;

	CB_SendPrivateMessage(pMsgInfo->pProxyPeer, RPCContext(pMsgInfo->SID), Result, 
		true, resultFlag); 
	return RPC_RESULT_OK;
}