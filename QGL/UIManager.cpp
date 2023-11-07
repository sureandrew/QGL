//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "UI.h"
//-- Library
#include "QGLApplication.h"
#include "Math.h"
#include <zmouse.h>
#include "Global.h"
#include "Configure.h"

#include <usp10.h>
#include <dimm.h>

#include <strsafe.h>

#include "LuaPush.h"
#include "Common/CallLuaFunc.h"
#include "XMLPanel.h"
#include "MenuFriendProcess.h"
#include "MenuSystemSetting.h"

UIManager::UIManager()
{
	m_UIPanelList.clear();
	m_LockUIPanelList.clear();
	m_WaitDelUIPanelList.clear();
	m_LuaState = NULL;
	m_Focus = 
	m_WaitItem = 
	m_MouseOverItem = 
	m_LeaveTimerItem = 
	m_MoveInTimerItem =
	m_MonoPanel = NULL;
	m_OldCurPanel = NULL;
	m_TempDragPanel =
	m_LClickPanel = m_RClickPanel = NULL;

	m_DragItemParent = NULL;
	m_CursorX = m_CursorY = 0;
	m_CursorOffsetX = m_CursorOffsetY = 0;
	m_CursorPreX = m_CursorPreY = 0;
	m_BackImage = NULL;
	m_OutputTexture = NULL;
	m_BackFilledImage = NULL;
	m_KeyInItem = NULL;
	m_Event.clear();
	m_RenderAllFlag = 0;
	m_StartTime = m_Now = GetTickCount();
	m_LuaShareFlag = false;

	m_LockMonoFlag = false;
	m_MouseMoveFlag = false;
	m_FreezeFlag = false;

	m_DragItem = NULL;
	m_DropPanel = NULL;
	m_DragNow = m_TipsNow = false;

	//m_DragX = 0;
	//m_DragY = 0;

	m_MouseOnUIFlag = 0;

	m_UIPanelListPtr = &m_UIPanelList;

	m_LuaFuncName = "";

	m_IMETexture = NULL;
}

	
void UIManager::Release()
{
	SafeRelease(m_IMETexture);

	for(UInt i = 0; i < m_UIPanelList.size(); i++)
		SafeDeleteObject(m_UIPanelList[i]);
	for(UInt i = 0; i < m_WaitDelUIPanelList.size(); i++)
		SafeDeleteObject(m_WaitDelUIPanelList[i]);
	
	m_UIPanelList.clear();
	m_LockUIPanelList.clear();
	m_WaitDelUIPanelList.clear();
	m_UIIDMap.clear();
	m_Event.clear();
	m_UIPanelMap.clear();
	m_UIPanelPosRec.clear();

	if(!m_LuaShareFlag)
		SafeCloseLua(m_LuaState);

	/*
	for(i = 0; i < m_ImageList.size(); ++i)
	{
		CImageManager::GetInstance().DeleteImage(m_ImageList[i].c_str());
	}
	*/

	if (m_BackFilledImage)
		SafeDeleteObject(m_BackFilledImage);
}


/*
void UIManager::InitIME()
{
	IMEInit();
	IME_Layout::GetInstance().Init();
	SafeRelease(m_IMETexture);
	D3DXCreateTexture(m_Render->GetDevice(), max(IME_CANDIDATE_WIDTH, IME_INPUT_WIDTH), max(IME_CANDIDATE_HEIGHT, IME_INPUT_HEIGHT), 0, 0 ,
					D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_IMETexture);
	IME_Layout::GetInstance().SetTexture(m_IMETexture);
}
*/

bool UIManager::Detect(PCSTRINGA Name, Int32 CursorX, Int32 CursorY)
{
	UIItem *Item = GetUIItem(Name);
	if(Item)
		return Item->Detect(CursorX, CursorY);
	return false;
}


void UIManager::SetKeyInItem(PCSTRINGA pName)
{ 
	UIItem *TempUIItem = GetUIItem(pName); 

	if (TempUIItem)
	{
		if(m_KeyInItem && TempUIItem != m_KeyInItem)
			m_KeyInItem->SetFocus(false);  
		m_KeyInItem = TempUIItem;
		m_KeyInItem->SetFocus(true); 
	}
}


void UIManager::SetKeyInItem(UIItem *Item)
{
	if(m_KeyInItem && Item != m_KeyInItem)
		m_KeyInItem->SetFocus(false);  
	m_KeyInItem = Item;
	m_KeyInItem->SetFocus(true); 
}


void UIManager::SetBackImageA(PCSTRINGA ImageName, UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom)
{
	StringW Filename;

	UTF82StringW(Filename, ImageName);
	m_BackImage = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);

	if(Left == 0 && Top == 0 && Right == 0 && Bottom == 0)
	{
		SetRect(&m_BackRect, 0, 0, m_BackImage->GetWidth(), m_BackImage->GetHeight());
	}
	else
	{
		SetRect(&m_BackRect, Left, Top, Right, Bottom);
	}

	ReFillBackImage();

}

void UIManager::SetBackImage(CImage *pImage, UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom)
{
	m_BackImage = pImage;
	if(Left == 0 && Top == 0 && Right == 0 && Bottom == 0)
	{
		SetRect(&m_BackRect, 0, 0, pImage->GetWidth(), pImage->GetHeight());
	}
	else
	{
		SetRect(&m_BackRect, Left, Top, Right, Bottom);
	}

	ReFillBackImage();
}



UInt32 UIManager::IconTimer()
{
	static UInt32 OldOffset = 0;
	m_Now = GetTickCount();
	if(m_Now < m_StartTime)
	{
		m_StartTime = m_Now;
	}
	
	m_AniIconTimeOffset = (m_Now - m_StartTime) / 512;

	if(m_AniIconTimeOffset != 0)
	{
		OldOffset = m_AniIconTimeOffset;
		return 1;
	}
	else
	{
		return 0;
	}
}



void UIManager::ReFillBackImage()
{
	if (m_OutputTexture && m_BackImage)
	{
		if (m_BackFilledImage)
			SafeDeleteObject(m_BackFilledImage);
		
		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIManager::ReFillBackImage(),  SafeDeleteObject(m_BackFilledImage); m_BackFilledImage = %p"),
				m_BackFilledImage);

		m_BackFilledImage = SafeCreateObject2(CImage, m_OutputTexture, 0);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIManager::ReFillBackImage(),  SafeCreateObject2(CImage, m_OutputTexture, 0); m_BackFilledImage = %p"),
				m_BackFilledImage);

		m_BackFilledImage->Lock();
		UInt32 Width = m_BackRect.right - m_BackRect.left;
		UInt32 Height = m_BackRect.bottom - m_BackRect.top; 
		UInt32 WidthCount = m_BackFilledImage->GetWidth() / Width;
		UInt32 LastWidth = m_BackFilledImage->GetWidth() % Width;
		UInt32 HeightCount = m_BackFilledImage->GetHeight() / Height;
		UInt32 LastHeight = m_BackFilledImage->GetHeight() % Height;

		

		Alpha8888To8888(0, 0, m_BackImage, m_BackFilledImage, &m_BackRect);

		
		UInt32	Y = 0;
		for(UInt32 i = 0; i < HeightCount; ++i)
		{
			UInt32 X = 0;
			for(UInt32 j = 0; j < WidthCount; ++j)
			{
				Alpha8888To8888(X, Y, m_BackImage, m_BackFilledImage, &m_BackRect);
				X += Width;
			}

			RECT	SRect;

			if(LastWidth)
			{
				SetRect(&SRect, m_BackRect.left, m_BackRect.top, m_BackRect.left + LastWidth, m_BackRect.bottom);
				Alpha8888To8888(X, Y, m_BackImage, m_BackFilledImage, &SRect);
			}

			Y += Height;
		}


		if(LastHeight)
		{
			RECT	SRect;

			SetRect(&SRect, m_BackRect.left, m_BackRect.top, m_BackRect.left + Width, m_BackRect.top + LastHeight);

			UInt32 X = 0;
			for(UInt32 j = 0; j < WidthCount; ++j)
			{
				Alpha8888To8888(X, Y, m_BackImage, m_BackFilledImage, &m_BackRect);
				X += Width;
			}

			

			if(LastWidth)
			{
				SRect.right = m_BackRect.left + LastWidth;
				Alpha8888To8888(X, Y, m_BackImage, m_BackFilledImage, &SRect);
			}
		}
		
		m_BackFilledImage->Unlock();
	}
}


void UIManager::LoadLuaUI(PCSTRINGA Filename)
{
	CODEREPLACE_START;

	if(!m_LuaState)
	{
		gGlobal.InitUILua(m_LuaState);
	}

	if(luaL_loadfile(m_LuaState, Filename))
	{
		TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(m_LuaState, -1)).c_str());
	}
	else
	{
		LuaPush(m_LuaState, this);
		lua_setglobal(m_LuaState, "MyUIManager"); 
		if(lua_pcall(m_LuaState, 0, 0, 0))
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(m_LuaState, -1)).c_str());
		}
	}

	CODEREPLACE_END;
}


