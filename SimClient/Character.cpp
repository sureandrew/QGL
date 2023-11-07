//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "SimClient/Character.h"
//-- Library
#include "SimClient/Global.h"
#include "SimClient/Party.h"
#include "SimClient/ItemBag.h"
#include "SimClient/Map.h"
#include "SimClient/Skill.h"
#include "SimClient/QuestSystem.h"
#include "SimClient/Trading.h"
#include "SimClient/Pet.h"
#include "SimClient/PetGroup.h"
#include "SimClient/PartnerGroup.h"
#include "SimClient/Partner.h"
#include "SimClient/HawkGroup.h"	
#include "SimClient/HawkItem.h"
#include "SimClient/Item.h"
#include "SimClient/MessageService.h"
#include "SimClient/Question.h"


PROTO_LuaPush(CCharacter)

REGISTER_CLASSTYPE(CCharacter, Character_CLASSID);

CCharacter::CCharacter()
{
	m_pParty = NULL;
	m_bHackInProcess = FALSE;
	m_WrongAns = 0;		
	m_bIsBuyingChargeItem = false;
}

CCharacter::~CCharacter()
{
	DeleteNetObject(m_pParty);
}

Boolean CCharacter::IsInBattle()
{
	UInt16 action = GetAction();

	return (action == ST_BATTLE || action == ST_MONITOR || action == ST_NEEDHELP);
}

String CCharacter::DebugInfo()
{
	String s(Parent::DebugInfo());
	s.AppendFormat(_T(", acct_id %d, char_id %d"), 
		GetAcct_id(), GetChar_id());
	return s;
}

void CCharacter::OnUpdateId(const UInt32 &acct_id, const UInt32 &char_id)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update Character %d: acct_id %d, char_id %d"), 
			GetChar_id(), acct_id, char_id);
}

void CCharacter::OnUpdateLevel(const UInt16 &level)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update Character %d: level %d"), GetChar_id(), level);
}

void CCharacter::OnUpdateExp(const UInt32 &exp)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter) {
		if (IsOption(OPT_SHOW_SELF)) {
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update Character %d: exp %d, next level %d"), 
				GetChar_id(), exp, GetNextLvExp());
		}

		if (GetExp() >= GetNextLvExp()) {
			TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Character %d: your character can level up (%d/%d)!"), 
				GetChar_id(), exp, GetNextLvExp());
		}
	}
}

void CCharacter::OnUpdateAttrib(const UInt16 &STA, const UInt16 &SPR, const UInt16 &STR, const UInt16 &CON, const UInt16 &AGI)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_6(GLOBAL_LOGGER, _F("Update Character %d: STA %d, SPR %d, STR %d, CON %d, AGI %d"), 
			GetChar_id(), STA, SPR, STR, CON, AGI);
}

void CCharacter::OnUpdateAttrib2(const UInt16 &ATK, const UInt16 &DEF, const UInt16 &HIT, const UInt16 &EVA, const UInt16 &POW, const UInt16 &SPD)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_7(GLOBAL_LOGGER, _F("Update Character %d: ATK %d, DEF %d, HIT %d, EVA %d, POW %d, SPD %d"), 
			GetChar_id(), ATK, DEF, HIT, EVA, POW, SPD);
}

void CCharacter::OnUpdateAttrPoint(const UInt16 &attrPoint)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update Character %d: attrPoint %d"), GetChar_id(), attrPoint);
}

void CCharacter::OnUpdateHP(const UInt16 &HP, const UInt16 &mHP, const UInt16 &maxHP)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Update Character %d: HP %d, mHP %d, maxHP %d"), GetChar_id(), HP, mHP, maxHP);
}

void CCharacter::OnUpdateSP(const UInt16 &SP, const UInt16 &mSP, const UInt16 &maxSP)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Update Character %d: SP %d, mSP %d, maxSP %d"), GetChar_id(), SP, mSP, maxSP);
}

void CCharacter::OnUpdateDP(const UInt16 &DP, const UInt16 &mDP, const UInt16 &maxDP)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Update Character %d: DP %d, mDP %d, maxDP %d"), GetChar_id(), DP, mDP, maxDP);
}

void CCharacter::OnUpdateWC(const UInt16 &WC, const UInt16 &maxWC, const UInt16 &WcExp)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Update Character %d: WC %d, maxWC %d, WcExp %d"), GetChar_id(), WC, maxWC, WcExp);
}

void CCharacter::OnUpdateVP(const UInt16 &VP, const UInt16 &maxVP)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update Character %d: VP %d, maxVP %d"), GetChar_id(), VP, maxVP);
}

void CCharacter::OnUpdateHonor(const UInt32 &honor)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update Character %d: honor %d"), GetChar_id(), honor);
}

void CCharacter::OnUpdateMoney(const UInt32 &money)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update Character %d: money %d"), GetChar_id(), money);
}

