//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "Image.h"


template<Int32 Code>
inline UInt32 ChangeTo8888(UInt32	Pixel)
{
	return Pixel;
}


template <>
inline UInt32 ChangeTo8888<565>(UInt32 Pixel)
{	
	UInt32 Pixel1 = (0xF0000000 | 
			((Pixel & 0x0000F000) << 8) |
			((Pixel & 0x00000780) << 5) |
			((Pixel & 0x0000001E) << 3));

	return Pixel1 | (Pixel1 >> 4);
}


template <>
inline UInt32 ChangeTo8888<4444>(UInt32 Pixel)
{	
	UInt32 Pixel1 = (((Pixel & 0xF000) << 16) |
		((Pixel & 0x0F00) << 12) |
		((Pixel & 0x00F0) <<  8) |
		((Pixel & 0x000F) <<  4));

	return Pixel1 | (Pixel1 >> 4);
}


template<Int32 Code>
inline  UInt32 ChangeTo4444(UInt32	Pixel)
{
	return Pixel;
}


template <>
inline UInt32 ChangeTo4444<565>(UInt32	Pixel)
{	
	UInt32 Pixel1 = (0xF0000000 | 
			((Pixel & 0x0000F000) << 8) |
			((Pixel & 0x00000780) << 5) |
			((Pixel & 0x0000001E) << 3));

	return Pixel1 | (Pixel1 >> 4);
}

	
template <>
inline UInt32 ChangeTo4444<8888>(UInt32	Pixel)
{
	return (((Pixel & 0xF0000000) >> 16) |
			((Pixel & 0x00F00000) >> 12) |
			((Pixel & 0x0000F000) >> 8) |
			((Pixel & 0x000000F0) >> 4));
}



template<Int32 Code>
inline UInt32 ChangeTo565(UInt32	Pixel)
{
	return Pixel;
}


template <>
inline UInt32 ChangeTo565<4444>(UInt32 Pixel)
{	
	return (
			((Pixel & 0x0F00) << 4) |
			((Pixel & 0x00F0) << 3) |
			((Pixel & 0x000F) << 1));
}

	
template <>
inline UInt32 ChangeTo565<8888>(UInt32 Pixel)
{
	return (((Pixel & 0x00F00000) >> 8) |
			((Pixel & 0x0000F000) >> 5) |
			((Pixel & 0x000000F0) >> 3));
}




template <Int32 Code>
inline UInt32 Change8888To(UInt32	Pixel)
{
	return Pixel;
}


template <>
inline UInt32 Change8888To<565>(UInt32	Pixel)
{
	return ((Pixel & 0x00F00000) >> 8) |
			((Pixel & 0x0000F000) >> 5) |
			((Pixel & 0x000000F0) >> 3);
}


template <>
inline UInt32 Change8888To<4444>(UInt32	Pixel)
{
	return (((Pixel & 0xF0000000) >> 16) |
			((Pixel & 0x00F00000) >> 12) |
			((Pixel & 0x0000F000) >> 8) |
			((Pixel & 0x000000F0) >> 4));
}



template <Int32 Code>
inline UInt32 DetectAlpha(UInt32 Pixel)
{
	return 1;
}

template <>
inline UInt32 DetectAlpha<4444>(UInt32 Pixel)
{
	return (Pixel & 0xF000);
}


template <>
inline UInt32 DetectAlpha<8888>(UInt32 Pixel)
{
	return (Pixel & 0xFF000000);
}


template <Int32 Code>
inline bool CheckAlpha(UInt32 Flag)
{
	return 1;
}

template <>
inline bool CheckAlpha<4444>(UInt32 Flag)
{
	return (Flag == 0xF000);
}

template <>
inline bool CheckAlpha<8888>(UInt32 Flag)
{
	return (Flag == 0xFF000000);
}


inline UInt32 MMXAlpha(UInt32 pSrcData, UInt32 pDesData)
{
	const __int64		T1 = 0x0001000100010001;
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;
	
	__asm
	{
		movd		mm0, pSrcData
		movd		mm1, pDesData
		pxor		mm2, mm2

		PUNPCKLBW	mm0, mm2
		PUNPCKLBW	mm1, mm2
		movq		mm3, mm0
		PUNPCKHWD	mm3, mm3
		PUNPCKHDQ	mm3, mm3

		movq		mm4, mm0
		movq		mm5, mm0

		pminsw		mm4, mm1
		pmaxsw		mm5, mm1

		psubw		mm0, mm1
		paddw		mm3, T1
		pmullw		mm0, mm3
		PSRLW		mm0, 8
		paddb		mm1, mm0

		pmullw		mm4, mm5
		PSRLW		mm4, 8
		paddusb		mm4, mm5

		PAND		mm4, T3
		PAND		mm1, T2
		por			mm1, mm4
		
		Packuswb	mm1, mm1
		
		movd	pDesData, mm1
		
		emms
	}

	return pDesData;
}

inline void Alpha32Pixel(UInt32 &SPixel, UInt32 &DPixel)
{
	DPixel = MMXAlpha(SPixel, DPixel);

	/*
	PUInt8SPixelPtr = reinterpret_cast<PUInt8>(&SPixel);
	PUInt8DPixelPtr = reinterpret_cast<PUInt8>(&DPixel);

	DPixelPtr[0] = static_cast<UInt8>((SPixelPtr[3] * (SPixelPtr[0] - DPixelPtr[0]) >> 8 ) + DPixelPtr[0]); //B
	DPixelPtr[1] = static_cast<UInt8>((SPixelPtr[3] * (SPixelPtr[1] - DPixelPtr[1]) >> 8 ) + DPixelPtr[1]); //G
	DPixelPtr[2] = static_cast<UInt8>((SPixelPtr[3] * (SPixelPtr[2] - DPixelPtr[2]) >> 8 ) + DPixelPtr[2]); //R
	DPixelPtr[3] = static_cast<UInt8>(((SPixelPtr[3] - DPixelPtr[3]) >> 8 ) + DPixelPtr[3]); //A
	*/
}


inline UInt32 AdditivePixel(UInt32 SPixel, UInt32 DPixel)
{
	__asm
	{
		movd	mm0, DPixel
		mov		eax, SPixel
		and		eax, 0x00FFFFFF
		movd	mm1, eax

		paddusb	mm0, mm1

		movd	DPixel, mm0

		emms
	}

	return DPixel;
}


inline UInt32 SubtractivePixel(UInt32 SPixel, UInt32 DPixel)
{
	__asm
	{
		movd	mm0, DPixel
		mov		eax, SPixel
		and		eax, 0x00FFFFFF
		movd	mm1, eax

		psubusb	mm0, mm1

		movd	DPixel, mm0

		emms
	}

	return DPixel;
}



enum BlitOption
{
	_Copy,
	_AlphaBlending,
	_Additive,
	_Subtractive
};




