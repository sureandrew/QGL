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



UIPanel::UIPanel() : UIItem()
{
	m_HotSpot = NULL;
	m_Texture = NULL;
	m_BackImage = NULL;
	m_OnMouseOverItem = NULL;
	m_AutoDeleteImage = false;
	m_SavePos = true;
	m_TextureWidth = m_TextureHeight = 0;
	m_BackColor = m_BoxColor = 0;
	m_IsOpened = false;
	m_CanRemove = true;
	m_Freeze = false;
	m_DragWidth = m_DragHeight = 25;

	m_FallowPanel = NULL;

	m_LuaPerLButtonUp.clear();
	m_LuaPerRButtonUp.clear();
}

void UIPanel::Dispose()
{
	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		SafeDeleteObject(*i);
	}

	m_SubItemList.clear();

	//if(m_AutoDeleteImage)
	{
		CImageManager::GetInstance().DeleteImage(m_BackImage);
		m_BackImage = NULL;
	}

	SafeRelease(m_Texture);
}


bool UIPanel::Detect(Int32 pX, Int32 pY)
{
	if(UIItem::Detect(pX, pY))
	{
		if (IsDelectImage() && m_BackImage)
		{
			PUInt32	PixelPtr = GetPixelPtr(pX, pY);

			if(*PixelPtr)
			{
				m_OnMouseOverItem = m_HotSpot = this;
				DetectSubItem(pX, pY);
				return 1;
			}
			else
				return 0;
		}
		else
		{
			m_OnMouseOverItem = NULL;
			return DetectSubItem(pX, pY);
		}
	}
	
	return 0;
}


bool UIPanel::DetectSubItem(Int32 pX, Int32 pY)
{

	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->Detect(pX, pY))
		{
			m_OnMouseOverItem = m_HotSpot = m_SubItemList[i]->GetHotSpot();
			return 1;
		}
	}
	
	return 0;
}




Int32 UIPanel::OnMouseMove(Int32 pX, Int32 pY)
{
	m_OnMouseOverItem = NULL;

	
	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->Detect(pX, pY))
		{
			if(m_SubItemList[i]->OnMouseMove(pX, pY) != -1)
			{
				m_OnMouseOverItem = m_HotSpot = m_SubItemList[i]->GetHotSpot();
				return 1;
			}
		}
	}

	if(IsDelectImage() && m_BackImage)
	{
		PUInt32	PixelPtr = GetPixelPtr(pX, pY);

		if(*PixelPtr)
		{
			m_OnMouseOverItem = m_HotSpot = this;
			return 1;
		}
	}

	m_HotSpot = NULL;
	return 0;
}

UInt32 UIPanel::Update(UInt32 dwDelta)
{
	return 0;
}


Int32 UIPanel::OnLButtonUp(Int32 pX, Int32 pY)
{
	m_HotSpot = NULL;
	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->OnLButtonUp(pX, pY))
		{
			m_HotSpot = m_SubItemList[i];
			return 1;
		}
	}
	return 0;
}

Int32 UIPanel::OnLButtonDown(Int32 pX, Int32 pY)
{
	if(!m_IsEnable)
		return 0;

	if( GetCanDrag() == 1)
	{
		gGlobal.pUIManager->SetDragPanel2Top();
	}

	m_HotSpot = NULL;
	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->OnLButtonDown(pX, pY))
		{
			m_HotSpot = m_SubItemList[i];
			return 1;
		}
	}

	if(m_BackImage)
	{
		if(UIItem::Detect(pX, pY))
		{
			if (IsDelectImage())
			{
				PUInt32	PixelPtr = GetPixelPtr(pX, pY);

				if(*PixelPtr)
					return 1;
			}
			else
				return 1;
		}
	}

	return 0;
}

Int32 UIPanel::OnRButtonUp(Int32 pX, Int32 pY)
{
	if(m_HotSpot)
	{
		m_HotSpot->OnLButtonUp(pX, pY);
	}
	return 0;
}

