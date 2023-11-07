#pragma once
#ifndef _MAPVIEW_H_
#define _MAPVIEW_H_

#include "Proxy/Logic/Character.h"
class CCharacter;

class CMapView
{
public:

	virtual VOID NotifyObjectAppeared(NetObject* obj) = 0;
	virtual VOID NotifyObjectDisappeared(NetObject* obj) = 0;

};

#endif