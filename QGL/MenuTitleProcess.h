#pragma once
#ifndef _MENUTITLE_H_
#define _MENUTITLE_H_

#include <Reuben\Network\Network.h>


class CMenuTitleProcess
{
public:
	CMenuTitleProcess();
	~CMenuTitleProcess();

	void RefreshMenu();
	void PageUp();
	void PageDown();
	void Sroll();
	void SetSelectIndex(Int index);
	void ConfirmChange();
	void HideAllText();

	StlMap<UInt16, String>	m_titleDataLst;

	UInt16	m_currIndex;
	UInt16	m_currSelIndex;
};

#endif 
