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
#include "MenuSystemSetting.h"



//////////////////////////////////////


HWND g_hWnd = NULL;
HWND	g_UIhWnd = NULL;
CImage		*g_EditCursor;
const UInt EditCursorRefreshCount = 500;


///////////////font color/////////////
const UInt32 FontColor[] = 
{
	0xFFFFFFFF,
		0xFF00FFFF,
		0xFFFF00FF,
		0xFFFFFF00,
		0xFFFF0000,
		0xFF00FF00,
		0xFF4040FF,

		0xFF38F838,
		0xFF7880F0,
		0xFFB054D0,
		0xFFF89000,
};
///////////////////////////////////////////////////////////////////////////////////////////

UInt32	HexToDWORD(PCSTRINGA HexStr)
{
	
	if(HexStr)
	{
		Int32	Len = (Int32)strlen(HexStr);
		UInt32 D = 0;

		for(Int32 i = 0, j = Len - 1; i < Len; ++i, j--)
		{
			UInt32 T = 0;

			if(HexStr[i] >= '0' && HexStr[i] <= '9')
				T = HexStr[i] - '0';
			else
			if(HexStr[i] >= 'a' && HexStr[i] <= 'f')
				T = 10 + HexStr[i] - 'a';
			else
			if(HexStr[i] >= 'A' && HexStr[i] <= 'F')
				T = 10 + HexStr[i] - 'A';

			D |= T << (j << 2);
		}

		return D;
	}
	else
		return 0;


}


Int32  UTF82StringW(StringW &Str, PCSTRINGA Text)
{
	if(Text)
	{
		Int32 Len = MultiByteToWideChar(CP_UTF8, 0, Text, -1, NULL, 0);
		PSTRINGW TempStr = SafeAllocate(CharW, Len);
		MultiByteToWideChar(CP_UTF8, 0, Text, -1, TempStr, Len);
		Str = TempStr;
		SafeDeallocate(TempStr);

		return Len;
	}
	else
	{
		Str = _T("");
		return 0;
	}
}


void ClearCtrlText(String &MyText)
{
	String TempText;

	bool HideNow = false, JumpNow = false;
	for(Int i = 0; i < (Int)MyText.size(); ++i)
	{
		if(MyText[i] == 15)
		{
			HideNow = true;
			continue;
		}
		else
		if(MyText[i] == 16)
		{
			HideNow = false;
			continue;
		}
		else
		if(MyText[i] == 17)
		{
			JumpNow = true;
			continue;
		}
		else
		if(MyText[i] == 18)
		{
			JumpNow = false;
			continue;
		}
		else
		if(HideNow || MyText[i] <= 26)
		{
			continue;
		}

		TempText.push_back(MyText[i]);
	}

	MyText = TempText;
}


///////////////////////////////////////////////////////////////////////////////////////////


UIItem::UIItem()
{
	m_X = m_Y = m_OffsetX = m_OffsetY =
	m_Width = m_Height = 0;
	SetRect(&m_DetectRect, 0, 0, 0, 0);
	m_IsEnable = true;
	m_PickUp = false;
	m_LuaState = NULL;
	m_CanDrag = 0;
	m_Focus = false;
	m_LastKey = 0;
	m_RenderStyle = 0;
	m_Parent = NULL;
	m_StopPlaySoundOnce = false;
	m_DetectImage = false;
	m_TimeOutCount = 0;
	m_TimeOutPreTime = ::timeGetTime();
	m_TimeOutState = -1;
	m_EnableTimeOutEvent = true;

	m_Color = 0xFFFFFFFF;
	m_ChangeFlag = false;

	m_NotDetected = false;

	//m_LuaFuncTable.reserve(8);

	m_pUIManager = NULL;
}




void UIItem::DeleteMapItem(StlMap<StringA, UIItem *> &UIIDMap)
{
	StlMap<StringA, UIItem *>::iterator i = UIIDMap.find(m_Name);

	if(i != UIIDMap.end())
	{
		UIIDMap.erase(i);
	}

}



bool UIItem::IsDefaultTips()
{
	if(m_LuaFuncTable[UI_FUNC_TIMEOUT].IsEmpty() || m_Tips.IsEmpty())
		return false;
	return (m_LuaFuncTable[UI_FUNC_TIMEOUT] == "DefaultTips");
}




Int32	UIItem::LuaOnMouseWheel(UIItem *Item, Int32 pX, Int32 pY, Int32 pZ)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_MOUSE_WHEEL].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_MOUSE_WHEEL].c_str(), String(), pX, pY, pZ);  
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnMouseMove(UIItem *Item, Int32 pX, Int32 pY, Int32 Code)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_MOUSE_MOVE].size())
	{
		CallLuaFunc(Item->m_LuaState,  Item->m_LuaFuncTable[UI_FUNC_MOUSE_MOVE].c_str(), String(), pX, pY, Code);
		return 1;
	}
	return 0;
}



Int32	UIItem::LuaOnLButtonUp(UIItem *Item, Int32 pX, Int32 pY)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_LBUTTON_UP].size())
	{
		CallLuaFunc(Item->m_LuaState,  Item->m_LuaFuncTable[UI_FUNC_LBUTTON_UP].c_str(), String(), pX, pY);
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnLButtonDown(UIItem *Item, Int32 pX, Int32 pY)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_LBUTTON_DOWN].size())
	{
		CallLuaFunc(Item->m_LuaState,  Item->m_LuaFuncTable[UI_FUNC_LBUTTON_DOWN].c_str(), String(), pX, pY);
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnRButtonUp(UIItem *Item, Int32 pX, Int32 pY)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_RBUTTON_UP].size())
	{
		CallLuaFunc(Item->m_LuaState,  Item->m_LuaFuncTable[UI_FUNC_RBUTTON_UP].c_str(), String(), pX, pY);
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnRButtonDown(UIItem *Item, Int32 pX, Int32 pY)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_RBUTTON_DOWN].size())
	{
		CallLuaFunc(Item->m_LuaState,  Item->m_LuaFuncTable[UI_FUNC_RBUTTON_DOWN].c_str(), String(), pX, pY);
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnKeyDown(UIItem *Item, Int32 KeyCode)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_KEYDOWN].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_KEYDOWN].c_str(), String(), KeyCode);  
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnKeyUp(UIItem *Item, Int32 KeyCode)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_KEYUP].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_KEYUP].c_str(), String(), KeyCode);  
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnChar(UIItem *Item, Int32 KeyCode)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_CHAR].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_CHAR].c_str(), String(), KeyCode);
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnTimeOut(UIItem *Item, TimeOutType Code)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_TIMEOUT].size())
	{
		const char *S1 = Item->m_LuaFuncTable[UI_FUNC_TIMEOUT].c_str();
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_TIMEOUT].c_str(), String(), (Int32)Code);
		
		return 1;
	}
	return 0;
}



Int32	UIItem::LuaOnStartDrag(UIItem *Item)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_STARTDRAG].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_STARTDRAG].c_str(), String());
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnEndDrag(UIItem *Item, Int32 Code)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNG_ENDDRAG].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNG_ENDDRAG].c_str(), String(), Code);
		return 1;
	}
	return 0;
}

Int32	UIItem::LuaOnMouseOut(UIItem *Item)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_MOUSE_OUT].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_MOUSE_OUT].c_str(), String());
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnMouseIn(UIItem *Item)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_MOUSE_IN].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_MOUSE_IN].c_str(), String());
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnUnPickUp(UIItem *Item)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_UNPICKUP].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_UNPICKUP].c_str(), String());
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnLButtonDBLCLK(UIItem *Item, Int32 pX, Int32 pY)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_LBUTTONDBLCLK].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_LBUTTONDBLCLK].c_str(), String(), pX, pY);
		return 1;
	}
	return 0;
}


Int32	UIItem::LuaOnRButtonDBLCLK(UIItem *Item, Int32 pX, Int32 pY)
{
	if(Item->m_LuaState && Item->m_LuaFuncTable[UI_FUNC_RBUTTONDBLCLK].size())
	{
		CallLuaFunc(Item->m_LuaState, Item->m_LuaFuncTable[UI_FUNC_RBUTTONDBLCLK].c_str(), String(), pX, pY);
		return 1;
	}
	return 0;
}

void UIItem::SetRenderStyle(UInt32 Style)
{
	if (m_RenderStyle != Style)
	{
		m_RenderStyle = Style;
		RenderParent();
	}
}
void UIItem::RenderParent()
{
	
	if(m_Parent)
	{
		if(m_Parent->IsPanel())
		{
			RECT RRect;
			SetRealOffsetRect(RRect);
			UIPanel * pPanel = (UIPanel *)m_Parent;
			if (pPanel)
				pPanel->AddPanelDirtyRect(RRect);
			//m_Parent->Render(NULL, RRect);
		}
	}

}

void UIItem::SetOffsetXY(Int32 pX, Int32 pY)
{
	if (m_OffsetX != pX || m_OffsetY != pY)
	{
		RenderParent();

		Int32 TempX = m_X - m_OffsetX;
		Int32 TempY = m_Y - m_OffsetY;

		m_OffsetX = pX;
		m_OffsetY = pY;

		SetPos(TempX + m_OffsetX, TempY + m_OffsetY);

		RenderParent();
	}
}

void UIItem::SetRealRect(RECT &RRect)
{
	RRect.left = m_X;
	RRect.top = m_Y;
	RRect.right = m_X + m_Width;
	RRect.bottom = m_Y + m_Height;
}


void UIItem::SetRealOffsetRect(RECT &RRect)
{
	RRect.left = m_OffsetX;
	RRect.top = m_OffsetY;
	RRect.right = m_OffsetX + m_Width;
	RRect.bottom = m_OffsetY + m_Height;
}

void UIItem::SetEnable(bool Flag)
{
	if (m_IsEnable != Flag)
	{
		RECT realRect;
		SetRealRect(realRect);
		if (m_pUIManager)
			m_pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);
		m_IsEnable = Flag;

		//if (!IsPanel())
		{
			RenderParent();
		}

		ResetTimer();
		m_TimeOutState = -1;
	}
}
void UIItem::RenderParent(RECT	&RRect)
{
	if(m_Parent)
	{
		if(m_Parent->IsPanel())
		{
			m_Parent->Render(NULL, RRect);
		}
	}
}

void UIItem::SetPos(Int32 pX, Int32 pY)
{
	if (m_X != pX || m_Y != pY)
	{
		RECT realRect;
		SetRealRect(realRect);
		if (m_pUIManager)
			m_pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);


		m_DetectRect.right = (m_DetectRect.left = m_X = pX) + m_Width;
		m_DetectRect.bottom = (m_DetectRect.top = m_Y = pY) + m_Height;

		SetRealRect(realRect);
		if (m_pUIManager)
			m_pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);
	}
}


void UIItem::MovePos(Int32 pX, Int32 pY)
{	
	m_X += pX; m_Y += pY;
	m_DetectRect.left += pX;
	m_DetectRect.top += pY;
	m_DetectRect.right += pX;
	m_DetectRect.bottom += pY;
}


void UIItem::SetDetectRect()
{
	m_DetectRect.left = m_X;
	m_DetectRect.top = m_Y;
	m_DetectRect.right = m_X + m_Width;
	m_DetectRect.bottom = m_Y + m_Height;
}



void UIItem::ChangeParent(UIItem *Parent)
{
	if(m_Parent)
	{
		m_Parent->LeaveUIItem(this);
		Parent->AddUIItem(this);
	}
	m_Parent = Parent;
}


void UIItem::Copy(UIItem *Item)
{
	m_X = Item->m_X; 
	m_Y = Item->m_Y; 
	m_OffsetX = Item->m_OffsetX; 
	m_OffsetY = Item->m_OffsetY;
	m_Width = Item->m_Width; 
	m_Height = Item->m_Height;
	m_DetectRect = Item->m_DetectRect;
	m_DragRect = Item->m_DragRect;
	m_IsEnable = Item->m_IsEnable;
	m_PickUp = Item->m_PickUp;
	m_Focus = Item->m_Focus;
	m_CanDrag = Item->m_CanDrag;
	m_Color = Item->m_Color;
	m_LastKey = Item->m_LastKey;
	m_LastWndMsg = Item->m_LastWndMsg;
	m_RenderStyle = Item->m_RenderStyle;
	m_Parent = Item->m_Parent;
	
	m_LuaState = Item->m_LuaState;
	m_Name = Item->m_Name;
	m_Next = Item->m_Next;
	m_Prev = Item->m_Prev;
	
	for(UInt32 i = 0; i < MAX_UI_FUNC; ++i)
		m_LuaFuncTable[i] = Item->m_LuaFuncTable[i];

	m_ChangeFlag = Item->m_ChangeFlag;
	m_NotDetected = Item->m_NotDetected;
	m_StopPlaySoundOnce = Item->m_StopPlaySoundOnce;
	m_DetectImage = Item->m_DetectImage;

	m_TimeOutCount = Item->m_TimeOutCount;
	m_TimeOutPreTime = Item->m_TimeOutPreTime;
	m_TimeOutState = Item->m_TimeOutState;
	m_EnableTimeOutEvent = Item->m_EnableTimeOutEvent;

	m_pUIManager = Item->m_pUIManager;

}

UIItem *UIItem::Dup()
{
	UIItem *TempItem = SafeCreateObject(UIItem);

	Copy(TempItem);
	
	return TempItem;
}



void UIItem::DeleteUpdateSetR(StlSet<UIItem *> &AutoUpdateSet, StlSet<UIItem *> &MouseInSet)
{
	StlSet<UIItem *>::iterator i = AutoUpdateSet.find(this);

	if(i != AutoUpdateSet.end())
	{
		AutoUpdateSet.erase(i);
	}

	i = MouseInSet.find(this);

	if(i != MouseInSet.end())
	{
		MouseInSet.erase(i);
	}
}



///////////////////////////////////////////////////////////////////////


void UIStaticText::SetImage(UInt32 pRow, UInt32 pColumn)
{
	UInt32	TempFlag = m_FontStyle & (0x3 << 1);

	switch(TempFlag)
	{
	case EDGE_FONT1:
		m_PicthWidth = 1;
		break;
	case EDGE_FONT2:
		m_PicthWidth = 2;
		break;
	case EDGE_SHADOW:
		m_PicthWidth = 1;
		break;
	default:
		m_PicthWidth = 0;
	}

	m_Row = pRow;
	m_Column = pColumn;
	m_Width = m_Column * (m_Font->GetWidth() + m_PicthWidth ) + 14;
	m_Height = m_Row * (m_Font->GetHeight() + m_PicthWidth) + 14;

	if(m_BufImage)
		CImageManager::GetInstance().DeleteImage(m_BufImage);
	
	CImageManager::GetInstance().DeleteImage(m_BufImage);
	m_BufImage = CImageManager::GetInstance().CreateImage(m_Width, m_Height, 8888);
	m_BufImage->FillUInt32();
	SetRect(&m_DetectRect, m_X, m_Y, m_X + m_Width, m_X + m_Height);
}


void UIStaticText::SetImageWH(UInt32 Width, UInt32 Height)
{
	UInt32	TempFlag = m_FontStyle & (0x3 << 1);

	m_Width = (Width > m_Font->GetWidth() ? Width : m_Font->GetWidth());
	m_Height = (Height > m_Font->GetHeight() ? Height : m_Font->GetHeight());

	switch(TempFlag)
	{
	case EDGE_FONT1:
		m_PicthWidth = 1;
		break;
	case EDGE_FONT2:
		m_PicthWidth = 2;
		break;
	case EDGE_SHADOW:
		m_PicthWidth = 1;
		break;
	default:
		m_PicthWidth = 0;
	}

	
	
	m_Row = m_Height / (m_Font->GetHeight() + m_PicthWidth);
	m_Column = m_Width  / (m_Font->GetWidth() + m_PicthWidth);

	//if(m_BufImage)
		//SafeDeleteObject(m_BufImage);
	
	//m_BufImage = SafeCreateObject3(CImage, m_Width, m_Height, 8888);
	CImageManager::GetInstance().DeleteImage(m_BufImage);
	m_BufImage = CImageManager::GetInstance().CreateImage(m_Width, m_Height, 8888);
	m_BufImage->FillUInt32();
	SetRect(&m_DetectRect, m_X, m_Y, m_X + m_Width, m_X + m_Height);
}


void UIStaticText::DrawFont()
{
	if(!Empty())
	{
		if(m_SelStart != m_SelEnd)
		{
			m_Font->SetSel(m_SelStart, m_SelEnd);
		}
		Int32	DrawWidth = 0;
		UInt32	Len = m_BufImage->GetPitch() * m_BufImage->GetHeight();

		m_Font->SetAlign(m_Align);
		if(m_IsPassword)
		{
			StringW	TempText;

			for(UInt32 i = 0; i < m_Text.size(); ++i)
				TempText += _T('*'); 
			m_Font->SetLinePitch(m_LinePitch);
			DrawWidth = m_Font->DrawFontStrG(TempText.c_str(),
				m_BufImage->GetPtr(), 0, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, 0);
			m_Font->SetLinePitch(0);
		}
		else
		{
			m_Font->SetLinePitch(m_LinePitch);
			DrawWidth = m_Font->DrawFontStrG(m_Text.c_str(),
				m_BufImage->GetPtr(), 0, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, 0);
			m_Font->SetLinePitch(0);
		}
		m_Font->SetAlign(ALIGN_LEFT);		
	}
}


void UIStaticText::Render(CImage *pImage)
{
	if(m_SelStart != m_SelEnd && !m_Text.IsEmpty())
	{
		RECT	rect;

		m_SelStartX = m_Font->GetSelStartX();
		SetRect(&rect, m_OffsetX + m_Font->GetSelStartX(), m_OffsetY, 
			m_OffsetX + m_Font->GetSelStartX() + m_Font->GetSelEndWidth(), m_OffsetY + m_SelHeight);
		pImage->FillUInt32(m_SelColor, &rect);
	}

	RECT	SRect = {0, 0, m_Width, m_Height};

	Alpha8888To8888S(m_OffsetX, m_OffsetY, m_BufImage, pImage, &SRect, m_RenderStyle, m_Color);
}


void UIStaticText::Render(CImage *pImage, RECT &RRect)
{
	if(m_SelStart != m_SelEnd && !m_Text.IsEmpty())
	{
		Int32		X = m_OffsetX + m_Font->GetSelStartX(), Y = m_OffsetY;
		Int32		Liner = 1;
		RECT	SRect;

		m_SelStartX = m_Font->GetSelStartX();
		SetRect(&SRect, m_OffsetX + m_Font->GetSelStartX(), m_OffsetY, 
			m_OffsetX + m_Font->GetSelStartX() + m_Font->GetSelEndWidth(), m_OffsetY + m_SelHeight);

		if(CImageDirtyRect(X, Y, SRect, RRect))
			pImage->FillUInt32(m_SelColor, &SRect);
	}

	RECT	SRect = {0, 0, m_Width, m_Height};
	Int32	X = m_OffsetX, Y = m_OffsetY;

	if(CImageDirtyRect(X, Y, SRect, RRect))
		Alpha8888To8888S(X, Y, m_BufImage, pImage, &SRect, m_RenderStyle, m_Color);
}

//////////////////////////////////////////////////////////////////////

void UIDynamicText::Reset()
{
	m_StartX = 0;
	m_PanellStartX = m_MoveWidth;
	m_count = 0;
}
UInt32 UIDynamicText::Update(UInt32 dwDelta)
{
	UInt32 Diff = (dwDelta - m_PreUpdateTime) * m_Speed / 1000;
	if (Diff > 0)
	{
		if (m_PanellStartX > 0)
		{
			m_PanellStartX --;//= Diff;
			if (m_PanellStartX < 0)
				m_PanellStartX = 0;
		}
		else
		{
			m_StartX ++;//= Diff;
			if (m_StartX > (Int32)(m_DrawWidth))
			{
				m_StartX = 0;
				m_PanellStartX = m_MoveWidth;
				m_count ++;
			}
		}
		m_PreUpdateTime = dwDelta;

		RenderParent();

		return 1;
	}
	return 0;
}

void UIDynamicText::Render(CImage *pImage)
{
	RECT	SRect = {m_StartX, 0, m_Width - m_PanellStartX, m_Height};

	if (m_StartX == m_Width - m_PanellStartX)
		return;

	
	Alpha8888To8888S(m_OffsetX + m_PanellStartX, m_OffsetY, m_BufImage, pImage, &SRect, m_RenderStyle, m_Color);
}

void UIDynamicText::DrawFont()
{
	if(!Empty())
	{
		if(m_IsPassword)
		{
			{
				StringW	TempText;

				m_Font->SetLinePitch(m_LinePitch);
				for(UInt32 i = 0; i < m_Text.size(); ++i)
					TempText += _T('*'); 
				m_Font->DrawFontStrG(TempText.c_str(),
					m_BufImage->GetPtr(), 0, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
					m_FontColor, m_ShadowColor, 0);
				m_Font->SetLinePitch(0);
			}

		}
		else
		{
			PCSTRINGW pString = m_Text.c_str();
			
			Int X = 0;

			while(*pString)
			{
				StringW WStr;
				UInt32 Len;
				Int32	Ret = m_Font->DrawFontStrC(pString, WStr, X, 0, m_BufImage->GetWidth(), m_BufImage->GetPitch(), m_FontStyle, Len);
				
				m_DrawWidth = m_Font->GetLastDrawWidth();
				if (!WStr.IsEmpty())
				{
					m_Font->SetLinePitch(m_LinePitch);
					m_Font->DrawFontStrG(WStr.c_str(),
						m_BufImage->GetPtr(), X, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
						m_FontColor, m_ShadowColor, 0);
					m_Font->SetLinePitch(0);
				}
				X = m_DrawWidth;

				pString += Len;

				if(Ret == -1)
				{
					break;
				}

				while(pString[0] == _T('#'))
				{
					if(pString[1] == _T('#'))
					{
						break;
					}
					else
					{
						BOOL Found = FALSE;
						for (UInt i = 0; i < MaxTextColor; i++)
						{
							if (TextColorStr[i][0] == pString[1])
							{
								pString += 2;
								m_FontColor = TextColor[i];
								Found = TRUE;
								break;
							}
						}
						if (!Found)
						{
							if (pString[1] == RedColorCmd.GetAt(0))
							{
								pString += 2;
								m_FontColor = RedColor;
							}
							else
								++pString;
						}
					}
				}


				while(pString[0] == _T('\\'))
				{
					if(pString[1] == _T('r'))
					{
						pString += 2;
					}
					else
					if(pString[1] == _T('c'))
					{
						pString += 2;

						if(pString[0] >= _T('0') && pString[0] <= _T('9'))
						{

							++pString;
						}
					}
					else
					if(pString[1] == _T('\\'))
					{
						break;
					}
					else
					{
						++pString;
					}
				}
			}

		}
	}
}

void UIDynamicText::Render(CImage *pImage, RECT &RRect)
{
	RECT	SRect = {m_StartX, 0, m_Width - m_PanellStartX, m_Height};
	Int32	X = m_OffsetX + m_PanellStartX, Y = m_OffsetY;

	if (m_StartX == m_Width - m_PanellStartX)
		return;

	if(CImageDirtyRect(X, Y, SRect, RRect))
		Alpha8888To8888S(X, Y, m_BufImage, pImage, &SRect, m_RenderStyle, m_Color);
}

//////////////////////////////////////////////////////////////////////


UIButton::UIButton() : UIItem()
{
	m_PicImage = NULL;
	m_State = 0;
	ZeroMemory(m_AniIcon, _MaxButton_State * sizeof(UIAniIcon *));
	ZeroMemory(m_SubOffsetX, _MaxButton_State * sizeof(Int32));
	ZeroMemory(m_SubOffsetY, _MaxButton_State * sizeof(Int32));
//		m_DetectImage = false;
	m_PreAniIcon = NULL;
	m_Simple = false;
	m_LockState = false;
}


void UIButton::Dispose()
{
	for (UInt i = 0; i < _MaxButton_State; i++)
	{
		if (m_AniIcon[i])
			SafeDeleteObject(m_AniIcon[i]);
	}
	CImageManager::GetInstance().DeleteImage(m_PicImage);
	m_PicImage = NULL;
}


void	UIButton::SetRect(Int32 pIndex, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	m_PicRect[pIndex].left = pLeft;
	m_PicRect[pIndex].top = pTop;
	m_PicRect[pIndex].right = pRight;
	m_PicRect[pIndex].bottom = pBottom;
	m_UseFlag[pIndex] = true;
}



void	UIButton::SetStateDetectRect(BUTTON_STATE pState)
{
	Int32 PanelX = m_X - m_OffsetX;
	Int32 PanelY = m_Y - m_OffsetY;
	m_Width = m_PicRect[pState].right - m_PicRect[pState].left;
	m_Height = m_PicRect[pState].bottom - m_PicRect[pState].top;
	SetOffsetXY(m_SubOffsetX[pState], m_SubOffsetY[pState]);
	SetOffsetPos(PanelX, PanelY);
	
	SetDetectRect();

}

void UIButton::SetNotMouseOver()
{
	if (m_LockState || m_State == _ButtonDisable)
		return;

	if (m_State != _ButtonNormal && m_AniIcon[m_State])
	{
		//gGlobal.pUIManager->RemoveUpdateUIItem(m_AniIcon[m_State]);
		m_PreAniIcon = m_AniIcon[m_State];
	}
	SetState(_ButtonNormal);
}

void UIButton::SetRealRect(RECT &RRect)
{
	RRect.left = m_X;
	RRect.top = m_Y;
	RRect.right = m_X + m_Width;
	RRect.bottom = m_Y + m_Height;
	if (m_PreAniIcon)
	{
		QGLMath::CalculateBiggestRect(RRect, m_PreAniIcon->GetPreDirtyRect(), RRect);
		//m_PreAniIcon = NULL;
	}
	else if (m_AniIcon[m_State])
		m_AniIcon[m_State]->SetRealRect(RRect);
}

void UIButton::SetRealOffsetRect(RECT &RRect)
{

	RRect.left = m_OffsetX;
	RRect.top = m_OffsetY;
	RRect.right = m_OffsetX + m_Width;
	RRect.bottom = m_OffsetY + m_Height;
	if (m_PreAniIcon)
	{
		QGLMath::CalculateBiggestRect(RRect, m_PreAniIcon->GetPreDirtyRect(), RRect);
		//m_PreAniIcon = NULL;
	}
	else if (m_AniIcon[m_State])
		m_AniIcon[m_State]->SetRealOffsetRect(RRect);
}


void UIButton::SetSubOffSet(Int32 pIndex, Int32 OffX, Int32 OffY)
{
	m_SubOffsetX[pIndex] = OffX;
	m_SubOffsetY[pIndex] = OffY;
}

void UIButton::SetNormalState()
{
	if(m_LockState)
		return;
	SetState(_ButtonNormal);
}

void UIButton::SetMouseOverState()
{
	if(m_LockState)
		return;
	SetState(_ButtonMouseOver);
}

void UIButton::SetClickState()
{
	if(m_LockState)
		return;
	SetState(_ButtonClick);
}

void UIButton::SetDisableState()
{
	if(m_LockState)
		return;
	SetState(_ButtonDisable);
}

void UIButton::SetNoticeState()
{
	if(m_LockState)
		return;
	SetState(_ButtonNotice);
}

void	UIButton::SetState(BUTTON_STATE pState)
{
	if(m_LockState)
		return;
	if (m_State != pState)
	{
		if(!m_Simple)
		{
			if (m_AniIcon[m_State])
			{
				m_PreAniIcon = m_AniIcon[m_State];
				//gGlobal.pUIManager->RemoveUpdateUIItem(m_AniIcon[m_State]);
			}
			if (m_State != pState)
			{
				SetChange(true);
				/*if (GetParent())
				{
				RECT rect;
				SetRealOffsetRect(rect);
				GetParent()->Render(NULL, rect);
				}*/
			}
			m_State = pState;
			//if (m_AniIcon[m_State])
			//	gGlobal.pUIManager->AddUpdateUIItem(m_AniIcon[m_State]);
			SetStateDetectRect(pState);
		}
		else
		{


			SetChange(true);
			/*if (GetParent())
			{
			RECT rect;
			SetRealOffsetRect(rect);
			GetParent()->Render(NULL, rect);
			}*/

			m_State = pState;
		}
		SetChange(true);
		RenderParent();
	}
}


void	UIButton::SimpleSetState(BUTTON_STATE pState)
{
	if(m_LockState)
		return;
	if (m_State != pState)
	{
		m_State = pState;
		SetChange(true);
		RenderParent();
	}
}


void UIButton::Render(CImage *pImage)
{
	
	if(m_UseFlag[m_State])
	{
		if (m_AniIcon[m_State])
		{
			m_AniIcon[m_State]->Render(pImage);
		}
		else
			Alpha8888To8888S(m_OffsetX, m_OffsetY, m_PicImage, pImage, m_PicRect + m_State, m_RenderStyle, m_Color);
	}
}


void UIButton::Render(CImage *pImage, RECT &RRect)
{
	if(m_UseFlag[m_State])
	{
		if (m_AniIcon[m_State])
		{
			if(m_LockState)
				m_AniIcon[m_State]->Render(pImage);
			else
				m_AniIcon[m_State]->Render(pImage, RRect);
		}
		else
		{

			RECT	SRect;
			Int32	X = m_OffsetX, Y = m_OffsetY;

			memcpy(&SRect, m_PicRect + m_State, sizeof(RECT));
			if(CImageDirtyRect(X, Y, SRect, RRect))
				Alpha8888To8888S(X, Y, m_PicImage, pImage, &SRect, m_RenderStyle, m_Color);
		}
	}
}


void UIButton::SetAniIcon(Int32 pIndex, UIAniIcon * pAniIcon)
{
	m_AniIcon[pIndex] = pAniIcon;
}

void UIButton::SetOffsetPos(Int32 pX, Int32 pY)
{
	SetPos(pX + m_OffsetX, pY + m_OffsetY);
	for (UInt i = 0; i < _MaxButton_State; i++)
	{
		if (m_AniIcon[i])
			m_AniIcon[i]->SetOffsetPos(pX + m_OffsetX, pY + m_OffsetY);
	}
}


void	UIButton::SetMouseOver()
{
	SetState(_ButtonMouseOver);
}

Int32	UIButton::OnMouseMove(Int32 pX, Int32 pY)
{
	if (m_State == _ButtonDisable)
		return 0;

	bool	ret = Detect(pX, pY);

	if(ret)
	{
		if(m_State == _ButtonNormal)
		{
			SetState(_ButtonMouseOver);
		}

	}
	else
	{
		if(m_State == _ButtonMouseOver)
		{
			SetState(_ButtonNormal);
			return 1;
		}
	}

	return 0;
}


Int32 UIButton::OnLButtonUp(Int32 pX, Int32 pY)
{
	if (m_State == _ButtonDisable)
		return 0;

	bool ret = UIItem::Detect(pX, pY);
	
	if(ret)
	{
		if( !m_StopPlaySoundOnce )
		{
			FSound* sfx = FSOUND_Manager::GetInstance().CreateSoundA("Data\\Wav\\SoundI0005_click.wav");
			if (sfx)
			{
				sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
				sfx->Play();
			}
		}
		SetState(_ButtonMouseOver);
		return 1;
	}
	else
	{
		SetState(_ButtonNormal);
	}

	return 0;
}


Int32 UIButton::OnLButtonDown(Int32 pX, Int32 pY)
{
	if (m_State == _ButtonDisable)
		return 0;

	if(UIItem::Detect(pX, pY))
	{
		SetState(_ButtonClick);
		return 1;
	}
	
	return 0;
}

Int32 UIButton::OnRButtonUp(Int32 pX, Int32 pY)
{
	if (m_State == _ButtonDisable)
		return 0;

	if(UIItem::Detect(pX, pY))
	{
		SetState(_ButtonClick);
		return 1;
	}
	
	return 0;
}
Int32 UIButton::OnRButtonDown(Int32 pX, Int32 pY)
{
	if (m_State == _ButtonDisable)
		return 0;

	return UIItem::Detect(pX, pY);
}
Int32 UIButton::OnLButtonDBLCLK(Int32 pX, Int32 pY)
{
	if (m_State == _ButtonDisable)
		return 0;

	return UIItem::Detect(pX, pY);
}
Int32 UIButton::OnRButtonDBLCLK(Int32 pX, Int32 pY)
{
	if (m_State == _ButtonDisable)
		return 0;

	return UIItem::Detect(pX, pY);
}

void UIButton::SetParent(UIItem *pParent)
{
	m_Parent = pParent;

	for(Int32 i = 0; i < _MaxButton_State; ++i)
	{
		if(m_AniIcon[i])
			m_AniIcon[i]->SetParent(pParent);
	}
}


///////////////////////////////////////////////////////////////////////


void UIListBox::SetBarRect(RECT &rect, Int32 pIndex)
{
	rect.left = 0;
	rect.top = pIndex * (m_Font->GetHeight() + m_LinePitch);
	rect.right = m_BarWidth;
	rect.bottom = rect.top + (m_Font->GetHeight() + m_LinePitch);
}


