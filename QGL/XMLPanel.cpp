//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "XMLPanel.h"
#include "Global.h"


XML_UI::XML_UI()
{
	m_BaseFuncMap["ID"]				= &XML_UI::SetName;
	m_BaseFuncMap["Image"]			= &XML_UI::RegisterImage;
	m_BaseFuncMap["Background"]		= &XML_UI::CreateBackground;
	m_BaseFuncMap["Panel"]			= &XML_UI::CreatePanel;
	m_BaseFuncMap["Button"]			= &XML_UI::CreateButton;
	m_BaseFuncMap["Font"]			= &XML_UI::CreateUIFont;
	m_BaseFuncMap["Lua"]			= &XML_UI::SetupLuaState;
	m_BaseFuncMap["KeyIn"]			= &XML_UI::SetupKeyInItem;
	m_BaseFuncMap["Default"]		= &XML_UI::SetDefault;
	m_BaseFuncMap["ReflashPanelEx"]	= &XML_UI::ReflashPanelEx;
	
	

	//m_PanelFuncMap["Panel"]		= CreatePanel;
	m_PanelFuncMap["UIImage"]		= &XML_UI::CreateUIImage;
	m_PanelFuncMap["AniIcon"]		= &XML_UI::CreateUIAniIcon;
	m_PanelFuncMap["LinerImage"]	= &XML_UI::CreateLinerImage;
	m_PanelFuncMap["LinerImageV"]	= &XML_UI::CreateLinerImageV;
	m_PanelFuncMap["StaticText"]	= &XML_UI::CreateStatic;
	m_PanelFuncMap["Button"]		= &XML_UI::CreateButton;
	m_PanelFuncMap["CheckBox"]		= &XML_UI::CreateCheckBox;
	m_PanelFuncMap["ListBox"]		= &XML_UI::CreateListBox;
	m_PanelFuncMap["PopUpMenu"]		= &XML_UI::CreatePopUpMenu;
	m_PanelFuncMap["ScrollBarV"]	= &XML_UI::CreateScrollBarV;
	m_PanelFuncMap["ScrollBarH"]	= &XML_UI::CreateScrollBarH;
	m_PanelFuncMap["ScrollListBox"]	= &XML_UI::CreateScrollListBox;
	m_PanelFuncMap["EditBox"]		= &XML_UI::CreateEditBox;
	m_PanelFuncMap["ChatPanel"]		= &XML_UI::CreateChatPanel;
	m_PanelFuncMap["MiniMap"]		= &XML_UI::CreateUIMiniMap;
	m_PanelFuncMap["DynamicText"]	= &XML_UI::CreateUIDynamicText;
	m_PanelFuncMap["Tips"]			= &XML_UI::CreateUITips;

	
	


	m_ButtonFuncMap["ButtonRect"]	= &XML_UI::SetButtonRect;

	m_CheckBoxFuncMap["CheckBoxRect"]	= &XML_UI::SetCheckBoxRect;

	m_ListBoxFuncMap["ListBoxString"]	= &XML_UI::ListBoxAddString;

	m_PopUpFuncMap["PopUpMenuString"]	= &XML_UI::PopUpAddString;

	m_StatictextFuncMap["STSubString"] = &XML_UI::STAddString;
	m_DynamictextFuncMap["DTSubString"] = &XML_UI::DTAddString;

	m_ScrollBarFuncMap["PrevButton"]	= &XML_UI::SetPrevScroll;
	m_ScrollBarFuncMap["NextButton"]	= &XML_UI::SetNextScroll;
	m_ScrollBarFuncMap["MoveButton"]	= &XML_UI::SetMoveScroll;

	m_ScrollListBoxFuncMap["String"]		= &XML_UI::ListBoxAddString;
	m_ScrollListBoxFuncMap["ScrollBarV"]	= &XML_UI::ListBoxScrollBarV;
	m_ScrollListBoxFuncMap["ScrollBarH"]	= &XML_UI::ListBoxScrollBarH;

	m_ChatPanelFuncMap["PrevButton"]	= &XML_UI::SetPrevChat;
	m_ChatPanelFuncMap["NextButton"]	= &XML_UI::SetNextChat;
	m_ChatPanelFuncMap["HomeButton"]	= &XML_UI::SetHomeChat;
	m_ChatPanelFuncMap["EndButton"]		= &XML_UI::SetEndChat;
	m_ChatPanelFuncMap["NewLine"]		= &XML_UI::ChatAddNewLine;
	m_ChatPanelFuncMap["String"]		= &XML_UI::ChatAddString;
	m_ChatPanelFuncMap["Link"]			= &XML_UI::ChatAddLink;
	m_ChatPanelFuncMap["Icon"]			= &XML_UI::ChatAddIcon;
	m_ChatPanelFuncMap["ChatString"]	= &XML_UI::ChatAddStringC;


	m_EnumMap["FONT1"] = EDGE_FONT1;
	m_EnumMap["FONT2"] = EDGE_FONT2;
	m_EnumMap["SHADOW"] = EDGE_SHADOW;
	m_EnumMap["LIGHT"] = EDGE_LIGHT;

	
	Init();
}


XML_UI::~XML_UI()
{
	m_UIPanelList.clear();
	m_PanelStack.clear();
	m_EnumMap.clear();
}


void XML_UI::Init()
{
	m_UIPanelList.clear();
	m_UIItemPtr = NULL;

	m_TempButton = NULL;
	m_TempCheckBox = NULL;
	m_TempListBox = NULL;
	m_TempPopUpMenu = NULL;
	m_TempStaticText = NULL;
	m_TempScrollBar = NULL;
	m_TempScrollListBox = NULL;
	m_TempChatPanel = NULL;

	m_LuaState = NULL;

	m_Render = NULL;
	
	m_TempUIManager = NULL;
	

	m_PanelStack.clear();
	
	m_FontName.clear();
	m_FontWidth = m_FontHeight = 0;

	m_KeyInItemName.clear();

}


bool XML_UI::ReadFormFile(PCSTRINGA XMLFile, UIManager *pUIManager)
{
	AddFormFile(XMLFile, pUIManager);

	if(!pUIManager->GetLuaShareFlag())
	{
		pUIManager->SetupLua(m_LuaState);

		if(m_LuaState)
		{
			if(lua_pcall(m_LuaState, 0, 0, 0))
			{
				TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(m_LuaState, -1)).c_str());
			}
		}
	}

	
	return true;
}


