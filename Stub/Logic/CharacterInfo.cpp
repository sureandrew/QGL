//-- Common
#include "Common.h"
//-- Self
#include "Stub/Logic/CharacterInfo.h"
//-- Library
#include "Proxy/Data/CharacterData.h"
#include "Proxy/Service/LogService.h"
#include "Proxy/Service/ObjectService.h"
#include "Proxy/Service/MapService.h"
#include "WorldService/Global.h"

REGISTER_CLASSTYPE(CCharacterInfo, CharacterInfo_CLASSID);

CCharacterInfo::CCharacterInfo()
{
	SetStatus(ST_LEAVE);
}

CCharacterInfo::~CCharacterInfo()
{
}

void CCharacterInfo::SetData(CCharacterData* pCharData)
{
	if (pCharData->IsUpdated(CCharacterData::idMask)) {
		SetId(pCharData->GetAcct_id(), pCharData->GetChar_id());
	}
	if (pCharData->IsUpdated(CCharacterData::infoMask)) {
		SetInfo(pCharData->GetNickName(), pCharData->GetSex(),
				pCharData->GetCclass(), pCharData->GetLevel(), 
				pCharData->GetExp(), pCharData->GetFaction(), pCharData->GetMoney());
	}
	// update only when not inside instance
	if (pCharData->IsUpdated(CCharacterData::mapMask)) {
		SetMap(pCharData->GetMap_id(), (pCharData->GetMap_ownerId() == 0) ? GetLine_id() : 0, 
			pCharData->GetMap_ownerId(), pCharData->GetMap_x(), pCharData->GetMap_y());
	}
	if (pCharData->IsUpdated(CCharacterData::rebornMask)) {
		SetReborn(pCharData->GetReborn_map(), pCharData->GetReborn_x(), 
			pCharData->GetReborn_y());
	}
	if (pCharData->IsUpdated(CCharacterData::styleMask)) {
		SetStyle(pCharData->GetPhoto_id(), pCharData->GetHairStyle1(), 
				pCharData->GetHairStyle2(), pCharData->GetHairColor());
	}
	if (pCharData->IsUpdated(CCharacterData::eqWeaponMask)) {
		SetEqWeapon(pCharData->GetEq_weapon(), pCharData->GetEq_weaponColor());
	}
	if (pCharData->IsUpdated(CCharacterData::eqHeadMask)) {
		SetEqHead(pCharData->GetEq_headBand(), pCharData->GetEq_headColor());
	}
	if (pCharData->IsUpdated(CCharacterData::eqClothMask)) {
		SetEqCloth(pCharData->GetEq_cloth(), pCharData->GetEq_clothColor());
	}
	if (pCharData->IsUpdated(CCharacterData::orgEqClothMask))
	{
		SetOrgEqCloth(pCharData->GetOrg_eq_cloth(), pCharData->GetOrg_eq_clothColor());
	}
	if (pCharData->IsUpdated(CCharacterData::orgHairStyleMask))
	{
		SetOrgHairStyle(pCharData->GetOrg_hairStyle1(), pCharData->GetOrg_hairColor());
	}
	if (pCharData->IsUpdated(CCharacterData::deleteTimeMask))
	{
		SetDeleteDate(pCharData->GetDelete_time());
	}

	ClearUpdateMask();
}

void CCharacterInfo::WriteData(CCharacterData* pCharData)
{
	if (IsUpdated(mapMask)) {
		pCharData->SetMap(GetMap_id(), GetMap_x(), GetMap_y(), GetMap_ownerId());
	}
}

void CCharacterInfo::PackBuffer(Buffer &buffer)
{
	CCharacterData charData;
	charData.ClearUpdateMask();
	WriteData(&charData);
	charData.WriteAttr(buffer, charData.GetUpdateMask());
}

