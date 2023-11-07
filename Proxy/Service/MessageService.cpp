//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Service/MessageService_proxy.h"

#ifdef QGL
#include "QGL/Global.h"
#include "QGL/RumorMsgProcess.h"
#include "QGL/MenuFriendProcess.h"
#include "QGL/MenuTargetProcess.h"
#include "QGL/MenuGuildProcess.h"
#include "QGL/MenuHelpProcess.h"
#include "QGL/UI.h"
#include "QGL/CallLuaFunc.h"
#include "Proxy/Logic/MessageCharacter.h"
#include "Proxy/Logic/Guild.h"
#include "QGL/MenuShopProcess.h"
#include "Proxy/Logic/Item.h"
#include "Proxy/Logic/Partner.h"
#include "Proxy/Logic/Pet.h"
#include "QGL/XMLPanel.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/MenuPartnerProcess.h"
#include "QGL/MenuPetProcess.h"
#include "Resource/ClientResManager.h"
#endif // QGL

// private function and namespace global variable here
namespace MessageService_Proxy
{
};

RPCResult MessageService_Proxy::CB_RequestApplicant(LPCPEER pPeer, RPCContext &context, NetGroup* pGrp)
{
	#ifdef QGL
		gGlobal.g_pMenuGuildProcess->SetGuildApplicantGrp(pGrp);
	#endif
	return RPC_RESULT_OK;
}
RPCResult MessageService_Proxy::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &err, const String &errMsg)
{
	#ifdef QGL
	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(errMsg.c_str()));
	if (err == 1)
	{
		CharTempFriendDataMap::iterator Itr = gGlobal.g_pMenuFriendProcess->m_TempFriendList.find(gGlobal.g_MenuTargetProcess->m_char_id);
		if (Itr != gGlobal.g_pMenuFriendProcess->m_TempFriendList.end())
		{
			Itr->second.state = FRIENC_CHAT_STATUS_OFFLINE;

			if (gGlobal.g_pMenuFriendProcess->m_CurrGroup == FRIEND_GROUP_TEMP)
				gGlobal.g_pMenuFriendProcess->RefreshMenu();
		}
	}
	#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetAvailService(LPCPEER pPeer, RPCContext &context, const ServiceID &serviceId)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveMessageChannel(LPCPEER pPeer, RPCContext &context, NetObject * channel, const BYTE &Type)
{
#ifdef QGL
	gGlobal.NotifyReceiveMessageChannel(pPeer, context, channel, Type);
#else
	DeleteNetObject(channel);
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SendHorseLightMessage(LPCPEER pPeer, RPCContext &context,  const BYTE &Result)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SendSystemMessage(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceivePrivateMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, 
														 const String &SrcName, const UInt32 &SrcChar_id, 
														 const String &TargetName,  const UInt32 &TargetChar_id,
														 const boolean &isFriend)
{
#ifdef QGL
	if (gGlobal.g_pCharacter == NULL)
	{
		gGlobal.g_pMenuFriendProcess->PushNextMessage(Msg, TargetName, SrcName, SrcName, SrcChar_id, 0);
		return RPC_RESULT_OK;
	}
	
	if (isFriend)
	{
		if ((gGlobal.g_pMenuFriendProcess->m_TargetTalkingCharName == TargetName  &&
			!TargetName.IsEmpty())||
			(gGlobal.g_pMenuFriendProcess->m_TargetTalkingCharName == SrcName &&
			!SrcName.IsEmpty()))
		{
			
			gGlobal.g_pMenuFriendProcess->PrintMessage(Msg, SrcName, TargetName);

			if (gGlobal.g_pCharacter->GetCharName() != SrcName)
				gGlobal.g_pMenuFriendProcess->WriteHistory(Msg, gGlobal.g_pCharacter->GetCharName(), SrcName, SrcName);
			else
				gGlobal.g_pMenuFriendProcess->WriteHistory(Msg, gGlobal.g_pCharacter->GetCharName(), TargetName, SrcName);
		}
		else
		{

			String otherName;

			if (gGlobal.g_pCharacter->GetCharName() == SrcName)
				otherName = TargetName;
			else
				otherName = SrcName;

			CMessageCharacter * pMsgChar = gGlobal.g_pMenuFriendProcess->FindInfo(otherName);
			if (pMsgChar != NULL)
			{
				if (gGlobal.g_pCharacter->GetCharName() != SrcName)
				{
					gGlobal.g_pMenuFriendProcess->PushNextMessage(Msg, TargetName, SrcName, pMsgChar->GetNickName(), pMsgChar->GetChar_id(), 0);
					gGlobal.g_pMenuFriendProcess->WriteHistory(Msg, gGlobal.g_pCharacter->GetCharName(), SrcName, SrcName);
				}
				else
					gGlobal.g_pMenuFriendProcess->WriteHistory(Msg, gGlobal.g_pCharacter->GetCharName(), TargetName, SrcName);

			}
			else if (SrcChar_id != gGlobal.g_pCharacter->GetMapActorID())
				gGlobal.g_pMenuFriendProcess->PushNextMessage(Msg, TargetName, SrcName, otherName, SrcChar_id, 0);
			else
				gGlobal.g_pMenuFriendProcess->PrintMessage(Msg, SrcName, TargetName);

		}
	}
	else
		gGlobal.NotifyReceivePrivateMessage(pPeer, context, Msg, SrcName, TargetName);
	//create temp friend if not exist

	gGlobal.g_pMenuFriendProcess->InserTempFriend(SrcChar_id, SrcName, FRIENC_CHAT_STATUS_ONLINE);
	gGlobal.g_pMenuFriendProcess->InserTempFriend(TargetChar_id, TargetName, FRIENC_CHAT_STATUS_ONLINE);
	

#endif // QGL
	
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveHorseLightMessage(LPCPEER pPeer, RPCContext &context, const String &Msg,
															const UInt32 &count, const SessionIDVector &listeners)
{
#ifdef QGL
		gGlobal.NotifyReceiveHorseLightMessage(pPeer, context, Msg, count);
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveRumorMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const BYTE & SubType, const SessionIDVector &listeners)
{
#ifdef QGL
	if (SubType == 0)
		gGlobal.g_pRumorMsgProcess->ShowRumorMessage(Msg);
	gGlobal.PrintMessage(Msg,CHANNELTYPE_RUMOR); 
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SendPrivateMessage(LPCPEER pPeer, RPCContext &context,  const BYTE &Result, const boolean &isFriend, const UInt32 &resultFlag)
{
#ifdef QGL
		gGlobal.NotifyCB_SendPrivateMessage(pPeer, context,  Result, isFriend, resultFlag);
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_InitMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SetListen(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_JoinMessageChannel(LPCPEER pPeer, RPCContext &context, const BYTE &Result)
{
#ifdef QGL
	gGlobal.NotifyCB_JoinMessageChannel(pPeer, context, Result);
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SendRumorMessage(LPCPEER pPeer, RPCContext &context,  const BYTE &Result)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ReceiveSystemMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const SessionIDVector &listeners)
{
#ifdef QGL
	gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM,gGlobal.GetStringTable()->Get(_T("GM_MESSAGE_NAME")));
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult  MessageService_Proxy::CB_UpdateMessengers(LPCPEER pPeer, RPCContext &context, NetGroup* messengers)
{
#ifdef QGL
	if (gGlobal.g_pMenuFriendProcess->m_Messengers &&
		gGlobal.g_pMenuFriendProcess->m_Messengers != messengers)
	{
		DeleteNetGroup(gGlobal.g_pMenuFriendProcess->m_Messengers);
	}
	gGlobal.g_pMenuFriendProcess->m_Messengers = messengers;
	gGlobal.g_pMenuFriendProcess->ConstructData();
	if (messengers->GetCount() > 0)
		gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_MAKEFRIEND);

	if (gGlobal.g_MenuTargetProcess->IsOpenDetail())
		gGlobal.g_MenuTargetProcess->ShowDetail();
#else
	DeleteNetGroup(messengers, BOOLEAN_TRUE);
#endif
	return RPC_RESULT_OK;
}
RPCResult MessageService_Proxy::CB_RemoveMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32 &target_id)
{
	#ifdef QGL
	gGlobal.g_pMenuFriendProcess->RemoveMessenger(owner_id, target_id);
	#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_UpdateMsgCharInfo(LPCPEER pPeer, RPCContext &context, 
													 NetGroup* magCharInfoGrp)
{
#ifdef QGL
	
	bool change = false;
	UInt32 preSize = gGlobal.g_pMenuFriendProcess->m_MessageCharacters.size();
	for (Int i = 0; i < magCharInfoGrp->GetCount(); ++i)
	{
		CMessageCharacter * pMsgChar = magCharInfoGrp->GetChild<CMessageCharacter>(i);
		if (pMsgChar)
		{
			CharMsgCharPtrMap::iterator Itr = gGlobal.g_pMenuFriendProcess->m_MessageCharacters.find(pMsgChar->GetChar_id());
			if (Itr != gGlobal.g_pMenuFriendProcess->m_MessageCharacters.end())
			{
				CMessageCharacter * pMsgCharTmp = FindNetObject<CMessageCharacter>(Itr->second);
				if (pMsgCharTmp != pMsgChar)
				{
					DeleteNetObject(pMsgCharTmp);
				}
				gGlobal.g_pMenuFriendProcess->m_MessageCharacters.erase(Itr);
				change = true;
			}

			gGlobal.g_pMenuFriendProcess->m_MessageCharacters.insert(std::make_pair(pMsgChar->GetChar_id(), pMsgChar->GetNetId()));
			if (gGlobal.g_pCharacter && 
				gGlobal.g_pCharacter->GetMapActorID() == pMsgChar->GetChar_id())
				gGlobal.g_pMenuFriendProcess->m_pOwnChar = pMsgChar;
		}
	}

	if (change || preSize != gGlobal.g_pMenuFriendProcess->m_MessageCharacters.size())
	{
		gGlobal.g_pMenuFriendProcess->ConstructData();
		if (gGlobal.g_MenuTargetProcess->IsOpenDetail())
			gGlobal.g_MenuTargetProcess->ShowDetail();
	}
	gGlobal.g_pMenuFriendProcess->m_bInit = true;
#else
	DeleteNetGroup(magCharInfoGrp, BOOLEAN_TRUE);
#endif

	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_RequestInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgCharData, const bool &update)
{
#ifdef QGL
		CMessageCharacter * pMsgChar = CastNetObject<CMessageCharacter>(msgCharData);
		if (pMsgChar)
		{
			CharMsgCharPtrMap::iterator Itr = gGlobal.g_pMenuFriendProcess->m_MessageCharacters.find(pMsgChar->GetChar_id());
			if (Itr != gGlobal.g_pMenuFriendProcess->m_MessageCharacters.end())
				gGlobal.g_pMenuFriendProcess->m_MessageCharacters.erase(Itr);

			gGlobal.g_pMenuFriendProcess->m_MessageCharacters.insert(std::make_pair(pMsgChar->GetChar_id(), pMsgChar->GetNetId()));

			if (gGlobal.g_pCharacter->GetMapActorID() == pMsgChar->GetChar_id())
				gGlobal.g_pMenuFriendProcess->m_pOwnChar = pMsgChar;

			gGlobal.g_pMenuFriendProcess->ConstructData();
			gGlobal.g_MenuTargetProcess->ResetTargetDetail();
			gGlobal.g_MenuTargetProcess->SetFriendDetail(pMsgChar);
			gGlobal.g_MenuTargetProcess->ShowDetail();

			CharTempFriendDataMap::iterator ItrTemp = gGlobal.g_pMenuFriendProcess->m_TempFriendList.find(pMsgChar->GetChar_id());
			if (ItrTemp != gGlobal.g_pMenuFriendProcess->m_TempFriendList.end())
			{
				ItrTemp->second.state = pMsgChar->GetFriend_status();
				gGlobal.g_pMenuFriendProcess->RefreshMenu();
			}
		}
		else
			DeleteNetObject(msgCharData);
#else
	DeleteNetObject(msgCharData);
#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_BattleEndUpdate(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_MessageReturn(LPCPEER pPeer, RPCContext &context, const String &msg, const String &name)
{
#ifdef QGL
	if (_tcsicmp(msg.c_str(), _T("MSG_FRIEND_IS_ADDED_SUCCESS")) == 0)
	{
		CMessageCharacter * pMsgChar = gGlobal.g_pMenuFriendProcess->FindInfo(name);
		if (pMsgChar)
		{
			gGlobal.g_pMenuFriendProcess->InserTempFriend(pMsgChar->GetChar_id(), name, FRIENC_CHAT_STATUS_ONLINE, true);
		}
	}
	String Msg;
	Msg.Format(gGlobal.GetStringTable()->Get(msg.c_str()), name.c_str());
	//gGlobal.PrintPlayerMessage(Msg);

	gGlobal.g_pMenuFriendProcess->PushNextMessage(Msg, name, name, name, 0, 1);
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_RemoveMessengersByOwner(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_CreateGuild(LPCPEER pPeer, RPCContext &context, const UInt8 &result)
{
	return RPC_RESULT_OK; 
}

RPCResult MessageService_Proxy::CB_DonateMoney(LPCPEER pPeer, RPCContext &context, const UInt32 &addedGDonate, const UInt8 &returnFlag, const UInt32 &addedGMoney)
{
	#ifdef QGL
	if (addedGDonate > 0)
	{
		String Msg;
		Msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_DONATE_SUCCESS")), addedGDonate);
		gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM);
	}

	if (returnFlag == GUILD_DONATE_RESULT_FAIL_ONCE_ADAY)
	{		
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_DONATE_FAIL_ONCE_ADAY")), CHANNELTYPE_SYSTEM);
	}
	else if (returnFlag == GUILD_DONATE_RESULT_FAIL_MONEY_ENOUGH)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_DONATE_MONEY_ENOUGH")), CHANNELTYPE_SYSTEM);
	
	#endif
	return RPC_RESULT_OK; 
}
RPCResult MessageService_Proxy::CB_Guilds(LPCPEER pPeer, RPCContext &context, NetGroup* GuildGrp, const bool &openApply)
{
	#ifdef QGL
		gGlobal.g_pMenuGuildProcess->SetGuildGrp(GuildGrp);
		if (openApply)
		{
			UIPanel * pPanel = gGlobal.pUIManager->FindUIPanel("MenuGuildApply");
			if (pPanel == NULL)
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenGuildApplyPanel", String());
		}
		
	#endif
	return RPC_RESULT_OK; 
}

RPCResult MessageService_Proxy::CB_GuildMembers(LPCPEER pPeer, RPCContext &context, NetGroup* GuildMemberGrp)
{
	#ifdef QGL
		gGlobal.g_pMenuGuildProcess->SetGuildMemberGrp(GuildMemberGrp);
	#endif
	return RPC_RESULT_OK; 
}


RPCResult MessageService_Proxy::CB_SendShutdownMessage(LPCPEER pPeer, RPCContext &context, 
													   const UInt32 &downTime)
{
#ifdef QGL
	gGlobal.m_downTime = downTime;
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_CreateGuildMsg(LPCPEER pPeer, RPCContext &context, const String &guildName, const String &creator , const SessionIDVector &listeners)
{
	#ifdef QGL
		String Msg;
		Msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_CREATE_GUILD")), creator.c_str(), guildName.c_str());
		gGlobal.g_pRumorMsgProcess->ShowRumorMessage(Msg);
		gGlobal.PrintMessage(Msg,CHANNELTYPE_RUMOR); 
	#endif // QGL
	return RPC_RESULT_OK;
}
RPCResult MessageService_Proxy::CB_ApplyGuild(LPCPEER pPeer, RPCContext &context, const UInt32 &guild_uid)
{
	#ifdef QGL
	CGuild * pGuild = gGlobal.g_pMenuGuildProcess->FindGuild(guild_uid);
	if (pGuild)
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_APPLY_SUCC")), pGuild->GetName().c_str());
		gGlobal.PrintPlayerMessage(text);
	}
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ChangeMsgIcon(LPCPEER pPeer, RPCContext &context, const Boolean &canChange)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetMessengers(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const UInt32Vector &messgers)
{
	TRACE_INFODTL(GLOBAL_LOGGER,_F("MessageService_Proxy::CB_GetMessengers"));
	return RPC_RESULT_OK;
}
RPCResult MessageService_Proxy::CB_GetMessengersName(LPCPEER pPeer, RPCContext &context, const UInt32 &owner_id, const StringVector &messgers)
{
	TRACE_INFODTL(GLOBAL_LOGGER,_F("MessageService_Proxy::CB_GetMessengersName"));
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetFriendly(LPCPEER pPeer, RPCContext &context, const UInt32 &friendly)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_IsFriend(LPCPEER pPeer, RPCContext &context, const UInt32 &isFriend)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetTargetFromHawk(LPCPEER pPeer, RPCContext &context, const bool &isonline,const String &char_name)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_BuyItemFromBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt16 &finalCount, const UInt32 &finalPrice)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_SellItemToBusinessShop(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt32 &finalPrice)
{
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_OpenBusinessShopMenu(LPCPEER pPeer, RPCContext &context, const Boolean &sell, const UInt16 &shop_id, const UInt8 &shopName, const UInt32 &SynTime, const UInt32 &BusinessMoney, const BusinessShopItemInfoVector &ItemInfoList)
{
#ifdef QGL
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->OpenBusinessShopMenu(sell, shop_id, shopName, SynTime, BusinessMoney, ItemInfoList);
#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_UpdateBusinessShopList(LPCPEER pPeer, RPCContext &context, const BusinessShopItemInfoVector &ItemInfoList)
{
#ifdef QGL
	if ( gGlobal.g_pShopProcess )
		gGlobal.g_pShopProcess->UpdateBusinessShopMenu(ItemInfoList);
#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_GetTargetInfo(LPCPEER pPeer, RPCContext &context, NetObject* msgTargetData)
{
#ifdef QGL
	CMessageCharacter * pMsgChar = CastNetObject<CMessageCharacter>(msgTargetData);
	if (pMsgChar)
	{
		gGlobal.g_MenuTargetProcess->ResetTargetDetail();
		gGlobal.g_MenuTargetProcess->SetFriendDetail(pMsgChar);
		gGlobal.g_MenuTargetProcess->ShowDetail();
	}
#endif
	DeleteNetObject(msgTargetData);
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ShowPublicItemDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CItem &item)
{
#ifdef QGL
	CItem *pItem = const_cast<CItem *>(&item);
	if (!pItem)
		return RPC_RESULT_OK;
	pItem->m_data = gGlobal.m_resource->GetItemData(item.GetItem_id());
	UIPanelEx* pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");

	if (pPanelEx)
	{
		gGlobal.pUIManager->DeletePanel("ItemDetailMenuPanel");
	}

	XML_UI::GetInstance().Init();
	XML_UI::GetInstance().ReadFormFile("Data\\ItemDetail.xml", gGlobal.pUIManager);

	pPanelEx = gGlobal.pUIManager->FindUIPanelEx("ItemDetailMenuPanel");
	if (pPanelEx == NULL)
		return RPC_RESULT_OK;		

	gGlobal.pUIManager->SetPanelEx2Top(pPanelEx);
	pPanelEx->SetEnable(true);

	gGlobal.g_pItemProcess->ItemDetailPopUp(item.GetItem_id(), false,pItem);

	Int Width = pPanelEx->GetWidth();
	Int Height = pPanelEx->GetHeight();
	Int ItemDetailX = gGlobal.pUIManager->GetCursorX();
	Int ItemDetailY = gGlobal.pUIManager->GetCursorY();

	if(ItemDetailY + Height > 599)
		ItemDetailY -= Height;
	if(ItemDetailX + Width > 799)
		ItemDetailX -= Width;
	if(ItemDetailY < 0)
		ItemDetailY = 0;
	if(ItemDetailX < 0)
		ItemDetailX = 0;

	pPanelEx->SetPos(ItemDetailX, ItemDetailY);
#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ShowPublicPartnerDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPartner &partner, const UInt16Vector &skillID)
{

#ifdef QGL

	CPartner* pPartner = const_cast<CPartner *>(&partner);
	if (!pPartner)
		return RPC_RESULT_OK;
	pPartner->m_data = gGlobal.LoadMobTemplate(pPartner->GetMob_id());

	if (pPartner)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\PartnerDetail.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("PartnerDetailPanel");
			if (pPanel == NULL)
				return RPC_RESULT_OK;		
		}

		gGlobal.g_pPartnerProcess->ShowPartnerDetail(pPartner, skillID);

		gGlobal.pUIManager->SetPanel2Top(pPanel);
		pPanel->SetNotDetected(false);

		Int32 posX = 0;
		Int32 posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetHeight();

		if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
			posX = 800 - pPanel->GetWidth();
		else
			posX = gGlobal.pUIManager->GetCursorX();

		if ( posY + pPanel->GetHeight() > 600 )
			posY = 600 - pPanel->GetHeight();
		else
			if ( posY < 0 )
				posY = 0;


		pPanel->SetPos(posX, posY);
		pPanel->SetEnable(true);
	}
#endif
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ShowPublicPetDetailFromOtherLine(LPCPEER pPeer, RPCContext &context, const CPet &pet, const UInt16Vector &skillID)
{
#ifdef QGL
	CPet* pPet = const_cast<CPet *>(&pet);

	if (pPet)
	{
		UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
		if (pPanel == NULL)
		{
			XML_UI::GetInstance().Init();
			XML_UI::GetInstance().ReadFormFile("Data\\PetDetail.xml", gGlobal.pUIManager);

			pPanel = gGlobal.pUIManager->FindUIPanel("PetDetailPanel");
			if (pPanel == NULL)
				return RPC_RESULT_OK;		
		}
		gGlobal.g_pMenuPetProcess->ShowPetDetail(pPet, skillID);
		gGlobal.pUIManager->SetPanel2Top(pPanel);

		Int32 posX = 0;
		Int32 posY = gGlobal.pUIManager->GetCursorY() - pPanel->GetHeight();

		if ( ( gGlobal.pUIManager->GetCursorX() + pPanel->GetWidth() ) > 800 )
			posX = 800 - pPanel->GetWidth();
		else
			posX = gGlobal.pUIManager->GetCursorX();

		if ( posY + pPanel->GetHeight() > 600 )
			posY = 600 - pPanel->GetHeight();
		else
			if ( posY < 0 )
				posY = 0;


		pPanel->SetPos(posX, posY);
		pPanel->SetEnable(true);
	}

#endif
	return RPC_RESULT_OK;

}

RPCResult MessageService_Proxy::CB_ReceiveFriendMessage(LPCPEER pPeer, RPCContext &context, const String &Msg, const String &SrcName)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("ReceiveMessage from %s: %s"), SrcName.c_str(), Msg.c_str());
#ifdef QGL
	//gGlobal.NotifyReceiveMessage(pPeer, context, Msg, SrcName);
	gGlobal.PrintMessage(Msg,CHANNELTYPE_FRIEND, SrcName);
#endif // QGL
	return RPC_RESULT_OK;
}

RPCResult MessageService_Proxy::CB_ChangeFriendly(LPCPEER pPeer, RPCContext &context)
{	
	return RPC_RESULT_OK;
}