template <typename SRCTYPE, typename DESTYPE, Int32 SRCFORMAT, Int32 DESFORMAT, BlitOption Option>
void BlitLoop(PUInt8 SPtr, PUInt8 DPtr, UInt32 Width, UInt32 High, UInt32 SPitch, UInt32 DPitch)
{
	for(UInt32 i = 0; i < High; ++i)
	{
		if(SRCFORMAT == 565 && DESFORMAT == 565)
		{
			memcpy(DPtr, SPtr, Width * 2);
		}
		else
		if(SRCFORMAT == 4444 && DESFORMAT == 4444 && Option == _Copy)
		{
			memcpy(DPtr, SPtr, Width * 2);
		}
		else
		if(SRCFORMAT == 8888 && DESFORMAT == 8888 && Option == _Copy)
		{
			memcpy(DPtr, SPtr, Width * 4);
		}
		else
		{
			SRCTYPE *SLine = reinterpret_cast<SRCTYPE *>(SPtr);
			DESTYPE *DLine = reinterpret_cast<DESTYPE *>(DPtr);

			for(UInt32 j = 0; j < Width; j++)
			{
				if(Option == _Copy || (Option == _AlphaBlending && SRCFORMAT == 565))
				{
					if(DESFORMAT == 8888)
					{

						DLine[j] = static_cast<DESTYPE>(ChangeTo8888<SRCFORMAT>(SLine[j]));
					}
					else
					if(DESFORMAT == 4444)
					{
						DLine[j] = static_cast<DESTYPE>(ChangeTo4444<SRCFORMAT>(SLine[j]));
					}
					else
					if(DESFORMAT == 565)
					{
						DLine[j] = static_cast<DESTYPE>(ChangeTo565<SRCFORMAT>(SLine[j]));
					}
				}
				else
				if(Option == _AlphaBlending)
				{
					UInt32	STempPixel = ChangeTo8888<SRCFORMAT>(SLine[j]);
					UInt32	DTempPixel = ChangeTo8888<DESFORMAT>(DLine[j]);
					UInt32	SOpAlpha = DetectAlpha<SRCFORMAT>(SLine[j]);
					UInt32	DOpAlpha = DetectAlpha<DESFORMAT>(DLine[j]);

					
					if(SOpAlpha)
					{
						if(CheckAlpha<SRCFORMAT>(SOpAlpha) && CheckAlpha<DESFORMAT>(DOpAlpha))
						{
							
							if(SRCFORMAT == DESFORMAT)
							{
								DLine[j] = static_cast<DESTYPE>(SLine[j]);
							}
							else
							if(DESFORMAT == 8888)
							{
								DLine[j] = static_cast<DESTYPE>(STempPixel);

							}
							else
							{
								DLine[j] = static_cast<DESTYPE>(Change8888To<DESFORMAT>(STempPixel));
							}
							
						}
						else
						{
							Alpha32Pixel(STempPixel, DTempPixel);
							DLine[j] = static_cast<DESTYPE>(Change8888To<DESFORMAT>(DTempPixel));
						}
					}
				}
				else
				if(Option == _Additive)
				{
					//if(SLine[j])
					{
						UInt32	STempPixel = ChangeTo8888<SRCFORMAT>(SLine[j]);
						UInt32	DTempPixel = ChangeTo8888<DESFORMAT>(DLine[j]);

						DLine[j] = static_cast<DESTYPE>(Change8888To<DESFORMAT>(AdditivePixel(STempPixel, DTempPixel)));
					}
				}
				else
				if(Option == _Subtractive)
				{
					//if(SLine[j])
					{
						UInt32	STempPixel = ChangeTo8888<SRCFORMAT>(SLine[j]);
						UInt32	DTempPixel = ChangeTo8888<DESFORMAT>(DLine[j]);

						DLine[j] = static_cast<DESTYPE>(Change8888To<DESFORMAT>(SubtractivePixel(STempPixel, DTempPixel)));
					}
				}
			}
		}

		SPtr += SPitch;
		DPtr += DPitch;
	}

}




template <Int32 SRCFORMAT, Int32 DESFORMAT, BlitOption Option>
UInt32 TranFunc(UInt32 X, UInt32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	D3DLOCKED_RECT SLockedRect;
	D3DLOCKED_RECT DLockedRect;

	UInt32 Width  = lpSRect->right - lpSRect->left;
	UInt32 High   = lpSRect->bottom - lpSRect->top;

	RECT  DRect = {X, Y, X + Width, Y + High};

	STexture->LockRect(0, &SLockedRect, lpSRect, D3DLOCK_NO_DIRTY_UPDATE);
	DTexture->LockRect(0, &DLockedRect, &DRect, D3DLOCK_NO_DIRTY_UPDATE);

	if(SRCFORMAT == 8888)
	{
		if(DESFORMAT == 8888)
		{
			BlitLoop<UInt32, UInt32, SRCFORMAT, DESFORMAT, Option>
				(reinterpret_cast<PUInt8>(SLockedRect.pBits), reinterpret_cast<PUInt8>(DLockedRect.pBits), Width, High, SLockedRect.Pitch, DLockedRect.Pitch);
		}
		else
		{
			BlitLoop<UInt32, UInt16, SRCFORMAT, DESFORMAT, Option>
				(reinterpret_cast<PUInt8>(SLockedRect.pBits), reinterpret_cast<PUInt8>(DLockedRect.pBits), Width, High, SLockedRect.Pitch, DLockedRect.Pitch);
		}
	}
	else
	{
		if(DESFORMAT == 8888)
		{
			BlitLoop<UInt16, UInt32, SRCFORMAT, DESFORMAT, Option>
				(reinterpret_cast<PUInt8>(SLockedRect.pBits), reinterpret_cast<PUInt8>(DLockedRect.pBits), Width, High, SLockedRect.Pitch, DLockedRect.Pitch);
		}
		else
		{
			BlitLoop<UInt16, UInt16, SRCFORMAT, DESFORMAT, Option>
				(reinterpret_cast<PUInt8>(SLockedRect.pBits), reinterpret_cast<PUInt8>(DLockedRect.pBits), Width, High, SLockedRect.Pitch, DLockedRect.Pitch);
		}
		
	}
	
	
	STexture->UnlockRect(0);
	DTexture->UnlockRect(0);
	
	return 1;
}





template <Int32 SRCFORMAT, Int32 DESFORMAT, BlitOption Option>
UInt32 TranImageFunc(UInt32 X, UInt32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	UInt32 Width  = lpSRect->right - lpSRect->left;
	UInt32 High   = lpSRect->bottom - lpSRect->top;

	PUInt8   SpBits = pSImage->GetPtrXY(lpSRect->left, lpSRect->top);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	if(SRCFORMAT == 8888)
	{
		if(DESFORMAT == 8888)
		{
			BlitLoop<UInt32, UInt32, SRCFORMAT, DESFORMAT, Option>
				(SpBits, DpBits, Width, High, pSImage->GetPitch(), pDImage->GetPitch());
		}
		else
		{
			BlitLoop<UInt32, UInt16, SRCFORMAT, DESFORMAT, Option>
				(SpBits, DpBits, Width, High, pSImage->GetPitch(), pDImage->GetPitch());
		}
	}
	else
	{
		if(DESFORMAT == 8888)
		{
			BlitLoop<UInt16, UInt32, SRCFORMAT, DESFORMAT, Option>
				(SpBits, DpBits, Width, High, pSImage->GetPitch(), pDImage->GetPitch());
		}
		else
		{
			BlitLoop<UInt16, UInt16, SRCFORMAT, DESFORMAT, Option>
				(SpBits, DpBits, Width, High, pSImage->GetPitch(), pDImage->GetPitch());
		}
		
	}
	

	return 1;
}







UInt32 Alpha565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 8888, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 8888, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 8888, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 4444, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 4444, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 4444, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 565, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 565, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Alpha8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 565, _AlphaBlending>(X, Y, STexture, DTexture, lpSRect);
}



//////////////////////


UInt32 Copy565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 8888, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Copy4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 8888, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Copy8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 8888, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Copy565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 4444, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Copy4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 4444, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Copy8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 4444, _Copy>(X, Y, STexture, DTexture, lpSRect);
}



UInt32 Copy565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 565, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Copy4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 565, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Copy8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 565, _Copy>(X, Y, STexture, DTexture, lpSRect);
}


//////////////////////


UInt32 Additive565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 8888, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Additive4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 8888, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Additive8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 8888, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Additive565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 4444, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Additive4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 4444, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Additive8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 4444, _Additive>(X, Y, STexture, DTexture, lpSRect);
}



UInt32 Additive565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 565, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Additive4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 565, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Additive8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 565, _Additive>(X, Y, STexture, DTexture, lpSRect);
}


//////////////////////


UInt32 Subtractive565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 8888, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 8888, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 8888, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 4444, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 4444, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 4444, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<565, 565, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<4444, 565, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}


UInt32 Subtractive8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect)
{
	return TranFunc<8888, 565, _Subtractive>(X, Y, STexture, DTexture, lpSRect);
}



//////////////////////////////////////////////////////////////////////////////////////////////

UInt32 Alpha565To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	//return TranImageFunc<565, 8888, _AlphaBlending>(X, Y, pSImage, pDImage, lpSRect);
	return Copy565To8888(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Alpha4444To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	UInt32 Width  = lpSRect->right - lpSRect->left;
	UInt32 High   = lpSRect->bottom - lpSRect->top;

	PUInt8   SpBits = pSImage->GetPtrXY(lpSRect->left, lpSRect->top);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	for(UInt32 i = 0; i < High; ++i)
	{
		UInt16	*SLine = reinterpret_cast<UInt16 *>(SpBits);
		UInt32	*DLine = reinterpret_cast<UInt32 *>(DpBits);

		for(UInt32 j = 0; j < Width; j++)
		{
			UInt32	STempPixel = ChangeTo8888<4444>(SLine[j]);
			UInt32	SOpAlpha = DetectAlpha<8888>(STempPixel);
			UInt32	DOpAlpha = DetectAlpha<8888>(DLine[j]);

			if(SOpAlpha)
			{
				if(CheckAlpha<8888>(SOpAlpha) && CheckAlpha<8888>(DOpAlpha))
					DLine[j] = STempPixel;
				else
					Alpha32Pixel(STempPixel, DLine[j]);
			}
		}

		SpBits += pSImage->GetPitch();
		DpBits += pDImage->GetPitch();
	}

	return 1;
}

