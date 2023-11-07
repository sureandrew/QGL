#pragma once
#ifndef _MAPSERVICE_H_
#define _MAPSERVICE_H_
#include "Stub/Service/MapService_Stub.h"

class CMapActor;
class CCharacter;
class CParty;

typedef StlMap<UInt32, UInt16Vector, std::less<UInt32>,
	ManagedAllocator<std::pair<UInt32, UInt16Vector> > > CharIdTitleListMap;

// public functions here
namespace MapService_Stub
{
	void JumpMap(CMapActor *pActor, UInt16 map_id, UInt8 line_id, UInt32 owner_id, 
				UInt16 map_x, UInt16 map_y, UInt16 instId=0);
	Boolean IsInManorInstance(const EnterInfo &info);
	RPCResult LeaveWorld(RPCContext &context); 
};

#endif //_MAPSERVICE_H_
	