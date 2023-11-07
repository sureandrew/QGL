#pragma once
#ifndef PATH_H
#define PATH_H

//CONST Int EntityMoveSpeed = 300;
#include "Common\PathFinder.h"
#include "EntityBase.h"

//path finding class
namespace CPathMove
{
	VOID UpDateMoveData(UInt32 CurrTime, Int EntityMoveSpeed, UInt32 & PreMoveTime,
						    PixelPos & CurrPixelPos, BlockPos & CurrBkPos, BlockPos & DesBkPos,
						    UInt16 MaxBlockX, UInt16 MaxBlockY,Int & Rotation, PathFindData  & RefPathFindData, 
						   PathFinder * pPathFinder, Real32 & MoveFactor);
	VOID UpDateMoveOffsetData(UInt32 CurrTime, UInt32 & StartTime, Int & OffsetX, Int & OffsetY, Int & OffsetJumpY,
								 Int InitialMoveYSpeed, Int GravityAcc, Int desX, Int desY, Real64 TotalTime, Int OrgPosX, Int OrgPosY);

};

#endif