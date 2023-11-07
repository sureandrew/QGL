//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "XMLPanel.h"

void MakeLuaFIleStrA(StringA &DesStr, PCSTRINGA SrcStr)
{
	StringA	TempStr = SrcStr;

	DesStr.clear();
	for(UInt i = 0; i < TempStr.size(); ++i)
	{
		if(TempStr[i] == '\\')
		{
			DesStr += "\\\\"; 
		}
		else
		{
			DesStr += TempStr[i];
		}
	}
}


XMLBIN_UI::XMLBIN_UI()
{
	m_BaseFuncMap["ID"]				= &XMLBIN_UI::SetName;
	m_BaseFuncMap["Image"]			= &XMLBIN_UI::RegisterImage;
	m_BaseFuncMap["Background"]		= &XMLBIN_UI::CreateBackground;
	m_BaseFuncMap["Panel"]			= &XMLBIN_UI::CreatePanel;
	m_BaseFuncMap["Button"]			= &XMLBIN_UI::CreateButton;
	m_BaseFuncMap["Font"]			= &XMLBIN_UI::CreateUIFont;
	m_BaseFuncMap["Lua"]			= &XMLBIN_UI::SetupLuaState;
	m_BaseFuncMap["KeyIn"]			= &XMLBIN_UI::SetupKeyInItem;
	m_BaseFuncMap["Default"]		= &XMLBIN_UI::SetDefault;
	m_BaseFuncMap["ReflashPanelEx"]	= &XMLBIN_UI::ReflashPanelEx;
	
	

	//m_PanelFuncMap["Panel"]		= CreatePanel;
	m_PanelFuncMap["UIImage"]		= &XMLBIN_UI::CreateUIImage;
	m_PanelFuncMap["AniIcon"]		= &XMLBIN_UI::CreateUIAniIcon;
	m_PanelFuncMap["LinerImage"]	= &XMLBIN_UI::CreateLinerImage;
	m_PanelFuncMap["LinerImageV"]	= &XMLBIN_UI::CreateLinerImageV;
	m_PanelFuncMap["StaticText"]	= &XMLBIN_UI::CreateStatic;
	m_PanelFuncMap["Button"]		= &XMLBIN_UI::CreateButton;
	m_PanelFuncMap["CheckBox"]		= &XMLBIN_UI::CreateCheckBox;
	m_PanelFuncMap["ListBox"]		= &XMLBIN_UI::CreateListBox;
	m_PanelFuncMap["PopUpMenu"]		= &XMLBIN_UI::CreatePopUpMenu;
	m_PanelFuncMap["ScrollBarV"]	= &XMLBIN_UI::CreateScrollBarV;
	m_PanelFuncMap["ScrollBarH"]	= &XMLBIN_UI::CreateScrollBarH;
	m_PanelFuncMap["ScrollListBox"]	= &XMLBIN_UI::CreateScrollListBox;
	m_PanelFuncMap["EditBox"]		= &XMLBIN_UI::CreateEditBox;
	m_PanelFuncMap["ChatPanel"]		= &XMLBIN_UI::CreateChatPanel;
	m_PanelFuncMap["MiniMap"]		= &XMLBIN_UI::CreateUIMiniMap;
	m_PanelFuncMap["DynamicText"]	= &XMLBIN_UI::CreateUIDynamicText;

	
	


	m_ButtonFuncMap["ButtonRect"]	= &XMLBIN_UI::SetButtonRect;

	m_CheckBoxFuncMap["CheckBoxRect"]	= &XMLBIN_UI::SetCheckBoxRect;

	m_ListBoxFuncMap["ListBoxString"]	= &XMLBIN_UI::ListBoxAddString;

	m_PopUpFuncMap["PopUpMenuString"]	= &XMLBIN_UI::PopUpAddString;

	m_StatictextFuncMap["STSubString"] = &XMLBIN_UI::STAddString;
	m_DynamictextFuncMap["DTSubString"] = &XMLBIN_UI::DTAddString;

	m_ScrollBarFuncMap["PrevButton"]	= &XMLBIN_UI::SetPrevScroll;
	m_ScrollBarFuncMap["NextButton"]	= &XMLBIN_UI::SetNextScroll;
	m_ScrollBarFuncMap["MoveButton"]	= &XMLBIN_UI::SetMoveScroll;

	m_ScrollListBoxFuncMap["String"]		= &XMLBIN_UI::ListBoxAddString;
	m_ScrollListBoxFuncMap["ScrollBarV"]	= &XMLBIN_UI::ListBoxScrollBarV;
	m_ScrollListBoxFuncMap["ScrollBarH"]	= &XMLBIN_UI::ListBoxScrollBarH;

	m_ChatPanelFuncMap["PrevButton"]	= &XMLBIN_UI::SetPrevChat;
	m_ChatPanelFuncMap["NextButton"]	= &XMLBIN_UI::SetNextChat;
	m_ChatPanelFuncMap["HomeButton"]	= &XMLBIN_UI::SetHomeChat;
	m_ChatPanelFuncMap["EndButton"]		= &XMLBIN_UI::SetEndChat;
	m_ChatPanelFuncMap["NewLine"]		= &XMLBIN_UI::ChatAddNewLine;
	m_ChatPanelFuncMap["String"]		= &XMLBIN_UI::ChatAddString;
	m_ChatPanelFuncMap["Link"]			= &XMLBIN_UI::ChatAddLink;
	m_ChatPanelFuncMap["Icon"]			= &XMLBIN_UI::ChatAddIcon;
	m_ChatPanelFuncMap["ChatString"]	= &XMLBIN_UI::ChatAddStringC;


	m_EnumMap["FONT1"] = EDGE_FONT1;
	m_EnumMap["FONT2"] = EDGE_FONT2;
	m_EnumMap["SHADOW"] = EDGE_SHADOW;
	m_EnumMap["LIGHT"] = EDGE_LIGHT;

	
	Init();
}


XMLBIN_UI::~XMLBIN_UI()
{
	//m_UIPanelList.clear();
	//m_PanelStack.clear();
	m_EnumMap.clear();
}


void XMLBIN_UI::Init()
{
	//m_UIPanelList.clear();
	//m_UIItemPtr = NULL;

	/*
	m_TempButton = NULL;
	m_TempCheckBox = NULL;
	m_TempListBox = NULL;
	m_TempPopUpMenu = NULL;
	m_TempStaticText = NULL;
	m_TempScrollBar = NULL;
	m_TempScrollListBox = NULL;
	m_TempChatPanel = NULL;
	*/

	//m_LuaState = NULL;

	//m_Render = NULL;
	
	//m_TempUIManager = NULL;
	

	//m_PanelStack.clear();
	
	m_FontName.clear();
	m_FontWidth = m_FontHeight = 0;

	m_KeyInItemName.clear();
	m_LuaFilename.clear();

	m_fFile = stdout;
}


bool XMLBIN_UI::ReadFormFile(PCSTRINGA XMLFile, PCSTRINGA LuaFile)
{
	AddFormFile(XMLFile, LuaFile);


	return true;
}


bool XMLBIN_UI::AddFormFile(PCSTRINGA XMLFile, PCSTRINGA LuaFile)
{
	Init();
	TiXmlDocument doc( XMLFile );

	/*
	if(pUIManager->GetLuaShareFlag())
		m_LuaState = pUIManager->GetLuaState();
		*/

	sprintf(m_TempStr, "if UIManager:GetLuaShareFlag() then\n");
	OutputFile();
	sprintf(m_TempStr, "	LuaState = UIManager:GetLuaState();\n");
	OutputFile();
	sprintf(m_TempStr, "end;\n");
	OutputFile();

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

	StringW	LuaFileW = LuaFile;
	m_fFile = _tfopen(LuaFileW.c_str(), _T("w"));


	sprintf(m_TempStr, "UIManager = LuaUI.GetUIManager();\n");
	OutputFile();
	sprintf(m_TempStr, "LuaState = UIManager:GetLuaState();\n");
	OutputFile();

	//m_Render = pUIManager->GetRender();
	//m_LuaState = pUIManager->GetLuaState();
	
	TiXmlHandle	docH( &doc );
	TiXmlElement* root = doc.FirstChildElement( );

	//m_TempUIManager = pUIManager;

	//m_UIPanelList.clear();
	
	if ( root )
	{
		ReadElement(root->Value(), root, m_BaseFuncMap);
	}

	
	/*
	for(UInt32 i = 0; i < m_UIPanelList.size(); ++i)
	{
		pUIManager->AddUIPanel(m_UIPanelList[i]);
	}
	*/

	if(!m_KeyInItemName.empty())
	{
		//pUIManager->SetKeyInItem(m_KeyInItemName.c_str());
		sprintf(m_TempStr, "UIManager:SetKeyInItem(\"%s\")", m_KeyInItemName.c_str());
		OutputFile();
	}


	//m_UIPanelList.clear();

	//m_TempUIManager = NULL;

	if(!m_LuaFilename.empty())
	{
		sprintf(m_TempStr, "\n\nDoUILuaFunc();\n");
		OutputFile();
		sprintf(m_TempStr, "\n\nDoUILuaFunc = nil;\n");
		OutputFile();
	}

	fclose(m_fFile);

	return true;
}



Int32 XMLBIN_UI::ReadElement(PCSTRINGA Parent, TiXmlElement *root, FuncMap &FuncTable)
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

extern "C"
{
int Luaui_Init(lua_State* L); // declare the wrapped module
}


Int32 XMLBIN_UI::SetupLuaState(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA LuaFilename = pElement->Attribute("Filename");
	/*
	lua_State			*LuaState;

	
	LuaState = lua_open();

	
	luaopen_base(LuaState);
	Luaui_Init(LuaState);
	//luaopen_io(LuaState);
	luaopen_string(LuaState);
	luaopen_math(LuaState);

	if( g_GdfResource )
	{
		String tempfilename = const_cast<char*>(LuaFilename);
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = g_GdfResource->GetMapViewOfFile(tempfilename, beginOffset, fileSize);
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
	*/

	StringA FileStr;

	m_LuaFilename = LuaFilename;
	MakeLuaFIleStrA(FileStr, LuaFilename);
	sprintf(m_TempStr, "DoUILuaFunc = loadfile(\"%s\");\n", FileStr.c_str());
	OutputFile();

	return 1;
}



