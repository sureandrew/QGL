//-- Common
#include "Common.h"
//-- Self
#include "ViewManager.h"
//-- Library
#include "Stub/Logic/Map.h"
#include "Stub/Logic/MapNPC.h"
#include "Stub/Logic/Character.h"
#include "Stub/Logic/Party.h"
#include "MapService/Global.h"
#include "MapService/Configure.h"
#include "RPCEnum/Account_enum.h"
#include <algorithm>

//#define DEBUG_CHECK_VIEW		1			// do check view if defined

#define ACTOR_AGING_TIME		10000		// 10 second of actor aging time in view
#define VIEW_CHECK_INTERVAL		5000
#define SYNC_ACTOR_LIMIT		10	//20
//#define VIEW_TESTING			1

enum
{
	CONCERN_NONE = 0,
	CONCERN_NPC = 1,
	CONCERN_CHAR = 2,
	CONCERN_MEMBER = 3,
	CONCERN_SELF = 4,
};

UInt32 ViewManager::m_agingTime = ACTOR_AGING_TIME;
UInt16 ViewManager::m_syncLimit = SYNC_ACTOR_LIMIT;

ViewManager::ViewManager(CCharacter *owner)
{
	m_owner = owner;
	m_nextView = NULL;
	m_ringBelong = NULL;
	m_delLink = NULL;

	// owner view
	m_rootLink = SafeCreateObject(ActorView);
	m_rootLink->classId = Character_CLASSID;
	m_rootLink->pChar = owner;
	m_rootLink->level = 1;
	m_rootLink->concernMask = &CCharacter::m_selfMask;
	m_rootLink->updateMask.Clear();
	m_rootLink->prev = m_rootLink;
	m_rootLink->next = m_rootLink;
	m_curLink = m_rootLink;
	m_viewCount = 0;

	//gGlobal.LinkView(this);
}

ViewManager::~ViewManager()
{
	// safe unlink and remove
	gGlobal.UnlinkView(this);
	RemoveAllView();
	SafeDeleteObject(m_rootLink);
}

VOID ViewManager::Init(VOID)
{
	TRACE_ENSURE(m_owner && m_rootLink);
	m_rootLink->netId = m_owner->GetNetId();
	m_rootLink->appear = BOOLEAN_TRUE;
}

INLINE ActorView* ViewManager::FindView(CMapActor* actor)
{
	if (actor == m_owner || actor == NULL) return NULL;

	ActorView* pview = m_rootLink->next;
	while (pview != m_rootLink) {
		if (pview->pActor == actor)
			break;
		pview = pview->next;
	}

	// still not found
	if (pview == m_rootLink)
		return NULL;
	else
		return pview;
}

INLINE ActorView* ViewManager::CreateView()
{
	ActorView* pview;
	if (m_delLink)									// use old view
	{
		pview = m_delLink;
		m_delLink = m_delLink->next;
		pview->next = NULL;
		pview->classId = 0;
	}
	else
	{	
		pview = SafeCreateObject(ActorView);		// create new ActorView object
	}
	TRACE_ENSURE(pview);
	return pview;
}

INLINE void ViewManager::DeleteView(ActorView* pview)
{
	TRACE_ENSURE(pview);
	//TRACE_ENSURE(pview->next->prev == pview && pview->prev->next == pview);
	pview->next->prev = pview->prev;		// remove from order link
	pview->prev->next = pview->next;
	pview->next = m_delLink;				// move to deleted link
	pview->prev = NULL;
	pview->ageTime = 0;
	pview->appear = BOOLEAN_FALSE;
	m_delLink = pview;
}

