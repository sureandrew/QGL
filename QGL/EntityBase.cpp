//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "EntityBase.h"
//-- Library
#include "Math.h"
#include "Resources.h"
#include "Render.h"
#include "QGLApplication.h"
#include "PathMove.h"
#include "Global.h"

const Int DefaultSpeed = 150;
CEntityBase::CEntityBase()
{
	m_CharPos.PosX = 0;
	m_CharPos.PosY = 0;
	m_CurrTexture = NULL;
	m_Updated = FALSE;
	m_EntOrgWidth = 0;
	m_EntOrgHeight = 0;
	m_InitialMoveYSpeed = 0;
	m_OffsetJumpY = 0;
	m_OffsetY = 0;
	m_OffsetX = 0;
	m_StartTime = 0;
	m_CurrPartBlitType = 0;

	m_GravityAcc = -800;
	m_OffsetDesX = 0;
	m_OffsetDesY = 0;
	m_OverlappedRegion.clear();
	m_TotalTime = 0;
	m_AlphaOffset = 0;
	m_pParentEnt = NULL;
	m_ExtraZBuffer = 0;
	m_OrgPosX = 0;
	m_OrgPosY = 0;
	m_EntityMoveSpeed = DefaultSpeed;

	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_FootStepCount = 0;
	m_FootStepType = 0;

	m_pFollowTargetEnt = NULL;
	SetRect(&m_BattleTotalRect, -1, -1, -1, -1);

	m_Visible = TRUE;

	m_OffsetType = 0;

	//just assign the cache texture first
	m_CurrTexture = CResourcesManager::GetInstance().GetEntPicCacheTexture();
	m_CurrPicData = m_CurrTexture;
	m_OffsetDesX = 0;
	m_OffsetDesY = 0;
	m_EntityType = 0;
	
	m_ShakeX = 0;
	m_ShakeY = 0;
	m_ShakeSpeedX = 0;
	m_ShakeSpeedY = 0;
	m_ShakePreTimeX = 0;
	m_ShakePreTimeY = 0;
	m_ShakeWidth = 0;
	m_ShakeHeight = 0;
	m_PicWidth = MaxEntSizeX;

	m_HighLight = FALSE;
}


CEntityBase::~CEntityBase()
{	
	//parent and children nodes
	LeaveParent();

	EntityMap TempLst = m_pChildrenEntLst;

	EntityMap::iterator Itr = TempLst.begin();
	while (Itr != TempLst.end())
	{
		Itr->second->LeaveParent();
		Itr ++;
	}
	
	RemoveFollowTarget();
	RemoveAllFollowers();

	m_OverlappedRegion.clear();	
	QGLApplication::GetInstance().RemoveEntityInLst(this);
}

BOOL CEntityBase::IsMoving()
{
	if (m_CharPos != m_DesPos)
		return TRUE;

	if (m_StartTime > 0)
		return TRUE;

	if (m_OffsetDesX != m_OffsetX ||
		m_OffsetDesY != m_OffsetY)
		return TRUE;

	return FALSE;
}


BOOL CEntityBase::GetHaveEffect()
{ 
	return FALSE;
}

Int CEntityBase::GetTotalRenderPart()
{
	return 0;
}

Int CEntityBase::GetFlip(Int Index)
{
	return FALSE;
}

BOOL CEntityBase::GetIsMapObject()
{
	return FALSE;
}
VOID CEntityBase::SetJumpData(Int height, Int BkPosX, Int BkPosY, Real64 timeInMS, UInt32 StartDelay,
							  BOOL IsPixel)
{
	//if (m_OffsetX != m_OffsetDesX)
	//	m_OffsetX = m_OffsetDesX;

	//if (m_OffsetY != m_OffsetDesY)
	//	m_OffsetY = m_OffsetDesY;

	m_StartTime = ::timeGetTime() + StartDelay;
	m_TotalTime = timeInMS / 1000;
	m_GravityAcc = (Int)( -2 * height / (m_TotalTime /2 * m_TotalTime/2));
	m_InitialMoveYSpeed = (Int)(-1 * m_GravityAcc * m_TotalTime /2);
	
	m_OrgPosX = m_OffsetDesX;
	m_OrgPosY = m_OffsetDesY;
	//QGLMath::BlockPosToPixel(BkPosX, BkPosY, m_OffsetDesX, m_OffsetDesY);
	if (IsPixel)
	{
		m_OffsetDesX += BkPosX;
		m_OffsetDesY += BkPosY;
	}
	else
	{
		m_OffsetDesX += BkPosX * BlockSizeX;
		m_OffsetDesY += BkPosY * BlockSizeY / 2;
		if (BkPosY % 2 == 1)
			m_OffsetDesX += (Int)BlockSizeX / 2;
	}
	
}