Int32 XMLBIN_UI::SetupKeyInItem(TiXmlNode *pNode, TiXmlElement *pElement)
{
	m_KeyInItemName = pElement->Attribute("Item");
	
	return 1;
}


Int32 XMLBIN_UI::CreateBackground(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		Int32	Left, Top, Right, Bottom;

		pElement->Attribute("Left", &Left);
		pElement->Attribute("Top", &Top);
		pElement->Attribute("Right", &Right);
		pElement->Attribute("Bottom", &Bottom);

		/*	--Real XML
		StringW Filename;
		UTF82StringW(Filename, ImageName);
		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		m_TempUIManager->SetBackImage(Image, Left, Top, Right, Bottom);
		*/

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);
		
		sprintf(m_TempStr, "UIManager:SetBackImageA(\"%s\", %d, %d, %d, %d);\n", FileStr.c_str(), Left, Top, Right, Bottom);
		OutputFile();

	}

	return 1;
}


Int32 XMLBIN_UI::RegisterImage(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA ImageName = pElement->Attribute("Name");
	if(ImageName)
	{
		/*	--Real XML
		StringW Filename;
		UTF82StringW(Filename, ImageName);
		m_TempUIManager->RegisterImage(Filename.c_str());
		*/

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		
		sprintf(m_TempStr, "UIManager:RegisterImageA(\"%s\");\n", FileStr.c_str());
		OutputFile();
	}

	
	return 1;
}


Int32 XMLBIN_UI::SetDefault(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Name = pElement->Attribute("Item");
	if(Name)
	{
		//m_TempUIManager->ChangeFocus(Name);

		sprintf(m_TempStr, "UIManager:ChangeFocus(\"%s\");\n", Name);
		OutputFile();
	}

	Name = pElement->Attribute("Enter");
	if(Name)
	{
		//m_TempUIManager->SetDefaultEnterFunc(Name);

		sprintf(m_TempStr, "UIManager:SetDefaultEnterFunc(\"%s\");\n", Name);
		OutputFile();
	}

	return 1;
}


Int32 XMLBIN_UI::ReflashPanelEx(TiXmlNode *pNode, TiXmlElement *pElement)
{

	return 1;
}


Int32 XMLBIN_UI::SetName(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Name = pElement->Attribute("Name");
	if(Name)
	{
		//m_TempUIManager->SetIDName(Name);

		sprintf(m_TempStr, "UIManager:SetIDName(\"%s\");\n", Name);
		OutputFile();
	}

	return 1;
}


void XMLBIN_UI::SetLuaFuncAttr(TiXmlElement *pElement, PCSTRINGA LuaFuncMsg, UI_LUA_FUNC_CODE Code)
{
	PCSTRINGA FuncName = pElement->Attribute(LuaFuncMsg);
	if(FuncName)
	{
		//pItem->SetLuaFunc(Code, FuncName);

		
		sprintf(m_TempStr, "MyItem:SetLuaFunc(%d, \"%s\");\n", Code, FuncName);
		OutputFile();
	}
}




void XMLBIN_UI::TakeBaseItemAttr(TiXmlElement *pElement)
{
	UInt32 IColor = HexToDWORD(pElement->Attribute("IColor"));
	
		
	if(IColor)
	{
		/*
		pItem->SetColor(IColor);
		*/
	
		sprintf(m_TempStr, "MyItem:SetColor(%d);\n", IColor);
		OutputFile();	
	}

	PCSTRINGA IDName = pElement->Attribute("Name");
	if(IDName)
	{
		/*
		m_TempUIManager->AddUIID(IDName, pItem);
		pItem->SetName(IDName);
		*/

		sprintf(m_TempStr, "UIManager:AddUIID(\"%s\", MyItem);\n"
				"MyItem:SetName(\"%s\");\n", 
				IDName, IDName);
		OutputFile();	
	}

	PCSTRINGA NextName = pElement->Attribute("Next");
	if(NextName)
	{
		//pItem->SetNext(NextName);

		sprintf(m_TempStr, "MyItem:SetNext(\"%s\");\n", NextName);
		OutputFile();	
	}

	PCSTRINGA PrevName = pElement->Attribute("Prev");
	if(PrevName)
	{
		//pItem->SetPrev(PrevName);

		sprintf(m_TempStr, "MyItem:SetPrev(\"%s\");\n", PrevName);
		OutputFile();	
	}

	Int32 IsFlag;
	pElement->Attribute("Hide", &IsFlag);
	//pItem->SetEnable(IsFlag ? false : true);

	sprintf(m_TempStr, "MyItem:SetEnable(%s);\n", (IsFlag ? "false" : "true"));
	OutputFile();	



	{
		Int32	Drag;
		pElement->Attribute("Drag", &Drag);

		if(Drag)
		{
			//pItem->SetCanDrag(Drag);

			Int32	DLeft, DTop, DRight, DBottom;

			pElement->Attribute("DragLeft", &DLeft);
			pElement->Attribute("DragTop", &DTop);
			pElement->Attribute("DragRight", &DRight);
			pElement->Attribute("DragBottom", &DBottom);

			//pItem->SetDragRect(DLeft, DTop, DRight, DBottom); 

			sprintf(m_TempStr, "MyItem:SetCanDrag(%d);\n"
				"MyItem:SetDragRect(%d, %d, %d, %d);\n", Drag, DLeft, DTop, DRight, DBottom);
			OutputFile();	
		}
	}	

	Int32 NotDetected;
	pElement->Attribute("NotDetected", &NotDetected);
	//pItem->SetNotDetected(NotDetected > 0);
	sprintf(m_TempStr, "MyItem:SetNotDetected(%s);\n", (NotDetected > 0 ? "true" : "false"));
	OutputFile();	

	Int32	DetectImage= 0;
	pElement->Attribute("DetectImage", &DetectImage);
	//pItem->SetDetectImage(DetectImage > 0);
	sprintf(m_TempStr, "MyItem:SetDetectImage(%s);\n", (DetectImage > 0 ? "true" : "false"));
	OutputFile();	

	Int32 TimeOutCount = 0;
	pElement->Attribute("TimeOutCount", &TimeOutCount);
	if (TimeOutCount > 0)
	{
		//pItem->SetTimeOutCount(TimeOutCount);
		//m_TempUIManager->AddUpdateUIItem(pItem);

		sprintf(m_TempStr, 
			"MyItem:SetTimeOutCount(%d);\n"
			"UIManager:AddUpdateUIItem(MyItem);\n", TimeOutCount);
	}
	else
	{
		//pItem->SetTimeOutCount(0);
		sprintf(m_TempStr, "MyItem:SetTimeOutCount(0);\n");
	}
	OutputFile();	

	Int32 DisableTimeOutEvent = 0;
	pElement->Attribute("DisableTimeOutEvent", &DisableTimeOutEvent);
	//pItem->SetEnableTimeOutEvent(DisableTimeOutEvent == 0);
	sprintf(m_TempStr, "MyItem:SetEnableTimeOutEvent(%s);\n", (DisableTimeOutEvent == 0 ? "true" : "false"));
	OutputFile();	


	Int32 InsertToEventList = 0;
	pElement->Attribute("InsertToEventList", &InsertToEventList);
	if (InsertToEventList > 0)
	{
		//m_TempUIManager->InsertToInOutEvent(pItem);
		sprintf(m_TempStr, "UIManager:InsertToInOutEvent(MyItem);\n");
		OutputFile();	
	}
	
	SetLuaFuncAttr(pElement, "LuaLButtonUp", UI_FUNC_LBUTTON_UP);
	SetLuaFuncAttr(pElement, "LuaLButtonDown", UI_FUNC_LBUTTON_DOWN);
	SetLuaFuncAttr(pElement, "LuaRButtonUp", UI_FUNC_RBUTTON_UP);
	SetLuaFuncAttr(pElement, "LuaRButtonDown", UI_FUNC_RBUTTON_DOWN);
	SetLuaFuncAttr(pElement, "LuaMouseMove", UI_FUNC_MOUSE_MOVE);

	SetLuaFuncAttr(pElement, "LuaMouseWheel", UI_FUNC_MOUSE_WHEEL);
	SetLuaFuncAttr(pElement, "LuaKeyChar", UI_FUNC_CHAR);
	SetLuaFuncAttr(pElement, "LuaKeyUp", UI_FUNC_KEYUP);
	SetLuaFuncAttr(pElement, "LuaKeyDown", UI_FUNC_KEYDOWN);
	SetLuaFuncAttr(pElement, "LuaTimeOut", UI_FUNC_TIMEOUT);
	SetLuaFuncAttr(pElement, "LuaUnPickUp", UI_FUNC_UNPICKUP);

	SetLuaFuncAttr(pElement, "LuaStartDrag", UI_FUNC_STARTDRAG);
	SetLuaFuncAttr(pElement, "LuaEndDrag", UI_FUNG_ENDDRAG);

	SetLuaFuncAttr(pElement, "LuaMouseOut", UI_FUNC_MOUSE_OUT);
	SetLuaFuncAttr(pElement, "LuaMouseIn", UI_FUNC_MOUSE_IN);

	SetLuaFuncAttr(pElement, "LuaLButtonDBLCLK", UI_FUNC_LBUTTONDBLCLK);
	SetLuaFuncAttr(pElement, "LuaRButtonDBLCLK", UI_FUNC_RBUTTONDBLCLK);

	PCSTRINGA UILuaFunc = pElement->Attribute("UILuaFunc");
	if(UILuaFunc)
	{
		//m_TempUIManager->SetLuaFuncName(UILuaFunc);

		sprintf(m_TempStr, "UIManager:SetLuaFuncName(\"%s\");\n", UILuaFunc);
		OutputFile();	
	}
}

