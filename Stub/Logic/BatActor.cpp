//-- Common
#include "Common.h"
#include "Common/common_binary.h"
#include "Common/Procedure.h"
#include "Common/DataCode.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "Stub/Logic/BatActor.h"
//-- Library
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/BatMob.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Proxy/Data/BuffData.h"
#include "Proxy/Service/LogService.h"
#include "MapService/Global.h"
#include "MapService/AnimTimer.h"
#include "Resource/ResSkill.h"
#include "Resource/ResMob.h"
#include "Resource/ResItem.h"
#include "Resource/ResBuff.h"
#include "Resource/ServerResManager.h"
#include "Stub/Logic/Party.h"
#include "Proxy/Logic/Guild.h"

REGISTER_CLASSTYPE(CBatActor, BatActor_CLASSID);

#define WITHIN(A, X, Y)		((A) < (X) ? (X) : ((A) > (Y) ? (Y) : (A)))
#define AUTO_TURN_COUNT		25

PROTO_LuaPush(CBatActor)
PROTO_LuaPush(BatActorLinkLst)
PROTO_LuaPush(CBattle)
PROTO_LuaPush(ExtractedBuffData)
PROTO_LuaPush(actorlist)


CBatActor::CBatActor()
{
	m_pBatChar = NULL;
	m_pMob = NULL;
	m_pBattle = NULL;
	m_pClient = NULL;
	m_pBag = NULL;
	m_command = BATCMD_ATTACK;
	m_cmdTarget = 0;
	m_cmdTargetType = TARGET_ENEMY;
	m_cmdSubType = 0;
	m_bInput = BOOLEAN_FALSE;
	m_bAction = BOOLEAN_FALSE;
	m_isLastHit = BOOLEAN_TRUE;

	m_postSkillID = 0;
	m_postSkillTarget = 0;
	m_postSkillTurn = 0;

	m_tempAction = BATCMD_IDLE;
	m_tempReaction = BATCMD_IDLE;

	m_CancelAction = false;
	m_buffActing = BOOLEAN_FALSE;

	m_CurBuffContAct = 0;
	m_CurContAct = 0;
	ZeroMemory(m_cmdBlockMap, sizeof(m_cmdBlockMap));

	m_protectCount = 0;
	m_ignoreTeam = FALSE;
	m_punish = true;
	m_jumpBack = false;

	m_RDHP = 0;
	m_RDMHP = 0;
	m_RDSP = 0;
	m_RDMSP = 0;
	m_RDDP = 0;
	m_RDVP = 0;
	m_speed = 0.0f;	
	m_inGetFinalValue = false;
	SetPosition(255);

	// not initialized variables before 25/6/2009
	m_ReturnType = 0;
	m_Msg.Empty();
	m_cmdAdditional = 0;

	m_MissSkill = BOOLEAN_FALSE;
	m_BuffActive = BOOLEAN_FALSE;
	m_AI.Empty();
	m_isLastHit = BOOLEAN_FALSE;
	m_ActiveBuffIDs.clear();

	m_BlockSkills.clear();
	m_BuffMap.clear();
	m_BuffCacheMap.clear();

	m_AttriChangeLst.clear();
	m_AttriBuffChangeLst.clear();

	m_postSkillBaseLv = 0;
	m_BoundsExp = 0;
	m_pProtector = NULL;
	m_SkillMsg.Empty();
	m_SkillMap.clear();
}

CBatActor::~CBatActor()
{
	if( m_pBatChar )
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Character %d:%d"), GetNetId().srvId, GetNetId().objId);
	}

	if( m_pMob )
	{
		if( m_pMob->IsParnter() )
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Partner %d:%d"), GetNetId().srvId, GetNetId().objId);
		}else
		{
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Mob %d:%d"), GetNetId().srvId, GetNetId().objId);
		}
	}

	if( m_pBattle )
	{
		TRACE_INFODTL_2(GLOBAL_LOGGER, _F("belongs to Battle %d:%d"), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId);
	}
}

void CBatActor::TurnClear()
{
	//m_bInput = BOOLEAN_FALSE;
	SetIsInput(BOOLEAN_FALSE);
	m_bAction = BOOLEAN_FALSE;
	m_AttriChangeLst.clear();
	m_AttriBuffChangeLst.clear();
	ResetDropValue();
	SetDP(GetDP(), 0, 0);
	m_CancelAction = false;
	SetPet_active(false);
	m_protectCount = 0;
	m_pProtector = NULL;
}

void CBatActor::RestoreDropValue()
{
	SetHP(GetHP() - m_RDHP, 0, 0);
	SetSP(GetSP() - m_RDSP, 0, 0);
	SetMHP(GetMHP() - m_RDMHP, 0, 0);
	SetMSP(GetMSP() - m_RDMSP, 0, 0);
}

void CBatActor::ResetDropValue()
{
	SetHP(GetHP(), 0, 0);
	SetSP(GetSP(), 0, 0);
	SetMHP(GetMHP(), 0, 0);
	SetMSP(GetMSP(), 0, 0);
	SetDP(GetDP(), 0, 0);
	UnmarkUpdate(HPMask);
	UnmarkUpdate(SPMask);
	UnmarkUpdate(mHPMask);
	UnmarkUpdate(mSPMask);
	UnmarkUpdate(DPMask);
	m_RDHP = 0;
	m_RDMHP = 0;
	m_RDSP = 0;
	m_RDMSP = 0;
	m_RDDP = 0;
	m_RDVP = 0;
}

void CBatActor::ResetAction()
{
	// reset not repeat command
	if (m_command != BATCMD_ATTACK && m_command != BATCMD_SKILL &&
		m_command != BATCMD_DEFEND)
	{
		m_command = BATCMD_IDLE;
	}
}

RPCResult CBatActor::SendCommand(LPCPEER pPeer, RPCContext &context, const UInt8 &cmd, 
								 const UInt8 &target, const UInt32 &sub_type, 
								 const UInt32 &additional)
{
	TRACE_ENSURE(m_pBattle != NULL);
	UInt32 errCode = 0;
	String errMsg;

	do
	{
		BatClient* pClient = m_pBattle->GetClient(context.sessionId);
		if (pClient == NULL || !pClient->IsConnect())
		{
			errMsg.Format(_T("invalid client"));
			break;
		}
		else if (cmd != BATCMD_REPEAT && cmd != BATCMD_AUTO && (IsCmdBlocked(cmd, BOOLEAN_TRUE) ||
			(m_pBattle && m_pBattle->IsCmdBlocked(cmd, BOOLEAN_TRUE))))
		{
			errMsg.Format(_T("command %d blocked"), cmd);
			break;
		}
		else if (!m_pBattle->CanInput())
		{
			errMsg.Format(_T("cannot input command at this moment"));
			break;
		}
		else if (m_bInput)
		{
			errMsg.Format(_T("already inputed"));
			break;
		}
		else if (pClient->bAuto && cmd != BATCMD_AUTO)
		{
			errMsg.Format(_T("cannot input command in auto mode"));
			break;
		}

		CBatActor* pTarget = m_pBattle->GetActor(target);
		if(pTarget == NULL)
		{
			errMsg.Format(_T("invalid target"));
			break;
		}

		// check valid target for command
		switch (cmd)
		{
		case BATCMD_IDLE:
			break;

		case BATCMD_ATTACK:
			if (pTarget == NULL)
			{
				errCode = 1;
				errMsg = _T("Attack invalid target");
			}
			break;
		case BATCMD_SPECIAL:
		case BATCMD_SKILL:
			{
				UInt16 skillId = (UInt16) sub_type;
				CSkill* pSkill = GetSkill(skillId);
				if (pSkill == NULL)
				{
					errCode = 1;
					errMsg.Format(_T("actor haven't learn skill %d"), skillId);
				}
			}
			break;

		case BATCMD_DEFEND:
			break;

		case BATCMD_ITEM:
		case BATCMD_WINE:
			{
				UInt8 bagIndex = (UInt8) sub_type;
				CItem* item;
				if (m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
					(item = m_pBag->GetItem(bagIndex)) == NULL || item->GetLock())
				{
					errCode = 1;
					errMsg.Format(_T("invalid item at %d to use"), bagIndex);
				}
				else
				{
					UInt8 target_type = LogService_Proxy::Item::TARGET_UNKNOW;
					UInt32 target_id = 0;
					if (pTarget->IsPlayer())
					{
						target_type = LogService_Proxy::Item::TARGET_PLAYER;
						target_id = pTarget->m_pClient->pOwner->GetChar_id();
					}
					else if (pTarget->IsParnter())
					{
						CBatMob* pMob = CastNetObject<CBatMob>(pTarget);
						target_type = LogService_Proxy::Item::TARGET_PARTNER;
						target_id = pMob->GetPartner_id();
					}
					else if (pTarget->IsMob())
					{
						target_type = LogService_Proxy::Item::TARGET_MOB;
						target_id = pTarget->GetActorId();
					}
					m_pBag->CB_ItemCount(pPeer, context, bagIndex, item->m_tryStack, 
						item->m_tryRemain);
				}
			}
			break;

		case BATCMD_PROTECT:
			if (pTarget == this || pTarget->GetTeam() != GetTeam() ||
				pTarget->IsEscaped())
			{
				errCode = 1;
				errMsg.Format(_T("protect invalid target %d"), target);
			}
			break;

		case BATCMD_ESCAPE:
			break;

		case BATCMD_HELP:
			{
				if (m_pBattle && m_pBattle->GetBattleType() != NPC_BATTLE)
				{
					errCode = 1;
					errMsg.Format(_T("MSG_PVP_NOHELP"));
				}
			}
			break;

		case BATCMD_REPEAT:
			break;

		case BATCMD_CATCH:
			break;

		case BATCMD_FRIEND:
			{
				pClient->pBatChar->m_nextPartnerID = sub_type;					
			}
			break;

		case BATCMD_AUTO:
			{
				if (!pClient->bAuto)
				{
					pClient->bAuto = BOOLEAN_TRUE;
					//pClient->autoCount = AUTO_TURN_COUNT;
					pClient->autoCount = pClient->pOwner->GetInitAutoCount();
					pClient->pBatChar->SetAuto(pClient);
					if (m_pBattle)
					{
						CBatActor * actor = m_pBattle->GetActor(m_cmdTarget);
						if (actor && actor->GetTeam() == GetTeam())
							m_cmdTargetType = 0;
						else
							m_cmdTargetType = 1;
					}
					//check target type
					
					CB_CommandSuccess(pPeer, context, m_command);
				}
				else
				{
					pClient->bAuto = BOOLEAN_FALSE;
					pClient->autoCount = 0;
					pClient->pBatChar->StopAutoEvent();
				}
				CB_AutoCount(pPeer, context, pClient->autoCount);

			}
			break;

		default:
			errCode = 1;
			errMsg.Format(_T("invalid command %d"), cmd);
		}

	} while (false);

	// error return
	if (!errMsg.IsEmpty())
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("actor %d, %s"), 
			GetIndex(), errMsg.c_str());
		CB_Error(pPeer, context, errCode, errMsg);
		return RPC_RESULT_FAIL;
	}

	if (cmd != BATCMD_REPEAT && cmd != BATCMD_AUTO)
	{
		m_command = cmd;
		m_cmdTarget = target;
		m_cmdSubType = sub_type;
		m_cmdAdditional = additional;
	}

	if (IsCmdBlocked(m_command, BOOLEAN_TRUE)||
			(m_pBattle && m_pBattle->IsCmdBlocked(m_command, BOOLEAN_TRUE)))
		m_command = BATCMD_IDLE;
	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);
	if (m_cmdSubType != GetSubType())
		SetSubType(m_cmdSubType);
	if (m_cmdAdditional != GetAdditional())
		SetAdditional(m_cmdAdditional);

	// notify back to Battle
	if (cmd != BATCMD_AUTO)
	{
		CB_CommandSuccess(pPeer, context, m_command);
		if (cmd == BATCMD_REPEAT)
			AutoInput();
		m_pBattle->NotifyInput(this);
	}

	return RPC_RESULT_OK;
}

UInt16 CBatActor::GetRandomSkill(int skill_form)
{
	

	StlVector<UInt16> m_UseSkills;
	for (SkillMap::iterator Itr = m_SkillMap.begin(); Itr != m_SkillMap.end(); ++Itr)
	{
		const SkillData* pSkillData = gGlobal.m_resource->GetSkillData(Itr->second->GetSkill_id());
		if (pSkillData && pSkillData->battleorField == SKILL_USE_TIME_BATTLE
			&& pSkillData->active == SKILL_ACTIVE
			&& (skill_form == -1 || pSkillData->form == skill_form))
		{
			m_ReturnType = 0;

			/*if (Itr->second->GetSkill_level() < pSkillData->reqBaseSkillRank ||
			GetLevel() < pSkillData->reqLv)
			continue;
			*/

			UInt16	skillBaseID = pSkillData->id;

			if (pSkillData->reqBaseSkill != 0)
				skillBaseID = pSkillData->reqBaseSkill;
			StringA LuaFunc;
			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second->GetSkill_id(), "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this, 
					GetSkillLv(skillBaseID), pSkillData->reqBaseSkillRank,
					pSkillData->reqLv);

			if (m_ReturnType == 0)
				m_UseSkills.push_back(pSkillData->id);
		}
	}

	if (m_UseSkills.size() > 0)
		return m_UseSkills[rand() % m_UseSkills.size()];
	else
		return 0;
}

void CBatActor::InputCommand(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional_type)
{
	TRACE_ENSURE(m_pBattle != NULL);

	TRACE_INFODTL_7(GLOBAL_LOGGER, _F("CBatActor::InputCommand: battle %d:%d Actor %d cmd %d, target %d, subtype %d additional %d"), 
		m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, GetIndex(), cmd, target, sub_type, additional_type);

	if (cmd != BATCMD_REPEAT && cmd != BATCMD_AUTO)
	{
		m_command = cmd;
		m_cmdTarget = target;
		m_cmdSubType = sub_type;
		m_cmdAdditional = additional_type;
	}
	if (IsCmdBlocked(m_command, BOOLEAN_TRUE) ||
		(m_pBattle && m_pBattle->IsCmdBlocked(m_command, BOOLEAN_TRUE)))
		m_command = BATCMD_IDLE;
	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);
	if (m_cmdSubType != GetSubType())
		SetSubType(m_cmdSubType);
	if (m_cmdAdditional != GetAdditional())
		SetAdditional(m_cmdAdditional);

	if (cmd == BATCMD_SKILL)
	{
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(sub_type);
		if (pSkillData && pSkillData->targetType == TARGET_TYPE_SELF)
		{
			m_cmdTarget = GetIndex();
			if (m_cmdTarget != GetTarget())
				SetTarget(m_cmdTarget);
		}
	}
	if (m_pBattle && m_pBattle->m_talkNum == 0 && m_pClient && m_pClient->GetPeer())
		CB_CommandSuccess(m_pClient->GetPeer(), 
			m_pClient->GetContext(), m_command);

	SetIsInput(BOOLEAN_TRUE);
	if (m_pBattle && m_pBatChar )
		m_pBattle->NotifyInput(this);
	
	//m_bInput = BOOLEAN_TRUE;
}

void CBatActor::TestChangeAttri(int type, int value)
{
	if ( type == 1 )
	{
		SetHP(GetHP() + value, value, false);
	}
}

void CBatActor::ChangePlayerBatCmd(UInt8 cmd, UInt8 target, UInt32 sub_type, UInt32 additional_type)
{
	if (cmd != BATCMD_REPEAT)
	{
		m_command = cmd;
		m_cmdTarget = target;
		m_cmdSubType = sub_type;
		m_cmdAdditional = additional_type;
	}
	SetIsInput(BOOLEAN_TRUE);
	//m_bInput = true;
}

