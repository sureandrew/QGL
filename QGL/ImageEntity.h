#pragma once
#ifndef IMAGE_ENTITY_H_
#define IMAGE_ENTITY_H_

#include "EntityBase.h"

class CImageEntity : public CEntityBase
{
	
public:
	CImageEntity();
	~CImageEntity();

	virtual BOOL GetEntityPart(size_t index, Int ImageType, RECT * LockRegion);
	virtual Int GetTotalRenderPart();
	virtual BOOL GetHaveEffect();

	VOID SetImage(LPDIRECT3DTEXTURE9 pImage);
	VOID SetImage(LPCTSTR filename);
	VOID CreateImage(Int Width, Int Height, D3DPOOL  Pool, D3DFORMAT Format);

	inline LPDIRECT3DTEXTURE9 GetImage(){return m_pImage;}
protected:
	LPDIRECT3DTEXTURE9 m_pImage;
};
#endif