INLINE void ViewManager::RemoveLink(ActorView* pview, CMapActor* actor)
{
	TRACE_ENSURE(pview && pview->next || actor && actor != m_owner);

	//TRACE_INFODTL_4(GLOBAL_LOGGER, _F("ViewManager::RemoveLink (1) owner %d, target %d, view %d, actor %d"),
	//	m_owner->GetID(), (pview) ? pview->pActor : 0, pview, actor);

	if (actor)
	{
		ActorViewMap::iterator it = m_actorView.find(actor->GetNetId());
		if (it == m_actorView.end())
		{
			//TRACE_INFODTL_6(GLOBAL_LOGGER, _F("ViewManager::RemoveLink (2) owner %d, target %d, view %d, actor %d netid %d:%d"),
			//	m_owner->GetID(), (pview) ? pview->pActor : 0, pview, actor, actor->GetNetId().srvId, actor->GetNetId().objId);
			// view not exist
			return;
		}

		pview = it->second;
		TRACE_ENSURE(pview->netId == actor->GetNetId());
		m_actorView.erase(it);
		m_delIds.push_back(pview->netId);

		//TRACE_INFODTL_6(GLOBAL_LOGGER, _F("ViewManager::RemoveLink (3) owner %d, target %d, view %d, actor %d netid %d:%d"),
		//	m_owner->GetID(), (pview) ? pview->pActor : 0, pview, actor, pview->pActor->GetNetId().srvId, pview->pActor->GetNetId().objId);

	}
	else
	{
		ActorViewMap::iterator it = m_actorView.find(pview->netId);
		if (it == m_actorView.end())
		{
			TRACE_ERRORDTL_6(GLOBAL_LOGGER, _F("ViewManager::RemoveLink Error (4) owner %d, target %d, view %d, actor %d netid %d:%d"),
				m_owner->GetID(), (pview) ? pview->pActor : 0, pview, actor, pview->netId.srvId, pview->netId.objId);
		}
		else
		{
			TRACE_ENSURE(it->second == pview && it->second->pActor == pview->pActor);
			m_actorView.erase(it);
			m_delIds.push_back(pview->netId);
		}
		//TRACE_INFODTL_6(GLOBAL_LOGGER, _F("ViewManager::RemoveLink (5) owner %d, target %d, view %d, actor %d netid %d:%d"),
		//	m_owner->GetID(), (pview) ? pview->pActor : 0, pview, actor, pview->pActor->GetNetId().srvId, pview->pActor->GetNetId().objId);
	}

	if (pview->appear && m_viewCount > 0)
		m_viewCount--;

	if (pview->classId == MapNPC_CLASSID && pview->pNpc)
		pview->pNpc->Unlink(m_owner);

//	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("RemoveLink: owner %d, actor_id %d, netId %d:%d, objId %d"),
//		m_owner->GetChar_id(), pview->pActor->GetActorId(), 
//		pview->pActor->GetNetId().srvId, pview->pActor->GetNetId().objId, 
//		pview->pActor->GetID());

	if (m_curLink == pview)					// if current link removed from list
		m_curLink = m_curLink->next;
	DeleteView(pview);

	// also remove opposite link
	if (pview->pChar && actor == NULL && pview->classId == Character_CLASSID)
	{
		if (pview->pChar == FindNetObject<CCharacter>(pview->netId))
		pview->pChar->m_viewManager->RemoveLink(NULL, m_owner);
	}

	// ensure no view in root order link after removed
//	ActorView* curView = m_rootLink->next;		// skip self
//	while (curView && curView != m_rootLink)
//	{
//		TRACE_ENSURE(curView != pview && curView->pActor != pview->pActor);
//		if (curView->ageTime > 0)
//			TRACE_ENSURE(curView->pActor && curView->pActor->IsSubClass(MapActor_CLASSID));
//		curView = curView->next;
//	}
}

INLINE Boolean ViewManager::CanAddView(CMapActor *actor)
{
	// not reach view limit
	if (m_viewCount < gConfigure.viewLimit)
		return BOOLEAN_TRUE;

	// special ignore cases: npc, hawking, party requestting or party member
	CCharacter* pchar = CastNetObject<CCharacter>(actor);
	return (pchar == NULL || pchar->IsHawking() || m_owner->HasRequested(pchar) ||
			m_owner->GetParty() && m_owner->GetParty()->IsMember(pchar));
}

