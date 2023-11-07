#pragma once
#ifndef _MENUCHARACTERSIMULATOR_H_
#define _MENUCHARACTERSIMULATOR_H_

#include "Common.h"

class CMenuCharacterSimulator
{
private:
	UInt16 sta_add;
	UInt16 spr_add;
	UInt16 str_add;
	UInt16 con_add;
	UInt16 agi_add;
	UInt16 attrib_minus; // negative value
	UInt16 attrib_original;

public:
	CMenuCharacterSimulator()
	{ 
		sta_add = 0;
		spr_add = 0;
		str_add = 0;
		con_add = 0;
		agi_add = 0;
		attrib_minus = 0;
		attrib_original = 0;
	};

	VOID SetOriginalAttrib(UInt16 value)
	{
		attrib_original = value;
	};

	VOID SetStaAdd(Int16 delta)
	{
		if( attrib_minus + delta <= attrib_original && sta_add + delta >= 0 )
		{
			sta_add += delta;
			attrib_minus += delta;
		}
	};

	VOID SetSprAdd(Int16 delta)
	{
		if( attrib_minus + delta <= attrib_original && spr_add + delta >= 0 )
		{
			spr_add += delta;
			attrib_minus += delta;
		}
	};

	VOID SetStrAdd(Int16 delta)
	{
		if( attrib_minus + delta <= attrib_original && str_add + delta >= 0 )
		{
			str_add += delta;
			attrib_minus += delta;
		}
	};

	VOID SetConAdd(Int16 delta)
	{
		if( attrib_minus + delta <= attrib_original && con_add + delta >= 0 )
		{
			con_add += delta;
			attrib_minus += delta;
		}
	};

	VOID SetAgiAdd(Int16 delta)
	{
		if( attrib_minus + delta <= attrib_original && agi_add + delta >= 0 )
		{
			agi_add += delta;
			attrib_minus += delta;
		}
	};

	UInt16 GetStaAdd() {return sta_add;};
	UInt16 GetSprAdd() {return spr_add;};
	UInt16 GetStrAdd() {return str_add;};
	UInt16 GetConAdd() {return con_add;};
	UInt16 GetAgiAdd() {return agi_add;};
	UInt16 GetAttribMinus() {return attrib_minus;};

	VOID Reset()
	{
		sta_add = 0;
		spr_add = 0;
		str_add = 0;
		con_add = 0;
		agi_add = 0;
		attrib_minus = 0;
		attrib_original = 0;
	};
};

#endif //_MENUCHARACTERSIMULATOR_H_