Int32 XMLBIN_UI::CreatePanel(TiXmlNode *pNode, TiXmlElement *pElement)
{
	
	//UIPanel			*Panel = NULL;
	//UIPanelEx		*PanelEx = NULL;
	Int32 X, Y; 
	Int32	IsExpand;
	bool	ExFlag;
		
	pElement->Attribute("Expand", &IsExpand);

	if(!IsExpand)
	{
		//Panel = SafeCreateObject(UIPanel);
		ExFlag = false;

		sprintf(m_TempStr, "-------------------------->>>> Create Panel\n");
		OutputFile();
		sprintf(m_TempStr, "MyPanel = LuaUI.LuaCreateUIPanel();\n"
				"MyItem = MyPanel:GetUIItemPtr();\n");
		OutputFile();	
	}
	else
	{
		//Panel = PanelEx = SafeCreateObject(UIPanelEx);
		ExFlag = true;

		sprintf(m_TempStr, "-------------------------->>>> Create PanelEx\n");
		OutputFile();
		sprintf(m_TempStr, "MyPanel = LuaUI.LuaCreateUIPanelEx();\n"
				"MyItem = MyPanel:GetUIItemPtr();\n");
		OutputFile();	
	}

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	

	TakeBaseItemAttr(pElement);

	Int32	Left, Top, Right, Bottom;

	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);

	
	Int32 TWidth, THeight; 
	pElement->Attribute("TWidth", &TWidth);
	pElement->Attribute("THeight", &THeight);


	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		/*
		StringW Filename;
		UTF82StringW(Filename, ImageName);
		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		Panel->SetImage(Image, Left, Top, Right, Bottom);
		*/

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		sprintf(m_TempStr, "MyPanel:SetImageFileA(\"%s\", %d, %d, %d, %d);\n", FileStr.c_str(), Left, Top, Right, Bottom);
		OutputFile();	

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

			//PanelEx->SetBottomRect(Left, Top, Right, Bottom);
			sprintf(m_TempStr, "MyPanel:SetBottomRect(%d, %d, %d, %d);\n", Left, Top, Right, Bottom);
			OutputFile();	

			pElement->Attribute("LineRectLeft", &Left);
			pElement->Attribute("LineRectTop", &Top);
			pElement->Attribute("LineRectRight", &Right);
			pElement->Attribute("LineRectBottom", &Bottom);

			CheckWidth = max(CheckWidth, Right - Left);

			Int32	Count; 
			
			pElement->Attribute("LineCount", &Count);

			CheckHeight += (Bottom - Top) * Count;

			//PanelEx->SetLineRect(Left, Top, Right, Bottom, Count);
			sprintf(m_TempStr, "MyPanel:SetLineRect(%d, %d, %d, %d, %d);\n", Left, Top, Right, Bottom, Count);
			OutputFile();	


			TWidth = max(TWidth, CheckWidth);
			THeight = max(THeight, CheckHeight);

			//PanelEx->SetTextureWH(TWidth, THeight);
			sprintf(m_TempStr, "MyPanel:SetTextureWH(%d, %d);\n", TWidth, THeight);
			OutputFile();	
		}
		else
		{
			//TWidth = max(TWidth, (Int32)Panel->GetWidth());
			//THeight = max(THeight, (Int32)Panel->GetHeight());

			sprintf(m_TempStr, "if %d > MyPanel:GetWidth() then\n", TWidth);
			OutputFile();	
			sprintf(m_TempStr, "	TWidth = %d\n", TWidth);
			OutputFile();	
			sprintf(m_TempStr, "else\n");
			OutputFile();	
			sprintf(m_TempStr, "	TWidth = MyPanel:GetWidth()\n");
			OutputFile();	
			sprintf(m_TempStr, "end\n");
			OutputFile();	

			sprintf(m_TempStr, "if %d > MyPanel:GetHeight() then\n", THeight);
			OutputFile();	
			sprintf(m_TempStr, "	THeight = %d\n", THeight);
			OutputFile();	
			sprintf(m_TempStr, "else\n");
			OutputFile();	
			sprintf(m_TempStr, "	THeight = MyPanel:GetHeight()\n");
			OutputFile();	
			sprintf(m_TempStr, "end\n");
			OutputFile();	

			//Panel->SetTextureWH(TWidth, THeight);
			sprintf(m_TempStr, "MyPanel:SetTextureWH(TWidth, THeight);\n");
			OutputFile();	
		}
	}
	else
	{
		//Panel->SetImage(NULL, Left, Top, Right, Bottom);
		sprintf(m_TempStr, "MyPanel:SetImageNil(%d, %d, %d, %d);\n", Left, Top, Right, Bottom);
		OutputFile();	
		
		TWidth = max(TWidth, Right - Left);//Bottom - Panel->GetWidth());
		THeight = max(THeight, Bottom - Top);//Panel->GetHeight());

		//Panel->SetTextureWH(TWidth, THeight);
		sprintf(m_TempStr, "MyPanel:SetTextureWH(%d, %d);\n", TWidth, THeight);
		OutputFile();	
	}

	
	Int		AutoDelete;

	pElement->Attribute("AutoDelete", &AutoDelete);
	//Panel->SetAutoDelete(AutoDelete ? true : false);
	sprintf(m_TempStr, "MyPanel:SetAutoDelete(%s);\n", (AutoDelete ? "true" : "false"));
	OutputFile();	

	PCSTRINGA LuaPerLButtonUp = pElement->Attribute("LuaPerLButtonUp");
	if(LuaPerLButtonUp)
	{
		//Panel->SetLuaPerLButtonUp(LuaPerLButtonUp);
		sprintf(m_TempStr, "MyPanel:SetLuaPerLButtonUp(\"%s\");\n", LuaPerLButtonUp);
		OutputFile();	
	}

	PCSTRINGA LuaPerRButtonUp = pElement->Attribute("LuaPerRButtonUp");
	if(LuaPerRButtonUp)
	{
		//Panel->SetLuaPerRButtonUp(LuaPerRButtonUp);
		sprintf(m_TempStr, "MyPanel:SetLuaPerRButtonUp(\"%s\");\n", LuaPerRButtonUp);
		OutputFile();	
	}

	PCSTRINGA ToPanelName = pElement->Attribute("ToPanel");
	//UIPanel *ToPanelPtr = m_TempUIManager->FindUIPanel(ToPanelName);

	if(ToPanelName)
	{
		sprintf(m_TempStr, "ToPanelPtr = UIManager:FindUIPanel(\"%s\");\n", ToPanelName);
		OutputFile();	


		sprintf(m_TempStr, "if ToPanelPtr ~= nil then\n", ToPanelName);
		OutputFile();	
		sprintf(m_TempStr, "    ToPanelPtr:AddUIItem(MyPanel);\n");
		OutputFile();	
		sprintf(m_TempStr, "end\n", ToPanelName);
		OutputFile();	
	}
	sprintf(m_TempStr, "UIManager:AddUIPanel(MyPanel);\n");
	OutputFile();	
	/*
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
	*/

	ReadElement(pNode->Value(), pElement, m_PanelFuncMap);
	
	//m_PanelStack.pop_back();

	/*
	Panel->SetPos(X, Y);
	Panel->SetTexture(m_TempUIManager->GetRender()->GetDevice());
	Panel->SetLuaState(m_LuaState);
	*/

	sprintf(m_TempStr, "MyPanel:SetPos(%d, %d);\n", X, Y);
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:SetTextureUI(UIManager);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:SetLuaState(LuaState);\n");
	OutputFile();	
		
	/*if(ToPanelPtr)
	{
		Panel->SetOffsetXY(X - ToPanelPtr->GetX(), Y - ToPanelPtr->GetY());
	}
	*/
	if(ToPanelName)
	{
		sprintf(m_TempStr, "    MyPanel:SetOffsetXY(%d - ToPanelPtr:GetX(), %d - ToPanelPtr:GetY());\n", X, Y);
		OutputFile();	
	}

	return 1;
}

void XMLBIN_UI::TakeEntAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	sprintf(m_TempStr, "MyItem = MyAniIcon:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	PCSTRINGA ImageName = pElement->Attribute("Image");

	Int32	Rotation, AnimationIndex, Loop, Align, Shadow, Detect, Lock;

	pElement->Attribute("Rotation", &Rotation);
	pElement->Attribute("AnimationIndex", &AnimationIndex);
	pElement->Attribute("Loop", &Loop);
	pElement->Attribute("Align", &Align);
	pElement->Attribute("Shadow", &Shadow);
	pElement->Attribute("Detect", &Detect);
	pElement->Attribute("LockResources", &Lock);

	/*
	pAniIcon->SetAlign(Align);
	pAniIcon->SetAnimationIndex(AnimationIndex);
	pAniIcon->SetRotation(Rotation);
	pAniIcon->SetDetect(Detect);
	*/


	sprintf(m_TempStr, "MyAniIcon:SetAlign(%d);\n", Align);
	OutputFile();	
	sprintf(m_TempStr, "MyAniIcon:SetAnimationIndex(%d);\n", AnimationIndex);
	OutputFile();	
	sprintf(m_TempStr, "MyAniIcon:SetRotation(%d);\n", Rotation);
	OutputFile();	
	sprintf(m_TempStr, "MyAniIcon:SetDetect(%d);\n", Detect);
	OutputFile();	
	

	if(ImageName)
	{
		//pAniIcon->SetAnimationData(String(ImageName).c_str());	
		sprintf(m_TempStr, "MyAniIcon:SetAnimationDataA(\"%s\");\n", ImageName);
		OutputFile();	
		if (Lock)
		{
			//pAniIcon->GetAniEnt()->LockResources();
			sprintf(m_TempStr, "MyAniIcon:LockResources();\n", ImageName);
			OutputFile();	
		}
	}
}

void  XMLBIN_UI::TakeImageAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	sprintf(m_TempStr, "MyItem = MyImage:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);

	Int32	Left, Top, Right, Bottom;

	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);

	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(strlen(ImageName))
	{
		/*
		StringW Filename;
		UTF82StringW(Filename, ImageName);
		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		pImage->SetImage(Image, Left, Top, Right, Bottom);
		*/

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		sprintf(m_TempStr, "MyImage:SetImageFileA(\"%s\", %d, %d, %d, %d);\n", FileStr.c_str(), Left, Top, Right, Bottom);
		OutputFile();	
	}
	else
	{
		//pImage->SetImage(NULL, Left, Top, Right, Bottom);

		sprintf(m_TempStr, "MyImage:SetImage(nil, %d, %d, %d, %d);\n", Left, Top, Right, Bottom);
		OutputFile();	
	}

	Int		AutoDelete;
	pElement->Attribute("AutoDelete", &AutoDelete);
	
	//pImage->SetAutoDelete(AutoDelete ? true : false);
	sprintf(m_TempStr, "MyImage:SetAutoDelete(%s);\n", (AutoDelete ? "true" : "false"));
	OutputFile();	

}



