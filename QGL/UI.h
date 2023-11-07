#pragma once
#ifndef _UI_H_
#define _UI_H_

#include <typeinfo>
#include "Render.h"
#include "Image.h"
#include "Font.h"
#include "DirtyRectManager.h"
#include <set>
#include "PlayOggVorbis.h"
#include "AnimatedEntity.h"
#include <Reuben/Math/SHA.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace ZLib
{
#include <zlib.h>
};



extern	HWND g_UIhWnd;
extern  HWND g_hWnd;

class UIManager;
//////////////////////////////////////////////////////////

//Basic Interface for Renderable Object {Below interface will throws some specific exception}

//void Create(/*Specific*/);			//Memory Create
//void Dispose();					//Memory Dispose
//void Validate(/*Renderer*/);			//Device Validate
//void Invalidate();				//Device Invalidate
//void Update(UInt32 dwDelta);			//Update Frame
//void Render(/*Renderer*/);			//Render Frame


UInt32	HexToDWORD(PCSTRINGA HexStr);
Int32  UTF82StringW(StringW &Str, PCSTRINGA Text);

enum UI_LUA_FUNC_CODE
{
	UI_FUNC_LBUTTON_UP = 0,
	UI_FUNC_LBUTTON_DOWN,
	UI_FUNC_CHAR,
	UI_FUNC_KEYUP,
	UI_FUNC_KEYDOWN,

	UI_FUNC_MOUSE_MOVE,
	UI_FUNC_RBUTTON_UP,
	UI_FUNC_RBUTTON_DOWN,
	UI_FUNC_MOUSE_WHEEL,
	UI_FUNC_TIMEOUT,

	UI_FUNC_STARTDRAG,
	UI_FUNG_ENDDRAG,

	UI_FUNC_MOUSE_OUT,
	UI_FUNC_MOUSE_IN,

	UI_FUNC_UNPICKUP,

	UI_FUNC_LBUTTONDBLCLK,
	UI_FUNC_RBUTTONDBLCLK,

	MAX_UI_FUNC,
};



class UIItem
{
public:
	UIItem();
	virtual	~UIItem(){	Dispose(); }

	virtual void Create(){};
	virtual void Dispose(){};

	virtual void Validate(CImage *pImage){};
	virtual void Invalidate(){};
	virtual UInt32 Update(UInt32 dwDelta){ return 0; }
	virtual void Render(CImage *pImage){};

	virtual void Render(CImage *pImage, RECT &RRect){};
	virtual void RenderParent();
	virtual void RenderParent(RECT &RRect);
	virtual void AddDirtyRectToMan(RECT *rect = NULL){}

	virtual void	SetDetectRect(UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom)
	{
		m_DetectRect.left = Left;
		m_DetectRect.top = Top;
		m_DetectRect.right = Right;
		m_DetectRect.bottom = Bottom;
		m_Width = Right - Left;
		m_Height = Bottom - Top;
	}

	virtual void SetNotMouseOver(){}
	virtual void SetMouseOver(){}
	
	virtual Int32		OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ){	return 0; }
	virtual Int32		OnMouseMove(Int32 pX, Int32 pY){ return 0; }
	virtual Int32		OnLButtonUp(Int32 pX, Int32 pY){ return (Detect(pX, pY)); }
	virtual Int32		OnLButtonDown(Int32 pX, Int32 pY){ return (Detect(pX, pY)); }
	virtual Int32		OnRButtonUp(Int32 pX, Int32 pY){ return (Detect(pX, pY)); }
	virtual Int32		OnRButtonDown(Int32 pX, Int32 pY){ return (Detect(pX, pY)); }
	virtual Int32		OnLButtonDBLCLK(Int32 pX, Int32 pY){ return (Detect(pX, pY)); }
	virtual Int32		OnRButtonDBLCLK(Int32 pX, Int32 pY){ return (Detect(pX, pY)); }
	virtual Int32		OnKeyDown(WPARAM wParam){ return 0;	}
	virtual Int32		OnKeyUp(WPARAM wParam){	return 0; }
	virtual Int32		OnChar(WPARAM wParam){ return 0; }
	//virtual Int32		OnIME(UINT uMsg, WPARAM wParam, LPARAM lParam ){ return 0; }
	virtual Int32		OnOtherMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ return 0; }

	virtual bool	Detect(Int32 pX, Int32 pY)
	{
		return (pX >= m_DetectRect.left && pX < m_DetectRect.right && pY >= m_DetectRect.top && pY < m_DetectRect.bottom && 
			!m_NotDetected && m_IsEnable);
	}

	enum TimeOutType
	{
		TIMEOUT_LEAVE = 0,
		TIMEOUT_MOVEIN,
		TIMEOUT_COUNTDOWN, 
	};
	
	static Int32		LuaOnMouseWheel(UIItem *Item, Int32 pX, Int32 pY, Int32 pZ);
	static Int32		LuaOnMouseMove(UIItem *Item, Int32 pX, Int32 pY, Int32 Code);
	static Int32		LuaOnLButtonUp(UIItem *Item, Int32 pX, Int32 pY);
	static Int32		LuaOnLButtonDown(UIItem *Item, Int32 pX, Int32 pY);
	static Int32		LuaOnRButtonUp(UIItem *Item, Int32 pX, Int32 pY);

	static Int32		LuaOnRButtonDown(UIItem *Item, Int32 pX, Int32 pY);
	static Int32		LuaOnKeyDown(UIItem *Item, Int32 KeyCode);
	static Int32		LuaOnKeyUp(UIItem *Item, Int32 KeyCode);
	static Int32		LuaOnChar(UIItem *Item, Int32 KeyCode);
	static Int32		LuaOnTimeOut(UIItem *Item, TimeOutType Code);

	static  Int32		LuaOnStartDrag(UIItem *Item);
	static  Int32		LuaOnEndDrag(UIItem *Item, Int32 Code);
	static  Int32		LuaOnMouseOut(UIItem *Item);
	static  Int32		LuaOnMouseIn(UIItem *Item);
	static	Int32		LuaOnUnPickUp(UIItem *Item);
	static  Int32		LuaOnLButtonDBLCLK(UIItem *Item, Int32 pX, Int32 pY);
	static  Int32		LuaOnRButtonDBLCLK(UIItem *Item, Int32 pX, Int32 pY);


	virtual UInt32		GetLuaOnMouseWheel(){ return (UInt32)m_LuaFuncTable[UI_FUNC_MOUSE_WHEEL].size(); }
	virtual UInt32		GetLuaOnMouseMove(){ return (UInt32)m_LuaFuncTable[UI_FUNC_MOUSE_MOVE].size(); }
	virtual UInt32		GetLuaOnLButtonUp(){ return (UInt32)m_LuaFuncTable[UI_FUNC_LBUTTON_UP].size(); }
	virtual UInt32		GetLuaOnLButtonDown(){ return (UInt32)m_LuaFuncTable[UI_FUNC_LBUTTON_DOWN].size(); }
	virtual UInt32		GetLuaOnRButtonUp(){ return (UInt32)m_LuaFuncTable[UI_FUNC_RBUTTON_UP].size(); }
	virtual UInt32		GetLuaOnRButtonDown(){ return (UInt32)m_LuaFuncTable[UI_FUNC_RBUTTON_DOWN].size(); }
	virtual UInt32		GetLuaOnKeyDown(){ return (UInt32)m_LuaFuncTable[UI_FUNC_KEYDOWN].size(); }
	virtual UInt32		GetLuaOnKeyUp(){ return (UInt32)m_LuaFuncTable[UI_FUNC_KEYUP].size(); }
	virtual UInt32		GetLuaOnChar(){ return (UInt32)m_LuaFuncTable[UI_FUNC_CHAR].size();	}
	virtual UInt32		GetLuaOnTimeOut(){ return (UInt32)m_LuaFuncTable[UI_FUNC_TIMEOUT].size(); }
	virtual UInt32		GetOnStartDrag(){ return (UInt32)m_LuaFuncTable[UI_FUNC_STARTDRAG].size(); }
	virtual UInt32		GetOnEndDrag(){	return (UInt32)m_LuaFuncTable[UI_FUNG_ENDDRAG].size(); }
	virtual UInt32		GetOnMouseOut(){ return (UInt32)m_LuaFuncTable[UI_FUNC_MOUSE_OUT].size(); }
	virtual UInt32		GetOnMouseIn(){	return (UInt32)m_LuaFuncTable[UI_FUNC_MOUSE_IN].size();	}
	virtual UInt32		GetOnLDButtonDBLCLK(){ return (UInt32)m_LuaFuncTable[UI_FUNC_LBUTTONDBLCLK].size();	}
	virtual UInt32		GetOnRBurttonDBLCLK(){ return (UInt32)m_LuaFuncTable[UI_FUNC_RBUTTONDBLCLK].size(); }
	virtual bool		CheckLuaFunc(UI_LUA_FUNC_CODE Index){  return (!m_LuaFuncTable[Index].IsEmpty()); }

	virtual void 	UnDetect(){ };
	virtual Int32	GetX(){ return m_X; }
	virtual Int32	GetY(){ return m_Y; }
	virtual UInt32	GetWidth(){ return m_Width; }
	virtual UInt32	GetHeight(){	return m_Height; }
	virtual bool	IsPickUp(){ return m_PickUp; }
	virtual void	PickUp(){ m_PickUp = true;	}
	virtual void	UnPickUp(){ m_PickUp = false; }
	virtual void	SimpleUnPickUp(){ m_PickUp = false; }

	virtual void ResetXY(){	m_X = m_Y = m_OffsetX = m_OffsetY = 0; }
	virtual void SetXY(Int32 pX, Int32 pY){	m_X = pX; m_Y = pY;	}

	virtual void SetPos(Int32 pX, Int32 pY);
	virtual void MovePos(Int32 pX, Int32 pY);
	virtual void SetDetectRect();

	virtual void SetOffsetXY(Int32 pX, Int32 pY);
	virtual void SetOffsetXYOnly(Int32 pX, Int32 pY){ m_OffsetX = pX; m_OffsetY = pY; }
	virtual void SetOffsetPos(Int32 pX, Int32 pY){	SetPos(pX + m_OffsetX, pY + m_OffsetY);	}

	virtual void SetLuaState(lua_State *pLuaState){	m_LuaState = pLuaState; }
	virtual lua_State *GetLuaState(){ return m_LuaState; }
	virtual void CloseLua(){ SafeCloseLua(m_LuaState); }
	virtual void SetCanDrag(UInt32 Code){ m_CanDrag = Code;	}
	virtual UInt32 GetCanDrag(){ return m_CanDrag; }
	virtual void SetLuaFunc(UInt32 pIndex, PCSTRINGA pFuncStr){	m_LuaFuncTable[pIndex] = pFuncStr; }

	virtual void SetFocus(bool pState){	m_Focus = pState; }
	virtual bool GetFocus(){ return m_Focus; }
	virtual UIItem *GetHotSpot(){ return this; }
	virtual void SetHotSpot(UIItem *pUIItem){}
	virtual UIItem *GetMouseOver(){ return this; }

	virtual RECT	*GetDetectRect(){ return &m_DetectRect; }
	virtual void	SetRealRect(RECT &RRect);
	virtual void	SetRealOffsetRect(RECT &RRect);

	virtual void SetName(PCSTRINGA pStr){ m_Name = pStr; }
	virtual StringA	*GetName(){ return &m_Name;	}

	virtual void SetTips(PCSTRINGW Str){ m_Tips = Str; }
	virtual PCSTRINGW	GetTips(){ return (m_Tips.IsEmpty() ? NULL : m_Tips.c_str()); }

	virtual void SetLastKey(WPARAM Key){ m_LastKey = Key; }
	virtual WPARAM GetLastKey(){ return m_LastKey; }

	virtual void SetLastWndMsg(UInt32 Msg){	m_LastWndMsg = Msg;	}
	virtual UInt32 GetLastWndMsg(){	return	m_LastWndMsg; }

	virtual void SetEnable(bool Flag);

	virtual bool IsEnable()
	{
		if (m_Parent && !m_Parent->IsEnable())
			return false;
		return m_IsEnable;
	}

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }

	virtual void SetNext(PCSTRINGA pStr){ m_Next = pStr; }
	virtual PCSTRINGA GetNext(){ return m_Next.c_str();}
	virtual void SetPrev(PCSTRINGA pStr){ m_Prev = pStr; }
	virtual PCSTRINGA GetPrev(){ return m_Prev.c_str();	}


	virtual void DeleteMapItem(StlMap<StringA, UIItem *> &UIIDMap);

	virtual void DeleteUpdateSet(StlSet<UIItem *> &AutoUpdateSet, StlSet<UIItem *> &MouseInSet)
	{
		DeleteUpdateSetR(AutoUpdateSet, MouseInSet);
	}
	virtual void SetDragRect(UInt32 xLeft, UInt32 yTop, UInt32 xRight, UInt32 yBottom)
	{
		SetRect(&m_DragRect, xLeft, yTop, xRight, yBottom);
	}
	virtual Boolean DetectDragRect(Int32 pX, Int32 pY)
	{
		return (!m_NotDetected && pX >= m_DragRect.left + m_X && pX < m_DragRect.right + m_X &&
			pY >= m_DragRect.top + m_Y && pY < m_DragRect.bottom + m_Y);
	}

	virtual void SetParent(UIItem *Parent){m_Parent = Parent;}
	virtual UIItem* GetParent(){return m_Parent;}

	virtual void ChangeParent(UIItem *Parent);

	virtual void Copy(UIItem *Item);
	virtual UIItem *Dup();


	virtual void AddUIItem(UIItem *pItem){}
	virtual void LeaveUIItem(UIItem *Item){}

	virtual void SetDetectImage(bool Flag){m_DetectImage = Flag;}
	virtual bool IsDelectImage(){return m_DetectImage;}

	virtual bool ContinueRun(){ return false; }

	virtual void LeaveParent(UIItem *Parent){}

	virtual bool CheckName(StringA &Name){ return (m_Name == Name); }
	virtual bool CheckName(PCSTRINGA Name){ StringA SName(Name); return (m_Name == SName); }
	virtual bool Equal(UIItem *pItem){ return (this == pItem); };

	inline Int GetOffsetX(){ return m_OffsetX;}
	inline Int GetOffsetY(){ return m_OffsetY;}
	

	virtual inline void SetNotDetected(bool value){m_NotDetected = value;}

	virtual void SetChange(bool Flag){ m_ChangeFlag = Flag; }
	virtual bool IsChange(){ return m_ChangeFlag; }

	virtual bool IsDefaultTips();

	virtual void SetRenderStyle(UInt32 Style);
	virtual UInt32 GetRenderStyle(){ return m_RenderStyle; }

	virtual void SetColor(UInt32 Color){ m_Color = Color; }
	virtual UInt32 GetColor(){ return m_Color; }

	virtual bool IsPanel(){	return false;}
	
	inline UInt32 GetTimeOutCount(){ return m_TimeOutCount;}
	inline UInt32 GetTimeOutPreTime(){ return m_TimeOutPreTime;}
	inline Int GetTimeOutState(){ return m_TimeOutState;}
	inline bool GetEnableTimeOutEvent(){ return m_EnableTimeOutEvent;}

	inline void SetTimeOutCount(UInt32 Value){m_TimeOutCount = Value;}
	inline void SetTimeOutPreTime(UInt32 Value){m_TimeOutPreTime = Value;}
	inline void SetTimeOutState(Int Value){ m_TimeOutState = Value;}
	inline void SetEnableTimeOutEvent(bool Value){m_EnableTimeOutEvent = Value;}

	inline void ResetTimer(){m_TimeOutPreTime = ::timeGetTime();}

	inline void SetUIManager(UIManager * pManager){ m_pUIManager = pManager; }

	inline void SetDragX(Int DragX){ m_DragX = DragX; }
	inline Int  GetDragX(){ return m_DragX; }

	inline void SetDragY(Int DragY){ m_DragY = DragY; }
	inline Int  GetDragY(){ return m_DragY; }

	inline void SetOldDragX(Int OldDragX){ m_OldDragX = OldDragX; }
	inline Int  GetOldDragX(){ return m_OldDragX; }

	inline void SetOldDragY(Int OldDragY){ m_OldDragY = OldDragY; }
	inline Int  GetOldDragY(){ return m_OldDragY; }

	