Int32 UIPanel::OnRButtonDown(Int32 pX, Int32 pY)
{
	if(!m_IsEnable)
		return 0;

	m_HotSpot = NULL;
	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->OnRButtonDown(pX, pY))
		{
			m_HotSpot = m_SubItemList[i];
			return 1;
		}
	}

	if(m_BackImage)
	{
		if(UIItem::Detect(pX, pY))
		{
			if (IsDelectImage())
			{
				PUInt32	PixelPtr = GetPixelPtr(pX, pY);

				if(*PixelPtr)
					return 1;
			}
			else
				return 1;
		}
	}

	return 0;
}


Int32  UIPanel::OnLButtonDBLCLK(Int32 pX, Int32 pY)
{
	if(!m_IsEnable)
		return 0;

	if( GetCanDrag() == 1)
	{
		gGlobal.pUIManager->SetDragPanel2Top();
	}

	m_HotSpot = NULL;
	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->OnLButtonDBLCLK(pX, pY))
		{
			m_HotSpot = m_SubItemList[i];
			return 1;
		}
	}

	if(m_BackImage)
	{
		if(UIItem::Detect(pX, pY))
		{
			if (IsDelectImage())
			{
				PUInt32	PixelPtr = GetPixelPtr(pX, pY);

				if(*PixelPtr)
					return 1;
			}
			else
				return 1;
		}
	}

	return 0;
}

Int32  UIPanel::OnRButtonDBLCLK(Int32 pX, Int32 pY)
{
	if(!m_IsEnable)
		return 0;

	m_HotSpot = NULL;
	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->OnRButtonDBLCLK(pX, pY))
		{
			m_HotSpot = m_SubItemList[i];
			return 1;
		}
	}

	if(m_BackImage)
	{
		if(UIItem::Detect(pX, pY))
		{
			if (IsDelectImage())
			{
				PUInt32	PixelPtr = GetPixelPtr(pX, pY);

				if(*PixelPtr)
					return 1;
			}
			else
				return 1;
		}
	}
	return 0;
}


Int32  UIPanel::OnMouseWheel(Int32 pX, Int32 pY, Int32 pZ)
{
	for(Int32 i = (Int32)m_SubItemList.size() - 1; i >= 0; --i)
	{
		if(m_SubItemList[i]->OnMouseWheel(pX, pY, pZ))
		{
			UIItem::LuaOnMouseWheel(m_SubItemList[i], pX, pY, pZ);
			return 1;
		}
	}

	if (UIItem::Detect(pX, pY))
		return UIItem::LuaOnMouseWheel(this, pX, pY, pZ);
	else
		return 0;
}

void UIPanel::RenderParent()
{
	m_PanelDirRectMan.AddAllDirty();
}

void UIPanel::RenderParent(RECT &RRect)
{
	m_PanelDirRectMan.AddDirtyRectToMan(RRect);
}
void UIPanel::Render(CImage *pImage)
{
	
	bool created = false;
	if (pImage == NULL)
	{
		CImage	*TempImage = SafeCreateObject1(CImage, m_Texture);
		pImage = TempImage;
		created = true;

		//TRACE_VERBOSEL_1(GLOBAL_LOGGER, _F("UI: UIPanel::Render(CImage *pImage), SafeCreateObject1(CImage, m_Texture); pImage = %p"),
		//pImage);

	}

	RECT	SRect;

	SetRect(&SRect, 0, 0, m_Width, m_Height);

	RECT	STextRect;
	SetRect(&STextRect, 0, 0, pImage->GetWidth(), pImage->GetHeight());

	if (!QGLMath::CalculateOverlappedRect(STextRect, SRect, SRect))
	{
		if (created)
		{
			SafeDeleteObject(pImage);

			//TRACE_VERBOSEL_1(GLOBAL_LOGGER, _F("UI: UIPanel::Render(CImage *pImage), SafeDeleteObject(pImage); pImage = %p"),
			//pImage);

		}
		return;
	}

	pImage->Lock();
	
	if(m_BackImage)
	{
		pImage->FillUInt32(0, &SRect);
		if(m_RenderStyle == 0 && m_Color == 0xFFFFFFFF)
		{
			Copy8888To8888(0, 0, m_BackImage, pImage, &m_BackRect);
		}
		else
			Alpha8888To8888S(0, 0, m_BackImage, pImage, &m_BackRect, m_RenderStyle, m_Color);
	}
	else
	{
		pImage->FillUInt32(m_BackColor, &SRect);
		if(m_BoxColor)
			pImage->DrawBox(m_BoxColor, &SRect);
	}

	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		if((*i)->IsEnable())
		{
			(*i)->Render(pImage);
		}
	}		

	pImage->Unlock();

	if (created)
	{
		SafeDeleteObject(pImage);
		//TRACE_VERBOSEL_1(GLOBAL_LOGGER, _F("UI: UIPanel::Render(CImage *pImage), (2) SafeDeleteObject(pImage); pImage = %p"),
		//	pImage);
	}

	if(FAILED(m_Texture->AddDirtyRect(NULL)))
		DebugBreak();

	if (m_pUIManager)
	{
		RECT realRect;
		SetRealRect(realRect);
		m_pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);
	}

	//m_PanelDirRectMan.ClearDirtyRect();
}