Boolean ViewManager::AddView(CMapActor *actor, Boolean bNext)
{
	if (actor == NULL || actor == m_owner || actor != FindNetObject(actor->GetNetId()))
		return BOOLEAN_FALSE;

	// check in actor map
	ActorView* pview = NULL;
	ActorViewMap::iterator it = m_actorView.find(actor->GetNetId());
	bool found = (it != m_actorView.end());

	if (found)
	{
		pview = it->second;
		TRACE_ENSURE(pview->pActor == actor);
		if (pview->appear)		 // already appeared in view
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("AddView Warning (1): [view exist] actor netId %d:%d, objId %d, view of char_id %d"),
				actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetID(), 
				m_owner->GetChar_id());
			return BOOLEAN_TRUE;
		}
	} else {
		if (!CanAddView(actor)) {
			//TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("AddView Warning (2): [can't add view] actor netId %d:%d, objId %d, view of char_id %d"),
			//	actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetID(), 
			//	m_owner->GetChar_id());
			return (BOOLEAN_FALSE);
		}
	}

	if (bNext)
	{
		if (actor->GetClassID() == Character_CLASSID) {
			CCharacter* pchar = CastNetObject<CCharacter>(actor);
			if (pchar)
			{
				// remove view when another character can't add view
				if (!pchar->m_viewManager->AddView(m_owner, BOOLEAN_FALSE))
					return BOOLEAN_FALSE;
			}
		} else if (actor->GetClassID() == MapNPC_CLASSID) {
			CMapNPC* pnpc = CastNetObject<CMapNPC>(actor);
			if (pnpc)
				pnpc->Link(m_owner);
		}
	}

//	if (actor->GetClassID() == Character_CLASSID)
//		TRACE_INFODTL_5(GLOBAL_LOGGER, _F("AddView: owner %d, char_id %d, netId %d:%d, objId %d"),
//			m_owner->GetChar_id(), ((CCharacter*)actor)->GetChar_id(), 
//			actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetID());

	if (!found)				// get ActorView object from deleted cache
		pview = CreateView();

	if (actor->IsSubClass(Character_CLASSID))
	{
		CCharacter *pchar = (CCharacter*) actor;
		pview->classId = Character_CLASSID;
		pview->level = 0;
		pview->concernMask = &CCharacter::m_charMask;
	}
	else if (actor->IsSubClass(MapNPC_CLASSID))
	{
		pview->classId = MapNPC_CLASSID;
		pview->level = 1;
		pview->concernMask = &CCharacter::m_npcMask;
	}
	else
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("Character AddView Warning: Invalid classID %d"),
			actor->GetClassID());
		if (!found)
			SafeDeleteObject(pview);
		return BOOLEAN_FALSE;
	}

	if (!found)
	{
		// set initial mask for new actor view
		//TRACE_INFODTL_9(GLOBAL_LOGGER, _F("ViewManager::AddView owner %d, target %d netid %d:%d, target_class %d, region %d, map_id %d, map_x %d, map_y %d"),
		//	m_owner->GetID(), actor->GetID(), actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetClassID(), actor->m_curRegion, actor->GetMap_id(),
		//	actor->GetMap_x(), actor->GetMap_y());
		pview->pActor = actor;
		pview->updateMask = CCharacter::m_initMask;
		pview->updateMask.Mask(*pview->concernMask);
		pview->netId = actor->GetNetId();
		m_actorView[actor->GetNetId()] = pview;		// insert into actorView map

		// append to last of current order list
		TRACE_ENSURE(m_curLink && m_curLink->prev);
		pview->prev = m_curLink->prev;
		pview->next = m_curLink;
		m_curLink->prev->next = pview;
		m_curLink->prev = pview;
	}

	// remove in delete list if any
	RPCNetIDVector::iterator itfind = std::find(m_delIds.begin(), m_delIds.end(), 
										actor->GetNetId());
	if (itfind != m_delIds.end())
		m_delIds.erase(itfind);

	pview->updateMask.MaskBit(CMapActor::appearedMask);
	pview->appear = BOOLEAN_TRUE;
	pview->ageTime = UINT32_MAX_VALUE;
	m_viewCount ++;

	// ensure no duplicate view in root order link
//	ActorView* curView = m_rootLink->next;		// skip self
//	while (curView && curView != m_rootLink)
//	{
//		if (curView->ageTime > 0) {
//			TRACE_ENSURE(curView == pview || curView->pActor != actor);
//			TRACE_ENSURE(pview->pActor && pview->pActor->IsSubClass(MapActor_CLASSID));
//		}
//		curView = curView->next;
//	}

	return BOOLEAN_TRUE;
}