RPCResult CBatActor::CheatCode(LPCPEER pPeer, RPCContext &context, const String &code)
{
	if (m_pClient == NULL || m_pClient->pOwner == NULL || !m_pClient->pOwner->CanCheat() || m_pClient->pOwner->IsInMap())
	{
		TRACE_WARNDTL_2(GLOBAL_LOGGER, _FW("can't cheat, actor index %d, code %s"), GetIndex(), code.c_str());
		return RPC_RESULT_OK;
	}

	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("battle %d:%d:%d Actor %d CheatCode: %s"), 
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, GetIndex(), code.c_str());

	TCHAR *delim = _T(" ");
	TCHAR *cmd = NULL; 
	TCHAR *arg1 = NULL;
	TCHAR *arg2 = NULL;
	TCHAR *buf = SafeAllocate(TCHAR, code.size() + 1);
	CBatActor *pActor = this;

	_tcscpy(buf, code.c_str());
	cmd = _tcstok(buf, delim);
	
	if (cmd && cmd[0] >= _T('0') && cmd[0] <= _T('9')) {		// specify actor index
		UInt8 index = _ttoi(cmd);
		CBatActor *pActor = m_pBattle->GetActor(index);
		cmd = _tcstok(buf, delim);
	}
	if (cmd == NULL) {
		SafeDeallocate(buf);
		return RPC_RESULT_FAIL;
	}

	if (_tcsicmp(cmd, _T("hp")) == 0) { 
		arg1 = _tcstok(NULL, delim);
		if (arg1) {
			UInt16 hp = _ttoi(arg1);
			pActor->SetHP(hp, 0, 0);
			if (hp > 0 && pActor->IsDead())
				pActor->SetState(CHAR_ST_NORM);
			else if (hp == 0 && pActor->GetState() == CHAR_ST_NORM)
			{
				pActor->SetState(CHAR_ST_DEAD);
				pActor->RemoveBuffAfterDead();
			}
		}
	} else if (_tcsicmp(cmd, _T("sp")) == 0) { 
		arg1 = _tcstok(NULL, delim);
		if (arg1) {
			UInt16 sp = _ttoi(arg1);
			pActor->SetSP(sp, 0, 0);
		}		
	} else if (_tcsicmp(cmd, _T("dp")) == 0) { 
		arg1 = _tcstok(NULL, delim);
		if (arg1) {
			UInt16 dp = _ttoi(arg1);
			pActor->SetDP(dp);
		}
	}
	else if (_tcsicmp(cmd, _T("state")) == 0) { 
		arg1 = _tcstok(NULL, delim);
		if (arg1) {
			if (_tcsicmp(arg1, _T("norm")) == 0) {
				pActor->SetState(CHAR_ST_NORM);
			} else if (_tcsicmp(arg1, _T("dead")) == 0) {
				pActor->SetState(CHAR_ST_DEAD);
				pActor->RemoveBuffAfterDead();
			}
		}
	}
	else if (_tcsicmp(cmd, _T("partner")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);		

		if (arg1 && arg2)
		{
			if ( _tcsicmp(arg1, _T("remove")) == 0 )
			{
				if (GetOwner() && GetOwner()->GetPartners())
				{
					UInt32 mobID = _ttoi(arg2);
					if (mobID > 0)
					{
						GetOwner()->GetPartners()->RemovePartner(mobID);
					}
				}
			}
		}
	}
	else if (_tcsicmp(cmd, _T("manor")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			if( _tcsicmp(arg1, _T("scale")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && GetOwner()->m_ownGuild)
				{
					Int32 scale = _ttoi(arg1);
					CGuild *pGuild = GetOwner()->m_ownGuild;
					if (pGuild)
                        pGuild->SetNewManorScale(scale);
				}
			}
		}
	}

	SafeDeallocate(buf);
	return RPC_RESULT_OK;
}

bool CBatActor::TryAction()
{
	if (IsCmdBlocked(m_command) ||
		(m_pBattle && m_pBattle->IsCmdBlocked(m_command)))
		return false;

	switch (m_command)
	{
	case BATCMD_ATTACK:
		return true;
		break;
	case BATCMD_DEFEND:
		return true;
		break;
	case BATCMD_PROTECT:
		return true;
		break;

	case BATCMD_SPECIAL:
	case BATCMD_SKILL:
		{
			const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(m_cmdSubType);
			if (pSkillData && pSkillData->Skill_AutoSwitch)
				return true;
			else
				return false;
		}
		break;

	//case BATCMD_SPECIAL:
	//	return true;
	//	break;

	case BATCMD_ITEM:
		{
			UInt8 bagIndex = (UInt8)m_cmdSubType;
			CItem* item;
			// if can't use item, then auto attack
			if (m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
				(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
			{
				return false;
			}

			CBatActor *pTarget = m_pBattle->GetActor(m_cmdTarget);
			if(pTarget == NULL)
				return true;
			gGlobal.g_TempResult = 1;
			CallLuaFunc(gGlobal.g_pLuaState, validActionTargetStr.c_str(), String(), this, pTarget);
			if (pTarget == NULL || !pTarget->IsValidTarget(BOOLEAN_FALSE) ||
				gGlobal.g_TempResult == 0)
				return false;
			if (item && !item->CanUse(this, pTarget, String()))
				return false;

			return true;
		}
		break;

	case BATCMD_FRIEND:
		return true;
		break;

	case BATCMD_CATCH:
		return true;
		break;

	case BATCMD_ESCAPE:
		return true;
		break;

	case BATCMD_HELP:
		return true;
		break;

	case BATCMD_AUTO:
		return true;
		break;
	}

	return false;
}

void CBatActor::DoAction()
{
	//activate action buff before start
	if (m_pBattle == NULL|| IsCmdBlocked(m_command) || 
		(m_pBattle && m_pBattle->IsCmdBlocked(m_command)))
	{
		SendBattleMsg(BATTLE_ERROR_MSG_BLOCK_ACTION, _T(""), 0);
		return;
	}

	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);

	SetAction(m_command);
	SetSubType(m_cmdSubType);

	if (!GetLeftBattle() && !IsDead())
		DoBuffActive(NULL, BUFF_ACTIVE_TIME_TYPE_DO_ACTION);

	// safe guard, should fix this bug
	if (m_cmdTarget >= m_pBattle->m_actors.size())
		m_cmdTarget = 0;

	if (m_command != BATCMD_DEFEND && m_command != BATCMD_FRIEND)
	{
		CBatActor *pTarget = m_pBattle->GetActor(m_cmdTarget);
		if (pTarget && !pTarget->GetLeftBattle() && !pTarget->IsDead())
			pTarget->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_LEAD_REACTION);
	}

	// switch for action command
	switch (m_command)
	{
	case BATCMD_ATTACK:
		DoAttack();
		break;

	case BATCMD_SKILL:
		DoSkill();
		break;

	case BATCMD_SPECIAL:
		DoSpecial();
		break;

	case BATCMD_ITEM:
		DoItem();
		break;

	case BATCMD_FRIEND:
		DoFriend();
		break;

	case BATCMD_CATCH:
		DoCatch();
		break;

	case BATCMD_ESCAPE:
		DoEscape();
		break;

	case BATCMD_HELP:
		DoHelp();
		break;
	case BATCMD_WINE:
		DoWine();
		break;
	}

	//activate Reaction buff after finish
	if (m_command != BATCMD_SKILL && m_command != BATCMD_DEFEND && 
		m_command != BATCMD_FRIEND)
	{
		CBatActor *pTarget = m_pBattle->GetActor(m_cmdTarget);
		if (pTarget)
		{
			if (!pTarget->GetLeftBattle())
				pTarget->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_REACTION);
		}
	}

	//reset all protector and drop value and check dead status
	for (Actors::iterator it = m_pBattle->m_actors.begin(); it != m_pBattle->m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor == NULL || actor->GetLeftBattle()) 
			continue;		
		actor->ResetDropValue();
		actor->m_pProtector = NULL;
	}
	
}

void CBatActor::PreSend()
{
	ClearSendMask();
	m_sendMask.MaskBits(indexMask, HPMask, mHPMask, maxHPMask, SPMask, mSPMask, maxSPMask, 
		stateMask, buffMask, buffCacheMask, teamMask, positionMask, levelMask, petMask, leftBattleMask,  -1);
}

void CBatActor::PreSendEnemy()
{
	ClearSendMask();
	m_sendMask.MaskBits(indexMask,      
		stateMask, buffMask, buffCacheMask, teamMask, positionMask, levelMask, petMask, leftBattleMask,   -1);
}

Boolean CBatActor::IsValidTarget(Boolean bAlive)
{
	if (m_pMob && IsParnter() && m_pMob->IsNew())
		return BOOLEAN_FALSE;

	return (!GetLeftBattle() && (!bAlive || !IsDead()) && 
		GetState() != CHAR_ST_ESCAPED && GetState() != CHAR_ST_BACKUP && 
		GetState() != CHAR_ST_CATCHED );
}

void CBatActor::WriteAction()
{
	if (m_pBattle == NULL)
		return;

	//check dead status before write buffer
	CheckDeadStatus();
	RPCMask mask;						// set mask for writing buffer

	mask.MaskBits(actionMask, targetMask, attrib2Mask,  stateMask, subTypeMask, linkNextMask, leftBattleMask, HPMask, mHPMask, SPMask, mSPMask, buffMask, buffCacheMask, 
		hitTogetherMask,attrib1Mask, -1);
	mask &= m_updateMask;				// only update updated attributes
	m_pBattle->m_turnBuffer1->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBuffer2->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBufferMon->Write<UInt8>(GetIndex());

	if (GetTeam() == ENEMY_TEAM)
	{
		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
	}
	else
	{
		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
	}
	

	UnmarkUpdate(mask);			// clear update mask after writing buffer

	if (!IsPlayer())
		ResetDropValue();
}

void CBatActor::WriteReaction()
{
	//check dead status before write buffer
	CheckDeadStatus();
	//reset the protector
	m_pProtector = NULL;
	
	RPCMask mask;						// set mask for writing buffer

	mask.MaskBits(HPMask, mHPMask, SPMask, mSPMask, stateMask, attrib2Mask, 
		reactionMask, targetMask, subTypeMask, linkNextMask, leftBattleMask, buffMask, buffCacheMask, pet_activeMask, hitTogetherMask, 
		attrib1Mask,-1);
	mask &= m_updateMask;				// only update updated attributes
	m_pBattle->m_turnBuffer1->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBuffer2->Write<UInt8>(GetIndex());
	m_pBattle->m_turnBufferMon->Write<UInt8>(GetIndex());

	//check miss but get hurt 
	if (IsUpdated(HPMask) && IsUpdated(reactionMask) && GetReaction() == BATCMD_MISS && GetDShowHP() != 0)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("CBatActor WriteReaction Error: miss but show hurt: DHP: %d DShowHP: %d"),
			GetDHP(), GetDShowHP());
	}

	if (GetTeam() == ENEMY_TEAM)
	{
		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		UInt32 orgMHP = GetMHP();
		UInt32 orgMSP = GetMSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);
		SetMHP(0);
		SetMSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
		SetMHP(orgMHP);
		SetMSP(orgMSP);
	}
	else
	{
		WriteAttr(*m_pBattle->m_turnBuffer2, mask);
		UInt32 orgHP = GetHP();
		UInt32 orgSP = GetSP();
		UInt32 orgMHP = GetMHP();
		UInt32 orgMSP = GetMSP();
		if (GetHP() > 0)
			SetHP(1);
		else
			SetHP(0);
		SetSP(0);
		SetMHP(0);
		SetMSP(0);

		WriteAttr(*m_pBattle->m_turnBuffer1, mask);
		WriteAttr(*m_pBattle->m_turnBufferMon, mask);
		
		SetHP(orgHP);
		SetSP(orgSP);
		SetMHP(orgMHP);
		SetMSP(orgMSP);
	}
	UnmarkUpdate(mask);					// clear update mask after writing buffer

	if (!IsPlayer())
		ResetDropValue();
	
}

// find next valid target
CBatActor* CBatActor::NextTarget(Boolean bEnemy, Boolean bSelfInc)
{
	TRACE_ENSURE(m_pBattle != NULL);

	CBatActor *pTarget = NULL;
	int count = (int) m_pBattle->m_actors.size();
	int rand = GetRand(count);

	// loop to find valid target, start from random index
	for (int i = rand; i < rand + count; i++)
	{
		CBatActor* actor = m_pBattle->GetActor(i % count);
		if(actor == NULL)
			continue;
		gGlobal.g_TempResult = 1;
		CallLuaFunc(gGlobal.g_pLuaState, validActionTargetStr.c_str(), String(),this, actor);
		if (actor->IsValidTarget() && gGlobal.g_TempResult && (bEnemy ^ (actor->GetTeam() == GetTeam()))) {
			if (!bSelfInc && this == actor)
				continue;
			pTarget = actor;
			m_cmdTarget = i % count;
			SetTarget(m_cmdTarget);
			break;
		}
	}

	// no valid target
	return pTarget;
}

float CBatActor::CalcDamage(CBatActor *pTarget, UInt32 atk, Boolean bAction, Boolean bBreak)
{
	gGlobal.g_TempResult = 0;
	pTarget->m_tempReaction = BATCMD_NORM_HIT;
	CallLuaFunc(gGlobal.g_pLuaState, "CalcDamage", String(),this, pTarget);
	pTarget->SetReaction(pTarget->m_tempReaction);
	return (float)gGlobal.g_TempResult;

	/*float dmg;
	UInt8 reaction = BATCMD_NORM_HIT;

	// calculate physical damage if break-through attack
	if (GetTeam() == pTarget->GetTeam()) {
	dmg = 1.0f;
	} else if ((Int64)atk  + (GetMSP() / 100 * 3 - pTarget->GetMSP() / 100 * 2) <= (Int64)pTarget->GetDEF()) {
	dmg = GetRand(1.0f, 10.0f);
	} else if (bBreak && (float(atk) / float(pTarget->GetDEF()) > 1.2f) && GetRand(1.0f) < 0.05f) {
	dmg = (float(atk) - float(pTarget->GetDEF() / 2) + (GetMSP() / 100 * 3 - pTarget->GetMSP() / 100 * 2)) * GetRand(0.9f, 1.1f);
	reaction = BATCMD_BREAK_HIT;
	} else {
	dmg = float(atk - pTarget->GetDEF() + (GetMSP() / 100 * 3 - pTarget->GetMSP() / 100 * 2)) * GetRand(0.9f, 1.1f);
	}

	if (bAction) 
	pTarget->SetReaction(reaction);

	return dmg;
	*/
}


Boolean CBatActor::IsHitTarget(CBatActor *pTarget)
{

	if (!pTarget)
		return BOOLEAN_FALSE;
	if (pTarget->GetTeam() == GetTeam())
		return BOOLEAN_TRUE;

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "CanHit", String(),this, pTarget);
	return gGlobal.g_TempResult > 0;
	/*float hitRate = 0.35f + (float(GetHIT()) / float(pTarget->GetEVA()) - 0.1f) / 3.158f;
	hitRate = WITHIN(hitRate, 0.35f, 0.95f);
	return (GetRand(1.0f) < hitRate);
	*/
}

void CBatActor::HitAttack(CBatActor* pTarget, CBatActor* &protector, UInt32 atk, Boolean bBreak, Boolean bCritHit)
{

	//if the target is the same team of the player, then the dmg will be 1 and no protect or no defend action perform
	/*if (pTarget->GetTeam() == GetTeam())
	{
	float dmg = 1.0f;
	pTarget->ApplyDamage((Int16) dmg);
	pTarget->m_tempReaction = BATCMD_NORM_HIT;
	pTarget->SetReaction(pTarget->m_tempReaction);
	}
	else
	*/
	{

		// if someone protect this actor
		protector = m_pBattle->GetProtector(this, pTarget, bCritHit);
		float dmg = CalcDamage(pTarget, atk, BOOLEAN_TRUE, bBreak);

		float dmg_prot = 0.0f;
		if (bCritHit) dmg *= 1.5f;
		if (protector) {
			dmg_prot = CalcDamage(protector, atk, BOOLEAN_FALSE, bBreak);
			if (protector != pTarget)
			{
				protector->SetReaction(BATCMD_PROTECT_HIT);
				protector->SetTarget(pTarget->GetIndex());
			}
			if (bCritHit) dmg_prot *= 1.5f;
			dmg *= 0.20f;
			dmg_prot *= 0.75f;
		}

		// add critical hit effect
		if (bCritHit) 
		{
			SetAction(BATCMD_CRIT_ATTACK);
			pTarget->SetReaction(BATCMD_CRIT_HIT);
		}

		if (pTarget->m_command == BATCMD_DEFEND)
		{
			dmg *= 0.5f;
			pTarget->SetReaction(BATCMD_DEFEND_HIT);
		}

		if ((Int16) dmg <= 0)
		{
				dmg = 1;
		}

		if ((Int16) dmg_prot <= 0)
		{
			dmg_prot = 1;
		}

		// apply damage to target
		pTarget->ChangeHP((Int16) -dmg);
		if (protector && protector != pTarget) {			
			protector->m_bAction = BOOLEAN_TRUE;
		}
	}
}