bool XML_UI::AddFormFile(PCSTRINGA XMLFile, UIManager *pUIManager)
{
	Init();
	TiXmlDocument doc( XMLFile );

	if(pUIManager->GetLuaShareFlag())
		m_LuaState = pUIManager->GetLuaState();

	String tempxmlfile = XMLFile;
	
	if( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(tempxmlfile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(tempxmlfile, beginOffset, fileSize);
		if( fbuf )
		{
			if( fileSize > 0 )
			{
				BYTE* buf = fbuf + beginOffset;
				doc.Parse((char*)buf);
			}
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		doc.LoadFile();
	}
	m_Render = pUIManager->GetRender();
	m_LuaState = pUIManager->GetLuaState();
	
	TiXmlHandle	docH( &doc );
	TiXmlElement* root = doc.FirstChildElement( );

	m_TempUIManager = pUIManager;

	pUIManager->ClearWaitDelPanel();
	m_UIPanelList.clear();
	
	if ( root )
	{
		ReadElement(root->Value(), root, m_BaseFuncMap);
	}

	
	for(UInt32 i = 0; i < m_UIPanelList.size(); ++i)
	{
		pUIManager->AddUIPanel(m_UIPanelList[i]);
	}

	if(!m_KeyInItemName.empty())
	{
		pUIManager->SetKeyInItem(m_KeyInItemName.c_str());
	}


	m_UIPanelList.clear();

	m_TempUIManager = NULL;
	return true;
}


bool XML_UI::ReadFormFile(PCSTRINGA XMLFile)
{
	Init();
	TiXmlDocument doc( XMLFile );

	String tempxmlfile = XMLFile;
	
	if( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(tempxmlfile, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(tempxmlfile, beginOffset, fileSize);
		if( fbuf )
		{
			if( fileSize > 0 )
			{
				BYTE* buf = fbuf + beginOffset;
				doc.Parse((char*)buf);
			}
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		doc.LoadFile();
	}
	
	TiXmlHandle	docH( &doc );
	TiXmlElement* root = doc.FirstChildElement( );

	m_UIPanelList.clear();
	
	if ( root )
	{
		ReadElement(root->Value(), root, m_BaseFuncMap);
	}

	m_TempUIManager = NULL;
	return true;

}



Int32 XML_UI::ReadElement(PCSTRINGA Parent, TiXmlElement *root, FuncMap &FuncTable)
{
	TiXmlElement	*Element = root->FirstChildElement();
	TiXmlNode		*Node = Element;

	Int32		Index = 0;
	while (Element)
	{
		PCSTRINGA NodeValue = Node->Value();

		FuncMap::iterator it = FuncTable.find(NodeValue);

		if(it != FuncTable.end())
		{
			UIFunc	P1 = it->second;
			(this->*P1)(Node, Element);
		}
		else
		{
			ReadElement(Node->Value(), Element, FuncTable);

		}
		
		
		Element = (TiXmlElement *)Element->NextSibling();
		Node = Element;
		Index++;
	}

	return Index;
}

Int32 XML_UI::SetupLuaState(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA LuaFilename = pElement->Attribute("Filename");
	lua_State			*LuaState = NULL;


	if(!m_LuaState)
	{
		gGlobal.InitUILua(LuaState);
	}
	else
	{
		LuaState = m_LuaState;
	}

	if( g_GdfResource || g_GdfResourceEx )
	{
		String tempfilename = const_cast<char*>(LuaFilename);
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
	    if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(tempfilename, beginOffset, fileSize);
	    if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(tempfilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			int error = 1;
			if( fileSize > 0 )
			{
				error  = luaL_loadbuffer(LuaState, (char*)buf, fileSize, "line");
			}
			if (error != 0)
			{
				LuaFilename =  lua_tostring(LuaState, -1);
			}
			else
			{
				m_LuaState = LuaState;
			}
			UnmapViewOfFile(fbuf);
		}
		else
		{
			LuaFilename =  lua_tostring(LuaState, -1);
		}
	}
	else
	{
		if(luaL_loadfile(LuaState, LuaFilename))
		{
			LuaFilename =  lua_tostring(LuaState, -1);
		}
		else
		{
			m_LuaState = LuaState;
		}
	}

	return 1;
}



Int32 XML_UI::SetupKeyInItem(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_KeyInItemName = pElement->Attribute("Item");

	return 1;
}


Int32 XML_UI::CreateBackground(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32	Left, Top, Right, Bottom;

	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);

	
	
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		StringW Filename;

		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);

		m_TempUIManager->SetBackImage(Image, Left, Top, Right, Bottom);
	}

	return 1;
}


Int32 XML_UI::RegisterImage(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA ImageName = pElement->Attribute("Name");
	if(ImageName)
	{
		StringW Filename;

		UTF82StringW(Filename, ImageName);

		m_TempUIManager->RegisterImage(Filename.c_str());
	}

	
	return 1;
}


Int32 XML_UI::SetDefault(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Name = pElement->Attribute("Item");
	if(Name)
	{
		m_TempUIManager->ChangeFocus(Name);
	}

	Name = pElement->Attribute("Enter");
	if(Name)
	{
		m_TempUIManager->SetDefaultEnterFunc(Name);
	}

	return 1;
}


Int32 XML_UI::ReflashPanelEx(TiXmlNode *pNode, TiXmlElement *pElement)
{

	return 1;
}


Int32 XML_UI::SetName(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Name = pElement->Attribute("Name");
	if(Name)
	{
		m_TempUIManager->SetIDName(Name);
	}

	return 1;
}


void XML_UI::SetLuaFuncAttr(UIItem *pItem, TiXmlElement *pElement, PCSTRINGA LuaFuncMsg, UI_LUA_FUNC_CODE Code)
{
	PCSTRINGA FuncName = pElement->Attribute(LuaFuncMsg);
	if(FuncName)
	{
		pItem->SetLuaFunc(Code, FuncName);
	}
}




void XML_UI::TakeBaseItemAttr(UIItem	*pItem, TiXmlElement *pElement)
{
	UInt32 IColor = HexToDWORD(pElement->Attribute("IColor"));

	if(IColor)
	{
		pItem->SetColor(IColor);
	}

	PCSTRINGA IDName = pElement->Attribute("Name");
	if(IDName && m_TempUIManager)
	{
		m_TempUIManager->AddUIID(IDName, pItem);
		pItem->SetName(IDName);
	}

	PCSTRINGA NextName = pElement->Attribute("Next");
	if(NextName)
	{
		pItem->SetNext(NextName);
	}

	PCSTRINGA PrevName = pElement->Attribute("Prev");
	if(PrevName)
	{
		pItem->SetPrev(PrevName);
	}

	Int32 IsFlag;
	pElement->Attribute("Hide", &IsFlag);
	pItem->SetEnable(IsFlag ? false : true);


	{
		Int32	Drag;
		pElement->Attribute("Drag", &Drag);

		if(Drag)
		{
			pItem->SetCanDrag(Drag);

			Int32	DLeft, DTop, DRight, DBottom;

			pElement->Attribute("DragLeft", &DLeft);
			pElement->Attribute("DragTop", &DTop);
			pElement->Attribute("DragRight", &DRight);
			pElement->Attribute("DragBottom", &DBottom);

			pItem->SetDragRect(DLeft, DTop, DRight, DBottom); 
		}
	}	

	Int32 NotDetected;
	pElement->Attribute("NotDetected", &NotDetected);
	pItem->SetNotDetected(NotDetected > 0);

	Int32	DetectImage= 0;
	pElement->Attribute("DetectImage", &DetectImage);
	pItem->SetDetectImage(DetectImage > 0);

	Int32 InsertToEventList = 0;
	pElement->Attribute("InsertToEventList", &InsertToEventList);
	if (InsertToEventList > 0)
		m_TempUIManager->InsertToInOutEvent(pItem);


	PCSTRINGA HexColor = pElement->Attribute("Color");
	if(HexColor)
		pItem->SetColor(HexToDWORD(HexColor));

	Int32	RenderStyle;
	pElement->Attribute("RenderStyle", &RenderStyle);
	pItem->SetRenderStyle(RenderStyle);
	
	SetLuaFuncAttr(pItem, pElement, "LuaLButtonUp", UI_FUNC_LBUTTON_UP);
	SetLuaFuncAttr(pItem, pElement, "LuaLButtonDown", UI_FUNC_LBUTTON_DOWN);
	SetLuaFuncAttr(pItem, pElement, "LuaRButtonUp", UI_FUNC_RBUTTON_UP);
	SetLuaFuncAttr(pItem, pElement, "LuaRButtonDown", UI_FUNC_RBUTTON_DOWN);
	SetLuaFuncAttr(pItem, pElement, "LuaMouseMove", UI_FUNC_MOUSE_MOVE);

	SetLuaFuncAttr(pItem, pElement, "LuaMouseWheel", UI_FUNC_MOUSE_WHEEL);
	SetLuaFuncAttr(pItem, pElement, "LuaKeyChar", UI_FUNC_CHAR);
	SetLuaFuncAttr(pItem, pElement, "LuaKeyUp", UI_FUNC_KEYUP);
	SetLuaFuncAttr(pItem, pElement, "LuaKeyDown", UI_FUNC_KEYDOWN);
	SetLuaFuncAttr(pItem, pElement, "LuaTimeOut", UI_FUNC_TIMEOUT);
	SetLuaFuncAttr(pItem, pElement, "LuaUnPickUp", UI_FUNC_UNPICKUP);

	SetLuaFuncAttr(pItem, pElement, "LuaStartDrag", UI_FUNC_STARTDRAG);
	SetLuaFuncAttr(pItem, pElement, "LuaEndDrag", UI_FUNG_ENDDRAG);

	SetLuaFuncAttr(pItem, pElement, "LuaMouseOut", UI_FUNC_MOUSE_OUT);
	SetLuaFuncAttr(pItem, pElement, "LuaMouseIn", UI_FUNC_MOUSE_IN);

	SetLuaFuncAttr(pItem, pElement, "LuaLButtonDBLCLK", UI_FUNC_LBUTTONDBLCLK);
	SetLuaFuncAttr(pItem, pElement, "LuaRButtonDBLCLK", UI_FUNC_RBUTTONDBLCLK);

	PCSTRINGA UILuaFunc = pElement->Attribute("UILuaFunc");
	if(UILuaFunc)
	{
		m_TempUIManager->SetLuaFuncName(UILuaFunc);
	}


	PCSTRINGA Tips = pElement->Attribute("Tips");
	
	if(Tips)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Tips);
		pItem->SetTips(TempStr.c_str());
		
	}

	Int32 TimeOutCount = 0;
	pElement->Attribute("TimeOutCount", &TimeOutCount);
	if (TimeOutCount > 0)
	{
		pItem->SetTimeOutCount(TimeOutCount);
		if(!pItem->IsDefaultTips())
			m_TempUIManager->AddUpdateUIItem(pItem);
	}
	else
		pItem->SetTimeOutCount(0);

	Int32 DisableTimeOutEvent = 0;
	pElement->Attribute("DisableTimeOutEvent", &DisableTimeOutEvent);
	pItem->SetEnableTimeOutEvent(DisableTimeOutEvent == 0);


	pItem->SetLuaState(m_LuaState);
}

