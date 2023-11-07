//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "PathMove.h"
#include "Math.h"
#include "QGLApplication.h"
#include "Global.h"

#define MAXPreSteps 30

CONST Int SmallestMove = 0;

VOID CPathMove::UpDateMoveData(UInt32 CurrTime, Int EntityMoveSpeed, UInt32 & PreMoveTime,
						   PixelPos & CurrPixelPos, BlockPos & CurrBkPos, BlockPos & DesBkPos,
						   UInt16 MaxBlockX, UInt16 MaxBlockY, Int & Rotation, PathFindData & RefPathFindData, 
						   PathFinder * pPathFinder, Real32 & MoveFactor)
{

	if (pPathFinder == NULL || CurrBkPos == DesBkPos)
	{		
		PreMoveTime = CurrTime;
		return;
	}

	if (PreMoveTime > CurrTime)
		PreMoveTime = CurrTime;


	BlockPos NextPos = CurrBkPos;
	/*if (RefPathFindData.m_ptList.size() > 0)
	{
		NextPos.PosX = (*RefPathFindData.m_ptList.begin()).PosX;
		NextPos.PosY = (*RefPathFindData.m_ptList.begin()).PosY;
	}*/
	while (PreMoveTime < CurrTime)//(Int)TotalDiffX > 0 || (Int)TotalDiffY > 0)
	{

		//check whether there is path remaining
		if (CurrBkPos == NextPos)
		{
			//find path again, if the destination had not been reached
			if (RefPathFindData.m_ptList.size() == 0)
			{
				if (//RefPathFindData.Index >= RefPathFindData.m_gList.size() && 
					DesBkPos != CurrBkPos)
				{
					float dist = 0;
					RefPathFindData.Init();

					//pPathFinder->GetPath(CurrBkPos.PosX, CurrBkPos.PosY, 
					//	DesBkPos.PosX, DesBkPos.PosY, dist, RefPathFindData.m_ptList, 
					//	RefPathFindData.m_gList);

					pPathFinder->GetWholePath(CurrBkPos.PosX, CurrBkPos.PosY, 
						DesBkPos.PosX, DesBkPos.PosY, RefPathFindData.m_ptList);

					if (RefPathFindData.m_ptList.size() == 0)// &&
						//RefPathFindData.m_gList.size() == 0)
					{
						DesBkPos = CurrBkPos;
						return;
					}
					else
					{
						DesBkPos = RefPathFindData.m_ptList[RefPathFindData.m_ptList.size() - 1];
					}
				}
				/*if (RefPathFindData.Index < RefPathFindData.m_gList.size())
				{
					pPathFinder->NextPathLine(CurrBkPos.PosX, CurrBkPos.PosY, 
						DesBkPos.PosX, DesBkPos.PosY, RefPathFindData.m_ptList, 
						RefPathFindData.m_gList, RefPathFindData.Index);

				}*/
			}
			
			if (RefPathFindData.m_ptList.size() > 0)
			{
				NextPos.PosX = (*RefPathFindData.m_ptList.begin()).PosX;
				NextPos.PosY = (*RefPathFindData.m_ptList.begin()).PosY;

				//RefPathFindData.m_ptList.erase(RefPathFindData.m_ptList.begin());
				/*
				if (RefPathFindData.PreSteps.size() >= MAXPreSteps)
					RefPathFindData.PreSteps.pop_back();
				
				if (RefPathFindData.PreSteps.size() == 0 ||
					(RefPathFindData.PreSteps.size() > 0 &&
					RefPathFindData.PreSteps[0] != NextPos))
				RefPathFindData.PreSteps.push_front(NextPos);
				*/

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

		if (!Found)
		{
			int i = 0;
			i ++;
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
			MoveFactor = 0;

			if (RefPathFindData.PreSteps.size() >= MAXPreSteps)
					RefPathFindData.PreSteps.pop_back();
				
				if (RefPathFindData.PreSteps.size() == 0 ||
					(RefPathFindData.PreSteps.size() > 0 &&
					RefPathFindData.PreSteps[0] != NextPos))
				RefPathFindData.PreSteps.push_front(NextPos);

			QGLMath::BlockPosToPixel(CurrBkPos.PosX, CurrBkPos.PosY, CurrPixelPos.PixelPosX, CurrPixelPos.PixelPosY);
			if (RefPathFindData.m_ptList.size() > 0)
			{
				RefPathFindData.m_ptList.erase(RefPathFindData.m_ptList.begin());
			}
			
			if (NextPos == DesBkPos)
			{
				PreMoveTime = CurrTime;
				break;
			}
		}
		else
		{
			UInt32 TimeDiff = CurrTime - PreMoveTime;
			MoveFactor = (Real32)TimeDiff / (Real32)TimeNeeded;
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
			return;
		}

		Real64 timediff = CurrTime - StartTime;
		timediff /= 1000;
		Int temp = (Int)(InitialMoveYSpeed * timediff + 0.5 * GravityAcc * timediff * timediff);

		Int DiffX = 0;
		if (TotalTime > 0)
			DiffX = (Int)((desX - OrgPosX) * timediff / TotalTime);
		
		Int DiffY = 0;
		if (TotalTime > 0)
			DiffY = (Int)((desY - OrgPosY) * timediff / TotalTime);
		
		if (OffsetX != OrgPosX + DiffX)
		{
			OffsetX = OrgPosX + DiffX;
		}

		if (OffsetY != OrgPosY + DiffY)
		{
			OffsetY = OrgPosY + DiffY;
		}

		if (OffsetJumpY != temp)
		{
			OffsetJumpY = temp;
		}

		if (timediff > TotalTime)
		{
			OffsetJumpY = 0;
			StartTime = 0;
			OffsetX = desX;
			OffsetY = desY;	
		}
	}
}