Int32 XMLBIN_UI::CreateUIImage(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//UIImage			*MyImage = NULL;//SafeCreateObject(UIImage);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyImage = LuaUI.LuaCreateUIImage();\n");
	OutputFile();	

	Int32 X, Y; 

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	
	TakeImageAttr(pNode, pElement);
	
	/*
	MyImage->SetOffsetXY(X, Y);
	MyImage->SetDetectRect();
	MyImage->SetLuaState(m_LuaState);
	*/

	sprintf(m_TempStr, "MyImage:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();	
	sprintf(m_TempStr, "MyImage:SetDetectRect();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyImage:SetLuaState(LuaState);\n");
	OutputFile();	

	//m_PanelStack.back()->AddUIItem(MyImage);

	sprintf(m_TempStr, "MyPanel:AddUIItem(MyImage);\n------------------\n");
	OutputFile();	

	return 1;
}




Int32 XMLBIN_UI::CreateUIAniIcon(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//UIAniIcon	*AniIcon = NULL;//SafeCreateObject(UIAniIcon);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyAniIcon = LuaUI.LuaCreateUIAniIcon();\n");
	OutputFile();	

	Int32 X, Y; 

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	TakeEntAttr(pNode, pElement);

	/*
	AniIcon->SetOffsetXY(X, Y);
	AniIcon->SetDetectRect();
	AniIcon->SetLuaState(m_LuaState);
	*/

	sprintf(m_TempStr, "MyAniIcon:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();	
	sprintf(m_TempStr, "MyAniIcon:SetDetectRect();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyAniIcon:SetLuaState(LuaState);\n");
	OutputFile();	

	//m_PanelStack.back()->AddUIItem(AniIcon);
	//m_TempUIManager->AddUpdateUIItem(AniIcon);

	sprintf(m_TempStr, "UIManager:AddUpdateUIItem(MyAniIcon);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyAniIcon);\n------------------\n");
	OutputFile();	
	



	return 1;
}


Int32 XMLBIN_UI::CreateLinerImage(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//UILinerImage	*LinerImage = NULL;//SafeCreateObject(UILinerImage);
	Int32 X, Y; 

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyLinerImage = LuaUI.LuaCreateUILinerImage();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyImage = MyLinerImage:GetUIImagePtr();\n");
	OutputFile();	

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	TakeImageAttr(pNode, pElement);

	Int32	Width;

	pElement->Attribute("LineWidth", &Width);
	//LinerImage->SetLinerWidth(Width);
	sprintf(m_TempStr, "MyLinerImage:SetLinerWidth(%d);\n", Width);
	OutputFile();	

	pElement->Attribute("BaseWidth", &Width);
	if(Width)
	{
		//LinerImage->SetBaseWidth(Width);
		sprintf(m_TempStr, "MyLinerImage:SetBaseWidth(%d);\n", Width);
		OutputFile();	
	}
	else
	{
		//LinerImage->SetBaseWidth(LinerImage->GetLinerWidth());
		sprintf(m_TempStr, "MyLinerImage:SetBaseWidth(MyLinerImage:GetLinerWidth());\n");
		OutputFile();	
	}

	pElement->Attribute("Denumerator", &Width);
	if(Width == 0)
	{
		//LinerImage->SetDenumerator(100);
		//LinerImage->SetNumerator(100);

		sprintf(m_TempStr, "MyLinerImage:SetDenumerator(100);\n");
		OutputFile();	
		sprintf(m_TempStr, "MyLinerImage:SetNumerator(100);\n");
		OutputFile();	
	}
	else
	{
		//LinerImage->SetDenumerator(Width);
		sprintf(m_TempStr, "MyLinerImage:SetDenumerator(%d);\n", Width);
		OutputFile();	

		pElement->Attribute("Numerator", &Width);
		//LinerImage->SetNumerator(Width);
		sprintf(m_TempStr, "MyLinerImage:SetNumerator(%d);\n", Width);
		OutputFile();	
	}
	

	//LinerImage->SetOffsetXY(X, Y);
	//LinerImage->SetDetectRect();
	//LinerImage->SetLuaState(m_LuaState);

	sprintf(m_TempStr, "MyLinerImage:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();	
	sprintf(m_TempStr, "MyLinerImage:SetDetectRect();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyLinerImage:SetLuaState(LuaState);\n");
	OutputFile();	
		
	//m_PanelStack.back()->AddUIItem(LinerImage);
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyLinerImage);\n------------------\n");
	OutputFile();	

	return 1;
}


Int32 XMLBIN_UI::CreateLinerImageV(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//UILinerImageV	*LinerImageV = SafeCreateObject(UILinerImageV);
	Int32 X, Y; 

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyLinerImage = LuaUI.LuaCreateUILinerImageV();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyImage = MyLinerImage:GetUIImagePtr();\n");
	OutputFile();	

	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);

	TakeImageAttr(pNode, pElement);

	Int32	Height;

	pElement->Attribute("LineHeight", &Height);
	//LinerImageV->SetLinerHeight(Height);
	sprintf(m_TempStr, "MyLinerImage:SetLinerHeight(%d);\n", Height);
	OutputFile();	

	pElement->Attribute("BaseHeight", &Height);
	if(Height)
	{
		//LinerImageV->SetBaseHeight(Height);
		sprintf(m_TempStr, "MyLinerImage:SetBaseHeight(%d);\n", Height);
		OutputFile();	
	}
	else
	{
		//LinerImageV->SetBaseHeight(LinerImageV->GetLinerHeight());
		sprintf(m_TempStr, "MyLinerImage:SetBaseHeight(MyLinerImage:GetLinerHeight());\n");
		OutputFile();	
	}

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
	
	/*
	LinerImageV->SetOffsetXY(X, Y);
	LinerImageV->SetDetectRect();
	LinerImageV->SetLuaState(m_LuaState);
	*/

	sprintf(m_TempStr, "MyLinerImage:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();	
	sprintf(m_TempStr, "MyLinerImage:SetDetectRect();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyLinerImage:SetLuaState(LuaState);\n");
	OutputFile();	
		
	//m_PanelStack.back()->AddUIItem(LinerImageV);
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyLinerImage);\n------------------\n");
	OutputFile();	
	

	return 1;
}


void XMLBIN_UI::TakeStaticTextAttr(TiXmlElement *pElement)
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
		//pStaticText->SetFont(FontName, FontWidth, FontHeight, FontCache);

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, FontName);

		sprintf(m_TempStr, "MyText:SetFont(\"%s\", %d, %d, %d);\n", FileStr.c_str(), FontWidth, FontHeight, FontCache);
		OutputFile();	
	}
	else
	{
		
		StringA FileStr;
		MakeLuaFIleStrA(FileStr, m_FontName.c_str());

		sprintf(m_TempStr, "MyText:SetFont(\"%s\", %d, %d);\n", FileStr.c_str(), m_FontWidth, m_FontHeight);
		OutputFile();	
	}
	

	if(Align)
	{
		if(!strcmpi(Align, "Right"))
		{
			//pStaticText->SetAlign(UIStaticText::ALIGN_RIGHT);

			sprintf(m_TempStr, "MyText:SetAlign(%d);\n", UIStaticText::ALIGN_RIGHT);
			OutputFile();	
		}
		else
		if(!strcmpi(Align, "Center"))
		{
			//pStaticText->SetAlign(UIStaticText::ALIGN_CENTER);

			sprintf(m_TempStr, "MyText:SetAlign(%d);\n", UIStaticText::ALIGN_CENTER);
			OutputFile();	
		}

	}


	Int32	LinePitch;

	pElement->Attribute("LinePitch", &LinePitch);
	//pStaticText->SetLinePitch(LinePitch);

	sprintf(m_TempStr, "MyText:SetLinePitch(%d);\n", LinePitch);
	OutputFile();	
	
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		/*
		StringW m_TempStr;

		UTF82StringW(m_TempStr, Text);
		pStaticText->SetText(m_TempStr.c_str());
		*/
		sprintf(m_TempStr, "MyText:LuaSetText(\"%s\");\n", Text);
		OutputFile();	
	}


	//pStaticText->SetOffsetXY(X, Y);
	sprintf(m_TempStr, "MyText:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();	
	
	if(EdgeCode)
	{
		//pStaticText->SetFontStyle(m_EnumMap[EdgeCode]);
		sprintf(m_TempStr, "MyText:SetFontStyle(%d);\n", m_EnumMap[EdgeCode]);
		OutputFile();	
	}
	
	//pStaticText->SetFontColor(FontColor);
	//pStaticText->SetShadowColor(ShadowColor);

	sprintf(m_TempStr, "MyText:SetFontColor(%d);\n", FontColor);
	OutputFile();	
	sprintf(m_TempStr, "MyText:SetShadowColor(%d);\n", ShadowColor);
	OutputFile();	

	if(Width > 0 && Height > 0)
	{
		//pStaticText->SetImageWH(Width, Height);

		sprintf(m_TempStr, "MyText:SetImageWH(%d, %d);\n", Width, Height);
		OutputFile();	
	}
	else
	if(Row > 0 && Column > 0)
	{
		//pStaticText->SetImage(Row, Column);

		sprintf(m_TempStr, "MyText:SetImage(%d, %d);\n", Row, Column);
		OutputFile();	
	}
	else
	{
		//pStaticText->SetImageWH(pStaticText->GetFont()->GetWidth() * 8 + 30, pStaticText->GetFont()->GetHeight() + 14);

		sprintf(m_TempStr, "MyText:SetImageWH(MyText:GetWidth() * 8 + 30, MyText:GetHeight() + 14);\n");
		OutputFile();	
	}


	//pStaticText->SetLuaState(m_LuaState);
	//pStaticText->SetPassword(IsPassword ? true : false);
	//pStaticText->SetNumOnly(NumOnly ? true : false);

	sprintf(m_TempStr, "MyText:SetLuaState(LuaState);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyText:SetPassword(%s);\n", (IsPassword ? "true" : "false"));
	OutputFile();
	sprintf(m_TempStr, "MyText:SetNumOnly(%s);\n", (NumOnly ? "true" : "false"));
	OutputFile();

}