void CBatActor::DoDefaultAttack(CBatActor * pBatTarget, int Dam, int crit)
{
	TRACE_ENSURE(m_pBattle != NULL);

	if (!IsValidTarget())
		return;

	if (pBatTarget->GetIndex() != GetTarget())
		SetTarget(pBatTarget->GetIndex());

	CBatActor *pTarget = pBatTarget;
	if (pTarget == NULL || !pTarget->IsValidTarget()) {
		return;
	}
	UInt8 OrgActID = GetAction();


	pTarget->ChangeHP((Int16) -Dam);

	if (crit)
	{
		SetAction(BATCMD_CRIT_ATTACK);
		pTarget->SetReaction(BATCMD_CRIT_HIT);
	}
	else
	{
		SetAction(BATCMD_ATTACK);
		pTarget->SetReaction(BATCMD_NORM_HIT);
	}

	
	SetTempAction(GetAction());
	pTarget->SetTempReaction(pTarget->GetReaction());
	DoBuffActive(pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
	SetAction(m_tempAction);
	pTarget->SetReaction(pTarget->m_tempReaction);

	SetTempAction(GetAction());
	pTarget->SetTempReaction(pTarget->GetReaction());
	pTarget->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
	SetAction(m_tempAction);
	pTarget->SetReaction(pTarget->m_tempReaction);

	// mark source action attack and target
	MarkUpdate(targetMask);
	SetLinkNext(BOOLEAN_FALSE);

	AttackAnimTime(pTarget);		// calculate attack animation time
	// write this actor, target or protector action/reaction to buffer
	WriteAction();

	pTarget->SetLinkNext(BOOLEAN_FALSE);
	pTarget->WriteReaction();

	SetAction(OrgActID);

}

void CBatActor::DoAttack()
{
	TRACE_ENSURE(m_pBattle != NULL);

	if (!IsValidTarget())
	{
		SetAction(BATCMD_IDLE);
		return;
	}

	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);

	CBatActor *pTarget = m_pBattle->GetActor(m_cmdTarget);
	if(pTarget == NULL)
		return;

	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, validActionTargetStr.c_str(), String(), this, pTarget);
	if (pTarget == NULL || !pTarget->IsValidTarget() ||
		gGlobal.g_TempResult == 0) {
			if (pTarget == NULL)
				pTarget = NextTarget();
			else
				pTarget = NextTarget(pTarget->GetTeam() != GetTeam());
			if (pTarget == NULL)
			{
				SetAction(BATCMD_IDLE);
				return;
			}
		}
		SetAction(BATCMD_ATTACK);
		CBatActor* protector = NULL;

		// check hit
		if (!IsHitTarget(pTarget)) {
			// attack miss
			pTarget->SetReaction(BATCMD_MISS);
		} else {
			// check critical hit
			//float critHitRate = (float(GetHP()) / float(GetMHP()) <= 0.25f) ? 0.10f : 0.05f;
			//bool bCritHit = GetRand(1.0f) < critHitRate;
			gGlobal.g_TempResult = 0;
			CallLuaFunc(gGlobal.g_pLuaState, "CanCritical", String(), this, pTarget);
			bool bCritHit = gGlobal.g_TempResult > 0;
			HitAttack(pTarget, protector, GetATK(), BOOLEAN_TRUE, bCritHit);
		}

		/*pTarget->SetTempReaction(pTarget->GetReaction());
		DoBuffActive(buffer, pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
		pTarget->DoBuffActive(buffer, this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
		*/
		SetTempAction(GetAction());
		pTarget->SetTempReaction(pTarget->GetReaction());
		DoBuffActive(pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
		SetAction(m_tempAction);
		pTarget->SetReaction(pTarget->m_tempReaction);

		SetTempAction(GetAction());
		pTarget->SetTempReaction(pTarget->GetReaction());
		pTarget->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
		SetAction(m_tempAction);
		pTarget->SetReaction(pTarget->m_tempReaction);

		if (m_CancelAction)
		{
			m_CancelAction = false;
			m_bAction = BOOLEAN_TRUE;
			return;
		}
		// mark source action attack and target
		MarkUpdate(targetMask);
		SetLinkNext(BOOLEAN_FALSE);

		AttackAnimTime(pTarget);		// calculate attack animation time

		// write this actor, target or protector action/reaction to buffer
		WriteAction();
		if (protector && protector != pTarget) {
			protector->SetLinkNext(BOOLEAN_TRUE);
			protector->WriteReaction();
		}
		
		if (m_pBattle)
			m_pBattle->WriteExtraActor();
		
		pTarget->SetLinkNext(BOOLEAN_FALSE);
		pTarget->WriteReaction();
		m_bAction = BOOLEAN_TRUE;
}