Int32 XML_UI::CreatePanel(TiXmlNode *pNode, TiXmlElement *pElement)
{
	UIPanel			*Panel = NULL;
	UIPanelEx		*PanelEx = NULL;
	Int32 X, Y; 
	Int32	IsExpand;
	bool	ExFlag;

		
	pElement->Attribute("Expand", &IsExpand);

	if(!IsExpand)
	{
		Panel = SafeCreateObject(UIPanel);
		ExFlag = false;
	}
	else
	{
		Panel = PanelEx = SafeCreateObject(UIPanelEx);
		ExFlag = true;
	}

	UInt32 BColor = HexToDWORD(pElement->Attribute("BackColor"));
	Panel->SetBackColor(BColor);

	BColor = HexToDWORD(pElement->Attribute("BoxColor"));
	Panel->SetBoxColor(BColor);

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	TakeBaseItemAttr(Panel, pElement);

	Int32	Left, Top, Right, Bottom;

	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);

	
	Int32 TWidth, THeight; 
	pElement->Attribute("TWidth", &TWidth);
	pElement->Attribute("THeight", &THeight);

	Int		DragWidth, DragHeight;
	pElement->Attribute("DragWidth", &DragWidth);
	pElement->Attribute("DragHeight", &DragHeight);

	Int32 NotRemove = 0;
	pElement->Attribute("NotRemove", &NotRemove);

	if(NotRemove)
		Panel->SetCanRemove(false);

	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		StringW Filename;
		
		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		
		Panel->SetImage(Image, Left, Top, Right, Bottom);

		Int32 CheckWidth = Right - Left;
		Int32 CheckHeight = Bottom - Top;

		if(ExFlag)
		{
			pElement->Attribute("BottomRectLeft", &Left);
			pElement->Attribute("BottomRectTop", &Top);
			pElement->Attribute("BottomRectRight", &Right);
			pElement->Attribute("BottomRectBottom", &Bottom);

			CheckWidth = max(CheckWidth, Right - Left);
			CheckHeight += Bottom - Top;

			PanelEx->SetBottomRect(Left, Top, Right, Bottom);

			pElement->Attribute("LineRectLeft", &Left);
			pElement->Attribute("LineRectTop", &Top);
			pElement->Attribute("LineRectRight", &Right);
			pElement->Attribute("LineRectBottom", &Bottom);

			CheckWidth = max(CheckWidth, Right - Left);

			Int32	Count; 
			
			pElement->Attribute("LineCount", &Count);

			CheckHeight += (Bottom - Top) * Count;

			PanelEx->SetLineRect(Left, Top, Right, Bottom, Count);

			TWidth = max(TWidth, CheckWidth);
			THeight = max(THeight, CheckHeight);

			PanelEx->SetTextureWH(TWidth, THeight);
		}
		else
		{
			TWidth = max(TWidth, (Int32)Panel->GetWidth());
			THeight = max(THeight, (Int32)Panel->GetHeight());

			Panel->SetTextureWH(TWidth, THeight);
		}
	}
	else
	{
		Panel->SetImage(NULL, Left, Top, Right, Bottom);
		
		TWidth = max(TWidth, (Int32)Panel->GetWidth());
		THeight = max(THeight, (Int32)Panel->GetHeight());

		Panel->SetTextureWH(TWidth, THeight);
	}

	if(DragWidth == -1)
		Panel->SetDragWidth(TWidth);
	else
	if(DragWidth == -2)
		Panel->SetDragWidth(0);
	else
	if(DragWidth > 0)
		Panel->SetDragWidth(DragWidth);

	if(DragHeight == -1)
		Panel->SetDragHeight(THeight);
	else
	if(DragHeight == -2)
		Panel->SetDragHeight(0);
	else
	if(DragHeight > 0)
		Panel->SetDragHeight(DragHeight);
	
	
	Int		AutoDelete;

	pElement->Attribute("AutoDelete", &AutoDelete);
	Panel->SetAutoDelete(AutoDelete ? true : false);

	PCSTRINGA LuaPerLButtonUp = pElement->Attribute("LuaPerLButtonUp");
	if(LuaPerLButtonUp)
		Panel->SetLuaPerLButtonUp(LuaPerLButtonUp);

	PCSTRINGA LuaPerRButtonUp = pElement->Attribute("LuaPerRButtonUp");
	if(LuaPerRButtonUp)
		Panel->SetLuaPerRButtonUp(LuaPerRButtonUp);


	PCSTRINGA ToPanelName = pElement->Attribute("ToPanel");
	UIPanel *ToPanelPtr = (m_TempUIManager ? m_TempUIManager->FindUIPanel(ToPanelName) : NULL);

	if(ToPanelPtr)
	{
		ToPanelPtr->AddUIItem(Panel);
		m_PanelStack.push_back(Panel);
	}	
	else
	{
		if(m_PanelStack.size() > 0)
		{
			m_PanelStack.back()->AddUIItem(Panel);
			m_PanelStack.push_back(Panel);
		}
		else
		{
			m_PanelStack.push_back(Panel);
			m_UIPanelList.push_back(Panel);
		}
	}

	PCSTRINGA FollowPanelName = pElement->Attribute("FollowPanel");

	if(FollowPanelName)
	{
		UIPanel *FollowPanelPtr = m_TempUIManager->FindUIPanel(FollowPanelName);

		if(FollowPanelName && FollowPanelPtr)
		{
			FollowPanelPtr->SetFallowPanel(Panel);
		}
	}
	
	ReadElement(pNode->Value(), pElement, m_PanelFuncMap);
	
	m_PanelStack.pop_back();

	Panel->SetPos(X, Y);
	Panel->SetDetectRect();
	if(m_TempUIManager)
		Panel->SetTexture(m_TempUIManager->GetRender()->GetDevice());
	
		
	if(ToPanelPtr)
	{
		Panel->SetOffsetXY(X - ToPanelPtr->GetX(), Y - ToPanelPtr->GetY());
	}

	return 1;
}

