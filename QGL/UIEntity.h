#pragma once
#ifndef UI_ENTITY_H_
#define UI_ENTITY_H_

#include "EntityBase.h"
#include "UI.h"

class CUIEntity : public CEntityBase
{
	
public:
	CUIEntity();
	~CUIEntity();

	virtual BOOL GetEntityPart(size_t index, Int ImageType, RECT * LockRegion);
	virtual Int GetTotalRenderPart();
	virtual BOOL GetHaveEffect();

	//update the entity data
	virtual VOID UpDate(UInt32 CurrentTime);

	//VOID SetPanelData(UIPanel * pPanel);

	VOID ReadXMLData(PCSTRINGA FileName);

	//inline StlVector<StringA> * GetXMLFileNameLst(){return &m_XMLFileNameLst;}
	
	virtual VOID		SetChatPanel(UIChatPanel *pChatPanel){m_pChatPanel = pChatPanel;}
	virtual UIChatPanel *GetChatPanel(){return m_pChatPanel;}
	virtual VOID		SetPanelEx(UIPanelEx *pPanelEx){m_pPanelEx = pPanelEx;}
	virtual UIPanelEx	*GetPanelEx(){return m_pPanelEx;}
	virtual VOID		SetMsg(PCSTRING Msg){m_Msg = Msg;}
	virtual PCSTRING	GetMsg(){return m_Msg.c_str();}
	
	virtual VOID		SetupPanel();

	VOID ResetRect();


	//inline UIManager* GetUIManager(){ return m_pUIManager;}

protected:
	//UIManager* m_pUIManager;
	UIChatPanel *m_pChatPanel;
	UIPanelEx	*m_pPanelEx;
	String		m_Msg;
	//StlVector<StringA> m_XMLFileNameLst;

};
#endif