VOID CEntityBase::SetCharDesPos(UInt16 x, UInt16 y)
{
	m_DesPos.PosX = x;
	m_DesPos.PosY = y;
	
	if (m_PathFindData.m_ptList.size() > 0)
	{
		BlockPos TempPos = m_PathFindData.m_ptList[0];
		m_PathFindData.Init();
		m_PathFindData.m_ptList.push_back(TempPos);
		
		//find out which direction to go
		BOOL Found = FALSE;
		Int Rotation = 0;
		for (Byte TempDir = SOUTH; TempDir < MAXROTATION; TempDir++)
		{
			Int TempMinX = m_CharPos.PosX;
			Int TempMinY = m_CharPos.PosY;
			QGLMath::MoveToBlock(TempMinX, TempMinY, TempDir, 1);
			if (TempMinX == TempPos.PosX && 
				TempMinY == TempPos.PosY)
			{
				Rotation = TempDir;
				Found = TRUE;
			}
		}

		if (!Found)
		{
			int i = 0;
			i ++;
		}

		PathFindData::PointList Temp;
		gGlobal.g_PathFinder->GetWholePath(TempPos.PosX, TempPos.PosY, x, y, Temp);

		for (Int i = 0; i < (Int)Temp.size(); i++)
		{
			m_PathFindData.m_ptList.push_back(Temp[i]);
		}
	}
	else
	{
		m_PathFindData.Init();
		gGlobal.g_PathFinder->GetWholePath(m_CharPos.PosX, m_CharPos.PosY, x, y, m_PathFindData.m_ptList);
	}

	
	
	
	
}

VOID CEntityBase::SetAlphaOffset(Byte Value, BOOL SetChildren)
{
	if (m_AlphaOffset != Value)
	{
		m_AlphaOffset = Value;
		m_Updated = TRUE;
	}

	if (SetChildren)
	{
		EntityMap::iterator Itr = m_pChildrenEntLst.begin();
		while (Itr != m_pChildrenEntLst.end())
		{
			Itr->second->SetAlphaOffset(Value);
			Itr++;
		}
	}
}


BOOL CEntityBase::GetEntityPart(size_t index, Int ImageType, RECT * LockRegion)
{
	return FALSE;
}

VOID CEntityBase::SetScaleX(Real32 Value){ 

		if (Value == m_ScaleX)
			return;

		if (Value > 1)
			m_ScaleX = 1.0f;
		else
			m_ScaleX = Value;
		m_Updated = TRUE;

}

VOID CEntityBase::SetScaleY(Real32 Value)
{ 

	if (Value == m_ScaleY)
		return;

	if (Value > 1)
		m_ScaleY = 1.0f;
	else
		m_ScaleY = Value;
	m_Updated = TRUE;
}