void CCharacter::OnUpdateStyle(const UInt16 &photo_id, const UInt16 &hairStyle1, 
							   const UInt16 &hairStyle2, const UInt16 &hairColor)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Update Character %d: photo_id %d, hairStyle1 %d, hairStyle2 %d, hairColor %d"), 
			GetChar_id(), photo_id, hairStyle1, hairStyle2, hairColor);
}

void CCharacter::OnUpdateEqWeapon(const UInt32 &eq_weapon, const UInt16 &eq_weaponColor)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update Character %d: eq_weapon %d, color %d"), GetChar_id(), 
			eq_weapon, eq_weaponColor);
}

void CCharacter::OnUpdateEqHead(const UInt32 &eq_headBand, const UInt16 &eq_headColor)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update Character %d: eq_headBand %d, color %d"), GetChar_id(), 
			eq_headBand, eq_headColor);
}

void CCharacter::OnUpdateEqCloth(const UInt32 &eq_cloth, const UInt16 &eq_clothColor)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Update Character %d: eq_cloth %d, color %d"), GetChar_id(), 
			eq_cloth, eq_clothColor);
}

void CCharacter::OnUpdateState(const UInt8 &state)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update Character %d: state %d"), GetChar_id(), state);
}

void CCharacter::OnUpdateBuff(const String &buff)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Update Character %d: buff %s"), GetChar_id(), buff.c_str());
}

void CCharacter::OnUpdatePartyInfo(const Boolean &hasParty, const Boolean &isLeader, const RPCNetID &partyNetID)
{
	if (this == gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_SELF) ||
		this != gGlobal.m_pSimClient->m_pCharacter && IsOption(OPT_SHOW_OTHER))
		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("Update Character %d: hasParty %d %d, isLeader %d %d"), GetChar_id(), 
		hasParty, GetHasParty(), isLeader, GetIsLeader());
}

RPCResult CCharacter::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("error_code %d"), errCode);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_JoinParty(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
	CParty* pParty = CastNetObject<CParty>(party);

	if (pParty == NULL)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("invalid party"));
		return RPC_RESULT_FAIL;
	}
	if (m_pParty && m_pParty != party)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("already have party"));
		return RPC_RESULT_FAIL;
	}

	m_pParty = pParty;
	m_pParty->ShowMember();

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ChangeDesc(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyRequestOk(LPCPEER pPeer, RPCContext &context, const String &name)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyRequestNo(LPCPEER pPeer, RPCContext &context, const UInt16 &nState)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyInviteOk(LPCPEER pPeer, RPCContext &context, const String &name)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyJoinRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId)
{

	CCharacter* pchar = FindNetObject<CCharacter>(targetId);
	if (pchar) {

	}

	Boolean reply = true;
	Boolean showMsg = true;

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_PartyJoinRequest: target %d, %d party join request auto-reply OK"),
		targetId.srvId, targetId.objId);
	PartyJoinReply(gGlobal.m_pSimClient->m_pProxyPeer, context, targetId, reply, showMsg);
	SetFollowPartner(gGlobal.m_pSimClient->m_pProxyPeer, context, 0);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyInviteRequest(LPCPEER pPeer, RPCContext &context, NetGroup* party)
{
	Boolean reply = BOOLEAN_TRUE;
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_PartyInviteRequest: party invite request auto-reply PartyID=%d,%d"), party->GetNetId().srvId, party->GetNetId().objId);
	CParty* pParty = CastNetObject<CParty>(party);
	if (pParty == NULL)
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, _F("CB_PartyInviteRequest Error: invalid party"));
		return RPC_RESULT_FAIL;
	}
	if (m_pParty)
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_PartyInviteRequest: character already insdie another party PartyID=%d,%d"), m_pParty->GetNetId().srvId, m_pParty->GetNetId().objId);
        reply = BOOLEAN_FALSE;
	}
	PartyInviteReply(gGlobal.m_pSimClient->m_pProxyPeer, context, pParty->GetNetId(), reply, true);
	SetFollowPartner(gGlobal.m_pSimClient->m_pProxyPeer, context, 0);
	TRACE_ENSURE(m_pParty != party);
	DeleteNetGroup(party, BOOLEAN_FALSE);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyClearRequest(LPCPEER pPeer, RPCContext &context, const RPCNetID &targetId, const Boolean &showMsg)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_PartyClearRequest: target %d, %d"),
		targetId.srvId, targetId.objId);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PartyClearInvite(LPCPEER pPeer, RPCContext &context, const RPCNetID &partyId, const Boolean &showMsg)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_PartyClearInvite: partyId %d, %d"),
		partyId.srvId, partyId.objId);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_CancelAuto(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_CancelAuto:"));

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_LevelUp(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_LevelUp:"));

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RaiseAttrib(LPCPEER pPeer, RPCContext &context)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CCharacter CB_RaiseAttrib:"));

	return RPC_RESULT_OK;
}

void CCharacter::ShowSummary(int i)
{
	TRACE_INFODTL_7(GLOBAL_LOGGER, _F("%3d: Character %d: netId %d,%d, sex %d, cclass %d, level %d"),
		i, GetChar_id(), GetNetId().srvId, GetNetId().objId, GetSex(), GetCclass(), GetLevel());
	TRACE_INFODTL_8(GLOBAL_LOGGER, _F("   : curPos %d,%d, dstPos %d,%d, action %d, followId %d, hasParty %d, isLeader %d"),
		GetCurPosX(), GetCurPosY(), GetDstPosX(), GetDstPosY(), 
		GetAction(), GetFollowId(), GetHasParty(), GetIsLeader());
}

void CCharacter::ShowDetail()
{
	if (gGlobal.m_pSimClient->m_pMap == NULL)
		return;

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Show Character: char_id %d, netId %d,%d, objId %d"), 
		GetChar_id(), GetNetId().srvId, GetNetId().objId, GetID());
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("  acct_id %d, char_id %d"), 
		GetAcct_id(), GetChar_id());
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("  sex %d, cclass %d, faction %d, level %d, exp %d, next level %d"), 
		GetSex(), GetCclass(), GetFaction(), GetLevel(), GetExp(), GetNextLvExp());
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("  STA %d, SPR %d, STR %d, CON %d, AGI %d"), 
		GetSTA(), GetSPR(), GetSTR(), GetCON(), GetAGI());
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("  ATK %d, DEF %d, HIT %d, EVA %d, POW %d, SPD %d"), 
		GetATK(), GetDEF(), GetHIT(), GetEVA(), GetPOW(), GetSPD());
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("  HP %d/%d/%d, SP %d/%d/%d"),
		GetHP(), GetMHP(), GetMaxHP(), GetSP(), GetMSP(), GetMaxSP());
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("  honey %d, money %d, hasParty %d, isLeader %d"), 
		GetHonor(), GetMoney(), HasParty(), IsLeader());
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("  eq_weapon %d, eq_headBand %d, eq_cloth %d"), 
		GetEq_weapon(), GetEq_headBand(), GetEq_cloth());
	TRACE_INFODTL_7(GLOBAL_LOGGER, _F("  map_id %d, line_id %d, curPos %d,%d, dstPos %d,%d, followId %d"), 
		gGlobal.m_pSimClient->m_pMap->GetMap_id(), gGlobal.m_pSimClient->m_pMap->GetLine_id(),
		GetCurPosX(), GetCurPosY(), GetDstPosX(), GetDstPosY(), GetFollowId());
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("  state %d, action %d, enablePK %d"), 
		GetState(), GetAction(), GetEnablePK());
}