Boolean ViewManager::RemoveView(CMapActor *actor, Boolean bDelete, Boolean bNext)
{
	if (m_owner == NULL || m_curLink == NULL || actor == NULL || actor == m_owner)
		return BOOLEAN_FALSE;

	ActorViewMap::iterator it = m_actorView.find(actor->GetNetId());
	if (it == m_actorView.end())
	{
		// view not exist, should be can't added before because of too many views
		//TRACE_WARNDTL_5(GLOBAL_LOGGER, _F("RemoveView Warning (1): actorId %d, netId %d:%d, objId %d is not in view of char_id %d"),
		//	actor->GetActorId(), actor->GetNetId().srvId, actor->GetNetId().objId, 
		//	actor->GetID(), m_owner->GetChar_id());
		return BOOLEAN_FALSE;
	}

	ActorView* pview = it->second;
	TRACE_ENSURE(pview->pActor == actor);
	
	//Delete By Tuwei
	//Because Add Temporarily leave Party.A party member leave Leader view is possible.The later lines must delete
	// warning when remove party member view
	/*if (m_owner->GetParty() && m_owner->GetParty()->IsMember(pview->pChar))
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _F("RemoveView Warning (2): delete view netId %d:%d but in same party %d:%d"),
			it->first.srvId, it->first.objId, m_owner->GetParty()->GetNetId().srvId, m_owner->GetParty()->GetNetId().objId);

		if (pview->pChar)
		{
			TRACE_WARNL_8(GLOBAL_LOGGER, _F("RemoveView Warning (2): view char_id %d, map %d, %d:%d, this char_id %d, map %d, %d:%d"),
 				pview->pChar->GetChar_id(), pview->pChar->GetMap_id(), pview->pChar->GetCurPosX(), pview->pChar->GetCurPosY(),
				m_owner->GetChar_id(), m_owner->GetMap_id(), m_owner->GetCurPosX(), m_owner->GetCurPosY());
		}
		return BOOLEAN_FALSE;
	}*/

//	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("RemoveView: owner %d, char_id %d, netId %d:%d, objId %d, bDelete %d"),
//		m_owner->GetChar_id(), ((CCharacter*)actor)->GetChar_id(), actor->GetNetId().srvId, actor->GetNetId().objId, actor->GetID(), bDelete);

	if (pview->ageTime == UINT32_MAX_VALUE && 				// still haven't sync
		pview->updateMask.IsBit(CMapActor::appearedMask))	// remove the view
	{
		//TRACE_INFODTL_7(GLOBAL_LOGGER, _F("ViewManager::RemoveView (1) owner %d, target %d, target_class %d, region %d, map_id %d, map_x %d, map_y %d"),
		//	m_owner->GetID(), actor->GetID(), actor->GetClassID(), actor->m_curRegion, actor->GetMap_id(), actor->GetMap_x(), actor->GetMap_y());
		RemoveLink(pview);
		bDelete = BOOLEAN_TRUE;
	}
	else if (bDelete)
	{
		//TRACE_INFODTL_7(GLOBAL_LOGGER, _F("ViewManager::RemoveView (2) owner %d, target %d, target_class %d, region %d, map_id %d, map_x %d, map_y %d"),
		//	m_owner->GetID(), actor->GetID(), actor->GetClassID(), actor->m_curRegion, actor->GetMap_id(), actor->GetMap_x(), actor->GetMap_y());
		// if object deleted
		RemoveLink(pview);
	}
	else if (pview->ageTime == UINT32_MAX_VALUE)
	{
		//TRACE_INFODTL_7(GLOBAL_LOGGER, _F("ViewManager::RemoveView (3) owner %d, target %d, target_class %d, region %d, map_id %d, map_x %d, map_y %d"),
		//	m_owner->GetID(), actor->GetID(), actor->GetClassID(), actor->m_curRegion, actor->GetMap_id(), actor->GetMap_x(), actor->GetMap_y());
		pview->ageTime = m_agingTime;
		pview->appear = BOOLEAN_FALSE;
		pview->updateMask.MaskBit(CMapActor::appearedMask);
		if (m_viewCount > 0)
			m_viewCount--;
	}

	if (bNext && !bDelete && pview->classId == Character_CLASSID)
	{
		if (pview->pChar == FindNetObject<CCharacter>(pview->netId))
			pview->pChar->m_viewManager->RemoveView(m_owner, BOOLEAN_FALSE, BOOLEAN_FALSE);
	}

	// ensure no deleted actor
//	ActorView* curView = m_rootLink->next;		// skip self
//	while (curView && curView != m_rootLink)
//	{
//		if (curView->ageTime > 0)
//			TRACE_ENSURE(curView->pActor && curView->pActor->IsSubClass(MapActor_CLASSID));
//		curView = curView->next;
//	}

	return BOOLEAN_TRUE;
}

