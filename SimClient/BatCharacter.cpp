//-- Common
#include "Common.h"
//-- Self
#include "SimClient/BatCharacter.h"

REGISTER_CLASSTYPE(CBatCharacter, BatCharacter_CLASSID);

void CBatCharacter::ShowDetail()
{
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("Show Battle Character: char_id %d, netId %d,%d, objId %x"), 
		GetChar_id(), GetNetId().srvId, GetNetId().objId, GetID());
	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("  acct_id %d, char_id %d"), 
		GetAcct_id(), GetChar_id());
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("  sex %d, cclass %d, faction %d, level %d"), 
		GetSex(), GetCclass(), GetFaction(), GetLevel());
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("  ATK %d, DEF %d, HIT %d, EVA %d, POW %d, SPD %d"), 
		GetATK(), GetDEF(), GetHIT(), GetEVA(), GetPOW(), GetSPD());
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("  HP %d, mHP %d, maxHP %d"), 
		GetHP(), GetMHP(), GetMaxHP());
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("  SP %d, mSP %d, maxSP %d"), 
		GetSP(), GetMSP(), GetMaxSP());
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("  eq_weapon %d, eq_headBand %d, eq_cloth %d"), 
		GetEq_weapon(), GetEq_headBand(), GetEq_cloth());
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("  index %d, team %d, position %d"), 
		GetIndex(), GetTeam(), GetPosition());
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("  state %d, action %d, target %d, subType %d"), 
		GetState(), GetAction(), GetTarget(), GetSubType());
}