Int32	UIListBox::OnLButtonDown(Int32 pX, Int32 pY)
{
	bool ret = UIItem::Detect(pX, pY);

	if(ret)
	{
		UInt32	Index = (pY - m_DetectRect.top) / (m_Font->GetHeight() + m_LinePitch);

		if(Index + m_StartIndex < m_StringList.size() && Index < m_Row)
		{
			RECT	BarRect;
			if(m_SelIndex < (Int)m_Row)
			{
				SetBarRect(BarRect, m_SelIndex);
				m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
				m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
					m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
					m_FontColor, m_ShadowColor, m_Column);
			}
			m_SelIndex = Index;
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_SelFontColor, m_ShadowColor, m_Column);
		}
		else
			return 0;
	}

	return ret;
}

Int32 UIListBox::SetCurSel(int iCurSel)
{
	if (iCurSel>=m_StartIndex && iCurSel < m_StringList.size()) {
		return m_SelIndex=iCurSel-m_StartIndex;
	}
	return -1;
}

Int32 UIListBox::OnKeyDown(WPARAM wParam)
{
	m_LastKey = wParam;
	switch(wParam)
	{
	case VK_LEFT:		
		
		break;
	case VK_RIGHT:		
		
		break;
	case VK_UP:
		UpSel();
		break;
	case VK_DOWN:
		DownSel();
		break;
	case VK_PRIOR:
		PageUp();
		break;
	case VK_NEXT:
		PageDown();
		break;
	case VK_HOME:
		Home();
		break;
	case VK_END:
		End();
		break;
	case VK_SPACE:
		
		break;
	default:
		m_LastKey = 0;
	}

	return 1;
}


void UIListBox::UpSel()
{
	RECT	BarRect;

	if(m_SelIndex < (Int)m_Row)
	{
	}
	else
	if(m_SelIndex > 0)
	{
		SetBarRect(BarRect, m_SelIndex);
		m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
		m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
			m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
			m_FontColor, m_ShadowColor, m_Column);
		--m_SelIndex;
		SetBarRect(BarRect, m_SelIndex);
		m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
		m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
			m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
			m_SelFontColor, m_ShadowColor, m_Column);
	}
	else
	if(m_StartIndex > 0)		
	{
		SetBarRect(BarRect, m_SelIndex);
		m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
		m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
			m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
			m_FontColor, m_ShadowColor, m_Column);
		m_BufImage->ScrollDown(m_Font->GetHeight() + m_LinePitch);
		--m_StartIndex;
		m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
		m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
			m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
			m_SelFontColor, m_ShadowColor, m_Column);
		m_BufImage->ClearBottomLine(4);
	}
}


void UIListBox::DownSel()
{
	RECT	BarRect;

	if(m_SelIndex < (Int)m_Row)
	{
	}
	else
	if(m_StartIndex + m_SelIndex < (Int)m_StringList.size() - 1)
	{
		if(m_SelIndex < (Int)m_Row - 1)
		{
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(0, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, m_Column);
			++m_SelIndex;
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_SelFontColor, m_ShadowColor, m_Column);
		}
		else	
		{
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(0, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, m_Column);
			m_BufImage->ScrollUp(m_Font->GetHeight() + m_LinePitch);
			++m_StartIndex;
			m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_SelFontColor, m_ShadowColor, m_Column);
		}
	}
}


void UIListBox::Home()
{
	if(m_StartIndex || m_SelIndex)
	{
		if(!m_StartIndex && m_SelIndex)
		{
			RECT	BarRect;

			if(m_SelIndex < (Int)m_Row)
			{
				SetBarRect(BarRect, m_SelIndex);
				m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
				m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
					m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
					m_FontColor, m_ShadowColor, m_Column);
			}
			m_SelIndex = 0;
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_SelFontColor, m_ShadowColor, m_Column);
		}
		else
		{
			m_StartIndex = m_SelIndex = 0;
			m_BufImage->ClearAll();
			DrawFont();
		}
		
	}
}


void UIListBox::End()
{
	if(m_StartIndex + m_SelIndex < (Int)m_StringList.size() - 1)
	{
		if(m_StringList.size() <= m_Row)
		{
			RECT	BarRect;

			if(m_SelIndex < (Int)m_Row)
			{
				SetBarRect(BarRect, m_SelIndex);
				m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
				m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
					m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
					m_FontColor, m_ShadowColor, m_Column);
			}
			m_StartIndex = 0;
			m_SelIndex = static_cast<UInt32>(m_StringList.size() - 1);
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_SelFontColor, m_ShadowColor, m_Column);
			return;
		}
		else
		{
			m_StartIndex = static_cast<UInt32>(m_StringList.size() - m_Row);
			m_SelIndex = m_Row - 1;
		}
		
		m_BufImage->ClearAll();
		DrawFont();
	}
}


void UIListBox::PageUp()
{
	if(m_SelIndex < (Int)m_Row)
	{
	}
	else
	if(m_StartIndex >= (Int32)m_Row)
	{
		m_StartIndex -= m_Row;
		m_BufImage->ClearAll();
		DrawFont();
	}
	else
	{
		Home();
	}
}


void UIListBox::PageDown()
{
	if(m_SelIndex < (Int)m_Row)
	{
	}
	else
	if(m_StartIndex + (Int)m_Row < (Int32)m_StringList.size())
	{
		m_StartIndex += m_Row;
		m_BufImage->ClearAll();
		DrawFont();
	}
	else
	{
		End();
	}
}


void UIListBox::DrawFont()
{
	RECT	BarRect;
	if(m_SelIndex < 1000)
	{
		SetBarRect(BarRect, m_SelIndex);


		m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
	}
	for(UInt32 i = m_StartIndex, j = 0; i < m_StringList.size() && j < m_Row; ++i, ++j)
	{
		SetBarRect(BarRect, i);
		//		m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
		m_Font->DrawFontStr(m_StringList[i].c_str(),
			m_BufImage->GetPtr(), 0, j * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
			m_FontColor, m_ShadowColor, m_Column);
		/*if (i != m_SelIndex) {
			m_Font->DrawFontStr(m_StringList[i].c_str(),
				m_BufImage->GetPtr(), 0, j * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, m_Column);
		}
		else
			m_Font->DrawFontStr(m_StringList[i].c_str(),
			m_BufImage->GetPtr(), 0, j * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
			m_SelFontColor, m_ShadowColor, m_Column);*/
	}
}


Int32 UIListBox::GetBarPos()
{
	////VMProtectBegin; VM_START;

	Int32	Pos = -1;
	if(m_StringList.size())
	{
		VMProtectBegin; VM_START;
		UInt32	Sel = m_SelIndex + m_StartIndex;
		if(Sel < m_StringList.size())
		{
			Pos = (Int32)(Sel);
		}
		VMProtectEnd; VM_END;
	}
	
	return Pos;
	
	////VMProtectEnd; VM_END;
}


PCSTRINGW UIListBox::GetBarString()
{
	////VMProtectBegin; VM_START;

	PCSTRINGW Str = NULL;

	if(m_StringList.size())
	{
		VMProtectBegin; VM_START;

		UInt32	Sel = m_SelIndex + m_StartIndex;
		if(Sel < m_StringList.size())
		{
			Str = m_StringList[m_SelIndex + m_StartIndex].c_str();
		}

		VMProtectEnd; VM_END;
	}

	return Str;

	////VMProtectEnd; VM_END;
}


/////////////////////////////////////////////////////////////////////////////////////////////


void UIPopUpMenu::SetNotMouseOver()
{
	RECT	BarRect;

	if (m_SelIndex < 1000 && m_SelIndex >= 0)
	{
		SetBarRect(BarRect, m_SelIndex);
		m_BufImage->FillUInt32(0, &BarRect);
		
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, m_Column);
		
	}
	m_SelIndex = -1;
}


Int32	UIPopUpMenu::OnMouseMove(Int32 pX, Int32 pY)
{
	bool ret = UIItem::Detect(pX, pY);

	SetChange(true);

	if(ret)
	{

		Int32	Index = (pY - m_DetectRect.top) / (m_Font->GetHeight() + m_LinePitch);

		RECT	BarRect;

		if(m_SelIndex != Index )
		{
			if(m_SelIndex >= 0 && m_SelIndex < (Int)m_StringList.size() && m_SelIndex < (Int32)m_Row)
			{
				SetBarRect(BarRect, m_SelIndex);
				m_BufImage->FillUInt32(0, &BarRect);
				m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
					m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
					m_FontColor, m_ShadowColor, m_Column);
				SetChange(true);
			}
		}

		if(Index >= 0 && Index < (Int32)m_StringList.size() && Index < (Int32)m_Row)
		{
			m_SelIndex = Index;
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(m_SelBarColor, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch) + 1, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_SelFontColor, m_ShadowColor, m_Column);
			SetChange(true);
		}
		
	}
	else
	{
		RECT	BarRect;

		if(m_SelIndex >= 0 && m_SelIndex < (Int32)m_StringList.size() && m_SelIndex < (Int32)m_Row)
		{
			SetBarRect(BarRect, m_SelIndex);
			m_BufImage->FillUInt32(0, &BarRect);
			m_Font->DrawFontStr(m_StringList[m_StartIndex + m_SelIndex].c_str(),
				m_BufImage->GetPtr(), 0, m_SelIndex * (m_Font->GetHeight() + m_LinePitch), m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, m_Column);
			SetChange(true);
		}
		m_SelIndex = -1;
	}

	return ret;
}

void UIPopUpMenu::ClearData()
{
	m_FuncCodeList.clear();
	m_NextMenu.clear();
	m_StringList.clear();
	m_SelIndex = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////


UIScrollBarV::UIScrollBarV() : UIItem()

{
	m_PrevBtn = m_NextBtn = m_MoveBtn = NULL;
	m_Image = NULL;
	SetRect(&m_BarRect, 0, 0, 0, 0);
	m_SelNum = m_NumCount = 0;
	m_BarRenderType = 0;
	m_HotSpot = NULL;
	m_PrevBtnOffsetX = m_PrevBtnOffsetY =
	m_NextBtnOffsetX = m_NextBtnOffsetY =
	m_MoveBtnOffsetX = m_MoveBtnOffsetY = m_MoveBtnOffsetGo = 0;
};


void UIScrollBarV::Dispose()
{
	SafeDeleteObject(m_PrevBtn);
	SafeDeleteObject(m_NextBtn);
	SafeDeleteObject(m_MoveBtn);
}


void UIScrollBarV::InitButton()
{
	m_PrevBtn = SafeCreateObject(UIButton);
	m_NextBtn = SafeCreateObject(UIButton);
	m_MoveBtn = SafeCreateObject(UIButton);
};


void UIScrollBarV::MovePos(Int32 pX, Int32 pY)
{	
	UIItem::MovePos(pX, pY);
	m_PrevBtn->MovePos(pX, pY);
	m_NextBtn->MovePos(pX, pY);
	m_MoveBtn->MovePos(pX, pY);
}


void UIScrollBarV::SetBarRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	SetRect(&m_BarRect, pLeft, pTop, pRight, pBottom);
	m_Width = pRight - pLeft;
	m_Height = pBottom - pTop;

	//SetBarInfo();
}


void UIScrollBarV::SetBarWH(UInt32 pWidth, UInt16 pHeight)
{
	m_Width = pWidth;
	m_Height = pHeight;

	//SetBarInfo();
}


void UIScrollBarV::SetBarHeight(UInt32 pHeight)
{
	m_Width = m_PrevBtn->GetWidth();
	m_Height = pHeight;

	//SetBarInfo();
}


void UIScrollBarV::ResetNumCount(UInt32	pNumCount)
{
	m_NumCount = pNumCount;
	if(m_SelNum >= m_NumCount)
	{
		m_SelNum = (m_NumCount ? m_NumCount - 1 : 0);
	};
	m_MoveBtnOffsetGo = m_ScrollAreaLen * m_SelNum / m_NumCount;
	m_MoveBtn->SetPos(m_X, m_ScrollStart + m_MoveBtnOffsetGo);
	m_MoveBtn->SetDetectRect();
}



void UIScrollBarV::SetNotMouseOver()
{
	m_PrevBtn->SimpleSetState(_ButtonNormal);
	m_NextBtn->SimpleSetState(_ButtonNormal);
	m_MoveBtn->SimpleSetState(_ButtonNormal);

	SetChange(true);
}


void UIScrollBarV::SetOffsetPos(Int32 pX, Int32 pY)
{
	/*
	UInt32 DY = 0;
	if(m_NumCount)
		DY = m_ScrollAreaLen * m_SelNum / m_NumCount;
		*/

	SetPos(pX + m_OffsetX, pY + m_OffsetY);

	m_PrevBtn->SetPos(pX + m_PrevBtnOffsetX, pY + m_PrevBtnOffsetY);
	m_NextBtn->SetPos(pX + m_NextBtnOffsetX, pY + m_NextBtnOffsetY);
	m_MoveBtn->SetPos(pX + m_MoveBtnOffsetX, pY + m_MoveBtnOffsetY + m_MoveBtnOffsetGo);
}


void UIScrollBarV::SetOffsetXY(Int32 pX, Int32 pY)
{
	Int32 TempX = m_X - m_OffsetX;
	Int32 TempY = m_Y - m_OffsetY;

	m_OffsetX = pX;
	m_OffsetY = pY;

	SetPos(TempX + m_OffsetX, TempY + m_OffsetY);

	if(m_PrevBtn)
	{
		m_PrevBtnOffsetX = pX; m_PrevBtnOffsetY = pY;
		m_PrevBtn->SetOffsetXY(m_PrevBtnOffsetX , m_PrevBtnOffsetY);
		
	}
	if(m_NextBtn)
	{
		m_NextBtnOffsetX = pX; m_NextBtnOffsetY = pY + m_Height - m_NextBtn->GetHeight();
		m_NextBtn->SetOffsetXY(m_NextBtnOffsetX, m_NextBtnOffsetY);
	}
	if(m_MoveBtn)
	{
		m_ScrollStart = pY + m_PrevBtn->GetHeight();
		m_MoveBtnOffsetX = pX; 
		m_MoveBtnOffsetY = m_ScrollStart;
		m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY + m_MoveBtnOffsetGo);
	}
}


void UIScrollBarV::SetBarInfo()
{
	if(m_PrevBtn)
	{
		m_PrevBtn->SetStateDetectRect();
	}
	if(m_NextBtn)
	{
		m_NextBtn->SetStateDetectRect();
	}
	if(m_MoveBtn)
	{
		m_MoveBtn->SetStateDetectRect();
	}

	m_PrevCount = m_Y + m_PrevBtn->GetHeight();
	m_NextCount = m_Y + m_Height - m_NextBtn->GetHeight() - m_MoveBtn->GetHeight();

	m_ScrollAreaLen = m_Height - m_PrevBtn->GetHeight() - m_NextBtn->GetHeight() - m_MoveBtn->GetHeight();
}


void UIScrollBarV::Render(CImage *pImage)
{
	switch(m_BarRenderType)
	{
	case 1:
		if(m_BarColor)
		{
			RECT	rect;
			SetRect(&rect, m_OffsetX, m_OffsetY, m_OffsetX + m_Width, m_OffsetY + m_Height);
			pImage->FillUInt32(m_BarColor, &rect);
		}
		break;
	case 2:
		Copy8888To8888(m_OffsetX, m_OffsetY, m_Image, pImage, &m_BarRect);
		break;
	};
	
	m_PrevBtn->Render(pImage);
	m_NextBtn->Render(pImage);
	m_MoveBtn->Render(pImage);

}


void UIScrollBarV::Render(CImage *pImage, RECT &RRect)
{
	RECT	SRect;
	Int32	X = m_OffsetX, Y = m_OffsetY;

	switch(m_BarRenderType)
	{
	case 1:
		if(m_BarColor)
		{
			memcpy(&SRect, &m_DetectRect, sizeof(RECT));
			if(CImageDirtyRect(X, Y, SRect, RRect))
				pImage->FillUInt32(m_BarColor, &SRect);
		}
		break;
	case 2:
		memcpy(&SRect, &m_BarRect, sizeof(RECT));
		if(CImageDirtyRect(X, Y, SRect, RRect))
			Copy8888To8888(X, Y, m_Image, pImage, &SRect);
		break;
	};
	
	m_PrevBtn->Render(pImage, RRect);
	m_NextBtn->Render(pImage, RRect);
	m_MoveBtn->Render(pImage, RRect);
}



bool UIScrollBarV::Detect(Int32 pX, Int32 pY)
{
	if (m_NotDetected)
		return false;

	if(IsPickUp())
	{
		return true;
	}

	bool ret = UIItem::Detect(pX, pY);

	
	if(ret)
	{
		
		UIItem	*TempItem = m_HotSpot;

		
		if(m_PrevBtn->Detect(pX, pY))
		{
			m_HotSpot = m_PrevBtn;
		}
		else
		if(m_NextBtn->Detect(pX, pY))
		{
			m_HotSpot = m_NextBtn;
		}
		else
		if(m_MoveBtn->Detect(pX, pY))
		{
			m_HotSpot = m_MoveBtn;
		}
		else
			m_HotSpot = NULL;

		if(TempItem)
		{
			if(TempItem != m_HotSpot)
				TempItem->Detect(pX, pY);
		}
		

	}
	else
	{
		if(m_HotSpot)
		{
			m_HotSpot->Detect(pX, pY);
		}
	}
	
	return ret;
}

void UIScrollBarV::SetSelNum(UInt32 SelNum)
{
	if(SelNum < m_NumCount)
	{
		m_SelNum = SelNum;
		m_MoveBtnOffsetGo = m_ScrollAreaLen * m_SelNum / m_NumCount;
		m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY + m_MoveBtnOffsetGo);	
	}
}

void UIScrollBarV::HomeScroll()
{
	m_SelNum = 0;
	m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY);	
}


void UIScrollBarV::EndScroll()
{
	m_SelNum = m_NumCount - 1;
	m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY + m_ScrollAreaLen);	
}


Int32	 UIScrollBarV::OnLButtonDown(Int32 pX, Int32 pY)
{
	if(UIItem::Detect(pX, pY))
	{
		if(m_PrevBtn->Detect(pX, pY))
		{
			
			MoveUp();
			m_HotSpot = m_PrevBtn;
			m_PickUp = false;
			m_PrevBtn->PickUp();
			m_PrevBtn->SimpleSetState(_ButtonClick);
			m_NextBtn->SimpleSetState(_ButtonNormal);
			m_MoveBtn->SimpleSetState(_ButtonNormal);
			m_ChangeFlag = true;
		}
		else
		if(m_NextBtn->Detect(pX, pY))
		{
			MoveDown();
			m_HotSpot = m_NextBtn;
			m_PickUp = false;
			m_NextBtn->PickUp();
			m_PrevBtn->SimpleSetState(_ButtonNormal);
			m_NextBtn->SimpleSetState(_ButtonClick);
			m_MoveBtn->SimpleSetState(_ButtonNormal);
			m_ChangeFlag = true;
		}
		else
		if(m_MoveBtn->Detect(pX, pY))
		{
			m_MoveSetPoint = pY;
			m_HotSpot = m_MoveBtn;
			m_PickUp = true;
			m_MoveBtn->PickUp();
			m_PrevBtn->SimpleSetState(_ButtonNormal);
			m_NextBtn->SimpleSetState(_ButtonNormal);
			m_MoveBtn->SimpleSetState(_ButtonClick);
			m_ChangeFlag = true;
		}
		else
		{
			m_MoveBtnOffsetGo = 0;

			if(pY > m_Y + (Int32)m_PrevBtn->GetHeight())
			{
				m_MoveBtnOffsetGo = pY - (m_Y + m_PrevBtn->GetHeight());
				if(m_MoveBtnOffsetGo > (Int32)m_ScrollAreaLen)
					m_MoveBtnOffsetGo = m_ScrollAreaLen;
				m_SelNum = m_NumCount * m_MoveBtnOffsetGo / m_ScrollAreaLen;		
			
				m_MoveBtnOffsetY = m_ScrollStart;
				m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY + m_MoveBtnOffsetGo);
			}
		}

		return 1;	
	}
	
	return 0;
}


void UIScrollBarV::UnPickUp()
{
	UIItem::UnPickUp();
	m_PrevBtn->SimpleSetState(_ButtonNormal);
	m_NextBtn->SimpleSetState(_ButtonNormal);
	m_MoveBtn->SimpleSetState(_ButtonNormal);
	SetChange(true);
	if(m_HotSpot)
	{
		if(m_HotSpot->IsPickUp())
		{
			Int32	OffsetX = m_HotSpot->GetOffsetX();
			Int32	OffsetY = m_HotSpot->GetOffsetY();
			m_HotSpot->SimpleUnPickUp();
			m_HotSpot = NULL;
		}
	}
}


Int32	UIScrollBarV::OnLButtonUp(Int32 pX, Int32 pY)
{
	Int32	mX = pX, mY = pY;


	if(IsPickUp())
	{
		if(UIItem::Detect(pX, pY))
		{
			if(m_HotSpot)
			{
				if(m_HotSpot->Detect(pX, pY))
					UIItem::LuaOnLButtonUp(m_HotSpot, pX, pY);
			}
		}
		UnPickUp();
		return 1;
	}
	else
	{
		
	}

	return 0;
}


UIItem *UIScrollBarV::GetHotSpot()
{
	if(m_HotSpot && m_HotSpot != m_MoveBtn)
		return m_HotSpot;
	else
		return this;

}


void	UIScrollBarV::MoveUp()
{
	if(m_SelNum > 0)
	{
		--m_SelNum;
		if ( m_NumCount == 0 )
			m_MoveBtnOffsetGo = 0;
		else
			m_MoveBtnOffsetGo = m_ScrollAreaLen * m_SelNum / m_NumCount;
		m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY + m_MoveBtnOffsetGo);
	}
}


void	UIScrollBarV::MoveDown()
{
	if(m_SelNum < m_NumCount)
	{
		++m_SelNum;
		m_MoveBtnOffsetGo = m_ScrollAreaLen * m_SelNum / m_NumCount;
		m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY + m_MoveBtnOffsetGo);
	}
}


Int32	UIScrollBarV::OnMouseMove(Int32 pX, Int32 pY)
{
	if(m_PickUp)
	{
		if(m_HotSpot == m_MoveBtn && m_MoveBtn->IsPickUp())
		{

			m_MoveBtnOffsetGo = m_MoveBtn->GetOffsetY() - m_MoveBtnOffsetY;

			m_MoveBtnOffsetGo += pY - m_MoveSetPoint;

			if(m_MoveBtnOffsetGo < 0)
				m_MoveBtnOffsetGo = 0;

			if(m_MoveBtnOffsetGo > (Int32)m_ScrollAreaLen)
				m_MoveBtnOffsetGo = m_ScrollAreaLen - 1;

			{
				m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX, m_MoveBtnOffsetY + m_MoveBtnOffsetGo);	
				m_SelNum = m_NumCount * m_MoveBtnOffsetGo / m_ScrollAreaLen;	
				m_MoveSetPoint = pY;

				LuaOnMouseMove(m_MoveBtn, pX, pY, 0);
			}

			return 1;
		}
	}
	
	{
		if(m_PrevBtn->Detect(pX, pY))
		{
			m_NextBtn->SimpleSetState(_ButtonNormal);
			m_MoveBtn->SimpleSetState(_ButtonNormal);
			m_PrevBtn->SimpleSetState(_ButtonMouseOver);
			SetChange(true);
		}
		else
		if(m_NextBtn->Detect(pX, pY))
		{
			m_PrevBtn->SimpleSetState(_ButtonNormal);
			m_NextBtn->SimpleSetState(_ButtonMouseOver);
			m_MoveBtn->SimpleSetState(_ButtonNormal);
			SetChange(true);
		}
		else
		if(m_MoveBtn->Detect(pX, pY))
		{
			m_PrevBtn->SimpleSetState(_ButtonNormal);
			m_NextBtn->SimpleSetState(_ButtonNormal);
			m_MoveBtn->SimpleSetState(_ButtonMouseOver);
			SetChange(true);
		}
	}

	return 0;
}


Int32	UIScrollBarV::OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ)
{
	if(UIItem::Detect(pX, pY))
	{
		if(pZ > 0)
		{
			UIItem::LuaOnLButtonDown(m_PrevBtn, pX, pY);
			UIItem::LuaOnLButtonUp(m_PrevBtn, pX, pY);
			MoveUp();
		}
		else
		{
			UIItem::LuaOnLButtonDown(m_NextBtn, pX, pY);
			UIItem::LuaOnLButtonUp(m_NextBtn, pX, pY);
			MoveDown();
		}

		return 1;
	}

	return 0;
}


void UIScrollBarV::SetParent(UIItem *pParent)
{
	m_Parent = pParent;

	if (m_PrevBtn)
		m_PrevBtn->SetParent(pParent);

	if (m_NextBtn)
		m_NextBtn->SetParent(pParent);

	if (m_MoveBtn)
		m_MoveBtn->SetParent(pParent);
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////



inline UIScrollBarH::UIScrollBarH() : UIScrollBarV()
{
	
};


void UIScrollBarH::ResetNumCount(UInt32	pNumCount)
{
	m_NumCount = pNumCount;
	if(m_SelNum >= m_NumCount)
	{
		m_SelNum = (m_NumCount ? m_NumCount - 1 : 0);
	};
	m_MoveBtnOffsetGo = m_ScrollAreaLen * m_SelNum / m_NumCount;
	m_MoveBtn->SetPos(m_ScrollStart + m_MoveBtnOffsetGo, m_Y);
	m_MoveBtn->SetDetectRect();
}


void UIScrollBarH::SetBarWidth(UInt32 pWidth)
{
	m_Width = pWidth;
	m_Height = m_PrevBtn->GetHeight();

	//SetBarInfo();
}

void UIScrollBarH::SetOffsetPos(Int32 pX, Int32 pY)
{
	/*
	UInt32 DX = 0;
	if(m_NumCount)
		DX = m_ScrollAreaLen * m_SelNum / m_NumCount;
	*/

	SetPos(pX + m_OffsetX, pY + m_OffsetY);

	m_PrevBtn->SetPos(pX + m_PrevBtnOffsetX, pY + m_PrevBtnOffsetY);
	m_NextBtn->SetPos(pX + m_NextBtnOffsetX, pY + m_NextBtnOffsetY);
	m_MoveBtn->SetPos(pX + m_MoveBtnOffsetX + m_MoveBtnOffsetGo, pY + m_MoveBtnOffsetY);
}

void UIScrollBarH::SetBarInfo()
{
	if(m_PrevBtn)
	{
		m_PrevBtn->SetStateDetectRect();
	}
	if(m_NextBtn)
	{
		m_NextBtn->SetStateDetectRect();
	}
	if(m_MoveBtn)
	{
		m_MoveBtn->SetStateDetectRect();
	}

	m_PrevCount = m_X + m_PrevBtn->GetWidth();
	m_NextCount = m_X + m_Width - m_NextBtn->GetHeight() - m_MoveBtn->GetWidth();

	m_ScrollAreaLen = m_Width - m_PrevBtn->GetWidth() - m_NextBtn->GetWidth() - m_MoveBtn->GetWidth();
}


void	UIScrollBarH::MoveUp()
{
	if(m_SelNum > 0)
	{
		--m_SelNum;
		UInt32 DX = m_ScrollAreaLen * m_SelNum / m_NumCount;
		m_MoveBtn->SetPos(m_ScrollStart + DX, m_Y);
	}
}


void	UIScrollBarH::MoveDown()
{
	if(m_SelNum < m_NumCount)
	{
		++m_SelNum;
		UInt32 DX = m_ScrollAreaLen * m_SelNum / m_NumCount;
		m_MoveBtn->SetPos(m_ScrollStart + DX, m_Y);
	}
}





Int32	 UIScrollBarH::OnLButtonDown(Int32 pX, Int32 pY)
{
	if(UIItem::Detect(pX, pY))
	{
		if(m_PrevBtn->Detect(pX, pY))
		{
			MoveUp();
			m_HotSpot = m_PrevBtn;
			m_PickUp = false;
			m_PrevBtn->PickUp();
			m_PrevBtn->SimpleSetState(_ButtonClick);
			m_NextBtn->SimpleSetState(_ButtonNormal);
			m_MoveBtn->SimpleSetState(_ButtonNormal);
			m_ChangeFlag = true;
			RenderParent();
		}
		else
		if(m_NextBtn->Detect(pX, pY))
		{
			MoveDown();
			m_HotSpot = m_NextBtn;
			m_PickUp = false;
			m_NextBtn->PickUp();
			m_PrevBtn->SimpleSetState(_ButtonNormal);
			m_NextBtn->SimpleSetState(_ButtonClick);
			m_MoveBtn->SimpleSetState(_ButtonNormal);
			m_ChangeFlag = true;
			RenderParent();
		}
		else
		if(m_MoveBtn->Detect(pX, pY))
		{
			m_MoveSetPoint = pX;
			m_HotSpot = m_MoveBtn;
			m_PickUp = true;
			m_MoveBtn->PickUp();
			m_PrevBtn->SimpleSetState(_ButtonNormal);
			m_NextBtn->SimpleSetState(_ButtonNormal);
			m_MoveBtn->SimpleSetState(_ButtonClick);
			m_ChangeFlag = true;
		}
		else
		{
			if(pX > m_X + (Int32)m_PrevBtn->GetWidth())
			{
				m_MoveBtnOffsetGo = pX - (m_X + m_PrevBtn->GetWidth());
				if(m_MoveBtnOffsetGo > (Int32)m_ScrollAreaLen)
					m_MoveBtnOffsetGo = m_ScrollAreaLen;
				m_SelNum = m_NumCount * m_MoveBtnOffsetGo / m_ScrollAreaLen;		
			
				m_MoveBtnOffsetX = m_ScrollStart;
				m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX + m_MoveBtnOffsetGo, m_MoveBtnOffsetX);
			}
		}

		return 1;	
	}
	
	return 0;
}