void ViewManager::RemoveAllView()
{
	TRACE_ENSURE(m_rootLink);
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("char_id = %d"), m_owner->GetChar_id());

	ActorView* pview = m_rootLink->next;		// skip owner
	ActorView* tmp;

	// remove view in order list
	while (pview && pview != m_rootLink)
	{
		tmp = pview->next;
		if (pview->classId == Character_CLASSID && pview->ageTime > 0)
		{
			__try
			{
				if (pview && pview->pChar && pview->pChar->m_viewManager)
				{
					if (pview->pChar == FindNetObject<CCharacter>(pview->netId))
						pview->pChar->m_viewManager->RemoveView(m_owner, BOOLEAN_TRUE);
					else
					{
						TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("char not match, view %d, char %d, netid %d:%d"),
							pview, pview->pChar, pview->netId.srvId, pview->netId.objId);
					}
				}
				else
				{
					TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("something null, view %d, char %d, view manager %d"),
						pview, (pview) ? pview->pChar : 0, (pview) ? (pview->pChar) ? pview->pChar->m_viewManager : 0 : 0);
				}
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("exception occuss, view %d"), pview);
			}
		}
		else if (pview->classId == MapNPC_CLASSID && pview->ageTime > 0)
		{
			pview->pNpc->Unlink(m_owner);
		}
		pview = tmp;
	}

	//__try
	//{
	pview = m_rootLink->next;
	while (pview && pview != m_rootLink)
	{
		//TRACE_ENSURE(pview);
		tmp = pview;
		pview = pview->next;
		SafeDeleteObject(tmp);
	}
	//}
	//__except(EXCEPTION_EXECUTE_HANDLER)
	//{
	//	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("ViewManager Exception (2): RemoveAllView %d"), pview);
	//}

	//TRACE_INFODTL_1(GLOBAL_LOGGER, _F("ViewManager::RemoveAllView [m_actorView.clear()] owner %d"), m_owner->GetID());
	m_actorView.clear();

	// remove old cache view
	pview = m_delLink;
	//__try
	//{
	while (pview)
	{
		tmp = pview;
		pview = pview->next;
		SafeDeleteObject(tmp);
	}
	//}
	//__except(EXCEPTION_EXECUTE_HANDLER)
	//{
	//	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("ViewManager Exception (3): RemoveAllView %d"), pview);
	//}
	m_delLink = NULL;

	// only keep owner in order list
	m_rootLink->prev = m_rootLink;
	m_rootLink->next = m_rootLink;
	m_curLink = m_rootLink;
	m_delIds.clear();
	m_viewCount = 0;

	// safe remove this actor in all actors
	//	if (gGlobal.m_viewRing) {
	//		ViewRing *ring = gGlobal.m_viewRing->next;
	//		while (ring && ring != gGlobal.m_viewRing) {
	//			ViewManager* viewMan = ring->child;
	//			while (viewMan) {
	//				viewMan->SafeRemove(m_owner);
	//				viewMan = viewMan->m_nextView;
	//			}
	//			ring = ring->next;
	//		}
	//	}
}

