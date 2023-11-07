//-- Common
#include "Common.h"
#include "QGL/QGL.h"
#include "Common/PathFinder.h"
//-- Self
#include "Proxy/Logic/Map.h"
//-- Library
#include "Proxy/Logic/Character.h"
#include "Proxy/Logic/MapNPC.h"
#include "QGL/Global.h"
#include "QGL/CharacterControl.h"
#include "QGL/QGLApplication.h"
#include "Common/PathFinder.h"
#include "QGL/Math.h"
#include "qgl/MiddleMap.h"
//#define DEBUG_CHECK_VIEW		1

REGISTER_CLASSTYPE(CMap, Map_CLASSID);

CMap::CMap()
{
	m_pMapView = NULL;
}

CMap::~CMap()
{
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		DeleteNetObject(it->second);
	}
	m_objects.clear();
}

UInt16 CMap::GetWidth()
{
	if (gGlobal.g_PathFinder)
		return gGlobal.g_PathFinder->GetWidth();
	else
		return 0;
}

UInt16 CMap::GetHeight()
{
	if (gGlobal.g_PathFinder)
		return gGlobal.g_PathFinder->GetHeight();
	else
		return 0;
}

UInt16 CMap::GetBlock(UInt16 x, UInt16 y)
{
	return (gGlobal.g_PathFinder ? gGlobal.g_PathFinder->GetBlockType(x, y) : 0);
}

bool CMap::IsBlocked(UInt16 x, UInt16 y)
{
	return (gGlobal.g_PathFinder == NULL || gGlobal.g_PathFinder->IsBlocked(x, y));
}

UInt16 CMap::GetActorCount()
{
	return ((UInt16) m_objects.size());
}

RPCResult CMap::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)
{
#ifdef QGL
	if( errMsg.CompareNoCase(_T("MSG_MAP_FORCE_LEAVE"))==0 )
	{
		String text(gGlobal.GetStringTable()->Get(errMsg.c_str()));
		String empty;
		if (!text.Equals(empty.c_str()))
		{
			gGlobal.PrintMessage(text, CHANNELTYPE_SYSTEM);
			gGlobal.PrintPlayerMessage(text.c_str());
		}
	}
		
#endif // QGL

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_GetCharInRange(LPCPEER pPeer, RPCContext &context, NetGroup * grp)
{
	DeleteNetGroup(grp);

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_GetJumpPoints(LPCPEER pPeer, RPCContext &context, const JumpInfoVector &jumpInfoList)
{
	return RPC_RESULT_OK;
}

RPCResult CMap::CB_ObjectChanged(LPCPEER pPeer, RPCContext &context, NetGroup* grp, 
								 const RPCNetIDVector &delIds,
								 const RPCNetIDVector &curIds)
{
	TRACE_ENSURE(grp);

	TRACE_VERBOSEDTL_2(GLOBAL_LOGGER, _F("CB_ObjectChanged: count %d, del count %d"), 
		grp->GetCount(), delIds.size());

#ifdef DEBUG_CHECK_VIEW
	String s;
	UInt16 checkCount = 0;
#endif
	CMapActor* actor;

	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		actor = grp->GetChild<CMapActor>(i);
		if (actor == NULL ||
			gGlobal.g_pCharacter && actor == gGlobal.g_pCharacter->GetCharNetObject())
			continue;

#ifdef DEBUG_CHECK_VIEW
		if (actor->IsUpdated(CMapActor::appearedMask)) {
			s.AppendFormat(_T("%s%d, "), (actor->GetAppeared() ? _T("A") : _T("D")), 
				actor->GetNetId().objId);
			checkCount++;
		}
#endif // DEBUG_CHECK_VIEW
		m_objects[actor->GetNetId()] = actor;
		if (actor->GetClassID() == Character_CLASSID)
		{
			CCharacter* pchar = CastNetObject<CCharacter>(actor);
			if (pchar && pchar->IsUpdated(CMapActor::appearedMask)) {
				if (pchar->GetAppeared()) {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, 
						_T("New Character Appeared: char_id %d, netId %d,%d, curPos %d,%d"), 
						pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
						pchar->GetCurPosX(), pchar->GetCurPosY());
				} else {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, 
						_T("Character Disappeared: char_id %d, netId %d,%d, curPos %d,%d"), 
						pchar->GetChar_id(), pchar->GetNetId().srvId, pchar->GetNetId().objId, 
						pchar->GetCurPosX(), pchar->GetCurPosY());
				}
			}
		}
		else if (actor->GetClassID() == MapNPC_CLASSID)
		{
			CMapNPC* pnpc = CastNetObject<CMapNPC>(actor);
			if (pnpc && pnpc->IsUpdated(CMapActor::appearedMask)) {
				if (pnpc->GetAppeared()) {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("New NPC Appeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
						pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
						pnpc->GetCurPosX(), pnpc->GetCurPosY());
				} else {
					TRACE_VERBOSEDTL_5(GLOBAL_LOGGER, _F("NPC Disappeared: npc_id %d, netId %d,%d, curPos %d,%d"), 
						pnpc->GetNpc_uid(), pnpc->GetNetId().srvId, pnpc->GetNetId().objId,
						pnpc->GetCurPosX(), pnpc->GetCurPosY());
				}
			}
		}

		if (m_pMapView && actor->IsUpdated(CMapActor::infoMask) && actor->GetAppeared()) {
			m_pMapView->NotifyObjectAppeared(actor);
		}
	}

	DeleteNetGroup(grp);

	for (RPCNetIDVector::const_iterator it = delIds.begin(); it != delIds.end(); ++it)
	{
		MapActorMap::iterator it2 = m_objects.find(*it);
		if (it2 != m_objects.end())
		{
#ifdef DEBUG_CHECK_VIEW
			s.AppendFormat(_T("R%d, "), (*it).objId);
			checkCount++;
#endif // DEBUG_CHECK_VIEW
			actor = it2->second;
			TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CB_ObjectChanged: delete actor %d, netId %d,%d deleted"), actor->GetActorId(), it->srvId, it->objId);
			if (m_pMapView)
				m_pMapView->NotifyObjectDisappeared(actor);
			else
				DeleteNetObject(actor);
			m_objects.erase(it2);
		}
		else
		{
			actor = FindNetObject<CMapActor>(*it);
			if (actor)
			{
			TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, _F("CB_ObjectChanged: delete actor %d, netId %d,%d deleted"), actor->GetActorId(), it->srvId, it->objId);
				if (m_pMapView)
					m_pMapView->NotifyObjectDisappeared(actor);
				else
					DeleteNetObject(actor);
			}
		}
	}

	RPCNetID myNetId = gGlobal.g_pCharacter->GetCharNetObject()->GetNetId();
