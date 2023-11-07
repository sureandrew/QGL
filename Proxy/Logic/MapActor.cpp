//-- Common
#include "Common.h"
//-- Self
#include "Proxy/Logic/MapActor.h"
//-- Library
#include "QGL/MapActorControl.h"
#include "QGL/CharacterControl.h"
#include "QGL/Global.h"
#include "QGL/MenuCharProcess.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/PartyCharInfoClient.h"

REGISTER_CLASSTYPE(CMapActor, MapActor_CLASSID);

CMapActor::CMapActor()
{
	m_pControl = NULL;
}

CMapActor::~CMapActor()
{
	if (m_pControl)
		m_pControl->SetMapActorObject(NULL);
}

void CMapActor::OnUpdateInfo(const String &nickName, const UInt8 &sex, const UInt8 &cclass, const UInt8 &sexClass, const UInt8 &faction)
{
	if (m_pControl)
	{
		m_pControl->OnUpdateInfo(nickName, EDGE_SHADOW);
		if (m_pControl == gGlobal.g_pCharacter)
			gGlobal.g_pCharProcess->UpdateCharMenu();
	}
	
}

void CMapActor::OnUpdateState(const UInt8 &state)
{
}

void CMapActor::OnUpdateMap(const UInt16 &map_id, const UInt16 &map_x, const UInt16 &map_y, const UInt8 &direction, const UInt32 &map_ownerId)
{
}

void CMapActor::OnUpdateReborn(const UInt16 &reborn_map, const UInt16 &reborn_x, const UInt16 &reborn_y)
{
}

void CMapActor::OnUpdateCurPos(const UInt16 &curPosX, const UInt16 &curPosY)
{
	if (m_pControl)
		m_pControl->OnUpdateCurPos();
	if (GetClassID() == Character_CLASSID) 
	{
		CParty* pParty = gGlobal.g_pParty == NULL ? gGlobal.g_pTempLeaveParty : gGlobal.g_pParty;
		if ( NULL != pParty) 
		{
			Global::PartyCharInfoMap::iterator it = gGlobal.g_mapPartyChar.begin();
			for (;it!= gGlobal.g_mapPartyChar.end();it++) 
			{
				CPartyCharInfoClient* pCpcic = it->second;
				if (NULL != pCpcic) 
				{
					if (pCpcic->GetStrNickName() == GetNickName()) 
					{
						pCpcic->SetU16XPos(curPosX);
						pCpcic->SetU16YPos(curPosY);
						break;
					}
				}
			}
		}
	}
}

void CMapActor::OnUpdateDstPos(const UInt16 &dstPosX, const UInt16 &dstPosY)
{
	if (m_pControl)
		m_pControl->OnUpdateDstPos();
}

void CMapActor::OnUpdateAction(const UInt16 &action, const UInt32 &actionTime)
{
	if (m_pControl)
		m_pControl->OnUpdateAction(action, actionTime);
}

void CMapActor::OnUpdateSpeed(const UInt32 &speed)
{
	if (m_pControl)
		m_pControl->OnUpdateSpeed();
}
VOID CMapActor::SetControl(CMapActorControl * pCtrl)
{
	m_pControl = pCtrl;
}

void CMapActor::OnUpdateFollowId(const UInt32 &followId, const UInt8 &followOrder)
{
	if (m_pControl)
		m_pControl->OnUpdateFollowId(followId, followOrder);
}

void CMapActor::OnUpdateAppeared(const Boolean &appeared)
{
	if (m_pControl)
		m_pControl->OnUpdateAppeared();
}