protected:

	virtual void DeleteUpdateSetR(StlSet<UIItem *> &AutoUpdateSet, StlSet<UIItem *> &MouseInSet);

	Int			m_X, m_Y, m_OffsetX, m_OffsetY;
	UInt		m_Width, m_Height;
	RECT		m_DetectRect;
	RECT		m_DragRect;
	bool		m_IsEnable;
	bool		m_PickUp;
	bool		m_Focus;
	UInt32		m_CanDrag;
	UInt32		m_Color;
	WPARAM		m_LastKey;
	UInt32		m_LastWndMsg;
	UInt32		m_RenderStyle;
	UIItem		*m_Parent;
	Int			m_DragX, m_DragY, m_OldDragX, m_OldDragY;
	
	lua_State			*m_LuaState;
	StringA				m_Name, m_Next, m_Prev;
	StringA				m_LuaFuncTable[MAX_UI_FUNC];
	StringW				m_Tips;

	bool		m_ChangeFlag;
	bool		m_NotDetected;
	bool		m_StopPlaySoundOnce;
	bool		m_DetectImage;

	UInt32		m_TimeOutCount;
	UInt32		m_TimeOutPreTime;
	Int			m_TimeOutState;
	bool		m_EnableTimeOutEvent;

	UIManager *	m_pUIManager;
};




///////////////////////////////////////////////////////////////////////////////


class UIPanel : public UIItem
{
public:
	UIPanel();
	virtual ~UIPanel(){ Dispose(); }

	virtual void Create(){}
	virtual void Dispose();
	
	virtual void Validate(CImage *pImage){}
	virtual void Invalidate(){}
	virtual UInt32 Update(UInt32 dwDelta);
	
	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual void SetImageFileA(PCSTRINGA ImageName, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	virtual void SetImage(CImage *pImage, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);

	void SetImageNil(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
	{
		SetImage(NULL, pLeft, pTop, pRight, pBottom);
	}

	virtual void SetTextureUI(UIManager *pUIManager);
	virtual void SetTexture(LPDIRECT3DDEVICE9 pDevice);
	virtual void SetTextureWH(UInt32 TWidth, UInt32 THeight)
	{
		m_TextureWidth = TWidth;
		m_TextureHeight = THeight;
		m_PanelDirRectMan.SetBoundRect(0, 0, m_TextureWidth, m_TextureHeight);
	}
	virtual void SetWH(UInt32 TWidth, UInt32 THeight)
	{
		m_Width = TWidth;
		m_Height = THeight;
	}
	virtual LPDIRECT3DTEXTURE9 GetTexture(){ return m_Texture; }
	virtual void AddDirtyRect(RECT *pRect){ m_Texture->AddDirtyRect(pRect); }
	virtual UInt32 GetTextureWidth(){ return m_TextureWidth; } 
	virtual UInt32 GetTextureHeight(){ return m_TextureHeight; }
	virtual UInt32 GetBlitWidth(){ return m_TextureWidth; } 
	virtual UInt32 GetBlitHeight(){ return m_TextureHeight; }

	virtual void AddUIItem(UIItem *pItem)
	{
		m_SubItemList.push_back(pItem);
		pItem->SetParent(this);
	}

	virtual void LeaveUIItem(UIItem *pItem);
	virtual void LeaveAllUIItem(){ m_SubItemList.clear(); }

	virtual void MovePos(Int32 X, Int32 Y);
	virtual void SetPos(Int32 pX, Int32 pY);
	virtual void SetOffsetPos(Int32 pX, Int32 pY){ SetPos(pX + m_OffsetX, pY + m_OffsetY); }

	virtual Int32		OnMouseMove(Int32 pX, Int32 pY);
	virtual Int32		OnLButtonUp(Int32 pX, Int32 pY);
	virtual Int32		OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32		OnRButtonUp(Int32 pX, Int32 pY);
	virtual Int32		OnRButtonDown(Int32 pX, Int32 pY);
	virtual Int32		OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ);
	virtual Int32		OnLButtonDBLCLK(Int32 pX, Int32 pY);
	virtual Int32		OnRButtonDBLCLK(Int32 pX, Int32 pY);


	virtual bool		Detect(Int32 pX, Int32 pY);
	virtual bool		DetectSubItem(Int32 pX, Int32 pY);

	virtual PUInt32		GetPixelPtr(Int32 pX, Int32 pY)
	{
		return (PUInt32)(m_BackImage->GetPtrXY(m_BackRect.left + (pX - m_X), m_BackRect.top + (pY - m_Y)));
	}


	virtual UIItem *GetHotSpot()
	{
		return (m_HotSpot && m_HotSpot != this ? m_HotSpot->GetHotSpot() : this);
	}

	virtual void SetHotSpot(UIItem *pUIItem){ m_HotSpot = pUIItem; }

	virtual void SetAllColor(UInt32 Color);
	
	virtual UIItem *GetMouseOver()
	{
		return (m_OnMouseOverItem && m_OnMouseOverItem != this ? m_OnMouseOverItem->GetMouseOver() : this);
	}


	virtual void SetBackColor(UInt32 Color){ m_BackColor = Color; }
	virtual UInt32 GetBackColor(){ return m_BackColor; }

	virtual void SetBoxColor(UInt32 Color){ m_BoxColor = Color; }
	virtual UInt32 GetBoxColor(){ return m_BoxColor; }

	virtual void DeleteMapItem(StlMap<StringA, UIItem *> &UIIDMap)
	{
		UIItem::DeleteMapItem(UIIDMap);
		for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
		{
			(*i)->DeleteMapItem(UIIDMap);
		}
	}


	virtual void DeleteUpdateSet(StlSet<UIItem *> &AutoUpdateSet, StlSet<UIItem *> &MouseInSet)
	{
		
		for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
		{
			(*i)->DeleteUpdateSet(AutoUpdateSet, MouseInSet);
		}
		//james modify some codes here, Add DeleteUpdateSetR
		DeleteUpdateSetR(AutoUpdateSet, MouseInSet);
	}

	virtual void Freeze(){ m_Freeze = true; };
	virtual void Unfreeze(){ m_Freeze = false; };
	virtual bool IsFreeze(){ return m_Freeze; }

	virtual void SetLuaPerLButtonUp(StringA	LuaPerLButtonUp){ m_LuaPerLButtonUp = LuaPerLButtonUp; }
	virtual StringA &GetLuaPerLButtonUp(){ return m_LuaPerLButtonUp; }

	virtual void SetLuaPerRButtonUp(PCSTRINGA	LuaPerRButtonUp){ m_LuaPerRButtonUp = LuaPerRButtonUp; }
	virtual StringA	&GetLuaPerRButtonUp(){ return m_LuaPerRButtonUp; }

	virtual void SetSavePos(Boolean SavePos){ m_SavePos = m_SavePos; }
	virtual Boolean IsSavePos(){ return m_SavePos; }

	virtual void SetAutoDelete(Boolean flag){ m_AutoDeleteImage = flag;	}
	virtual Boolean GetAutoDelete(){ return m_AutoDeleteImage; }

	virtual void SetCanRemove(Boolean flag){ m_CanRemove = flag; }
	virtual Boolean GetCanRemove(){ return m_CanRemove; }

	virtual std::vector<UIItem *> &GetList(){ return m_SubItemList; }
	virtual void StopPlaySoundOnce(){ m_StopPlaySoundOnce = true;}
	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }
	virtual UIItem *GetSubItem(UInt Index)
	{
		if(Index < m_SubItemList.size())
			return m_SubItemList[Index];
	}
	virtual bool IsPanel(){ return true; }
	virtual void RenderDirtyRect();

	virtual Int GetDragWidth(){ return m_DragWidth; }
	virtual void SetDragWidth(Int Width){ m_DragWidth = Width; }
	virtual Int GetDragHeight(){ return m_DragHeight; }
	virtual void SetDragHeight(Int Height){ m_DragHeight = Height; }

	virtual void AddPanelDirtyRect(RECT rect){m_PanelDirRectMan.AddDirtyRectToMan(rect);}
	
	virtual inline void ClearDirtyRect() {m_PanelDirRectMan.ClearDirtyRect();}

	virtual void RenderParent();
	virtual void RenderParent(RECT &RRect);

	virtual inline void SetIsOpened(bool value){ m_IsOpened = value; }
	virtual inline bool GetIsOpened(){ return m_IsOpened; }

	virtual void SetFallowPanel(UIPanel *Panel){ m_FallowPanel  = Panel; }
	virtual UIPanel *GetFallowPanel(){ return m_FallowPanel; }

protected:

	Boolean		m_AutoDeleteImage;
	Boolean		m_SavePos;
	Boolean		m_Freeze;
	std::vector<UIItem *>		m_SubItemList;
	UIItem		 *m_HotSpot;
	UIItem		 *m_OnMouseOverItem;
	Int			m_DragWidth, m_DragHeight;

	CImage		*m_BackImage;
	LPDIRECT3DTEXTURE9		m_Texture;
	UInt32		m_TextureWidth, m_TextureHeight;
	UInt32		m_BackColor, m_BoxColor;
	RECT		m_BackRect;

	StringA				m_LuaPerLButtonUp;
	StringA				m_LuaPerRButtonUp;

	CDirtyRectManager	m_PanelDirRectMan;
	Boolean		m_IsOpened, m_CanRemove;

	UIPanel		*m_FallowPanel;
	
};




//////////////////////////////////////////////////////////


class UIPanelEx : public UIPanel
{
public:
	UIPanelEx() : UIPanel()
	{
		m_HotSpot = NULL;
		m_BackImage = NULL;
		m_BlitWidth = m_BlitHeight = 0;
		m_LineCount = 0;
	}
	virtual ~UIPanelEx()
	{
		
	}

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual PUInt32		GetPixelPtr(Int32 pX, Int32 pY);
	
	virtual void SetLineRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom, UInt32 Count)
	{
		SetRect(&m_LineRect, pLeft, pTop, pRight, pBottom);
		m_LineCount = Count;
		m_Height += (pBottom - pTop) * Count;
	}


	virtual void SetBottomRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
	{
		SetRect(&m_BottomRect, pLeft, pTop, pRight, pBottom);
		m_Height += pBottom - pTop;
	}

	virtual UInt32 GetLineCount()
	{
		return m_LineCount;
	}

	virtual void ChangeCount(UInt32 LineCount)
	{
		m_LineCount = LineCount;

		m_Height = m_BackRect.bottom - m_BackRect.top;
		m_Height += m_BottomRect.bottom - m_BottomRect.top;
		m_Height += (m_LineRect.bottom - m_LineRect.top) * m_LineCount;

		SetDetectRect();
	}

	virtual UInt32 GetBlitWidth(){ return m_BlitWidth; } 
	virtual UInt32 GetBlitHeight(){ return m_BlitHeight; }

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }

	inline RECT * GetLineRect(){ return &m_LineRect;}

	inline UInt32 GetLineRectHeight(){ return m_LineRect.bottom - m_LineRect.top;}

protected:

	UInt32		m_BlitWidth, m_BlitHeight;
	RECT		m_BottomRect;
	RECT		m_LineRect;
	UInt32		m_LineCount;
};



//////////////////////////////////////////////////////////


class UIImage : public UIItem
{
public:
	UIImage() : UIItem()
	{
		m_BackImage = NULL;
		m_AutoDeleteImage = false;
	}
	virtual ~UIImage()
	{
		Dispose();
	}

	virtual void Create(){}
	virtual void Dispose()
	{
		//if(m_AutoDeleteImage)
		if(m_BackImage)
		{
			CImageManager::GetInstance().DeleteImage(m_BackImage);
			m_BackImage = NULL;
		}
		/*
		if (m_BackImage && m_BackImage->GetTextureCode() == 3)
		{
			SafeDeleteObject(m_BackImage);
		}
		*/
	}
	virtual UInt32 Update(UInt32 dwDelta){ return 0; }
	virtual Int32		OnLButtonDown(Int32 pX, Int32 pY);

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);
	virtual void FillImage(UInt32 pColor = 0){	mem4set(m_BackImage->GetPtr(), pColor, m_Width * m_Height);	}


	virtual void SetImage(CImage *pImage, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	virtual void SetImage(UIImage *pUIImage);
	virtual void SetImageFileA(PCSTRINGA Filename, Int32 pLeft = 0, Int32 pTop = 0, Int32 pRight = 0, Int32 pBottom = 0);
	virtual void SetImageFile(PCSTRING Filename, Int32 pLeft = 0, Int32 pTop = 0, Int32 pRight = 0, Int32 pBottom = 0);
	virtual void SetAutoDelete(Boolean flag){ m_AutoDeleteImage = flag; }
	virtual Boolean GetAutoDelete(){ return m_AutoDeleteImage; }
	virtual void LoadCZipImage(PCSTRINGA Filename);
	virtual void LoadCZipMem(Byte *ZipBuf, UInt ZipSize);


	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIImage *GetUIImagePtr(){ return this; }
	virtual UIItem *GetUIItemPtr(){  return this; }

	virtual void SetWidth(UInt32 Width)
	{
		m_Width = Width;
		m_BackRect.right = m_BackRect.left + Width;
		SetDetectRect();
	}

	virtual void SetHeight(UInt32 Height)
	{
		m_Height = Height;
		m_BackRect.bottom = m_BackRect.top + Height;
		SetDetectRect();
	}

	virtual void Set2TopLeft()
	{
		SetOffsetXY(0, 0);
		SetDetectRect();
	}

	virtual void Set2BottomLeft()
	{
		SetOffsetXY(0, m_Parent->GetHeight() - m_Height);
		SetDetectRect();
	}

	virtual void Set2TopRight()
	{
		SetOffsetXY(m_Parent->GetWidth() - m_Width, 0);
		SetDetectRect();
	}

	virtual void Set2BottomRight()
	{
		SetOffsetXY(m_Parent->GetWidth() - m_Width, m_Parent->GetHeight() - m_Height);
		SetDetectRect();
	}

	virtual CImage * GetImage(){ return m_BackImage;}
	virtual RECT * GetBackRect(){ return &m_BackRect;}

	
protected:

	Boolean		m_AutoDeleteImage;
	
	CImage		*m_BackImage;
	RECT		m_BackRect;
};