VOID CEntityBase::UpdateShaking(UInt32 CurrentTime)
{
	Real32 SpeedFactor = 1.0f;
	if (m_TotalShakeTime > 0 && CurrentTime > m_ShakeStartTimeSpeed)
	{
		SpeedFactor = 1.0f - (Real32)(CurrentTime - m_ShakeStartTimeSpeed) / m_TotalShakeTime;
	}

	if (SpeedFactor <= 0)
	{
		StopShake();
		SpeedFactor = 0;
		m_Updated = TRUE;
		return;
	}
	
	if (m_ShakeWidth > 0)
	{
		/*if (m_ShakeX >= (Int)m_ShakeWidth)
			m_ShakeSpeedX = -1 * abs(m_ShakeSpeedX);
		else if (m_ShakeX <= -1 * (Int)m_ShakeWidth)
			m_ShakeSpeedX = abs(m_ShakeSpeedX);
			*/

		if (CurrentTime > m_ShakePreTimeX)
		{
			Int FinalSpeed = (Int)(m_ShakeSpeedX * SpeedFactor);
			if (FinalSpeed == 0)
				FinalSpeed = 1;
			Int DiffX = (Int)(CurrentTime - m_ShakePreTimeX) * FinalSpeed / 1000;
			if (DiffX != 0)
			{
				//return to current position
				if (SpeedFactor == 0)
				{
					if ((m_ShakeX > 0 && m_ShakeX + DiffX <= 0) ||
						(m_ShakeX < 0 && m_ShakeX + DiffX >= 0))
					{
						m_ShakeX = 0;
						m_ShakeWidth = 0;
						m_Updated = TRUE;
						DiffX = 0;
					}
				}
				m_ShakeX += DiffX;
				//m_ShakeX *= -1;
				if (m_ShakeX > (Int)m_ShakeWidth)
				{
					Int Q = (m_ShakeX - m_ShakeWidth) / (m_ShakeWidth * 2);
					if (Q % 2 == 0)
					{
						m_ShakeX = (Int)m_ShakeWidth - (m_ShakeX - m_ShakeWidth) % (m_ShakeWidth * 2);
						m_ShakeSpeedX = -1 * abs(m_ShakeSpeedX);
					}
					else
					{
						m_ShakeX = (m_ShakeX - (Int)m_ShakeWidth) % (m_ShakeWidth * 2);
						m_ShakeSpeedX = abs(m_ShakeSpeedX);
					}
				}
				else if (m_ShakeX < -1 * (Int)m_ShakeWidth)
				{
					Int Q = (m_ShakeX + (Int)m_ShakeWidth) / (m_ShakeWidth * 2);
					if (Q % 2 == 0)
					{
						m_ShakeX = (abs((m_ShakeX + (Int)m_ShakeWidth)) % (m_ShakeWidth * 2)) - m_ShakeWidth;
						m_ShakeSpeedX = abs(m_ShakeSpeedX);
					}
					else
					{
						m_ShakeX = (Int)m_ShakeWidth - (m_ShakeX + m_ShakeWidth) % (m_ShakeWidth * 2);
						m_ShakeSpeedX = -1 * abs(m_ShakeSpeedX);
					}
				}
					
				m_Updated = TRUE;

				m_ShakePreTimeX = CurrentTime;
			}
		}
		
	}

	if (m_ShakeHeight > 0)
	{
		if (CurrentTime > m_ShakePreTimeY)
		{
			Int FinalSpeed = (Int)(m_ShakeSpeedY * SpeedFactor);
			if (FinalSpeed == 0)
				FinalSpeed = 1;
			Int DiffY = (Int)(CurrentTime - m_ShakePreTimeY) * FinalSpeed/ 1000;
			if (DiffY != 0)
			{
				//return to current position
				if (SpeedFactor == 0)
				{
					if ((m_ShakeY > 0 && m_ShakeY + DiffY <= 0) ||
						(m_ShakeY < 0 && m_ShakeY + DiffY >= 0))
					{
						m_ShakeY = 0;
						m_ShakeHeight = 0;
						m_Updated = TRUE;
						DiffY = 0;
					}
				}
				m_ShakeY += DiffY;
				//m_ShakeX *= -1;
				if (m_ShakeY > (Int)m_ShakeHeight)
				{
					Int Q = (m_ShakeY - m_ShakeHeight) / (m_ShakeHeight * 2);
					if (Q % 2 == 0)
					{
						m_ShakeY = (Int)m_ShakeHeight - (m_ShakeY - m_ShakeHeight) % (m_ShakeHeight * 2);
						m_ShakeSpeedY = -1 * abs(m_ShakeSpeedY);
					}
					else
					{
						m_ShakeY = (m_ShakeY - m_ShakeHeight) % (m_ShakeHeight * 2);
						m_ShakeSpeedY = abs(m_ShakeSpeedY);
					}
				}
				else if (m_ShakeY < -1 * (Int)m_ShakeHeight)
				{
					Int Q = (m_ShakeY + m_ShakeHeight) / (m_ShakeHeight * 2);
					if (Q % 2 == 0)
					{
						m_ShakeY = (m_ShakeY + m_ShakeHeight) % (m_ShakeHeight * 2);
						m_ShakeSpeedY = abs(m_ShakeSpeedY);
					}
					else
					{
						m_ShakeY = (Int)m_ShakeHeight - (m_ShakeY + m_ShakeHeight) % (m_ShakeHeight * 2);
						m_ShakeSpeedY = -1 * abs(m_ShakeSpeedY);
					}
				}
					
				m_Updated = TRUE;

				m_ShakePreTimeY = CurrentTime;
			}
		}

	}
	
}