RPCResult CCharacter::CB_PKRequest(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag, const UInt32 &Target_ID, const UInt8 &teamCount)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_PKRequest: target char_id %d, returnFlag %d"),
		Target_ID, returnFlag);

	// accept PK battle
	if (returnFlag == PK_REQUEST_RESULT_MATCH_BATTLE_REQUEST && GetAction() != ST_BATTLE)
	{
		PKRequest(pPeer, context, Target_ID, MATCH_BATTLE);
	}
	return RPC_RESULT_OK;
}


RPCResult CCharacter::CB_SetEnablePK(LPCPEER pPeer, RPCContext &context, const BYTE &result)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_JoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddHelper(LPCPEER pPeer, RPCContext &context, const UInt32 &Char_id)
{
	// auto-allow join battle
	JoinBattle(pPeer, context, Char_id);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RejectHelper(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_LeaveHelpersList(LPCPEER pPeer, RPCContext &context, const UInt32 &Char_id)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ClearHelpersList(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RequestJoinBattle(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_HelpRequestFail(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RespondTest(LPCPEER pPeer, RPCContext &context, const UInt16 &playerCount)
{
	if (gGlobal.m_pSimClient)
		gGlobal.m_pSimClient->RespondTest(playerCount);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_LearnSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_UseSkill(LPCPEER pPeer, RPCContext &context, const UInt16 &skill_id, const BYTE &returnFlag, const String &Msg, const UInt32 &SrcID, const UInt32 &TargetID)
{
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

RPCResult CCharacter::CB_SkillShopLearn(LPCPEER pPeer, RPCContext &context, const BYTE &returnFlag)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetTarget(LPCPEER pPeer, RPCContext &context, const Boolean &bResult, const UInt32 &char_id, const UInt16 &x, const UInt16 &y)
{
#ifndef REUBEN_PUBLISH
	if (bResult)
	{
		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_SetTarget: Server found char_id %d at x %d y %d"), 
			char_id, x, y);
	}
	else
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_SetTarget: Server cannot find char_id %d"), 
			char_id);
	}
#endif // REUBEN_PUBLISH
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenProcess(LPCPEER pPeer, RPCContext &context, const Boolean &success, const UInt8 &msgIndex)
{
	//if (message == _T("MSG_GIVEN_ITEM_BOUND"))
	if (msgIndex == GIVEN_SUCCESS)
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Error:This item is bound. Item transfer failed.")); 

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenItem(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32Vector &itemID, const UInt8Vector &itemStacks)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenPartner(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PartnerNames)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenPet(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const StringVector &PetNames)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GivenMoney(LPCPEER pPeer, RPCContext &context, const Boolean &GivenBy, const String &targetName, const UInt32 &gmoney)
{

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



RPCResult CCharacter::CB_AskForTrading(LPCPEER pPeer, RPCContext &context, const RPCNetID &askId)
{
	CCharacter* target = FindNetObject<CCharacter>(askId);
	if (!(target))
		return RPC_RESULT_OK;

	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;

	gGlobal.m_pSimClient->m_TarTradeID = target->GetChar_id();
	gGlobal.m_pSimClient->m_getTradReq = TRUE;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("A trade request is raised by character with ID:%d"), 
					target->GetChar_id());
	if ((gGlobal.m_pSimClient->m_bRobot)&&(gGlobal.m_pSimClient->m_pCharacter)&&(gGlobal.m_pSimClient->m_pBatChar == NULL))
	{   
	//	CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "TradeAcceptAction");
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Robot is in use. Automatically accept the trade request:%d"));
		if (gGlobal.m_pSimClient->m_getTradReq)// if get the trade request from the opposite side
		{
			gGlobal.m_pSimClient->m_getTradReq = FALSE;
			if (target)
			{
				gGlobal.m_pSimClient->m_pCharacter->ReplyReqTrading(gGlobal.m_pSimClient->m_pProxyPeer, 
					                     gGlobal.m_pSimClient->m_context, TRUE, target->GetNetId());
				TRACE_INFODTL(GLOBAL_LOGGER, _F("You accept a trade request"));	
			}
		}	
	}

    return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ReplyReqTrading(LPCPEER pPeer, RPCContext &context, const Boolean &trade)
{

	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;	

	if (!trade)
	{
        gGlobal.m_pSimClient->m_bTradReqCfm = FALSE; 
		gGlobal.m_pSimClient->m_TarTradeID = 0; 
		gGlobal.m_pSimClient->m_getTradReq = FALSE;
		gGlobal.m_pSimClient->m_btradelock = FALSE;	
		gGlobal.m_pSimClient->m_btarlock = FALSE;
		//DeleteNetObject(gGlobal.m_pSimClient->m_pTrading);
		if (gGlobal.m_pSimClient->m_pTrading) {
			TRACE_INFODTL_2(GLOBAL_LOGGER, 
				_T("An agreement of trade is destroyed, trading netId %d,%d"), 
				gGlobal.m_pSimClient->m_pTrading->GetNetId().srvId, 
				gGlobal.m_pSimClient->m_pTrading->GetNetId().objId);
		} else {
			TRACE_INFODTL(GLOBAL_LOGGER, _F("An agreement of trade is destroyed, NULL trading object"));
		}
//		if (gGlobal.m_pSimClient->m_bRobot)
//			CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "TradeFreeHold");
	}

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

	TRACE_INFODTL(GLOBAL_LOGGER, _F(""));
	
	return RPC_RESULT_OK;
}


RPCResult CCharacter::CB_TradingObject(LPCPEER pPeer, RPCContext &context, NetObject* trading)
{
    CTrading* pTrading = CastNetObject<CTrading>(trading);
	if (pTrading == NULL || gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;

	if (gGlobal.m_pSimClient->m_pTrading != pTrading)
		DeleteNetObject(gGlobal.m_pSimClient->m_pTrading);
	gGlobal.m_pSimClient->m_pTrading = pTrading;
	gGlobal.m_pSimClient->m_getTradReq = FALSE;
	gGlobal.m_pSimClient->m_bTradReqCfm = TRUE;

	gGlobal.m_pSimClient->m_pTrading->m_tradeItemPos.clear();
	gGlobal.m_pSimClient->m_pTrading->m_tradePartnerPos.clear();
	gGlobal.m_pSimClient->m_pTrading->m_tradePetPos.clear(); 
	TRACE_INFODTL_2(GLOBAL_LOGGER, 
		_T("An agreement of trade is accepted on both sides, trading netId %d,%d"),
		pTrading->GetNetId().srvId, pTrading->GetNetId().objId);
	if (gGlobal.m_pSimClient->m_bRobot)
	{
		if ((gGlobal.m_pSimClient->m_pBag == NULL) ||(gGlobal.m_pSimClient->m_pPartnerGroup == NULL)||(gGlobal.m_pSimClient->m_pPetGroup == NULL))
			return RPC_RESULT_OK;
		int itemSlot = 100;
		int parnterID = 0;
		int petID = 0;
		CPartner *partner = NULL;
		CPet *pet = NULL;
        for (int i=0; i<BAG_ITEM_COUNT; i++) // check all items on the first page
		{
            CItem *item = gGlobal.m_pSimClient->m_pBag->GetItem(i);
			if (item)
			{
				if (item->GetItem_soulboundState() != SOULBOUND_ALREADY_BOUND)
				{
					itemSlot = i;
					break;
				}
			}
		}
        if (gGlobal.m_pSimClient->m_pPartnerGroup)
		{
			for (int i = 0; i < gGlobal.m_pSimClient->m_pPartnerGroup->GetCount(); i++)
			{
				partner = gGlobal.m_pSimClient->m_pPartnerGroup->GetChild<CPartner>(i);
				if (partner)
				{
					if (partner->GetSoulboundState() != SOULBOUND_ALREADY_BOUND)
					{
						parnterID = partner->GetPartner_id();
						break;
					}
				}
			}
		}
        
		if (gGlobal.m_pSimClient->m_pPetGroup)
		{
			for (int i = 0; i < gGlobal.m_pSimClient->m_pPetGroup->GetCount(); ++i)
			{
                pet = gGlobal.m_pSimClient->m_pPetGroup->GetChild<CPet>(i);
				if (pet)
				{
					if (pet->GetSoulbound() != SOULBOUND_ALREADY_BOUND)
					{
						petID = pet->GetPet_uid();
						break;
					}
				}
			}
		}

		CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "TradeAction", String(), itemSlot, 
			petID, parnterID);  
	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_TalkToNPC(LPCPEER pPeer, RPCContext &context, const UInt32 &npc_id, const bool &bresult)
{
//	if (!bresult)
//	{
        TRACE_INFODTL(GLOBAL_LOGGER, _F("Talked Failed."));
		gGlobal.m_pSimClient->m_talkInProcess = FALSE;
		
//	}

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_TitleListUpdate(LPCPEER pPeer, RPCContext &context, const UInt16Vector &title_id_list, const StringVector &title_string_list, const UInt8Vector &updateState)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_StartHawk(LPCPEER pPeer, RPCContext &context, NetGroup* hawkGroup, const UInt8 &successIndex)
{
	if (successIndex == HAWK_START)
	{
		CHawkGroup * pHawkGroup = CastNetObject<CHawkGroup>(hawkGroup);

		if (pHawkGroup == NULL)
			return RPC_RESULT_OK; 
	
		gGlobal.m_pSimClient->m_pMyHawkGroup = pHawkGroup;
		gGlobal.m_pSimClient->m_pMyHawkGroup->ConstructHawkItem();
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Successfully open a hawking business"));	
	}
	else if (successIndex == HAWK_NO_TIME)
	{
		DeleteNetGroup(hawkGroup, BOOLEAN_TRUE);
		TRACE_INFODTL(GLOBAL_LOGGER, _F("Failed to open a hawking business: Zero hawker time"));		
	}	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetHawk(LPCPEER pPeer, RPCContext &context, const Boolean &self, 
								 NetGroup* hawkGroup, NetGroup* itemGroup, 
								 NetGroup* partnerGroup, const UInt16Vector &partnerSkillID,
								 NetGroup* petGroup, const UInt16Vector &petSkillID)
{
	if (gGlobal.m_pSimClient == NULL)
		return RPC_RESULT_OK;
	if (!self)
	{
		//if (gGlobal.g_pTargetHawkGroup && gGlobal.g_pTargetHawkGroup != hawkGroup)
		//	DeleteNetGroup(gGlobal.g_pTargetHawkGroup, BOOLEAN_TRUE);

		CHawkGroup * pHawkGroup = CastNetObject<CHawkGroup>(hawkGroup);
		if (pHawkGroup)
		{
			gGlobal.m_pSimClient->m_pTargetHawkGroup = pHawkGroup;
			gGlobal.m_pSimClient->m_pTargetHawkGroup->ConstructHawkItem();
			DeleteNetGroup(itemGroup);
			DeleteNetGroup(partnerGroup);
			DeleteNetGroup(petGroup);
			TRACE_INFODTL(GLOBAL_LOGGER, _F("Successfully start shopping process over a targeted hawking business"));
			if (gGlobal.m_pSimClient->m_bRobot)
			{  
				HawkItemMap itmlst = gGlobal.m_pSimClient->m_pTargetHawkGroup->GetItemList();
				HawkItemMap petlst = gGlobal.m_pSimClient->m_pTargetHawkGroup->GetPetList();
				HawkItemMap prtnrlst = gGlobal.m_pSimClient->m_pTargetHawkGroup->GetPartnerList();	
				int maxItem = (int) itmlst.size();
				int maxPet = (int) petlst.size();
				int maxPartner = (int) prtnrlst.size();
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "BuyHawkerItem", String(), maxItem, maxPet, maxPartner);
			}
			return RPC_RESULT_OK;
		}
	}
	DeleteNetGroup(hawkGroup, BOOLEAN_TRUE);
	DeleteNetGroup(itemGroup, BOOLEAN_TRUE);
	DeleteNetGroup(partnerGroup, BOOLEAN_TRUE);
	DeleteNetGroup(petGroup, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AntiHack(LPCPEER pPeer, RPCContext &context, const ByteArray &image, const AntiHackData &answerList)
{
	if (gGlobal.m_pSimClient->m_bRobot)
	{  
		//CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "HackAnswer", x, y);
	}
	TRACE_INFODTL(GLOBAL_LOGGER, _F("Server has sinked a Anti-Hack image down to client"));

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ReportAntiHack(LPCPEER pPeer, RPCContext &context, const Boolean &is_right, const UInt8 &cwrong, const UInt16 &reward)
{
	if (is_right)
	{
		TRACE_INFODTL(GLOBAL_LOGGER, _F("The Anti-Hack answer is correct"));
	}
	else
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("The Anti-Hack answer is incorrect. The total no. of wrong answer is %d"), cwrong);
	}
	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_DepositInBank(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{


	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_WithdrawFmBank(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{



	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_HawkMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

void CCharacter::Move(LPCPEER pPeer, RPCContext &context, UInt16 x, UInt16 y)
{
	if (m_pParty && !m_pParty->GetAvail())
		return;

	if (GetMap() && (GetAction() == ST_MOVE || GetAction() == ST_STOP || 
		GetAction() == ST_NOBATTLE))
	{
		Parent::Move(pPeer, context, x, y);
		MoveTo(pPeer, context, x, y, GetHint1(), GetHint2());
	}
}

void CCharacter::GivenProcess(RPCNetID &targetId, UInt8Vector &bagIndices, 
							  UInt8Vector &stackList, UInt32Vector &partnerIdList, 
							  UInt32Vector &petIdList, UInt32 money)
{
	if (gGlobal.m_pSimClient == NULL)
		return;

	String s;
	if (money != 0)
		s.AppendFormat(_T("money %d, "), money);
	if (!bagIndices.empty())
		s.AppendFormat(_T("items %s, "), GetVectorString(bagIndices).c_str());
	if (!partnerIdList.empty())
		s.AppendFormat(_T("partners %s, "), GetVectorString(partnerIdList).c_str());
	if (!petIdList.empty())
		s.AppendFormat(_T("pets %s, "), GetVectorString(petIdList).c_str());
	if (!s.IsEmpty()) {
		TRACE_INFODTL_3(GLOBAL_LOGGER,_F("GivenProcess: target netId %d,%d, %s"), 
			targetId.srvId, targetId.objId, s.c_str());
		Parent::GivenProcess(gGlobal.m_pSimClient->m_pProxyPeer, gGlobal.m_pSimClient->m_context,
				targetId, bagIndices, stackList, partnerIdList, petIdList, money);
	}
}

int CCharacter::GetValidItem()
{
	int itemSlot = 255;	
	for (int i = 0; i < BAG_ITEM_COUNT; i++) // check all items on the first page
	{
		CItem *item = gGlobal.m_pSimClient->m_pBag->GetItem(i);
		if (item)
		{
			if (item->GetItem_soulboundState() != SOULBOUND_ALREADY_BOUND)
			{
				itemSlot = i;
				break;
			}
		}
	}
	return itemSlot;
}

void CCharacter::DisplayHackQuestion(Boolean server, UInt8 direc, UInt8 guy)
{
	Char *face, *side;
	side = server? _T("Server") : _T("Client");
	bool back;
	if ((direc == 1)||(direc == 7))
	{
		back = FALSE;
        face = _T("to you");
	}
	else
	{
		back = TRUE;
		face = _T("back to you");
	}
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("(From %s)Please choose the person who face %s"), side, face);
	for (int i = 1; i < 5; ++i)
	{
		if (guy == i)
			face = back? _T("Back") : _T("Front");
		else
			face = back? _T("Front") : _T("Back");
		
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%d) %s"), i, face);	
	}
}

RPCResult CCharacter::CB_PartyMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgType, const bool &inviteOrJoin, const String &charName)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SendGrpMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &Result, const UInt32 &resultFlag)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_OpenCreateGuild(LPCPEER pPeer, RPCContext &context)
{
	if (gGlobal.m_pSimClient->m_bRobot)
	{
		Char randomName[7];
		randomName[6] = 0;

		for (int i = 0; i < 6; ++i)
			randomName[i] =  gGlobal.m_rand.GetRandom(26) + 65;
		MessageService_Proxy::CreateGuild(randomName);

	}
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_OpenGiftBox(LPCPEER pPeer, RPCContext &context, NetGroup* giftBox)
{
	DeleteNetGroup(giftBox);
	return RPC_RESULT_OK;
}



RPCResult CCharacter::CB_HawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PlyInterestDwn(LPCPEER pPeer, RPCContext &context, const UInt8 &interest)
{


	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpdateHawkHistory(LPCPEER pPeer, RPCContext &context, const HawkerHistoryVector &historys, const StringVector &itemNames, const UInt32Vector &deleteHistoryID)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddictionWarning(LPCPEER pPeer, RPCContext &context, const String &warnMsg)
{

	return RPC_RESULT_OK;

}
RPCResult CCharacter::CB_AddictCurrentStatus(LPCPEER pPeer, RPCContext &context, const String &statusMsg)
{
	return RPC_RESULT_OK;

}
RPCResult CCharacter::CB_AddictionShutDown(LPCPEER pPeer, RPCContext &context, const UInt32 &accOffTime)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UseItemCharacter(LPCPEER pPeer, RPCContext &context, const UInt32 &item_id, const UInt32 &SrcID, const UInt32 &TargetID)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AskEnterQuestion(LPCPEER pPeer, RPCContext &context)
{
	UInt32 r = ::GetRand(100) % 100;
	if(r > 5)
	{
		EnterQuestion(pPeer,context);
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_EnterQuestion(LPCPEER pPeer, RPCContext &context, NetObject* pQuestion)
{
	CQuestion*p = CastNetObject<CQuestion>(pQuestion);
	if(p != NULL)
	{
		gGlobal.m_pSimClient->m_pQuestion = p;
	}
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ExitQuestion(LPCPEER pPeer, RPCContext &context)
{
	DeleteNetObject(gGlobal.m_pSimClient->m_pQuestion);
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_AnswerQuestion(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const UInt32 &CharID)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_OpenWareHouse(LPCPEER pPeer, RPCContext &context, NetGroup* items, NetGroup* partners, NetGroup* pets, NetGroup* partnerSkillGroup, NetGroup* petSkillGroup)
{
	DeleteNetGroup(items, BOOLEAN_TRUE);
	DeleteNetGroup(partners, BOOLEAN_TRUE);
	DeleteNetGroup(pets, BOOLEAN_TRUE);
	DeleteNetGroup(partnerSkillGroup, BOOLEAN_TRUE);
	DeleteNetGroup(petSkillGroup, BOOLEAN_TRUE);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetRobot(LPCPEER pPeer, RPCContext &context, const Boolean &bRobot, 
								  const String &robotScript)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RobotMapAction(LPCPEER pPeer, RPCContext &context, const String &func,
										const RPCNetID &mapNetId, const RPCNetID &charNetId,
										const RPCNetID &otherNetId)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RobotBattleTurn(LPCPEER pPeer, RPCContext &context, const String &func,
										 const RPCNetID &battleNetId, const RPCNetID &charNetId,
										 const RPCNetID &batCharNetId, const RPCNetID &partnerNetId)
{
	return RPC_RESULT_OK;

}

RPCResult CCharacter::CB_AssetProtectResult(LPCPEER pPeer, RPCContext &context, const UInt8 &resultMsg, const UInt32 &index, const UInt8 &type)
{ 
	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &success)
{

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AssetReleaseLockResult(LPCPEER pPeer, RPCContext &context, const UInt8 &resultMsg, const UInt32 &index, const UInt8 &type)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_WarningMessage(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex, const Int32 &value, const UInt8 &showIn)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ForceResetAssetPassword(LPCPEER pPeer, RPCContext &context, const Boolean &bSuccess)
{

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_DonateResult(LPCPEER pPeer, RPCContext &context, const UInt32 &donate, const UInt8 &returnFlag, const UInt32 &donateMoney)
{
	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_SetAutoWalkHitBoss(LPCPEER pPeer, RPCContext &context, const bool& hit)
{
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
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddPartnerCollection(LPCPEER pPeer, RPCContext &context, const UInt32 &MOBid, const BYTE &returnFlag)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER,_F("CCharacter::CB_AddPartnerCollection:MOBid %d,returnFlag%d"),MOBid,returnFlag);
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ChangeBetweenYBandYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RequestTargetEquip(LPCPEER pPeer, RPCContext &context, NetGroup * items)
{
	DeleteNetGroup(items, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, NetGroup * items,const UInt32 &char_id)
{
	DeleteNetGroup(items, BOOLEAN_TRUE);

	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_RelationSystem(LPCPEER pPeer, RPCContext &context, NetObject* pRelationSystem)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AddSellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_BuySellYuanBao(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetAllMoneyAndYBT(LPCPEER pPeer, RPCContext &context, const UInt8 &msgIndex)
{
	return RPC_RESULT_OK;
}












RPCResult CCharacter::CB_LearnLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
	return	RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ModifyLineup(LPCPEER pPeer, RPCContext &context, const UInt16 &uiLineupId)
{
	return	RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ForgiveLineup(LPCPEER pPeer, RPCContext &context, const int &iResault)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ModifyLineupKeyPos(LPCPEER pPeer, RPCContext &context, const int &iKeyPos)
{
	return	RPC_RESULT_OK;
}
RPCResult CCharacter::CB_SetTargetNameFormHawk(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id ,const String &char_name,const Boolean& isonline)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_InitTargetHawkConcernState(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpdateWantedReward(LPCPEER pPeer, RPCContext &context, const UInt32 &targetId, const Int32 &money, const Boolean &result)
{	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_JailDelivery(LPCPEER pPeer, RPCContext &context)
{	
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_WantedBail(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_WantedTrace(LPCPEER pPeer, RPCContext &context, const String &name, const UInt32 &lineId, const UInt32 &mapId, const UInt32 &mapX, const UInt32 &mapY)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RequestWantedTrace(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetGenerationGroup(LPCPEER pPeer, 
											RPCContext &context,
											NetGroup* pGntGrp)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RequestGenerationTrans(LPCPEER pPeer,
												RPCContext &context, 
												const UInt32 &uiRequstId)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RequestGenerationResult(LPCPEER pPeer, 
												 RPCContext &context,
												 const int &iResultCode, 
												 const UInt32 &uiRelatedId) 
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_DeleteGeneration(LPCPEER pPeer, 
										  RPCContext &context, 
										  const int &iResultCode,
										  const UInt32 &uiRelatedId)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ChangeGenerationType(LPCPEER pPeer,
											  RPCContext &context,
											  const int &iType)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_UpdateGenerationAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &uiCurAcc, const UInt32 &uiHisAcc)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_OpenBusinessLog(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_AwardMsg(LPCPEER pPeer, RPCContext &context,const String &msg)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_SetMarriageNpcAction(LPCPEER pPeer, RPCContext &context,const UInt32 &npc_uid,const UInt8 &atcion,const UInt8 &direction)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ShowMarriageEff(LPCPEER pPeer, RPCContext &context,const UInt8 &state,const UInt32 &husbandID,const UInt32 &wifeID)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_SetChangePos(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt8 &ui8PreIndex, const UInt8 &ui8DestIndex)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_BugAccShopItem(LPCPEER pPeer, RPCContext &context, const int &iRetCode, const UInt32 &iIndex)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_GenerationGainAcc(LPCPEER pPeer, RPCContext &context, const UInt32 &iGainedAcc)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ResetChangeLine(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_PetUseSkillMsg(LPCPEER pPeer, RPCContext &context, const String &Msg){	return RPC_RESULT_OK;}RPCResult CCharacter::CB_SearchParty(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList)
{
	return RPC_RESULT_OK;	
}

RPCResult CCharacter::CB_SearchChar(LPCPEER pPeer, RPCContext &context, const CharInfoVector &CharInfoList)
{
	return RPC_RESULT_OK;	
}

RPCResult CCharacter::CB_GetPartyInfo(LPCPEER pPeer, RPCContext &context, const PartyInfoVector &PartyInfoList)
{
	return RPC_RESULT_OK;	
}

RPCResult CCharacter::CB_OpenSearchPartyPanel(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_LeaveTeamTemporarily(LPCPEER pPeer,RPCContext &context, const UInt32 &uRetCode,const UInt32 &uId)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_GetPartyInfoWhenJumpMap(LPCPEER pPeer, RPCContext &context, NetGroup* pParty)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_ReturnParty(LPCPEER pPeer, RPCContext &context, const UInt32 &uRetCode, NetObject* pChar)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RefreshPartyCharInfo(LPCPEER pPeer, RPCContext &context, NetObject* pPartyCharInfo)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_OpenWeaponsRankPanel(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_GetPlayerEquipData(LPCPEER pPeer, RPCContext &context , const UInt32 &ID)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_HandleGuideEvent(LPCPEER pPeer, RPCContext &context, const UInt16 &ID)
{
	return RPC_RESULT_OK;

}

RPCResult CCharacter::CB_GetPlayerEquip(LPCPEER pPeer, RPCContext &context, const UInt32 &char_id)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_RestorePartyAsTemp(LPCPEER pPeer, RPCContext &context, NetGroup* pParty)
{
	return RPC_RESULT_OK;
}

RPCResult CCharacter::CB_ShowIncognitoBoxEff(LPCPEER pPeer, RPCContext &context, const UInt16 &posx, const UInt16 &posy, const UInt8 &effType)
{
	return RPC_RESULT_OK;
}
RPCResult CCharacter::CB_SetChangeModelSkillGroup(LPCPEER pPeer, RPCContext &context, NetGroup* skillGroup)
{
	return RPC_RESULT_OK;
}