void CCharacterInfo::Connect(const SessionID& sid)
{
	if (GetStatus() == CCharacterInfo::ST_ENTER)
		return;

	RPCContext context(sid);

	SetStatus(CCharacterInfo::ST_ENTER);
	gGlobal.IncLineCount(GetLine_id());			// player count in line increase
	TRACE_INFODTL_3(GLOBAL_LOGGER, _F("Character Connect: player increased %d, sessionId %s, char_id %d"), 
		gGlobal.GetTotalCount(), sid.GetString().c_str(), GetChar_id());
	ObjectService_Proxy::KeepCharacter(gGlobal.pObjectService->pPeer, context, 
		GetAcct_id(), GetChar_id());
	if (gGlobal.pLogService)
		LogService_Proxy::LogCharacter(gGlobal.pLogService->pPeer, context, 
			GetAcct_id(), GetChar_id(), GetLine_id(), LogService_Proxy::Character::ACT_CONNECT);
}

void CCharacterInfo::Disconnect(const SessionID& sid)
{
	if (GetStatus() == CCharacterInfo::ST_DISCONNECT)
		return;

	RPCContext context(sid);

	SetStatus(CCharacterInfo::ST_DISCONNECT);
	gGlobal.DecLineCount(GetLine_id());
	TRACE_INFODTL_4(GLOBAL_LOGGER, 
		_F("player decreased %d, sessionId %s, acct_id %d, char_id %d"), 
		gGlobal.GetTotalCount(), sid.GetString().c_str(), GetAcct_id(), GetChar_id());
	const MapInfo& mInfo = gGlobal.GetMapInfo(MapIndex(GetMap_id(), GetLine_id(), GetMap_ownerId()));
	MapService_Proxy::ClientDisconnect(mInfo.pPeer, context);
	if (gGlobal.pLogService)
		LogService_Proxy::LogCharacter(gGlobal.pLogService->pPeer, context, 
			GetAcct_id(), GetChar_id(), GetLine_id(), LogService_Proxy::Character::ACT_DISCONNECT);
}

void CCharacterInfo::Release(const SessionID& sid)
{
	const MapInfo& mInfo = gGlobal.GetMapInfo(MapIndex(GetMap_id(), GetLine_id(), GetMap_ownerId()));
	RPCContext context(sid);
	MapService_Proxy::ReleaseSession(mInfo.pPeer, context, sid, BOOLEAN_FALSE);
}

Boolean CCharacterInfo::ValidateMap()
{
	Boolean bValid = BOOLEAN_FALSE;
	MapIndex mi(GetMap_id(), 0, GetMap_ownerId());
	LPCPEER pMapPeer = gGlobal.GetAvailMapInfo(mi).pPeer;
	if (pMapPeer)
		bValid = BOOLEAN_TRUE;

	if (!bValid)
	{
		TRACE_INFODTL_4(GLOBAL_LOGGER, 
			_F("[map offline], acct_id %d, char_id %d, map %d, map_ownerId %d"), 
			GetAcct_id(), GetChar_id(), GetMap_id(), GetMap_ownerId());
		// set back reborn
		SetMap(GetReborn_map(), 0, 0, GetReborn_x(), GetReborn_y());
		mi.map_id = GetReborn_map();
		mi.owner_id = 0;
		mi.line_id = 0;
		pMapPeer = gGlobal.GetAvailMapInfo(mi).pPeer;
		if (pMapPeer == NULL)
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, 
				_FW("[reborn map offline], acct_id %d, char_id %d, map %d, map_ownerId %d"), 
				GetAcct_id(), GetChar_id(), GetMap_id(), GetMap_ownerId());
		}
		else
		{
			TRACE_INFODTL_4(GLOBAL_LOGGER, 
				_F("[back to reborn], acct_id %d, char_id %d, map %d, map_ownerId %d"), 
				GetAcct_id(), GetChar_id(), GetMap_id(), GetMap_ownerId());
		}
	}

	return bValid;
}