#pragma once
#ifndef MATH_QGL_H
#define MATH_QGL_H

#include "Common/common_char_map.h"

struct PixelDirection
{
	Real32 PixelX;
	Real32 PixelY;
	PixelDirection()
	{
		PixelX = 0;
		PixelY = 0;
	}
};

//frame rotation
enum FrameRotation
{
	SOUTH = 0,
	SOUTHEAST,
	EAST,
	NORTHEAST,
	NORTH,
	NORTHWEST,
	WEST,
	SOUTHWEST,
	MAXROTATION,
};

namespace QGLMath
{

	static bool m_BlockPixelAreaRange[16][32];

	BOOL IsPointInRange(Real32 x, Real32 y, Real32 X1, Real32 Y1, Real32 X2, Real32 Y2, BOOL Large);
	UInt32 CalculateBlockPosDistance(BlockPos Start, BlockPos End, PixelDirection & dir);
	UInt32 CalculateBlockPosBkDistance(BlockPos Start, BlockPos End, PixelDirection & dir);
	UInt32 CalculatePixelPosDistance(PixelPos StartPixelPos, PixelPos EndPixelPos, PixelDirection & dir);
	void CalcBlock32X16Range();
	void MoveToBlock(Int & OrgPosX, Int & OrgPosY, Byte Dir, Int NoOfSteps);

	VOID BlockPosToPixel(UInt16 BkX, UInt16 BkY, Int & PixelX, Int & PixelY);
};

#endif