#ifdef DEBUG_CHECK_VIEW
	if (checkCount > 0)
		TRACE_INFODTL_4(GLOBAL_LOGGER, _F("CheckView: owner netId %d,%d, count %d, %s"), 
			myNetId.srvId, myNetId.objId, checkCount, s.c_str());
#endif // DEBUG_CHECK_VIEW

	// check if all current net Ids are really in view when all sync
	if (!curIds.empty()) {
		for (RPCNetIDVector::const_iterator it = curIds.begin(); it != curIds.end(); ++it)
		{
			const RPCNetID &netId = *it;
			if (myNetId == netId)
				continue;
			MapActorMap::iterator it2 = m_objects.find(netId);
			if (it2 == m_objects.end()) {
				TRACE_WARNDTL_2(GLOBAL_LOGGER, 
					_T("CB_ObjectChanged Warning: [missing actor], netId %d,%d"), 
					netId.srvId, netId.objId);
			} else {
				actor = it2->second;
				if (!actor->GetAppeared()) {
					TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("CB_ObjectChanged Warning: [actor disappeared] actor %d, netId %d,%d, objID %d"), 
						actor->GetActorId(), actor->GetNetId().srvId, 
						actor->GetNetId().objId, actor->GetID());
				}
			}
		}
	}

	return RPC_RESULT_OK;
}