VOID CEntityBase::UpDate(UInt32 CurrentTime)
{
	UpdateEntityMoveOffset(CurrentTime);
	
	//update Shakeing
	UpdateShaking(CurrentTime);
}

VOID CEntityBase::SetCharPos(UInt16 x, UInt16 y, BOOL SetDes)
{
	if (m_CharPos.PosX != x || m_CharPos.PosY != y)
	{
		m_CharPos.PosX = x; 
		m_CharPos.PosY = y;
		m_EntPixelPos.PixelPosX = (UInt32)(m_CharPos.PosX * BlockSizeX + BlockSizeX / 2);
		m_EntPixelPos.PixelPosY = (UInt32)(m_CharPos.PosY * BlockSizeY/2 + BlockSizeY / 2);
		if (m_CharPos.PosY % 2 == 1)
			m_EntPixelPos.PixelPosX += (Int)BlockSizeX / 2;
		if (SetDes)
		{
			m_DesPos = m_CharPos;
		}
		m_Updated = TRUE;

		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
		m_PreMoveTime = ::timeGetTime();
		//m_PathFindData.Init();
	}
}

VOID CEntityBase::SetCharPos(BlockPos GivenPos, BOOL SetDes){

	if (m_CharPos != GivenPos)
	{
		m_CharPos = GivenPos; 
		m_EntPixelPos.PixelPosX = (UInt32)(m_CharPos.PosX * BlockSizeX + BlockSizeX / 2);
		m_EntPixelPos.PixelPosY = (UInt32)(m_CharPos.PosY * BlockSizeY/2 + BlockSizeY / 2);
		if (m_CharPos.PosY % 2 == 1)
			m_EntPixelPos.PixelPosX += (Int)BlockSizeX / 2;

		if (SetDes)
		{
			m_DesPos = m_CharPos;
		}

		m_Updated = TRUE;
		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
		//m_PathFindData.Init();
	}
}

//remove child from this entity

VOID CEntityBase::RemoveChild(CEntityBase * pEnt)
{
	EntityMap::iterator pEntItr = m_pChildrenEntLst.find((UInt)pEnt);

	if (pEntItr != m_pChildrenEntLst.end())
	{
		m_pChildrenEntLst.erase(pEntItr);
		pEnt->m_pParentEnt = NULL;
	}
}

//add child to this entity
VOID CEntityBase::AddChild(CEntityBase * pEnt)
{
	EntityMap::iterator pEntItr = m_pChildrenEntLst.find((UInt)pEnt);

	if (pEntItr == m_pChildrenEntLst.end())
	{
		m_pChildrenEntLst.insert(std::make_pair((UInt)pEnt, pEnt));
		pEnt->m_pParentEnt = this;
	}
}

//remove the parent 
VOID CEntityBase::LeaveParent()
{
	if (m_pParentEnt)
		m_pParentEnt->RemoveChild(this);

	m_pParentEnt = NULL;
}

//set the parent entity class
VOID CEntityBase::SetEntParent(CEntityBase* pEnt)
{
	if (pEnt == NULL || pEnt == this)
		return;

	pEnt->AddChild(this);

}

VOID CEntityBase::SetEntPixelPos(PixelPos GivenPos)
{
	PixelPos OrgPos = m_EntPixelPos;
	if (m_pParentEnt)
		m_EntPixelPos = GivenPos - m_pParentEnt->GetEntPixelPos();
	else
		m_EntPixelPos = GivenPos;

	//Set char pos
	/*if (m_EntPixelPos.PixelPosX % BlockSizeX == 0 && 
		m_EntPixelPos.PixelPosX % (BlockSizeY / 2) == 0)
	{
		m_CharPos.PosX = m_EntPixelPos.PixelPosX / BlockSizeX;
		m_CharPos.PosY = m_EntPixelPos.PixelPosY / (BlockSizeY / 2);
	}
	*/

	//movement made
	if (OrgPos != m_EntPixelPos)
	{
		m_Updated = TRUE;
		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}
}

