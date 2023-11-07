//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "MapActorControl.h"
//-- Library
#include "Configure.h"
#include "Global.h"
#include "Math.h"
#include "QGLApplication.h"
#include "AppMapState.h"
#include "Proxy/Logic/Party.h"
#include "Proxy/Logic/BatCharacter.h"
#include "Proxy/Logic/BatMob.h"
#include "UIEntity.h"

#define ChatBalloonReleaseTime	5000
#define MaxChatBalloons	3
#define ChangeAnimBase	5000
#define ChangeAnimVar	5000

CMapActorControl::CMapActorControl()
{
	//state init
	m_mapState = NULL;

	m_CharMainView.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
	m_CharMainView.SetCanBeSelected(TRUE);
	m_CharNameView.SetEntParent(&m_CharMainView);
	m_CharNameView.SetExtraZBuffer(NameExtraZ);
	m_CharNameView.SetEntOffsetY(gConfigure.NameOffsetY);

	m_CharFrontNameView.SetEntParent(&m_CharMainView);
	m_CharFrontNameView.SetExtraZBuffer(NameExtraZ);
	m_CharFrontNameView.SetEntOffsetY(gConfigure.NameOffsetY);
	
	m_CharShadowView.SetEntParent(&m_CharMainView);
	m_CharShadowView.SetExtraZBuffer(-1);
	m_CharHeadIconView.SetEntParent(&m_CharMainView);
	m_CharHeadIconView.SetExtraZBuffer(-2);
	m_CharHeadIconView.SetEntityType(ENTITY_TYPE_ENTITY_WITHOUTEFFECT);

	m_pMapActor = NULL;
	
	State::ChangeState(m_mapState, (State *) SafeCreateObject(CCharMapState));

	m_MapActorType = MAPACTORTYPE_UNDEFINE;

	m_State = 0;

	m_AnimIdleChangeCounter = ::timeGetTime();
	m_AnimIdleChangeTime = ChangeAnimBase + rand() % ChangeAnimVar;
}

CMapActorControl::~CMapActorControl()
{	
	State::ChangeState(m_mapState, NULL);
	//if (m_mapState)
	//SafeDeleteObject(m_mapState);

	QGLApplication::GetInstance().RemoveMapActorRenderLst(this);

	for (Int i = 0; i < (Int)m_ChatBalloonLst.size(); i++)
		SafeDeleteObject(m_ChatBalloonLst[i].pBalloon);
	m_ChatBalloonLst.clear();

	if (m_pMapActor)
		m_pMapActor->SetControl(NULL);
}


VOID CMapActorControl::InitShadow()
{
	m_CharShadowView.InitEntPicInfo();
	m_CharShadowView.LoadEntityFile(gConfigure.strShadowName.c_str(), gGlobal.pD3DDevice);
	m_CharShadowView.SetExtraZBuffer(-1);
	m_CharShadowView.SetEntParent(&m_CharMainView);
}

VOID CMapActorControl::LoadDefaultPart()
{
	m_CharMainView.InitEntPicInfo();
	m_CharMainView.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
	m_CharMainView.LoadEntityFile(gConfigure.strHead.c_str(), gGlobal.pD3DDevice, EQUIP_HEAD);
	m_CharMainView.LoadEntityFile(gConfigure.strBody.c_str(), gGlobal.pD3DDevice, EQUIP_BODY);
	m_CharMainView.LoadEntityFile(gConfigure.strWeapon.c_str(), gGlobal.pD3DDevice, EQUIP_WEAPON);
	m_CharMainView.LoadEntityFile(gConfigure.strHair.c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);
	m_CharMainView.LoadEntityFile(gConfigure.strExtra.c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);
	ResetNamePosition();
}

VOID CMapActorControl::LoadDefaultMob(const String & modelFilename, const String  &modelColor)
{
	m_CharMainView.InitEntPicInfo();
	
	m_CharMainView.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
	m_CharMainView.LoadEntityFile(modelFilename.c_str(), gGlobal.pD3DDevice);
	m_CharMainView.ChangeColorP(modelFilename.c_str(), modelColor.c_str());

	//calculate the name distance
	ResetNamePosition();
}

