//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "ParticleSystem.h"
//-- Library
#include "QGLApplication.h"
#include "AnimatedEntity.h"
#include "FontEntity.h"
#include "ImageEntity.h"
#include "UIEntity.h"

CParticleSystem::CParticleSystem()
{
	m_PreUpDateTimeX = 0;
	m_PreUpDateTimeY = 0;
	m_Speed = 0;
	m_pEntTarget = NULL;

}

CParticleSystem::~CParticleSystem()
{
	QGLApplication::GetInstance().RemoveParticleSystem(this);
	StlMap<CEntityBase *, ParticleStruct>::iterator PSItr = m_ParticleEntLst.begin();
	while (PSItr != m_ParticleEntLst.end())
	{
		SafeDeleteObject(PSItr->second.pEnt);
		PSItr++;
	}
	m_ParticleEntLst.clear();
}

VOID CParticleSystem::UpDate(UInt32 CurrentTime)
{
	if (m_PreUpDateTimeX > CurrentTime)
		m_PreUpDateTimeX = CurrentTime;

	if (m_PreUpDateTimeY > CurrentTime)
		m_PreUpDateTimeY = CurrentTime;

	UInt32 TimeDiffX = CurrentTime - m_PreUpDateTimeX;
	UInt32 TimeDiffY = CurrentTime - m_PreUpDateTimeY;

	if (m_pEntTarget)
	{
		m_EffectBounding.left = m_pEntTarget->GetEntPixelPos().PixelPosX + m_TargetRelativeBound.left - 
			(m_TargetRelativeBound.right - m_TargetRelativeBound.left) / 2;
		m_EffectBounding.top = m_pEntTarget->GetEntPixelPos().PixelPosY + m_TargetRelativeBound.top - 
			(m_TargetRelativeBound.bottom - m_TargetRelativeBound.top) / 2;
		m_EffectBounding.right = m_pEntTarget->GetEntPixelPos().PixelPosX + m_TargetRelativeBound.right - 
			(m_TargetRelativeBound.right - m_TargetRelativeBound.left) / 2;
		m_EffectBounding.bottom = m_pEntTarget->GetEntPixelPos().PixelPosY + m_TargetRelativeBound.bottom - 
			(m_TargetRelativeBound.bottom - m_TargetRelativeBound.top) / 2;
	}
	else
		m_EffectBounding = m_TargetRelativeBound;

	StlVector<StlMap<CEntityBase *, ParticleStruct>::iterator > PSItrRmLst;
	Int TotalMoveX = (Int)(m_Dir.PixelX * m_Speed * TimeDiffX / 1000);
	Int TotalMoveY = (Int)(m_Dir.PixelY * m_Speed * TimeDiffY / 1000);

	for (StlMap<CEntityBase *, ParticleStruct>::iterator PSItr = m_ParticleEntLst.begin();
		PSItr != m_ParticleEntLst.end(); PSItr ++)
	{
		Int MoveX = TotalMoveX;
		Int MoveY = TotalMoveY;

		BOOL Removed = FALSE;

		if (PSItr->second.StartTime > CurrentTime)
			continue;

		if (CurrentTime - PSItr->second.StartTime >= PSItr->second.LiveTime)
		{
			if (PSItr->second.ResetOutSideScreen)
				PSItr->second.StartTime = CurrentTime;
			else
			{
				if (PSItr->second.DisposalTime > 0 &&
					CurrentTime - PSItr->second.StartTime >= PSItr->second.LiveTime + PSItr->second.WaitTime)
				{
					FLOAT Percen = (FLOAT)(CurrentTime - PSItr->second.StartTime - PSItr->second.LiveTime
						- PSItr->second.WaitTime) / 
						PSItr->second.DisposalTime;
					if (Percen > 1)
						Percen = 1;

					if (PSItr->second.pEnt->GetAlphaOffset() != (Byte)(255 * Percen))
						PSItr->second.pEnt->m_Updated = TRUE;
					PSItr->second.pEnt->SetAlphaOffset((Byte)(255 * Percen));

				}

				BOOL FrameEnd = TRUE;
				UInt32 DisposalTime = PSItr->second.DisposalTime;
				UInt32 TimeDiff = CurrentTime - PSItr->second.StartTime;


				PSItr->second.pEnt->SetAnimationIndexByIndex(PSItr->second.DisposalAnim, 
					PSItr->second.pEnt->GetAnimationLoop(),
					PSItr->second.pEnt->GetAnimationRotation());
				FrameEnd = (PSItr->second.pEnt->FrameEnd(CurrentTime) || PSItr->second.pEnt->GetStopAnimation());

				if (FrameEnd && 
					CurrentTime - PSItr->second.StartTime >= PSItr->second.LiveTime + PSItr->second.DisposalTime
					+ PSItr->second.WaitTime)
				{
					if (PSItr->second.DestoryAfterFinish)
					{
						if (!Removed)
							PSItrRmLst.push_back(PSItr);
						Removed = TRUE;
					}
					else
					{
						
						PSItr->second.pEnt->SetAnimationIndexByIndex(ANIM_IDLE1, PSItr->second.pEnt->GetAnimationLoop());
						RanPickPos(PSItr->second.pEnt, PSItr->second.ResetOutSideScreen);
						PSItr->second.StartTime = CurrentTime;
						PSItr->second.pEnt->SetAlphaOffset(0);
					}
				}
				else
				{
					MoveX = 0;
					MoveY = 0;
				}
			}
		}
		while ((MoveX != 0 || MoveY != 0) && !Removed)
		{	
			if (MoveX != 0)
			{
				PSItr->second.pEnt->SetEntOffsetX( PSItr->second.pEnt->GetRelativeEntOffsetX() + MoveX);
				m_PreUpDateTimeX = CurrentTime;
				if ( OutSideBounding(PSItr->second.pEnt))
				{
					if ( PSItr->second.DestoryAfterFinish)
					{
						if (!Removed)
							PSItrRmLst.push_back(PSItr);
						Removed = TRUE;
					}
					else
					{
						RanPickPos( PSItr->second.pEnt, PSItr->second.ResetOutSideScreen);
						MoveX = (Int)(m_Dir.PixelX * m_Speed * (Real32)(TimeDiffX %  PSItr->second.LiveTime) / 1000);

					}
					PSItr->second.StartTime = CurrentTime;
				}
				else
					MoveX = 0;
			}
			if (MoveY != 0)
			{

				PSItr->second.pEnt->SetEntOffsetY( PSItr->second.pEnt->GetRelativeEntOffsetY() + MoveY);
				m_PreUpDateTimeY = CurrentTime;
				if (OutSideBounding(PSItr->second.pEnt))
				{
					if ( PSItr->second.DestoryAfterFinish)
					{
						if (!Removed)
							PSItrRmLst.push_back(PSItr);
						Removed = TRUE;
					}
					else
					{
						RanPickPos( PSItr->second.pEnt, PSItr->second.ResetOutSideScreen);

						MoveY = (Int)(m_Dir.PixelY * m_Speed * (Real32)(TimeDiffY %  PSItr->second.LiveTime) / 1000);
					}
					PSItr->second.StartTime = CurrentTime;
				}
				else
					MoveY = 0;
			}	
		}

		PSItr->second.pEnt->UpDate(CurrentTime);

	}

	//remove the entity in the particle list
	//Int Size = (Int)m_ParticleEntLst.size();
	for (Int i = 0; i < (Int)PSItrRmLst.size(); i++)
	{
		if (PSItrRmLst[i]->second.FreeAfterFinish)
			PSItrRmLst[i]->second.pEnt->FreeEntityData();
		SafeDeleteObject(PSItrRmLst[i]->second.pEnt);
		m_ParticleEntLst.erase(PSItrRmLst[i]);
	}
	//Int Size2 = (Int)m_ParticleEntLst.size();
	//Size2 ++;
}

