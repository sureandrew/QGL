#pragma once
#ifndef _MENUHELPPROCESS_H_
#define _MENUHELPPROCESS_H_

#include "Common.h"

struct NewbieTipsData;
struct NewGuideData;

struct HelpItem
{
	HelpItem * parent;
	HelpItem * right;
	HelpItem * left;
	HelpItem * Leftchild;
	bool isOpen;
	UInt16	contentID;
	String itemName;
	UInt8	level;

	HelpItem()
	{
		parent = NULL;
		right = NULL;
		left = NULL;
		Leftchild = NULL;
		isOpen = false;
		contentID = 0;
		level = 0;
	}
	~HelpItem()
	{
		itemName.Empty();
	}
};
class CMenuHelpProcess
{
public:
	CMenuHelpProcess();
	~CMenuHelpProcess();

	void ConstructData();
	void Release();
	void FreeAllHelpItem(HelpItem * pHelpItem);
	void RefreshMenu();

	void PageUp();
	void PageDown();
	void IsOpenChange(int index);

	HelpItem * GetHelpItemByIndex(int index);
	void ShowTextContent(int index, size_t contentLinePos = 0, UInt len = 0);
	void ShowContent(UInt16 contentID, size_t contentLinePos, UInt len);

	void SearchContent(PCSTRINGW text);

	HelpItem * MoveNextHelpItem(HelpItem * pItem);
	HelpItem * MovePreHelpItem(HelpItem * pItem);

	inline void ResetSearch(){
		m_SearchStartPos = 0;
		m_SearchStartID = 0;
		m_SearchStartText.Empty();
	}

	void SetIndex(Int index);

	void CalOpenCount();

	void ReadHelpTipsFile();
	void WriteHelpTipsFile();
	void CheckHelpTipsEncounter();
	void HandleEvent(UInt8 event);

	bool ReadGuideFile();
	void WriteGuideFile();
	void CheckGuideEncounter();
	void HandleGuideEvent(UInt16 ID);
	void CloseNewGuide(UInt8 flag);

private:
	HelpItem * m_headHelpItem;
	HelpItem * m_CurrentHelpItem;
	HelpItem * m_CurrentHelpLastItem;

	UInt16	m_CurrContentID;
	size_t	m_CurrContentPos;
	size_t	m_CurrContentLine;
	
	UInt16	m_SearchStartID;
	size_t	m_SearchStartPos;
	String	m_SearchStartText;

	UInt32	m_MaxContentID;

	UInt32	m_openCount;
	UInt32	m_CurrIndex;

	UInt16Vector	m_finishHelpTipsID;
	StlMultiMap<UInt16, const NewbieTipsData * >	m_unFinishHelpTipsID;

	UInt16Vector	m_finishGuideID;
	StlMultiMap<UInt16, const NewGuideData * >		m_unFinishGuideID;
	
};


#endif // _MENUSHOPPROCESS_H_