/*
RPCResult CMap::CB_ObjectAppeared(LPCPEER pPeer, RPCContext &context, NetObject * obj)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_ObjectAppeared:"));
	m_objects->Add(obj);
	
	if (m_pMapView)
		m_pMapView->NotifyObjectAppeared(obj);

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_ObjectDisappeared(LPCPEER pPeer, RPCContext &context, NetObject * obj)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_ObjectDisappeared:"));
	//
	CMapNPC * pNPC = CastNetObject<CMapNPC>(obj);
	if ( pNPC )
	{
		pNPC->CancelTalkingTarget();
	}
	//

	m_objects->Remove(obj);

	if (m_pMapView)
		m_pMapView->NotifyObjectDisappeared(obj);
	else
		DeleteNetObject(obj);

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_GroupAppeared(LPCPEER pPeer, RPCContext &context, NetGroup * grp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GroupAppeared:"));
	//DeleteNetGroup(grp, TRUE);
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		m_objects->Add(grp->GetChild(i));
		if (m_pMapView)
			m_pMapView->NotifyObjectAppeared(grp->GetChild(i));

	}

	// remove object after used
	Reuben::Simulation::DeleteObject(grp);

	
		
	
	

	return RPC_RESULT_OK;
}

RPCResult CMap::CB_GroupDisappeared(LPCPEER pPeer, RPCContext &context, NetGroup * grp)
{
	TRACE_INFODTL(GLOBAL_LOGGER, _F("CB_GroupDisappeared:"));
	NetObject* child;
	for (UInt16 i = 0; i < grp->GetCount(); i++)
	{
		child = grp->GetChild(i);
		m_objects->Remove(child);

		if (m_pMapView)
			m_pMapView->NotifyObjectDisappeared(child);
		else
			DeleteNetObject(child);
	}

	// remove object after used
	Reuben::Simulation::DeleteObject(grp);

	return RPC_RESULT_OK;
}
*/

CMapNPC* CMap::FindNpc(UInt32 npc_id)
{
	NetObject* obj;
	CMapNPC* pnpc;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == MapNPC_CLASSID) {
			pnpc = CastNetObject<CMapNPC>(obj);
			if (pnpc && pnpc->GetNpc_uid() == npc_id)
				return pnpc;
		}
	}
	return NULL;
}

CCharacter* CMap::FindCharacter(UInt32 Char_id)
{
	NetObject* obj;
	CCharacter* pChar = NULL;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == Character_CLASSID) {
			pChar = CastNetObject<CCharacter>(obj);
			if (pChar && pChar->GetChar_id() == Char_id)
				return pChar;
		}
	}
	return NULL;
}

// get nearby character within radius block
// and type, 0=all, 1=single, 2=in party, 3=in battle, 4=battle help, 5=pk enable
CCharacterControl* CMap::GetNearCharacter(UInt16 x, UInt16 y, UInt16 radius, UInt8 type)
{
	NetObject* obj;
	CCharacter* pchar;
	UInt16 d2, r2;

	r2 = radius * radius;
	for (MapActorMap::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		obj = it->second;
		if (obj && obj->GetClassID() == Character_CLASSID) {
			pchar = CastNetObject<CCharacter>(obj);
			if (pchar && pchar->GetAppeared() && pchar->GetChar_id() > 0 &&
				(type == 0 ||
				type == 1 && !pchar->GetHasParty() ||
				type == 2 && pchar->GetHasParty() ||
				type == 3 && 
					(pchar->GetAction() == CCharacter::ST_BATTLE ||
					 pchar->GetAction() == CCharacter::ST_NEEDHELP) ||
				type == 4 && pchar->GetAction() == CCharacter::ST_NEEDHELP ||
				type == 5 && pchar->GetEnablePK() && 
					(pchar->GetAction() != CCharacter::ST_BATTLE && 
					 pchar->GetAction() != CCharacter::ST_NEEDHELP))
				)
			{
				d2 = (pchar->GetCurPosX() - x) * (pchar->GetCurPosX() - x) +
					(pchar->GetCurPosY() - y) * (pchar->GetCurPosY() - y);
				if (d2 <= radius)
				{
					CCharacterControl* pCtrl = QGLApplication::GetInstance().FindCharCtrlByNetObject(pchar);
					return pCtrl;
				}
			}
		}
	}

	return NULL;
}

void CMap::MoveToRandomGuidePos()
{
	static UInt16	curIndex = 0;

	CCharacter* pchar = gGlobal.g_pCharacter->GetCharNetObject();
	if (pchar && gGlobal.g_PathFinder)
	{
		//BlockPos currentPos = pchar->GetCharacterView()->GetCharMainView()->GetCharPos();

		if(curIndex >= gGlobal.g_PathFinder->GetGuideCount() - 1)
			curIndex = 0;
		else
			curIndex++;

		BlockPos newPos = gGlobal.g_PathFinder->GetGuidePoint(curIndex);

		gGlobal.g_pCharacter->SetEntityDesPos(newPos.PosX, newPos.PosY);

		if (gGlobal.g_pMiddleMap->IsOpenMiddleMap())
			gGlobal.g_pMiddleMap->DrawPath();
		else
		{
			gGlobal.g_pMiddleMap->SetCharDes(newPos.PosX, newPos.PosY);
			gGlobal.g_pMiddleMap->SetPathEnd(newPos.PosX, newPos.PosY);
		}

	}
}