BOOL CParticleSystem::OutSideBounding(CEntityBase * pEnt)
{
	RECT ExpandedBound = m_EffectBounding;

	if (m_Dir.PixelX < 0)
	{
		ExpandedBound.right = (LONG)(ExpandedBound.right - m_Dir.PixelX * (ExpandedBound.right - ExpandedBound.left));
	}
	else
	{
		ExpandedBound.left = (LONG)(ExpandedBound.left - m_Dir.PixelX * (ExpandedBound.right - ExpandedBound.left));
	}

	if (m_Dir.PixelY < 0)
	{
		ExpandedBound.bottom = (LONG)(ExpandedBound.bottom - m_Dir.PixelY * (ExpandedBound.bottom - ExpandedBound.top));
	}
	else
	{
		ExpandedBound.top = (LONG)(ExpandedBound.top - m_Dir.PixelY * (ExpandedBound.bottom - ExpandedBound.top));
	}

	if ( pEnt->GetEntOffsetY() + 
		(pEnt->m_EntOrgHeight) / 2 < ExpandedBound.top || 
		pEnt->GetEntOffsetY() - 
		(pEnt->m_EntOrgHeight) / 2 > ExpandedBound.bottom)
	{
		return TRUE;
	}

	if ( pEnt->GetEntOffsetX() + 
		(pEnt->m_EntOrgWidth) / 2 < ExpandedBound.left || 
		pEnt->GetEntOffsetX() - 
		(pEnt->m_EntOrgWidth) / 2 > ExpandedBound.right)
	{
		return TRUE;
	}

	return FALSE;
}
VOID CParticleSystem::RanPickPos(CEntityBase * pEnt, BOOL Outside)
{
	if (m_pEntTarget)
	{
		m_ParSysPos = m_pEntTarget->GetEntPixelPos();
	}


	if (m_EffectBounding.right - m_EffectBounding.left > 0)
	{
		Int PosX = rand() % (m_EffectBounding.right - m_EffectBounding.left ) + 
		m_ParSysPos.PixelPosX - (m_EffectBounding.right - 
		m_EffectBounding.left) / 2;

		if (Outside)
		{
			while (PosX >= m_ParSysPos.PixelPosX - QGLApplication::GetInstance().GetRenderClass()->GetWinWidth() / 2 &&
				PosX <= m_ParSysPos.PixelPosX + QGLApplication::GetInstance().GetRenderClass()->GetWinWidth() / 2)
			{
				PosX = rand() % (m_EffectBounding.right - m_EffectBounding.left ) + 
					m_ParSysPos.PixelPosX - (m_EffectBounding.right - 
					m_EffectBounding.left) / 2;
			}
		}

		pEnt->SetEntOffsetX(PosX);
	}

	if (m_EffectBounding.bottom - m_EffectBounding.top > 0)
	{
		Int PosY = rand() % ( m_EffectBounding.bottom - m_EffectBounding.top ) + 
		m_ParSysPos.PixelPosY - (m_EffectBounding.bottom -
		m_EffectBounding.top) / 2;

		if (Outside)
		{
			while (PosY >= m_ParSysPos.PixelPosX - QGLApplication::GetInstance().GetRenderClass()->GetWinHeight() / 2 &&
				PosY <= m_ParSysPos.PixelPosX + QGLApplication::GetInstance().GetRenderClass()->GetWinHeight() / 2)
			{
				PosY = rand() % ( m_EffectBounding.bottom - m_EffectBounding.top ) + 
					m_ParSysPos.PixelPosY - (m_EffectBounding.bottom -
					m_EffectBounding.top) / 2;
			}
		}

		pEnt->SetEntOffsetY(PosY);
	}
}

