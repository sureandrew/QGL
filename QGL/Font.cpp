//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "Font.h"



inline UInt32 GetFontPixelMode2(PUInt8 buffer, UInt32 x, UInt32 y, UInt32 Pitch)
{
	return buffer[x + Pitch * y];
}


inline UInt32 GetFontPixel(PUInt8 buffer, UInt32 x, UInt32 y, UInt32 Pitch)
{
	return  ((buffer[(x >> 3) + Pitch * y] & (1 << (7 - (x % 8)))) ? 0xFF : 0);
}




CFont::CFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight)
{
	m_FontsName = pName;
	m_Width = pWidth;
	m_Height = pHeight;
	m_SelStartX = -1;
	m_SelEndWidth = 0;
	m_LinePitch = 0;
	m_DrawOver = false;

	FT_Open_Args  args;
	m_FileDataInMemory	= NULL;
	m_mapView			= NULL;

    /* test for valid `library' and `aface' delayed to FT_Open_Face() */

	BOOL readGdfSuccess = FALSE;
	if( g_GdfResource || g_GdfResourceEx )
	{
		String fontfilename = const_cast<CharA *>(pName);
		UInt32 beginOffset, fileSize;
		if ( g_GdfResourceEx )
			m_mapView = g_GdfResourceEx->GetMapViewOfFile(fontfilename, beginOffset, fileSize);
		if ( (!m_mapView) && g_GdfResource )
			m_mapView = g_GdfResource->GetMapViewOfFile(fontfilename, beginOffset, fileSize);
		if( m_mapView )
		{
			BYTE* buf = m_mapView + beginOffset;
			args.flags = FT_OPEN_MEMORY;
			args.memory_base = buf;
			args.memory_size = fileSize;
			m_FTError = FT_Open_Face( CFontManager::GetInstance().GetLibrary(), &args, 0, &m_FontFace);
			if( m_FTError==0 )
			{
				readGdfSuccess = TRUE;
				m_FileDataInMemory = buf;
			}else
			{
				UnmapViewOfFile(m_mapView);
			}
		}
	}
	if( !readGdfSuccess )
	{
		args.flags    = FT_OPEN_PATHNAME;
		args.pathname = const_cast<CharA *>(pName);
		
		m_FTError = FT_Open_Face( CFontManager::GetInstance().GetLibrary(), &args, 0, &m_FontFace);
	}

	m_HasCache = false;
	

	if (m_FTError)
		return;
	m_FTError = FT_Select_Charmap(m_FontFace, FT_ENCODING_UNICODE);
	if (m_FTError)
		return;
	m_FTError = FT_Set_Pixel_Sizes(m_FontFace, pWidth, pHeight);
	if (m_FTError)
		return;
	
	FT_UInt glyph_index = FT_Get_Char_Index(m_FontFace, _T('a'));
	FT_Load_Glyph(m_FontFace, glyph_index, FT_LOAD_RENDER); 
	FT_Bitmap &bitmap = m_FontFace->glyph->bitmap;
	m_Char_a_Height = bitmap.rows;

	m_Format = false;
	m_Align = ALIGN_LEFT;
	m_LineWidth.clear();
	m_LineStart.clear();
};


Int32 CFont::GetFontChar(CharW c)
{
	bool	IsNew = false;
	
	

	FT_UInt glyph_index = FT_Get_Char_Index(m_FontFace, c);
	m_FTError = FT_Load_Glyph(m_FontFace, glyph_index, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT |
		(true ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_NORMAL)); 
	if (m_FTError)
		return -1;

	if (m_FontFace->glyph->format != FT_GLYPH_FORMAT_BITMAP)
	{
		m_FTError = FT_Render_Glyph(m_FontFace->glyph, FT_RENDER_MODE_MONO);
		if (m_FTError)
			return -1;
	}


	{
		FT_Bitmap &bitmap = m_FontFace->glyph->bitmap;

		m_PixelMode	= bitmap.pixel_mode;
		m_CharWidth = bitmap.width;
		m_CharHeight = bitmap.rows;
		m_CharLeft = m_FontFace->glyph->bitmap_left;
		
		m_CharTop = m_CharHeight;
		
		m_CharPitch = m_CharWidth;
		m_AdvanceX = (m_FontFace->glyph->advance.x >> 6 ) + ( m_FontFace->glyph->metrics.horiBearingX >> 6 );

		
		
		if(m_PixelMode != 2)
		{
			m_CharPitch = bitmap.pitch;
		}
		m_CharBuffer = bitmap.buffer;
			
	
	}

	return 1;
}


Int32 CFont::DrawFontStrG(PCSTRINGW WStr, PUInt8 pBuffer, UInt32 X, UInt32 Y, UInt32 Width, UInt32 High, UInt32 Pitch, UInt32 Flag, D3DCOLOR Color, D3DCOLOR pEdgeColor, UInt32 pLen)
{
	m_Format = true;
	UInt32 Ret = DrawFontStr(WStr, pBuffer, X, Y, Width, High, Pitch, Flag, Color, pEdgeColor, pLen, 0);
	m_Format = false;

	return Ret;
}

void CFont::CalFontMax(Int32 &max_height, Int32 &max_width, Int32 &max_bear, CharW c)
{
	FT_UInt glyph_index = FT_Get_Char_Index(m_FontFace, c);
	m_FTError = FT_Load_Glyph(m_FontFace, glyph_index, FT_LOAD_RENDER);

	if( ( 2 * ( m_FontFace->glyph->bitmap.rows << 6 ) - m_FontFace->glyph->metrics.horiBearingY ) > max_height )
		max_height = ( 2 * ( m_FontFace->glyph->bitmap.rows << 6 ) - m_FontFace->glyph->metrics.horiBearingY );

	if( m_FontFace->glyph->metrics.horiBearingY > max_bear )
		max_bear = m_FontFace->glyph->metrics.horiBearingY;

	if( (m_FontFace->glyph->advance.x >> 6 ) + ( m_FontFace->glyph->metrics.horiBearingX >> 6 ) > max_width)
		max_width = (m_FontFace->glyph->advance.x >> 6 ) + ( m_FontFace->glyph->metrics.horiBearingX >> 6 );
}


