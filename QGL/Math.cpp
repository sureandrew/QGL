//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "Math.h"
#include "Common/PathFinder.h"

VOID QGLMath::CalcBlock32X16Range()
{
	// leftcenter
	float X1 =  0;
	float Y1 =  BlockSizeY / 2;
	// topcenter	
   	float X2 =  BlockSizeX / 2;
	float Y2 =  0;
	// 	rightcenter
	float X3 =  BlockSizeX ;
	float Y3 =  BlockSizeY / 2;
	// bottomcenter
	float X4 =  BlockSizeX / 2;
	float Y4 =  BlockSizeY ;

	for(int y=0; y<BlockSizeY; ++y)
	{
		for (int x=0; x<BlockSizeX; ++x)
		{
			if (IsPointInRange((float)x ,(float)y , X1, Y1, X2, Y2, true) && // leftup
				IsPointInRange((float)x ,(float)y , X2, Y2, X3, Y3, true) && //rightup
				IsPointInRange((float)x ,(float)y , X4, Y4, X3, Y3, false) && //rightdown
				IsPointInRange((float)x ,(float)y , X1, Y1, X4, Y4, false))//leftdown
			{
				m_BlockPixelAreaRange[y][x] = true;
			}		
			else
				m_BlockPixelAreaRange[y][x] = false;
		}
	}
}



bool QGLMath::FindBlockByPixel(DWORD MapCurrPixelX, DWORD MapCurrPixelY, BlockPos& GetPos, DWORD mapwidth, DWORD mapheight, 
					  WORD maxblockx, WORD maxblocky)
{
	BlockPos SelectBlock;
	SelectBlock.PosX = 0;
	SelectBlock.PosY = 0;

	//DWORD mapwidth = pMap->GetMapWidth(), mapheight = pMap->GetMapHeight();
	//MapDataLst[m_CurrMapIndex]->GetCurrMapWidthAndHeight(mapwidth, mapheight);
	
	if ( MapCurrPixelX >= 0 && MapCurrPixelX <= (mapwidth - 1) &&
		 MapCurrPixelY >= 0 && MapCurrPixelY <= (mapheight - 1))
	{
		/* [ old ver ]
		SelectBlock.PosX = (WORD)(MapCurrPixelX / BlockSizeX);
		SelectBlock.PosY = (WORD)(MapCurrPixelY / (BlockSizeY / 2));
		GetPos = SelectBlock;
		return true;
		*/
		int HalfofBlockY = BlockSizeY/2;

		SelectBlock.PosX = (WORD)(MapCurrPixelX / BlockSizeX);
		SelectBlock.PosY = (WORD)(MapCurrPixelY / HalfofBlockY );
		
		int OffSetX = MapCurrPixelX % BlockSizeX;
		int OffSetY = MapCurrPixelY % BlockSizeY;

		if ( OffSetY >= HalfofBlockY )
		{
			if ( SelectBlock.PosY > 0 )
				SelectBlock.PosY -= 1;
			else
				return false;
		}

		if ( !m_BlockPixelAreaRange[OffSetY][OffSetX] )
		{
			if ( OffSetY < HalfofBlockY )
			{
				if ( SelectBlock.PosY > 0 )
					SelectBlock.PosY -= 1;
				else
					return false;
				if ( OffSetX < BlockSizeX / 2 )
				{// leftup corner
					if ( SelectBlock.PosY % 2 == 1)
					{
						if ( SelectBlock.PosX > 0 )
							SelectBlock.PosX -= 1;
						else
							return false;
					}
				}
			}
			else 
			{
				SelectBlock.PosY += 1;
				if ( OffSetX < BlockSizeX / 2 )
				{//leftdown corner
					if ( SelectBlock.PosY % 2 == 1)
					{
						if ( SelectBlock.PosX > 0 )
							SelectBlock.PosX -= 1;
						else
							return false;	
					}
				}
			}
		}
		
		//WORD maxblockx = 0, maxblocky = 0;
		//MapDataLst[m_CurrMapIndex]->GetCurrMapMaxNumBlock(maxblockx, maxblocky);

		if ( SelectBlock.PosX < 0 || SelectBlock.PosX > ( maxblockx - 1 ) ||
			SelectBlock.PosY < 0 || SelectBlock.PosY > (maxblocky - 2) )
			return false;
		
		GetPos = SelectBlock;
		return true;
	}
	else
	{
		GetPos = SelectBlock;
		return false;
	}
}

