#pragma once
#ifndef _TEXTENTMANAGER_H_
#define _TEXTENTMANAGER_H_
class CAnimatedEntity;

//remember to deallocate the list after used!
namespace TextEntManager
{
	VOID CreateEntityNumber(StlVector<CAnimatedEntity *> & NumLst, Int Num, LPDIRECT3DDEVICE9	pD3DDevice, 
		BYTE AddSymbolIndex, PCSTRINGW NumFileName, PCSTRINGW SymbolFileName);
}
#endif