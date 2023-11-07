//-- Common
#include "Common.h"
#include "common/common_binary.h"
#include "common/CallLuaFunc.h"
//-- Self
#include "Proxy/Logic/Character.h"
//-- Library
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/ItemBag.h"
#include "Proxy/Logic/Map.h"
#include "Proxy/Logic/QuestSystem.h"
#include "Proxy/Logic/Trading.h"
#include "Proxy/Logic/ChargeShop.h"
#include "Proxy/Logic/PetGroup.h"
#include "Proxy/Logic/HawkGroup.h"
#include "Proxy/Logic/GiftBox.h"
#include "Proxy/Logic/Gift.h"
#include "Proxy/Logic/Account.h"
#include "Proxy/Logic/Battle.h"
#include "Proxy/Logic/BatCharacter.h"
#include "Proxy/Logic/BatMob.h"
#include "Proxy/Logic/Partner.h"
#include "Resource/ResLineup.h"
#include "Proxy/Logic/Pet.h"
#ifdef QGL
#include "QGL/Global.h"
#include "QGL/Configure.h"
#include "QGL/CharacterControl.h"
#include "QGL/MapNPCControl.h"
#include "QGL/AccountControl.h"
#include "Resource/ResItem.h"
#include "Resource/ClientResManager.h"
#include "QGL/MenuPartyProcess.h"
#include "QGL/BattleControl.h"
#include "QGL/MenuCharacterSimulator.h"
#include "Common/CallLuaFunc.h"
#include "QGL/QGLApplication.h"
#include "QGL/MenuJoinBattleProcess.h"
#include "QGL/MenuSkillProcess.h"
#include "QGL/MenuItemProcess.h"
#include "QGL/MenuQuestLogProcess.h"
#include "QGL/MenuCharProcess.h"
#include "QGL/MenuGivenProcess.h"
#include "QGL/MiddleMap.h"
#include "QGL/MenuSystemSetting.h"
#include "QGL/MenuFriendProcess.h"
#include "QGL/MenuTradingProcess.h"
#include "QGL/MenuHelpProcess.h"
#include "QGL/MenuOnlineShopProcess.h"
#include "QGL/MenuPetProcess.h"
#include "QGL/MenuTitleProcess.h"
#include "QGL/AntiHack.h"
#include "QGL/MenuHawkerProcess.h"
#include "QGL/MenuGiftbox.h"
#include "QGL/ScriptControl.h"
#include "QGL/UIEntity.h"
#include "QGL/QGLApplication.h"
#include "QGL/Math.h"
#include "QGL/QuestionProcess.h"
#include "Proxy/Logic/Question.h"
#include "Proxy/Logic/RelationSystem.h"
#include "QGL/MenuWareHouseProcess.h"
#include "QGL/MenuAutoWalkProcess.h"
#include "QGL/MenuPartnerProcess.h"
#include "QGL/MenuTargetEquipProcess.h"
//#include "QGL/WeaponsPointSystem.h"

#include "QGL/RelationSystemProcess.h"
#include "QGL/MenuSellYuanBaoProcess.h"
#include "qgl/GenerationProcess.h"
#include "Partner.h"
#include "ChannelType.h"
#include "QGL/MenuGuildProcess.h"
#include "QGL/CrossMapPathFinder.h"
#include "QGL/MenuSearchListProcess.h"
#include "Proxy/Logic/PartyCharInfo.h"
#include "Proxy/Logic/PartyCharInfoClient.h"
#include "QGL/PointsShopProcess.h"
#include "Proxy/Logic/SkillGroup.h"
#endif // QGL

REGISTER_CLASSTYPE(CCharacter, Character_CLASSID);

PROTO_LuaPush(CMap)
PROTO_LuaPush(CBatActor)
PROTO_LuaPush(CBatMob)
PROTO_LuaPush(CBatCharacter)
PROTO_LuaPush(CBattleControl)
PROTO_LuaPush(CCharacterControl)

PSTRINGA PKName = "ForcePK";

CCharacter::CCharacter()
{
	pCharView = NULL;
	m_uiCurAcc=0;
	m_uiHisAcc=0;
}

CCharacter::~CCharacter()
{
	if(pCharView)
		pCharView->SetMapActorObject(NULL);

	if (gGlobal.g_pJoinBattleProcess)
		gGlobal.g_pJoinBattleProcess->RemoveCharacterFromList(this);

	if (gGlobal.g_pPartyProcess && gGlobal.g_pPartyProcess->IsInRequestList(GetNetId()))
		gGlobal.g_pPartyProcess->RemoveRequest(GetNetId());

	if (gGlobal.pUIManager)
	{
		StringA panelName;
		panelName.Format("Hawker%dPanel", GetChar_id());
		gGlobal.pUIManager->DeletePanel(panelName.c_str());
		if (gGlobal.g_pTargetHawkGroup && gGlobal.g_pTargetHawkGroup->GetOwnerID() == GetChar_id())
			CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseHawk", String());
	}
}

VOID CCharacter::OnUpdateGoodness(const Int32 &goodness)
{
	#ifdef QGL
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.g_pCharProcess->UpdateCharGoodness();
	#endif
}


VOID CCharacter::OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("OnUpdateId: acct_id %d, char_id %d"), acct_id, char_id);
}

VOID CCharacter::OnUpdateLevelInfo(const UInt16 &level, const UInt32 &nextLvExp)
{
#ifdef QGL
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
	{
		gGlobal.g_pCharProcess->UpdateCharLevel();
		gGlobal.g_pMenuGuildProcess->RefreshInfoMenu();
	}
	
#endif // QGL
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("OnUpdateLevelInfo %d: level %d, nextExp %d"), GetChar_id(), level, nextLvExp);
}

VOID CCharacter::OnUpdatePartyInfo(const Boolean &hasParty, const Boolean &isLeader, const RPCNetID &partyNetID)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("OnUpdatePartyInfo: hasParty %d, isLeader %d"), 
		hasParty, isLeader);

#ifdef QGL
	if (pCharView)
		pCharView->OnUpdatePartyInfo(hasParty, isLeader);

		if (!hasParty && gGlobal.g_pPartyProcess && gGlobal.g_pPartyProcess->IsInInviteList(GetChar_id()))
		{
			gGlobal.g_pPartyProcess->InviteRejectByCharID(GetChar_id());
		}
		else if (hasParty)
		{
			RPCNetID noneID;
			if (partyNetID != noneID && gGlobal.g_pPartyProcess)
			{
				gGlobal.g_pPartyProcess->InsertCharToOtherParty(partyNetID, this);
			}
		}
	
#endif // QGL
}


RPCResult CCharacter::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CCharacter CB_Error: errcode %d: %s"), errCode, errMsg.c_str());

#ifdef QGL
	if (!errMsg.IsEmpty()) {
		if (errMsg.Left(2) == _T("MS")) {
			String text(gGlobal.GetStringTable()->Get(errMsg.c_str()));
			if (!text.IsEmpty())
				gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
			else
				gGlobal.PrintMessage(errMsg, CHANNELTYPE_SYSTEM);
		} else {

			gGlobal.PrintMessage(errMsg, CHANNELTYPE_SYSTEM);

		}
	}
		
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &Result, const UInt32 &resultFlag)
{
	#ifdef QGL
	if (Result == MSG_RETURN_FAIL_TOO_FREQUENT)
	{
		String text;
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANNEL_TOOFREQ")), resultFlag);
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM, String());
	}
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_NO_COUNT)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_NO_COUNT")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_LV)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_LV")), CHANNELTYPE_SYSTEM, String());
	else if (Result == MSG_RETURN_FAIL_NO_RIGHT_BLOCK)
	{
		String text;
		UInt32 now = (UInt32) time(NULL);
		UInt32 count = 0;
		if (now <  gGlobal.g_pCharacter->GetCharNetObject()->GetSpeakBlock())
			count = gGlobal.g_pCharacter->GetCharNetObject()->GetSpeakBlock() - now;
		String remainText;

		if (count >= 86400)
			remainText.Format(_T("%d%s"), count / 86400, gGlobal.GetStringTable()->Get(_T("MSG_DAY")));
		else if (count >= 3600)
			remainText.Format(_T("%d%s"), count / 3600, gGlobal.GetStringTable()->Get(_T("MSG_HOUR")));
		else
			remainText.Format(_T("%d%s"), count, gGlobal.GetStringTable()->Get(_T("MSG_SECOND")));
		text.Format(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_FAIL_NO_RIGHT_BLOCK")), remainText.c_str());
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM, String());
	}
	#endif // QGL
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_JoinParty(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_JoinParty: Party joined succeeded"));

#ifdef QGL
	CParty* pParty = CastNetObject<CParty>(party);

	if (pParty == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CCharacter CB_JoinParty Error: invalid party"));
		return RPC_RESULT_FAIL;
	}
	if (gGlobal.g_pParty || gGlobal.g_pTempLeaveParty)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CCharacter CB_JoinParty Error: already have party"));
		return RPC_RESULT_FAIL;
	}

	gGlobal.g_pParty = pParty;
	gGlobal.g_vctPartyMemberPointer.clear();
	for (UInt16 j=0;j<pParty->GetCount();j++) 
	{
		if (pParty->GetM_bTempLeave(j))
			gGlobal.g_vctPartyMemberPointer.push_back(NULL);
		else
			gGlobal.g_vctPartyMemberPointer.push_back(pParty->GetChild<CCharacter>(j));
	}

	gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_FORMTEAM);

	//create temp friend if not exist

	for (UInt i = 0; i < pParty->GetCount(); i++)
	{
		CCharacter* pMember = CastNetObject<CCharacter>(pParty->GetChild<CCharacter>(i));
		if (pMember)
		{
			gGlobal.g_pMenuFriendProcess->InserTempFriend(pMember->GetChar_id(), pMember->GetNickName(), FRIENC_CHAT_STATUS_ONLINE);	
		}
	}
	
	///////////////////////////////////////////////////////
	SetFollowPartner(gGlobal.pPeerProxy, gGlobal.g_context, 0);
	/////////////////////////////////////////////////////////

	pParty->RedrawUI();
	pParty->ResetNameColor();

	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
	{
		UIButton *pItem = gGlobal.pUIManager->FindUIButton("DownBParty");
		if (pItem)
		{
			if (pParty->IsLeader())
				pItem->SetNormalState();
			else
				pItem->SetDisableState();
		}

	}

	if (!pParty->IsLeader())
	{
		gGlobal.g_pMiddleMap->ResetCharDes();
		CCrossMapPathFinder::GetInstance()->DestroyPathData();
		gGlobal.m_u32DestNpcId = 0;
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
	}

	//print message
	/*{
		CCharacter* pchar = pParty->GetChild<CCharacter>(pParty->GetCount() - 1);
		if (pchar)
		{
			String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_JOIN"));
			text.Format(text.c_str(), pchar->GetNickName().c_str());
			gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		}
	}*/
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseHawk", String());
	gGlobal.g_pPartyProcess->UpdatePartyPanel(pParty);
	
	//m_pParty->ShowMember();
#endif // QGL

	return RPC_RESULT_OK;
}


RPCResult CCharacter::CB_ChangeDesc(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
#ifdef QGL
	CParty* pParty = CastNetObject<CParty>(party);

	if (pParty == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CCharacter CB_JoinParty Error: invalid party"));
		return RPC_RESULT_FAIL;
	}
	if (gGlobal.g_pParty)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CCharacter CB_JoinParty Error: already have party"));
		return RPC_RESULT_FAIL;
	}

	gGlobal.g_pParty = pParty;

	//gGlobal.g_pParty->GetDesc();

#endif
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_PartyRequestOk(LPCPEER pPeer, RPCContext &context, const String &name)
{
#ifdef QGL
	//print message
	{
		String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_REQUEST_JOIN")));
		text.Format(text.c_str(), name.c_str());
		gGlobal.PrintPlayerMessage(text);
	}
#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyRequestNo(LPCPEER pPeer, RPCContext &context, const UInt16 &nState)
{
#ifdef QGL
	//print message
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_LEVEL_UNQUALIFIED")));
	}
#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyInviteOk(LPCPEER pPeer, RPCContext &context, const String &name)
{
#ifdef QGL
	//print message
	{
		String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_REQUEST_INVITE")));
		text.Format(text.c_str(), name.c_str());
		gGlobal.PrintPlayerMessage(text);
	}
#endif
	return RPC_RESULT_OK;

}

RPCResult CCharacter::CB_PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId)
{
#ifdef QGL
	Boolean reply = true;
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_PartyJoinRequest: target %d, %d party join request auto-reply OK"),
		targetId.srvId, targetId.objId);

	CCharacter* target = FindNetObject<CCharacter>(targetId);

	/*int autoReply = GetPrivateProfileInt(_T("AUTOREPLY"), _T("AUTO"), 0, CONFIGURATION_INI_FILE);

	if (autoReply)
		PartyJoinReply(gGlobal.pPeerProxy, context, targetId, reply, true);
	else*/
		gGlobal.g_pPartyProcess->JoinRequest(target);
	//
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyInviteRequest(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
#ifdef QGL
	Boolean reply = true;
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_PartyInviteRequest: party invite request auto-reply OK"));
	CParty* pParty = CastNetObject<CParty>(party);
	if (pParty == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CB_PartyInviteRequest Error: invalid party"));
		return RPC_RESULT_FAIL;
	}

	/*int autoReply = GetPrivateProfileInt(_T("AUTOREPLY"), _T("AUTO"), 0, CONFIGURATION_INI_FILE);

	if (autoReply)
		PartyInviteReply(gGlobal.pPeerProxy, context, pParty->GetNetId(), reply, true);
	else*/
		gGlobal.g_pPartyProcess->InviteComing(pParty);

	
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyClearRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &showMsg)
{
#ifdef QGL
	CCharacter* target = FindNetObject<CCharacter>(targetId);

	if (target && gGlobal.g_pParty == NULL && showMsg)
	{
		String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_JREJECT")));
		text.Format(text.c_str(), target->GetNickName().c_str());
		gGlobal.PrintPlayerMessage(text);
	}

	gGlobal.g_pPartyProcess->RemoveRequest(targetId);
#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyClearInvite(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &showMsg)
{
#ifdef QGL

	{
		//for show the message for rejecting some party
		CCharacter* target = FindNetObject<CCharacter>(partyId);
			if (target && showMsg)
			{
				String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_IREJECT")));
				text.Format(text.c_str(), target->GetNickName().c_str());
				gGlobal.PrintPlayerMessage(text);
			}
	}

	gGlobal.g_pPartyProcess->RemoveParty(partyId);
#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_LevelUp(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_LevelUp:"));

	FSound* sfx = FSOUND_Manager::GetInstance().CreateSound(_T("Data\\Wav\\SoundE0009_lv_up.wav"));
	if( sfx )
	{
		sfx->SetVolume( gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
		sfx->Play();
	}

	if (pCharView)
	{
		EffectData EData;
		EData.effectFileName = _T("eff_scr_levelup.ent");
		EData.targetEnt = pCharView->GetCharMainView();
		EData.rotation = pCharView->GetCharMainView()->GetAnimationRotation();
		QGLApplication::GetInstance().AddEffectToLst(::timeGetTime(), EData);
	}

	gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_LEVEL_UP);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_RaiseAttrib:"));

	if( gGlobal.g_pMenuCharSimulator )
		gGlobal.g_pMenuCharSimulator->Reset();

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_CancelAuto(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_CancelAuto:"));

	if(pCharView && pCharView == gGlobal.g_pCharacter)
	{
		gGlobal.g_pBattleControl->ShowAutoCountBox(0);
	}

	return RPC_RESULT_OK;
}

VOID CCharacter::OnUpdateSpeakCount(const UInt16 &globalSpeakCount)
{
	String msg;
	msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_SPEAK_COUNT")), globalSpeakCount);
	gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
}
VOID CCharacter::OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	if (pCharView == NULL)
		return;

	pCharView->OnUpdateEqWeapon(eq_weapon, eq_weaponColor, GetSexClass());
	if (this == gGlobal.g_pCharacter->GetCharNetObject())
	{
		if (gGlobal.g_pItemProcess )
			gGlobal.g_pItemProcess->UpdateCharWeaponEnt(eq_weapon, eq_weaponColor);
		if (gGlobal.g_pHawkerProcess)
			gGlobal.g_pHawkerProcess->UpdateCharWeaponEnt(eq_weapon, eq_weaponColor);
	}

}
VOID CCharacter::OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	if (pCharView == NULL)
		return;

	pCharView->OnUpdateEqHead(eq_headBand, eq_headColor, GetSexClass());
	if (this == gGlobal.g_pCharacter->GetCharNetObject())
	{
		if (gGlobal.g_pItemProcess )
			gGlobal.g_pItemProcess->UpdateHead(eq_headBand, eq_headColor);
		if (gGlobal.g_pHawkerProcess)
			gGlobal.g_pHawkerProcess->UpdateHead(eq_headBand, eq_headColor);
	}

}

void CCharacter::OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	if (pCharView == NULL)
		return;

	UInt16 tempHairStyle1 = hairStyle1;
	UInt16 tempHairColor = hairColor;

	if (hairStyle1 == 0)
		tempHairStyle1 = GetOrg_hairStyle1();
	if (hairColor == 0)
		tempHairColor = GetOrg_hairColor();

	pCharView->OnUpdateHead(tempHairStyle1, tempHairColor, GetSexClass());
}

void CCharacter::OnUpdateOrgHairStyle(const UInt16 &org_hairStyle1, const UInt16 &org_hairColor)
{
	if (pCharView == NULL)
		return;

	UInt16 hairStyle1 = org_hairStyle1;
	UInt16 hairColor = org_hairColor;

	if (GetHairStyle1() > 0)
		hairStyle1 = GetHairStyle1();
	if (GetHairColor() > 0)
		hairColor = GetHairColor();

	pCharView->OnUpdateHead(hairStyle1, hairColor, GetSexClass());
}

VOID CCharacter::OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	if (pCharView == NULL)
		return;

	UInt32 eqCloth = eq_cloth;
	UInt16 eqClothColor = eq_clothColor;

	if (eq_cloth == 0)
	{
		eqCloth = GetOrg_eq_cloth();
		eqClothColor = GetOrg_eq_clothColor();
	}

	pCharView->OnUpdateEqCloth(eqCloth, eqClothColor, GetSexClass());

	if (this == gGlobal.g_pCharacter->GetCharNetObject())
	{
		if (gGlobal.g_pItemProcess )
			gGlobal.g_pItemProcess->UpdateBody(eqCloth);
		if (gGlobal.g_pHawkerProcess)
			gGlobal.g_pHawkerProcess->UpdateBody(eqCloth);
	}
}