void UIManager::SavePanelPos(PCSTRINGW Filename)
{
	FILE *File = _tfopen(Filename, _T("w"));
	StringA TempStrA;

	TempStrA = "<?xml version=\"1.0\" ?>\n";
	fwrite(TempStrA.c_str(), TempStrA.size(), 1, File);
	TempStrA = "<!-- Our to do list data -->\n<Position>\n";
	fwrite(TempStrA.c_str(), TempStrA.size(), 1, File);

	StlMap<StringA, std::pair<Int32, Int32> >::iterator i;
	for(i = m_UIPanelPosRec.begin(); i != m_UIPanelPosRec.end(); ++i)
	{
		TempStrA.Format("     <PanelPos Name=\"%s\" X=\"%d\" Y=\"%d\" />\n",
			(*i).first.c_str(), (*i).second.first, (*i).second.second);
		fwrite(TempStrA.c_str(), TempStrA.size(), 1, File);
	}
	TempStrA = "</Position>\n";
	fwrite(TempStrA.c_str(), TempStrA.size(), 1, File);

	fclose(File);
}

void UIManager::LoadPanelPos(PCSTRINGW Filename)
{
	StringA XMLFile = Filename;
	TiXmlDocument doc( XMLFile.c_str() );

	
	m_UIPanelPosRec.clear();

	doc.LoadFile();
	if(!doc.Error())
	{

		TiXmlHandle	docH( &doc );
		TiXmlElement	*root = doc.FirstChildElement( );

		TiXmlElement	*Element = root->FirstChildElement();
		TiXmlNode		*Node = Element;

		while (Element)
		{
			const char *NodeValue = Node->Value();

			if(NodeValue)
			{
				if(!strcmp(NodeValue, "PanelPos"))
				{
					Int32	X, Y;
					const char *Name = Element->Attribute("Name");
					Element->Attribute("X", &X);
					Element->Attribute("Y", &Y);

					m_UIPanelPosRec[Name] = std::pair<Int32, Int32>(X, Y);
				}
			}
			
			Element = (TiXmlElement *)Element->NextSibling();
			Node = Element;
		}
	}
}

void UIManager::ClearWaitDelPanel()
{
	StlMap<StringA, UIPanel *>::iterator it;

	for(UInt i = 0; i < m_WaitDelUIPanelList.size(); i++)
	{
		it = m_UIPanelMap.find(m_WaitDelUIPanelList[i]->GetName()->c_str());

		if(it != m_UIPanelMap.end())
		{
			m_UIPanelMap.erase(it);
		}

		SafeDeleteObject(m_WaitDelUIPanelList[i]);
	}
	m_WaitDelUIPanelList.clear();
}


void UIManager::AddUIPanel(UIPanel *pUIPanel)
{
	ClearWaitDelPanel();
	if(pUIPanel)
	{
		pUIPanel->SetUIManager(this);
		for (Int i = 0; i < (Int)pUIPanel->GetList().size(); i++)
		{
			pUIPanel->GetList()[i]->SetUIManager(this);
		}
		m_UIPanelList.push_back(pUIPanel);

		
		if(!pUIPanel->GetName()->IsEmpty())
		{
			PCSTRINGA	PanelName = pUIPanel->GetName()->c_str();

			StlMap<StringA, std::pair<Int32, Int32> >::iterator it = m_UIPanelPosRec.find(PanelName);
			if(it != m_UIPanelPosRec.end())
			{
				Int32		X = (*it).second.first, Y = (*it).second.second;
				
				pUIPanel->SetPos(X, Y);
				pUIPanel->SetDetectRect();
			}
		}


		AddDirtyUIItem(pUIPanel);
	}
}


void UIManager::RenderAll()
{
	IconTimer();

	m_DirRectMan.AddAllDirty();


	if (m_OutputTexture)
	{
		CImage	*Image2 = SafeCreateObject2(CImage, m_OutputTexture, 1);
		Image2->Lock();

		Image2->GetTexture()->AddDirtyRect(NULL);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIManager::RenderAll(),  SafeCreateObject2(CImage, m_OutputTexture, 1); Image2 = %p"),
				Image2);
		
		if(m_BackImage)
		{			
			RECT TotalRect;
			SetRect(&TotalRect, 0, 0, Image2->GetWidth(), Image2->GetHeight());
			Copy8888To8888(0, 0, m_BackFilledImage, Image2, &TotalRect);
			
		}
		Image2->Unlock();
		SafeDeleteObject(Image2);


		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIManager::RenderAll(),  SafeDeleteObject(m_BackFilledImage); m_BackFilledImage = %p"),
				m_BackFilledImage);

	}

	if(m_DragNow)
		m_DragItem->SetEnable(false);
	

	for(std::vector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if((*i)->IsEnable())
		{
			(*i)->Render(NULL);
		}
	}

	
	if(m_DragNow)
	{
		m_DragItem->SetEnable(true);
		m_TempDragPanel->Render(NULL);
	}

	
	if (gGlobal.pUIManager == this)
		gGlobal.g_DrawAll = TRUE;
}

void UIManager::RenderAllPanelsByRects()
{
	for(std::vector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		(*i)->RenderDirtyRect();
	}
}
void UIManager::RemoveAllPanelsRects()
{
	for(std::vector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		(*i)->ClearDirtyRect();
	}
}

void UIManager::BlitAll(LPD3DXSPRITE pSprite, Int X, Int Y, RECT * pRect)
{
	BOOL DragPanelDrawed = false;
	for(std::vector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if((*i)->IsEnable())
		{
			UIPanel* Panel = (UIPanel *)(*i);
			
			if (m_TempDragPanel == Panel)
				DragPanelDrawed = true;

			RECT realRect;
			Panel->SetRealRect(realRect);

			if (pRect)
			{
				if (!QGLMath::CalculateOverlappedRect(realRect, *pRect, realRect))
					continue;
			}
			D3DXVECTOR3 LeftUpPos;

			LeftUpPos.x = float(realRect.left + X);
			LeftUpPos.y = float(realRect.top + Y);
			LeftUpPos.z = 0;

			RECT	Rect;
			SetRect(&Rect, realRect.left - Panel->GetX(), realRect.top - Panel->GetY(),
				realRect.right - Panel->GetX(), realRect.bottom - Panel->GetY());

			HRESULT hr = pSprite->Draw(Panel->GetTexture(), &Rect, NULL, &LeftUpPos, 0xFFFFFFFF);
			if (FAILED(hr))
			{
				TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Draw Sprite Failed"));
				if (hr == D3DERR_INVALIDCALL)
				{
					MessageBox(NULL,_T("Draw Sprite Failed. D3DERR_INVALIDCALL"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Draw Sprite Failed. D3DERR_INVALIDCALL"));
				}
				else if (hr == D3DXERR_INVALIDDATA)
				{
					MessageBox(NULL,_T("Draw Sprite Failed. D3DXERR_INVALIDDATA"),_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
					TRACE_ERRORDTL(GLOBAL_LOGGER, _F("Draw Sprite Failed. D3DXERR_INVALIDDATA"));
				}
			}
		}
	}

	if (!DragPanelDrawed && m_TempDragPanel)
	{
		if(m_TempDragPanel->IsEnable())
		{
			UIPanel* Panel = m_TempDragPanel;

			RECT realRect;
			Panel->SetRealRect(realRect);

			if (pRect)
			{
				if (!QGLMath::CalculateOverlappedRect(realRect, *pRect, realRect))
					return;
			}
			D3DXVECTOR3 LeftUpPos;

			LeftUpPos.x = float(realRect.left + X);
			LeftUpPos.y = float(realRect.top + Y);
			LeftUpPos.z = 0;

			RECT	Rect;
			SetRect(&Rect, realRect.left - Panel->GetX(), realRect.top - Panel->GetY(),
				realRect.right - Panel->GetX(), realRect.bottom - Panel->GetY());
			pSprite->Draw(Panel->GetTexture(), &Rect, NULL, &LeftUpPos, 0xFFFFFFFF);
		}
	}


	
}

void UIManager::SetDragPanel2Top()
{
	if(m_DragItem)
	{
		for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
		{
			if((*i) == m_DragItem)
			{
				m_UIPanelList.erase(i);
				m_UIPanelList.push_back((UIPanel *)m_DragItem);
				break;
			}
		}
	}
}

void UIManager::SetPanel2Top(UIPanel * pPanel)
{
	if (pPanel == NULL || (UIItem *)pPanel == m_UIPanelList.back())
		return;

	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if((*i) == (UIItem *)pPanel)
		{
			m_UIPanelList.erase(i);
			m_UIPanelList.push_back(pPanel);
			AddDirtyUIItem(pPanel);
			break;
		}
	}
}


void UIManager::SetPanelEx2Top(UIPanelEx* pPanel)
{
	if (pPanel == NULL || (UIItem *)pPanel == m_UIPanelList.back())
		return;

	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if((*i) == (UIItem *)pPanel)
		{
			m_UIPanelList.erase(i);
			m_UIPanelList.push_back(pPanel);
			AddDirtyUIItem(pPanel);
			break;
		}
	}
}


void UIManager::SetPanel2Bottom(UIPanel * pPanel)
{
	if (pPanel == NULL || (UIItem *)pPanel == m_UIPanelList.front())
		return;

	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if((*i) == (UIItem *)pPanel)
		{
			m_UIPanelList.erase(i);
			m_UIPanelList.insert(m_UIPanelList.begin(), pPanel);
			AddDirtyUIItem(pPanel);
			break;
		}
	}
}


