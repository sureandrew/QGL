#pragma once
#ifndef ENTITY_H_
#define ENTITY_H_

#include "EntityBase.h"

const Int AboveHeadOffset = 20;
const Int CommonEntSizX = 512;
const Int CommonEntSizY = 512;

enum EntityOffsetType
{
	ENTITYOFFSETTYPE_NORMAL = 0,
	ENTITYOFFSETTYPE_FOOT,
	ENTITYOFFSETTYPE_CENTER,
	ENTITYOFFSETTYPE_HEAD,
	ENTITYOFFSETTYPE_FULLSCREEN,
	MAX_ENTITYOFFSETTYPE,
};

enum Equip_Type
{
	EQUIP_HEAD = 0,
	EQUIP_BODY,
	EQUIP_HAIR,
	EQUIP_HEAD_ACCESSORY,
	EQUIP_WEAPON,
	EQUIP_MOUNT,
	EQUIP_OTHERS,
};



struct EntityData
{
	//EntityPicInfo * pEntPic;
	String FileName;
	ColorPaletteStruct * pColorP;
	Int CurrFrIndex;
	AnimationFrameData PreFrData;
	UInt32 PreTime;
	Int AnimationIndex;
	Int AnimationRotation;
	BOOL flip;
	Equip_Type EquipType;
	BOOL Destory;
	UInt32 DeleteTime;
	//StlMultiMap<Int, AnimationFrameData> m_EntityHeader;
	StlVector<AnimationFrameData> m_EntityHeader[MAX_ANIM_TYPE];
	BYTE PicType;

	BOOL FrameEnd;
	EntityPicInfo * pEntPic;
	BOOL Free;
	Int ExtendX;
	Int ExtendY;
	EntityData()
	{
		CurrFrIndex = 0;
		PreTime= 0;
		AnimationIndex = 0;
		AnimationRotation = 0;
		flip = FALSE;
		EquipType = EQUIP_HEAD;
		FileName = "";
		pColorP = NULL;
		Destory = FALSE;
		PicType = 0;
		FrameEnd = FALSE;
		pEntPic = NULL;
		DeleteTime = 0;
		Free = FALSE;
		ExtendX = 0;
		ExtendY = 0;
	}
	~EntityData()
	{
		/*for (Int i = 0; i < MAX_ANIM_TYPE; i++)
		{
			if (m_EntityHeader[i])
				SafeDeallocate(m_EntityHeader[i]);
		}*/
	}

	/*EntityData& operator=(const EntityData& r) { 
		
		return *this; 
	} */

};

struct RenderLstData
{
	PicPosData PicData;
	//EntityTextureData Image;
	PicPosData ShadowPicData;
	//EntityTextureData ShadowImage;
	Int EffectType;
	Int BlockBkIndex;
	Int AlphaBkIndex;
	Int ShadowBkIndex;
	Byte PicType;
	BOOL Flip;
	Int OffsetType;
	ColorPaletteStruct * pColorP;

	Int ExtendX;
	Int ExtendY;

	String EntFileName;

	Int OffsetY;

	Byte EquipType;

	RenderLstData()
	{
		memset(this,0, sizeof(RenderLstData));
	}

};


class CAnimatedEntity : public CEntityBase
{
public:
	//construct and distruct
	CAnimatedEntity();
	virtual ~CAnimatedEntity();

	StlVector<BlockPos> GetCurrBlockPositionLst(Int index);

	//set whether to loop the current animation
	inline void SetAnimationLoop(BOOL Loop){ m_AnimationLoop = Loop;};

	//Flip the entity
	VOID FlipEntity();

	//load media file
	BOOL LoadEntityFile(LPCTSTR filename, LPDIRECT3DDEVICE9	pD3DDevice, Equip_Type EquipType = EQUIP_HEAD, BOOL Destory = FALSE,
		UInt32 DeleteTime = 0, BOOL Free = FALSE);

	//get Animation start index by current rotation
	Int GetCurrAnimStartIndexByRotaion(EntityData * pEnData,  Int Rotation, Int AnimationIndex);

	//set current animation index and whether it should be looped
	virtual VOID SetAnimationIndexByIndex(Int Index, BOOL Loop = TRUE, Int GivenRotation = ERROR_INDEX, BOOL flush = FALSE);

	//set the rotation of the entity
	virtual VOID SetRotation(Int Rotation);

