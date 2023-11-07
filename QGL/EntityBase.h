#pragma once
#ifndef ENTITY_BASE_H_
#define ENTITY_BASE_H_

#include "Resources.h"
//#include "PathFind\PathFinder.h"

const Int ERROR_INDEX = -1;

enum EntityType
{
	ENTITY_TYPE_MAP_PLAYER = 0,
	ENTITY_TYPE_MAP_CONTROL_PLAYER,
	ENTITY_TYPE_MAP_OBJECT,
	ENTITY_TYPE_BATTLE_PLAYER,
	ENTITY_TYPE_PARTICLE_WITHEFFECT,
	ENTITY_TYPE_PARTICLE_WITHOUTEFFECT,
	ENTITY_TYPE_ENTITY_WITHOUTEFFECT,
};

struct PathFindData
{
	//path find list
	typedef StlDeque<BlockPos, ManagedAllocator<BlockPos> > PointList;
	PointList m_ptList;
	StlDeque<BlockPos> PreSteps;
	PathFindData()
	{
	}
	VOID Init()
	{
		m_ptList.clear();	
	}

	VOID Clear()
	{
		m_ptList.clear();
		PreSteps.clear();
	}
};

class CEntityBase
{
public:

	typedef StlMap<UInt, CEntityBase *> EntityMap;

	//construct and distruct
	CEntityBase();
	virtual ~CEntityBase();

	//virtual function

	//update the entity data
	virtual VOID UpDate(UInt32 CurrentTime);
	
	//get one part of the entity
	virtual BOOL GetEntityPart(size_t index, Int ImageType, RECT * LockRegion);

	virtual BOOL GetHaveEffect();

	virtual Int GetTotalRenderPart();

	virtual Int GetFlip(Int Index);

	virtual BOOL GetIsMapObject();

	//set the entity position by given position X and Y
	VOID SetCharPos(UInt16 x, UInt16 y, BOOL SetDes = TRUE);

	//set the entity position by given block position
	VOID SetCharPos(BlockPos GivenPos, BOOL SetDes = TRUE);

	//get the current character position in terms of BlockPos
	inline BlockPos GetCharPos(){
		if(m_pParentEnt)
			return m_pParentEnt->GetCharPos() + m_CharPos;
			return m_CharPos;
	};
	
	//get the destination position in terms of BlockPos
	inline BlockPos GetCharDesPos(){return m_DesPos;};

	//set entity destination position
	VOID SetCharDesPos(UInt16 x, UInt16 y);
	//set the jump data for the entity
	VOID SetJumpData(Int height, Int BkPosX, Int BkPosY, Real64 timeInMS, UInt32 StartDelay = 0, BOOL IsPixel = FALSE);

	//update the animation chain data
	BOOL Updateanimationset(UInt32 CurrentTime);

	//check whether it is the end of the animation
	BOOL FrameEnd();

	//add child to this entity
	VOID AddChild(CEntityBase * pEnt);

	//remove child from this entity
	VOID RemoveChild(CEntityBase * pEnt);

	//remove the parent 
	VOID LeaveParent();

	//set the parent entity class
	VOID SetEntParent(CEntityBase* pEnt);

	virtual VOID FreeEntityData(){};

	virtual VOID SetAnimationIndexByIndex(Int Index, BOOL Loop = TRUE, Int GivenRotation = ERROR_INDEX, BOOL flush = FALSE);

	virtual inline Int GetAnimationIndex() { return 0;}

	virtual inline Int GetAnimationRotation() { return  0;}

	//set the rotation of the entity
	virtual VOID SetRotation(Int Rotation);

	//check whether it is the end of the animation
	virtual BOOL FrameEnd(UInt32 CurrentTime);
	virtual inline BOOL GetStopAnimation()
	{
		return TRUE;
	}

	virtual int inline GetAnimationLoop(){return 0;}
	//get the world position of the entity
	inline PixelPos GetEntPixelPos()
	{
		if (m_pParentEnt)
			return m_pParentEnt->GetEntPixelPos() + m_EntPixelPos;
		return m_EntPixelPos;
	};

	//get the world left up position of the entity
	inline PixelPos GetEntLeftUpPos()
	{
		if (m_pParentEnt)
			return m_pParentEnt->GetEntLeftUpPos() + m_EntLeftUpPos;
		return m_EntLeftUpPos;
	};

	//get the relative position of the entity
	inline PixelPos GetRelativeEntPixelPos()
	{
		return m_EntPixelPos;
	};

	//get the relative left up position of the entity
	inline PixelPos GetRelativeEntLeftUpPos()
	{
		return m_EntLeftUpPos;
	};

	//get the relative position of the entity
	inline Int GetEntOffsetJumpY()
	{
		return m_OffsetJumpY;
	};