VOID CParticleSystem::SetBaseAttri(ParticleStruct & ParStr, ParticleStructSetting Setting, CEntityBase * pRefEntPos, BOOL SetParent)
{
	if (pRefEntPos)
	{
		if (SetParent)
		{
			ParStr.pEnt->SetEntParent(pRefEntPos);
		}
		else
		{
			ParStr.pEnt->SetCharPos(pRefEntPos->GetCharPos());
			ParStr.pEnt->SetEntPixelPos(pRefEntPos->GetEntPixelPos());
			ParStr.pEnt->SetEntOffsetX(pRefEntPos->GetEntOffsetX());
			ParStr.pEnt->SetEntOffsetJumpY(pRefEntPos->GetEntOffsetJumpY());
			Int EntityLeftUpPosY = 0;
			if (ParStr.pEnt->GetOffsetType() == ENTITYOFFSETTYPE_FOOT)
			{
				EntityLeftUpPosY = ParStr.pEnt->m_EntOrgHeight / 2 - AboveHeadOffset;
			}
			else if (ParStr.pEnt->GetOffsetType() == ENTITYOFFSETTYPE_CENTER)
			{
				RECT TotalRect = pRefEntPos->GetBattleIdleRect();
				EntityLeftUpPosY = (TotalRect.top + (TotalRect.bottom - TotalRect.top) / 2) -  ParStr.pEnt->m_EntOrgHeight / 2;
			}
			else if (ParStr.pEnt->GetOffsetType() == ENTITYOFFSETTYPE_HEAD)
			{
				RECT TotalRect = pRefEntPos->GetBattleIdleRect();
				EntityLeftUpPosY = TotalRect.bottom - AboveHeadOffset - ParStr.pEnt->m_EntOrgHeight / 2;
			}
			ParStr.pEnt->SetEntOffsetY(pRefEntPos->GetEntOffsetY() + EntityLeftUpPosY);
		}
	}
	else
		ParStr.pEnt->SetCharPos(0,0);

	ParStr.pEnt->StopEntity();

	ParStr.pEnt->RemoveAllChildren();
	ParStr.pEnt->SetExtraZBuffer(Setting.ExtraZValue);
	if (ParStr.pEnt->GetEntOffsetX() != 0)
	{
		int i = 0;
		i ++;
	}
	ParStr.pEnt->SetEntOffsetX(ParStr.pEnt->GetRelativeEntOffsetX() + Setting.SubOffsetX);
	ParStr.pEnt->SetEntOffsetY(ParStr.pEnt->GetRelativeEntOffsetY() + Setting.SubOffsetY);

	ParStr.pEnt->SetAlphaOffset(Setting.StartAlphaOffset);
	ParStr.DisposalTime = Setting.GivenDisposalTime;
	ParStr.DisposalAnim = Setting.GivenDisposalAnim;
	ParStr.WaitTime = Setting.GivenWaitTime;
	ParStr.DestoryAfterFinish = Setting.Destory;
	ParStr.ResetOutSideScreen = Setting.ResetOutSideScreen;
	ParStr.FreeAfterFinish = Setting.FreeAfterFinish;

	if (Setting.LiveTimeVar > 0)
		ParStr.LiveTime = Setting.LiveTimeBase + rand() % Setting.LiveTimeVar;
	else
		ParStr.LiveTime = Setting.LiveTimeBase;
	if (Setting.StartTime == 0)
		ParStr.StartTime = ::timeGetTime() + Setting.StartDelay;
	else
		ParStr.StartTime = Setting.StartTime + Setting.StartDelay;
}