/////////////////////////////////////////////////////////////////////////////////////


class UILinerImage : public UIImage
{
public:
	UILinerImage() : UIImage()
	{
		m_Liner = 0;
	}
	virtual ~UILinerImage()
	{
	}

	virtual void Create()
	{
	}

	virtual void Validate(CImage *pImage)
	{
	}




	virtual Int32		OnLButtonDown(Int32 pX, Int32 pY)
	{
		return UIItem::Detect(pX, pY);
	}

	virtual void Render(CImage *pImage)
	{
		if(m_BackImage)
			if(m_Liner > 0)
				Liner8888To8888(m_OffsetX, m_OffsetY, m_Liner, m_BackImage, pImage, &m_BackRect);
	}

	virtual void Render(CImage *pImage, RECT &RRect);
	
	virtual void SetLinerWidth(UInt32 Liner)
	{
		m_Liner = Liner;
	}


	virtual UInt32 GetLinerWidth()
	{
		return m_Liner;
	}

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}

	virtual void SetBaseWidth(UInt32 Width)
	{
		m_Width = m_BaseLiner = Width;
		SetDetectRect();
	}

	virtual void SetDenumerator(UInt32 Denumerator);

	virtual void SetNumerator(UInt32 Numerator);

	virtual void FullWidth()
	{
		m_Width = m_Liner = m_BaseLiner;
	}

	virtual void Set2Top()
	{
		m_Width = m_BaseLiner = m_Liner = m_Parent->GetWidth();
		SetOffsetXY(0, 0);
		SetDetectRect();
	}

	virtual void Set2Bottom()
	{
		m_Width = m_BaseLiner = m_Liner = m_Parent->GetWidth();
		SetOffsetXY(0, m_Parent->GetHeight() - m_Height);
		SetDetectRect();
	}

	virtual void SetRealRect(RECT &RRect);

	virtual void SetRealOffsetRect(RECT &RRect);
	
protected:

	UInt32		m_Liner;
	UInt32		m_BaseLiner, m_Numerator, m_Denumerator;
	//m_Numerator = ?’ï , m_Denumerator = ??;
};



class UIBarImage : public UILinerImage
{
public:
	UIBarImage() : UILinerImage(){}
	virtual ~UIBarImage(){}

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual const type_info &GetTypeID(){ return typeid(this);}
	virtual UIItem *GetUIItemPtr(){ return this; }

	
protected:

};


/////////////////////////////////////////////////////////////////////////////////////


class UILinerImageV : public UILinerImage
{
	public:
	UILinerImageV() : UILinerImage()
	{
		
	}
	virtual ~UILinerImageV()
	{
	}

	virtual void Render(CImage *pImage, RECT &RRect);
	
	virtual void SetLinerHeight(UInt32 Liner)
	{
		m_Height = m_Liner = Liner;
		SetDetectRect();
	}


	virtual UInt32 GetLinerHeight()
	{
		return m_Liner;
	}

	virtual void SetBaseHeight(UInt32 Height)
	{
		m_BaseLiner = Height;
	}

	virtual void Render(CImage *pImage)
	{
		if(m_BackImage)
			if(m_Liner > 0)
				Liner8888To8888V(m_OffsetX, m_OffsetY, m_Liner, m_BackImage, pImage, &m_BackRect);
	}

	virtual void Set2Left()
	{
		m_Height = m_BaseLiner = m_Liner = m_Parent->GetHeight();
		SetOffsetXY(0, 0);
		SetDetectRect();
	}

	virtual void Set2Right()
	{
		m_Height = m_BaseLiner = m_Liner = m_Parent->GetHeight();
		SetOffsetXY(m_Parent->GetWidth() - m_Width, 0);
		SetDetectRect();
	}

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}

};



/////////////////////////////////////////////////////////////////////////////////////


class UIAniIcon : public UIImage
{
public:
	UIAniIcon() : UIImage()
	{
		m_Count = m_Offset = 0;
		m_LastTime = 0;
		m_TimeOffset = 0;
		SetRect(&m_PreDirtyRect, 0, 0, 0, 0);
	}

	virtual ~UIAniIcon()
	{
		Dispose();
	}

	virtual void Create()
	{
		m_Align = 0;
		m_Rotation = 0;
		m_AnimationIndex = 0;
		m_Detect = 0;
		SetRect(&m_PreDirtyRect, 0,0,0,0);
	}
	virtual void Dispose()
	{
	}

	virtual void Validate(CImage *pImage)
	{

	}
	virtual void Invalidate()
	{

	}

	virtual void Render(CImage *pImage);

	virtual void Render(CImage *pImage, RECT &RRect);

	virtual void SetImageFileA(PCSTRINGA Filename, Int32 pLeft = 0, Int32 pTop = 0, Int32 pRight = 0, Int32 pBottom = 0)
	{
	}


	virtual void SetImageFile(PCSTRING Filename, Int32 pLeft = 0, Int32 pTop = 0, Int32 pRight = 0, Int32 pBottom = 0)
	{
	}
	
	virtual void SetImage(CImage *pImage, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
	{
	}

	virtual void SetAnimationData(LPCTSTR Filename, int equipType = 0);
	virtual void SetAnimationDataA(PCSTRINGA Filename, int equipType = 0)
	{
		SetAnimationData(String(Filename).c_str(), equipType);
	}
	virtual void SetAnimationData(CAnimatedEntity * pAniEnt);
	virtual void SetAnimationData(UIAniIcon * pAniIcon);

	virtual void LockResources()
	{
		GetAniEnt()->LockResources();
	}

	virtual void DrawEntityToImage(CImage *pImage, RECT * Rect);


	virtual void SetCount(UInt32	Count)
	{
		m_Count = Count;
	}

	virtual UInt32 GetCount()
	{
		return m_Count;
	}

	virtual void SetTimeOffset(UInt32	TimeOffset)
	{
		m_TimeOffset = TimeOffset;
	}

	virtual UInt32 GetTimeOffset()
	{
		return m_TimeOffset;
	}
	
	virtual void DeleteUpdateSet(StlSet<UIItem *> &AutoUpdateSet, StlSet<UIItem *> &MouseInSet)
	{
		DeleteUpdateSetR(AutoUpdateSet, MouseInSet);
	}
	
	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }

	inline CAnimatedEntity * GetAniEnt(){return &m_AniEnt;}

	void SetAlign(Int Value);

	void ResetPosition();

	void SetAnimationIndex(Int AnimationIndex);
	void SetRotation(Int Rotation);
	inline Int GetRotation(){return m_Rotation;}

	virtual Int32	OnLButtonDown(Int32 pX, Int32 pY);

	virtual bool Detect(Int32 pX, Int32 pY);

	virtual void SetDetect(Int Value);
	
	virtual void CleanPreDirty(CImage *pImage);

	inline RECT GetPreDirtyRect(){return m_PreDirtyRect;}

	virtual void SetRealRect(RECT &RRect);
	virtual void SetRealOffsetRect(RECT &RRect);

	virtual UInt32 Update(UInt32 dwDelta);
	virtual UInt32 UpdateNow();
	
	inline void ClearAniData(){m_AniEnt.InitEntPicInfo();}

	bool GetHasAni();


protected:

	RECT		m_BaseLinerRect;

	UInt32		m_Count, m_Offset;
	UInt32		m_LastTime, m_TimeOffset;

	CAnimatedEntity m_AniEnt;

	Int m_Align;
	Int m_Rotation;
	Int m_AnimationIndex;
	Int m_Detect;
	RECT m_PreDirtyRect;
};





//////////////////////////////////////////////////////////


class UIStaticText : public UIItem
{
public:
	enum {
		ALIGN_LEFT = 0, 
		ALIGN_RIGHT = 1, 
		ALIGN_CENTER = 2, 
	};

	UIStaticText() : UIItem()
	{
		m_BufImage = NULL;
		m_Font = NULL;
		m_IsPassword = false;
		m_NumOnly = false;
		m_PicthWidth = 0;
		m_FontStyle  =  0;
		m_Align = ALIGN_LEFT;
		m_LinePitch  = 0;

		m_SelStartX = m_SelStart = m_SelEnd = m_SelHeight = 0;
		m_SelColor = 0xFFFF0000;
	}

	virtual ~UIStaticText(){ Dispose();}

	virtual void Create(){};

	virtual void Dispose(){ CImageManager::GetInstance().DeleteImage(m_BufImage);}

	virtual void Validate(CImage *pImage){};

	virtual void Invalidate(){};

	virtual UInt32 Update(UInt32 dwDelta)
	{
		Redraw();
		return 1;
	};

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual void SetText(PCSTRINGW pText){ m_Text = pText; }

	virtual void SetTextA(PCSTRINGA pText)
	{
		StringW m_TempStr;

		UTF82StringW(m_TempStr, pText);
		SetText(m_TempStr.c_str());
	}
	virtual void LuaSetText(PCSTRINGA pText){ UTF82StringW(m_Text, pText);}
	virtual void SetText(StringW	&pText){ m_Text = pText;}
	virtual void AddText(PCSTRINGW pText){ m_Text += pText; }

	virtual void AddTextA(PCSTRINGA	Text)
	{
		StringW m_TempStr;

		UTF82StringW(m_TempStr, Text);
		m_Text += m_TempStr.c_str();
	}

	virtual void AddText(StringW	&pText){ m_Text += pText;}
	virtual void SetFontStyle(UInt32 pFontStyle){ m_FontStyle = pFontStyle;};
	inline UInt32 GetFontStyle(){ return m_FontStyle;}

	
	virtual void SetFont(CFont *pFont)
	{
		m_Font = pFont;
		m_SelHeight = m_Font->GetHeight();
	};

	virtual void SetFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight)
	{
		m_Font = CFontManager::GetInstance().CreateFont(pName, pWidth, pHeight);
		m_SelHeight = m_Font->GetHeight();
	}

	virtual CFont *GetFont(){ return m_Font; }
	virtual UInt32 GetFontWidth(){ return m_Font->GetWidth(); }
	virtual UInt32 GetFontHeight(){ return m_Font->GetHeight(); }

	virtual void SetFontColor(D3DCOLOR pColor){	m_FontColor = pColor; };
	virtual void SetShadowColor(D3DCOLOR pColor){ m_ShadowColor = pColor; };
	inline D3DCOLOR GetFontColor(){ return m_FontColor;	}
	inline D3DCOLOR GetShadowColor(){ return m_ShadowColor; }

	virtual void SetImage(UInt32 pRow, UInt32 pColumn);
	virtual void SetImageWH(UInt32 Width, UInt32 Height);
	virtual void FillImage(UInt32 pColor = 0)
	{	
		mem4set(m_BufImage->GetPtr(), pColor, m_Width * m_Height);
	}
	virtual void DrawFont();

	virtual void Redraw()
	{
		SetChange(true);
		FillImage();
		DrawFont();
		RenderParent();
	}

	virtual void SetPassword(Boolean Flag){	m_IsPassword = Flag; }
	virtual Boolean GetPassword(){ return m_IsPassword;	}

	virtual void SetNumOnly(Boolean Flag){	m_NumOnly = Flag; }
	virtual Boolean GetNumOnly(){ return m_NumOnly; }

	virtual void ClearText(){ m_Text.clear(); }
	virtual PCSTRINGW GetText(){ return (m_Text.c_str()); }

	virtual StringW &GetString(){ return m_Text; }
	virtual UInt32 GetTextSize(){ return (UInt32)(m_Text.size()); }

	void SetLinePitch(UInt32 LinePitch){ m_LinePitch = LinePitch; }
	UInt32 GetLinePitch(){ return m_LinePitch; }
	
	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }
	virtual UIStaticText *GetUIStaticText(){ return this; }

	virtual void SetSelColor(UInt32 SelColor){ m_SelColor = SelColor; }
	virtual UInt32 GetSelColor(){ return m_SelColor; }
	virtual void SetSelStart(UInt32 SelStart){ m_SelStart = SelStart; }
	virtual UInt32 GetSelStart(){ return m_SelStart; }
	virtual void SetSelEnd(UInt32 SelEnd){ m_SelEnd = SelEnd; }
	virtual UInt32 GetSelEnd(){ return m_SelEnd; }
	virtual void SetSelHeight(UInt32 SelHeight){ m_SelHeight = SelHeight; }
	virtual UInt32 GetSelHeight(){ return m_SelHeight; }

	virtual bool Empty(){ return m_Text.empty(); }
	virtual void SetAlign(UInt32 Align){ m_Align = Align; }
	virtual UInt32 GetAlign(){ return m_Align; }

	
	
	

protected:

	StringW			m_Text;
	D3DCOLOR		m_FontColor;
	D3DCOLOR		m_ShadowColor;
	UInt32			m_FontStyle;
	UInt32			m_Align;
	CFont			*m_Font;
	Boolean			m_IsPassword, m_NumOnly;

	UInt32			m_Row, m_Column;
	UInt32			m_PicthWidth;
	UInt32			m_LinePitch;

	CImage			*m_BufImage;

	UInt32			m_SelStart, m_SelEnd, m_SelStartX, m_SelColor, m_SelHeight;
};


/////////////////////////////////////////////////////////////////
class UIDynamicText :public UIStaticText
{
public:
	UIDynamicText() : UIStaticText()
	{
		m_StartX  =  0;
		m_PreUpdateTime = 0;
		m_PanellStartX = 0;
		m_MoveWidth = 0;
		m_DrawWidth = 0;
	}
	virtual UInt32 Update(UInt32 dwDelta);
	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual void DrawFont();

	inline void SetSpeed(Int32 value){m_Speed = value;}
	inline Int32 GetSpeed(){return m_Speed;}

	inline void SetMoveWidth(Int32 value){m_MoveWidth = value;}
	inline Int32 GetMoveWidth(){return m_MoveWidth;}

	void Reset();

	inline UInt32 GetCount(){return m_count;}
protected:

	Int32 m_StartX;
	Int32 m_PanellStartX;
	Int32 m_Speed;
	Int32 m_MoveWidth;
	UInt32 m_PreUpdateTime;