Int32	UIScrollBarH::OnMouseMove(Int32 pX, Int32 pY)
{
	if(IsPickUp())
	{
		if(m_HotSpot == m_MoveBtn && m_MoveBtn->IsPickUp())
		{
			m_MoveBtnOffsetGo = m_MoveBtn->GetOffsetX() - m_MoveBtnOffsetX;

			m_MoveBtnOffsetGo += pX - m_MoveSetPoint;

			if(m_MoveBtnOffsetGo < 0)
				m_MoveBtnOffsetGo = 0;

			if(m_MoveBtnOffsetGo > (Int32)m_ScrollAreaLen)
				m_MoveBtnOffsetGo = m_ScrollAreaLen - 1;

			{
				m_MoveBtn->SetOffsetXY(m_MoveBtnOffsetX + m_MoveBtnOffsetGo, m_MoveBtnOffsetY);	
				m_SelNum = m_NumCount * m_MoveBtnOffsetGo / m_ScrollAreaLen;	
				m_MoveSetPoint = pX;

				LuaOnMouseMove(m_MoveBtn, pX, pY, 0);
			}
			return 1;
		}
	}
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////



void UIScrollListBox::Render(CImage *pImage)
{
	m_ListBox->Render(pImage);
	if(m_ScrollBarV)
		m_ScrollBarV->Render(pImage);
	if(m_ScrollBarH)
		m_ScrollBarH->Render(pImage);
}


void UIScrollListBox::Render(CImage *pImage, RECT &RRect)
{
}

void UIScrollListBox::SetupScrollBarV()
{
	SafeDeleteObject(m_ScrollBarV);
	m_ScrollBarV = SafeCreateObject(UIScrollBarV);
}

void UIScrollListBox::SetupScrollBarH()
{
	SafeDeleteObject(m_ScrollBarH);
	m_ScrollBarH = SafeCreateObject(UIScrollBarH);
}

void UIScrollListBox::SetPos(Int32 pX, Int32 pY)
{
	m_ListBox->SetPos(pX, pY);
	m_Width = m_ListBox->GetWidth();
	m_Height = m_ListBox->GetHeight();
	if(m_ScrollBarV)
	{
		m_ScrollBarV->SetPos(pX + m_ListBox->GetWidth(), pY);
		m_ScrollBarV->SetBarHeight(m_ListBox->GetHeight());
		m_ScrollBarV->SetDetectRect();
		m_Width += m_ScrollBarV->GetWidth();
	}
	if(m_ScrollBarH)
	{
		m_ScrollBarH->SetPos(pX, pY + m_ListBox->GetHeight());
		m_ScrollBarH->SetBarWidth(m_ListBox->GetWidth());
		m_ScrollBarH->SetDetectRect();
		m_Height += m_ScrollBarH->GetHeight();
	}
	
	m_DetectRect.right = (m_DetectRect.left = m_X = pX) + m_Width;
	m_DetectRect.bottom = (m_DetectRect.top = m_Y = pY) + m_Height;
}

void UIScrollListBox::MovePos(Int32 pX, Int32 pY)
{	
	UIItem::MovePos(pX, pY);
	m_ListBox->MovePos(pX, pY);
	if(m_ScrollBarV)
		m_ScrollBarV->MovePos(pX, pY);
	if(m_ScrollBarH)
		m_ScrollBarH->MovePos(pX, pY);
}



bool UIScrollListBox::Detect(Int32 pX, Int32 pY)
{
	if (m_NotDetected)
		return false;

	if(IsPickUp())
	{
		return true;
	}

	bool ret = UIItem::Detect(pX, pY);

	if(ret)
	{
		UIItem	*TempItem = m_HotSpot;

		if(m_ListBox->Detect(pX, pY))
		{
			m_HotSpot = m_ListBox;
			
		}
		else
		if(m_ScrollBarV->Detect(pX, pY))
		{
			m_HotSpot = m_ScrollBarV; 
		}
		else
		if(m_ScrollBarH->Detect(pX, pY))
		{
			m_HotSpot = m_ScrollBarH;
		}
		else
			m_HotSpot = NULL;

		if(TempItem)
		{
			if(TempItem != m_HotSpot)
				TempItem->Detect(pX, pY);
		}
	}
	else
	{
		if(m_HotSpot)
		{
			if(m_HotSpot)
			{
				m_HotSpot->Detect(pX, pY);
			}
		}
	}

	return ret;
}


Int32	UIScrollListBox::OnLButtonDown(Int32 pX, Int32 pY)
{
	bool Flag = UIItem::Detect(pX, pY);
	Int32	 ret = 0;

	m_HotSpot = NULL;

	if(Flag)
	{
		ret = m_ListBox->OnLButtonDown(pX, pY);

		if(ret)
		{
			m_HotSpot = m_ListBox;
			return ret;
		}
		
		if(m_ScrollBarV)
		{
			ret = m_ScrollBarV->OnLButtonDown(pX, pY);

			if(ret)
			{
				m_HotSpot = m_ScrollBarV;
				PickUp();
				m_HotSpot->PickUp();
				return ret;
			}
		}

		if(m_ScrollBarH)
		{
			ret = m_ScrollBarH->OnLButtonDown(pX, pY);

			if(ret)
			{
				m_HotSpot = m_ScrollBarH;
				PickUp();
				m_HotSpot->PickUp();
				return ret;
			}
		}

	}


	return ret;
}


void UIScrollListBox::UnPickUp()
{
	UIItem::UnPickUp();
	if(m_HotSpot->IsPickUp())
	{
		m_HotSpot->UnPickUp();
		m_HotSpot = NULL;
	}
}


Int32	UIScrollListBox::OnLButtonUp(Int32 pX, Int32 pY)
{
	if(IsPickUp())
	{
		UnPickUp();
		Detect(pX, pY);
		return  1;
	}

	Int32	 ret = 0;

	ret = m_ListBox->OnLButtonUp(pX, pY);
	
	if(m_ScrollBarV)
	{
		ret = m_ScrollBarV->OnLButtonUp(pX, pY);
	}

	if(m_ScrollBarH)
	{
		ret = m_ScrollBarH->OnLButtonUp(pX, pY);
	}

	return ret;
}


Int32	UIScrollListBox::OnMouseMove(Int32 pX, Int32 pY)
{
	if(IsPickUp())
	{
		m_HotSpot->OnMouseMove(pX, pY);
		return 1;
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////


void UIChatString::Redraw(CFont *pFont, CImage *pImage)
{
	if(m_SelWidth)
	{
		RECT	rect;

		SetRect(&rect, m_X, m_Y, m_X + m_Width, m_Y + m_SelHeight);
		pImage->FillUInt32(m_SelColor, &rect);
	}

	m_Width = pFont->DrawFontStr(m_String.c_str(),
		pImage->GetPtr(), m_X, m_Y, pImage->GetWidth(), pImage->GetHeight(), pImage->GetPitch(), m_FontStyle, 
		m_FontColor, m_ShadowColor, 0);

	if(m_UnderLine)
	{
		RECT	rect;

		SetRect(&rect, m_X, m_Y + m_Height - 1, m_X + m_Width, m_Y + m_Height);
		pImage->FillUInt32(m_FontColor, &rect);
	}
	//m_Height = pFont->GetMaxY();

	/*
	if(m_SelStart != m_SelEnd && !m_String.IsEmpty())
	{
		RECT	rect;

		m_SelStartX = pFont->GetSelStartX();
		m_SelWidth =  pFont->GetSelEndWidth();
		m_SelHeight = pFont->GetHeight();
	}
	*/
}


/////////////////////////////////////////////////////////////////////////////


void UIChatLink::SetLinkStringA(PCSTRINGA pStr)
{
	Int32 Len = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
	PSTRINGW m_TempStr = SafeAllocate(CharW, Len);
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, m_TempStr, Len);
	m_LinkString = m_TempStr;
	SafeDeallocate(m_TempStr);
}


/*
void UIChatLink::Redraw(CFont *pFont, CImage *pImage)
{
	m_Width = pFont->DrawFontStr(m_String.c_str(),
		pImage->GetPtr(), m_X, m_Y, pImage->GetWidth(), pImage->GetHeight(), pImage->GetPitch(), m_FontStyle, 
		m_FontColor, m_ShadowColor, 0);
	m_Height = pFont->GetHeight();
}
*/


bool UIChatLink::CheckLinkHead(PCSTRINGA Str)
{
	StringW Str1 = Str;
	StringW Str2 = m_LinkString.substr(0, 4);

	return false;
}


/////////////////////////////////////////////////////////////////////////////


UIChatRow::UIChatRow()
{
	m_Y = m_Width = m_Height = 0;
	m_EndLine = false;
}


UIChatRow::~UIChatRow()
{
	for(std::vector<UIChatItem *>::iterator i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		SafeDeleteObject(*i);
	}
	m_RowItemList.clear();
}


UIChatItem *UIChatRow::GetLastChatItem()
{ 
	if(m_RowItemList.size())
		return m_RowItemList.back(); 
	else
		return NULL;
}


void UIChatRow::AddItem(UIChatItem *pItem)
{
	if(pItem->GetHeight() + UIChatRowPitch > m_Height)
		m_Height = pItem->GetHeight() + UIChatRowPitch; 
	m_RowItemList.push_back(pItem);
}


void UIChatRow::AddStringW(PCSTRINGW pString, D3DCOLOR Color, D3DCOLOR ShadowColor, UInt32 FontStyle, UInt32 m_Height)
{
	UIChatString *MyStr = SafeCreateObject(UIChatString);
	StringW WStr = pString;

	//LuaAnsi2StringW(WStr, (PCSTRINGW)pString);

	MyStr->SetString(WStr.c_str());
	MyStr->SetFontColor(Color);
	MyStr->SetShadowColor(ShadowColor);
	MyStr->SetFontStyle(FontStyle);

	MyStr->SetHeight( FontStyle == 0 ? m_Height : m_Height + 2);

	AddItem(MyStr);
}


bool UIChatRow::IsTextExist(PCSTRINGW key)
{
	for(std::vector<UIChatItem *>::iterator i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		UIChatString * pString = (UIChatString *)(*i);
		if (pString)
		{
			String temp = pString->GetString();
			if (temp.find(key) != std::string::npos)
				return true;
		}
	}
	return false;
}


UInt32 UIChatRow::Update(UInt32 dwDelta, CFont *pFont, CImage *pImage)
{
	UInt32 Result = 0;
	for(std::vector<UIChatItem *>::iterator i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		if ((*i)->Update(dwDelta, pFont, pImage))
			Result = 1;
	}

	return Result;
}


void UIChatRow::Redraw(CFont *pFont, CImage *pImage)
{
	m_Width = 0;
	UInt32	MaxHeight = 0;
	std::vector<UIChatItem *>::iterator i;
	for(i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		if((*i)->GetHeight() > MaxHeight)
			MaxHeight = (*i)->GetHeight();
	}
	for(i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		UInt32 TempHeight = (*i)->GetHeight();
		UInt32 TempY = m_Y + (MaxHeight - (*i)->GetHeight());
		UInt32 TempWidth = (*i)->GetPosX() + (*i)->GetWidth();
		
		(*i)->SetPosY(m_Y + (MaxHeight - (*i)->GetHeight()));
		(*i)->Redraw(pFont, pImage);
		if(TempWidth > m_Width)
			m_Width = TempWidth;
	}
}


void UIChatRow::ResetChatItemsPos()
{
	m_Width = 0;
	UInt32	MaxY = 0;
	std::vector<UIChatItem *>::iterator i;
	for(i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		if((*i)->GetHeight() > MaxY)
			MaxY = (*i)->GetHeight();
	}
	for(i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		UInt32 TempHeight = (*i)->GetHeight();
		UInt32 TempY = m_Y + (MaxY - (*i)->GetHeight());
		(*i)->SetPos(m_Width, m_Y + (MaxY - (*i)->GetHeight()));
		m_Width += (*i)->GetWidth();
	}
}


void UIChatRow::UpdateDetectRect(Int32 pStartX, Int32 pStartY)
{

	m_DetectRect.right = (m_DetectRect.left = pStartX) + m_Width;
	m_DetectRect.bottom = (m_DetectRect.top = pStartY + m_Y) + m_Height;
	
	for(std::vector<UIChatItem *>::iterator i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		(*i)->UpdateDetectRect(pStartX, pStartY);
	}
}


void UIChatRow::SetupDetectRect(Int32 pStartX, Int32 pStartY, Int32 pHeight, Int32 pWidth)
{
	m_DetectRect.right = (m_DetectRect.left = pStartX) + m_Width;
	m_DetectRect.bottom = (m_DetectRect.top = pStartY + m_Y) + pHeight;
}



UIChatItem	*UIChatRow::FindClick(Int32 pX, Int32 pY)
{
	for(std::vector<UIChatItem *>::iterator i = m_RowItemList.begin(); i != m_RowItemList.end(); ++i)
	{
		if(*i)
		{
			if((*i)->Detect(pX, pY))
			{
				return (*i);
			}
		}
	}

	return NULL;
}


/////////////////////////////////////////////////////////////////////////////


UIChatPanel::UIChatPanel() : UIStaticText()
{
	m_ZOffset = 2;
	m_StartRow = m_EndRow = 0;
	m_MaxLine = 100;
	m_ClickItem = NULL;

	m_PrevBtn = NULL;
	m_NextBtn = NULL;
	m_HomeBtn = NULL;
	m_EndBtn = NULL;

	m_EnableScroll = true;
	m_ContinueRun = false;

	m_AutoEndRow = true;
}


void UIChatPanel::InitButton()
{
	m_PrevBtn = SafeCreateObject(UIButton);
	m_NextBtn = SafeCreateObject(UIButton);
	m_HomeBtn = SafeCreateObject(UIButton);
	m_EndBtn = SafeCreateObject(UIButton);
}


void UIChatPanel::Dispose()
{
	SafeDeleteObject(m_PrevBtn);
	SafeDeleteObject(m_NextBtn);
	SafeDeleteObject(m_HomeBtn);
	SafeDeleteObject(m_EndBtn);

	for(std::deque<UIChatRow *>::iterator i = m_RowList.begin(); i != m_RowList.end(); ++i)
	{
		SafeDeleteObject(*i);
	}

	m_RowList.clear();
	m_TempRowList.clear();
}



void UIChatPanel::SetOffsetPos(Int32 pX, Int32 pY)
{
	SetPos(pX + m_OffsetX, pY + m_OffsetY);
	if(m_PrevBtn)
		m_PrevBtn->SetOffsetPos(pX + m_OffsetX, pY + m_OffsetY);
	if(m_NextBtn)
		m_NextBtn->SetOffsetPos(pX + m_OffsetX, pY + m_OffsetY);
	if(m_HomeBtn)
		m_HomeBtn->SetOffsetPos(pX + m_OffsetX, pY + m_OffsetY);
	if(m_EndBtn)
		m_EndBtn->SetOffsetPos(pX + m_OffsetX, pY + m_OffsetY);
}


UInt32	UIChatPanel::FindHighLightWord(UInt16 startLine)
{
	UInt32 i = 0;
	for (std::deque<UIChatRow *>::iterator Itr = m_RowList.begin(); Itr != m_RowList.end(); ++Itr)
	{
		i ++;
		if (i >= startLine)
		{
			for(std::vector<UIChatItem *>::iterator itRow = (*Itr)->GetRowItemList()->begin(); itRow != (*Itr)->GetRowItemList()->end(); ++itRow)
			{
				UIChatString * pString = (UIChatString *)(*itRow);
				if (pString && pString->GetHighLight())
					return i;
			}
		}
	}

	return i;
}


UInt32	UIChatPanel::FindUnderLineWord(UInt16 startLine)
{
	UInt32 i = 0;
	for (std::deque<UIChatRow *>::iterator Itr = m_RowList.begin(); Itr != m_RowList.end(); ++Itr)
	{
		i ++;
		if (i >= startLine)
		{
			for(std::vector<UIChatItem *>::iterator itRow = (*Itr)->GetRowItemList()->begin(); itRow != (*Itr)->GetRowItemList()->end(); ++itRow)
			{
				UIChatString * pString = (UIChatString *)(*itRow);
				if (pString && pString->GetUnderLine())
					return i;
			}
		}
	}

	return i;
}


UInt32	UIChatPanel::FindWord(PCSTRINGW key, UInt16 startLine)
{
	UInt32 i = 0;
	for (std::deque<UIChatRow *>::iterator Itr = m_RowList.begin(); Itr != m_RowList.end(); ++Itr)
	{
		i ++;
		if (i >= startLine && (*Itr)->IsTextExist(key))
			return i;
	}

	return i;
}

bool UIChatPanel::Detect(Int32 pX, Int32 pY)
{
	if (m_NotDetected)
		return false;

	if(UIItem::Detect(pX, pY))
	{
		return true;
	}
	
	if(m_PrevBtn)
	{
		if(m_PrevBtn->Detect(pX, pY))
		{
			return true;
		}
	}

	if(m_NextBtn)
	{
		if(m_NextBtn->Detect(pX, pY))
		{
			return true;
		}
	}

	if(m_HomeBtn)
	{
		if(m_HomeBtn->Detect(pX, pY))
		{
			return true;
		}
	}
	
	if(m_EndBtn)
	{
		if(m_EndBtn->Detect(pX, pY))
		{
			return true;
		}
	}

	return	false;
}


Int32 UIChatPanel::OnMouseMove(Int32 pX, Int32 pY)
{
	return 0;
}


Int32 UIChatPanel::OnLButtonUp(Int32 pX, Int32 pY)
{
	m_ClickItem = NULL;
	return 0;
}


Int32 UIChatPanel::OnRButtonUp(Int32 pX, Int32 pY)
{
	m_ClickItem = NULL;
	return 0;
}


Int32 UIChatPanel::OnLButtonDown(Int32 pX, Int32 pY)
{
	m_ClickItem = NULL;
	if(!UIItem::Detect(pX,  pY))
		return 0;

	if(m_RowList.size())
	{
		for(Int32 i = m_StartRow; i <= m_EndRow; ++i)
		{
			if(m_RowList[i]->Detect(pX, pY))
			{
				m_ClickItem = m_RowList[i]->FindClick(pX, pY);
				return 1;
			}
		}
	}

	if(m_PrevBtn)
	{
		if(m_PrevBtn->Detect(pX, pY))
		{
			UpRow();
			return 1;
		}
	}
	
	if(m_NextBtn)
	{
		if(m_NextBtn->Detect(pX, pY))
		{
			DownRow();
			return 1;
		}
	}

	if(m_HomeBtn)
	{
		if(m_HomeBtn->Detect(pX, pY))
		{
			HomeRow();
			return 1;
		}
	}

	if(m_EndBtn)
	{
		if(m_EndBtn->Detect(pX, pY))
		{
			EndRow();
			return 1;
		}
	}

	


	return 1;
}


Int32 UIChatPanel::OnRButtonDown(Int32 pX, Int32 pY)
{
	m_ClickItem = NULL;
	if(!UIItem::Detect(pX,  pY))
		return 0;

	if(m_RowList.size())
	{
		for(Int32 i = m_StartRow; i <= m_EndRow; ++i)
		{
			if(m_RowList[i]->Detect(pX, pY))
			{
				m_ClickItem = m_RowList[i]->FindClick(pX, pY);
				return 1;
			}
		}
	}


	return 1;
}


Int32	UIChatPanel::OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ)
{
	if(m_EnableScroll && UIItem::Detect(pX, pY))
	{
		if(pZ < 0)
		{
			DownRow();

			return 1;
		}
		else
		if(pZ > 0)
		{
			UpRow();

			return 1;
		}
	}

	return 0;
}


Int32 UIChatPanel::OnKeyDown(WPARAM wParam)
{
	switch(wParam)
	{
	case VK_LEFT:		
		
		break;
	case VK_RIGHT:		
		
		break;
	case VK_UP:
		UpRow();
		break;
	case VK_DOWN:
		DownRow();
		break;

	case VK_PRIOR:
		
		break;
	case VK_NEXT:
		
		break;
	case VK_HOME:
		HomeRow();
		break;
	case VK_END:
		EndRow();
		break;
	case VK_SPACE:

		break;
	}

	return 1;
}


void UIChatPanel::SetFocus(bool pState)
{
	m_Focus = pState;
	if(pState == false)
	{
		m_ClickItem = NULL;	
	}
}


void UIChatPanel::Redraw()
{
	if(m_RowList.size())
	{
		UInt32	Y = 0, i;
		SetChange(true);
		for(i = m_StartRow; i < (Int32)m_RowList.size() && i <= m_EndRow; ++i)
		{
			UInt32	Height = m_RowList[i]->GetHeight();

			if(Y + m_RowList[i]->GetHeight() > m_Height)
				break;

			m_RowList[i]->SetY(Y);
			m_RowList[i]->Redraw(m_Font, m_BufImage);
			Y += Height;
		}
		if(m_EndRow < i - 1)
		{
			m_EndRow = i - 1;
		}
	}
	RenderParent();
}


void UIChatPanel::UpdateDetectRect()
{
	if(m_RowList.size())
	{
		if(m_StartRow < 0)
			m_StartRow = 0;
	
		if(m_EndRow >= (Int)m_RowList.size())
			m_EndRow = (Int)m_RowList.size() - 1;

		for(Int32 i = m_StartRow; i <= m_EndRow; ++i)
		{
			if(m_RowList[i])
				m_RowList[i]->UpdateDetectRect(m_X, m_Y);
		}
	}
}


void UIChatPanel::Start2BottomRow()
{
	UInt32	High = 0;
	Int32 i;
	for(i = (Int32)m_StartRow; i < (Int32)m_RowList.size(); ++i)
	{
		if(High + m_RowList[i]->GetHeight() < m_Height)
		{
			High += m_RowList[i]->GetHeight();
		}
		else
		{
			break;
		}
	}
	m_EndRow = i - 1;
}

void UIChatPanel::SetText(PCSTRINGW pText)
{
	m_Text = pText;
	if (m_BufImage)
	{
		ClearAllRow();
		AddStringW(pText);
	}
}

void UIChatPanel::End2TopRow()
{
	UInt32	High = 0;
	Int32 i;
	for(i = m_EndRow; i >= 0; --i)
	{
		if(High + m_RowList[i]->GetHeight() < m_Height)
		{
			High += m_RowList[i]->GetHeight();
		}
		else
		{
			break;
		}
	}
	m_StartRow = i + 1;
}
UInt32 UIChatPanel::GetAllRowHeight()
{
	UInt32	High = 0;
	for(Int32 i = 0; i < (Int32)m_RowList.size(); ++i)
	{
		High += m_RowList[i]->GetHeight();
	}

	return High;
}
UInt32 UIChatPanel::GetTotalHeight()
{
	if (m_RowList.size() == 0)
		return 0 ;

	UInt32	High = 0;
	for(Int32 i = m_EndRow; i >= 0; --i)
	{
		if(High + m_RowList[i]->GetHeight() < m_Height)
		{
			High += m_RowList[i]->GetHeight();
		}
		else
		{
			break;
		}
	}
	return High;
}

void UIChatPanel::UpRow()
{
	if(m_StartRow > 0 && m_RowList.size() > 0)
	{
		m_StartRow--;
		Start2BottomRow();
		FillImage();
		Redraw();
		UpdateDetectRect();
	}
}


void UIChatPanel::DownRow()
{
	if(m_EndRow < (Int32)m_RowList.size() - 1)
	{
		m_StartRow++;
		Start2BottomRow();

		FillImage();
		Redraw();
		UpdateDetectRect();
	}
}


void UIChatPanel::HomeRow()
{
	m_StartRow = 0;
	Start2BottomRow();

	FillImage();
	Redraw();
	UpdateDetectRect();
}

void UIChatPanel::EndRow()
{
	if (!m_AutoEndRow)
	{
		Int32 OrgStart = m_StartRow;
		Int32 OrgEnd = m_EndRow;
		m_EndRow = (UInt32)(m_RowList.size() - 1);
		End2TopRow();
		if (m_StartRow != OrgStart)
		{
			m_StartRow = OrgStart;
			m_EndRow = OrgEnd;
		}
	}
	else
	{
		m_EndRow = (UInt32)(m_RowList.size() - 1);
		End2TopRow();
	}

	FillImage();
	Redraw();
	UpdateDetectRect();
}


void UIChatPanel::SetRow(UInt32 a, UInt32 b)
{
	if (m_RowList.size() == 0)
		return;

	Int32 OrgStart = m_StartRow;
	Int32 OrgEnd = m_EndRow;
	
	m_StartRow = Int32(m_RowList.size() * a / b);
	if(m_StartRow < 0)
		m_StartRow = 0;
	else
	if(m_StartRow >= (Int32)m_RowList.size())
		m_StartRow = (Int32)m_RowList.size() - 1;

	Start2BottomRow();

	if(m_StartRow != 0 && m_EndRow == (UInt32)(m_RowList.size() - 1))
	{
		End2TopRow();
	}

	FillImage();
	Redraw();
	UpdateDetectRect();
}

void UIChatPanel::Render(CImage *pImage)
{
	UIStaticText::Render(pImage);
	if(m_HomeBtn)
		m_HomeBtn->Render(pImage);
	if(m_EndBtn)
		m_EndBtn->Render(pImage);
	if(m_PrevBtn)
		m_PrevBtn->Render(pImage);
	if(m_NextBtn)
		m_NextBtn->Render(pImage);
}


void UIChatPanel::Render(CImage *pImage, RECT &RRect)
{
	UIStaticText::Render(pImage, RRect);
	if(m_HomeBtn)
		m_HomeBtn->Render(pImage, RRect);
	if(m_EndBtn)
		m_EndBtn->Render(pImage, RRect);
	if(m_PrevBtn)
		m_PrevBtn->Render(pImage, RRect);
	if(m_NextBtn)
		m_NextBtn->Render(pImage, RRect);
}


void UIChatPanel::ClearAllRow()
{
	for(std::deque<UIChatRow *>::iterator i = m_RowList.begin(); i != m_RowList.end(); ++i)
	{
		SafeDeleteObject(*i);
	}

	m_RowList.clear();

	m_StartRow = m_EndRow = 0;
	FillImage();
}

UInt32 UIChatPanel::Update(UInt32 dwDelta)
{
	if (m_RowList.size() == 0)
		return 0;

	UInt32 Result = 0;
	for(Int32 i = m_StartRow; i <= m_EndRow; ++i)
	{
		if(m_RowList[i])
			if (m_RowList[i]->Update(dwDelta, m_Font, m_BufImage))
				Result = 1;
	}

	if(Result)
	{
		RenderParent();
	}

	return Result;
}


void UIChatPanel::ClearOverLine()
{
	while((Int32)m_RowList.size() > m_MaxLine)
	{
		SafeDeleteObject(m_RowList[0]);
		m_RowList.pop_front();
	}
}


void UIChatPanel::AddStringWNoPrefix(PCSTRINGW pString)
{
	RealAddStringW(pString, false);

	for (std::deque<UIChatRow *>::iterator Itr = m_TempRowList.begin(); Itr != m_TempRowList.end(); ++Itr)
		m_RowList.push_back(*Itr);
	m_TempRowList.clear();
}


void UIChatPanel::AddStringW(PCSTRINGW pString)
{
	RealAddStringW(pString, true);

	for (std::deque<UIChatRow *>::iterator Itr = m_TempRowList.begin(); Itr != m_TempRowList.end(); ++Itr)
		m_RowList.push_back(*Itr);
	m_TempRowList.clear();
}


void UIChatPanel::RealAddStringW(PCSTRINGW pString, bool PrefixFlag)
{
	bool	SelFlag = false;
	bool	UnderLineFlag = false;
	UInt32	PitchHeight, RealFontHeight;
	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	UIChatRow	*RowPtr = NULL;
	m_TempRowList.clear();

	RowPtr = SafeCreateObject(UIChatRow);
	m_TempRowList.push_back(RowPtr);
	//ClearOverLine();

	UInt32		X = 0;
	Color32 OrgColor = m_FontColor;
	Color32 LastColor = m_FontColor;

	if(PrefixFlag)
	{
		if (gConfigure.strMsgDel.find(pString[0]) == 0)
		{
			for (Int i = 0; i < MAX_CHANNELTYPE ; i++)
			{
				if (gConfigure.strChannelMsgCom[i].find(pString[1]) == 0)
				{
					//if (i == CHANNELTYPE_SYSTEM)
					//	m_FontColor = RedColor;
					
					UIImage * pBu = gGlobal.pUIManager->FindUIImage(ChannelBkXMLName[i]);

					if (pBu)
					{
						UIChatIcon *ChatIcon = SafeCreateObject(UIChatIcon);

						ChatIcon->SetImage(pBu->GetImage());
						ChatIcon->SetRect(pBu->GetBackRect()->left, pBu->GetBackRect()->top, 
							pBu->GetBackRect()->right, pBu->GetBackRect()->bottom);
						ChatIcon->SetPosX(X);
						X += pBu->GetWidth();
						RowPtr->AddItem(ChatIcon);
					}

					pString += 2;
					break;
				}
			}	
		}
	}

	StringW LinkStr = "";
	StringW TempStr = "";
	while(*pString)
	{
		StringW WStr;
		UInt32 Len;
		Int32	Ret = m_Font->DrawFontStrC(pString, WStr, X, 0, m_BufImage->GetWidth(), m_BufImage->GetPitch(), m_FontStyle, Len, gConfigure.Wrap);
		UIChatString *MyStr;
		UIChatLink *MyLink;
		D3DCOLOR	TempColor = m_FontColor;
		
		if(LinkStr.IsEmpty())
			MyStr = SafeCreateObject(UIChatString);
		else
		{
			MyLink = SafeCreateObject(UIChatLink);
			MyLink->SetLinkString(LinkStr.c_str());

			String	LinkTag = LinkStr.substr(0, 5);

			if(LinkTag == _T("NAME:"))
			{
				MyLink->SetLinkType(UIChatLink::PlayerName);
			}
			else
			if(LinkTag == _T("ITEM:"))
			{
				MyLink->SetLinkType(UIChatLink::Item);
				m_FontColor = TextColor[ColorIndex_Violet];
			}
			else
			if(LinkTag == _T("MENU:"))
			{
				MyLink->SetLinkType(UIChatLink::PopMenu);
			}
			else
			if(LinkTag == _T("POSI:"))
			{
				MyLink->SetLinkType(UIChatLink::Postion);
			}
			else
			if(LinkTag == _T("PTNR:"))
			{
				MyLink->SetLinkType(UIChatLink::Partner);
				m_FontColor = TextColor[ColorIndex_Violet];
			}
			else
			if(LinkTag == _T("PETS:"))
			{
				MyLink->SetLinkType(UIChatLink::Pet);
				m_FontColor = TextColor[ColorIndex_Violet];
			}
			else
			{
				MyLink->SetLinkType(UIChatLink::None);
				m_FontColor = 0xFF7F7F7F;
			}

			if(WStr[(Int)WStr.size() - 1] == _T(']'))
				LinkStr.clear();
			MyStr = MyLink;
		}
		
		UInt	TempX = X;
		MyStr->SetWidth(m_Font->GetLastDrawWidth() - X);
		X = m_Font->GetLastDrawWidth();
		pString += Len;

		MyStr->SetString(WStr.c_str());

		if(TempX != 0 || MyStr->GetString()[0] != _T(' ') || !gConfigure.Wrap)
		{
			MyStr->SetPosX(TempX);
			MyStr->SetFontColor(m_FontColor);

			if(SelFlag)
			{
				MyStr->SetSelColor(m_SelColor);
				MyStr->SetSelWidth(1);
				MyStr->SetSelHeight(m_Font->GetHeight() + PitchHeight + 2);
			}

			m_FontColor = TempColor;
			MyStr->SetShadowColor(m_ShadowColor);
			MyStr->SetFontStyle(m_FontStyle);
			MyStr->SetHeight(m_Font->GetHeight() + PitchHeight);
			MyStr->SetUnderLine(UnderLineFlag);
			UnderLineFlag = false;
			
			RowPtr->AddItem(MyStr);
		}
		else
		{
			SafeDeleteObject(MyStr);
		}
		
		if(Ret == -1)
		{
			RowPtr = SafeCreateObject(UIChatRow);
			m_TempRowList.push_back(RowPtr);
			//ClearOverLine();
			X = 0;
			if(pString[0] == _T('\\') && pString[1] == _T('r'))
			{
				pString += 2;
			}
			else
			if(pString[0] == _T('\r'))
			{
				++pString;
			}
		}

		while(pString[0] == _T('\r'))
		{
			++pString;
			RowPtr->SetEndLine(true);
			RowPtr = SafeCreateObject(UIChatRow);
			m_TempRowList.push_back(RowPtr);
			LinkStr.clear();
			ClearOverLine();
			X = 0;
		}

		while(pString[0] == _T('#') && pString[0])
		{
			if(pString[1] >= _T('0') && pString[1] <= _T('9'))
			{
				UInt32 Code = pString[1] - _T('0');
				pString += 2;

				if(pString[0] >= _T('0') && pString[0] <= _T('9'))
				{
					Code =  Code * 10 + (pString[0] - _T('0'));
					++pString;

					if(pString[0] >= _T('0') && pString[0] <= _T('9'))
					{
						Code =  Code * 10 + (pString[0] - _T('0'));
						++pString;
					}
				}

				if(Code > 0 && Code < 256)
				{
					UIChatAniIcon * AniChatIcon =  SafeCreateObject(UIChatAniIcon);
					
					AniChatIcon->SetAnimationData(gGlobal.GetChatIconFilePathbyNum(Code).c_str());

					if(X + AniChatIcon->GetWidth() >= m_BufImage->GetWidth())
					{
						RowPtr = SafeCreateObject(UIChatRow);
						m_TempRowList.push_back(RowPtr);
						//ClearOverLine();
						X = 0;
					}
					AniChatIcon->SetPosX(X);
					X += AniChatIcon->GetWidth();
					
					AniChatIcon->SetLinkString(gGlobal.GetChatIconCodebyNum(Code).c_str());
					RowPtr->AddItem(AniChatIcon);

				}
			}
			else if(pString[1] == _T('#'))
			{
				break;
			}
			else if(pString[1] == _T(':'))
			{
				pString += 2;
				break;
			}
			else if(pString[1] == _T('<'))
			{
				LastColor = m_FontColor;
				pString += 2;
				break;
			}
			else if(pString[1] == _T('>'))
			{
				m_FontColor = LastColor;
				pString += 2;
				break;
			}
			else
			{
				BOOL Found = FALSE;
				for (UInt i = 0; i < MaxTextColor; i++)
				{
					if (TextColorStr[i][0] == pString[1])
					{
						pString += 2;
						m_FontColor = TextColor[i];
						Found = TRUE;
						break;
					}
					else if (RedColorCmd.GetAt(0) == pString[1])
					{
						pString += 2;
						m_FontColor = RedColor;
						Found = TRUE;
						break;
					}
				}
				if (!Found)
				{
					++pString;
				}
			}
		}
		

		while(pString[0] == _T('\\'))
		{
			if(pString[1] == _T('['))
			{
				pString += 2;
				LinkStr.clear();
				while(pString[0])
				{
					if(pString[0] == _T('\\'))
					{
						if(pString[1] == _T(']'))
						{
							pString += 2;
							break;
						}
						else
						{
							LinkStr += pString[0];
							++pString;
						}
					}
					else
					{
						LinkStr += pString[0];
						++pString;
					}
				}
			}
			else
			if(pString[1] == _T('r'))
			{
				pString += 2;
				RowPtr->SetEndLine(true);
				RowPtr = SafeCreateObject(UIChatRow);
				m_TempRowList.push_back(RowPtr);
				LinkStr.clear();
				ClearOverLine();
				X = 0;
			}
			else
			if(pString[1] == _T('c'))
			{
				pString += 2;

				if(pString[0] >= _T('0') && pString[0] <= _T('9'))
				{
					UInt	Index = pString[0] - _T('0');
					++pString;
					if(pString[0] >= _T('0') && pString[0] <= _T('9'))
					{
						Index = Index * 10 + pString[0] - _T('0');
						++pString;
					}
					m_FontColor = FontColor[Index];
				}
			}
			else
			if(pString[1] == _T('C'))
			{
				pString += 2;

				if(pString[0] >= _T('0') && pString[0] <= _T('9'))
				{
					UInt	Index = pString[0] - _T('0');
					++pString;
					if(pString[0] >= _T('0') && pString[0] <= _T('9'))
					{
						Index = Index * 10 + pString[0] - _T('0');
						++pString;
					}
					m_SelColor = FontColor[Index];
				}

			}
			else
			if(pString[1] == _T('T'))
			{
				pString += 2;

				if(pString[0] >= _T('0') && pString[0] <= _T('9'))
				{
					X = pString[0] - _T('0');
					++pString;
					if(pString[0] >= _T('0') && pString[0] <= _T('9'))
					{
						X = X * 10 + pString[0] - _T('0');
						++pString;
						if(pString[0] >= _T('0') && pString[0] <= _T('9'))
						{
							X = X * 10 + pString[0] - _T('0');
							++pString;
						}
					}
				}
			}
			else
			if(pString[1] == _T('u'))
			{
				pString += 2;

				UnderLineFlag = true;
			}
			else
			if(pString[1] == _T('U'))
			{
				pString += 2;
			}
			else
			if(pString[1] == _T('i'))
			{
				pString += 2;

				UInt32 ItemID = 0, i = 0;

				if(pString[0] >= _T('0') && pString[0] <= _T('9'))
				{
					ItemID = pString[0] - _T('0');
					++pString;
					++i;
				}

				while(pString[0] >= _T('0') && pString[0] <= _T('9') && i < 8)
				{
					ItemID *= 10;
					ItemID += pString[0] - _T('0');
					++pString;
					++i;
				}

				if(ItemID != 0)
				{
					

				}
			}
			else
			if(pString[1] == _T('('))
			{
				pString += 2;
				SelFlag = true;
				break;
			}
			else
			if(pString[1] == _T(')'))
			{
				pString += 2;
				SelFlag = false;
				break;
			}
			else
			if(pString[1] == _T('\\'))
			{
				break;
			}
			else
			{
				++pString;
			}
		}
	}

	RowPtr->SetEndLine(true);

	m_FontColor = OrgColor;
}


void UIChatPanel::AddStringA(PCSTRINGA pString)
{
	AddStringW((PCSTRINGW)pString);
}


///////////////////////////////////////////////////////////////////////


UIChatEditPanel::UIChatEditPanel() : UIChatPanel()
{
	m_CursorX = -CursorStartX;
	m_CursorY = m_InsertPos = 0;
	m_DrawWidth = 0;
	m_CurLine = -1;
	m_EditItem = NULL;
	m_EditRow = NULL;
	m_EditString = NULL;
	m_Limit = 0, m_StringSize = 0;

	m_ShowEditCursor = true;
	m_EditCursorRefreshTime = 0;
	m_ShiftDown = false;

	m_SelNow = false;
	m_SelStartLine = m_SelStartPos = m_SelEndLine = m_SelEndPos = m_SelLeft = m_SelRight = 0;
	m_TInsertPos = 0;

	m_UseEnter = false;
}


void UIChatEditPanel::RenderSel(CImage *pImage)
{
	Int	Spacing = (m_LinePitch + m_Font->GetHeight());

	Int		SelStartLine, SelStartPos;
	Int		SelEndLine, SelEndPos;
	Int		SelLeft,  SelRight;
	
	if(m_SelStartLine > m_SelEndLine || (m_SelStartLine == m_SelEndLine && m_SelStartPos > m_SelEndPos))
	{
		SelStartLine = m_SelEndLine; 
		SelStartPos = m_SelEndPos;
		SelEndLine = m_SelStartLine; 
		SelEndPos = m_SelStartPos;
		SelLeft = m_SelRight;  
		SelRight = m_SelLeft;
	}
	else
	{
		SelStartLine = m_SelStartLine; 
		SelStartPos = m_SelStartPos;
		SelEndLine = m_SelEndLine; 
		SelEndPos = m_SelEndPos;
		SelLeft = m_SelLeft;  
		SelRight = m_SelRight;
	}


	if(SelStartLine != SelEndLine)
	{
		Int		Y = m_OffsetY + SelStartLine * Spacing;
		Int		i = SelStartLine;
		
		RECT	rect;
		SetRect(&rect, m_OffsetX + SelLeft, Y, m_OffsetX + m_RowList[i]->GetWidth(), Y + Spacing);
		pImage->FillUInt32(0xFFFF0000,  &rect);
		Y += Spacing;
		for(++i; i < SelEndLine; ++i)
		{
			SetRect(&rect, m_OffsetX, Y, m_OffsetX + m_RowList[i]->GetWidth(), Y + Spacing);
			pImage->FillUInt32(0xFFFF0000,  &rect);
			Y += Spacing;
		}
		SetRect(&rect, m_OffsetX, Y, m_OffsetX + SelRight, Y + Spacing);
		pImage->FillUInt32(0xFFFF0000,  &rect);

	}
	else
	{
		Int		Y = m_OffsetY + SelStartLine * Spacing;

		RECT	rect;
		SetRect(&rect, m_OffsetX + SelLeft, Y, m_OffsetX + SelRight, Y + Spacing);
		pImage->FillUInt32(0xFFFF0000,  &rect);
	}

}


void UIChatEditPanel::Render(CImage *pImage)
{
	if(m_SelNow && (m_SelStartPos != m_SelEndPos || m_SelStartLine != m_SelEndLine))
		RenderSel(pImage);
	UIChatPanel::Render(pImage);

	if(m_pUIManager->GetKeyInItem() == this ||
		m_pUIManager->GetFocusItem() == this)
	{
		RECT	SRect = {0, 0, 8, 12};

		if (m_ShowEditCursor)
			Alpha8888To8888S(m_OffsetX + m_CursorX, m_OffsetY + m_CursorY, 	g_EditCursor, pImage, &SRect, m_RenderStyle, m_Color);
	}
}


void UIChatEditPanel::RenderSel(CImage *pImage, RECT &RRect)
{
	Int	Spacing = (m_LinePitch + m_Font->GetHeight());

	Int		SelStartLine, SelStartPos;
	Int		SelEndLine, SelEndPos;
	Int		SelLeft,  SelRight;
	
	if(m_SelStartLine > m_SelEndLine || (m_SelStartLine == m_SelEndLine && m_SelStartPos > m_SelEndPos))
	{
		SelStartLine = m_SelEndLine; 
		SelStartPos = m_SelEndPos;
		SelEndLine = m_SelStartLine; 
		SelEndPos = m_SelStartPos;
		SelLeft = m_SelRight;  
		SelRight = m_SelLeft;
	}
	else
	{
		SelStartLine = m_SelStartLine; 
		SelStartPos = m_SelStartPos;
		SelEndLine = m_SelEndLine; 
		SelEndPos = m_SelEndPos;
		SelLeft = m_SelLeft;  
		SelRight = m_SelRight;
	}


	if(SelStartLine != SelEndLine)
	{
		Int		i = SelStartLine;
		Int		Y = m_OffsetY + i * Spacing;
		
		RECT	rect;
		Int		X = m_OffsetX + SelLeft;
		Int		Width = m_OffsetX + m_Width;
		SetRect(&rect, X, Y, Width, Y + Spacing);

		if(CImageDirtyRect(X, Y, rect, RRect))
			pImage->FillUInt32(0xFFFF0000,  &rect);
		
		for(++i; i < SelEndLine; ++i)
		{
			X = m_OffsetX;
			Y = m_OffsetY + i * Spacing;
			SetRect(&rect, X, Y, Width, Y + Spacing);
			if(CImageDirtyRect(X, Y, rect, RRect))
				pImage->FillUInt32(0xFFFF0000,  &rect);
		}
		X = m_OffsetX;
		Y = m_OffsetY + i * Spacing;
		if(CImageDirtyRect(X, Y, rect, RRect))
			SetRect(&rect, X, Y, m_OffsetX + SelRight, Y + Spacing);
		pImage->FillUInt32(0xFFFF0000,  &rect);

	}
	else
	{
		Int		Y = m_OffsetY + SelStartLine * Spacing;
		Int		X = m_OffsetX + SelLeft;

		RECT	rect;
		SetRect(&rect, X, Y, m_OffsetX + SelRight, Y + Spacing);
		if(CImageDirtyRect(X, Y, rect, RRect))
			pImage->FillUInt32(0xFFFF0000,  &rect);
	}

}


void UIChatEditPanel::Render(CImage *pImage, RECT &RRect)
{
	if(m_SelNow && (m_SelStartPos != m_SelEndPos || m_SelStartLine != m_SelEndLine))
		RenderSel(pImage, RRect);
		
	UIChatPanel::Render(pImage, RRect);

	if(m_pUIManager->GetKeyInItem() == this ||
		m_pUIManager->GetFocusItem() == this)
	{
		RECT	SRect = {0, 0, 8, 12};
		
		if (m_ShowEditCursor)
			Alpha8888To8888S(m_OffsetX + m_CursorX, m_OffsetY + m_CursorY, 	g_EditCursor, pImage, &SRect, m_RenderStyle, m_Color);
	}
}


void UIChatEditPanel::UpdateDetectRect()
{
	for(UInt32 i = 0; i < m_RowList.size(); ++i)
	{
		if(m_RowList[i])
			m_RowList[i]->UpdateDetectRect(m_X, m_Y);
	}
}


UInt32 UIChatEditPanel::Update(UInt32 dwDelta)
{
	if(m_Focus)
	{
		if (::timeGetTime() > EditCursorRefreshCount + m_EditCursorRefreshTime)
		{
			m_ShowEditCursor = !m_ShowEditCursor;
			RenderParent();
			m_EditCursorRefreshTime = ::timeGetTime();
			return 1;
		}
	}
	return 0;
}


Int UIChatEditPanel::FindCursorPos(Int32 pX, Int32 pY)
{
	Int	Spacing = m_LinePitch + m_Font->GetHeight();
	Int Y = (pY - m_DetectRect.top) / Spacing;
	if(Y < (Int)m_RowList.size())
	{
		SetCurLine(Y);
		m_InsertPos = 0;
		m_CursorX = -CursorStartX;
		m_CursorY = Y * Spacing;

		Int Size = (Int)m_EditString->GetStringRef().size();
		Int X = pX - m_DetectRect.left;
		Int OffsetX = X - m_CursorX;
		Int TCursorX = m_CursorX;
		Int TInsertPos = m_InsertPos;

		for(Int i  = 0; i < Size; ++i)
		{
			CursorMoveRight();
			Int TOffsetX = X - m_CursorX;
			if(abs(TOffsetX) < OffsetX)
			{
				OffsetX = TOffsetX;
				TCursorX = m_CursorX;
				TInsertPos = m_InsertPos;
			}
		}
		m_CursorX = TCursorX;
		m_InsertPos = TInsertPos;

		return 1;
	}

	return 0;
}


Int32 UIChatEditPanel::OnMouseMove(Int32 pX, Int32 pY)
{
	bool ret = UIItem::Detect(pX, pY);

	if(ret && m_PickUp)
	{
		if(FindCursorPos(pX, pY))
			PowerSetEndSel();
	}
	return 0;
}


Int32 UIChatEditPanel::OnLButtonDown(Int32 pX, Int32 pY)
{
	m_ClickItem = NULL;
	if(!UIItem::Detect(pX,  pY))
		return 0;
	
	if(FindCursorPos(pX, pY))
	{
		PowerSetStartSel();
		m_PickUp = true;
	}

	if(m_pUIManager)
	{
		m_pUIManager->SetKeyInItem(this);
	}

	return 1;
}


void UIChatEditPanel::SetImage(UInt32 pWidth, UInt32 pHeight)
{
	UIChatPanel::SetImage(pWidth, pHeight);
	m_DrawWidth = (pWidth > 5 ? pWidth - 5 : 0);
}


void UIChatEditPanel::SetCurLine(Int Index)
{
	m_CurLine = Index;
	m_EditRow = m_RowList[m_CurLine];
	m_EditItem = m_EditRow->GetChatItem(0);
	m_EditString = (UIChatString *)m_EditItem;
}


void UIChatEditPanel::SetNoCurLine()
{
	m_CurLine = -1;
	m_EditRow = NULL;
	m_EditItem = NULL;
	m_EditString = NULL;
}


void UIChatEditPanel::CursorMoveLeft()
{
	if(m_RowList.size())
	{
		if(m_InsertPos > 0)
		{
			--m_InsertPos;
			Int Width = m_Font->GetCharWidth(m_EditString->GetStringRef().at(m_InsertPos));
			m_CursorX -= Width;
		}
		else
		{
			if(m_CurLine > 0)
			{
				SetCurLine(m_CurLine - 1);
				m_InsertPos = (Int32)m_EditString->GetStringRef().size();

				m_CursorY -= m_Font->GetHeight() + m_LinePitch;
				m_CursorX = m_EditRow->GetWidth() - CursorStartX;
			}
		}
	}
}


void UIChatEditPanel::CursorMoveRight()
{
	if(m_RowList.size())
	{
		if(m_InsertPos < (Int)m_EditString->GetStringRef().size())
		{
			
			Int Width = m_Font->GetCharWidth(m_EditString->GetStringRef().at(m_InsertPos));
			++m_InsertPos;
			m_CursorX += Width;
		}
		else
		{
			if(m_CurLine < (Int32)m_RowList.size() - 1)
			{
				SetCurLine(m_CurLine + 1);
				m_InsertPos = 0;

				m_CursorY += m_Font->GetHeight() + m_LinePitch;
				m_CursorX = -CursorStartX;
			}
		}
	}
}


void UIChatEditPanel::CursorMoveUp()
{
	if(m_RowList.size())
	{
		if(m_CurLine > 0)
		{
			UIChatRow *TempRow = m_EditRow;
			Int TempX = m_CursorX;

			SetCurLine(m_CurLine - 1);
			m_InsertPos = (Int32)m_EditString->GetStringRef().size();

			m_CursorY -= m_Font->GetHeight() + m_LinePitch;
			m_CursorX = m_EditRow->GetWidth() - CursorStartX;

			String &EditString = m_EditString->GetStringRef();
			Int InsertPos = m_InsertPos;
			Int X = -CursorStartX, RX = m_CursorX;
			Int OffsetX = abs(TempX - m_CursorX);
			for(Int i = 0; i < m_InsertPos; ++i)
			{
				if(abs(TempX - X) < OffsetX)
				{
					InsertPos = i;
					OffsetX = abs(TempX - X);
					RX = X;
				}
				X += m_Font->GetCharWidth(EditString[i]);
			}

			m_CursorX = RX;
			m_InsertPos = InsertPos;
		}
	}
}


void UIChatEditPanel::CursorMoveDown()
{
	if(m_RowList.size())
	{
		if(m_CurLine < (Int)m_RowList.size() - 1)
		{
			UIChatRow *TempRow = m_EditRow;
			Int TempX = m_CursorX;

			SetCurLine(m_CurLine + 1);
			m_InsertPos = (Int32)m_EditString->GetStringRef().size();

			m_CursorY += m_Font->GetHeight() + m_LinePitch;
			m_CursorX = m_EditRow->GetWidth() - CursorStartX;

			String &EditString = m_EditString->GetStringRef();
			Int InsertPos = m_InsertPos;
			Int X = -CursorStartX, RX = m_CursorX;
			Int OffsetX = abs(TempX - m_CursorX);
			for(Int i = 0; i < m_InsertPos; ++i)
			{
				if(abs(TempX - X) < OffsetX)
				{
					InsertPos = i;
					OffsetX = abs(TempX - X);
					RX = X;
				}
				X += m_Font->GetCharWidth(EditString[i]);
			}

			m_CursorX = RX;
			m_InsertPos = InsertPos;
		}
	}
}


UInt UIChatEditPanel::DelChar()
{
	Int Spacing = m_Font->GetHeight() + m_LinePitch;

	if(m_RowList.size())
	{
		if(m_EditString->GetStringRef().empty())
		{
			if(m_CurLine < (Int)m_RowList.size() - 1)
			{
				MoveArea(0, m_CursorY, 0, m_CursorY + Spacing, m_DrawWidth, 
					Spacing * ((Int)m_RowList.size() - m_CurLine - 1));
				FillImage(0, m_DrawWidth, ((Int)m_RowList.size() - 1) * Spacing);
				SafeDeleteObject(m_RowList[m_CurLine]);
				m_RowList.erase(m_RowList.begin() + m_CurLine);
				SetCurLine(m_CurLine);
				m_StartRow = 0;
				m_EndRow = (Int)m_RowList.size() - 1;
				--m_StringSize;
			}

			return 1;
		}
		else
		{
			Int	Line, Width, CursorX, RowWidth, CWidth, Y, InsertPos;
			UIChatString *EditString;
			bool DelFlag = true;
			if(m_InsertPos < (Int32)m_EditString->GetStringRef().size())
			{
				Line = m_CurLine;
				CursorX = m_CursorX + CursorStartX;
				Y = m_CursorY;
				InsertPos = m_InsertPos;
				--m_StringSize;
			}
			else
			{
				if(m_CurLine  < (Int)m_RowList.size() - 1)
				{
					if(m_RowList[m_CurLine + 1]->GetWidth() == 0)
					{
						Line = m_CurLine;
						MoveArea(0, (Line + 1) * Spacing, 0, (Line + 2) * Spacing, m_DrawWidth, 
							Spacing * ((Int)m_RowList.size() - Line - 2));
						FillImage(0, m_DrawWidth, ((Int)m_RowList.size() - 1) * Spacing);
						SafeDeleteObject(m_RowList[Line + 1]);
						m_RowList.erase(m_RowList.begin() + Line + 1);
						m_EditRow->SetEndLine(true);
						--m_StringSize;

						return 1;
					}

					if(m_EditRow->GetEndLine())
					{
						m_EditRow->SetEndLine(false);
						Line = m_CurLine;
						CursorX = m_CursorX + CursorStartX;
						Y = m_CursorY;
						InsertPos = m_InsertPos;
						Width = 0;
						DelFlag = false;
						RowWidth = m_RowList[Line]->GetWidth();
						CWidth = m_DrawWidth - RowWidth;
						--m_StringSize;
					}
					else
					{
						Line = m_CurLine + 1;
						CursorX = 0;
						Y = m_CursorY + Spacing;
						InsertPos = 0;
						--m_StringSize;
					}
				}
				else
					return 1;
			}
				
			EditString = (UIChatString *)(m_RowList[Line]->GetChatItem(0));
			if(EditString->GetStringRef().size() == 0)
			{
				return 1;
			}

			if(DelFlag)
			{
				Width = m_Font->GetCharWidth(EditString->GetStringRef().at(InsertPos));
				RowWidth = m_RowList[Line]->GetWidth() - Width;
				CWidth = m_DrawWidth - RowWidth;

				MoveLine(CursorX, Y, CursorX + Width, Y, m_RowList[Line]->GetWidth() - (CursorX + Width));
				FillImage(RowWidth, m_RowList[Line]->GetWidth(), Y);
				m_RowList[Line]->SetWidth(RowWidth);
				EditString->GetStringRef().Delete(InsertPos, 1);
			}
			
			while(Line < (Int)m_RowList.size() - 1 && !m_RowList[Line]->GetEndLine())
			{
				String *AddString = ((UIChatString *)(m_RowList[Line]->GetChatItem(0)))->GetStringPtr();
				String *DelString = ((UIChatString *)(m_RowList[Line + 1]->GetChatItem(0)))->GetStringPtr();
				if(DelString->size())
				{
					UInt	TWidth = 0, Count;
					for(Count = 0; Count < DelString->size(); ++Count)
					{
						Int EWidth = m_Font->GetCharWidth(DelString->at(Count));

						if(CWidth >= EWidth)
						{
							CWidth -= EWidth;
							TWidth += EWidth;
						}
						else
							break;
					}

					
					if(Count)
					{
						Int Y = Line * Spacing, Y2 = Y + Spacing;
						MoveLine(m_RowList[Line]->GetWidth(), Y, 0, Y2, TWidth);
						m_RowList[Line]->AddWidth(TWidth);
						Int SubWidth = m_RowList[Line + 1]->GetWidth() - TWidth;
						MoveLine(0, Y2, TWidth, Y2, SubWidth);
						FillImage(SubWidth, SubWidth + TWidth, Y2);
						m_RowList[Line + 1]->SetWidth(SubWidth);

						CWidth = m_DrawWidth - m_RowList[Line + 1]->GetWidth();

						AddString->append(DelString->substr(0, Count));
						DelString->Delete(0, Count);

					}
					else
						break;
				}
				
				if(DelString->size() == 0)
				{
					MoveArea(0, Y + Spacing, 0, Y + Spacing * 2, m_DrawWidth, 
							Spacing * ((Int)m_RowList.size() - Line - 2));
					FillImage(0, m_DrawWidth, ((Int)m_RowList.size() - 1) * Spacing);
					SafeDeleteObject(m_RowList[Line + 1]);
					m_RowList.erase(m_RowList.begin() + Line + 1);
					m_RowList[Line]->SetEndLine(true);
					m_StartRow = 0;
					m_EndRow = (Int)m_RowList.size() - 1;
					break;
				}
				++Line;
			}

			
			//RenderParent();
			//UpdateDetectRect();
		}
	}

	return 0;
}



void UIChatEditPanel::UnPickUp()
{
	UIItem::UnPickUp();
	if(m_SelNow)
	{
		if(m_SelStartLine == m_SelEndLine && m_SelStartPos == m_SelEndPos)
			m_SelNow = false;
	}
}


void UIChatEditPanel::SetFocus(bool pState)
{
	if (pState != m_Focus)
		RenderParent();
	m_Focus = pState;
	if(pState == false)
	{
		m_ClickItem = NULL;	
		m_SelStart = m_SelEnd = 0;
		DrawFont();
	}
}


void UIChatEditPanel::SetStartSel()
{
	if(m_ShiftDown)
	{
		if(!m_SelNow && m_RowList.size())
		{
			m_SelNow = true;
			m_SelStartLine = m_SelEndLine = m_CurLine;
			m_SelStartPos = m_SelEndPos = m_InsertPos;
			m_SelLeft = m_SelRight = m_CursorX + CursorStartX;
		}
	}
	else
		m_SelNow =  false;
}



void UIChatEditPanel::PowerSetStartSel()
{
	if(m_RowList.size())
	{
		m_SelNow = true;
		m_SelStartLine = m_SelEndLine = m_CurLine;
		m_SelStartPos = m_SelEndPos = m_InsertPos;
		m_SelLeft = m_SelRight = m_CursorX + CursorStartX;
	}
}


void UIChatEditPanel::SetEndSel()
{
	if(m_ShiftDown && m_SelNow && m_RowList.size())
	{
		m_SelEndLine = m_CurLine;
		m_SelEndPos = m_InsertPos;
		m_SelRight = m_CursorX + CursorStartX;
		SetChange(true);
	}
}


void UIChatEditPanel::PowerSetEndSel()
{
	if(m_RowList.size())
	{
		m_SelEndLine = m_CurLine;
		m_SelEndPos = m_InsertPos;
		m_SelRight = m_CursorX + CursorStartX;
		SetChange(true);
	}
}



Int32 UIChatEditPanel::OnKeyDown(WPARAM wParam)
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	m_ShiftDown = ((GetKeyState(VK_LSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 ||
		(GetKeyState(VK_RSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0);

	switch(wParam)
	{
	case VK_LEFT:
		SetStartSel();
		CursorMoveLeft();
		SetEndSel();
		break;
	case VK_RIGHT:
		SetStartSel();
		CursorMoveRight();
		SetEndSel();
		break;
	case VK_UP:
		SetStartSel();
		CursorMoveUp();
		SetEndSel();
		break;
	case VK_DOWN:
		SetStartSel();
		CursorMoveDown();
		SetEndSel();
		break;
	case VK_HOME:
		{
			SetStartSel();
			if(m_RowList.size())
			{
				if(gGlobal.g_bCtrlButtonUp)
				{
					m_CursorX = -CursorStartX;
					m_InsertPos = 0;				
				}
				else
				{
					m_CursorX = -CursorStartX;
					m_CursorY = m_InsertPos = 0;
					SetCurLine(0);
				}
			}
			SetEndSel();
		}
		break;
	case VK_END:
		{
			SetStartSel();
			if(m_RowList.size())
			{
				if(gGlobal.g_bCtrlButtonUp)
				{
					m_CursorX = m_EditRow->GetWidth() - CursorStartX;
					m_InsertPos = (Int)m_EditString->GetStringPtr()->size();				
				}
				else
				{
					SetCurLine((Int)m_RowList.size() - 1);
					m_CursorX = m_EditRow->GetWidth() - CursorStartX;
					m_CursorY = m_CurLine * (m_LinePitch + m_Font->GetHeight());
					m_InsertPos = (Int)m_EditString->GetStringPtr()->size();
					
				}
			}
			SetEndSel();
		}
		break;
	case VK_DELETE:
		if(m_SelNow)
		{
			DelSelText();
		}
		else
			DelChar();
		break;		
	default:
		m_ShiftDown = false;
		return 0;
		break;
	}

	m_ShiftDown = false;

	return 1;
}


void UIChatEditPanel::ResetAll()
{
	ClearAllRow();

	m_CursorX = -CursorStartX;
	m_CursorY = m_InsertPos = 0;
	m_CurLine = -1;
	m_EditItem = NULL;
	m_EditRow = NULL;
	m_EditString = NULL;

	m_ShowEditCursor = true;
	m_EditCursorRefreshTime = 0;
	m_ShiftDown = false;

	m_SelNow = false;
	m_SelStartLine = m_SelStartPos = m_SelEndLine = m_SelEndPos = m_SelLeft = m_SelRight = 0;
	m_TInsertPos = 0;
	m_StringSize = 0;
}


Int32 UIChatEditPanel::OnChar(WPARAM wParam)
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	m_LastKey = 0;

	switch(wParam)
	{
		case VK_BACK:
			{
				if(m_SelNow)
				{
					DelSelText();
				}
				else
				if(m_InsertPos || m_CurLine)
				{
					Int CurLine = m_CurLine;

					CursorMoveLeft();

					if(CurLine > m_CurLine)
					{
						if(!m_EditRow->GetEndLine())
							CursorMoveLeft();
					}						
					DelChar();
					/*
					if(Len && Len == m_EditString->GetStringPtr()->size())
					{
						CursorMoveLeft();
						DelChar();
					}
					*/
				}
			}
			break;
		case VK_RETURN:			//
			{
				m_SelNow = false;
				m_SelStartLine = m_SelStartPos = m_SelEndLine = m_SelEndPos = m_SelLeft = m_SelRight = 0;
				m_TInsertPos = 0;

				if(m_UseEnter)
					InsertReturn();
			}
			break;
			
		case VK_TAB:
			break;//
			
		case 1:					// Ctrl-A Select All
			{
				//ResetAll();
				if(m_RowList.size() && m_StringSize)
				{
					m_SelNow = true;
					m_SelStartLine = 0;
					m_SelLeft = 0;
					m_SelStartPos = 0;

					SetCurLine((Int)m_RowList.size() - 1);
					m_CursorX = m_EditRow->GetWidth() - CursorStartX;
					m_CursorY = m_CurLine * (m_LinePitch + m_Font->GetHeight());
					m_InsertPos = (Int)m_EditString->GetStringPtr()->size();

					m_SelEndLine = m_CurLine;
					m_SelEndPos = m_InsertPos;
					m_SelRight = m_CursorX + CursorStartX;
				}
			}
			break;
        case 22:				// Ctrl-V Paste
			{
				if(m_SelNow)
					SaveNoSelText();
				else
					SaveText();

				if(InsertClipBoard())
				{
					if(TestRedraw())
					{
						LoadText();
						m_BufImage->FillUInt32();
						Redraw();
						SetCurLine(m_TCurLine);
						m_CursorX = m_TCursorX - CursorStartX;
						m_CursorY = m_TCursorY;
						m_InsertPos = m_TInsertPos;

						m_SelNow = false;
					}
				}
			}
			break;			
		case 24:				// Ctrl-X Cut
			SelText2ClipBoard();
			if(m_SelNow)
				DelSelText();
			break;
        case VK_CANCEL:			// Ctrl-C Copy
			SelText2ClipBoard();
			break;

        case 2:   // Ctrl B
		{
			SaveText();
			LoadText();
			m_BufImage->FillUInt32();
			Redraw();
			break;
		}
        case 4:   // Ctrl D
		case 5:   // Ctrl E
        case 6:   // Ctrl F
        case 7:   // Ctrl G
        //case 9:   // Ctrl I		tab
		case 10:  // Ctrl J
        case 11:  // Ctrl K
        case 12:  // Ctrl L
		case 14:  // Ctrl N
        case 15:  // Ctrl O
        case 16:  // Ctrl P
		case 17:  // Ctrl Q
		case 18:  // Ctrl R
		case 19:  // Ctrl S
        case 20:  // Ctrl T            
        case 21:  // Ctrl U
        case 23:  // Ctrl W
		case 25:  // Ctrl Y
		case 26:  // Ctrl Z
        case 27:  // Ctrl [
		case 28:  // Ctrl \ 
        case 29:  // Ctrl ]            
			break;
		default:
			if(m_StringSize < m_Limit || m_Limit==0) 
				return AddCharW((CharW)wParam);
			break;
	}

	return 1;
}


Int UIChatEditPanel::DrawString(String &S, Int X, Int Y)
{
	for(UInt i = 0; i < S.size(); ++i)
	{
		X = m_Font->DrawFontChar(S[i], m_BufImage->GetPtr(), X, Y, 
			m_BufImage->GetWidth(), m_BufImage->GetHeight(), m_BufImage->GetPitch(), m_FontStyle, 
			m_FontColor, m_ShadowColor);
	}

	return X;
}


UInt32 UIChatEditPanel::NewLine()
{
	UIChatRow	*RowPtr = NULL;

	RowPtr = SafeCreateObject(UIChatRow);
	m_RowList.push_back(RowPtr);

	UIChatString *MyStr;
	MyStr = SafeCreateObject(UIChatString);
	MyStr->SetFontColor(m_FontColor);
	MyStr->SetShadowColor(m_ShadowColor);
	MyStr->SetFontStyle(m_FontStyle);
	MyStr->SetHeight(m_Font->GetHeight() + m_LinePitch);
	
	RowPtr->AddItem(MyStr);
	RowPtr->SetEndLine(true);

	return 1;
}


UInt32 UIChatEditPanel::InsertLine(Int Line)
{
	UIChatRow	*RowPtr = NULL;

	if(Line <= (Int)m_RowList.size())
	{
		RowPtr = SafeCreateObject(UIChatRow);
		std::deque<UIChatRow *>::iterator	it = m_RowList.begin() + Line;
		m_RowList.insert(it, RowPtr);

		UIChatString *MyStr;
		MyStr = SafeCreateObject(UIChatString);
		MyStr->SetFontColor(m_FontColor);
		MyStr->SetShadowColor(m_ShadowColor);
		MyStr->SetFontStyle(m_FontStyle);
		MyStr->SetHeight(m_Font->GetHeight() + m_LinePitch);
		RowPtr->AddItem(MyStr);
		
		m_StartRow = 0;
		m_EndRow = (Int)m_RowList.size() - 1;
	}

	return 1;
}


UInt32 UIChatEditPanel::InsertReturn()
{
	if(m_RowList.size() == 0)
	{
		InsertLine(0);
		m_RowList[0]->SetEndLine(true);
		InsertLine(1);
		m_RowList[1]->SetEndLine(true);
		++m_StringSize;
		SetCurLine(1);
		m_CursorY = m_LinePitch + m_Font->GetHeight();
	}
	else
	if((Int)m_RowList.size() < m_MaxLine)
	{
		Int Spacing = m_LinePitch + m_Font->GetHeight();
		String *EditString = m_EditString->GetStringPtr();
		Int		Y = m_CurLine * Spacing;
		String	SubStr;
		Int	CWidth = 0, Count = 0;;
		//Int Index = 0;
		bool	EndLineFlag = m_RowList[m_CurLine]->GetEndLine();

		if(m_InsertPos == 0)
		{
			CWidth = m_EditRow->GetWidth();
			MoveArea(0, Y + Spacing, 0, Y, m_DrawWidth, Spacing * ((Int)m_RowList.size() - m_CurLine));
			FillImage(0, m_EditRow->GetWidth(), Y);
			m_EditRow->SetWidth(0);
			InsertLine(++m_CurLine);
			++m_StringSize;
			m_EditRow->SetEndLine(true);

			SetCurLine(m_CurLine);

			m_EditRow->SetEndLine(EndLineFlag);
			m_EditRow->SetWidth(CWidth);
			m_CursorY += Spacing;
			String *EditString2 = ((UIChatString *)(m_RowList[m_CurLine]->GetChatItem(0)))->GetStringPtr();
			
			EditString2->append(*EditString);
			EditString->clear();

			return 1;		
		}
		else
		if(m_InsertPos < (Int)EditString->size())
		{
			Int Index = 0;
			for(Index = (Int)EditString->size() - 1; Index > 0 && Index >= m_InsertPos; --Index, ++Count)
				CWidth += m_Font->GetCharWidth(EditString->at(Index));
			++Index;
			Int X = m_EditRow->GetWidth() - CWidth;

			SubStr = EditString->substr(Index, Count);
			EditString->Delete(Index, Count);
			
			if(m_CurLine < (Int)m_RowList.size() - 1)
			{
				MoveArea(0, Y + Spacing * 2, 0, Y + Spacing, m_DrawWidth, 
					Spacing * ((Int)m_RowList.size() - m_CurLine - 1));
				FillImage(0, m_DrawWidth, Y + Spacing);
			}
			MoveLine(0, Y + Spacing, X, Y, CWidth);
			FillImage(X, m_EditRow->GetWidth(), Y);
			m_EditRow->AddWidth(-CWidth);
		}
		else
		{
			CWidth = m_EditRow->GetWidth();
			MoveArea(0, Y + Spacing * 2, 0, Y + Spacing, m_DrawWidth, 
				Spacing * ((Int)m_RowList.size() - m_CurLine - 1));
			FillImage(0, m_DrawWidth, Y + Spacing);
			m_EditRow->SetEndLine(true);
			InsertLine(++m_CurLine);
			++m_StringSize;
			SetCurLine(m_CurLine);
			m_EditRow->SetEndLine(true);
			m_CursorX = -CursorStartX;
			m_CursorY += Spacing;
			m_InsertPos = 0;
		
			return 1;		
		}

		m_EditRow->SetEndLine(true);


		InsertLine(++m_CurLine);
		++m_StringSize;
		SetCurLine(m_CurLine);
		m_EditRow->SetEndLine(EndLineFlag);
		EditString = m_EditString->GetStringPtr();
		m_InsertPos = 0;
		if(Count)
		{
			m_EditRow->SetWidth(CWidth);
			EditString->append(SubStr);
		}
		else
			m_EditRow->SetWidth(0);
		m_CursorX = -CursorStartX;
		m_CursorY += Spacing;

		if(!EndLineFlag)
		{
			Int DWidth = m_DrawWidth - CWidth;
			Int OldCWidth = CWidth;
			Int Line = m_CurLine + 1;
			Int Y = Line * Spacing;
			for(Int i = Line; i < (Int)m_RowList.size(); ++i)
			{
				String *EditString2 = ((UIChatString *)(m_RowList[i]->GetChatItem(0)))->GetStringPtr();
				CWidth = 0;
				Int Count = 0;
				for(Count = 0; Count < (Int)EditString2->size(); ++Count)
				{
					Int Width = m_Font->GetCharWidth(EditString2->at(Count));
					if(CWidth + Width >= DWidth)
						break;					
					CWidth += Width;
				}	
				

				MoveLine(OldCWidth, Y - Spacing, 0, Y, CWidth);
				m_RowList[i - 1]->AddWidth(CWidth);
				MoveLine(0, Y, CWidth, Y, m_RowList[i]->GetWidth() - CWidth);
				FillImage(m_RowList[i]->GetWidth() - CWidth, m_RowList[i]->GetWidth(), Y);
				m_RowList[i]->AddWidth(-CWidth);
				if(Count)
				{
					EditString->append(EditString2->substr(0, Count));
					EditString2->Delete(0, Count);
				}
				EditString = EditString2;
				Y += Spacing;
				if(m_RowList[i]->GetEndLine())
				{
					if(EditString2->empty() && Count)
					{
						MoveArea(0, Y - Spacing, 0, Y, m_DrawWidth, 
							Spacing * ((Int)m_RowList.size() - i - 1));
						FillImage(0, m_DrawWidth, ((Int)m_RowList.size() - 1) * Spacing);
						m_RowList[i - 1]->SetEndLine(true);
						SafeDeleteObject(m_RowList[i]);
						m_RowList.erase(m_RowList.begin() + i);
						m_StartRow = 0;
						m_EndRow = (Int)m_RowList.size() - 1;
						
					}
					break;
				}
			}
		}
		
	}

	return 1;
}


Int UIChatEditPanel::DrawOnNewLine(CharW C)
{
	Int Width = DrawChar(C, 0, m_CursorY);

	NewLine();
	m_EditRow = m_RowList.back();
	m_EditString = (UIChatString *)(m_EditItem = m_EditRow->GetChatItem(0));
	
	m_EditString->GetStringRef().push_back(C);
	m_StringSize = m_InsertPos = 1;
	m_EditRow->SetWidth(Width);

	return Width;
}


Int UIChatEditPanel::DrawOnNewLine(String &S)
{
	Int Width = DrawString(S, 0, m_CursorY);

	NewLine();
	m_EditRow = m_RowList.back();
	m_EditString = (UIChatString *)(m_EditItem = m_EditRow->GetChatItem(0));
	
	m_EditString->GetStringRef().append(S);
	m_InsertPos = S.size();
	m_StringSize += S.size();
	m_EditRow->SetWidth(Width);

	return Width;
}


void UIChatEditPanel::MoveLine(Int DX, Int DY, Int SX, Int SY, Int Width)
{
	UInt	Len = m_Font->GetHeight() + m_LinePitch;
	UInt8	*DBufPtr	= m_BufImage->GetPtrXY(DX, DY);
	UInt8	*SBufPtr = m_BufImage->GetPtrXY(SX, SY);
	Int		MoveWidth = Width * 4;

	if(MoveWidth <= 0)
		return;

	if(SY > DY)
	{
		UInt8	*DBufPtr = m_BufImage->GetPtrXY(DX, DY);
		UInt8	*SBufPtr = m_BufImage->GetPtrXY(SX, SY);

		for(UInt i = 0; i < Len; ++i)
		{
			memmove(DBufPtr, SBufPtr, MoveWidth);
			DBufPtr += m_BufImage->GetPitch();
			SBufPtr += m_BufImage->GetPitch();
		}
	}
	else
	{
		UInt8	*DBufPtr = m_BufImage->GetPtrXY(DX, DY + Len - 1);
		UInt8	*SBufPtr = m_BufImage->GetPtrXY(SX, SY + Len - 1);
		for(UInt i = 0; i < Len; ++i)
		{
			memmove(DBufPtr, SBufPtr, MoveWidth);
			DBufPtr -= m_BufImage->GetPitch();
			SBufPtr -= m_BufImage->GetPitch();
		}
	}
}


void UIChatEditPanel::MoveArea(Int DX, Int DY, Int SX, Int SY, Int Width, Int Height)
{
	Int		MoveWidth = Width * 4;

	if(MoveWidth <= 0 || Height <= 0)
		return;

	if(SY > DY)
	{
		UInt8	*DBufPtr = m_BufImage->GetPtrXY(DX, DY);
		UInt8	*SBufPtr = m_BufImage->GetPtrXY(SX, SY);
		for(Int i = 0; i < (Int)Height; ++i)
		{
			memmove(DBufPtr, SBufPtr, MoveWidth);
			DBufPtr += m_BufImage->GetPitch();
			SBufPtr += m_BufImage->GetPitch();
		}
	}
	else
	{
		UInt8	*DBufPtr = m_BufImage->GetPtrXY(DX, DY + Height - 1);
		UInt8	*SBufPtr = m_BufImage->GetPtrXY(SX, SY + Height - 1);
		for(Int i = 0; i < Height; ++i)
		{
			memmove(DBufPtr, SBufPtr, MoveWidth);
			DBufPtr -= m_BufImage->GetPitch();
			SBufPtr -= m_BufImage->GetPitch();
		}
	}
}


void UIChatEditPanel::FillImage(Int Left, Int Right, Int Y)
{
	RECT rect;
	
	rect.left = Left;
	rect.right = Right;
	rect.top = Y;
	rect.bottom = rect.top + m_Font->GetHeight();

	m_BufImage->FillUInt32(0, &rect);
}


UInt32 UIChatEditPanel::InsertString(String &S, Int InsertPos)
{
	Int CharWidth = 0;
	ClearInsertList();
		
	for(Int i = 0; i < (Int)S.size(); ++i)
		CharWidth += m_Font->GetCharWidth(S[i]);
	if(m_RowList.size() == 0)
	{
		NewLine();
		m_EditRow = m_RowList.back();
		m_EditString = (UIChatString *)(m_EditItem = m_EditRow->GetChatItem(0));
		m_CurLine = 0;
	}
	
	Int Line = m_CurLine;
	UIChatRow *EditRow = m_RowList[Line];
	String *EditString = ((UIChatString *)(EditRow->GetChatItem(0)))->GetStringPtr();
	Int Spacing = m_LinePitch + m_Font->GetHeight();
	Int X = m_CursorX + CursorStartX;
	Int Y = Line * Spacing;

Label_1:

	if(CharWidth < m_DrawWidth - (Int)m_RowList[Line]->GetWidth())
	{
		if(m_InsertPos < (Int)EditString->size())
		{
			MoveLine(X + CharWidth, Y, X, Y, m_RowList[Line]->GetWidth() - X);	//OK
			FillImage(X, X + CharWidth, Y);
		}
		Int Width = DrawString(S, X, Y);
		EditString->Insert(m_InsertPos, S.c_str());
		m_CursorX += CharWidth;
		m_InsertPos += S.size();
		m_StringSize += S.size();
		EditRow->AddWidth(CharWidth);
	}
	else
	{
		if(m_InsertPos >= (Int)EditString->size())
		{
			if(Line + 1 == m_RowList.size())
			{
				Y += Spacing;
				if(Y + m_Font->GetHeight() > m_Height)
					return 1;

				m_CursorY = Y;
				EditRow->SetEndLine(false);
				Int Width = DrawOnNewLine(S);
				m_CursorX = CharWidth - CursorStartX;
				++m_CurLine;

				return 1;
			}
			else
			{
				if(Line + 1 < m_MaxLine)
				{
					if(m_RowList[m_CurLine]->GetEndLine())
					{

						if((Int)m_RowList.size() < m_MaxLine)
						{
							Int Y = Spacing * (Line + 1);
							MoveArea(0, Y + Spacing, 0, Y, m_Width, ((Int)m_RowList.size() - Line - 1) * Spacing); //OK
							FillImage(0, m_Width, Y);
							InsertLine(m_CurLine + 1);
							++m_StringSize;
							m_RowList[m_CurLine]->SetEndLine(false);
							m_RowList[m_CurLine + 1]->SetEndLine(true);
						}
						else
							return 0;
					}
					Line = ++m_CurLine;
					SetCurLine(Line);
					EditRow = m_EditRow;
					EditString = m_EditString->GetStringPtr();
					m_InsertPos = 0;
					m_CursorX = -CursorStartX;
					X = 0;
					m_CursorY = Y += Spacing;

					

					goto Label_1;
				}
				else
				{
					m_EndRow = (Int)m_RowList.size() - 1;
					return 1;
				}
			}
		}

		if(Line + 1 == m_RowList.size() || m_RowList[Line]->GetEndLine())
		{
			if((Int)m_RowList.size() < m_MaxLine)
			{
		
				m_RowList[Line]->SetEndLine(false);
				InsertLine(Line + 1);
				++m_StringSize;
				m_RowList[Line + 1]->SetEndLine(true);

				if(Line + 1 < (Int)m_RowList.size() - 1)
				{
					Int Y = Spacing * (Line + 1);
					MoveArea(0, Y + Spacing, 0, Y, m_Width, ((Int)m_RowList.size() - Line - 1) * Spacing);
					FillImage(0, m_Width, Y);
				}
			}
			else
				return 0;
		}
			
		String	SubStr;
		Int DelWidth = 0;
		Int Index = (Int)EditString->size() - 1, Count = 1;
		for(; Index >= 0; --Index, ++Count)
		{
			DelWidth += m_Font->GetCharWidth(EditString->at(Index));
			if((Int)EditRow->GetWidth() - DelWidth + CharWidth < m_DrawWidth)
				break;
		}


		SubStr = EditString->substr(Index, Count);
		
		
		m_InsertList.push_back(SafeCreateObject7(UIChatInsertNode, Line, X, m_InsertPos, Index, Count, CharWidth, DelWidth));
		
		for(Int i = Line + 1; i < (Int)m_RowList.size(); ++i)
		{
			EditString = ((UIChatString *)(m_RowList[i]->GetChatItem(0)))->GetStringPtr();
			CharWidth = 0;
			for(Int j = 0; j < (Int)SubStr.size(); ++j)
				CharWidth += m_Font->GetCharWidth(SubStr[j]);	
		
			DelWidth = 0;
			Index = (Int)EditString->size() - 1, Count = 1;
			if((Int)m_RowList[i]->GetWidth() + CharWidth < m_DrawWidth)
			{
				m_InsertList.push_back(SafeCreateObject7(UIChatInsertNode, i, 0, 0, 0, 0, CharWidth, 0));
				SubStr.clear();
				break;
			}
			else
			{
				for(; Index >= 0; --Index, ++Count)
				{
					DelWidth += m_Font->GetCharWidth(EditString->at(Index));
					if((Int)m_RowList[i]->GetWidth() - DelWidth + CharWidth < m_DrawWidth)
						break;
				}
			}

			m_InsertList.push_back(SafeCreateObject7(UIChatInsertNode, i, 0, 0, Index, Count, CharWidth, DelWidth));

			SubStr = EditString->substr(Index, Count);

			if(i == m_RowList.size() - 1 || m_RowList[i]->GetEndLine())
			{
				if((Int)m_RowList.size() < m_MaxLine)
				{
					m_RowList[i]->SetEndLine(false);
					InsertLine(i + 1);
					++m_StringSize;
					m_RowList[i + 1]->SetEndLine(true);

					if(i + 1 < (Int)m_RowList.size() - 1)
					{
						Int Y = Spacing * (i + 1);
						MoveArea(0, Y + Spacing, 0, Y, m_Width, ((Int)m_RowList.size() - i - 2) * Spacing);//OK
						FillImage(0, m_Width, Y);
					}
				}
				else
				{
					ClearInsertList();
					return 0;
				}
			}
		}

		Int InsertPos;
		m_InsertList[0]->Get(Line, X, InsertPos, Index, Count, CharWidth, DelWidth);
		EditString = ((UIChatString *)(m_RowList[Line]->GetChatItem(0)))->GetStringPtr();
		
		FillImage(EditRow->GetWidth() - DelWidth, EditRow->GetWidth(), Y);
		MoveLine(X + CharWidth, Y, X, Y, m_RowList[Line]->GetWidth() - X - DelWidth);
		FillImage(X, X + CharWidth, Y);
		Int Width = DrawString(S, X, Y);

		SubStr = EditString->substr(Index, Count);
		EditString->Delete(Index, Count);

		EditString->Insert(m_InsertPos, S.c_str());
		m_CursorX += CharWidth;
		m_InsertPos += S.size();
		m_StringSize += S.size();
		EditRow->AddWidth(CharWidth - DelWidth);
		Y += Spacing;
		
		for(Int i = 1; i < (Int)m_InsertList.size(); ++i)
		{
			m_InsertList[i]->Get(Line, X, InsertPos, Index, Count, CharWidth, DelWidth);
			EditString = ((UIChatString *)(m_RowList[Line]->GetChatItem(0)))->GetStringPtr();

			FillImage(m_RowList[Line]->GetWidth() - DelWidth, m_RowList[Line]->GetWidth(), Y);
			MoveLine(X + CharWidth, Y, X, Y, m_RowList[Line]->GetWidth() - X - DelWidth);
			FillImage(X, X + CharWidth, Y);
			Width = 0;
			for(Int j = 0; j < (Int)SubStr.size(); ++j)
			{
				Width = DrawChar(SubStr[j], Width, Y);
			}

			String TempSubStr = EditString->substr(Index, Count);
			EditString->Delete(Index, Count);
			EditString->insert(InsertPos, SubStr);
			SubStr = TempSubStr;
			m_RowList[Line]->AddWidth(CharWidth - DelWidth);
			Y += Spacing;
		}
	}

	ClearInsertList();

	return 1;
}


void UIChatEditPanel::ClearInsertList()
{
	for(std::vector<UIChatInsertNode *>::iterator i = m_InsertList.begin(); i != m_InsertList.end(); ++i)
	{
		SafeDeleteObject(*i);
	}
	m_InsertList.clear();
}


UInt32 UIChatEditPanel::AddCharW(CharW C)
{
	if(m_SelNow)
	{
		DelSelText();
	}

	if(m_Limit && m_StringSize >= m_Limit)
	{
		return 0;
	}

	Int CursorX = m_CursorX + CursorStartX;
	if(m_RowList.size() == 0)
	{
		Int Width = DrawOnNewLine(C);

		m_CurLine = 0;
		RenderParent();

		m_CursorX = Width - CursorStartX;
	}
	else
	{
		String S;

		S += C;
		return InsertString(S, m_InsertPos);
	}

	return 1;
}



void UIChatEditPanel::LoadText()
{
	ClearAllRow();

	Int Width = 0;
	String TempStr;

	if(m_Text.size() > m_Limit && m_Limit)
	{
		m_Text = m_Text.substr(0, m_Limit);
	}

	Int i;
	for(i = 0; i < (Int)m_Text.size(); ++i)
	{
		Char C = m_Text[i];
		if(C == _T('\r'))
		{
			NewLine();
			m_EditRow = m_RowList.back();
			m_EditString = (UIChatString *)(m_EditItem = m_EditRow->GetChatItem(0));
			
			m_EditRow->SetWidth(Width);
			m_EditString->GetStringRef().assign(TempStr);
			TempStr.clear();
			Width = 0;
			if((Int)m_RowList.size() >= m_MaxLine)
				break;
		}
		else
		{
			Int FontWidth = m_Font->GetCharWidth(C);
			if(Width + FontWidth > m_DrawWidth)
			{
				NewLine();
				m_EditRow = m_RowList.back();
				m_EditString = (UIChatString *)(m_EditItem = m_EditRow->GetChatItem(0));
	
				m_EditString->GetStringRef().assign(TempStr);
				m_EditRow->SetWidth(Width);

				TempStr.clear();
				TempStr.push_back(C);
				Width = FontWidth;
				m_EditRow->SetEndLine(false);
				if((Int)m_RowList.size() >= m_MaxLine)
					break;
			}
			else
			{	
				TempStr.push_back(C);
				Width += FontWidth;
			}
		}
	}

	if(i == (Int)m_Text.size() && i)
	{
		NewLine();
		m_EditRow = m_RowList.back();
		m_EditString = (UIChatString *)(m_EditItem = m_EditRow->GetChatItem(0));
		m_EditString->GetStringRef().assign(TempStr);
		m_EditRow->SetWidth(Width);
	}

	/*
	if(m_Text.size() && m_RowList.size() == 0)
	{
		NewLine();
		m_EditRow = m_RowList.back();
		m_EditString = (UIChatString *)(m_EditItem = m_EditRow->GetChatItem(0));

		m_EditString->GetStringRef().assign(TempStr);
		m_EditRow->SetWidth(Width);
	}
	*/
	if(m_RowList.size())
		SetCurLine(0);
	m_InsertPos = 0;
	m_CursorX = -CursorStartX;
	m_CursorY = 0;

	m_StringSize = m_Text.size();
}


void UIChatEditPanel::SaveText()
{
	m_Text.clear();
	m_TInsertPos = 0;
	for(Int i = 0; i < (Int)m_RowList.size(); ++i)
	{
		if(i == m_CurLine)
		{
			m_TInsertPos = (Int)m_Text.size() + m_InsertPos;
		}
		String *EditString = ((UIChatString *)(m_RowList[i]->GetChatItem(0)))->GetStringPtr();
		m_Text += *EditString;
		if(m_RowList[i]->GetEndLine() && i < (Int)m_RowList.size() - 1)
		{
			m_Text += _T("\r");
		}
	}
}


void UIChatEditPanel::SaveNoSelText()
{
	Int		SelStartLine, SelStartPos;
	Int		SelEndLine, SelEndPos;
	Int		SelLeft,  SelRight;
	
	if(m_SelStartLine > m_SelEndLine || (m_SelStartLine == m_SelEndLine && m_SelStartPos > m_SelEndPos))
	{
		SelStartLine = m_SelEndLine; 
		SelStartPos = m_SelEndPos;
		SelEndLine = m_SelStartLine; 
		SelEndPos = m_SelStartPos;
		SelLeft = m_SelRight;  
		SelRight = m_SelLeft;
	}
	else
	{
		SelStartLine = m_SelStartLine; 
		SelStartPos = m_SelStartPos;
		SelEndLine = m_SelEndLine; 
		SelEndPos = m_SelEndPos;
		SelLeft = m_SelLeft;  
		SelRight = m_SelRight;
	}

	m_Text.clear();
	if(SelStartLine == SelEndLine)
	{
		for(Int i = 0; i < (Int)m_RowList.size(); ++i)
		{
			String EditString = ((UIChatString *)(m_RowList[i]->GetChatItem(0)))->GetStringRef();

			if(i == SelStartLine)
			{
				EditString.Delete(SelStartPos, SelEndPos - SelStartPos);
				m_TInsertPos = (Int)m_Text.size() + SelStartPos;
			}
			m_Text += EditString;
			if(m_RowList[i]->GetEndLine() && i < (Int)m_RowList.size() - 1)
			{
				m_Text += _T("\r");
			}
		}
	}
	else
	{
		for(Int i = 0; i < (Int)m_RowList.size(); ++i)
		{
			String EditString = ((UIChatString *)(m_RowList[i]->GetChatItem(0)))->GetStringRef();

			if(i > SelStartLine && i < SelEndLine)
				continue;
			else
			if(i == SelStartLine)
			{
				EditString.Delete(SelStartPos, (Int)EditString.size() - SelStartPos);
				m_TInsertPos = (Int)m_Text.size() + SelStartPos;
				m_Text += EditString;
				continue;
			}
			else
			if(i == SelEndLine)
				EditString.Delete(0, SelEndPos);

			m_Text += EditString;
			if(m_RowList[i]->GetEndLine() && i < (Int)m_RowList.size() - 1)
			{
				m_Text += _T("\r");
			}
		}
	}
}



void UIChatEditPanel::SaveOnlySelText()
{
	Int		SelStartLine, SelStartPos;
	Int		SelEndLine, SelEndPos;
	Int		SelLeft,  SelRight;

	if(m_RowList.size() == 0)
		return;
	
	if(m_SelStartLine > m_SelEndLine || (m_SelStartLine == m_SelEndLine && m_SelStartPos > m_SelEndPos))
	{
		SelStartLine = m_SelEndLine; 
		SelStartPos = m_SelEndPos;
		SelEndLine = m_SelStartLine; 
		SelEndPos = m_SelStartPos;
		SelLeft = m_SelRight;  
		SelRight = m_SelLeft;
	}
	else
	{
		SelStartLine = m_SelStartLine; 
		SelStartPos = m_SelStartPos;
		SelEndLine = m_SelEndLine; 
		SelEndPos = m_SelEndPos;
		SelLeft = m_SelLeft;  
		SelRight = m_SelRight;
	}

	m_Text.clear();
	if(SelStartLine == SelEndLine)
	{
		String EditString = ((UIChatString *)(m_RowList[SelStartLine]->GetChatItem(0)))->GetStringRef();
		m_Text += EditString.substr(SelStartPos, SelEndPos - SelStartPos);
	}
	else
	{
		String EditString;
		for(Int i = 0; i < (Int)m_RowList.size(); ++i)
		{
			EditString = ((UIChatString *)(m_RowList[i]->GetChatItem(0)))->GetStringRef();

			if(i > SelStartLine && i < SelEndLine)
			{
				m_Text += EditString;
				if(m_RowList[i]->GetEndLine())
					m_Text += _T("\r");
			}
			else
			if(i == SelStartLine)
			{
				m_Text += EditString.substr(SelStartPos, (Int)EditString.size() - SelStartPos);
				if(m_RowList[i]->GetEndLine())
					m_Text += _T("\r");
			}
			else
			if(i == SelEndLine)
				m_Text += EditString.substr(0, SelEndPos);
		}
		if(SelEndLine == (Int)m_RowList.size() - 1)
		{
			if(m_InsertPos == EditString.size() && m_RowList[SelEndLine]->GetEndLine())
				m_Text += _T("\r");
		}
	}
}


void UIChatEditPanel::SelText2ClipBoard()
{
	UInt32		Len;
	HANDLE		hMem;
	LPVOID		lpData;

	//SaveText();
	SaveOnlySelText();

	Len = ((Int)m_Text.size() + 1) * 2;
	
	OpenClipboard(g_UIhWnd);
	EmptyClipboard();
	
	hMem = GlobalAlloc(GHND, Len);

	lpData = (LPSTR)GlobalLock(hMem);
	String TempStr = m_Text;
	ClearCtrlText(TempStr);
	memcpy(lpData, TempStr.c_str(), TempStr.size() * 2);
	GlobalUnlock(hMem);
	SetClipboardData(CF_UNICODETEXT, hMem);

	CloseClipboard();

	
}


void UIChatEditPanel::SetSelStartCursorPos()
{
	if(m_RowList.size())
	{
		if(m_SelStartLine > m_SelEndLine || (m_SelStartLine == m_SelEndLine && m_SelStartPos > m_SelEndPos))
		{
			SetCurLine(m_SelEndLine);
			m_InsertPos = m_SelEndPos;
			m_CursorY = m_SelEndLine * (m_Font->GetHeight() + m_LinePitch);
			m_CursorX = m_SelRight - CursorStartX;
		}
		else
		{
			SetCurLine(m_SelStartLine);
			m_InsertPos = m_SelStartPos;
			m_CursorY = m_SelStartLine * (m_Font->GetHeight() + m_LinePitch);
			m_CursorX = m_SelLeft - CursorStartX;
		}
	}
	else
	{
		SetNoCurLine();
		m_InsertPos = 0;
		m_CursorY = 0;
		m_CursorX = -CursorStartX;
	}
}


void UIChatEditPanel::DelSelText()
{
	SaveNoSelText();
	LoadText();
	SetSelStartCursorPos();
	m_BufImage->FillUInt32();
	Redraw();
	m_SelNow = false;
}


void UIChatEditPanel::Redraw()
{
	if(m_RowList.size())
	{
		Int32	Y = 0;
		Int32	Spacing = m_Font->GetHeight() + m_LinePitch;

		SetChange(true);
		Int32 i;
		for(i = 0; i < (Int32)m_RowList.size(); ++i)
		{
			/*
			RECT	rect;

			SetRect(&rect, 0, Y, m_RowList[i]->GetWidth(), Y + Spacing);
			m_BufImage->FillUInt32(0xFFFF7F00, &rect);
			if(m_RowList[i]->GetEndLine())
			{
				RECT	rect;

				SetRect(&rect, 0, Y, 16, Y + Spacing);
				m_BufImage->FillUInt32(0xFF007F00, &rect);
			}
			*/
			String *StrPtr = ((UIChatString *)(m_RowList[i]->GetChatItem(0)))->GetStringPtr();
			Int X = 0;
			for(Int j = 0; j < (Int)StrPtr->size(); ++j)
			{
				X = DrawChar(StrPtr->at(j), X, Y);
			}
			Y += Spacing;
		}
		if(m_EndRow < i - 1)
		{
			m_EndRow = i - 1;
		}
	}
	RenderParent();
}



bool  UIChatEditPanel::TestRedraw()
{
	if(m_Text.size())
	{
		Int Width = 0;
		Int LineCount = 0;
		Int InsertPos = 0;
		Int CheckPoint = m_TInsertPos;
		Int Height = m_LinePitch + m_Font->GetHeight();
		Int TextSize = (Int)m_Text.size();

		if(TextSize && TextSize > m_Limit)
		{
			TextSize = m_Limit;
		}

		Int i;
		for(i = 0; i < TextSize; ++i)
		{
			if(i == CheckPoint)
			{
				m_TCurLine = LineCount;
				m_TCursorX = Width;
				m_TCursorY = LineCount * Height;
				m_TInsertPos = InsertPos;
			}
			
			Char C = m_Text[i];
			if(C == _T('\r'))
			{
				++LineCount;
				Width = 0;
				InsertPos = 0;
				if(LineCount >= m_MaxLine)
				{
					if(i == (Int)m_Text.size() - 1)
						return true;
					break;
				}
			}
			else
			{
				Int FontWidth = m_Font->GetCharWidth(C);
				if(Width + FontWidth > m_DrawWidth)
				{
					++LineCount;
					Width = FontWidth;
					InsertPos = 1;
					if(LineCount >= m_MaxLine)
						break;
				}
				else
				{	
					Width += FontWidth;
					++InsertPos;
				}
			}
		}

		if(i == CheckPoint)
		{
			m_TCurLine = LineCount;
			m_TCursorX = Width;
			m_TCursorY = LineCount * Height;
			m_TInsertPos = InsertPos;
		}
		
	
		if(i == TextSize)
		{
			return true;
		}
	}
	return false;
}


bool UIChatEditPanel::InsertClipBoard()
{

	HANDLE		hMem;
	String		TempStr, MyText;

	OpenClipboard(g_UIhWnd);

	if(hMem = GetClipboardData(CF_UNICODETEXT))
	{
		TempStr = (PSTRINGW)GlobalLock(hMem);
		GlobalUnlock(hMem);
	}
	else
	if(hMem = GetClipboardData(CF_TEXT))
	{
		TempStr = (LPSTR)GlobalLock(hMem);
		GlobalUnlock(hMem);
	}
	
	CloseClipboard();

	ClearCtrlText(TempStr);
	for(UInt i = 0; i < TempStr.size(); ++i)
	{
		if(TempStr[i] > 26 || TempStr[i] == _T('\r'))
			MyText += TempStr[i];
	}

	if(m_Limit)
		if(MyText.size() + m_Text.size() > m_Limit)
			return false;

	if(!MyText.empty())
	{
		if(m_Text.size() == 0)
		{
			m_Text = MyText;
			m_TInsertPos = (Int)m_Text.size();
			m_TCurLine = 0;
			m_TCursorX = 0;
			m_TCursorY = 0;
			
		}
		else
		if(m_TInsertPos == 0)
		{
			m_Text = MyText + m_Text;
			m_TInsertPos = (Int)MyText.size();
		}
		else
		if(m_TInsertPos == (Int)m_Text.size())
		{
			m_Text += MyText;
			m_TInsertPos = (Int)m_Text.size();
		}
		else
		{
			String S = m_Text.substr(0, m_TInsertPos);
			S += MyText;
			Int TempPos = (Int)S.size();
			S += m_Text.substr(m_TInsertPos, m_Text.size() - m_TInsertPos); 
			m_Text = S;
			m_TInsertPos = TempPos;
		}

		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////



UIEditBox::UIEditBox() : UIStaticText()
{
	m_InsertPos = 0;
	m_CursorX = -CursorStartX; m_CursorY = 0;
	m_StartPoint = 0;
	m_EditCount = 0;
	m_EditLimit = 0;
	m_OnScreenWidth = 0;
	m_ShowEditCursor = true;
	m_EditCursorRefreshTime = 0;
	
	m_ShiftDown = false;
	m_SelColor = 0xFFFF0000;
	m_WaitText = "";
	m_ReadPastStr = false;
	m_NoSpace = false;

	m_PastMsg.clear();
	m_PastMsgIndex = 0;
	m_HashedPassword.SetCapacity(SHA256_BLOCK_SIZE);
	m_HashedPassword.SetSize(SHA256_DIGEST_SIZE);
	m_HashedPassword.Zero();
}

void UIEditBox::Dispose()
{
	if (m_pUIManager && m_pUIManager->GetKeyInItem() == this)
		m_pUIManager->SetKeyInItem("EditLine");

	m_PastMsg.clear();
	m_HashedPassword.Zero();
}
void UIEditBox::AddText(PCSTRINGW pText)
{
	if(m_IsPassword)
		m_OnScreenWidth = End();

	m_InsertPos = (UInt32)m_Text.size();
	m_CursorX = -CursorStartX; m_CursorY = 0;
	if(m_IsPassword)
	{
		Int Size = m_Font->GetCharWidth(_T('*')) + m_PicthWidth;
		for(UInt i = 0; pText[i]; ++i)
		{
			m_CursorX += Size;
			m_Text += _T('*');
			*((CharW*)&m_HashedPassword) = pText[i];
			Reuben::Math::SHA256(&m_HashedPassword, &m_HashedPassword, SHA256_DIGEST_SIZE);
		}
	}
	else
	{
		m_Text += pText;
		for(UInt32 i = 0; pText[i]; ++i)
			m_CursorX += m_Font->GetCharWidth(pText[i]) + m_PicthWidth;
	}
}


void UIEditBox::AddText(PCSTRINGA pText)
{
	
	StringW StrW = pText;

	if(m_IsPassword)
		m_OnScreenWidth = End();

	m_InsertPos = (UInt32)m_Text.size();
	m_CursorX = -CursorStartX; m_CursorY = 0;
	if(m_IsPassword)
	{
		Int Size = m_Font->GetCharWidth(_T('*')) + m_PicthWidth;
		for(UInt i = 0; pText[i]; ++i)
		{
			m_CursorX += Size;
			m_Text += _T('*');
			*((CharW*)&m_HashedPassword) = StrW[i];
			Reuben::Math::SHA256(&m_HashedPassword, &m_HashedPassword, SHA256_DIGEST_SIZE);
		}
	}
	else
	{
		m_Text += StrW;
		for(UInt32 i = 0; pText[i]; ++i)
			m_CursorX += m_Font->GetCharWidth(StrW[i]) + m_PicthWidth;
	}
}


void UIEditBox::SetupCursor(CRender *pRender)
{
	g_EditCursor = CImageManager::GetInstance().CreateImage(_T("Data\\ECursor.tga"), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
}
	


void UIEditBox::Render(CImage *pImage)
{
	UIStaticText::Render(pImage);

	if(m_pUIManager->GetKeyInItem() == this ||
		m_pUIManager->GetFocusItem() == this)
	{
		RECT	SRect = {0, 0, 8, 12};
		
		if (m_ShowEditCursor)
			Alpha8888To8888S(m_OffsetX + m_CursorX, m_OffsetY + m_CursorY, 	g_EditCursor, pImage, &SRect, m_RenderStyle, m_Color);
	}
}


void UIEditBox::Render(CImage *pImage, RECT &RRect)
{
	UIStaticText::Render(pImage, RRect);

	if(m_Focus)
	{
		UInt32		X = m_OffsetX + m_CursorX, Y = m_OffsetY + m_CursorY;
		RECT	SRect = {0, 0, 8, 12};
		
		if (m_ShowEditCursor)
			Alpha8888To8888S(X, Y, g_EditCursor, pImage, &SRect, m_RenderStyle, m_Color);
	}
}


void UIEditBox::UpPastMsg()
{
	m_PastMsgIndex --;
	if (m_PastMsgIndex < 0)
		m_PastMsgIndex = 0;
}


void UIEditBox::DownPastMsg()
{
	m_PastMsgIndex ++;
	if (m_PastMsgIndex >= (Int)m_PastMsg.size())
	{
		m_PastMsgIndex = (Int)m_PastMsg.size() - 1;
	}
}


Int UIEditBox::ShowHisInput()
{
	if (m_PastMsgIndex >= (Int)m_PastMsg.size() ||
		m_PastMsgIndex == -1)
		return 0;
	
	if (m_ReadPastStr)
	{
		ClearText();
		SetText(m_PastMsg[m_PastMsgIndex]);

		End();
		FillImage();
		DrawFont();
		
		return 1;
	}

	return 0;
}


void UIEditBox::AddPastStr()
{
	if(!m_IsPassword)
	{
		m_PastMsg.push_back(GetText());
		if (m_PastMsg.size() > MaxHis)
			m_PastMsg.erase(m_PastMsg.begin());
		m_PastMsgIndex = (Int)m_PastMsg.size();
	}
}


UInt32 UIEditBox::Home()
{
	UInt32	PitchHeight, RealFontHeight;

	if(m_ShiftDown && m_SelStart == m_SelEnd)
		m_SelStart = m_InsertPos;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	UInt32	DrawWidth = 
	m_StartPoint = 
	m_InsertPos = 0;
	m_CursorX = -CursorStartX; m_CursorY = 0;

	if(m_ShiftDown)
		m_SelEnd = 0;

	m_OnScreenWidth = 0;
	for(UInt32 i = 0; i < m_Text.size(); ++i)
	{
		UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[i]);
		if(DrawWidth + CharWidth + RealFontHeight > m_Width)
		{
			break;
		}
		m_OnScreenWidth = DrawWidth + CharWidth + RealFontHeight;
		DrawWidth += CharWidth + PitchHeight;

	}

	return DrawWidth;
}


UInt32 UIEditBox::End()
{
	UInt32	PitchHeight, RealFontHeight;

	if(m_ShiftDown && m_SelStart == m_SelEnd)
		m_SelStart = m_InsertPos;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	UInt32	DrawWidth = 0;
	bool HideNow = false;

	for(UInt32 i = m_StartPoint; i < m_Text.size(); ++i)
	{
		if(m_Text[i] == 15)
		{
			HideNow = true;
			continue;
		}
		else
		if(m_Text[i] == 16)
		{
			HideNow = false;
			continue;
		}

		if(HideNow || m_Text[i] <= 26)
		{
			continue;
		}
		UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[i]);
		if(DrawWidth + CharWidth + RealFontHeight > m_Width)
		{
			goto Label_1;
		}
		m_OnScreenWidth = DrawWidth + CharWidth + RealFontHeight;
		DrawWidth += CharWidth + PitchHeight;
		
	}
	m_InsertPos = (UInt32)m_Text.size();
	if(m_ShiftDown)
		m_SelEnd = m_InsertPos;
	m_CursorX = DrawWidth - CursorStartX;

	return DrawWidth;

Label_1:

	DrawWidth = 0;
	m_StartPoint = 0;
	HideNow = false;
	for(Int32 i = (Int32)m_Text.size() - 1; i >= 0; --i)
	{
		if(m_Text[i] == 15)
		{
			HideNow = false;
			continue;
		}
		else
		if(m_Text[i] == 16)
		{
			HideNow = true;
			continue;
		}

		if(HideNow || m_Text[i] <= 26)
		{
			continue;
		}

		UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[i]);
		UInt32	TempWidth = CharWidth + RealFontHeight;
			
		if(DrawWidth + TempWidth >= m_Width)
		{
			m_StartPoint = i + 1;
			break;
		}

		DrawWidth += (CharWidth + PitchHeight);
	}

	m_InsertPos = (UInt32)m_Text.size();
	if(m_ShiftDown)
		m_SelEnd = m_InsertPos;
	m_CursorX = DrawWidth - CursorStartX;

	return DrawWidth;
}


void UIEditBox::ClearText()
{
	m_Text.clear();
	m_InsertPos = 0;
	m_CursorX = -CursorStartX; m_CursorY = 0;
	m_StartPoint = 0;
	m_EditCount = 0;
	m_SelStartX = m_SelStart = m_SelEnd = 0;
	m_HashedPassword.Zero();

	FillImage();
}


void UIEditBox::SetText(PCSTRINGW pText)
{
	UIStaticText::SetText(pText);
	m_InsertPos = 0;
	m_CursorX = -CursorStartX; m_CursorY = 0;
	m_StartPoint = m_SelStartX = m_SelStart = m_SelEnd = 0;
	/*
	for(UInt32 i = 0; i < m_Text.size(); ++i)
	{
		m_CursorX += m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[i]) + m_PicthWidth;
	}
	*/
}


void UIEditBox::SetText(PCSTRINGA pText)
{

	StringW Str(pText);
	UIStaticText::SetText(Str);
	m_InsertPos = 0;
	m_CursorX = -CursorStartX; m_CursorY = 0;
	m_StartPoint = m_SelStartX = m_SelStart = m_SelEnd = 0;
	/*
	for(UInt32 i = 0; i < m_Text.size(); ++i)
	{
		m_CursorX += m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[i]) + m_PicthWidth;
	}
	*/
}


void UIEditBox::SetText(StringW	&pText)
{
	UIStaticText::SetText(pText);
	m_InsertPos = 0;
	m_CursorX = -CursorStartX; m_CursorY = 0;
	m_StartPoint = m_SelStartX = m_SelStart = m_SelEnd = 0;
	/*
	for(UInt32 i = 0; i < m_Text.size(); ++i)
	{
		m_CursorX += m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[i]) + m_PicthWidth;
	}
	*/
}


void UIEditBox::InsertText(PCSTRINGW Text)
{
	StringW Str = Text;

	InsertTextS(Str);
}

void UIEditBox::InsertText(PCSTRINGA Text)
{
	
	StringW Str(Text);

	InsertTextS(Str);
}



void UIEditBox::InsertTextS(StringW &Text)
{
	UInt32	PitchHeight, RealFontHeight;

	if(m_IsPassword)
		m_OnScreenWidth = End();

	UInt Size1 = NoCodeStringSize(m_Text);
	UInt Size2 = NoCodeStringSize(Text);

	if(NoCodeStringSize(m_Text) + NoCodeStringSize(Text) > m_EditLimit && m_EditLimit != 0)
		return;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);
	if(m_IsPassword)
	{
		for(UInt i = 0; i < Text.size(); ++i)
		{
			m_Text.insert(m_InsertPos + i, _T("*"));
			*((CharW*)&m_HashedPassword) = Text[i];
			Reuben::Math::SHA256(&m_HashedPassword, &m_HashedPassword, SHA256_DIGEST_SIZE);
		}
	}
	else
	{
		m_Text.insert(m_InsertPos, Text);
	}
	/*
	std::vector<UInt32>::iterator it = m_CharWidth.begin() + m_InsertPos;
	std::vector<UInt32>::iterator Mark_it = m_CharMark.begin() + m_InsertPos;
	for(UInt32 i = 0; i < Text.size(); ++i)
	{
		std::vector<UInt32>::iterator it = m_CharWidth.begin() + m_InsertPos + i;
		std::vector<UInt32>::iterator Mark_it = m_CharMark.begin() + m_InsertPos + i;

		m_CharWidth.insert(it, m_Font->GetCharWidth(Text[m_InsertPos]));
		m_CharMark.insert(Mark_it, 0);

	}
	*/

	bool HideNow = false;

	for(UInt32 i = 0; i < Text.size(); ++i)
	{
		if(Text[i] == 15)
		{
			HideNow = true;
			++m_InsertPos;
			continue;
		}
		else
		if(Text[i] == 16)
		{
			HideNow = false;
			++m_InsertPos;
			continue;
		}

		if(HideNow || Text[i] <= 26)
		{
			++m_InsertPos;
			continue;
		}
		UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[m_InsertPos]);
		UInt32	TempWidth = CharWidth + RealFontHeight;

		bool SubHideNow = false;
		while(m_CursorX + CursorStartX + TempWidth >= m_Width)
		{
			if(m_Text[m_StartPoint] == 15)
			{
				SubHideNow = true;
				++m_StartPoint;
				continue;
			}
			else
			if(m_Text[m_StartPoint] == 16)
			{
				SubHideNow = false;
				++m_StartPoint;
				continue;
			}

			if(SubHideNow || m_Text[m_StartPoint] <= 26)
			{
				++m_StartPoint;
				continue;
			}

			UInt32	CharWidth2 = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[m_StartPoint]);
			m_CursorX -= CharWidth2 + PitchHeight;
			++m_StartPoint;
			
		}

		m_CursorX += CharWidth + PitchHeight;
		++m_InsertPos;
	}

	FillImage();
	DrawFont();
}