	//update the entity data
	virtual VOID UpDate(UInt32 CurrentTime);
	
	//get one part of the entity
	virtual BOOL GetEntityPart(size_t index, Int ImageType, RECT * LockRegion);

	//check whether the entity is flipped
	virtual BOOL GetFlip(Int Index);
	
	//update the rectangle of the entity
	VOID UpDateCurrRect();
	

	//update the animation chain data
	BOOL Updateanimationset(UInt32 CurrentTime);

	//check whether it is the end of the animation
	virtual BOOL FrameEnd(UInt32 CurrentTime);

	//make 32 bits texture from 256 index color
	VOID CopyTextureFrom256Color(Byte * ColorPalette, Byte * ColorPaletteIndex, UInt16 ImageWidth, 
		UInt16 ImageHeight, Int  DescWidth, Byte * pDesTData,  RECT SrcCopyRect, RECT DesCopyRect);

	//make 32 bits texture from 16 bits data
	VOID CopyTextureFrom16Bits(Byte * PictureData, UInt16 ImageWidth, UInt16 ImageHeight,
									   D3DSURFACE_DESC  pDescDes, Byte * pDesTData,  RECT SrcCopyRect, RECT DesCopyRect);

	//get the attack tyoe of the weapon
	int GetWeaponAttackType();

	inline Int GetTotalRenderPart(){ return (Int)m_RenderLst.size();};

	BOOL IsAnimExist(Int AnimIndex);
	//check whether the entity need to have shadow or alpha blending effect 
	virtual inline BOOL GetHaveEffect()
	{
		return m_EntityType == ENTITY_TYPE_MAP_PLAYER || m_EntityType == ENTITY_TYPE_PARTICLE_WITHEFFECT || 
			m_EntityType == ENTITY_TYPE_MAP_CONTROL_PLAYER;
	};

	inline Int GetOppositeRotation() {
		switch(m_AnimationRotation)
		{
		case NORTHWEST:
			return SOUTHEAST;
			break;
		case WEST:
			return EAST;
			break;
		case SOUTHWEST:
			return NORTHEAST;
			break;
		case EAST:
			return WEST;
			break;
		case NORTHEAST:
			return SOUTHWEST;
			break;
		case SOUTHEAST:
			return NORTHWEST;
			break;
		default:
			return ERROR_INDEX;
		}
	};

	
	void SetStopAnimation(BOOL Value);

	virtual inline BOOL GetStopAnimation()
	{
		return m_StopAnim;
	};

	inline BOOL GetIsMapObject()
	{
		return m_EntityType == ENTITY_TYPE_MAP_OBJECT;
	};

	//get the maximum number of frame in current animation
	Int GetCurrAnimationMaxFrame(EntityData * pEnData);

	//get the rectangle reference to world map
	RECT GetMapReferenceTotalRect();

	//random get the index
	VOID RandomCurrFrIndex();

	//init the timing of this entity
	VOID InitTime();

	//Total Time of this Animation
	UInt32 GetTotalTimeOfGivenAnimation(Int GivenAnimationIndex = ERROR_INDEX, Int GivenRotation = ERROR_INDEX);

	//find the time to hit the target
	UInt32Vector FindHitTime(Int GivenAnimationIndex, Int GivenRotation);

	//reset the update time 
	VOID ResetUpdateTime(UInt32 CurrTime);

	virtual inline Int GetAnimationIndex() { return m_AnimationIndex;}

	virtual inline Int GetAnimationRotation() { return  m_AnimationRotation;}

	//get the battle suitable animation
	Int GetBattleAnimation();
	Int GetBattleAttack1Animation();
	Int GetBattleAttack2Animation();

	//remove one part of the entity
	BOOL RemovePart(Equip_Type EQUIPTYPE);

	//remove one part by name
	BOOL RemovePartByName(String Name);

	//get the offset type, normal, foot, center, head, fullscreen
	virtual Int GetOffsetType();

	//get the rectangle of a animation
	RECT GetAnimationTotalRect(Int GivenAnimationIndex);

	VOID InitEntPicInfo();

	EntityPicInfo * GetEntPic(EntityData * EnData);

	VOID ChangeColorP(String EntPartName, String ColorPName);

	virtual inline BOOL GetAnimationLoop(){return m_AnimationLoop;}

	//wait all resources ready
	VOID WaitResReady();

	VOID CheckRemove(UInt32 CurrentTime);

