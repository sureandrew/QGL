#pragma once
#ifndef _ANTIHACK_MANAGER_H_
#define _ANTIHACK_MANAGER_H_
#include "Reuben/Math/Random.h"

struct GdfMapViewKey;
class GDFManager;

class CAntiHackManager
{
public:

	typedef StlVector<GdfMapViewKey*, ManagedAllocator<GdfMapViewKey*> > MapViewKeyVector;
	
public:

	CAntiHackManager();
	~CAntiHackManager();

	VOID GetHackImageInfo(UInt32 picId, BYTE*& mapViewPtr, BYTE*& imagePtr, UInt32& imageSize);
	VOID ReleaseHackImagePtr(BYTE* mapViewPtr);
	UInt32 GetHackDataSize();
	VOID Release();

private:

	BOOL LoadData();
	GDFManager* m_gdfManager;
	MapViewKeyVector m_mapViewKey;
	Reuben::Math::Random m_rand;

};

#endif // _ANTIHACK_MANAGER_H__