VOID CParticleSystem::AddUIEntityToSystem(CUIEntity * pGivenEnt, ParticleStructSetting Setting, 
										CEntityBase * pRefEntPos, BOOL SetParent)
{
	if (pGivenEnt == NULL)
		return;

	m_Speed = Setting.MoveSpeed;
	m_Dir = Setting.MoveDir;
	m_PreUpDateTimeX = ::timeGetTime();
	m_PreUpDateTimeY = m_PreUpDateTimeX;
	ParticleStruct ParStr;
	ParStr.pEnt = pGivenEnt;

	SetBaseAttri(ParStr, Setting, pRefEntPos, SetParent);
	m_ParticleEntLst.insert(std::make_pair(ParStr.pEnt ,ParStr));
	
}

VOID CParticleSystem::AddFontEntityToSystem(CFontEntity * pGivenEnt, ParticleStructSetting Setting, 
										CEntityBase * pRefEntPos, BOOL SetParent)
{
	if (pGivenEnt == NULL)
		return;

	m_Speed = Setting.MoveSpeed;
	m_Dir = Setting.MoveDir;
	m_PreUpDateTimeX = ::timeGetTime();
	m_PreUpDateTimeY = m_PreUpDateTimeX;

	for (Int i = 0; i < Setting.NoOfParticle; i++)
	{
		ParticleStruct ParStr;

		CFontEntity * pTempFontEnt = SafeCreateObject(CFontEntity);
		*pTempFontEnt = *pGivenEnt;
		ParStr.pEnt = pTempFontEnt;

		SetBaseAttri(ParStr, Setting, pRefEntPos, SetParent);
		m_ParticleEntLst.insert(std::make_pair(ParStr.pEnt ,ParStr));
	}
}