//////////////////////////////////////////////////////////////////

VOID QGLMath::MoveToBlock(Int & OrgPosX, Int & OrgPosY, Byte Dir, Int NoOfSteps)
{
	for (Int i = 0; i < NoOfSteps; i++)
	{
		switch(Dir)
		{
		case SOUTH:
			OrgPosX = OrgPosX;
			OrgPosY = OrgPosY + 2;
			break;
		case SOUTHEAST:
			if (OrgPosY % 2 == 0)
			{
				OrgPosX = OrgPosX;
				OrgPosY = OrgPosY + 1;
			}
			else
			{
				OrgPosX = OrgPosX + 1;
				OrgPosY = OrgPosY + 1;
			}
			break;
		case EAST:
			OrgPosX = OrgPosX + 1;
			OrgPosY = OrgPosY;
			break;
		case NORTHEAST:
			if (OrgPosY % 2 == 0)
			{
				OrgPosX = OrgPosX;
				OrgPosY = OrgPosY - 1;
			}
			else
			{

				OrgPosX = OrgPosX + 1;

				OrgPosY = OrgPosY - 1;
			}
			break;
		case NORTH:
			OrgPosX = OrgPosX;
			OrgPosY = OrgPosY - 2;
			break;
		case NORTHWEST:
			if (OrgPosY % 2 == 0)
			{
				OrgPosX = OrgPosX - 1;
				OrgPosY = OrgPosY - 1;
			}
			else
			{
				OrgPosX = OrgPosX;
					OrgPosY = OrgPosY - 1;
			}
			break;
		case WEST:
			OrgPosX = OrgPosX - 1;
			OrgPosY = OrgPosY;
			break;
		case SOUTHWEST:
			if (OrgPosY % 2 == 0)
			{
				OrgPosX = OrgPosX - 1;
				OrgPosY = OrgPosY + 1;
			}
			else
			{
				OrgPosX = OrgPosX;

				OrgPosY = OrgPosY + 1;
			}
			break;
		default:
			break;
		}
	}

}

BOOL QGLMath::isPositionInLst(StlVector<Position2D> PastPosLst, Int x, Int y)
{
	for (Int j = 0; j < (Int)PastPosLst.size(); j++)
		if (PastPosLst[j].X == x && PastPosLst[j].Y)
			return TRUE;
	return FALSE;
}

BOOL QGLMath::IsPointInRange(Real32 x, Real32 y, Real32 X1, Real32 Y1, Real32 X2, Real32 Y2, BOOL Large)
{
	if (X1 == X2)
	{
		if (x == X1)
			return TRUE;
		return FALSE;
	}

	Real32 slope = ((Y1 - Y2)/(X1 - X2));
	Real32 constC = Y1 - slope * X1;
	if (Large)
		return y - slope * x - constC >= 0;
	else
		return y - slope * x - constC <= 0;
}