void UIEditBox::DrawFont()
{
	if(!Empty())
	{
		if(m_SelStart != m_SelEnd)
		{
			if(m_StartPoint > 0)
			{
				UInt32 SelStart = (m_SelStart > m_StartPoint ? m_SelStart - m_StartPoint : 0);
				UInt32 SelEnd = (m_SelEnd > m_StartPoint ? m_SelEnd - m_StartPoint : 0);
				
				m_Font->SetSel(SelStart, SelEnd);

			}
			else
				m_Font->SetSel(m_SelStart, m_SelEnd);

		}
		if(m_IsPassword)
		{
			StringW	TempText;

			for(UInt32 i = 0; i < m_Text.size(); ++i)
				TempText += _T('*'); 
			
			m_Font->DrawFontStr(TempText.c_str() + m_StartPoint,
				m_BufImage->GetPtr(), 0, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, 0);
		}
		else
		{
			m_Font->DrawFontStr(m_Text.c_str() + m_StartPoint,
				m_BufImage->GetPtr(), 0, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle, 
				m_FontColor, m_ShadowColor, 0);
		}

		
		m_Font->SetSel(0, 0);
	}
}


void UIEditBox::CursorMoveLeft(Int32	PitchHeight, UInt Count)
{
	if(m_ShiftDown && m_SelStart == m_SelEnd)
		m_SelStart = m_InsertPos;

	bool	HideNow = false, JumpChar = false;

	for (UInt i = 0; i < Count || JumpChar; i++)
	{
		if(m_InsertPos > 0)
		{
			--m_InsertPos;

			if(m_Text[m_InsertPos] == 15)
			{
				HideNow = false;
				JumpChar = false;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 16)
			{
				HideNow = true;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 17)
			{
				JumpChar = (m_InsertPos > 0 && m_Text[m_InsertPos - 1] == 16);
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 18)
			{
				JumpChar = true;
				--i;
				continue;
			}
			

			if(HideNow || m_Text[m_InsertPos] <= 26)
			{
				--i;
				continue;
			}

			m_CursorX -= m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[m_InsertPos]) + PitchHeight;
			if(m_InsertPos < m_StartPoint)
			{
				m_CursorX = -CursorStartX; m_CursorY = 0;
				--m_StartPoint;

				bool SubHideNow = false;
				while((SubHideNow || m_Text[m_StartPoint] <= 26) && m_StartPoint > 0)
				{
					if(m_Text[m_StartPoint] == 15)
					{
						SubHideNow = false;
					}
					else
					if(m_Text[m_StartPoint] == 16)
					{
						SubHideNow = true;
					}

					--m_StartPoint;
				}
			}
			
		}
		else
			break;
	}

	if(m_InsertPos == 0)
		m_StartPoint = 0;	

	if(m_ShiftDown)
		m_SelEnd = m_InsertPos;


}