void ViewManager::SafeRemove(CMapActor *actor)
{
	if (actor == NULL || actor == m_owner) return;

	ActorViewMap::iterator it = m_actorView.find(actor->GetNetId());
	
	if (it != m_actorView.end())
	{
		TRACE_WARNDTL_6(GLOBAL_LOGGER, _F("SafeRemove Warning: actorId %d, netId %d:%d, objId %d, ageTime %u is still in view of char_id %d"),
			actor->GetActorId(), actor->GetNetId().srvId, actor->GetNetId().objId, 
			actor->GetID(), it->second->ageTime, m_owner->GetChar_id());

		RemoveLink(it->second);
	}
	else
	{
		// try to find actor from link, then remove it
//		ActorView* pview = FindView(actor);
//		if (pview) {
//		TRACE_WARNDTL_6(GLOBAL_LOGGER, _F("SafeRemove Warning (2): actorId %d, netId %d:%d, objId %d, ageTime %u is still in view of char_id %d"),
//			actor->GetActorId(), actor->GetNetId().srvId, actor->GetNetId().objId, 
//			actor->GetID(), pview->ageTime, m_owner->GetChar_id());
//			RemoveLink(pview);
//		}
	}
}

void ViewManager::ClearView(CMapActor *actor, RPCMask &mask)
{
	if (actor == NULL || actor != FindNetObject(actor->GetNetId()))
		return;

	if (actor == m_owner) {						// self view
		TRACE_ENSURE(m_rootLink);
		m_rootLink->updateMask.Unmask(mask);
		return;
	}

	ActorViewMap::iterator it = m_actorView.find(actor->GetNetId());
	
	if (it == m_actorView.end())				// not exist
		return;

	it->second->updateMask.Unmask(mask);
}

void ViewManager::ResendView()
{
	TRACE_ENSURE(m_owner && m_rootLink);

	ActorView* pview = m_rootLink->next;		// skip self
	ActorView* tmpView;

	m_viewCount = 0;
	while (pview && pview != m_rootLink)
	{
		if (pview->appear) {					// inside view, re-initial mask 
			if (pview->classId == Character_CLASSID) {
				pview->level = 0;
			} else if (pview->classId == MapNPC_CLASSID) {
				pview->level = 1;
			}
			pview->updateMask = CCharacter::m_initMask;
			pview->updateMask.Mask(*pview->concernMask);
			pview->updateMask.MaskBit(CMapActor::appearedMask);
			pview = pview->next;
			m_viewCount++;
		} else {											// outside view, remove all
			tmpView = pview->next;
			RemoveLink(pview);
			pview = tmpView;
		}
	}
	m_delIds.clear();
	m_curLink = m_rootLink;
}

void ViewManager::SetConcernMask(CMapActor *actor, RPCMask &mask)
{
	ActorViewMap::iterator it = m_actorView.find(actor->GetNetId());
	
	if (it == m_actorView.end())				// not exist
		return;

	ActorView* pview = it->second;
	pview->concernMask = &mask;
}

Boolean ViewManager::IsValid()
{
	return (m_owner && m_curLink && m_rootLink && m_owner->GetNetId() == m_rootLink->netId &&
		m_owner == FindNetObject<CCharacter>(m_owner->GetNetId()));
}

