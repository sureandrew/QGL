//-- Common
#include "Common.h"
//-- Self
#include "SimClient/Battle.h"
//-- Library
#include "SimClient/Global.h"
#include "Proxy/Logic/BatActor.h"
#include "SimClient/BatMob.h"
#include "SimClient/BatCharacter.h"
#include "SimClient/Global.h"	
#include "Common/BattleCode.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"
#include "Resource/ClientResManager.h"
#include "Resource/ResItem.h"

#define BATTLE_TURNDEAD_TIME		30000

REGISTER_CLASSTYPE(CBattle, Battle_CLASSID);

PROTO_LuaPush(CBattle)
PROTO_LuaPush(CBatCharacter)
PROTO_LuaPush(CBatMob)

RPCResult CBattle::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_PlayTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum,
							   const ByteArray &array, const UInt32 &totalSize)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_T("CB_PlayTurn: battle %d,%d, turn %d, total size %d"), 
		GetNetId().srvId, GetNetId().objId, turnNum, totalSize);

	m_turnBuffer.ResetCursor();
	m_turnBuffer.WriteArray(array.GetData(), (UInt32) array.GetSize());
	if (m_turnBuffer.GetCursor() >= 
		totalSize + REUBEN_NETWORK_BUFFER_HEADER_SIZE)			// no more array
		PlayAction(pPeer, context, totalSize);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_PlayTurnMore(LPCPEER pPeer, RPCContext &context, const ByteArray &array,
								   const UInt32 &totalSize)
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_T("CB_PlayTurnMore: battle %d,%d, array size %d, total size %d"), 
		GetNetId().srvId, GetNetId().objId, array.GetSize(), totalSize);

	m_turnBuffer.WriteArray(array.GetData(), (UInt32) array.GetSize());
	if (m_turnBuffer.GetCursor() >= 
		totalSize + REUBEN_NETWORK_BUFFER_HEADER_SIZE)			// no more array
		PlayAction(pPeer, context, totalSize);

	return RPC_RESULT_OK;
}

