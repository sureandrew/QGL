#pragma once
#ifndef _MENUBUFFICON_H_
#define _MENUBUFFICON_H_

#include "QGL.h"
#include "common_binary.h"

struct BuffData;
class CMenuBuffIconProcess
{
public:
	CMenuBuffIconProcess();
	virtual ~CMenuBuffIconProcess();
	void HideAllBuff();
	void ShowBuffIcon();
	void OpenBuffDetail(int index);
	void HideBuffDetail();

	const BuffData * m_pDetailMenuBuff;
};


#endif 