void XMLBIN_UI::TakeDynamicTextAttr(TiXmlElement *pElement)
{
	Int32 Speed;
	pElement->Attribute("Speed", &Speed);
	//pDynamicText->SetSpeed(Speed);

	sprintf(m_TempStr, "MyText:SetSpeed(%d);\n", Speed);
	OutputFile();

}

Int32 XMLBIN_UI::CreateStatic(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//UIStaticText	*StaticText = NULL;//SafeCreateObject(UIStaticText);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyText = LuaUI.LuaCreateUIStaticText();\n");
	OutputFile();
	sprintf(m_TempStr, "MyItem = MyText:GetUIItemPtr();\n");
	OutputFile();

	//TakeBaseItemAttr(StaticText, pElement);
	//TakeStaticTextAttr(StaticText, pElement);
	TakeBaseItemAttr(pElement);
	TakeStaticTextAttr(pElement);

	//m_TempStaticText = StaticText;
	ReadElement(pNode->Value(), pElement, m_StatictextFuncMap);
	//m_TempStaticText = NULL;

	//StaticText->DrawFont();
	//assume all static text is not detected
	//StaticText->SetNotDetected(true);
	//m_PanelStack.back()->AddUIItem(StaticText);

	sprintf(m_TempStr, "MyText:DrawFont();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyText:SetNotDetected(true);\n");
	OutputFile();
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyText);\n------------------\n");
	OutputFile();	

	return 1;
}

Int32 XMLBIN_UI::CreateUIDynamicText(TiXmlNode *pNode, TiXmlElement *pElement)
{
	
	//UIDynamicText	*DynamicText = SafeCreateObject(UIDynamicText);
	
	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyDynamicText = LuaUI.LuaCreateUIDynamicText();\n");
	OutputFile();
	sprintf(m_TempStr, "MyText = MyDynamicText;\n");
	OutputFile();
	sprintf(m_TempStr, "MyItem = MyText:GetUIItemPtr();\n");
	OutputFile();

	/*
	TakeBaseItemAttr(DynamicText, pElement);
	TakeStaticTextAttr(DynamicText, pElement);
	TakeDynamicTextAttr(DynamicText, pElement);
	*/
	TakeBaseItemAttr(pElement);
	TakeStaticTextAttr(pElement);
	TakeDynamicTextAttr(pElement);

	//m_TempDynamicText = DynamicText;
	ReadElement(pNode->Value(), pElement, m_DynamictextFuncMap);
	//m_TempDynamicText = NULL;

	//DynamicText->DrawFont();
	//m_PanelStack.back()->AddUIItem(DynamicText);
	//m_TempUIManager->AddUpdateUIItem(DynamicText);

	sprintf(m_TempStr, "MyText:DrawFont();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyText:SetNotDetected(true);\n");
	OutputFile();
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyText);\n------------------\n");
	OutputFile();	

	return 1;
}

void XMLBIN_UI::TakeButtonAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32 X, Y;
	
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		/*
		StringW Filename;

		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		m_TempButton->SetImage(Image);
		*/

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		sprintf(m_TempStr, "MyButton:SetImageFileA(\"%s\");\n", FileStr.c_str());
		OutputFile();	
	}

	
	
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	//m_TempButton->SetOffsetXY(X, Y);
	
	sprintf(m_TempStr, "MyButton:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();	

	ReadElement(pNode->Value(), pElement, m_ButtonFuncMap);
	
	//if(m_PanelStack.size() <= 0)
	{
		//m_TempButton->SetPos(X, Y);
		sprintf(m_TempStr, "MyButton:SetPos(%d, %d);\n", X, Y);
		OutputFile();	
	}

	
	//m_TempButton->SetStateDetectRect();
	//m_TempButton->SetLuaState(m_LuaState);

	sprintf(m_TempStr, "MyButton:SetStateDetectRect(%d);\n", _ButtonNormal);
	OutputFile();	
	sprintf(m_TempStr, "MyButton:SetLuaState(LuaState);\n");
	OutputFile();	
	
}


Int32 XMLBIN_UI::CreateButton(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = NULL;//SafeCreateObject(UIButton);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyButton = LuaUI.LuaCreateUIButton();\n");
	OutputFile();
	sprintf(m_TempStr, "MyItem = MyButton:GetUIItemPtr();\n");
	OutputFile();

	
	TakeBaseItemAttr(pElement);
	TakeButtonAttr(pNode, pElement);
	
	//if(m_PanelStack.size() > 0)
	{
		//m_PanelStack.back()->AddUIItem(m_TempButton);

		sprintf(m_TempStr, "MyPanel:AddUIItem(MyButton);\n------------------\n");
		OutputFile();	
	}
	

	//m_TempButton = NULL;

	return 1;
}


void XMLBIN_UI::TakeCheckBoxAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32 X, Y, CheckX, CheckY, State;
	
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		/*
		StringW Filename;

		UTF82StringW(Filename, ImageName);

		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		m_TempCheckBox->SetImage(Image);
		*/

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		sprintf(m_TempStr, "MyCheckBox:SetImageFileA(\"%s\");\n", FileStr.c_str());
		OutputFile();	
	}

	
	
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	pElement->Attribute("CheckX", &CheckX);
	pElement->Attribute("CheckY", &CheckY);
	//m_TempCheckBox->SetOffsetXY(X, Y);
	//m_TempCheckBox->SetCheckXY(CheckX, CheckY);

	sprintf(m_TempStr, "MyCheckBox:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();	
	sprintf(m_TempStr, "MyCheckBox:SetCheckXY(%d, %d);\n", CheckX, CheckY);
	OutputFile();	

	pElement->Attribute("State", &State);
	//m_TempCheckBox->SetState(State ? true : false);
	sprintf(m_TempStr, "MyCheckBox:SetState(%s);\n", (State ? "true" : "false"));
	OutputFile();	

	ReadElement(pNode->Value(), pElement, m_CheckBoxFuncMap);
	
	//if(m_PanelStack.size() <= 0)
	{
		//m_TempCheckBox->SetPos(X, Y);
		sprintf(m_TempStr, "MyCheckBox:SetPos(%d, %d);\n", X, Y);
		OutputFile();
	}

	//m_TempCheckBox->SetDetectRect();
	//m_TempCheckBox->SetLuaState(m_LuaState);

	sprintf(m_TempStr, "MyCheckBox:SetDetectRect();\n");
	OutputFile();
	sprintf(m_TempStr, "MyCheckBox:SetLuaState(LuaState);\n");
	OutputFile();	
	
}


Int32 XMLBIN_UI::CreateCheckBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempCheckBox = SafeCreateObject(UICheckBox);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyCheckBox = LuaUI.LuaCreateUICheckBox();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyCheckBox:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	TakeCheckBoxAttr(pNode, pElement);
	
	//if(m_PanelStack.size() > 0)
	{
		//m_PanelStack.back()->AddUIItem(m_TempCheckBox);

		sprintf(m_TempStr, "MyPanel:AddUIItem(MyCheckBox);\n------------------\n");
		OutputFile();
	}
	/*
	else
	{
		m_UIPanelList.push_back(m_TempCheckBox);
	}
	*/

	//m_TempCheckBox = NULL;

	return 1;
}


Int32 XMLBIN_UI::CreateListBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempListBox = NULL;//SafeCreateObject(UIListBox);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyListBox = LuaUI.LuaCreateUIListBox();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyListBox:GetUIItemPtr();\n");
	OutputFile();
	sprintf(m_TempStr, "MyText = MyListBox:GetUIStaticText();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);

	TakeStaticTextAttr(pElement);

	Int32 BarWidth;

	pElement->Attribute("BarWidth", &BarWidth);
	//m_TempListBox->SetBarWidth(BarWidth);
	sprintf(m_TempStr, "MyListBox:SetBarWidth(%d);\n", BarWidth);
	OutputFile();	

	{
		UInt32 SelBarColor = HexToDWORD(pElement->Attribute("SelBarColor"));

		if(pElement->Attribute("SelBarColor"))
		{
			//m_TempListBox->SetSelBarColor(SelBarColor);
			sprintf(m_TempStr, "MyListBox:SetSelBarColor(%d);\n", SelBarColor);
			OutputFile();	
		}

		UInt32 SelFontColor = HexToDWORD(pElement->Attribute("SelFontColor"));

		if(pElement->Attribute("SelFontColor"))
		{
			//m_TempListBox->SetSelFontColor(SelFontColor);
			sprintf(m_TempStr, "MyListBox:SetSelFontColor(%d);\n", SelFontColor);
			OutputFile();	
		}
		else
		{
			//m_TempListBox->SetSelFontColor(0xFF00FF00);
			sprintf(m_TempStr, "MyListBox:SetSelFontColor(4278255360);\n");
			OutputFile();	
		}
	}

	ReadElement(pNode->Value(), pElement, m_ListBoxFuncMap);
	
	//m_TempListBox->DrawFont();
	//m_PanelStack.back()->AddUIItem(m_TempListBox);
	sprintf(m_TempStr, "MyListBox:DrawFont();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyListBox);\n------------------\n");
	OutputFile();	

	//m_TempListBox->SetDetectRect();
	sprintf(m_TempStr, "MyListBox:SetDetectRect();\n");
	OutputFile();	

	//m_TempListBox = NULL;

	return 1;
}


