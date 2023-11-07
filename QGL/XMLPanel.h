#pragma once
#ifndef _XML_PANEL_H_
#define _XML_PANEL_H_

#include "Render.h"
#include "Font.h"
#include "UI.h"
#include <TinyXML.h>

class XML_UI : public Reuben::Utility::Singleton<XML_UI>
{
	typedef Int32(XML_UI::*UIFunc)(TiXmlNode *, TiXmlElement *);
	typedef std::map<StringA, UIFunc>	FuncMap;

public:
	
	XML_UI();
	~XML_UI();

	SINGLETON(XML_UI);

	bool ReadFormFile(PCSTRINGA XMLFile, UIManager *pUIManager);
	bool ReadFormFile(PCSTRINGA XMLFile);
	bool AddFormFile(PCSTRINGA XMLFile, UIManager *pUIManager);

	UIItem *GetUI(int Index){ return m_UIPanelList[Index];}


	void ClearPanelList(){ m_UIPanelList.clear(); }

	UInt32 GetUISize(){ return  static_cast<UInt32>(m_UIPanelList.size()); }

	lua_State *GetLuaState(){ return m_LuaState; }

	void Init();

private:

	
	

	Int32 ReadElement(PCSTRINGA Parent, TiXmlElement *root, FuncMap &FuncTable);