void CBatActor::DoComboAttack(Actors &fellowList)
{
	TRACE_ENSURE(m_pBattle != NULL);

	if (!IsValidTarget())
	{
		SetAction(BATCMD_IDLE);
		return;
	}

	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);

	CBatActor *pTarget = m_pBattle->GetActor(m_cmdTarget);
	if (pTarget == NULL || !pTarget->IsValidTarget())
	{
		SetAction(BATCMD_IDLE);
		return;
	}

	// add all attack and link up all attack actions
	UInt16 atk = GetATK();
	//float dmg = CalcDamage(pTarget, atk, BOOLEAN_TRUE, BOOLEAN_FALSE);

	//form list
	pTarget->SetReaction(BATCMD_NORM_HIT);
	actorlist * pActorListHead = SafeCreateObject(actorlist);
	pActorListHead->actor = this;
	actorlist * pActorListCurr = pActorListHead;
	for (Actors::iterator it = fellowList.begin();
		it != fellowList.end(); ++it)
	{
		CBatActor *pActor = FindNetObject<CBatActor>(*it);
		//atk += pActor->GetATK();
		//dmg += CalcDamage(pTarget, pActor->GetATK(), BOOLEAN_TRUE, BOOLEAN_FALSE);
		/*pActor->SetAction(BATCMD_ATTACK);
		pActor->MarkUpdate(targetMask);
		pActor->SetLinkNext(BOOLEAN_TRUE);		// link up all attack
		pActor->WriteAction(buffer);			// write action to buffer
		pActor->m_bAction = BOOLEAN_TRUE;
		*/

		if(pActor == NULL)
			continue;

		if (pActorListCurr->actor == NULL)
			pActorListCurr->actor = pActor;
		else
		{
			actorlist * pActorList = SafeCreateObject(actorlist);
			pActorList->actor = pActor;
			pActorListCurr->next = pActorList;
			pActorListCurr = pActorList;
		}

	}

	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "ComboAttack", String(), pActorListHead, pTarget);
	float dmg = (float)gGlobal.g_TempResult;

	pActorListCurr = pActorListHead;
	while (pActorListCurr != NULL)
	{
		actorlist * temp = pActorListCurr;
		pActorListCurr = pActorListCurr->next;
		SafeDeleteObject(temp);
	}
	CBatActor* protector = NULL;

	if (fellowList.size() > 0)
	{
		//dmg *= (10.0f + (fellowList.size() + 1))/10.0f;

		if ((Int16) dmg == 0)
			dmg = 1;

		pTarget->ChangeHP((Int16)- dmg);
		pTarget->SetReaction(BATCMD_NORM_HIT);
	}
	else
		HitAttack(pTarget, protector, atk, BOOLEAN_FALSE, BOOLEAN_FALSE);
	// then apply the attack


	/*pTarget->SetTempReaction(pTarget->GetReaction());
	DoBuffActive(buffer, pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
	pTarget->DoBuffActive(buffer, this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
	*/
	SetTempAction(GetAction());
	pTarget->SetTempReaction(pTarget->GetReaction());
	DoBuffActive( pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
	SetAction(m_tempAction);
	pTarget->SetReaction(pTarget->m_tempReaction);

	SetTempAction(GetAction());
	pTarget->SetTempReaction(pTarget->GetReaction());
	pTarget->DoBuffActive( this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
	SetAction(m_tempAction);
	pTarget->SetReaction(pTarget->m_tempReaction);

	SetAction(BATCMD_ATTACK);
	MarkUpdate(targetMask);
	SetLinkNext(BOOLEAN_TRUE);					// last link of attack
	AttackAnimTime(pTarget);		// calculate attack animation time
	WriteAction();						// write action to buffer
	m_bAction = BOOLEAN_TRUE;

	for (Actors::iterator it = fellowList.begin();
		it != fellowList.end(); ++it)
	{
		CBatActor *pActor = FindNetObject<CBatActor>(*it);
		if(pActor == NULL)
			continue;
		//atk += pActor->GetATK();
		//dmg += CalcDamage(pTarget, pActor->GetATK(), BOOLEAN_TRUE, BOOLEAN_FALSE);
		pActor->SetAction(BATCMD_ATTACK);
		pActor->MarkUpdate(targetMask);

		if (it + 1 == fellowList.end())
			pActor->SetLinkNext(BOOLEAN_FALSE);		// link up all attack
		else
			pActor->SetLinkNext(BOOLEAN_TRUE);		// link up all attack
		pActor->WriteAction();			// write action to buffer
		pActor->m_bAction = BOOLEAN_TRUE;
	}


	if (protector && protector != pTarget) protector->WriteReaction();
	pTarget->WriteReaction();
}

void CBatActor::CancelAction()
{
	//m_command = 0;
	m_CancelAction = true;
}

void CBatActor::DoSkill()
{
	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("Start battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);
	bool postSkill = HasPostSkill();
	
	UInt16 skillID = m_cmdSubType;
	UInt8 target = m_cmdTarget;
	
	if (postSkill)
	{
		skillID = m_postSkillID;
		target  = m_postSkillTarget;
		ClearPostSkill();
	}
	else 
	{
		if (m_SkillMap.find(skillID) == m_SkillMap.end())
		{
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _FE("actor %d:%d:%d, actor_id %d, skill_id %d"),
				GetID(), GetNetId().objId, GetNetId().srvId, GetActorId(), skillID);
			return;
		}
	}

    if (!IsValidTarget())
	{
		return;
	}

	m_CancelAction = false;

	if (target != GetTarget())
		SetTarget(target);
	if (skillID != GetSubType())
		SetSubType(skillID);

	Actors targetList;

	const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(skillID);

	if (pSkillData == NULL)
	{
		m_command = BATCMD_IDLE;
		return;
	}
	m_Msg = _T("");

	UInt16	skillBaseID = skillID;

	if (pSkillData->reqBaseSkill != 0)
		skillBaseID = pSkillData->reqBaseSkill;

	BlockSkillMap::iterator Itr = m_BlockSkills.find(skillID);
	if (Itr != m_BlockSkills.end())
	{
		SendBattleMsg(SKILL_USE_FAIL_TYPE_BLOCK, m_Msg, GetSubType());
		m_command = BATCMD_IDLE;
		return;
	}

	if (pSkillData->form != SKILL_FORM_TYPE_RELIEF)
	{
		if (!IsValidTargetType(pSkillData->targetType, m_pBattle->GetActor(target)))
		{
			SendBattleMsg(SKILL_USE_FAIL_TYPE_TARGET_INVALID, m_Msg, GetSubType());
			m_command = BATCMD_IDLE;
			return;
		}
	}

	CBatActor *pTarget = m_pBattle->GetActor(target);

	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, validActionTargetStr.c_str(), String(), this, pTarget);
	if (pTarget == NULL || (!pTarget->IsValidTarget() && 
		pSkillData->form != SKILL_FORM_TYPE_RELIEF) || gGlobal.g_TempResult == 0)
	{
		pTarget = NextTarget(pTarget->GetTeam() != GetTeam(), 
			pSkillData->form == SKILL_FORM_TYPE_HEAL ||
			pSkillData->form == SKILL_FORM_TYPE_BUFF ||
			pSkillData->form == SKILL_FORM_TYPE_CLEAR_BUFF ||
			pSkillData->form == SKILL_FORM_TYPE_SUPPORT);
		
		if (pTarget == NULL)
		{
			SendBattleMsg(SKILL_USE_FAIL_TYPE_NO_TARGET, m_Msg, GetSubType());
			m_command = BATCMD_IDLE;
			return;
		}
	}

	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("After Calling validActionTarget battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);
	StringA LuaFunc;
	LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(skillID, "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_CANUSE].c_str());

	m_ReturnType = 0;
	
	if (pSkillData != NULL && !postSkill && HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
	{
		CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this, 
			GetSkillLv(skillBaseID), pSkillData->reqBaseSkillRank, pSkillData->reqLv);
	}

	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("After Calling canuse battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);
	//fail skill
	if (GetReturnType() > 0)
	{
		SendBattleMsg(BATTLE_ERROR_MSG_GIVEN, m_Msg, GetSubType());
		m_command = BATCMD_IDLE;
		return;
	}

	if (pSkillData->Skill_ThrowItemType != 255)
	{
		UInt8 bagIndex = (UInt8) m_cmdAdditional;
		CItem* item;                                                                                                                                                        
		// if can't use item, then auto attack
		if (m_pClient == NULL || m_pClient->pBatChar == NULL || m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
			(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
		{
			SendBattleMsg(SKILL_USE_FAIL_TYPE_NOITEM, m_Msg, GetSubType());
			m_command = BATCMD_IDLE;
			return;
		}
		if (item->GetRemain() == 0)
		{
			SendBattleMsg(SKILL_USE_FAIL_TYPE_NOITEM, m_Msg, GetSubType());
			m_command = BATCMD_IDLE;
			return;
		}
	}

	//check skill can use
	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "skillCanHit", String(), this, pTarget);

	UInt8 targetTeam = pTarget->GetTeam();

	for (Actors::iterator it = m_pBattle->m_actors.begin(); it != m_pBattle->m_actors.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL)
			continue;
		actor->m_MissSkill = false;
		actor->m_tempAction = GetAction();
		actor->m_tempReaction = BATCMD_SKILL_HIT;
	}

	LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pSkillData->id, "skill").c_str(), CommonSkillFuncName[SKILL_FUNC_NAME_USE].c_str());

	SetTempAction(GetAction());

	if (postSkill)
		CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this, pTarget, m_pBattle, m_postSkillBaseLv);
	else
		CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), this, pTarget, m_pBattle, GetSkillLv(skillBaseID));	

	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("After Calling skilluse battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);

	if (m_ReturnType > 0)
	{
		SendBattleMsg(BATTLE_ERROR_MSG_GIVEN, m_Msg, GetSubType());
		m_command = BATCMD_IDLE;
		return;
	}

	if (pSkillData->Skill_ThrowItemType != 255)
	{
		UInt8 bagIndex = (UInt8) m_cmdAdditional;
		CItem* item;
		// if can't use item, then auto attack
		if (m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
			(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
		{
			return;
		}
		item->ReduceCount();
	}

	if (m_CancelAction)
	{
		m_CancelAction = false;
		m_bAction = BOOLEAN_TRUE;
		return;
	}
	// target reaction

	SetAction(m_tempAction);

	int count = (int)m_pBattle->m_SkillReactionsMap.size();

	if (count == 0)
	{
		if (!m_Msg.IsEmpty())
			SendBattleMsg(BATTLE_ERROR_MSG_GIVEN, m_Msg, GetSubType());
		m_command = BATCMD_IDLE;
		return;
	}

	if (!m_Msg.IsEmpty())
		SendBattleMsg(BATTLE_USE_MSG_GIVEN, m_Msg, GetSubType());

	UInt attriChangeLstCount = 0;
	SetAction(BATCMD_SKILL);
	for (Actors::iterator it = m_pBattle->m_SkillReactionsMap.begin(); it != m_pBattle->m_SkillReactionsMap.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if(actor == NULL)
			continue;
		if (actor->m_tempReaction != GetReaction())
			actor->SetReaction(actor->m_tempReaction);
		else
			actor->SetReaction(BATCMD_SKILL_HIT);
		if (actor->m_AttriChangeLst.size() > attriChangeLstCount)
			attriChangeLstCount = (UInt)actor->m_AttriChangeLst.size();
	}

	/*for (Actors::iterator it2 = m_pBattle->m_SkillReactionsMap.begin(); it2 != m_pBattle->m_SkillReactionsMap.end(); ++it2)
	{
		CBatActor * pProtector = NULL;
		if (IsProtectActive())
		{
			bool bProtect = true;
			pProtector = m_pBattle->GetProtector(*it2, pTarget);
			bProtect = pProtector != NULL;

			if (bProtect)
			{
				pProtector->m_protectCount--;				
				for (Actors::iterator it = m_pBattle->m_SkillReactionsMap.begin(); it != m_pBattle->m_SkillReactionsMap.end(); ++it)
				{
					CBatActor* actor = *it;
					if (pProtector == actor && 
						pProtector != pTarget)
					{
						bProtect = false;
						break;
					}
				}

				if (!bProtect)
				{					
					pProtector->m_command = BATCMD_IDLE;
				}
			}
		}
	}

	*/
	SetLinkNext(BOOLEAN_FALSE);
	
	UInt32 MaxTime = 0;
	
	UInt i = 0;

	if (m_pBattle->m_SkillReactionsMap.size() > 1 && attriChangeLstCount > 0)
	{
		for (Int j = 0; j < (Int)attriChangeLstCount; j ++)
		{
			i = 0;
			bool first = true;
			MarkUpdate(actionMask);
			for (Actors::iterator it = m_pBattle->m_SkillReactionsMap.begin(); 
			it != m_pBattle->m_SkillReactionsMap.end(); ++it, i ++)
			{
				CBatActor* actor = FindNetObject<CBatActor>(*it);
				if(actor == NULL)
					continue;
				actor->SetLinkNext(BOOLEAN_FALSE);
				UInt32 Time = gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, actor,
				GetAction(), GetSubType(), /*j + */1);

				bool last = true;
				Actors::iterator it2 = it;
				++it2 ;
				while (it2 != m_pBattle->m_SkillReactionsMap.end())
				{
					CBatActor* actor2 = FindNetObject<CBatActor>(*it2);
					if(actor2 == NULL)
						continue;
					if (j < (Int)(actor2->m_AttriChangeLst.size()))
					{
						last = false;
						break;
					}
					++it2;
				}
				// calculate skill animation time
				if (j < (Int)(actor->m_AttriChangeLst.size()))
				{
					//DoAttriAction(actor,  last , Time, actor->m_AttriChangeLst[j], first, j == actor->m_AttriChangeLst.size() - 1);
					DoAttriAction(actor,  last , Time, actor->m_AttriChangeLst[j], first, true);
					if (!last && (pSkillData->animation_Type == SKILL_ANIM_TYPE_ORG_POS_1BY1 ||
						pSkillData->animation_Type == SKILL_ANIM_TYPE_CENTER_POS_1BY1 ||
						pSkillData->animation_Type == SKILL_ANIM_TYPE_TARGET_POS_1BY1))
						MaxTime += Time;
					else if (Time > MaxTime)
						MaxTime = Time;
					first = false;
					if (actor->GetHP() == 0 && actor->m_AttriChangeLst[j].HpChange < 0)
						actor->m_AttriChangeLst.clear();
				}
				if (j == actor->m_AttriChangeLst.size() - 1)
					actor->m_AttriChangeLst.clear();
			}	
		}
	}
	else
	{
		for (Actors::iterator it = m_pBattle->m_SkillReactionsMap.begin(); 
			it != m_pBattle->m_SkillReactionsMap.end(); ++it, i ++)
		{
			CBatActor* actor = FindNetObject<CBatActor>(*it);
			if(actor == NULL)
				continue;
			actor->SetLinkNext(BOOLEAN_FALSE);
			if (actor->m_AttriChangeLst.size() > 0)
			{
				// calculate skill animation time
				UInt32 Time = 0;
				DoContinueAction(actor, i == count -1, Time );
				actor->m_AttriChangeLst.clear();

				if (Time > MaxTime)
					MaxTime = Time;
			}
			else
			{
				// calculate skill animation time
				UInt32 Time = 0;
				if ( pSkillData->animation_Type == SKILL_ANIM_TYPE_ORG_POS_1BY1 ||
					pSkillData->animation_Type == SKILL_ANIM_TYPE_CENTER_POS_1BY1 ||
					pSkillData->animation_Type == SKILL_ANIM_TYPE_TARGET_POS_1BY1)
					Time = gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, actor, 
					GetAction(), GetSubType(), (UInt16)(m_pBattle->m_SkillReactionsMap.size()) );
				else
					Time = gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, actor, 
					GetAction(), GetSubType(), 1);

				if (Time > MaxTime)
					MaxTime = Time;
				m_bAction = BOOLEAN_TRUE;

				actor->SetSubType(skillID);

				CBatActor * pProtector = NULL;
				if (IsProtectActive())
				{
					pProtector = m_pBattle->GetProtector(this, actor);
					if (pProtector != NULL)
					{
						Int dmg = actor->GetDHP();
						if (dmg != 0 && actor->IsUpdated(CBatActor::HPMask))
						{
							actor->ResetHP();
							if (dmg * 0.20f < 0 && dmg * 0.20f > -1)
								actor->ChangeHP(-1);
							else
								actor->ChangeHP(Int(dmg * 0.20f));
							
						}
						dmg = actor->GetDSP();
						if (dmg != 0 && actor->IsUpdated(CBatActor::SPMask))
						{
							actor->ResetSP();
							if (dmg * 0.20f < 0 && dmg * 0.20f > -1)
								actor->ChangeSP(1);
							else
								actor->ChangeSP(Int(dmg * 0.20f));
							

						}
					}
				}
				SetTempAction(GetAction());
				actor->SetTempReaction(actor->GetReaction());
				DoBuffActive( actor, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
				SetAction(m_tempAction);
				actor->SetReaction(actor->m_tempReaction);

				SetTempAction(GetAction());
				actor->SetTempReaction(actor->GetReaction());
				actor->DoBuffActive( this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
				SetAction(m_tempAction);
				actor->SetReaction(actor->m_tempReaction);

				if (m_CancelAction)
				{
					m_CancelAction = false;
					m_bAction = BOOLEAN_TRUE;
					return;
				}

				if (it == m_pBattle->m_SkillReactionsMap.begin())
				{
					MarkUpdate(targetMask);
					MarkUpdate(subTypeMask);
					SetLinkNext(BOOLEAN_FALSE);					// last link of attack
				}
				else
					UnmarkUpdate(actionMask);

				WriteAction();						// write action to buffer

				if (pProtector != NULL && IsProtectActive() && pProtector->IsUpdated() &&
					actor != pProtector)
				{
					pProtector->SetReaction(BATCMD_PROTECT_HIT);
					pProtector->SetTarget(actor->GetIndex());
					pProtector->SetLinkNext(BOOLEAN_TRUE);
					pProtector->WriteReaction();
				}

				if (m_pBattle)
					m_pBattle->WriteExtraActor();

				if (i == count -1 )
					actor->SetLinkNext(BOOLEAN_FALSE);
				else
					actor->SetLinkNext(BOOLEAN_TRUE);
				actor->WriteReaction();
			}
		}
	}
	m_pBattle->m_animTime += MaxTime;

	//do reaction buff
	for (Actors::iterator it = m_pBattle->m_SkillReactionsMap.begin(); 
		it != m_pBattle->m_SkillReactionsMap.end(); ++it)
	{
		CBatActor* actor = FindNetObject<CBatActor>(*it);
		if (actor && !actor->GetLeftBattle()  && actor != this)
			actor->DoBuffActive( this, BUFF_ACTIVE_TIME_TYPE_DO_REACTION);
	}

	m_pBattle->m_SkillReactionsMap.clear();

	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("Do Skill End battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);
}

void CBatActor::MissSkill()
{
	m_tempReaction = BATCMD_MISS;
	m_MissSkill = true;
}
bool CBatActor::HasSkill(UInt16 SkillId)
{
	SkillMap::iterator Itr = m_SkillMap.find(SkillId);
	if (Itr != m_SkillMap.end())
		return true;
	return false;
}
void CBatActor::DoSpecial()
{
	//if (!IsValidTarget())
	//	return;
	DoSkill();
}

void CBatActor::DoWine()
{
	if (!IsValidTarget())
		return;

	//only drink by self
	if (m_cmdTarget != GetIndex())
		return;

	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);
	if (m_cmdSubType != GetSubType())
		SetSubType(m_cmdSubType);
	
	UInt8 bagIndex = (UInt8) m_cmdSubType;
	CItem* item;
	// if can't use item, then auto attack
	if (m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
		(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
	{
		return;
	}

	CBatActor* pTarget = m_pBattle->GetActor(m_cmdTarget);
	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, validActionTargetStr.c_str(), String(), this, pTarget);
	if (pTarget == NULL || gGlobal.g_TempResult == 0 || 
		!pTarget->IsValidTarget(BOOLEAN_FALSE))
		return;

	String errMsg;
	if (item->UseOn(this, pTarget, errMsg))
	{
		UInt8 target_type = LogService_Proxy::Item::TARGET_UNKNOW;
		UInt32 target_id = 0;
		if (pTarget->IsPlayer())
		{
			target_type = LogService_Proxy::Item::TARGET_PLAYER;
			target_id = pTarget->m_pClient->pOwner->GetChar_id();
		}
		else if (pTarget->IsParnter())
		{
			CBatMob* pMob = CastNetObject<CBatMob>(pTarget);
			target_type = LogService_Proxy::Item::TARGET_PARTNER;
			target_id = pMob->GetPartner_id();
		}
		else if (pTarget->IsMob())
		{
			target_type = LogService_Proxy::Item::TARGET_MOB;
			target_id = pTarget->GetActorId();
		}
		if (m_pClient->pOwner)
			LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, m_pClient->GetContext(), 
				m_pClient->pOwner->GetChar_id(), target_type, target_id, item->GetItem_uid(), 
				item->GetItem_id(), item->GetStack(), LogService_Proxy::Item::ACT_USE);

		SetAction(BATCMD_WINE);
		SetTempAction(GetAction());
		pTarget->SetTempReaction(pTarget->GetReaction());
		DoBuffActive(pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
		SetAction(m_tempAction);
		pTarget->SetReaction(pTarget->m_tempReaction);

		SetTempAction(GetAction());
		pTarget->SetTempReaction(pTarget->GetReaction());
		pTarget->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
		SetAction(m_tempAction);
		pTarget->SetReaction(pTarget->m_tempReaction);

		if (m_CancelAction)
		{
			m_CancelAction = false;
			m_bAction = BOOLEAN_TRUE;
			return;
		}

		if (IsUpdated(CBatActor::DPMask))
		{
			CalcAttrib();
		}
		
		MarkUpdate(targetMask);
		SetSubType(item->GetItem_id());
		SetLinkNext(BOOLEAN_FALSE);					// last link of attackA

		// calculate use item animation time
		m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, pTarget, 
			GetAction(), item->GetItem_id());

		WriteAction();						// write action to buffer
		m_bAction = BOOLEAN_TRUE;
		pTarget->SetReaction(BATCMD_USED_ITEM);
		pTarget->SetLinkNext(BOOLEAN_FALSE);
		pTarget->WriteReaction();
	}
	else
	{
		m_command = BATCMD_IDLE;
	}
}

CItem * CBatActor::GetSkillUseItem()
{
	UInt8 bagIndex = (UInt8) m_cmdAdditional;
	CItem* item = NULL;
	
	if (m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
		(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
	{	
		return NULL;
	}

	if (item && item->m_data)
		return item;
	return NULL;
}
UInt8 CBatActor::GetUseItemType()
{
	UInt8 bagIndex = (UInt8) m_cmdSubType;
	CItem* item = NULL;
	
	if (m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
		(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
	{
		SetAction(BATCMD_IDLE);
		return 0;
	}

	if (item && item->m_data)
		return item->m_data->item_type;
	return 0;
}

void CBatActor::IdleAction()
{
	SetAction(BATCMD_IDLE);
	m_bAction = BOOLEAN_TRUE;
}

void CBatActor::DoItem()
{
	if (!IsValidTarget())
	{
		IdleAction();
		return;
	}

	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);
	if (m_cmdSubType != GetSubType())
		SetSubType(m_cmdSubType);

	UInt8 bagIndex = (UInt8)m_cmdSubType;
	CItem* item = NULL;
	// if can't use item, then auto attack
	if (m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
		(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
	{
		IdleAction();
		return;
	}

	if (item->m_data == NULL)
	{
		IdleAction();
		return;
	}
	CBatActor* pTarget = m_pBattle->GetActor(m_cmdTarget);
	gGlobal.g_TempResult = 1;
	CallLuaFunc(gGlobal.g_pLuaState, validActionTargetStr.c_str(), String(), this, pTarget);
	if (pTarget == NULL || gGlobal.g_TempResult == 0 || 
		!pTarget->IsValidTarget(BOOLEAN_FALSE) ||
		(item->m_data->form == ITEMFORM_RESURRECT && !pTarget->IsDead()) ||
		(item->m_data->form != ITEMFORM_RESURRECT && pTarget->IsDead()))
	{
		IdleAction();
		return;
	}

	//lua check
	m_Msg.Empty();
	ClearReturnType();
	{
		//StringA luaName = m_data->battleScript;
		StringA luaName = item->m_data->script;
		size_t pos = luaName.find(".");
		if (pos != std::string::npos)
		{
			luaName.Format("%s%s", luaName.substr(0, pos).c_str(), "CanUse");
			CallLuaFunc(gGlobal.g_pLuaState, luaName.c_str(), String(), this, pTarget);
		}
	}
	if (GetReturnType() > 0)
	{
		SendBattleMsg(BATTLE_ERROR_MSG_GIVEN, m_Msg, GetSubType());
		SetAction(BATCMD_IDLE);
		return;
	}
	// valid form
	if (item->m_data->form == ITEMFORM_RESURRECT && pTarget->GetState() != CHAR_ST_DEAD ||
		item->m_data->form != ITEMFORM_RESURRECT && pTarget->GetState() == CHAR_ST_DEAD)
	{
		SetAction(BATCMD_IDLE);
		return;
	}

	String errMsg;
	if (item->UseOn(this, pTarget, errMsg))
	{
		UInt8 target_type = LogService_Proxy::Item::TARGET_UNKNOW;
		UInt32 target_id = 0;
		if (pTarget->IsPlayer())
		{
			target_type = LogService_Proxy::Item::TARGET_PLAYER;
			target_id = pTarget->m_pClient->pOwner->GetChar_id();
		}
		else if (pTarget->IsParnter())
		{
			CBatMob* pMob = CastNetObject<CBatMob>(pTarget);
			target_type = LogService_Proxy::Item::TARGET_PARTNER;
			target_id = pMob->GetPartner_id();
		}
		else if (pTarget->IsMob())
		{
			target_type = LogService_Proxy::Item::TARGET_MOB;
			target_id = pTarget->GetActorId();
		}
		if (m_pClient->pOwner)
			LogService_Proxy::LogItem(gGlobal.pLogService->pPeer, m_pClient->GetContext(), 
				m_pClient->pOwner->GetChar_id(), target_type, target_id, item->GetItem_uid(), 
				item->GetItem_id(), item->GetStack(), LogService_Proxy::Item::ACT_USE);

		SetAction(BATCMD_ITEM);
		SetTempAction(GetAction());
		pTarget->SetTempReaction(pTarget->GetReaction());
		DoBuffActive( pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
		SetAction(m_tempAction);
		pTarget->SetReaction(pTarget->m_tempReaction);

		SetTempAction(GetAction());
		pTarget->SetTempReaction(pTarget->GetReaction());
		pTarget->DoBuffActive( this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
		SetAction(m_tempAction);
		pTarget->SetReaction(pTarget->m_tempReaction);

		if (m_CancelAction)
		{
			m_CancelAction = false;
			m_bAction = BOOLEAN_TRUE;
			return;
		}

		
		MarkUpdate(targetMask);
		SetSubType(item->GetItem_id());
		SetLinkNext(BOOLEAN_FALSE);					// last link of attackA

		// calculate use item animation time
		m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, pTarget, 
			GetAction(), item->GetItem_id());

		WriteAction();						// write action to buffer
		m_bAction = BOOLEAN_TRUE;
		pTarget->SetReaction(BATCMD_USED_ITEM);
		pTarget->SetLinkNext(BOOLEAN_FALSE);

		if (m_pBattle)
			m_pBattle->WriteExtraActor();

		pTarget->WriteReaction();
	}
	else
	{
		if (GetReturnType() > 0)
		{
			SendBattleMsg(BATTLE_ERROR_MSG_GIVEN, m_Msg, GetSubType());		
		}
		IdleAction();
	}
}

void CBatActor::DoFriend()
{
}

void CBatActor::DoCatch()
{
}

void CBatActor::DoHelp()
{
}

void CBatActor::DoEvent()
{
}

void CBatActor::DoEscape()
{
	TRACE_ENSURE(m_pBattle != NULL);

	if (!IsValidTarget())
		return;

	//float escRate = 0.5f;
	// adjust by self escape skill and other team de-escape skills
	//escRate = WITHIN(escRate, 0.05f, 0.95f);

	gGlobal.g_TempResult = 0;

	// can always escape for mob
	if (IsSurrender() || m_pMob)
		gGlobal.g_TempResult = 1;
	else
		CallLuaFunc(gGlobal.g_pLuaState, "CanEscape", String(), this);

	// check if escape success
	if (gGlobal.g_TempResult > 0)
	{
		SetState(CHAR_ST_ESCAPED);
		SetLeftBattle(BOOLEAN_TRUE);
		SetAction(BATCMD_ESCAPE);

	} else {
		SetAction(BATCMD_ESCAPE_FAIL);
	}

	// calculate escape animation time
	m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, NULL,
		GetAction());

	WriteAction();
	m_bAction = BOOLEAN_TRUE;

	// actor's partner escape together when actor escaped successfully
	if (gGlobal.g_TempResult > 0 && m_pClient && m_pClient->pPartner)
		m_pClient->pPartner->EscapeTogether();
}

void CBatActor::AutoInput()
{
	if (m_command == BATCMD_FRIEND || m_command == BATCMD_CATCH || 
		m_command == BATCMD_HELP || m_command == BATCMD_ESCAPE)
	{
		if (IsCmdBlocked(m_command) || 
			(m_pBattle && m_pBattle->IsCmdBlocked(m_command)))
			m_command = BATCMD_IDLE;
	}
	if (!TryAction() && !HasPostSkill()) {
		m_command = BATCMD_ATTACK;
		m_cmdTarget = 0;
		if (NextTarget(TRUE) == NULL)
				m_command = BATCMD_IDLE;
	}
}
    
void CBatActor::EscapeTogether()
{
	if (GetLeftBattle())
		return;

	SetAction(BATCMD_ESCAPE);
	SetLeftBattle(BOOLEAN_TRUE);
	WriteAction();
	m_bAction = BOOLEAN_TRUE;

	if (!IsDead())
	{
		SetState(CHAR_ST_ESCAPED);
		// calculate escape animation time
		m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, NULL,
			GetAction());
	}
}

void CBatActor::UpdateSkillBuff(VOID)
{
	for (SkillMap::iterator it = m_SkillMap.begin(); it != m_SkillMap.end(); ++it)
	{
		//check add on buff
		const SkillData * pSkillCSVData = gGlobal.m_resource->GetSkillData(it->second->GetSkill_id());
		if (pSkillCSVData)
		{
			for (Int i = 0; i < (Int)pSkillCSVData->AutoAdd_Buffs.size(); i ++)
			{
				AddBuff(pSkillCSVData->AutoAdd_Buffs[i], 0, 0, 0, 99999, 0); 
			}
		}
	}

	UpdateBlockCmd();
	UpdateBlockSkill();
}

void CBatActor::CheckDeadStatus()
{
	if (m_pMob && m_pMob->m_pPartner && m_pMob->m_pPartner->GetState() == CPartner::ST_REST)
		return;

	if (IsDead() && GetHP() > 0) {
		SetState(CHAR_ST_NORM);
	} else if (!IsDead() && GetHP() == 0) {
		SetState(CHAR_ST_DEAD);
		//set M hp to 0 when he is dead
		if (IsPlayer())
		{			
			m_RDMHP += -(Int)GetMHP();
			SetMHP(0, -(Int)GetMHP(), 0);
			//dead all DP to 0
			m_RDDP += -GetDP();
			SetDP(0, -GetDP(), 0);
#ifdef LINEUP_SYSTEM_ON
			CCharacter* pChar = GetOwner();
			UInt8 team = GetTeam();
			if (NULL != pChar) {
				CParty* pParty = pChar->GetParty();
				if (NULL != pParty) {
					if (pParty->GetCharactorIndexById(pChar->GetChar_id()) == m_pBattle->m_teamData[team].iLineupKeyPos
						&& m_pBattle->m_teamData[team].bDoLineupBuffer) {
							m_pBattle->CancleLineupBuffer(team);
						}
				}
			}
#endif
		}
		RemoveBuffAfterDead();
	}
}
void CBatActor::ChangeHP(Int change, bool show)
{
	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("Start battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);
#ifdef LINEUP_SYSTEM_ON
	UInt32 uiCurHp = GetHP();
	if (0 == uiCurHp && change > 0 && IsPlayer()) {
		UInt8 team=GetTeam();
		if (team<2) {
			CCharacter* pChar = GetOwner();
			if (NULL != pChar) {
				CParty* pParty = pChar->GetParty();
				if (NULL != pParty) {
					if (pParty->GetCharactorIndexById(pChar->GetChar_id()) == m_pBattle->m_teamData[team].iLineupKeyPos
						&& !m_pBattle->m_teamData[team].bDoLineupBuffer && pParty->GetRealMemNum() == TEAMMATE_MAX) {
							m_pBattle->DoLineupBuffer(team);
					}
				}
			}
		}
	}
#endif
	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("After Calling lineup Buffer battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);
	//just store the real change in DHP
	UInt32 FinalHp = (UInt32)WITHIN((Int) GetHP() + change, 0, (Int)GetMHP());
	m_RDHP += FinalHp - GetHP();
	if (change > 0 && GetHP() == 0)
		SetHP(FinalHp, GetDHP() + change, (show ? change : 0));
	else
		SetHP(FinalHp, GetDHP() + change, (show ? GetDShowHP() + change : 0));
	
	
	if (m_pProtector && m_pProtector != this && change < 0)
	{
		//protector dam : change / 0.2f * 0.75f
		Int dam = change * 375 / 100;
		if (dam == 0 && change < 0)
			dam = -1;

		UInt32 FinalHp = (UInt32)WITHIN((Int) m_pProtector->GetHP() + dam, 0, (Int)m_pProtector->GetMHP());
		m_pProtector->m_RDHP += FinalHp - m_pProtector->GetHP();
		m_pProtector->SetHP(FinalHp, m_pProtector->GetDHP() + dam, (show ? m_pProtector->GetDShowHP() + dam : 0));	
		m_pProtector->DoBuffActive(NULL, BUFF_ACTIVE_TIME_TYPE_DO_PROTECTTING);
		
	}

	TRACE_INFODTL_8(GLOBAL_LOGGER, 
		_F("End battle %d:%d:%d, actor %d, cmd %d, target %d, subType %d, addtional %d"),
		m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, this,
		m_command, m_cmdTarget, m_cmdSubType, m_cmdAdditional);
}

bool CBatActor::RealDead()
{
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData &&
			Itr->second.pBuffData->Buff_KeepBody)
			return false;

	}

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData &&
			Itr->second.pBuffData->Buff_KeepBody)
			return false;
	}

	return true;
}
void CBatActor::ChangeMHP(Int change, bool show)
{
	//not effective for mob
	if (m_pMob)
		return;
	//just store the real change in DMHP
	UInt32 FinalMHp = (UInt32)WITHIN((Int) GetMHP() + change, 0, (Int)GetMaxHP());
	m_RDMHP += FinalMHp - GetMHP();
	SetMHP(FinalMHp, GetDMHP() + change, (show ? GetDShowMHP() + change : 0));
	if (GetHP() > GetMHP())
		SetHP(GetMHP(), GetDHP(), GetDShowHP());
}

void CBatActor::ChangeSP(Int change, bool show)
{
	//just store the real change in DSP
	UInt32 FinalSP = (UInt32)WITHIN((Int) GetSP() + change, 0, (Int)GetMSP());
	m_RDSP += FinalSP - GetSP();
	SetSP(FinalSP, GetDSP() + change, (show ? GetDShowSP() + change : 0));

	if (m_pProtector && m_pProtector != this && change < 0)
	{
		//protector dam : change / 0.2f * 0.75f
		Int dam = change * 375 / 100;
		if (dam == 0 && change < 0)
			dam = -1;

		UInt32 FinalSP = (UInt32)WITHIN((Int) m_pProtector->GetSP() + change, 0, (Int)m_pProtector->GetMSP());
		m_pProtector->m_RDSP += FinalSP - m_pProtector->GetSP();
		m_pProtector->SetSP(FinalSP, m_pProtector->GetDSP() + dam, (show ? m_pProtector->GetDShowSP() + dam : 0));						
	}
}

void CBatActor::ChangeMSP(Int change, bool show)
{
	//not effective for mob
	if (m_pMob)
		return;
	//just store the real change in DMSP
	UInt32 FinalMSP = (UInt32)WITHIN((Int) GetMSP() + change, 0, (Int)GetMaxSP());
	m_RDMSP += FinalMSP - GetMSP();
	SetMSP(FinalMSP, GetDMSP() + change, (show ? GetDShowMSP() + change : 0));
	if (GetSP() > GetMSP())
		SetSP(GetMSP(), GetDSP(), GetDShowSP());
}

void CBatActor::ResetHP()
{ 
	UInt32 FinalHp = (UInt32)WITHIN((Int) GetHP() - m_RDHP, 0, (Int)GetMHP());
	SetHP(FinalHp, 0, 0); 
	m_RDHP = 0;		

	if (m_pProtector && m_pProtector != this)
	{
		m_pProtector->SetHP(m_pProtector->GetHP() - m_pProtector->m_RDHP, 0, 0); 
		m_pProtector->m_RDHP = 0;		
		m_pProtector->m_protectCount = 0;
		m_pProtector->m_bAction = false;
		m_pProtector->MarkUpdate(CBatActor::HPMask);
	}
}

void CBatActor::ResetMHP()
{
	UInt32 FinalmHp = (UInt32)WITHIN((Int) GetMHP() - m_RDMHP, 0, (Int)GetMaxHP());
	SetMHP(FinalmHp, 0, 0);
	m_RDMHP = 0;
}

void CBatActor::ResetSP()
{
	UInt32 FinalSp = (UInt32)WITHIN((Int) GetSP() - m_RDSP, 0, (Int)GetMSP());
	SetSP(FinalSp, 0, 0);
	m_RDSP = 0;

	if (m_pProtector && m_pProtector != this)
	{
		m_pProtector->SetSP(m_pProtector->GetSP() - m_pProtector->m_RDSP, 0, 0);
		m_pProtector->m_RDSP = 0;
		m_pProtector->m_bAction = false;
	}
}

void CBatActor::ResetMSP()
{
	UInt32 FinalmSp = (UInt32)WITHIN((Int) GetMSP() - m_RDMSP, 0, (Int)GetMaxSP());
	SetMSP(FinalmSp, 0, 0);
	m_RDMSP = 0;
}

UInt16 CBatActor::GetSkillLv(UInt16 ID)
{
	//if (m_pSkillMap) 
	{
		SkillMap::iterator Itr = m_SkillMap.find(ID);
		if (Itr != m_SkillMap.end())
			return Itr->second->GetSkill_level();
	}
	return 0;
}

void CBatActor::SkillFail(PCSTRINGA Text)
{
	m_ReturnType = 1;
	UTF8ToStr(Text, m_Msg);
}

void CBatActor::UseSkillMsg(PCSTRINGA Text)
{
	UTF8ToStr(Text, m_Msg);
}

void CBatActor::ItemFail(PCSTRINGA Text)
{
	m_ReturnType = 1;
	UTF8ToStr(Text, m_Msg);
}

void CBatActor::UseItemMsg(PCSTRINGA Text)
{
	UTF8ToStr(Text, m_Msg);
}

void CBatActor::DefaultInput()
{
	if (IsCmdBlocked(BATCMD_ATTACK) ||
		(m_pBattle && m_pBattle->IsCmdBlocked(BATCMD_ATTACK)))
	{
		m_command = BATCMD_IDLE;
		return;
	}

	m_command = BATCMD_ATTACK;
	NextTarget();
}

bool CBatActor::AddBuff(UInt16 Buff_id, UInt32 Buff_Value1, UInt32 Buff_Value2, UInt32 Buff_Value3, UInt32 Buff_CoolDown, UInt8 Pro)
{
	//find whether it is battle buff
	//......

	if (Buff_id == 0)
		return false;

	if (IsAntiBuff(Buff_id))
		return false;

	ExtractedBuffData Data;
	Data.ID = Buff_id;
	Data.Value1 = Buff_Value1;
	Data.Value2 = Buff_Value2;
	Data.Value3 = Buff_Value3;
	Data.CoolDown = Buff_CoolDown;
	Data.Pro = Pro;

	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		Data.StartDelay = (UInt8) pBuffData->Buff_starttime;
		//addume they are in minute
		if (pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			Data.CoolDown *= 60;
		//only add to battle buff

		if (pBuffData->Buff_RemoveAftOffline)
		{
			BuffMap::iterator Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
			if (Itr != m_BuffCacheMap.end())
			{
				//clear buffs
				ExtractedBuffData oldData = Itr->second;
				if (Data >= oldData)
				{
					for (Int i = 0; i < (Int)pBuffData->Buff_ClearBuffs.size(); i++)
					{
						RemoveBuff(pBuffData->Buff_ClearBuffs[i]);
					}
					RemoveBuff(oldData.GetBuffID());

					Data.pBuffData = pBuffData;

					StringA LuaFunc;

					LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
						CommonBuffFuncName[BUFF_FUNC_NAME_ADD].c_str());
					if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), &Data,  this);
					m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
					ConstructBuffCacheString();

					for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
					{
						if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
						{
							CalcAttrib();
							break;
						}
					}
					//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
					//	CalcAttrib();

					if (pBuffData->Buff_Blockcommands.size() > 0 ||
						pBuffData->Buff_BlockActions.size() > 0)
						UpdateBlockCmd();

					if (pBuffData->Buff_BlockSkills.size() > 0)
						UpdateBlockSkill();
				}
			}
			else
			{
				Data.pBuffData = pBuffData;
				for (Int i = 0; i < (Int)pBuffData->Buff_ClearBuffs.size(); i++)
				{
					RemoveBuff(pBuffData->Buff_ClearBuffs[i]);
				}
				StringA LuaFunc;

				LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
					CommonBuffFuncName[BUFF_FUNC_NAME_ADD].c_str());
				if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
					CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), &Data,  this);

				m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
				ConstructBuffCacheString();
				for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
				{
					if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
					{
						CalcAttrib();
						break;
					}
				}
				//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
				//	CalcAttrib();

				if (pBuffData->Buff_Blockcommands.size() > 0 ||
					pBuffData->Buff_BlockActions.size() > 0)
					UpdateBlockCmd();

				if (pBuffData->Buff_BlockSkills.size() > 0)
					UpdateBlockSkill();


			}
		}
		else
		{
			if (m_BuffMap.size() == MAX_BUFF_NUM)
				return false;

			BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
			if (Itr != m_BuffMap.end())
			{
				//clear buffs
				ExtractedBuffData oldData = Itr->second;
				if (Data >= oldData)
				{
					for (Int i = 0; i < (Int)pBuffData->Buff_ClearBuffs.size(); i++)
					{
						RemoveBuff(pBuffData->Buff_ClearBuffs[i]);
					}
					RemoveBuff(oldData.GetBuffID());

					Data.pBuffData = pBuffData;

					StringA LuaFunc;

					LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
						CommonBuffFuncName[BUFF_FUNC_NAME_ADD].c_str());
					if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
						CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(), String(), &Data,  this);
					m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
					ConstructBuffString();

					for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
					{
						if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
						{
							CalcAttrib();
							break;
						}
					}
					//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
					//	CalcAttrib();

					if (pBuffData->Buff_Blockcommands.size() > 0 ||
						pBuffData->Buff_BlockActions.size() > 0)
						UpdateBlockCmd();

					if (pBuffData->Buff_BlockSkills.size() > 0)
						UpdateBlockSkill();
				}
			}
			else
			{
				Data.pBuffData = pBuffData;
				for (Int i = 0; i < (Int)pBuffData->Buff_ClearBuffs.size(); i++)
				{
					RemoveBuff(pBuffData->Buff_ClearBuffs[i]);
				}
				StringA LuaFunc;

				LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
					CommonBuffFuncName[BUFF_FUNC_NAME_ADD].c_str());
				if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
					CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Data,  this);

				m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
				ConstructBuffString();
				for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
				{
					if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
					{
						CalcAttrib();
						break;
					}
				}
				//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
				//	CalcAttrib();

				if (pBuffData->Buff_Blockcommands.size() > 0 ||
					pBuffData->Buff_BlockActions.size() > 0)
					UpdateBlockCmd();

				if (pBuffData->Buff_BlockSkills.size() > 0)
					UpdateBlockSkill();


			}
		}

		if (m_pBatChar && m_pBatChar->m_pOwner && !pBuffData->Buff_RemoveAftBattle)
			m_pBatChar->m_pOwner->AddBuff( Buff_id,  Buff_Value1,  Buff_Value2,  Buff_Value3,  Buff_CoolDown,  Pro);

		//check left battle
		if (GetLeftBattle() && GetState() == CHAR_ST_DEAD &&
			pBuffData->Buff_KeepBody)
		{
			SetLeftBattle(BOOLEAN_FALSE);
		}
	}

	
	return true;
	//m_BuffMap

	//m_pBuffChangeList
	//PROCEDURE_DO_METHOD3(CBatActor, DoAddBuff, this, m_context, UInt16, Buff_id, UInt16, Buff_Value, UInt32, Buff_CoolDown)
}

