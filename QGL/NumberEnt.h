#pragma once
#ifndef _NUMBERENT_H_
#define _NUMBERENT_H_
#include "AnimatedEntity.h"
#include "QGL.h"

class CNumEnt
{
public:
	CNumEnt();
	~CNumEnt();
	VOID SetNumber(UInt32 Num);
	VOID SetFileNameW(PCSTRINGW FileName);

	VOID SetEntPixelPos(PixelPos Pos);

	StlVector<CAnimatedEntity *> m_NumEntLst;

	VOID SetVisible(BOOL Value);

	VOID SetExtraZValue(Int64 Value);

	VOID Release();
private:
	CharW	m_NumFileName[64];
	Int m_Number;
	PixelPos m_CurrPixelPos;
	Int64 m_ExtraZValue;
};

#endif