void CFont::DrawAlignment(PUInt8 MoveBuffer, Int32	AlignSpace, UInt32 Len)
{
	if(AlignSpace > 0 && m_Align)
	{
		if(m_Align == ALIGN_RIGHT)
			AlignSpace *= 4;
		else
		if(m_Align == ALIGN_CENTER)
			AlignSpace = AlignSpace / 2 * 4;

		if(AlignSpace < 0)
			AlignSpace = 0;

		memmove(MoveBuffer + AlignSpace, MoveBuffer, Len - AlignSpace);
		memset(MoveBuffer, 0, AlignSpace);
		m_SelStartX += AlignSpace / 4;
	}
}


Int32 CFont::GetDrawHeight(UInt32 Flag, UInt32 LinePitch)
{
	UInt32	TempFlag = Flag & (0x7 << 1);
	UInt32	PitchHeight = 0;

	switch(TempFlag)
	{
	case EDGE_FONT1:
		PitchHeight = 1;
		break;
	case EDGE_FONT2:
		PitchHeight = 2;
		break;
	case EDGE_SHADOW:
		PitchHeight = 1;
		break;
	}
	
	return m_Height + PitchHeight + LinePitch;
}



Int32 CFont::DrawFontChar(CharW C, PUInt8 pBuffer, UInt32 X, UInt32 Y, UInt32 Width, UInt32 High, UInt32 Pitch, UInt32 Flag, 
						D3DCOLOR Color, D3DCOLOR pEdgeColor, UInt32 Simulate)
{
	PUInt8	my_Buffer = pBuffer + X * 4 + Y  * Pitch;
	PUInt8	MoveBuffer = pBuffer;
	UInt32	TempFlag = Flag & (0x7 << 1);
	UInt32	WrapFlag = Flag & (0x1 << 7);
	UInt32	BufferLen = Pitch * High;

	UInt32	(CFont::*RenderFontFunc)(PUInt8, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, D3DCOLOR, D3DCOLOR);
	UInt32	OffsetX, OffsetY;
	UInt32	PitchHeight, RealFontHeight;

	m_DrawOver = false;

	m_LastDrawWidth = X;
	m_LastDrawHeight = Y;
	
	switch(TempFlag)
	{
	case EDGE_FONT1:
		RenderFontFunc = &CFont::RenderEdgeFont;
		OffsetX = 1;
		OffsetY = 1;
		RealFontHeight = 2;
		PitchHeight = 1;
		break;
	case EDGE_FONT2:
		RenderFontFunc = &CFont::RenderEdgeFont;
		OffsetX = 2;
		OffsetY = 2;
		RealFontHeight = 4;
		PitchHeight = 2;
		break;
	case EDGE_SHADOW:
		RenderFontFunc = &CFont::RenderShadowFont;
		OffsetX = 0;
		OffsetY = 0;
		RealFontHeight = 1;
		PitchHeight = 1;
		break;
	case EDGE_LIGHT:
		RenderFontFunc = &CFont::RenderFontDL;
		OffsetX = 0;
		OffsetY = 0;
		RealFontHeight = 0;
		PitchHeight = 0;
		break;
	default:
		RenderFontFunc = &CFont::RenderFont;
		OffsetX = 0;
		OffsetY = 0;
		RealFontHeight = 0;
		PitchHeight = 0;
	}

	Int32 max_height = 0, max_width = 0, max_bear = 0;


	CalFontMax(max_height, max_width, max_bear, 0x570B);
	CalFontMax(max_height, max_width, max_bear, _T('y'));
	CalFontMax(max_height, max_width, max_bear, C);

	GetFontChar(C);

	int y_bearnig = (max_bear >> 6) - ( m_FontFace->glyph->metrics.horiBearingY >> 6 );
	
	Int32	TempWidth = m_AdvanceX + RealFontHeight + X;

	if( TempWidth >= (Int32)Width)
	{
		m_DrawOver = true;
		TempWidth = 0;
	}
	else
	{
		if(!Simulate)
			(this->*RenderFontFunc)(my_Buffer + (y_bearnig) * Pitch + m_CharLeft * 4, 
				OffsetX, OffsetY, Width, High, Pitch, Flag, Color, pEdgeColor);
	}

	return TempWidth;
}