void XML_UI::TakeEntAttr(UIAniIcon	*pAniIcon, TiXmlNode *pNode, TiXmlElement *pElement)
{
	TakeBaseItemAttr(pAniIcon, pElement);
	PCSTRINGA ImageName = pElement->Attribute("Image");

	Int32	Rotation, AnimationIndex, Loop, Align, Shadow, Detect, Lock, EquipType;

	pElement->Attribute("Rotation", &Rotation);
	pElement->Attribute("AnimationIndex", &AnimationIndex);
	pElement->Attribute("Loop", &Loop);
	pElement->Attribute("Align", &Align);
	pElement->Attribute("Shadow", &Shadow);
	pElement->Attribute("Detect", &Detect);
	pElement->Attribute("LockResources", &Lock);
	pElement->Attribute("EquipType", &EquipType);

	pAniIcon->SetAlign(Align);
	pAniIcon->SetAnimationIndex(AnimationIndex);
	pAniIcon->SetRotation(Rotation);
	pAniIcon->SetDetect(Detect);
	if(ImageName)
	{
		pAniIcon->SetAnimationData(String(ImageName).c_str(), EquipType);	
		if (Lock)
			pAniIcon->GetAniEnt()->LockResources();
	}
}


void  XML_UI::TakeImageAttr(UIImage	*pImage, TiXmlNode *pNode, TiXmlElement *pElement)
{
	

	TakeBaseItemAttr(pImage, pElement);

	Int32	Left, Top, Right, Bottom;

	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);

	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		StringW Filename;
		
		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		
		pImage->SetImage(Image, Left, Top, Right, Bottom);
	}
	else
	{
		pImage->SetImage(NULL, Left, Top, Right, Bottom);

		PCSTRINGA ZipImageName = pElement->Attribute("ZipImage");
		if(ZipImageName)
		{
			pImage->LoadCZipImage(ZipImageName);
		}
	}

	Int		AutoDelete;
	pElement->Attribute("AutoDelete", &AutoDelete);

	pImage->SetAutoDelete(AutoDelete ? true : false);

}



Int32 XML_UI::CreateUIImage(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	UIImage			*MyImage = SafeCreateObject(UIImage);

	Int32 X, Y; 

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	
	TakeImageAttr(MyImage, pNode, pElement);
	
	MyImage->SetOffsetXY(X, Y);
	MyImage->SetDetectRect();
	

	m_PanelStack.back()->AddUIItem(MyImage);


	return 1;
}




Int32 XML_UI::CreateUIAniIcon(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	
	UIAniIcon	*AniIcon = SafeCreateObject(UIAniIcon);
	Int32 X, Y; 

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	TakeEntAttr(AniIcon, pNode, pElement);

	AniIcon->SetOffsetXY(X, Y);
	AniIcon->SetDetectRect();
	

	m_PanelStack.back()->AddUIItem(AniIcon);
	m_TempUIManager->AddUpdateUIItem(AniIcon);


	return 1;
}


Int32 XML_UI::CreateLinerImage(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	UILinerImage	*LinerImage = SafeCreateObject(UILinerImage);
	Int32 X, Y; 

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	TakeImageAttr(LinerImage, pNode, pElement);

	Int32	Width;

	pElement->Attribute("LineWidth", &Width);
	LinerImage->SetLinerWidth(Width);

	pElement->Attribute("BaseWidth", &Width);
	if(Width)
		LinerImage->SetBaseWidth(Width);
	else
		LinerImage->SetBaseWidth(LinerImage->GetLinerWidth());

	pElement->Attribute("Denumerator", &Width);
	if(Width == 0)
	{
		LinerImage->SetDenumerator(100);
		LinerImage->SetNumerator(100);
	}
	else
	{
		LinerImage->SetDenumerator(Width);

		pElement->Attribute("Numerator", &Width);
		LinerImage->SetNumerator(Width);
	}
	

	LinerImage->SetOffsetXY(X, Y);
	LinerImage->SetDetectRect();
	
		
	m_PanelStack.back()->AddUIItem(LinerImage);

	return 1;
}


Int32 XML_UI::CreateLinerImageV(TiXmlNode *pNode, TiXmlElement *pElement)
{
	UILinerImageV	*LinerImageV = SafeCreateObject(UILinerImageV);
	Int32 X, Y; 

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	TakeImageAttr(LinerImageV, pNode, pElement);

	Int32	Height;

	pElement->Attribute("LineHeight", &Height);
	LinerImageV->SetLinerHeight(Height);

	pElement->Attribute("BaseHeight", &Height);
	if(Height)
		LinerImageV->SetBaseHeight(Height);
	else
		LinerImageV->SetBaseHeight(LinerImageV->GetLinerHeight());

	/*
	pElement->Attribute("Denumerator", &Width);
	if(Width == 0)
	{
		LinerImage->SetDenumerator(100);
		LinerImage->SetNumerator(100);
	}
	else
	{
		LinerImage->SetDenumerator(Width);

		pElement->Attribute("Numerator", &Width);
		LinerImage->SetNumerator(Width);
	}
	*/
	

	LinerImageV->SetOffsetXY(X, Y);
	LinerImageV->SetDetectRect();
	
		
	m_PanelStack.back()->AddUIItem(LinerImageV);

	

	return 1;
}


