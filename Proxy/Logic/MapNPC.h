#pragma once
#ifndef _MAPNPC_H_
#define _MAPNPC_H_

#include "Proxy/Logic/MapActor.h"
#include "Proxy/Logic/MapNPC_Proxy.h"

class CMapNPCControl;

class CMapNPC : public MapNPC_Proxy
{
private:
	typedef MapNPC_Proxy Parent;

	CMapNPCControl * m_pMapNPCCtrl;

public:
	DEFINE_CLASS(MapNPC_CLASSID);

	CMapNPC();
	virtual ~CMapNPC();

	VOID SetMapNPCControl(CMapNPCControl * pMapNPC);

	VOID ResponseExecuteAll(String filename);
	VOID CancelTalkingTarget();
	VOID OnUpdateVisible(const bool &visible);
// OnUpdate can be implemented selectively
};

#endif //_MAPNPC_H_
