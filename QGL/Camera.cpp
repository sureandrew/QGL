//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "Camera.h"
//-- Library
#include "Math.h"
#include "QGLApplication.h"
#include "Configure.h"

CCamera::CCamera()
{
	m_LeftUpCurrPos.x = 0;
	m_LeftUpCurrPos.y = 0;
	m_LeftUpCurrPos.z = 0;

	m_PosMoveOffset.x = 0;
	m_PosMoveOffset.y = 0;
	m_PosMoveOffset.z = 0;

	m_PreMoveTime = ::timeGetTime();

	m_Moved = FALSE;

	m_pEntTarget = NULL;
}

CCamera::~CCamera()
{
}
VOID CCamera::SetCamerLeftUpPos(Int PosX, Int PosY)
{
	D3DXVECTOR3 TargetLeftUpPos;
	TargetLeftUpPos.x =   (Real32)(PosX) - m_CurrWinWidth / 2 ;
	TargetLeftUpPos.y =   (Real32)(PosY) - m_CurrWinHeight / 2 ;
	TargetLeftUpPos.z = 0;
	
	if (TargetLeftUpPos.x < 0)
	{	
		m_LeftUpCurrPos.x = 0;
	}
	else if (m_MapWidth > 0 && TargetLeftUpPos.x + m_CurrWinWidth > m_MapWidth)
	{
		m_LeftUpCurrPos.x = (Real32)(m_MapWidth - m_CurrWinWidth);
	}
	else
		m_LeftUpCurrPos.x = TargetLeftUpPos.x;



	if (TargetLeftUpPos.y < 0)
	{
		m_LeftUpCurrPos.y = 0;
	}
	else if (m_MapHeight > 0 && TargetLeftUpPos.y + m_CurrWinHeight > m_MapHeight)
	{

		m_LeftUpCurrPos.y = (Real32)(m_MapHeight - m_CurrWinHeight);
	}
	else
		m_LeftUpCurrPos.y = TargetLeftUpPos.y;
}
VOID CCamera::SetCamerLeftUpPos(PixelPos GivenPixelPos)
{
	D3DXVECTOR3 TargetLeftUpPos;
	TargetLeftUpPos.x =   (Real32)(GivenPixelPos.PixelPosX) - m_CurrWinWidth / 2 ;
	TargetLeftUpPos.y =   (Real32)(GivenPixelPos.PixelPosY) - m_CurrWinHeight / 2 ;
	TargetLeftUpPos.z = 0;
	
	if (TargetLeftUpPos.x < 0)
	{	
		m_LeftUpCurrPos.x = 0;
	}
	else if (m_MapWidth > 0 && TargetLeftUpPos.x + m_CurrWinWidth > m_MapWidth)
	{
		m_LeftUpCurrPos.x = (Real32)(m_MapWidth - m_CurrWinWidth);
	}
	else
		m_LeftUpCurrPos.x = TargetLeftUpPos.x;



	if (TargetLeftUpPos.y < 0)
	{
		m_LeftUpCurrPos.y = 0;
	}
	else if (m_MapHeight > 0 && TargetLeftUpPos.y + m_CurrWinHeight > m_MapHeight)
	{

		m_LeftUpCurrPos.y = (Real32)(m_MapHeight - m_CurrWinHeight);
	}
	else
		m_LeftUpCurrPos.y = TargetLeftUpPos.y;

}

