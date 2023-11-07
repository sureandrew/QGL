//-- Common
#include "Common.h"
#include "Common/Procedure.h"
#include "Common/common_binary.h"
#include "Common/SettingTable.h"
#include "Common/DataCode.h"
#include "Common/StateCode.h"
#include "Common/CallLuaFunc.h"
//-- Self
#include "MapService/MapCheat.h"
//-- Library
#include "Proxy/Service/LogService.h"
#include "Proxy/Service/MessageService_proxy.h"
#include "Proxy/Service/WorldService_proxy.h"
#include "Proxy/Logic/Guild.h"
#include "Proxy/Logic/GuildMember.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Map.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/Party.h"
#include "Stub/Logic/PartnerGroup.h"
#include "Stub/Logic/Partner.h"
#include "Stub/Logic/ItemBag.h"
#include "Stub/Logic/Item.h"
#include "Stub/Logic/Battle.h"
#include "Stub/Logic/ScriptSystem.h"
#include "Stub/Logic/Skill.h"
#include "Stub/Logic/SkillGroup.h"
#include "Stub/Logic/QuestLog.h"
#include "Stub/Logic/QuestType.h"
#include "Stub/Logic/QuestSystem.h"
#include "Stub/Logic/Trading.h"
#include "Stub/Logic/Pet.h"
#include "Stub/Logic/PetGroup.h"
#include "Stub/Logic/BatCharacter.h"
#include "Stub/Logic/Question.h"
#include "Stub/Logic/Trading.h"
#include "Stub/Logic/HawkGroup.h"
#include "Stub/Logic/GiftBox.h"
#include "Stub/Logic/Trading.h"
#include "Stub/Service/MapService.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "MapService/Instance.h"
#include "Resource/ServerResManager.h"
#include "Resource/ResSkill.h"
#include "MapService/Global.h"
#include "Resource/GameStringTable.h"

Boolean CMapCheat::IsGameMaster(UInt32 acct_id)
{
	if (gConfigure.bAllGMMode)
		return true;

	bool found = false;
	for (Int i = 0; i < (Int)gConfigure.gmIdList.size(); i++)
	{
		if (gConfigure.gmIdList[i] == acct_id)
		{
			found = true;
			break;
		}
	}

	return found;
}

#pragma optimize("", off)		// turn off "if-else" optimization here