VOID CParticleSystem::AddImageEntityToSystem(CImageEntity * pGivenEnt, ParticleStructSetting Setting, 
										CEntityBase * pRefEntPos, BOOL SetParent)
{
	if (pGivenEnt == NULL)
		return;

	m_Speed = Setting.MoveSpeed;
	m_Dir = Setting.MoveDir;
	m_PreUpDateTimeX = ::timeGetTime();
	m_PreUpDateTimeY = m_PreUpDateTimeX;
	
	for (Int i = 0; i < Setting.NoOfParticle; i++)
	{
		ParticleStruct ParStr;

		CImageEntity * pTempImEnt = SafeCreateObject(CImageEntity);
		pTempImEnt->SetOffsetType(pGivenEnt->GetOffsetType());
		pTempImEnt->SetImage(pGivenEnt->GetImage());
		ParStr.pEnt = pTempImEnt;

		SetBaseAttri(ParStr, Setting, pRefEntPos, SetParent);
		m_ParticleEntLst.insert(std::make_pair(ParStr.pEnt ,ParStr));
	}
}

VOID CParticleSystem::AddAnimatedEntityToSystem(CAnimatedEntity * pGivenEnt, ParticleStructSetting Setting, 
										CEntityBase * pRefEntPos, BOOL SetParent)
{
	if (pGivenEnt == NULL)
		return;

	m_Speed = Setting.MoveSpeed;
	m_Dir = Setting.MoveDir;
	m_PreUpDateTimeX = ::timeGetTime();
	m_PreUpDateTimeY = m_PreUpDateTimeX;
	
	for (Int i = 0; i < Setting.NoOfParticle; i++)
	{
		ParticleStruct ParStr;

		CAnimatedEntity * pAnimatedEnt = SafeCreateObject(CAnimatedEntity);
		*pAnimatedEnt =  *pGivenEnt;
		pAnimatedEnt->m_animationset.clear();
		pAnimatedEnt->SetRotation(Setting.Rotation);
		pAnimatedEnt->SetAnimationLoop(Setting.Loop);
		pAnimatedEnt->SetStopAnimation(Setting.Stop);
		if (Setting.HaveEffect)
			pAnimatedEnt->SetEntityType(ENTITY_TYPE_PARTICLE_WITHEFFECT);
		else
			pAnimatedEnt->SetEntityType(ENTITY_TYPE_PARTICLE_WITHOUTEFFECT);

		ParStr.pEnt = pAnimatedEnt;

		SetBaseAttri(ParStr, Setting, pRefEntPos, SetParent);
		m_ParticleEntLst.insert(std::make_pair(ParStr.pEnt ,ParStr));
	}
}

VOID CParticleSystem::Reset()
{
	m_PreUpDateTimeY = m_PreUpDateTimeX = ::timeGetTime();

	if (m_pEntTarget)
	{
		m_EffectBounding.left = m_pEntTarget->GetEntPixelPos().PixelPosX + m_TargetRelativeBound.left - 
			(m_TargetRelativeBound.right - m_TargetRelativeBound.left) / 2;
		m_EffectBounding.top = m_pEntTarget->GetEntPixelPos().PixelPosY + m_TargetRelativeBound.top - 
			(m_TargetRelativeBound.bottom - m_TargetRelativeBound.top) / 2;
		m_EffectBounding.right = m_pEntTarget->GetEntPixelPos().PixelPosX + m_TargetRelativeBound.right - 
			(m_TargetRelativeBound.right - m_TargetRelativeBound.left) / 2;
		m_EffectBounding.bottom = m_pEntTarget->GetEntPixelPos().PixelPosY + m_TargetRelativeBound.bottom - 
			(m_TargetRelativeBound.bottom - m_TargetRelativeBound.top) / 2;
	}
	else
		m_EffectBounding = m_TargetRelativeBound;

	for (StlMap<CEntityBase *, ParticleStruct>::iterator PSItr = m_ParticleEntLst.begin();
		PSItr != m_ParticleEntLst.end(); PSItr ++)
	{
		RanPickPos(PSItr->second.pEnt, FALSE);
		PSItr->second.StartTime = m_PreUpDateTimeY;
		CAnimatedEntity * pEnt = dynamic_cast<CAnimatedEntity*>(PSItr->second.pEnt);
		if (pEnt)
		{
			pEnt->SetAnimationIndexByIndex(ANIM_IDLE1);
		}
	}

}

VOID CParticleSystem::ClearLst()
{
	StlMap<CEntityBase *, ParticleStruct>::iterator PSItr = m_ParticleEntLst.begin();
	while (PSItr != m_ParticleEntLst.end())
	{
		SafeDeleteObject(PSItr->second.pEnt);
		PSItr++;
	}
	m_ParticleEntLst.clear();
}