void CBattle::PlayAction(LPCPEER pPeer, RPCContext &context, UInt32 totalSize)
{
	// reset battle turn dead lock time
	if (gGlobal.m_pSimClient)
		gGlobal.m_pSimClient->m_batDeadlock = 0;

	Size size = (Size) totalSize;
	UInt8 index = 0;
	CBatActor *pActor = NULL;
	CBatActor *pTarget = NULL;
	Char srcName[81];
	Char targetName[81];
	UInt8 curStep = 0;

	m_turnBuffer.ResetCursor();
	while (m_turnBuffer.GetCursor() < size)
	{
		m_turnBuffer.Read<UInt8>(index);		// get actor index
		if (index >= m_actors.size())
		{
			String s;
			for (Index idx = C_INDEX(0); idx < m_actors.size(); ++idx)
			{
				pActor = m_actors[idx];
				if (pActor->GetClassID() == BatMob_CLASSID)
				{
					CBatMob *pmob = CastNetObject<CBatMob>(pActor);

					if (pmob->GetPartner_id() == 0)					// mob
						s.Format(_T("Mob %d: objId %d, mob_id %d"),
						pmob->GetIndex(), pmob->GetID(), pmob->GetMob_id());
					else
						s.Format(_T("Partner %d: objId %d, partner_id %d, owner_id %d"),
						pmob->GetIndex(), pmob->GetID(), pmob->GetPartner_id(), 
						pmob->GetOwner_id());
					s.AppendFormat(_T(", level %d, hp %d/%d, sp %d/%d, state %d"),
						pmob->GetLevel(), pmob->GetHP(), pmob->GetMHP(), pmob->GetSP(), 
						pmob->GetMSP(), pmob->GetState());
					TRACE_INFOL(GLOBAL_LOGGER, s.c_str());
					TRACE_INFOL_6(GLOBAL_LOGGER, _F("       ATK %d, DEF %d, SPD %d, POW %d, HIT %d, EVA %d"),
						pmob->GetATK(), pmob->GetDEF(), pmob->GetSPD(), pmob->GetPOW(),
						pmob->GetHIT(), pmob->GetEVA());
				}
				else if (pActor->GetClassID() == BatCharacter_CLASSID)
				{
					CBatCharacter *pchar = CastNetObject<CBatCharacter>(pActor);
					TRACE_INFOL_9(GLOBAL_LOGGER, _F("Character %d: objId %d, char_id %d, level %d, hp %d/%d, sp %d/%d, state %d"),
						pchar->GetIndex(), pchar->GetID(), pchar->GetChar_id(),
						pchar->GetLevel(), pchar->GetHP(), pchar->GetMHP(), 
						pchar->GetSP(), pchar->GetMSP(), pchar->GetState());
					TRACE_INFOL_6(GLOBAL_LOGGER, _F("             ATK %d, DEF %d, SPD %d, POW %d, HIT %d, EVA %d"),
						pchar->GetATK(), pchar->GetDEF(), pchar->GetSPD(), pchar->GetPOW(),
						pchar->GetHIT(), pchar->GetEVA());
				}
			}
		}
		TRACE_ENSURE(index < m_actors.size());
		pActor = m_actors[index];
		TRACE_ENSURE(pActor != NULL);
		pActor->ClearUpdateMask();
		pActor->ReadAttr(m_turnBuffer);	// read attributes for that actors

		if (pActor->GetClassID() == BatMob_CLASSID) {
			CBatMob *pmob = CastNetObject<CBatMob>(pActor);
			if (pmob && pmob->GetPartner_id() != 0)
				_stprintf(srcName, _T("Partner %d"), index);
			else
				_stprintf(srcName, _T("Mob %d"), index);
		} else if (pActor->GetClassID() == BatCharacter_CLASSID)
			_stprintf(srcName, _T("Character %d"), index);
		if (pActor->IsUpdated(CBatActor::targetMask))
		{
			if (pActor->GetTarget() < (UInt8) m_actors.size())
			{
				pTarget = m_actors[pActor->GetTarget()];
				if (pTarget->GetClassID() == BatMob_CLASSID) {
					CBatMob *pmob = CastNetObject<CBatMob>(pTarget);
					if (pmob && pmob->GetPartner_id() != 0)
						_stprintf(targetName, _T("Partner %d"), pActor->GetTarget());
					else
						_stprintf(targetName, _T("Mob %d"), pActor->GetTarget());
				} else if (pTarget->GetClassID() == BatCharacter_CLASSID)
					_stprintf(targetName, _T("Character %d"), pActor->GetTarget());
			}
			else if (pActor->GetTarget() == BATIDX_ALL)
			{
				_stprintf(targetName, _T("ALL"));
			}
			else if (pActor->GetTarget() == BATIDX_TEAM0 || pActor->GetTarget() == BATIDX_TEAM1)
			{
				_stprintf(targetName, _T("Team %d"), pActor->GetTarget() - BATIDX_TEAM);
			}
			else
			{
				TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("PlayTurn Error: invalid target %d"), pActor->GetTarget());
				continue;
			}
		}

		if (pActor->IsUpdated(CBatActor::pet_activeMask))
		{
			String petFight = pActor->GetPet_active()? _T("Pet Fight"):_T("Pet Not Fight");
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%s %s"), targetName, petFight.c_str()); 	
		}

		// play action
		if (pActor->IsUpdated(CBatActor::actionMask))
		{
			if (pActor->GetLinkNext() == BOOLEAN_FALSE)
			{
				curStep++;
			}
			switch (pActor->GetAction())
			{
			case BATCMD_IDLE:
				{
					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%d: %s has no action"), curStep, srcName);
				}
				break;
			case BATCMD_ATTACK:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%d: %s attack %s"), curStep, srcName, targetName);
				}
				break;
			case BATCMD_SKILL:
				{
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("%d: %s use skill %d on %s"), curStep, srcName,
						pActor->GetSubType(), targetName);
				}
				break;
			case BATCMD_CRIT_ATTACK:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%d: %s critical attack %s"), curStep, srcName, targetName);
				}
				break;
			case BATCMD_ITEM:
				{
					const ItemData *pData = gGlobal.m_resource->GetItemData(pActor->GetSubType());
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("%d: %s use item %d on %s"), curStep, srcName, 
						pActor->GetSubType(), targetName);
				}
				break;
			case BATCMD_ESCAPE:
				{
					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%d: %s escaped from battle"), curStep, srcName);
					if (gGlobal.m_pSimClient &&	gGlobal.m_pSimClient->m_pBatChar && 
						gGlobal.m_pSimClient->m_pBatChar->GetIndex() == index)
					{
						// end turn after you escaped
						size = m_turnBuffer.GetCursor();
					}
				}
				break;
			case BATCMD_ESCAPE_FAIL:
				{
					TRACE_INFODTL_2(GLOBAL_LOGGER, _F("%d: %s escaped failed"), curStep, srcName);
				}
				break;
			case BATCMD_CATCH:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%d: %s try to catch %s"), curStep, srcName, targetName);
				}
			}
		}

		// play reaction
		if (pActor->IsUpdated(CBatActor::reactionMask)) {
			switch (pActor->GetReaction()) {
			case BATCMD_MISS:
				{
					TRACE_INFODTL_1(GLOBAL_LOGGER, _F("    %s evaded the attack (miss)"), srcName);
				}
				break;
			case BATCMD_NORM_HIT:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("    %s hitted (%d) -> hp remain %d"), srcName, 
						pActor->GetDHP(), pActor->GetHP());
				}
				break;
			case BATCMD_CRIT_HIT:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("    %s critical hitted (%d) -> hp remain %d"), srcName, 
						pActor->GetDHP(), pActor->GetHP());
				}
				break;
			case BATCMD_BREAK_HIT:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("    %s blocked hitted (%d) -> hp remain %d"), srcName, 
						pActor->GetDHP(), pActor->GetHP());
				}
				break;
			case BATCMD_DEFEND_HIT:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("    %s defend hitted (%d) -> hp remain %d"), srcName, 
						pActor->GetDHP(), pActor->GetHP());
				}
				break;
			case BATCMD_PROTECT_HIT:
				{
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("    %s protect %s hitted (%d) -> hp remain %d"), srcName, 
						targetName, pActor->GetDHP(), pActor->GetHP());
				}
				break;
			case BATCMD_SKILL_HIT:
				{
					TRACE_INFODTL_4(GLOBAL_LOGGER, _F("    %s hitted (%d) by skill %d -> hp remain %d"),
						srcName, pActor->GetDHP(), pActor->GetSubType(), pActor->GetHP());
				}
				break;
			case BATCMD_KNOCKBACK:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("    %s knockback hitted (%d) -> hp remain %d"), srcName, 
						pActor->GetDHP(), pActor->GetHP());
				}
				break;
			case BATCMD_USED_ITEM:
				{
					TRACE_INFODT_1(GLOBAL_LOGGER, _F("    %s used item: "), srcName);
					if (pActor->IsUpdated(CBatActor::HPMask))
						TRACE_INFO_2(GLOBAL_LOGGER, _F("restore HP %d -> %d "), pActor->GetDHP(), pActor->GetHP());
					if (pActor->IsUpdated(CBatActor::SPMask))
						TRACE_INFO_2(GLOBAL_LOGGER, _F("restore SP %d -> %d "), pActor->GetDSP(), pActor->GetSP());
					TRACE_INFOL(GLOBAL_LOGGER, _F(""));
				}
				break;
			case BATCMD_CATCHED:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%d: %s is caught by %s"), curStep, srcName, targetName);
				}
				break;
			case BATCMD_CATCHED_FAIL:
				{
					TRACE_INFODTL_3(GLOBAL_LOGGER, _F("%d: %s is not caught by %s"), curStep, srcName, targetName);
				}
				break;
			}
		}
	}

	// end turn after played the actions
	TRACE_VERBOSEDTL_1(GLOBAL_LOGGER, _F("Battle %d EndTurn:"), GetID());
	EndTurn(pPeer, context);
	m_turnBuffer.ResetCursor();
}