VOID CEntityBase::SetEntPixelPos(Int X, Int Y)
{
	PixelPos OrgPos = m_EntPixelPos;
	if (m_pParentEnt)
	{
		m_EntPixelPos.PixelPosX = X - m_pParentEnt->GetEntPixelPos().PixelPosX;
		m_EntPixelPos.PixelPosY = Y - m_pParentEnt->GetEntPixelPos().PixelPosY;
	}
	else
	{
		m_EntPixelPos.PixelPosX = X;
		m_EntPixelPos.PixelPosY = Y;
	}

	//movement made
	if (OrgPos != m_EntPixelPos)
	{
		m_Updated = TRUE;
		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}
}

//set the value for the position of the entity
VOID CEntityBase::SetRelativeEntPixelPos(PixelPos GivenPos)
{
	//movement made
	if (GivenPos != m_EntPixelPos)
	{
		m_Updated = TRUE;
		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}
	
	m_EntPixelPos = GivenPos;
}

VOID CEntityBase::SetRelativeEntPixelPos(Int X, Int Y)
{
	//movement made
	if (m_EntPixelPos.PixelPosX != X ||
		m_EntPixelPos.PixelPosY != Y)
	{
		m_Updated = TRUE;
		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
	}
	m_EntPixelPos.PixelPosX = X;
	m_EntPixelPos.PixelPosY = Y;
}

//getter and setter for offset position
Int CEntityBase::GetEntOffsetX()
{
	if (m_pParentEnt)
		return m_OffsetX + m_pParentEnt->GetEntOffsetX();
	return m_OffsetX;
}
Int CEntityBase::GetEntOffsetY()
{
	if (m_pParentEnt)
		return m_OffsetY + m_pParentEnt->GetEntOffsetY();
	return m_OffsetY;
}

Int CEntityBase::GetEntOffsetDesX()
{
	if (m_pParentEnt)
		return m_OffsetDesX + m_pParentEnt->GetEntOffsetDesX();
	return m_OffsetDesX;
}
Int CEntityBase::GetEntOffsetDesY()
{
	if (m_pParentEnt)
		return m_OffsetDesY + m_pParentEnt->GetEntOffsetDesY();
	return m_OffsetDesY;
}

VOID CEntityBase::SetEntOffsetX(Int Value, BOOL SetDes)
{
	if (SetDes)
		m_OffsetDesX = Value;

	if (m_OffsetX != Value)
	{
		m_Updated = TRUE;
		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
		m_OffsetX = Value;
	}
}
VOID CEntityBase::SetEntOffsetY(Int Value, BOOL SetDes)
{
	if (SetDes)
		m_OffsetDesY = Value;

	if (m_OffsetY != Value)
	{
		m_Updated = TRUE;
		//check for all children to update
		EntityMap::iterator pEntItr = m_pChildrenEntLst.begin();

		while (pEntItr != m_pChildrenEntLst.end())
		{
			CEntityBase * pEnt = pEntItr->second;
			pEntItr->second->m_Updated = TRUE;
			pEntItr++;
		}
		m_OffsetY = Value;
	}
}

VOID CEntityBase::SetEntDesOffsetX(Int Value)
{
	m_OffsetDesX = Value;
	m_OffsetMoveXTime = ::timeGetTime();
}
VOID CEntityBase::SetEntDesOffsetY(Int Value)
{
	m_OffsetDesY = Value;
	m_OffsetMoveYTime = ::timeGetTime();
}
VOID CEntityBase::SetEntOffsetJumpY(Int Value)
{
	if (m_OffsetJumpY != Value)
	{
		m_OffsetJumpY = Value;
		m_Updated = TRUE;
	}
}

VOID CEntityBase::StopEntity()
{
	if(m_PathFindData.m_ptList.size() > 0)
	{
		SetCharDesPos(m_PathFindData.m_ptList[0].PosX,m_PathFindData.m_ptList[0].PosY);
	}
	else
		SetCharDesPos(m_CharPos.PosX,m_CharPos.PosY);
}