Int32 CFont::DrawFontStr(PCSTRINGW WStr, PUInt8 pBuffer, UInt32 X, UInt32 Y, UInt32 Width, UInt32 High, UInt32 Pitch, UInt32 Flag, 
						D3DCOLOR Color, D3DCOLOR pEdgeColor, UInt32 pLen, UInt32 Simulate)
{
	PUInt8	my_Buffer = pBuffer + X * 4 + Y  * Pitch;
	PUInt8	MoveBuffer = pBuffer;
	UInt32	TempFlag = Flag & (0x7 << 1);
	UInt32	WrapFlag = Flag & (0x1 << 7);
	UInt32	i;
	UInt32	BufferLen = Pitch * High;
	UInt32	DrawWidth = 0, DrawHeight = 0, Row = 0, MaxHeight = 0;
	UInt32	Limit = lstrlen(WStr);
	if(pLen && pLen < Limit)
		Limit = pLen;

	UInt32	(CFont::*RenderFontFunc)(PUInt8, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, D3DCOLOR, D3DCOLOR);
	UInt32	OffsetX, OffsetY;
	UInt32	PitchHeight, RealFontHeight;

	bool RealDraw = !Simulate;
	m_DrawOver = false;

	m_LastDrawWidth = X;
	m_LastDrawHeight = Y;

	m_LineWidth.clear();
	m_LineStart.clear();

	switch(TempFlag)
	{
	case EDGE_FONT1:
		RenderFontFunc = &CFont::RenderEdgeFont;
		OffsetX = 1;
		OffsetY = 1;
		RealFontHeight = 2;
		PitchHeight = 1;
		break;
	case EDGE_FONT2:
		RenderFontFunc = &CFont::RenderEdgeFont;
		OffsetX = 2;
		OffsetY = 2;
		RealFontHeight = 4;
		PitchHeight = 2;
		break;
	case EDGE_SHADOW:
		RenderFontFunc = &CFont::RenderShadowFont;
		OffsetX = 0;
		OffsetY = 0;
		RealFontHeight = 1;
		PitchHeight = 1;
		break;
	case EDGE_LIGHT:
		RenderFontFunc = &CFont::RenderFontDL;
		OffsetX = 0;
		OffsetY = 0;
		RealFontHeight = 0;
		PitchHeight = 0;
		break;
	default:
		RenderFontFunc = &CFont::RenderFont;
		OffsetX = 0;
		OffsetY = 0;
		RealFontHeight = 0;
		PitchHeight = 0;
	}

	Int32 max_height = 0, max_width = 0, max_bear = 0;


	CalFontMax(max_height, max_width, max_bear, 0x570B);
	CalFontMax(max_height, max_width, max_bear, _T('y'));
	bool	HideChar = false;
	for(i = 0; i < Limit; ++i)
	{
		if(WStr[i] < 26)
		{
			switch(WStr[i])
			{
			case 15:
				HideChar = true;
				break;
			case 16:
				HideChar = false;
				break;
			case 17:
				break;
			case 18:
				break;
			}
			continue;
		}

		if(!HideChar)
			CalFontMax(max_height, max_width, max_bear, WStr[i]);
	}

	m_SelEndWidth = 0;
	m_SelStartX = 0;
	HideChar = false;

	m_LineStart.push_back(0);
	
	for(i = 0; i < Limit; ++i)
	{
		if(WStr[i] < 26)
		{
			switch(WStr[i])
			{
			case 15:
				HideChar = true;
				break;
			case 16:
				HideChar = false;
				break;
			case 17:
				break;
			case 18:
				break;
			}
			continue;
		}

		if(HideChar)
			continue;
		
		if(i == m_SelStart)
			m_SelStartX = DrawWidth;
	
		GetFontChar(WStr[i]);

		int y_bearnig = (max_bear >> 6) - ( m_FontFace->glyph->metrics.horiBearingY >> 6 );
		//int y_bearnig = m_Height - ( m_FontFace->glyph->metrics.horiBearingY >> 6 );

		if(m_Format)
		{
			if(WStr[i] == _T('\\'))
			{
				if(WStr[i + 1] == _T('r'))
				{
					DrawAlignment(MoveBuffer + Y * Pitch, (Int32)Width - (Int32)DrawWidth, (m_Height + PitchHeight) * Pitch);

					m_LineWidth.push_back(DrawWidth);

					Y += m_Height + PitchHeight + m_LinePitch;
					if(Y >= High)
						break;
					DrawWidth = 0;
					if(i == m_SelStart)
						m_SelStartX = 0;
					if(i >= m_SelStart && i < m_SelEnd)
						m_SelEndWidth = 0;
					++Row;
					X = 0;
					m_LineStart.push_back(i);
					DrawHeight += m_Height + PitchHeight + m_LinePitch;

					my_Buffer = pBuffer + Y * Pitch;
					++i;
					continue;
				}
			}
		}

		Int32	TempWidth = m_AdvanceX + RealFontHeight;

		if(X + DrawWidth + TempWidth >= (Int32)Width)
		{
			DrawAlignment(MoveBuffer + Y * Pitch, (Int32)Width - (Int32)DrawWidth, (m_Height + PitchHeight) * Pitch);

			Y += m_Height + PitchHeight + m_LinePitch;
			if(Y >= High)
				break;
			m_LineWidth.push_back(DrawWidth);
			DrawWidth = 0;
			if(i == m_SelStart)
				m_SelStartX = 0;
			if(i >= m_SelStart && i < m_SelEnd)
				m_SelEndWidth = 0;
			++Row;
			X = 0;
			m_LineStart.push_back(i);
			DrawHeight += m_Height + PitchHeight + m_LinePitch;

			my_Buffer = pBuffer + Y * Pitch;
			
		}

		
		
		if(Y + m_Height + RealFontHeight >= High)
		{
			m_DrawOver = true;
			break;
		}
		if(RealDraw)
			(this->*RenderFontFunc)(my_Buffer + (y_bearnig) * Pitch + m_CharLeft * 4, 
				OffsetX, OffsetY, Width, High, Pitch, Flag, Color, pEdgeColor);
		
		UInt32 TempHeight = (y_bearnig) + m_Height + RealFontHeight;

		if(Y + TempHeight > m_LastDrawHeight)
			m_LastDrawHeight = Y + TempHeight;
		if(TempHeight > MaxHeight)
			MaxHeight = TempHeight;
		if(TempWidth + X > m_LastDrawWidth)
			m_LastDrawWidth = TempWidth + X;

		my_Buffer += (m_AdvanceX + PitchHeight) * 4;
		DrawWidth += (m_AdvanceX + PitchHeight);
		
		if(i >= m_SelStart && i < m_SelEnd)
			m_SelEndWidth += TempWidth;
	}

	if(DrawWidth)
	{
		DrawAlignment(MoveBuffer + Y * Pitch, (Int32)Width - (Int32)DrawWidth, (m_Height + PitchHeight) * Pitch);
		m_LineWidth.push_back(DrawWidth);
	}


	m_MaxY = MaxHeight;

	return DrawWidth;
}