void UIEditBox::CursorMoveRight(Int32	PitchHeight, Int32 RealFontHeight,  UInt Count)
{
	if(m_ShiftDown && m_SelStart == m_SelEnd)
		m_SelStart = m_InsertPos;

	bool	HideNow = false, JumpChar = false;

	for (Int i = 0; i < (Int)Count || JumpChar; i++)
	{
		if(m_InsertPos < m_Text.size())
		{
			if(m_Text[m_InsertPos] == 15)
			{
				HideNow = true;
				++m_InsertPos;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 16)
			{
				HideNow = false;
				++m_InsertPos;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 17)
			{
				JumpChar = true;
				++m_InsertPos;
				--i;
				continue;

			}
			else
			if(m_Text[m_InsertPos] == 18)
			{
				JumpChar = false;
				++m_InsertPos;
				--i;
				continue;

			}

			if(HideNow || m_Text[m_InsertPos] <= 26)
			{
				++m_InsertPos;
				--i;
				continue;
			}
			UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[m_InsertPos]);
			UInt32	TempWidth = CharWidth + RealFontHeight;

			bool SubHideNow = false, SubJumpChar = false;
			while(m_CursorX + CursorStartX + TempWidth >= m_Width || SubJumpChar)
			{
				if(m_Text[m_StartPoint] == 15)
				{
					SubHideNow = true;
					++m_StartPoint;
					continue;
				}
				else
				if(m_Text[m_StartPoint] == 16)
				{
					SubHideNow = false;
					++m_StartPoint;
					continue;
				}
				else
				if(m_Text[m_InsertPos] == 17)
				{
					SubJumpChar = true;
				}
				else
				if(m_Text[m_InsertPos] == 18)
				{
					SubJumpChar = false;
				}

				if(SubHideNow || m_Text[m_StartPoint] <= 26)
				{
					++m_StartPoint;
					continue;
				}

				UInt32	CharWidth2 = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[m_StartPoint]);
				m_CursorX -= CharWidth2 + PitchHeight;
				++m_StartPoint;

				
				
			}

			m_CursorX += CharWidth + PitchHeight;
			++m_InsertPos;

		}
		else 
			break;
	}

	if(m_ShiftDown)
		m_SelEnd = m_InsertPos;

	
}


