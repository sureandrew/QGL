#pragma once
#ifndef _MAPNPCCONTROL_H_
#define _MAPNPCCONTROL_H_

#include "Proxy/Logic/MapNPC.h"

#include "MapActorControl.h"
#include "FontEntity.h"

class CAnimatedEntity;
class CFontEntity;
struct NPCDB_Client;

class CMapNPCControl : public CMapActorControl
{

protected:
	CMapNPC * m_pMapNPC;
	NPCDB_Client * m_NpCData;

public:

	CMapNPCControl();
	virtual ~CMapNPCControl();
	
	virtual VOID SetMapActorObject(CMapActor* pMapActor);

	virtual UInt32 GetMapActorID();
	UInt32 GetTemplateID();

	Boolean OnClickOnCtrl();
	inline CMapNPC * GetNetObject(){ return m_pMapNPC;}

	inline void SetNPCData(NPCDB_Client * NPCData){ m_NpCData = NPCData;}

	inline NPCDB_Client * GetNPCData(){ return m_NpCData;}

	VOID OnCB_ClickOnCtrl(Boolean bResult);
	VOID StopNPCTalkingState();

	VOID ResponseExecuteAllCtrl(String filename);
	VOID CancelTalkingTargetHandle();

	UInt32 GetNpc_uid() { return GetMapActorID(); }
	UInt32 GetNpc_id() { return GetTemplateID(); }

	void SetEnable(Boolean enable);
};

#endif //_CHARACTER_H_