	UInt32 m_count;

	UInt32	m_DrawWidth;

};
/////////////////////////////////////////////////////////////////
enum BUTTON_STATE
{
	_ButtonNormal = 0,
	_ButtonMouseOver,
	_ButtonClick,
	_ButtonDisable,
	_ButtonNotice,
	_MaxButton_State
};

class UIButton : public UIItem
{
public:
	UIButton();
	virtual ~UIButton()
	{
		Dispose();
	}

	virtual void Create()
	{
	}
	virtual void Dispose();
	virtual void Validate(CImage *pImage)
	{
	}
	virtual void Invalidate()
	{
	}
	virtual UInt32 Update(UInt32 dwDelta)
	{
		UInt32 result = 0;
		if (m_AniIcon[m_State])
		{
			result = m_AniIcon[m_State]->Update(dwDelta);
		}
		return result;
	}

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);
	
	virtual void SetNotMouseOver();
	virtual void SetMouseOver();

	virtual Int32	OnMouseMove(Int32 pX, Int32 pY);
	virtual Int32	OnLButtonUp(Int32 pX, Int32 pY);
	virtual Int32	OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32	OnRButtonUp(Int32 pX, Int32 pY);
	virtual Int32	OnRButtonDown(Int32 pX, Int32 pY);
	virtual Int32	OnLButtonDBLCLK(Int32 pX, Int32 pY);
	virtual Int32	OnRButtonDBLCLK(Int32 pX, Int32 pY);

	
	virtual void	SetRect(Int32 pIndex, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);

	virtual void	SetSubOffSet(Int32 pIndex, Int32 OffX, Int32 OffY);

	virtual void	SetAniIcon(Int32 pIndex, UIAniIcon * pAniIcon);
	virtual void	SetImageFileA(PCSTRINGA	ImageName)
	{
		StringW Filename;
		
		UTF82StringW(Filename, ImageName);
		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		SetImage(Image);
	}
	
	virtual void	SetImage(CImage *pImage)
	{
		m_PicImage = pImage;
	}
	virtual void	SetState(BUTTON_STATE pState);
	virtual void	SetNormalState();
	virtual void	SetMouseOverState();
	virtual void	SetClickState();
	virtual void	SetDisableState();
	virtual void	SetNoticeState();
	
	virtual void	SetStateDetectRect(BUTTON_STATE pState = _ButtonNormal);
	virtual CImage	*GetImage()
	{
		return m_PicImage;
	}

	virtual void  UnPickUp()
	{
		UIItem::UnPickUp();
		SetState(_ButtonNormal);
	}

	virtual void	SimpleSetState(BUTTON_STATE pState);

	virtual void	SimpleUnPickUp()
	{
		UIItem::UnPickUp();
		SimpleSetState(_ButtonNormal);
	}

	virtual void	SetSimple(bool Flag)
	{
		m_Simple = Flag;
	}

	virtual bool	GetSimple()
	{
		return m_Simple;
	}


	virtual void SetLockState(bool State)
	{
		m_LockState = State;
	}


	virtual bool IsLockState()
	{
		return m_LockState;
	}

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}

	virtual void SetParent(UIItem *pParent);
	
	virtual void SetOffsetPos(Int32 pX, Int32 pY);

	virtual void SetRealRect(RECT &RRect);
	virtual void SetRealOffsetRect(RECT &RRect);

	inline Int32 GetState(){return m_State;}
	
	
protected:
	
	RECT			m_PicRect[_MaxButton_State];
	UIAniIcon *		m_AniIcon[_MaxButton_State];
	bool			m_UseFlag[_MaxButton_State];
	Int32			m_SubOffsetX[_MaxButton_State];
	Int32			m_SubOffsetY[_MaxButton_State];
	CImage			*m_PicImage;
	Int32			m_State;
	UIAniIcon *		m_PreAniIcon;
	bool			m_Simple;
	bool			m_LockState;
};


////////////////////////////////////////////////////////////


class UICheckBox : public UIItem
{
public:
	UICheckBox() : UIItem()
	{
		m_PicImage = NULL;
		m_State = false;
		m_CheckX = m_CheckY = 0;
	}
	virtual ~UICheckBox(){ Dispose(); }
	virtual void Dispose(){ CImageManager::GetInstance().DeleteImage(m_PicImage);  m_PicImage = NULL; }
	
	virtual UInt32 Update(UInt32 dwDelta)
	{
		return 0;
	}
	virtual void Render(CImage *pImage)
	{
		Alpha8888To8888S(m_OffsetX, m_OffsetY, m_PicImage, pImage, m_PicRect, m_RenderStyle, m_Color);
		Alpha8888To8888S(m_OffsetX + m_CheckX, m_OffsetY + m_CheckY, m_PicImage, pImage, m_PicRect + (m_State ? 2 : 1), m_RenderStyle, m_Color);
	}

	virtual void Render(CImage *pImage, RECT &RRect);
	
	virtual Int32	OnMouseMove(Int32 pX, Int32 pY)
	{
		bool	ret = Detect(pX, pY);
		return ret;
	}
	
	virtual void	SetRect(Int32 pIndex, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	virtual void	SetImage(CImage *pImage)
	{
		m_PicImage = pImage;
	}

	virtual void	SetImageFileA(PCSTRINGA ImageName);
	virtual void	SetState(bool pState);
	
	virtual bool GetState()
	{
		return m_State;
	}

	virtual CImage	*GetImage()
	{
		return m_PicImage;
	}

	virtual Int32		OnLButtonUp(Int32 pX, Int32 pY);
	virtual Int32		OnLButtonDown(Int32 pX, Int32 pY);
	
	virtual void SetCheckXY(UInt32 X, UInt32 Y)
	{
		m_CheckX = X; m_CheckY = Y;
	}

	virtual void StopPlaySoundOnce()
	{
		m_StopPlaySoundOnce = true;
	}

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }

protected:
	
	RECT			m_PicRect[_MaxButton_State];
	CImage			*m_PicImage;
	bool			m_State;
	UInt32			m_CheckX, m_CheckY;

};


////////////////////////////////////////////////////////////


class UIListBox : public UIStaticText
{
public:
	UIListBox() : UIStaticText()
	{
		m_BarWidth = m_SelIndex = m_StartIndex = m_ScanIndex = 0;
		m_SelBarColor = D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF);
		m_SelFontColor = D3DCOLOR_ARGB(0xFF, 0, 0xFF, 0);
	}
	virtual ~UIListBox()
	{
		Dispose();
	}

	virtual void Create(){};
	virtual void Dispose()
	{
		m_StringList.clear();
	}


	virtual Int32	OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32 OnKeyDown(WPARAM wParam);
	virtual Int32 SetCurSel(int iCurSel);
	
	virtual void SetBarWidth(UInt32	Width)
	{
		m_BarWidth = Width;
		m_Width = Width;
		SetDetectRect();
	}

	virtual void	SetDetectRect()
	{
		m_DetectRect.left = m_X;
		m_DetectRect.top = m_Y;
		m_DetectRect.right = m_X + m_BarWidth;
		m_DetectRect.bottom = m_Y + m_Height;
	}

	virtual void SetBarRect(RECT &rect, Int32 pIndex);

	virtual void AddString(PCSTRINGW pString)
	{
		m_StringList.push_back(pString);
	}

	virtual void AddStringA(PCSTRINGA Text)
	{
		if(Text)
		{
			StringW m_TempStr;

			UTF82StringW(m_TempStr, Text);
			AddString(m_TempStr.c_str());
		}
	}
	virtual void DelString(UInt32 pIndex)
	{
		if (pIndex<m_StringList.size()) {
			StlVector<StringW>::iterator it = m_StringList.begin();
			for (UInt32 i=0;i<pIndex;i++) {
				if (it != m_StringList.end())
					it++;
			}
			if (it != m_StringList.end())
				m_StringList.erase(it);
		}
	}
	virtual void DrawFont();

	virtual void UpSel();
	virtual void DownSel();
	virtual void Home();
	virtual void End();
	virtual void PageUp();
	virtual void PageDown();
	virtual Int32 GetBarPos();
	virtual PCSTRINGW GetBarString();


	virtual void SetSelBarColor(D3DCOLOR pSelBarColor)
	{
		m_SelBarColor = pSelBarColor;
	}

	virtual void SetSelFontColor(D3DCOLOR pSelFontColor)
	{
		m_SelFontColor = pSelFontColor;
	}

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}

	
	
	virtual UInt32 GetStringListSize()
	{
		return (UInt32)m_StringList.size();
	}

	
protected:

	StlVector<StringW>		m_StringList;
	Int32			m_SelIndex, m_StartIndex;
	UInt32			m_ScanIndex;
	UInt32			m_BarWidth;
	D3DCOLOR		m_SelBarColor, m_SelFontColor;
};



//////////////////////////////////////////////////////////////////////////


class UIPopUpMenu : public UIListBox
{
public:
	UIPopUpMenu() : UIListBox()
	{
		
	}
	virtual ~UIPopUpMenu()
	{
		Dispose();
	}

	virtual void Create(){};
	virtual void Dispose()
	{
		m_FuncCodeList.clear();
	}

	virtual Int32 OnMouseMove(Int32 pX, Int32 pY);
	
	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}

	virtual UIStaticText *GetUIStaticText()
	{
		return this;
	}

	virtual UIListBox *GetUIListBox()
	{
		return this;
	}
	
	virtual void AddString(PCSTRINGW pString)
	{
	}

	virtual void AddString(PCSTRINGW pString, Int32 FuncCode, PCSTRINGA pNextMenuName)
	{
		m_StringList.push_back(pString);
		m_FuncCodeList.push_back(FuncCode);
		m_NextMenu.push_back(pNextMenuName);
	}

	virtual void AddStringA(PCSTRINGA Text, Int32 FuncCode, PCSTRINGA pNextMenuName)
	{
		if(Text)
		{
			StringW m_TempStr;

			UTF82StringW(m_TempStr, Text);
			AddString(m_TempStr.c_str(), FuncCode, pNextMenuName);
		}
	}

	virtual Int32 GetBarCode()
	{
		if(m_FuncCodeList.size())
		{
			return m_FuncCodeList[m_SelIndex + m_StartIndex];
		}
		else
			return -1;
	}

	virtual void SetNotMouseOver();
	
	virtual PCSTRINGA GetNextMenu()
	{
		if(m_NextMenu.size())
		{
			if(!m_NextMenu[m_SelIndex + m_StartIndex].empty())
				return m_NextMenu[m_SelIndex + m_StartIndex].c_str();
		}
		
		return NULL;
	}

	virtual void SetLinePitch(UInt32 LinePitch)
	{
		UIStaticText::SetLinePitch(LinePitch);
	}

	virtual void SetBarWidth(UInt32	Width)
	{
		UIListBox::SetBarWidth(Width);
	}

	virtual void SetSelBarColor(D3DCOLOR pSelBarColor)
	{
		UIListBox::SetSelBarColor(pSelBarColor);
	}

	virtual void SetSelFontColor(D3DCOLOR pSelFontColor)
	{
		UIListBox::SetSelFontColor(pSelFontColor);
	}

	virtual void ClearData();

protected:

	StlVector<Int32>		m_FuncCodeList;
	StlVector<StringA>		m_NextMenu;
};



//////////////////////////////////////////////////////////////////////////

class UIScrollBarV : public UIItem	//úÊú\Bar
{
public:
	UIScrollBarV();
	virtual	~UIScrollBarV()
	{
		Dispose();
	}

	virtual void Create()
	{
	}
	virtual void Dispose();

	virtual void Validate(CImage *pImage)
	{
	}
	virtual void Invalidate()
	{
	}
	virtual UInt32 Update(UInt32 dwDelta)
	{
		return 0;
	}
	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual void InitButton();
	virtual UIButton *GetPrevBtn()
	{
		return m_PrevBtn;
	}
	virtual UIButton *GetNextBtn()
	{
		return m_NextBtn;
	}
	virtual UIButton *GetMoveBtn()
	{
		return m_MoveBtn;
	}
	virtual void SetPrevButton(UIButton *pButton)
	{
		m_PrevBtn = pButton;
	}
	virtual void SetNextButton(UIButton *pButton)
	{
		m_NextBtn = pButton;
	}
	virtual void SetMoveButton(UIButton *pButton)
	{
		m_MoveBtn = pButton;
	}
	
	virtual void	SetNotMouseOver();

	virtual void	MovePos(Int32 pX, Int32 pY);
	virtual Int32	OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32	OnLButtonUp(Int32 pX, Int32 pY);
	virtual Int32	OnMouseMove(Int32 pX, Int32 pY);
	virtual Int32	OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ);

	virtual void	MoveUp();
	virtual void	MoveDown();

	virtual UIItem *GetHotSpot();
	
	virtual void SetBarRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom);
	
	virtual void SetBarWH(UInt32 pWidth, UInt16 pHeight);
	virtual void SetBarHeight(UInt32 pHeight);
	
	virtual bool Detect(Int32 pX, Int32 pY);

	virtual void SetImage(CImage *pImage)
	{
		m_Image = pImage;
	}
	virtual void SetNumCount(UInt32	pNumCount)
	{
		m_NumCount = pNumCount;
	}
	virtual UInt32 GetNumCount()
	{
		return m_NumCount;
	}
	
	virtual UInt32 GetSelNum()
	{
		return m_SelNum;
	}

	virtual void SetSelNum(UInt32 SelNum);
	

	virtual void HomeScroll();
	virtual void EndScroll();

	virtual void ResetNumCount(UInt32	pNumCount);

	virtual void SetBarRenderType(UInt32 pType)
	{
		m_BarRenderType = pType;
	}
	virtual void SetBarColor(D3DCOLOR pBarColor)
	{
		m_BarColor = pBarColor;
	}

	virtual void UnPickUp();
	virtual void SetOffsetPos(Int32 pX, Int32 pY);
	virtual void SetOffsetXY(Int32 pX, Int32 pY);

	virtual void SetRealRect(RECT &RRect)
	{
		RRect.left = m_X;
		RRect.top = m_Y;
		RRect.right = m_X + m_Width;
		RRect.bottom = m_Y + m_Height;
	}

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}
	
	

	virtual void SetBarInfo();

	virtual void SetParent(UIItem *Parent);
	
protected:
	UIButton	*m_PrevBtn, *m_NextBtn, *m_MoveBtn;
	UInt32		m_PrevBtnOffsetX, m_PrevBtnOffsetY;
	UInt32		m_NextBtnOffsetX, m_NextBtnOffsetY;
	UInt32		m_MoveBtnOffsetX, m_MoveBtnOffsetY;
	Int32		m_MoveBtnOffsetGo;
	RECT		m_BarRect;
	CImage		*m_Image;
	bool		m_IsRefImage;
	UInt32		m_PrevCount, m_NextCount;
	UInt32		m_ScrollStart, m_ScrollAreaLen, m_SelNum, m_NumCount;
	Int32		m_MoveSetPoint;
	UInt32		m_BarRenderType;
	D3DCOLOR	m_BarColor;
	UIItem		 *m_HotSpot;
	
	friend class UIScrollListBox;
};


