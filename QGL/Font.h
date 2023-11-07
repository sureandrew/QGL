#pragma once
#ifndef _FONT_H_
#define _FONT_H_

#include <ft2build.h>
#include <freetype/config/ftconfig.h>
#include <freetype/freetype.h>
#include <map>
#include <vector>


enum CFONT_STYLE
{
	EDGE_FONT1  = 0x1 << 1,
	EDGE_FONT2  = 0x2 << 1,
	EDGE_SHADOW = 0x3 << 1,
	EDGE_LIGHT = 0x4 << 1,
};




class CFont
{
public:
	enum {
		ALIGN_LEFT = 0, 
		ALIGN_RIGHT = 1, 
		ALIGN_CENTER = 2, 
	};

	CFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight);
	~CFont()
	{
		FT_Done_Face( m_FontFace );
		
		if( m_FileDataInMemory )
		{
			m_FileDataInMemory= NULL;
			UnmapViewOfFile(m_mapView);
		}
	};

	FT_Face	GetFontFace()
	{
		return m_FontFace;
	};
	Int32		GetFontChar(CharW c);
	Int32		GetDrawHeight(UInt32 Flag, UInt32 LinePitch);

	Int32		DrawFontChar(CharW C, PUInt8 pBuffer, 
				UInt32 X, UInt32 Y, 
				UInt32 Width, UInt32 High, 
				UInt32 Pitch, UInt32 Flag, 
				D3DCOLOR Color, 
				D3DCOLOR pEdgeColor = 0xFF000000, UInt32 Simulate = 0);

	Int32		DrawFontStr(PCSTRINGW Wstr, PUInt8 pBuffer, 
				UInt32 X, UInt32 Y, 
				UInt32 Width, UInt32 High, 
				UInt32 Pitch, UInt32 Flag, 
				D3DCOLOR Color, 
				D3DCOLOR pEdgeColor = 0xFF000000, UInt32 pLen = 0, UInt32 Simulate = 0);

	Int32		DrawFontStrG(PCSTRINGW Wstr, PUInt8 pBuffer, 
				UInt32 X, UInt32 Y, 
				UInt32 Width, UInt32 High, 
				UInt32 Pitch, UInt32 Flag, 
				D3DCOLOR Color, 
				D3DCOLOR pEdgeColor = 0xFF000000, UInt32 pLen = 0);

	Int32		DrawFontStrC(PCSTRINGW Wstr, String &MyStr,
				UInt32 X, UInt32 Y, UInt32 Width, UInt32 Pitch, 
				UInt32 Flag, UInt32 &pLen, bool Wrap = false);

	UInt32	RenderFont(PUInt8 pBuffer, 
				UInt32 X, UInt32 Y, 
				UInt32 Width, UInt32 High, 
				UInt32 Pitch, UInt32 Flag, 
				D3DCOLOR Color = 0xFFFFFFFF,
				D3DCOLOR pEdgeColor = 0x00000000);

	UInt32  RenderFontDL(PUInt8  pBuffer, 
				UInt32 X, UInt32 Y, 
				UInt32 pWidth, UInt32 pHigh, 
				UInt32 Pitch, UInt32 Flag, \
				D3DCOLOR Color = 0xFFFFFFFF,
				D3DCOLOR pEdgeColor = 0x00000000);

	UInt32	RenderEdgeFont(PUInt8 pBuffer, 
				UInt32 X, UInt32 Y, 
				UInt32 Width, UInt32 High, 
				UInt32 Pitch, UInt32 Flag, 
				D3DCOLOR pFontColor,
				D3DCOLOR pEdgeColor = 0xFF000000
				);
	UInt32	RenderShadowFont(PUInt8	pBuffer, 
				UInt32 X, UInt32 Y, 
				UInt32 Width, UInt32 High, 
				UInt32 Pitch, UInt32 Flag, 
				D3DCOLOR Color,
				D3DCOLOR pShadowColor = 0xFF000000
				);
	

	UInt32	GetAdvanceX(){ return m_AdvanceX; }
	UInt32	GetAdvanceY(){ return m_FontFace->glyph->advance.y >> 4; }
	FT_Bitmap &GetBitmap(){ return m_FontFace->glyph->bitmap; }
	UInt32	GetError();

	bool	Check(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight);
	UInt32	GetMaxY(){ return m_MaxY; }
	UInt32	GetWidth(){ return m_Width; }
	UInt32	GetHeight(){ return m_Height; }

	void SetLinePitch(UInt32	LinePitch){ m_LinePitch = LinePitch; }
	UInt32	GetLinePitch(){ return m_LinePitch; }
	
	
	UInt32	GetCharWidth(CharW c);


	UInt32	GetLastDrawWidth(){ return m_LastDrawWidth; }
	UInt32 GetLastDrawHeight(){ return m_LastDrawHeight; }

	void TakePitchHeight(UInt32 Flag, UInt32 &PitchHeight, UInt32 &RealFontHeight);


	void	SetupCache();

	void CalFontMax(Int32 &max_height, Int32 &max_width, Int32 &max_bear, CharW c);

	void SetAlign(UInt32 align){ m_Align = align; }
	void DrawAlignment(PUInt8 MoveBuffer, Int32	AlignSpace, UInt32 Len);

	void		SetSel(UInt32 Start, UInt32 End);

	Boolean GetDrawOver(){ return m_DrawOver; }
	UInt32 GetSelStartX(){ return m_SelStartX; }
	UInt32 GetSelEndWidth(){ return m_SelEndWidth; }

	std::vector<UInt32>		&GetLineWidth(){ return m_LineWidth; };
	std::vector<UInt32>		&GetLineStart(){ return m_LineStart; };

private:
	FT_Face			m_FontFace;
	FT_Error		m_FTError;
	StringA			m_FontsName;
	UInt32			m_Width;
	UInt32			m_Height;
	UInt32			m_Char_a_Height;
	UInt32			m_MaxY;

	bool			m_HasCache;



	UInt32			m_PixelMode;
	UInt32			m_CharWidth;
	UInt32			m_CharHeight;
	UInt32			m_CharPitch, m_LinePitch;
	UInt32			m_CharLeft;
	UInt32			m_CharTop;
	UInt32			m_AdvanceX;
	PUInt8			m_CharBuffer;
	
	UInt32			m_LastDrawWidth;
	UInt32			m_LastDrawHeight;
	UInt32			m_Align;
	Boolean			m_Format;
	Boolean			m_DrawOver;

	BYTE*			m_FileDataInMemory;
	BYTE*			m_mapView;
	UInt32			m_SelStart, m_SelEnd;
	UInt32			m_SelStartX, m_SelEndWidth;
	std::vector<UInt32>		m_LineWidth;
	std::vector<UInt32>		m_LineStart;
};


class CFontManager : public Reuben::Utility::Singleton<CFontManager>
{
public:
	CFontManager()
	{
	}
	~CFontManager()
	{
		Release();
	}

	SINGLETON(CFontManager);

	void Release()
	{
		ClearFonts();
	};
	Int32 InitFonts();
	void ClearFonts();

	CFont *FindFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight);
	CFont *DeleteFont(CFont *pFont);
	//CFont *DeleteFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight);
	CFont *CreateFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight);
	FT_Library		GetLibrary()
	{
		return m_FTLibrary;
	}
	
private:
	FT_Library				m_FTLibrary;
	FT_Error				m_FTError;
	std::vector<CFont *>	m_FontList;
	

};

#endif // _FONT_H_