void CBatActor::RemoveBuffAfterDead()
{
	StlVector<UInt16> rmBuffIDs;
	BuffMap TempMap = m_BuffCacheMap;
	for (BuffMap::iterator Itr = TempMap.begin(); Itr != TempMap.end(); ++ Itr)
	{
		ExtractedBuffData Data = Itr->second;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData && pBuffData->Buff_removeAfterDead &&
			pBuffData->Buff_type != BUFF_TYPE_EQ && pBuffData->Buff_type != BUFF_TYPE_PASSIVE)
		{

			/*
			StringA LuaFunc;

			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_REMOVE].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Data,  this);

			ConstructBuffCacheString();
			//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
			//	CalcAttrib();
			for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
			{
				if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					CalcAttrib();
					break;
				}
			}
			if (pBuffData->Buff_Blockcommands.size() > 0 ||
				pBuffData->Buff_BlockActions.size() > 0)
				UpdateBlockCmd();

			if (pBuffData->Buff_BlockSkills.size() > 0)
				UpdateBlockSkill();
				*/
			rmBuffIDs.push_back(Itr->second.ID);
		}
	}

	for (Int i = 0; i < (Int)rmBuffIDs.size(); i++)
	{
		RemoveBuff(rmBuffIDs[i]);
	}

}