UInt32 Liner8888To8888(Int32 X, Int32 Y, UInt32 Width, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	Int32 Height = lpSRect->bottom - lpSRect->top;
	Int32 WidthLocal = Width;
	Int32 X2 = lpSRect->left, Y2 = lpSRect->top;

	if(X < 0)
	{
		WidthLocal += X;
		X2 -= X;
		X = 0;
	}

	if(Y < 0)
	{
		Height += Y;
		Y2 -= Y;
		Y = 0;
	}

	if(X + WidthLocal > (Int32)pDImage->GetWidth())
	{
		WidthLocal -= X + WidthLocal - pDImage->GetWidth();
	}

	if(Y + Height > (Int32)pDImage->GetHeight())
	{
		Height -= Y + Height - pDImage->GetHeight();
	}

	if( Height <= 0 || WidthLocal <= 0 ) // height/width validation
		return 1;

	PUInt8   SpBits = pSImage->GetPtrXY(lpSRect->left, lpSRect->top);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	for(Int32 i = 0; i < Height; ++i)
	{
		mem4set(DpBits, *(UInt32 *)(SpBits), WidthLocal);

		SpBits += pSImage->GetPitch();
		DpBits += pDImage->GetPitch();
	}

	return 1;
}


UInt32 Liner8888To8888V(Int32 X, Int32 Y, UInt32 Height, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	Int32 Width = lpSRect->right - lpSRect->left;
	Int32 HeightLocal = Height;
	Int32 X2 = lpSRect->left, Y2 = lpSRect->top;

	if(X < 0)
	{
		Width += X;
		X2 -= X;
		X = 0;
	}

	if(Y < 0)
	{
		HeightLocal += Y;
		Y2 -= Y;
		Y = 0;
	}

	if(X + Width > (Int32)pDImage->GetWidth())
	{
		Width -= X + Width - pDImage->GetWidth();
	}

	if(Y + HeightLocal > (Int32)pDImage->GetHeight())
	{
		HeightLocal -= Y + HeightLocal - pDImage->GetHeight();
	}

	if( HeightLocal <= 0 || Width <= 0 ) // height/width validation
		return 1;

	PUInt8   SpBits = pSImage->GetPtrXY(lpSRect->left, lpSRect->top);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	
	RECT	rect;

	SetRect(&rect, X, Y, X + Width, Y + HeightLocal);
	pDImage->FillUInt32(0xFF000000, &rect);

	for(UInt32 i = 0; i < Height; ++i)
	{
		mem4cpy(DpBits, SpBits, Width);
		DpBits += pDImage->GetPitch();
	}
	

	return 1;
}



UInt32 Alpha8888To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	if (pSImage == NULL || pDImage == NULL)
		return 0;

	const __int64		T1 = 0x0001000100010001;
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;

	Int32 Width  = lpSRect->right - lpSRect->left;
	Int32 Height = lpSRect->bottom - lpSRect->top;
	Int32 X2 = lpSRect->left, Y2 = lpSRect->top;

	if(X < 0)
	{
		Width += X;
		X2 -= X;
		X = 0;
		
	}

	if(Y < 0)
	{
		Height += Y;
		Y2 -= Y;
		Y = 0;
	}

	if(X + Width > (Int32)pDImage->GetWidth())
	{
		Width -= X + Width - pDImage->GetWidth();
	}

	if(Y + Height > (Int32)pDImage->GetHeight())
	{
		Height -= Y + Height - pDImage->GetHeight();
	}

	if( Height <= 0 || Width <= 0 ) // height/width validation
		return 0;

	PUInt8   SpBits = pSImage->GetPtrXY(X2, Y2);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	UInt32	SPitch = pSImage->GetPitch() - Width * 4;
	UInt32	DPitch = pDImage->GetPitch() - Width * 4;

	/*
	for(UInt32 i = 0; i < Height; ++i)
	{
		UInt32 *SLine = reinterpret_cast<UInt32 *>(SpBits);
		UInt32 *DLine = reinterpret_cast<UInt32 *>(DpBits);

		for(UInt32 j = 0; j < Width; j++)
		{
			UInt32	SOpAlpha = DetectAlpha<8888>(SLine[j]);
			UInt32	DOpAlpha = DetectAlpha<8888>(DLine[j]);

			if(SOpAlpha)
			{
				if(CheckAlpha<8888>(SOpAlpha) || DOpAlpha == 0)
					DLine[j] = SLine[j];
				else
					Alpha32Pixel(SLine[j], DLine[j]);
			}
		}
		SpBits += SPitch;
		DpBits += DPitch;
	}
	*/

	__try
	{
	__asm
	{
		pushad

		pxor	mm2, mm2

		mov		ecx, Height

		mov		esi, SpBits
		mov		edi, DpBits

Label_0:
		push	ecx
		mov		ecx, Width

Label_1:

		lodsd
		mov		edx, DWORD PTR [edi]

		bswap	edx
		cmp		dl, 0
		bswap	edx
		je		Label_Copy
		
		movd		mm0, eax
		movd		mm1, edx
		

		PUNPCKLBW	mm0, mm2
		PUNPCKLBW	mm1, mm2
		movq		mm3, mm0
		PUNPCKHWD	mm3, mm3
		PUNPCKHDQ	mm3, mm3

		movq		mm4, mm0
		movq		mm5, mm0

		pminsw		mm4, mm1
		pmaxsw		mm5, mm1

		psubw		mm0, mm1
		paddw		mm0, T1
		pmullw		mm0, mm3
		PSRLW		mm0, 8
		paddb		mm1, mm0

		paddw		mm4, T1
		pmullw		mm4, mm5
		PSRLW		mm4, 8
		paddusb		mm4, mm5

		PAND		mm4, T3
		PAND		mm1, T2
		por			mm1, mm4
		
		Packuswb	mm1, mm1
		
		movd		eax, mm1

Label_Copy:


		stosd


		loop	Label_1

//Label_OK:

		pop		ecx

		add		esi, SPitch
		add		edi, DPitch

		loop	Label_0
//Label_OK2:

		popad

		emms
	}

	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE_ERRORDTL_2(GLOBAL_LOGGER, _F("Alpha8888To8888 [Exception] pSImage = %d, pDImage = %d"), pSImage, pDImage);
	}

	return 1;
}


UInt32 Alpha8888To8888C(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect, UInt32 Color)
{
	const __int64		T1 = 0x0001000100010001;
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;

	Int32 Width  = lpSRect->right - lpSRect->left;
	Int32 Height = lpSRect->bottom - lpSRect->top;
	Int32 X2 = lpSRect->left, Y2 = lpSRect->top;

	if(X < 0)
	{
		Width += X;
		X2 -= X;
		X = 0;
		
	}

	if(Y < 0)
	{
		Height += Y;
		Y2 -= Y;
		Y = 0;
	}

	if(X + Width > (Int32)pDImage->GetWidth())
	{
		Width -= X + Width - pDImage->GetWidth();
	}

	if(Y + Height > (Int32)pDImage->GetHeight())
	{
		Height -= Y + Height - pDImage->GetHeight();
	}

	if( Height <= 0 || Width <= 0 ) // height/width validation
		return 0;

	PUInt8   SpBits = pSImage->GetPtrXY(X2, Y2);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	UInt32	SPitch = pSImage->GetPitch() - Width * 4;
	UInt32	DPitch = pDImage->GetPitch() - Width * 4;

	__asm
	{
		pushad

		pxor		mm2, mm2
		movq		mm6, Color
		PUNPCKLBW	mm6, mm2

		mov		ecx, Height

		mov		esi, SpBits
		mov		edi, DpBits

Label_0:
		push	ecx
		mov		ecx, Width

Label_1:

		lodsd
		mov		edx, DWORD PTR [edi]

		movd		mm0, eax
		movd		mm1, edx

		PUNPCKLBW	mm0, mm2
		PUNPCKLBW	mm1, mm2

		paddw		mm0, T1
		pmullw		mm0, mm6
		PSRLW		mm0, 8

		bswap	edx
		cmp		dl, 0
		bswap	edx
		je		Label_Copy


		movq		mm3, mm0
		PUNPCKHWD	mm3, mm3
		PUNPCKHDQ	mm3, mm3

		

		movq		mm4, mm0
		movq		mm5, mm0

		pminsw		mm4, mm1
		pmaxsw		mm5, mm1

		psubw		mm0, mm1
		paddw		mm0, T1
		pmullw		mm0, mm3
		PSRLW		mm0, 8
		paddb		mm0, mm1

		paddw		mm4, T1
		pmullw		mm4, mm5
		PSRLW		mm4, 8
		paddusb		mm4, mm5

		PAND		mm4, T3
		PAND		mm0, T2
		por			mm0, mm4

Label_Copy:
		
		Packuswb	mm0, mm0
		movd		eax, mm0

		stosd

		loop	Label_1

//Label_OK:

		pop		ecx

		add		esi, SPitch
		add		edi, DPitch

		loop	Label_0
//Label_OK2:

		popad

		emms
	}




	return 1;
}



