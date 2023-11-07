#pragma once
#ifndef _CHARACTERVIEW_H_
#define _CHARACTERVIEW_H_
class CharacterView
{
public:

	virtual VOID OnUpdateDstPos(VOID) = 0;
	virtual VOID OnUpdateInfo(VOID) = 0;
};

#endif