void XML_UI::TakeStaticTextAttr(UIStaticText	*pStaticText, TiXmlElement *pElement)
{
	PCSTRINGA FontName = pElement->Attribute("FontName");
	PCSTRINGA EdgeCode = pElement->Attribute("EdgeCode");
	UInt32 FontColor = HexToDWORD(pElement->Attribute("FontColor"));
	UInt32 ShadowColor = HexToDWORD(pElement->Attribute("ShadowColor"));
	PCSTRINGA Align = pElement->Attribute("Align");
	
	Int32 X, Y;
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	Int32 Row, Column;
	pElement->Attribute("Row", &Row);
	pElement->Attribute("Column", &Column);

	Int32 Width, Height;
	pElement->Attribute("Width", &Width);
	pElement->Attribute("Height", &Height);


	Int32 IsPassword;
	pElement->Attribute("Password", &IsPassword);

	Int32 NumOnly;
	pElement->Attribute("NumOnly", &NumOnly);

	if(FontName)
	{
		Int32		FontWidth;
		Int32		FontHeight;
		Int32		FontCache;
	
		pElement->Attribute("FontWidth", &FontWidth);
		pElement->Attribute("FontHeight", &FontHeight);
		pElement->Attribute("FontCache", &FontCache);
		pStaticText->SetFont(FontName, FontWidth, FontHeight);
	}
	else
	{
		pStaticText->SetFont(m_FontName.c_str(), m_FontWidth, m_FontHeight);
	}
	

	if(Align)
	{
		if(!strcmpi(Align, "Right"))
		{
			pStaticText->SetAlign(UIStaticText::ALIGN_RIGHT);
		}
		else
		if(!strcmpi(Align, "Center"))
		{
			pStaticText->SetAlign(UIStaticText::ALIGN_CENTER);
		}

	}

	Int32	SelStart, SelEnd;

	pElement->Attribute("SelStart", &SelStart);
	pElement->Attribute("SelEnd", &SelEnd);

	pStaticText->SetSelStart(SelStart);
	pStaticText->SetSelEnd(SelEnd);

	if(pElement->Attribute("SelColor"))
	{
		UInt32 SelColor = HexToDWORD(pElement->Attribute("SelColor"));
		pStaticText->SetSelColor(SelColor);
	}

	if(pElement->Attribute("SelHeight"))
	{
		Int32 SelHeight;

		pElement->Attribute("SelHeight", &SelHeight);
		pStaticText->SetSelHeight(SelHeight);
	}


	Int32	LinePitch;

	pElement->Attribute("LinePitch", &LinePitch);
	pStaticText->SetLinePitch(LinePitch);

	
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Text);
		pStaticText->SetText(TempStr.c_str());
	}


	pStaticText->SetOffsetXY(X, Y);
	
	if(EdgeCode)
		pStaticText->SetFontStyle(m_EnumMap[EdgeCode]);
	
	pStaticText->SetFontColor(FontColor);
	pStaticText->SetShadowColor(ShadowColor);
	if(Width > 0 && Height > 0)
		pStaticText->SetImageWH(Width, Height);
	else
	if(Row > 0 && Column > 0)
		pStaticText->SetImage(Row, Column);
	else
		pStaticText->SetImageWH(pStaticText->GetFont()->GetWidth() * 32 + 30, pStaticText->GetFont()->GetHeight() * 2);
	
	pStaticText->SetPassword(IsPassword ? true : false);
	pStaticText->SetNumOnly(NumOnly ? true : false);

}



void XML_UI::TakeDynamicTextAttr(UIDynamicText	*pDynamicText, TiXmlElement *pElement)
{
	Int32 Speed, MoveWidth;
	pElement->Attribute("Speed", &Speed);
	pDynamicText->SetSpeed(Speed);

	pElement->Attribute("MoveWidth", &MoveWidth);
	pDynamicText->SetMoveWidth(MoveWidth);
}

Int32 XML_UI::CreateStatic(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	UIStaticText	*StaticText = SafeCreateObject(UIStaticText);

	TakeBaseItemAttr(StaticText, pElement);
	TakeStaticTextAttr(StaticText, pElement);

	m_TempStaticText = StaticText;
	ReadElement(pNode->Value(), pElement, m_StatictextFuncMap);
	m_TempStaticText = NULL;

	StaticText->DrawFont();
	Int textDetect = 0;
	pElement->Attribute("textDetect", &textDetect);
	//assume all static text is not detected
	if (textDetect == 0)
		StaticText->SetNotDetected(true);
	m_PanelStack.back()->AddUIItem(StaticText);

	return 1;
}

Int32 XML_UI::CreateUIDynamicText(TiXmlNode *pNode, TiXmlElement *pElement)
{
	
	if(m_PanelStack.empty())
		return 0;

	UIDynamicText	*DynamicText = SafeCreateObject(UIDynamicText);

	TakeBaseItemAttr(DynamicText, pElement);
	TakeStaticTextAttr(DynamicText, pElement);
	TakeDynamicTextAttr(DynamicText, pElement);

	m_TempDynamicText = DynamicText;
	ReadElement(pNode->Value(), pElement, m_DynamictextFuncMap);
	m_TempDynamicText = NULL;

	DynamicText->DrawFont();
	m_PanelStack.back()->AddUIItem(DynamicText);
	m_TempUIManager->AddUpdateUIItem(DynamicText);

	return 1;
}

void XML_UI::TakeButtonAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32 X, Y;
	
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		StringW Filename;

		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		m_TempButton->SetImage(Image);
	}

	
	
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	m_TempButton->SetOffsetXY(X, Y);
	//m_TempButton->SetState(_ButtonNormal);

	ReadElement(pNode->Value(), pElement, m_ButtonFuncMap);
	
	if(m_PanelStack.size() <= 0)
	{
		m_TempButton->SetPos(X, Y);
	}

	
	m_TempButton->SetStateDetectRect();
	
	

	
}


Int32 XML_UI::CreateButton(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	m_TempButton = SafeCreateObject(UIButton);
	
	TakeBaseItemAttr(m_TempButton, pElement);
	TakeButtonAttr(pNode, pElement);
	
	if(m_PanelStack.size() > 0)
	{
		m_PanelStack.back()->AddUIItem(m_TempButton);

	}
	/*
	else
	{
		m_UIPanelList.push_back(m_TempButton);
	}
	*/

	m_TempButton = NULL;

	return 1;
}


void XML_UI::TakeCheckBoxAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32 X, Y, CheckX, CheckY, State;
	
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		StringW Filename;

		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		m_TempCheckBox->SetImage(Image);
	}

	
	
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	pElement->Attribute("CheckX", &CheckX);
	pElement->Attribute("CheckY", &CheckY);
	m_TempCheckBox->SetOffsetXY(X, Y);
	m_TempCheckBox->SetCheckXY(CheckX, CheckY);

	pElement->Attribute("State", &State);
	m_TempCheckBox->SetState(State ? true : false);

	ReadElement(pNode->Value(), pElement, m_CheckBoxFuncMap);
	
	if(m_PanelStack.size() <= 0)
	{
		m_TempCheckBox->SetPos(X, Y);
	}

	m_TempCheckBox->SetDetectRect();
	
	


}


Int32 XML_UI::CreateCheckBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	m_TempCheckBox = SafeCreateObject(UICheckBox);

	TakeBaseItemAttr(m_TempCheckBox, pElement);
	TakeCheckBoxAttr(pNode, pElement);
	
	if(m_PanelStack.size() > 0)
	{
		m_PanelStack.back()->AddUIItem(m_TempCheckBox);

	}
	/*
	else
	{
		m_UIPanelList.push_back(m_TempCheckBox);
	}
	*/

	m_TempCheckBox = NULL;

	return 1;
}


Int32 XML_UI::CreateListBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	m_TempListBox = SafeCreateObject(UIListBox);

	TakeBaseItemAttr(m_TempListBox, pElement);

	TakeStaticTextAttr(m_TempListBox, pElement);

	Int32 BarWidth;

	pElement->Attribute("BarWidth", &BarWidth);
	m_TempListBox->SetBarWidth(BarWidth);

	{
		UInt32 SelBarColor = HexToDWORD(pElement->Attribute("SelBarColor"));

		if(pElement->Attribute("SelBarColor"))
		{
			m_TempListBox->SetSelBarColor(SelBarColor);
		}

		UInt32 SelFontColor = HexToDWORD(pElement->Attribute("SelFontColor"));

		if(pElement->Attribute("SelFontColor"))
		{
			m_TempListBox->SetSelFontColor(SelFontColor);
		}
		else
		{
			m_TempListBox->SetSelFontColor(0xFF00FF00);
		}
	}

	ReadElement(pNode->Value(), pElement, m_ListBoxFuncMap);
	
	m_TempListBox->DrawFont();
	m_PanelStack.back()->AddUIItem(m_TempListBox);

	m_TempListBox->SetDetectRect();

	m_TempListBox = NULL;

	return 1;
}