void UIPanel::Render(CImage *pImage, RECT &RRect)
{
	
	bool created = false;
	if (pImage == NULL)
	{
		CImage	*TempImage = SafeCreateObject1(CImage, m_Texture);
		pImage = TempImage;
		created = true;

		//TRACE_VERBOSEL_1(GLOBAL_LOGGER, _F("UI: UIPanel::Render(CImage *pImage, RECT &RRect), SafeCreateObject1(CImage, m_Texture); pImage = %p"),
		//pImage);
	}

	//image and panel
	RECT imageRect;
	RECT panelRect;
	SetRect(&panelRect, 0, 0, GetWidth(), GetHeight());
	SetRect(&imageRect, 0, 0, pImage->GetWidth(), pImage->GetHeight());

	RECT resultRect;

	if (!QGLMath::CalculateOverlappedRect(imageRect, panelRect, resultRect))
	{
		if (created)
			SafeDeleteObject(pImage);
		return;
	}

	if (!QGLMath::CalculateOverlappedRect(RRect, resultRect, resultRect))
	{
		if (created)
		{
			SafeDeleteObject(pImage);
			//TRACE_VERBOSEL_1(GLOBAL_LOGGER, _F("UI: UIPanel::Render(CImage *pImage, RECT &RRect), SafeDeleteObject(pImage); pImage = %p"),
			//	pImage);
		}
		return;
	}

	pImage->Lock();
	if(m_BackImage)
	{
		Int32		X = m_OffsetX, Y = m_OffsetY;
		RECT	SRect;

		memcpy(&SRect, &m_BackRect, sizeof(RECT));
		if(CImageDirtyRect(X, Y, SRect, resultRect))
		{
			pImage->FillUInt32(0, &resultRect);
			if(m_RenderStyle == 0 && m_Color == 0xFFFFFFFF)
			{
				Copy8888To8888(X, Y, m_BackImage, pImage, &SRect);
			}
			else
				Alpha8888To8888S(X, Y, m_BackImage, pImage, &SRect, m_RenderStyle, m_Color);
		}
	}
	else
	{
		pImage->FillUInt32(m_BackColor, &resultRect);
		if(m_BoxColor)
			pImage->DrawBox(m_BoxColor, &panelRect);
	}

	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		if((*i)->IsEnable())
			(*i)->Render(pImage, resultRect);
	}
	

	pImage->Unlock();

	if (created)
	{
		SafeDeleteObject(pImage);
		//TRACE_VERBOSEL_1(GLOBAL_LOGGER, _F("UI: UIPanel::Render(CImage *pImage, RECT &RRect), (2) SafeDeleteObject(pImage); pImage = %p"),
		//		pImage);
	}


	D3DSURFACE_DESC  pDesc;
	m_Texture->GetLevelDesc(0, &pDesc);

	
	if (!QGLMath::InvalidLock(&resultRect, m_TextureWidth, m_TextureHeight))
	{
		if (FAILED(m_Texture->AddDirtyRect(&resultRect)))
			DebugBreak();
	}

	if (m_pUIManager)
	{
		RECT realRect;
		SetRealRect(realRect);
		realRect.left += resultRect.left;
		realRect.top += resultRect.top;
		if (GetWidth() > resultRect.right)
			realRect.right -= GetWidth() - resultRect.right;
		if (GetHeight() > resultRect.bottom)
			realRect.bottom -= GetHeight() - resultRect.bottom;
		m_pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);
	}
}