Int32 XMLBIN_UI::CreatePopUpMenu(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempPopUpMenu = SafeCreateObject(UIPopUpMenu);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyPopUpMenu = LuaUI.LuaCreateUIPopUpMenu();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyPopUpMenu:GetUIItemPtr();\n");
	OutputFile();
	sprintf(m_TempStr, "MyText = MyPopUpMenu:GetUIStaticText();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	TakeStaticTextAttr(pElement);

	Int32 BarWidth;

	pElement->Attribute("BarWidth", &BarWidth);
	//m_TempPopUpMenu->SetBarWidth(BarWidth);
	sprintf(m_TempStr, "MyPopUpMenu:SetBarWidth(%d);\n", BarWidth);
	OutputFile();

	{
		UInt32 SelBarColor = HexToDWORD(pElement->Attribute("SelBarColor"));

		if(pElement->Attribute("SelBarColor"))
		{
			//m_TempPopUpMenu->SetSelBarColor(SelBarColor);
			sprintf(m_TempStr, "MyPopUpMenu:SetSelBarColor(%d);\n", SelBarColor);
			OutputFile();	
		}

		UInt32 SelFontColor = HexToDWORD(pElement->Attribute("SelFontColor"));

		if(pElement->Attribute("SelFontColor"))
		{
			//m_TempPopUpMenu->SetSelFontColor(SelFontColor);
			sprintf(m_TempStr, "MyPopUpMenu:SetSelFontColor(%d);\n", SelFontColor);
			OutputFile();	
		}
		else
		{
			//m_TempPopUpMenu->SetSelFontColor(0xFF00FF00);
			sprintf(m_TempStr, "MyPopUpMenu:SetSelBarColor(4278255360);\n");
			OutputFile();	
		}
	}

	Int32	LinePitch;

	pElement->Attribute("LinePitch", &LinePitch);
	//m_TempPopUpMenu->SetLinePitch(LinePitch);
	sprintf(m_TempStr, "MyPopUpMenu:SetLinePitch(%d);\n", LinePitch);
	OutputFile();	


	ReadElement(pNode->Value(), pElement, m_PopUpFuncMap);
	
	//m_TempPopUpMenu->DrawFont();
	//m_PanelStack.back()->AddUIItem(m_TempPopUpMenu)
	sprintf(m_TempStr, "MyPopUpMenu:DrawFont();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyPopUpMenu);\n------------------\n");
	OutputFile();	;

	//m_TempPopUpMenu->SetDetectRect();
	sprintf(m_TempStr, "MyPopUpMenu:SetDetectRect();\n");
	OutputFile();	

	//m_TempListBox = NULL;

	return 1;
}


Int32	XMLBIN_UI::SetPrevScroll(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = m_TempScrollBar->GetPrevBtn();
	sprintf(m_TempStr, "MyButton = MyScrollBar:GetPrevBtn();\n");
	OutputFile();	

	//m_TempButton->SetSimple(true);
	sprintf(m_TempStr, "MyScrollBar:SetSimple(true);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyButton:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	TakeButtonAttr(pNode, pElement);
	//m_TempButton = NULL;

	return 1;
}


Int32	XMLBIN_UI::SetNextScroll(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = m_TempScrollBar->GetNextBtn();
	sprintf(m_TempStr, "MyButton = MyScrollBar:GetNextBtn();\n");
	OutputFile();	


	//m_TempButton->SetSimple(true);
	sprintf(m_TempStr, "MyScrollBar:SetSimple(true);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyButton:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	TakeButtonAttr(pNode, pElement);
	//m_TempButton = NULL;

	return 1;
}


Int32	XMLBIN_UI::SetMoveScroll(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = m_TempScrollBar->GetMoveBtn();
	sprintf(m_TempStr, "MyButton = MyScrollBar:GetMoveBtn();\n");
	OutputFile();	

	//m_TempButton->SetSimple(true);
	sprintf(m_TempStr, "MyScrollBar:SetSimple(true);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyButton:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	TakeButtonAttr(pNode, pElement);
	//m_TempButton = NULL;

	return 1;
}


void XMLBIN_UI::TakeScrollBarAttr(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32 X, Y;
	Int32	Width, Height;
	Int32 Limit;
	UInt32 BarColor = HexToDWORD(pElement->Attribute("BarColor"));
	
	//m_TempScrollBar->InitButton();
	sprintf(m_TempStr, "MyScrollBar:InitButton();\n");
	OutputFile();
	
	pElement->Attribute("X", &X);
	pElement->Attribute("Y", &Y);
	pElement->Attribute("Width", &Width);
	pElement->Attribute("Height", &Height);
	pElement->Attribute("Limit", &Limit);

	
	ReadElement(pNode->Value(), pElement, m_ScrollBarFuncMap);
	

	
	//m_TempScrollBar->SetBarWH(Width, Height);
	sprintf(m_TempStr, "MyScrollBar:SetBarWH(%d, %d);\n", Width, Height);
	OutputFile();
	//m_TempScrollBar->SetBarInfo();
	sprintf(m_TempStr, "MyScrollBar:SetBarInfo();\n");
	OutputFile();
	//m_TempScrollBar->SetOffsetXY(X, Y);
	sprintf(m_TempStr, "MyScrollBar:SetOffsetXY(%d, %d);\n", X, Y);
	OutputFile();

	//m_TempScrollBar->SetNumCount(Limit);
	sprintf(m_TempStr, "MyScrollBar:SetOffsetXY(%d);\n", Limit);
	OutputFile();
	
	//m_TempScrollBar->SetDetectRect();
	sprintf(m_TempStr, "MyScrollBar:SetDetectRect();\n");
	OutputFile();
	//m_TempScrollBar->SetBarColor(BarColor);
	sprintf(m_TempStr, "MyScrollBar:SetBarColor(%d);\n", BarColor);
	OutputFile();
	//m_TempScrollBar->SetBarRenderType(1);
	sprintf(m_TempStr, "MyScrollBar:SetBarRenderType(1);\n");
	OutputFile();

	//m_TempScrollBar->SetLuaState(m_LuaState);
	sprintf(m_TempStr, "MyScrollBar:SetLuaState(LuaState);\n");
	OutputFile();

	//m_PanelStack.back()->AddUIItem(m_TempScrollBar);
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyScrollBar);\n------------------\n");
	OutputFile();	
}


Int32 XMLBIN_UI::CreateScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempScrollBar = NULL;//SafeCreateObject(UIScrollBarV);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyScrollBar = LuaUI.LuaCreateUIScrollBarV();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyCheckBox:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	TakeScrollBarAttr(pNode, pElement);
	//m_TempScrollBar = NULL;
	
	return 1;
}


Int32 XMLBIN_UI::CreateScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempScrollBar = NULL;//SafeCreateObject(UIScrollBarH);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyScrollBar = LuaUI.LuaCreateUIScrollBarH();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyCheckBox:GetUIItemPtr();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);
	TakeScrollBarAttr(pNode, pElement);
	//m_TempScrollBar = NULL;

	return 1;
}


Int32 XMLBIN_UI::ListBoxScrollBarV(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempScrollListBox->SetupScrollBarV();
	sprintf(m_TempStr, "MyScrollListBox:SetupScrollBarV();\n");
	OutputFile();	
	//m_TempScrollBar = m_TempScrollListBox->GetScrollBarV();
	sprintf(m_TempStr, "MyScrollBar = MyScrollListBox:GetScrollBarV();\n");
	OutputFile();	
	TakeScrollBarAttr(pNode, pElement);
	//m_TempScrollBar = NULL;

	return 1;
}


Int32 XMLBIN_UI::ListBoxScrollBarH(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempScrollListBox->SetupScrollBarH();
	sprintf(m_TempStr, "MyScrollListBox:SetupScrollBarH();\n");
	OutputFile();	
	//m_TempScrollBar = m_TempScrollListBox->GetScrollBarH();
	sprintf(m_TempStr, "MyScrollBar = MyScrollListBox:GetScrollBarH();\n");
	OutputFile();	
	TakeScrollBarAttr(pNode, pElement);
	//m_TempScrollBar = NULL;

	return 1;
}



Int32 XMLBIN_UI::CreateScrollListBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempScrollListBox	= NULL;//SafeCreateObject(UIScrollListBox);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyScrollListBox = LuaUI.LuaCreateUIScrollListBox();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyScrollListBox:GetUIItemPtr();\n");
	OutputFile();
	sprintf(m_TempStr, "MyListBox = MyScrollListBox:GetListBox();\n");
	OutputFile();
	sprintf(m_TempStr, "MyText = MyListBox:GetUIStaticText();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);

	//m_TempListBox = m_TempScrollListBox->GetListBox();
	sprintf(m_TempStr, "MyScrollListBox:DrawFont();\n");
	TakeStaticTextAttr(pElement);

	ReadElement(pNode->Value(), pElement, m_ScrollListBoxFuncMap);
	
	//m_TempListBox->DrawFont();
	//m_PanelStack.back()->AddUIItem(m_TempListBox);
	sprintf(m_TempStr, "MyListBox:DrawFont();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyScrollListBox);\n------------------\n");
	OutputFile();	

	//m_TempScrollListBox	= NULL;
	//m_TempListBox = NULL;

	return 1;
}


Int32 XMLBIN_UI::CreateEditBox(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//UIEditBox	*EditBox = NULL;//SafeCreateObject(UIEditBox);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyEditBox = LuaUI.LuaCreateUIEditBox();\n");
	OutputFile();	
	sprintf(m_TempStr, "MyItem = MyEditBox:GetUIItemPtr();\n");
	OutputFile();
	sprintf(m_TempStr, "MyText = MyEditBox:GetUIStaticText();\n");
	OutputFile();

	TakeBaseItemAttr(pElement);

	Int32 EditLimit;
	pElement->Attribute("Limit", &EditLimit);
	//EditBox->SetEditLimit(EditLimit);
	sprintf(m_TempStr, "MyEditBox:SetEditLimit(%d);\n", EditLimit);
	OutputFile();


	if(pElement->Attribute("SelColor"))
	{
		UInt32 SelColor = HexToDWORD(pElement->Attribute("SelColor"));
		//EditBox->SetSelColor(SelColor);
		sprintf(m_TempStr, "MyEditBox:SetSelColor(%d);\n", SelColor);
		OutputFile();
	}
	
	TakeStaticTextAttr(pElement);
	
	if(pElement->Attribute("SelHeight"))
	{
		Int32 SelHeight;

		pElement->Attribute("SelHeight", &SelHeight);
		//EditBox->SetSelHeight(SelHeight);
		sprintf(m_TempStr, "MyEditBox:SetSelHeight(%d);\n", SelHeight);
		OutputFile();
	}

	//EditBox->DrawFont();
	//m_PanelStack.back()->AddUIItem(EditBox);
	//m_TempUIManager->AddUpdateUIItem(EditBox);

	sprintf(m_TempStr, "MyEditBox:DrawFont();\n");
	OutputFile();	
	sprintf(m_TempStr, "UIManager:AddUpdateUIItem(MyEditBox);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyEditBox);\n------------------\n");
	OutputFile();	


	return 1;
}


Int32	XMLBIN_UI::SetPrevChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = m_TempChatPanel->CreatePrevBtn();

	sprintf(m_TempStr, "MyButton = MyChatPanel:CreatePrevBtn();\n");
	OutputFile();	
	TakeButtonAttr(pNode, pElement);
	//m_TempButton = NULL;

	return 1;
}