RPCResult CBattle::CB_NextTurn(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum,
							   const UInt32 &timeLeft, const UInt16 &autoCount, 
							   const Boolean &bInput, const Boolean &bPartnerInput)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("NextTurn %d: timeout %d, autoCount %d"), 
		turnNum, timeLeft, autoCount);

	ShowBattle();
	if (gGlobal.m_pSimClient) {
		if (gGlobal.m_pSimClient->m_pBatChar)
		{
			TRACE_INFOL(GLOBAL_LOGGER, _F("Please Enter Battle Command:"));
		}
		else
		{
			TRACE_INFOL(GLOBAL_LOGGER, _F("Monitoring Battle:"));
		}

	//gGlobal.Respond(_T("Battle Turn"));
		if (gGlobal.m_pSimClient->m_bRobot) {
			if (gGlobal.m_pSimClient->m_pBatChar && autoCount == 0)
			{
				CBatCharacter *pBatChar = gGlobal.m_pSimClient->m_pBatChar;
				CBatMob *pPartner = gGlobal.m_pSimClient->m_pPartner;

				pBatChar->m_commandOk = bInput;
				if (pPartner)
					pPartner->m_commandOk = bPartnerInput;
				if (!bInput) {						// character can input
					CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "BattleTurn", String(), this, 
						pBatChar, (int)turnNum);
				}
				if (!bPartnerInput && pPartner) {	// partner can input
					CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "PartnerTurn", String(), this, 
						pBatChar, pPartner);
				}
			} else if (gGlobal.m_pSimClient->m_pBatChar == NULL) {	// monitor battle
				CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "MonitorTurn", String(), this,
					(int)turnNum);
			}
		}
		// count down battle turn dead lock time
		gGlobal.m_pSimClient->m_batDeadlock = BATTLE_TURNDEAD_TIME;
	}

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_EndBattle(LPCPEER pPeer, RPCContext &context, const UInt8 &endState)
{
	switch (endState)
	{
	case END_WIN:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You have won in battle"));
		break;
	case END_LOSE:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You have lost in battle"));
		break;
	case END_ESCAPED:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You have escaped from battle"));
		break;
	case END_DRAW:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: Draw, no one win or lose"));
		break;
	case END_MONITOR:
		TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_EndBattle: You monitor the battle which is ended"));
		break;
	default:
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_EndBattle Error: endState %d"), endState);
	}

	CheckEnd(endState);				// check end battle correctness
	//gGlobal.Respond(_T("Battle End"));
	if (gGlobal.m_pSimClient && gGlobal.m_pSimClient->m_bRobot)
		CallLuaFunc(gGlobal.m_pSimClient->m_pLuaState, "BattleEnd", String());

	if (gGlobal.m_pSimClient)
		gGlobal.m_pSimClient->m_batDeadlock = 0;

	//DeleteNetObject(this);
	DeleteNetObject(gGlobal.m_pSimClient->m_pBattle);

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_AddActors(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &Team)
{
	CBatActor* pActor;
	for (UInt16 i = 0; i < grp->GetCount(); ++i) {
		pActor = grp->GetChild<CBatActor>(i);
		if (pActor == NULL) continue;

		TRACE_INFODTL_3(GLOBAL_LOGGER, _F("CB_AddActor: add new actor (%d,%d), index %d"), 
			pActor->GetNetId().srvId, pActor->GetNetId().objId, pActor->GetIndex());

		UInt8 index = pActor->GetIndex();
		if (index < m_actors.size())
		{
			// replace actor that escaped
			CBatActor *pOldActor = m_actors[index];
			m_actors[index] = pActor;
			if (pActor != pOldActor)
				DeleteNetObject(pOldActor);
		}
		else
		{
			// append actor to actor list
			m_actors.push_back(pActor);
		}
	}

	DeleteNetGroup(grp);



	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_ControlCharacter(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CB_ControlCharacter: control character %d"), charIdx);

	if (charIdx < m_actors.size())
	{
		gGlobal.m_pSimClient->m_pBatChar = CastNetObject<CBatCharacter>(m_actors[charIdx]);
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CB_ControlCharacter Error: wrong character index %d"), charIdx);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_ControlPartner(LPCPEER pPeer, RPCContext &context, const UInt8 &charIdx, const UInt8 &mobIdx)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_ControlCharacter: control partner %d of character %d"), mobIdx, charIdx);

	if (mobIdx < m_actors.size())
	{
		gGlobal.m_pSimClient->m_pPartner = CastNetObject<CBatMob>(m_actors[mobIdx]);
	}
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CB_ControlPartner Error: wrong mob index %d"), mobIdx);
		return RPC_RESULT_FAIL;
	}

	return RPC_RESULT_OK;
}