void UIEditBox::CursorMoveLeft(UInt Count)
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);
	CursorMoveLeft(PitchHeight, Count);
}


void UIEditBox::CursorMoveRight(UInt Count)
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);
	CursorMoveRight(PitchHeight, RealFontHeight, Count);
}


UInt32 UIEditBox::DelSelText(UInt32	PitchHeight, UInt32 RealFontHeight)
{
	if(m_SelStart == m_SelEnd)
		return 0;

	UInt32		SelStart = min(m_SelStart, m_SelEnd);
	UInt32		SelEnd = max(m_SelStart, m_SelEnd);
	UInt32		Len = SelEnd - SelStart;
	
	if(m_Text.size())
	{
		if(m_InsertPos > SelStart)
		{
			CursorMoveLeft(PitchHeight, m_InsertPos - SelStart);
		}
		else
		if(SelStart > m_InsertPos)
		{
			CursorMoveRight(PitchHeight, RealFontHeight, SelStart - m_InsertPos);
		}
		/*
		std::vector<UInt32>::iterator it = m_CharWidth.begin() + SelStart;
		m_CharWidth.erase(it, Len + it);
		it = m_CharMark.begin() + SelStart;
		m_CharMark.erase(it, Len + it);
		*/
	
		m_Text.erase(SelStart, Len);
	}

	m_SelStart = m_SelEnd = 0;

	if(m_Text.size() == 0)
	{
		m_InsertPos = 0;
		m_CursorX = -CursorStartX;
		m_CursorY = 0;
	}

	return Len;
}




Int32 UIEditBox::OnKeyDown(WPARAM wParam)
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	m_ShiftDown = ((GetKeyState(VK_LSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 ||
		(GetKeyState(VK_RSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0);

	switch(wParam)
	{
	case VK_LEFT:
		if(!m_ShiftDown)
			m_SelStart = m_SelEnd = 0;
		CursorMoveLeft(PitchHeight, 1);
		FillImage();
		DrawFont();
		break;
	case VK_RIGHT:		
		if(!m_ShiftDown)
			m_SelStart = m_SelEnd = 0;
		CursorMoveRight(PitchHeight, RealFontHeight, 1);
		FillImage();
		DrawFont();
		break;
	case VK_HOME:
		if(!m_ShiftDown)
			m_SelStart = m_SelEnd = 0;
		m_OnScreenWidth = Home();
		FillImage();
		DrawFont();
		break;
	case VK_END:
		if(!m_ShiftDown)
			m_SelStart = m_SelEnd = 0;
		m_OnScreenWidth = End();
		FillImage();
		DrawFont();
		break;
	case VK_DELETE:
		if(m_IsPassword)
		{
			m_SelStart = 0;
			m_SelEnd = (Int32)m_Text.size();
			DelSelText(PitchHeight, RealFontHeight);
			m_HashedPassword.Zero();
			FillImage();
		}
		else
		if(m_Text.size())
		{
			if(m_SelStart == m_SelEnd)
			{
				if(m_InsertPos < m_Text.size())
				{
					Int	i = 1;
					if(m_Text[m_InsertPos] == 15)
					{
						while(m_Text[m_InsertPos + i] != 16 && m_InsertPos + i < m_Text.size())
						{
							++i;
						}
						if(m_Text[m_InsertPos + i] == 16)
							++i;
					}
					if(m_Text[m_InsertPos + i] == 17)
					{
						while(m_Text[m_InsertPos + i] != 18 && m_InsertPos + i < m_Text.size())
						{
							++i;
						}
						if(m_Text[m_InsertPos + i] == 18)
							++i;
					}

					/*
					std::vector<UInt32>::iterator it = m_CharWidth.begin() + m_InsertPos;
					m_CharWidth.erase(it, i + it);
					it = m_CharMark.begin() + m_InsertPos;
					m_CharMark.erase(it, i + it);
					*/
					m_Text.erase(m_InsertPos, i);
				}
			}
			else
			{
				DelSelText(PitchHeight, RealFontHeight);
			}

			FillImage();
			DrawFont();
		}
		break;		
	default:
		m_ShiftDown = false;
		return 0;
		break;
	}

	m_ShiftDown = false;

	return 1;
}


UInt32 UIEditBox::AddCharW(WPARAM wParam, UInt32	PitchHeight, UInt32 RealFontHeight)
{
	Int Len = NoCodeStringSize(m_Text);
	if(NoCodeStringSize(m_Text) < m_EditLimit || m_EditLimit == 0)
	{
		if(m_NumOnly && (wParam < _T('0') || wParam > _T('9')))
			return 1;

		StringW		A1;
		
		A1 = CharW(wParam);
		
		if(m_IsPassword)
			m_OnScreenWidth = End();

		if(m_SelEnd != m_SelStart && !m_IsPassword)
		{
			DelSelText(PitchHeight, RealFontHeight);
		}
		
		UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : CharW(wParam));
		UInt32	TempWidth = CharWidth + RealFontHeight;

		StringW	TempStr = m_Text;
		if(m_IsPassword)
		{
			m_Text.insert(m_InsertPos, _T("*"));
			*((CharW*)&m_HashedPassword) = CharW(wParam);
			Reuben::Math::SHA256(&m_HashedPassword, &m_HashedPassword, SHA256_DIGEST_SIZE);
		}
		else
			m_Text.insert(m_InsertPos, A1);

		if(!CheckDrawOver())
		{
			CursorMoveRight(PitchHeight, RealFontHeight, 1);
			if(m_CursorX == -CursorStartX)
				CursorMoveRight(PitchHeight, RealFontHeight, 1);
			
			FillImage();
			DrawFont();
		}
		else
		{
			m_Text = TempStr;
		}
	}

	//TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UIEditBox::AddCharW(WPARAM wParam, UInt32	PitchHeight, UInt32 RealFontHeight)%s"), &m_HashedPassword);
	return 1;
}

void UIEditBox::BackSpaceKey(UInt32	PitchHeight, Int32 RealFontHeight)
{
	if(m_StartPoint > 0)
	{
		UInt InsertPos = m_InsertPos;
				
		for(Int i = 0; i < 5; i++)
			CursorMoveLeft(PitchHeight, 1);
		for(Int i = 0; i < 4; i++)
			CursorMoveRight(PitchHeight, RealFontHeight, 1);
		while(m_InsertPos >= InsertPos && m_InsertPos > 0)
			CursorMoveLeft(PitchHeight, 1);
	}
	else
		CursorMoveLeft(PitchHeight, 1);

}


void UIEditBox::SelAll()
{
	if(!m_IsPassword)
	{
		End();
		m_SelStart = 0;
		m_SelEnd = m_InsertPos;
		FillImage();
		DrawFont();
	}
}



Int32 UIEditBox::OnChar(WPARAM wParam)
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	m_LastKey = 0;

	switch(wParam)
	{
		case VK_BACK:
			if(m_IsPassword)
			{
				m_SelStart = 0;
				m_SelEnd = (Int32)m_Text.size();
				DelSelText(PitchHeight, RealFontHeight);
				m_HashedPassword.Zero();
				FillImage();
			}
			else
			if(m_Text.size())
			{
				if(m_SelStart == m_SelEnd && m_InsertPos > 0)
				{
					BackSpaceKey(PitchHeight, RealFontHeight);
					
					if(m_InsertPos < m_Text.size())
					{
						Int	i = 1;
						if(m_Text[m_InsertPos] == 15)
						{
							while(m_Text[m_InsertPos + i] != 16 && m_InsertPos + i < m_Text.size())
							{
								++i;
							}
							if(m_Text[m_InsertPos + i] == 16)
								++i;
						}
						if(m_Text[m_InsertPos + i] == 17)
						{
							while(m_Text[m_InsertPos + i] != 18 && m_InsertPos + i < m_Text.size())
							{
								++i;
							}
							if(m_Text[m_InsertPos + i] == 18)
								++i;
						}
						/*
						std::vector<UInt32>::iterator it = m_CharWidth.begin() + m_InsertPos;
						m_CharWidth.erase(it, i + it);
						it = m_CharMark.begin() + m_InsertPos;
						m_CharMark.erase(it, i + it);
						*/
						m_Text.erase(m_InsertPos, i);
					}
					
				}
				else
				{
					DelSelText(PitchHeight, RealFontHeight);
				}

				FillImage();
				DrawFont();
			}
			
			break;
		case VK_RETURN:			//
			{
				m_LastKey = wParam;
			}	
			break;
			
		case VK_TAB:
			{
				m_LastKey = wParam;
			}
			break;//
			
		case 1:					// Ctrl-A Select All
			{
				SelAll();
			}
			break;
        case 22:				// Ctrl-V Paste
            {
				if(!m_IsPassword)
				{
					//PasteFromClipBoard();
					HANDLE		hMem;
					String		MyText;

					OpenClipboard(g_UIhWnd);

					if(hMem = GetClipboardData(CF_UNICODETEXT))
					{
						MyText = (PSTRINGW)GlobalLock(hMem);
						GlobalUnlock(hMem);
					}
					else
					if(hMem = GetClipboardData(CF_TEXT))
					{
						MyText = (LPSTR)GlobalLock(hMem);
						GlobalUnlock(hMem);
					}
					
					CloseClipboard();

					if(!MyText.empty())
					{
						ClearCtrlText(MyText);

						if(m_SelEnd != m_SelStart)
						{
							DelSelText(PitchHeight, RealFontHeight);
						}
						FillImage();
						//m_Text = MyText;
						for(UInt32 i = 0; i < MyText.size(); i++)
						{
							AddCharW(MyText[i], PitchHeight, RealFontHeight);
						}
						/*
						m_Text.insert(m_InsertPos++, MyText);
						CursorMoveRight(PitchHeight, RealFontHeight, MyText.size());
						*/
					}
				}
            }
			break;			
		case 24:				// Ctrl-X Cut
        case VK_CANCEL:			// Ctrl-C Copy
            {
				if(m_SelStart != m_SelEnd  && !m_IsPassword)
				{
					UInt32		SelStart = min(m_SelStart, m_SelEnd);
					UInt32		SelEnd = max(m_SelStart, m_SelEnd);
					UInt32		StrLen = (SelEnd - SelStart);
					UInt32		MemLen = (SelEnd - SelStart + 1) * 2;
					HANDLE		hMem;
					LPVOID		lpData;

			
					OpenClipboard(g_UIhWnd);
					EmptyClipboard();
					
					hMem = GlobalAlloc(GHND, MemLen);

					lpData = (LPSTR)GlobalLock(hMem);
					String TempStr = m_Text.substr(SelStart, StrLen);
					ClearCtrlText(TempStr);
					memcpy(lpData, TempStr.c_str(), StrLen * 2);
					GlobalUnlock(hMem);
					SetClipboardData(CF_UNICODETEXT, hMem);
					
					CloseClipboard();

					if(wParam == 24)
					{
						DelSelText(PitchHeight, RealFontHeight);
						FillImage();
						DrawFont();
					}
				}
            }
			break;

        case 2:   // Ctrl B
        case 4:   // Ctrl D
		case 5:   // Ctrl E
        case 6:   // Ctrl F
        case 7:   // Ctrl G
        //case 9:   // Ctrl I		tab
		case 10:  // Ctrl J
        case 11:  // Ctrl K
        case 12:  // Ctrl L
		case 14:  // Ctrl N
        case 15:  // Ctrl O
        case 16:  // Ctrl P
		case 17:  // Ctrl Q
		case 18:  // Ctrl R
		case 19:  // Ctrl S
        case 20:  // Ctrl T            
        case 21:  // Ctrl U
        case 23:  // Ctrl W
		case 25:  // Ctrl Y
		case 26:  // Ctrl Z
        case 27:  // Ctrl [
		case 28:  // Ctrl \ 
        case 29:  // Ctrl ]            
			break;
		case VK_SPACE:
			if(!m_NoSpace)
			{
				return AddCharW(wParam, PitchHeight, RealFontHeight);
			}
			break;
		default:
			{
				return AddCharW(wParam, PitchHeight, RealFontHeight);
			}
			break;
	}

	return 1;
}



void UIEditBox::SetRealRect(RECT &RRect)
{
	RRect.left = m_X;
	RRect.left -= 4;
	if (RRect.left < 0)
		RRect.left = 0;
	RRect.top = m_Y;
	RRect.right = m_X + m_Width + 4;
	RRect.bottom = m_Y + m_Height;
}

void UIEditBox::SetRealOffsetRect(RECT &RRect)
{
	RRect.left = m_OffsetX;
	RRect.left -= 4;
	if (RRect.left < 0)
		RRect.left = 0;
	RRect.top = m_OffsetY;
	RRect.right = m_OffsetX + m_Width + 4;
	RRect.bottom = m_OffsetY + m_Height;
}


UInt32 UIEditBox::Update(UInt32 dwDelta)
{
	if(m_Focus)
	{
		if (::timeGetTime() > EditCursorRefreshCount + m_EditCursorRefreshTime)
		{
			m_ShowEditCursor = !m_ShowEditCursor;
			RenderParent();
			m_EditCursorRefreshTime = ::timeGetTime();
			return 1;
		}
	}
	return 0;
}


Int32 UIEditBox::OnLButtonDown(Int32 pX, Int32 pY)
{
	bool ret = UIItem::Detect(pX, pY);
	
	if(ret)
	{
		UInt32	PitchHeight, RealFontHeight;
		m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

		
		UInt32	DrawWidth = 0;
		Int32	TempCursorX = -CursorStartX;
		Int32	MouseX = pX - m_X;
		Int32	MouseOffset = 800;

		m_CursorX = TempCursorX;
		m_InsertPos = m_StartPoint;

		UInt32 TempInsertPos = m_StartPoint;
		while(m_InsertPos < m_Text.size() && m_CursorX < pX - m_X - 10)
			CursorMoveRight(PitchHeight, RealFontHeight, 1);

		if(m_StartPoint)
		{
			for(Int i = m_StartPoint; i < m_Text.size(); ++i)
			{
				if(m_Text[i] == 15)
				{
					break;
				}
				else
				if(m_Text[i] == 18 && i > m_InsertPos)
				{
					while(m_InsertPos < m_Text.size())
					{
						CursorMoveRight(PitchHeight, RealFontHeight, 1);
						if(m_Text[m_InsertPos - 2] == 18)
						{
							CursorMoveLeft(PitchHeight, 1);
							break;
						}
					}
					break;
				}
			}
		}

		{
			m_SelStart = m_SelEnd = m_InsertPos;
			SetChange(true);

			FillImage();
			DrawFont();
		}

		m_PickUp = true;

		if (m_pUIManager)
		{
			m_pUIManager->SetKeyInItem(m_Name.c_str());
		}
	}

	return ret;
}


Int32 UIEditBox::OnLButtonUp(Int32 pX, Int32 pY)
{
	m_PickUp = false;
	return 1;
}


Int32 UIEditBox::OnMouseMove(Int32 pX, Int32 pY)
{
	bool ret = UIItem::Detect(pX, pY);

	if(ret && m_PickUp)
	{
		UInt32	PitchHeight, RealFontHeight;
		m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

		UInt32	DrawWidth = 0;
		Int32	TempCursorX = -CursorStartX;
		Int32	MouseX = pX - m_X;
		Int32	MouseOffset = 800;

		m_CursorX = TempCursorX;
		m_InsertPos = m_StartPoint;

		UInt32 TempInsertPos = m_StartPoint;
		while(m_InsertPos < m_Text.size() && m_CursorX < pX - m_X - 10)
			CursorMoveRight(PitchHeight, RealFontHeight, 1);

		if(m_StartPoint)
		{
			for(Int i = m_StartPoint; i < m_Text.size(); ++i)
			{
				if(m_Text[i] == 15)
				{
					break;
				}
				else
				if(m_Text[i] == 18 && i > m_InsertPos)
				{
					while(m_InsertPos < m_Text.size())
					{
						CursorMoveRight(PitchHeight, RealFontHeight, 1);
						if(m_Text[m_InsertPos - 2] == 18)
						{
							CursorMoveLeft(PitchHeight, 1);
							break;
						}
					}
					break;
				}
			}
		}

	
		if(m_SelEnd != m_InsertPos)
		{
			m_SelEnd = m_InsertPos;
			SetChange(true);

			FillImage();
			DrawFont();
		}
	}

	return 1;
}

void UIEditBox::SetFocus(bool pState)
{
	if (pState != m_Focus)
		RenderParent();
	m_Focus = pState;
	if(pState == false)
	{
		m_SelStart = m_SelEnd = 0;
		DrawFont();
	}
}

void UIEditBox::SetNotMouseOver()
{
	
}



void UIEditBox::BackSpace()
{
	OnChar(VK_BACK);
}



////////////////////////////////////////////////////////



Int32 UIImage::OnLButtonDown(Int32 pX, Int32 pY)
{
	if(UIItem::Detect(pX, pY))
	{
		if (IsDelectImage() && m_BackImage)
		{
			PUInt32	PixelPtr = (PUInt32)(m_BackImage->GetPtrXY(m_BackRect.left + (pX - m_X), m_BackRect.top + (pY - m_Y)));

			if(*PixelPtr)
				return 1;
		}
		else
			return 1;
	}
	
	return 0;
}


void UIImage::Render(CImage *pImage)
{
	if(m_BackImage)
	{
		Alpha8888To8888S(m_OffsetX, m_OffsetY, m_BackImage, pImage, &m_BackRect, m_RenderStyle, m_Color);
	}
}


void UIImage::Render(CImage *pImage, RECT &RRect)
{
	if(m_BackImage)
	{
		Int32		X = m_OffsetX, Y = m_OffsetY;
		RECT	SRect;

		memcpy(&SRect, &m_BackRect, sizeof(RECT));
		if(CImageDirtyRect(X, Y, SRect, RRect))
		{
			Alpha8888To8888S(X, Y, m_BackImage, pImage, &SRect, m_RenderStyle, m_Color);
		}
	}
}




void UIImage::SetImage(CImage *pImage, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	m_BackImage = pImage;
	//m_DetectImage = (pImage ? true : false);
	SetRect(&m_BackRect, pLeft, pTop, pRight, pBottom);
	m_Width = pRight - pLeft;
	m_Height = pBottom - pTop;
}


void UIImage::SetImage(UIImage *pUIImage)
{
	if(pUIImage)
	{
		m_BackImage = pUIImage->m_BackImage;
		memcpy(&m_BackRect, &pUIImage->m_BackRect, sizeof(RECT));
		m_Width = pUIImage->m_Width;
		m_Height = pUIImage->m_Height;
	}
	else
	{
		m_BackImage = NULL;
	}
}


void UIImage::SetImageFileA(PCSTRINGA Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	String	StrW = Filename;

	SetImageFile(StrW.c_str(), pLeft, pTop, pRight, pBottom);
}



void UIImage::SetImageFile(PCSTRING Filename, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	if(m_BackImage)
	{
		//if(m_AutoDeleteImage)
		CImageManager::GetInstance().DeleteImage(m_BackImage);
		m_BackImage = NULL;	
	}
	m_BackImage = CImageManager::GetInstance().CreateImage(Filename, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	
	if(pRight && pBottom)
	{
		SetRect(&m_BackRect, pLeft, pTop, pRight, pBottom);
		m_Width = pRight - pLeft;
		m_Height = pBottom - pTop;
	}
	else
	{
		if( !m_BackImage )
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("UI: SetImageFile() File Not Loaded, filename=%s"), Filename);
			return;
		}

		m_Width = m_BackImage->GetWidth();
		m_Height = m_BackImage->GetHeight();
		SetRect(&m_BackRect, 0, 0, m_Width, m_Height);
	}
}