Int32 XML_UI::CreatePopUpMenu(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	m_TempPopUpMenu = SafeCreateObject(UIPopUpMenu);

	TakeBaseItemAttr(m_TempPopUpMenu, pElement);

	TakeStaticTextAttr(m_TempPopUpMenu, pElement);

	Int32 BarWidth;

	pElement->Attribute("BarWidth", &BarWidth);
	m_TempPopUpMenu->SetBarWidth(BarWidth);

	{
		UInt32 SelBarColor = HexToDWORD(pElement->Attribute("SelBarColor"));

		if(pElement->Attribute("SelBarColor"))
		{
			m_TempPopUpMenu->SetSelBarColor(SelBarColor);
		}

		UInt32 SelFontColor = HexToDWORD(pElement->Attribute("SelFontColor"));

		if(pElement->Attribute("SelFontColor"))
		{
			m_TempPopUpMenu->SetSelFontColor(SelFontColor);
		}
		else
		{
			m_TempPopUpMenu->SetSelFontColor(0xFF00FF00);
		}
	}

	Int32	LinePitch;

	pElement->Attribute("LinePitch", &LinePitch);
	m_TempPopUpMenu->SetLinePitch(LinePitch);


	ReadElement(pNode->Value(), pElement, m_PopUpFuncMap);
	
	m_TempPopUpMenu->DrawFont();
	m_PanelStack.back()->AddUIItem(m_TempPopUpMenu);

	m_TempPopUpMenu->SetDetectRect();

	m_TempListBox = NULL;

	return 1;
}


Int32	XML_UI::SetPrevScroll(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempButton = m_TempScrollBar->GetPrevBtn();
	m_TempButton->SetSimple(true);
	TakeBaseItemAttr(m_TempButton, pElement);
	TakeButtonAttr(pNode, pElement);
	m_TempButton = NULL;

	return 1;
}


Int32	XML_UI::SetNextScroll(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempButton = m_TempScrollBar->GetNextBtn();
	m_TempButton->SetSimple(true);
	TakeBaseItemAttr(m_TempButton, pElement);
	TakeButtonAttr(pNode, pElement);
	m_TempButton = NULL;

	return 1;
}


Int32	XML_UI::SetMoveScroll(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempButton = m_TempScrollBar->GetMoveBtn();
	m_TempButton->SetSimple(true);
	TakeBaseItemAttr(m_TempButton, pElement);
	TakeButtonAttr(pNode, pElement);
	m_TempButton = NULL;

	return 1;
}


void XML_UI::TakeScrollBarAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32 X, Y;
	Int32	Width, Height;
	Int32 Limit;
	UInt32 BarColor = HexToDWORD(pElement->Attribute("BarColor"));

	

	
	m_TempScrollBar->InitButton();
	
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	pElement->Attribute("Width", &Width);
	pElement->Attribute("Height", &Height);
	pElement->Attribute("Limit", &Limit);

	

	/*
	if(m_TempScrollListBox && m_TempListBox)
	{
		if(!strcmp(pElement->Attribute("X"), "BoxX"))
			X = m_TempListBox->GetX();
		else
		if(!strcmp(pElement->Attribute("X"), "BoxX+Width"))
			X = m_TempListBox->GetX() + m_TempListBox->GetWidth();
		
		if(!strcmp(pElement->Attribute("Y"), "BoxY"))
			Y = m_TempListBox->GetY();
		else
		if(!strcmp(pElement->Attribute("Y"), "BoxY+Height"))
			Y = m_TempListBox->GetY() + m_TempListBox->GetHeight();

		if(!strcmp(pElement->Attribute("Width"), "BoxWidth"))
			Width = m_TempListBox->GetWidth();
			
		if(!strcmp(pElement->Attribute("Height"), "BoxHeight"))
			Height = m_TempListBox->GetHeight();
		
	}
	*/


	ReadElement(pNode->Value(), pElement, m_ScrollBarFuncMap);
	

	//m_TempScrollBar->SetPos(X, Y);
	m_TempScrollBar->SetBarWH(Width, Height);
	m_TempScrollBar->SetBarInfo();
	m_TempScrollBar->SetOffsetXY(X, Y);

	m_TempScrollBar->SetNumCount(Limit);
	
	m_TempScrollBar->SetDetectRect();
	m_TempScrollBar->SetBarColor(BarColor);
	m_TempScrollBar->SetBarRenderType(1);

	m_TempScrollBar->SetLuaState(m_LuaState);

	m_PanelStack.back()->AddUIItem(m_TempScrollBar);
	
}


Int32 XML_UI::CreateScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	m_TempScrollBar = SafeCreateObject(UIScrollBarV);

	TakeBaseItemAttr(m_TempScrollBar, pElement);
	TakeScrollBarAttr(pNode, pElement);
	m_TempScrollBar = NULL;
	
	return 1;
}


Int32 XML_UI::CreateScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	m_TempScrollBar = SafeCreateObject(UIScrollBarH);

	TakeBaseItemAttr(m_TempScrollBar, pElement);
	TakeScrollBarAttr(pNode, pElement);
	m_TempScrollBar = NULL;

	return 1;
}


Int32 XML_UI::ListBoxScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempScrollListBox->SetupScrollBarV();
	m_TempScrollBar = m_TempScrollListBox->GetScrollBarV();
	TakeScrollBarAttr(pNode, pElement);
	m_TempScrollBar = NULL;

	return 1;
}


Int32 XML_UI::ListBoxScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempScrollListBox->SetupScrollBarH();
	m_TempScrollBar = m_TempScrollListBox->GetScrollBarH();
	TakeScrollBarAttr(pNode, pElement);
	m_TempScrollBar = NULL;

	return 1;
}



Int32 XML_UI::CreateScrollListBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	m_TempScrollListBox	= SafeCreateObject(UIScrollListBox);

	TakeBaseItemAttr(m_TempScrollListBox, pElement);

	m_TempListBox = m_TempScrollListBox->GetListBox();
	TakeStaticTextAttr(m_TempListBox, pElement);

	ReadElement(pNode->Value(), pElement, m_ScrollListBoxFuncMap);
	
	m_TempListBox->DrawFont();
	m_PanelStack.back()->AddUIItem(m_TempListBox);

	m_TempScrollListBox	= NULL;
	m_TempListBox = NULL;

	return 1;
}


Int32 XML_UI::CreateEditBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	UIEditBox	*EditBox;
	UIEditBoxEx	*EditBoxEx;
	Int32	IsExpand;
	bool	ExFlag;

	pElement->Attribute("Expand", &IsExpand);

	if(!IsExpand)
	{
		EditBox = SafeCreateObject(UIEditBox);
		ExFlag = false;
	}
	else
	{
		EditBox = EditBoxEx = SafeCreateObject(UIEditBoxEx);
		ExFlag = true;
	}

	TakeBaseItemAttr(EditBox, pElement);

	Int32 EditLimit;
	pElement->Attribute("Limit", &EditLimit);
	EditBox->SetEditLimit(EditLimit);

	Int32 ReadPast;
	pElement->Attribute("ReadPast", &ReadPast);
	EditBox->SetReadPastStr(ReadPast ? true : false);

	Int32 NoSpace;
	pElement->Attribute("NoSpace", &NoSpace);
	EditBox->SetNoSpace(NoSpace ? true : false);

	TakeStaticTextAttr(EditBox, pElement);
	
	EditBox->DrawFont();
	m_PanelStack.back()->AddUIItem(EditBox);
	m_TempUIManager->AddUpdateUIItem(EditBox);

	return 1;
}