void CBatActor::RemoveBuffAfterBattle()
{
	StlVector<UInt16> rmBuffIDs;
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData &&
			Itr->second.pBuffData->Buff_RemoveAftBattle)
			rmBuffIDs.push_back(Itr->second.ID);

	}

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++ Itr)
	{
		if (Itr->second.pBuffData &&
			Itr->second.pBuffData->Buff_RemoveAftBattle)
			rmBuffIDs.push_back(Itr->second.ID);
	}

	for (Int i = 0; i < (Int)rmBuffIDs.size(); i++)
	{
		RemoveBuff(rmBuffIDs[i]);
	}
}

void CBatActor::RemoveAllCacheBuff()
{
	bool cal = false;
	BuffMap TempMap = m_BuffCacheMap;
	for (BuffMap::iterator Itr = TempMap.begin(); Itr != TempMap.end(); ++ Itr)
	{
		ExtractedBuffData Data = Itr->second;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{

			StringA LuaFunc;

			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_REMOVE].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Data,  this);

			ConstructBuffCacheString();
			//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
			//	CalcAttrib();
			for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
			{
				if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					cal = true;
					break;
				}
			}
			if (pBuffData->Buff_Blockcommands.size() > 0 ||
				pBuffData->Buff_BlockActions.size() > 0)
				UpdateBlockCmd();

			if (pBuffData->Buff_BlockSkills.size() > 0)
				UpdateBlockSkill();
		}
	}

	if (cal)
		CalcAttrib();
	m_BuffCacheMap.clear();
}

void CBatActor::RemoveBuff(UInt16 Buff_id)
{
	//find whether it is battle buff
	//......
	
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			ExtractedBuffData Data = Itr->second;

			m_BuffMap.erase(Itr);

			StringA LuaFunc;

			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_REMOVE].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Data,  this);

			ConstructBuffString();
			for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
			{
				if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					CalcAttrib();
					break;
				}
			}
			//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
			//	CalcAttrib();
			if (pBuffData->Buff_Blockcommands.size() > 0 ||
				pBuffData->Buff_BlockActions.size() > 0)
				UpdateBlockCmd();

			if (pBuffData->Buff_BlockSkills.size() > 0)
				UpdateBlockSkill();

			if (m_pBatChar && m_pBatChar->m_pOwner)
				m_pBatChar->m_pOwner->RemoveBuff(Buff_id);
			return;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
		{
			ExtractedBuffData Data = Itr->second;

			m_BuffCacheMap.erase(Itr);

			StringA LuaFunc;

			LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(pBuffData->id, "buff").c_str(), 
				CommonBuffFuncName[BUFF_FUNC_NAME_REMOVE].c_str());
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Data,  this);

			ConstructBuffCacheString();
			//if (pBuffData->IsActive(BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE))
			//	CalcAttrib();
			for (Int k = 0; k < (Int)pBuffData->Buff_acttimes.size(); k++)
			{
				if (pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_ATTRI_BATTLE)
				{
					CalcAttrib();
					break;
				}
			}
			if (pBuffData->Buff_Blockcommands.size() > 0 ||
				pBuffData->Buff_BlockActions.size() > 0)
				UpdateBlockCmd();

			if (pBuffData->Buff_BlockSkills.size() > 0)
				UpdateBlockSkill();

			if (m_pBatChar && m_pBatChar->m_pOwner)
				m_pBatChar->m_pOwner->RemoveBuff(Buff_id);

			return;
		}

		//check left battle
		if (!GetLeftBattle() && GetState() == CHAR_ST_DEAD &&
			pBuffData->Buff_KeepBody)
		{
			SetLeftBattle(BOOLEAN_TRUE);
		}
	}

}

void CBatActor::RemoveBuffSchool(UInt16 school_id)
{
	BuffMap::iterator it = m_BuffMap.find(school_id);
	if (it != m_BuffMap.end()) {
		RemoveBuff(it->second.ID);
	} else {
		it = m_BuffCacheMap.find(school_id);
		if (it != m_BuffCacheMap.end())
			RemoveBuff(it->second.ID);
	}
}

ExtractedBuffData * CBatActor::FindBuff(UInt16 Buff_id)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			return &Itr->second;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
			return &Itr->second;
	}
	return NULL;
}

bool CBatActor::HasBuff(UInt16 Buff_id)
{
	const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Buff_id);
	if (pBuffData)
	{
		BuffMap::iterator Itr = m_BuffMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffMap.end() &&
			Itr->second.ID == Buff_id)
		{
			return true;
		}

		Itr = m_BuffCacheMap.find(pBuffData->Buff_school);
		if (Itr != m_BuffCacheMap.end() &&
			Itr->second.ID == Buff_id)
			return true;
	}
	return false;
}

//extract the buff cache string
void CBatActor::ExtractBuffCacheString()
{
	m_BuffCacheMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, GetBuffCache().size() + 1);
	buf[GetBuffCache().size()] = _T('\0');
	_tcscpy(buf, GetBuffCache().c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value);
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","while (value != NULL)
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;

		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;
			m_BuffCacheMap.insert(std::make_pair(pBuffData->Buff_school, Data));
		}
	}
	SafeDeallocate(buf);
}

void CBatActor::ConstructBuffCacheString()
{
	String BuffList;
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	if (m_pBattle)
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CBatActor::ConstructBuffCacheString: battle %d:%d Actor %d before construct buff: %s"), 
						m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, GetIndex(), GetBuffCache().c_str());
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++ Itr)
	{
		String Buff;
		Buff.Format(_T("%d%s%d%s%d%s%d%s%d%s%d%s%d%s"), Itr->second.ID, valueDelim, Itr->second.Value1,
			valueDelim, Itr->second.Value2, valueDelim, Itr->second.Value3, valueDelim, Itr->second.Pro, valueDelim, Itr->second.CoolDown, valueDelim, Itr->second.Repeat, buffDelim);
		BuffList += Buff.c_str();
	}	

	SetBuffCache(BuffList);

	if (m_pBattle)
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CBatActor::ConstructBuffCacheString: battle %d:%d Actor %d after construct buff: %s"), 
						m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, GetIndex(), GetBuffCache().c_str());
}
//extract the buff string
VOID CBatActor::ExtractBuffString()
{
	m_BuffMap.clear();
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");
	TCHAR *value = NULL; 
	TCHAR *buf = SafeAllocate(TCHAR, GetBuff().size() + 1);
	buf[GetBuff().size()] = _T('\0');
	_tcscpy(buf, GetBuff().c_str());
	_tcstok(buf, valueDelim);
	bool first = true;
	while (true)
	{
		ExtractedBuffData Data;
		//buff id
		if (first)
		{
			if (buf)
				Data.ID = _ttoi(buf);
			else
				break;
			first = false;
		}
		else
		{

			value = _tcstok(NULL, valueDelim);
			if (value)
				Data.ID = _ttoi(value);
			else
				break;
		}

		//buff Value1
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value1 = _ttoi(value);
		else
			break;

		//buff Value2
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value2 = _ttoi(value);
		else
			break;

		//DOT Value3
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Value3 = _ttoi(value);
		else
			break;

		//Pro
		value = _tcstok(NULL, valueDelim );
		if (value)
			Data.Pro = _ttoi(value);
		else
			break;

		//buff CoolDown
		value = _tcstok(NULL, buffDelim);
		if (value)
		{
			Data.CoolDown = _ttoi(value); // input the cool down value first
			while (value && (((*value >= 48) && (*value <= 57)) || (*value == 44) || (*value == 130))) // while the values are numbers or ","
			{
				if (value && (*value == 44 || *value == 130)) // check if there is a comma "," in the string
					break;
				value++;
			}
			if (value && (*value == 44 || *value == 130))// buff Repeat
			{
				value++;
				if (value && (*value >= 48) && (*value <= 57))
				{
					Data.Repeat = _ttoi(value);
				}
				else
					Data.Repeat = 0;
			}
			else
				Data.Repeat = 0;
		}
		else
			break;

		const BuffData * pBuffData = gGlobal.m_resource->GetBuffData(Data.ID);
		if (pBuffData)
		{
			Data.pBuffData = pBuffData;
			m_BuffMap.insert(std::make_pair(pBuffData->Buff_school, Data));
		}
	}
	SafeDeallocate(buf);
}

void CBatActor::ConstructBuffString()
{
	String BuffList;
	TCHAR *valueDelim = _T(",");
	TCHAR *buffDelim = _T(" ");

	if (m_pBattle)
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CBatActor::ConstructBuffString: battle %d:%d Actor %d before construct buff: %s"), 
						m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, GetIndex(), GetBuff().c_str());

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++ Itr)
	{
		String Buff;
		Buff.Format(_T("%d%s%d%s%d%s%d%s%d%s%d%s%d%s"), Itr->second.ID, valueDelim, Itr->second.Value1,
			valueDelim, Itr->second.Value2, valueDelim, Itr->second.Value3, valueDelim, Itr->second.Pro, valueDelim, Itr->second.CoolDown, valueDelim, Itr->second.Repeat, buffDelim);
		BuffList += Buff.c_str();
	}

	SetBuff(BuffList);

	if (m_pBattle)
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CBatActor::ConstructBuffString: battle %d:%d Actor %d after construct buff: %s"), 
						m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, GetIndex(), GetBuff().c_str());
}

void CBatActor::SetSkillCoolDown(UInt16 ID, DWORD countDown)
{
	//if (m_pSkillMap) 
	{
		SkillMap::iterator Itr = m_SkillMap.find(ID);
		if (Itr != m_SkillMap.end())
			Itr->second->SetCoolDown(countDown);
	}
}

UInt16 CBatActor::GetWeaponATK()
{
	if (IsParnter() || IsMob())
		return 0;
	CItem * pItem = m_pBag->GetItem(BAG_MAX_ITEM + EQPLACE_WEAPON);
	return (pItem ? pItem->GetATK() : 0);
}

void CBatActor::UpdateBuff(UInt32 DeTime)
{
	
	UInt32 curTime = ::timeGetTime();
	StlVector<UInt16> RmIDLst;
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_TURN)
			{
				if (Itr->second.StartDelay > 0)
				{
					Itr->second.StartDelay --;
					if (Itr->second.StartDelay == 0)
					{
						UpdateBlockCmd();
					}
				}
				else if (Itr->second.CoolDown == 0)
				{
					
					RmIDLst.push_back(Itr->second.ID);
				}
				else
					Itr->second.CoolDown --;
			}
			else if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			{
				UInt32 delTime = 0 ;
				if (curTime > Itr->second.PreUpdateTime)
				{
					delTime = (curTime - Itr->second.PreUpdateTime) / 1000;
					if (delTime > 0)
						Itr->second.PreUpdateTime += delTime * 1000;
				}
				if (Itr->second.CoolDown > delTime)
					Itr->second.CoolDown -= delTime;
				else
					Itr->second.CoolDown = 0;

				if (Itr->second.CoolDown == 0)
				{
					
					RmIDLst.push_back(Itr->second.ID);
				}
			}
		}
	}
	BuffMap::iterator Itr;
	for (Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_endtype == BUFF_END_TYPE_TIME_OUT_END)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_TURN)
			{
				if (Itr->second.StartDelay > 0)
				{
					Itr->second.StartDelay --;
					if (Itr->second.StartDelay == 0)
					{
						UpdateBlockCmd();
					}
				}
				else if (Itr->second.CoolDown == 0)
				{
					
					RmIDLst.push_back(Itr->second.ID);
				}
				else
					Itr->second.CoolDown --;
			}
			else if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_period_type == BUFF_PERIOD_TYPE_MINUTES)
			{
				UInt32 delTime = 0 ;
				if (curTime > Itr->second.PreUpdateTime)
				{
					delTime = (curTime - Itr->second.PreUpdateTime) / 1000;
					if (delTime > 0)
						Itr->second.PreUpdateTime += delTime * 1000;
				}
				if (Itr->second.CoolDown > delTime)
					Itr->second.CoolDown -= delTime;
				else
					Itr->second.CoolDown = 0;

				if (Itr->second.CoolDown == 0)
				{
					
					RmIDLst.push_back(Itr->second.ID);
				}
			}
		}
	}

	for (Int i = 0; i < (Int)RmIDLst.size(); i++)
	{
		RemoveBuff(RmIDLst[i]);

		m_BuffActive = false;
		StringA LuaFunc;

		LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(RmIDLst[i], "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_TIMEOUT].c_str());
		if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
			CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  this);

	}

	//check itembag buff
	CCharacter * pChar = GetOwner();
	if (pChar && pChar->m_pBag)
		pChar->m_pBag->UpdateTurnBuff();

}

void CBatActor::PackBuff()
{
	if (IsUpdated() && m_pBattle)
	{
		RPCMask mask;						// set mask for writing buffer

		UnmarkUpdate(actionMask);
		SetReaction(BATCMD_BUFF_EFFECT_HIT);
		SetLinkNext(BOOLEAN_FALSE);
		m_pBattle->m_turnBuffer1->Write<UInt8>(GetIndex());
		WriteAttr(*m_pBattle->m_turnBuffer1, m_updateMask);
		m_pBattle->m_turnBuffer2->Write<UInt8>(GetIndex());
		WriteAttr(*m_pBattle->m_turnBuffer2, m_updateMask);
		m_pBattle->m_turnBufferMon->Write<UInt8>(GetIndex());
		WriteAttr(*m_pBattle->m_turnBufferMon, m_updateMask);
		UnmarkUpdate(m_updateMask);		
	}
}

bool CBatActor::IsProtectActive()
{
	if (GetAction() == BATCMD_ATTACK ||
		GetAction() == BATCMD_CRIT_ATTACK)
		return true;

	if (GetAction() == BATCMD_SKILL)
	{
		const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(GetSubType());
		if (pSkillData && 
			(pSkillData->animation_Type == SKILL_ANIM_TYPE_TARGET_POS || 
			pSkillData->animation_Type == SKILL_ANIM_TYPE_TARGET_POS_1BY1))
		{
			return true;
		}
	}
	return false;
}


