#pragma once
#ifndef FONT_ENTITY_H_
#define FONT_ENTITY_H_

#include "EntityBase.h"
#include "Font.h"

enum TEXTPOSITION
{
	ALIGN_LEFT = 0,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

class CFontEntity : public CEntityBase
{
	
public:
	CFontEntity();
	~CFontEntity();

	virtual BOOL GetEntityPart(size_t index, Int ImageType, RECT * LockRegion);
	virtual Int GetTotalRenderPart();
	VOID LoadText(LPCTSTR TextContent, PCSTRINGA FontStyle, Int FontSize, UInt32 FontColor, UInt32 FontFlag = 0,
		D3DCOLOR pEdgeColor = 0xFF000000, UInt32 pLen = 0);

	VOID SetFontColor(D3DCOLOR Color);

	VOID SetShadowColor(D3DCOLOR Color);

	VOID SetColumn(UInt32 Col);

	VOID SetTextContent(LPCTSTR TextContent);

	VOID SetTextPosition(TEXTPOSITION position);

	VOID ResetTextPosition();

	virtual BOOL GetHaveEffect();

	VOID SetFontFlag(BYTE FontFlag);

	VOID DrawFont();

	inline bool haveContent(){return !m_TextContent.IsEmpty();}
protected:
	CFont * m_Font;

	String m_TextContent;

	UInt32 m_FontFlag;

	D3DCOLOR m_FontColor;
	
	D3DCOLOR m_ShadowColor;

	UInt32 m_Column;

	Int m_FontSize;

	TEXTPOSITION m_position;

	Byte * m_pCacheData;
};
#endif