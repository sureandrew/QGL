#pragma once
#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include "Camera.h"
#include "EntityBase.h"
class CAnimatedEntity;
class CFontEntity;
class CImageEntity;
class CUIEntity;

struct ParticleStructSetting
{
	Int NoOfParticle; 
	Int MoveSpeed; 
	PixelDirection MoveDir; 
	Int LiveTimeBase; 
	Int LiveTimeVar; 
	BOOL Loop;
	Int GivenDisposalTime; 
	Int GivenWaitTime; 
	Int GivenDisposalAnim; 
	Int64 ExtraZValue;
	BOOL Destory;
	BOOL Stop;
	BOOL HaveEffect;
	BOOL ResetOutSideScreen;
	Int Rotation;
	Int SubOffsetX;
	Int SubOffsetY;
	Int StartAlphaOffset;
	BOOL FreeAfterFinish;
	UInt32 StartDelay;
	UInt32 StartTime;

	ParticleStructSetting()
	{
		memset(this, 0, sizeof(ParticleStructSetting));
	}
};
struct ParticleStruct
{
	CEntityBase * pEnt;
	UInt32 LiveTime;
	UInt32 StartTime; 
	Int DisposalTime;
	Int WaitTime;
	Int DisposalAnim;
	BOOL DestoryAfterFinish;
	BOOL FreeAfterFinish;
	BOOL ResetOutSideScreen;

	ParticleStruct()
	{
		memset(this, 0, sizeof(ParticleStruct));
	}
};

//inherit from Entity Class
class CParticleSystem 
{
	public:
	//constrtuct and distruct
	CParticleSystem();
	~CParticleSystem();

	//particle list
	StlMap<CEntityBase *, ParticleStruct> m_ParticleEntLst;
private:
	
	//udpate time
	UInt32 m_PreUpDateTimeY;
	UInt32 m_PreUpDateTimeX;

	//speed of the Particle Effect
	Int m_Speed;

	//direction of the Particle Effect
	PixelDirection m_Dir;
	
	//the bounding effect
	RECT m_EffectBounding;

	//effect type including rain, fog, snow
	Int m_EffectType;

	//Target can be Null
	CEntityBase * m_pEntTarget;

	//position data for reborn the particle
	PixelPos m_ParSysPos;

	//rectangle for the effect bounding, (0, 0) is the center of the target
	RECT m_TargetRelativeBound; 

public:

	//set the effect type for this effect
	inline VOID SetEffectType(Int GivenEffectType){ m_EffectType = GivenEffectType;};

	//get the effect type for this effect
	inline Int GetEffectType(){return m_EffectType;};

	//get the direction of the effect
	inline PixelDirection GetDirection(){return m_Dir;};

	//set the particle system target
	inline void SetTarget(CEntityBase* pEnt){ m_pEntTarget = pEnt;};

	inline void SetRelativeEffectBounding(RECT GivenBound){ m_TargetRelativeBound = GivenBound;};

	//update the effect
	virtual VOID UpDate(UInt32 CurrentTime);

	//random pick start position
	VOID RanPickPos(CEntityBase * pEnt, BOOL Outside);

	//set starting data
	VOID AddAnimatedEntityToSystem(CAnimatedEntity * pGivenEnt, 
		ParticleStructSetting Setting, CEntityBase * pRefEntPos, BOOL SetParent = FALSE);
	VOID AddImageEntityToSystem(CImageEntity * pGivenEnt, ParticleStructSetting Setting, CEntityBase * pRefEntPos, BOOL SetParent = FALSE);
	VOID AddFontEntityToSystem(CFontEntity * pGivenEnt, ParticleStructSetting Setting, CEntityBase * pRefEntPos, BOOL SetParent = FALSE);

	//only allow one entity
	VOID AddUIEntityToSystem(CUIEntity * pGivenEnt, ParticleStructSetting Setting, CEntityBase * pRefEntPos, BOOL SetParent = FALSE);
	VOID SetBaseAttri(ParticleStruct & ParStr, ParticleStructSetting Setting, CEntityBase * pRefEntPos, BOOL SetParent = FALSE);
	BOOL OutSideBounding(CEntityBase * pEnt);

	VOID Reset();

	VOID ClearLst();


	
};
#endif