void CFont::TakePitchHeight(UInt32 Flag, UInt32 &PitchHeight, UInt32 &RealFontHeight)
{
	UInt32	TempFlag = Flag & (0x7 << 1);

	switch(TempFlag)
	{
	case EDGE_FONT1:
		RealFontHeight = 2;
		PitchHeight = 1;
		break;
	case EDGE_FONT2:
		RealFontHeight = 4;
		PitchHeight = 2;
		break;
	case EDGE_SHADOW:
		RealFontHeight = 1;
		PitchHeight = 1;
		break;
	default:
		RealFontHeight = 0;
		PitchHeight = 0;
	}

}


Int32 CFont::DrawFontStrC(PCSTRINGW WStr, String &MyStr,
				UInt32 X, UInt32 Y, UInt32 Width, UInt32 Pitch, UInt32 Flag, UInt32 &pLen, bool Wrap)
{
	UInt32	i;
	Int32	Ret = 0;
	Int32	StartX = X;
	StringW	TWStr = WStr;
	
	UInt32	PitchHeight, RealFontHeight;

	m_DrawOver = false;

	TakePitchHeight(Flag, PitchHeight, RealFontHeight);

	m_LastDrawWidth = X;
	m_LastDrawHeight = 0;

	MyStr.clear();

	Int32 max_height = 0, max_width = 0, max_bear = 0;

	CalFontMax(max_height, max_width, max_bear, _T('d'));
	CalFontMax(max_height, max_width, max_bear, _T('y'));

	bool HideChar = false;
	for(i = 0; i < TWStr[i]; ++i)
	{
		if(TWStr[i] < 26)
		{
			switch(TWStr[i])
			{
			case 15:
				HideChar = true;
				break;
			case 16:
				HideChar = false;
				break;
			case 17:
				break;
			case 18:
				break;
			}
			continue;
		}

		if(!HideChar)
			CalFontMax(max_height, max_width, max_bear, TWStr[i]);
	}

	HideChar = false;

	if(WStr[0] == _T(' ') && Wrap)
	{
		GetFontChar(_T(' '));
		UInt32	TempWidth = m_AdvanceX + RealFontHeight;

		for(i = 0; TWStr[i]; ++i)
		{
			if(WStr[i] != _T(' '))
			{
				break;
			}

			if(X + TempWidth < Width)
			{
				m_LastDrawWidth += TempWidth;
				X += m_AdvanceX + PitchHeight;

				MyStr.push_back(_T(' '));
			}
			else
			{
				if(StartX != 0)
				{
					i = 0;
					MyStr.clear();
				}
				
				Ret = -1;
				break;
			}
		}
	}
	else
	{
		for(i = 0; TWStr[i]; ++i)
		{
			if(WStr[i] == '\r')
			{
				break;
			}

			if(WStr[i] < 26)
			{
				switch(WStr[i])
				{
				case 15:
					HideChar = true;
					break;
				case 16:
					HideChar = false;
					break;
				case 17:
					break;
				case 18:
					MyStr.push_back(TWStr[i]);
					goto Label_1;
					break;
				}
				continue;
			}

			if(TWStr[i] == _T(' ') && Wrap)
			{
				break;
			}
			else
			if(TWStr[i] == _T('#'))
			{
				if(TWStr[i + 1] == _T('#'))
				{
					++i;
				}
				else
				{
					break;
				}
			}
			else
			if(TWStr[i] == _T('\\'))
			{
				if(TWStr[i + 1] == _T('\\'))
				{
					++i;
				}
				else
				if(TWStr[i + 1] == _T('*'))
				{
					TWStr[i + 1] = 15;
					++i;
					HideChar = true;
				}
				else
				if(TWStr[i + 1] == _T('$'))
				{
					TWStr[i + 1] = 16;
					++i;
					HideChar = false;
				}
				else
				{
					StringW	Temp = "rcC[]()iuUT";
					StringW::size_type Index = Temp.find(TWStr[i + 1]);
				
					if(Index != StringW::npos)
						break;
					else
						continue;
				}
			}
			/*
			else
			if(TWStr[i] == _T(' '))
			{
				UInt32	TempWidth = m_Width / 2 + PitchHeight;

				if(X + TempWidth < Width)
				{
					m_LastDrawWidth += TempWidth;
					X += m_Width / 2 + PitchHeight;

					MyStr.push_back(_T(' '));
				}
				else
				{
					Ret = -1;
					break;
				}
				continue;
			}
			*/
			
			if(HideChar)
				continue;

			{
				GetFontChar(TWStr[i]);
				//int y_bearnig = (max_bear >> 6) - ( m_FontFace->glyph->metrics.horiBearingY >> 6 );
				//int y_bearnig = m_Height - ( m_FontFace->glyph->metrics.horiBearingY >> 6 );

				UInt32	TempWidth = m_AdvanceX + RealFontHeight;
				if(X + TempWidth < Width)
				{
					m_LastDrawWidth += TempWidth;
					X += m_AdvanceX + PitchHeight;

					MyStr.push_back(TWStr[i]);
				}
				else
				{
					if(StartX != 0 && Wrap)
					{
						i = 0;
						MyStr.clear();
					}

					Ret = -1;
					break;
				}
			}
		}
	}

Label_1:

	pLen = i;

	return Ret;
}



inline void PlotFontTo8888(PUInt8 MyPtr, UInt32 Offset, PUInt8 Color, UInt8 pValue)
{
	MyPtr += Offset;

	MyPtr[0] = Color[0];
	MyPtr[1] = Color[1];
	MyPtr[2] = Color[2];
	MyPtr[3] = static_cast<UInt8>(((Color[3] - pValue) >> 8) + pValue);

	//Ptr[3] = static_cast<UInt8>((pValue * Color[3]) / 255);

	//DPixelPtr[3] = static_cast<UInt8>(((SPixelPtr[3] - DPixelPtr[3]) >> 8 ) + DPixelPtr[3]); //A
}