	//get the relative left up position of the entity
	VOID SetEntOffsetJumpY(Int Value);

	//set the value for the left up position of the entity
	inline void SetEntLeftUpPos(PixelPos GivenPos)
	{
		if (m_pParentEnt)
			m_EntLeftUpPos = GivenPos - m_pParentEnt->GetEntLeftUpPos();
		else
			m_EntLeftUpPos = GivenPos;
	};
	
	inline void  SetEntLeftUpPos(Int X, Int Y)
	{
		if (m_pParentEnt)
		{
			m_EntLeftUpPos.PixelPosX = X - m_pParentEnt->GetEntLeftUpPos().PixelPosX ;
			m_EntLeftUpPos.PixelPosY = Y - m_pParentEnt->GetEntLeftUpPos().PixelPosY;
		}
		else
		{
			m_EntLeftUpPos.PixelPosX = X;
			m_EntLeftUpPos.PixelPosY = Y;
		}
	};

	//set the value for the position of the entity
	VOID SetEntPixelPos(PixelPos GivenPos);
	
	VOID SetEntPixelPos(Int X, Int Y);
	
	//set the Relative value for the left up position of the entity
	inline void  SetRelativeEntLeftUpPos(PixelPos GivenPos)
	{
		m_EntLeftUpPos = GivenPos;
	};
	
	inline void SetRelativeEntLeftUpPos(Int X, Int Y)
	{
		m_EntLeftUpPos.PixelPosX = X;
		m_EntLeftUpPos.PixelPosY = Y;
	};

	//set the value for the position of the entity
	VOID SetRelativeEntPixelPos(PixelPos GivenPos);
	
	VOID SetRelativeEntPixelPos(Int X, Int Y);

	//init overlapped regions 
	inline VOID initOverlappedRect()
	{
		m_OverlappedRegion.clear();
	};
	
	//check whether there is overlapped regions
	inline BOOL IsOverlappedRectValid()
	{
		return (m_OverlappedRegion.size() > 0);
	};


	inline void SetExtraZBuffer(Int64 Value)
	{
		m_ExtraZBuffer = Value;
	};

	inline Int64 GetExtraZBuffer()
	{
		if (m_pParentEnt)
			return m_ExtraZBuffer + m_pParentEnt->GetExtraZBuffer();
		return m_ExtraZBuffer;
	};

	//getter and setter for offset position
	Int GetEntOffsetX();
	Int GetEntOffsetY();
	Int GetEntOffsetDesX();
	Int GetEntOffsetDesY();

	inline Int GetRelativeEntOffsetX()
	{
		return m_OffsetX;
	};

	inline Int GetRelativeEntOffsetY()
	{
		return m_OffsetY;
	};
	inline Int GetRelativeEntDesOffsetX()
	{
		return m_OffsetDesX;
	};

	inline Int GetRelativeEntDesOffsetY()
	{
		return m_OffsetDesY;
	};
	VOID SetEntOffsetX(Int Value, BOOL SetDes = TRUE);
	VOID SetEntOffsetY(Int Value, BOOL SetDes = TRUE);
	VOID SetEntDesOffsetX(Int Value);
	VOID SetEntDesOffsetY(Int Value);

	inline void RemoveAllChildren()
	{
		m_pChildrenEntLst.clear();
	};

	//return the speed of entity
	inline Int GetEntityMoveSpeed(){ return m_EntityMoveSpeed;};

	//set the speed of entity
	inline void SetEntityMoveSpeed(Int Value){ m_EntityMoveSpeed = Value;};

	//init the timing of this entity
	VOID InitTime();

	//reset the update time 
	VOID ResetUpdateTime(UInt32 CurrTime);

	//set the scale of the entity
	VOID SetScaleX(Real32 Value);
	VOID SetScaleY(Real32 Value);

	inline Byte GetAlphaOffset(){return m_AlphaOffset;}
	VOID SetAlphaOffset(Byte Value, BOOL SetChildren = FALSE);

	VOID StopEntity();

	virtual BOOL IsMoving();

	VOID SetFollowTarget(CEntityBase * pEntBase);
	VOID AddFollower(CEntityBase * pEntBase);
	VOID InsertFollower(CEntityBase * pEntBase, int index);
	VOID RemoveFollowTarget();
	VOID RemoveFollower(CEntityBase * pEntBase);

	CEntityBase * GetFollowTarget();
	StlVector<CEntityBase *> GetFollowerLst();

	virtual RECT GetBattleIdleRect();

	VOID SetVisible(BOOL visible, BOOL SetChildren = FALSE);

	inline BOOL GetVisible(){ return m_Visible;}

	virtual BOOL GetCanBeSelected(){return FALSE;}