void UIPanel::SetImageFileA(PCSTRINGA ImageName, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	StringW Filename;
	
	UTF82StringW(Filename, ImageName);
	CImage	*Image = CImageManager::GetInstance().CreateImage(Filename.c_str(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	SetImage(Image, pLeft, pTop, pRight, pBottom);
}

void UIPanel::SetImage(CImage *pImage, Int32 pLeft, Int32 pTop, Int32 pRight, Int32 pBottom)
{
	m_BackImage = pImage;
	//m_DetectImage = (pImage ? false : true);//(pImage ? true : false);
	SetRect(&m_BackRect, pLeft, pTop, pRight, pBottom);
	m_Width = pRight - pLeft;
	m_Height = pBottom - pTop;
}


void UIPanel::SetAllColor(UInt32 Color)
{
	SetColor(Color);

	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		(*i)->SetColor(Color);
	}
}


void UIPanel::SetTexture(LPDIRECT3DDEVICE9 pDevice)
{
	SafeRelease(m_Texture);

	Int tempHeight = 2;
	while (true)
	{
		if (tempHeight >= m_TextureHeight)
			break;
		tempHeight *= 2;
	}

	Int tempWidth = 2;
	while (true)
	{
		if (tempWidth >= m_TextureWidth)
			break;
		tempWidth *= 2;
	}

	//m_TextureWidth = tempWidth;
	//m_TextureHeight = tempHeight;
	D3DXCreateTexture(pDevice, tempWidth, tempHeight, 0, 0 ,
						D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_Texture);
	m_PanelDirRectMan.SetBoundRect(0, 0, m_TextureWidth, m_TextureHeight);
}


void UIPanel::SetTextureUI(UIManager *pUIManager)
{
	SetTexture(pUIManager->GetRender()->GetDevice());
}


void UIPanel::RenderDirtyRect()
{
	for (Int i = 0; i < (Int)m_PanelDirRectMan.GetDirtyRectLst().size(); i++)
	{
		Render(NULL, m_PanelDirRectMan.GetDirtyRectLst()[i]);
	}
}


void UIPanel::LeaveUIItem(UIItem *pItem)
{
	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		if(pItem == (*i))
		{
			m_SubItemList.erase(i);
			break;
		}
	}
}


void UIPanel::MovePos(Int32 X, Int32 Y)
{
	UIItem::MovePos(X, Y);
	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		(*i)->MovePos(X, Y);
	}
}


void UIPanel::SetPos(Int32 pX, Int32 pY)
{
	UIItem::SetPos(pX, pY);
	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		(*i)->SetOffsetPos(pX, pY);
		
	}
}


///////////////////////////////////////////////////////////////////////


PUInt32	UIPanelEx::GetPixelPtr(Int32 pX, Int32 pY)
{
	Int32 X = pX - m_X;
	Int32 Y = pY - m_Y;
	Int32 TopHigh = m_BackRect.bottom - m_BackRect.top; 
	Int32 LineHigh = m_LineRect.bottom - m_LineRect.top;
	Int32 TempHigh = LineHigh * m_LineCount;
	Int32 BottomY = TopHigh + TempHigh;
	PUInt32	PixelPtr = NULL;

	if(Y < TopHigh)
		PixelPtr = (PUInt32)(m_BackImage->GetPtrXY(m_BackRect.left + X, m_BackRect.top + Y));
	else
	if(Y >= BottomY)
		PixelPtr = (PUInt32)(m_BackImage->GetPtrXY(m_BottomRect.left + X, m_BottomRect.top + Y - BottomY));
	else
	{
		Int32 A = (Y - TopHigh) % LineHigh; 
		
		PixelPtr = (PUInt32)(m_BackImage->GetPtrXY(m_LineRect.left + X, m_LineRect.top + A));
	}

	return PixelPtr;
}