UInt32 Alpha8888To8888BW(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	const __int64		T1 = 0x0001000100010001;
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;
	const __int64		T4 = 85;

	Int32 Width  = lpSRect->right - lpSRect->left;
	Int32 Height = lpSRect->bottom - lpSRect->top;
	Int32 X2 = lpSRect->left, Y2 = lpSRect->top;

	if(X < 0)
	{
		Width += X;
		X2 -= X;
		X = 0;
		
	}

	if(Y < 0)
	{
		Height += Y;
		Y2 -= Y;
		Y = 0;
	}

	if(X + Width > (Int32)pDImage->GetWidth())
	{
		Width -= X + Width - pDImage->GetWidth();
	}

	if(Y + Height > (Int32)pDImage->GetHeight())
	{
		Height -= Y + Height - pDImage->GetHeight();
	}

	if( Height <= 0 || Width <= 0 ) // height/width validation
		return 0;

	PUInt8   SpBits = pSImage->GetPtrXY(X2, Y2);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	UInt32	SPitch = pSImage->GetPitch() - Width * 4;
	UInt32	DPitch = pDImage->GetPitch() - Width * 4;


	__asm
	{
		pxor		mm2, mm2

		mov		ecx, Height

		mov		esi, SpBits
		mov		edi, DpBits

Label_0:
		push	ecx
		mov		ecx, Width

Label_1:

		
		lodsd

		movd		mm0, eax
		movd		mm1, DWORD PTR [edi]

		PUNPCKLBW	mm0, mm2
		PUNPCKLBW	mm1, mm2
		movq		mm3, mm0
		PUNPCKHWD	mm3, mm3
		PUNPCKHDQ	mm3, mm3

		movq		mm4, mm0
		movq		mm5, mm0
		PSRLQ		mm5, 16
		paddw		mm4, mm5
		PSRLQ		mm5, 16
		paddw		mm4, mm5
		pmullw		mm4, T4
		PSRLD		mm4, 8

		PUNPCKLWD	mm4, mm4
		PUNPCKLDQ	mm4, mm4
		PAND		mm4, T2
		PAND		mm0, T3
		POR			mm0, mm4
		

		movq		mm4, mm0
		movq		mm5, mm1

		
		pminsw		mm4, mm1
		pmaxsw		mm5, mm1
		

		psubw		mm0, mm1
		paddw		mm0, T1
		pmullw		mm0, mm3
		PSRLW		mm0, 8
		paddb		mm1, mm0

		paddw		mm4, T1
		pmullw		mm4, mm5
		PSRLW		mm4, 8
		paddusb		mm4, mm5

		PAND		mm4, T3
		PAND		mm1, T2
		por			mm1, mm4
		
		Packuswb	mm1, mm1
		
		movd		eax, mm1

		stosd


		//loop	Label_1
		dec		ecx
		jcxz	Label_EndLoop_2
		jmp		Label_1

Label_EndLoop_2:

		pop		ecx

		add		esi, SPitch
		add		edi, DPitch

		dec		ecx
		jcxz	Label_End
		jmp		Label_0

Label_End:

		emms
	}


	return 1;
}


UInt32 Alpha8888To8888P(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect, UInt32 Percent)
{
	const __int64		T1 = 0x0001000100010001;
	const __int64		T2 = 0x0000FFFFFFFFFFFF;
	const __int64		T3 = 0xFFFF000000000000;
	const __int64		T4 = 85;

	Int32 Width  = lpSRect->right - lpSRect->left;
	Int32 Height = lpSRect->bottom - lpSRect->top;
	Int32 X2 = lpSRect->left, Y2 = lpSRect->top;

	if(X < 0)
	{
		Width += X;
		X2 -= X;
		X = 0;
		
	}

	if(Y < 0)
	{
		Height += Y;
		Y2 -= Y;
		Y = 0;
	}

	if(X + Width > (Int32)pDImage->GetWidth())
	{
		Width -= X + Width - pDImage->GetWidth();
	}

	if(Y + Height > (Int32)pDImage->GetHeight())
	{
		Height -= Y + Height - pDImage->GetHeight();
	}

	if( Height <= 0 || Width <= 0 ) // height/width validation
		return 0;

	PUInt8   SpBits = pSImage->GetPtrXY(X2, Y2);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	UInt32	SPitch = pSImage->GetPitch() - Width * 4;
	UInt32	DPitch = pDImage->GetPitch() - Width * 4;


	__asm
	{
		pxor	mm2, mm2
		movq	mm7, Percent
		PSLLW	mm7, 48

		mov		ecx, Height

		mov		esi, SpBits
		mov		edi, DpBits

Label_0:
		push	ecx
		mov		ecx, Width

Label_1:

		
		lodsd

		movd		mm0, eax
		movd		mm1, DWORD PTR [edi]

		PUNPCKLBW	mm0, mm2
		PUNPCKLBW	mm1, mm2
		movq		mm3, mm0
		PUNPCKHWD	mm3, mm3
		PUNPCKHDQ	mm3, mm3

		movq		mm4, mm0

		PAND		mm4, T2
		PAND		mm0, T3
		pmullw		mm4, mm7
		PSRLW		mm4, 8

		POR			mm0, mm4
		

		movq		mm4, mm0
		movq		mm5, mm1

		
		pminsw		mm4, mm1
		pmaxsw		mm5, mm1
		

		psubw		mm0, mm1
		paddw		mm0, T1
		pmullw		mm0, mm3
		PSRLW		mm0, 8
		paddb		mm1, mm0

		paddw		mm4, T1
		pmullw		mm4, mm5
		PSRLW		mm4, 8
		paddusb		mm4, mm5

		PAND		mm4, T3
		PAND		mm1, T2
		por			mm1, mm4
		
		Packuswb	mm1, mm1
		
		movd		eax, mm1

		stosd


		//loop	Label_1
		dec		ecx
		jcxz	Label_EndLoop_2
		jmp		Label_1

Label_EndLoop_2:

		pop		ecx

		add		esi, SPitch
		add		edi, DPitch

		dec		ecx
		jcxz	Label_End
		jmp		Label_0

Label_End:

		emms
	}


	return 1;
}



UInt32 Alpha8888To8888S(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect, UInt32 Style, UInt32 Color)
{
	switch(Style)
	{
	case 0:
		if(Color == 0xFFFFFFFF)
			return Alpha8888To8888(X, Y, pSImage, pDImage, lpSRect);
		else
			return Alpha8888To8888C(X, Y, pSImage, pDImage, lpSRect, Color);
	case 1:
		return Alpha8888To8888BW(X, Y, pSImage, pDImage, lpSRect);
	case 2:
		return Alpha8888To8888P(X, Y, pSImage, pDImage, lpSRect, Color);
	}

	return 0;
}


UInt32 Alpha565To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 4444, _AlphaBlending>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Alpha4444To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 4444, _AlphaBlending>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Alpha8888To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 4444, _AlphaBlending>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Alpha565To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 565, _AlphaBlending>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Alpha4444To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 565, _AlphaBlending>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Alpha8888To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 565, _AlphaBlending>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 BlitAlpha(UInt32 X, UInt32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	switch(pSImage->GetType())
	{
	case 565:
		{
			switch(pDImage->GetType())
			{
			case 565:
				return Alpha565To565(X, Y, pSImage, pDImage, lpSRect);
				break;
			case 4444:
				return Alpha565To4444(X, Y, pSImage, pDImage, lpSRect);
				break;
			case 8888:
				return Alpha565To8888(X, Y, pSImage, pDImage, lpSRect);
				break;
			}
		}
		break;
	case 4444:
		{
			switch(pDImage->GetType())
			{
			case 565:
				return Alpha4444To565(X, Y, pSImage, pDImage, lpSRect);
				break;
			case 4444:
				return Alpha4444To4444(X, Y, pSImage, pDImage, lpSRect);
				break;
			case 8888:
				return Alpha4444To8888(X, Y, pSImage, pDImage, lpSRect);
				break;
			}
		}
		break;
	case 8888:
		{
			switch(pDImage->GetType())
			{
			case 565:
				return Alpha8888To565(X, Y, pSImage, pDImage, lpSRect);
				break;
			case 4444:
				return Alpha8888To4444(X, Y, pSImage, pDImage, lpSRect);
				break;
			case 8888:
				return Alpha8888To8888(X, Y, pSImage, pDImage, lpSRect);
				break;
			}
		}
		break;
	}

	return 0;
}

//////////////////////