	Int32 CreateBackground(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 RegisterImage(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetName(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetDefault(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetupLuaState(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetupKeyInItem(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ReflashPanelEx(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 CreatePanel(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIImage(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIAniIcon(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateLinerImage(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateLinerImageV(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateStatic(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIDynamicText(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateButton(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateCheckBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateListBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreatePopUpMenu(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateScrollListBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateEditBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateChatPanel(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIFont(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIMiniMap(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUITips(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetButtonRect(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetCheckBoxRect(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ListBoxAddString(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 PopUpAddString(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 STAddString(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 DTAddString(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetPrevScroll(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetNextScroll(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetMoveScroll(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 ListBoxScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ListBoxScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetPrevChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetNextChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetHomeChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetEndChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddNewLine(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddString(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddStringC(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddLink(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddIcon(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetLuaLButtonUp(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaLButtonDown(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaRButtonUp(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaRButtonDown(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaMouseMove(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaMouseWheel(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaKeyChar(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaKeyUp(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaKeyDown(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaTimeOut(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaUnPickUp(TiXmlNode *pNode, TiXmlElement *pElement);

	void TakeBaseItemAttr(UIItem *pItem, TiXmlElement *pElement);
	void TakeStaticTextAttr(UIStaticText	*pStaticText, TiXmlElement *pElement);
	void TakeDynamicTextAttr(UIDynamicText	*pDynamicText, TiXmlElement *pElement);
	void TakeButtonAttr(TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeCheckBoxAttr(TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeScrollBarAttr(TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeImageAttr(UIImage	*pImage, TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeEntAttr(UIAniIcon	*pAniIcon, TiXmlNode *pNode, TiXmlElement *pElement);

	void TakeChatString(UIChatString *ChatString, TiXmlElement *pElement);
	void TakeChatLink(UIChatLink *ChatLink, TiXmlElement *pElement);
	void TakeChatIcon(UIChatIcon *ChatIcon, TiXmlElement *pElement);

	void SetLuaFuncAttr(UIItem *pItem, TiXmlElement *pElement, PCSTRINGA LuaFuncMsg, UI_LUA_FUNC_CODE Code);

	FuncMap			m_BaseFuncMap;
	FuncMap			m_PanelFuncMap;
	FuncMap			m_ButtonFuncMap;
	FuncMap			m_CheckBoxFuncMap;
	FuncMap			m_ListBoxFuncMap;
	FuncMap			m_PopUpFuncMap;
	FuncMap			m_StatictextFuncMap;
	FuncMap			m_DynamictextFuncMap;
	FuncMap			m_ScrollBarFuncMap;
	FuncMap			m_ScrollListBoxFuncMap;
	FuncMap			m_ChatPanelFuncMap;
	

	///////////////////////////////////////////////
	
	CRender *m_Render;

	StlVector<UIPanel *> m_UIPanelList;
	UIItem	*m_UIItemPtr;	

	UIButton		*m_TempButton;
	UICheckBox		*m_TempCheckBox;
	UIListBox		*m_TempListBox;
	UIPopUpMenu		*m_TempPopUpMenu;
	UIStaticText	*m_TempStaticText;
	UIDynamicText	*m_TempDynamicText;
	UIScrollBarV	*m_TempScrollBar;
	UIScrollListBox	*m_TempScrollListBox;
	UIChatPanel		*m_TempChatPanel;

	UIManager		*m_TempUIManager;
	

	std::vector<UIPanel *>	m_PanelStack;
	std::map<StringA, UInt32>	m_EnumMap;

	

	StringA			m_FontName;
	UInt32			m_FontWidth, m_FontHeight;

	StringA			m_KeyInItemName;

	lua_State		*m_LuaState;
	
};


class XMLBIN_UI : public Reuben::Utility::Singleton<XMLBIN_UI>
{
	typedef Int32(XMLBIN_UI::*UIFunc)(TiXmlNode *, TiXmlElement *);
	typedef std::map<StringA, UIFunc>	FuncMap;

public:
	
	XMLBIN_UI();
	~XMLBIN_UI();

	SINGLETON(XMLBIN_UI);

	bool ReadFormFile(PCSTRINGA XMLFile, PCSTRINGA LuaFile);
	bool AddFormFile(PCSTRINGA XMLFile, PCSTRINGA LuaFile);

	/*
	UIItem *GetUI(int Index)
	{
		return m_UIPanelList[Index];
	}
	*/

	/*
	UInt32 GetUISize()
	{
		return  static_cast<UInt32>(m_UIPanelList.size());
	}
	*/

	/*
	lua_State *GetLuaState()
	{
		return m_LuaState;
	}
	*/

	void Init();

private:

	
	

	Int32 ReadElement(PCSTRINGA Parent, TiXmlElement *root, FuncMap &FuncTable);

	Int32 CreateBackground(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 RegisterImage(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetName(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetDefault(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetupLuaState(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetupKeyInItem(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ReflashPanelEx(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 CreatePanel(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIImage(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIAniIcon(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateLinerImage(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateLinerImageV(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateStatic(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIDynamicText(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateButton(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateCheckBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateListBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreatePopUpMenu(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateScrollListBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateEditBox(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateChatPanel(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIFont(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIMiniMap(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 CreateUIShortCut(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetButtonRect(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetCheckBoxRect(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ListBoxAddString(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 PopUpAddString(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 STAddString(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 DTAddString(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetPrevScroll(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetNextScroll(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetMoveScroll(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 ListBoxScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ListBoxScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetPrevChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetNextChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetHomeChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetEndChat(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddNewLine(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddString(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddStringC(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddLink(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 ChatAddIcon(TiXmlNode *pNode, TiXmlElement *pElement);

	Int32 SetLuaLButtonUp(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaLButtonDown(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaRButtonUp(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaRButtonDown(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaMouseMove(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaMouseWheel(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaKeyChar(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaKeyUp(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaKeyDown(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaTimeOut(TiXmlNode *pNode, TiXmlElement *pElement);
	Int32 SetLuaUnPickUp(TiXmlNode *pNode, TiXmlElement *pElement);

	void TakeBaseItemAttr(TiXmlElement *pElement);
	void TakeStaticTextAttr(TiXmlElement *pElement);
	void TakeDynamicTextAttr(TiXmlElement *pElement);
	void TakeButtonAttr(TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeCheckBoxAttr(TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeScrollBarAttr(TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeImageAttr(TiXmlNode *pNode, TiXmlElement *pElement);
	void TakeEntAttr(TiXmlNode *pNode, TiXmlElement *pElement);

	void TakeChatString(TiXmlElement *pElement);
	void TakeChatLink(TiXmlElement *pElement);
	void TakeChatIcon(TiXmlElement *pElement);

	void SetLuaFuncAttr(TiXmlElement *pElement, PCSTRINGA LuaFuncMsg, UI_LUA_FUNC_CODE Code);

	void OutputFile()
	{
		fwrite(m_TempStr, strlen(m_TempStr), 1, m_fFile);
	}

	FuncMap			m_BaseFuncMap;
	FuncMap			m_PanelFuncMap;
	FuncMap			m_ButtonFuncMap;
	FuncMap			m_CheckBoxFuncMap;
	FuncMap			m_ListBoxFuncMap;
	FuncMap			m_PopUpFuncMap;
	FuncMap			m_StatictextFuncMap;
	FuncMap			m_DynamictextFuncMap;
	FuncMap			m_ScrollBarFuncMap;
	FuncMap			m_ScrollListBoxFuncMap;
	FuncMap			m_ChatPanelFuncMap;
	

	///////////////////////////////////////////////
	
	//CRender *m_Render;

	//StlVector<UIPanel *> m_UIPanelList;
	//UIItem	*m_UIItemPtr;	

	/*
	UIButton		*m_TempButton;
	UICheckBox		*m_TempCheckBox;
	UIListBox		*m_TempListBox;
	UIPopUpMenu		*m_TempPopUpMenu;
	UIStaticText	*m_TempStaticText;
	UIDynamicText	*m_TempDynamicText;
	UIScrollBarV	*m_TempScrollBar;
	UIScrollListBox	*m_TempScrollListBox;
	UIChatPanel		*m_TempChatPanel;
	*/

	//UIManager		*m_TempUIManager;
	

	//std::vector<UIPanel *>	m_PanelStack;
	std::map<StringA, UInt32>	m_EnumMap;

	

	StringA			m_FontName;
	UInt32			m_FontWidth, m_FontHeight;

	StringA			m_KeyInItemName;
	StringA			m_LuaFilename;

	//lua_State		*m_LuaState;
	char			m_TempStr[512];

	FILE			*m_fFile;
	
};



struct CUIFilename
{
	StringA		XMLName;
	StringA		LuaName;

	~CUIFilename()
	{
		XMLName.clear();
		LuaName.clear();
	}
};


class  UINameList : public Reuben::Utility::Singleton<UINameList>
{
public:
	UINameList(){ m_Type = 0; }
	~UINameList(){ Clear(); }
	
	SINGLETON(UINameList);

	void Clear();
	PCSTRINGA UIName(PCSTRINGA Index);
	bool XMLReadFormFile(PCSTRINGA XMLFile);
	void LoadUIFile(UIManager *pUImanager, PCSTRINGA Filename);
	void SetType(Int32 Type){ m_Type = Type;}
	Int32	GetType(){ return	m_Type;}

private:
	Int32		m_Type;
	std::map<StringA, CUIFilename>	m_UIFilename;
};



#endif // _XML_PANEL_H_
