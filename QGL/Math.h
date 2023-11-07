#pragma once
#ifndef _MATH_H
#define _MATH_H
//#include "PathFind\PathFinder.h"

class PathFinder;

namespace QGLMath
{

	static bool m_BlockPixelAreaRange[16][32];

	//return the overlapped area
	static inline BOOL __fastcall CalculateOverlappedRect(RECT rect1,
		RECT rect2,
		RECT & ResultRect)
	{
		ResultRect.left = MAX(rect1.left, rect2.left);
		ResultRect.top = MAX(rect1.top, rect2.top);
		ResultRect.right = MIN(rect1.right, rect2.right);
		ResultRect.bottom = MIN(rect1.bottom, rect2.bottom);
		/*if (rect1.left > rect2.left)
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
		*/
		if (ResultRect.left >= ResultRect.right ||
			ResultRect.top >= ResultRect.bottom)
			return FALSE;

		return TRUE;
	}

	//return the rectangle which include rect1 and rect2
	static inline VOID __fastcall CalculateBiggestRect(RECT  rect1,
		RECT  rect2,
		RECT & ResultRect)
	{
		ResultRect.left = MIN(rect1.left, rect2.left);
		ResultRect.top = MIN(rect1.top, rect2.top);
		ResultRect.right = MAX(rect1.right, rect2.right);
		ResultRect.bottom = MAX(rect1.bottom, rect2.bottom);
	}

	BOOL IsPointInRange(Real32 x, Real32 y, Real32 X1, Real32 Y1, Real32 X2, Real32 Y2, BOOL Large);
	BOOL isPositionInLst(StlVector<Position2D> PastPosLst, Int x, Int y);
	BOOL InvalidLock(RECT * LockRect, UInt MaxWidth, UInt MaxHeight);
	BOOL IsTwoRectEqual(RECT rect1, RECT rect2);

	UInt32 CalculateBlockPosDistance(BlockPos Start, BlockPos End, PixelDirection & dir);
	UInt32 CalculatePixelPosDistance(PixelPos StartPixelPos, PixelPos EndPixelPos, PixelDirection & dir);
	
	static inline BOOL __fastcall  CalculateReductedArea(const RECT & NewRect, RECT  RectToBeBroken, StlVector<RECT> & NewDirtyRectLst)
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

			return NewDirtyRectLst.size() > 0;
		}
		return FALSE;
	}

	bool FindBlockByPixel(DWORD MapCurrPixelX, DWORD MapCurrPixelY, BlockPos& GetPos,  DWORD mapwidth, DWORD mapheight, 
		WORD maxblockx, WORD maxblocky);
	VOID CalcBlock32X16Range();

	Int GetFlipRotation(Int Rotation);

	VOID MoveToBlock(Int & OrgPosX, Int & OrgPosY, Byte Dir, Int NoOfSteps);

	VOID BlockPosToPixel(Int BkX, Int BkY, Int & PixelX, Int & PixelY);

	Int GetOppRotation(Int Rotation);
	UInt GetNumOfDigit(UInt32 Num);

	UInt CalculateDistanceInBlock(BlockPos Start, BlockPos End, PathFinder * pPathFind);

	Int GetRotationFromTwoPoint(Int StartBkX, Int StartBkY, Int EndBkX, Int EndBkY);
	Int GetDiaRotationFromTwoPoint(Int StartBkX, Int StartBkY, Int EndBkX, Int EndBkY);

};
#endif