void UIPanelEx::Render(CImage *pImage)
{
	bool created = false;
	if (pImage == NULL)
	{
		CImage	*TempImage = SafeCreateObject2(CImage, m_Texture, 1);

		//TempImage->Lock();
		pImage = TempImage;
		created = true;

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIPanelEx::Render(CImage *pImage), SafeCreateObject2(pImage); pImage = %p"),
			pImage);
	}

	RECT	SRect;

	SetRect(&SRect, 0, 0, m_Width, m_Height);

	RECT	STextRect;
	SetRect(&STextRect, 0, 0, pImage->GetWidth(), pImage->GetHeight());

	if (!QGLMath::CalculateOverlappedRect(STextRect, SRect, SRect))
	{
		if (created)
		{
			SafeDeleteObject(pImage);

			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIPanelEx::Render(CImage *pImage), SafeDeleteObject(pImage); pImage = %p"),
				pImage);
		}
		return;
	}
	pImage->Lock();
	pImage->FillUInt32(0, &SRect);

	if(m_BackImage)
	{
		UInt32	TempY = m_OffsetY;

		Alpha8888To8888S(m_OffsetX, TempY, m_BackImage, pImage, &m_BackRect, m_RenderStyle, m_Color);
		m_BlitWidth = m_BackRect.right - m_BackRect.left;

		TempY += m_BackRect.bottom - m_BackRect.top;
		UInt32	TempHigh = m_LineRect.bottom - m_LineRect.top;
		for(UInt32 i = 0; i < m_LineCount; ++i)
		{
			Alpha8888To8888S(m_OffsetX, TempY, m_BackImage, pImage, &m_LineRect, m_RenderStyle, m_Color);
			TempY += TempHigh;
		}
		m_BlitWidth = max(m_BlitWidth, (UInt32)(m_LineRect.right - m_LineRect.left));
		Alpha8888To8888S(m_OffsetX, TempY, m_BackImage, pImage, &m_BottomRect, m_RenderStyle, m_Color);

		m_BlitWidth = max(m_BlitWidth, (UInt32)(m_BottomRect.right - m_BottomRect.left));
		m_BlitHeight = TempY + m_BottomRect.bottom - m_BottomRect.top;
	}
	
	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		if((*i)->IsEnable())
			(*i)->Render(pImage);
	}

	pImage->Unlock();
	if (created)
	{
		SafeDeleteObject(pImage);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIPanelEx::Render(CImage *pImage), SafeDeleteObject(pImage); pImage = %p"),
				pImage);
	}
	if(FAILED(m_Texture->AddDirtyRect(NULL)))
		DebugBreak();

	if (m_pUIManager)
	{
		RECT realRect;
		SetRealRect(realRect);
		m_pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);
	}

	//m_PanelDirRectMan.ClearDirtyRect();
}