void UIManager::SetPanelEx2Bottom(UIPanelEx* pPanel)
{
	if (pPanel == NULL || (UIItem *)pPanel == m_UIPanelList.front())
		return;

	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if((*i) == (UIItem *)pPanel)
		{
			m_UIPanelList.erase(i);
			m_UIPanelList.insert(m_UIPanelList.begin(), pPanel);
			AddDirtyUIItem(pPanel);
			break;
		}
	}
}


void UIManager::ResetTipsPanel(UIItem *Item)
{
	UIPanel *TipsPanel = FindUIPanel("TipsPanel");
	UIPanelEx *TipsPanelEx = NULL;
	UIChatPanel *pChat = FindUIChatPanel("TipsChat");

	if(!TipsPanel)
	{
		TipsPanel = TipsPanelEx = FindUIPanelEx("TipsPanel");
	}

	if(TipsPanel)// && pText)
	{
		RECT realRectNew;

		TipsPanel->SetEnable(true);
		if(pChat)
		{
			pChat->ClearAllRow();
			pChat->AddStringW(Item->GetTips());
			pChat->HomeRow();
			pChat->Redraw();

			UInt32 uRow = pChat->RowListCount();

			if ( uRow > 0 )
			{
				pChat->SetEnable(true);

				pChat->SetImage(pChat->GetWidth(), uRow * 19 );
				pChat->SetDetectRect();
		
				pChat->HomeRow();
				pChat->Redraw();

				UInt32 Width = 0;

				if(uRow == 1)
				{
					UIChatRow *pRow = pChat->GetHomeRow();
					Width = pRow->GetWidth() + 6;
				}
				else
				{
					Width = 0;
					
					for(UInt i = 0; i < uRow; ++i)
					{
						UIChatRow *pRow = pChat->GetRow(i);
						if(pRow->GetWidth() > Width)
							Width = pRow->GetWidth();
					}
					Width +=  6;
				}

				if(TipsPanelEx)
					TipsPanelEx->ChangeCount(uRow);
				else
				{
					TipsPanel->SetWH(Width, uRow * 19 + 6);
					TipsPanel->SetTextureWH(Width, uRow * 19 + 6);
					TipsPanel->SetTextureUI(this);
					TipsPanel->SetDetectRect();
	
				}
			}
		}

		Int32	X = m_CursorX, Y = m_CursorY + 24;

		if(X + TipsPanel->GetWidth() > 800)
		{
			X = 800 - TipsPanel->GetWidth();
		}

		if(Y + TipsPanel->GetHeight() > 600 || Y > 600 - 24)
		{
			Y = m_CursorY - TipsPanel->GetHeight();
		}
		TipsPanel->Render(NULL);
		TipsPanel->SetPos(X, Y);
		TipsPanel->SetRealRect(realRectNew);
		SetPanel2Top(TipsPanel);
	}
	m_TipsNow = true;
}


void  UIManager::DeletePanel(PCSTRINGA ItemName)
{

	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if(!(*i)->GetName()->IsEmpty())
		{
			if(!(*i)->GetName()->Compare(ItemName))
			{
				if(m_LeaveTimerItem == *i)
					m_LeaveTimerItem = NULL;

				if(m_MonoPanel == *i)
				{
					m_MonoPanel = NULL;
					m_LockMonoFlag = false;
				}

				if (m_RClickPanel ==  *i)
				{
					m_RClickPanel = NULL;
				}

				if (m_LClickPanel ==  *i)
				{
					m_LClickPanel = NULL;
				}

				RECT realRect;
				(*i)->SetRealRect(realRect);
				m_DirRectMan.AddDirtyRectToMan(realRect);
				(*i)->DeleteMapItem(m_UIIDMap);
				(*i)->DeleteUpdateSet(m_AutoUpdateSet, m_MouseInOutEventSet);
				if (m_TempDragPanel == (*i))
					m_TempDragPanel = NULL;
				if(m_OldCurPanel == (*i))
					m_OldCurPanel = NULL;
				
				//(*i)->Dispose();

				AddPanelPos(*i);

				m_WaitDelUIPanelList.push_back(*i);
				//SafeDeleteObject(*i);
				m_UIPanelList.erase(i);

				m_MouseOverItem = 
				m_Focus = NULL;

				break;
			}
		}
	}
}


void  UIManager::DeletePanel(UIItem *Item)
{
	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if(*i == Item)
		{
			if(m_LeaveTimerItem == *i)
				m_LeaveTimerItem = NULL;

			if(m_MonoPanel == *i)
			{
				m_MonoPanel = NULL;
				m_LockMonoFlag = false;
			}

			RECT realRect;
				(*i)->SetRealRect(realRect);
			m_DirRectMan.AddDirtyRectToMan(realRect);

			(*i)->DeleteMapItem(m_UIIDMap);
			(*i)->DeleteUpdateSet(m_AutoUpdateSet, m_MouseInOutEventSet);
			if (m_TempDragPanel == (*i))
					m_TempDragPanel = NULL;
			(*i)->Dispose();

			if(!(*i)->GetName()->IsEmpty())
			{
				PCSTRINGA	PanelName = (*i)->GetName()->c_str();

				m_UIPanelPosRec[PanelName] = std::pair<Int32, Int32>((*i)->GetX(), (*i)->GetY());
			}

			m_WaitDelUIPanelList.push_back(*i);
			//SafeDeleteObject(*i);
			m_UIPanelList.erase(i);
			
			m_MouseOverItem =
			m_Focus = NULL;

			break;
		}
	}
}


void UIManager::SetMonoItem(UIItem *MonoPanel)
{
	if(!m_LockMonoFlag)
	{
		m_MonoPanel = MonoPanel;
		m_MouseOverItem = 
		m_Focus = 
		m_WaitItem = 
		m_LeaveTimerItem = 
		m_MoveInTimerItem =
			NULL;
	}
}


void UIManager::ClearMonoPanel()
{
	if(!m_LockMonoFlag)
	{
		m_MonoPanel =
		m_MouseOverItem = 
		m_Focus = 
		m_WaitItem = 
		m_LeaveTimerItem = 
		m_MoveInTimerItem =
			NULL;
	}
}

void UIManager::LeavePanel(PCSTRINGA ItemName)
{
	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if(!(*i)->GetName()->IsEmpty())
		{
			if(!(*i)->GetName()->Compare(ItemName))
			{
				CloseTips();
				m_UIPanelList.erase(i);
				break;
			}
		}
	}
}


void UIManager::LeavePanel(UIItem *Item)
{
	for(StlVector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		if(*i == Item)
		{
			CloseTips();
			m_UIPanelList.erase(i);
			break;
		}
	}

}


void UIManager::AddUIID(PCSTRINGA pName, UIItem *pUIItem)
{
	m_UIIDMap[pName] = pUIItem;
}

void UIManager::RemoveUIID(PCSTRINGA pName)
{
	StlMap<StringA, UIItem *>::iterator itr = m_UIIDMap.find(pName);
	if(itr != m_UIIDMap.end())
	{
		m_UIIDMap.erase(itr);
	}
}

UIItem *UIManager::GetUIItem(PCSTRINGA pName)
{
	StlMap<StringA, UIItem *>::iterator itr = m_UIIDMap.find(pName);
	if(itr != m_UIIDMap.end())
	{
		return itr->second;
	}

	return NULL;
}

void UIManager::SetUIItemEnable(PCSTRINGA name, bool value)
{
	StlMap<StringA, UIItem *>::iterator itr = m_UIIDMap.find(name);
	if(itr != m_UIIDMap.end())
	{
		itr->second->SetEnable(value);
	}
}

