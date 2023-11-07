#pragma once
#ifndef _MAPNPC_H_
#define _MAPNPC_H_

#include "Proxy/Logic/MapActor.h"
#include "Proxy/Logic/MapNPC_Proxy.h"

class CMapNPC : public MapNPC_Proxy
{
private:
	typedef MapNPC_Proxy Parent;

public:
	DEFINE_CLASS(MapNPC_CLASSID);

	void ShowSummary(int i);
	virtual UInt32 GetActorId() { return GetNpc_uid(); }
	virtual String DebugInfo();

// OnUpdate can be implemented selectively
};

#endif //_MAPNPC_H_