Int32	XML_UI::SetPrevChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempButton = m_TempChatPanel->CreatePrevBtn();
	TakeButtonAttr(pNode, pElement);
	m_TempButton = NULL;

	return 1;
}


Int32	XML_UI::SetNextChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempButton = m_TempChatPanel->CreateNextBtn();
	TakeButtonAttr(pNode, pElement);
	m_TempButton = NULL;
	return 1;
}


Int32	XML_UI::SetHomeChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempButton = m_TempChatPanel->CreateHomeBtn();
	TakeButtonAttr(pNode, pElement);
	m_TempButton = NULL;
	return 1;
}


Int32	XML_UI::SetEndChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempButton = m_TempChatPanel->CreateEndBtn();
	TakeButtonAttr(pNode, pElement);
	m_TempButton = NULL;
	return 1;
}


Int32 XML_UI::ChatAddNewLine(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_TempChatPanel->CreateRow();

	return 1;
}


void XML_UI::TakeChatString(UIChatString *ChatString, TiXmlElement *pElement)
{
	UInt32 FontColor = HexToDWORD(pElement->Attribute("FontColor"));
	UInt32 ShadowColor = HexToDWORD(pElement->Attribute("ShadowColor"));
	PCSTRINGA EdgeCode = pElement->Attribute("EdgeCode");
	

	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Text);
		ChatString->SetString(TempStr.c_str());
		
	}

	ChatString->SetFontColor(FontColor);
	ChatString->SetShadowColor(ShadowColor);
	if(EdgeCode)
		ChatString->SetFontStyle(m_EnumMap[EdgeCode]);
}


Int32 XML_UI::ChatAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(!m_TempChatPanel->GetLastRow())
		m_TempChatPanel->CreateRow();
	UIChatRow *LastRow = m_TempChatPanel->GetLastRow();
	UIChatString *ChatString = SafeCreateObject(UIChatString);
	TakeChatString(ChatString, pElement);
	LastRow->AddItem(ChatString);

	return 1;
}


Int32 XML_UI::ChatAddStringC(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Text);
		m_TempChatPanel->AddStringW(TempStr.c_str());
	}

	return 1;
}


void XML_UI::TakeChatLink(UIChatLink *ChatLink, TiXmlElement *pElement)
{
	PCSTRINGA EdgeCode = pElement->Attribute("EdgeCode");
	

	PCSTRINGA Link = pElement->Attribute("LinkTo");
	Int32 Len = MultiByteToWideChar(CP_UTF8, 0, Link, -1, NULL, 0);
	PSTRINGW TempStr = SafeAllocate(CharW, Len);
	MultiByteToWideChar(CP_UTF8, 0, Link, -1, TempStr, Len);
	ChatLink->SetLinkString(TempStr);
	SafeDeallocate(TempStr);

	TakeChatString(ChatLink, pElement);
}



Int32 XML_UI::ChatAddLink(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(!m_TempChatPanel->GetLastRow())
		m_TempChatPanel->CreateRow();
	UIChatRow *LastRow = m_TempChatPanel->GetLastRow();
	UIChatLink *ChatLink = SafeCreateObject(UIChatLink);
	TakeChatLink(ChatLink, pElement);
	LastRow->AddItem(ChatLink);

	return 1;
}


void XML_UI::TakeChatIcon(UIChatIcon *ChatIcon, TiXmlElement *pElement)
{
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		Int32	Left, Top, Right, Bottom;

		StringW Filename;

		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	
		pElement->Attribute("Left", &Left);
		pElement->Attribute("Top", &Top);
		pElement->Attribute("Right", &Right);
		pElement->Attribute("Bottom", &Bottom);

		ChatIcon->SetImage(Image);
		ChatIcon->SetRect(Left, Top, Right, Bottom);
	}

	TakeChatLink(ChatIcon, pElement);
}


Int32 XML_UI::ChatAddIcon(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(!m_TempChatPanel->GetLastRow())
		m_TempChatPanel->CreateRow();
	UIChatRow *LastRow = m_TempChatPanel->GetLastRow();
	UIChatIcon *ChatIcon = SafeCreateObject(UIChatIcon);
	TakeChatIcon(ChatIcon, pElement);

	LastRow->AddItem(ChatIcon);

	return 1;
}


Int32 XML_UI::CreateChatPanel(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	Int32		Width, Height;
	Int32		ContinueRun;
	Int32		EditFlag;
	Int32		LinePitch;
	Int32		MaxLine;

	pElement->Attribute("Width", &Width);
	pElement->Attribute("Height", &Height);
	pElement->Attribute("ContinueRun", &ContinueRun);
	pElement->Attribute("Edit", &EditFlag);
	pElement->Attribute("LinePitch", &LinePitch);
	pElement->Attribute("MaxLine", &MaxLine);

	if(EditFlag)
	{
		m_TempChatPanel = SafeCreateObject(UIChatEditPanel);

		Int		UseEnter;
		pElement->Attribute("UseEnter", &UseEnter);
		((UIChatEditPanel *)m_TempChatPanel)->SetUseEnter(UseEnter);

		Int		UseLimit;
		pElement->Attribute("Limit", &UseLimit);
		((UIChatEditPanel *)m_TempChatPanel)->SetLimit(UseLimit);
	}
	else
		m_TempChatPanel = SafeCreateObject(UIChatPanel);
	TakeBaseItemAttr(m_TempChatPanel, pElement);
	TakeStaticTextAttr(m_TempChatPanel, pElement);
	m_TempChatPanel->SetImage(Width, Height);
	m_TempChatPanel->SetContinueRun(ContinueRun > 0);
	m_TempChatPanel->SetLinePitch(LinePitch);
	if(MaxLine)
		m_TempChatPanel->SetMaxLine(MaxLine);

	ReadElement(pNode->Value(), pElement, m_ChatPanelFuncMap);
	
	m_TempChatPanel->HomeRow();

	m_TempChatPanel->FillImage();
	m_TempChatPanel->Redraw();
	m_TempChatPanel->UpdateDetectRect();

	m_PanelStack.back()->AddUIItem(m_TempChatPanel);
	if(m_TempUIManager)
		m_TempUIManager->AddUpdateUIItem(m_TempChatPanel);

	Int32 X, Y; 

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	m_TempChatPanel->SetOffsetXY(X, Y);
	m_TempChatPanel->SetDetectRect();

	Int32 DisableScroll;

	pElement->Attribute("DisableScroll", &DisableScroll);
	m_TempChatPanel->SetEnableScroll(DisableScroll ? false : true);

	
	m_TempChatPanel = NULL;

	return 1;
}


Int32 XML_UI::CreateUITips(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	UITips *Tips = SafeCreateObject(UITips);

	TakeBaseItemAttr(Tips, pElement);

	Int32 X, Y, Width, Height; 
	
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	Tips->SetOffsetXY(X, Y);

	pElement->Attribute("Width", &Width);
	pElement->Attribute("Height", &Height);
	Tips->SetWH(Width, Height);

	m_PanelStack.back()->AddUIItem(Tips);
	

	return 1;
}