enum FINAL_VALUE_TYPE
{
	FINAL_VALUE_TYPE_LIFE = 0,
	FINAL_VALUE_TYPE_HP,
	FINAL_VALUE_TYPE_SP,

};
Int CBatActor::GetFinalValue(Int change, CBatActor * pAtk, UInt8 tempAction, UInt8 valueType)
{
	if (pAtk == NULL)
		return false;

	if (m_inGetFinalValue)
		return false;

	m_inGetFinalValue = true;

	//store the ori hp and sp
	UInt32 orgHp = GetHP();
	UInt32 orgDHp = GetDHP();
	UInt32 orgShowHp = GetDShowHP();
	UInt32 orgRDHP = m_RDHP;
	UInt32 orgProHp = 0;
	UInt32 orgProDHp = 0;
	UInt32 orgProShowHp = 0;
	UInt32 orgProRDHP = 0;

	UInt32 orgAtkHp = pAtk->GetHP();	
	UInt32 orgAtkDHp = pAtk->GetDHP();	
	UInt32 orgAtkShowHp = pAtk->GetDShowHP();	
	UInt32 orgAtkRDHP = pAtk->m_RDHP;	
	UInt8 orgAction = pAtk->GetAction();
	pAtk->SetAction(tempAction);
	SetHP(GetHP(), 0 ,0);
	pAtk->SetHP(GetHP(), 0 ,0);


	if (m_pProtector != NULL)
	{
		orgProHp = m_pProtector->GetHP();
		orgProDHp = m_pProtector->GetDHP();
		orgProShowHp = m_pProtector->GetDShowHP();
		orgProRDHP = m_pProtector->m_RDHP;
	}			
	

	if (valueType == FINAL_VALUE_TYPE_LIFE || valueType == FINAL_VALUE_TYPE_HP)
		ChangeHP(change);
	else if (valueType == FINAL_VALUE_TYPE_SP)
		ChangeSP(change);

	pAtk->SetTempAction(pAtk->GetAction());
	SetTempReaction(GetReaction());
	//pAtk->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);

	BuffMultiMap tempMap;

	tempMap.clear();
	//establish temp buff Map according to Buff_Priority
	for (BuffMap::iterator Itr = pAtk->m_BuffMap.begin(); Itr != pAtk->m_BuffMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_RESIST)
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); ++k)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER)
				{
					tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_Priority, Itr->second));
					break;
				}
			}
		}
	}

	for (BuffMap::iterator Itr = pAtk->m_BuffCacheMap.begin(); Itr != pAtk->m_BuffCacheMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_RESIST)
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); ++k)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER)
				{
					tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_Priority, Itr->second));
					break;
				}
			}
		}
	}		

	for (BuffMultiMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); ++Itr)
	{
		StringA LuaFunc;
		LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_ACTIVE].c_str());
		if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
			CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  pAtk, this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
	}
	
	pAtk->SetAction(pAtk->m_tempAction);
	SetReaction(m_tempReaction);

	pAtk->SetTempAction(pAtk->GetAction());
	SetTempReaction(GetReaction());
	//DoBuffActive(pAtk, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
	tempMap.clear();
	//establish temp buff Map according to Buff_Priority
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_RESIST)
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); ++k)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_DO_HITTING)
				{
					tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_Priority, Itr->second));
					break;
				}
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++Itr)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_RESIST)
		{
			for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); ++k)
			{
				if (Itr->second.pBuffData->Buff_acttimes[k] == BUFF_ACTIVE_TIME_TYPE_DO_HITTING)
				{
					tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_Priority, Itr->second));
					break;
				}
			}
		}
	}		

	for (BuffMultiMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); ++Itr)
	{
		StringA LuaFunc;
		LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_ACTIVE].c_str());
		if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
			CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  this, pAtk, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
	}
	pAtk->SetAction(pAtk->m_tempAction);
	SetReaction(m_tempReaction);

	Int result = 0;
	if (valueType == FINAL_VALUE_TYPE_LIFE)
		result = GetHP() > 0;
	else if (valueType == FINAL_VALUE_TYPE_HP)
		result = GetDHP();
	else if (valueType == FINAL_VALUE_TYPE_SP)
		result = GetDSP();		
	//restore the hp and sp
	SetHP(orgHp, orgDHp, orgShowHp);
	m_RDHP = orgRDHP;
	if (m_pProtector != NULL)
	{
		m_pProtector->SetHP(orgProHp, orgProDHp, orgProShowHp);
		m_pProtector->m_RDHP = orgProRDHP;
	}		
	pAtk->SetHP(orgAtkHp, orgAtkDHp, orgAtkShowHp);
	pAtk->SetAction(orgAction);
	pAtk->m_RDHP = orgAtkRDHP;
	m_inGetFinalValue = false;
	return result;
}

void CBatActor::ResetProtector()
{
	if (m_pProtector)
		m_pProtector->m_protectCount = 0;
	m_pProtector = NULL;
}
void CBatActor::DoAttriAction(CBatActor * pBatActor, bool LastOne, UInt32 & Time, AttriChangeData data, bool first, bool LastHit)
{
	CBatActor * pProtector = NULL;
	if (IsProtectActive())
		pProtector = m_pBattle->GetProtector(this, pBatActor);

	pBatActor->SetReaction(data.reaction);
	if (data.action != -1)
	{
		SetAction(data.action);
	}

	pBatActor->ChangeDP(data.DpChange);
	pBatActor->ChangeWC(data.WcChange);

	pBatActor->SetSubType(m_cmdSubType);

	if (pProtector != NULL)
	{
		if (data.HpChange * 0.20f < 0 && data.HpChange * 0.20f > -1)
			pBatActor->ChangeHP(-1);
		else
			pBatActor->ChangeHP(Int(data.HpChange * 0.20f));

		
		if (data.SpChange * 0.20f < 0 && data.SpChange * 0.20f > -1)
			pBatActor->ChangeSP(-1);
		else
			pBatActor->ChangeSP(Int(data.SpChange * 0.20f));

		

	}
	else
	{
		pBatActor->ChangeHP(data.HpChange);
		pBatActor->ChangeSP(data.SpChange);
	}
	SetTempAction(GetAction());
	pBatActor->SetTempReaction(pBatActor->GetReaction());
	DoBuffActive(pBatActor, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
	SetAction(m_tempAction);
	pBatActor->SetReaction(pBatActor->m_tempReaction);

	SetTempAction(GetAction());
	pBatActor->SetTempReaction(pBatActor->GetReaction());
	pBatActor->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
	SetAction(m_tempAction);
	pBatActor->SetReaction(pBatActor->m_tempReaction);

	if (data.HpChange < 0 && pBatActor->GetHP() == 0)
		LastHit = true;
	if (LastOne && LastHit)
	{
		pBatActor->SetLinkNext(BOOLEAN_FALSE);
	}
	else
	{
		pBatActor->SetLinkNext(BOOLEAN_TRUE);

		if (GetAction() != BATCMD_SKILL)
		{
			switch( pBatActor->GetReaction() )
			{
			case BATCMD_NORM_HIT:
			case BATCMD_CRIT_HIT:
			case BATCMD_BREAK_HIT:
			case BATCMD_DEFEND_HIT:
			case BATCMD_PROTECT_HIT:
				Time += HOLDHURTTIME;
				break;
			}
		}
	}

	if (!first)
		UnmarkUpdate(actionMask);
	WriteAction();						// write action to buffer

	if (pProtector != NULL  && pProtector->IsUpdated()
		&& pBatActor != pProtector) {
			pProtector->SetReaction(BATCMD_PROTECT_HIT);
			pProtector->SetTarget(pBatActor->GetIndex());
			pProtector->SetLinkNext(BOOLEAN_TRUE);
			pProtector->WriteReaction();
			pProtector->m_protectCount = 1;
			
		}

		if (m_pBattle)
			m_pBattle->WriteExtraActor();		

		pBatActor->MarkUpdate(reactionMask);
		pBatActor->WriteReaction();
}
void CBatActor::DoContinueAction(CBatActor * pBatActor, bool LastOne, UInt32 & Time)
{
	if (pBatActor == NULL)
		return;


	AttriChangeDataList temp;
	if (m_buffActing)
	{
		temp = pBatActor->m_AttriBuffChangeLst;
		pBatActor->m_AttriBuffChangeLst.clear();
	}
	else
	{
		temp = pBatActor->m_AttriChangeLst;
		pBatActor->m_AttriChangeLst.clear();
	}

	Time = gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, pBatActor,
		GetAction(), GetSubType(), (UInt16) temp.size());

	Int k = 0;

	for (; k < (Int)temp.size(); k++)
	{		
		if (m_buffActing)
			m_CurBuffContAct = k + 1;
		else
			m_CurContAct = k + 1;

		if (!IsValidTarget())
			break;
		m_isLastHit = k == temp.size() - 1;
		pBatActor->m_isLastHit = k == temp.size() - 1;
		
		DoAttriAction(pBatActor,   LastOne,  Time, temp[k], k == 0, k == temp.size() -1);
		if (pBatActor->GetHP() == 0)
			break;
	}

	if (m_buffActing)
		m_CurBuffContAct = 0;
	else
		m_CurContAct = 0;

	m_isLastHit = BOOLEAN_TRUE;
	pBatActor->m_isLastHit = BOOLEAN_TRUE;
}

bool CBatActor::DetectBuff(UInt16 Buff_ID)
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr ++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_DetectBuffs.size() > 0)
		{
			for (UInt i = 0; i < Itr->second.pBuffData->Buff_DetectBuffs.size(); i ++)
			{
				if (Itr->second.pBuffData->Buff_DetectBuffs[i] == Buff_ID)
					return true;
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr ++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_DetectBuffs.size() > 0)
		{
			for (UInt i = 0; i < Itr->second.pBuffData->Buff_DetectBuffs.size(); i ++)
			{
				if (Itr->second.pBuffData->Buff_DetectBuffs[i] == Buff_ID)
					return true;
			}
		}
	}

	return false;
}
void CBatActor::DoBuffActive(CBatActor * pActor, BYTE ActiveType, StlVector<CBatActor *> * activeList)
{
	BuffMultiMap tempMap;

	UInt16Vector	activatedBuffID;

	for (Int index = 0; index < 2; index ++)
	{
		tempMap.clear();
		//establish temp buff Map according to Buff_Priority
		if (index == 0)
		{
			for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); ++Itr)
			{
				if (Itr->second.pBuffData)
					tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_Priority, Itr->second));
			}
			
		}
		else
		{
			for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); ++Itr)
			{
				if (Itr->second.pBuffData)
					tempMap.insert(std::make_pair(Itr->second.pBuffData->Buff_Priority, Itr->second));
			}
		}
		for (BuffMultiMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); ++Itr)
		{
			if (Itr->second.pBuffData)// && Itr->second.pBuffData->IsActive(ActiveType))
			{
				for (Int k = 0; k < (Int)Itr->second.pBuffData->Buff_acttimes.size(); ++k)
				{
					if (Itr->second.pBuffData->Buff_acttimes[k] == ActiveType)
					{
						bool found = false;
						for (UInt16Vector::iterator ItrPostIDs = m_ActiveBuffIDs.begin();
							ItrPostIDs != m_ActiveBuffIDs.end(); ++ItrPostIDs)
						{
							UInt16 ID1 = (*ItrPostIDs);
							UInt16 ID2 = Itr->second.ID;
							if ((*ItrPostIDs) == Itr->second.ID)
							{
								found = true;
								break;
							}
						}
						if (found)
							continue;

						UInt16 ID2 = Itr->second.ID;

						TRACE_INFODTL_8(GLOBAL_LOGGER, _F("CBatActor::DoBuffActive: battle %d:%d Actor %d cmd %d, target %d, subtype %d additional %d buff_id: %d"), 
						m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId, GetIndex(), GetCommand(), GetTarget(), GetSubType(), GetAdditional(), ID2);
						m_buffActing = BOOLEAN_TRUE;
						if (pActor)
							pActor->m_buffActing = BOOLEAN_TRUE; 

						
						m_ActiveBuffIDs.push_back(Itr->second.ID);
						m_BuffActive = false;
						StringA LuaFunc;

						ExtractedBuffData Datab4 = Itr->second;
						LuaFunc.Format("%s%s", gGlobal.GetPrefixStr(Itr->second.pBuffData->id, "buff").c_str(), CommonBuffFuncName[BUFF_FUNC_NAME_ACTIVE].c_str());
						if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
							CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),  String(), &Itr->second,  this, pActor, ActiveType);

						ExtractedBuffData DataAfter = Itr->second;
						if (index == 0)
						{
							BuffMap::iterator ItrSub = m_BuffMap.find(Itr->second.pBuffData->Buff_school);
							if (ItrSub != m_BuffMap.end() &&
								ItrSub->second.ID == Itr->second.ID)
							{
								ItrSub->second = Itr->second;
							}
						}
						else
						{
							BuffMap::iterator ItrSub = m_BuffCacheMap.find(Itr->second.pBuffData->Buff_school);
							if (ItrSub != m_BuffCacheMap.end() &&
								ItrSub->second.ID == Itr->second.ID)
							{
								ItrSub->second = Itr->second;
							}
						}

						if (pActor != NULL &&
							(pActor->m_AttriBuffChangeLst.size() > 0 || m_AttriBuffChangeLst.size() > 0))
						{
							if ( m_AttriBuffChangeLst.size() > 0)
							{
								pActor->SetAction(pActor->m_tempAction);
								pActor->MarkUpdate(targetMask);
								pActor->MarkUpdate(actionMask);
								UInt32 Time = 0;
								pActor->DoContinueAction(this,  true, Time);
								m_pBattle->m_animTime += Time;
								//break immediately
								index = 2;
								m_buffActing = BOOLEAN_FALSE; 
								if (pActor)
									pActor->m_buffActing = BOOLEAN_FALSE; 

								activatedBuffID.push_back(Itr->second.ID);

								break;
							}
							else
							{
								SetAction(m_tempAction);
								MarkUpdate(targetMask);
								MarkUpdate(actionMask);
								UInt32 Time = 0;
								DoContinueAction(pActor,  true, Time);
								m_pBattle->m_animTime += Time;
								//break immediately
								index = 2;
								m_buffActing = BOOLEAN_FALSE; 
								if (pActor)
									pActor->m_buffActing = BOOLEAN_FALSE; 

								activatedBuffID.push_back(Itr->second.ID);

								break;
							}
						}
						else
						{
							if (ActiveType == BUFF_ACTIVE_TIME_TYPE_DO_HITTING )
							{
								if (m_tempReaction != GetReaction())
									SetReaction(m_tempReaction);
							}
							else 
							{
								if (m_BuffActive)
								{
									SetReaction(BATCMD_BUFF_EFFECT_HIT);									
									MarkUpdate(reactionMask);
									SetSubType(Itr->second.pBuffData->id);

									if (activeList)
									{
										activeList->push_back(this);
									}
									else
									{
										
										SetLinkNext(BOOLEAN_FALSE);


										UInt8 numShowCount = 0;

										{
											if (IsUpdated(CBatActor::HPMask) &&
												GetDShowHP())
												numShowCount ++;

											if (IsUpdated(CBatActor::SPMask) &&
												GetDShowSP())
												numShowCount ++;

											if (IsUpdated(CBatActor::DPMask) &&
												GetDShowDP())
												numShowCount ++;

											if (IsUpdated(CBatActor::WCMask) &&
												GetDShowWC())
												numShowCount ++;

											if (IsUpdated(CBatActor::VPMask) &&
												GetDShowVP())
												numShowCount ++;
										}


										m_pBattle->m_animTime += numShowCount * NUM_DISPLAY_TIME;

										WriteReaction();
									}
								}
							}
						}

						m_buffActing = BOOLEAN_FALSE; 
						if (pActor)
							pActor->m_buffActing = BOOLEAN_FALSE; 

						activatedBuffID.push_back(Itr->second.ID);
						
					}
				}
			}
		}
	}

	for (UInt16Vector::iterator Itr = activatedBuffID.begin();
		Itr != activatedBuffID.end(); ++Itr)
	{
		for (UInt16Vector::iterator ItrPostIDs = m_ActiveBuffIDs.begin();
			ItrPostIDs != m_ActiveBuffIDs.end(); ++ItrPostIDs)
		{		
			if ((*Itr) == (*ItrPostIDs))
			{
				m_ActiveBuffIDs.erase(ItrPostIDs);
				break;
			}
		}
	}
	
	
}

INLINE UInt8 ConvBlockValue(UInt8 buffBlock)
{
	UInt8 v = 0;

	switch(buffBlock)
	{
	case BUFF_BLOCK_COMMAND_ATTACK:
		v = BATCMD_ATTACK;
		break;
	case BUFF_BLOCK_COMMAND_DEFEND:
		v = BATCMD_DEFEND;
		break;
	case BUFF_BLOCK_COMMAND_SKILL:
		v = BATCMD_SKILL;
		break;
	case BUFF_BLOCK_COMMAND_SPECAIL:
		v = BATCMD_SPECIAL;
		break;
	case BUFF_BLOCK_COMMAND_ITEM:
		v = BATCMD_ITEM;
		break;
	case BUFF_BLOCK_COMMAND_FRIEND:
		v = BATCMD_FRIEND;
		break;
	case BUFF_BLOCK_COMMAND_PROTECT:
		v = BATCMD_PROTECT;
		break;
	case BUFF_BLOCK_COMMAND_CATCH:
		v = BATCMD_CATCH;
		break;
	case BUFF_BLOCK_COMMAND_HELP:
		v = BATCMD_HELP;
		break;
	case BUFF_BLOCK_COMMAND_AUTO:
		v = BATCMD_AUTO;
		break;
	case BUFF_BLOCK_COMMAND_ESCAPE:
		v = BATCMD_ESCAPE;
		break;
	case BUFF_BLOCK_COMMAND_ALL:
		v = MAX_ACTION_CMD;
		break;
	}

	return v;
}

