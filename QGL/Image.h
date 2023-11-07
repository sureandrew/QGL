#pragma once
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Render.h"
#include "EntityBase.h"

void FillUInt32T(LPDIRECT3DTEXTURE9	pTexture, UInt32 pData, LPRECT pSRect);

//////////////////////////////////////////////////////

UInt32 Alpha565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Alpha4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Alpha8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Alpha565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Alpha4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Alpha8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Alpha565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Alpha4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Alpha8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);



UInt32 Copy565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Copy4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Copy8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Copy565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Copy4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Copy8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Copy565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Copy4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Copy8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);



UInt32 Additive565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Additive4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Additive8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Additive565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Additive4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Additive8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Additive565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Additive4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Additive8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);



UInt32 Subtractive565To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Subtractive4444To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Subtractive8888To8888(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Subtractive565To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Subtractive4444To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Subtractive8888To4444(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

UInt32 Subtractive565To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Subtractive4444To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);
UInt32 Subtractive8888To565(Int32 X, Int32 Y, LPDIRECT3DTEXTURE9 STexture, LPDIRECT3DTEXTURE9 DTexture, RECT *lpSRect);

/////////////////////////////////////////////////////////////////////////////////



UInt32 FillTexture(LPDIRECT3DTEXTURE9 STexture, RECT *lpSRect, UInt32 DataLen);


////////////////////////////////////////////////////////////////




class CImage
{
public:

	CImage();
	CImage(UInt32 pWidth, UInt32 pHeight, UInt32 pType);
	CImage(PUInt8 BytePtr, UInt32 pWidth, UInt32 pHeight, UInt32 pType);
	//CImage(Char *pFilename);
	CImage(LPDIRECT3DTEXTURE9 pTexture);
	CImage(LPDIRECT3DTEXTURE9 pTexture, UInt32 pIsRef);
	CImage(LPDIRECT3DTEXTURE9 pTexture, D3DXIMAGE_INFO *pSrcInfo, UInt32 pIsRef);
	CImage(LPDIRECT3DTEXTURE9 pTexture, UInt32 Width, UInt32 Height, UInt32 Type, UInt32 Size);

	~CImage()
	{
		//TRACE_INFODTL_2(GLOBAL_LOGGER, _F("~CImage = %d FileName = %s"), this, m_strFileName.c_str());
		Release();
	};

	void Release()
	{
		if(m_TextureCode == 0 || m_TextureCode == 3)
		{
			SafeDeallocate(m_Ptr);
		}

		if (pInFile)
		{
			UnmapViewOfFile(pInFile);
			pInFile = NULL;
		}
		if (hMapFile)
		{
			CloseHandle(hMapFile);
			hMapFile = NULL;
		}
		if (hFile)
		{
			CloseHandle(hFile);
			hFile = NULL;
		}
	}


	void TextureRelease()
	{
		if(m_TextureCode  == 1)
		{
			SafeRelease(m_Texture);
		}
	}


	LPDIRECT3DTEXTURE9	GetTexture()
	{
		return m_Texture;
	}

	void Unlock();
	void Lock();

	UInt32 GetWidth()
	{
		return m_Width;
	};
	UInt32 GetHeight()
	{
		return m_Height;
	};
	UInt32 GetPitch()
	{
		return m_Pitch;
	};
	UInt32 GetLength()
	{
		return m_Len;
	};
	PUInt8	GetPtr()
	{
		return m_Ptr;
	};
	PUInt8	GetPtrXY(Int32 pX, Int32 pY)
	{
		return m_Ptr + (pX * m_PixelSize + pY * m_Pitch);
	};
	UInt32	GetType()
	{
		return m_Type;
	};

	
	void FillUInt16(UInt16 pData, LPRECT	pSRect);
	void FillUInt32(UInt32 pData, LPRECT	pSRect);
	void FillUInt32();
	void ZeroUInt16(UInt16 pData, LPRECT	pSRect);
	void ZeroUInt32(UInt32 pData, LPRECT	pSRect);
	void FillZeroUInt16(UInt16 pData, LPRECT	pSRect);
	void FillZeroUInt32(UInt32 pData, LPRECT	pSRect);
	void DrawBox(UInt32 pData);
	void DrawBox(UInt32 pData, LPRECT pSRect);

	void ScrollUp(UInt32 pLine, bool pFill0Now = true);
	void ScrollDown(UInt32 pLine, bool pFill0Now = true);

	void SetFileName(PCSTRING filename)
	{
		m_strFileName = filename;
	};

	PCSTRING GetFileName(VOID)
	{
		return (m_strFileName.c_str());
	};

	void ClearAll()
	{
		memset(m_Ptr, 0, m_Height * m_Pitch);
	}
	void ClearTopLine(UInt32 pLine)
	{
		memset(m_Ptr, 0, pLine * m_Pitch);
	}
	void ClearBottomLine(UInt32 pLine)
	{
		memset(m_Ptr + (m_Height - pLine) * m_Pitch, 0, pLine * m_Pitch);
	}

	inline void SetHandle(HANDLE File, HANDLE MapFile, Byte * InFile)
	{
		hFile = File;
		hMapFile = MapFile;
		pInFile = InFile;
	}

	inline UInt32 GetTextureCode(){return m_TextureCode;}

	Int32 IncCounter()
	{
		return ++m_InitCounter;
	}

	Int32 DecCounter()
	{
		return --m_InitCounter;
	}

	Int32 GetCounter()
	{
		return m_InitCounter;
	}

private:

	Int32	m_InitCounter;
	UInt32	m_Width;
	UInt32	m_Height;
	UInt32	m_Pitch;
	UInt32	m_Len;
	UInt32	m_Type;
	UInt32	m_PixelSize;
	UInt32	m_TextureCode;
	PUInt8	m_Ptr;
	LPDIRECT3DTEXTURE9	m_Texture;

	
	String	m_strFileName;

	HANDLE hFile;
	HANDLE hMapFile;
	Byte * pInFile;
};


///////////////////////////////////////////////////////////////////////////////////

UInt32 Liner8888To8888(Int32 X, Int32 Y, UInt32 Width, CImage *pSImage, CImage *pDImage, RECT *lpSRect);
UInt32 Liner8888To8888V(Int32 X, Int32 Y, UInt32 Height, CImage *pSImage, CImage *pDImage, RECT *lpSRect);


UInt32 Alpha565To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha4444To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha8888To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha8888To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha8888To8888_A(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Alpha8888To8888C(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect, UInt32 Color);
UInt32 Alpha8888To8888BW(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect);
UInt32 Alpha8888To8888P(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect, UInt32 Percent);

UInt32 Alpha8888To8888S(Int32 X, Int32 Y, CImage *pSImage, CImage *pDImage, RECT *lpSRect, UInt32 Style, UInt32 Color = 0xFFFFFFFF);

UInt32 Alpha565To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha4444To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha8888To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Alpha565To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha4444To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Alpha8888To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 BlitAlpha(UInt32 X, UInt32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);


UInt32 Copy565To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Copy4444To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Copy8888To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Copy565To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Copy4444To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Copy8888To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Copy565To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Copy4444To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Copy8888To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Additive565To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Additive4444To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Additive8888To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Additive565To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Additive4444To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Additive8888To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Additive565To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Additive4444To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Additive8888To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Subtractive565To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Subtractive4444To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Subtractive8888To8888(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Subtractive565To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Subtractive4444To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Subtractive8888To4444(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 Subtractive565To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Subtractive4444To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);
UInt32 Subtractive8888To565(Int32 X, Int32 Y, CImage *SImage, CImage *DImage, RECT *lpSRect);

UInt32 CImageDirtyRect(Int32 &X, Int32 &Y, RECT &SRect, RECT &RRect);
UInt32 CImageDirtyRect(RECT	&DRect, RECT &SRect, RECT &RRect);

/////////////////////////////////////////////////////////

class CImageManager : public Reuben::Utility::Singleton<CImageManager>
{
	public:
	CImageManager()
	{
		m_BaseTempCode = 1;
	}
	~CImageManager()
	{
		Release();
	}

	SINGLETON(CImageManager);

	void Release();
	
	void SetDevice(LPDIRECT3DDEVICE9 Device)
	{
		m_Device = Device;
	}
	
	CImage *CreateImage(PCSTRING pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag = false);
	CImage *CreateImageA(PCSTRINGA pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag = false);
	CImage *CreateImage(CImage *pImage);
	CImage *CreateImage(UInt32 pWidth, UInt32 pHeight, UInt32 pType);
	CImage *CreateImage(PUInt8 BytePtr, UInt32 pWidth, UInt32 pHeight, UInt32 pType);

	CImage *FindImage(PCSTRING pFileName);
	CImage *FindImageA(PCSTRINGA pFileName);
	/*
	bool DeleteImage(PCSTRING pFileName);
	bool DeleteImageA(PCSTRINGA pFileName);
	*/
	bool DeleteImage(CImage *pImage);


	CImage *CreateStaticIamge(PCSTRING pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag = false);
	CImage *CreateStaticIamgeA(PCSTRINGA pFileName, D3DFORMAT pFormat, D3DPOOL pPool, Boolean Flag = false);
	void DeleteStaticImage(CImage *pImage);
	
private:

	std::map<String, CImage *>	m_ImageMap;
	std::map<CImage *, String>	m_ImagePtrMap;

	std::set<CImage *>			m_StaticImageSet;
	LPDIRECT3DDEVICE9			m_Device;
	
	UInt32						m_BaseTempCode;
};



//////////////////////////////
class CAniIcon/////
{
public:
	CAniIcon()
	{
		memset(this, 0, sizeof(CAniIcon));
	};

	~CAniIcon()
	{
		CImageManager::GetInstance().DeleteImage(m_Image);
		m_Image = NULL;
	};

	

	void SetImage(PCSTRING pFileName)
	{
		m_Image =  CImageManager::GetInstance().CreateImage(pFileName, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	}


	void SetImageA(PCSTRINGA pFileName)
	{
		m_Image =  CImageManager::GetInstance().CreateImageA(pFileName, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH);
	}


	void SetRect(UInt32 Left, UInt32 Top, UInt32 Right, UInt32 Bottom)
	{
		::SetRect(&m_BaseRect, Left, Top, Right, Bottom);
		::SetRect(&m_BackRect, Left, Top, Right, Bottom);
		m_Offset = 0;
		m_Width = m_BaseRect.right - m_BaseRect.left;
	}


	void Render(UInt32 x, UInt32 y, CImage *OutputImage)
	{
		Alpha8888To8888(x, y, m_Image, OutputImage, &m_BackRect);
	}

	void SetCount(UInt32 Count)
	{
		m_Count = Count;
	}

	UInt32 GetCount()
	{
		return m_Count;
	}

	void Play()
	{
		if(++m_Offset >= m_Count)
			m_Offset = 0;
		m_BackRect.left = m_BackRect.left + m_Width * m_Offset;
		m_BackRect.right = m_BackRect.right + m_Width * m_Offset;
	}

private:

	CImage	*m_Image;
	RECT	m_BaseRect, m_BackRect;
	UInt32	m_Count, m_Offset, m_Width, m_Height;
};



/////////////////////////////////

class IconManager
{
public:
	IconManager()
	{
		
	}
	~IconManager()
	{
		m_IconMap.clear();
	}

	CAniIcon * CreateIcon(PCSTRING pFileName);
	CAniIcon * CreateIconA(PCSTRINGA pFileName);
	void DelIcon(PCSTRING pFileName);
	void DelIcon(CAniIcon *);

private:

	std::map<String, CAniIcon *>	m_IconMap;
	std::map<CAniIcon *, String>	m_IconPtrMap;
};

/////////////////////////////////


void SetupEditCursor(CRender *pRender);
void ReleaseEditCursor();

#endif // _IMAGE_H_