void CCharacter::OnUpdateOrgEqCloth(const UInt32 &org_eq_cloth, const UInt16 &org_eq_clothColor)
{
	if (pCharView == NULL)
		return;
	UInt32 eqCloth = org_eq_cloth;
	UInt16 eqClothColor = org_eq_clothColor;

	if (GetEq_cloth() > 0)
	{
		eqCloth = GetEq_cloth();
		eqClothColor = GetEq_clothColor();
	}
	
	pCharView->OnUpdateEqCloth(eqCloth, eqClothColor, GetSexClass());

	if (this == gGlobal.g_pCharacter->GetCharNetObject())
	{
		if (gGlobal.g_pItemProcess )
			gGlobal.g_pItemProcess->UpdateBody(eqCloth);
		if (gGlobal.g_pHawkerProcess)
			gGlobal.g_pHawkerProcess->UpdateBody(eqCloth);
	}
}



VOID CCharacter::OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP)
{
	bool needChange = false;
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
	{
		gGlobal.SetDisplayHP(HP, mHP, maxHP);
		needChange = true;
	}
	else if (gGlobal.g_pParty )
	{
		CParty* pParty = gGlobal.g_pParty;
		UInt16 memberPos = 0;
		for (UInt16 i = 0; i < pParty->GetCount() ; ++i)
		{
			CCharacter* member = pParty->GetChild<CCharacter>(i);

			if (member !=  gGlobal.g_pCharacter->GetCharNetObject())
				memberPos ++;

			if (member == this)
			{
				pParty->SetUIMemberHPBar(memberPos,HP, mHP, maxHP);
				needChange =true;
				break;
			}
		}

		if (!needChange) 
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();
			for (;it!= gGlobal.g_mapPartyChar.end();it++) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					if (pCpcic->GetU32CharId() == GetChar_id()) 
					{
						pCpcic->SetU16Hp(HP);
						pCpcic->SetU16MHp(mHP);
						pCpcic->SetU16MaxHp(maxHP);
						gGlobal.g_pTempLeaveParty->SetUIMemberHPBar(it->first,HP,mHP,maxHP);
						break;
					}
				}
			}
		}
	}
	else if (gGlobal.g_pTempLeaveParty) 
	{//temporarily leave now
		Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();
		for (;it!= gGlobal.g_mapPartyChar.end();it++) 
		{
			CPartyCharInfoClient* pCpcic = it->second;
			if (NULL != pCpcic) 
			{
				if (pCpcic->GetU32CharId() == GetChar_id()) 
				{
					pCpcic->SetU16Hp(HP);
					pCpcic->SetU16MHp(mHP);
					pCpcic->SetU16MaxHp(maxHP);
					gGlobal.g_pTempLeaveParty->SetUIMemberHPBar(it->first,HP,mHP,maxHP);
					break;
				}
			}
		}
	}
	
	if (needChange && QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE && gGlobal.g_pBattleControl)
	{
		CAnimatedEntity* pHpBar = gGlobal.g_pBattleControl->FindCharHpBar(gGlobal.g_pBattleControl->FindCharCtrlByChar_id(GetChar_id()));
		if (pHpBar)
		{
			Real32 ScaleX = (Real32)GetHP() / 
				(Real32)GetMaxHP();
			pHpBar->SetScaleX(ScaleX);
		}
	}

	
}

VOID CCharacter::OnUpdateDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.SetDisplayDP(DP, maxDP);
}

VOID CCharacter::OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.SetDisplaySP(SP, mSP, maxSP);
	else if (gGlobal.g_pParty )
	{
		bool bFind = false;
		CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
		UInt16 memberPos = 0;
		for (UInt16 i = 0; i < pParty->GetCount() ; ++i)
		{
			CCharacter* member = pParty->GetChild<CCharacter>(i);

			if (member !=  gGlobal.g_pCharacter->GetCharNetObject())
				memberPos ++;
			if (member == this)
			{
				bFind =true;
				pParty->SetUIMemberSPBar(memberPos,SP, mSP, maxSP);
				break;
			}
		}

		if (!bFind) 
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();
			for (;it!= gGlobal.g_mapPartyChar.end();it++) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					if (pCpcic->GetU32CharId() == GetChar_id()) 
					{
						pCpcic->SetU16Sp(SP);
						pCpcic->SetU16MSp(mSP);
						pCpcic->SetU16MaxSp(maxSP);
						gGlobal.g_pTempLeaveParty->SetUIMemberHPBar(it->first,SP,mSP,maxSP);
						break;
					}
				}
			}
		}
	}
	else if (gGlobal.g_pTempLeaveParty) 
	{//temporarily leave now
		Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();
		for (;it!= gGlobal.g_mapPartyChar.end();it++) 
		{
			CPartyCharInfoClient* pCpcic = it->second;
			if (NULL != pCpcic) 
			{
				if (pCpcic->GetU32CharId() == GetChar_id()) 
				{
					pCpcic->SetU16Sp(SP);
					pCpcic->SetU16MSp(mSP);
					pCpcic->SetU16MaxSp(maxSP);
					gGlobal.g_pTempLeaveParty->SetUIMemberHPBar(it->first,SP,mSP,maxSP);
					break;
				}
			}
		}
	}
}

VOID CCharacter::OnUpdateWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp, const UInt32 &nextWCExp)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.SetDisplayWC(WC, maxWC, WcExp, gGlobal.g_pCharacter->GetCharNetObject()->GetNextWCExp());
}

VOID CCharacter::OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.SetDisplayVP(VP, maxVP);
}

VOID CCharacter::OnUpdateExp(const UInt32 &exp)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
	{
		if(pCharView)
		{	
			UIButton* pBtn = gGlobal.pUIManager->FindUIButton("CharLevelUp");
			if(pCharView->GetLevel() <= 10 && exp >= pCharView->GetNextLvExp() && !pBtn)
			{
				if ((pCharView->GetLevel()+1) == 10)
					gGlobal.g_pMenuHelpProcess->HandleGuideEvent(21);
				UIAniIcon* MyFaceSparkUi = gGlobal.pUIManager->FindUIAniIcon("MyFaceSpark");
				if (MyFaceSparkUi)
				{
					if(!MyFaceSparkUi->GetAniEnt()->HasAniFile())
						MyFaceSparkUi->SetAnimationData(String(_T("HeadIconS.ent")).c_str());
					if (!MyFaceSparkUi->IsEnable())
						MyFaceSparkUi->SetEnable(true);
					gGlobal.pUIManager->AddDirtyUIItem(MyFaceSparkUi);
				}
			}

		}

		gGlobal.g_pCharProcess->UpdateCharExp();//UpdateCharMenu();
		if ((pCharView->GetLevel()+1) < 10)
		{
			if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
				gGlobal.g_pCharacter->GetCharNetObject()->LevelUp(gGlobal.pPeerProxy, gGlobal.g_context);
		}
		gGlobal.g_pSkillProcess->RefreshShopDetail();
	}
}

void CCharacter::OnUpdateAttrib1(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.g_pCharProcess->UpdateCharAttr2();//UpdateCharMenu();
}

void CCharacter::OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.g_pCharProcess->UpdateCharAttr1();//UpdateCharMenu();
}

void CCharacter::OnUpdateAttrPoint(const UInt16 &attrPoint)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
	{
		gGlobal.g_pCharProcess->UpdateAttrPoint();//UpdateCharMenu();
		if( gGlobal.g_pMenuCharSimulator )
			gGlobal.g_pMenuCharSimulator->Reset();
	}
}

RPCResult CCharacter::CB_PKRequest(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag, const UInt32 &Target_ID, const UInt8 &teamCount)
{
	switch(returnFlag)
	{
	case PK_REQUEST_RESULT_FAIL_MONITOR_FULL:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_MONITOR_FULL")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NOT_FOUND_TARGET:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_FOUND_TARGET")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_CANNOT_TARGET_SELF:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_CANNOT_TARGET_SELF")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_IN_BATTLE_TARGET:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_IN_BATTLE_TARGET")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_TARGET:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_TARGET")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_TARGET:
		gGlobal.g_PKRequestBox.ShowMessageBox(0, gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_TARGET")), PKName);
		break;
	case PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_TARGET:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_TARGET")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NOT_LEADER_TARGET:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_LEADER_TARGET")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NOT_FOUND_SELF:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_FOUND_SELF")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_IN_BATTLE_SELF:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_IN_BATTLE_SELF")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_SELF:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_BATTLEFIELD_SELF")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_SELF:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_SELF")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_SELF:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_LV_SELF")), CHANNELTYPE_SYSTEM, String());
		break;
	//case PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_HONOR_SELF:
	//	gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_ENOUGH_HONOR_SELF")), CHANNELTYPE_SYSTEM, String());
	//	break;
	case PK_REQUEST_RESULT_FAIL_NOT_LEADER_SELF:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NOT_LEADER_SELF")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_MATCH_BATTLE_REQUEST:
		{
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(Target_ID);
			gGlobal.g_TempPKTargetID = Target_ID;
			if (pCtrl && pCtrl->GetCharNetObject())
			{
				String buf;
				buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_MATCH_BATTLE_REQUEST")), pCtrl->GetCharName(),
					pCtrl->GetCharNetObject()->GetLevel());
				gGlobal.g_PKRequestBox.ShowMessageBox(3, buf, PKName);
			}
		}
		break;
	case PK_REQUEST_RESULT_LOW_MARTIAL_BATTLE_REQUEST:
		{
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(Target_ID);
			gGlobal.g_TempPKTargetID = Target_ID;
			if (pCtrl && pCtrl->GetCharNetObject())
			{
				String buf;
				buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_LOW_MARTIAL_BATTLE_REQUEST")), 
					 pCtrl->GetCharName(), pCtrl->GetCharNetObject()->GetLevel(), teamCount);
				gGlobal.g_PKRequestBox.ShowMessageBox(3, buf, PKName);
			}
		}
		break;
	case PK_REQUEST_RESULT_HIGH_MARTIAL_BATTLE_REQUEST:
		{
			CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(Target_ID);
			gGlobal.g_TempPKTargetID = Target_ID;
			if (pCtrl && pCtrl->GetCharNetObject())
			{
				String buf;
				buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_HIGH_MARTIAL_BATTLE_REQUEST")), 
					 pCtrl->GetCharName(), pCtrl->GetCharNetObject()->GetLevel(), teamCount);
				gGlobal.g_PKRequestBox.ShowMessageBox(3, buf, PKName);
			}
		}
		break;
	case PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_REJECT:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_REJECT")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_TARGET_NOT_ENABLE:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_TARGET_NOT_ENABLE")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_TARGET_CHANGE:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_MATCH_BATTLE_FAIL_TARGET_CHANGE")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_BLOCK_PK:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_BLOCK_PK")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_PROTECT_PK:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_PROTECT_PK")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_TARGET_BUSY:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_TARGET_BUSY")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_TARGET_ALREADY_ASKED:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_TARGET_ALREADY_ASKED")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_WAR_NOT_START:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_WAR_NOT_START")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_SAME_GUILD:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_SAME_GUILD")), CHANNELTYPE_SYSTEM, String());
		break;
	case PK_REQUEST_RESULT_FAIL_RESTING:
		{
		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(Target_ID);
		gGlobal.g_TempPKTargetID = Target_ID;
		if (pCtrl && pCtrl->GetCharNetObject())
		{
			String buf;
			buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_RESTING")), pCtrl->GetCharName());
			gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM, String());
		}
		}
		break;
	case PK_REQUEST_RESULT_FAIL_VP_NOT_ENOUGH:
		{
		CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(Target_ID);
		gGlobal.g_TempPKTargetID = Target_ID;
		if (pCtrl && pCtrl->GetCharNetObject())
		{
			String buf;
			buf.Format(gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_VP_NOT_ENOUGH")), pCtrl->GetCharName());
			gGlobal.PrintMessage(buf, CHANNELTYPE_SYSTEM, String());
		}
		}
		break;
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetEnablePK(LPCPEER pPeer, RPCContext &context, const BYTE &result)
{
	return RPC_RESULT_OK;
}

VOID CCharacter::OnUpdateHonor(const UInt32 &honor)
{
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.g_pCharProcess->UpdateCharHonor();//UpdateCharMenu();

	if (pCharView == gGlobal.g_pCharacter)
	{
		gGlobal.g_pSkillProcess->RefreshShopDetail();	
	}
}

VOID CCharacter::OnUpdatePartner(const UInt32 &partner, const UInt32 &partnerMob_id, const String &partnerName, const UInt16 &colorIndex)
{
	if (pCharView)
	{
		if (partner != 0)
		{
			pCharView->SetHasPartner(true);
			pCharView->SetPartner(partnerMob_id, partnerName, colorIndex);
		}
		else
		{
			pCharView->SetHasPartner(false);
			pCharView->SetPartner(0, String(), 0);
		}
	}
}

VOID CCharacter::OnUpdatePet(const UInt32 &pet, const UInt32 &pet_id, const String &petName, const UInt16 &petColorIndex)
{
	if (pCharView)
		pCharView->OnUpdatePet(pet, pet_id, petName, petColorIndex);
}

VOID CCharacter::OnUpdateManor(const UInt32 &manor, const UInt32 &pettwo_uid, const UInt32 &pettwo_id, const String &pettwoName, const UInt16 &pettwoColorIndex)
{
	if (pCharView)
		pCharView->OnUpdatePettwo(pettwo_uid,pettwo_id,pettwoName,pettwoColorIndex);
}

VOID CCharacter::OnUpdateFollowerList(const UInt32Vector &followerList)
{
	if (pCharView)
		pCharView->OnUpdateFollowerList(followerList);
}

void CCharacter::OnUpdateHistoryGenerationAcc(const UInt32 &iHisGenAcc)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("OnUpdateHistoryGenerationAcc: iHisGenAcc %d"), iHisGenAcc);
}
void CCharacter::OnUpdateCurrentGenerationAcc(const UInt32 &uiCurGenAcc)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("OnUpdateHistoryGenerationAcc: uiCurGenAcc %d"), uiCurGenAcc);
}