void UIManager::Update(UInt32 uTimeDelta)
{
	DWORD	Now = GetTickCount();

	if(m_LeaveTimerItem)
	{
		if(m_LeaveTimerItem->Detect(GetRender()->GetCursorPosX(), GetRender()->GetCursorPosY()))
			m_LeaveTimerItemStartTime = Now;
		else
		if(Now >= m_LeaveTimerItemStartTime + m_LeaveTimerItemShiftTime)
		{
			UIItem::LuaOnTimeOut(m_LeaveTimerItem, UIItem::TIMEOUT_LEAVE);
			m_LeaveTimerItem = NULL;
		}
	}

	if(m_MoveInTimerItem)
	{
		if(Now >= m_MoveInTimerItemStartTime + m_MoveInTimerItemShiftTime)
		{
			if(m_MoveInTimerItem->GetTypeID() == typeid(UITips *) || m_MoveInTimerItem->IsDefaultTips())
			{
				ResetTipsPanel(m_MoveInTimerItem);
			}
			else
			{
				UIItem::LuaOnTimeOut(m_MoveInTimerItem, UIItem::TIMEOUT_MOVEIN);
			}
			m_MoveInTimerItem = NULL;
		}
	}
	

	m_UpdateFlag = 0;


	Int j = 0;
	for(StlSet<UIItem *>::iterator i = m_AutoUpdateSet.begin(); i != m_AutoUpdateSet.end(); ++i)
	{
		if(*i)
		{
			if((*i)->IsEnable())
			{
				if((*i)->Update(uTimeDelta))
				{
					RECT RRect;
					(*i)->SetRealRect(RRect);
					m_DirRectMan.AddDirtyRectToMan(RRect);
					m_UpdateFlag = 1;
				}

				//check the timeout event
				if ((*i)->GetTimeOutCount() > 0)
				{
					//check time out event
					if ((*i)->GetEnableTimeOutEvent() && !(*i)->IsDefaultTips())
					{
						if (Now > (*i)->GetTimeOutPreTime() + (*i)->GetTimeOutCount())
						{
							(*i)->SetTimeOutPreTime(Now);
							UIItem::LuaOnTimeOut((*i), UIItem::TIMEOUT_COUNTDOWN);
						}
					}
					else
						(*i)->SetTimeOutPreTime(Now);
				}
			}
		}		

	}


	for(StlSet<UIItem *>::iterator i = m_MouseInOutEventSet.begin(); i != m_MouseInOutEventSet.end(); ++i)
	{
		if((*i)->IsEnable())
		{
			UIPanel *Panel = (UIPanel *)((*i)->IsPanel() ? *i : (*i)->GetParent());
				
			if(Panel->IsFreeze())
				continue;

			if ((*i)->GetTimeOutState() == -1)
			{
				if((*i)->Detect(GetRender()->GetCursorPosX(), GetRender()->GetCursorPosY()))
				{
					(*i)->SetTimeOutState(UIItem::TIMEOUT_MOVEIN);
					UIItem::LuaOnMouseIn((*i));
				}
				else
				{
					(*i)->SetTimeOutState(UIItem::TIMEOUT_LEAVE);
					UIItem::LuaOnMouseOut((*i));
				}
			}
			else if ((*i)->GetTimeOutState() == UIItem::TIMEOUT_MOVEIN)
			{
				if(!(*i)->Detect(GetRender()->GetCursorPosX(), GetRender()->GetCursorPosY()))
				{
					(*i)->SetTimeOutState(UIItem::TIMEOUT_LEAVE);
					UIItem::LuaOnMouseOut((*i));
				}
			}
			else if ((*i)->GetTimeOutState() == UIItem::TIMEOUT_LEAVE)
			{
				if((*i)->Detect(GetRender()->GetCursorPosX(), GetRender()->GetCursorPosY()))
				{
					(*i)->SetTimeOutState(UIItem::TIMEOUT_MOVEIN);
					UIItem::LuaOnMouseIn((*i));
				}
			}
		}

	}

	if(m_MouseMoveFlag)
	{
		UpdateMouseMove();
		//m_MouseMoveFlag = false;
	}
}

void UIManager::RemoveFromInOutEvent(UIItem * pItem)
{
	if (m_MouseInOutEventSet.find(pItem) != m_MouseInOutEventSet.end())
		m_MouseInOutEventSet.erase(m_MouseInOutEventSet.find(pItem));
}
void UIManager::InsertToInOutEvent(UIItem * pItem)
{
	if (m_MouseInOutEventSet.find(pItem) == m_MouseInOutEventSet.end())
		m_MouseInOutEventSet.insert(pItem);
}
UInt32 UIManager::CheckTexture(int pX, int pY)
{
	D3DLOCKED_RECT LockedRect;
	UInt32 ret; 
	BYTE	*P1;

	D3DSURFACE_DESC  pDesc;
	m_OutputTexture->GetLevelDesc(0, &pDesc);
	
	if (pX > (int)pDesc.Width ||
		pY > (int)pDesc.Height)
		return 0;

	m_OutputTexture->LockRect(0, &LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE);

	P1 = (BYTE *)LockedRect.pBits;
	P1 += pY * LockedRect.Pitch;

	DWORD *P2 = (DWORD *)P1;
	ret = P2[pX];

	m_OutputTexture->UnlockRect(0);

	return ret;
}


LRESULT UIManager::OnMouseMove(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	if(m_CursorX == (Int16)(LOWORD(lParam)) && m_CursorY == (Int16)(HIWORD(lParam)))
		return 0;

	m_CursorPreX = m_CursorX;
	m_CursorPreY = m_CursorY;

	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));

	/*
	while (gGlobal.WinCursorCount > -1)
		gGlobal.WinCursorCount = ShowCursor(FALSE);
		*/
	/*
	if (gGlobal.pUIManager == this)
	{
		
		gGlobal.MarkMouseDirtyRect();
	}
	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));

	m_MouseMoveFlag = true;
	*/

	return 0;
}


void UIManager::CloseTips()
{
	if(m_TipsNow)
	{
		UIPanel *TipsPanel = FindUIPanel("TipsPanel");

		if(!TipsPanel)
			TipsPanel = FindUIPanelEx("TipsPanel");
	
		if(TipsPanel)
		{
			RECT realRectNew;

			TipsPanel->SetEnable(false);
			TipsPanel->SetRealRect(realRectNew);
			
			m_TipsNow = false;
		}
	}
}


LRESULT UIManager::UpdateMouseMove()
{
	if(m_DragNow)
	{

		m_MoveInTimerItem = NULL;
		CloseTips();
		
		if (m_TempDragPanel)
		{
			Int		TempX = m_CursorX - m_TempDragPanel->GetDragX();
			Int		TempY = m_CursorY - m_TempDragPanel->GetDragY();
			RECT	realRectOld;
			RECT	realRectNew;

			Int TempDragWidth = 800 - m_TempDragPanel->GetDragWidth();
			if(TempX > TempDragWidth)
			{
				TempX = TempDragWidth;
				m_CursorX = TempX + m_TempDragPanel->GetDragX();
			}
			else
			if(TempX < 0 && m_TempDragPanel->GetWidth() + TempX < m_TempDragPanel->GetDragWidth())
			{
				TempX = m_TempDragPanel->GetDragWidth() - m_TempDragPanel->GetWidth();
				m_CursorX = TempX + m_TempDragPanel->GetDragX();
			}

			Int TempDragHeight = 600 - m_TempDragPanel->GetDragHeight();
			if(TempY > TempDragHeight)
			{
				TempY = TempDragHeight;
				m_CursorY = TempY + m_TempDragPanel->GetDragY();
			}
			else
			if(TempY < 0 && m_TempDragPanel->GetHeight() + TempY < m_TempDragPanel->GetDragHeight())
			{
				TempY = m_TempDragPanel->GetDragHeight() - m_TempDragPanel->GetHeight();
				m_CursorY = TempY + m_TempDragPanel->GetDragY();
			}

			m_TempDragPanel->SetRealRect(realRectOld);
			m_DirRectMan.AddDirtyRectToMan(realRectOld);
			m_TempDragPanel->SetPos(TempX, TempY);
			m_TempDragPanel->SetRealRect(realRectNew);
			m_DirRectMan.AddDirtyRectToMan(realRectNew);

			if(m_DragItem->IsPanel())
			{
				UIPanel *FollowPanel = m_TempDragPanel->GetFallowPanel();

				if(FollowPanel)
				{
					Int32	TempX = m_CursorX - FollowPanel->GetDragX();
					Int32	TempY = m_CursorY - FollowPanel->GetDragY();
					FollowPanel->SetRealRect(realRectOld);
					m_DirRectMan.AddDirtyRectToMan(realRectOld);
					FollowPanel->SetPos(TempX, TempY);
					FollowPanel->SetRealRect(realRectNew);
					m_DirRectMan.AddDirtyRectToMan(realRectNew);
				}
			}

		}

		return 0;
	}


	if(m_Focus && m_Focus->IsPickUp())
	{
		m_MoveInTimerItem = NULL;
		CloseTips();

		m_Focus->OnMouseMove(m_CursorX, m_CursorY);
		m_Focus->RenderParent();
		return  0;
	}
	

	bool	DetectImageFlag = false;


	if(m_MouseOverItem)
		DetectImageFlag = !m_MouseOverItem->IsDelectImage();

	m_MouseOnUIFlag = false;

	UIPanel	*CurPanel = NULL;
	

	

	{
		if(m_MonoPanel)
		{
			if(m_MonoPanel->Detect(m_CursorX, m_CursorY))
			{
				CurPanel = (UIPanel *)m_MonoPanel;
				m_MouseOnUIFlag = true;
			}
		}
		else
		{
			for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
			{
				if(!(*i)->IsFreeze() && (*i)->Detect(m_CursorX, m_CursorY))
				{
					CurPanel = *i;
					m_MouseOnUIFlag = true;
					break;
				}
			}
		}

		if(CurPanel)
		{
			UIItem::LuaOnMouseMove(CurPanel, m_CursorX, m_CursorY, 0);

			if(m_MouseOverItem)
			{
				if(m_MouseOverItem != CurPanel->GetMouseOver())
				{
					m_MoveInTimerItem = NULL;
					CloseTips();

					m_MouseOverItem->SetNotMouseOver();
					UIItem::LuaOnMouseMove(m_MouseOverItem, m_CursorX, m_CursorY, 2);
					
					RECT	Rect;
					m_MouseOverItem->SetRealOffsetRect(Rect);

					if (m_MouseOverItem->IsChange())
					{
						if (m_OldCurPanel)
						{
							m_OldCurPanel->Render(NULL, Rect);
							m_OldCurPanel->AddDirtyRectToMan(NULL);
						}

						m_MouseOverItem->SetChange(false);
					}

					m_MouseOverItem = NULL;
				}
				else
				{

					UIItem::LuaOnMouseMove(m_MouseOverItem, m_CursorX, m_CursorY, 0);
				}
			}

			if(CurPanel->GetMouseOver())
			{
				if(CurPanel->GetMouseOver() != CurPanel)
				{
					
					m_MouseOverItem = CurPanel->GetMouseOver();
					if(m_MouseOverItem != m_MoveInTimerItem)
					{
						//m_MoveInTimerItem = NULL;
						//CloseTips();

						if(m_MouseOverItem->GetTypeID() == typeid(UITips *) || m_MouseOverItem->IsDefaultTips())
							SetMoveInTimerItem(m_MouseOverItem, m_MouseOverItem->GetTimeOutCount());
					}

					UIItem::LuaOnMouseMove(m_MouseOverItem, m_CursorX, m_CursorY, 1);

					m_MouseOverItem->OnMouseMove(m_CursorX, m_CursorY);

					m_OldCurPanel = CurPanel;

					RECT	Rect;
					m_MouseOverItem->SetRealOffsetRect(Rect);

					if (m_MouseOverItem->IsChange())
					{
						//CurPanel->Render(NULL, Rect);
						//CurPanel->AddDirtyRectToMan(NULL);
						m_MouseOverItem->RenderParent();

						m_MouseOverItem->SetChange(false);
					}

					if(m_MouseOverItem->ContinueRun())
						m_MouseOnUIFlag = false;

					return 0;
				}
			}

			
		}
		
		m_MoveInTimerItem = NULL;
		CloseTips();

		{
			if(m_MouseOverItem)
			{
				m_MouseOverItem->SetNotMouseOver();
				UIItem::LuaOnMouseMove(m_MouseOverItem, m_CursorX, m_CursorY, 2);

				RECT	Rect;
				m_MouseOverItem->SetRealOffsetRect(Rect);
				
				if (m_MouseOverItem->IsChange() && m_OldCurPanel)
				{
					m_OldCurPanel->Render(NULL, Rect);
					m_OldCurPanel->AddDirtyRectToMan(NULL);
					m_MouseOverItem->SetChange(false);
				}
				
				m_OldCurPanel = NULL;
				m_MouseOverItem = NULL;

				m_MoveInTimerItem = NULL;
				CloseTips();
			}
		}
	}

	return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
}