inline void PlotFontTo8888A(PUInt8 MyPtr, UInt32 Color, UInt32 pValue)
{
	__asm
	{
		mov		edi, MyPtr
		mov		eax, Color
		bswap	eax
		movzx	edx, al
		sub		edx, pValue
		shr		edx, 8
		add		edx, pValue
		mov		al, dl
		bswap	eax

		stosd
	}


	//Ptr[3] = static_cast<UInt8>((pValue * Color[3]) / 255);

	//DPixelPtr[3] = static_cast<UInt8>(((SPixelPtr[3] - DPixelPtr[3]) >> 8 ) + DPixelPtr[3]); //A
}



void AlphaFontTo8888(PUInt8 Ptr, UInt32 Offset, PUInt8 Color, UInt32 pValue)
{
	UInt8	NewColor[4]; 
	
	Ptr += Offset;
	if(Ptr[3])
	{
		NewColor[0] = Color[0];
		NewColor[1] = Color[1];
		NewColor[2] = Color[2];
		NewColor[3] = static_cast<UInt8>(((Color[3] - pValue) >> 8) + pValue);
		//NewColor[3] = static_cast<UInt8>((pValue * Color[3]) / 255);

		UInt8	DOp = 255 - NewColor[3];

		Ptr[0] = static_cast<UInt8>((NewColor[3] * (NewColor[0] - Ptr[0]) >> 8) + Ptr[0]);
		Ptr[1] = static_cast<UInt8>((NewColor[3] * (NewColor[1] - Ptr[1]) >> 8) + Ptr[1]);
		Ptr[2] = static_cast<UInt8>((NewColor[3] * (NewColor[2] - Ptr[2]) >> 8) + Ptr[2]);
		if(NewColor[3] > Ptr[3])
			Ptr[3] = NewColor[3];
			//static_cast<UInt8>(((NewColor[3] - Ptr[3]) >> 8) + Ptr[3]);
	}
	else
	{
		Ptr[0] = Color[0];
		Ptr[1] = Color[1];
		Ptr[2] = Color[2];
		Ptr[3] = static_cast<UInt8>(((Color[3] - pValue) >> 8) + pValue);
		//Ptr[3] = static_cast<UInt8>((pValue * Color[3]) / 255);
	}
}


/*
void AlphaFontTo8888A(PUInt8 MyPtr, UInt32 Color, UInt32 pValue)
{
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;

	
	
	__asm
	{
		mov			edi, MyPtr

		mov			eax, Color
		bswap	eax
		movzx		edx, al
		sub			edx, pValue
		shr			edx, 8
		add			edx, pValue
		mov			al, dl
		bswap	eax

		
		cmp			DWORD PTR [edi], 0
		je			Label_71	

		movd		mm0, eax
		movd		mm1, DWORD PTR [edi]
		pxor		mm2, mm2

		PUNPCKLBW	mm0, mm2
		PUNPCKLBW	mm1, mm2
		movq		mm3, mm0

		PUNPCKHWD	mm3, mm3
		PUNPCKHDQ	mm3, mm3
		
		psubw		mm0, mm1
		pmullw		mm0, mm3
		PSRLW		mm0, 8
		paddb		mm1, mm0
	

		Packuswb	mm1, mm1
		
		movd		eax, mm1
		emms

Label_71:

		stosd
	}
	
}
*/


inline void PlotEage(UInt32	*pPtr, UInt32 pPitch, UInt32 pEdgeColor)
{
	__asm
	{
		mov		edx, pPitch
		shl		edx, 2	

		mov		edi, pPtr
		sub		edi, edx
		sub		edi, 4

		mov		eax, pEdgeColor

		stosd
		stosd
		stosd

		add		edi, edx
		sub		edi, 12

		stosd
		stosd
		stosd

		add		edi, edx
		sub		edi, 12

		stosd
		stosd
		stosd

	}
}


inline void PlotEage2(UInt32	*pPtr, UInt32 pPitch, UInt32 pEdgeColor)
{
	
	__asm
	{
		mov		edx, pPitch
		shl		edx, 2	

		mov		edi, pPtr
		sub		edi, edx
		sub		edi, edx
		sub		edi, 4

		mov		eax, pEdgeColor

		mov		ecx, 3
		rep		stosd
		
		add		edi, edx
		sub		edi, 12

		mov		ecx, 5
		rep		stosd

		add		edi, edx
		sub		edi, 20

		mov		ecx, 5
		rep		stosd
		
		add		edi, edx
		sub		edi, 20

		mov		ecx, 5
		rep		stosd

		add		edi, edx
		sub		edi, 24

		mov		ecx, 3
		rep		stosd

	}
}




