#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <Reuben/Math/Random.h>
#include <Reuben/Math/AES.h>
#include <Reuben/Math/CRC.h>
#include "Resource/ServerResManager.h"

struct Global
{
	CONST Listener* pWanListener;
	CONST Listener* pLanListener;
	Service*	pWorldService;
	Service*	pMessageService;
	LPCPEER		pManagerPeer;
	Boolean		bExit; // exit flag
	Reuben::Math::Random random; // random number	
	CServerResManager resource;

	Global(VOID)
		: pWanListener(NULL)
		, pLanListener(NULL)
		, pWorldService(NULL)
		, pMessageService(NULL)
		, pManagerPeer(NULL)
		, bExit(BOOLEAN_FALSE)
	{}

	Boolean Initialize(VOID)
	{
		random.Randomize();	
		resource.LoadAntihackDAT();	
		return (BOOLEAN_TRUE);
	}

	VOID Uninitialize(VOID)
	{
		resource.~CServerResManager();
	}

};

extern Global gGlobal;

#endif // _GLOBAL_H_