void UIManager::SetDragItem(UIItem *Item)
{
	m_DragItem = Item;
	m_DragItemParent = Item->GetParent();
	m_Focus = NULL;
	m_DragNow = true;

	m_StartDragX = m_CursorX;
	m_StartDragY = m_CursorY;

	m_DragItem->SetOldDragX(m_DragItem->GetX());
	m_DragItem->SetOldDragY(m_DragItem->GetY());

	m_DragItem->SetDragX(m_CursorX - Item->GetX());
	m_DragItem->SetDragY(m_CursorY - Item->GetY());

	if(m_DragItem->IsPanel())
	{
		SetDragPanel2Top();
	}
	

	SetDragPanel();

	UIItem::LuaOnStartDrag(m_DragItem);
	AddDirtyUIItem(m_DragItem);
	
	
}


LRESULT UIManager::OnLButtonDown_GetSpot(UIItem *Item)
{
	m_Focus = Item->GetHotSpot();

	if(m_Focus->GetCanDrag() == 1 || m_Focus->GetCanDrag() == 12)
	{
		if(m_Focus->DetectDragRect(m_CursorX, m_CursorY))
		{
			SetDragItem(m_Focus);

			return 0;
		}
	}

	SetFocusTrue();
	m_WaitItem = m_Focus;
	m_WaitItem->SetLastWndMsg(WM_LBUTTONDOWN);
	UIItem::LuaOnLButtonDown(m_Focus, m_CursorX, m_CursorY);
	
	if (m_Focus && m_Focus->IsChange())
	{
		AddDirtyUIItem(m_Focus);
		m_Focus->SetChange(false);
	}
	m_WaitItem = m_Focus;

	if(m_Focus)
	{
		if(m_Focus->ContinueRun())
			return 1;
		else
			return 0;
	}
	else
		return 0;
}



