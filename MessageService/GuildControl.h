#pragma once
#ifndef _GUILD_CONTROL_H_
#define _GUILD_CONTROL_H_

#include "Reuben/Simulation/Object.h"

#define MAX_GUILD_WAR_SIZE	16

class CGuildControl : 
	public Reuben::Simulation::Object
{
public:
	CGuildControl();
	~CGuildControl();

	VOID GetGuildByID(UInt32 guild_uid);
	RPCResult GetGuildByID( RPCContext &context);

	//only max 16 guild
	VOID GetGuildByPvPGroup();
	RPCResult GetGuildByPvPGroup( RPCContext &context);

	//only max 16 guild
	VOID GenerateGuildPvPGroup(LPCPEER pPeer);
	RPCResult GenerateGuildPvPGroup( RPCContext &context);

	VOID DeleteGuildByID(UInt32 guild_uid);
	RPCResult DeleteGuildByID( RPCContext &context);

	DEFINE_PARENT_CLASS(Reuben::Simulation::Object);
	DEFINE_CLASS(0);			// don't care class id
	
};
#endif // _RANK_CONTROL_H_