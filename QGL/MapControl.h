#pragma once
#ifndef _MAPCONTROL_H_
#define _MAPCONTROL_H_

#include "Proxy/Logic/Map.h"
#include "Proxy/View/MapView.h"


class CMapControl : public CMapView
{
private:
	CMap * m_pMap;

public:
	CMapControl();
	~CMapControl();

	VOID SetMapObject( CMap * pGivMap);

	virtual VOID NotifyObjectAppeared(NetObject* obj);
	virtual VOID NotifyObjectDisappeared(NetObject* obj);
	

	inline CMap * GetMapObject(){ return m_pMap;}

	

};

#endif //_CHARACTER_H_