VOID CMapActorControl::ResetNamePosition()
{
	//RECT battleRect = m_CharMainView.GetBattleIdleRect();

	//if (battleRect.bottom + CharPicOffsetY > CommonEntSizY )
	//	m_CharNameView.SetEntOffsetY(battleRect.bottom + CharPicOffsetY - CommonEntSizY);

}
VOID CMapActorControl::LoadDefaultPart(UInt ModelID)
{
	m_CharMainView.InitEntPicInfo();
	
	m_CharMainView.SetEntityType(ENTITY_TYPE_MAP_PLAYER);
	char buf[256];
	sprintf(buf, "h%05d_t001.ent", ModelID);
	m_CharMainView.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_HEAD);

	sprintf(buf, "j%05d_t001.ent", ModelID);
	m_CharMainView.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_HAIR);

	sprintf(buf, "b%05d_t001.ent", ModelID);
	m_CharMainView.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_BODY);

	ResetNamePosition();
	//sprintf(buf, "k0000%d_t001.ent", ModelID);
	//m_CharMainView.LoadEntityFile(String(buf).c_str(), gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);
}

VOID CMapActorControl::LoadDefaultPart2()
{
	m_CharMainView.InitEntPicInfo();

	//real time setting
	Char FileName[50];
	GetPrivateProfileString(_T("LOADFILE"), _T("HEAD2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	m_CharMainView.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_HEAD);

	GetPrivateProfileString(_T("LOADFILE"), _T("BODY2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	m_CharMainView.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_BODY);

	GetPrivateProfileString(_T("LOADFILE"), _T("HAIR2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	m_CharMainView.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_HAIR);

	GetPrivateProfileString(_T("LOADFILE"), _T("EXTRA2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	m_CharMainView.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_HEAD_ACCESSORY);

	GetPrivateProfileString(_T("LOADFILE"), _T("WEAPON2"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	m_CharMainView.LoadEntityFile(FileName, gGlobal.pD3DDevice, EQUIP_WEAPON);

	
	ResetNamePosition();

}


VOID CMapActorControl::SetEntityCurrPos(UInt16 x, UInt16 y)
{
	m_CharMainView.SetCharPos(x, y);
	m_CharMainView.m_PathFindData.Init();
}

VOID CMapActorControl::SetEntityDesPos(UInt16 x, UInt16 y, BOOL SendToServer)
{
	if (m_CharMainView.GetFollowTarget())
		return;

	m_CharMainView.SetCharDesPos(x, y);
}

VOID CMapActorControl::Stop()
{
	m_CharMainView.StopEntity();
}

VOID CMapActorControl::OnUpdateInfo(const String & Name, BYTE FontFlag)
{
	StringA strTemp = gConfigure.strFontStyle.c_str();
	m_CharNameView.LoadText(Name.c_str(), 
		strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
			gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), FontFlag);
	m_CharNameView.SetEntParent(&m_CharMainView);
}

VOID CMapActorControl::OnUpdateCurPos()
{
	if (m_pMapActor)// && this != gGlobal.g_pCharacter)
	{
		BlockPos Des;
		Des.PosX = m_pMapActor->GetCurPosX();
		Des.PosY = m_pMapActor->GetCurPosY();

		UInt32 diff = QGLMath::CalculateBlockPosDistance(Des, m_CharMainView.GetCharPos(), PixelDirection());

//		DEBUG_INFODTL_7(GLOBAL_LOGGER, _F("OnUpdateCurPos: action %d,%d, view curPos %d,%d, actor curPos %d,%d, diff %d"), 
//			m_pMapActor->GetAction(), m_pMapActor->GetActionTime(),
//			m_CharMainView.GetCharPos().PosX, m_CharMainView.GetCharPos().PosY, 
//			m_pMapActor->GetCurPosX(), m_pMapActor->GetCurPosY(), diff);
		if (diff > gConfigure.uMinCurPosSync)
		{
			DEBUG_INFODTL_7(GLOBAL_LOGGER, _F("OnUpdateCurPos UnSync: action %d,%d, view curPos %d,%d, actor curPos %d,%d, diff %d"), 
				m_pMapActor->GetAction(), m_pMapActor->GetActionTime(),
				m_CharMainView.GetCharPos().PosX, m_CharMainView.GetCharPos().PosY, 
				m_pMapActor->GetCurPosX(), m_pMapActor->GetCurPosY(), diff);

			m_CharMainView.SetCharPos(m_pMapActor->GetCurPosX(), m_pMapActor->GetCurPosY());
			m_CharMainView.m_PathFindData.Init();
		}
	}
}

VOID CMapActorControl::OnUpdateDstPos()
{
	if (m_pMapActor && this != gGlobal.g_pCharacter)
	{
		m_CharMainView.SetCharDesPos(m_pMapActor->GetDstPosX(), m_pMapActor->GetDstPosY());

	}
}

VOID CMapActorControl::SetMapActorObject(CMapActor* pMapActor)
{
	m_pMapActor = pMapActor;
	
	if (pMapActor == NULL)
		return;
	
	pMapActor->SetControl(this);
	OnUpdateInfo(pMapActor->GetNickName(), EDGE_SHADOW);
	OnUpdateSpeed();
	OnUpdateFollowId(pMapActor->GetFollowId(), pMapActor->GetFollowOrder());
	OnUpdateAction(pMapActor->GetAction(), pMapActor->GetActionTime());
	SetEntityCurrPos(pMapActor->GetCurPosX(), pMapActor->GetCurPosY());
	if (m_pMapActor && m_pMapActor->IsUpdated(CMapActor::dstPosMask))
		OnUpdateDstPos();

//	OnUpdateDstPos();
}

VOID CMapActorControl::ResetStatus()
{
	m_CharMainView.m_Updated = FALSE;
	m_CharShadowView.m_Updated = FALSE;
	m_CharNameView.m_Updated = FALSE;
}

bool CMapActorControl::IsHide()
{
	if (gGlobal.g_HideOtherPlayers == 0)
		return false;

	if (gGlobal.g_pCharacter == this)
		return false;

	if ((gGlobal.g_pParty 
		&& gGlobal.g_pParty->FindMember(GetMapActorID()) != NULL)
		|| (gGlobal.g_pTempLeaveParty 
		&& gGlobal.g_pTempLeaveParty->FindMember(GetMapActorID())!=NULL))
	{
		return false;
	}
	if (gGlobal.g_HideOtherPlayers == 2)
	{
		if (IsLeader())
			return false;
	}

	return true;
}
VOID CMapActorControl::UpDateView(UInt32 CurrTime)
{
	//update anim idle status
	if (m_CharMainView.IsMount())
	{
		if (m_CharMainView.GetAnimationIndex() == ANIM_MOUNT_IDLE1)
		{
			if (CurrTime > m_AnimIdleChangeCounter + m_AnimIdleChangeTime)
			{
				if (m_CharMainView.IsAnimExist(ANIM_MOUNT_IDLE2))
					m_CharMainView.SetAnimationIndexByIndex(ANIM_MOUNT_IDLE2, FALSE);
				else
					m_AnimIdleChangeCounter += m_AnimIdleChangeTime;
			}
		}
		else
		{
			if (m_CharMainView.GetAnimationIndex() == ANIM_MOUNT_IDLE2 &&
				m_CharMainView.FrameEndNoOthers(CurrTime))
				m_CharMainView.SetAnimationIndexByIndex(ANIM_MOUNT_IDLE1, TRUE);
			m_AnimIdleChangeCounter = CurrTime;
		}
	}
	else
	{
		if (m_CharMainView.GetAnimationIndex() == ANIM_IDLE1)
		{
			if (CurrTime > m_AnimIdleChangeCounter + m_AnimIdleChangeTime)
			{
				if (m_CharMainView.IsAnimExist(ANIM_IDLE2))
					m_CharMainView.SetAnimationIndexByIndex(ANIM_IDLE2, FALSE);
				else
					m_AnimIdleChangeCounter += m_AnimIdleChangeTime;
			}
		}
		else
		{
			if (m_CharMainView.GetAnimationIndex() == ANIM_IDLE2 &&
				m_CharMainView.FrameEndNoOthers(CurrTime))
				m_CharMainView.SetAnimationIndexByIndex(ANIM_IDLE1, TRUE);
			m_AnimIdleChangeCounter = CurrTime;
		}
	}
	
	m_CharMainView.UpDate(CurrTime);
	m_CharShadowView.UpDate(CurrTime);
	m_CharNameView.UpDate(CurrTime);
	m_CharHeadIconView.UpDate(CurrTime);

	if (m_pMapActor)
	{
		if (!m_CharMainView.GetFollowTarget() && m_pMapActor->GetFollowId() != 0)
			OnUpdateFollowId(m_pMapActor->GetFollowId(), m_pMapActor->GetFollowOrder());
	}

	Int count = 0;
	for (StlVector<ChatBalloonData>::iterator Itr = m_ChatBalloonLst.begin();
		Itr != m_ChatBalloonLst.end(); ++Itr)
	{
		if ( CurrTime >= Itr->StartTime )
		{
			if (CurrTime - Itr->StartTime > ChatBalloonReleaseTime &&
				!Itr->pBalloon->IsMoving())
				count ++;
			else
				Itr->pBalloon->UpDate(CurrTime);
		}
	}


	for (Int i = 0; i < count; i++)
	{
		//SafeDeleteObject(RmLst[i]->pBalloon);
		if (QGLApplication::GetInstance().GetMapStateID()  == ST_APP_BATTLE ||
			!IsHide())
		{
			ParticleStructSetting ParSetting;
			ParSetting.Destory = TRUE;
			Int WaitTime = gConfigure.uChatWaitCount;
			ParSetting.ExtraZValue = NameExtraZ + 1;
			ParSetting.NoOfParticle = 1;
			ParSetting.Rotation = 0;
			ParSetting.Loop = FALSE;
			ParSetting.GivenDisposalAnim = 0;
			ParSetting.GivenDisposalTime = WaitTime;
			ParSetting.SubOffsetY = 0;
			ParSetting.StartTime = CurrTime;

			gGlobal.g_DisposalEntityEffect->AddUIEntityToSystem(m_ChatBalloonLst.begin()->pBalloon, ParSetting, 
				NULL, FALSE);
		}
		m_ChatBalloonLst.erase(m_ChatBalloonLst.begin());
		if (gGlobal.g_balloonCount > 0)
			gGlobal.g_balloonCount --;
	}

}

String CMapActorControl::GetCharName()
{
	if (m_pMapActor)
		return m_pMapActor->GetNickName();
	return String();
}

VOID CMapActorControl::RemoveFollowTarget()
{
	m_CharMainView.RemoveFollowTarget();
	if (m_pMapActor)
		m_CharMainView.SetCharPos(m_pMapActor->GetCurPosX(), m_pMapActor->GetCurPosY(), 1);
}

VOID CMapActorControl::FollowTarget(CMapActorControl * Target, UInt8 followOrder)
{
	if (Target && Target->m_pMapActor)
	{
		m_CharMainView.SetCharDesPos(Target->m_pMapActor->GetCurPosX(), 
			Target->m_pMapActor->GetCurPosY());
		if (followOrder == 0)
			Target->GetCharMainView()->AddFollower(&m_CharMainView);
		else
			Target->GetCharMainView()->InsertFollower(&m_CharMainView, followOrder - 1);
		Target->RefreshFollower();
	}
}

VOID CMapActorControl::OnUpdateSpeed()
{
	if (m_pMapActor)
		m_CharMainView.SetEntityMoveSpeed(m_pMapActor->GetSpeed());
}

VOID CMapActorControl::OnUpdateFollowId(const UInt32 &followId, const UInt8 &followOrder)
{
	if (followId == 0)
	{
		m_CharMainView.RemoveFollowTarget();
		if (m_pMapActor)
			m_CharMainView.SetCharPos(m_pMapActor->GetCurPosX(), m_pMapActor->GetCurPosY(), 1);
		return;
	}

	CMapActorControl * pMapActor = NULL;
	//if (FollowTargetClassID == Character_CLASSID)
	pMapActor = QGLApplication::GetInstance().FindCharCtrlByChar_id(followId);
	//else if (FollowTargetClassID == MapNPC_CLASSID)
	//	pMapActor = QGLApplication::GetInstance().FindNPCCtrlByChar_id(FollowTargetID);
	
	if (pMapActor)
	{
		FollowTarget(pMapActor, followOrder);

		if (!QGLApplication::GetInstance().CheckEntityFollower(GetCharMainView(), ::timeGetTime(), ::timeGetTime()))
		{
			GetCharMainView()->SetCharPos(pMapActor->GetCharMainView()->GetCharPos(), TRUE);
		}
		if (this == gGlobal.g_pCharacter)
		{
			QGLApplication::GetInstance().GetRenderCamera()->MoveToTargetImm();
		}
	}
	else
	{
		m_CharMainView.RemoveFollowTarget();
	}
}

VOID CMapActorControl::OnUpdateAppeared()
{
	//if (m_pMapActor)
	//	m_CharMainView.SetVisible(m_pMapActor->GetAppeared());
}

RPCNetID CMapActorControl::GetMapActorNetID()
{
	if (m_pMapActor)
		return m_pMapActor->GetNetId();

	return RPCNetID();
}

VOID CMapActorControl::LoadDefaultHeadIcon()
{
	m_CharHeadIconView.InitEntPicInfo();

	//real time setting
	Char FileName[50];
	GetPrivateProfileString(_T("LOADFILE"), _T("HEADICON"), _T(""), FileName, 50, CONFIGURATION_INI_FILE);
	m_CharHeadIconView.LoadEntityFile(FileName, gGlobal.pD3DDevice);
}

VOID CMapActorControl::RemoveIcon()
{
	m_CharHeadIconView.InitEntPicInfo();
}

VOID CMapActorControl::OnUpdateAction(const UInt16 &action, const UInt32 &actionTime)
{
	m_State = action;
}

VOID CMapActorControl::AddChatBalloon(CUIEntity * pBalloon, UInt32 delayTime)
{
	gGlobal.g_balloonCount ++;
	if (m_ChatBalloonLst.size() == MaxChatBalloons)
	{
		ParticleStructSetting ParSetting;
		ParSetting.Destory = TRUE;
		Int WaitTime = gConfigure.uChatWaitCount;
		ParSetting.ExtraZValue = NameExtraZ + 1;
		ParSetting.NoOfParticle = 1;
		ParSetting.Rotation = 0;
		ParSetting.Loop = FALSE;
		ParSetting.GivenDisposalAnim = 0;
		ParSetting.GivenDisposalTime = WaitTime;
		ParSetting.SubOffsetY = 0;

		gGlobal.g_DisposalEntityEffect->AddUIEntityToSystem(m_ChatBalloonLst.begin()->pBalloon, ParSetting, 
			NULL, FALSE); 
		//SafeDeleteObject(m_ChatBalloonLst.begin()->pBalloon);
		m_ChatBalloonLst.erase(m_ChatBalloonLst.begin());
	}
	
	Int Speed = gConfigure.uChatBalloonSpeed;
	pBalloon->SetEntityMoveSpeed(Speed);
	pBalloon->SetOffsetType(ENTITYOFFSETTYPE_HEAD);
	pBalloon->SetExtraZBuffer(NameExtraZ + 1);
	//pBalloon->SetEntOffsetY(pBalloon->GetHeight() / 2 + AboveHeadOffset);
	pBalloon->SetEntOffsetY((m_CharMainView.GetBattleIdleRect().bottom - m_CharMainView.GetBattleIdleRect().top) / 2
		+ AboveHeadOffset - pBalloon->GetHeight() / 2);
	pBalloon->SetEntityType(ENTITY_TYPE_PARTICLE_WITHOUTEFFECT);
	Int Diff = -1 * (Int)(m_CharMainView.GetBattleIdleRect().bottom - m_CharMainView.GetBattleIdleRect().top) / 2;
	pBalloon->SetJumpData(0, 0, Diff, abs(Diff) * 1000 / Speed, 0, TRUE);
		//SetEntDesOffsetY(-1 * (Int)pBalloon->GetHeight() / 2 + AboveHeadOffset / 2);
	pBalloon->SetEntParent(&m_CharMainView);
	ChatBalloonData Data;
	Data.pBalloon = pBalloon;
	Data.StartTime = ::timeGetTime() + delayTime;
	for (Int i = 0; i < (Int)m_ChatBalloonLst.size(); i++)
	{
		Diff = -1 * pBalloon->GetHeight();
		m_ChatBalloonLst[i].pBalloon->SetJumpData(0, 0, Diff, abs(Diff) * 1000 / Speed, 0, TRUE);
		//m_ChatBalloonLst[i].pBalloon->SetEntDesOffsetY(m_ChatBalloonLst[i].pBalloon->GetEntOffsetDesY() -
		//	pBalloon->GetHeight());
	}
	m_ChatBalloonLst.push_back(Data);

}

VOID CMapActorControl::ChangeRotation(Int Rotation)
{
	m_CharMainView.SetRotation(Rotation);
}

UInt32 CMapActorControl::GetMapActorID()
{
	return 0;
}

VOID CMapActorControl::SetFontColor(Color32 Color)
{
	m_OrgFontColor = Color; 
}

VOID CMapActorControl::SetFrontFontColor(Color32 Color)
{
	m_OrgFrontFontColor = Color;
}

VOID CMapActorControl::SetFrontName(const String & Name, BYTE FontFlag)
{
	StringA strTemp = gConfigure.strFontStyle.c_str();
	m_CharFrontNameView.LoadText(Name.c_str(), 
		strTemp.c_str(), gConfigure.uNameFontSize, D3DCOLOR_ARGB(255, gConfigure.uNameFontColorR,
		gConfigure.uNameFontColorG, gConfigure.uNameFontColorB), FontFlag);
	m_CharFrontNameView.SetEntParent(&m_CharMainView);
	if (!Name.IsEmpty())
	{
		m_CharNameView.SetEntOffsetY(gConfigure.NameOffsetY  + 18);
	}
	else
	{
		m_CharNameView.SetEntOffsetY(gConfigure.NameOffsetY);
	}
	
}

void CMapActorControl::SetHeadIcon(UInt8 iconType)
{
	if (iconType < MAX_HEADICONTYPE) {
		m_CharHeadIconView.InitEntPicInfo();
		m_CharHeadIconView.LoadEntityFile(gConfigure.HeadIconName[iconType].c_str(), 
			gGlobal.pD3DDevice);
		m_CharMainView.StopEntity();
	}
}

UInt8 CMapActorControl::GetSex()
{
	return (m_pMapActor ? m_pMapActor->GetSex() : 0);
}

UInt8 CMapActorControl::GetCclass()
{
	return (m_pMapActor ? m_pMapActor->GetCclass() : 0);
}

UInt8 CMapActorControl::GetSexClass()
{
	return (m_pMapActor ? m_pMapActor->GetSexClass() : 0);
}

UInt8 CMapActorControl::GetFaction()
{
	return (m_pMapActor ? m_pMapActor->GetFaction() : 0);
}

UInt16 CMapActorControl::GetMap_id()
{
	return (m_pMapActor ? m_pMapActor->GetMap_id() : 0);
}

UInt16 CMapActorControl::GetCurPosX()
{
	return m_CharMainView.GetCharPos().PosX;
}

UInt16 CMapActorControl::GetCurPosY()
{
	return m_CharMainView.GetCharPos().PosY;
}

UInt16 CMapActorControl::GetDstPosX()
{
	return m_CharMainView.GetCharDesPos().PosX;
}

UInt16 CMapActorControl::GetDstPosY()
{
	return m_CharMainView.GetCharDesPos().PosY;
}

UInt16 CMapActorControl::GetAction()
{
	return (m_pMapActor ? m_pMapActor->GetAction() : 0);
}

UInt32 CMapActorControl::GetSpeed()
{
	return (m_pMapActor ? m_pMapActor->GetSpeed() : 0);
}

UInt32 CMapActorControl::GetFollowId()
{
	return (m_pMapActor ? m_pMapActor->GetFollowId() : 0);
}