CBattle::~CBattle()
{
	ClearContent();
}

CBatCharacter* CBattle::GetCharacter(UInt8 index)
{
	if (index < (UInt8) m_actors.size())
		return CastNetObject<CBatCharacter>(m_actors[index]);
	else
		return NULL;
}

CBatMob* CBattle::GetMob(UInt8 index)
{
	if (index < (UInt8) m_actors.size())
		return CastNetObject<CBatMob>(m_actors[index]);
	else
		return NULL;
}

UInt16 CBattle::GetTeamCount(UInt8 team)
{
	UInt16 count = 0;
	CBatActor *actor;

	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		actor = *it;
		if (!actor->GetLeftBattle() && actor->GetTeam() == team) {
			++count;
		}
	}

	return count;
}

void CBattle::ClearContent()
{
	for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		DeleteNetObject(*it);
	}
	m_actors.clear();
	gGlobal.m_pSimClient->m_pBatChar = NULL;
	gGlobal.m_pSimClient->m_pPartner = NULL;
	gGlobal.m_pSimClient->m_pBattle = NULL;
}

void CBattle::ShowAllDetail()
{
	Actors::iterator it;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Battle %d,%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());

	// show mobs
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->GetClassID() == BatMob_CLASSID) {
			CBatMob *pmob = CastNetObject<CBatMob>(*it);
			if (pmob->GetLeftBattle()) {
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Mob %d: already left"), pmob->GetIndex());
			} else {
				TRACE_INFOL_3(GLOBAL_LOGGER, _F("Mob %d: id %d, state %d"),
					pmob->GetIndex(), pmob->GetMob_id(), pmob->GetState());
			//TRACE_INFOL_6(GLOBAL_LOGGER, _F("      : ATK %d, DEF %d, SPD %d, POW %d, HIT %d, EVA %d"),
			//	pmob->GetATK(), pmob->GetDEF(), pmob->GetSPD(), pmob->GetPOW(),
			//	pmob->GetHIT(), pmob->GetEVA());
			}
		}
	}

	// show characters
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->GetClassID() == BatCharacter_CLASSID) {
			CBatCharacter *pchar = CastNetObject<CBatCharacter>(*it);
			if (pchar->GetLeftBattle()) {
				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Character %d: already left"), 
					pchar->GetIndex());
			} else {
				TRACE_INFOL_8(GLOBAL_LOGGER, _F(
					"Character %d: id %d, level %d, hp %d/%d, sp %d/%d, state %d"),
					pchar->GetIndex(), pchar->GetChar_id(),
					pchar->GetLevel(), pchar->GetHP(), pchar->GetMHP(), 
					pchar->GetSP(), pchar->GetMSP(), pchar->GetState());
			//TRACE_INFOL_6(GLOBAL_LOGGER, _F("            : ATK %d, DEF %d, SPD %d, POW %d, HIT %d, EVA %d"),
			//	pchar->GetATK(), pchar->GetDEF(), pchar->GetSPD(), pchar->GetPOW(),
			//	pchar->GetHIT(), pchar->GetEVA());
			}
		}
	}
}