Boolean HelpToSetAppear(ActorView* pview)
{
	__try
	{
		pview->pActor->SetAppeared(BOOLEAN_TRUE);
		return (BOOLEAN_TRUE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return (BOOLEAN_FALSE);
	}
}

void ViewManager::SyncClient(CONST UInt32 dt)
{
	if (!IsValid())
	{
		RPCNetID netId;
		if (m_owner)
			netId = m_owner->GetNetId();
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("[invalid owner], m_owner %u, netId %d:%d"),
			m_owner, netId.srvId, netId.objId);
		return;
	}

	// set old view update mask
	CMap* pmap = m_owner->GetMap();
	m_owner->m_oldViewMask = m_owner->GetViewMask();
	m_owner->ClearViewMask();

	// no need to sync client
	if (pmap == NULL || m_owner->GetPeer() == NULL)
		return;

	if (pmap != FindNetObject<CMap>(pmap->GetNetId()))
	{
		TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("[invalid map], map %u, netId %d:%d"),
			pmap, pmap->GetNetId().srvId, pmap->GetNetId().objId);
		m_owner->OnLeaveMap();
		m_owner->ForceLogout(_T("invalid map"));
		return;
	}

	//bool bRoot = false;									// root link traversed
	bool delFirst;										// first pview deleted
	NetGroup grp;
	ActorView* tmpView;
	ActorView* pview = m_curLink;
	ActorView* lastLink = m_curLink;					// last link for sync
	UInt16 count = 0;
	int i = 0;

	do
	{
		TRACE_ENSURE(pview && i < 9999);				// dead loop checking
		++i;
		delFirst = false;
		// delete pview if too old 
		if (pview->pActor == NULL || !pview->appear && pview->ageTime <= dt)
		{
			if (lastLink == pview)				// lastLink deleted, so assign to next
				lastLink = lastLink->next;
			if (pview == m_curLink)				// is deleting first view?
				delFirst = true;
			tmpView = pview->next;				// store pview->next
			RemoveLink(pview);
			pview = tmpView;					// next to pview->next
			continue;
		}
		if (!pview->updateMask.IsZero() ||							// some update before
			pview->pActor->m_oldViewMask.IsMask(*pview->concernMask))	// old update
		{
			pview->updateMask.low |= pview->concernMask->low & 
				pview->pActor->m_oldViewMask.low;
			pview->updateMask.high |= pview->concernMask->high & 
				pview->pActor->m_oldViewMask.high; 

			// update curPos if action change or follow target change
			if (pview->updateMask.IsBit(CMapActor::actionMask) ||
				pview->updateMask.IsBit(CMapActor::followIdMask))
				pview->updateMask.MaskBit(CMapActor::curPosMask);

			// won't update dstPos if following
			if (pview->updateMask.IsBit(CMapActor::dstPosMask) && 
				pview->pActor->GetFollowId() > 0)
				pview->updateMask.UnmaskBit(CMapActor::dstPosMask);

			// inside view
			if (pview->appear)
			{
				if (count < m_syncLimit && !pview->updateMask.IsZero() &&
					FindNetObject(pview->netId) == pview->pActor)
				{
					if (pview->updateMask.IsBit(CMapActor::appearedMask))	// just appear
					{
						if (!HelpToSetAppear(pview))
						{
							TRACE_ERRORDTL_3(GLOBAL_LOGGER, _FE("set appear failed, view %d, actor %d, class_id %d"),
								pview, pview->pActor, pview->classId);
							pview->pActor = NULL;
							pview->updateMask.Clear();
							pview = pview->next;
							continue;
						}
					}
					if (pview->level == 0)
					{
						// only update simple mask
						RPCMask mask(CCharacter::m_simpleMask);
						mask &= pview->updateMask;
						pview->pActor->SetSendMask(mask);
						pview->updateMask.Unmask(mask);
						pview->level++;
					}
					else
					{
						// update all concern mask
						pview->pActor->SetSendMask(pview->updateMask);
						pview->updateMask.Clear();
					}
#ifdef VIEW_TESTING
					if (pview->pActor == m_owner)	// for testing
#endif // VIEW_TESTING
					grp.Add(pview->pActor);
					lastLink = pview->next;			// set last link to next sync link
					++count;
				}
			} 
		}

		if (pview->ageTime == m_agingTime)
		{
			// just disappear
			if (FindNetObject(pview->netId) == pview->pActor)
			{
				pview->pActor->SetAppeared(BOOLEAN_FALSE);
				pview->pActor->ClearSendMask();
				pview->pActor->MarkSend(CMapActor::appearedMask);
				grp.Add(pview->pActor);
			}
			pview->ageTime -= dt;
		}
		else if (pview->ageTime < m_agingTime)
		{
			pview->ageTime -= dt;
		}

		pview = pview->next;
	}
	while (pview && (pview != m_curLink || delFirst));
	
	// change current orderLink pointing
	if (lastLink != m_curLink)
		m_curLink = lastLink;

	// no need sync if disconnected or no view update
	if (m_owner->GetPeer() == NULL || grp.GetCount() == 0 && m_delIds.empty())
		return;

	// send updated and/or deleted objects
#ifdef VIEW_TESTING
	if (!m_delIds.empty())					// for testing
		m_delIds.clear();
#endif // VIEW_TESTING

	RPCNetIDVector netIds;

#ifdef DEBUG_CHECK_VIEW
	// check view only when all view sync
	if (count <= m_syncLimit)
	{
		pmap->GetViewNetIds(m_owner->m_curRegion, netIds);
		CheckView(grp, m_delIds, netIds);
	}