Int32	XMLBIN_UI::SetNextChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = m_TempChatPanel->CreateNextBtn();
	sprintf(m_TempStr, "MyButton = MyChatPanel:CreateNextBtn();\n");
	OutputFile();	
	TakeButtonAttr(pNode, pElement);
	//m_TempButton = NULL;
	return 1;
}


Int32	XMLBIN_UI::SetHomeChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = m_TempChatPanel->CreateHomeBtn();
	sprintf(m_TempStr, "MyButton = MyChatPanel:CreateHomeBtn();\n");
	OutputFile();	
	TakeButtonAttr(pNode, pElement);
	//m_TempButton = NULL;
	return 1;
}


Int32	XMLBIN_UI::SetEndChat(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempButton = m_TempChatPanel->CreateEndBtn();
	sprintf(m_TempStr, "MyButton = MyChatPanel:CreateEndBtn();\n");
	OutputFile();	
	TakeButtonAttr(pNode, pElement);
	//m_TempButton = NULL;
	return 1;
}


Int32 XMLBIN_UI::ChatAddNewLine(TiXmlNode *pNode, TiXmlElement *pElement)
{
	//m_TempChatPanel->CreateRow();
	sprintf(m_TempStr, "MyChatPanel:CreateRow();\n");
	OutputFile();	

	return 1;
}


void XMLBIN_UI::TakeChatString(TiXmlElement *pElement)
{
	UInt32 FontColor = HexToDWORD(pElement->Attribute("FontColor"));
	UInt32 ShadowColor = HexToDWORD(pElement->Attribute("ShadowColor"));
	PCSTRINGA EdgeCode = pElement->Attribute("EdgeCode");
	

	PCSTRINGA Text = pElement->GetText();

	
	if(Text)
	{
		/*
		StringW TempStr;

		UTF82StringW(TempStr, Text);
		ChatString->SetString(TempStr.c_str());
		*/

		sprintf(m_TempStr, "MyChatString:SetStringA(\"%s\");\n", Text);
		OutputFile();	
	}

	//ChatString->SetFontColor(FontColor);
	//ChatString->SetShadowColor(ShadowColor);
	sprintf(m_TempStr, "MyChatString:SetFontColor(\"%s\");\n", FontColor);
	OutputFile();	
	sprintf(m_TempStr, "MyChatString:SetShadowColor(\"%s\");\n", ShadowColor);
	OutputFile();	
	if(EdgeCode)
	{
		//ChatString->SetFontStyle(m_EnumMap[EdgeCode]);

		sprintf(m_TempStr, "MyChatString:SetFontStyle(%d);\n", m_EnumMap[EdgeCode]);
		OutputFile();	
	}
}


Int32 XMLBIN_UI::ChatAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();

	//if(!m_TempChatPanel->GetLastRow())
	//	m_TempChatPanel->CreateRow();
	sprintf(m_TempStr, 
			"if MyChatPanel:GetLastRow() == nil then\n"
			"    MyChatPanel:CreateRow();\n"
			"end\n");
	OutputFile();	
	//UIChatRow *LastRow = m_TempChatPanel->GetLastRow();
	sprintf(m_TempStr, "LastRow = MyChatPanel:GetLastRow();\n");
	OutputFile();	
	//UIChatString *ChatString = SafeCreateObject(UIChatString);
	sprintf(m_TempStr, "MyChatString = LuaUI.LuaCreateUIChatString();\n");
	OutputFile();	

	//TakeChatString(ChatString, pElement);
	TakeChatString(pElement);
	//LastRow->AddItem(ChatString);
	
	sprintf(m_TempStr, "LastRow:AddItem(MyChatString);\n");
	OutputFile();	

	return 1;
}


Int32 XMLBIN_UI::ChatAddStringC(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		/*
		StringW m_TempStr;

		UTF82StringW(m_TempStr, Text);
		m_TempChatPanel->AddStringW(m_TempStr.c_str());
		*/
		sprintf(m_TempStr, "MyChatPanel:AddStringA(\"%s\");\n", Text);
		OutputFile();
	}

	return 1;
}


void XMLBIN_UI::TakeChatLink(TiXmlElement *pElement)
{
	PCSTRINGA Link = pElement->Attribute("LinkTo");
	if(Link)
	{
		/*
		Int32 Len = MultiByteToWideChar(CP_UTF8, 0, Link, -1, NULL, 0);
		PSTRINGW m_TempStr = SafeAllocate(CharW, Len);
		MultiByteToWideChar(CP_UTF8, 0, Link, -1, m_TempStr, Len);
		ChatLink->SetLinkString(m_TempStr);
		SafeDeallocate(m_TempStr);
		*/

		sprintf(m_TempStr, "MyChatLink:SetLinkStringA(\"%s\");\n", Link);
		OutputFile();
	}

	sprintf(m_TempStr, "MyChatString = MyChatLink:GetChatStringPtr();\n");
	OutputFile();

	TakeChatString(pElement);
}



Int32 XMLBIN_UI::ChatAddLink(TiXmlNode *pNode, TiXmlElement *pElement)
{
	/*
	if(!m_TempChatPanel->GetLastRow())
		m_TempChatPanel->CreateRow();

	UIChatRow *LastRow = m_TempChatPanel->GetLastRow();
	UIChatLink *ChatLink = SafeCreateObject(UIChatLink);
	TakeChatLink(ChatLink, pElement);
	LastRow->AddItem(ChatLink);
	*/
	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();

	sprintf(m_TempStr, "LastRow = MyChatPanel:GetLastRow();\n");
	OutputFile();
	sprintf(m_TempStr, "if LastRow == nil then\n");
	OutputFile();
	sprintf(m_TempStr, "    LastRow =  MyChatPanel:CreateRow();\n");
	OutputFile();
	sprintf(m_TempStr, "end\n");
	OutputFile();
	sprintf(m_TempStr, "MyChatLink = LuaUI.LuaCreateUIChatLink();\n");
	OutputFile();
	TakeChatLink(pElement);

	return 1;
}


void XMLBIN_UI::TakeChatIcon(TiXmlElement *pElement)
{
	PCSTRINGA ImageName = pElement->Attribute("Image");
	if(ImageName)
	{
		Int32	Left, Top, Right, Bottom;

		/*
		StringW Filename;
		UTF82StringW(Filename, ImageName);
		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		*/

		pElement->Attribute("Left", &Left);
		pElement->Attribute("Top", &Top);
		pElement->Attribute("Right", &Right);
		pElement->Attribute("Bottom", &Bottom);

		//ChatIcon->SetImage(Image);
		//ChatIcon->SetRect(Left, Top, Right, Bottom);

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		sprintf(m_TempStr, "MyChatUIcon::SetImageFileA(\"%s\", %d, %d, %d, %d);\n", FileStr.c_str(), Left, Top, Right, Bottom);
		OutputFile();
	}

	sprintf(m_TempStr, "MyChatLink = MyChatLink:GetChatLinkPtr();\n");
	OutputFile();

	TakeChatLink(pElement);
}


Int32 XMLBIN_UI::ChatAddIcon(TiXmlNode *pNode, TiXmlElement *pElement)
{
	/*
	if(!m_TempChatPanel->GetLastRow())
		m_TempChatPanel->CreateRow();
	UIChatRow *LastRow = m_TempChatPanel->GetLastRow();
	UIChatIcon *ChatIcon = SafeCreateObject(UIChatIcon);
	TakeChatIcon(ChatIcon, pElement);

	LastRow->AddItem(ChatIcon);
	*/

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();

	sprintf(m_TempStr, "LastRow = MyChatPanel:GetLastRow();\n");
	OutputFile();
	sprintf(m_TempStr, "if LastRow == nil then\n");
	OutputFile();
	sprintf(m_TempStr, "    LastRow = MyChatPanel:CreateRow();\n");
	OutputFile();
	sprintf(m_TempStr, "end\n");
	OutputFile();
	sprintf(m_TempStr, "MyChatIcon = LuaUI.LuaCreateUIChatIcon();\n");
	OutputFile();
	TakeChatIcon(pElement);


	return 1;
}


Int32 XMLBIN_UI::CreateChatPanel(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32		Width, Height;
	Int32		ContinueRun;

	pElement->Attribute("Width", &Width);
	pElement->Attribute("Height", &Height);
	pElement->Attribute("ContinueRun", &ContinueRun);

	//m_TempChatPanel = SafeCreateObject(UIChatPanel);

	sprintf(m_TempStr, "-------------------------->>>> Create\n");
	OutputFile();
	sprintf(m_TempStr, "MyChatPanel = LuaUI.LuaCreateUIChatPanel();\n");
	OutputFile();
	sprintf(m_TempStr, "MyText = MyChatPanel:GetUIStaticText();\n");
	OutputFile();
	sprintf(m_TempStr, "MyItem = MyChatPanel:GetUIItemPtr();\n");
	OutputFile();


	TakeBaseItemAttr(pElement);
	TakeStaticTextAttr(pElement);
	//m_TempChatPanel->SetImage(Width, Height);
	sprintf(m_TempStr, "MyChatPanel:SetImage(%d, %d);\n", Width, Height);
	OutputFile();
	//m_TempChatPanel->SetContinueRun(ContinueRun > 0);
	sprintf(m_TempStr, "MyChatPanel:SetContinueRun(%s);\n", (ContinueRun > 0 ? "true" : "false"));
	OutputFile();

	ReadElement(pNode->Value(), pElement, m_ChatPanelFuncMap);
	
	/*
	m_TempChatPanel->HomeRow();
	m_TempChatPanel->FillImage();
	m_TempChatPanel->Redraw();
	m_TempChatPanel->SetLuaState(m_LuaState);
	m_TempChatPanel->UpdateDetectRect();
	*/

	sprintf(m_TempStr, "MyChatPanel:HomeRow();\n");
	OutputFile();
	sprintf(m_TempStr, "MyChatPanel:FillImage(0);\n");
	OutputFile();
	sprintf(m_TempStr, "MyChatPanel:Redraw();\n");
	OutputFile();
	sprintf(m_TempStr, "MyChatPanel:SetLuaState(LuaState);\n");
	OutputFile();
	sprintf(m_TempStr, "MyChatPanel:UpdateDetectRect();\n");
	OutputFile();

	//m_PanelStack.back()->AddUIItem(m_TempChatPanel);
	//m_TempUIManager->AddUpdateUIItem(m_TempChatPanel);

	sprintf(m_TempStr, "UIManager:AddUpdateUIItem(MyChatPanel);\n");
	OutputFile();	
	sprintf(m_TempStr, "MyPanel:AddUIItem(MyChatPanel);\n------------------\n");
	OutputFile();	
	

	
	//m_TempChatPanel = NULL;

	return 1;
}