void CBattle::ShowBattle()
{
	Actors::iterator it;
	String s;

	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Show Battle %d,%d: actor count %d"),
		GetNetId().srvId, GetNetId().objId, m_actors.size());

	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->GetClassID() == BatMob_CLASSID)			// show mob or partner
		{
			CBatMob *pmob = CastNetObject<CBatMob>(*it);

			if (pmob->GetPartner_id() == 0)					// mob
				s.Format(_T("Mob %d: objId %d, mob_id %d"),
					pmob->GetIndex(), pmob->GetID(), pmob->GetMob_id());
			else
				s.Format(_T("Partner %d: objId %d, partner_id %d, owner_id %d"),
					pmob->GetIndex(), pmob->GetID(), pmob->GetPartner_id(), 
					pmob->GetOwner_id());
			TRACE_INFOL(GLOBAL_LOGGER, s.c_str());
			s.Format(_T("       level %d, hp %d/%d, sp %d/%d, state %d"),
				pmob->GetLevel(), pmob->GetHP(), pmob->GetMHP(), pmob->GetSP(), 
				pmob->GetMSP(), pmob->GetState());
			if (pmob->GetLeftBattle())
				s.AppendFormat(_T(", already left"));
			else
				s.AppendFormat(_T(", hp %d/%d, sp %d/%d, state %d"),
					pmob->GetHP(), pmob->GetMHP(), pmob->GetSP(), 
					pmob->GetMSP(), pmob->GetState());
			TRACE_INFOL(GLOBAL_LOGGER, s.c_str());
		}
		else if ((*it)->GetClassID() == BatCharacter_CLASSID) // show character
		{
			CBatCharacter *pchar = CastNetObject<CBatCharacter>(*it);
			s.Format(_T("Character %d: objId %d, char_id %d, team %d"),
				pchar->GetIndex(), pchar->GetID(), pchar->GetChar_id(), 
				pchar->GetTeam());
			TRACE_INFOL(GLOBAL_LOGGER, s.c_str());
			if (pchar->GetLeftBattle()) {
				TRACE_INFOL(GLOBAL_LOGGER, _F("             already left"));
			} else {
				TRACE_INFOL_6(GLOBAL_LOGGER, 
					_T("             level %d, hp %d/%d, sp %d/%d, state %d"),
					pchar->GetLevel(), pchar->GetHP(), pchar->GetMHP(), 
					pchar->GetSP(), pchar->GetMSP(), pchar->GetState());
			}
		}
	}
}

