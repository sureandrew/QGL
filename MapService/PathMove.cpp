//#include "QGL.h"
#include "PathMove.h"
//#include "Math.h"
#include "Math_QGL.h"

#include "Common/PathFinder.h"
//#include "common_char_map.h"
//#include "Global.h"

CONST Int SmallestMove = 0;

VOID CPathMove::UpDateMoveData(UInt32 CurrTime, Int EntityMoveSpeed, UInt32 & PreMoveTime,
						   PixelPos & CurrPixelPos, BlockPos & CurrBkPos, BlockPos & DesBkPos,
						   UInt16 MaxBlockX, UInt16 MaxBlockY, Int & Rotation, PathFindData & RefPathFindData, 
						   PathFinder * pPathFinder)
{

	if (pPathFinder == NULL || CurrBkPos == DesBkPos)
	{		
		PreMoveTime = CurrTime;
		return;
	}

	if (PreMoveTime > CurrTime)
		PreMoveTime = CurrTime;


	BlockPos NextPos = CurrBkPos;
	while (PreMoveTime < CurrTime)//(Int)TotalDiffX > 0 || (Int)TotalDiffY > 0)
	{

		//check whether there is path remaining
		if (CurrBkPos == NextPos)
		{
			//find path again, if the destination had not been reached
			if (RefPathFindData.m_ptList.size() == 0)
			{
				if (RefPathFindData.Index >= RefPathFindData.m_gList.size() && 
					DesBkPos != CurrBkPos)
				{
					float dist = 0;
					RefPathFindData.Init();

					pPathFinder->GetPath(CurrBkPos.PosX, CurrBkPos.PosY, 
						DesBkPos.PosX, DesBkPos.PosY, dist, RefPathFindData.m_ptList, 
						RefPathFindData.m_gList);

					if (RefPathFindData.m_ptList.size() == 0 &&
						RefPathFindData.m_gList.size() == 0)
					{
						DesBkPos = CurrBkPos;
						return;
					}
				}
				if (RefPathFindData.Index < RefPathFindData.m_gList.size())
				{
					pPathFinder->NextPathLine(CurrBkPos.PosX, CurrBkPos.PosY, 
						DesBkPos.PosX, DesBkPos.PosY, RefPathFindData.m_ptList, 
						RefPathFindData.m_gList, RefPathFindData.Index);

				}
			}
			
			if (RefPathFindData.m_ptList.size() > 0)
			{
				NextPos.PosX = (*RefPathFindData.m_ptList.begin()).PosX;
				NextPos.PosY = (*RefPathFindData.m_ptList.begin()).PosY;
			}
		}

		//find out which direction to go
		BOOL Found = FALSE;
		for (Byte TempDir = SOUTH; TempDir < MAXROTATION; TempDir++)
		{
			Int TempMinX = CurrBkPos.PosX;
			Int TempMinY = CurrBkPos.PosY;
			QGLMath::MoveToBlock(TempMinX, TempMinY, TempDir, 1);
			if (TempMinX == NextPos.PosX && 
				TempMinY == NextPos.PosY)
			{
				Rotation = TempDir;
				Found = TRUE;
			}
		}

		PixelPos DesPixelPos;
		QGLMath::BlockPosToPixel(NextPos.PosX, NextPos.PosY, DesPixelPos.PixelPosX, DesPixelPos.PixelPosY);

		PixelPos CurrPixelPos2;
		QGLMath::BlockPosToPixel(CurrBkPos.PosX, CurrBkPos.PosY, CurrPixelPos2.PixelPosX, CurrPixelPos2.PixelPosY);

		PixelDirection  dir;

		UInt32 TotalDiff = QGLMath::CalculatePixelPosDistance(CurrPixelPos2, DesPixelPos, dir);
		UInt32 TimeNeeded = 0;
		if (EntityMoveSpeed == 0)
			TimeNeeded = 0xFFFFFFFF;
		else
			TimeNeeded = (TotalDiff) * 1000 /  abs(EntityMoveSpeed);

		if (CurrTime - PreMoveTime >= TimeNeeded)
		{
			PreMoveTime += TimeNeeded;
			CurrBkPos = NextPos;
			QGLMath::BlockPosToPixel(CurrBkPos.PosX, CurrBkPos.PosY, CurrPixelPos.PixelPosX, CurrPixelPos.PixelPosY);
			if (RefPathFindData.m_ptList.size() > 0)
				RefPathFindData.m_ptList.erase(RefPathFindData.m_ptList.begin());
			if (NextPos == DesBkPos)
			{
				PreMoveTime = CurrTime;
				break;
			}
		}
		else
		{
			UInt32 TimeDiff = CurrTime - PreMoveTime;
			Real32 MoveFactor = (Real32)TimeDiff / (Real32)TimeNeeded;
			CurrPixelPos.PixelPosX = CurrPixelPos2.PixelPosX + (Int)((DesPixelPos.PixelPosX - CurrPixelPos2.PixelPosX) * MoveFactor);
			CurrPixelPos.PixelPosY = CurrPixelPos2.PixelPosY + (Int)((DesPixelPos.PixelPosY - CurrPixelPos2.PixelPosY) * MoveFactor);
			break;
		}
	}
}
VOID CPathMove::UpDateMoveOffsetData(UInt32 CurrTime, UInt32 & StartTime, Int & OffsetX, Int & OffsetY, Int & OffsetJumpY,
								 Int InitialMoveYSpeed, Int GravityAcc, Int desX, Int desY, Real64 TotalTime, Int OrgPosX, Int OrgPosY)
{
	if (StartTime > 0)
	{
		if (StartTime > CurrTime)
		{
			StartTime = CurrTime;
			return;
		}

		Real64 timediff = CurrTime - StartTime;
		timediff /= 1000;
		Int temp = (Int)(InitialMoveYSpeed * timediff + 0.5 * GravityAcc * timediff * timediff);

		Int DiffX = (Int)((desX - OrgPosX) * timediff / TotalTime);
		//(Int)(pEnt->PixelSpeedY * dir.PixelX * (CurrTime - pEnt->PreTimeMoveX) / 1000);
		Int DiffY = (Int)((desY - OrgPosY) * timediff / TotalTime);
		//(Int)(pEnt->PixelSpeedY * dir.PixelY * (CurrTime - pEnt->PreTimeMoveY) / 1000);

		if (OffsetX != OrgPosX + DiffX)
		{
			//pEnt->SetEntOffsetX(OrgPosX + DiffX);
			//Updated = TRUE;
			OffsetX = OrgPosX + DiffX;
		}

		if (OffsetY != OrgPosY + DiffY)
		{
			//FootStepCount += abs(pEnt->GetRelativeEntOffsetY() - (OrgPosY + DiffY));
			//pEnt->SetEntOffsetY(OrgPosY + DiffY);
			//Updated = TRUE;
			OffsetY = OrgPosY + DiffY;
		}

		if (OffsetJumpY != temp)
		{
			OffsetJumpY = temp;
			//Updated = TRUE;
		}

		if (timediff > TotalTime)
		{
			//Updated = TRUE;
			//InitialMoveYSpeed = 0;
			OffsetJumpY = 0;
			StartTime = 0;
			OffsetX = desX;
			OffsetY = desY;
			//pEnt->Updateanimationset();
			//if (!pEnt->m_AnimationLoop)
			//	pEnt->Updateanimationset();
			
		}
	}
}