void UIPanelEx::Render(CImage *pImage, RECT &RRect)
{
	bool created = false;
	if (pImage == NULL)
	{
		CImage	*TempImage = SafeCreateObject2(CImage, m_Texture, 1);

		//TempImage->Lock();
		pImage = TempImage;
		created = true;

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIPanelEx::Render(CImage *pImage, RECT &RRect), SafeCreateObject2(CImage, m_Texture, 1); pImage = %p"),
				pImage);
	}

	//image and panel
	RECT imageRect;
	RECT panelRect;
	SetRect(&panelRect, 0, 0, GetWidth(), GetHeight());
	SetRect(&imageRect, 0, 0, pImage->GetWidth(), pImage->GetHeight());

	RECT resultRect;

	if (!QGLMath::CalculateOverlappedRect(imageRect, panelRect, resultRect))
	{
		if (created)
			SafeDeleteObject(pImage);
		return;
	}

	if (!QGLMath::CalculateOverlappedRect(RRect, resultRect, resultRect))
	{
		if (created)
		{
			SafeDeleteObject(pImage);

			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIPanelEx::Render(CImage *pImage, RECT &RRect), SafeDeleteObject(pImage); pImage = %p"),
				pImage);
		}
		return;
	}

	
	pImage->Lock();

	if(m_BackImage)
	{
		Int32	TempY = m_OffsetY;
		Int32	X = m_OffsetX, Y = TempY;
		RECT	SRect;

		memcpy(&SRect, &m_BackRect, sizeof(RECT));
		if(CImageDirtyRect(X, Y, SRect, resultRect))
		{
			RECT	CRect;

			SetRect(&CRect, X, Y, X + SRect.right - SRect.left, Y + SRect.bottom - SRect.top);
			pImage->FillUInt32(0, &CRect);
			Alpha8888To8888S(X, Y, m_BackImage, pImage, &SRect, m_RenderStyle, m_Color);
		}

		m_BlitWidth = m_BackRect.right - m_BackRect.left;

		TempY += m_BackRect.bottom - m_BackRect.top;
		UInt32	TempHigh = m_LineRect.bottom - m_LineRect.top;
		for(UInt32 i = 0; i < m_LineCount; ++i)
		{
			X = m_OffsetX; Y = TempY;

			memcpy(&SRect, &m_LineRect, sizeof(RECT));
			if(CImageDirtyRect(X, Y, SRect, resultRect))
			{
				RECT	CRect;

				SetRect(&CRect, X, Y, X + SRect.right - SRect.left, Y + SRect.bottom - SRect.top);
				pImage->FillUInt32(0, &CRect);
				Alpha8888To8888S(X, Y, m_BackImage, pImage, &SRect, m_RenderStyle, m_Color);
			}
			TempY += TempHigh;
		}
		X = m_OffsetX; Y = TempY;
		m_BlitWidth = max(m_BlitWidth, (UInt32)(m_LineRect.right - m_LineRect.left));

		memcpy(&SRect, &m_BottomRect, sizeof(RECT));
		if(CImageDirtyRect(X, Y, SRect, resultRect))
		{
			RECT	CRect;

			SetRect(&CRect, X, Y, X + SRect.right - SRect.left, Y + SRect.bottom - SRect.top);
			pImage->FillUInt32(0, &CRect);
			Alpha8888To8888S(X, Y, m_BackImage, pImage, &SRect, m_RenderStyle, m_Color);
		}
		m_BlitHeight = TempY + m_BottomRect.bottom - m_BottomRect.top;
	}
	for(std::vector<UIItem *>::iterator i = m_SubItemList.begin(); i != m_SubItemList.end(); ++i)
	{
		if((*i)->IsEnable())
			(*i)->Render(pImage, resultRect);
	}

	pImage->Unlock();
	if (created)
	{
		SafeDeleteObject(pImage);

		TRACE_INFODTL_1(GLOBAL_LOGGER, _F("UI: UIPanelEx::Render(CImage *pImage, RECT &RRect), SafeDeleteObject(pImage); pImage = %p"),
				pImage);
	}

	D3DSURFACE_DESC  pDesc;
	m_Texture->GetLevelDesc(0, &pDesc);
	TRACE_INFODTL_6(GLOBAL_LOGGER, _F("UI: UIPanelEx::Render(CImage *pImage, RECT &RRect), RECT: Left=%d Top=%d Bottom=%d Right=%d Texture Wdith: %d Height: %d"),
		resultRect.left, resultRect.top, resultRect.bottom, resultRect.right, pDesc.Width, pDesc.Height);

	
	if (!QGLMath::InvalidLock(&resultRect, m_TextureWidth, m_TextureHeight))
	{
		if (FAILED(m_Texture->AddDirtyRect(&resultRect)))
			DebugBreak();
	}

	if (m_pUIManager)
	{
		RECT realRect;
		SetRealRect(realRect);
		realRect.left += resultRect.left;
		realRect.top += resultRect.top;
		realRect.right -= GetWidth() - resultRect.right;
		realRect.bottom -= GetHeight() - resultRect.bottom;
		m_pUIManager->GetDirtyRectMan()->AddDirtyRectToMan(realRect);
	}
}