Int32 XMLBIN_UI::CreateUIMiniMap(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA ImageName = pElement->Attribute("MapImage");
	if(ImageName)
	{
		/*
		StringW Filename;
		
		UTF82StringW(Filename, ImageName);
		CImage	*MapImage = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		*/

		
		//UIMiniMap	*MiniMap = SafeCreateObject(UIMiniMap);

		sprintf(m_TempStr, "-------------------------->>>> Create\n");
		OutputFile();
		sprintf(m_TempStr, "MyMiniMap = LuaUI.LuaCreateUIMiniMap();\n");
		OutputFile();
		sprintf(m_TempStr, "MyItem = MyMiniMap:GetUIItemPtr();\n");
		OutputFile();

		TakeBaseItemAttr(pElement);

		Int32 X, Y; 
		pElement->Attribute("X", &X);
		pElement->Attribute("Y", &Y);
		//MiniMap->SetOffsetXY(X, Y);

		sprintf(m_TempStr, "MyMiniMap:SetOffsetXY(%d, %d);\n", X, Y);
		OutputFile();

		//MiniMap->SetImage(MapImage);

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		sprintf(m_TempStr, "MyMiniMap:SetImageA(\"%s\");\n", FileStr.c_str());
		OutputFile();

		//CImage *MaskImage = CImageManager::GetInstance().CreateImage(_T("Data\\Mask.TGA"), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		//MiniMap->SetMask(MaskImage);

		sprintf(m_TempStr, "MyMiniMap:SetDefaultMask();\n");
		OutputFile();	

		PCSTRINGA PlayerImageName = pElement->Attribute("PlayerIcon");
		Int32 PlayerIconLeft, PlayerIconTop,PlayerIconRight, PlayerIconBottom;
		/*
		CImage	*PlayerImage = CImageManager::GetInstance().CreateImage(String(PlayerImageName).c_str(), 
			D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		MiniMap->SetPlayerImage(PlayerImage);
		*/

		MakeLuaFIleStrA(FileStr, PlayerImageName);

		sprintf(m_TempStr, "MyMiniMap:SetPlayerImageA(\"%s\");\n", FileStr.c_str()); 
		OutputFile();	

		pElement->Attribute("PlayerIconLeft", &PlayerIconLeft);
		pElement->Attribute("PlayerIconTop", &PlayerIconTop);
		pElement->Attribute("PlayerIconRight", &PlayerIconRight);
		pElement->Attribute("PlayerIconBottom", &PlayerIconBottom);
		//MiniMap->SetPlayerImageRect(PlayerIconLeft, PlayerIconTop, PlayerIconRight, PlayerIconBottom);

		sprintf(m_TempStr, "MyMiniMap:SetPlayerImageRect(%d, %d, %d, %d);\n", PlayerIconLeft, PlayerIconTop, PlayerIconRight, PlayerIconBottom); 
		OutputFile();	

		//PlayerIcon
		//m_PanelStack.back()->AddUIItem(MiniMap);
		sprintf(m_TempStr, "MyPanel:AddUIItem(MyMiniMap);\n------------------\n");
		OutputFile();	
	}

	return 1;
}




Int32	XMLBIN_UI::CreateUIFont(TiXmlNode *pNode, TiXmlElement *pElement)
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


Int32	XMLBIN_UI::SetButtonRect(TiXmlNode *pNode, TiXmlElement *pElement)
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
		//UIAniIcon	*pAniIcon = SafeCreateObject(UIAniIcon);

		sprintf(m_TempStr, "-------------------------->>>> Create\n");
		OutputFile();
		sprintf(m_TempStr, "MyAniIcon = LuaUI.LuaCreateUIAniIcon();\n");
		OutputFile();

		Int32	Rotation, AnimationIndex, Loop, Align, Detect, Lock;

		pElement->Attribute("Rotation", &Rotation);
		pElement->Attribute("AnimationIndex", &AnimationIndex);
		pElement->Attribute("Loop", &Loop);
		pElement->Attribute("Align", &Align);
		pElement->Attribute("Detect", &Detect);
		pElement->Attribute("LockResources", &Lock);
		
		/*
		pAniIcon->SetAlign(Align);
		pAniIcon->SetAnimationIndex(AnimationIndex);
		pAniIcon->SetRotation(Rotation);
		pAniIcon->SetDetect(Detect);
		pAniIcon->SetAnimationData(String(ImageName).c_str());
		*/

		StringA FileStr;
		MakeLuaFIleStrA(FileStr, ImageName);

		sprintf(m_TempStr, "MyAniIcon:SetAlign(%d);\n", Align);
		OutputFile();
		sprintf(m_TempStr, "MyAniIcon:SetAnimationIndex(%d);\n", AnimationIndex);
		OutputFile();
		sprintf(m_TempStr, "MyAniIcon:SetRotation(%d);\n", Rotation);
		OutputFile();
		sprintf(m_TempStr, "MyAniIcon:SetDetect(%d);\n", Detect);
		OutputFile();
		sprintf(m_TempStr, "MyAniIcon:SetAnimationDataA(\"%s\");\n", FileStr.c_str());
		OutputFile();

		if (Lock)
		{
			//pAniIcon->GetAniEnt()->LockResources();
			sprintf(m_TempStr, "MyAniIcon:LockResources();\n");
			OutputFile();
		}
		//pAniIcon->Update(::timeGetTime());
		//pAniIcon->SetOffsetXY(X + m_TempButton->GetOffsetX(), Y  + m_TempButton->GetOffsetY());
		//m_TempButton->SetAniIcon(ButtonState, pAniIcon);
		//m_TempButton->SetRect(ButtonState, 0, 0, pAniIcon->GetWidth(), pAniIcon->GetHeight());
		//m_TempUIManager->AddUpdateUIItem(m_TempButton);

		sprintf(m_TempStr, "MyAniIcon:UpdateNow();\n");
		OutputFile();
		sprintf(m_TempStr, "MyAniIcon:SetOffsetXY(%d + MyButton:GetOffsetX(), %d + MyButton:GetOffsetY());\n", X, Y);
		OutputFile();
		sprintf(m_TempStr, "MyButton:SetAniIcon(%d, MyAniIcon);\n", ButtonState);
		OutputFile();
		sprintf(m_TempStr, "MyButton:SetRect(%d, 0, 0, MyAniIcon:GetWidth(), MyAniIcon:GetHeight());\n",
			ButtonState);
		OutputFile();
		sprintf(m_TempStr, "UIManager:AddUpdateUIItem(MyButton);\n");
		OutputFile();
	}
	else
	{
		//m_TempButton->SetRect(ButtonState, Left, Top, Right, Bottom);
		sprintf(m_TempStr, "MyButton:SetRect(%d, %d, %d, %d, %d);\n",
			ButtonState, Left, Top, Right, Bottom);
		OutputFile();
	}

	//m_TempButton->SetSubOffSet(ButtonState, X + m_TempButton->GetOffsetX(), Y  + m_TempButton->GetOffsetY());
	sprintf(m_TempStr, "MyButton:SetSubOffSet(%d, %d + MyButton:GetOffsetX(), %d + MyButton:GetOffsetY());\n",
		ButtonState, X, Y);
		OutputFile();

	return 1;
}


Int32	XMLBIN_UI::SetCheckBoxRect(TiXmlNode *pNode, TiXmlElement *pElement)
{
	Int32		Index;
	Int32		Left, Top, Right, Bottom;
	
	pElement->Attribute("Index", &Index);
	pElement->Attribute("Left", &Left);
	pElement->Attribute("Top", &Top);
	pElement->Attribute("Right", &Right);
	pElement->Attribute("Bottom", &Bottom);

	//m_TempCheckBox->SetRect(Index, Left, Top, Right, Bottom);
	sprintf(m_TempStr,"MyCheckBox:SetRect(%d, %d, %d, %d, %d);\n", Index, Left, Top, Right, Bottom);
	OutputFile();

	return 1;
}


Int32	XMLBIN_UI::ListBoxAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		/*
		StringW m_TempStr;

		UTF82StringW(m_TempStr, Text);
		m_TempListBox->AddString(m_TempStr.c_str());
		*/
	
		sprintf(m_TempStr,"MyListBox:AddStringA(\"%s\");\n", Text);
		OutputFile();
	}

	return 1;
}


Int32 XMLBIN_UI::PopUpAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	Int32	Code;
	
	pElement->Attribute("Code", &Code);

	if(Text)
	{
		/*
		StringW m_TempStr;

		UTF82StringW(m_TempStr, Text);
		*/

		PCSTRINGA	NextMenu = pElement->Attribute("NextMenu");

		//m_TempPopUpMenu->AddString(m_TempStr.c_str(), Code, (NextMenu ? NextMenu :  ""));
		sprintf(m_TempStr,"MyPopUpMenu:AddStringA(\"%s\", %d, \"%s\");\n", Text, Code, (NextMenu ? NextMenu :  ""));
		OutputFile();
	}

	return 1;
}



Int32 XMLBIN_UI::STAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		/*
		StringW m_TempStr;

		UTF82StringW(m_TempStr, Text);
		
		m_TempStaticText->AddText(m_TempStr);
		*/

		sprintf(m_TempStr,"MyStaticText:AddTextA(\"%s\");\n", Text);
		OutputFile();
	}

	return 1;
}

Int32 XMLBIN_UI::DTAddString(TiXmlNode *pNode, TiXmlElement *pElement)
{
	PCSTRINGA Text = pElement->GetText();

	if(Text)
	{
		/*
		StringW m_TempStr;

		UTF82StringW(m_TempStr, Text);
		
		m_TempDynamicText->AddText(m_TempStr);
		*/

		sprintf(m_TempStr,"MyDynamicText:AddTextA(\"%s\");\n", Text);
		OutputFile();
	}

	return 1;
}
