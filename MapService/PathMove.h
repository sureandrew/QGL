#pragma once
#ifndef PATH_H
#define PATH_H

//#include "MapView.h"
#include "Reuben/System/Reuben.h"
#include "Common/common_char_map.h"
#include "MapService/PathFinder.h"
//#include "../Stub/Logic/Character.h"



//CONST Int EntityMoveSpeed = 300;

//path finding class
namespace CPathMove
{
	void UpDateMoveData(UInt32 CurrTime, Int EntityMoveSpeed, UInt32 & PreMoveTime,
						    PixelPos & CurrPixelPos, BlockPos & CurrBkPos, BlockPos & DesBkPos,
						    UInt16 MaxBlockX, UInt16 MaxBlockY,Int & Rotation, PathFindData  & RefPathFindData, 
						   PathFinder * pPathFinder);
	void UpDateMoveOffsetData(UInt32 CurrTime, UInt32 & StartTime, Int & OffsetX, Int & OffsetY, Int & OffsetJumpY,
								 Int InitialMoveYSpeed, Int GravityAcc, Int desX, Int desY, Real64 TotalTime, Int OrgPosX, Int OrgPosY);

};

#endif