UInt32 Copy565To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	UInt32 Width  = lpSRect->right - lpSRect->left;
	UInt32 High   = lpSRect->bottom - lpSRect->top;

	PUInt8   SpBits = pSImage->GetPtrXY(lpSRect->left, lpSRect->top);
	PUInt8   DpBits = pDImage->GetPtrXY(X, Y);

	for(UInt32 i = 0; i < High; ++i)
	{
		UInt16 *SLine = reinterpret_cast<UInt16 *>(SpBits);
		UInt32 *DLine = reinterpret_cast<UInt32 *>(DpBits);

		for(UInt32 j = 0; j < Width; j++)
		{
			DLine[j] = static_cast<UInt32>(ChangeTo8888<565>(SLine[j]));
		}

		SpBits += pSImage->GetPitch();
		DpBits += pDImage->GetPitch();
	}


	return 1;//TranImageFunc<565, 8888, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Copy4444To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 8888, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Copy8888To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 8888, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Copy565To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 4444, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Copy4444To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 4444, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Copy8888To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 4444, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}



UInt32 Copy565To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 565, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Copy4444To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 565, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Copy8888To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 565, _Copy>(X, Y, pSImage, pDImage, lpSRect);
}


//////////////////////


UInt32 Additive565To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 8888, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Additive4444To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 8888, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Additive8888To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 8888, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Additive565To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 4444, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Additive4444To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 4444, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Additive8888To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 4444, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}



UInt32 Additive565To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 565, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Additive4444To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 565, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Additive8888To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 565, _Additive>(X, Y, pSImage, pDImage, lpSRect);
}


//////////////////////