/*
void UIImage::LoadCZipMem(Byte *ZipBuf, UInt ZipSize)
{
	String palFilename(_T("Data\\Palette.Dat"));;
	UInt32 Palette[256];
	if( g_GdfResource )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = g_GdfResource->GetMapViewOfFile(palFilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			memcpy(Palette, buf, 1024);
			UnmapViewOfFile(fbuf);
		}else
		{
			TRACE_WARNDTL(GLOBAL_LOGGER, _F("UIImage::LoadCZipMem Warning (1) [Data\\Palette.Dat in Patch not found]"));
			return;
		}
	}else
	{
		FILE *PalFile;
		if( PalFile = _tfopen(palFilename.c_str(), _T("r")) )
		{
			fread(Palette, 1024, 1, PalFile);
			fclose(PalFile);
		}else
		{
			TRACE_WARNDTL(GLOBAL_LOGGER, _F("UIImage::LoadCZipMem Warning (2) [Data\\Palette.Dat not found]"));
			return;
		}
	}

	Byte	RealBuf[50 * 52];
	ZLib::uLongf	DataLen = 50 * 52;

	ZLib::uncompress(RealBuf, &DataLen, ZipBuf, ZipSize); 

	if(m_BackImage)
		CImageManager::GetInstance().DeleteImage(m_BackImage);
	m_BackImage = CImageManager::GetInstance().CreateImage(100, 100, 8888);

	Byte *PicBits = m_BackImage->GetPtr() + 49 * 2 * m_BackImage->GetPitch();
	Byte *PalIndex = RealBuf;
	for(Int i = 0; i < 50; ++i)
	{
		Byte *Pixel = PicBits;
		
		for(Int j = 0 ; j < 50; j++)
		{
			Int	Index = RealBuf[i * 52 + j];
			UInt MyPixel;
			
			MyPixel = Palette[Index];
			
			mem4set(Pixel, MyPixel, 2);
			mem4set(Pixel + m_BackImage->GetPitch(), MyPixel, 2);

			Pixel += 8;
		}

		PicBits -= m_BackImage->GetPitch() * 2;
	}

}

*/