/*BOOL QGLMath::CalculateReductedArea(RECT NewRect, RECT RectToBeBroken, StlVector<RECT> & NewDirtyRectLst)
{
	RECT ResultRect;
	if (CalculateOverlappedRect(NewRect, RectToBeBroken, ResultRect))
	{
		if (ResultRect.left > RectToBeBroken.left)
		{
			RECT ExtractRect = RectToBeBroken;
			ExtractRect.right = ResultRect.left;
			NewDirtyRectLst.push_back(ExtractRect);
			RectToBeBroken.left = ResultRect.left;
		}

		if (ResultRect.top > RectToBeBroken.top)
		{
			RECT ExtractRect = RectToBeBroken;
			ExtractRect.bottom = ResultRect.top;
			NewDirtyRectLst.push_back(ExtractRect);
			RectToBeBroken.top = ResultRect.top;
		}

		if (ResultRect.right < RectToBeBroken.right)
		{
			RECT ExtractRect = RectToBeBroken;
			ExtractRect.left = ResultRect.right;
			NewDirtyRectLst.push_back(ExtractRect);
			RectToBeBroken.right = ResultRect.right;
		}

		if (ResultRect.bottom < RectToBeBroken.bottom)
		{
			RECT ExtractRect = RectToBeBroken;
			ExtractRect.top = ResultRect.bottom;
			NewDirtyRectLst.push_back(ExtractRect);
			RectToBeBroken.bottom = ResultRect.bottom;
		}

		//DebugCheck
		/*for (Int i = 0; i < (Int)NewDirtyRectLst.size(); i++)
		{
			for (Int j = i + 1; j < (Int)NewDirtyRectLst.size(); j++)
			{
				RECT ResultRect;
				if (CalculateOverlappedRect(NewDirtyRectLst[i], NewDirtyRectLst[j], ResultRect))
				{
					RECT One = NewDirtyRectLst[i];
					RECT Two = NewDirtyRectLst[j];
					DebugBreak();
					//return TRUE;
				}
			}
			RECT ResultRect;
			if (CalculateOverlappedRect(NewDirtyRectLst[i], NewRect, ResultRect))
				DebugBreak();
				
		}*/
		/*return NewDirtyRectLst.size() > 0;
	}
	return FALSE;
}*/
/*BOOL QGLMath::CalculateBiggestRect(RECT rect1,
							 RECT rect2,
							 RECT & ResultRect)
{

	if (rect1.left > rect2.left)
		ResultRect.left = rect2.left;
	else
		ResultRect.left = rect1.left;

	if (rect1.top > rect2.top)
		ResultRect.top = rect2.top;
	else
		ResultRect.top = rect1.top;

	if (rect1.right > rect2.right)
		ResultRect.right = rect1.right;
	else
		ResultRect.right = rect2.right;

	if (rect1.bottom > rect2.bottom)
		ResultRect.bottom = rect1.bottom;
	else
		ResultRect.bottom = rect2.bottom;
	return TRUE;
}
*/

/*BOOL QGLMath::CalculateOverlappedRect(const RECT & rect1,
							 const RECT & rect2,
							 RECT & ResultRect)
{
	ResultRect.left = MAX(rect1.left, rect2.left);
	ResultRect.top = MAX(rect1.top, rect2.top);
	ResultRect.right = MIN(rect1.right, rect2.right);
	ResultRect.bottom = MIN(rect1.bottom, rect2.bottom);
	if (rect1.left > rect2.left)
		ResultRect.left = rect1.left;
	else
		ResultRect.left = rect2.left;

	if (rect1.top > rect2.top)
		ResultRect.top = rect1.top;
	else
		ResultRect.top = rect2.top;

	if (rect1.right > rect2.right)
		ResultRect.right = rect2.right;
	else
		ResultRect.right = rect1.right;

	if (rect1.bottom > rect2.bottom)
		ResultRect.bottom = rect2.bottom;
	else
		ResultRect.bottom = rect1.bottom;
		
	if (ResultRect.left >= ResultRect.right ||
		ResultRect.top >= ResultRect.bottom)
		return FALSE;

	return TRUE;
}*/

BOOL QGLMath::InvalidLock(RECT * LockRect,  UInt MaxWidth, UInt MaxHeight)
{
	if (!LockRect)
		return FALSE;

	if (LockRect->left == 0 && LockRect->right == 0 && LockRect->top == 0 && LockRect->bottom == 0)
		return FALSE;

	if (LockRect->left < 0 || LockRect->right < 0 || LockRect->top < 0 || LockRect->bottom < 0)
		return TRUE;

	if (LockRect->left >= LockRect->right ||
		LockRect->top >= LockRect->bottom)
		return TRUE;

	if (LockRect->right > (Int)MaxWidth ||
		LockRect->bottom > (Int)MaxHeight)
		return TRUE;

	return FALSE;
}