UInt32 Subtractive565To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 8888, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive4444To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 8888, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive8888To8888(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 8888, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive565To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 4444, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive4444To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 4444, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive8888To4444(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 4444, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive565To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<565, 565, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive4444To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<4444, 565, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


UInt32 Subtractive8888To565(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect)
{
	return TranImageFunc<8888, 565, _Subtractive>(X, Y, pSImage, pDImage, lpSRect);
}


//////////////

UInt32 FillTexture(LPDIRECT3DTEXTURE9 STexture, RECT *lpSRect, UInt32 DataLen)
{
	D3DLOCKED_RECT SLockedRect;

	UInt32 Width  = lpSRect->right - lpSRect->left;
	UInt32 High   = lpSRect->bottom - lpSRect->top;
	
	STexture->LockRect(0, &SLockedRect, lpSRect, D3DLOCK_NO_DIRTY_UPDATE);

	PUInt8   SPtr = reinterpret_cast<PUInt8>(SLockedRect.pBits);

	for(UInt32 i = 0; i < High; ++i)
	{
		memset(SPtr, 0, Width * DataLen);

		SPtr += SLockedRect.Pitch;
	}

	STexture->UnlockRect(0);


	
	return 1;
}



/////////////////////////////////////////////////////////////////



void FillUInt32T(LPDIRECT3DTEXTURE9	pTexture, UInt32 pData, LPRECT pSRect)
{
	D3DLOCKED_RECT DLockedRect;

	UInt32 Width  = pSRect->right - pSRect->left;
	UInt32 High   = pSRect->bottom - pSRect->top;

	pTexture->LockRect(0, &DLockedRect, pSRect, D3DLOCK_NO_DIRTY_UPDATE);

	PUInt8	SpBits = (PUInt8)(DLockedRect.pBits) + (pSRect->left + pSRect->top * Width) * 4;

	for(LONG i = pSRect->top; i < pSRect->bottom; ++i)
	{		
		mem4set(SpBits, pData, Width);
		SpBits += DLockedRect.Pitch;
	}

	pTexture->UnlockRect(0);
}


CImage::CImage()
{
	m_Width =
	m_Height =
	m_Pitch =
	m_Len =
	m_Type =
	m_PixelSize =
	m_TextureCode = 0;
	m_Ptr = NULL;

	m_Texture = NULL;

	m_InitCounter = 0;
	m_strFileName = "";

	hFile = hMapFile = 0;
	pInFile = NULL;
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("CImage = %d"), this);
}



CImage::CImage(UInt32 pWidth, UInt32 pHeight, UInt32 pType)
{
	m_InitCounter = 0;
	m_Width = pWidth;
	m_Height = pHeight;
	m_TextureCode = 3;
	m_Type = pType;
	switch(m_Type)
	{
	case 8888:
	case 4444:
		m_Pitch = m_Width * 4;
		m_PixelSize = 4;
		break;
	case 565:
		m_Pitch = m_Width * 2;
		m_PixelSize = 2;
		break;
	default:
		m_Pitch = m_Width * 4;
		m_PixelSize = 4;
		m_Type = 8888;
		break;

	}
	m_Len = m_Pitch * m_Height;
	m_Ptr = SafeAllocate(UInt8, m_Len);
	ZeroMemory(m_Ptr, m_Len);
	hFile = NULL;
	hMapFile = NULL;
	pInFile = NULL;

}


CImage::CImage(PUInt8 BytePtr, UInt32 pWidth, UInt32 pHeight, UInt32 pType)
{
	m_InitCounter = 0;
	m_Width = pWidth;
	m_Height = pHeight;
	m_TextureCode = 2;
	m_Type = pType;
	switch(m_Type)
	{
	case 8888:
	case 4444:
		m_Pitch = m_Width * 4;
		m_PixelSize = 4;
		break;
	case 565:
		m_Pitch = m_Width * 2;
		m_PixelSize = 2;
		break;
	default:
		m_Pitch = m_Width * 4;
		m_PixelSize = 4;
		m_Type = 8888;
		break;

	}
	m_Len = m_Pitch * m_Height;
	m_Ptr = BytePtr;

	hFile = NULL;
	hMapFile = NULL;
	pInFile = NULL;
}


CImage::CImage(LPDIRECT3DTEXTURE9 pTexture)
{
	m_InitCounter = 0;
	D3DSURFACE_DESC SurfaceDesc;

	pTexture->GetLevelDesc(0, &SurfaceDesc);
	switch(SurfaceDesc.Format)
	{
	case D3DFMT_A8R8G8B8:
		m_Type = 8888;
		m_PixelSize = 4;
		break;
	case D3DFMT_A4R4G4B4:
		m_Type = 4444;
		m_PixelSize = 4;
		break;
	case D3DFMT_R5G6B5:
		m_Type = 565;
		m_PixelSize = 2;
		break;
	default:
		memset(this, 0, sizeof(CImage));
		return;
	}

	m_Width = SurfaceDesc.Width;
	m_Height = SurfaceDesc.Height;

	m_TextureCode = 1;
	m_Texture = pTexture;

	D3DLOCKED_RECT LockedRect;
	m_Texture->LockRect(0, &LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
	m_Ptr = reinterpret_cast<PUInt8>(LockedRect.pBits);
	m_Pitch = LockedRect.Pitch;
	m_Len = m_Pitch * m_Height;

	m_Texture->UnlockRect(0);

	hFile = NULL;
	hMapFile = NULL;
	pInFile = NULL;
}



CImage::CImage(LPDIRECT3DTEXTURE9 pTexture, UInt32 Width, UInt32 Height, UInt32 Type, UInt32 Size)
{
	m_InitCounter = 0;
	D3DSURFACE_DESC SurfaceDesc;

	pTexture->GetLevelDesc(0, &SurfaceDesc);
	m_Type = Type;
	m_PixelSize = Size;

	m_Width = SurfaceDesc.Width;
	m_Height = SurfaceDesc.Height;

	m_TextureCode = 1;
	m_Texture = pTexture;

	D3DLOCKED_RECT LockedRect;
	m_Texture->LockRect(0, &LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
	m_Ptr = reinterpret_cast<PUInt8>(LockedRect.pBits);
	m_Pitch = LockedRect.Pitch;
	m_Len = m_Pitch * m_Height;
	m_Texture->UnlockRect(0);

	hFile = NULL;
	hMapFile = NULL;
	pInFile = NULL;
}


	
CImage::CImage(LPDIRECT3DTEXTURE9 pTexture, UInt pIsRef)
{
	m_InitCounter = 0;
	D3DSURFACE_DESC SurfaceDesc;

	pTexture->GetLevelDesc(0, &SurfaceDesc);
	switch(SurfaceDesc.Format)
	{
	case D3DFMT_A8R8G8B8:
		m_Type = 8888;
		m_PixelSize = 4;
		break;
	case D3DFMT_A4R4G4B4:
		m_Type = 4444;
		m_PixelSize = 4;
		break;
	case D3DFMT_R5G6B5:
		m_Type = 565;
		m_PixelSize = 2;
		break;
	default:
		memset(this, 0, sizeof(CImage));
		return;
	}

	m_Width = SurfaceDesc.Width;
	m_Height = SurfaceDesc.Height;

	m_TextureCode = pIsRef;
	if(m_TextureCode == 1)
	{
		m_Texture = pTexture;

		D3DLOCKED_RECT LockedRect;
		m_Texture->LockRect(0, &LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		m_Pitch = LockedRect.Pitch;
		m_Texture->UnlockRect(0);
	}
	else
	{
		D3DLOCKED_RECT LockedRect;
		m_Pitch = m_Width * m_PixelSize;
		m_Len = m_Pitch * m_Height;
		m_Ptr = SafeAllocate(UInt8, m_Len);
	
		pTexture->LockRect(0, &LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		PUInt8 TempByte = reinterpret_cast<PUInt8>(LockedRect.pBits);
		PUInt8 Ptr = m_Ptr;

		for(UInt32 i = 0; i < m_Height; ++i)
		{
			memcpy(Ptr, TempByte, m_Pitch);
			TempByte += LockedRect.Pitch;
			Ptr += m_Pitch;
		}
		pTexture->UnlockRect(0);

	}

	hFile = NULL;
	hMapFile = NULL;
	pInFile = NULL;
}


CImage::CImage(LPDIRECT3DTEXTURE9 pTexture, D3DXIMAGE_INFO *pSrcInfo, UInt pIsRef)
{
	m_InitCounter = 0;
	D3DSURFACE_DESC SurfaceDesc;

	pTexture->GetLevelDesc(0, &SurfaceDesc);
	switch(SurfaceDesc.Format)
	{
	case D3DFMT_A8R8G8B8:
		m_Type = 8888;
		m_PixelSize = 4;
		break;
	case D3DFMT_A4R4G4B4:
		m_Type = 4444;
		m_PixelSize = 4;
		break;
	case D3DFMT_R5G6B5:
		m_Type = 565;
		m_PixelSize = 2;
		break;
	default:
		memset(this, 0, sizeof(CImage));
		return;
	}

	m_Width = pSrcInfo->Width;
	m_Height = pSrcInfo->Height;

	m_TextureCode = pIsRef;
	if(m_TextureCode)
	{
		m_Texture = pTexture;

		D3DLOCKED_RECT LockedRect;
		m_Texture->LockRect(0, &LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		m_Pitch = LockedRect.Pitch;
		m_Texture->UnlockRect(0);
	}
	else
	{
		D3DLOCKED_RECT LockedRect;
		RECT			SRect;


		SRect.top = SRect.left = 0;
		SRect.right = m_Width;
		SRect.bottom = m_Height;
		m_Pitch = m_Width * m_PixelSize;
		m_Len = m_Pitch * m_Height;
		m_Ptr = SafeAllocate(UInt8, m_Len);
	
		pTexture->LockRect(0, &LockedRect, &SRect, D3DLOCK_NO_DIRTY_UPDATE);
		PUInt8 TempByte = reinterpret_cast<PUInt8>(LockedRect.pBits);
		PUInt8 Ptr = m_Ptr;

		for(UInt32 i = 0; i < m_Height; ++i)
		{
			memcpy(Ptr, TempByte, m_Pitch);
			TempByte += LockedRect.Pitch;
			Ptr += m_Pitch;
		}
		pTexture->UnlockRect(0);

	}

	hFile = NULL;
	hMapFile = NULL;
	pInFile = NULL;
}




void CImage::Unlock()
{
	if(m_TextureCode == 1)
	{
		m_Texture->UnlockRect(0);
		m_Ptr = NULL;
		m_Pitch = m_Len = 0;
	}
}

void CImage::Lock()
{
	if(m_TextureCode == 1)
	{
		D3DLOCKED_RECT LockedRect;
	
		m_Texture->LockRect(0, &LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		m_Ptr = reinterpret_cast<PUInt8>(LockedRect.pBits);
		m_Pitch = LockedRect.Pitch;
		m_Len = m_Pitch * m_Height;
	}
}


void CImage::FillUInt16(UInt16 pData, LPRECT pSRect)
{
	UInt32	Width = pSRect->right - pSRect->left;
	PUInt8   SpBits = GetPtrXY(pSRect->left, pSRect->top);

	for(LONG i = pSRect->top; i < pSRect->bottom; ++i)
	{
		memfset(SpBits, pData, Width);
		SpBits += m_Pitch;
	}
}


void CImage::FillUInt32(UInt32 pData, LPRECT pSRect)
{
	Int32	X = pSRect->left;
	Int32	Y = pSRect->top;
	Int32	Width = pSRect->right - pSRect->left;
	Int32	Height = pSRect->bottom - pSRect->top;
	

	if(X < 0)
	{
		Width += X;
		X = 0;
	}

	if(Y < 0)
	{
		Height += Y;
		Y = 0;
	}

	if(X + Width > (Int32)m_Width)
	{
		Width -= X + Width - m_Width;
	}

	if(Y + Height > (Int32)m_Height)
	{
		Height -= Y + Height - m_Height;
	}

	if( Height <= 0 || Width <= 0 ) // height/width validation
		return;

	PUInt8	SpBits = GetPtrXY(X, Y);

	for(Int32 i = 0; i < Height; ++i)
	{		
		mem4set(SpBits, pData, Width);
		SpBits += m_Pitch;
	}
}


void CImage::FillUInt32()
{
	UInt32	Width = m_Width;
	PUInt8	SpBits = GetPtrXY(0, 0);

	for(UInt32 i = 0; i < m_Height; ++i)
	{		
		mem4set(SpBits, 0, Width);
		SpBits += m_Pitch;
	}
}


void CImage::DrawBox(UInt32 pData)
{
	UInt32	Width = m_Width;
	PUInt8	SpBits = GetPtrXY(0, 0);

	for(Int32 i = 0; i < m_Height; ++i)
	{		
		if(i == 0 || i == m_Height - 2)
			mem4set(SpBits, pData, Width);
		else
		{
			PUInt32		Bits32 = (PUInt32)SpBits;
			Bits32[0] = pData;
			Bits32[Width - 10] = pData;
		}
		SpBits += m_Pitch;
	}
}


void CImage::DrawBox(UInt32 pData, LPRECT pSRect)
{
	Int32	X = pSRect->left;
	Int32	Y = pSRect->top;
	Int32	Width = pSRect->right - pSRect->left;
	Int32	Height = pSRect->bottom - pSRect->top;
	

	if(X < 0)
	{
		Width += X;
		X = 0;
	}

	if(Y < 0)
	{
		Height += Y;
		Y = 0;
	}

	if(X + Width > (Int32)m_Width)
	{
		Width -= X + Width - m_Width;
	}

	if(Y + Height > (Int32)m_Height)
	{
		Height -= Y + Height - m_Height;
	}

	if( Height <= 0 || Width <= 0 ) // height/width validation
		return;

	PUInt8	SpBits = GetPtrXY(X, Y);

	for(Int32 i = 0; i < Height; ++i)
	{		
		if(i == 0 || i == Height - 1)
			mem4set(SpBits, pData, Width);
		else
		{
			PUInt32		Bits32 = (PUInt32)SpBits;
			Bits32[0] = pData;
			Bits32[Width - 1] = pData;
		}
		SpBits += m_Pitch;
	}
}


void CImage::ZeroUInt16(UInt16 pData, LPRECT pSRect)
{
	UInt32	Width = pSRect->right - pSRect->left;
	UInt32  High = pSRect->bottom - pSRect->top;
	PUInt8   SpBits = GetPtrXY(pSRect->left, pSRect->top);

	if(Width > m_Width)
		Width = m_Width;

	if(High > m_Height)
		High = m_Height;

	for(UInt32 i = 0; i < High; ++i)
	{
		UInt16	*SLine = reinterpret_cast<UInt16 *>(SpBits);
		for(UInt32 j = 0; j < Width; ++j)
		{
			if(SLine[j] == pData)
				SLine[j] = 0;
		}
		SpBits += m_Pitch;
	}
}


void CImage::ZeroUInt32(UInt32 pData, LPRECT pSRect)
{
	UInt32	Width = pSRect->right - pSRect->left;
	UInt32  High = pSRect->bottom - pSRect->top;
	PUInt8   SpBits = GetPtrXY(pSRect->left, pSRect->top);

	if(Width > m_Width)
		Width = m_Width;

	if(High > m_Height)
		High = m_Height;

	for(UInt32 i = 0; i < High; ++i)
	{
		UInt32	*SLine = reinterpret_cast<UInt32 *>(SpBits);
		for(UInt32 j = 0; j < Width; ++j)
		{
			if(SLine[j] == pData)
				SLine[j] = 0;
		}
		SpBits += m_Pitch;
	}
}


void CImage::FillZeroUInt16(UInt16 pData, LPRECT pSRect)
{
	UInt32	Width = pSRect->right - pSRect->left;
	PUInt8   SpBits = GetPtrXY(pSRect->left, pSRect->top);

	for(LONG i = pSRect->top; i < pSRect->bottom; ++i)
	{
		UInt16	*SLine = reinterpret_cast<UInt16 *>(SpBits);
		for(UInt32 j = 0; j < Width; ++j)
		{
			if(SLine[j] == 0)
				SLine[j] = pData;
		}
		SpBits += m_Pitch;
	}
}



void CImage::FillZeroUInt32(UInt32 pData, LPRECT pSRect)
{
	UInt32	Right = pSRect->right;
	UInt32	Bottom = pSRect->bottom;

	if(pSRect->left >= m_Width || pSRect->right > m_Width ||
		pSRect->top >= m_Height || pSRect->bottom > m_Height ||
		pSRect->left < 0 || pSRect->right <= 0 ||
		pSRect->top < 0 || pSRect->bottom <= 0)
		return;
	
	UInt32	Width = pSRect->right - pSRect->left;
	UInt32  High = pSRect->bottom - pSRect->top;
	PUInt8   SpBits = GetPtrXY(pSRect->left, pSRect->top);

	for(UInt32 i = 0; i < High; ++i)
	{
		UInt32	*SLine = reinterpret_cast<UInt32 *>(SpBits);
		for(UInt32 j = 0; j < Width; ++j)
		{
			if(SLine[j] == 0)
				SLine[j] = pData;
		}
		SpBits += m_Pitch;
	}
}



void CImage::ScrollUp(UInt32 pLine, bool pFill0Now)
{
	PUInt8   SpBits = m_Ptr + pLine * m_Pitch;
	UInt32	Len = (m_Height - pLine) * m_Pitch;

	memmove(m_Ptr, SpBits, Len);
	if(pFill0Now)
		memset(m_Ptr + Len, 0, pLine * m_Pitch);	
}


void CImage::ScrollDown(UInt32 pLine, bool pFill0Now)
{
	PUInt8   SpBits = m_Ptr + pLine * m_Pitch;
	UInt32	Len = (m_Height - pLine) * m_Pitch;

	memmove(SpBits, m_Ptr, Len);
	if(pFill0Now)
		memset(m_Ptr, 0, pLine * m_Pitch);	
}





//////////////////////////////////////////////////////////////////////////////////////


CImage *CImageManager::CreateImage(CImage *pImage)
{
	if(pImage)
	{
		std::map<CImage *, String>::iterator j = m_ImagePtrMap.find(pImage);

		if(j != m_ImagePtrMap.end())
		{
			pImage->IncCounter();

			return pImage;
		}
		else
		{
			StringA TempName;

			TempName.Format("1QGLImgTemp%d", m_BaseTempCode++);
			m_ImagePtrMap[pImage] = TempName.c_str();
			m_ImageMap[TempName.c_str()] = pImage;
			pImage->IncCounter();

			return pImage;
		}
	}

	return NULL;
}

CImage *CImageManager::CreateImage(PCSTRING pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag)
{
	//return NULL;
	String	uFileName = pFileName;

	uFileName.ToLower();
	pFileName = uFileName.c_str();

	std::map<String, CImage *>::iterator it = m_ImageMap.find(pFileName);

	if(it != m_ImageMap.end())
	{
		it->second->IncCounter();

		
		return it->second;
	}

	String picFileNameStr = pFileName;
	size_t pos = picFileNameStr.find_last_of(_T("."));
	picFileNameStr = picFileNameStr.substr(0, pos);
	picFileNameStr.Format(_T("%s.pic"), picFileNameStr.c_str());
	it = m_ImageMap.find(picFileNameStr.c_str());

	if(it != m_ImageMap.end())
	{
		it->second->IncCounter();

		

		return it->second;
	}
	else
	{
		LPDIRECT3DTEXTURE9 Texture = NULL;
		D3DXIMAGE_INFO SrcInfo;
		HANDLE hFile = NULL;
		HANDLE hMapFile = NULL;
		Byte * pInFile = NULL;
		Byte * pPicData = NULL;
		UInt width = 0;
		UInt height = 0;
		if( g_GdfResource || g_GdfResourceEx )
		{
			String filename = picFileNameStr.c_str();
			UInt32 beginOffset, fileSize;
			BYTE* fbuf = NULL;
			if ( g_GdfResourceEx )
				fbuf = g_GdfResourceEx->GetMapViewOfFile(filename, beginOffset, fileSize);
			if ( (!fbuf) && g_GdfResource )
				fbuf = g_GdfResource->GetMapViewOfFile(filename, beginOffset, fileSize);
			if( fbuf )
			{
				pInFile = fbuf;
				width = *(WORD *)(pInFile + beginOffset);
				height = *(WORD *)(pInFile + 4 + beginOffset);
				pPicData = pInFile + 8 + beginOffset;
				
				
			}
			else
			{
				if ( g_GdfResourceEx )
					fbuf = g_GdfResourceEx->GetMapViewOfFile(String(pFileName), beginOffset, fileSize); 
				if ( (!fbuf) && g_GdfResource )
					fbuf = g_GdfResource->GetMapViewOfFile(String(pFileName), beginOffset, fileSize); 
				if (fbuf)
				{
					BYTE * buf = fbuf + beginOffset;
					if (FAILED(D3DXCreateTextureFromFileInMemoryEx (m_Device, buf, fileSize, 0, 0, 1, 0, 
						pFormat, pPool, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &SrcInfo, NULL, &Texture)))
					{
						UnmapViewOfFile(fbuf);
						return NULL;
					}
					UnmapViewOfFile(fbuf);
				}
				else
					return NULL;
			}
			
		}
		else
		{
			
			hFile = CreateFile(picFileNameStr.c_str(), FILE_READ_DATA, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

			String fileNameStr = picFileNameStr.c_str();
			String::size_type pos = fileNameStr.find_last_of(_T("\\"));

			if (pos != String::npos)
			{
				fileNameStr = fileNameStr.substr(pos + 1);
			}

			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 
				0, 0, fileNameStr.c_str());

			pInFile = (Byte *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
			if (pInFile && hMapFile && hFile)
			{
				width = *(WORD *)(pInFile);
				height = *(WORD *)(pInFile + 4);
				pPicData = pInFile + 8;
				
			}
			else if (FAILED(D3DXCreateTextureFromFileEx(m_Device, pFileName, 0, 0, 1, 0, 
					pFormat, pPool, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &SrcInfo, NULL, &Texture)))
			{
				return NULL;
			}
		}

		
		if (pInFile)
		{
			CImage *Image = SafeCreateObject4(CImage, pPicData, width, height, 8888);

			Image->SetHandle(hFile, hMapFile, pInFile);
			
			m_ImageMap[picFileNameStr.c_str()] = Image;
			m_ImagePtrMap[Image] = picFileNameStr.c_str();

			Image->SetFileName(picFileNameStr.c_str());
			Image->IncCounter();

			

			return Image;
		}
		else if (Texture)
		{
			CImage *Image = SafeCreateObject3(CImage, Texture, &SrcInfo, Flag);
			SafeRelease(Texture);

			m_ImageMap[pFileName] = Image;
			m_ImagePtrMap[Image] = pFileName;
			Image->SetFileName(pFileName);
			Image->IncCounter();

			
			return Image;
		}
		else
			return NULL;
	}

	return  NULL;
}


CImage *CImageManager::CreateImageA(PCSTRINGA pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag)
{
	String		StrW = pFileName;

	return CreateImage(StrW.c_str(), pFormat, pPool, Flag);
}


CImage *CImageManager::CreateImage(UInt32 pWidth, UInt32 pHeight, UInt32 pType)
{
	CImage *pImage = SafeCreateObject3(CImage, pWidth, pHeight, pType);
	StringA TempName;

	TempName.Format("2QGLImgTemp%d", m_BaseTempCode++);
	m_ImagePtrMap[pImage] = TempName.c_str();
	m_ImageMap[TempName.c_str()] = pImage;
	pImage->IncCounter();

	return pImage;
}

CImage *CImageManager::CreateImage(PUInt8 BytePtr, UInt32 pWidth, UInt32 pHeight, UInt32 pType)
{
	CImage *pImage = SafeCreateObject4(CImage, BytePtr, pWidth, pHeight, pType);
	StringA TempName;

	TempName.Format("3QGLImgTemp%d", m_BaseTempCode++);
	m_ImagePtrMap[pImage] = TempName.c_str();
	m_ImageMap[TempName.c_str()] = pImage;
	pImage->IncCounter();

	
	return pImage;
}


CImage *CImageManager::FindImage(PCSTRING pFileName)
{
	String	uFileName = pFileName;

	uFileName.ToLower();
	pFileName = uFileName.c_str();
	std::map<String, CImage *>::iterator i = m_ImageMap.find(pFileName);

	if(i != m_ImageMap.end())
	{
		return i->second;
	}

	return  NULL;
}


CImage *CImageManager::FindImageA(PCSTRINGA pFileName)
{
	String		StrW = pFileName;

	return FindImage(StrW.c_str());
}


/*
bool CImageManager::DeleteImage(PCSTRING pFileName)
{
	std::map<String, CImage *>::iterator i = m_ImageMap.find(pFileName);

	if(i != m_ImageMap.end())
	{
		CImage	*ImagePtr = i->second;

		if(ImagePtr->DecCounter() > 0)
			return true;

		SafeDeleteObject(i->second);
		m_ImageMap.erase(i);

		std::map<CImage *, String>::iterator j = m_ImagePtrMap.find(ImagePtr);
		if(j != m_ImagePtrMap.end())
		{
			m_ImagePtrMap.erase(j);
		}

		return true;
	}

	return false;
}

bool CImageManager::DeleteImageA(PCSTRINGA pFileName)
{
	String		StrW = pFileName;

	return DeleteImage(StrW.c_str());
}
*/

bool CImageManager::DeleteImage(CImage *pImage)
{
	if(!pImage)
		return true;
	std::map<CImage *, String>::iterator j = m_ImagePtrMap.find(pImage);
	if(j != m_ImagePtrMap.end())
	{
		std::map<String, CImage *>::iterator i = m_ImageMap.find(j->second.c_str());
		
		if(i != m_ImageMap.end())
		{

			if(pImage->DecCounter() > 0)
			{

				return true;
			}

			SafeDeleteObject(pImage);
			m_ImageMap.erase(i);
		}

		m_ImagePtrMap.erase(j);

		return true;
	}


	return false;
	
	
}



void CImageManager::Release()
{
	std::map<String, CImage *>::iterator i_End = m_ImageMap.end();

	for(std::map<String, CImage *>::iterator i = m_ImageMap.begin(); i != i_End; ++i)
	{
		SafeDeleteObject(i->second);
	}
	m_ImageMap.clear();
	m_ImagePtrMap.clear();
}


CImage *CImageManager::CreateStaticIamge(PCSTRING pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag)
{
	LPDIRECT3DTEXTURE9		Texture;
	D3DXIMAGE_INFO SrcInfo;

	String	uFileName = pFileName;

	uFileName.ToLower();
	pFileName = uFileName.c_str();

	if( g_GdfResource || g_GdfResourceEx )
	{
		String filename = pFileName;
		UInt32 beginOffset, fileSize;
		BYTE* fbuf = NULL;
		if ( g_GdfResourceEx )
			fbuf = g_GdfResourceEx->GetMapViewOfFile(filename, beginOffset, fileSize);
		if ( (!fbuf) && g_GdfResource )
			fbuf = g_GdfResource->GetMapViewOfFile(filename, beginOffset, fileSize);
		if( fbuf )
		{
			BYTE* buf = fbuf + beginOffset;
			if (FAILED(D3DXCreateTextureFromFileInMemoryEx ( m_Device, buf, fileSize, 0, 0, 1, 0, 
				pFormat, pPool, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &SrcInfo, NULL, &Texture)))
			{
				UnmapViewOfFile(fbuf);
				return NULL;
			}
			UnmapViewOfFile(fbuf);
		}
	}
	else
	{
		if (FAILED(D3DXCreateTextureFromFileEx (m_Device, pFileName, 0, 0, 1, 0, 
				pFormat, pPool, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &SrcInfo, NULL, &Texture)))
		{
			return NULL;
		}
	}
	CImage *Image = SafeCreateObject3(CImage, Texture, &SrcInfo, Flag);
	
	m_StaticImageSet.insert(Image);
		
	return Image;
}


CImage *CImageManager::CreateStaticIamgeA(PCSTRINGA pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag)
{
	String		StrW = pFileName;

	return CreateStaticIamge(StrW.c_str(), pFormat, pPool, Flag);
}


void CImageManager::DeleteStaticImage(CImage *pImage)
{
	std::set<CImage *>::iterator i = m_StaticImageSet.find(pImage);
	if(i != m_StaticImageSet.end())
	{
		SafeDeleteObject(*i);
		m_StaticImageSet.erase(i);
	}
}









/////////////////////////////////////////////////////////

	


CAniIcon * IconManager::CreateIcon(PCSTRING pFileName)
{
	String	uFileName = pFileName;

	uFileName.ToLower();
	pFileName = uFileName.c_str();
	std::map<String, CAniIcon *>::iterator it = m_IconMap.find(pFileName);

	if(it != m_IconMap.end())
	{
		return it->second;
	}
	else
	{
		CAniIcon *Icon = SafeCreateObject(CAniIcon);
		Icon->SetImage(pFileName);

		m_IconMap[pFileName] = Icon;
		m_IconPtrMap[Icon] = pFileName;
	}

	return NULL;
}

CAniIcon * IconManager::CreateIconA(PCSTRINGA pFileName)
{
	String		StrW = pFileName;

	return CreateIcon(StrW.c_str());
}

void IconManager::DelIcon(PCSTRING pFileName)
{
	String	uFileName = pFileName;

	uFileName.ToLower();
	pFileName = uFileName.c_str();
	std::map<String, CAniIcon *>::iterator i = m_IconMap.find(pFileName);

	if(i != m_IconMap.end())
	{
		CAniIcon	*Icon = i->second;

		SafeDeleteObject(i->second);
		m_IconMap.erase(i);

		std::map<CAniIcon *, String>::iterator j = m_IconPtrMap.find(Icon);
		if(j != m_IconPtrMap.end())
		{
			m_IconPtrMap.erase(j);
		}
	}
}

void IconManager::DelIcon(CAniIcon *Icon)
{
	std::map<CAniIcon *, String>::iterator j = m_IconPtrMap.find(Icon);

	if(j != m_IconPtrMap.end())
	{
		std::map<String, CAniIcon *>::iterator i = m_IconMap.find(j->second.c_str());

		if(i != m_IconMap.end())
		{
			SafeDeleteObject(i->second);
			m_IconMap.erase(i);
		}

		m_IconPtrMap.erase(j);
	}
}
	
////////////////////////////////////////////////////////////////////////////////////////

UInt32 CImageDirtyRect(Int32 &X, Int32 &Y, RECT &SRect, RECT &RRect)
{
	RECT	DRect;

	DRect.left = X;
	DRect.top = Y;
	DRect.right = X + (SRect.right - SRect.left);
	DRect.bottom = Y + (SRect.bottom - SRect.top);

	if(RRect.bottom < DRect.top || DRect.bottom < RRect.top ||
		RRect.right < DRect.left || DRect.right < RRect.left)
		return 0;

	if(RRect.left > DRect.left)
	{
		X = RRect.left;
		SRect.left += RRect.left - DRect.left;
	}

	if(RRect.top > DRect.top)
	{
		Y = RRect.top;
		SRect.top += RRect.top - DRect.top;
	}

	if(RRect.right < DRect.right)
	{
		SRect.right -= DRect.right - RRect.right;
		if(SRect.left == SRect.right)
			++SRect.right;
	}
	
	if(RRect.bottom < DRect.bottom)
	{
		SRect.bottom -= DRect.bottom - RRect.bottom;
		if(SRect.top == SRect.bottom)
			++SRect.bottom;
	}

	if(SRect.left == SRect.right)
	{
		--SRect.left;
		--X;
	}

	if(SRect.top == SRect.bottom)
	{
		--SRect.top;
		--Y;
	}

	return 1;
}



UInt32 CImageDirtyRect(RECT	&DRect, RECT &SRect, RECT &RRect)
{
	if(RRect.bottom < DRect.top || DRect.bottom < RRect.top ||
		RRect.right < DRect.left || DRect.right < RRect.left)
		return 0;

	if(RRect.left > DRect.left)
	{
		DRect.left = RRect.left;
		SRect.left += RRect.left - DRect.left;
	}

	if(RRect.top > DRect.top)
	{
		DRect.top = RRect.top;
		SRect.top += RRect.left - DRect.left;
	}

	if(RRect.right < DRect.right)
	{
		LONG	Temp = DRect.right - RRect.right;
		DRect.right -= Temp;
		SRect.right -= Temp;
	}
	
	if(RRect.bottom < DRect.bottom)
	{
		LONG Temp = DRect.bottom - RRect.bottom;

		DRect.bottom -= Temp;
		SRect.bottom -= Temp;
	}
	

	return 1;
}


