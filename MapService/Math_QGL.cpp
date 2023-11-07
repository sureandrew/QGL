//-- Common
#include "Common.h"
//-- Self
#include "Math_QGL.h"

CONST Int BlockSizeX = 32;
CONST Int BlockSizeY = 16;

void QGLMath::CalcBlock32X16Range()
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

//////////////////////////////////////////////////////////////////

void QGLMath::MoveToBlock(Int & OrgPosX, Int & OrgPosY, Byte Dir, Int NoOfSteps)
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


UInt32 QGLMath::CalculateBlockPosBkDistance(BlockPos Start, BlockPos End, PixelDirection & dir)
{
	Int DiffPosX = (Int)((Int)(End.PosX - Start.PosX) * BlockSizeX);
	//assume the block has the same width and height
	Int DiffPosY = (Int)((Int)(End.PosY - Start.PosY) * BlockSizeX/2);
	if (End.PosY % 2 == 1 &&  Start.PosY % 2 == 0)
		DiffPosX += (Int)BlockSizeX / 2;
	else if (End.PosY % 2 == 0 &&  Start.PosY % 2 == 1)
		DiffPosX -= (Int)BlockSizeX / 2;

	Real32 TotalDiff = (Real32)(sqrt(pow((double)DiffPosX, (int)2) + pow((double)DiffPosY, (int)2)));

	dir.PixelX = (Real32)((Real32)DiffPosX / TotalDiff);
	dir.PixelY = (Real32)((Real32)DiffPosY / TotalDiff);
	return (UInt32)TotalDiff;
	
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

VOID QGLMath::BlockPosToPixel(UInt16 BkX, UInt16 BkY, Int & PixelX, Int & PixelY)
{
	PixelX =BkX * BlockSizeX + BlockSizeX / 2;
	PixelY = BkY * BlockSizeY/2 + BlockSizeY / 2;
	if (BkY % 2 == 1)
		PixelX += (Int)BlockSizeX / 2;
}