Int32 XML_UI::CreateUIMiniMap(TiXmlNode *pNode, TiXmlElement *pElement)
{
	if(m_PanelStack.empty())
		return 0;

	

	//PCSTRINGA ImageName = pElement->Attribute("MapImage");
	//if(ImageName)
	{
		//StringW Filename;
		
		//UTF82StringW(Filename, ImageName);

		//CImage	*MapImage = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		CImage	*MapImage = //SafeCreateObject3(CImage, 128, 128, 8888);
			CImageManager::GetInstance().CreateImage(128, 128, 8888);
		if(MapImage)
		{
			UIMiniMap	*MiniMap = SafeCreateObject(UIMiniMap);

			TakeBaseItemAttr(MiniMap, pElement);
	
			Int32 X, Y; 
			pElement->Attribute("X", &X);
			pElement->Attribute("Y", &Y);
			MiniMap->SetOffsetXY(X, Y);

	
			MiniMap->SetImage(MapImage);

			CImage *MaskImage = CImageManager::GetInstance().CreateImage(_T("Data\\Mask.TGA"), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
			MiniMap->SetMask(MaskImage);

			

			PCSTRINGA PlayerImageName = pElement->Attribute("PlayerIcon");
			if (PlayerImageName)
			{
				Int32 PlayerIconLeft, PlayerIconTop,PlayerIconRight, PlayerIconBottom;
				CImage	*PlayerImage = CImageManager::GetInstance().CreateImage(String(PlayerImageName).c_str(), 
					D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
				MiniMap->SetPlayerImage(PlayerImage);
				pElement->Attribute("PlayerIconLeft", &PlayerIconLeft);
				pElement->Attribute("PlayerIconTop", &PlayerIconTop);
				pElement->Attribute("PlayerIconRight", &PlayerIconRight);
				pElement->Attribute("PlayerIconBottom", &PlayerIconBottom);
				MiniMap->SetPlayerImageRect(PlayerIconLeft, PlayerIconTop,PlayerIconRight, PlayerIconBottom);
			}
			//PlayerIcon
			m_PanelStack.back()->AddUIItem(MiniMap);
		}
	}

	return 1;
}


Int32	XML_UI::CreateUIFont(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA FontName = pElement->Attribute("Name");
	Int32		FontWidth;
	Int32		FontHeight;
	Int32		FontCache;
	
	if(FontName)
	{
		m_FontName = FontName;
		pElement->Attribute("Width", &FontWidth);
		pElement->Attribute("Height", &FontHeight);
		pElement->Attribute("Cache", &FontCache);
		m_FontWidth = (FontWidth ? FontWidth : 12);
		m_FontHeight = (FontHeight ? FontHeight : 12);
		
	}

	return 1;
}


Int32	XML_UI::SetButtonRect(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32		ButtonState;
	Int32		Left, Top, Right, Bottom,X, Y;
	
	pElement->Attribute("State", &ButtonState);
	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	PCSTRINGA ImageName = pElement->Attribute("AniIcon");
	if(ImageName)
	{
		UIAniIcon	*pAniIcon = SafeCreateObject(UIAniIcon);

		Int32	Rotation, AnimationIndex, Loop, Align, Detect, Lock, EquipType;

		pElement->Attribute("Rotation", &Rotation);
		pElement->Attribute("AnimationIndex", &AnimationIndex);
		pElement->Attribute("Loop", &Loop);
		pElement->Attribute("Align", &Align);
		pElement->Attribute("Detect", &Detect);
		pElement->Attribute("LockResources", &Lock);
		pElement->Attribute("EquipType", &EquipType);
		
		pAniIcon->SetAlign(Align);
		pAniIcon->SetAnimationIndex(AnimationIndex);
		pAniIcon->SetRotation(Rotation);
		pAniIcon->SetDetect(Detect);
		pAniIcon->SetAnimationData(String(ImageName).c_str(), EquipType);

		if (Lock)
			pAniIcon->GetAniEnt()->LockResources();
		pAniIcon->Update(::timeGetTime());
		pAniIcon->SetOffsetXY(X + m_TempButton->GetOffsetX(), Y  + m_TempButton->GetOffsetY());
		m_TempButton->SetAniIcon(ButtonState, pAniIcon);
		m_TempButton->SetRect(ButtonState, 0, 0, pAniIcon->GetWidth(), pAniIcon->GetHeight());
		m_TempUIManager->AddUpdateUIItem(m_TempButton);
	}
	else
		m_TempButton->SetRect(ButtonState, Left, Top, Right, Bottom);

	m_TempButton->SetSubOffSet(ButtonState, X + m_TempButton->GetOffsetX(), Y  + m_TempButton->GetOffsetY());

	return 1;
}


Int32	XML_UI::SetCheckBoxRect(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32		Index;
	Int32		Left, Top, Right, Bottom;
	
	pElement->Attribute("Index", &Index);
	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);

	m_TempCheckBox->SetRect(Index, Left, Top, Right, Bottom);

	return 1;
}


Int32	XML_UI::ListBoxAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Text);
		m_TempListBox->AddString(TempStr.c_str());
		
	}

	return 1;
}


Int32 XML_UI::PopUpAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	Int32	Code;
	
	pElement->Attribute("Code", &Code);

	if(Text)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Text);


		PCSTRINGA	NextMenu = pElement->Attribute("NextMenu");

		m_TempPopUpMenu->AddString(TempStr.c_str(), Code, (NextMenu ? NextMenu :  ""));
		
	}

	return 1;
}



Int32 XML_UI::STAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Text);
		
		m_TempStaticText->AddText(TempStr);
	}

	return 1;
}

Int32 XML_UI::DTAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		StringW TempStr;

		UTF82StringW(TempStr, Text);


		
		m_TempDynamicText->AddText(TempStr);
	}

	return 1;
}



//////////////////////////////////////////////////////////////////


	
void UINameList::Clear()
{
	for(std::map<StringA, CUIFilename>::iterator i = m_UIFilename.begin(); i != m_UIFilename.end(); ++i)
	{
		i->second.XMLName.clear();
		i->second.LuaName.clear();
	}
	m_UIFilename.clear();
}


PCSTRINGA UINameList::UIName(PCSTRINGA Index)
{
	switch(m_Type)
	{
	case 0:
		return m_UIFilename[Index].XMLName.c_str();
	case 1:
		return m_UIFilename[Index].LuaName.c_str();
	}

	return NULL;
}


void UINameList::LoadUIFile(UIManager *pUIManager, PCSTRINGA Filename)
{
	switch(m_Type)
	{
	case 0:
		XML_UI::GetInstance().Init();
		XML_UI::GetInstance().ReadFormFile(m_UIFilename[Filename].XMLName.c_str(), pUIManager);
		break;
	case 1:
		pUIManager->LoadLuaUI(m_UIFilename[Filename].LuaName.c_str());
		break;
	}
}

bool UINameList::XMLReadFormFile(PCSTRINGA XMLFile)
{
	TiXmlDocument doc( XMLFile );

	doc.LoadFile();
	
	TiXmlHandle	docH( &doc );
	TiXmlElement* root = doc.FirstChildElement( );

	if ( root )
	{
		TiXmlElement	*Element = root->FirstChildElement();
		TiXmlNode		*Node = Element;

		while (Element)
		{
			const char *NodeValue = Node->Value();

			const char *UIName = Element->Attribute("UIName");
			const char *XMLFile = Element->Attribute("XMLFile");
			const char *LuaFile = Element->Attribute("LuaFile");

			m_UIFilename[UIName].XMLName = XMLFile;
			m_UIFilename[UIName].LuaName = LuaFile;

			Element = (TiXmlElement *)Element->NextSibling();
			Node = Element;
			
		}
	}

	return true;

}