VOID QGLMath::BlockPosToPixel(Int BkX, Int BkY, Int & PixelX, Int & PixelY)
{
	PixelX =BkX * BlockSizeX + BlockSizeX / 2;
	PixelY = BkY * BlockSizeY/2 + BlockSizeY / 2;
	if (BkY % 2 == 1)
		PixelX += (Int)BlockSizeX / 2;
}

UInt32 QGLMath::CalculatePixelPosDistance(PixelPos StartPixelPos, PixelPos EndPixelPos, PixelDirection & dir)
{
	Int DiffPosX = EndPixelPos.PixelPosX - StartPixelPos.PixelPosX;
	Int DiffPosY = EndPixelPos.PixelPosY - StartPixelPos.PixelPosY;
	Real32 TotalDiff = (Real32)(sqrt(pow((double)DiffPosX, (int)2) + pow((double)DiffPosY, (int)2)));

	if (TotalDiff > 0)
	{
		dir.PixelX = (Real32)((Real32)DiffPosX / TotalDiff);
		dir.PixelY = (Real32)((Real32)DiffPosY / TotalDiff);
	}
	else
	{
		dir.PixelX = 0;
		dir.PixelY = 0;
	}

	return (UInt32)TotalDiff;
}

UInt32 QGLMath::CalculateBlockPosDistance(BlockPos Start, BlockPos End, PixelDirection & dir)
{
	PixelPos StartPixelPos;
	StartPixelPos.PixelPosX = (UInt32)(Start.PosX * BlockSizeX + BlockSizeX / 2);
	StartPixelPos.PixelPosY = (UInt32)(Start.PosY * BlockSizeY/2 + BlockSizeY / 2);
	if (Start.PosY % 2 == 1)
		StartPixelPos.PixelPosX += (Int)BlockSizeX / 2;
	PixelPos EndPixelPos;
	EndPixelPos.PixelPosX = (UInt32)(End.PosX * BlockSizeX + BlockSizeX / 2);
	EndPixelPos.PixelPosY = (UInt32)(End.PosY * BlockSizeY/2 + BlockSizeY / 2);
	if (End.PosY % 2 == 1)
		EndPixelPos.PixelPosX += (Int)BlockSizeX / 2;
	
	return CalculatePixelPosDistance(StartPixelPos, EndPixelPos, dir);
}

BOOL QGLMath::IsTwoRectEqual(RECT rect1, RECT rect2)
{
	if (rect1.left == rect2.left &&
		rect1.right == rect2.right &&
		rect1.bottom == rect2.bottom &&
		rect1.top == rect2.top)
		return TRUE;
	return FALSE;
}

Int QGLMath::GetFlipRotation(Int Rotation)
{
	switch(Rotation)
	{
	case NORTHWEST:
		return NORTHEAST;
	case WEST:
		return EAST;
	case SOUTHWEST:
		return SOUTHEAST;
	case EAST:
		return WEST;
	case NORTHEAST:
		return NORTHWEST;
	case SOUTHEAST:
		return SOUTHWEST;
	default:
		return -1;
	}
}

Int QGLMath::GetOppRotation(Int Rotation)
{
	switch(Rotation)
	{
	case SOUTH:
		return NORTH;
	case NORTH:
		return SOUTH;
	case NORTHWEST:
		return SOUTHEAST;
	case WEST:
		return EAST;
	case SOUTHWEST:
		return NORTHEAST;
	case EAST:
		return WEST;
	case NORTHEAST:
		return SOUTHWEST;
	case SOUTHEAST:
		return NORTHWEST;
	default:
		return -1;
	}
}