//////////////////////////////////////////////////////////////////////////////////


class UIScrollBarH : public UIScrollBarV	//•V?Bar
{
public:
	UIScrollBarH();
	virtual	~UIScrollBarH()
	{
		Dispose();
	}

	virtual Int32	OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32	OnMouseMove(Int32 pX, Int32 pY);

	virtual void	MoveUp();
	virtual void	MoveDown();

	virtual void ResetNumCount(UInt32 pNumCount);

	virtual void SetBarWidth(UInt32 pWidth);
	virtual void SetOffsetPos(Int32 pX, Int32 pY);

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}
	

	virtual void SetBarInfo();
	
	
protected:
	friend class UIScrollListBox;
};


///////////////////////////////////////////////////////////////////////////


class UIScrollListBox : public UIItem
{
public:
	UIScrollListBox() : UIItem()
	{
		m_ListBox = SafeCreateObject(UIListBox);
		m_ScrollBarV = NULL;
		m_ScrollBarH = NULL;

		m_HotSpot = NULL;
	}
	virtual ~UIScrollListBox()
	{
		Dispose();
	}

	virtual void Dispose()
	{
		SafeDeleteObject(m_ListBox);
		SafeDeleteObject(m_ScrollBarV);
		SafeDeleteObject(m_ScrollBarH);
	}
	virtual void Validate(CImage *pImage)
	{
	}
	virtual void Invalidate()
	{
	}
	virtual UInt32 Update(UInt32 dwDelta)
	{
		return 0;
	}
	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);


	virtual bool Detect(Int32 pX, Int32 pY);

	virtual Int32	OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32	OnLButtonUp(Int32 pX, Int32 pY);
	virtual Int32	OnMouseMove(Int32 pX, Int32 pY);


	virtual void SetupScrollBarV();
	virtual void SetupScrollBarH();

	virtual void SetPos(Int32 pX, Int32 pY);
	virtual void MovePos(Int32 pX, Int32 pY);
	
	virtual UIListBox	 *GetListBox()
	{
		return m_ListBox;
	}
	virtual UIScrollBarV *GetScrollBarV()
	{
		return m_ScrollBarV;
	}
	virtual UIScrollBarH *GetScrollBarH()
	{
		return m_ScrollBarH;
	}
	
	virtual void UnPickUp();

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIItem *GetUIItemPtr()
	{
		return this;
	}

protected:

	UIItem		 *m_HotSpot;
	UIListBox	 *m_ListBox;
	UIScrollBarV *m_ScrollBarV;
	UIScrollBarH *m_ScrollBarH;
		
};


////////////////////////////////////////////////////////////////////////////

/*
#define MAX_CANDLIST 10
#define MAX_COMPSTRING_SIZE 256


void IMEInit();
void IMEUninit();
*/

/*
class IME_Layout : public Reuben::Utility::Singleton<IME_Layout>
{
public:
	IME_Layout();
	~IME_Layout()
	{
		Release();
	}

	SINGLETON(IME_Layout);

	void Init();
	void Release();
	
	void Update(UInt32 dwDelta);
	void Render(CImage *pImage);
	void Blit(LPD3DXSPRITE pSprite);

	void ClearInputLine();
	void RedrawInputLine(PCSTRINGW pStr);

	void ClearCandidate();
	void RedrawCandidate(UInt32 pIndex, PCSTRINGW pStr);


	void CheckInputLocale();
	void CheckToggleState(HWND hWnd);
	void GetPrivateReadingString(HWND hWnd);


	void SetupImeAPI();

	UInt32 GetImeId(UINT uIndex = 0);

	Int32	OnIME(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	void SetTexture(LPDIRECT3DTEXTURE9 pTexture)
	{
		m_Texture = pTexture;
	}


private:

	CFont	*m_Font;
	CImage	*m_InputLineBuf;
	CImage	*m_CandidateBuf;
	CImage	*m_InputLineBG;
	CImage	*m_CandidateBG;

	LPDIRECT3DTEXTURE9 m_Texture;

	Int32		m_X, m_Y, m_LX, m_LY;
	UInt32	m_State;

	StringW	m_ResultStr;

};
*/


/*
struct CCandList
{
    CharW awszCandidate[MAX_CANDLIST][256];
    Int32   nFirstSelected; // First character position of the selected string in HoriCand
    Int32   nHoriSelectedLen; // Length of the selected string in HoriCand
    UInt32 dwCount;       // Number of valid entries in the candidate list
    UInt32 dwSelection;   // Currently selected candidate entry relative to page top
    UInt32 dwPageSize;
    Int32   nReadingError; // Index of the error character
    bool  bShowWindow;   // Whether the candidate list window is visible
    RECT  rcCandidate;   // Candidate rectangle computed and filled each time before rendered
};
*/


const Int32 CursorStartX = 2;



class UIEditBox : public UIStaticText
{
public:
	UIEditBox();
	virtual ~UIEditBox()
	{

	}

	virtual Int32 OnKeyDown(WPARAM wParam);
	virtual Int32 OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32 OnLButtonUp(Int32 pX, Int32 pY);
	virtual Int32 OnMouseMove(Int32 pX, Int32 pY);
	virtual Int32 OnChar(WPARAM wParam);

	virtual void SetNotMouseOver();

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual void SetText(PCSTRINGW pText);
	virtual void SetText(PCSTRINGA pText);
	virtual void SetText(StringW	&pText);


	virtual void Dispose();
	virtual void AddText(PCSTRINGW pText);
	virtual void AddText(PCSTRINGA pText);

	virtual void ClearWaitText(){ m_WaitText.clear(); }
	virtual void AddWaitText(PCSTRINGW pText){ m_WaitText += pText; }
	virtual void AddWaitText(PCSTRINGA pText){ m_WaitText += StringW(pText); }
	virtual void SendWaitText(){ InsertTextS(m_WaitText); m_WaitText.clear(); }


	virtual void InsertText(PCSTRINGW Text);
	virtual void InsertTextS(StringW &Text);
	virtual void InsertText(PCSTRINGA Text);
	virtual void BackSpace();
	virtual UInt32 DelSelText(UInt32 PitchHeight, UInt32 RealFontHeight);

	virtual UInt32 Home();
	virtual UInt32 End();

	virtual void SetFocus(bool pState);
	
	virtual void ClearText();

	static void SetupCursor(CRender *pRender);

	virtual void DrawFont();

	virtual void SetEditLimit(UInt32 Limit){ m_EditLimit = Limit; }
	virtual UInt32 GetEditLimit(){ return m_EditLimit; }

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }
	virtual UIStaticText *GetUIStaticText(){ return this; }

	virtual int GetInt()
	{
		StringA str(m_Text.c_str());
		return atoi(str.c_str());
	}

	virtual void CursorMoveLeft(Int32	PitchHeight, UInt Count);
	virtual void CursorMoveRight(Int32	PitchHeight, Int32 RealFontHeight, UInt Count);
	virtual void CursorMoveLeft(UInt Count);
	virtual void CursorMoveRight(UInt Count);
	virtual void BackSpaceKey(UInt32	PitchHeight, Int32 RealFontHeight);
	virtual UInt32	GetInsertPos(){ return m_InsertPos; }
	

	virtual UInt32 Update(UInt32 dwDelta);

	virtual UInt32 AddCharW(WPARAM wParam, UInt32	PitchHeight, UInt32 RealFontHeight);
	virtual bool CheckDrawOver(){ return false; };

	virtual void SelAll();

	virtual void SetRealRect(RECT &RRect);
	virtual void SetRealOffsetRect(RECT &RRect);

	virtual void SetReadPastStr(bool Flag){ m_ReadPastStr = Flag; }
	virtual bool GetReadPastStr(){ return m_ReadPastStr; }

	virtual void SetNoSpace(bool Flag){ m_NoSpace = Flag; }
	virtual bool SetNoSpace(){ return m_NoSpace; }

	virtual StlVector<String>	*GetPastMsg(){ return &m_PastMsg; }
    virtual void	SetPastMsgIndex(Int Index){ m_PastMsgIndex = Index; }
	virtual Int 	GetPastMsgIndex(){ return m_PastMsgIndex; }

	virtual void	UpPastMsg();
	virtual void	DownPastMsg();
	virtual Int		ShowHisInput();
	virtual void	AddPastStr();

	virtual const ByteArray &GetHashedPassword(){ return m_HashedPassword; }

protected:
	UInt32			m_InsertPos;
	UInt32			m_StartPoint;
	UInt32			m_EditLimit, m_EditCount;
	UInt32			m_OnScreenWidth;
	Int32			m_CursorX, m_CursorY;
	UInt			m_EditCursorRefreshTime;
	StringW			m_WaitText;
	bool			m_ShowEditCursor;
	bool			m_NoSpace;
	bool			m_ShiftDown;

	ByteArray		m_HashedPassword;

	bool			m_ReadPastStr;
	StlVector<String>	m_PastMsg;
	Int					m_PastMsgIndex;
	//std::vector<UInt32>		m_CharWidth;
	//std::vector<UInt32>		m_CharMark;
};


/////////////////////////////////////////////////////////////////////////////


class UIEditBoxEx : public UIEditBox
{
public:
	UIEditBoxEx();

	virtual Int32 OnKeyDown(WPARAM wParam);
	virtual Int32 OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32 OnMouseMove(Int32 pX, Int32 pY);
	virtual Int32 OnChar(WPARAM wParam);

	virtual void SetLineLimit(UInt32 LineLimit){ m_LineLimit = LineLimit; }
	virtual UInt32 GetLineLimit(){ return m_LineLimit;}

	virtual void CursorMoveLeft(Int32	PitchHeight, UInt Count);
	virtual void CursorMoveRight(Int32	PitchHeight, Int32 RealFontHeight,  UInt Count);
	virtual void CursorMoveUp(Int32	PitchHeight, Int32 RealFontHeight);
	virtual void CursorMoveDown(Int32 PitchHeight, Int32 RealFontHeight);
	virtual UInt32 Home();
	virtual UInt32 End();
	virtual UInt32 AddCharW(WPARAM wParam, UInt32	PitchHeight, UInt32 RealFontHeight);

	virtual void BackSpaceKey(UInt32	PitchHeight, Int32 RealFontHeight);

	virtual bool CheckDrawOver();
	virtual void FindCursorPos(Int32 pX, Int32 pY);

	virtual void SelAll();

	virtual void DrawFont();

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }
	virtual UIStaticText *GetUIStaticText(){ return this; }

protected:

	UInt32			m_CurLine;
	UInt32			m_LineLimit;
	std::vector<UInt32>		m_LineWidth;
	std::vector<UInt32>		m_LineStart;
	UInt32			m_SelStartLine, m_SelEndLine, m_SelEndX;
};

/////////////////////////////////////////////////////////////////////////////

class UIChatItem
{
public:
	UIChatItem()
	{
		m_X = m_Y = m_Width = m_Height = 0;
		m_UnderLine = false;
	}
	virtual ~UIChatItem(){}

	virtual void	SetPos(Int32 pX, Int32 pY){ m_X = pX; m_Y = pY; }
	virtual void	SetPosX(Int32 pX){ m_X = pX; }
	virtual void	SetPosY(Int32 pY){ m_Y = pY; }


	virtual void Redraw(CFont *pFont, CImage *pImage){}

	virtual UInt32 GetWidth(){ return m_Width; } 
	virtual UInt32 GetHeight(){ return m_Height; }

	virtual bool Detect(Int32 pX, Int32 pY){ return false; }

	virtual void UpdateDetectRect(Int32 pStartX, Int32 pStartY){}

	virtual UInt32 Update(UInt32 dwDelta, CFont *pFont, CImage *pImage){ return 0;}

	virtual const type_info &GetTypeID(){  return typeid(this);}

	virtual PCSTRINGW GetString(){return NULL;}
	virtual PCSTRINGW GetLinkString(){return NULL;}
	virtual PCSTRINGW GetLinkSubString(UInt32 Start){return NULL;}
	virtual bool CheckLinkHead(PCSTRINGA Str){return false;}
	virtual void SetLinkType(UInt32	LinkType){}
	virtual UInt32 GetLinkType(){ return 0; }

	inline Int32 GetPosX(){ return m_X;}
	inline Int32 GetPosY(){ return m_Y;}

	void	SetUnderLine(bool UnderLine){ m_UnderLine = UnderLine; }
	bool	GetUnderLine(){ return m_UnderLine; }
	

protected:
	Int32		m_X, m_Y;
	UInt32		m_Width, m_Height;
	bool		m_UnderLine;
};


class UIChatString : public UIChatItem
{
public:
	UIChatString() : UIChatItem()
	{
		m_FontColor = 0xFFFFFFFF;
		m_FontStyle = 0;
		m_ShadowColor = 0xFF000000;
		m_SelStartX = m_SelWidth = m_SelHeight = m_SelStart = m_SelEnd = 0;
		m_SelColor = 0xFFFF0000;
	}
	virtual ~UIChatString(){}

	virtual void SetString(PCSTRINGW pStr){ m_String = pStr;}
	virtual void SetStringA(PCSTRINGA pStr)
	{
		StringW TempStr;

		UTF82StringW(TempStr, pStr);
		m_String = TempStr.c_str();
		
	}

	virtual void SetFontColor(D3DCOLOR pColor){ m_FontColor = pColor; }
	virtual void SetShadowColor(D3DCOLOR pShadowColor){ m_ShadowColor = pShadowColor; }
	virtual void SetFontStyle(UInt32 pFontStyle){ m_FontStyle = pFontStyle; }

	virtual void Redraw(CFont *pFont, CImage *pImage);

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIChatItem *GetUIChatItemPtr(){	return this; }

	virtual void SetHeight(UInt32 Height){ m_Height = Height; }
	virtual void SetWidth(UInt32 Width){ m_Width = Width; }

	virtual void SetSelColor(UInt32 SelColor){ m_SelColor = SelColor; }
	virtual UInt32 GetSelColor(){ return m_SelColor; }
	virtual void SetSelStart(UInt32 SelStart){ m_SelStart = SelStart; }
	virtual UInt32 GetSelStart(){ return m_SelStart; }
	virtual void SetSelEnd(UInt32 SelEnd){ m_SelEnd = SelEnd; }
	virtual UInt32 GetSelEnd(){ return m_SelEnd; }
	virtual void SetSelWidth(UInt32 SelWidth){ m_SelWidth = SelWidth; }
	virtual UInt32 GetSelWidth(){ return m_SelWidth; }
	virtual void SetSelHeight(UInt32 SelHeight){ m_SelHeight = SelHeight; }
	virtual UInt32 GetSelHeight(){ return m_SelHeight; }

	
	virtual PCSTRINGW GetString(){return m_String.c_str();}
	virtual StringW &GetStringRef(){ return m_String ;}
	virtual StringW *GetStringPtr(){ return &m_String; }
	virtual bool	GetHighLight(){ return m_SelWidth > 0;}
	
protected:
	StringW			m_String;
	D3DCOLOR		m_FontColor;
	D3DCOLOR		m_ShadowColor;
	UInt32			m_FontStyle;