UInt32 CFont::RenderFont(PUInt8  pBuffer, UInt32 X, UInt32 Y, UInt32 pWidth, UInt32 pHeight, 
					   UInt32 Pitch, UInt32 Flag, D3DCOLOR Color, D3DCOLOR pEdgeColor)
{
	PUInt8	CharBuffer = m_CharBuffer;
	UInt32	CharWidth = m_CharWidth;
	UInt32	CharHeight = m_CharHeight;

	if(X + m_CharWidth > pWidth)
		return 0;

	if(Y + m_CharHeight > pHeight)
		return 0;

	if(m_PixelMode == 2)
	{
		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			/*
			for (UInt32 j = 0; j < m_CharWidth; ++j)
			{
				UInt32 _vl = CharBuffer[j];

				if(_vl)
				{
					PlotFontTo8888A(pBuffer  + (j << 2), Color, _vl);
				}
			}
			*/
			__asm
			{
			pushad

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

Label_1:
			lodsb

			cmp		al, 0
			je		Label_2

			
			movzx	ebx, al
		
			mov		eax, Color
			bswap	eax
			movzx	edx, al
			sub		edx, ebx
			shr		edx, 8
			add		edx, ebx
			mov		al, dl
			bswap	eax
			

			jmp		Label_3
Label_2:
			add		edi, 4
			loop	Label_1
			jmp		Label_5
Label_3:
			stosd

Label_4:
			loop	Label_1

Label_5:

			popad
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
	}
	else
	{
		//= ((CharBuffer[(j >> 3)] & (1 << (7 - (j % 8)))) ? 0xFF : 0);
		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			__asm
			{
			pushad

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

			lodsb
			mov		bl, al
			mov		bh, 0x80

Label_21:
			and		al, bh
			
			cmp		al, 0
			je		Label_22

			
			mov		eax, Color
			
			bswap	eax
			movzx	edx, al
			sub		edx, 0x000000FF
			shr		edx, 8
			add		edx, 0x000000FF
			mov		al, dl
			
			bswap	eax
			
			jmp		Label_23
Label_22:
			add		edi, 4
			jmp		Label_24
Label_23:
			stosd

Label_24:
			shr		bh, 1
			cmp		bh, 0
			jne		Label_26

			mov		bh, 0x80
			lodsb
			mov		bl, al
Label_26:
			mov		al, bl


			loop	Label_21

			popad
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
	}

	return 1;
}



UInt32 CFont::RenderFontDL(PUInt8  pBuffer, UInt32 X, UInt32 Y, UInt32 pWidth, UInt32 pHeight, 
					   UInt32 Pitch, UInt32 Flag, D3DCOLOR Color, D3DCOLOR pEdgeColor)
{
	PUInt8	CharBuffer = m_CharBuffer;
	UInt32	CharWidth = m_CharWidth;
	UInt32	CharHeight = m_CharHeight;

	if(X + m_CharWidth > pWidth)
		return 0;

	if(Y + m_CharHeight > pHeight)
		return 0;

	if(m_PixelMode == 2)
	{
		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			__asm
			{
			pushad

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

Label_1:
			lodsb

			cmp		al, 0
			je		Label_2

			
			movzx	ebx, al
		
			mov		eax, Color
			bswap	eax
			movd	mm0, ebx
			paddusb	mm0, mm0
			movd	ebx, mm0
			emms	
			mov		al, bl
			bswap	eax

			jmp		Label_3
Label_2:
			add		edi, 4
			loop	Label_1
			jmp		Label_5
Label_3:
			stosd

Label_4:
			loop	Label_1

Label_5:

			popad
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
	}
	else
	{
		//= ((CharBuffer[(j >> 3)] & (1 << (7 - (j % 8)))) ? 0xFF : 0);
		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			__asm
			{
			pushad

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

			lodsb
			mov		bl, al
			mov		bh, 0x80

Label_21:
			and		al, bh
			
			cmp		al, 0
			je		Label_22

			
			mov		eax, Color

			bswap	eax
			movzx	edx, al
			sub		edx, 0x000000FF
			shr		edx, 8
			add		edx, 0x000000FF
			mov		al, dl

			bswap	eax
			
			jmp		Label_23
Label_22:
			add		edi, 4
			jmp		Label_24
Label_23:
			stosd

Label_24:
			shr		bh, 1
			cmp		bh, 0
			jne		Label_26

			mov		bh, 0x80
			lodsb
			mov		bl, al
Label_26:
			mov		al, bl


			loop	Label_21

			popad
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
	}

	return 1;
}


/*
void CFont::RenderShadowFont(PUInt8	pBuffer, UInt32 X, UInt32 Y, UInt32 Width, UInt32 High, 
							UInt32 Pitch, UInt32 Flag, D3DCOLOR pFontColor, D3DCOLOR pShadowColor)
{
	PUInt8	ColorByte = reinterpret_cast<PUInt8>(&pShadowColor);

	
	
	UInt32	OffsetPitch = Pitch + 4;

	pBuffer += OffsetPitch;
	ColorByte = reinterpret_cast<PUInt8>(&pFontColor);
	
	if(m_PixelMode == 2)
	{
		for (UInt32 i = 0; i < m_CharHeight; ++i)
		{
			UInt32	*DWBuffer = (UInt32	*)(pBuffer);
			for (UInt32 j = 0; j < m_CharWidth; ++j)
			{
				UInt32 _vl = m_CharBuffer[j + m_CharPitch * i];

				if(_vl)
				{
					DWBuffer[j] = pShadowColor;
					pBuffer -= OffsetPitch;
					AlphaFontTo8888(pBuffer, j * 4, ColorByte, _vl);
					pBuffer += OffsetPitch;
				}
			}

			pBuffer += Pitch;
		}
	}
	else
	{
		for (UInt32 i = 0; i < m_CharHeight; ++i)
		{
			UInt32	*DWBuffer = (UInt32	*)(pBuffer);
			for (UInt32 j = 0; j < m_CharWidth; ++j)
			{
				UInt32 _vl = ((m_CharBuffer[(j >> 3) + m_CharPitch * i] & (1 << (7 - (j % 8)))) ? 0xFF : 0);

				if(_vl)
				{
					DWBuffer[j] = pShadowColor;
					pBuffer -= OffsetPitch;
					AlphaFontTo8888(pBuffer, j * 4, ColorByte, _vl);
					pBuffer += OffsetPitch;
				}
			}

			pBuffer += Pitch;
		}
	}



}
*/



UInt32 CFont::RenderShadowFont(PUInt8	pBuffer, UInt32 X, UInt32 Y, UInt32 Width, UInt32 Height, 
							UInt32 Pitch, UInt32 Flag, D3DCOLOR Color, D3DCOLOR pShadowColor)
{
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;

	

	UInt32	OffsetPitch = Pitch + 4;
	PUInt8	CharBuffer = m_CharBuffer;
	Int32	CharWidth = m_CharWidth;
	Int32	CharHeight = m_CharHeight;

	if(X + m_CharWidth + 1 > Width)
		return 0;
		

	if(Y + m_CharHeight + 1> Height)
		return 0;
		

	pBuffer += Pitch + 4;

	if(m_PixelMode == 2)
	{
		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			__asm
			{
			pushad

			movd	mm5, Color
			movd	mm6, pShadowColor

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

Label_1:
			lodsb

			cmp		al, 0
			je		Label_2

			
			movzx	ebx, al
			movd	eax, mm5
			bswap	eax
			movzx	edx, al
			sub		edx, ebx
			shr		edx, 8
			add		edx, ebx
			mov		al, dl
			bswap	eax

			cmp			DWORD PTR [edi], 0
			je			Label_3	

			movd		mm0, eax
			movd		mm1, DWORD PTR [edi]
			pxor		mm2, mm2

			PUNPCKLBW	mm0, mm2
			PUNPCKLBW	mm1, mm2
			movq		mm3, mm0

			PUNPCKHWD	mm3, mm3
			PUNPCKHDQ	mm3, mm3
			
			psubw		mm0, mm1
			pmullw		mm0, mm3
			PSRLW		mm0, 8
			paddb		mm1, mm0
		

			Packuswb	mm1, mm1
			
			jmp		Label_3
Label_2:
			add		edi, 4
			jmp		Label_4
Label_3:
			
			movd	edx, mm6
			mov		DWORD PTR [edi], edx
			sub		edi, OffsetPitch
			stosd
			add		edi, OffsetPitch

Label_4:

			loop	Label_1

			popad

			emms
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
	}
	else
	{
		PUInt8	ColorByte = reinterpret_cast<PUInt8>(&Color);;

		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			UInt32	*DWBuffer = (UInt32	*)(pBuffer);
			for (UInt32 j = 0; j < CharWidth; ++j)
			{
				UInt32 _vl = ((m_CharBuffer[(j >> 3) + m_CharPitch * i] & (1 << (7 - (j % 8)))) ? 0xFF : 0);

				if(_vl)
				{
					DWBuffer[j] = pShadowColor;
					pBuffer -= OffsetPitch;
					AlphaFontTo8888(pBuffer, j * 4, ColorByte, _vl);
					pBuffer += OffsetPitch;
				}
			}

			pBuffer += Pitch;
		}

		/*
		for (UInt32 i = 0; i < m_CharHeight; ++i)
		{
			__asm
			{
			pushad

			movd	mm5, Color
			movd	mm6, pShadowColor

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

			lodsb
			mov		bl, al
			mov		bh, 0x80

Label_21:
			and		al, bh
			
			cmp		al, 0
			je		Label_22

			
			movd	eax, mm5
			bswap	eax
			movzx	edx, al
			sub		edx, 0x000000FF
			shr		edx, 8
			add		edx, 0x000000FF
			mov		al, dl
			bswap	eax

			cmp			DWORD PTR [edi], 0
			je			Label_23	

			movd		mm0, eax
			movd		mm1, DWORD PTR [edi]
			pxor		mm2, mm2

			PUNPCKLBW	mm0, mm2
			PUNPCKLBW	mm1, mm2
			movq		mm3, mm0

			PUNPCKHWD	mm3, mm3
			PUNPCKHDQ	mm3, mm3
			
			psubw		mm0, mm1
			pmullw		mm0, mm3
			PSRLW		mm0, 8
			paddb		mm1, mm0
		

			Packuswb	mm1, mm1
			
			movd		eax, mm1
			

			
			jmp		Label_23

Label_26:
			mov		al, bl
			loop	Label_21
			jmp		Label_EndLoop


Label_22:
			add		edi, 4
			jmp		Label_24
Label_23:
			movd	edx, mm6
			mov		DWORD PTR [edi], edx
			sub		edi, OffsetPitch
			stosd
			add		edi, OffsetPitch


Label_24:


			shr		bh, 1
			cmp		bh, 0
			jne		Label_26

			mov		bh, 0x80
			lodsb
			mov		bl, al

			jmp		Label_26

Label_EndLoop:

			popad

			emms
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
		*/
	}

	return 1;
}



UInt32 CFont::RenderEdgeFont(PUInt8 pBuffer, UInt32 X, UInt32 Y, UInt32 Width, UInt32 High, 
							UInt32 Pitch, UInt32 Flag, D3DCOLOR pFontColor, D3DCOLOR pEdgeColor)
{
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;

	PUInt8	ColorByte = reinterpret_cast<PUInt8>(&pFontColor);

	void	(*PlotEageFunc)(UInt32 *, UInt32, UInt32);

	pBuffer = pBuffer + X * 4 + Pitch * Y;
	PUInt8	TempBufferPtr = pBuffer;
	UInt32	SubPitch = Pitch / 4;

	PlotEageFunc = ((Flag == EDGE_FONT2) ? PlotEage2 : PlotEage);


	Flag &= 1;

	
	if(m_PixelMode == 2)
	{
		for (UInt32 i = 0; i < m_CharHeight; ++i)
		{
			for (UInt32 j = 0; j < m_CharWidth; ++j)
			{
				UInt32 _vl = GetFontPixelMode2(m_CharBuffer, j, i, m_CharPitch);

				if(_vl)
				{
					(*PlotEageFunc)(reinterpret_cast<UInt32 *>(pBuffer + j * 4), SubPitch, pEdgeColor);
				}
			}

			pBuffer += Pitch;
		}
	}
	else
	{
		for (UInt32 i = 0; i < m_CharHeight; ++i)
		{
			for (UInt32 j = 0; j < m_CharWidth; ++j)
			{
				UInt32 _vl = GetFontPixel(m_CharBuffer, j, i, m_CharPitch);

				if(_vl)
				{
					(*PlotEageFunc)(reinterpret_cast<UInt32 *>(pBuffer + j * 4), SubPitch, pEdgeColor);
				}
			}

			pBuffer += Pitch;
		}

	}
	

	pBuffer = TempBufferPtr;

	PUInt8	CharBuffer = m_CharBuffer;
	UInt32	CharWidth = m_CharWidth;
	UInt32	CharHeight = m_CharHeight;

	if(m_PixelMode == 2)
	{
		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			
			__asm
			{
			pushad

			movd		mm5, pFontColor

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

Label_1:
			lodsb

			cmp		al, 0
			je		Label_2

			
			movzx	ebx, al
			movd	eax, mm5
			bswap	eax
			movzx	edx, al
			sub		edx, ebx
			shr		edx, 8
			add		edx, ebx
			mov		al, dl
			bswap	eax

			cmp			DWORD PTR [edi], 0
			je			Label_3	

			movd		mm0, eax
			movd		mm1, DWORD PTR [edi]
			pxor		mm2, mm2

			PUNPCKLBW	mm0, mm2
			PUNPCKLBW	mm1, mm2
			movq		mm3, mm0

			PUNPCKHWD	mm3, mm3
			PUNPCKHDQ	mm3, mm3
			
			psubw		mm0, mm1
			pmullw		mm0, mm3
			PSRLW		mm0, 8
			paddb		mm1, mm0
		

			Packuswb	mm1, mm1
			
			movd		eax, mm1
			
			

			jmp		Label_3
Label_2:
			add		edi, 4
			jmp		Label_4
Label_3:
			stosd

Label_4:
			loop	Label_1

			popad

			emms
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
	}
	else
	{
		for (UInt32 i = 0; i < CharHeight; ++i)
		{
			__asm
			{
			pushad

			movd	mm5, pFontColor

			mov		ecx, CharWidth

			mov		edi, pBuffer
			mov		esi, CharBuffer

			lodsb
			mov		bl, al
			mov		bh, 0x80

Label_21:
			and		al, bh
			
			cmp		al, 0
			je		Label_22

			
			movd	eax, mm5
			bswap	eax
			movzx	edx, al
			sub		edx, 0x000000FF
			shr		edx, 8
			add		edx, 0x000000FF
			mov		al, dl
			bswap	eax

			cmp			DWORD PTR [edi], 0
			je			Label_23	

			movd		mm0, eax
			movd		mm1, DWORD PTR [edi]
			pxor		mm2, mm2

			PUNPCKLBW	mm0, mm2
			PUNPCKLBW	mm1, mm2
			movq		mm3, mm0

			PUNPCKHWD	mm3, mm3
			PUNPCKHDQ	mm3, mm3
			
			psubw		mm0, mm1
			pmullw		mm0, mm3
			PSRLW		mm0, 8
			paddb		mm1, mm0
		

			Packuswb	mm1, mm1
		
			
			movd		eax, mm1
			
			
			jmp		Label_23
Label_22:
			add		edi, 4
			jmp		Label_24
Label_23:
			stosd

Label_24:
			shr		bh, 1
			cmp		bh, 0
			jne		Label_26

			mov		bh, 0x80
			lodsb
			mov		bl, al
Label_26:
			mov		al, bl


			loop	Label_21

			popad

			emms
			}

			pBuffer += Pitch;
			CharBuffer += m_CharPitch;
		}
	}


	
	/*
	for (UInt32 i = 0; i < m_CharHeight; ++i)
	{
		for (UInt32 j = 0; j < m_CharWidth; ++j)
		{
			UInt32 _vl = (*FontPixelFunc)(m_CharBuffer, j, i, m_CharPitch);

			if(_vl)
			{
				AlphaFontTo8888A(pBuffer + j * 4, pFontColor, _vl);
			}
		}

		pBuffer += Pitch;
	}
	*/

	return 1;
}




UInt32	CFont::GetCharWidth(CharW c)
{
	/*
	if(c == _T(' '))
	{
		return m_Width / 2;
	}
	*/
	
	/*
	FT_UInt glyph_index = FT_Get_Char_Index(m_FontFace, c);
	FT_Load_Glyph(m_FontFace, glyph_index, FT_LOAD_RENDER); 
	FT_Bitmap &bitmap = m_FontFace->glyph->bitmap;
	*/

	GetFontChar(c);//bitmap.width + m_FontFace->glyph->bitmap_left;

	return m_AdvanceX;
}


bool CFont::Check(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight)
{
	if(strcmp(m_FontsName.c_str(), pName))
		return false;
	if(pWidth != m_Width)
		return false;
	if(pHeight != m_Height)
		return false;

	return true;
}



void CFont::SetSel(UInt32 Start, UInt32 End)
{
	if(Start < End)
	{
		m_SelStart = Start;
		m_SelEnd = End;
	}
	else
	{
		m_SelStart = End;
		m_SelEnd = Start;
	}
}


////////////////////////////////////////////////////////////////




Int32 CFontManager::InitFonts()
{
	m_FTError = FT_Init_FreeType(&m_FTLibrary);
	if (m_FTError)
		return -1;
	
	m_FontList.clear();

	return 1;
}


void CFontManager::ClearFonts()
{
	for(UInt32 i = 0; i < m_FontList.size(); ++i)
	{
		SafeDeleteObject(m_FontList[i]);
	}
	m_FontList.clear();
	FT_Done_FreeType(m_FTLibrary);
}


CFont *CFontManager::FindFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight)
{
	for(UInt32 i = 0; i < m_FontList.size(); ++i)
	{
		if(m_FontList[i]->Check(pName, pWidth, pHeight))
		{
			return m_FontList[i];
		}
	}

	return NULL;
}


CFont *CFontManager::DeleteFont(CFont *pFont)
{
	std::vector<CFont *>::iterator Pos = find(m_FontList.begin(), m_FontList.end(), pFont);

	if(Pos != m_FontList.end())
	{
		m_FontList.erase(Pos);
		return NULL;
	}

	return pFont;
}


CFont *CFontManager::CreateFont(PCSTRINGA pName, UInt32 pWidth, UInt32 pHeight)
{
	for(std::vector<CFont *>::iterator i = m_FontList.begin(); i != m_FontList.end(); ++i)
	{
		if((*i)->Check(pName, pWidth, pHeight))
		{
			return *i;
		}
	}

	CFont *TempFont = SafeCreateObject3(CFont, pName, pWidth, pHeight);

	m_FontList.push_back(TempFont);

	return TempFont;
}