void UIImage::LoadCZipMem(Byte *ZipBuf, UInt ZipSize)
{
	String palFilename(_T("Data\\Palette.Dat"));;
	UInt32 Palette[256];
	if( g_GdfResource || g_GdfResourceEx )
	{
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(palFilename, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(palFilename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			memcpy(Palette, buf, 1024);
			UnmapViewOfFile(fbuf);
		}else
		{
			TRACE_WARNDTL(GLOBAL_LOGGER, _F("UIImage::LoadCZipMem Warning (1) [Data\\Palette.Dat in Patch not found]"));
			return;
		}
	}else
	{
		FILE *PalFile;
		if( PalFile = _tfopen(palFilename.c_str(), _T("rb")) )
		{
			fread(Palette, 1024, 1, PalFile);
			fclose(PalFile);
		}else
		{
			TRACE_WARNDTL(GLOBAL_LOGGER, _F("UIImage::LoadCZipMem Warning (2) [Data\\Palette.Dat not found]"));
			return;
		}
	}

	Byte	RealBuf[50 * 52];
	ZLib::uLongf	DataLen = 50 * 52;

	ZLib::uncompress(RealBuf, &DataLen, ZipBuf, ZipSize); 

	if(m_BackImage)
		CImageManager::GetInstance().DeleteImage(m_BackImage);
	m_BackImage = CImageManager::GetInstance().CreateImage(200, 200, 8888);

	Byte *PicBits = m_BackImage->GetPtr() + 49 * 4 * m_BackImage->GetPitch();
	Byte *PalIndex = RealBuf;
	for(Int i = 0; i < 50; ++i)
	{
		Byte *Pixel = PicBits;
		
		for(Int j = 0 ; j < 50; j++)
		{
			Int	Index = RealBuf[i * 52 + j];
			UInt MyPixel;
			
			MyPixel = Palette[Index];
			
			mem4set(Pixel, MyPixel, 4);
			mem4set(Pixel + m_BackImage->GetPitch(), MyPixel, 4);
			mem4set(Pixel + m_BackImage->GetPitch() * 2, MyPixel, 4);
			mem4set(Pixel + m_BackImage->GetPitch() * 3, MyPixel, 4);

			Pixel += 16;
		}

		PicBits -= m_BackImage->GetPitch() * 4;
	}

}


void UIImage::LoadCZipImage(PCSTRINGA Filename)
{
	

	FILE *ZipFile = fopen(Filename, "rb");
	fseek(ZipFile, 0, SEEK_END);
	Int	ZipSize = ftell(ZipFile);
	fseek(ZipFile, 0, SEEK_SET);
	
	BYTE *ZipBuf = SafeAllocate(BYTE, ZipSize);
	fread(ZipBuf, ZipSize, 1, ZipFile);
	fclose(ZipFile);

	LoadCZipMem(ZipBuf, ZipSize);

	SafeDeallocate(ZipBuf);

}


////////////////////////////////////////////////////////

void UILinerImage::SetRealRect(RECT &RRect)
{
	RRect.left = m_X;
	RRect.top = m_Y;
	RRect.right = m_X + m_Liner;
	RRect.bottom = m_Y + m_Height;
}

void UILinerImage::SetRealOffsetRect(RECT &RRect)
{
	RRect.left = m_OffsetX;
	RRect.top = m_OffsetY;
	RRect.right = m_X + m_Liner;
	RRect.bottom = m_Y + m_Height;
}
void UILinerImage::SetDenumerator(UInt32 Denumerator)
{
	RenderParent();
	m_Denumerator = Denumerator;

	RenderParent();
	
}

void UILinerImage::SetNumerator(UInt32 Numerator)
{
	RenderParent();
	m_Numerator = Numerator;
	m_Liner = (UInt32)((UInt64)m_BaseLiner * m_Numerator / m_Denumerator);

	RenderParent();
}

void UILinerImage::Render(CImage *pImage, RECT &RRect)
{
	Int32		X = m_OffsetX, Y = m_OffsetY;
	Int32		Liner = 1;
	RECT	SRect;

	
	if(m_BackImage && m_Numerator && m_Liner > 0)
	{
		memcpy(&SRect, &m_BackRect, sizeof(RECT));
		SRect.right = SRect.left + m_Liner;
		if(CImageDirtyRect(X, Y, SRect, RRect))
		{
			Liner = SRect.right - SRect.left;
			SRect.left = m_BackRect.left;
			
			Liner8888To8888(X, Y, Liner, m_BackImage, pImage, &SRect);
		}		
	}
	
}


////////////////////////////////////////////////////////

void UIBarImage::Render(CImage *pImage)
{
	if(m_BackImage)
		if(m_Liner > 0)
		{
			RECT	rect = m_BackRect;
			Alpha8888To8888S(m_OffsetX, m_OffsetY, m_BackImage, pImage, &m_BackRect, m_RenderStyle, m_Color);
		}
}

void UIBarImage::Render(CImage *pImage, RECT &RRect)
{

}



////////////////////////////////////////////////////////


void UILinerImageV::Render(CImage *pImage, RECT &RRect)
{
	Int32		X = m_OffsetX, Y = m_OffsetY;
	Int32		Liner = 1;
	RECT	SRect;

	if(m_BackImage && m_Liner > 0)
	{
		memcpy(&SRect, &m_BackRect, sizeof(RECT));
		SRect.bottom = SRect.top + m_Liner;
		if(CImageDirtyRect(X, Y, SRect, RRect))
		{
			Liner = SRect.bottom - SRect.top;
			SRect.top = m_BackRect.top;
			
			Liner8888To8888V(X, Y, Liner, m_BackImage, pImage, &SRect);
		}		
	}
}


////////////////////////////////////////////////////////

bool UITips::Detect(Int32 pX, Int32 pY)
{
	if(UIItem::Detect(pX, pY))
	{
		if(m_OldX != pX || m_OldY != pY)
		{
			m_OldX = pX;
			m_OldY = pY;
		}
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////


void UIMiniMap::SetImageA(PCSTRINGA Filename)
{
	String	StrW = Filename;

	SetImage(StrW.c_str());
}



void UIMiniMap::SetImage(PCSTRING Filename)
{
	if(m_MapImage)
		CImageManager::GetInstance().DeleteImage(m_MapImage);
	m_MapImage = CImageManager::GetInstance().CreateImage(Filename, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
}

void UIMiniMap::SetImage(CImage *pImage)
{
	if(m_MapImage != pImage)
	{
		if(m_MapImage)
			CImageManager::GetInstance().DeleteImage(m_MapImage);
		m_MapImage = pImage;
	}
}


void UIMiniMap::SetPlayerImageA(PCSTRINGA	Filename)
{
	String	StrW = Filename;

	if(m_PlayerImage)
		CImageManager::GetInstance().DeleteImage(m_PlayerImage);
	m_PlayerImage = CImageManager::GetInstance().CreateImage(StrW.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
}


void UIMiniMap::SetPlayerImage(CImage *pImage)
{ 
	if(m_PlayerImage != pImage)
	{
		if(m_PlayerImage)
			CImageManager::GetInstance().DeleteImage(m_PlayerImage);
		m_PlayerImage = pImage; 
	}
}


void UIMiniMap::SetDefaultMask()
{
	CImage *MaskImage = CImageManager::GetInstance().CreateImage(_T("Data\\Mask.TGA"), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	SetMask(MaskImage);
}

void UIMiniMap::DrawIcon(CImage* pSrcImage, RECT iconRect, Int PixelX, Int PixelY)
{
	if (pSrcImage && m_MapImage)
	{
		Int startX = (Int)(PixelX / m_WRatio - (iconRect.right - iconRect.left) / 2 );
		Int startY = (Int)(PixelY / m_HRatio - (iconRect.bottom - iconRect.top) / 2 );

		if (startX < 0)
			startX = 0;
		if (startY < 0)
			startY = 0;

		int width = iconRect.right - iconRect.left;
		int height = iconRect.bottom - iconRect.top;

		Alpha8888To8888(startX, startY, pSrcImage, m_MapImage, &iconRect);

		/*
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				UInt32 *srcBit = (UInt32 *)(pSrcImage->GetPtrXY(iconRect.left + i, iconRect.top + j));
				UInt32 *desBit = (UInt32 *)(m_MapImage->GetPtrXY(startX + i, startY + j));

				//if (srcBit[0] != 0)
					desBit[0] = CRender::MMXAlpha(srcBit[0], desBit[0]);
				//desBit[0] = srcBit[0];
			}
		}
		*/
	}
	
}


UInt UIMiniMap::ResetPlayerPos(UInt X, UInt Y)
{
	if ( (m_WRatio >= 0 && m_WRatio < 0.2 ) || m_HRatio >= 0 && m_HRatio < 0.2)
		return 0;
	UInt32 TempCenterX = (UInt32)(X / m_WRatio);
	UInt32 TempCenterY = (UInt32)(Y / m_HRatio);

	if(m_CenterX != TempCenterX || m_CenterY != TempCenterY)
	{
		
		m_CenterX = TempCenterX;
		m_CenterY = TempCenterY;

		m_MapX = m_CenterX - m_Width / 2;
		m_MapY = m_CenterY - m_Height / 2;

		
		if(m_MapX < 0)
		{
			m_MapX = 0;
		}
		else
		if(m_MapX + m_Width >= (Int32)(m_MapImage->GetWidth()))
		{
			m_MapX = m_MapImage->GetWidth() - m_Width;
			
		}
		

		if(m_MapY < 0)
		{
			m_MapY = 0;
		}
		else
		if(m_MapY + m_Height >= (Int32)(m_MapImage->GetHeight()))
		{
			m_MapY = m_MapImage->GetHeight() - m_Height;
		}

		Int32 TempMapX = m_MapX + (m_PlayerRect.right - m_PlayerRect.left) / 8;
		Int32 TempMapY = m_MapY + (m_PlayerRect.bottom - m_PlayerRect.top) / 4;

 		m_PlayerX = m_CenterX;
		m_PlayerY = m_CenterY;
		//calculate the radius of current position and the center
		UInt Radius = m_Width / 2 - max((m_PlayerRect.right - m_PlayerRect.left) / 2, 
			(m_PlayerRect.bottom - m_PlayerRect.top) / 2);
		PixelDirection Dir;
		UInt Diff = QGLMath::CalculatePixelPosDistance(//PixelPos((m_MapX + m_Width / 2), (m_MapY + m_Height / 2)),
			PixelPos((TempMapX + m_Width / 2), (TempMapY + m_Height / 2)),
			PixelPos(m_PlayerX, m_PlayerY), Dir);
		if (Diff > Radius)
		{
			m_PlayerX = (Int32)(TempMapX + m_Width / 2 + Radius * Dir.PixelX);
			m_PlayerY = (Int32)(TempMapY + m_Height / 2 + Radius * Dir.PixelY);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

void UIMiniMap::Render(CImage *pImage)
{
	UInt8	*SPtr = m_MapImage->GetPtr() + m_MapY * m_MapImage->GetPitch() + m_MapX * 4;
	UInt8	*MPtr = m_Mask->GetPtr();
	UInt8	*DPtr = pImage->GetPtr() + m_OffsetY * pImage->GetPitch() + m_OffsetX * 4;

	UInt32	Width = m_Mask->GetWidth();
	UInt32	Height = m_Mask->GetHeight();

	UInt32	SPitch = m_MapImage->GetPitch() - Width * 4;
	UInt32	MPitch = m_Mask->GetPitch() - Width * 4;
	UInt32	DPitch = pImage->GetPitch() - Width * 4;


	__asm
	{
		pushad

		mov		esi, SPtr
		mov		edi, DPtr
		mov		edx, MPtr

		mov		ecx, Height

Label_Loop_1:

		push	ecx

		mov		ecx, Width
				
Label_Loop_2:

		lodsd
		and		eax, DWORD	PTR [edx]
		stosd
		add		edx, 4
		loop	Label_Loop_2


		add		esi, SPitch
		add		edi, DPitch
		add		edx, MPitch

		pop		ecx

		loop	Label_Loop_1


		popad

		
		
	}

	
	if (m_PlayerImage)
	{
		Alpha8888To8888S(m_OffsetX + (m_PlayerX - m_MapX) - (m_PlayerRect.right - m_PlayerRect.left) / 2, 
			m_OffsetY + (m_PlayerY - m_MapY) - (m_PlayerRect.bottom - m_PlayerRect.top), m_PlayerImage, pImage, 
			&m_PlayerRect, m_RenderStyle, m_Color);
	}
	
/*
	for(UInt i = 0;  i < m_Mask->GetHeight(); ++i)
	{
		UInt32	*SPtr32 = SPtr;
		UInt32	*MPtr32 = MPtr;
		UInt32	*DPtr32 = DPtr;		

		for(UInt j = 0; j < m_Mask->GetWidth();  ++j)
		{
			//if(MPtr32[j])
				DPtr32[j] = MPtr32[j] & SPtr32[j];
		}
	
		SPtr += m_MapImage->GetPitch();
		DPtr += pImage->GetPitch();
		MPtr += m_Mask->GetPitch();
		
		
		SPtr += SPitch;
		DPtr += DPitch;
		MPtr += MPitch;
		
	}
*/	
	
}


void UIMiniMap::Render(CImage *pImage, RECT &RRect)
{
	Int32		X = m_OffsetX, Y = m_OffsetY;
	RECT	SRect;

	SetRect(&SRect, 0, 0, m_Mask->GetWidth(), m_Mask->GetHeight());
	if(!CImageDirtyRect(X, Y, SRect, RRect))
	{
		return;
	}
	
	UInt8	*SPtr = m_MapImage->GetPtr() + (m_MapY + SRect.top) * m_MapImage->GetPitch() + (m_MapX + SRect.left) * 4;
	UInt8	*MPtr = m_Mask->GetPtr() + SRect.top * m_Mask->GetPitch() + SRect.left * 4;
	UInt8	*DPtr = pImage->GetPtr() + Y * pImage->GetPitch() + X * 4;

	UInt32	Width = SRect.right - SRect.left;
	UInt32	Height = SRect.bottom - SRect.top;

	UInt32	SPitch = m_MapImage->GetPitch() - Width * 4;
	UInt32	MPitch = m_Mask->GetPitch() - Width * 4;
	UInt32	DPitch = pImage->GetPitch() - Width * 4;


	__asm
	{
		pushad

		mov		esi, SPtr
		mov		edi, DPtr
		mov		edx, MPtr

		mov		ecx, Height

Label_Loop_1:

		push	ecx

		mov		ecx, Width
				
Label_Loop_2:

		lodsd
		and		eax, DWORD	PTR [edx]
		stosd
		add		edx, 4
		loop	Label_Loop_2


		add		esi, SPitch
		add		edi, DPitch
		add		edx, MPitch

		pop		ecx

		loop	Label_Loop_1


		popad

		
		
	}

	
	if (m_PlayerImage)
	{
		RECT SrcRect;
		SetRect(&SrcRect, m_OffsetX + (m_PlayerX - m_MapX) - (m_PlayerRect.right - m_PlayerRect.left) / 2
			, m_OffsetY + (m_PlayerY - m_MapY) - (m_PlayerRect.bottom - m_PlayerRect.top), 
			m_OffsetX + (m_PlayerX - m_MapX) + (m_PlayerRect.right - m_PlayerRect.left) / 2, 
			m_OffsetY + (m_PlayerY - m_MapY));
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(SrcRect, RRect, ResultRect))
		{
			RECT Final = m_PlayerRect;
			Final.left += ResultRect.left - SrcRect.left;
			Final.top += ResultRect.top - SrcRect.top;
			Final.right -= SrcRect.right - ResultRect.right;
			Final.bottom -= SrcRect.bottom - ResultRect.bottom;
			Alpha8888To8888S(ResultRect.left, 
			ResultRect.top, m_PlayerImage, pImage, &Final, m_RenderStyle, m_Color);
		}
	}
}


////////////////////////////////////////////////////////
// IMM






/////////////////////////////////////////////////////////////////////////////




////////////////UIAniIcon////////////////////
bool UIAniIcon::GetHasAni()
{
	return m_AniEnt.HasAniFile() == TRUE;
}

void UIAniIcon::SetRealRect(RECT &RRect)
{
	Int32 LeftUpPosX = m_X + m_AniEnt.GetEntOffsetX();
	Int32 LeftUpPosY = m_Y + m_AniEnt.GetEntOffsetY();

	RRect.left = LeftUpPosX + m_AniEnt.m_EntityTotalRect.left;
	RRect.top = LeftUpPosY + m_AniEnt.m_EntityTotalRect.top;
	RRect.right = RRect.left + m_AniEnt.m_EntityTotalRect.right - m_AniEnt.m_EntityTotalRect.left;
	RRect.bottom = RRect.top + m_AniEnt.m_EntityTotalRect.bottom - m_AniEnt.m_EntityTotalRect.top;

	QGLMath::CalculateBiggestRect(RRect, m_PreDirtyRect, RRect);
}

void UIAniIcon::SetRealOffsetRect(RECT &RRect)
{
	Int32 LeftUpPosX = m_OffsetX + m_AniEnt.GetEntOffsetX();
	Int32 LeftUpPosY = m_OffsetY + m_AniEnt.GetEntOffsetY();

	RRect.left = LeftUpPosX + m_AniEnt.m_EntityTotalRect.left;
	RRect.top = LeftUpPosY + m_AniEnt.m_EntityTotalRect.top;
	RRect.right = RRect.left + m_AniEnt.m_EntityTotalRect.right - m_AniEnt.m_EntityTotalRect.left;
	RRect.bottom = RRect.top + m_AniEnt.m_EntityTotalRect.bottom - m_AniEnt.m_EntityTotalRect.top;

	QGLMath::CalculateBiggestRect(RRect, m_PreDirtyRect, RRect);
}
UInt32 UIAniIcon::Update(UInt32 dwDelta)
{
	m_AniEnt.UpDate(::timeGetTime());

	BOOL result = m_AniEnt.m_Updated;
	m_AniEnt.m_Updated = 0;

	if(result)
	{
		RenderParent();
	}

	return result;
}

UInt32 UIAniIcon::UpdateNow()
{
	return Update(::timeGetTime());
}


void UIAniIcon::Render(CImage *pImage)
{
	DrawEntityToImage(pImage, NULL);
}

void UIAniIcon::CleanPreDirty(CImage *pImage)
{
	if (pImage == NULL)
		return;

	Byte * pDesTData = (Byte *)pImage->GetPtr() + m_PreDirtyRect.top * pImage->GetPitch() +
		m_PreDirtyRect.left * sizeof(Color32);
	//clear the previous rect 
	UInt copyLen = (m_PreDirtyRect.right - m_PreDirtyRect.left) * sizeof(Color32);
	
	for (Int y = 0; y < m_PreDirtyRect.bottom - m_PreDirtyRect.top ; y++)
	{
		ZeroMemory(pDesTData, copyLen);
		pDesTData += pImage->GetPitch();
	}
}
void UIAniIcon::DrawEntityToImage(CImage *pImage, RECT * Rect)
{
	RECT NextDirtyRegion;

	if (Rect)
		NextDirtyRegion = *Rect;
	else 
	{
		NextDirtyRegion.left = NextDirtyRegion.top = 0;
		NextDirtyRegion.right = pImage->GetWidth();
		NextDirtyRegion.bottom = pImage->GetHeight();
	}

	if (NextDirtyRegion.left < 0)
		NextDirtyRegion.left = 0;
	if (NextDirtyRegion.top < 0)
		NextDirtyRegion.top = 0;
	if (NextDirtyRegion.bottom < 0)
		NextDirtyRegion.bottom = 0;
	if (NextDirtyRegion.right < 0)
		NextDirtyRegion.right = 0;
	Int32 LeftUpPosX = m_OffsetX + m_AniEnt.GetEntOffsetX();
	Int32 LeftUpPosY = m_OffsetY + m_AniEnt.GetEntOffsetY();

	m_PreDirtyRect = NextDirtyRegion;

	RECT rect2;
	rect2.left = LeftUpPosX + m_AniEnt.m_EntityTotalRect.left;
	/*if (rect2.left < 0)
	{
		LeftUpPosX -= rect2.left;
		rect2.left = 0;
	}*/
	rect2.top = LeftUpPosY + m_AniEnt.m_EntityTotalRect.top;
	/*if (rect2.top < 0)
	{
		LeftUpPosY -= rect2.top;
		rect2.top = 0;
	}*/
	rect2.right = rect2.left + m_AniEnt.m_EntityTotalRect.right - m_AniEnt.m_EntityTotalRect.left;
	rect2.bottom = rect2.top + m_AniEnt.m_EntityTotalRect.bottom - m_AniEnt.m_EntityTotalRect.top;

	if(!QGLMath::CalculateOverlappedRect(NextDirtyRegion, rect2, NextDirtyRegion))
		return;

	

	D3DSURFACE_DESC  pDescDes;

	pDescDes.Width = pImage->GetWidth();
	pDescDes.Height = pImage->GetHeight();
	
	RECT TotalEntLockRect;
	TotalEntLockRect.left = NextDirtyRegion.left - LeftUpPosX;
	TotalEntLockRect.right = TotalEntLockRect.left + m_AniEnt.m_EntityTotalRect.right - m_AniEnt.m_EntityTotalRect.left;
	TotalEntLockRect.top = NextDirtyRegion.top - LeftUpPosY ;
	TotalEntLockRect.bottom = TotalEntLockRect.top + m_AniEnt.m_EntityTotalRect.bottom - m_AniEnt.m_EntityTotalRect.top;

	Byte * pDesTData = (Byte *)pImage->GetPtr() + NextDirtyRegion.top * pImage->GetPitch() + NextDirtyRegion.left * sizeof(Color32);
	
	for (int i = 0; i < m_AniEnt.GetTotalRenderPart() ; i++)
	{
		BOOL Flip = m_AniEnt.GetFlip(i);
		for (Int j = 0; j < 2; j ++)
		{
			BOOL Get = FALSE;

			if (j == 0)
				Get = m_AniEnt.GetEntityPart(i, ShadowPic, NULL);
			else if ( j == 1)
				Get = m_AniEnt.GetEntityPart(i, ImagePic, NULL);
			
			if(Get)
			{
				RECT CurrRect = m_AniEnt.GetCurrRect();
				Int ObjectLeftUpX = LeftUpPosX
					+ CurrRect.left ;

				Int ObjectLeftUpY = LeftUpPosY
					+ CurrRect.top ;

				RECT rect2;
				rect2.left = ObjectLeftUpX;
				rect2.top = ObjectLeftUpY;
				rect2.right = ObjectLeftUpX + CurrRect.right - CurrRect.left;
				rect2.bottom = ObjectLeftUpY + CurrRect.bottom - CurrRect.top;

				RECT resultRect;

				if (QGLMath::CalculateOverlappedRect(NextDirtyRegion,rect2, resultRect))
				{
					RECT SubEntLockRect;

					SubEntLockRect.left = resultRect.left - rect2.left + CurrRect.left;
					SubEntLockRect.right = resultRect.right - rect2.left + CurrRect.left;

					if (Flip)
					{
						SubEntLockRect.left -= resultRect.left - rect2.left;
						SubEntLockRect.left += rect2.right - resultRect.right;
						SubEntLockRect.right -= resultRect.left - rect2.left;
						SubEntLockRect.right += rect2.right - resultRect.right;
					}

					SubEntLockRect.top = resultRect.top - rect2.top + CurrRect.top;
					SubEntLockRect.bottom = resultRect.bottom - rect2.top  + CurrRect.top;

					D3DSURFACE_DESC  pDescSrc;

					//check invalid lock
					if (QGLMath::InvalidLock(&SubEntLockRect, MaxEntSizeX, MaxEntSizeY))
						continue;
				
					pDescSrc.Width =  m_AniEnt.GetPicWidth();
					pDescSrc.Height = MaxEntSizeY;

					Byte * pSrcTData =  m_AniEnt.GetPicData();

					//Int StartIndex = 0;
					//if (rect2.left < 0)
					//	pSrcTData -= rect2.left * 4;
					//else
						pSrcTData += (resultRect.left - ObjectLeftUpX) * 4;
					//if (rect2.top < 0)
					//	pSrcTData -= rect2.top * pDescSrc.Width * 4;
					//else
						pSrcTData += (resultRect.top - ObjectLeftUpY) * pDescSrc.Width * 4;



					if (m_AniEnt.GetHighLight())
					{
						Byte * pTempSrcTData = pSrcTData;
						Int Len = (SubEntLockRect.right - SubEntLockRect.left) * sizeof(Color32);
						for (Int j = 0; j < SubEntLockRect.bottom - SubEntLockRect.top; j ++)
						{
							for (Int i = 0; i < SubEntLockRect.right - SubEntLockRect.left; i ++)
							{
								pTempSrcTData[0] = gGlobal.AddtitiveTable[pTempSrcTData[0]][HighLightValue];
								pTempSrcTData[1] = gGlobal.AddtitiveTable[pTempSrcTData[1]][HighLightValue];
								pTempSrcTData[2] = gGlobal.AddtitiveTable[pTempSrcTData[2]][HighLightValue];
								pTempSrcTData += sizeof(Color32);
							}
							pTempSrcTData += pDescSrc.Width * sizeof(Color32) - Len;
						}
					}

					Int SrcStartIndex = 0;

					Int DesStartIndex = (SubEntLockRect.top - TotalEntLockRect.top) * pDescDes.Width + 
						(SubEntLockRect.left - TotalEntLockRect.left);
					if (Flip)
					{
						DesStartIndex += resultRect.left - rect2.left;
						DesStartIndex -= rect2.right - resultRect.right;
					}

					if (m_AniEnt.m_CurrPartBlitType == NoEffect)
					{

						if (m_RenderStyle == 0)
						{
							CRender::BlitPic(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
							(UInt16)(SubEntLockRect.bottom - SubEntLockRect.top ), pDescSrc.Width,
							pDesTData, DesStartIndex, (UInt16)pDescDes.Width, 
							Flip, m_AniEnt.GetAlphaOffset(), sizeof(Color32), sizeof(Color32));
						}
						else
						if (m_RenderStyle == 1)
						{
							CRender::BlitPicBW(pSrcTData, 0, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
							(UInt16)(SubEntLockRect.bottom - SubEntLockRect.top ), pDescSrc.Width,
							pDesTData, DesStartIndex, (UInt16)pDescDes.Width, 
							Flip, m_AniEnt.GetAlphaOffset(), sizeof(Color32), sizeof(Color32));
						}

					}
					/*else if (m_AniEnt.m_CurrPartBlitType == Additive)
					CRender::BlitAdditivePic(pSrcTData, SrcStartIndex, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
						(UInt16)(SubEntLockRect.bottom - SubEntLockRect.top ), pDescSrc.Width,
						pDesTData, DesStartIndex, (UInt16)pDescDes.Width, Flip, sizeof(Color32));
					else if (m_AniEnt.m_CurrPartBlitType == Subtractive)
						CRender::BlitSubtractivePic(pSrcTData, SrcStartIndex, (UInt16)(SubEntLockRect.right - SubEntLockRect.left ), 
						(UInt16)(SubEntLockRect.bottom -SubEntLockRect.top ), pDescSrc.Width,
						pDesTData, DesStartIndex, (UInt16)pDescDes.Width, 
						Flip, FALSE, sizeof(Color32));
						*/
				}
			}
		}
	}

}
void UIAniIcon::Render(CImage *pImage, RECT &RRect)
{
	DrawEntityToImage(pImage, &RRect);
	
}
void UIAniIcon::SetAnimationData(UIAniIcon * pAniIcon)
{
	if (pAniIcon == NULL)
		return;

	m_AniEnt = *pAniIcon->GetAniEnt();
	m_AniEnt.RemoveAllChildren();
	m_AniEnt.m_pFollowerEntLst.clear();
	m_AniEnt.m_pFollowTargetEnt = NULL;
	//m_AnimationIndex = pAniEnt->GetAnimationIndex();
	//m_Rotation = pAniEnt->GetAnimationRotation();

	SetWidth(m_AniEnt.m_EntOrgWidth);
	SetHeight(m_AniEnt.m_EntOrgHeight);
	ResetPosition();
	m_AniEnt.m_Updated = TRUE;
}
void UIAniIcon::SetAnimationData(LPCTSTR Filename, int equipType)
{
	m_AniEnt.InitEntPicInfo();
	m_AniEnt.LoadEntityFile(Filename, QGLApplication::GetInstance().GetRenderClass()->GetDevice(),
		(Equip_Type)equipType);
	SetWidth(m_AniEnt.m_EntOrgWidth);
	SetHeight(m_AniEnt.m_EntOrgHeight);
	m_AniEnt.UpDate(::timeGetTime());
	ResetPosition();
	m_AniEnt.m_Updated = TRUE;

}

void UIAniIcon::SetAnimationData(CAnimatedEntity * pAniEnt)
{
	if (pAniEnt == NULL)
		return;

	m_AniEnt = *pAniEnt;
	m_AniEnt.RemoveAllChildren();
	m_AniEnt.m_pFollowerEntLst.clear();
	m_AniEnt.m_pFollowTargetEnt = NULL;
	//m_AnimationIndex = pAniEnt->GetAnimationIndex();
	//m_Rotation = pAniEnt->GetAnimationRotation();

	SetWidth(m_AniEnt.m_EntOrgWidth);
	SetHeight(m_AniEnt.m_EntOrgHeight);
	ResetPosition();
	m_AniEnt.m_Updated = TRUE;
	
}

void UIAniIcon::SetAlign(Int Value)
{
	m_Align = Value;
	ResetPosition();
}

void UIAniIcon::ResetPosition()
{
	if (m_Align == 1)
	{
		m_AniEnt.SetEntOffsetX(-1 * m_AniEnt.m_EntOrgWidth / 2);
		m_AniEnt.SetEntOffsetY(-1 * m_AniEnt.m_EntOrgWidth / 2);
	}
	else
	{
		m_AniEnt.SetEntOffsetX(0);
		m_AniEnt.SetEntOffsetY(0);
	}

	m_AniEnt.SetAnimationIndexByIndex(m_AnimationIndex);
	m_AniEnt.SetRotation(m_Rotation);
}

void UIAniIcon::SetAnimationIndex(Int AnimationIndex)
{
	m_AnimationIndex = AnimationIndex;
	ResetPosition();
}
void UIAniIcon::SetRotation(Int Rotation)
{
	m_Rotation = Rotation;
	ResetPosition();
}

Int32 UIAniIcon::OnLButtonDown(Int32 pX, Int32 pY)
{
	return UIItem::OnLButtonDown(pX, pY);
}

bool UIAniIcon::Detect(Int32 pX, Int32 pY)
{
	if (m_NotDetected)
		return false;

	if (m_Detect)
		return UIItem::Detect(pX, pY);

	return false;
}

void UIAniIcon::SetDetect(Int Value)
{

	m_Detect = Value;
}

void UIChatAniIcon::Redraw(CFont *pFont, CImage *pImage)
{
	DrawEntityToImage(pImage, NULL);
}

void UIChatAniIcon::DrawEntityToImage(CImage *pImage, RECT * Rect)
{
	RECT NextDirtyRegion;

	
	if (Rect)
		NextDirtyRegion = *Rect;
	else 
	{
		NextDirtyRegion.left = NextDirtyRegion.top = 0;
		NextDirtyRegion.right = pImage->GetWidth();
		NextDirtyRegion.bottom = pImage->GetHeight();
	}

	Int32 LeftUpPosX = m_X + m_AniEnt.GetEntOffsetX();
	Int32 LeftUpPosY = m_Y + m_AniEnt.GetEntOffsetY();

	RECT rect2;
	rect2.left = LeftUpPosX + m_AniEnt.m_EntityTotalRect.left;
	rect2.top = LeftUpPosY + m_AniEnt.m_EntityTotalRect.top;
	rect2.right = rect2.left + m_AniEnt.m_EntityTotalRect.right - m_AniEnt.m_EntityTotalRect.left;
	rect2.bottom = rect2.top + m_AniEnt.m_EntityTotalRect.bottom - m_AniEnt.m_EntityTotalRect.top;

	if(!QGLMath::CalculateOverlappedRect(NextDirtyRegion, rect2, NextDirtyRegion))
		return;

	D3DSURFACE_DESC  pDescDes;

	pDescDes.Width = pImage->GetWidth();
	pDescDes.Height = pImage->GetHeight();
	
	RECT TotalEntLockRect;
	TotalEntLockRect.left = NextDirtyRegion.left - LeftUpPosX;
	TotalEntLockRect.right = TotalEntLockRect.left + m_AniEnt.m_EntityTotalRect.right - m_AniEnt.m_EntityTotalRect.left;
	TotalEntLockRect.top = NextDirtyRegion.top - LeftUpPosY ;
	TotalEntLockRect.bottom = TotalEntLockRect.top + m_AniEnt.m_EntityTotalRect.bottom - m_AniEnt.m_EntityTotalRect.top;

	Byte * pDesTData = (Byte *)pImage->GetPtr() + NextDirtyRegion.top * pImage->GetPitch() + NextDirtyRegion.left * sizeof(Color32);

	//clear the previous rect 
	UInt copyLen = (m_Width - 1) * sizeof(Color32);
	Byte * pTempDesData = (Byte *)pImage->GetPtr() + m_Y * pImage->GetPitch() + (m_X) * sizeof(Color32);
	for (UInt y = 0; y < m_Height ; y++)
	{
		ZeroMemory(pTempDesData, copyLen);
		pTempDesData += pImage->GetPitch();
	}
	
	for (int i = 0; i < m_AniEnt.GetTotalRenderPart() ; i++)
	{
		BOOL Flip = m_AniEnt.GetFlip(i);
		for (Int j = 0; j < 2; j ++)
		{
			BOOL Get = FALSE;

			if (j == 0)
				Get = m_AniEnt.GetEntityPart(i, ShadowPic, NULL);
			else if ( j == 1)
				Get = m_AniEnt.GetEntityPart(i, ImagePic, NULL);
			
			if(Get)
			{

				RECT CurrRect = m_AniEnt.GetCurrRect();

				Int ObjectLeftUpX = LeftUpPosX
					+ CurrRect.left ;

				Int ObjectLeftUpY = LeftUpPosY
					+ CurrRect.top ;

				RECT rect2;
				rect2.left = ObjectLeftUpX;
				rect2.top = ObjectLeftUpY;
				rect2.right = ObjectLeftUpX + CurrRect.right - CurrRect.left;
				rect2.bottom = ObjectLeftUpY + CurrRect.bottom - CurrRect.top;

				RECT resultRect;

				if (QGLMath::CalculateOverlappedRect(NextDirtyRegion,rect2, resultRect))
				{
					RECT SubEntLockRect;

					SubEntLockRect.left = resultRect.left - rect2.left + CurrRect.left;
					SubEntLockRect.right = resultRect.right - rect2.left + CurrRect.left;

					if (Flip)
					{
						SubEntLockRect.left -= resultRect.left - rect2.left;
						SubEntLockRect.left += rect2.right - resultRect.right;
						SubEntLockRect.right -= resultRect.left - rect2.left;
						SubEntLockRect.right += rect2.right - resultRect.right;
					}

					SubEntLockRect.top = resultRect.top - rect2.top + CurrRect.top;
					SubEntLockRect.bottom = resultRect.bottom - rect2.top  + CurrRect.top;

					D3DSURFACE_DESC  pDescSrc;

					//check invalid lock
					if (QGLMath::InvalidLock(&SubEntLockRect, MaxEntSizeX, MaxEntSizeY))
						continue;
				
					pDescSrc.Width =  m_AniEnt.GetPicWidth();
					pDescSrc.Height = MaxEntSizeY;

					Byte * pSrcTData =  m_AniEnt.GetPicData();

					if (m_AniEnt.GetHighLight())
					{
						Byte * pTempSrcTData = pSrcTData;
						Int Len = (SubEntLockRect.right - SubEntLockRect.left) * sizeof(Color32);
						for (Int j = 0; j < SubEntLockRect.bottom - SubEntLockRect.top; j ++)
						{
							for (Int i = 0; i < SubEntLockRect.right - SubEntLockRect.left; i ++)
							{
								pTempSrcTData[0] = gGlobal.AddtitiveTable[pTempSrcTData[0]][HighLightValue];
								pTempSrcTData[1] = gGlobal.AddtitiveTable[pTempSrcTData[1]][HighLightValue];
								pTempSrcTData[2] = gGlobal.AddtitiveTable[pTempSrcTData[2]][HighLightValue];
								pTempSrcTData += sizeof(Color32);
							}
							pTempSrcTData += pDescSrc.Width * sizeof(Color32) - Len;
						}
					}

					Int DesStartIndex = (SubEntLockRect.top - TotalEntLockRect.top) * pDescDes.Width + 
						(SubEntLockRect.left - TotalEntLockRect.left);
					if (Flip)
					{
						DesStartIndex += resultRect.left - rect2.left;
						DesStartIndex -= rect2.right - resultRect.right;
					}

					UInt copyLen = (SubEntLockRect.right - SubEntLockRect.left - 1) * sizeof(Color32);

					pDesTData += DesStartIndex * sizeof(Color32);


					for (Int y = 0; y < SubEntLockRect.bottom - SubEntLockRect.top - 1; y++)
					{
						memcpy(pDesTData, pSrcTData, copyLen);
						pDesTData += pDescDes.Width * sizeof(Color32);
						pSrcTData += pDescSrc.Width * sizeof(Color32);
					}

					
				}
			}
		}
	}
}

void UIChatAniIcon::SetAnimationData(LPCTSTR Filename)
{
	m_AniEnt.InitEntPicInfo();
	m_AniEnt.LoadEntityFile(Filename, QGLApplication::GetInstance().GetRenderClass()->GetDevice());
	SetWidth(m_AniEnt.m_EntOrgWidth);
	SetHeight(m_AniEnt.m_EntOrgHeight);
	m_AniEnt.m_Updated = TRUE;

}

void UIChatAniIcon::SetAnimationData(CAnimatedEntity * pAniEnt)
{
	if (pAniEnt == NULL)
		return;

	m_AniEnt = *pAniEnt;
	m_AniEnt.RemoveAllChildren();

	SetWidth(m_AniEnt.m_EntOrgWidth);
	SetHeight(m_AniEnt.m_EntOrgHeight);
	m_AniEnt.m_Updated = TRUE;
}

UInt32 UIChatAniIcon::Update(UInt32 dwDelta, CFont *pFont, CImage *pImage)
{
	m_AniEnt.UpDate(::timeGetTime());

	BOOL result = m_AniEnt.m_Updated;
	if (result)
		Redraw(pFont, pImage);
	m_AniEnt.m_Updated = 0;
	return result;
}


void UICheckBox::SetState(bool pState)
{
	if (m_State != pState)
	{
		m_State = pState;
		RenderParent();
	}
}


void UICheckBox::Render(CImage *pImage, RECT &RRect)
{
	Int32		X = m_OffsetX, Y = m_OffsetY;
	RECT	SRect;

	memcpy(&SRect, m_PicRect, sizeof(RECT));
	if(CImageDirtyRect(X, Y, SRect, RRect))
		Alpha8888To8888S(X, Y, m_PicImage, pImage, &SRect, m_RenderStyle, m_Color);
	
	X = m_OffsetX + m_CheckX;
	Y = m_OffsetY + m_CheckY;
	memcpy(&SRect, m_PicRect + (m_State ? 2 : 1), sizeof(RECT));
	if(CImageDirtyRect(X, Y, SRect, RRect))
		Alpha8888To8888S(X, Y, m_PicImage, pImage, &SRect, m_RenderStyle, m_Color);
}


void UICheckBox::SetRect(Int32 pIndex, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	m_PicRect[pIndex].left = pLeft;
	m_PicRect[pIndex].top = pTop;
	m_PicRect[pIndex].right = pRight;
	m_PicRect[pIndex].bottom = pBottom;

	if(pIndex == 0)
	{
		m_Width = pRight - pLeft;
		m_Height = pBottom - pTop;
	}
}


void UICheckBox::SetImageFileA(PCSTRINGA ImageName)
{
	StringW Filename;

	UTF82StringW(Filename, ImageName);

	CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	SetImage(Image);
}


Int32 UICheckBox::OnLButtonUp(Int32 pX, Int32 pY)
{
	Int32  Result = UIItem::Detect(pX, pY);
	if( Result && !m_StopPlaySoundOnce )
	{
		//ALSoundManager::GetInstance().CreateSoundA("Data\\Wav\\SoundI0006_choose.wav")->Play();	
		FSound* sfx = FSOUND_Manager::GetInstance().CreateSoundA("Data\\Wav\\SoundI0006_choose.wav");
		if (sfx)
		{
			sfx->SetVolume(gGlobal.g_pSystemSetting->GetSoundEffectVolForFmod());
			sfx->Play();
		}
		
	}
	return Result;
}


Int32 UICheckBox::OnLButtonDown(Int32 pX, Int32 pY)
{
	if(UIItem::Detect(pX, pY))
	{
		m_State = !m_State;
		return 1;
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////


UIEditBoxEx::UIEditBoxEx() : UIEditBox()
{
	m_LineLimit = 1;
	m_CurLine = 0;
	m_SelStartLine = m_SelEndLine = m_SelEndX = 0;
}


void UIEditBoxEx::DrawFont()
{
	if(!Empty())
	{
		if(m_SelStart != m_SelEnd)
		{
			m_Font->SetSel(m_SelStart, m_SelEnd);
		}

		{
			UInt32	LinePitch = m_Font->GetLinePitch();

			m_Font->SetLinePitch(m_LinePitch);
			m_Font->DrawFontStr(m_Text.c_str(),
				m_BufImage->GetPtr(), 0, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle | (0x1 << 7), 
				m_FontColor, m_ShadowColor, 0);
			m_Font->SetLinePitch(0);


			m_LineWidth.clear();
			m_LineStart.clear();
			m_LineWidth = m_Font->GetLineWidth();
			m_LineStart = m_Font->GetLineStart();
		}

		
		m_Font->SetSel(0, 0);
	}
	else
	{
		m_LineWidth.clear();
		m_LineStart.clear();
	}
}


void UIEditBoxEx::FindCursorPos(Int32 pX, Int32 pY)
{
	UInt32	PitchHeight, RealFontHeight;
	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	m_StartPoint = m_InsertPos = 0;
	m_CursorX = -CursorStartX; 
	m_CursorY = m_CurLine = 0;

	Int32 Height = m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
	Int32 TargetX = pX - m_X;
	Int32 TargetY = (pY - m_Y) / Height * Height;

	Int32 TargetOffset = TargetY * m_Width + TargetX;
	Int32 NowOffset = m_CursorY * m_Width + m_CursorX;
	Int32 SubTract, KeepSubTract;
	Int32 KeepCursorX = m_CursorX;
	Int32 KeepCursorY = m_CursorY;
	Int32 KeepInsertPos = m_InsertPos;
	Int32 KeepCurLine = m_CurLine;
	
	KeepSubTract = SubTract = TargetOffset - NowOffset;

	while(m_InsertPos < m_Text.size())
	{
		CursorMoveRight(PitchHeight, RealFontHeight, 1);
		NowOffset = m_CursorY * m_Width + m_CursorX;
		SubTract = abs(TargetOffset - NowOffset);
		if(SubTract < KeepSubTract)
		{
			KeepSubTract = SubTract;
			KeepCursorX = m_CursorX;
			KeepCursorY = m_CursorY;
			KeepInsertPos = m_InsertPos;
			KeepCurLine = m_CurLine;
		}
	}

	m_CursorX = KeepCursorX;
	m_CursorY = KeepCursorY;
	m_InsertPos = KeepInsertPos;
	m_CurLine = KeepCurLine;
}


Int32 UIEditBoxEx::OnLButtonDown(Int32 pX, Int32 pY)
{
	bool ret = UIItem::Detect(pX, pY);
	
	if(ret)
	{
		FindCursorPos(pX, pY);

		m_SelStart = m_SelEnd = m_InsertPos;
		m_SelStartLine = m_SelEndLine = m_CurLine;
		SetChange(true);

		FillImage();
		DrawFont();

		
		m_PickUp = true;

		if (m_pUIManager)
		{
			m_pUIManager->SetKeyInItem(m_Name.c_str());
		}
		
	}

	return ret;
}


Int32 UIEditBoxEx::OnKeyDown(WPARAM wParam)
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	m_ShiftDown = ((GetKeyState(VK_LSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 ||
		(GetKeyState(VK_RSHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0);

	switch(wParam)
	{
	case VK_LEFT:
		if(!m_ShiftDown)
			m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		CursorMoveLeft(PitchHeight, 1);
		FillImage();
		DrawFont();
		break;
	case VK_RIGHT:		
		if(!m_ShiftDown)
			m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		CursorMoveRight(PitchHeight, RealFontHeight, 1);
		FillImage();
		DrawFont();
		break;
	case VK_UP:
		if(!m_ShiftDown)
			m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		CursorMoveUp(PitchHeight, RealFontHeight);
		FillImage();
		DrawFont();
		break;
	case VK_DOWN:		
		if(!m_ShiftDown)
			m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		CursorMoveDown(PitchHeight, RealFontHeight);
		FillImage();
		DrawFont();
		break;

	case VK_HOME:
		if(!m_ShiftDown)
			m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		m_OnScreenWidth = Home();
		FillImage();
		DrawFont();
		m_CurLine = 0;
		break;
	case VK_END:
		if(!m_ShiftDown)
			m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		m_OnScreenWidth = End();
		FillImage();
		DrawFont();
		break;
	case VK_DELETE:
		if(m_IsPassword)
		{
			m_SelStart = 0;
			m_SelEnd = (UInt32)m_Text.size();
			DelSelText(PitchHeight, RealFontHeight);
			FillImage();
			m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		}
		else
		if(m_Text.size())
		{
			if(m_SelStart == m_SelEnd)
			{
				if(m_InsertPos < m_Text.size())
				{
					Int	i = 1;
					if(m_Text[m_InsertPos] == 15)
					{
						while(m_Text[m_InsertPos + i] != 16 && m_InsertPos + i < m_Text.size())
						{
							++i;
						}
						if(m_Text[m_InsertPos + i] == 16)
							++i;
					}
					if(m_Text[m_InsertPos + i] == 17)
					{
						while(m_Text[m_InsertPos + i] != 18 && m_InsertPos + i < m_Text.size())
						{
							++i;
						}
						if(m_Text[m_InsertPos + i] == 18)
							++i;
					}
					m_Text.erase(m_InsertPos, i);
				}
			}
			else
			{
				DelSelText(PitchHeight, RealFontHeight);
				UInt32 TempPos = m_InsertPos;

				m_InsertPos = m_CurLine = m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
				m_CursorX = -CursorStartX; m_CursorY = 0;

				while(TempPos > m_InsertPos)
				{
					CursorMoveRight(PitchHeight, RealFontHeight, 1);
				}
				
			}

			FillImage();
			DrawFont();
		}
		break;		
	default:
		m_ShiftDown = false;
		return 0;
		break;
	}

	m_ShiftDown = false;

	return 1;
}


Int32 UIEditBoxEx::OnChar(WPARAM wParam)
{
	Int32 Result = UIEditBox::OnChar(wParam);


	switch(wParam)
	{
	case 22:
		m_SelStartLine = m_SelEndLine = m_SelStart = m_SelEnd = 0;
		m_LineWidth.clear();
		m_LineWidth = m_Font->GetLineWidth();
		Redraw();
		break;
	}

	//m_LineWidth.clear();
	//m_LineWidth = m_Font->GetLineWidth();

	return Result;
}


void UIEditBoxEx::BackSpaceKey(UInt32	PitchHeight, Int32 RealFontHeight)
{
	CursorMoveLeft(PitchHeight, 1);
	if(m_CursorX == m_LineWidth[m_CurLine] - CursorStartX)
		CursorMoveLeft(PitchHeight, 1);
}


void UIEditBoxEx::CursorMoveLeft(Int32	PitchHeight, UInt Count)
{
	if(m_ShiftDown && m_SelStart == m_SelEnd)
	{
		m_SelStart = m_InsertPos;
		m_SelStartLine = m_CurLine;
	}

	bool	HideNow = false, JumpChar = false;

	for (UInt i = 0; i < Count || JumpChar; i++)
	{
		if(m_InsertPos > 0)
		{
			--m_InsertPos;

			if(m_Text[m_InsertPos] == 15)
			{
				HideNow = false;
				JumpChar = false;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 16)
			{
				HideNow = true;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 17)
			{
				JumpChar = (m_InsertPos > 0 && m_Text[m_InsertPos - 1] == 16);
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 18)
			{
				JumpChar = true;
				--i;
				continue;
			}
			

			if(HideNow || m_Text[m_InsertPos] <= 26)
			{
				--i;
				continue;
			}

			m_CursorX -= m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[m_InsertPos]) + PitchHeight;
			if(m_CursorX < -CursorStartX)
			{
				m_CursorY -= m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
				--m_CurLine;
				m_CursorX = m_LineWidth[m_CurLine] - CursorStartX;
				if(Count == 1)
					++m_InsertPos;
				
				bool SubHideNow = false;
				while((SubHideNow || m_Text[m_StartPoint] <= 26) && m_StartPoint > 0)
				{
					if(m_Text[m_StartPoint] == 15)
					{
						SubHideNow = false;
					}
					else
					if(m_Text[m_StartPoint] == 16)
					{
						SubHideNow = true;
					}
				}
			}
			
			if(m_ShiftDown)
			{
				m_SelEnd = m_InsertPos;
				m_SelEndLine = m_CurLine;
			}

		}
		else
			break;
	}
}


void UIEditBoxEx::CursorMoveRight(Int32	PitchHeight, Int32 RealFontHeight,  UInt Count)
{
	if(m_ShiftDown && m_SelStart == m_SelEnd)
	{
		m_SelStart = m_InsertPos;
		m_SelStartLine = m_CurLine;
	}

	bool	HideNow = false, JumpChar = false;

	for (Int i = 0; i < (Int)Count || JumpChar; i++)
	{
		if(m_InsertPos < m_Text.size())
		{
			if(m_Text[m_InsertPos] == 15)
			{
				HideNow = true;
				++m_InsertPos;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 16)
			{
				HideNow = false;
				++m_InsertPos;
				--i;
				continue;
			}
			else
			if(m_Text[m_InsertPos] == 17)
			{
				JumpChar = true;
				++m_InsertPos;
				--i;
				continue;

			}
			else
			if(m_Text[m_InsertPos] == 18)
			{
				JumpChar = false;
				++m_InsertPos;
				--i;
				continue;

			}

			if(HideNow || m_Text[m_InsertPos] <= 26)
			{
				++m_InsertPos;
				--i;
				continue;
			}
			UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : m_Text[m_InsertPos]);
			UInt32	TempWidth = CharWidth + RealFontHeight;

			bool SubHideNow = false, SubJumpChar = false;
			while(m_CursorX + CursorStartX + TempWidth >= m_Width || SubJumpChar)
			{
				if(m_Text[m_StartPoint] == 15)
				{
					SubHideNow = true;
					++m_StartPoint;
					continue;
				}
				else
				if(m_Text[m_StartPoint] == 16)
				{
					SubHideNow = false;
					++m_StartPoint;
					continue;
				}
				else
				if(m_Text[m_InsertPos] == 17)
				{
					SubJumpChar = true;
				}
				else
				if(m_Text[m_InsertPos] == 18)
				{
					SubJumpChar = false;
				}

				if(SubHideNow || m_Text[m_StartPoint] <= 26)
				{
					++m_StartPoint;
					continue;
				}

				m_CursorX = -CursorStartX;
				m_CursorY += m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
				++m_CurLine;

				if(Count == 1)
					goto Label_1;
			}

			m_CursorX += CharWidth + PitchHeight;
			++m_InsertPos;

Label_1:

			if(m_ShiftDown)
			{
				m_SelEnd = m_InsertPos;
				m_SelEndLine = m_CurLine;
			}

		}
		else 
			break;
	}
}


void UIEditBoxEx::CursorMoveUp(Int32 PitchHeight, Int32 RealFontHeight)
{
	if(m_CurLine > 0)
	{
		Int32 Height = m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
		Int32 TargetX = m_CursorX;
		Int32 TargetY = m_CursorY - Height;

		Int32 TargetOffset = TargetY * m_Width + TargetX;
		Int32 NowOffset = m_CursorY * m_Width + m_CursorX;
		Int32 SubTract, KeepSubTract;
		
		KeepSubTract = SubTract = NowOffset - TargetOffset;

		while(m_InsertPos > 0)
		{
			CursorMoveLeft(PitchHeight, 1);
			NowOffset = m_CursorY * m_Width + m_CursorX;
			SubTract = abs(NowOffset - TargetOffset);
			if(SubTract < KeepSubTract)
			{
				KeepSubTract = SubTract;
			}
			else
			if(SubTract == 0)
			{
				break;
			}
			else
			{
				CursorMoveRight(PitchHeight, RealFontHeight, 1);
				break;
			}
		}
	}
}


void UIEditBoxEx::CursorMoveDown(Int32 PitchHeight, Int32 RealFontHeight)
{
	if(m_CurLine + 1 < m_LineWidth.size())
	{
		Int32 Height = m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
		Int32 TargetX = m_CursorX;
		Int32 TargetY = m_CursorY + Height;

		Int32 TargetOffset = TargetY * m_Width + TargetX;
		Int32 NowOffset = m_CursorY * m_Width + m_CursorX;
		Int32 SubTract, KeepSubTract;
		
		KeepSubTract = SubTract = TargetOffset - NowOffset;

		while(m_InsertPos < m_Text.size())
		{
			CursorMoveRight(PitchHeight, RealFontHeight, 1);
			NowOffset = m_CursorY * m_Width + m_CursorX;
			SubTract = abs(TargetOffset - NowOffset);
			if(SubTract < KeepSubTract)
			{
				KeepSubTract = SubTract;
			}
			else
			if(SubTract == 0)
			{
				break;
			}
			else
			{
				CursorMoveLeft(PitchHeight, 1);
				break;
			}
		}
	}
}


UInt32 UIEditBoxEx::Home()
{
	UInt32 ret = 0;
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);
	if(m_ShiftDown)
	{
		while(m_InsertPos > 0)
			CursorMoveLeft(PitchHeight, 1);
	}
	else
		ret = UIEditBox::Home();

	return ret;
}

UInt32 UIEditBoxEx::End()
{
	UInt32	PitchHeight, RealFontHeight;

	m_Font->TakePitchHeight(m_FontStyle, PitchHeight, RealFontHeight);

	while(m_InsertPos < m_Text.size())
	{
		CursorMoveRight(PitchHeight, RealFontHeight, 1);
	}

	return m_LineWidth[m_CurLine];
}



bool UIEditBoxEx::CheckDrawOver()
{
	m_Font->SetLinePitch(m_LinePitch);
	m_Font->DrawFontStr(m_Text.c_str(),
		m_BufImage->GetPtr(), 0, 0, m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle | (0x1 << 7), 
		m_FontColor, m_ShadowColor, 0, 1);
	m_Font->SetLinePitch(0);

	return m_Font->GetDrawOver();
}


void UIEditBoxEx::Render(CImage *pImage)
{
	if(m_SelStart != m_SelEnd && !m_Text.IsEmpty())
	{
		Int32 Height = m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
		RECT	rect;

		if(m_SelStartLine == m_SelEndLine)
		{
			m_SelStartX = m_Font->GetSelStartX();
			SetRect(&rect, m_OffsetX + m_Font->GetSelStartX(), 
				m_OffsetY + Height * m_SelStartLine, 
				m_OffsetX + m_Font->GetSelStartX() + m_Font->GetSelEndWidth(), 
				m_OffsetY + Height * m_SelStartLine + Height);
			pImage->FillUInt32(m_SelColor, &rect);
		}
		else
		{
			UInt SelStartLine = min(m_SelStartLine, m_SelEndLine);
			UInt SelEndLine = max(m_SelStartLine, m_SelEndLine);

			if(!(m_SelStartLine > m_SelEndLine && m_CursorX == m_LineWidth[SelStartLine] - CursorStartX))
			{
				m_SelStartX = m_Font->GetSelStartX();
				SetRect(&rect, m_OffsetX + m_Font->GetSelStartX(), 
					m_OffsetY + Height * SelStartLine, 
					m_OffsetX + m_Width, 
					m_OffsetY + Height * SelStartLine + Height);

				pImage->FillUInt32(m_SelColor, &rect);
			}

			for(UInt i = SelStartLine + 1; i < m_SelEndLine; ++i)
			{
				SetRect(&rect, m_OffsetX, 
					m_OffsetY + Height * i, 
					m_OffsetX + m_Width, 
					m_OffsetY + Height * i + Height);

				pImage->FillUInt32(m_SelColor, &rect);
			}


			if(!(m_SelStartLine < m_SelEndLine && m_CursorX == -CursorStartX))
			{
				SetRect(&rect, m_OffsetX, 
					m_OffsetY + Height * SelEndLine, 
					m_OffsetX + m_Font->GetSelEndWidth(), 
					m_OffsetY + Height * SelEndLine + Height);

				pImage->FillUInt32(m_SelColor, &rect);
			}
		}
	}

	RECT	SRect = {0, 0, m_Width, m_Height};

	Alpha8888To8888S(m_OffsetX, m_OffsetY, m_BufImage, pImage, &SRect, m_RenderStyle, m_Color);

	if(m_pUIManager->GetKeyInItem() == this ||
		m_pUIManager->GetFocusItem() == this)
	{
		RECT	SRect = {0, 0, 8, 12};
		
		if (m_ShowEditCursor)
			Alpha8888To8888S(m_OffsetX + m_CursorX, m_OffsetY + m_CursorY, 	g_EditCursor, pImage, &SRect, m_RenderStyle, m_Color);
	}
}


void UIEditBoxEx::Render(CImage *pImage, RECT &RRect)
{
	if(m_SelStart != m_SelEnd && !m_Text.IsEmpty())
	{
		Int32	Height = m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
		Int32	X, Y;
		//Int32	Liner = 1;
		RECT	SRect;

		if(m_SelStartLine == m_SelEndLine)
		{
			X = m_OffsetX + m_Font->GetSelStartX(), Y = m_OffsetY + Height * m_SelStartLine;
			m_SelStartX = m_Font->GetSelStartX();
			SetRect(&SRect, m_OffsetX + m_Font->GetSelStartX(), 
				m_OffsetY + Height * m_SelStartLine, 
				m_OffsetX + m_Font->GetSelStartX() + m_Font->GetSelEndWidth(), 
				m_OffsetY + Height * m_SelStartLine + Height);

			if(CImageDirtyRect(X, Y, SRect, RRect))
				pImage->FillUInt32(m_SelColor, &SRect);
		}
		else
		{
			UInt SelStartLine = min(m_SelStartLine, m_SelEndLine);
			UInt SelEndLine = max(m_SelStartLine, m_SelEndLine);

			if(!(m_SelStartLine > m_SelEndLine && m_CursorX == m_LineWidth[SelStartLine] - CursorStartX))
			{
				X = m_OffsetX + m_Font->GetSelStartX(), Y = m_OffsetY + Height * SelStartLine;
				m_SelStartX = m_Font->GetSelStartX();
				SetRect(&SRect, m_OffsetX + m_Font->GetSelStartX(), 
					m_OffsetY + Height * SelStartLine, 
					m_OffsetX + m_Width, 
					m_OffsetY + Height * SelStartLine + Height);

				if(CImageDirtyRect(X, Y, SRect, RRect))
					pImage->FillUInt32(m_SelColor, &SRect);
			}


			for(UInt i = SelStartLine + 1; i < SelEndLine; ++i)
			{
				X = m_OffsetX;
				Y = m_OffsetY + Height * i;

				SetRect(&SRect, X, Y, X + m_Width, Y + Height);

				if(CImageDirtyRect(X, Y, SRect, RRect))
					pImage->FillUInt32(m_SelColor, &SRect);
			}

			if(!(m_SelStartLine < m_SelEndLine && m_CursorX == -CursorStartX))
			{
				X = m_OffsetX;
				Y = m_OffsetY + Height * SelEndLine;

				SetRect(&SRect, m_OffsetX, 
					m_OffsetY + Height * SelEndLine, 
					m_OffsetX + m_Font->GetSelEndWidth(), 
					m_OffsetY + Height * SelEndLine + Height);

				if(CImageDirtyRect(X, Y, SRect, RRect))
					pImage->FillUInt32(m_SelColor, &SRect);
			}
		}
	}

	RECT	SRect = {0, 0, m_Width, m_Height};
	Int32	X = m_OffsetX, Y = m_OffsetY;

	if(CImageDirtyRect(X, Y, SRect, RRect))
		Alpha8888To8888S(X, Y, m_BufImage, pImage, &SRect, m_RenderStyle, m_Color);

	if(m_Focus)
	{
		UInt32		X = m_OffsetX + m_CursorX, Y = m_OffsetY + m_CursorY;
		RECT	SRect = {0, 0, 8, 12};
		
		if (m_ShowEditCursor)
			Alpha8888To8888S(X, Y, g_EditCursor, pImage, &SRect, m_RenderStyle, m_Color);
	}
}


Int32 UIEditBoxEx::OnMouseMove(Int32 pX, Int32 pY)
{
	bool ret = UIItem::Detect(pX, pY);

	if(ret && m_PickUp)
	{
		FindCursorPos(pX, pY);
	
		//if(m_SelEnd != m_InsertPos)
		{
			m_SelEnd = m_InsertPos;
			m_SelEndLine = m_CurLine;
			SetChange(true);

			FillImage();
			DrawFont();
		}
	}

	return 1;
}



void UIEditBoxEx::SelAll()
{
	if(!m_IsPassword)
	{
		End();
		m_SelStartLine = m_SelStart = 0;
		m_SelEnd = m_InsertPos;
		m_SelEndLine = (UInt32)m_LineWidth.size() - 1;
		FillImage();
		DrawFont();
	}
}


UInt32 UIEditBoxEx::AddCharW(WPARAM wParam, UInt32	PitchHeight, UInt32 RealFontHeight)
{
	if(m_Text.size() < m_EditLimit || m_EditLimit == 0)
	{
		StringW		A1;

		A1 = CharW(wParam);
		
		if(m_NumOnly && (wParam < _T('0') || wParam > _T('9')))
		{
			return 1;
		}

		if(m_SelEnd != m_SelStart)
		{
			DelSelText(PitchHeight, RealFontHeight);
			FillImage();
		}
		
		UInt32	CharWidth = m_Font->GetCharWidth(m_IsPassword ? _T('*') : CharW(wParam));
		UInt32	TempWidth = CharWidth + RealFontHeight;

		StringW	TempStr = m_Text;
		bool	AppendFlag;
		if(m_InsertPos == m_Text.size())
		{
			m_Text.append(A1);
			AppendFlag = true;
		}
		else
		{
			m_Text.insert(m_InsertPos, A1);
			AppendFlag = false;
		}


		//if(!CheckDrawOver())
		{
			/*
			std::vector<UInt32>::iterator it = m_CharWidth.begin() + m_InsertPos;
			m_CharWidth.insert(it, CharWidth + PitchHeight);
			it = m_CharMark.begin() + m_InsertPos;
			m_CharMark.insert(it, 0);
			*/

			Int	FontX = m_CursorX + CursorStartX;
			Int FontY = m_CursorY;

			CursorMoveRight(PitchHeight, RealFontHeight, 1);
			Int TempCharWidth = m_CursorX + CursorStartX - FontX;
			if(m_CursorX == -CursorStartX)
			{
				FontX = m_CursorX + CursorStartX;
				FontY = m_CurLine * m_Font->GetDrawHeight(m_FontStyle, m_LinePitch);
				CursorMoveRight(PitchHeight, RealFontHeight, 1);
				TempCharWidth = m_CursorX + CursorStartX - FontX;
				if(AppendFlag)
				{
					m_LineWidth.push_back(0);
				}
			}
			
			if(AppendFlag)
			{
				String  S = A1;
				m_Font->DrawFontStr(S.c_str(),
					m_BufImage->GetPtr(), FontX, FontY,
				m_Width, m_Height, m_BufImage->GetPitch(), m_FontStyle | (0x1 << 7), 
				m_FontColor, m_ShadowColor, 0);
				if(m_LineWidth.size() && m_CurLine < m_LineWidth.size())
				{
					m_LineWidth[m_CurLine] += TempCharWidth;
				}
				else
				{
					m_LineWidth.push_back(TempCharWidth);
					
				}
			}
			else
			{
				FillImage();
				DrawFont();
			}
		}
		/*
		else
		{
			m_Text = TempStr;
		}
		*/
	}

	return 1;
}


