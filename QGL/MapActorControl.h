#pragma once
#ifndef _MAPACTORCONTROL_H_
#define _MAPACTORCONTROL_H_

#include "Proxy/Logic/MapActor.h"
#include "FontEntity.h"
#include "AnimatedEntity.h"

class CMapActor;
class CAnimatedEntity;
class CFontEntity;
class CUIEntity;

struct ChatBalloonData
{
	CUIEntity * pBalloon;
	UInt32 StartTime;

	ChatBalloonData()
	{
		pBalloon = NULL;
		StartTime = 0;
	}
};

enum MAPACTORTYPE
{
	MAPACTORTYPE_UNDEFINE = 0,
	MAPACTORTYPE_PLAYER,
	MAPACTORTYPE_NPC,
};
class CMapActorControl 
{

protected:

	CAnimatedEntity m_CharMainView;
	CAnimatedEntity m_CharShadowView;
	CAnimatedEntity m_CharHeadIconView;
	CFontEntity m_CharNameView;
	CFontEntity m_CharFrontNameView;

	Byte m_MapActorType;

	UInt16 m_State;
	
	StlVector<ChatBalloonData> m_ChatBalloonLst;

	UInt32 m_AnimIdleChangeCounter;
	UInt32 m_AnimIdleChangeTime;

	UInt8 m_InitRotation;

	Color32	m_OrgFontColor;
	Color32	m_OrgFrontFontColor;

public:

	CMapActorControl();
	virtual ~CMapActorControl();

	CMapActor * m_pMapActor;
	State*		m_mapState;			// [leave, enter, inside]

	inline CAnimatedEntity* GetCharMainView(){ return &m_CharMainView;};
	inline CAnimatedEntity* GetCharShadowView(){ return &m_CharShadowView;};
	inline CAnimatedEntity* GetHeadIconView(){ return &m_CharHeadIconView;};
	inline CFontEntity* GetCharNameView(){ return &m_CharNameView;};
	inline CFontEntity* GetCharFrontNameView(){ return &m_CharFrontNameView;};



	//init the shadow data
	VOID InitShadow();

	//load default part of the entity
	VOID LoadDefaultPart();
	VOID LoadDefaultPart2();

	VOID SetEntityCurrPos(UInt16 x, UInt16 y);
	VOID SetEntityDesPos(UInt16 x, UInt16 y, BOOL SendToServer = TRUE);
	VOID Stop();	//stop moving the character

	//reset the update state of the entity
	VOID ResetStatus();

	//update the view data
	VOID UpDateView(UInt32 CurrTime);

	virtual VOID OnUpdateInfo(const String &Name, BYTE FontFlag);
	virtual VOID OnUpdateCurPos();
	virtual VOID OnUpdateDstPos();
	virtual VOID OnUpdateSpeed();
	virtual VOID OnUpdateFollowId(const UInt32 &followId, const UInt8 &followOrder);
	virtual VOID OnUpdateAppeared();

	String GetCharName();

	VOID RemoveFollowTarget();
	VOID FollowTarget(CMapActorControl * Target, UInt8 followOrder);

	virtual VOID SetMapActorObject(CMapActor* pMapActor);

	RPCNetID GetMapActorNetID();

	VOID LoadDefaultHeadIcon();
	VOID RemoveIcon();
	void SetHeadIcon(UInt8 iconType);

	VOID LoadDefaultPart(UInt ModelID);
	VOID LoadDefaultMob(const String & modelFilename, const String & modelColor);

	virtual VOID OnUpdateAction(const UInt16 &action, const UInt32 &actionTime);

	virtual Byte GetMapActorType(){return m_MapActorType;}

	virtual VOID AddChatBalloon(CUIEntity * pBalloon, UInt32 delayTime = 0);

	inline StlVector<ChatBalloonData> * GetChatBalloonLst(){return &m_ChatBalloonLst;}

	VOID ChangeRotation(Int Rotation);
	VOID SetInitRotation(UInt8 rotation) {m_InitRotation = rotation;};
	UInt8 GetInitRotation() {return m_InitRotation;}; 

	virtual UInt32 GetMapActorID();

	virtual VOID SetFontColor(Color32 Color);
	inline Color32 GetFontColor(){return m_OrgFontColor;};

	virtual VOID SetFrontFontColor(Color32 Color);
	inline Color32 GetFrontFontColor(){return m_OrgFrontFontColor;};


	VOID ResetNamePosition();

	VOID SetFrontName(const String & Name, BYTE FontFlag);	

	bool IsHide();
	virtual bool IsLeader(){return false;};
	virtual void RefreshFollower() {}

	UInt8 GetSex();
	UInt8 GetCclass();
	UInt8 GetSexClass();
	UInt8 GetFaction();
	UInt16 GetMap_id();
	UInt16 GetCurPosX();
	UInt16 GetCurPosY();
	UInt16 GetDstPosX();
	UInt16 GetDstPosY();
	UInt16 GetAction();
	UInt32 GetSpeed();
	UInt32 GetFollowId();
};

#endif //_CHARACTER_H_