VOID CCamera::UpDateCam(UInt32 CurrTime)
{ 
	if (m_pEntTarget == NULL)
	{
		m_PreMoveTime = CurrTime;
		return;
	}

	m_Moved = FALSE;

	//update the camera position
	D3DXVECTOR3 TargetLeftUpPos;
	TargetLeftUpPos.x =   (Real32)(m_pEntTarget->m_EntPixelPos.PixelPosX) - m_CurrWinWidth / 2;
	TargetLeftUpPos.y =   (Real32)(m_pEntTarget->m_EntPixelPos.PixelPosY) - m_CurrWinHeight / 2;

	D3DXVECTOR3 TargetLeftUpCurrPos = TargetLeftUpPos;
	
	if (TargetLeftUpPos.x < 0)
	{
		TargetLeftUpCurrPos.x = 0;
	}
	else if (TargetLeftUpPos.x + m_CurrWinWidth > m_MapWidth && m_MapWidth > 0)
	{
		TargetLeftUpCurrPos.x = (Real32)(m_MapWidth - m_CurrWinWidth);
	}

	if (TargetLeftUpPos.y < 0)
	{
		TargetLeftUpCurrPos.y = 0;
	}
	else if (TargetLeftUpPos.y + m_CurrWinHeight > m_MapHeight && m_MapHeight > 0)
	{
		TargetLeftUpCurrPos.y = (Real32)(m_MapHeight - m_CurrWinHeight);
	}

	/*Int TotalDiffX = (Int)(TargetLeftUpCurrPos.x - m_LeftUpCurrPos.x);
	Int TotalDiffY = (Int)(TargetLeftUpCurrPos.y - m_LeftUpCurrPos.y);
	if (TotalDiffX == 0 && TotalDiffY == 0)
	{
		m_PreMoveTime = CurrTime;
		return;
	}*/

	//slowly move the camera to prevent shaking of the camera
	Int CameSpeed = (Int)(abs(m_pEntTarget->GetEntityMoveSpeed()));
	PixelDirection dir;
	UInt32 TotalDiff = QGLMath::CalculatePixelPosDistance(PixelPos((Int)m_LeftUpCurrPos.x, (Int)m_LeftUpCurrPos.y), 
		PixelPos((Int)TargetLeftUpCurrPos.x, (Int)TargetLeftUpCurrPos.y), dir);

	if (TotalDiff >= gConfigure.uCameraMaxDiff)
		MoveToTargetImm();
	else if (TotalDiff >= gConfigure.uCamUpDateMin && (m_pEntTarget->IsMoving() ||
		(m_pEntTarget->GetFollowTarget() && m_pEntTarget->GetFollowTarget()->IsMoving())))
	{
		Real32 MoveFactor = (Real32)(TotalDiff - gConfigure.uCamUpDateMin) / (Real32)TotalDiff;
		m_PosMoveOffset.x += (TargetLeftUpCurrPos.x - m_LeftUpCurrPos.x) * MoveFactor;
		m_PosMoveOffset.y += (TargetLeftUpCurrPos.y - m_LeftUpCurrPos.y) * MoveFactor;
		m_LeftUpCurrPos.x += (Int)m_PosMoveOffset.x;
		m_LeftUpCurrPos.y += (Int)m_PosMoveOffset.y;
		m_PreMoveTime = CurrTime;
	}
	else
	{
		CameSpeed = (Int)(CameSpeed * (Real32)gConfigure.uCamSpeedFactor / 100);

		//UInt32 TotalDiff = QGLMath::CalculatePixelPosDistance(CurrPixelPos2, DesPixelPos, dir);
		UInt32 TimeNeeded = 0;
		if (CameSpeed == 0)
			TimeNeeded = 0xFFFFFFFF;
		else
			TimeNeeded = (TotalDiff) * 1000 /  abs(CameSpeed);

		if (CurrTime - m_PreMoveTime >= TimeNeeded)
		{
			m_PosMoveOffset.x = (Int)((TargetLeftUpCurrPos.x - m_LeftUpCurrPos.x) );
			m_PosMoveOffset.y = (Int)((TargetLeftUpCurrPos.y - m_LeftUpCurrPos.y) );
			m_LeftUpCurrPos = TargetLeftUpCurrPos;
			m_PreMoveTime = CurrTime;
		}
		else
		{
			UInt32 TimeDiff = CurrTime - m_PreMoveTime;
			Real32 MoveFactor = (Real32)TimeDiff / (Real32)TimeNeeded;

			m_PosMoveOffset.x += (TargetLeftUpCurrPos.x - m_LeftUpCurrPos.x) * MoveFactor;
			m_PosMoveOffset.y += (TargetLeftUpCurrPos.y - m_LeftUpCurrPos.y) * MoveFactor;
			m_LeftUpCurrPos.x += (Int)m_PosMoveOffset.x;
			m_LeftUpCurrPos.y += (Int)m_PosMoveOffset.y;
			m_PreMoveTime = CurrTime;
		}
	}
	/*else
	{
		Real32 MoveFactor = (Real32)(TotalDiff - CamUpDateMin) / (Real32)TotalDiff;
		m_PosMoveOffset.x += (TargetLeftUpCurrPos.x - m_LeftUpCurrPos.x) * MoveFactor;
		m_PosMoveOffset.y += (TargetLeftUpCurrPos.y - m_LeftUpCurrPos.y) * MoveFactor;
		m_LeftUpCurrPos.x += (Int)m_PosMoveOffset.x;
		m_LeftUpCurrPos.y += (Int)m_PosMoveOffset.y;
		m_PreMoveTime = CurrTime;
	}*/
	m_Moved = (Int)m_PosMoveOffset.x != 0 || (Int)m_PosMoveOffset.y != 0;
	/*PixelDirection  MoveDir;
	MoveDir.PixelX = (Real32)TotalDiffX / PixelDiff;
	MoveDir.PixelY = (Real32)TotalDiffY / PixelDiff;

	Real32 DiffX = (Int)(MoveDir.PixelX * (Real32)(CurrTime - m_PreMoveTime) /1000 * CameSpeed);
	Real32 DiffY = (Int)(MoveDir.PixelY * (Real32)(CurrTime - m_PreMoveTime) /1000 * CameSpeed);	

	if (DiffX != 0)
	{
		if ((TargetLeftUpCurrPos.x >= m_LeftUpCurrPos.x &&
			TargetLeftUpCurrPos.x <= m_LeftUpCurrPos.x + DiffX) ||
			(TargetLeftUpCurrPos.x <= m_LeftUpCurrPos.x &&
			TargetLeftUpCurrPos.x >= m_LeftUpCurrPos.x + DiffX))
		{
			m_PosMoveOffset.x += (Real32)TotalDiffX;
		}
		else
		{
			m_PosMoveOffset.x += DiffX;
		}
		m_PreMoveTime = CurrTime;
	}

	if (DiffY != 0)
	{
		if ((TargetLeftUpCurrPos.y >= m_LeftUpCurrPos.y &&
			TargetLeftUpCurrPos.y <= m_LeftUpCurrPos.y + DiffY) ||
			(TargetLeftUpCurrPos.y <= m_LeftUpCurrPos.y &&
			TargetLeftUpCurrPos.y >= m_LeftUpCurrPos.y + DiffY))
		{
			m_PosMoveOffset.y += (Real32)TotalDiffY;
		}
		else
		{
			m_PosMoveOffset.y += DiffY;
		}
		m_PreMoveTime = CurrTime;
	}


	

	m_LeftUpCurrPos.x += (Int)m_PosMoveOffset.x;
	m_LeftUpCurrPos.y += (Int)m_PosMoveOffset.y;
	m_Moved = (Int)m_PosMoveOffset.x != 0 || (Int)m_PosMoveOffset.y != 0;\
	*/
}

VOID CCamera::MoveToTargetImm()
{
	if (m_pEntTarget == NULL)
		return;

	if (QGLApplication::GetInstance().GetMapStateID() != ST_APP_BATTLE)
	{
		QGLApplication::GetInstance().AddAllDirtyRect();
		SetCamerLeftUpPos(m_pEntTarget->m_EntPixelPos.PixelPosX, m_pEntTarget->m_EntPixelPos.PixelPosY);
	}

}

VOID CCamera::ResetMoveOffset()
{
	if ((Int)m_PosMoveOffset.x != 0)
		m_PosMoveOffset.x -= (Int)m_PosMoveOffset.x;
	if ((Int)m_PosMoveOffset.y != 0)
		m_PosMoveOffset.y -= (Int)m_PosMoveOffset.y;
}