	UInt32			m_SelStart, m_SelEnd, m_SelColor;
	UInt32			m_SelStartX, m_SelWidth, m_SelHeight;
};



class UIChatLink : public UIChatString
{
public:

	enum ChatLinkType
	{
		None = 0,
		PlayerName,
		Http,
		Postion,
		Item,
		Partner,
		Pet,
		PopMenu,
	};


	UIChatLink() : UIChatString()
	{
		m_LinkType = 0;
	}
	
	virtual ~UIChatLink()
	{
		
	}

	virtual void SetLinkType(UInt32	LinkType){ m_LinkType = LinkType; }
	virtual UInt32 GetLinkType(){	return m_LinkType; }
	
	virtual void SetLinkString(PCSTRINGW pStr)
	{
		m_LinkString = pStr;
	}

	virtual void SetLinkStringA(PCSTRINGA pStr);
	//virtual void Redraw(CFont *pFont, CImage *pImage);
	

	virtual void UpdateDetectRect(Int32 pStartX, Int32 pStartY)
	{
		m_LinkRect.right = (m_LinkRect.left = pStartX + m_X) + m_Width;
		m_LinkRect.bottom = (m_LinkRect.top = pStartY + m_Y) + m_Height;
	}

	virtual bool Detect(Int32 pX, Int32 pY)
	{
		return (pX >= m_LinkRect.left && pX < m_LinkRect.right &&
			pY >= m_LinkRect.top && pY < m_LinkRect.bottom);
	}

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIChatItem *GetUIChatItemPtr(){ return this; }
	virtual UIChatString *GetUIChatStringPtr(){ return this; }

	virtual PCSTRINGW GetLinkString(){ return m_LinkString.c_str();}
	virtual PCSTRINGW GetLinkSubString(UInt32 Start){ return m_LinkString.c_str() + Start;}
	virtual bool CheckLinkHead(PCSTRINGA Str);
	
protected:
	StringW				m_LinkString;
	UInt32				m_LinkType;
	RECT				m_LinkRect;
};


class UIChatAniIcon : public UIChatLink
{
	public:
	UIChatAniIcon() : UIChatLink(){}
	virtual ~UIChatAniIcon(){}

	virtual void Create(){	m_AnimationIndex = 0; }

	virtual void SetAnimationData(LPCTSTR Filename);
	virtual void SetAnimationData(CAnimatedEntity * pAniEnt);

	virtual void DrawEntityToImage(CImage *pImage, RECT * Rect);

	virtual UInt32 Update(UInt32 dwDelta, CFont *pFont, CImage *pImage);
		
	virtual const type_info &GetTypeID(){ return typeid(this); }

	virtual UIChatItem *GetUIChatItemPtr(){ return this; }
	virtual UIChatString *GetUIChatStringPtr(){ return this; }
	virtual UIChatLink *GetUIChatLinkPtr(){ return this;}
	virtual void Redraw(CFont *pFont, CImage *pImage);

	virtual void SetHeight(int Height){ m_Height = Height; }
	virtual void SetWidth(int Width){ m_Width = Width; }

	inline CAnimatedEntity * GetAniEnt(){return &m_AniEnt;}

	void SetAnimationIndex(Int AnimationIndex);
	void SetRotation(Int Rotation);

protected:

	RECT		m_BaseLinerRect;

	CAnimatedEntity m_AniEnt;

	Int m_Rotation;
	Int m_AnimationIndex;
};


class UIChatIcon : public UIChatLink
{
public:
	UIChatIcon() : UIChatLink()
	{

	}
	virtual ~UIChatIcon()
	{

	}

	/*
	virtual UInt32 GetHeight()
	{
		return m_SrcImage->GetHeight();
	}
	*/

	virtual void SetImage(CImage *pImage)
	{
		m_SrcImage = pImage;
	}

	virtual void SetImageFileA(PCSTRINGA ImageName, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
	{
		StringW Filename;
		UTF82StringW(Filename, ImageName);
		CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
		m_SrcImage = Image;
		SetRect(pLeft, pTop, pRight, pBottom);
	}

	virtual void SetRect(Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
	{
		::SetRect(&m_SrcRect, pLeft, pTop, pRight, pBottom);
		m_Width = pRight - pLeft;
		m_Height = pBottom - pTop;
	}

	virtual void Redraw(CFont *pFont, CImage *pImage)
	{
		Copy8888To8888(m_X, m_Y, m_SrcImage, pImage, &m_SrcRect);
	}

	virtual const type_info &GetTypeID()
	{
		return typeid(this);
	}

	virtual UIChatItem *GetUIChatItemPtr()
	{
		return this;
	}

	virtual UIChatString *GetUIChatStringPtr()
	{
		return this;
	}

	virtual UIChatLink *GetUIChatLinkPtr()
	{
		return this;
	}

	

protected:
	CImage	*m_SrcImage;
	RECT	m_SrcRect;
};


const UInt32 UIChatRowPitch = 4;

class UIChatRow
{
public:
	UIChatRow();
	~UIChatRow();

	void AddItem(UIChatItem *pItem);
	UIChatItem *GetChatItem(UInt32 pIndex){ return m_RowItemList[pIndex]; }
	UIChatItem *GetLastChatItem();

	void AddStringW(PCSTRINGW pString, D3DCOLOR Color, D3DCOLOR ShadowColor, UInt32 FontStyle, UInt32 m_Height);
	void AddStringA(PCSTRINGA pString, D3DCOLOR Color, D3DCOLOR ShadowColor, UInt32 FontStyle, UInt32 m_Height)
	{
		AddStringW((PCSTRINGW)pString, Color, ShadowColor, FontStyle, m_Height);
	}

	void Redraw(CFont *pFont, CImage *pImage);

	void UpdateDetectRect(Int32 pStartX, Int32 pStartY);
	void SetupDetectRect(Int32 pStartX, Int32 pStartY, Int32 pHeight, Int32 pWidth);

	virtual UInt32 Update(UInt32 dwDelta, CFont *pFont, CImage *pImage);
	
	void SetY(Int32 pY){ m_Y = pY; }

	UInt32 GetHeight(){ return m_Height; }
	void SetHeight(UInt Height){ m_Height = Height; }
	UInt32 GetWidth(){ return m_Width; }
	void SetWidth(UInt Width){ m_Width = Width; }
	void AddWidth(Int Value){ m_Width = (Int)m_Width + Value; }

	bool Detect(Int32 pX, Int32 pY)
	{
		return (pX >= m_DetectRect.left && pX < m_DetectRect.right &&
			pY >= m_DetectRect.top && pY < m_DetectRect.bottom);
	}



	UIChatItem	*FindClick(Int32 pX, Int32 pY);

	inline std::vector<UIChatItem *> * GetRowItemList(){return &m_RowItemList;}

	virtual void ResetChatItemsPos();

	bool IsTextExist(PCSTRINGW key);

	void SetEndLine(bool Flag){ m_EndLine = Flag; };
	bool GetEndLine(){ return m_EndLine; };

private:
	std::vector<UIChatItem *>	m_RowItemList;
	UInt32		m_Width, m_Height;
	UInt32		m_Y;
	RECT		m_DetectRect;
	bool		m_EndLine;
};


class UIChatPanel : public UIStaticText
{
public:
	UIChatPanel();
	~UIChatPanel()
	{
		Dispose();
	}



	virtual void InitButton();
	virtual void Dispose();

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);


	virtual bool	Detect(Int32 pX, Int32 pY);

	virtual Int32		OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ);
	virtual Int32		OnMouseMove(Int32 pX, Int32 pY);
	virtual Int32		OnLButtonUp(Int32 pX, Int32 pY);
	virtual Int32		OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32		OnRButtonUp(Int32 pX, Int32 pY);
	virtual Int32		OnRButtonDown(Int32 pX, Int32 pY);
	virtual Int32		OnKeyDown(WPARAM wParam);

	virtual void		SetFocus(bool pState);
	
	virtual	UIChatRow *CreateRow()
	{
		UIChatRow *ChatRowPtr = SafeCreateObject(UIChatRow);
		m_RowList.push_back(ChatRowPtr);

		return ChatRowPtr;
	}
	virtual UIChatRow *GetRow(UInt32 pIndex){ return (m_RowList.size() && pIndex < m_RowList.size() ? m_RowList[pIndex] : NULL); }
	virtual UIChatRow *GetLastRow(){ return (m_RowList.size() ? m_RowList[m_RowList.size() - 1] : NULL);}
	virtual UIChatRow *GetSafeLastRow(){ return (m_RowList.size() ? m_RowList[m_RowList.size() - 1] : CreateRow()); }
	
	
	virtual void SetImage(UInt32 pWidth, UInt32 pHeight)
	{
		//SafeDeleteObject(m_BufImage);
		CImageManager::GetInstance().DeleteImage(m_BufImage);
		m_Width = pWidth;
		m_Height = pHeight;
		//m_BufImage = SafeCreateObject3(CImage, pWidth, pHeight, 8888);
		m_BufImage = CImageManager::GetInstance().CreateImage(m_Width, m_Height, 8888);
	}

	virtual UIChatItem	*GetLinkItem(){	return m_ClickItem; }
	virtual void ClearLinkItem(){  m_ClickItem = NULL; }

	virtual void UpdateDetectRect();

	virtual void Start2BottomRow();
	virtual void End2TopRow();
	virtual void UpRow();
	virtual void DownRow();
	virtual void HomeRow();
	virtual void EndRow();
	virtual void SetRow(UInt32 a, UInt32 b);

	virtual bool IsEndRow(){ return (m_EndRow == (UInt32)(m_RowList.size() - 1)); }
	virtual bool IsStartRow(){ return (m_StartRow == 0); }

	virtual void Redraw();
	virtual void SetOffsetPos(Int32 pX, Int32 pY);


	virtual void ClearAllRow();
	
	virtual UIButton *CreatePrevBtn()
	{
		SafeDeleteObject(m_PrevBtn);
		m_PrevBtn = SafeCreateObject(UIButton);

		return m_PrevBtn;
	}


	virtual UIButton *CreateNextBtn()
	{
		SafeDeleteObject(m_NextBtn);
		m_NextBtn = SafeCreateObject(UIButton);

		return m_NextBtn;
	}

	
	virtual UIButton *CreateHomeBtn()
	{
		SafeDeleteObject(m_HomeBtn);
		m_HomeBtn = SafeCreateObject(UIButton);

		return m_HomeBtn;
	}


	virtual UIButton *CreateEndBtn()
	{
		SafeDeleteObject(m_EndBtn);
		m_EndBtn = SafeCreateObject(UIButton);

		return m_EndBtn;
	}


	virtual UIButton *GetPrevBtn(){	return m_PrevBtn; }
	virtual UIButton *GetNextBtn(){	return m_NextBtn; }
	virtual UIButton *GetHomeBtn(){	return m_HomeBtn; }
	virtual UIButton *GetEndBtn(){  return m_EndBtn; }

	virtual const type_info &GetTypeID(){ return typeid(this); }

	virtual UIItem *GetUIItemPtr(){	return this; }

	virtual UInt32 GetFontHeight(){ return m_Font->GetHeight(); }
	virtual void SetEnableScroll(Boolean EnableScroll){ m_EnableScroll = EnableScroll; }
	virtual Boolean IsEnableScroll(){ return m_EnableScroll; }

	virtual void SetContinueRun(bool Flag){	m_ContinueRun = Flag; }
	virtual bool ContinueRun(){	return m_ContinueRun && !m_ClickItem; }
	
	virtual UIChatRow *GetHomeRow(){ return m_RowList[0]; }


	virtual void AddStringA(PCSTRINGA pString);
	virtual void AddStringW(PCSTRINGW pString);
	virtual void AddStringWNoPrefix(PCSTRINGW pString);
	virtual void RealAddStringW(PCSTRINGW pString, bool Flag);

	virtual UInt32 Update(UInt32 dwDelta);

	virtual void DeleteUpdateSet(StlSet<UIItem *> &AutoUpdateSet, StlSet<UIItem *> &MouseInSet)
	{
		//james modify some codes here, Add DeleteUpdateSetR
		DeleteUpdateSetR(AutoUpdateSet, MouseInSet);
	}

	virtual void SetMaxLine(Int32 MaxLine){ m_MaxLine = MaxLine; }
	virtual Int32 GetMaxLine(){ return m_MaxLine; }

	virtual UInt32 RowListCount()
	{
		return (UInt32)(m_RowList.size());
	}

	virtual UInt32 GetTotalHeight();

	virtual UInt32 GetAllRowHeight();

	virtual inline Int32 GetStartRowIndex(){ return m_StartRow;}
	virtual inline Int32 GetEndRowIndex(){ return m_EndRow;}

	virtual inline UInt32 GetPitch(){return m_BufImage->GetPitch();}

	virtual void SetText(PCSTRINGW pText);

	virtual inline void LockChatPanel(){ m_AutoEndRow = false;}
	virtual inline void UnLockChatPanel(){ m_AutoEndRow = true;}
	virtual inline bool GetAutoLock(){return m_AutoEndRow;}

	virtual UInt32	FindWord(PCSTRINGW key, UInt16 startLine);
	virtual UInt32	FindUnderLineWord(UInt16 startLine);
	virtual UInt32	FindHighLightWord(UInt16 startLine);

	virtual void ClearOverLine();
	
protected:
	Int32		m_StartRow, m_EndRow, m_ZOffset, m_MaxLine;
	Boolean		m_EnableScroll;
	bool		m_ContinueRun;
	bool		m_AutoEndRow;

	std::deque<UIChatRow *>	m_RowList;
	std::deque<UIChatRow *>	m_TempRowList;
	UIChatItem	*m_ClickItem;

	UIButton	*m_PrevBtn;
	UIButton	*m_NextBtn;
	UIButton	*m_HomeBtn;
	UIButton	*m_EndBtn;
};


///////////////////////////////////////////////////////////////////////////////

struct UIChatInsertNode
{
	Int Line, X, InsertPos, Index, Count, CharWidth, DelWidth;

	UIChatInsertNode(Int pLine, Int pX, Int pInsertPos, Int pIndex, Int pCount, Int pCharWidth, Int pDelWidth)
	{
		Set(pLine, pX, pInsertPos, pIndex, pCount, pCharWidth, pDelWidth);
	}

