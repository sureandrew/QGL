//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "FontEntity.h"
//-- Library
#include "Math.h"

CFontEntity::CFontEntity()
{
	m_Font = NULL;
	m_FontFlag = 0;
	m_FontColor = 0;
	m_FontSize = 0;
	m_Column = 0;
	m_ShadowColor = 0xFF000000;

	m_position = ALIGN_CENTER;
	m_pCacheData = NULL;
}

CFontEntity::~CFontEntity()
{
	if (m_pCacheData)
		SafeDeallocate(m_pCacheData);
	m_TextContent.Empty();
}

Int CFontEntity::GetTotalRenderPart()
{
	//text only have one content
	return 1;
}
BOOL CFontEntity::GetEntityPart(size_t index, Int ImageType, RECT * LockRegion)
{
	if (!m_Font || m_pCacheData == NULL)
		return FALSE;

	if (index < 1 && ImageType == ImagePic)
	{
		UInt32 Width = m_CurrRect.right - m_CurrRect.left;
		UInt32 Height = m_CurrRect.bottom - m_CurrRect.top;

		RECT EntLockRegion = m_EntityTotalRect;

		if (LockRegion)
		{
			EntLockRegion = *LockRegion;
			EntLockRegion.left -= GetEntLeftUpPos().PixelPosX;
			EntLockRegion.right -= GetEntLeftUpPos().PixelPosX;
			EntLockRegion.top -= GetEntLeftUpPos().PixelPosY;
			EntLockRegion.bottom -= GetEntLeftUpPos().PixelPosY;
		}

		if (EntLockRegion.top < 0)
		{
			EntLockRegion.bottom -= EntLockRegion.top;
			EntLockRegion.top = 0;
		}
		//clear the data first
		RECT ResultRect;
		
		if (QGLMath::CalculateOverlappedRect(EntLockRegion, m_EntityTotalRect, ResultRect))
		{
			m_CurrPicData = m_pCacheData + (ResultRect.top * Width + ResultRect.left) * 4;
		}
		//m_CurrPicData = m_pCacheData;// + (ResultRect.top * Height + ResultRect.left) * 4;
		m_PicWidth = Width;
		/*Byte * pDataSrc = m_pCacheData;
		Byte * pDataDes = m_CurrTexture;
		for (UInt y = 0; y < Height; y++)
		{
			memcpy(pDataDes, pDataSrc, Width * sizeof(Color32));
			pDataSrc += Width * sizeof(Color32);
			pDataDes += MaxEntSizeX * sizeof(Color32);
		}*/
		
		//draw the text

		return TRUE;
		
	}
	return FALSE;
}
VOID CFontEntity::LoadText(LPCTSTR TextContent, PCSTRINGA FontStyle, Int FontSize, UInt32 FontColor, UInt32 FontFlag,
						   D3DCOLOR pEdgeColor, UInt32 pLen)
{

	if (!m_Font)
		m_Font = CFontManager::GetInstance().CreateFont(FontStyle, FontSize, FontSize);
	
	m_TextContent = TextContent;

	m_CurrRect.left = m_CurrRect.top = 0;
	
	m_FontSize = FontSize;

	m_FontFlag = FontFlag;

	m_FontColor = FontColor;

	//get the cache memory
	m_CurrTexture = CResourcesManager::GetInstance().GetEntPicCacheTexture();

	m_ShadowColor = pEdgeColor;
	m_Column = pLen;

	//draw the text
	DrawFont();
	
}

VOID CFontEntity::DrawFont()
{
	if (m_TextContent.IsEmpty())
		return;

	//draw the text
	UInt32 Width = m_FontSize * m_TextContent.GetLength();
	UInt32 Height = m_FontSize * m_TextContent.GetLength();

	Byte * pDataSrc = m_CurrTexture;

	ZeroMemory(pDataSrc, MaxEntSizeX * MaxEntSizeY * sizeof(Color32));

	
	m_CurrRect.right = m_Font->DrawFontStr(m_TextContent.c_str(), m_CurrTexture, 1, 1, 
		MaxEntSizeX, MaxEntSizeY,

		MaxEntSizeX * sizeof(Color32), m_FontFlag, m_FontColor, m_ShadowColor, m_Column);
	
	m_CurrRect.bottom = m_Font->GetMaxY() + 2;
	m_EntityTotalRect = m_CurrRect;

	//reset the text position if the rectangle is updated
	ResetTextPosition();

	if (m_pCacheData)
		SafeDeallocate(m_pCacheData);

	Width = m_EntityTotalRect.right - m_EntityTotalRect.left;
	Height = m_EntityTotalRect.bottom - m_EntityTotalRect.top;
	//m_EntOrgWidth = Width;
	//m_EntOrgHeight = Height;
	m_pCacheData = SafeAllocate(Byte, (Width) *
		(Height) * sizeof(Color32));

	pDataSrc = m_CurrTexture;
	Byte * pDataDes = m_pCacheData;
	for (UInt y = 0; y < Height; y++)
	{
		memcpy(pDataDes, pDataSrc, Width * sizeof(Color32));
		pDataDes += Width * sizeof(Color32);
		pDataSrc += MaxEntSizeX * sizeof(Color32);
	}

	m_Updated = TRUE;
}
VOID CFontEntity::SetFontColor(D3DCOLOR Color)
{ 
	if (m_FontColor != Color)
	{
		m_FontColor = Color;
		//draw the text
		DrawFont();
	}
}

VOID CFontEntity::SetShadowColor(D3DCOLOR Color)
{
	if (m_ShadowColor != Color)
	{
		m_ShadowColor = Color;
		//draw the text
		DrawFont();
	}
}

VOID CFontEntity::SetColumn(UInt32 Col)
{
	if (m_Column != Col)
	{
		m_Column = Col;
		m_Updated = TRUE;
	}
}

VOID CFontEntity::SetTextContent(LPCTSTR TextContent)
{
	m_TextContent = TextContent;

	//draw the text
	DrawFont();
}

VOID CFontEntity::SetTextPosition(TEXTPOSITION position)
{
	if (m_position != position)
	{
		m_position = position;

		ResetTextPosition();

		m_Updated = TRUE;
	}
}

VOID CFontEntity::ResetTextPosition()
{
	switch(m_position)
	{
	case ALIGN_LEFT:
		SetEntOffsetX(-1 * (m_EntityTotalRect.right - m_EntityTotalRect.left));
		break;
	case ALIGN_CENTER:
		SetEntOffsetX(m_OffsetX = -1 * (m_EntityTotalRect.right - m_EntityTotalRect.left) / 2);
		break;
	case ALIGN_RIGHT:
		SetEntOffsetX(0);
		break;
	}
}

BOOL CFontEntity::GetHaveEffect()
{
	return FALSE;
}

VOID CFontEntity::SetFontFlag(BYTE FontFlag)
{
	if (m_FontFlag != FontFlag)
	{
		m_FontFlag = FontFlag;
		m_CurrRect.right = m_Font->DrawFontStr(m_TextContent.c_str(), m_CurrTexture, 0, 0, 
			m_EntityTotalRect.right - m_EntityTotalRect.left, m_EntityTotalRect.bottom - m_EntityTotalRect.top,
			MaxEntSizeX * sizeof(Color32), m_FontFlag, m_FontColor, m_ShadowColor, m_Column);


		m_CurrRect.bottom = m_Font->GetMaxY() + 2;
		m_EntityTotalRect = m_CurrRect;
		m_Updated = TRUE;
	}
}