void CBatActor::UpdateBlockCmd()
{
	UInt8 v;

	ZeroMemory(m_cmdBlockMap, sizeof(m_cmdBlockMap));
	for (Int i = 0; i < 2; i ++)
	{
		BuffMap tempMap = m_BuffMap;
		if (i == 1)
			tempMap = m_BuffCacheMap;

		for (BuffMap::iterator Itr = tempMap.begin(); Itr != tempMap.end(); Itr++)
		{
			ExtractedBuffData &extBuff = Itr->second;
			if (extBuff.pBuffData && extBuff.StartDelay == 0)
			{
				for (Int j = 0; j < (Int)extBuff.pBuffData->Buff_Blockcommands.size(); j++)
				{
					v = ConvBlockValue((UInt8) extBuff.pBuffData->Buff_Blockcommands[j]);
					if (v == MAX_ACTION_CMD) {
						for (UInt8 i = 1; i < MAX_ACTION_CMD; i++)
							m_cmdBlockMap[i] |= 0x02;
						break;
					} else if (v != 0) {
						m_cmdBlockMap[v] |= 0x02;
					}
				}

				for (Int j = 0; j < (Int)extBuff.pBuffData->Buff_BlockActions.size(); j++)
				{
					v = ConvBlockValue((UInt8) extBuff.pBuffData->Buff_BlockActions[j]);
					if (v == MAX_ACTION_CMD) {
						for (UInt8 i = 1; i < MAX_ACTION_CMD; i++)
							m_cmdBlockMap[i] |= 0x01;
						break;
					} else if (v != 0) {
						m_cmdBlockMap[v] |= 0x01;
					}
				}
			}
		}
	}

	if (m_pBattle)
	{
		if (m_pBattle->GetBattleType() == LOW_MARTIAL_BATTLE ||
			m_pBattle->GetBattleType() == HIGH_MARTIAL_BATTLE)
		{
			m_cmdBlockMap[BATCMD_HELP] |= 0x01;
			m_cmdBlockMap[BATCMD_HELP] |= 0x02;
		}
	}
	// check if any command blocked
	UInt8 b1 = 0;
	UInt8 b2 = 0;
	for (UInt8 i = 1; i < MAX_ACTION_CMD; i++) {
		if (m_cmdBlockMap[i] > 0)
			b1 ++;
		if (m_cmdBlockMap[i] > 1)
			b2 ++;
	}
	if (b1 == MAX_ACTION_CMD - 1)
		m_cmdBlockMap[MAX_ACTION_CMD] |= 0x01;
	if (b2 == MAX_ACTION_CMD - 1)
		m_cmdBlockMap[MAX_ACTION_CMD] |= 0x02;

	if (IsCmdBlocked(m_command) ||
		(m_pBattle && m_pBattle->IsCmdBlocked(m_command)))
		m_command = BATCMD_IDLE;
}

void CBatActor::UpdateBlockSkill()
{
	m_BlockSkills.clear();

	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_BlockSkills.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockSkills.size(); i++)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(Itr->second.pBuffData->Buff_BlockSkills[i]);
				if (pSkillData)
					m_BlockSkills.insert(std::make_pair(Itr->second.pBuffData->Buff_BlockSkills[i], pSkillData));
			}

		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_BlockSkills.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockSkills.size(); i++)
			{
				const SkillData * pSkillData = gGlobal.m_resource->GetSkillData(Itr->second.pBuffData->Buff_BlockSkills[i]);
				if (pSkillData)
					m_BlockSkills.insert(std::make_pair(Itr->second.pBuffData->Buff_BlockSkills[i], pSkillData));
			}
		}
	}
}

bool CBatActor::IsAntiBuff(UInt16 Buff_id)
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_AntiBuffs.size(); i++)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_AntiBuffs.size() > 0)
			{
				if (Itr->second.pBuffData->Buff_AntiBuffs[i] == Buff_id)
					return true;
			}
		}
	}

	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_AntiBuffs.size(); i++)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_AntiBuffs.size() > 0)
			{
				if (Itr->second.pBuffData->Buff_AntiBuffs[i] == Buff_id)
					return true;
			}
		}
	}
	return false;
}

UInt16 CBatActor::GetSkillItemATK()
{
	UInt8 bagIndex = (UInt8) m_cmdAdditional;
	CItem* item;
	// if can't use item, then auto attack
	if (m_pClient->pBatChar->m_pBag == NULL || bagIndex >= BAG_ITEM_COUNT || 
		(item = m_pClient->pBatChar->m_pBag->GetItem(bagIndex)) == NULL)
	{
		return 0;
	}

	if (item->GetATK() != 0)
		return item->GetATK();//GetATK();

	const ItemData * pItemData = gGlobal.m_resource->GetItemData(item->GetItem_id());
	if (pItemData)
	{
		if (pItemData->ATK_u > 0)
			return pItemData->ATK_u;
		else
			return pItemData->ATK_d;
	}
	return 0;
}

void CBatActor::SetAIType(PCSTRINGA value)
{
	m_AI = value;

	if (!m_AI.IsEmpty())
	{
		CallLuaFunc(gGlobal.g_pLuaState, m_AI.c_str(),  String(), GetBattle(), this);
	}
}

void CBatActor::CalSpeed()
{
	gGlobal.g_TempResult = 0;
	CallLuaFunc(gGlobal.g_pLuaState, "speedOrder", String(), this);
	m_speed = (float)gGlobal.g_TempResult;
}
void CBatActor::CallHelper(UInt32 mob_id, UInt16 Level, UInt8 pos)
{
	const MobData * pMobData = gGlobal.m_resource->GetMobData(mob_id);
	if (pMobData  && m_pBattle)
	{		
		CBatMob * pMob = NULL;		
		pMob = CBatMob::NewInstance();						

		pMob->SetData(mob_id, Level);
				
		pMob->SetBattle(m_pBattle);
		pMob->SetTeam(GetTeam());
		pMob->SetLeftBattle(BOOLEAN_FALSE);
		pMob->SetPosition(pos);
		pMob->MobLearnSkill();
		
		m_pBattle->InsertActor(pMob);		

		pMob->m_command = BATCMD_IDLE;
		pMob->SetAction(BATCMD_IDLE);		
		SetLinkNext(BOOLEAN_FALSE);

		SetAction(BATCMD_SKILL);
		SetTarget(pMob->GetIndex());
		MarkUpdate(targetMask);
		MarkUpdate(subTypeMask);
		SetLinkNext(BOOLEAN_FALSE);					// last link of attack
		WriteAction();						// write action to buffer
		m_bAction = BOOLEAN_TRUE;

		SetSubType(m_cmdSubType);
		MarkUpdate(targetMask);
		MarkUpdate(subTypeMask);
		SetReaction(BATCMD_SKILL_HIT);
		SetLinkNext(BOOLEAN_FALSE);					// last link of attack
		WriteReaction();						// write action to buffer

		SetReaction(BATCMD_SHOW_FRIEND);
		WriteReaction();
		//pMob
		//CB_AddActors(pPeer, context, &addedGrp, client.team);
		m_pBattle->m_friendGroup.Add(pMob);			// added to coming friend group
		m_CancelAction = true;
	}
}

const SkillData * CBatActor::GetSkillCsvData(UInt16 skill_id)
{
	return gGlobal.m_resource->GetSkillData(skill_id);
}


void CBatActor::DoAction(CBatActor * pBatTarget)
{
	TRACE_ENSURE(m_pBattle != NULL);

	if (pBatTarget == NULL)
		return;

	if (!IsValidTarget())
		return;

	if (pBatTarget->GetIndex() != GetTarget())
		SetTarget(pBatTarget->GetIndex());

	CBatActor *pTarget = pBatTarget;
	if (pTarget == NULL || !pTarget->IsValidTarget()) {
		return;
	}
	UInt8 OrgActID = GetAction();

	SetTempAction(GetAction());
	pTarget->SetTempReaction(pTarget->GetReaction());
	DoBuffActive(pTarget, BUFF_ACTIVE_TIME_TYPE_DO_HITTING_ATTACKER);
	SetAction(m_tempAction);
	pTarget->SetReaction(pTarget->GetReaction());

	SetTempAction(GetAction());
	pTarget->SetTempReaction(pTarget->GetReaction());
	pTarget->DoBuffActive(this, BUFF_ACTIVE_TIME_TYPE_DO_HITTING);
	SetAction(m_tempAction);
	pTarget->SetReaction(pTarget->GetReaction());
	// mark source action attack and target
	MarkUpdate(targetMask);
	SetLinkNext(BOOLEAN_FALSE);

	// write this actor, target or protector action/reaction to buffer
	WriteAction();

	pTarget->SetLinkNext(BOOLEAN_FALSE);
	pTarget->WriteReaction();

	SetAction(OrgActID);
}

void CBatActor::ChangeTarget(UInt8 target)
{
	m_cmdTarget= target;
	if (m_cmdTarget != GetTarget())
		SetTarget(m_cmdTarget);
}

void CBatActor::AttackAnimTime(CBatActor * pTarget)
{
	// calculate attack animation time
	UInt16 subId = 0;

	if (m_pBatChar && m_pBag) {
		if (m_pClient && GetSession(m_pClient->GetContext().sessionId) == NULL) {
			TRACE_ERRORDTL_5(GLOBAL_LOGGER, _FE("session not found sessionId %s, actor index %d, battle %d:%d:%d"),
				m_pClient->GetContext().sessionId.GetString().c_str(), GetIndex(),
				m_pBattle->GetID(), m_pBattle->GetNetId().srvId, m_pBattle->GetNetId().objId);
		}

		CItem *pItem = m_pBag->GetItem(BAG_MAX_ITEM + EQPLACE_WEAPON);
		if (pItem)
			subId = pItem->GetItem_id();
	}
	m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, pTarget, GetAction(),
		subId);
}

bool CBatActor::HasParty()
{
	if (m_pBatChar && m_pBatChar->m_pOwner)
		return m_pBatChar->m_pOwner->HasParty();

	if (m_pMob && m_pMob->IsParnter() && m_pMob->m_pClient && m_pMob->m_pClient->pBatChar &&
		m_pMob->m_pClient->pBatChar->m_pOwner)
		return m_pMob->m_pClient->pBatChar->m_pOwner->HasParty();

	return false;
}

bool CBatActor::CheckBlockItem(UInt8 itemType)
{
	for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockItemType.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockItemType.size(); i++)
			{
				if (Itr->second.pBuffData->Buff_BlockItemType[i] == itemType)
					return true;
			}
		}
	}
	for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
	{
		if (Itr->second.pBuffData && 
			Itr->second.pBuffData->Buff_BlockItemType.size() > 0)
		{
			for (Int i = 0; i < (Int)Itr->second.pBuffData->Buff_BlockItemType.size(); i++)
			{
				if (Itr->second.pBuffData->Buff_BlockItemType[i] == itemType)
					return true;
			}
		}
	}
	return false;
}

void CBatActor::SetTempReaction(UInt8 reaction) 
{ 
	m_tempReaction = reaction; 
	if (m_tempReaction == BATCMD_COUNTER_ATTACK_HIT ||
		m_tempReaction == BATCMD_COUNTER_ATTACK_DEF_HIT)
	{
		m_pBattle->m_animTime += gGlobal.m_animTimer->GetActionTime(m_pBatChar, m_pMob, NULL,
			m_tempReaction, 0, 1);
	}
}

void CBatActor::SetTempAction(UInt8 action) 
{ 
	m_tempAction = action; 
}

void CBatActor::SendBattleMsg(UInt8 msgType, const String &msg, UInt16 subType)
{
	if (m_pClient && m_pClient->IsConnect())
		CB_BattleMsg(m_pClient->pOwner->GetPeer(), m_pClient->pOwner->GetContext(), 
			msgType, msg, subType);
}

void CBatActor::SendCommandSuccess(UInt8 cmd)
{
	if (m_pClient && m_pClient->IsConnect())
		CB_CommandSuccess(m_pClient->pOwner->GetPeer(), m_pClient->pOwner->GetContext(), cmd);
}

bool CBatActor::IsValidTargetType(UInt8 targetType, CBatActor * pTarget)
{
	if (pTarget == NULL)
		return false;

	bool valid = true;

	switch(targetType)
	{
	case TARGET_TYPE_ANY:
		break;

	case TARGET_TYPE_SELF:
		if (pTarget != this)
			valid = false;
		break;

	case TARGET_TYPE_ANY_SAME_TEAM:
		if (!m_ignoreTeam && GetTeam() != pTarget->GetTeam())
			valid = false;
		break;

	case TARGET_TYPE_ANY_ENEMY_TEAM:
		if (!m_ignoreTeam && GetTeam() == pTarget->GetTeam())
			valid = false;
		break;

	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PARNTER:
		if (!m_ignoreTeam && GetTeam() == pTarget->GetTeam() ||
			pTarget->IsParnter() || pTarget->IsMob())
			valid = false;
		break;

	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS_PLAYER:
		if (!m_ignoreTeam && GetTeam() == pTarget->GetTeam() ||
			pTarget->IsBoss() || pTarget->IsPlayer())
			valid = false;
		break;

	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_MOB_PLAYER_PARNTER:
		if (!m_ignoreTeam && GetTeam() == pTarget->GetTeam() ||
			pTarget->IsParnter() || pTarget->IsPlayer() ||
			(pTarget->IsMob() && !pTarget->IsBoss()))
			valid = false;
		break;

	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_BOSS:
		if (!m_ignoreTeam && GetTeam() == pTarget->GetTeam() ||
			pTarget->IsBoss())
			valid = false;
		break;

	case TARGET_TYPE_ANY_PARNTER:
		if (!pTarget->IsParnter())
			valid = false;
		break;

	case TARGET_TYPE_ANY_ENEMY_TEAM_WITHOUT_PLAYER_PARNTER:
		if (!m_ignoreTeam && GetTeam() == pTarget->GetTeam() ||
			pTarget->IsParnter() || pTarget->IsPlayer())
			valid = false;
		break;

	case TARGET_TYPE_ANY_PET:
		break;

	case TARGET_TYPE_ANY_SAME_TEAM_PLAYER:
		if (!m_ignoreTeam && GetTeam() != pTarget->GetTeam() ||
			!pTarget->IsPlayer())
			valid = false;
		break;

	case TARGET_TYPE_ANY_SAME_TEAM_PARNTER:
		if (!m_ignoreTeam && GetTeam() != pTarget->GetTeam() ||
			!pTarget->IsParnter())
			valid = false;
		break;

	case TARGET_TYPE_ANY_SAME_TEAM_PET:
		break;
	}

	return valid;
}

CItem * CBatActor::FindItemByid(UInt32 item_id)
{
	if (m_pBag)
		return m_pBag->FindItemById(item_id, 0, 0);

	return NULL;
}

CPet* CBatActor::GetUsingPet()
{
	if (m_pBatChar && m_pBatChar->m_pOwner && m_pBatChar->m_pOwner->m_pPetGroup)
	{
		return m_pBatChar->m_pOwner->m_pPetGroup->GetPet(m_pBatChar->m_pOwner->GetPet_uid());
	}
	return NULL;
}

void CBatActor::PetActive()
{
	SetPet_active(true);
	MarkUpdate(pet_activeMask);
}

CScriptSystem * CBatActor::GetScript()
{
	if (m_pBatChar && m_pBatChar->m_pOwner)
		return m_pBatChar->m_pOwner->GetScript();
	return NULL;
}

RPCResult CBatActor::CancelAuto(LPCPEER pPeer, RPCContext &context)
{
	BatClient *pClient = m_pBattle->GetClient(context.sessionId);
	if (pClient && pClient->bAuto)
	{
		pClient->bAuto = BOOLEAN_FALSE;
		pClient->autoCount = 0;
		pClient->pBatChar->StopAutoEvent();
		CB_AutoCount(pPeer, context, pClient->autoCount);
	}
	return RPC_RESULT_OK;
}

void CBatActor::ClearWineBuff()
{
	StlVector<UInt16> RmIDLst;
		for (BuffMap::iterator Itr = m_BuffMap.begin(); Itr != m_BuffMap.end(); Itr++)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_WINE)
			{
				RmIDLst.push_back(Itr->second.pBuffData->id);
			}
		}

		for (BuffMap::iterator Itr = m_BuffCacheMap.begin(); Itr != m_BuffCacheMap.end(); Itr++)
		{
			if (Itr->second.pBuffData && Itr->second.pBuffData->Buff_type == BUFF_TYPE_WINE)
			{
				RmIDLst.push_back(Itr->second.pBuffData->id);
			}
		}

		for (UInt i = 0; i < (UInt)RmIDLst.size(); i++)
		{
			RemoveBuff(RmIDLst[i]);
		}
}
void CBatActor::WineUpdate()
{
	/*Int DPDrop = (GetMaxDP() * 0.1);
	Int TPDrop = (GetMaxDP() * 0.05);

	if (GetDP() > DPDrop)
	{
		SetDP(GetDP() - DPDrop, -DPDrop, -DPDrop);
		CalcAttrib();
		
	}
	else
	{
		SetDP(0, -(Int)GetDP(), -(Int)GetDP());
		//clear the wine buff
		ClearWineBuff();
		
	}

	
	if (GetTP() > TPDrop)
	{
		SetTP(GetTP() - TPDrop);
	}
	else
	{
		SetTP(0);
	}
	*/
}

void CBatActor::SetIsInput(Boolean input)
{
	m_bInput = input;
	if (input && GetClassID() == BatCharacter_CLASSID && !HasPostSkill())
	{
		CBatCharacter *pChar = CastNetObject<CBatCharacter>(this);
		if (pChar)
			pChar->StopAutoEvent();
	}
}