RPCResult CBattle::CB_Reward(LPCPEER pPeer, RPCContext &context, const Boolean &levelUp, 
							 const UInt32 &exp, const UInt32Vector &dropItems, 
							 const UInt32Vector &discardItems, const UInt32 &partnerExp, 
							 const Boolean &partnerLvlUp, const UInt32 &boundsExp, const UInt32 &boundsPartnerExp)
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Battle Won: You got %d exp."), exp);
	if (levelUp)
		TRACE_INFODT(GLOBAL_LOGGER, _F("And you have level up!!!"));
	if (!dropItems.empty())
	{
		TRACE_INFODT(GLOBAL_LOGGER, _F("And you got items: "));
		for (UInt32Vector::const_iterator it = dropItems.begin(); it != dropItems.end(); it++)
		{
			TRACE_INFO_1(GLOBAL_LOGGER, _F("%d, "), *it);
		}
		TRACE_INFOL(GLOBAL_LOGGER, _F(""));
	}
	if (!discardItems.empty())
	{
		TRACE_INFODT(GLOBAL_LOGGER, _F("Your current bag full! Discarded items: "));
		for (UInt32Vector::const_iterator it = discardItems.begin(); it != discardItems.end(); it++)
		{
			TRACE_INFO_1(GLOBAL_LOGGER, _F("%d, "), *it);
		}
		TRACE_INFOL(GLOBAL_LOGGER, _F(""));
	}
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_Penalty(LPCPEER pPeer, RPCContext &context, const UInt32 &exp, const UInt32 &money, const UInt32 &honor)
{
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Battle Lose: You have dead, and lose %d exp, %d money, %d honor"), exp, money, honor);
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_AddPartners(LPCPEER pPeer, RPCContext &context, NetGroup* grp, const UInt8 &Team)
{
	CB_AddActors(pPeer, context, grp, Team);
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_NeedHelp(LPCPEER pPeer, RPCContext &context, const Boolean &bNeed)
{
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("CB_NeedHelp: battle %d, need %d"), GetID(), bNeed);
	m_needHelp = bNeed;

	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_WaitTalk(LPCPEER pPeer, RPCContext &context, const UInt8 &turnNum, const UInt32 &turnTime)
{
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_CancelTalk(LPCPEER pPeer, RPCContext &context)
{
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_Choose(LPCPEER pPeer, RPCContext &context, const UInt8 &askerType, const UInt32 &askerId, const Boolean &bLeft, const UInt8 &total, const StringVector &msgList)
{
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_Say(LPCPEER pPeer, RPCContext &context, const String &msg, const int &imageType, const Boolean &bLeft, const UInt32 &npc_id)
{
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_DisableCmd(LPCPEER pPeer, RPCContext &context, const UInt32 &blockCmd)
{
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_HideCmd(LPCPEER pPeer, RPCContext &context, const UInt8 &index, const Boolean &hide)
{
	return RPC_RESULT_OK;
}

RPCResult CBattle::CB_ResetCounter(LPCPEER pPeer, RPCContext &context, 
								   const UInt32 &turnTime)
{
	return RPC_RESULT_OK;
}


RPCResult CBattle::CB_MartialResult(LPCPEER pPeer, RPCContext &context, const UInt8 &result, const UInt32 &money, const UInt8 &point)
{
	return RPC_RESULT_OK;
}
void CBattle::CheckEnd(UInt8 endState)
{
	// check if all mob dead or left before end
	if (endState == END_WIN) {
		CBatActor* actor;
		for (Actors::iterator it = m_actors.begin(); it != m_actors.end(); it++)
		{
			actor = *it;
			if (actor->GetClassID() == BatMob_CLASSID) {
				CBatMob *pmob = CastNetObject<CBatMob>(actor);
				if (!pmob->GetLeftBattle() && pmob->GetState() != CHAR_ST_DEAD) {
					TRACE_WARNDTL_4(GLOBAL_LOGGER, 
						_T("CheckEnd Warning: Mob %d still alive, hp %d, state %d, left %d"), 
						pmob->GetIndex(), pmob->GetHP(), pmob->GetState(), 
						pmob->GetLeftBattle());
				}
			}
		}
	}
}