VOID CEntityBase::SetFollowTarget(CEntityBase * pEntBase)
{
	if (m_pFollowTargetEnt)
		m_pFollowTargetEnt->RemoveFollower(this);

	m_pFollowTargetEnt = pEntBase;
	SetCharPos(pEntBase->GetCharPos(), TRUE);
}

CEntityBase * CEntityBase::GetFollowTarget()
{
	return m_pFollowTargetEnt;
}

StlVector<CEntityBase *> CEntityBase::GetFollowerLst()
{
	return m_pFollowerEntLst;
}

Int CEntityBase::GetFollowerLstIndex(CEntityBase * pEntBase)
{
	Int i = 0;
	for (StlVector<CEntityBase *>::iterator Itr = m_pFollowerEntLst.begin(); Itr != m_pFollowerEntLst.end(); ++Itr)
	{
		if (pEntBase == *Itr)
			return i;
		i ++;
	}
	return -1;
}

VOID CEntityBase::AddFollower(CEntityBase * pEntBase)
{
	if (pEntBase == NULL)
		return;

	if (pEntBase == this)
		return;

	for (StlVector<CEntityBase *>::iterator Itr = m_pFollowerEntLst.begin(); Itr != m_pFollowerEntLst.end(); ++Itr)
	{
		if (pEntBase == *Itr)
			return;
	}

	m_pFollowerEntLst.push_back(pEntBase);
	pEntBase->SetFollowTarget(this);
	//QGLApplication::GetInstance().CheckEntityFollower(pEntBase, ::timeGetTime());

}

VOID CEntityBase::InsertFollower(CEntityBase * pEntBase, int index)
{
	if (pEntBase == NULL)
		return;

	if (pEntBase == this)
		return;

	for (StlVector<CEntityBase *>::iterator Itr = m_pFollowerEntLst.begin(); Itr != m_pFollowerEntLst.end(); ++Itr)
	{
		if (pEntBase == *Itr)
			return;
	}
	if (index <= (int) m_pFollowerEntLst.size())
		m_pFollowerEntLst.insert(m_pFollowerEntLst.begin() + index, pEntBase);
	else
		m_pFollowerEntLst.push_back(pEntBase);

	pEntBase->SetFollowTarget(this);
	//QGLApplication::GetInstance().CheckEntityFollower(pEntBase, ::timeGetTime());
}

VOID CEntityBase::RemoveFollowTarget()
{
	if (m_pFollowTargetEnt)
		m_pFollowTargetEnt->RemoveFollower(this);

	m_PathFindData.Init();
	SetCharPos(GetCharPos(), TRUE);
	m_pFollowTargetEnt = NULL;
}

VOID CEntityBase::RemoveFollower(CEntityBase * pEntBase)
{
	for (StlVector<CEntityBase *>::iterator Itr = m_pFollowerEntLst.begin(); Itr != m_pFollowerEntLst.end(); ++Itr)
	{
		if (pEntBase == *Itr)
		{
			pEntBase->m_pFollowTargetEnt = NULL;
			m_pFollowerEntLst.erase(Itr);
			break;
		}
	}
}

VOID CEntityBase::RemoveAllFollowers()
{
	if (m_pFollowerEntLst.size() > 0)
	{
		for (StlVector<CEntityBase *>::iterator Itr = m_pFollowerEntLst.begin(); Itr != m_pFollowerEntLst.end(); ++Itr)
		{
			(*Itr)->m_pFollowTargetEnt = NULL;
		}
		m_pFollowerEntLst.clear();
	}
}

VOID CEntityBase::SetVisible(BOOL visible, BOOL SetChildren)
{
	if (m_Visible != visible)
	{
		m_Visible = visible;
		if (SetChildren)
		{
			EntityMap::iterator Itr = m_pChildrenEntLst.begin();
			while (Itr != m_pChildrenEntLst.end())
			{
				Itr->second->SetVisible(visible);
				Itr++;
			}
		}
		m_Updated = TRUE;
	}

}

BlockPos CEntityBase::GetPreStep(Int Index)
{
	if (Index < (Int)m_PathFindData.PreSteps.size() && Index >= 0)
		return m_PathFindData.PreSteps[Index];
	return m_CharPos;
}