	void Set(Int pLine, Int pX, Int pInsertPos, Int pIndex, Int pCount, Int pCharWidth, Int pDelWidth)
	{
		Line = pLine; 
		X = pX;
		InsertPos = pInsertPos; 
		Index = pIndex; 
		Count = pCount; 
		CharWidth = pCharWidth;
		DelWidth = pDelWidth;
	}

	void Get(Int &pLine, Int &pX, Int &pInsertPos, Int &pIndex, Int &pCount, Int &pCharWidth, Int &pDelWidth)
	{
		pLine = Line; 
		pX = X;
		pInsertPos = InsertPos; 
		pIndex = Index; 
		pCount = Count; 
		pCharWidth = CharWidth;
		pDelWidth = DelWidth;
	}
};


class UIChatEditPanel : public UIChatPanel
{
public:
	

	UIChatEditPanel();

	virtual ~UIChatEditPanel() { Dispose(); }

	virtual void Dispose(){ m_InsertList.clear(); }

	virtual UInt32 Update(UInt32 dwDelta);

	virtual Int32 OnLButtonDown(Int32 pX, Int32 pY);
	virtual Int32 OnMouseMove(Int32 pX, Int32 pY);
	virtual Int32 OnKeyDown(WPARAM wParam);
	virtual Int32 OnChar(WPARAM wParam);

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);
	virtual void RenderSel(CImage *pImage);
	virtual void RenderSel(CImage *pImage, RECT &RRect);

	virtual void UpdateDetectRect();

	virtual UInt32 InsertLine(Int Line);
	virtual UInt32 NewLine();
	virtual Int DrawOnNewLine(CharW C);
	virtual Int DrawOnNewLine(String &S);
	virtual UInt32 InsertReturn();

	virtual UInt32 AddCharW(CharW C);
	virtual UInt DelChar();
	virtual UInt InsertString(String &S, Int InsertPos);
	virtual UInt InsertString(String &S){ return InsertString(S, m_InsertPos); }
	virtual void ResetAll();

	virtual void ClearInsertList();

	virtual void MoveLine(Int DX, Int DY, Int SX, Int SY, Int Width);
	virtual void MoveArea(Int DX, Int DY, Int SX, Int SY, Int Width, Int Height);
	virtual Int DrawChar(CharW C, Int X, Int Y)
	{
		return m_Font->DrawFontChar(C, m_BufImage->GetPtr(), X, Y, 
			m_BufImage->GetWidth(), m_BufImage->GetHeight(), m_BufImage->GetPitch(), m_FontStyle, 
			m_FontColor, m_ShadowColor);
	}
	virtual Int DrawString(String &S, Int X, Int Y);
	virtual void FillImage(Int Left, Int Right, Int Y);
	virtual void FillImage(){ m_BufImage->FillUInt32(); }
	virtual void Redraw();

	virtual void UnPickUp();
	virtual void SetFocus(bool pState);

	virtual void SetStartSel();
	virtual void PowerSetStartSel();
	virtual void SetEndSel();
	virtual void PowerSetEndSel();

	virtual void SetImage(UInt32 pWidth, UInt32 pHeight);

	virtual void LoadText();
	virtual void LoadText(PCSTRINGW		pStr)
	{
		m_Text = pStr;
		LoadText();
	}
	virtual void SaveText();
	virtual void SaveNoSelText();
	virtual void SaveOnlySelText();
	virtual void SelText2ClipBoard();
	virtual void SetSelStartCursorPos();
	virtual void DelSelText();

	virtual bool TestRedraw();
	virtual bool InsertClipBoard();

	virtual void CursorMoveLeft();
	virtual void CursorMoveRight();
	virtual void CursorMoveUp();
	virtual void CursorMoveDown();

	virtual void SetCurLine(Int Index);
	virtual void SetNoCurLine();

	virtual void SetLimit(Int Limit){ m_Limit = Limit; }
	virtual Int	 GetLimit(){ return m_Limit; }

	virtual void SetUseEnter(bool flag){ m_UseEnter = flag; }
	virtual bool GetUseEnter(){ return m_UseEnter; }

	virtual Int FindCursorPos(Int32 pX, Int32 pY);

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }


protected:
	Int		m_CursorX, m_CursorY;
	Int		m_InsertPos;
	Int		m_CurLine;
	Int		m_DrawWidth;
	Int		m_Limit, m_StringSize;

	bool	m_ShowEditCursor;
	bool	m_ShiftDown;

	bool	m_SelNow;
	Int		m_SelStartLine, m_SelStartPos;
	Int		m_SelEndLine, m_SelEndPos;
	Int		m_SelLeft,  m_SelRight;

	Int		m_TInsertPos;
	Int		m_TCurLine, m_TCursorX, m_TCursorY;

	bool	m_UseEnter;


	UIChatItem	*m_EditItem;
	UIChatString	*m_EditString;
	UIChatRow	*m_EditRow;
	UInt			m_EditCursorRefreshTime;

	std::vector<UIChatInsertNode *> m_InsertList;
};


///////////////////////////////////////////////////////////////////////////////


class UITips : public UIItem
{
public:
	UITips() : UIItem()
	{
		m_ShowFlag = false;
		m_OldX = m_OldY = 0;
		m_EnableTimeOutEvent = true;
	};

	virtual ~UITips()
	{
		Dispose();
	}

	virtual void Dispose()
	{
			
	}

	virtual bool	Detect(Int32 pX, Int32 pY);

	virtual void SetWH(UInt32 W, UInt32 H){ m_Width = W; m_Height = H; }

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }


protected:
	String			m_TipsStr;
	bool			m_ShowFlag;
	UInt32			m_OldX, m_OldY;
};


///////////////////////////////////////////////////////////////////////////////


class UIMiniMap : public UIItem
{
public:
	UIMiniMap() : UIItem()
	{
		m_MapImage = m_Mask = m_PlayerImage = NULL;
		m_CenterX = m_CenterY = m_PlayerX = m_PlayerY = 0;
		m_MapX = m_MapY = 0;
		m_WRatio = m_HRatio = 0;
		m_RedrawFlag = false;
	}

	~UIMiniMap()
	{
		Dispose();
	}

	virtual void Dispose()
	{
		//SafeDeleteObject(m_MapImage);
		CImageManager::GetInstance().DeleteImage(m_MapImage);
		m_MapImage = NULL;
		CImageManager::GetInstance().DeleteImage(m_PlayerImage);
		m_PlayerImage = NULL;
		CImageManager::GetInstance().DeleteImage(m_Mask);
		m_Mask = NULL;
	}

	virtual void Validate(CImage *pImage)
	{
	}

	virtual void Invalidate()
	{
	}

	virtual UInt32 Update(UInt32 dwDelta)
	{
		return 0;
	}

	virtual void Render(CImage *pImage);
	virtual void Render(CImage *pImage, RECT &RRect);
	

	virtual void SetImage(PCSTRING	Filename);
	virtual void SetImageA(PCSTRINGA	Filename);
	virtual void SetImage(CImage *pImage);

	virtual CImage* GetImage(){ return m_MapImage; }

	virtual void SetMask(CImage *pImage)
	{
		m_Mask = pImage;
		::SetRect(&m_MapRect, 0, 0, m_Mask->GetWidth(), m_Mask->GetHeight());
		m_Width = m_Mask->GetWidth();
		m_Height = m_Mask->GetHeight();
	}

	virtual void SetDefaultMask();


	virtual void SetupRatio(UInt32 MapWidth, UInt32 MapHeight)
	{
		m_WRatio = (double)MapWidth / m_MapImage->GetWidth();
		m_HRatio = (double)MapHeight / m_MapImage->GetHeight();
	};


	virtual bool GetRedrawFlag()
	{
		return m_RedrawFlag;
	}

	virtual void SetRedrawFlag(bool Flag)
	{
		m_RedrawFlag = Flag;
	}

	virtual UInt ResetPlayerPos(UInt X, UInt Y);
	

	virtual const type_info &GetTypeID(){ return typeid(this); }
	virtual UIItem *GetUIItemPtr(){ return this; }

	virtual Int32		OnMouseMove(Int32 pX, Int32 pY){ return -1; }
	
	virtual void SetPlayerImage(CImage *pImage);
	virtual void SetPlayerImageA(PCSTRINGA	Filename);

	virtual void SetPlayerImageRect(Int Left, Int Top, Int Right, Int Bottom)
	{
		::SetRect(&m_PlayerRect, Left, Top, Right, Bottom);
	}

	virtual void DrawIcon(CImage* pSrcImage, RECT iconRect, Int PixelX, Int PixelY);

protected:

	CImage			*m_MapImage, *m_Mask;
	Int32			m_CenterX, m_CenterY, m_PlayerX, m_PlayerY;
	Int32			m_MapX, m_MapY;
	double			m_WRatio, m_HRatio;
	RECT			m_MapRect;
	CImage			*m_PlayerImage;
	RECT			m_PlayerRect;
	bool			m_RedrawFlag;
};


///////////////////////////////////////////////////////////////////////////////

struct UIShortCutNode
{
	Int32			m_RefType;		//Item or skill
	Int32			m_RefNum;		//BoxNum of skill Code
	Int32			m_Count;

	UIShortCutNode()
	{
		m_RefType = 0;
		m_RefNum = 0;
		m_Count = 0;
	}
};


////////////////////////////////////////////////////////////////////////////////

class ChatPool : public Reuben::Utility::Singleton<ChatPool>
{
public:
	SINGLETON(ChatPool);

	ChatPool()
	{
		m_CopyIndex = m_Index = 0;
	};

	~ChatPool()
	{
		m_ChatMsg.clear();
	}

	void Clear()
	{
		m_CopyIndex = m_Index = 0;
		m_ChatMsg.clear();
	}

	void Add(PCSTRING Msg)
	{
		m_ChatMsg.push_back(Msg);
		while(m_ChatMsg.size() > 100)
		{
			m_ChatMsg.pop_front();
			if(m_CopyIndex > 0)
				--m_CopyIndex;
		}
	}

	PCSTRING GetStartMsg()
	{
		if(m_ChatMsg.empty())
			return NULL;

		m_Index = 0;
		return m_ChatMsg[m_Index].c_str();
	}

	PCSTRING GetLastMsg()
	{
		if(m_ChatMsg.empty())
			return NULL;
		
		m_Index = (UInt)m_ChatMsg.size() - 1;
		return m_ChatMsg[m_Index].c_str();
	}

	PCSTRING GetNextMsg()
	{
		if(m_ChatMsg.empty())
			return NULL;
		
		if(m_Index < m_ChatMsg.size() - 1)
		{
			return m_ChatMsg[++m_Index].c_str();
		}

		return NULL;
	}

	PCSTRING GetMsgFormIndex(UInt Index)
	{
		if(!m_ChatMsg.empty() && Index >= 0 && Index < m_ChatMsg.size() )
		{
			m_Index = Index;
			return m_ChatMsg[m_Index].c_str();
		}
		else
		{
			return NULL;
		}
	}

	StlDeque<String>	*GetChatMsg(){ return &m_ChatMsg; }

	UInt32 GetIndex(){ return m_Index; }

	void ResetCopyIndex(){ m_CopyIndex = (UInt32)(m_ChatMsg.size());}
	UInt32 GetCopyIndex(){ return m_CopyIndex; }

private:
	StlDeque<String>	m_ChatMsg;
	UInt32				m_Index;
	UInt32				m_CopyIndex;
};


inline ChatPool *GetChatPool()
{
	return &ChatPool::GetInstance();
}


///////////////////////////////////////////////////////////////////////////////



class UIManager
{
public:

	UIManager();
	~UIManager()
	{
		Release();
		//IMEUninit();
		//IME_Layout::GetInstance().Release();
	}
	
	void Release();
	

	void AddUIPanel(UIPanel *pUIPanel);
	void AddLockUIPanel(UIPanel *pUIPanel)
	{
		if(pUIPanel)
			m_LockUIPanelList.push_back(pUIPanel);
	}
	void ClearLockUIPanel(){ m_LockUIPanelList.clear();	}
	void LockPanelActive(){
		if(m_LockUIPanelList.size())
			m_UIPanelListPtr = &m_LockUIPanelList;
	}	
	void BasePanelActive(){ m_UIPanelListPtr = &m_UIPanelList; }	
	//void InitIME();

	UInt32 IconTimer();
	UInt32 CheckTexture(int pX, int pY);
	void RenderAll();
	
	void BlitAll(LPD3DXSPRITE pSprite, Int X = 0, Int Y = 0, RECT * pRect = NULL);
	StlVector<UIPanel *>	*GetUIPanelListPtr(){ return &m_UIPanelList; }

	void SetupLua(lua_State *LuaState, bool LuaShareFlag = false){ m_LuaShareFlag = LuaShareFlag;	m_LuaState = LuaState;	}
	void LoadLuaUI(PCSTRINGA Filename);
	bool GetLuaShareFlag(){	return m_LuaShareFlag; }
	Int32 GetCursorX(){	return m_CursorX;}
	inline void SetCursorX(Int32 value){ m_CursorX = value;	}
	Int32 GetCursorY() { return m_CursorY; }
	inline void SetCursorY(Int32 value){ m_CursorY = value; }
	Int32 GetCursorPreX(){ return m_CursorPreX; }
	Int32 GetCursorPreY(){ return m_CursorPreY;	}
	Int32 GetCursorOffsetX(){ return m_CursorOffsetX; }
	Int32 GetCursorOffsetY(){ return m_CursorOffsetY; }
	void SetCursorOffsetX(Int32 CursorOffsetX){ m_CursorOffsetX = CursorOffsetX; }
	void SetCursorOffsetY(Int32 CursorOffsetY){ m_CursorOffsetY = CursorOffsetY; }
	

	void SetOutputTexture(LPDIRECT3DTEXTURE9 pTexture){	m_OutputTexture = pTexture;	ReFillBackImage(); }

	LRESULT MsgProc(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );
	LRESULT OnMouseMove(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );
	LRESULT OnLButtonDown(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );
	LRESULT OnLButtonUp(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );
	LRESULT OnRButtonDown(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );
	LRESULT OnRButtonUp(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );
	LRESULT OnLButtonDBLCLK(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );
	LRESULT OnRButtonDBLCLK(  HWND  hWnd, UInt32  uMsg, WPARAM  wParam, LPARAM  lParam );

	LRESULT UpdateMouseMove();
	void MouseLeave();
	
	bool  KeyOnEsc();

	void SetMouseMoveFlag(bool Flag){ m_MouseMoveFlag = Flag; }
	bool GetMouseMoveFlag(){ return m_MouseMoveFlag; }
	void SetRender(CRender *pRender){ m_Render = pRender; }
	CRender *GetRender(){ return m_Render; }
	void AddUIID(PCSTRINGA pName, UIItem *pUIItem);
	void RemoveUIID(PCSTRINGA pName);
	UIItem *GetUIItem(PCSTRINGA pName);
	void AddUpdateUIItem(UIItem *pUIItem){ m_AutoUpdateSet.insert(pUIItem); }
	void RemoveUpdateUIItem(UIItem *pUIItem);
	UIItem *GetWaitItem()
	{
		return m_WaitItem;
	}