UInt QGLMath::GetNumOfDigit(UInt32 Num)
{
	UInt i = 1;
	Num /= 10;
	while (Num != 0)
	{
		Num /= 10;
		i ++;
	}
	return i;
}

UInt QGLMath::CalculateDistanceInBlock(BlockPos Start, BlockPos End, PathFinder * pPathFind)
{
	if (pPathFind == NULL)
		return 0;
	pPathFind->SetBlockingActive(FALSE);
	PathFinder::PointList ptList;
	pPathFind->GetWholePath(Start.PosX, Start.PosY, End.PosX, End.PosY, ptList);
	pPathFind->SetBlockingActive(TRUE);
	return (UInt)ptList.size();
}

Int QGLMath::GetRotationFromTwoPoint(Int StartBkX, Int StartBkY, Int EndBkX, Int EndBkY)
{
	Int EndX = 0;
	Int EndY = 0;
	BlockPosToPixel(EndBkX, EndBkY, EndX, EndY); 
	Int StartX = 0;
	Int StartY = 0;
	BlockPosToPixel(StartBkX, StartBkY, StartX, StartY);
	Int Y = StartY - EndY;
	Int X = EndX - StartX;
	if (X == 0)
	{
		if (Y > 0)
			return NORTH;
		else
			return SOUTH;
	}
	Real32 Slope = abs((Real32)Y/X);
	Int Deg = (Int)(atan(Slope) / 3.14 * 180);

	Int TotalDeg = 0;
	if (X >= 0 && Y >= 0)
		TotalDeg = 90 - Deg;
	else if (X >= 0 && Y < 0)
		TotalDeg = 90 + Deg;
	else if (X < 0 && Y < 0)
		TotalDeg = 270 - Deg;
	else
		TotalDeg = 270 + Deg;

	if (TotalDeg > 337 || TotalDeg <= 22)
		return NORTH;
	else if (TotalDeg > 22 && TotalDeg <= 67)
		return NORTHEAST;
	else if (TotalDeg > 67 && TotalDeg <= 112)
		return EAST;
	else if (TotalDeg > 112 && TotalDeg <= 157)
		return SOUTHEAST;
	else if (TotalDeg > 157 && TotalDeg <= 202)
		return SOUTH;
	else if (TotalDeg > 202 && TotalDeg <= 247)
		return SOUTHWEST;
	else if (TotalDeg > 247 && TotalDeg <= 292)
		return WEST;
	else 
		return NORTHWEST;
}

Int QGLMath::GetDiaRotationFromTwoPoint(Int StartBkX, Int StartBkY, Int EndBkX, Int EndBkY)
{
	Int EndX = 0;
	Int EndY = 0;
	BlockPosToPixel(EndBkX, EndBkY, EndX, EndY); 
	Int StartX = 0;
	Int StartY = 0;
	BlockPosToPixel(StartBkX, StartBkY, StartX, StartY);
	Int Y = StartY - EndY;
	Int X = EndX - StartX;
	if (X == 0)
	{
		if (Y > 0)
			return NORTHWEST;
		else
			return SOUTHEAST;
	}
	Real32 Slope = abs((Real32)Y/X);
	Int Deg = (Int)(atan(Slope) / 3.14 * 180);

	Int TotalDeg = 0;
	if (X >= 0 && Y >= 0)
		TotalDeg = 90 - Deg;
	else if (X >= 0 && Y < 0)
		TotalDeg = 90 + Deg;
	else if (X < 0 && Y < 0)
		TotalDeg = 270 - Deg;
	else
		TotalDeg = 270 + Deg;

	
	if (TotalDeg > 0 && TotalDeg <= 90)
		return NORTHEAST;
	else if (TotalDeg > 90 && TotalDeg <= 180)
		return SOUTHEAST;
	else if (TotalDeg > 180 && TotalDeg <= 270)
		return SOUTHWEST;
	else
		return NORTHWEST;
}