#ifndef _RES_AUTO_PATH_H_
#define _RES_AUTO_PATH_H_

//#ifdef AUTOPATHON
struct JUMPPOINT   //Jump point Desc Struct Using in cross map path finder
{
	UInt32 uSeq;
	UInt32 uJumpType;
	UInt32 uMapId;
	UInt16 u16PosX;
	UInt16 u16PosY;
	UInt16 u16PosXJumped;
	UInt16 u16PosYJumped;
	UInt32 uRelatedJumpPointNum;
	UInt32Vector uvRelatedPoint;

	JUMPPOINT()
		:uSeq(0)
		,uJumpType(0)
		,uMapId(0)
		,u16PosX(0)
		,u16PosY(0)
		,u16PosXJumped(0)
		,u16PosYJumped(0)
		,uRelatedJumpPointNum(0)
	{
		uvRelatedPoint.clear();
	}
	~JUMPPOINT()
	{
		uvRelatedPoint.clear();
	}
};
//#endif

#endif