	void SetFocusTrue();
	void ChangeFocus(PCSTRINGA pName);
	void ChangeFocus(UIItem *ItemPtr);

	void OutRectMouseCursor()
	{
		m_CursorPreX = m_CursorPreY = -1;
	}

	
	void SetBackImageA(PCSTRINGA ImageName, UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom);
	void SetBackImage(CImage *pImage, UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom);

	void ReFillBackImage();

	void RegisterImage(PCSTRING pStr){ m_ImageList.push_back(pStr); }
	void RegisterImageA(PCSTRINGA ImageName)
	{
		StringW Filename;

		UTF82StringW(Filename, ImageName);
		m_ImageList.push_back(Filename.c_str());
	}

	UIItem *GetDragItem(){ return m_DragItem; }
	void SetDragItem(UIItem *Item);
	void SetDragItemXY(UIItem *Item, Int32 OffsetX, Int32 OffsetY)
	{
		Item->SetPos(m_CursorX + OffsetX, m_CursorY + OffsetY);
		SetDragItem(Item);
	}
	void SetDragPanel2Top();
	void EndDrag(Int32 Code);
	void ClearDrag()
	{
		if(m_DragItem)
			if(m_DragItem->GetCanDrag() == 1)
				EndDrag(5);
	}
	void StopDrag(){ m_DragNow = false; m_DragItem = NULL; m_DropPanel = NULL; m_Focus = NULL; }
	void SetDragPanel();
	Int32	GetNowDragX(){ return m_CursorX - m_TempDragPanel->GetDragX(); }
	Int32	GetNowDragY(){ return m_CursorY - m_TempDragPanel->GetDragY(); }
    UIPanel	*GetDragPanel(){ return m_TempDragPanel; }
	bool	GetDragNow(){ return m_DragNow; }

	void SetIDName(PCSTRINGA pStr){ m_IDName = pStr; }
	PCSTRINGA GetIDName(){ return m_IDName.c_str(); }
	lua_State *GetLuaState(){ return m_LuaState; }

	void ClearWaitDelPanel();
	void ClearStateItem(){ m_MouseOverItem = m_Focus = m_WaitItem = NULL; }
	bool Detect(PCSTRINGA Name, Int32 CursorX, Int32 CursorY);

	void RemovePanel();

	void SetKeyInItem(PCSTRINGA pName);
	void SetKeyInItem(UIItem *Item);
	void SetEvent(Int32 Event){ m_Event.push_back(Event); }


	Int32	GetEvent()
	{
		if(m_Event.size())
			return m_Event[m_Event.size() - 1];
		else
			return 0;
	}

	Int32 CatchEvent()
	{
		if(m_Event.size())
		{
			UInt32	Event = m_Event.front();
			m_Event.pop_front();

			return Event;
		}
		else
			return 0;
	}

	UIItem *GetDropPanel(){ return m_DropPanel; }
	void ChangeParent()
	{
		if(m_DragItem && m_DropPanel)
			m_DragItem->ChangeParent(m_DropPanel);
	}

	void ResetDragItem();
	void ResetTipsPanel(UIItem *Item);
	void CloseTips();

	void SetDefaultEnterFunc(PCSTRINGA pFuncName){ m_DefaultEnterFunc = pFuncName; }

	UInt32 GetIconOffset(){	return m_AniIconTimeOffset;}
	PCSTRINGA SetDefaultEnterFunc(){ return m_DefaultEnterFunc.c_str(); }

	UInt32 GetRenderAllFlag(){ return m_RenderAllFlag;}
	bool DragNow(){	return m_DragNow; }

	void DeletePanel(PCSTRINGA ItemName);
	void DeletePanel(UIItem *Item);
	void LeavePanel(PCSTRINGA ItemName);
	void LeavePanel(UIItem *pItem);

	void Update(UInt32 uTimeDelta);

	UInt32 GetUpdateFlag(){	return m_UpdateFlag; }
	void SetLeaveTimerItem(UIItem *Item, UInt32 ShiftTime)
	{
		m_LeaveTimerItemStartTime = GetTickCount();
		m_LeaveTimerItemShiftTime = ShiftTime;
		m_LeaveTimerItem = Item;
	}
	void SetMoveInTimerItem(UIItem *Item, UInt32 ShiftTime)
	{
		m_MoveInTimerItemStartTime = GetTickCount();
		m_MoveInTimerItemShiftTime = ShiftTime;
		m_MoveInTimerItem = Item;
	}

	void ClearLeaveTimerItem(){ m_LeaveTimerItem = NULL; }
	void ClearMoveInTimerItem(){ m_MoveInTimerItem = NULL; }

	void SetMonoItem(UIItem *MonoPanel);
	void SetMonoPanel(UIPanel *MonoPanel){ SetMonoItem(MonoPanel); }
	void SetMonoPanelEx(UIPanelEx *MonoPanel){ SetMonoItem(MonoPanel); }
	UIItem *GetMonoItem(){ return m_MonoPanel; }
	void LockMonoPanel(PCSTRINGA Pass){ m_LockMonoCode = Pass; m_LockMonoFlag = true; }
	void UnlockMonoPanel(PCSTRINGA Pass)
	{ 
		if(m_LockMonoCode == Pass)
			m_LockMonoFlag = false; 
	}
	void SetLockMonoPanel(bool Flag){ m_LockMonoFlag = Flag; }
	bool IsLockMonoPanel(){ return m_LockMonoFlag; }
	UIPanel *GetMonoPanel();
	UIPanelEx *GetMonoPanelEx();
	void ClearMonoPanel();

	void AddDirtyUIItem(UIItem *pItem);

	void SetPanel2Top(UIPanel *pPanel);
	void SetPanelEx2Top(UIPanelEx *pPanel);
	void SetPanel2Bottom(UIPanel *pPanel);
	void SetPanelEx2Bottom(UIPanelEx *pPanel);
	bool CheckPanelOnTop(UIPanel *pPanel){ if(m_UIPanelList.size()) return (pPanel == m_UIPanelList.back()); return false; }
	UIPanel *GetPanelOnTop(UIPanel *pPanel){ if(m_UIPanelList.size()) return m_UIPanelList.back(); return NULL; }
	void SavePanelPos(PCSTRINGW Filename);
	void LoadPanelPos(PCSTRINGW Filename);
	void AddPanelPos(UIPanel *pPanel);
	void SetFollowPanel(UIPanel *PanelA, UIPanel *PanelB);
	void SetFollowPanel(PCSTRINGA PanelA, PCSTRINGA PanelB);

	void UIItemSetEnable(PCSTRINGA Name, bool Flag);

	void SetAllPanelDisable();
	void RestoreAllPanelEnable();

	void FreezeAll();
	void UnfreezeAll();
	bool IsFreeze(){ return m_FreezeFlag; }
	

	CImage * GetBackImage(){return m_BackImage;}

	template <typename T>
	T GetUIThing(PCSTRINGA pName)
	{
		UIItem *pItem = GetUIItem(pName);
		
		if(pItem)
		{
			if(typeid(T) == pItem->GetTypeID())
			{
				return static_cast<T>(pItem);
			}
		}
	
		return NULL;
	}

	template <typename T, typename T2>
	T GetUIThing2(PCSTRINGA pName)
	{
		UIItem *pItem = GetUIItem(pName);
		
		if(pItem)
		{
			if(typeid(T) == pItem->GetTypeID() || typeid(T2) == pItem->GetTypeID())
			{
				return static_cast<T>(pItem);
			}
		}
	
		return NULL;
	}

	Int32	SetMouseOnUIFlag(Int32 Flag){ return (m_MouseOnUIFlag = Flag); }
	Int32	GetMouseOnUIFlag(){ return (m_MouseOnUIFlag); }
	void SetLuaFuncName(PCSTRINGA	LuaFuncName){ m_LuaFuncName = LuaFuncName; }
	PCSTRINGA	GetLuaFuncName(){ return m_LuaFuncName.c_str();	}

	void RenderAllPanelsByRects();
	void RemoveAllPanelsRects();

	void SetUIItemEnable(PCSTRINGA name, bool value);


	#define RegisterFindUI(T)					\
	T *Find##T##(PCSTRINGA pName)				\
	{											\
		return GetUIThing<T *>(pName);			\
	}


	#define RegisterFindUI2(T, T2)					\
	T *Find##T##(PCSTRINGA pName)				\
	{											\
		return GetUIThing2<T *, T2 *>(pName);			\
	}


	
	RegisterFindUI(UIPanel);
	RegisterFindUI(UIPanelEx);
	RegisterFindUI(UIImage);
	RegisterFindUI(UIStaticText);
	RegisterFindUI(UIButton);
	RegisterFindUI(UICheckBox);
	RegisterFindUI(UIListBox);
	RegisterFindUI(UIPopUpMenu);
	RegisterFindUI(UIScrollBarH);
	RegisterFindUI(UIScrollBarV);
	RegisterFindUI(UIScrollListBox);
	RegisterFindUI(UIEditBox);
	RegisterFindUI(UIEditBoxEx);
	RegisterFindUI(UIChatPanel);
	RegisterFindUI(UIChatEditPanel);
	RegisterFindUI(UIMiniMap);
	RegisterFindUI(UILinerImage);
	RegisterFindUI(UILinerImageV);
	RegisterFindUI(UIAniIcon);
	RegisterFindUI(UITips);


	
	inline StlVector<RECT> GetDirtyRectLst(){return m_DirRectMan.GetDirtyRectLst();}

	inline CDirtyRectManager * GetDirtyRectMan(){return &m_DirRectMan;}
	inline void ClearDirtyRectLst(){ m_DirRectMan.ClearDirtyRect();}

	void InsertToInOutEvent(UIItem * pItem);
	void RemoveFromInOutEvent(UIItem * pItem);

	inline UIItem * GetKeyInItem(){ return m_KeyInItem;}
	inline UIItem * GetFocusItem(){ return m_Focus;}

	inline bool IsPanelExist(UIPanel * pPanel)
	{
		for (Int i = 0; i < (Int)m_UIPanelList.size(); i++)
		{
			if (pPanel == m_UIPanelList[i])
				return true;
		}
		return false;
	}
private:

	StringA				m_IDName;
	UIItem				*m_Focus;
	UIItem				*m_WaitItem;
	UIItem				*m_MouseOverItem;
	UIItem				*m_KeyInItem;
	UIItem				*m_LeaveTimerItem, *m_MoveInTimerItem;
	UIItem				*m_DragItem;
	UIItem				*m_DragItemParent;
	UIItem				*m_DropPanel;
	UIItem				*m_MonoPanel;
	UIPanel				*m_TempDragPanel;

	UIPanel				*m_LClickPanel, *m_RClickPanel;

	StlVector<UIPanel *>	m_UIPanelList, m_LockUIPanelList;
	StlVector<UIPanel *>	*m_UIPanelListPtr;
	StlVector<UIPanel *>	m_WaitDelUIPanelList;
	UIPanel				*m_OldCurPanel;
	lua_State			*m_LuaState;
	Int32				m_CursorX, m_CursorY;
	Int32				m_CursorOffsetX, m_CursorOffsetY;
	Int32				m_CursorPreX, m_CursorPreY;
	CImage				*m_BackImage;
	CImage				*m_BackFilledImage;
	//CImage				*m_DragImage;
	RECT				m_BackRect;
	LPDIRECT3DTEXTURE9	m_OutputTexture;
	LPDIRECT3DTEXTURE9	m_IMETexture;
	//LPDIRECT3DTEXTURE9	m_DragTexture;
	
	CRender				*m_Render;
	StlMap<StringA, UIItem *>		m_UIIDMap;
	StlMap<StringA, UIPanel *>		m_UIPanelMap;
	StlVector<StringW>	m_ImageList;
	StlDeque<Int32>		m_Event;
	StringA				m_DefaultEnterFunc;

	StlMap<StringA, std::pair<Int32, Int32> >		m_UIPanelPosRec;

	StlSet<UIItem *>	m_AutoUpdateSet;

	UInt32				m_RenderAllFlag, m_UpdateFlag;

	Int32				m_StartDragX, m_StartDragY;

	UInt32				m_StartTime, m_AniIconTimeOffset;
	UInt32				m_LeaveTimerItemStartTime, m_LeaveTimerItemShiftTime;
	UInt32				m_MoveInTimerItemStartTime, m_MoveInTimerItemShiftTime;
	UInt32				m_Now;
	UInt32				m_LButtonDownTime;
	Int32				m_MouseOnUIFlag;

	bool				m_LuaShareFlag;
	bool				m_DragNow, m_TipsNow;
	bool				m_LockMonoFlag;
	bool				m_MouseMoveFlag;
	bool				m_FreezeFlag;

	StringA				m_LockMonoCode;

	StringA				m_LuaFuncName;
	StringA				m_TipsPanelName, m_TipsStringName;


	LRESULT OnLButtonDown_GetSpot(UIItem *Item);
	LRESULT OnLButtonDBLCLK_GetSpot(UIItem *Item);

	CDirtyRectManager	m_DirRectMan;

	StlSet<UIItem *>	m_MouseInOutEventSet;

	
};


extern UIManager *g_UIManager;


/////////////////////////////////////////////////////////////////////////////

/*
UIItem *LuaCreateUIItem()
{
	return SafeCreateObject(UIItem);
}
*/


#define RegisterLuaCreateUI(T)				\
inline T *LuaCreate##T##()							\
{											\
	return SafeCreateObject(T);				\
}


RegisterLuaCreateUI(UIPanel);
RegisterLuaCreateUI(UIPanelEx);
RegisterLuaCreateUI(UIImage);
RegisterLuaCreateUI(UIStaticText);
RegisterLuaCreateUI(UIDynamicText);
RegisterLuaCreateUI(UIButton);
RegisterLuaCreateUI(UICheckBox);
RegisterLuaCreateUI(UIListBox);
RegisterLuaCreateUI(UIPopUpMenu);
RegisterLuaCreateUI(UIScrollBarH);
RegisterLuaCreateUI(UIScrollBarV);
RegisterLuaCreateUI(UIScrollListBox);
RegisterLuaCreateUI(UIEditBox);
RegisterLuaCreateUI(UIEditBoxEx);
RegisterLuaCreateUI(UIChatPanel);
RegisterLuaCreateUI(UIChatEditPanel);
RegisterLuaCreateUI(UIChatString);
RegisterLuaCreateUI(UIChatLink);
RegisterLuaCreateUI(UIMiniMap);
RegisterLuaCreateUI(UILinerImage);
RegisterLuaCreateUI(UILinerImageV);
RegisterLuaCreateUI(UIAniIcon);


#endif // _UI_H_