	EntityPicInfo * GetEntPic(UInt Index);

	BOOL FrameEnd(EntityData * pEnData, UInt32 CurrentTime);

	BOOL FrameEndNoOthers(UInt32 CurrentTime);

	VOID SetCurrFrameIndex(Int Index);

	virtual BOOL IsMoving();

	virtual BOOL GetCanBeSelected();

	VOID SetCanBeSelected(BOOL Value);

	VOID SetPlaySpeed(Real32 Value);

	virtual VOID FreeEntityData();

	VOID FindAvailAnimation(EntityData * pEntData);

	VOID AnimToMiddle();

	virtual BYTE GetEquipType(Int Index);

	VOID AppendAnimationSet(Int AnimationIndex, Int UpX, Int UpY, Int height, Real64 TimeInMS,
	BOOL Loop, Int Rotation, UInt32 StartTime, UInt32 JumpDelay, BOOL StopAnim, 
	BOOL IsPixel,//sharking info
	Int ShakeX, Int ShakeY, Int ShakeSpeedX, Int ShakeSpeedY, UInt ShakeWidth, UInt ShakeHeight,UInt ShakeTime);

	VOID ToFrameEnd();

	inline virtual Byte * GetCurrColorP(){ return m_CurrColorPalette;}
	inline virtual Byte GetPicType(){ return m_CurrPicType;}

	VOID LockResources();
	VOID UnLockResources();

	VOID InitChainData();
	VOID PushChainData();
	VOID SetAnimIndexChain(Int AnimationIndex, Int Rotation, BOOL Loop, BOOL StopAnim);
	VOID SetSharkChain(Int ShakeX, Int ShakeY, Int ShakeSpeedX, Int ShakeSpeedY, UInt ShakeWidth, UInt ShakeHeight,UInt ShakeTime);
	VOID SetJumpDataChain(Int height, Int BkPosX, Int BkPosY, Real64 timeInMS, UInt32 StartDelay, BOOL IsPixel);
	VOID SetChainStartTime(UInt32 Time);

	virtual BOOL IsMount(){return  m_MountState > 0;}

	Int ChangeMobAnim(Int Index);
	Int ChangeNorAnim(Int Index);

	inline BOOL HasAniFile(){return m_EnData.size() > 0;}

	inline void SetMountOffset(Int Value){m_MountExtendY = Value;}

	inline BOOL IsPartExist(String Name)
	{
		return m_EnData.size() > 0 && m_EnData.find(Name) != m_EnData.end();
	}

	UInt32 LastAnimSetStartTime();

	String GetPartByEQType(Equip_Type EQUIPTYPE);

	ColorPaletteStruct* GetPartClourByEQType(Equip_Type EQUIPTYPE);

	void ChangePartClourByEQType(Equip_Type EQUIPTYPE,ColorPaletteStruct* clourP);
public:
	
	//render list data including all parts
	StlMultiMap<UInt32, RenderLstData> m_RenderLst;

	//chain of animation
	StlMultiMap<UInt32, animationsetData> m_animationset;

	animationsetData	m_tempAnimSet;

private:

	//entity picture data
	StlMultiMap<String, EntityData> m_EnData;
	
	//Flag for stopping the entity
	BOOL m_StopAnim;

	//current animation index
	Int m_AnimationIndex;

	//current animation rotation
	Int m_AnimationRotation;

	//check whether the animation should be looped
	BOOL m_AnimationLoop;

	//current index of the entity's part
	Int  m_CurrentEntityIndex;

	BOOL m_CanBeSelected;

	Real32 m_PlaySpeed;

	BOOL m_WaitRes;

	BYTE * m_CurrColorPalette;
	BYTE  m_CurrPicType;

	BYTE m_MountState;

	BOOL m_Mounting;

	Int m_MountExtendY;
	
	BOOL m_OthersOnly;
};

struct ChatLst
{	
	CAnimatedEntity * pEnt;
	BYTE action;
	ChatLst * pNext;
	ChatLst()
	{
		pEnt = NULL;
		pNext = NULL;
		action = 0;
	}

	void Init()
	{
		pEnt = NULL;
		pNext = NULL;
		action = 0;
	}

	CAnimatedEntity * GetEntity()
	{
		return pEnt;
	}
	ChatLst * GetNext()
	{
		return pNext;
	}
	BYTE GetAction()
	{
		return action;
	}
};

#endif