VOID CEntityBase::UpdateEntityMoveOffset(UInt32 CurrTime)
{
	if (m_StartTime > 0)
	{
		Int OffsetX = m_OffsetX;
		Int OffsetY = m_OffsetY;
		Int OffsetJumpY = m_OffsetJumpY;
		CPathMove::UpDateMoveOffsetData(CurrTime, m_StartTime,OffsetX, OffsetY, OffsetJumpY, m_InitialMoveYSpeed,
			m_GravityAcc, m_OffsetDesX, m_OffsetDesY, m_TotalTime, m_OrgPosX, m_OrgPosY);
		SetEntOffsetX(OffsetX, FALSE);
		SetEntOffsetY(OffsetY, FALSE);
		SetEntOffsetJumpY(OffsetJumpY);
	}
}

UInt32 CEntityBase::GetZValue(UInt32 MapWidth)
{
	if (m_EntityType == ENTITY_TYPE_PARTICLE_WITHEFFECT || 
		m_EntityType == ENTITY_TYPE_PARTICLE_WITHOUTEFFECT)
	{
		Int64 ZValue = (GetEntPixelPos().PixelPosY ) * MapWidth + 
			GetEntPixelPos().PixelPosX + GetExtraZBuffer();
		if (ZValue < 0)
			return 0;
		else
			return (UInt32)ZValue;
	}
	else
	{
		Int64 ZValue = (GetEntPixelPos().PixelPosY + GetEntOffsetY()) * MapWidth + 
			GetEntPixelPos().PixelPosX + GetEntOffsetX()+ GetExtraZBuffer();
		if (ZValue < 0)
			return 0;
		else
			return (UInt32)ZValue;
	}
}

VOID CEntityBase::SetEntityType(Byte Value)
{
	if (Value == ENTITY_TYPE_MAP_OBJECT)
	{
		m_CurrTexture = CResourcesManager::GetInstance().GetObjEntPicCacheTexture();
	}
	m_EntityType = Value;
}

BYTE CEntityBase::GetEquipType(Int Index)
{
	return 255;
}

VOID CEntityBase::SetShakeInfo(Int ShakeX, Int ShakeY, UInt ShakeSpeedX,UInt ShakeSpeedY, 
							   UInt ShakeWidth, UInt ShakeHeight, UInt32 TotalShakeTime)
{
	m_ShakeX = ShakeX;
	m_ShakeY = ShakeY;
	m_ShakeSpeedX = ShakeSpeedX;
	m_ShakeSpeedY = ShakeSpeedY;
	m_ShakePreTimeX = ::timeGetTime();
	m_ShakePreTimeY = m_ShakePreTimeX;
	m_ShakeStartTimeSpeed = m_ShakePreTimeX;
	m_ShakeWidth = ShakeWidth;
	m_ShakeHeight = ShakeHeight;
	m_TotalShakeTime = TotalShakeTime;

}

VOID CEntityBase::StopShake()
{
	m_ShakeX = 0;
	m_ShakeY = 0;
	m_ShakeSpeedX = 0;
	m_ShakeSpeedY = 0;
	m_ShakePreTimeX = 0;
	m_ShakePreTimeY = 0;
	m_ShakeWidth = 0;
	m_ShakeHeight = 0;
}

VOID CEntityBase::SetAnimationIndexByIndex(Int Index, BOOL Loop, Int GivenRotation, BOOL flush)
{
}

//set the rotation of the entity
VOID CEntityBase::SetRotation(Int Rotation)
{
}

//check whether it is the end of the animation
BOOL CEntityBase::FrameEnd(UInt32 CurrentTime)
{
	return TRUE;
}

RECT CEntityBase::GetBattleIdleRect()
{ 
	return m_BattleTotalRect;
}

VOID CEntityBase::CopyPos(CEntityBase * pRef)
{
	if (pRef)
	{
		SetCharPos(pRef->GetCharPos());
		SetEntOffsetX(pRef->GetEntOffsetX());
		SetEntOffsetY(pRef->GetEntOffsetY());
		SetEntPixelPos(pRef->GetEntPixelPos());
	}

}