Boolean CMapCheat::DoCheat(CCharacter* pChar, LPCPEER pPeer, RPCContext &context, 
							UInt32 cheat_acct_id, const String &code)
{
	if (pChar == NULL)
		return BOOLEAN_FALSE;

	UInt32 acct_id = pChar->GetAcct_id();
	UInt32 char_id = pChar->GetChar_id();
	LPCPEER charPeer = pChar->GetPeer();
	RPCContext& charContext = pChar->GetContext();

	TCHAR *delim = _T(" ");
	TCHAR *cmd = NULL; 
	TCHAR *arg1, *arg2, *arg3, *arg4, *arg5, *arg6, *arg7, *arg8, *arg9, *arg10;
	TCHAR *buf = SafeAllocate(TCHAR, code.size() + 1);

	_tcscpy(buf, code.c_str());
	cmd = _tcstok(buf, delim);
	if (cmd == NULL)
		goto CHEAT_FAIL;

	if (!pChar->CanCheat(cheat_acct_id, cmd))
	{
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("cannot cheat, CheatCode %s"), code.c_str());
		return BOOLEAN_FALSE;
	}

	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Map %d Character %d by Account %d CheatCode: %s"), 
		pChar->GetMap_id(), char_id, acct_id, code.c_str());

	LogService_Proxy::LogCheat(gGlobal.pLogService->pPeer, charContext, 0, acct_id,
		acct_id, char_id, code, 0);

	// Manual Administration Cheat Command (violent method)	
	if (!pChar->IsInMap())
	{
		if (_tcsicmp(cmd, _T("state")) == 0)
		{ 
			TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Map %d Character %d by Account %d CheatCode: %s"), 
				pChar->GetMap_id(), char_id, cheat_acct_id, code.c_str());

			arg1 = _tcstok(NULL, delim);
			if (arg1)
			{
				if (_tcsicmp(arg1, _T("check")) == 0)
				{
					CBattle * pBattle = FindNetObject<CBattle>(pChar->m_BattleNetID);
					TRACE_WARNDTL_3(GLOBAL_LOGGER, 
						_FW("!IsInMap, info about battle object, char_id %d, battlePtr %p, batCharPtr %p"),
						char_id, pBattle, pChar->m_pBatChar);

					TRACE_WARNDTL_3(GLOBAL_LOGGER, 
						_FW("command executed, state %d, char_id %d, no-battle state should be %d"), 
						pChar->GetBattleStateID(), char_id, CCharacter::ST_NOBATTLE);
				}
				else if (_tcsicmp(arg1, _T("nobattle")) == 0)
				{
					// A running battle with this character will cause it crash
					CBattle * pBattle = FindNetObject<CBattle>(pChar->m_BattleNetID);
					TRACE_WARNDTL_3(GLOBAL_LOGGER, 
						_FW("!IsInMap, info about battle object, char_id %d, battlePtr %p, batCharPtr %p"),
						char_id, pBattle, pChar->m_pBatChar);

					pChar->m_BattleNetID = RPCNetID();
					pChar->m_pBatChar = NULL;
					pChar->ChangeState(CCharacter::ST_NOBATTLE);

					TRACE_WARNDTL_3(GLOBAL_LOGGER, 
						_FW("command executed, state %d, char_id %d, no-battle state should be %d"),
						pChar->GetBattleStateID(), char_id, CCharacter::ST_NOBATTLE);
				}
			}
		}
		else
		{
			TRACE_WARNDTL_5(GLOBAL_LOGGER, 
				_F("IsInMap false, cheat failed, cheater_acct %d, target_cheat_acct_id %d, target_char_id %d, mapstate %d, battlestate %d"), 
				cheat_acct_id, acct_id, char_id, pChar->GetMapStateID(), 
				pChar->GetBattleStateID());
		}
	}
	else if (_tcsicmp(cmd, _T("hp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 hp = _ttoi(arg1);
			pChar->SetHP(hp, pChar->GetMHP(), pChar->GetMaxHP());
			if (hp > 0 && pChar->GetState() == CHAR_ST_DEAD)
				pChar->SetState(CHAR_ST_NORM);
			else if (hp == 0 && pChar->GetState() == CHAR_ST_NORM)
				pChar->SetState(CHAR_ST_DEAD);
		}
	}
	else if (_tcsicmp(cmd, _T("friend")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			if (_tcsicmp(arg1, _T("RemoveRelation")) == 0)
			{
				arg2 = _tcstok(NULL, delim);
				arg3 = _tcstok(NULL, delim);
				arg4 = _tcstok(NULL, delim);
				
				UInt32 pid = _ttoi(arg2);
				UInt32 fid = _ttoi(arg3);
				UInt32 rel = _ttoi(arg4);

				MessageService_Proxy::RemoveRelation(gGlobal.pMessageService->pPeer, 
					pChar->GetContext(), pid, fid, rel);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("maxhp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 maxhp = _ttoi(arg1);
			pChar->SetHP(pChar->GetHP(), pChar->GetMHP(), maxhp);			
		}
	}
	else if (_tcsicmp(cmd, _T("mhp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 mhp = _ttoi(arg1);
			pChar->SetHP(pChar->GetHP(), mhp, pChar->GetMaxHP());			
		}
	}
	else if (_tcsicmp(cmd, _T("sp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 sp = _ttoi(arg1);
			pChar->SetSP(sp, pChar->GetMSP(), pChar->GetMaxSP());
		}
	}
	else if (_tcsicmp(cmd, _T("faction")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt8 faction = _ttoi(arg1);
			pChar->SetFaction(faction);
		}
	}
	else if (_tcsicmp(cmd, _T("wc")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 wc = _ttoi(arg1);
			pChar->SetWC(wc, pChar->GetMaxWC(), pChar->GetWcExp(), pChar->GetNextWCExp());
		}
	}
	else if (_tcsicmp(cmd, _T("wcexp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 wcexp = _ttoi(arg1);
			pChar->SetWC(pChar->GetWC(), pChar->GetMaxWC(), wcexp, pChar->GetNextWCExp());
		}
	}
	else if (_tcsicmp(cmd, _T("vp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 vp = _ttoi(arg1);
			pChar->SetVP(vp, pChar->GetMaxVP());
		}
	}
	else if (_tcsicmp(cmd, _T("dp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 dp = _ttoi(arg1);
			pChar->SetDP(dp, pChar->GetMDP(), pChar->GetMaxDP());
		}
	}
	else if (_tcsicmp(cmd, _T("maxdp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 maxdp = _ttoi(arg1);
			pChar->SetDP(pChar->GetDP(), pChar->GetMDP(), maxdp);
		}
	}
	else if (_tcsicmp(cmd, _T("tp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 tp = _ttoi(arg1);
			pChar->SetTP(tp);
		}
	}
	else if (_tcsicmp(cmd, _T("speakBlock")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			Int32 speakBlocks = _ttoi(arg1);
			if (speakBlocks < 0 || speakBlocks > 31104000)
				speakBlocks = 31104000;
			time_t now;
			time(&now);
			pChar->SetSpeakBlock((UInt32)now + speakBlocks);
			pChar->RaiseUniqEventID(CCharacter::EVT_MSGDATA_UPDATE);
		}
	}
	else if (_tcsicmp(cmd, _T("resetSpeakBlock")) == 0)
	{ 
		pChar->SetSpeakBlock(0);
		pChar->RaiseUniqEventID(CCharacter::EVT_MSGDATA_UPDATE);
	}
	else if (_tcsicmp(cmd, _T("faction")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt8 faction = _ttoi(arg1);
			pChar->SetFaction(faction);
		}
	}
	else if (_tcsicmp(cmd, _T("state")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			if (_tcsicmp(arg1, _T("norm")) == 0)
			{
				pChar->SetState(CHAR_ST_NORM);
			}
			else if (_tcsicmp(arg1, _T("dead")) == 0)
			{
				pChar->SetState(CHAR_ST_DEAD);
			}
			else if (_tcsicmp(arg1, _T("check")) == 0)
			{
				CBattle * pBattle = FindNetObject<CBattle>(pChar->m_BattleNetID);
				TRACE_WARNDTL_3(GLOBAL_LOGGER, 
					_FW("!IsInMap, info about battle object, char_id %d, battlePtr %p, batCharPtr %p"),
					char_id, pBattle, pChar->m_pBatChar);

				TRACE_WARNDTL_3(GLOBAL_LOGGER, 
					_FW("command executed, state %d, char_id %d, no-battle state should be %d"), 
					pChar->GetBattleStateID(), char_id, CCharacter::ST_NOBATTLE);
			}
			else if (_tcsicmp(arg1, _T("nobattle")) == 0)
			{
				// A running battle with this character will cause it crash
				CBattle * pBattle = FindNetObject<CBattle>(pChar->m_BattleNetID);
				TRACE_WARNDTL_3(GLOBAL_LOGGER, 
					_FW("!IsInMap, info about battle object, char_id %d, battlePtr %p, batCharPtr %p"),
					char_id, pBattle, pChar->m_pBatChar);

				pChar->m_BattleNetID = RPCNetID();
				pChar->m_pBatChar = NULL;
				pChar->ChangeState(CCharacter::ST_NOBATTLE);

				TRACE_WARNDTL_3(GLOBAL_LOGGER, 
					_FW("command executed, state %d, char_id %d, no-battle state should be %d"),
					pChar->GetBattleStateID(), char_id, CCharacter::ST_NOBATTLE);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("goodness")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			Int32 goodness = _ttoi(arg1);
			pChar->ChangeGoodness(goodness - pChar->GetGoodness());
		}
	}
	else if (_tcsicmp(cmd, _T("addeqbuff")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		arg5 = _tcstok(NULL, delim);
		arg6 = _tcstok(NULL, delim);
		arg7 = _tcstok(NULL, delim);
		if (arg1 && arg2 && arg3 && arg4 && arg5 && arg6 && arg7)
		{
			UInt8 index = _ttoi(arg1);
			UInt16 Buff_id = _ttoi(arg2); 
			UInt32 Buff_Value1 = _ttoi(arg3); 
			UInt32 Buff_Value2  = _ttoi(arg4); 
			UInt32 Buff_Value3 = _ttoi(arg5);  
			UInt32 Buff_CoolDown = _ttoi(arg6);  
			UInt8 Pro = _ttoi(arg7); 

			pChar->AddEquipmentBuff(index, Buff_id, Buff_Value1, Buff_Value2, 
				Buff_Value3, Buff_CoolDown, Pro);
		}
	}
	else if (_tcsicmp(cmd, _T("addAvailBag")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{	
			UInt8 availBag = _ttoi(arg1);
			pChar->SetAvailBackPack(availBag);
		}
	}
	else if (_tcsicmp(cmd, _T("changeMob")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1 )
		{	
			UInt32 mobId = _ttoi(arg1);
			pChar->ChangeToMob(mobId);
		}
	}
	else if (_tcsicmp(cmd, _T("inparty1")) == 0)
	{
		gGlobal.AddCharInIncognitoPkLstOne(pChar->GetChar_id());
	}
	else if (_tcsicmp(cmd, _T("inparty2")) == 0)
	{
		gGlobal.AddCharInIncognitoPkLstTwo(pChar->GetChar_id());
	}
	else if (_tcsicmp(cmd, _T("startbattle")) == 0)
	{
		gGlobal.StartIncognitoPK();
	}
	else if (_tcsicmp(cmd, _T("stopbattle")) == 0)
	{
		gGlobal.SetCharLstBattleOver();
	}
	else if (_tcsicmp(cmd, _T("setpvpbattleID")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{ 
			pChar->SetPvPBattleScriptID(_ttoi(arg1));
		}
	}
	else if (_tcsicmp(cmd, _T("randombox")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1 && arg2)
		{
			UInt8 boxType =  _ttoi(arg1);
			UInt8 boxcount = _ttoi(arg2);
			for (int i = 0; i < boxcount; i++)
				gGlobal.RandomIncognitoBox(pChar->GetMap_id(),boxType);
		}
	}
	else if (_tcsicmp(cmd, _T("partner")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		arg5 = _tcstok(NULL, delim);
		arg6 = _tcstok(NULL, delim);
		arg7 = _tcstok(NULL, delim);
		arg8 = _tcstok(NULL, delim);
		arg9 = _tcstok(NULL, delim);
		arg10 = _tcstok(NULL, delim);

		if (arg1 && arg2)
		{
			if ( _tcsicmp(arg1, _T("add")) == 0 )
			{
				if (pChar->GetPartners())
				{
					UInt32 mobID = _ttoi(arg2);
					if (mobID > 0)
					{
						UInt8 level = arg3 ? _ttoi(arg3) : 0;
						pChar->GetPartners()->AddPartner(mobID, level);
					}
				}
			}
			else if ( _tcsicmp(arg1, _T("maxPartner")) == 0 )
			{
				UInt8 maxPartner = _ttoi(arg2);
				pChar->SetMaxOther(maxPartner, pChar->GetMaxPet());
				pChar->RaiseUpdate();
			}
			else if (arg3)
			{
				if (pChar->GetPartners())
				{
					UInt16 index = _ttoi(arg1);
					if (index > pChar->GetPartners()->GetCount())
						goto CHEAT_FAIL;

					CPartner* partner = pChar->GetPartners()->GetPartnerByIndex(index);
					if (partner)
					{
						if (_tcsicmp(arg2, _T("favour")) == 0)
						{
							UInt8 favor = _ttoi(arg3);
							if (favor > 100)
								favor = 100;
							partner->SetFavor(favor);
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("exp")) == 0)
						{
							UInt32 exp = _ttoi(arg3);
							partner->SetExp(exp);
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("apoint")) == 0)
						{
							UInt16 apoint = (UInt16)_ttoi(arg3);
							partner->SetAttrPoint(apoint);
							partner->RaiseUpdate();
						}	
						else if (_tcsicmp(arg2, _T("goodness")) == 0)
						{
							Int32 goodness = _ttoi(arg3);
							partner->SetGoodness(goodness);
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("skill")) == 0)
						{
							UInt16 skillID = _ttoi(arg3);
							if (skillID > 0)
							{
								UInt16Vector skill_ids;
								skill_ids.push_back(skillID);

								UInt8 level = 1;
								
								PROCEDURE_DO_METHOD3(CPartner, DoLearnSkill, partner, context, UInt16Vector, skill_ids, UInt8, level, bool, false)
							}
						}
						else if (_tcsicmp(arg2, _T("removeskill")) == 0)
						{
							UInt16 skillID = _ttoi(arg3);
							if (skillID > 0)
							{
								UInt16Vector skill_ids;
								skill_ids.push_back(skillID);

								PROCEDURE_DO_METHOD1(CPartner, DoRemoveSkill, partner, context, UInt16Vector, skill_ids)
							}
						}					
						else if (_tcsicmp(arg2, _T("health")) == 0)
						{
							UInt32 health = _ttoi(arg3);
							partner->SetHealth(health);
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("level")) == 0)
						{
							UInt8 level = _ttoi(arg3);
							partner->ResetLevel(level);
						}
						else if (_tcsicmp(arg2, _T("grow")) == 0)
						{
							float grow = _ttoi(arg3) / 1000.0f ;
							partner->SetGrow(grow);
							partner->SetGrowLevel(partner->CalcGrowLevel());
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("sta")) == 0)
						{
							UInt16 sta = _ttoi(arg3);
							partner->SetSTA(sta);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("spr")) == 0)
						{
							UInt16 spr = _ttoi(arg3);
							partner->SetSPR(spr);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("str")) == 0)
						{
							UInt16 str = _ttoi(arg3);
							partner->SetSTR(str);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("con")) == 0)
						{
							UInt16 con = _ttoi(arg3);
							partner->SetCON(con);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("agi")) == 0)
						{
							UInt16 agi = _ttoi(arg3);
							partner->SetAGI(agi);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("attrib")) == 0)
						{
							if (arg3 && arg4 && arg5 && arg6 && arg7)
							{
								UInt16 sta = _ttoi(arg3);
								UInt16 spr = _ttoi(arg4);
								UInt16 str = _ttoi(arg5);
								UInt16 con = _ttoi(arg6);
								UInt16 agi = _ttoi(arg7);
								partner->SetAttrib1(sta, spr, str, con, agi);
								partner->CalcAttrib();
								partner->RaiseUpdate();
							}
						}
						else if (_tcsicmp(arg2, _T("hpnative")) == 0)
						{
							UInt16 hpnative = _ttoi(arg3);
							partner->SetHP_native(hpnative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("spnative")) == 0)
						{
							UInt16 spnative = _ttoi(arg3);
							partner->SetSP_native(spnative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("atknative")) == 0)
						{
							UInt16 atknative = _ttoi(arg3);
							partner->SetATK_native(atknative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("defnative")) == 0)
						{
							UInt16 defnative = _ttoi(arg3);
							partner->SetDEF_native(defnative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("spdnative")) == 0)
						{
							UInt16 spdnative = _ttoi(arg3);
							partner->SetSPD_native(spdnative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("pownative")) == 0)
						{
							UInt16 pownative = _ttoi(arg3);
							partner->SetPOW_native(pownative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("hitnative")) == 0)
						{
							UInt16 hitnative = _ttoi(arg3);
							partner->SetHIT_native(hitnative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("evanative")) == 0)
						{
							UInt16 evanative = _ttoi(arg3);
							partner->SetEVA_native(evanative);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("native")) == 0)
						{
							if (arg3 && arg4 && arg5 && arg6 && arg7 && arg8 && arg9 && arg10)
							{
								UInt16 hpnative = _ttoi(arg3);
								UInt16 spnative = _ttoi(arg4);
								UInt16 atknative = _ttoi(arg5);
								UInt16 defnative = _ttoi(arg6);
								UInt16 spdnative = _ttoi(arg7);
								UInt16 pownative = _ttoi(arg8);
								UInt16 hitnative = _ttoi(arg9);
								UInt16 evanative = _ttoi(arg10);

								partner->SetNative(hpnative, spnative, atknative, defnative, spdnative, pownative, hitnative, evanative);
								partner->CalcAttrib();
								partner->RaiseUpdate();
							}
						}
						else if (_tcsicmp(arg2, _T("cultivation")) == 0)
						{
							UInt16 cultivation = _ttoi(arg3);
							partner->SetCultivation(cultivation);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if (_tcsicmp(arg2, _T("practice")) == 0)
						{
							UInt16 practice = _ttoi(arg3);
							partner->SetPractice(practice);
							partner->CalcAttrib();
							partner->RaiseUpdate();
						}
						else if  (_tcsicmp(arg2, _T("hp")) == 0)
						{
							UInt16 hp =  _ttoi(arg3);
							partner->SetHP(hp);
							partner->RaiseUpdate();
						}
						else if  (_tcsicmp(arg2, _T("sp")) == 0)
						{
							UInt16 sp =  _ttoi(arg3);
							partner->SetSP(sp);
							partner->RaiseUpdate();
						}
					}
				}
				else
				{
					goto CHEAT_FAIL;
				}
			}
		}
	}
	else if (pChar->GetBag() && _tcsicmp(cmd, _T("item")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (_tcsicmp(arg1, _T("add")) == 0 || _tcsicmp(arg1, _T("remove")) == 0)
		{
			UInt32Vector item_ids;
			UInt8Vector bagIndices;
			UInt8 bagIndex = 0;
			bool bCreate = (_tcsicmp(arg1, _T("add")) == 0);
			arg2 = _tcstok(NULL, delim);

			Boolean added = BOOLEAN_FALSE;
			while (arg2)
			{
				TCHAR *p = _tcschr(arg2, _T('x'));
				UInt16 count = 1;
				if (p)
				{
					// set count by 'x'
					*p = _T('\0');
					count = _ttoi(p + 1);
					count = MAX(count, 1);
				}
				UInt32 item_id = _ttoi(arg2);
				if (bCreate)
					pChar->GetBag()->AddItem(item_id, count);
				else
					pChar->GetBag()->RemoveItem(item_id, count);
				arg2 = _tcstok(NULL, delim);
			}
			if (added)
			{
				pChar->GetBag()->RaiseUpdate();
			}
		}
		else if (_tcsicmp(arg1, _T("value")) == 0)
		{
			arg2 = _tcstok(NULL, delim);
			arg3 = _tcstok(NULL, delim);
			arg4 = _tcstok(NULL, delim);
			arg5 = _tcstok(NULL, delim);

			if (arg2 && arg3 && arg4 && arg5)
			{
				UInt16 item_id = _ttoi(arg2);
				CItem * pItem = pChar->GetBag()->FindItemById(item_id, 0, 0);
				if (pItem)
				{
					UInt16 value1 = _ttoi(arg3);
					UInt16 value2 = _ttoi(arg4);
					UInt16 value3 = _ttoi(arg5);
					pItem->SetValue(value1, value2, value3);
					pChar->GetBag()->RaiseUpdate();
				}
			}
		}
		else if (_tcsicmp(arg1, _T("quality")) == 0)
		{
			arg2 = _tcstok(NULL, delim);
			arg3 = _tcstok(NULL, delim);
			if (arg2 && arg3)
			{
				UInt16 item_id = _ttoi(arg2);
				CItem * pItem = pChar->GetBag()->FindItemById(item_id, 0, 0);
				if (pItem)
				{
					UInt16 quality = _ttoi(arg3);
					
					pItem->SetQuality(quality);
					pChar->GetBag()->RaiseUpdate();
				}
			}
		}
		else if (_tcsicmp(arg1, _T("ExpireDate")) == 0)
		{
			arg2 = _tcstok(NULL, delim);
			arg3 = _tcstok(NULL, delim);
			if (arg2 && arg3)
			{	
				UInt16 item_id = _ttoi(arg2);
				CItem * pItem = pChar->GetBag()->FindItemById(item_id, 0, 0);
				if (pItem)
				{
					String time = arg3;
					if (time.length() >= 12)
					{
						String temptext = time.substr(0, 4);
						UInt32 year = _ttoi(temptext.c_str());
						temptext = time.substr(4, 2);
						UInt32 month = _ttoi(temptext.c_str());
						temptext = time.substr(6, 2);
						UInt32 day = _ttoi(temptext.c_str());
						temptext = time.substr(8, 2);
						UInt32 hour = _ttoi(temptext.c_str());
						temptext = time.substr(10, 2);
						UInt32 min = _ttoi(temptext.c_str());

						struct tm	dateTime;
						ZeroMemory(&dateTime, sizeof(struct tm));
						dateTime.tm_year = year - 1900;
						dateTime.tm_mon = month - 1;
						dateTime.tm_mday = day;
						dateTime.tm_hour = hour;
						dateTime.tm_min = min;

						pItem->SetExpireDate((UInt32)mktime(&dateTime));
						pChar->GetBag()->RaiseUpdate();
						//DateTime startTime = new DateTime(1970,1,1,0,0,0,0);
					}
				}
			}
		}
		else if (_tcsicmp(arg1, _T("use")) == 0)
		{
			arg2 = _tcstok(NULL, delim);
			arg3 = _tcstok(NULL, delim);
			UInt32 partnerID = 0;
			if (arg2)
			{
				UInt8 usetype = _ttoi(arg2);
				if(arg3)
					partnerID = _ttoi(arg3);
				else
					partnerID = 0;
				pChar->GetBag()->UseFastFoodItem(charPeer, charContext,usetype,partnerID);
			}
		}
		else if (_tcsicmp(arg1, _T("jpack")) == 0)
		{
			arg2 = _tcstok(NULL, delim);
			if (arg2)
			{
				UInt8 bagtype = _ttoi(arg2);
				pChar->GetBag()->Jpack(charPeer, charContext,bagtype);
			}
		}
		else if (_tcsicmp(arg1, _T("jpackck")) == 0)
		{
			arg2 = _tcstok(NULL, delim);
			if (arg2)
			{
				UInt8 page = _ttoi(arg2);
				pChar->GetBag()->CleanWarehouse(charPeer, charContext,page);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("jump")) == 0)
	{
		// jump map
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		arg5 = _tcstok(NULL, delim);
		UInt32 map_id = (arg1 ? _ttoi(arg1) : 0);
		UInt8 line_id = (arg2 ? _ttoi(arg2) : 0);
		UInt16 map_x = (arg3 ? _ttoi(arg3) : 0);
		UInt16 map_y = (arg4 ? _ttoi(arg4) : 0);
		UInt32 owner_id = (arg5 ? _ttoi(arg5) : 0);
		if (arg1 && arg2 && map_id != 0)
		{
			// all argument provided
			SafeDeallocate(buf);
			pChar->JumpMap(map_id, map_x, map_y, line_id, owner_id,0,BOOLEAN_FALSE);
		}
	}
	else if (_tcsicmp(cmd, _T("yuanbao")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 yuanBao = _ttoi(arg1);
			Int32 change = yuanBao - pChar->GetYuanBao();
			
			PROCEDURE_DO_METHOD1(CCharacter, ChangeYuanBao, pChar, charContext, Int32, change)

			if (change < 0)
				LogService_Proxy::LogYuanBao(gGlobal.pLogService->pPeer, charContext, 
					acct_id, char_id, 
					LogService_Proxy::YuanBao::USEIN_CHEAT_SUB, -change);
			else
				LogService_Proxy::LogYuanBao(gGlobal.pLogService->pPeer, charContext, 
					acct_id, char_id, 
					LogService_Proxy::YuanBao::USEIN_CHEAT_ADD, change);
		}

	}
	else if (_tcsicmp(cmd, _T("money")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 gmoney = _ttoi(arg1);
			pChar->SetMoney(gmoney);
		}

	}
	else if (_tcsicmp(cmd, _T("smoney")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 smoney = _ttoi(arg1);
			pChar->SetSMoney(smoney);
		}

	}
	else if (_tcsicmp(cmd, _T("bmoney")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 bmoney = _ttoi(arg1);
			pChar->SetBankMoney(bmoney);
		}

	}
	else if (_tcsicmp(cmd, _T("br")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			pChar->m_stepMod = _ttoi(arg1);
			TRACE_INFODTL_2(GLOBAL_LOGGER, _F("Cheat: changed character %d battle rate (step mod) to %d"),
				char_id, pChar->m_stepMod);
		}
	}
	else if (_tcsicmp(cmd, _T("level")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 level = _ttoi(arg1);

			pChar->SetLevel(level);
			pChar->CalcAttrib();
			pChar->CalcNextLevel();
			pChar->RaiseUniqEventID(CCharacter::EVT_MSGDATA_UPDATE);
		}
	}
	else if (_tcsicmp(cmd, _T("exp")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 exp = _ttoi(arg1);
			pChar->SetExp(exp);
		}
	}
	else if (_tcsicmp(cmd, _T("honor")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 honor = _ttoi(arg1);
			pChar->SetHonor(honor);
		}
	}
	else if (_tcsicmp(cmd, _T("pk")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			Boolean enable = _ttoi(arg1) > 0;
			pChar->SetBattleInfo( enable);
			//SetPKInfo((Boolean)enable);
		}
	}
	else if (_tcsicmp(cmd, _T("skill")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			if (_tcsicmp(arg1, _T("open")) == 0)
			{
				arg2 = _tcstok(NULL, delim);

				if (arg2)
				{
					UInt16 shop_id = _ttoi(arg2);
					pChar->OpenInterface("skillshop", shop_id); 
				}
			}
			else if (_tcsicmp(arg1, _T("learn")) == 0)
			{
				arg2 = _tcstok(NULL, delim);

				if (arg2)
				{
					UInt16 skill_id = _ttoi(arg2);
					arg3 = _tcstok(NULL, delim);
					UInt8 level = arg3 ? _ttoi(arg3) : 1;
					if (pChar->HasSkill(skill_id))
						goto CHEAT_FAIL;

					UInt16Vector IdLst;
					IdLst.push_back(skill_id);					
					PROCEDURE_DO_METHOD2(CCharacter, DoLearnSkill, pChar, context,
						UInt16Vector, IdLst, UInt8, level)
				}
			}
			else if (_tcsicmp(arg1, _T("upgrade")) == 0)
			{
				arg2 = _tcstok(NULL, delim);

				if (arg2)
				{
					Int SkillId = _ttoi(arg2);
					pChar->UpGradeSkill(charPeer, charContext, SkillId);
				}
			}
			else if (_tcsicmp(arg1, _T("level")) == 0)
			{
				arg2 = _tcstok(NULL, delim);
				arg3 = _tcstok(NULL, delim);

				if (arg2 && arg3)
				{
					Int SkillId = _ttoi(arg2);
					Int level = _ttoi(arg3);

					CSkill* pSkill = pChar->GetSkill(SkillId);
					if (pSkill)
						pChar->ChangeSkillLv(SkillId, level - pSkill->GetSkill_level());
				}
			}
			else if(_tcsicmp(arg1, _T("remove")) == 0)
			{
				arg2 = _tcstok(NULL, delim);
				if (arg2)
				{
					Int SkillId = _ttoi(arg2);
					pChar->RemoveSkill(charPeer, charContext, SkillId);
				}
			}
			else if (_tcsicmp(arg1, _T("reload")) == 0)
			{
			}
		}
	}
	else if (_tcsicmp(cmd, _T("buff")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			if (_tcsicmp(arg1, _T("add")) == 0)
			{
				arg2 = _tcstok(NULL, delim);
				arg3 = _tcstok(NULL, delim);
				arg4 = _tcstok(NULL, delim);
				arg5 = _tcstok(NULL, delim);
				arg6 = _tcstok(NULL, delim);
				arg7 = _tcstok(NULL, delim);
				if (arg2 && arg3 && arg4 &&
					arg5 && arg6 && arg7)
				{
					pChar->AddBuff(_ttoi(arg2), _ttoi(arg3), _ttoi(arg4), _ttoi(arg5), 
						_ttoi(arg6), _ttoi(arg7));
				}
			}
			else if (_tcsicmp(arg1, _T("remove")) == 0)
			{
				arg2 = _tcstok(NULL, delim);
				if (arg2)
					pChar->RemoveBuff(_ttoi(arg2));
			}
		}
	}
	else if (_tcsicmp(cmd, _T("attrib")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		arg5 = _tcstok(NULL, delim);
		if (arg1 && arg2 && arg3 && arg4 && arg5)
		{
			UInt16 sta = _ttoi(arg1);
			UInt16 spr = _ttoi(arg2);
			UInt16 str = _ttoi(arg3);
			UInt16 con = _ttoi(arg4);
			UInt16 agi = _ttoi(arg5);

			pChar->SetAttrib1(sta, spr, str, con, agi);
			pChar->CalcAttrib();
		}
	}
	else if (_tcsicmp(cmd, _T("apoint")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt16 attribPoint = _ttoi(arg1);
			pChar->SetAttrPoint(attribPoint);
		}
	}

	else if (_tcsicmp(cmd, _T("quest")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		if (pChar->GetQuests() && arg1 && arg2 && arg3)
		{
			UInt16 id = _ttoi(arg3);
			if (_tcsicmp(arg1, _T("log")) == 0)
			{
				if (_tcsicmp(arg2, _T("add")) == 0) {
					arg4 = _tcstok(NULL, delim);
					UInt16 tid = arg4 ? _ttoi(arg4) : 0;
					String qname;
					String tname;
					qname.Format(_T("Q %d"), id);
					tname.Format(_T("T %d"), tid);
					pChar->GetQuests()->AddQuest(id, qname, tid, tname);
				} else if (_tcsicmp(arg2, _T("remove")) == 0) {
					pChar->GetQuests()->RemoveQuest(id);
				}
			}
			else if (_tcsicmp(arg1, _T("type")) == 0)
			{
				if( _tcsicmp(arg2, _T("add")) == 0 )
					pChar->GetQuests()->AddType(id);
				else if (_tcsicmp(arg2, _T("remove")) == 0)
					pChar->GetQuests()->RemoveType(id);
				else if (_tcsicmp(arg2, _T("clear")) == 0)
					pChar->GetQuests()->ClearTotalRound(id);
				else if (_tcsicmp(arg2, _T("round")) == 0 || 
						_tcsicmp(arg2, _T("totalround")) == 0)
				{
					arg4 = _tcstok(NULL, delim);
					UInt16 round = arg4 ? _ttoi(arg4) : 0;
					CQuestType* pType = pChar->GetQuests()->GetType(id);
					if (pType) {
						if (_tcsicmp(arg2, _T("round")) == 0)
							pType->SetRound(round);
						else
							pType->SetTotalRound(round);
						pChar->GetQuests()->RaiseUpdate();
					}
				}
			}
			else if( _tcsicmp(arg1, _T("flag")) == 0)
			{
				arg4 = _tcstok(NULL, delim);
				if ((_tcsicmp(arg2, _T("add")) == 0 || _tcsicmp(arg2, _T("set")) == 0) && 
					arg4)
				{
					UInt32 flag	= _ttoi(arg4);
					pChar->GetQuests()->SetFlag(id, flag);
				}
				else if( _tcsicmp(arg2, _T("remove")) == 0)
					pChar->GetQuests()->RemoveFlag(id);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("speed")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 speed = _ttoi(arg1);
			if( speed != 0 )
				pChar->SetSpeed(speed);
		}
	}
	else if (_tcsicmp(cmd, _T("char")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 char_id = _ttoi(arg1);
			if (char_id != 0)
			{
				String restPart;
				restPart.Format(_T("%s"), arg1 + _tcslen(arg1) + 1);
				// World Cheat with exe_id=0, target acct id=0 because inside game, 
				//   acct_id of target is often unknown, especially when it's offline
				WorldService_Proxy::WorldCheat(gGlobal.pWorldService->pPeer, context, 0, acct_id, 0, char_id, restPart);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("addSpeakCount")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt8 count = _ttoi(arg1);
			MessageService_Proxy::AddSpeakCount(gGlobal.pMessageService->pPeer, charContext,
				char_id, count);
		}
	}
	else if (_tcsicmp(cmd, _T("viewRanking")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt8 rankType = _ttoi(arg1);
			WorldService_Proxy::GetCharacterRank(gGlobal.pWorldService->pPeer, charContext, rankType);
		}
	}
	else if (_tcsicmp(cmd, _T("viewGuildRanking")) == 0)
	{
		WorldService_Proxy::GetGuildRank(gGlobal.pWorldService->pPeer, charContext);
	}
	else if (_tcsicmp(cmd, _T("guild")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1 && _tcsicmp(arg1, _T("create")) == 0)
		{
			pChar->OpenCreateGuildMenu();
			/*arg2 = _tcstok(NULL, delim);
			if (arg2)
				MessageService_Proxy::CreateGuild(gGlobal.pMessageService->pPeer, charContext, char_id, arg2, GetNickName(), GetNickName()); 
				*/
		}
		else if (arg1 && _tcsicmp(arg1, _T("apply")) == 0)
		{
			MessageService_Proxy::OpenGuildApply(gGlobal.pMessageService->pPeer, charContext, char_id);
		}
	}
	else if (_tcsicmp(cmd, _T("guildwar")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{ 
			if( _tcsicmp(arg1, _T("Bid")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					Int32Vector lists;
					lists.push_back(change);
					MessageService_Proxy::ApplyChange(gGlobal.pMessageService->pPeer,
						RPCContext(), pChar->m_ownGuild->GetGuild_uid(), 
						CGuild::guildPvPBidMask, lists, 0);	
				}
			}
			else if( _tcsicmp(arg1, _T("firstGen")) == 0)
			{
				MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, 
					RPCContext(), GUILD_WAR_STATE_BID_END);
			}
			else if( _tcsicmp(arg1, _T("startPrepare")) == 0)
			{
				gGlobal.g_GuildWarState = GUILD_WAR_STATE_WAR_PREPARE;
			}
			else if( _tcsicmp(arg1, _T("startWar")) == 0)
			{
				gGlobal.g_GuildWarState = GUILD_WAR_STATE_IN_WAR;
				gGlobal.StartGuildWar();
			}
			else if( _tcsicmp(arg1, _T("endWarFirst")) == 0)
			{
				gGlobal.g_GuildWarState = GUILD_WAR_STATE_WAR_FINISH_CONTINUE;
				MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, RPCContext(), GUILD_WAR_STATE_WAR_FINISH_CONTINUE);
				gGlobal.EndGuildWar();
			}
			else if( _tcsicmp(arg1, _T("endWarSecond")) == 0)
			{
				gGlobal.g_GuildWarState = GUILD_WAR_STATE_WAR_FINISH_END;
				MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, RPCContext(), GUILD_WAR_STATE_WAR_FINISH_END);
				gGlobal.EndGuildWar();
			}
			else if (_tcsicmp(arg1, _T("sendRumor")) == 0)
			{
				MessageService_Proxy::GenerateGuildGroup(gGlobal.pMessageService->pPeer, RPCContext(), GUILD_WAR_STATE_SEND_RUMOR_GROUP);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("manor")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			if( _tcsicmp(arg1, _T("prosperChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					arg2 = _tcstok(NULL, delim);
					UInt8 source = arg2? _ttoi(arg2) : 2; 
					if (pGuild)
                        pGuild->ChangeProsper(change, source);
				}
			}
			else if( _tcsicmp(arg1, _T("fund")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					arg2 = _tcstok(NULL, delim);
					UInt8 source = arg2? _ttoi(arg2) : 5; 
					if (pGuild)
                        pGuild->ChangeFund(change, source);
				}
			}
			else if( _tcsicmp(arg1, _T("scale")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 scale = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->SetNewManorScale(scale);
				}
			}
			else if( _tcsicmp(arg1, _T("materialChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeMaterial(change);
				}
			}
			else if( _tcsicmp(arg1, _T("roomChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeRoom(change);
				}
			}
			else if( _tcsicmp(arg1, _T("aura")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 aura = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->SetAuraValue(aura);
				}
			}
			else if( _tcsicmp(arg1, _T("trainCharExChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeTrainCharEx(change);
				}
			}
			else if( _tcsicmp(arg1, _T("trainCharInChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeTrainCharIn(change);
				}
			}
			else if( _tcsicmp(arg1, _T("trainPartnerExChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeTrainPartnerEx(change);
				}
			}
			else if( _tcsicmp(arg1, _T("trainPartnerInChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeTrainPartnerIn(change);
				}
			}
			else if( _tcsicmp(arg1, _T("drugChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeDrug(change);
				}
			}
			else if( _tcsicmp(arg1, _T("cookChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeCook(change);
				}
			}
			else if( _tcsicmp(arg1, _T("wineChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeWine(change);
				}
			}
			else if( _tcsicmp(arg1, _T("hpChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeManorHP(change);
				}
			}
			else if( _tcsicmp(arg1, _T("spChange")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					CGuild *pGuild = pChar->m_ownGuild;
					if (pGuild)
                        pGuild->ChangeManorSP(change);
				}
			}
			else if( _tcsicmp(arg1, _T("sit")) == 0)
			{
				pChar->Sit();
			}
			else if( _tcsicmp(arg1, _T("sendGuildmsg")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				if (arg1)
				{
					Int32 guildUID = _ttoi(arg1);
                    StringA Msg = _tcstok(NULL, _T(""));
					pChar->ShowGuildMsg(Msg.c_str(), guildUID); 
				}
			}
			else if( _tcsicmp(arg1, _T("sendPersonMsg")) == 0)
			{
                StringA Msg = _tcstok(NULL, _T(""));
				pChar->ShowAwardMsg(Msg.c_str()); 

			}
			else if( _tcsicmp(arg1, _T("donate")) == 0)
			{
				arg1 = _tcstok(NULL, delim);
				arg2 = _tcstok(NULL, delim); 
				if (arg1 && arg2 && pChar->m_ownGuild)
				{
					Int32 change = _ttoi(arg1);
					UInt8 source = _ttoi(arg2);
					pChar->ChangeGDonate(change, source);
				}
			}
		}
	}
	else if (_tcsicmp(cmd, _T("pet")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);		if (pChar->GetPets() && arg1 && arg2)
		{
			if (_tcsicmp(arg1, _T("add")) == 0)
			{
				UInt32 petid = _ttoi(arg2);
				if (petid > 0)
					pChar->GetPets()->AddPet(petid);
			}
			else if (_tcsicmp(arg1, _T("remove")) == 0)
			{
				UInt32 petid = _ttoi(arg2);
			}
			else if (arg3)
			{				
				if (pChar->GetPets())	
				{					
					UInt16 index = _ttoi(arg1);	
					if (index > pChar->GetPets()->GetCount())
						goto CHEAT_FAIL;					
					CPet* pet = pChar->GetPets()->GetPetByIndex(index);	
					if (pet)					
					{						
						if (_tcsicmp(arg2, _T("favor")) == 0)		
						{							
							UInt8 favor = _ttoi(arg3);			
							if (favor > 100)						
								favor = 100;						
							pet->SetHappiness(favor);				
							pet->RaiseUpdate();					
						}						
						else if (_tcsicmp(arg2, _T("vp")) == 0)				
						{		
							UInt8 vp = _ttoi(arg3);					
							pet->SetVP(vp);					
							pet->RaiseUpdate();						
						}				
					}			
				}			
			}		
		}
	}
	else if (_tcsicmp(cmd, _T("mixshop")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (pChar->GetScript() && arg1)
			pChar->GetScript()->CB_OpenInterface(charPeer, charContext, "mixshop", _ttoi(arg1));
	}
	else if (_tcsicmp(cmd, _T("hawkershop")) == 0)
	{
		pChar->OpenHawkerOwnerMenu();
	}
	else if (_tcsicmp(cmd, _T("warehouse")) == 0)
	{
		pChar->OpenWareHouse();
	}
	else if (_tcsicmp(cmd, _T("giftbox")) == 0)
	{
		pChar->OpenGiftBox();
	}
	else if (_tcsicmp(cmd, _T("antihack")) == 0)
	{
		pChar->AntiHack();
	}	
	else if (_tcsicmp(cmd, _T("npc")) == 0)
	{ 
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		if (arg1 && arg2) {
			UInt32 npc_id = _ttoi(arg2);
			if (pChar->GetMap() && _tcsicmp(arg1, _T("add")) == 0)
			{
				UInt32 lifeTime = arg3 ? _ttoi(arg3) : 60;
				pChar->GetMap()->CreateNpc(npc_id, pChar->GetCurPosX(), pChar->GetCurPosY(), 0, lifeTime);
			}
			if (pChar->GetQuests() && _tcsicmp(arg1, _T("find")) == 0)
			{
				UInt16 map_id = arg3 ? _ttoi(arg3) : pChar->GetMap_id();
				pChar->GetQuests()->CheckNpcTalk(npc_id, map_id);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("hawkerTime")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{
			UInt32 hawkerTime = _ttoi(arg1);
			pChar->SetHawkerTime(hawkerTime);
			pChar->RaiseUpdate();
		}

	}
	else if (_tcsicmp(cmd, _T("battle")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1 && arg2) {
			if (pChar->GetQuests() && _tcsicmp(arg1, _T("win")) == 0)
			{
				UInt32Vector mobList;
				UInt32 npc_id = _ttoi(arg2);
				arg3 = _tcstok(NULL, delim);

				while (arg3)
				{
					TCHAR *p = _tcschr(arg3, _T('x'));
					int count = 1;
					if (p)
					{
						// set count by 'x'
						*p = _T('\0');
						count = _ttoi(p + 1);
						count = MAX(count, 1);
					}
					UInt32 mob_id = _ttoi(arg3);
					for (int i = 0; i < count; i++)
						mobList.push_back(mob_id);
					arg3 = _tcstok(NULL, delim);
				}
				pChar->GetQuests()->CheckAfterBattle(END_WIN, npc_id, mobList);
			}
		} else {
			UInt32 npc_id = arg1 ? _ttoi(arg1) : pChar->CheckBattle();
			pChar->EncounterBattle(npc_id);
		}
	}
	else if (_tcsicmp(cmd, _T("title")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		
		if (arg1 && arg2)
		{
			if (_tcsicmp(arg1, _T("add")) == 0)
			{
				UInt32 titleID = _ttoi(arg2);
				if (titleID > 0)
					pChar->AddTitle(titleID);

			}
			else if (_tcsicmp(arg1, _T("remove")) == 0)
			{
				//UInt32 petid = _ttoi(arg2);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("reload")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) { 
			if (_tcsicmp(arg1, _T("script")) == 0)
				gGlobal.LoadAllScript();
		}
	}
	else if (_tcsicmp(cmd, _T("stop")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) { 
			if (_tcsicmp(arg1, _T("script")) == 0)
				if (pChar->GetScript()->IsRunning())
					pChar->GetScript()->StopScript();
				else
					pChar->GetScript()->TalkEnd(BOOLEAN_FALSE);
		}
	}
	else if (_tcsicmp(cmd, _T("kick")) == 0)
	{
		if (pChar->GetAction() == CCharacter::ST_HAWK && pChar->m_pHawkGroup)
			pChar->m_pHawkGroup->StopHawk();
		pChar->ForceLogout(_T("MSG_CHEAT_KICK"));
	}
	else if (_tcsicmp(cmd, _T("superkick")) == 0)
	{
		// remove procedures of that character, prevent procedure dead lock
		ProcedureManager::GetInstance().RemoveBySession(pChar->GetSessionId());
		if (pChar->GetAction() == CCharacter::ST_HAWK && pChar->m_pHawkGroup)
			pChar->m_pHawkGroup->StopHawk();
		pChar->ForceLogout(_T("MSG_CHEAT_KICK"));
		pChar->ScheduleLogout(0);
	}
	else if (_tcsicmp(cmd, _T("lua")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			StringA LuaFunc;
			StrToUTF8(arg1,LuaFunc);
			if (HasLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str()))
				CallLuaFunc(gGlobal.g_pLuaState, LuaFunc.c_str(),String());
		}
	}
	else if (_tcsicmp(cmd, _T("changeOrgColor")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			UInt16 orgColor = _ttoi(arg1);
			pChar->SetOrg_eq_clothColor(orgColor);
		}
	}
	else if (_tcsicmp(cmd, _T("addWareHouseItemBag")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			UInt8 count = _ttoi(arg1);
			pChar->SetAvailItemWareHouse(count);
		}
	}
	else if (_tcsicmp(cmd, _T("addWareHousePartner")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			UInt8 count = _ttoi(arg1);
			pChar->SetAvailPartnerWareHouse(count);
		}
	}
	else if (_tcsicmp(cmd, _T("addWareHousePet")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			UInt8 count = _ttoi(arg1);
			pChar->SetAvailPetWareHouse(count);
		}
	}
	else if (_tcsicmp(cmd, _T("changeEqColor")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			UInt8 color = _ttoi(arg1);
			CItem* item = pChar->GetBag()->GetItem(128);
			if (item)
			{
				item->SetEqColor(color);
				pChar->SetEqCloth(item->GetItem_id(), item->GetEqColor());
			}
		}
	}
	else if (_tcsicmp(cmd, _T("instance")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		if (arg1 && arg2 && arg3) 
		{ 
			UInt16 instId = _ttoi(arg2);
			UInt32 ownerId = _ttoi(arg3);
			if (_tcsicmp(arg1, _T("add")) == 0 && arg4)
			{
				StringA luafile(arg4);
				CInstance::DirectCreateInstance(pChar, luafile, instId, ownerId);
			}
			else if (_tcsicmp(arg1, _T("remove")) == 0)
			{
				CInstance::DestroyInstance(instId, ownerId);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("robot")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		if (arg1)
		{ 
			if (_tcsicmp(arg1, _T("on")) == 0)
			{
				UInt32 interval = (arg2 ? (_ttoi(arg2) * 1000) : 
						gConfigure.robotInterval);
				pChar->SetRobot(BOOLEAN_TRUE, interval);
			}
			else if (_tcsicmp(arg1, _T("off")) == 0)
			{
				pChar->SetRobot(BOOLEAN_FALSE, 0);
			}
		}
	}
	else if (_tcsicmp(cmd, _T("autocount")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1)
		{ 
			pChar->SetInitAutoCount(_ttoi(arg1));
		}
	}
	else if (_tcsicmp(cmd, _T("sellYuanBao")) == 0)
	{
		pChar->OpenSellYuanBao();
	}
	else
	{
		if (!DoGlobalCheat(pPeer, context, acct_id, code))
			goto CHEAT_FAIL;
	}

	SafeDeallocate(buf);
	return BOOLEAN_TRUE;

CHEAT_FAIL:
	SafeDeallocate(buf);
	return BOOLEAN_FALSE;
}

Boolean CMapCheat::DoGlobalCheat(LPCPEER pPeer, RPCContext &context, 
								 UInt32 acct_id, const String &code)
{
	if (!CMapCheat::IsGameMaster(acct_id))
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, 
			_FW("[account is not GM], acct_id %d"), acct_id);
		return BOOLEAN_FALSE;
	}

	//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("GlobalCheatCode: acct_id %d, %s"), acct_id, code.c_str());

	TCHAR *delim = _T(" ");
	TCHAR *cmd = NULL; 
	TCHAR *arg1, *arg2, *arg3, *arg4, *arg5, *arg6;
	TCHAR *buf = SafeAllocate(TCHAR, code.size() + 1);

	_tcscpy(buf, code.c_str());
	cmd = _tcstok(buf, delim);
	if (cmd == NULL)
		goto G_CHEAT_FAIL;

	if (_tcsicmp(cmd, _T("GM")) == 0 && gGlobal.pMessageService->pPeer)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		//arg3 = _tcstok(NULL, delim);
		if (arg1 && arg2)// && arg3)
		{
			if (_tcsicmp(arg1, _T("say")) == 0)
			{
				UInt16 type = _ttoi(arg2);
				if (type == 0)
				{
					arg3 = _tcstok(NULL, delim);
					if (arg3)
					{
						UInt16 count = _ttoi(arg3);

						//arg4 = _tcstok(NULL, delim);
						String Msg = _tcstok(NULL, _T(""));
						MessageService_Proxy::SendHorseLightMessage(gGlobal.pMessageService->pPeer, 
							context, Msg, 0, count);
					}
				}
				else if (type == 1)
				{
					String Msg = _tcstok(NULL, _T(""));
					
					MessageService_Proxy::SendSystemMessage(gGlobal.pMessageService->pPeer, 
						context, Msg, 0);
				}
				else if (type == 2)
				{
					arg3 = _tcstok(NULL, delim);
					if (arg3)
					{
						String Msg = _tcstok(NULL, _T(""));
						UInt16 type = _ttoi(arg3);
						MessageService_Proxy::SendRumorMessage(gGlobal.pMessageService->pPeer, 
							context, Msg, 0, (BYTE) type);
					}
				}
			}
		}
	}
	else if (_tcsicmp(cmd, _T("addMsgSchedule")) == 0 && gGlobal.pMessageService->pPeer)
	{
		arg1 = _tcstok(NULL, delim);
		arg2 = _tcstok(NULL, delim);
		arg3 = _tcstok(NULL, delim);
		arg4 = _tcstok(NULL, delim);
		arg5 = _tcstok(NULL, delim);
		arg6 = _tcstok(NULL, delim);
		String msg = _tcstok(NULL, _T(""));

		if (arg1 && arg2 && arg3 && arg4 && arg5 && arg6 && !msg.IsEmpty())
		{
			UInt16 id = _ttoi(arg1);
			UInt32 startTime = _ttoi(arg2);
			UInt16 periodCount = _ttoi(arg3);
			UInt16 count = _ttoi(arg4);
			UInt32 period = _ttoi(arg5); 
			UInt8 msgType = _ttoi(arg6); 

			MessageService_Proxy::AddMsgSchedule(gGlobal.pMessageService->pPeer, 
				context, id, startTime, periodCount, count, period, msgType, msg, 0);
		}
	}
	else if (_tcsicmp(cmd, _T("removeMsgSchedule")) == 0 && gGlobal.pMessageService->pPeer)
	{
		arg1 = _tcstok(NULL, delim);

		if (arg1)
		{
			UInt16 id = _ttoi(arg1);
			MessageService_Proxy::RemoveMsgSchedule(gGlobal.pMessageService->pPeer, 
				context, id);
		}
	}
	else if (_tcsicmp(cmd, _T("se")) == 0)
	{
		arg1 = _tcstok(NULL, delim);
		if (arg1) 
		{ 
			UInt32 eventId = _ttoi(arg1);
			WorldService_Proxy::StartEvent(gGlobal.pWorldService->pPeer, RPCContext(), eventId);
		}
	}
	else
	{
		goto G_CHEAT_FAIL;
	}

	SafeDeallocate(buf);
	return BOOLEAN_TRUE;

G_CHEAT_FAIL:
	SafeDeallocate(buf);
	return BOOLEAN_FALSE;
}

#pragma optimize("", on)			// resume optimization here