RPCResult CCharacter::CB_JoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	switch(returnFlag)
	{
	case JOIN_BATTLE_OK:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_JOIN_BATTLE_OK")), CHANNELTYPE_SYSTEM, String());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "HideBattleWatchPanel", String());
		if (!gGlobal.g_NoMoreHelp &&
			(gGlobal.g_pParty == NULL || 
			gGlobal.g_pParty->IsLeader()))
		{
			gGlobal.g_helpEnable = TRUE;
			UIButton * pBu = gGlobal.pUIManager->FindUIButton("BattleHelpB");
			if (pBu)
			{
				pBu->SetNormalState();
				gGlobal.pUIManager->AddDirtyUIItem(pBu);
			}
		}
		//show 
		break;
	case JOIN_BATTLE_FAIL_FULL:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_JOIN_BATTLE_FAIL_FULL")), CHANNELTYPE_SYSTEM, String());
		break;
	case JOIN_BATTLE_REJECT:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_JOIN_BATTLE_REJECT")), CHANNELTYPE_SYSTEM, String());
		break;
	case JOIN_BATTLE_INVALID:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_JOIN_BATTLE_INVALID")), CHANNELTYPE_SYSTEM, String());
		break;
	case JOIN_BATTLE_ALREADYJOIN:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_JOIN_BATTLE_ALREADYJOIN")), CHANNELTYPE_SYSTEM, String());
		UIButton * pBu = gGlobal.pUIManager->FindUIButton("BattleJoinB");
		if (pBu)
		{
			pBu->SetDisableState();
			gGlobal.pUIManager->AddDirtyUIItem(pBu);
		}
		break;
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &Char_id)
{
	if (gGlobal.g_pMap)
	{
		gGlobal.g_pJoinBattleProcess->AddCharacterToList(gGlobal.g_pMap->FindCharacter(Char_id));
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RejectHelper(LPCPEER pPeer, RPCContext &context)
{
	gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_JOIN_BATTLE_REJECT")), CHANNELTYPE_SYSTEM, String());
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_LeaveHelpersList(LPCPEER pPeer, RPCContext &context, const UInt32 &Char_id)
{
	if (gGlobal.g_pMap)
	{
		gGlobal.g_pJoinBattleProcess->RemoveCharacterFromList(gGlobal.g_pMap->FindCharacter(Char_id));
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ClearHelpersList(LPCPEER pPeer, RPCContext &context)
{
	gGlobal.g_pJoinBattleProcess->ClearList();
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	switch(returnFlag)
	{
	case REQUEST_JOIN_BATTLE_OK:
	case REQUEST_JOIN_BATTLE_FAIL_ALREADY_INSIDE:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_JOIN_BATTLE_FAIL_ALREADY_INSIDE")), CHANNELTYPE_SYSTEM, String());
		gGlobal.DisableJoinBattle();
		break;
	
	case REQUEST_JOIN_BATTLE_FAIL_TOO_FRE:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_JOIN_BATTLE_FAIL_TOO_FRE")), CHANNELTYPE_SYSTEM, String());
		break;
	case REQUEST_JOIN_BATTLE_FAIL_TOO_MANY:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_JOIN_BATTLE_FAIL_TOO_MANY")), CHANNELTYPE_SYSTEM, String());
		break;
	case REQUEST_JOIN_BATTLE_FAIL_PK_LV:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_JOIN_BATTLE_FAIL_PK_LV")), CHANNELTYPE_SYSTEM, String());
		break;
	case REQUEST_JOIN_BATTLE_FAIL_PK_ENABLE:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_JOIN_BATTLE_FAIL_PK_ENABLE")), CHANNELTYPE_SYSTEM, String());
		break;
	case REQUEST_JOIN_BATTLE_FAIL_PK_FORCE:
		gGlobal.g_PKRequestBox.ShowMessageBox(0, gGlobal.GetStringTable()->Get(_T("MSG_PK_REQUEST_RESULT_FAIL_NO_PK_ENABLE_TARGET")), PKName);
		break;
	case REQUEST_JOIN_BATTLE_FAIL:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_JOIN_BATTLE_FAIL")), CHANNELTYPE_SYSTEM, String());
		break;
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_HelpRequestFail(LPCPEER pPeer, RPCContext &context)
{
	gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_REQUEST_HELP_FAIL")), CHANNELTYPE_SYSTEM, String());
	return RPC_RESULT_OK;
}

void CCharacter::OnUpdateBattleInfo(const Boolean &EnablePK)
{
	if (pCharView == NULL)
		return;

	pCharView->OnUpdateBattleInfo(EnablePK);
}

VOID CCharacter::OnUpdatePKInfo(const UInt16 &PK_Value)
{
	if (pCharView == NULL)
		return;

	pCharView->OnUpdatePKInfo(PK_Value);
}

RPCResult CCharacter::CB_RespondTest(LPCPEER pPeer, RPCContext &context, const UInt16 &playerCount)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag, const String &Msg, const UInt32 &SrcID, const UInt32 &TargetID)
{
	if (gGlobal.g_pCharacter->GetMapActorID() == SrcID ||
		gGlobal.g_pCharacter->GetMapActorID() == TargetID)
		gGlobal.PrintMessage(Msg,  CHANNELTYPE_SYSTEM);

	switch (returnFlag)
	{
	case SKILL_USE_FAIL_TYPE_USING:
		{
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_USE_FAIL_TYPE_USING")),  CHANNELTYPE_SYSTEM);
		}
		break;
	case 0:
		gGlobal.ShowSkillEffectInMap(skill_id, SrcID, TargetID);
		break;
	}

		
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpGradeSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RemoveSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag)
{
	return RPC_RESULT_OK;
}



RPCResult CCharacter::CB_SetTarget(LPCPEER pPeer, RPCContext &context, const Boolean &bResult, const UInt32 &char_id, const UInt16 &x, const UInt16 &y)
{
#ifndef REUBEN_PUBLISH
	if (bResult)
	{
		String strMsg;
		strMsg.Format(_T("Server found char_id %d at x %d y %d"), char_id, x, y);
		gGlobal.PrintMessage(strMsg, CHANNELTYPE_SYSTEM);
	}
	else
	{
		String strMsg;
		strMsg.Format(_T("Server Cannot find char_id %d"), char_id);
		gGlobal.PrintMessage(strMsg, CHANNELTYPE_SYSTEM);
	}
#endif // REUBEN_PUBLISH
	return RPC_RESULT_OK;
}

void CCharacter::OnUpdateBuff(const String &buff)
{
	if (pCharView == NULL)
		return;

	pCharView->OnUpdateBuff(buff);
}

void CCharacter::OnUpdateBuffCache(const String &buffCache)
{
	if (pCharView == NULL)
		return;

	pCharView->OnUpdateBuffCache(buffCache);
}

RPCResult CCharacter::CB_SkillShopLearn(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	switch(returnFlag)
	{
	case SKILL_SHOP_FAIL_EXP:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_EXP")), CHANNELTYPE_SYSTEM);
		break;
	case SKILL_SHOP_FAIL_MONEY:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_MONEY")), CHANNELTYPE_SYSTEM);
		break;
	case SKILL_SHOP_FAIL_LV:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_LV")), CHANNELTYPE_SYSTEM);
		break;
	case SKILL_SHOP_FAIL_MAX_RANK:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_MAX_RANK")), CHANNELTYPE_SYSTEM);
		break;
	case SKILL_SHOP_FAIL_LV2:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_LV2")), CHANNELTYPE_SYSTEM);
		break;
	case SKILL_SHOP_FAIL_HONOR:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_SKILL_SHOP_FAIL_HONOR")), CHANNELTYPE_SYSTEM);
		break;
	}

	return RPC_RESULT_OK;
}

void CCharacter::OnUpdateYuanBao(const UInt32 &yuanBao, const UInt32 &yuanBaoTicket)
{
	if (pCharView == NULL)
		return;

	if (pCharView == gGlobal.g_pCharacter &&
		gGlobal.g_pSkillProcess->GetIsShop())
	{
		gGlobal.g_pSkillProcess->RefreshShopDetail();
	}

	if (pCharView == gGlobal.g_pCharacter)
	{
		if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_CHAR_MENU)
			gGlobal.g_pItemProcess->NewUpdateItemCharacter();
		if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_CHAR_MENU)
			gGlobal.g_pHawkerProcess->UpdateOwner();

		if (gGlobal.g_pOnlineShopProcess)
			gGlobal.g_pOnlineShopProcess->UpdateYuanBao();
		if (gGlobal.g_pMenuSellYBProcess)
			gGlobal.g_pMenuSellYBProcess->UpdateOwnerYBInfo();
	}
}

void CCharacter::OnUpdateMoney(const UInt32 &money)
{
	if (pCharView == NULL)
		return;

	if (pCharView == gGlobal.g_pCharacter)
	{
		if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_CHAR_MENU)
			gGlobal.g_pItemProcess->NewUpdateItemCharacter();
		if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_CHAR_MENU)
			gGlobal.g_pHawkerProcess->UpdateOwner();
		gGlobal.g_pHawkerProcess->UpdateUserMoneyDisplay();
		gGlobal.g_pSkillProcess->RefreshShopDetail();
		gGlobal.g_pMenuSellYBProcess->UpdateOwnerYBInfo();
		
	}

}

void CCharacter::OnUpdateBankMoney(const UInt32 &bankMoney)
{
	if (pCharView == NULL)
		return;

	if (pCharView == gGlobal.g_pCharacter &&
		gGlobal.g_pSkillProcess->GetIsShop())
	{
		gGlobal.g_pSkillProcess->RefreshShopDetail();
	}
}

RPCResult CCharacter::CB_GivenProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex)
{
	if (success)
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseGivenPanel", String());		
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "PlayFinishGivenEffect", String());
		//gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(message.c_str()),CHANNELTYPE_SYSTEM);
	}
	else
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
		//gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(message.c_str()),CHANNELTYPE_SYSTEM);
		String text;
		switch(msgIndex)
		{
		case GIVEN_RUNNING_SCRIPT_CANT_GIVEN:
			text = _T("MSG_SCRIPT_GIVEN_CANT");
			break;
		case GIVEN_MYSELF:
			text = _T("MSG_GIVEN_MYSELF");
			break;
		case GIVEN_CHAR_FAIL:
			text = _T("MSG_GIVEN_CHAR_FAIL");
			break;
		case GIVEN_BLOCK:
			text = _T("MSG_BLOCK_GIVEN");
			break;
		case GIVEN_TRADING_CANT_GIVEN:
			text = _T("MSG_GIVEN_TRADING_FAIL");
			break;
		case GIVEN_OFFLINE_HAWKING_CANT_GIVEN:
			text = _T("MSG_HAWK_DISCONNECT_HAWKING_CANT_GIVEN");
			break;
		case GIVEN_DIS_FAIL:
			text = _T("MSG_GIVEN_DIS_FAIL");
			break;
		case GIVEN_GIVE_NOTHING:
			text = _T("MSG_GIVEN_GIVE_NOTHING");
			break;
		case GIVEN_ITEM_STACK_FAIL:
			text = _T("MSG_GIVEN_STACK_FAIL");
			break;
		case GIVEN_ITEM_BOUND:
			text = _T("MSG_GIVEN_ITEM_BOUND");
			break;
		case GIVEN_ITEM_LOCK:
			text = _T("MSG_GIVEN_ITEM_LOCK");
			break;
		case GIVEN_ITEM_NO_SLOT:
			text = _T("MSG_GIVEN_SLOT_FAIL");
			break;
		case GIVEN_PARTNER_FAIL:
			text = _T("MSG_GIVEN_PARTNER_FAIL");
			break;
		case GIVEN_PARTNER_BOUND:
			text = _T("MSG_GIVEN_PARTNER_BOUND");
			break;
		case GIVEN_PARTNER_LOCK:
			text = _T("MSG_GIVEN_PARTNER_LOCK");
			break;
		case GIVEN_PET_FAIL:
			text = _T("MSG_GIVEN_PET_FAIL");
			break;
		case GIVEN_PET_BOUND:
			text = _T("MSG_GIVEN_PET_BOUND");
			break;
		case GIVEN_PET_LOCK:
			text = _T("MSG_GIVEN_PET_LOCK");
			break;
		case GIVEN_MONEY_FAIL:
			text = _T("MSG_GIVEN_MONEY_FAIL");
			break;
		case GIVEN_FLOWER_NOT_MINE:
			text = _T("MSG_GIVEN_FLOWER_NOT_MINE");
			break;
		case GIVEN_FLOWER_NOT_SEX_WOMAN:
			text = _T("MSG_GIVEN_FLOWER_NOT_SEX_WOMAN");
			break;
		case GIVEN_FLOWER_NOT_SEX_MAN:
			text = _T("MSG_GIVEN_FLOWER_NOT_SEX_MAN");
			break;
		case GIVEN_FLOWER_NOT_ENOUGH_FRIENDLY:
			text = _T("MSG_GIVEN_FLOWER_NOT_ENOUGH_FRIENDLY");
			break;
		case GIVEN_FLOWER_FULL_FRIENDLY:
			text = _T("MSG_GIVEN_FLOWER_FULL_FRIENDLY");
			break;
		}
		if (text.length() > 0)
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(text.c_str()),CHANNELTYPE_SYSTEM);
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenItem(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32Vector &itemID, const UInt8Vector &itemStacks)
{
	if (itemID.size() > 0)
	{
		String msg;
		if (GivenBy)
			msg.Format(_T("%s%s"), targetName.c_str(), gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_FROM")));
		else
			msg.Format(_T("%s%s"), gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_TO")), targetName.c_str());

		for (UInt8 i = 0; i < itemID.size(); ++i)
		{
			const ItemData* pItemData = gGlobal.m_resource->GetItemData(itemID[i]);
			if (pItemData)
				msg.Format(_T("%s%d%s%s,"), msg.c_str(), itemStacks[i], gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_ITEM")), String(pItemData->name).c_str());
		}
	
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenPartner(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PartnerNames)
{
	if (PartnerNames.size() > 0)
	{
		String msg;
		if (GivenBy)
		{
			msg.Format(_T("%s%s"),targetName.c_str(), gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE1")));
			for (UInt8 i = 0; i < PartnerNames.size(); ++i)
				msg.Format(_T("%s%s,"), msg.c_str(), PartnerNames[i].c_str());
		
			msg.Format(_T("%s%s"), msg.c_str(), gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE2")));
		}
		else
		{
			msg.Format(_T("%s"), gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE3")));
			for (UInt8 i = 0; i < PartnerNames.size(); ++i)
				msg.Format(_T("%s%s,"), msg.c_str(), PartnerNames[i].c_str());
		
			msg.Format(_T("%s%s%s"), msg.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE4")), targetName.c_str());
		}
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenPet(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PetNames)
{
	if (PetNames.size() > 0)
	{
		String msg;
		if (GivenBy)
		{
			msg.Format(_T("%s%s"),targetName.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PET_GIVE1")));
			for (UInt8 i = 0; i < PetNames.size(); ++i)
				msg.Format(_T("%s%s,"), msg.c_str(), PetNames[i].c_str());
		
			msg.Format(_T("%s%s"), msg.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE2")));
		}
		else
		{
			msg.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PET_GIVE3")));
			for (UInt8 i = 0; i < PetNames.size(); ++i)
				msg.Format(_T("%s%s,"), msg.c_str(), PetNames[i].c_str());
		
			msg.Format(_T("%s%s%s"), msg.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_PARTNER_GIVE4")), targetName.c_str());
		}
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenMoney(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32 &gmoney)
{
	if (gmoney > 0)
	{
		String msg;
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_MONEY")), gmoney);
		if (GivenBy)
			msg.Format(_T("%s%s%s"), targetName.c_str(),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_FROM")), msg.c_str());
		else
			msg.Format(_T("%s%s%s"),gGlobal.GetStringTable()->Get(_T("MSG_GIVEN_TO")), targetName.c_str(), msg.c_str());
	
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpdateShortCutSuccess(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SaveSystemSetting(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_TradingMessage(LPCPEER pPeer, RPCContext &context, const Boolean &trading, const UInt8 &msgIndex)
{
	String message;

	switch(msgIndex)
	{
	case TRADING_FAIL_SCRIPT:			message = _T("MSG_SCRIPT_TRADING_CANT");	break;
	case TRADING_FAIL_CHAR_NOT_FIND:	message = _T("MSG_TRADING_CHAR_FAIL");		break;
	case TRADING_FAIL_MYSELF:			message = _T("MSG_TRADING_MYSELF");			break;
	case TRADING_FAID_BLOCK:			message = _T("MSG_BLOCK_TRADING");			break;
	case TRADING_FAIL_BATTLE:			message = _T("MSG_TRADING_BATTLE");			break;
	case TRADING_FAIL_HAWKING:			message = _T("MSG_HAWK_DISCONNECT_HAWKING_CANT_TRADING");	break;
	case TRADING_FAIL_ALREADY_TRADING:	message = _T("MSG_TRADING_PLEASE_WAIT");	break;
	case TRADING_FAIL_TARGET_ALREADY_TRADING:	message = _T("MSG_TRADING_TARGET_BUSY");	break;
	case TRADING_FAIL_REQ_TIMEOUT:		message = _T("MSG_TRADING_REQ_TIMEOUT");	break;
	case TRADING_FAIL_REQ_CANCEL:		message = _T("MSG_TRADING_REQ_CANCEL");		break;
	case TRADING_CONFIRM:				message = _T("MSG_TRADING_CONFIRM");		break;
	case TRADING_FAIL_DISTANCE_TOO_FAR: message = _T("MSG_TRADING_DIS_FAIL");		break;
	case TRADING_FAIL_GUILD_WAR: message = _T("MSG_TRADING_FAIL_GUILD_WAR");		break;
	case TRADING_FAIL_NOT_SUIT_MAP: message = _T("MSG_TRADING_FAIL_NOT_SUIT_MAP");		break;
		
	}

	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(message.c_str()));
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &askId)
{
	if (gGlobal.g_pTradingProcess)
	{
		gGlobal.g_pTradingProcess->SetTargetNetID(askId);
		gGlobal.g_pTradingProcess->OpenRequestTradingBox();
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade)
{
	if (!trade && gGlobal.g_pTradingProcess)
	{
		gGlobal.g_pTradingProcess->ResetTargetNetID();
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseReqTrading", String());
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_TradingObject(LPCPEER pPeer, RPCContext &context, NetObject* trading)
{

	if (gGlobal.g_pTrading)
		return RPC_RESULT_OK;
	
	CTrading* pTrading = CastNetObject<CTrading>(trading);
	gGlobal.g_pTrading = pTrading;
	
	if (gGlobal.g_pTradingProcess)
		gGlobal.g_pTradingProcess->OpenTradingPanel();


	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const bool &bresult)
{
	CMapNPCControl * pNpcCtrl = QGLApplication::GetInstance().FindNPCCtrlByID(npc_id);
	if (pNpcCtrl)
		pNpcCtrl->OnCB_ClickOnCtrl(bresult);
	else
	{
		if( !bresult )
		{
			// release Player even if NPC does not exist.
			if ( gGlobal.g_CursorState->GetStateID() == ST_APP_MAP_TALKING )
			{
				if ( gGlobal.g_CursorState->IsBlocked() )
					gGlobal.g_CursorState->Unblock();
				State::ChangeState(gGlobal.g_CursorState, SafeCreateObject(CMapUINormalState));
			}
			gGlobal.g_pScriptControl->SetFinish();

			if ( gGlobal.g_pScriptControl ) 
			{
				gGlobal.g_pScriptControl->ResetNPCDirection();
				gGlobal.g_pScriptControl->OnLockPosHandle(BOOLEAN_FALSE);
			}
			String Msg =gGlobal.GetStringTable()->Get(_T("MSG_SCRIPT_NPCNOTREPLY"));
			gGlobal.PrintPlayerMessage(Msg);
		}
	}

	return RPC_RESULT_OK; 
}

RPCResult CCharacter::CB_TitleListUpdate(LPCPEER pPeer, RPCContext &context, const UInt16Vector &title_id_list, const StringVector &title_string_list, const UInt8Vector &updateState)
{
	for (UInt i = 0; i < title_id_list.size(); i++)
	{
		if (i >= title_string_list.size() ||
			i >= updateState.size())
			break;
		if (updateState[i] == TITLE_STATE_ADD)
		{
			gGlobal.g_pMenuTitleProcess->m_titleDataLst.insert(std::make_pair(title_id_list[i], title_string_list[i]));
		}
		else if (updateState[i] == TITLE_STATE_REMOVE)
		{
			StlMap<UInt16, String>::iterator Itr = gGlobal.g_pMenuTitleProcess->m_titleDataLst.find(title_id_list[i]);
			if (Itr != gGlobal.g_pMenuTitleProcess->m_titleDataLst.end())
				gGlobal.g_pMenuTitleProcess->m_titleDataLst.erase(Itr);
		}
		else if (updateState[i] == TITLE_STATE_UPDATE)
		{
			StlMap<UInt16, String>::iterator Itr = gGlobal.g_pMenuTitleProcess->m_titleDataLst.find(title_id_list[i]);
			if (Itr != gGlobal.g_pMenuTitleProcess->m_titleDataLst.end())
				Itr->second = title_string_list[i];
		}
	}

	gGlobal.g_pMenuTitleProcess->RefreshMenu();
	return RPC_RESULT_OK;
}



RPCResult CCharacter::CB_ReportAntiHack(LPCPEER pPeer, RPCContext &context, const Boolean &is_right, const UInt8 &cwrong, const UInt16 &reward)
{
	gGlobal.g_pAntiHack->CloseAntiHackPanel();
	String msg;
	if (is_right)//Server got the correct answer of Anti-Hack question
	{
		msg = gGlobal.GetStringTable()->Get(_T("MSG_HACK_CORRECT_MSG"));		
		String awardMsg;
		if (reward == 1)
			awardMsg = gGlobal.GetStringTable()->Get(_T("MSG_HACK_REWARD_ONE"));
		else if (reward == 2)
			awardMsg = gGlobal.GetStringTable()->Get(_T("MSG_HACK_REWARD_TWO"));
		msg.Format(_T("%s %s"), msg.c_str(), awardMsg.c_str());
	}
	else
	{   //Server got the wrong answer of Anti-Hack question
		int chance = 3 - cwrong;
		String wrgMsg = gGlobal.GetStringTable()->Get(_T("MSG_HACK_WRG_NUM"));
		String chcMsg = gGlobal.GetStringTable()->Get(_T("MSG_HACK_CHANCE"));	
		msg.Format(_T("%s:%d  %s:%d"), wrgMsg.c_str(), cwrong, chcMsg.c_str(), chance);
	}
	gGlobal.PrintPlayerMessage(msg);
	
	return RPC_RESULT_OK;	
 }

RPCResult CCharacter::CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray & image, const AntiHackData &data)
{ 
	gGlobal.g_context = context;
	gGlobal.g_pAntiHack->OpenAntiHackPopUp(const_cast<ByteArray&>(image), const_cast<AntiHackData&>(data));
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AssetProtectResult(LPCPEER pPeer, RPCContext &context, const UInt8 &resultMsg, const UInt32 &index, const UInt8 &type)
{

	if ( (resultMsg == ASSET_RESET_PASS) ||  (resultMsg == ASSET_INIT_PASS) )
	{
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenAssetLockPanel", String());
		UIChatPanel* pChatPanel = gGlobal.pUIManager->FindUIChatPanel("AssetLockTip");
		if (pChatPanel)
		{
			pChatPanel->ClearAllRow();
			String tips  = gGlobal.GetStringTable()->Get(_T("MSG_ASSET_LOCK_TIPS2"));		
			pChatPanel->AddStringW(tips.c_str());	
			pChatPanel->EndRow();
			gGlobal.pUIManager->AddDirtyUIItem(pChatPanel); 	
		}

	}
	else if (resultMsg == ASSET_LOCK_SUCCESS)
	{
		String msg;
		switch(type)
		{
		case ASSET_ITEM:
            msg = _T("MSG_ITEM_LOCK_SUCCESS");
			break;
		case ASSET_PARTNER:
			msg = _T("MSG_PARTNER_LOCK_SUCCESS");
			break;
		case ASSET_PET:
			msg = _T("MSG_PET_LOCK_SUCCESS");
			break;
		case ASSET_BANKMONEY:
			msg = _T("MSG_BANKMONEY_LOCK_SUCCESS");
			break;
		case ASSET_YUANBAO:
			msg = _T("MSG_YUANBAO_LOCK_SUCCESS");
			break;
		}
		if (!msg.empty())
            gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(msg.c_str()));
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AssetReleaseLockResult(LPCPEER pPeer, RPCContext &context, const UInt8 &resultMsg, const UInt32 &index, const UInt8 &type)
{

 	if (resultMsg == ASSET_UNLOCK_SUCCESS)
	{
		String msg;	
		switch(type)
		{
		case ASSET_ITEM:
            msg = _T("MSG_ITEM_UNLOCK_SUCCESS");
			break;
		case ASSET_PARTNER:
			msg = _T("MSG_PARTNER_UNLOCK_SUCCESS");
			break;
		case ASSET_PET:
			msg = _T("MSG_PET_UNLOCK_SUCCESS");
			break;
		case ASSET_BANKMONEY:
			msg = _T("MSG_BANKMONEY_UNLOCK_SUCCESS");
			break;
		case ASSET_YUANBAO:
			msg = _T("MSG_YUANBAO_UNLOCK_SUCCESS");
			break;
		}
		if (!msg.empty())
            gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(msg.c_str()));
		gGlobal.g_pItemProcess->m_bUnlockPassEnter = BOOLEAN_FALSE;	
	}
	else if (resultMsg == ASSET_PASSWORD_INVALID)
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_ITEM_LOCK_PASSWORD_INVALID")));	
		gGlobal.g_pItemProcess->m_bUnlockPassEnter = BOOLEAN_FALSE;		
	}
	else if (resultMsg == ASSET_PASSWORD_UNVERIFIED)
	{
		gGlobal.g_pItemProcess->OpenUnlockItemPanel(index, type);	
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	String resultMsg;
	if (success)
		resultMsg = gGlobal.GetStringTable()->Get(_T("MSG_ASSET_LOCKPASS_SUCCESS"));
	else
		resultMsg = gGlobal.GetStringTable()->Get(_T("MSG_ASSET_SET_LOCKPASS_FAIL"));

	gGlobal.PrintPlayerMessage(resultMsg);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &bSuccess)
{

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_DepositInBank(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	String deptMsg;
	if (success)
		deptMsg = gGlobal.GetStringTable()->Get(_T("MSG_BANK_DEPOSIT_SUCCESS"));	
	else
		deptMsg = gGlobal.GetStringTable()->Get(_T("MSG_BANK_DEPOSIT_FAIL"));

	gGlobal.PrintPlayerMessage(deptMsg);	

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{
	String withdrwMsg;
	if (success)
		withdrwMsg = gGlobal.GetStringTable()->Get(_T("MSG_BANK_WITHDRAW_SUCCESS"));
	else
		withdrwMsg = gGlobal.GetStringTable()->Get(_T("MSG_BANK_WITHDRAW_FAIL"));

	gGlobal.PrintPlayerMessage(withdrwMsg);	

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PlyInterestDwn(LPCPEER pPeer, RPCContext &context, const UInt8 &interest)
{
	String Msg;	
	if (interest == 1)
        Msg =gGlobal.GetStringTable()->Get(_T("MSG_PLY_INTST_DWN_TIRED"));
	else if (interest == 0)
        Msg =gGlobal.GetStringTable()->Get(_T("MSG_PLY_INTST_DWN_EXHUAST"));

    gGlobal.PrintMessage(Msg, CHANNELTYPE_NOTICE);
	gGlobal.PrintPlayerMessage(Msg);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg)
{
	String addictMsg;  
	addictMsg = gGlobal.GetStringTable()->Get(warnMsg.c_str());	
	gGlobal.PrintPlayerMessage(addictMsg);	
	gGlobal.PrintMessage(addictMsg, CHANNELTYPE_SYSTEM);	
	return RPC_RESULT_OK;

}
RPCResult CCharacter::CB_AddictCurrentStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg)
{
	UIImage* gdHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThgood");
	UIImage* avgeHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThaverage");
	UIImage* poorHealthIcon = gGlobal.pUIManager->FindUIImage("PlayerHeALThpoor");
	if ((gdHealthIcon) && (avgeHealthIcon) && (poorHealthIcon))
	{
		gdHealthIcon->SetEnable(false);	
		avgeHealthIcon->SetEnable(false);	
		poorHealthIcon->SetEnable(false);		

		if (statusMsg == _T("MSG_GAME_HEALTH_GOOD"))
		{
			gdHealthIcon->SetEnable(true);
		
		}
		else if (statusMsg == _T("MSG_GAME_HEALTH_AVG"))
		{
			avgeHealthIcon->SetEnable(true);
		}
		else if (statusMsg == _T("MSG_GAME_HEALTH_BAD"))
		{
			poorHealthIcon->SetEnable(true);
		}
		gGlobal.pUIManager->AddDirtyUIItem(gdHealthIcon);	
		gGlobal.pUIManager->AddDirtyUIItem(avgeHealthIcon);	
		gGlobal.pUIManager->AddDirtyUIItem(poorHealthIcon);		
	}

	return RPC_RESULT_OK;

}
RPCResult CCharacter::CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const UInt32 &accOffTime)
{
	UInt32 minute = 60;
	UInt32 hour = accOffTime/(60*minute);
	UInt32 n = accOffTime - (hour * 60*minute);
	UInt32 min = n/minute;

	String addictMsg = gGlobal.GetStringTable()->Get(_T("MSG_GAME_ADDICTION_FIVE_MIN_WARN"));	
	String hrStr = gGlobal.GetStringTable()->Get(_T("MSG_GAME_ADDICTION_HR"));
    String minStr = gGlobal.GetStringTable()->Get(_T("MSG_GAME_ADDICTION_MIN"));
	String timeStr;
	timeStr.Format(_T("%d%s%d%s"), hour, hrStr.c_str(), min, minStr.c_str());
	addictMsg.Format(_T("%s%s"), addictMsg.c_str(), timeStr.c_str());
	gGlobal.PrintPlayerMessage(addictMsg);
	gGlobal.PrintMessage(addictMsg, CHANNELTYPE_SYSTEM);	


	return RPC_RESULT_OK;

}

//void CCharacter::OnUpdateTitle(const String &title)
void CCharacter::OnUpdateTitle(const String &title, const UInt16 &title_id)
{
#ifdef QGL
	if (pCharView)
		pCharView->OnUpdateTitle(title, title_id);
	if( gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.g_pCharProcess->UpdateCharTitle();
#endif
}

RPCResult CCharacter::CB_OpenCreateGuild(LPCPEER pPeer, RPCContext &context)
{
	#ifdef QGL
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenGuildNameInput", String());
	#endif
		return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_StartHawk(LPCPEER pPeer, RPCContext &context, NetGroup* hawkGroup, const UInt8 &successIndex)
{
#ifdef QGL
	if (successIndex == HAWK_START)
	{
		CHawkGroup * pHawkGroup = CastNetObject<CHawkGroup>(hawkGroup);

		if (gGlobal.g_pMyHawkGroup && gGlobal.g_pMyHawkGroup != hawkGroup)
				DeleteNetGroup(gGlobal.g_pMyHawkGroup, BOOLEAN_TRUE);
		gGlobal.g_pMyHawkGroup = pHawkGroup;
		gGlobal.g_pMyHawkGroup->ConstructHawkItem();

		UIButton* button = gGlobal.pUIManager->FindUIButton("HawkerOwnerHawk");
		UIButton* button1 = gGlobal.pUIManager->FindUIButton("HawkerOwnerStopHawk");
		UIButton* button2 = gGlobal.pUIManager->FindUIButton("HawkerOwnerChangeName");

		if (button && button1 && button2)
		{
			button->SetEnable(false);
			button1->SetNormalState();
			button1->SetEnable(true);

			button2->SetNormalState();
			gGlobal.pUIManager->AddDirtyUIItem(button2);
		}
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseHawkerOwner", String());
		gGlobal.g_pHawkerProcess->ResumeHawkerOwner();
	}
	else
	{
		UIButton* button = gGlobal.pUIManager->FindUIButton("HawkerOwnerHawk");
		if (button)
		{
			button->SetNormalState();
			gGlobal.pUIManager->AddDirtyUIItem(button);
		}
		DeleteNetGroup(hawkGroup, BOOLEAN_TRUE);
		gGlobal.g_pHawkerProcess->SetStartHawkFlag(false);

		if (successIndex == HAWK_NO_TIME)			
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_NO_TIME1")));
		else if (successIndex == HAWK_NOT_RIGHT_PLACE)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_NOT_IN_HAWKER_ZONE")));
		else if (successIndex == HAWK_PLACE_HAVE_HAWKER)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSE_HAWK_PLACE_HAVE_HAWKER")));
		else if (successIndex == HAWK_CANT_HAVE_PARTY)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_PARTY_CANT_HAWKING")));
		else if (successIndex == HAWK_ITEM_LOCK)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_ITEM_LOCK")));	
		else if (successIndex == HAWK_PET_LOCK)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_PET_LOCK")));	
		else if (successIndex == HAWK_PARTNER_LOCK)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_PARTNER_LOCK")));
		else if (successIndex == HAWK_ITEM_FLOWER_MODEL)
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_HAWK_ITEM_FLOWER_MODEL")));
	}
#endif
	return RPC_RESULT_OK;
}

	
RPCResult CCharacter::CB_GetHawk(LPCPEER pPeer, RPCContext &context, const Boolean &self, NetGroup* hawkGroup, 
								 NetGroup* itemGroup, NetGroup* partnerGroup, const UInt16Vector &partnerSkillID, 
								 NetGroup* petGroup, const UInt16Vector &petSkillID)
{

	if (!self)
	{
		if (gGlobal.g_pTargetHawkGroup && gGlobal.g_pTargetHawkGroup != hawkGroup)
			DeleteNetGroup(gGlobal.g_pTargetHawkGroup, BOOLEAN_TRUE);

		CHawkGroup * pHawkGroup = CastNetObject<CHawkGroup>(hawkGroup);
		gGlobal.g_pTargetHawkGroup = pHawkGroup;
		gGlobal.g_pTargetHawkGroup->ConstructHawkItem(itemGroup, partnerGroup, partnerSkillID, petGroup, petSkillID);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenHawk", String());
		gGlobal.g_pHawkerProcess->UpdateHawkerTitle();
		gGlobal.g_pHawkerProcess->UpdateUserMoneyDisplay();
		DeleteNetGroup(itemGroup);
		DeleteNetGroup(partnerGroup);
		DeleteNetGroup(petGroup);

		gGlobal.g_pCharacter->Stop();
	}
	else //for resume
	{
		if (gGlobal.g_pMyHawkGroup && gGlobal.g_pMyHawkGroup != hawkGroup)
			DeleteNetGroup(gGlobal.g_pMyHawkGroup, BOOLEAN_TRUE);

		CHawkGroup * pHawkGroup = CastNetObject<CHawkGroup>(hawkGroup);
		gGlobal.g_pMyHawkGroup = pHawkGroup;

		gGlobal.g_pMyHawkGroup->ConstructHawkItem();

		if (gGlobal.g_pHawkerProcess)
			gGlobal.g_pHawkerProcess->ResumeHawkerOwner();

		DeleteNetGroup(itemGroup, BOOLEAN_TRUE);
		DeleteNetGroup(partnerGroup, BOOLEAN_TRUE);
		DeleteNetGroup(petGroup, BOOLEAN_TRUE);
	}

	return RPC_RESULT_OK;
}

void CCharacter::OnUpdateHawkInfo(const String &hawkName)
{
#ifdef QGL
	StringA panelName;
	panelName.Format("Hawker%dPanel", GetChar_id());
	if (pCharView)
	{
		if (!hawkName.Equals(_T("")) )
		{
			UIPanel* panel = gGlobal.pUIManager->FindUIPanel(panelName.c_str());
			if (panel == NULL)
			{
				panel = SafeCreateObject(UIPanel);
				gGlobal.pUIManager->AddUIID(panelName.c_str(), panel);
				
				panel->SetName(panelName.c_str());
				panel->SetNotDetected(false);
				panel->SetTextureWH(139, 32);
				panel->SetWH(139, 32);

				panel->SetDetectRect();
				panel->SetTexture(gGlobal.pUIManager->GetRender()->GetDevice());
				panel->SetLuaState(gGlobal.pUIManager->GetLuaState());
				RECT realRect;
				panel->SetRealRect(realRect);
				
				panel->Render(NULL);
				gGlobal.pUIManager->AddUIPanel(panel);

				UIImage* bgImage = SafeCreateObject(UIImage);
				StringA imgName;
				imgName.Format("HawkBgImage%d", GetChar_id());
				gGlobal.pUIManager->AddUIID(imgName.c_str(), bgImage);
				bgImage->SetName(imgName.c_str());
				bgImage->SetEnable(true);
				bgImage->SetNotDetected(false);
				bgImage->SetDetectImage(true);
				bgImage->SetTimeOutCount(0);
				String Filename = _T("Data\\interface0033_Hawker.tga");
				CImage* img = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);

				bgImage->SetImage(img, 470, 475, 609, 507);

				bgImage->SetOffsetXY(0, 0);
				bgImage->SetDetectRect();
				bgImage->SetLuaState(gGlobal.pUIManager->GetLuaState());
				bgImage->SetLuaFunc(UI_FUNC_RBUTTON_UP, "HawkClick");
				panel->AddUIItem(bgImage);


				UIStaticText* hawkNameText = SafeCreateObject(UIStaticText);

				StringA textName;
				textName.Format("HawkText%d", GetChar_id());
				gGlobal.pUIManager->AddUIID(textName.c_str(), hawkNameText);
				hawkNameText->SetName(textName.c_str());
				hawkNameText->SetEnable(true);
				hawkNameText->SetNotDetected(true);
				hawkNameText->SetDetectImage(false);
				hawkNameText->SetTimeOutCount(0);

				StringA fontName = "Data\\simsun.ttc";
				hawkNameText->SetOffsetPos(0, 0);
				hawkNameText->SetFont(fontName.c_str(), 14, 14);
				hawkNameText->SetAlign(UIStaticText::ALIGN_CENTER);
				hawkNameText->SetLinePitch(0);
				hawkNameText->SetText(hawkName.c_str());

				hawkNameText->SetFontStyle(EDGE_SHADOW);//EDGE_LIGHT);
				hawkNameText->SetFontColor(TextColor[ColorIndex_White]);//Black]);
				UInt32 shadowColor = HexToDWORD("FF000000");
				hawkNameText->SetShadowColor(shadowColor);
				hawkNameText->SetOffsetXY(0, 4);

				hawkNameText->SetImageWH(bgImage->GetWidth(), 16);
				hawkNameText->SetLuaState(gGlobal.pUIManager->GetLuaState());
				hawkNameText->DrawFont();
				if (gGlobal.g_pHawkerProcess)
				{
					if (gGlobal.g_pHawkerProcess->IsConcernTarget(GetChar_id()))
					{
						Color32 VermeilColor = D3DCOLOR_ARGB(255, 240, 133, 62);
						bgImage->SetColor(TextColor[ColorIndex_Yellow]);
						hawkNameText->SetColor(VermeilColor);
					}
				}

				panel->AddUIItem(hawkNameText);	

				Int32 posX = pCharView->GetCharMainView()->GetEntPixelPos().PixelPosX - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().x - panel->GetWidth() / 2;
				Int32 posY = pCharView->GetCharMainView()->GetEntPixelPos().PixelPosY - (Int32)QGLApplication::GetInstance().GetRenderCamera()->GetLeftUpPos().y - panel->GetHeight() - 100;
				panel->SetPos( posX, posY);
				
				gGlobal.pUIManager->SetPanel2Bottom(panel);
				if (gGlobal.g_pHawkerProcess)
				{
					if(!gGlobal.g_pHawkerProcess->GetAllHawkEnable())
					{
						bgImage->SetEnable(false);
						hawkNameText->SetEnable(false);
					}
					gGlobal.g_pHawkerProcess->AddStartHawkInAllList(GetChar_id());
				}
				if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
					panel->SetEnable(true);				
				else
					panel->SetEnable(false);				
			}
			else
			{
				StringA textName;
				textName.Format("HawkText%d", GetChar_id());
				UIStaticText* hawkNameText = gGlobal.pUIManager->FindUIStaticText(textName.c_str());
				if (hawkNameText)
				{
					hawkNameText->SetText(hawkName.c_str());
					hawkNameText->Redraw();
					gGlobal.pUIManager->AddDirtyUIItem(hawkNameText);
				}
			}
		}
		else
		{
			gGlobal.pUIManager->DeletePanel(panelName.c_str());
			pCharView->ResetHawkBubble();
			if (gGlobal.g_pHawkerProcess)
			{
				if (gGlobal.g_pHawkerProcess->IsConcernTarget(GetChar_id()))
				{
					gGlobal.g_pHawkerProcess->ReSetConcernTarget(GetChar_id());
					gGlobal.g_pHawkerProcess->ReSetHawkInAllList(GetChar_id());
				}
			}
		}
	}
#endif
}

RPCResult CCharacter::CB_SetTargetNameFormHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id ,const String &char_name,const Boolean& isonline)
{
	UIStaticText* targetNameText = gGlobal.pUIManager->FindUIStaticText("HawkCharacterUID");
	if(targetNameText)
	{
		String targetName;
		String NameText;
		String temp = char_name;
		if (isonline)
		{
			if(!char_name.Equals(_T("")))
			{
				NameText = gGlobal.GetStringTable()->Get(_T("MSG_HAWK_TARGET_NAME_ONLINE"));
				targetName.Format(NameText.c_str(),char_name.c_str(),char_id);
			}
			else
			{
				NameText = gGlobal.GetStringTable()->Get(_T("MSG_HAWK_TARGET_NO_NAME_ONLINE"));
				targetName.Format(NameText.c_str(),char_id);
			}
		}
		else	
		{
			if(!char_name.Equals(_T("")))
			{
				NameText = gGlobal.GetStringTable()->Get(_T("MSG_HAWK_TARGET_NAME_NO_ONLINE"));
				targetName.Format(NameText.c_str(),char_name.c_str(),char_id);
			}
			else
			{
				NameText = gGlobal.GetStringTable()->Get(_T("MSG_HAWK_TARGET_NO_NAME_NO_ONLINE"));
				targetName.Format(NameText.c_str(),char_id);
			}
		}
		targetNameText->SetText(targetName);
		targetNameText->Redraw();
		gGlobal.pUIManager->AddDirtyUIItem(targetNameText);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_HawkMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	if (msgIndex != HAWK_NO_MESSAGE)
	{
		String msg = _T("");
		switch(msgIndex)
		{
		case HAWK_NO_ITEM:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_NO_ITEM"));
			break;
		case HAWK_NO_MONEY:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_MONEY_FAIL"));
			break;
		case HAWK_ITEM_BIND:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_ITEM_BOUND"));
			break;
		case HAWK_PARTNER_BIND:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_PARTNER_BOUND"));
			break;
		case HAWK_PET_BIND:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_PET_BOUND"));
			break;
		case HAWK_NO_ITEM_SLOT:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_SLOT_FAIL"));
			break;
		case HAWK_NO_PARTNER_SLOT:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_PARTNER_FAIL"));
			break;
		case HAWK_NO_PET_SLOT:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_TRADING_PET_FAIL"));
			break;
		case HAWK_NOT_ENOUGH_LV:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_NOT_ENOUGH_LEVEL"));
			break;
		case HAWK_NOT_RIGHT_PLACE:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_NOT_IN_HAWKER_ZONE"));
			break;
		case HAWK_TIMEUP:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_NO_TIME"));
			break;
		case HAWK_PLACE_HAVE_HAWKER:
			msg =gGlobal.GetStringTable()->Get(_T("MSE_HAWK_PLACE_HAVE_HAWKER"));
			break;
		case HAWK_CANT_HAVE_PARTY:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_PARTY_CANT_HAWKING"));
			break;
		case HAWK_NOT_ENOUGH_HAWKITEM:
			msg =gGlobal.GetStringTable()->Get(_T("MSG_HAWK_ITEM_NO_ENOUNG"));
			break;
		case HAWK_MIXPET_LEVEL_MAX:
			msg = gGlobal.GetStringTable()->Get(_T("MSG_NOT_EATFOOD_PETLEVEL_MAX"));
			break;
		case HAWK_PARTNER_REMOVE:
			msg = gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_TIMEOVER"));
			break;
		case HAWK_ITEM_FLOWER_MODEL:
			msg = gGlobal.GetStringTable()->Get(_T("MSG_HAWK_ITEM_FLOWER_MODEL"));
			break;
		}

		if (msg.length() > 0)
			gGlobal.PrintPlayerMessage(msg.c_str());
	}
	return RPC_RESULT_OK;
}

void CCharacter::OnUpdateHawkerTime(const UInt32 &hawkerTime)
{
	if (gGlobal.g_pHawkerProcess && gGlobal.g_pCharacter && this==gGlobal.g_pCharacter->GetCharNetObject() )
		gGlobal.g_pHawkerProcess->UpdateHawkerTime();
}


RPCResult CCharacter::CB_PartyMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgType, const bool &inviteOrJoin, const String &charName)
{
	switch(msgType)
	{
	case PARTY_MSG_NO_PARTY:
		break;
	case PARTY_MSG_NOT_LEADER_CANT_REPLAY:
		break;
	case PARTY_MSG_MYSELF_IN_BATTLE:
		break;
	case PARTY_MSG_TARGET_NOT_FIND:
		break;
	case PARTY_MSG_TARGET_IN_BATTLE:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_TARGET_IN_BATTLE")), CHANNELTYPE_SYSTEM);
		break;
	case PARTY_MSG_TARGET_NOT_IN_SAME_MAP:
		break;
	case PARTY_MSG_TARGET_HAVE_PARTY:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_TARGET_HAVE_PARTY")), CHANNELTYPE_SYSTEM);
		break;
	case PARTY_MSG_TARGET_NO_PARTY:
		break;
	case PARTY_MSG_TARGET_HAVENOT_REQUEST:
		break;
	case PARTY_MSG_REPLAY_DISTANCE_TOO_FAR:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_DISTANCE_TOO_FAR")), CHANNELTYPE_SYSTEM);
		break;
	case PARTY_MSG_PARTY_FULL:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_FULL")), CHANNELTYPE_SYSTEM);
		break;
	case PARTY_MSG_MYSELF_HAVE_PARTY:
		break;
	case PARTY_MSG_NO_LEADER:
		break;
	case PARTY_MSG_MYSELF_HAVENOT_INVITED:
		break;
	case PARTY_MSG_IREJECT:
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_IREJECT")),charName.c_str());
			gGlobal.PrintPlayerMessage(text.c_str());
		}
		break;
	case PARTY_MSG_JREJECT:
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_JREJECT")),charName.c_str());
			gGlobal.PrintPlayerMessage(text.c_str());
		}
		break;
	case PARTY_MSG_JOIN_FAIL:
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_JOIN_FAIL")),charName.c_str());
			gGlobal.PrintPlayerMessage(text.c_str());
		}
		break;
	case PARTY_MSG_INVITE_FAIL:
		{
			String text;
			text.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_INVITE_FAIL")),charName.c_str());
			gGlobal.PrintPlayerMessage(text.c_str());
		}
		break;
	case PARTY_MSG_PARTY_CANNOT_JOIN_TALK:
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_CANNOT_JOIN_TALK")), CHANNELTYPE_SYSTEM);
		break;
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_OpenGiftBox(LPCPEER pPeer, RPCContext &context, NetGroup* giftBox)
{
	CGiftBox* pGiftBox = CastNetObject<CGiftBox>(giftBox);

	if (gGlobal.g_pGiftBox && gGlobal.g_pGiftBox != pGiftBox)
		DeleteNetGroup(gGlobal.g_pGiftBox, BOOLEAN_TRUE);

	gGlobal.g_pGiftBox = pGiftBox;
	gGlobal.g_pGiftBox->ConstructGiftBox();
	gGlobal.g_pGiftBoxProcess->OpenGiftBoxInterface();

	return RPC_RESULT_OK;
}



void CCharacter::OnUpdateSMoney(const UInt32 &SMoney)
{
	if (pCharView == NULL)
		return;

	if (pCharView == gGlobal.g_pCharacter)
	{
		if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_CHAR_MENU)
			gGlobal.g_pItemProcess->NewUpdateItemCharacter();
		if (gGlobal.g_pHawkerProcess->GetMenuIndex() == ITEM_CHAR_MENU)
			gGlobal.g_pHawkerProcess->UpdateOwner();
		gGlobal.g_pHawkerProcess->UpdateUserMoneyDisplay();
		gGlobal.g_pSkillProcess->RefreshShopDetail();	
	}
}

RPCResult CCharacter::CB_HawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames)
{
	for (Index i = C_INDEX(0); i < historys.size(); ++i)
	{
		gGlobal.g_pHawkerProcess->AddHawkerHistory(historys[i], itemNames[i]);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpdateHawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames, const UInt32Vector &deleteHistoryID)
{
	gGlobal.g_pHawkerProcess->RemoveHawkerHistory(deleteHistoryID);
	
	for (Index i = C_INDEX(0); i < historys.size(); ++i)
	{
		gGlobal.g_pHawkerProcess->AddHawkerHistory(historys[i], itemNames[i], false);
	}

	if (gGlobal.g_pHawkerProcess->GetHawkMenuIndex() == HAWKER_EXCHANGE_RECORD)
		gGlobal.g_pHawkerProcess->UpdateHawkExchangeRecord(0);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UseItemCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &item_id, const UInt32 &SrcID, const UInt32 &TargetID)
{
	CCharacterControl * pSrcCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(SrcID);
	CCharacterControl * pTargetCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(TargetID);
	if( pSrcCtrl == NULL || pTargetCtrl == NULL )
	{
		return RPC_RESULT_OK;
	}

	// make sure that it's displayed inside the screen
	PixelDirection dummy;
	BlockPos center = gGlobal.g_pCharacter->GetCharMainView()->GetCharPos();

	BlockPos SrcPos = pSrcCtrl->GetCharMainView()->GetCharPos();
	BlockPos SrcPosY = SrcPos;
	SrcPosY.PosY = center.PosY;
	BlockPos SrcPosX = SrcPos;
	SrcPosX.PosX = center.PosX;
	if( (QGLMath::CalculateBlockPosDistance(SrcPosY,center,dummy) > 400)
		|| (QGLMath::CalculateBlockPosDistance(SrcPosX,center,dummy) > 300) )
	{
		return RPC_RESULT_OK;
	}

	BlockPos TargetPos = pTargetCtrl->GetCharMainView()->GetCharPos();
	BlockPos TargetPosY = TargetPos;
	TargetPosY.PosY = center.PosY;
	BlockPos TargetPosX = TargetPos;
	TargetPosX.PosX = center.PosX;
	if( (QGLMath::CalculateBlockPosDistance(TargetPosY,center,dummy) > 400)
		|| (QGLMath::CalculateBlockPosDistance(TargetPosX,center,dummy) > 300) )
	{
		return RPC_RESULT_OK;
	}
	// make sure that it's displayed inside the screen end

	const ItemData * pItemData = gGlobal.m_resource->GetItemData(item_id);
	if (pItemData && (pItemData->targetType == 1||pItemData->targetType == 4))
	{
		gGlobal.ShowItemEffectInMap(item_id, SrcID, TargetID);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AskEnterQuestion(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER,_F("CCharacter::CB_AskEnterQuestion"));
#ifdef QGL	
	//gGlobal.g_QuestionBox.ShowMessageBox(MESSAGEBOX_TYPE_YES_NO,gGlobal.GetStringTable()->Get(_T("MSG_QUESTION_ASK_JOIN")), "OpenQuestion");	
	//UIItem * pItem = gGlobal.pUIManager->GetUIItem("OpenQuestionYes");
	//if (pItem)
	//{
	//	pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "OpenQuestion");
	//}
	//pItem = gGlobal.pUIManager->GetUIItem("OpenQuestionNo");
	//if (pItem)
	//{
	//	pItem->SetLuaFunc(UI_FUNC_LBUTTON_UP, "CloseQuestion");
	//}
#endif	
	//EnterQuestion(pPeer,context);
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_EnterQuestion(LPCPEER pPeer, RPCContext &context, NetObject* pQuestion)
{
	TRACE_INFODTL(GLOBAL_LOGGER,_F("CCharacter::CB_EnterQuestion"));
	CQuestion* p = CastNetObject<CQuestion>(pQuestion);
	if(p != NULL)
	{
		CQuestionProcess::GetInstance().SetQuestion(p);
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "EnterQuestion", String());
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenQuestionPanel", String());
	}
	
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ExitQuestion(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER,_F("CCharacter::CB_ExitQuestion"));
	CQuestionProcess::GetInstance().OnCB_ExitQuestion();
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ExitQuestion", String());
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AnswerQuestion(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt32 &CharID)
{
	TRACE_INFODTL(GLOBAL_LOGGER,_F("CCharacter::CB_AnswerQuestion"));
	CCharacterControl * pCharacterControl = QGLApplication::GetInstance().FindCharCtrlByChar_id(CharID);
	if( pCharacterControl == NULL )
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER,_F("CCharacter::CB_AnswerQuestion pCharacterControl == NULL"));
		return RPC_RESULT_OK;
	}

	String strIcon;
	switch(index)
	{
	case 0:
		pCharacterControl->RemoveIcon();
		return RPC_RESULT_OK;
	case 1:
		strIcon = gConfigure.HeadIconName[QUESTION_ICON_A];
		break;
	case 2:
		strIcon = gConfigure.HeadIconName[QUESTION_ICON_B];
		break;
	case 3:
		strIcon = gConfigure.HeadIconName[QUESTION_ICON_C];
		break;
	case 4:
		strIcon = gConfigure.HeadIconName[QUESTION_ICON_D];
		break;

	default:
		TRACE_ERRORDTL_1(GLOBAL_LOGGER,_F("CCharacter::CB_AnswerQuestion invalid index:%d"),index);
		return RPC_RESULT_OK;
	}

	pCharacterControl->GetHeadIconView()->InitEntPicInfo();
	pCharacterControl->GetHeadIconView()->LoadEntityFile(strIcon.c_str(), gGlobal.pD3DDevice);
	pCharacterControl->GetHeadIconView()->SetOffsetType(ENTITYOFFSETTYPE_HEAD);
	pCharacterControl->GetHeadIconView()->StopEntity();

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_OpenWareHouse(LPCPEER pPeer, RPCContext &context, NetGroup* items, NetGroup* partners, NetGroup* pets, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup)
{
	gGlobal.g_pWareHouseProcess->InitWareHouse(items, partners, pets, partnerSkillGroup, petSkillGroup);
	gGlobal.g_pWareHouseProcess->OpenWareHouse();
	DeleteNetGroup(items);
	DeleteNetGroup(partners);
	DeleteNetGroup(pets);
	DeleteNetGroup(partnerSkillGroup);
	DeleteNetGroup(petSkillGroup);
	return RPC_RESULT_OK;
}

void CCharacter::OnUpdateAvailWareHouse(const UInt8 &availItemWareHouse, const UInt8 &availPartnerWareHouse, const UInt8 &availPetWareHouse)
{
	gGlobal.g_pWareHouseProcess->UpdateWareHouse();
}

void CCharacter::OnUpdateYuanbaoLock(const Boolean &yuanbaoLock)
{
	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_CHAR_MENU)
		gGlobal.g_pItemProcess->NewUpdateItemCharacter();

}

void CCharacter::OnUpdateBankLock(const Boolean &bankLock)
{
	if (gGlobal.g_pItemProcess && gGlobal.g_pItemProcess->GetMenuIndex() == ITEM_CHAR_MENU)
		gGlobal.g_pItemProcess->NewUpdateItemCharacter();	

}

RPCResult CCharacter::CB_DonateResult(LPCPEER pPeer, RPCContext &context, const UInt32 &donate, const UInt8 &returnFlag, const UInt32 &donateMoney)
{
	#ifdef QGL
	if (donate > 0)
	{
		String Msg;
		Msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_DONATE_SUCCESS")), donate);
		gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM);
	}
	
	if (returnFlag == GUILD_DONATE_RESULT_FAIL_ONCE_ADAY)
	{		
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_DONATE_FAIL_ONCE_ADAY")), CHANNELTYPE_SYSTEM);
	}
	else if (returnFlag == GUILD_DONATE_RESULT_FAIL_MONEY_ENOUGH)
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_DONATE_MONEY_ENOUGH")), CHANNELTYPE_SYSTEM);
	if (donateMoney > 0)
	{
		String Msg;
		Msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GUILD_DONATE_MONEY")), donateMoney);
		gGlobal.PrintMessage(Msg, CHANNELTYPE_SYSTEM);
	}
	#endif
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_SetRobot(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot, 
								  const String &robotScript)
{
	if (bRobot)
	{
		CCrossMapPathFinder* pcmpf = CCrossMapPathFinder::GetInstance();
		if (NULL != pcmpf)
		{
			pcmpf->DestroyPathData();
		}
		gGlobal.m_u32DestNpcId = 0;
		gGlobal.InitRobotLua(robotScript);
	}
	else
	{
		if (gGlobal.g_pRobotLua)
		{
			String result;
			int r = CallLuaFunc(gGlobal.g_pRobotLua, "ScriptEnd", result);
			if (r != 0 && r != LUA_YIELD)
			{
				TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FW("lua error: ScriptEnd, %d, %s"),
					r, result.c_str());
				gGlobal.PrintMessage(result, CHANNELTYPE_SYSTEM);
			}
		}
		gGlobal.ReleaseRobotLua();
		// stop moving when robot script off
		if (gGlobal.g_pCharacter && gGlobal.g_pCharacter->IsLeader())
			gGlobal.g_pCharacter->Stop();
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RobotMapAction(LPCPEER pPeer, RPCContext &context, 
										const String &func, const RPCNetID &mapNetId, 
										const RPCNetID &charNetId, const RPCNetID &otherNetId)
{
	if (gGlobal.g_pRobotLua == NULL)
		return RPC_RESULT_FAIL;

	CMap* pMap = FindNetObject<CMap>(mapNetId);
	CCharacter* pChar = FindNetObject<CCharacter>(charNetId);
	if (pMap == NULL)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FW("map netId %d:%d not found"), 
			mapNetId.srvId, mapNetId.objId);
	}
	else if (pChar == NULL || gGlobal.g_pCharacter == NULL || 
		gGlobal.g_pCharacter->GetCharNetObject() != pChar)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FW("character netId %d:%d not found"), 
			charNetId.srvId, charNetId.objId);
	}
	else
	{
		String result;
		StringA funcA(func.c_str());
		int r = CallLuaFunc(gGlobal.g_pRobotLua, funcA.c_str(), result, pMap, 
			gGlobal.g_pCharacter);
		if (r != 0 && r != LUA_YIELD)
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FW("lua error: %s, %d, %s"), func.c_str(), 
				r, result.c_str());
			gGlobal.PrintMessage(result, CHANNELTYPE_SYSTEM);
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RobotBattleTurn(LPCPEER pPeer, RPCContext &context, 
										 const String &func,
										 const RPCNetID &battleNetId, const RPCNetID &charNetId,
										 const RPCNetID &batCharNetId, const RPCNetID &partnerNetId)
{
	if (gGlobal.g_pRobotLua == NULL)
		return RPC_RESULT_FAIL;

	CBattle* pBattle = FindNetObject<CBattle>(battleNetId);
	CCharacter* pChar = FindNetObject<CCharacter>(charNetId);
	CBatCharacter* pBatChar = NULL;
	if (batCharNetId.objId != 0)
		pBatChar = FindNetObject<CBatCharacter>(batCharNetId);
	CBatMob* pPartner = NULL;
	if (partnerNetId.objId != 0)
		pPartner = FindNetObject<CBatMob>(partnerNetId);

	if (pBattle == NULL || gGlobal.g_pBattleControl == NULL || 
		gGlobal.g_pBattleControl->GetBattleNetObject() != pBattle)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FW("battle netId %d:%d not found"), 
			battleNetId.srvId, battleNetId.objId);
	}
	else if (pChar == NULL || gGlobal.g_pCharacter == NULL || 
		gGlobal.g_pCharacter->GetCharNetObject() != pChar)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _FW("character netId %d:%d not found"), 
			charNetId.srvId, charNetId.objId);
	}
	else
	{
		String result;
		StringA funcA(func.c_str());
		CCharacterControl* pBatCharCtrl = pBatChar ? 
			gGlobal.g_pBattleControl->FindCharCtrlByBatActor(pBatChar) : NULL;
		CCharacterControl* pPartnerCtrl = pPartner ? 
			gGlobal.g_pBattleControl->FindCharCtrlByBatActor(pPartner) : NULL;
		int r;
		if (pPartnerCtrl)
			r = CallLuaFunc(gGlobal.g_pRobotLua, funcA.c_str(), result, 
				gGlobal.g_pBattleControl, pBatCharCtrl, pPartnerCtrl);
		else if (pBatCharCtrl)
			r = CallLuaFunc(gGlobal.g_pRobotLua, funcA.c_str(), result, 
				gGlobal.g_pBattleControl, pBatCharCtrl);
		else
			r = CallLuaFunc(gGlobal.g_pRobotLua, funcA.c_str(), result, 
				gGlobal.g_pBattleControl, gGlobal.g_pCharacter);
		if (r != 0 && r != LUA_YIELD)
		{
			TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FW("lua error: %s, %d, %s"), func.c_str(), 
				r, result.c_str());
			gGlobal.PrintMessage(result, CHANNELTYPE_SYSTEM);
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_WarningMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Int32 &value, const UInt8 &showIn)
{
	String text;
	switch(msgIndex)
	{
	case WARN_CHARACTER_OVERFLOW_EXP:
		text = gGlobal.GetStringTable()->Get(_T("MSG_CHARACTER_OVERFLOW_EXP"));
		break;
	case WARN_CHARACTER_CHANGE_LINE_FAIL:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANGE_LINE_FAIL"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_CHARACTER_CHANGE_LINE_TIME_LIMIT:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SYS_CANT_CHANGE_LINE_WITHIN_FIVE_MIN"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_CHARACTER_CHANGE_LINE_SUCCESS:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SYS_CHANGE_LINE_SUCCESS"));
		break;
	case WARN_CHAT_CHANGE_ICON_NO_YUANBAO:
		text = gGlobal.GetStringTable()->Get(_T("MSG_CHAT_ICON_CHANGE_NOYUANBAO"));
		break;
	case WARN_PARTY_REGROUP_FAIL:
		text = gGlobal.GetStringTable()->Get(_T("MSG_PARTY_REGROUP_FAIL"));
		break;
	case WARN_PARTY_CANT_CHANGE_LINE:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SYS_HAS_PARTY_CANT_CHANGE_LINE"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_PARTY_CANT_INVITE_MYSELF:
		text = gGlobal.GetStringTable()->Get(_T("MSG_PARTY_CANT_INVITE_MYSELF"));
		break;
	case WARN_TRADING_CANT_CHANGE_LINE:
		text = gGlobal.GetStringTable()->Get(_T("MSG_TRADING_CANT_CHANGE_LINE"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_HAWKING_CANT_CHANGE_LINE:
		text = gGlobal.GetStringTable()->Get(_T("MSG_HAWK_CANT_CHANGE_LINE"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_HAWKING_CANT_PARTY:
		text = gGlobal.GetStringTable()->Get(_T("MSG_HAWK_HAWKING_CANT_PARTY"));
		break;
	case WARN_SYS_LINE_FULL:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SYS_LINE_FULL"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_INSTANCE_FULL:
		text = gGlobal.GetStringTable()->Get(_T("MSG_INSTANCE_FULL"));
		break;
	case WARN_GUILDWAR_CANT_CHANGE_LINE:
		text = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_LINE_FAIL_MAP"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_SPECIFICMAP_CANT_CHANGE_LINE:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SYS_THIS_MAP_CANT_CHANGE_LINE"));
		gGlobal.m_bChangeLine = false;
		break;
	case WARN_CHAT_CHANGE_ICON_YUANBAO_LOCK:
		text = gGlobal.GetStringTable()->Get(_T("MSG_YUANBAO_LOCK_CANT_USE"));
		break;
	case WARN_TRADING_CANT_SELLYUAN:
		text = gGlobal.GetStringTable()->Get(_T("MSG_YB_IN_TRADING"));
		break;
	case WARN_HAWKING_CANT_SELLYUAN:
		text = gGlobal.GetStringTable()->Get(_T("MSG_YB_IN_HAWKING"));
		break;
	case WARN_NOINMAP_CANT_SELLYUAN:
		text = gGlobal.GetStringTable()->Get(_T("MSG_YB_NOT_IN_MAP"));
		break;
	case WARN_SCRIPTING_CANT_SELLYUAN:
		text = gGlobal.GetStringTable()->Get(_T("MSG_YB_SCRIPT_RUNNING"));
		break;
	}

	if (text.length() > 0)
	{
		switch(showIn)
		{
		case WARN_SHOW_IN_CHATBOX:
			gGlobal.PrintMessage(text.c_str(), CHANNELTYPE_SYSTEM);
			break;
		case WARN_SHOW_IN_MIDDLE:
			gGlobal.PrintPlayerMessage(text.c_str());
			break;
		case WARN_SHOW_IN_ALL:
			{
				gGlobal.PrintMessage(text.c_str(), CHANNELTYPE_SYSTEM);
				gGlobal.PrintPlayerMessage(text.c_str());
			}
			break;
		}
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const Boolean& hit)
{
	if (gGlobal.g_pAutoWalkProcess)
	{
		gGlobal.g_pAutoWalkProcess->SetHitBoss(hit);
	}
	return	RPC_RESULT_OK;
}


RPCResult CCharacter::CB_GetPartnerCollectionList(LPCPEER pPeer, RPCContext &context, const UInt32Vector &PartnerIDs)
{
	TRACE_INFODTL(GLOBAL_LOGGER,_F("CCharacter::CB_GetPartnerCollectionList:"));
#ifdef QGL	
	if (gGlobal.g_pPartnerProcess)
	{
		gGlobal.g_pPartnerProcess->SetPartnerCollectionList(PartnerIDs);
	}
#endif	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UsePartnerCollectionBuff(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER,_F("CCharacter::CB_UsePartnerCollectionBuff:returnFlag%d"),returnFlag);
#ifdef QGL	
	switch (returnFlag)
	{
	case PARTNER_COLLECTION_ENABLE_OR_DISABLE_FAIL:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ENABLE_OR_DISABLE_FAIL")));
		break;
	case PARTNER_COLLECTION_ENABLE_SUCCESS:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ENABLE_SUCCESS")));
		gGlobal.m_bUsePartnerCollectionBuff = false;
		break;
	case PARTNER_COLLECTION_DISABLE_SUCCESS:
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_DISABLE_SUCCESS")));
		gGlobal.m_bUsePartnerCollectionBuff = true;
		break;
	}
#endif	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &MOBid, const BYTE &returnFlag)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER,_F("CCharacter::CB_AddPartnerCollection:MOBid %d,returnFlag%d"),MOBid,returnFlag);
#ifdef QGL	
	if (gGlobal.g_pPartnerGroup && gGlobal.g_pPartnerGroup->GetCount() > 0)
	{
		MobDB_Client * mobTemplate = gGlobal.LoadMobTemplate(MOBid);
		String msg;
		String mobName;
		if (mobTemplate && mobTemplate->m_basicinfo)
		{
			mobName = mobTemplate->m_basicinfo->mid_name;
		}
		switch (returnFlag)
		{
		case PARTNER_COLLECTION_ADD_SUCCESS:
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ADD_SUCCESS")),mobName.c_str());
			gGlobal.PrintPlayerMessage(msg);
			break;
		case PARTNER_COLLECTION_ADD_NOT_EXIST:
			gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ADD_NOT_EXIST")));
			break;
		case PARTNER_COLLECTION_ADD_USELESS:
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ADD_USELESS")),mobName.c_str());
			gGlobal.PrintPlayerMessage(msg);
			break;
		case PARTNER_COLLECTION_ADD_ALREADY_EXIST_IN_COLLECTION:
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ADD_ALREADY_EXIST_IN_COLLECTION")),mobName.c_str());
			gGlobal.PrintPlayerMessage(msg);
			break;
		case PARTNER_COLLECTION_ADD_LEVEL_REQUIRE:
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ADD_LEVEL_REQUIRE")),mobName.c_str());
			gGlobal.PrintPlayerMessage(msg.c_str());
			break;
		case PARTNER_COLLECTION_ADD_LOCKED:
			msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_PARTNER_COLLECTION_ADD_LOCKED")),mobName.c_str());
			gGlobal.PrintPlayerMessage(msg);
			break;
		}
	}
#endif	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
#ifdef QGL
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenChangeYBSelect", String());
	
	String msg;
	switch (msgIndex)
	{
	case CHANGE_YB_SUCCESS:	msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_SUCCESS"));	break;
	case CHANGE_YB_FAIL:	msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_FAIL"));		break;		
	case CHANGE_YB_WRONGPASSWORD:	msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_WRONGPASSWORD"));		break;		
	case CHANGE_YB_IN_TRADING: msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_IN_TRADING"));	break;
	case CHANGE_YB_IN_HAWKING: msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_IN_HAWKING"));	break;
	case CHANGE_YB_NOT_IN_MAP: msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_NOT_IN_MAP"));	break;
	case CHANGE_YB_SCRIPT_RUNNING: msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_SCRIPT_RUNNING"));	break;
	case CHANGE_YB_YUANBAOLOCK: msg = gGlobal.GetStringTable()->Get(_T("MSG_CHANGE_YB_YUANBAOLOCK"));	break;
	}

	if (msg.length() > 0)
	{
		gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
		gGlobal.PrintPlayerMessage(msg);
	}

#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RequestTargetEquip(LPCPEER pPeer, RPCContext &context, NetGroup * items)
{

	if (gGlobal.g_pTargetEquipProcess)
	{
		gGlobal.g_pTargetEquipProcess->TargetEquipReply(items);
		DeleteNetGroup(items);
	}
	return RPC_RESULT_OK;
}

//RPCResult CCharacter::CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, NetGroup * items , const UInt32 &char_id )
//{
//
//	if (gGlobal.g_pWeaponsPointSystem)
//	{
//		OutputDebugString(L"CCharacter::CB_GetPlayerEquip\n");
//
//		gGlobal.g_pWeaponsPointSystem->PlayerEquipReply(items,char_id);
//		DeleteNetGroup(items);
//	}
//	return RPC_RESULT_OK;
//}

RPCResult CCharacter::CB_RelationSystem(LPCPEER pPeer, RPCContext &context, NetObject* pRelationSystem)
{
	CRelationSystem* p = CastNetObject<CRelationSystem>(pRelationSystem);
	if(p != NULL)
	{
		CRelationSystemProcess::GetInstance().SetRelationSystem(p);
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	String text;
	switch (msgIndex)
	{
	case SELL_YB_ADD_RECORD_SUCCESS:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_SUCCESS"));
		break;
	case SELL_YB_IN_TRADING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_IN_TRADING"));
		break;
	case SELL_YB_IN_HAWKING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_IN_HAWKING"));
		break;
	case SELL_YB_NOT_IN_MAP:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_NOT_IN_MAP"));
		break;
	case SELL_YB_SCRIPT_RUNNING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_SCRIPT_RUNNING"));
		break;
	case SELL_YB_MEET_MAX_RECORD:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_MEET_MAX_RECORD"));
		break;
	case SELL_YB_NOT_ENOUGH_MONEY:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_NO_MONEY"));
		break;
	case SELL_YB_NOT_ENOUGH_YBT:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_NO_YBT"));
		break;
	case SELL_YB_ERROR:
		text = gGlobal.GetStringTable()->Get(_T("MSG_SELL_YB_ERROR"));
		break;
	case YB_MSG_WRONG_PASSWORD:
		text = gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN_WRONGPASS"));
		break;
	}

	if (text.length() > 0)
	{
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		gGlobal.PrintPlayerMessage(text);
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	String text;
	switch (msgIndex)
	{
	case BUY_YB_SUCCESS:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_SUCCESS"));
		break;
	case BUY_YB_ERROR:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_ERROR"));
		break;
	case BUY_YB_SOLD_ALL:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_SOLD_ALL"));
		break;
	case BUY_YB_NO_MONEY:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_NO_MONEY"));
		break;
	case BUY_YB_IN_TRADING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_IN_TRADING"));
		break;
	case BUY_YB_IN_HAWKING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_IN_HAWKING"));
		break;
	case BUY_YB_NOT_IN_MAP:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_NOT_IN_MAP"));
		break;
	case BUY_YB_SCRIPT_RUNNING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_SCRIPT_RUNNING"));
		break;
	case BUY_YB_EXPIRE:
		text = gGlobal.GetStringTable()->Get(_T("MSG_BUY_YB_EXPIRE"));
		break;
	case YB_MSG_WRONG_PASSWORD:
		text = gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN_WRONGPASS"));
		break;
	}

	if (text.length() > 0)
	{
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		gGlobal.PrintPlayerMessage(text);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseAlertBox", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseSellYBTempBankPanel", String());

	String text;
	switch (msgIndex)
	{
	case TEMPBANK_SUCCESS:
		text = gGlobal.GetStringTable()->Get(_T("MSG_TEMPBANK_GET_SUCCESS"));
		break;
	case TEMPBANK_ERROR:
		text = gGlobal.GetStringTable()->Get(_T("MSG_TEMPBANK_GET_FAIL"));
		break;
	case TEMPBANK_IN_TRADING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_TEMPBANK_IN_TRADING"));
		break;
	case TEMPBANK_IN_HAWKING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_TEMPBANK_IN_HAWKING"));
		break;
	case TEMPBANK_NOT_IN_MAP:
		text = gGlobal.GetStringTable()->Get(_T("MSG_TEMPBANK_NOT_IN_MAP"));
		break;
	case TEMPBANK_SCRIPT_RUNNING:
		text = gGlobal.GetStringTable()->Get(_T("MSG_TEMPBANK_SCRIPT_RUNNING"));
		break;
	case YB_MSG_WRONG_PASSWORD:
		text = gGlobal.GetStringTable()->Get(_T("MSE_SYS_LOGIN_WRONGPASS"));
		break;
	}

	if (text.length() > 0)
	{
		gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
		gGlobal.PrintPlayerMessage(text);
	}

	return RPC_RESULT_OK;
}


RPCResult CCharacter::CB_ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
	String msg;
	if (uiLineupId < 14 && uiLineupId != 0) {
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_SET_LINEUP_SUCCESS")));
		UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("FormationName");
		if (NULL != pText) {
			const LINEUPINFO* pSelLineup=gGlobal.m_resource->GetLineupData(uiLineupId);
			if (NULL != pSelLineup) {
				pText->SetText(pSelLineup->szName);
				pText->Redraw();
			}
		}
	}
	else
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_SET_LINEUP_FAILED")));
	gGlobal.PrintPlayerMessage(msg);
	return	RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos)
{
	String msg;
	UIButton* pCurKeyPos = NULL;
	int iRealKeyPos = iKeyPos;
	if (iRealKeyPos <0 || iRealKeyPos >4) {
		iRealKeyPos = gGlobal.g_pParty->GetM_iLineupKeyPos();
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_SET_KEYPOSE_FAILED")));
	}
	else{
		UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("TeamMenuLineupKeyPos");
		if (NULL != pText) {
			String strTextKeyPos;
			strTextKeyPos.Format(_T("%d"),iRealKeyPos+1);
			pText->SetText(strTextKeyPos);
			pText->Redraw();
		}
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_SET_KEYPOSE_SUCCESS")));
	}
	switch(iRealKeyPos)
	{
	case 0:
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectA");
		break;
	case 1:
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectB");
		break;
	case 2:
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectC");
		break;
	case 3:
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectD");
		break;
	case 4:
		pCurKeyPos = (UIButton*)gGlobal.pUIManager->GetUIItem("KeyPosSelectE");
		break;
	default:
		break;
	}

	if (NULL != pCurKeyPos) {
		pCurKeyPos->SetState(_ButtonDisable);
		pCurKeyPos->SetNotDetected(true);
	}
	gGlobal.PrintPlayerMessage(msg);
	return	RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ForgiveLineup(LPCPEER pPeer, RPCContext &context, const int &iResault)
{
	String msg;
	if ( iResault > 0) {
		UInt32 uTotalLineup = 0;
		for (UInt16 i=1;i<14;i++) {
			if (gGlobal.g_pCharacter->GetSkillById(i) == NULL)
				continue;
			if (i == iResault)
				break;
			uTotalLineup++;
			if (uTotalLineup >= 5) 
				break;
		}
		UIListBox* pListBox = (UIListBox*)gGlobal.pUIManager->GetUIItem("LearnedLineup");
		if (NULL != pListBox && uTotalLineup < 5 ) {
			pListBox->DelString(uTotalLineup);
			pListBox->SetCurSel(0);
			UIPanelEx* pMenuLineupLearned= (UIPanelEx*)gGlobal.pUIManager->FindUIPanelEx("MenuLearnedLineup");
			if (NULL != pMenuLineupLearned) {
				pMenuLineupLearned->ChangeCount(pMenuLineupLearned->GetLineCount()-1);
			}
			pListBox->Redraw();
			pListBox->DrawFont();
			UIStaticText* pText = (UIStaticText*)gGlobal.pUIManager->GetUIItem("CurrentLineup");
			if (NULL != pText) {
				pText->SetText(pListBox->GetBarString());
				pText->Redraw();
			}
		}
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_FORGIVE_LINEUP_SUCCESS")));
	}
	else if (-1 == iResault) {
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_FORGIVE_LINEUP_BASE_CANNT")));
	}
	else if (-2 == iResault) {
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_LINEUP_FORGIVE_LINEUP_NO_EXSIT")));
	}
	gGlobal.PrintPlayerMessage(msg);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_InitTargetHawkConcernState(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	if (gGlobal.g_pHawkerProcess)
	{
		gGlobal.g_pHawkerProcess->InitTargetConcernState(char_id);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money, const Boolean &result)
{
	if(result)
	{
		gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_REWORD_SUCCESS")),CHANNELTYPE_SYSTEM);
	}
	else
	{
		gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_REWORD_NOT_ONLINE")));
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_JailDelivery(LPCPEER pPeer, RPCContext &context)
{	
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenWantedJailDeliveryPanel", String());
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_WantedBail(LPCPEER pPeer, RPCContext &context)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenWantedBailPanel", String());
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_WantedTrace(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY)
{
	if(mapId!=0)
	{
		const BinaryClientMapInfo* pCliMapInfo = gGlobal.m_resource->GetCliMapData(mapId);
		if(pCliMapInfo)
		{
			String temp;
			if(lineId != 0)
				temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL")), lineId,pCliMapInfo->m_mapname,mapX,mapY);
			else
				temp.Format(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL_INSTANCE")), pCliMapInfo->m_mapname,mapX,mapY);
			gGlobal.PrintPlayerMessage(temp.c_str());
			gGlobal.PrintMessage(temp,CHANNELTYPE_SYSTEM);
			return RPC_RESULT_OK;
		}
	}
	gGlobal.PrintPlayerMessage(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL_NOT_ONLINE")));
	gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_WANTED_SHOW_DETAIL_NOT_ONLINE")),CHANNELTYPE_SYSTEM);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RequestWantedTrace(LPCPEER pPeer, RPCContext &context)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenWantedTracePanel", String());
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetGenerationGroup(LPCPEER pPeer, 
											RPCContext &context,
											NetGroup* pGntGrp)
{
	if ( NULL != pGntGrp) {
		UInt32 uiCnt=0;
		m_svtGeneration.clear();
		for (;uiCnt<pGntGrp->GetCount();uiCnt++) {
			CGeneration* pGnt = pGntGrp->GetChild<CGeneration>(uiCnt);
			if (NULL != pGnt) {
				m_svtGeneration.push_back(pGnt);
			}
		}
		DeleteNetGroup(pGntGrp);
		gGlobal.g_pGenerationProcess->ShowGenerations();
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RequestGenerationTrans(LPCPEER pPeer,
												RPCContext &context, 
												const UInt32 &uiRequstId)
{
	if (gGlobal.g_pGenerationProcess->GetCurRequestId() == 0) {
		CCharacterControl* pRequestCharCtrl=QGLApplication::GetInstance().FindCharCtrlByChar_id(uiRequstId);
		if (NULL != pRequestCharCtrl) {
			UIPanel* pPanel = gGlobal.pUIManager->FindUIPanel("GenerationRequestPanel");
			if (NULL == pPanel) {
				CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "FunOpenGenerationPanel", String());
				pPanel = gGlobal.pUIManager->FindUIPanel("GenerationRequestPanel");
				if (NULL != pPanel)
					pPanel->SetEnable(true);
			}
			else
				pPanel->SetEnable(true);
			
			UIStaticText* pDescText = gGlobal.pUIManager->FindUIStaticText("GenerationRequestDesc");
			if (NULL != pDescText) {
				String strDesc=pRequestCharCtrl->GetCharName();
				String strTmp;
				if (pRequestCharCtrl->GetLevel() < 50) {
					strTmp.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_AS_YOUNGER")));
				}
				else{
					strTmp.Format(_T("%s"),gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_AS_ELDER")));
				}
				strDesc+=strTmp;
				pDescText->SetText(strDesc);
				pDescText->Redraw();
				gGlobal.g_pGenerationProcess->SetCurRequestId(uiRequstId);
			}
		}
	}

	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RequestGenerationResult(LPCPEER pPeer, 
												 RPCContext &context,
												 const int &iResultCode, 
												 const UInt32 &uiRelatedId) 
{
	CCharacterControl* pRequestCharCtrl=QGLApplication::GetInstance().FindCharCtrlByChar_id(uiRelatedId);
	if (NULL != pRequestCharCtrl) {
		String msg=pRequestCharCtrl->GetCharName();
		String msgTmp;
		switch(iResultCode) {
		case 1:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_ANS_REFUSED")));
			break;
		case 0:
			{
				if (pRequestCharCtrl->GetLevel()<50)
					msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_AGR_AS_YOUNG")));
				else
					msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_AGR_AS_ELDER")));
				gGlobal.g_pGenerationProcess->UpdateGeneration(false);
			}
			break;
		case -1:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_ANS_EXSITED")));
			break;
		case -2:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_ANS_LEVEL_LIMITED")));
			break;
		case -3:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_ANS_TARGET_FULL")));
			break;
		case -4:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_REQUEST_ANS_SELF_FULL")));
			break;
		case -5:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_TARGET_TOO_FAR")));
			break;
		case -6:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_SELF_TIME_LIMITED")));
			break;
		case -7:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_TARGET_TIME_LIMITED")));
			break;
		case -8:
			msgTmp.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_TARGET_IN_BATTLE")));
			break;
		default:
			break;
		}
		if (!msgTmp.IsEmpty()) {
			if (iResultCode > -4) {
				msg+=msgTmp;
				gGlobal.PrintPlayerMessage(msg);
			}
			else
				gGlobal.PrintPlayerMessage(msgTmp);
		}
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_DeleteGeneration(LPCPEER pPeer, 
										  RPCContext &context, 
										  const int &iResultCode,
										  const UInt32 &uiRelatedId)
{
	String msg;
	if ( 0 == iResultCode ) {
		GenerationVector::iterator it = m_svtGeneration.begin();
		for (;it != m_svtGeneration.end();it++) {
			CGeneration* pGnt = *it;
			if (NULL != pGnt) {
				msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_DISBAND_SUCCESS")),pGnt->GetStrNickName());
				if (pGnt->GetUiRelatedId() == uiRelatedId) {
					m_svtGeneration.erase(it);
					DeleteNetObject(pGnt);
					break;
				}
			}
		}

		gGlobal.g_pGenerationProcess->ShowGenerations();
	}
	else if (-1 == iResultCode) {
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_DISBAND_NOT_EXSITED")));
	}
	else{
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_SELF_TIME_LIMITED")));
	}

	if (!msg.IsEmpty()) {
		gGlobal.PrintPlayerMessage(msg);
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ChangeGenerationType(LPCPEER pPeer,
											  RPCContext &context,
											  const int &iType)
{
	UIButton* pBtn =NULL,*pBtnDisabled=NULL;
	pBtnDisabled= gGlobal.pUIManager->FindUIButton("OpenAsYounger");
	pBtn= gGlobal.pUIManager->FindUIButton("OpenAdElder");
	if (NULL != pBtn && NULL != pBtnDisabled) {
		if (!IsBattle()) {
			pBtn->SetState(_ButtonNormal);
			pBtn->SetNotDetected(false);
		}
		else{
			pBtn->SetState(_ButtonClick);
			pBtn->SetNotDetected(true);
		}
		pBtnDisabled->SetState(_ButtonClick);
		pBtnDisabled->SetNotDetected(true);
	}

	m_svtGeneration.clear();
	String msg(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_CHANGE_TYPE")));
	gGlobal.PrintPlayerMessage(msg);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpdateGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCurAcc, const UInt32 &uiHisAcc)
{
	UInt32 uiTmpAcc = GetCurAcc();
	SetCurAcc(uiCurAcc);
	SetHisAcc(uiHisAcc);
	
	UIStaticText* pAccOrExpText = gGlobal.pUIManager->FindUIStaticText("GenerationAccOrExpValue");
	if (NULL != pAccOrExpText) {
		String strAccOrExp;
		strAccOrExp.Format(_T("%d"),GetCurAcc());
		pAccOrExpText->SetText(strAccOrExp);
		pAccOrExpText->Redraw();
	}

	UIStaticText* pAccText = gGlobal.pUIManager->FindUIStaticText("AccShopCurAcc");
	if (NULL != pAccText) {
		String strPageDesc;
		strPageDesc.Format(_T("%d"),GetCurAcc());
		pAccText->SetText(strPageDesc);
		pAccText->Redraw();
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_OpenBusinessLog(LPCPEER pPeer, RPCContext &context)
{
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenGuildBusinessLogPanel", String());
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AwardMsg(LPCPEER pPeer, RPCContext &context,const String &msg)
{
	gGlobal.PrintPlayerMessage(msg);
	gGlobal.PrintMessage(msg, CHANNELTYPE_SYSTEM);
	return RPC_RESULT_OK;
}

void CCharacter::OnUpdateMark(const UInt32 &mark)
{
	if (!pCharView)
		return;
	pCharView->OnUpdateMark(mark);

}

void CCharacter::OnUpdateTP(const UInt16 &TP, const UInt32 &Model_mobid)
{
	if (!pCharView)
		return;
	pCharView->OnUpdateTP(TP,Model_mobid,false);
}

RPCResult CCharacter::CB_SetMarriageNpcAction(LPCPEER pPeer, RPCContext &context,const UInt32 &npc_uid,const UInt8 &atcion,const UInt8 &direction )
{
	CMapNPCControl * pNPCControl = QGLApplication::GetInstance().FindNPCCtrlByID(npc_uid);
	if( pNPCControl == NULL )
	{
		return RPC_RESULT_OK;
	}
	pNPCControl->GetCharMainView()->SetRotation(direction);
	pNPCControl->GetCharMainView()->SetAnimationIndexByIndex(atcion);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ShowMarriageEff(LPCPEER pPeer, RPCContext &context,const UInt8 &state,const UInt32 &husbandID,const UInt32 &wifeID)
{
	CCharacterControl * pSrcCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(husbandID);
	CCharacterControl * pTargetCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(wifeID);
	if (pSrcCtrl == NULL || pTargetCtrl == NULL)
		return RPC_RESULT_OK;
	String strEffect;
	switch(state)
	{
	case BOW_WAIT:
		strEffect = gConfigure.strBowWait;
		break;
	case BOW_HEAVEN_EARTH:
		strEffect = gConfigure.strBowHeavenEarth;
		break;
	case BOW_FOREFATHER:
		strEffect = gConfigure.strBowForefather;
		break;
	case BOW_COUPLE:
		strEffect = gConfigure.strBowCouple;
		break;
	case BOW_FINISH:
		strEffect = gConfigure.strBowFinish;
		break;
	default:
		break;
	}

	if (!strEffect.IsEmpty())
	{
		UInt32 CurrTime = ::timeGetTime();

		if (pSrcCtrl->GetCharMainView()->GetAnimationIndex() != ANIM_RUN)
		{
			Int rotation  = QGLMath::GetDiaRotationFromTwoPoint(pSrcCtrl->GetCharMainView()->GetCharPos().PosX,
				pSrcCtrl->GetCharMainView()->GetCharPos().PosY, pTargetCtrl->GetCharMainView()->GetCharPos().PosX,
				pTargetCtrl->GetCharMainView()->GetCharPos().PosY);
			pSrcCtrl->GetCharMainView()->SetRotation(rotation);
			pSrcCtrl->GetCharMainView()->SetAnimationIndexByIndex(ANIM_USE, FALSE);
			UInt32 animTime = pSrcCtrl->GetCharMainView()->GetTotalTimeOfGivenAnimation(ANIM_USE, rotation);
			pSrcCtrl->GetCharMainView()->AppendAnimationSet(ANIM_IDLE1, 0, 0, 0, 0, TRUE, rotation, CurrTime + animTime, 0,
				FALSE, FALSE, 0, 0, 0, 0, 0, 0, 0);//, 0,0,
		}

		CAnimatedEntity ent;
		ent.LoadEntityFile(strEffect.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);

		UIEffectData data;
		data.effectFileName = strEffect;
		data.PixelPosX = gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosX;
		data.PixelPosY = gGlobal.g_pCharacter->GetCharMainView()->GetEntPixelPos().PixelPosY;
		QGLApplication::GetInstance().AddUIEffectToLst(CurrTime/* + ent.GetTotalTimeOfGivenAnimation()*/, data);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetChangePos(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex)
{
	if (0 == iRetCode) {
		if (NULL != gGlobal.g_pParty)
			gGlobal.g_pParty->SetChangePos(ui8PreIndex,ui8DestIndex);
		else if (NULL != gGlobal.g_pTempLeaveParty) 
			gGlobal.g_pTempLeaveParty->SetChangePos(ui8PreIndex,ui8DestIndex);
	}
	else
	{
		String msg(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_CHANGE_MEMBER_POSITION_FAILED")));
		gGlobal.PrintPlayerMessage(msg);
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_BugAccShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &iIndex)
{
	String msg;
	switch(iRetCode) {
	case 0:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_SUCCESS")));
		break;
	case -1:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_ACC_ENOUGH")));
		break;
	case -2:
	case -3:
	case -4:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_NO_ITEM")));
		break;
	case -5:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_NO_SLOT")));
		break;
	default:
		msg.Empty();
		break;
	}
	if( !msg.IsEmpty() )
		gGlobal.PrintPlayerMessage(msg);

	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_GenerationGainAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedAcc)
{
	String msg;
	msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_GENERATION_GAIN_ACC")),iGainedAcc);
	gGlobal.PrintMessage(msg,CHANNELTYPE_SYSTEM);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ResetChangeLine(LPCPEER pPeer, RPCContext &context)
{
	gGlobal.m_bChangeLine = false;
	return RPC_RESULT_OK;

}
RPCResult CCharacter::CB_SearchParty(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList)
{
#ifdef QGL
	if (gGlobal.g_pSearchListProcess)
	{
		gGlobal.g_pSearchListProcess->ShowPartySearchList(PartyInfoList);
	}
#endif
	return RPC_RESULT_OK;	
}

RPCResult CCharacter::CB_SearchChar(LPCPEER pPeer, RPCContext &context, const CharInfoVector &CharInfoList)
{
#ifdef QGL
	if (gGlobal.g_pSearchListProcess)
	{
		gGlobal.g_pSearchListProcess->ShowCharSearchList(CharInfoList);
	}
#endif
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetPartyInfo(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList)
{
#ifdef QGL
	if (gGlobal.g_pSearchListProcess)
	{
		gGlobal.g_pSearchListProcess->ShowSelectPartyInfo(PartyInfoList);
	}
#endif
	return RPC_RESULT_OK;	
}

RPCResult CCharacter::CB_OpenSearchPartyPanel(LPCPEER pPeer, RPCContext &context)
{
#ifdef QGL
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseNPCSearch", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenSearchNPCList", String());
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "ShowPartyList", String());
#endif
	return RPC_RESULT_OK;	
}


RPCResult CCharacter::CB_LeaveTeamTemporarily(LPCPEER pPeer,RPCContext &context, const UInt32 &uRetCode, const UInt32 &uId)
{
	if (0 == uRetCode) 
	{
		if (uId == gGlobal.g_pCharacter->GetCharNetObject()->GetChar_id()) 
		{//self
			if(NULL != gGlobal.g_pParty )
			{
				if (NULL != gGlobal.g_pTempLeaveParty) 
					DeleteNetGroup(gGlobal.g_pTempLeaveParty,BOOLEAN_TRUE);
				gGlobal.g_pTempLeaveParty = gGlobal.g_pParty;
				for (UInt16 i=0;i<gGlobal.g_vctPartyMemberPointer.size();i++) 
				{
					if (!gGlobal.g_pParty->GetM_bTempLeave(i)) 
					{
						CCharacter* pMember = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[i]);
						if (NULL != pMember && pMember != gGlobal.g_pCharacter->GetCharNetObject())
						{
							Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(i);
							if (it != gGlobal.g_mapPartyChar.end()) 
							{
								CPartyCharInfoClient* pOldCpc=it->second;
								if (NULL != pOldCpc) 
									pOldCpc->SetData(pMember);
							}
							else
							{
								CPartyCharInfoClient* cpc = SafeCreateObject(CPartyCharInfoClient);
								cpc->SetData(pMember);
								gGlobal.g_mapPartyChar.insert(std::make_pair(i,cpc));
								gGlobal.g_vctPartyMemberPointer[i] = NULL;							
							}
							gGlobal.g_pParty->Remove(pMember);
						}	
					}
				}
			}
			gGlobal.g_pParty = NULL;
			gGlobal.g_pCharacter->GetCharMainView()->RemoveFollowTarget();
		}
		else 
		{//not self
			CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
			Boolean bTempLeave = (NULL == gGlobal.g_pParty && NULL != gGlobal.g_pTempLeaveParty);
			if (NULL != pParty && !bTempLeave) 
			{//self in party
				for (UInt16 i= 1;i< gGlobal.g_vctPartyMemberPointer.size();i++) 
				{
					Global::PartyCharInfoMap::iterator itTest = gGlobal.g_mapPartyChar.find(i);
					if (itTest != gGlobal.g_mapPartyChar.end())//check if the member temporarily leave
						continue;
					CCharacter* pCharTempLeave = CastNetObject<CCharacter>(gGlobal.g_vctPartyMemberPointer[i]);
					if (NULL != pCharTempLeave && pCharTempLeave->GetChar_id() == uId) 
					{
						CCharacterControl * pCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pCharTempLeave);
						if (NULL != pCtrl)
						{
							if (pCtrl != gGlobal.g_pCharacter)
							{
								pCtrl->SetFontColor(D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
									gConfigure.uNameFontColorG, gConfigure.uNameFontColorB));
								pCtrl->GetCharNameView()->SetFontColor(pCtrl->GetFontColor());
							}
							pCtrl->GetCharMainView()->RemoveFollowTarget();
						}
						Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(i);
						if (it != gGlobal.g_mapPartyChar.end()) 
						{
							CPartyCharInfoClient* pOldCpc=it->second;
							if (NULL != pOldCpc) 
								pOldCpc->SetData(pCharTempLeave);
						}
						else
						{
							CPartyCharInfoClient* cpc = SafeCreateObject(CPartyCharInfoClient);
							cpc->SetData(pCharTempLeave);
							gGlobal.g_mapPartyChar.insert(std::make_pair(i,cpc));
							gGlobal.g_vctPartyMemberPointer[i] = NULL;
							gGlobal.g_pParty->Remove(pCharTempLeave);
						}
						break;
					}
				}
			}
		}
		CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
		pParty->RedrawUI();
		gGlobal.g_pPartyProcess->UpdatePartyPanel(pParty);
		gGlobal.pUIManager->RenderAll();
	}
	else
	{
		String strMsg;
		switch(uRetCode) 
		{
		case 1:
		case 2:
			strMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_TEMP_LEAVE_NO_PARTY")));
			break;
		case 3:
			strMsg.Format(gGlobal.GetStringTable()->Get(_T("MSG_TEMP_LEAVE_LEADER")));
			break;
		default:
			break;
		}
		gGlobal.PrintMessage(strMsg,CHANNELTYPE_SYSTEM);

	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_GetPartyInfoWhenJumpMap(LPCPEER pPeer, RPCContext &context, NetGroup* pParty)
{
#ifdef QGL
	CParty* pCurrentParty = CastNetObject<CParty>(pParty);

	if (pCurrentParty == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _T("CCharacter CB_GetPartyInfoWhenJumpMap Error: invalid party"));
		return RPC_RESULT_FAIL;
	}
	if (gGlobal.g_pTempLeaveParty)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _T("CCharacter CB_GetPartyInfoWhenJumpMap Error: already have party"));
		return RPC_RESULT_FAIL;
	}

	if (gGlobal.g_pParty && gGlobal.g_pParty != pCurrentParty)
		DeleteNetGroup(gGlobal.g_pParty);
	gGlobal.g_pParty = pCurrentParty;
	gGlobal.g_vctPartyMemberPointer.clear();
	for (UInt16 j=0;j<pCurrentParty->GetCount();j++) 
	{
		if (pCurrentParty->GetM_bTempLeave(j))
			gGlobal.g_vctPartyMemberPointer.push_back(NULL);
		else
			gGlobal.g_vctPartyMemberPointer.push_back(pCurrentParty->GetChild<CCharacter>(j));
	}

	pCurrentParty->RedrawUI();
	pCurrentParty->ResetNameColor();

	if (!pCurrentParty->IsLeader())
	{
		gGlobal.g_pMiddleMap->ResetCharDes();
		CCrossMapPathFinder::GetInstance()->DestroyPathData();
		gGlobal.m_u32DestNpcId = 0;
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
	}

	gGlobal.g_pPartyProcess->UpdatePartyPanel(pCurrentParty);
#endif
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ReturnParty(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, NetObject* pChar)
{
	if (0 == uRetCode) 
	{
		CCharacter* pNewChar = CastNetObject<CCharacter>(pChar);
		if (NULL == pNewChar) 
			return RPC_RESULT_OK;
		if (pNewChar == gGlobal.g_pCharacter->GetCharNetObject()) 
		{//self
			if(gGlobal.g_pParty != gGlobal.g_pTempLeaveParty)
			{
				if (NULL != gGlobal.g_pParty) 
					DeleteNetGroup(gGlobal.g_pParty,BOOLEAN_TRUE);
				gGlobal.g_pParty = gGlobal.g_pTempLeaveParty;
			}
			gGlobal.g_pTempLeaveParty = NULL;
			UInt16 i;
			for (i = 0;i< gGlobal.g_vctPartyMemberPointer.size();i++) 
			{
				if (!gGlobal.g_pParty->GetM_bTempLeave(i)) 
				{
					Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(i);
					if (it != gGlobal.g_mapPartyChar.end()) 
					{
						CPartyCharInfoClient* pCpc = it->second;
						if (NULL != pCpc) 
						{
							//find character instance in client by char id,and restore pointer in party
							CCharacterControl* pCharCtrl = QGLApplication::GetInstance().FindCharCtrlByChar_id(pCpc->GetU32CharId());
							if (NULL != pCharCtrl) 
							{
								gGlobal.g_vctPartyMemberPointer[i] = pCharCtrl->GetCharNetObject();
								gGlobal.g_mapPartyChar.erase(it);
								SafeDeleteObject(pCpc);
								if (0 == i) 
								{
									pCharCtrl->SetFontColor(TextColor[ColorIndex_LightBlue]);
									pCharCtrl->GetCharNameView()->SetFontColor(pCharCtrl->GetFontColor());
								}
							}
						}
					}
				}
			}
			gGlobal.g_pParty->Clear(BOOLEAN_FALSE);
			for (UInt16 j=0;j<gGlobal.g_vctPartyMemberPointer.size();j++) 
			{
				if (NULL != gGlobal.g_vctPartyMemberPointer[j]) 
					gGlobal.g_pParty->Add(gGlobal.g_vctPartyMemberPointer[i]);
			}
		}
		else 
		{//other member
			CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
			if (NULL != pParty) 
			{
				Boolean bTempLeave =(gGlobal.g_pTempLeaveParty != NULL && gGlobal.g_pParty == NULL);
				for (UInt16 i= 1;i< gGlobal.g_vctPartyMemberPointer.size();i++) 
				{
					Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.find(i);
					if (it != gGlobal.g_mapPartyChar.end()) 
					{
						CPartyCharInfoClient* pCpc = it->second;
						if (NULL != pCpc && pCpc->GetU32CharId() == pNewChar->GetChar_id()) 
						{
							if (!bTempLeave) 
							{//self in party,restore the return member
								//find character instance in client by char,and restore pointer in party
								CCharacterControl * pCtrlMem = QGLApplication::GetInstance().FindCharCtrlByChar_id(pNewChar->GetChar_id());
								if (NULL != pCtrlMem)
								{
									pCtrlMem->SetFontColor(TextColor[ColorIndex_LightBlue]);
									pCtrlMem->GetCharNameView()->SetFontColor(pCtrlMem->GetFontColor());
									gGlobal.g_vctPartyMemberPointer[i] = pCtrlMem->GetCharNetObject();
									if (i < gGlobal.g_pParty->GetCount()) 
										gGlobal.g_pParty->Insert(pCtrlMem->GetCharNetObject(),i);
									else
										gGlobal.g_pParty->Add(pCtrlMem->GetCharNetObject());
									gGlobal.g_mapPartyChar.erase(it);
									SafeDeleteObject(pCpc);
								}
							}
							else//self temporarily leave,refresh the info
							{
								pCpc->SetData(pNewChar);
								CCharacterControl * pCtrlMem = QGLApplication::GetInstance().FindCharCtrlByChar_id(pNewChar->GetChar_id());
								if (NULL == pCtrlMem)
									DeleteNetObject(pNewChar);
							}	
							break;
						}
					}
				}
			}
		}
		CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
		pParty->RedrawUI();
		gGlobal.g_pPartyProcess->UpdatePartyPanel(pParty);
		gGlobal.pUIManager->RenderAll();
	}
	else
	{
		switch(uRetCode) {
		case 1:
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_PARTY_NOTINTEMP")),CHANNELTYPE_SYSTEM);
			break;
		case 2:
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_PARTY_DIFF_MAP")),CHANNELTYPE_SYSTEM);
			break;
		case 3:
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_PARTY_BUSSY")),CHANNELTYPE_SYSTEM);
			break;
		case 4:
		case 5:
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_PARTY_INBATTLE")),CHANNELTYPE_SYSTEM);
			break;
		case 6:
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_PARTY_IN_BUSSINESS")),CHANNELTYPE_SYSTEM);
			break;
		case 7:
			gGlobal.PrintMessage(gGlobal.GetStringTable()->Get(_T("MSG_RETURN_PARTY_TOO_FAR")),CHANNELTYPE_SYSTEM);
			break;
		default:
			break;
		}
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RefreshPartyCharInfo(LPCPEER pPeer, RPCContext &context, NetObject* pPartyCharInfo)
{
	Boolean bFind = BOOLEAN_FALSE;
	CPartyCharInfo* pNewInfo = CastNetObject<CPartyCharInfo>(pPartyCharInfo);
	for (Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();it != gGlobal.g_mapPartyChar.end();it++) 
	{
		CPartyCharInfoClient* pCpc = it->second;
		if (NULL != pCpc) 
		{
			if (pCpc->GetU32CharId() == pNewInfo->GetU32CharId()) 
			{
				pCpc->SetData(pNewInfo);
				bFind = BOOLEAN_TRUE;
				break;
			}
		}
	}
	if (!bFind) 
	{
		CPartyCharInfoClient* cpc = SafeCreateObject(CPartyCharInfoClient);
		cpc->SetData(pNewInfo);
		gGlobal.g_mapPartyChar.insert(std::make_pair(pNewInfo->GetU16Index(),cpc));
		/*if (NULL != gGlobal.g_pParty) 
		{
			gGlobal.g_pParty->RedrawUI();
			gGlobal.g_pPartyProcess->UpdatePartyPanel(gGlobal.g_pParty);
		}
		else if (NULL != gGlobal.g_pTempLeaveParty) 
		{
			gGlobal.g_pTempLeaveParty->RedrawUI();
			gGlobal.g_pPartyProcess->UpdatePartyPanel(gGlobal.g_pTempLeaveParty);
		}*/
	}
	DeleteNetObject(pPartyCharInfo);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_OpenWeaponsRankPanel(LPCPEER pPeer, RPCContext &context)
{
#ifdef QGL
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "OpenWeaponsRankPanel", String());
#endif
	return RPC_RESULT_OK;	
}

//RPCResult CCharacter::CB_GetPlayerEquipData(LPCPEER pPeer, RPCContext &context , const UInt32 &ID)
//{
//#ifdef QGL
//	if(gGlobal.g_pWeaponsPointSystem)
//	{
//		gGlobal.g_pWeaponsPointSystem->GetPlayerEquip(ID);
//	}
//#endif
//	return RPC_RESULT_OK;
//}

RPCResult CCharacter::CB_BuyPointsShopItem( LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &uiPoints )
{
#ifdef QGL
	String msg;

	switch(iRetCode) {
	case 0:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_SUCCESS")));
		if (gGlobal.g_pPointsShopProcess)
		{
			gGlobal.g_pPointsShopProcess->SetTotalPoints(uiPoints);
		}
		break;
	case -1:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_ACC_ENOUGH")));
		break;
	case -2:
	case -3:
	case -4:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_NO_ITEM")));
		break;
	case -5:
		msg.Format(gGlobal.GetStringTable()->Get(_T("MSG_ACC_SHOP_BUY_NO_SLOT")));
		break;
	case -6:
		if (gGlobal.g_pPointsShopProcess)
		{
			gGlobal.g_pPointsShopProcess->SetTotalPoints(uiPoints);
			gGlobal.g_pPointsShopProcess->ShowPointsShop();
		}
		break;
	default:
		msg.Empty();
		break;
	}
	if( !msg.IsEmpty() )
		gGlobal.PrintPlayerMessage(msg);

#endif

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GenerationGainPoints( LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedPoints )
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_HandleGuideEvent(LPCPEER pPeer, RPCContext &context, const UInt16 &ID)
{
#ifdef QGL
	gGlobal.g_pMenuHelpProcess->HandleGuideEvent(ID);
#endif
	return RPC_RESULT_OK;
}


RPCResult CCharacter::CB_PetUseSkillMsg(LPCPEER pPeer, RPCContext &context, const String &Msg)
{
	if(gGlobal.g_pCharacter && gGlobal.g_pCharacter->GetCharNetObject())
		gGlobal.PrintMessage(Msg,CHANNELTYPE_SYSTEM,String());
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetChangeModelSkillGroup(LPCPEER pPeer, RPCContext &context, NetGroup* skillGroup)
{
	if(gGlobal.g_ChangeModelSkillLst)
	{
		DeleteNetGroup(gGlobal.g_ChangeModelSkillLst,BOOLEAN_TRUE);
	}
	gGlobal.g_ChangeModelSkillLst = CastNetObject<CSkillGroup>(skillGroup);
	if(gGlobal.g_ChangeModelSkillLst)
	{
		gGlobal.g_ChangeModelSkillLst->SetOwnerType(SKILL_OWNER_TYPE_CHAR);
		TRACE_INFODTL_1(GLOBAL_LOGGER, _T("CB_SetChangeModelSkillGroup: g_ChangeModelSkillLst size =  %d"), gGlobal.g_ChangeModelSkillLst->GetCount());
	}
	if (gGlobal.g_pSkillProcess)
		gGlobal.g_pSkillProcess->ConstructData();
	gGlobal.ClearShortCutSlotAll();
	UIButton *SkillItem = gGlobal.pUIManager->FindUIButton("DownBSkill");
	if (SkillItem)
	{
		SkillItem->SetState(_ButtonMouseOver);
		gGlobal.pUIManager->AddDirtyUIItem(SkillItem);
	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ShowIncognitoBoxEff(LPCPEER pPeer, RPCContext &context, const UInt16 &posx, const UInt16 &posy,const UInt8 &effType)
{
	String strEffect;
	switch(effType)
	{
	case 1:
		strEffect = gConfigure.strIncognitoBoxRed;
		break;
	case 2:
		strEffect = gConfigure.strIncognitoBoxYellow;
		break;
	case 3:
		strEffect = gConfigure.strIncognitoBoxGreen;
		break;
	default:
		break;
	}

	if (!strEffect.IsEmpty())
	{
		UInt32 CurrTime = ::timeGetTime();
		CAnimatedEntity ent;
		ent.LoadEntityFile(strEffect.c_str(), gGlobal.pD3DDevice, EQUIP_OTHERS);

		UIEffectData data;
		data.effectFileName = strEffect;
		Int PixelPosX,PixelPosY;
		QGLMath::BlockPosToPixel(posx,posy,PixelPosX,PixelPosY);
		data.PixelPosX = PixelPosX;
		data.PixelPosY = PixelPosY;
		QGLApplication::GetInstance().AddUIEffectToLst(CurrTime/* + ent.GetTotalTimeOfGivenAnimation()*/, data);
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RestorePartyAsTemp(LPCPEER pPeer, RPCContext &context, NetGroup* pParty)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_RestorePartyAsTemp: Restore Party now.."));

#ifdef QGL
	CParty* pPartyTemp = CastNetObject<CParty>(pParty);

	if (pPartyTemp == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CCharacter CB_RestorePartyAsTemp Error: invalid party"));
		return RPC_RESULT_FAIL;
	}
	if (gGlobal.g_pParty || gGlobal.g_pTempLeaveParty)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CCharacter CB_RestorePartyAsTemp Error: already have party"));
		return RPC_RESULT_FAIL;
	}

	gGlobal.g_pTempLeaveParty = pPartyTemp;
	gGlobal.g_vctPartyMemberPointer.clear();
	for (UInt16 j=0;j<pPartyTemp->GetCount();j++) 
	{
		CCharacter* pCharMem = pPartyTemp->GetChild<CCharacter>(j);
		if (NULL != pCharMem && pCharMem->GetChar_id() == GetChar_id()) 
			gGlobal.g_vctPartyMemberPointer.push_back(this);
		else
			gGlobal.g_vctPartyMemberPointer.push_back(NULL);
	}

	gGlobal.g_pMenuHelpProcess->HandleEvent(HELP_TIPS_FIRST_FORMTEAM);

	//create temp friend if not exist

	for (UInt i = 0; i < pPartyTemp->GetCount(); i++)
	{
		CCharacter* pMember = CastNetObject<CCharacter>(pPartyTemp->GetChild<CCharacter>(i));
		if (pMember)
		{
			gGlobal.g_pMenuFriendProcess->InserTempFriend(pMember->GetChar_id(), pMember->GetNickName(), FRIENC_CHAT_STATUS_ONLINE);	
		}
	}

	///////////////////////////////////////////////////////
	SetFollowPartner(gGlobal.pPeerProxy, gGlobal.g_context, 0);
	/////////////////////////////////////////////////////////

	pPartyTemp->RedrawUI();
	pPartyTemp->ResetNameColor();

	if (QGLApplication::GetInstance().GetMapStateID() == ST_APP_BATTLE)
	{
		UIButton *pItem = gGlobal.pUIManager->FindUIButton("DownBParty");
		if (pItem)
		{
			if (pPartyTemp->IsLeader())
				pItem->SetNormalState();
			else
				pItem->SetDisableState();
		}

	}

	if (!pPartyTemp->IsLeader())
	{
		gGlobal.g_pMiddleMap->ResetCharDes();
		CCrossMapPathFinder::GetInstance()->DestroyPathData();
		gGlobal.m_u32DestNpcId = 0;
		CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseMiddleMap", String());
	}

	//print message
	/*{
	CCharacter* pchar = pParty->GetChild<CCharacter>(pParty->GetCount() - 1);
	if (pchar)
	{
	String text(gGlobal.GetStringTable()->Get(_T("MSG_PARTY_JOIN"));
	text.Format(text.c_str(), pchar->GetNickName().c_str());
	gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
	}
	}*/
	CallLuaFunc(gGlobal.pUIManager->GetLuaState(), "CloseHawk", String());
	gGlobal.g_pPartyProcess->UpdatePartyPanel(pPartyTemp);

	//m_pParty->ShowMember();
#endif // QGL

	return RPC_RESULT_OK;
}