LRESULT UIManager::OnLButtonDBLCLK_GetSpot(UIItem *Item)
{
	m_Focus = Item->GetHotSpot();

	SetFocusTrue();
	m_WaitItem = m_Focus;
	m_WaitItem->SetLastWndMsg(WM_LBUTTONDOWN);
	UIItem::LuaOnLButtonDBLCLK(m_Focus, m_CursorX, m_CursorY);
	
	if (m_Focus && m_Focus->IsChange())
	{
		AddDirtyUIItem(m_Focus);
		m_Focus->SetChange(false);
	}
	m_WaitItem = m_Focus;

	if(m_Focus)
	{
		if(m_Focus->ContinueRun())
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void UIManager::ResetDragItem()
{
	if (m_DragItem == NULL)
		return;

	RECT realRect;
	m_DragItem->SetRealRect(realRect);
	m_DirRectMan.AddDirtyRectToMan(realRect);
	m_DragItem->SetEnable(true);
	m_DragItem->SetPos(m_DragItem->GetOldDragX(), m_DragItem->GetOldDragY());
	m_DragItem->SetDetectRect();
}


void UIManager::AddPanelPos(UIPanel *pPanel)
{
	if(pPanel&&!pPanel->GetName()->IsEmpty())
	{
		PCSTRINGA	PanelName = pPanel->GetName()->c_str();

		m_UIPanelPosRec[PanelName] = std::pair<Int32, Int32>(pPanel->GetX(), pPanel->GetY());
		CloseTips();
	}
}

void UIManager::EndDrag(Int32 Code)
{
	m_DragItem->SetPos(m_CursorX - m_DragItem->GetDragX(), m_CursorY - m_DragItem->GetDragY());


	if(!m_DragItem->IsPanel())
	{
		RECT realRect;

		m_DragItem->SetRealRect(realRect);
		m_DirRectMan.AddDirtyRectToMan(realRect);
		m_TempDragPanel->LeaveUIItem(m_DragItem);
		m_DragItem->SetParent(m_DragItemParent);
		SafeDeleteObject(m_TempDragPanel);
	}
	else if(m_TempDragPanel)
	{
		AddPanelPos(m_TempDragPanel);
	}
	
	for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
	{
		if(!(*i)->IsFreeze() && (*i)->Detect(m_CursorX, m_CursorY) && *i != m_DragItem)
		{
			m_DropPanel = *i;
			m_Focus = (*i)->GetHotSpot();
			break;
		}
	}

	m_DragItem->SetDetectRect();
	UIItem::LuaOnEndDrag(m_DragItem, Code);

	m_TempDragPanel = NULL;
	StopDrag();

	AddDirtyUIItem(m_DragItem);
	
}

void UIManager::SetAllPanelDisable()
{
	StringA exceptPanel = "Char";

	CloseTips();
	for (UInt i = 0; i < m_UIPanelList.size(); i++)
	{
		if (m_UIPanelList[i]->IsEnable() && 
			*m_UIPanelList[i]->GetName() != exceptPanel)
		{
			m_UIPanelList[i]->SetEnable(false);
			m_UIPanelList[i]->SetIsOpened(true);
		}
	}
}

void UIManager::RestoreAllPanelEnable()
{
	CloseTips();
	for (UInt i = 0; i < m_UIPanelList.size(); i++)
	{
		if (m_UIPanelList[i]->GetIsOpened() )
		{
			m_UIPanelList[i]->SetEnable(true);
			m_UIPanelList[i]->SetIsOpened(false);
		}
	}
}
LRESULT UIManager::OnLButtonDown(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
	Boolean	Flag = false;
	
	m_LButtonDownTime = GetTickCount();

	m_MoveInTimerItem = NULL;
	CloseTips();

	if(m_LeaveTimerItem)
	{
		if(!m_LeaveTimerItem->Detect(m_CursorX, m_CursorY))
		{
			UIItem::LuaOnTimeOut(m_LeaveTimerItem, UIItem::TIMEOUT_LEAVE);
			m_LeaveTimerItem = NULL;
			Flag = true;
		}
	}

	if(m_DragNow && (m_DragItem->GetCanDrag() == 2 || m_DragItem->GetCanDrag() == 12))
	{
		EndDrag(m_DragItem->GetCanDrag());
		return 0;
	}


	if(m_Focus)
	{
		if(m_Focus != m_KeyInItem)
		{
			m_Focus->SetFocus(false);
			if(m_KeyInItem)
			{
				m_Focus = m_KeyInItem;
				m_KeyInItem->SetFocus(true);
			}
			else
			{
				m_Focus = NULL;
			}
		}
		
		Flag = true;
	}

	if(m_MonoPanel)
	{
		if(m_MonoPanel->OnLButtonDown(m_CursorX, m_CursorY))
		{
			return OnLButtonDown_GetSpot(m_MonoPanel);
		}

		return 0;
	}

	
	m_LClickPanel = NULL;
	for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
	{
		if(!(*i)->IsFreeze() && (*i)->OnLButtonDown(m_CursorX, m_CursorY))
		{
			m_LClickPanel = *i;
			return OnLButtonDown_GetSpot(*i);
		}
	}

	if(Flag)
		AddDirtyUIItem(m_Focus);
		

	return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
}


LRESULT UIManager::OnLButtonUp(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
	Boolean	Flag = false;

	UIPanel	*CurPanel = m_LClickPanel;
	m_LClickPanel = NULL;

	if(m_DragNow)
	{
		if(m_DragItem->GetCanDrag() == 1)
		{
			EndDrag(1);
			return 0;
		}
		else
		if(m_DragItem->GetCanDrag() == 12)
		{
			if(m_CursorX != m_StartDragX || m_CursorY != m_StartDragY)
			{
				EndDrag(12);
				return 0;
			}
			else
			if(GetTickCount() - m_LButtonDownTime > 2000)
			{
				if(UIItem::LuaOnLButtonUp(m_DragItem, m_CursorX, m_CursorY))
				{
					ResetDragItem();
					StopDrag();
					AddDirtyUIItem(m_DragItem);
					
				}
				return 0;
			}
		}
	}


	if(CurPanel)
	{
		if(!CurPanel->GetLuaPerLButtonUp().IsEmpty())
		{
			CallLuaFunc(m_LuaState, CurPanel->GetLuaPerLButtonUp().c_str(), String(), m_CursorX, m_CursorY);
			return 0;
		}
	}


	if(m_Focus)
	{
		if(typeid(UIEditBox *) == m_Focus->GetTypeID())
		{
			m_Focus->UnPickUp();
			UIItem::LuaOnUnPickUp(m_Focus);
			UIItem::LuaOnLButtonUp(m_Focus, m_CursorX, m_CursorY);
			return 0;
		}
		

		if(m_Focus != m_KeyInItem)
		{
			m_Focus->SetFocus(false);
			m_Focus->UnPickUp();
			UIItem::LuaOnUnPickUp(m_Focus);
			
			if(m_KeyInItem)
				m_KeyInItem->SetFocus(true);
		}
		else
		{
			m_Focus->UnPickUp();
			UIItem::LuaOnUnPickUp(m_Focus);
		}

		if(m_Focus->GetCanDrag() == 2 || m_Focus->GetCanDrag() == 12)
		{
			if(m_Focus->DetectDragRect(m_CursorX, m_CursorY))
			{
				SetDragItem(m_Focus);

				return 0;
			}
		}

		if(m_Focus->Detect(m_CursorX, m_CursorY))
		{
			int type = 0;
			if(typeid(UIButton *) == m_Focus->GetTypeID())
				type = 1;
			else if(typeid(UICheckBox *) == m_Focus->GetTypeID())
				type = 2;

			if( m_Focus )
			{
				m_Focus->OnLButtonUp(m_CursorX, m_CursorY);
				if(m_Focus->IsPickUp())
				{
					m_Focus->UnPickUp();
					UIItem::LuaOnUnPickUp(m_Focus);
				}
			}
			else
			{
				switch(type)
				{
				case 1:
					{
					FSound* sfx = FSOUND_Manager::GetInstance().CreateSoundA("Data\\Wav\\SoundI0005_click.wav");
					if (sfx)
					{
						sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
						sfx->Play();	
					}
					break;
					}
				case 2:
					{
					FSound* sfx = FSOUND_Manager::GetInstance().CreateSoundA("Data\\Wav\\SoundI0006_choose.wav");
					if (sfx)
					{
						sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
						sfx->Play();
					}
					break;
					}
				}
			}
			if(m_MouseOverItem)
			{
				m_MouseOverItem->SetNotMouseOver();
				m_MouseOverItem->RenderParent();
				m_MouseOverItem = NULL;
			}

			
			
			POINT Point1;
			GetCursorPos(&Point1);

			UIItem::LuaOnLButtonUp(m_Focus, m_CursorX, m_CursorY);
			
			m_Focus = NULL;
			
			POINT Point2;
			RECT	Rect;

			GetCursorPos(&Point2);
			GetClientRect(hWnd, &Rect);

			if(Point1.x != Point2.x && Point1.y != Point2.y)
			{
				m_CursorX = Point2.x - Rect.left;
				m_CursorY = Point2.y - Rect.top;
				
				lParam = (m_CursorX << 16) | m_CursorY;
			}
			//PostMessage(hWnd, WM_MOUSEMOVE, wParam, lParam);
			
		}
		else
		{
			m_Focus->UnPickUp();
			UIItem::LuaOnUnPickUp(m_Focus);
		}

		Flag = true;
	}

	if(Flag)
		AddDirtyUIItem(m_Focus);
		

	return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
}


LRESULT UIManager::OnRButtonDown(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
	Boolean	Flag = false;

	m_MoveInTimerItem = NULL;
	CloseTips();

	if(m_LeaveTimerItem)
	{
		if(!m_LeaveTimerItem->Detect(m_CursorX, m_CursorY))
		{
			UIItem::LuaOnTimeOut(m_LeaveTimerItem, UIItem::TIMEOUT_LEAVE);
			m_LeaveTimerItem = NULL;
			Flag = true;
		}
	}

	if(m_DragNow)
	{
		if(m_DragItem->GetCanDrag())
		{
			if(!UIItem::LuaOnRButtonDown(m_DragItem, m_CursorX, m_CursorY))
			{
				ResetDragItem();
				StopDrag();
				AddDirtyUIItem(m_DragItem);
				
			}
		}
		return 0;
	}
	
	if(m_Focus)
	{
		m_Focus->SetFocus(false);
		m_Focus = NULL;
		if(m_KeyInItem)
			m_KeyInItem->SetFocus(true);
		
		Flag = true;
	}

	if(m_MonoPanel)
	{
		if(m_MonoPanel->OnLButtonDown(m_CursorX, m_CursorY))
		{
			m_Focus = m_MonoPanel->GetHotSpot();
			SetFocusTrue();
			m_WaitItem = m_Focus;
			m_WaitItem->SetLastWndMsg(WM_LBUTTONDOWN);
			UIItem::LuaOnRButtonDown(m_Focus, m_CursorX, m_CursorY);
			AddDirtyUIItem(m_Focus);
			
			m_WaitItem = m_Focus;

			return 1;
		}

		return 0;
	}

	

	m_RClickPanel = NULL;
	for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
	{
		if(!(*i)->IsFreeze() && (*i)->OnRButtonDown(m_CursorX, m_CursorY))
		{
			m_RClickPanel = *i;
			m_Focus = (*i)->GetHotSpot();
			SetFocusTrue();
			m_WaitItem = m_Focus;
			m_WaitItem->SetLastWndMsg(WM_LBUTTONDOWN);
			UIItem::LuaOnRButtonDown(m_Focus, m_CursorX, m_CursorY);
			AddDirtyUIItem(m_Focus);
			
			m_WaitItem = m_Focus;

			if(m_Focus)
			{
				if(m_Focus->ContinueRun())
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}
	}

	if(Flag)
		AddDirtyUIItem(m_Focus);
		

	return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
}


LRESULT UIManager::OnRButtonUp(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
	Boolean	Flag = false;

	UIPanel	*CurPanel = m_RClickPanel;
	m_RClickPanel = NULL;

	m_MoveInTimerItem = NULL;
	CloseTips();

	if(CurPanel)
	{
		if(!CurPanel->GetLuaPerRButtonUp().IsEmpty())
		{
			CallLuaFunc(m_LuaState, CurPanel->GetLuaPerRButtonUp().c_str(), String(), m_CursorX, m_CursorY);
			return 0;
		}
	}

	if(m_Focus)
	{
		if(typeid(UIEditBox *) == m_Focus->GetTypeID())
		{
			UIItem::LuaOnRButtonUp(m_Focus, m_CursorX, m_CursorY);
			return 0;
		}

		
		m_Focus->SetFocus(false);
		m_Focus->UnPickUp();
		UIItem::LuaOnUnPickUp(m_Focus);
		if(m_KeyInItem)
			m_KeyInItem->SetFocus(true);

		if(m_Focus->Detect(m_CursorX, m_CursorY))
		{
			if(m_MouseOverItem)
			{
				m_MouseOverItem->SetNotMouseOver();
				m_MouseOverItem->RenderParent();
				m_MouseOverItem = NULL;
			}

			POINT Point1;
			GetCursorPos(&Point1);

			UIItem::LuaOnRButtonUp(m_Focus, m_CursorX, m_CursorY);
			
			m_Focus = NULL;

			POINT Point2;
			RECT	Rect;

			GetCursorPos(&Point2);
			GetClientRect(hWnd, &Rect);

			if(Point1.x != Point2.x && Point1.y != Point2.y)
			{
				m_CursorX = Point2.x - Rect.left;
				m_CursorY = Point2.y - Rect.top;
				
				lParam = (m_CursorX << 16) | m_CursorY;
			}
			//PostMessage(hWnd, WM_MOUSEMOVE, wParam, lParam);

			
		}

		Flag = true;
	}
	
	if(Flag)
		AddDirtyUIItem(m_Focus);
		

	return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
}



LRESULT UIManager::OnLButtonDBLCLK(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
	Boolean	Flag = false;
	
	m_LButtonDownTime = GetTickCount();

	if(m_LeaveTimerItem)
	{
		if(!m_LeaveTimerItem->Detect(m_CursorX, m_CursorY))
		{
			UIItem::LuaOnTimeOut(m_LeaveTimerItem, UIItem::TIMEOUT_LEAVE);
			m_LeaveTimerItem = NULL;
			Flag = true;
		}
	}

	if(m_Focus)
	{
		m_Focus->SetFocus(false);
		m_Focus = NULL;
		if(m_KeyInItem)
			m_KeyInItem->SetFocus(true);
		
		Flag = true;
	}

	if(m_MonoPanel)
	{
		if(m_MonoPanel->OnLButtonDBLCLK(m_CursorX, m_CursorY))
		{
			return OnLButtonDBLCLK_GetSpot(m_MonoPanel);
		}

		return 0;
	}

	{
		for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
		{
			if(!(*i)->IsFreeze() && (*i)->OnLButtonDBLCLK(m_CursorX, m_CursorY))
			{
				return OnLButtonDBLCLK_GetSpot(*i);
			}
		}
	}

	if(Flag)
		AddDirtyUIItem(m_Focus);
		

	return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
}


LRESULT UIManager::OnRButtonDBLCLK(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
	Boolean	Flag = false;

	m_MoveInTimerItem = NULL;
	CloseTips();

	if(m_LeaveTimerItem)
	{
		if(!m_LeaveTimerItem->Detect(m_CursorX, m_CursorY))
		{
			UIItem::LuaOnTimeOut(m_LeaveTimerItem, UIItem::TIMEOUT_LEAVE);
			m_LeaveTimerItem = NULL;
			Flag = true;
		}
	}

	if(m_DragNow)
	{
		return 0;
	}
	
	if(m_Focus)
	{
		m_Focus->SetFocus(false);
		m_Focus = NULL;
		if(m_KeyInItem)
			m_KeyInItem->SetFocus(true);
		
		Flag = true;
	}

	if(m_MonoPanel)
	{
		if(m_MonoPanel->OnRButtonDBLCLK(m_CursorX, m_CursorY))
		{
			m_Focus = m_MonoPanel->GetHotSpot();
			SetFocusTrue();
			m_WaitItem = m_Focus;
			m_WaitItem->SetLastWndMsg(WM_LBUTTONDOWN);
			UIItem::LuaOnRButtonDBLCLK(m_Focus, m_CursorX, m_CursorY);
			AddDirtyUIItem(m_Focus);
			
			m_WaitItem = m_Focus;

			return 1;
		}

		return 0;
	}

	
	for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
	{
		if(!(*i)->IsFreeze() && (*i)->OnRButtonDBLCLK(m_CursorX, m_CursorY))
		{
			m_Focus = (*i)->GetHotSpot();
			SetFocusTrue();
			m_WaitItem = m_Focus;
			m_WaitItem->SetLastWndMsg(WM_LBUTTONDOWN);
			UIItem::LuaOnRButtonDBLCLK(m_Focus, m_CursorX, m_CursorY);
			AddDirtyUIItem(m_Focus);
			
			m_WaitItem = m_Focus;

			if(m_Focus)
			{
				if(m_Focus->ContinueRun())
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}
	}

	if(Flag)
		AddDirtyUIItem(m_Focus);
		

	return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
}


void UIManager::MouseLeave()
{
	ClearDrag();
	if(m_MouseOverItem)
	{
		m_MouseOverItem->SetNotMouseOver();
		m_MouseOverItem->RenderParent();
		m_MouseOverItem = NULL;
	}
	if(m_Focus)
	{
		if(typeid(UIEditBox *) != m_Focus->GetTypeID())
		{
			if(m_Focus->IsPickUp())
			{
				m_Focus->UnPickUp();
				UIItem::LuaOnUnPickUp(m_Focus);
			}
			m_Focus = NULL;
		}
	}
}


LRESULT UIManager::MsgProc(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam )
{
	g_UIhWnd = hWnd;

	m_WaitItem = NULL;

	static UInt32 s_LClickCount = 0;
	static UInt32 s_RClickCount = 0;

	
	if(m_LuaFuncName.size())
	{
		Int32 CursorX = (Int16)(LOWORD(lParam)); Int32	CursorY = (Int16)(HIWORD(lParam));
		//CallLuaFunc(m_LuaState, m_LuaFuncName.c_str(), uMsg, CursorX, CursorY, (DWORD)(wParam), (DWORD)(lParam));

		lua_getglobal(m_LuaState, m_LuaFuncName.c_str());  
		LuaPush(m_LuaState, uMsg);
		LuaPush(m_LuaState, CursorX);
		LuaPush(m_LuaState, CursorY);
		LuaPush(m_LuaState, (DWORD)(wParam));
		LuaPush(m_LuaState, (DWORD)(lParam));

		if (lua_pcall(m_LuaState, 5, 1, 0) != 0)
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, String(lua_tostring(m_LuaState, -1)).c_str());
		}

		Int32	z = 0;

		if (!lua_isnumber(m_LuaState, -1)) 
		{
			TRACE_ERRORDTL(GLOBAL_LOGGER, _F("function `f' must return a number")); 
		}
		z = (Int32)lua_tonumber(m_LuaState, -1); 
		lua_pop(m_LuaState, 1);  

		if(z > 0)
		{
			return 1;
		}
		else
		if(z < 0)
			return 0;
	}
	

	if(m_UIPanelListPtr->size())
	{
		
		switch(uMsg)
		{
		case WM_MOUSEWHEEL:
			{
				//m_CursorX = (Int16)(LOWORD(lParam)); m_CursorY = (Int16)(HIWORD(lParam));
				Int32	ZDelta= (short) HIWORD(wParam); ;

				if(m_MonoPanel)
				{
					if(m_MonoPanel->OnMouseWheel(m_CursorX, m_CursorY, ZDelta))
					{
						RECT	RRect;

						m_MonoPanel->SetRealRect(RRect);
						//RenderRect(RRect);
						
						m_MonoPanel->Render(NULL, RRect);
						m_MonoPanel->AddDirtyRectToMan(NULL);
					}
	
					return 0;
				}
				else
				{
					for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
					{
						if(!(*i)->IsFreeze() && (*i)->OnMouseWheel(m_CursorX, m_CursorY, ZDelta))
						{
							///RECT	RRect;

							//m_UIPanelList[i]->SetRealRect(RRect);
							(*i)->Render(NULL);
							(*i)->AddDirtyRectToMan(NULL);
							//RenderRect(RRect);

							return 0;
						}
					}
				}
			}
			break;
	    
		case WM_MOUSEMOVE:
			{
				return (OnMouseMove(hWnd, uMsg, wParam, lParam));
			}
			break;

		case WM_LBUTTONDOWN:
			{
				++s_LClickCount;
	
				return (OnLButtonDown(hWnd, uMsg, wParam, lParam));
			}
			
			break;	

		case WM_LBUTTONUP:
			{
				if(s_LClickCount)
				{
					--s_LClickCount;
					return (OnLButtonUp(hWnd, uMsg, wParam, lParam));
				}
			}
			break;

		case WM_RBUTTONDOWN:
			{
				++s_RClickCount;

				return (OnRButtonDown(hWnd, uMsg, wParam, lParam));
			}
			break;

		case WM_RBUTTONUP:
			{
				if(s_RClickCount)
				{
					--s_RClickCount;
					return (OnRButtonUp(hWnd, uMsg, wParam, lParam));
				}
			}
			break;	

		case WM_LBUTTONDBLCLK:
			{
				return (OnLButtonDBLCLK(hWnd, uMsg, wParam, lParam));
			}
			break;

		case WM_RBUTTONDBLCLK:
			{
				return (OnRButtonDBLCLK(hWnd, uMsg, wParam, lParam));
			}
			break;

		case WM_CHAR:
			{
				if(m_MonoPanel && wParam == VK_ESCAPE)
				{
					UIItem::LuaOnChar(m_MonoPanel, VK_ESCAPE);
					return 0;
				}

				if(wParam == VK_RETURN && !m_DefaultEnterFunc.empty())
				{
					QGLApplication::GetInstance().SendMessage();
					CallLuaFunc(m_LuaState, m_DefaultEnterFunc.c_str(), String());
					return (0);
				}

				if(m_KeyInItem)
				{
					m_KeyInItem->OnChar(wParam);
					if(m_KeyInItem->GetLastKey())
					{
						m_WaitItem = m_KeyInItem;
						m_WaitItem->SetLastWndMsg(WM_CHAR);
					}
					UIItem::LuaOnChar(m_KeyInItem, static_cast<Int32>(wParam));
					m_KeyInItem->RenderParent();

					return (0);
				}
				
				if(m_Focus)
				{
					m_Focus->OnChar(wParam);
					if(m_Focus->GetLastKey())
					{
						m_WaitItem = m_Focus;
						m_WaitItem->SetLastWndMsg(WM_CHAR);
					}
					UIItem::LuaOnChar(m_Focus, static_cast<Int32>(wParam));
					m_Focus->RenderParent();

					return (0);
				}

				/*
				if(m_EditBox)
				{
					m_EditBox->OnChar(wParam);
				}
				*/

				return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
			}
			break;

		case WM_KEYDOWN:
			{
				CloseTips();
				/*
				if(GetAsyncKeyState(VK_RCONTROL ))
				{
					if(wParam == 'Z')
					{
						if(!m_FreezeFlag)
							FreezeAll();
						else
							UnfreezeAll();

						return 0;
					}
					break;

					return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
				}
				*/
				if(m_KeyInItem)
				{
					//check whether player send message
					if (wParam == VK_RETURN)
					{
						//send the message
						StringA mainEdit = "EditLine";
						StringA subEdit = "ChatEditLine";
						if (*m_KeyInItem->GetName() == mainEdit)
						{
							QGLApplication::GetInstance().SendMessage();
						}
						else 
						if (*m_KeyInItem->GetName() == subEdit)
							gGlobal.g_pMenuFriendProcess->SendMessageToFriend();
					}
					
					if(m_KeyInItem->OnKeyDown(wParam))
					{
						UIItem::LuaOnKeyDown(m_KeyInItem, static_cast<Int32>(wParam));
						m_KeyInItem->RenderParent();

						return (0);
					}

					if(QGLApplication::GetInstance().m_mapState->GetStateID() == ST_APP_LOGIN && wParam == VK_TAB)
					{
						ChangeFocus(GetAsyncKeyState(VK_SHIFT) ?
							m_KeyInItem->GetPrev() : m_KeyInItem->GetNext());
						RenderAll();

						return 0;
					}
				}

				if(m_Focus)
				{
					if(wParam == VK_TAB)
					{
						ChangeFocus(GetAsyncKeyState(VK_SHIFT) ?
							m_Focus->GetPrev() : m_Focus->GetNext());
						RenderAll();
					}
					else
					{
						m_Focus->OnKeyDown(wParam);
						UIItem::LuaOnKeyDown(m_Focus, static_cast<Int32>(wParam));
						m_Focus->RenderParent();
					}

					return (0);
				}

				return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
			}
			break;


		case WM_KEYUP:
			{
				if(m_KeyInItem)
				{
					m_KeyInItem->OnKeyUp(wParam);
					UIItem::LuaOnKeyUp(m_KeyInItem, static_cast<Int32>(wParam));
					m_KeyInItem->RenderParent();
							
					return (0);
				}

				if(m_Focus)
				{
					m_Focus->OnKeyUp(wParam);
					UIItem::LuaOnKeyUp(m_Focus, static_cast<Int32>(wParam));
					m_Focus->RenderParent();

					return (0);
				}

				return (m_MonoPanel || m_FreezeFlag ? 0 : 1);
			}
			break;

		}
	}

	return 1;
}



bool  UIManager::KeyOnEsc()
{
	for(StlVector<UIPanel *>::reverse_iterator i = m_UIPanelListPtr->rbegin(); i != m_UIPanelListPtr->rend(); ++i)
	{
		PCSTRINGA  Str = (*i)->GetName()->c_str();
		if(!(*i)->IsFreeze() && (*i)->IsEnable() && (*i)->GetCanRemove())
		{
			UIItem::LuaOnChar(*i, VK_ESCAPE);
			return true;
		}
	}

	return false;
}

void UIManager::AddDirtyUIItem(UIItem * pItem)
{
	if (pItem == NULL)
		return;

	pItem->RenderParent();
	/*if (pItem->IsPanel())
	{
		pItem->Render(NULL);
		pItem->AddDirtyRectToMan(NULL);
		pItem->m_
		/*if (gGlobal.pUIManager == this)
		{
			RECT rect;
			pItem->SetRealRect(rect);
			GetDirtyRectMan()->AddDirtyRectToMan(rect);
		}
	}
	else
	{
		//pItem->RenderParent();
		UIPanel * pPanel = (UIPanel *)pItem->GetParent();
		if (pPanel)
		{
			RECT RRect;
			pItem->SetRealOffsetRect(RRect);
			pPanel->AddPanelDirtyRect(RRect);
		}
		if (gGlobal.pUIManager == this && pItem->GetParent())
		{
			RECT rect;
			pItem->GetParent()->SetRealRect(rect);
			GetDirtyRectMan()->AddDirtyRectToMan(rect);
		}
	}*/
}


void UIManager::SetDragPanel()
{
	if(m_DragItem)
	{
		if(m_DragItem->IsPanel())
		{
			m_TempDragPanel = (UIPanel *)m_DragItem;

			UIPanel *FollowPanel = m_TempDragPanel->GetFallowPanel();

			if(FollowPanel)
			{
				FollowPanel->SetDragX(m_CursorX - FollowPanel->GetX());
				FollowPanel->SetDragY(m_CursorY - FollowPanel->GetY());
			}
		}
		else
		{
			m_TempDragPanel = SafeCreateObject(UIPanel);
			
			m_TempDragPanel->SetTextureWH(m_DragItem->GetWidth(), m_DragItem->GetHeight());
			m_TempDragPanel->SetTexture(m_Render->GetDevice());
			m_TempDragPanel->SetWH(m_DragItem->GetWidth(), m_DragItem->GetHeight());
			m_TempDragPanel->AddUIItem(m_DragItem);
			m_TempDragPanel->SetPos(m_DragItem->GetX(), m_DragItem->GetY());
			m_TempDragPanel->SetDragX(m_DragItem->GetDragX());
			m_TempDragPanel->SetDragY(m_DragItem->GetDragY());

			m_TempDragPanel->Render(NULL);
			
			RECT realRect;
			m_DragItem->SetRealRect(realRect);
			m_DirRectMan.AddDirtyRectToMan(realRect);

		}
	}
}



void UIManager::RemoveUpdateUIItem(UIItem *pUIItem)
{ 
	StlSet<UIItem *>::iterator i = m_AutoUpdateSet.find(pUIItem);

	if(i != m_AutoUpdateSet.end())
	{
		m_AutoUpdateSet.erase(i);
	}
}


void UIManager::SetFocusTrue()
{
	m_Focus->SetFocus(true);
	if(m_KeyInItem && m_Focus != m_KeyInItem && m_Focus->GetTypeID() == typeid(UIEditBox *))
	{
		m_KeyInItem->SetFocus(false);
		m_KeyInItem->SetChange(true);
	}
}


void UIManager::ChangeFocus(PCSTRINGA pName)
{
	if(m_Focus)
		m_Focus->SetFocus(false);

	UIItem *ItemPtr = GetUIItem(pName);

	if(ItemPtr)
	{
		m_Focus = ItemPtr;
		UIEditBox * pEdit = (UIEditBox *) m_Focus;
		if (pEdit)
		{
			SetKeyInItem(pEdit->GetName()->c_str());
		}
		else
			m_Focus->SetFocus(true);
	}
}


void UIManager::ChangeFocus(UIItem *ItemPtr)
{
	if(ItemPtr)
	{
		if(m_Focus)
			m_Focus->SetFocus(false);
		m_Focus = ItemPtr;
		m_Focus->SetFocus(true);
	}
}


void UIManager::RemovePanel()
{
	StlVector<UIPanel *>::iterator i = m_UIPanelList.begin();

	if((*i)->GetCanRemove())
	{
		DeletePanel(*i);
	}

}


UIPanel *UIManager::GetMonoPanel()
{ 
	if(m_MonoPanel)
		return (typeid(UIPanel *) == m_MonoPanel->GetTypeID() ? (UIPanel *)m_MonoPanel:  NULL); 
	else
		return NULL;
}


UIPanelEx *UIManager::GetMonoPanelEx()
{ 
	if(m_MonoPanel)
		return (typeid(UIPanelEx *) == m_MonoPanel->GetTypeID() ? (UIPanelEx *)m_MonoPanel:  NULL); 
	else
		return NULL;
}


void UIManager::UIItemSetEnable(PCSTRINGA Name, bool Flag)
{
	UIItem *Item = GetUIItem(Name);

	if(Item)
	{
		Item->SetEnable(Flag);
	}
}


void UIManager::SetFollowPanel(UIPanel *PanelA, UIPanel *PanelB)
{
	PanelA->SetFallowPanel(PanelB);
}


void UIManager::SetFollowPanel(PCSTRINGA PanelA, PCSTRINGA PanelB)
{
	UIPanel	*PanelPtrA = FindUIPanel(PanelA);
	if(!PanelPtrA)
		PanelPtrA = FindUIPanelEx(PanelA);

	UIPanel	*PanelPtrB = FindUIPanel(PanelB);
	if(!PanelPtrB)
		PanelPtrB = FindUIPanelEx(PanelB);

	if(PanelPtrA && PanelPtrB)
	{
		PanelPtrA->SetFallowPanel(PanelPtrB);
	}
}


void UIManager::FreezeAll()
{
	m_FreezeFlag = true;
	CloseTips();
	for(std::vector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		(*i)->Freeze();
	}	
}


void UIManager::UnfreezeAll()
{
	m_FreezeFlag = false;
	for(std::vector<UIPanel *>::iterator i = m_UIPanelList.begin(); i != m_UIPanelList.end(); ++i)
	{
		(*i)->Unfreeze();
	}
}