	BlockPos GetPreStep(Int Index);

	inline RECT GetCurrRect(){return m_CurrRect;}

	VOID RemoveAllFollowers();

	Int GetFollowerLstIndex(CEntityBase * pEntBase);

	virtual Int GetOffsetType(){ return m_OffsetType;}

	inline void SetOffsetType(BYTE value){m_OffsetType = value;}

	inline UInt16 GetWidth(){return m_EntOrgWidth;}
	inline UInt16 GetHeight(){return m_EntOrgHeight;}

	VOID UpdateEntityMoveOffset(UInt32 CurrTime);
	
	UInt32 GetZValue(UInt32 MapWidth);

	inline Byte GetEntityType()
	{
		return m_EntityType;
	}

	//set the entity Type
	VOID SetEntityType(Byte Value);

	virtual BYTE GetEquipType(Int Index);

	VOID SetShakeInfo(Int ShakeX, Int ShakeY, UInt ShakeSpeedX,UInt ShakeSpeedY, UInt ShakeWidth, UInt ShakeHeight, UInt32 TotalShakeTime);

	VOID StopShake();

	inline Int GetShakeX(){ return m_ShakeX;}
	inline Int GetShakeY(){ return m_ShakeY;}

	VOID UpdateShaking(UInt32 CurrentTime);

	inline UInt32 GetPicWidth() {return m_PicWidth;}

	inline BOOL GetHighLight(){ return m_HighLight;}
	inline void SetHighLight (BOOL Value){ m_HighLight = Value;}

	inline virtual Byte * GetCurrColorP(){ return NULL;}
	inline virtual Byte GetPicType(){ return HighQ32Bits;}
	inline virtual Byte * GetPicData(){ return m_CurrPicData;}

	virtual BOOL IsMount(){return FALSE;}

	VOID CopyPos(CEntityBase * pRef);

public:

	Real64 m_TotalTime;
	//check whether the entity is updated
	BOOL m_Updated;

	//jump data offsetY in pixels
	Int m_OffsetJumpY;

	//used to form footstep
	Int m_FootStepCount;
	Int m_FootStepType;

	//the entity original picture size
	UInt16 m_EntOrgWidth;
	UInt16 m_EntOrgHeight;
		
	//type of blit of the current part
	UInt16 m_CurrPartBlitType;
	
	//entity total rectangle 
	RECT m_EntityTotalRect;

	//overlapped regions with other entities
	StlVector<RECT> m_OverlappedRegion;

	//parent node
	CEntityBase * m_pParentEnt;

	//Follow node
	CEntityBase * m_pFollowTargetEnt;

	//children list
	EntityMap m_pChildrenEntLst;

	//Follower list
	StlVector<CEntityBase *> m_pFollowerEntLst;

	//center poixel position of the current block position
	PixelPos m_EntPixelPos;

	PathFindData m_PathFindData;

	UInt32 m_OffsetMoveXTime;
	UInt32 m_OffsetMoveYTime;

	UInt32 m_PreMoveTime;

	//destination position
	BlockPos m_DesPos;


protected:

	//current part's rectangle
	RECT m_CurrRect;
	//Entity alpha offset
	Byte m_AlphaOffset;
	
	//left up corner poixel position of the entity 
	PixelPos m_EntLeftUpPos;

	//extra Z-Buffer
	Int64 m_ExtraZBuffer;

	//offsetX in pixels
	Int m_OffsetX;

	//offsetY in pixels
	Int m_OffsetY;

	Int m_EntityMoveSpeed;

	Byte m_EntityType;

	//Scale X and Y
	Real32 m_ScaleX;
	Real32 m_ScaleY;

	RECT m_BattleTotalRect;

	BOOL m_Visible;

	BYTE m_OffsetType;

	//offsetX Des in pixels
	Int m_OffsetDesX;

	//offsetY Des in pixels
	Int m_OffsetDesY;

	Int m_InitialMoveYSpeed;
	Int m_GravityAcc;
	UInt32 m_StartTime;
	Int m_OrgPosX;
	Int m_OrgPosY;

	//Shaking info
	Int m_ShakeX;
	Int m_ShakeY;
	Int m_ShakeSpeedX;
	Int m_ShakeSpeedY;
	UInt32 m_ShakePreTimeX;
	UInt32 m_ShakePreTimeY;
	UInt32 m_ShakeStartTimeSpeed;
	UInt32 m_TotalShakeTime;
	Int m_ShakeWidth;
	Int m_ShakeHeight;

	UInt32 m_PicWidth;

	BOOL m_HighLight;

	//cache texture for this entity
	Byte * m_CurrTexture;

	BYTE * m_CurrPicData;

	//character block position
	BlockPos m_CharPos;

	
};

#endif