#endif // DEBUG_CHECK_VIEW

	pmap->CB_ObjectChanged(m_owner->GetPeer(), m_owner->GetContext(), 
		&grp, m_delIds, netIds);

	if (!m_delIds.empty())
		m_delIds.clear();
}

void ViewManager::CheckView(NetGroup &grp, RPCNetIDVector &delIds, const RPCNetIDVector &netIds)
{
	ActorView* pview;
	CMapActor* actor;
	ActorViewMap::iterator itfind;
	String s;
	UInt16 count = 0;

	for (UInt16 i = 0; i < grp.GetCount(); i++)
	{
		actor = grp.GetChild<CMapActor>(i);
		if (actor->GetSendMask().IsBit(CMapActor::appearedMask)) {
			s.AppendFormat(_T("%s%d, "), (actor->GetAppeared() ? _T("A") : _T("D")), 
				actor->GetNetId().objId);
			count++;
		}
	}
	for (RPCNetIDVector::iterator it = delIds.begin(); it != delIds.end(); ++it)
	{
		s.AppendFormat(_T("R%d, "), (*it).objId);
		count++;
	}
	if (count > 0)
		TRACE_INFODTL_9(GLOBAL_LOGGER, 
			_F("owner %d:%d:%d, map_id %d, region %d, count %d, viewCount%d(%d), %s"), 
			m_owner->GetID(), m_owner->GetNetId().srvId, m_owner->GetNetId().objId, m_owner->GetMap_id(),
			m_owner->m_curRegion, count, m_viewCount, netIds.size(), s.c_str());

	for (RPCNetIDVector::const_iterator it = netIds.begin(); it != netIds.end(); ++it)
	{
		const RPCNetID& netId = *it;
		if (netId == m_owner->GetNetId())
			continue;
		itfind = m_actorView.find(netId);
		if (itfind == m_actorView.end())
		{
			TRACE_WARNDTL_2(GLOBAL_LOGGER, 
				_FW("missing actor, netId %d:%d"), 
				netId.srvId, netId.objId);
		}
		else
		{
			pview = itfind->second;
			if (!pview->appear)
			{
				actor = pview->pActor;			
				TRACE_WARNDTL_4(GLOBAL_LOGGER, 
					_FW("actor disappeared, netId %d:%d, map_id %d, region %d"), 
					actor->GetNetId().srvId, actor->GetNetId().objId, 
					actor->GetMap_id(), actor->m_curRegion);
			}
		}
	}
}

CCharacter* HelpCastToCharacter(ActorView* pView)
{
	if (!pView || !pView->pActor)
		return (NULL);
	__try
	{
		return CastNetObject<CCharacter>(pView->pActor);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _FE("cast exception occuss, actor %d"), pView->pActor);
		pView->pActor = NULL;
		return (NULL);
	}
}

void ViewManager::GetCharacters(CharacterPtrList& chars)
{
	chars.clear();
	RPCNetID netId = m_owner->GetNetId();

	for (ActorViewMap::iterator it = m_actorView.begin(); it != m_actorView.end(); ++it)
	{
		ActorView* pview = it->second;
		if (pview && pview->classId == Character_CLASSID && pview->appear)
		{
			CCharacter * pChar = FindNetObject<CCharacter>(pview->netId);
			if (pChar)
			{
				ActorViewMap::iterator itfind = pChar->m_viewManager->m_actorView.find(netId);
				if (itfind != pChar->m_viewManager->m_actorView.end() &&
					itfind->second->appear && itfind->second->netId == netId)
					chars.push_back(pChar);
			}
		}
	}	
}

CCharacter* ViewManager::FindCharacter(UInt32 char_id)
{
	for (ActorViewMap::iterator it = m_actorView.begin(); it != m_actorView.end(); ++it)
	{
		ActorView* pview = it->second;
		if (pview && pview->classId == Character_CLASSID && pview->appear)
		{
			CCharacter* pChar = FindNetObject<CCharacter>(pview->netId);
			if (pChar && pChar->GetChar_id() == char_id)
				return (pChar);
		}
	}
	return (NULL);
}
