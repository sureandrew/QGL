#pragma once
#ifndef DIRTYRECTMANAGER_H_
#define DIRTYRECTMANAGER_H_

class CDirtyRectManager
{
private:

	//add the dirty rectangle and make sure no overlapping in the rectangle list
	VOID AddDirtyRectUnique(RECT NewDirtyRect);

	VOID AddDirtyRectUnique(RECT NewDirtyRect, StlVector<RECT> & DirtyRectLst);

	public:
	//construct and distruct
	CDirtyRectManager();
	~CDirtyRectManager();

	//clear the dirtyRectLst
	VOID ClearDirtyRect();

	//add DirtyRect To Lst
	VOID AddDirtyRectToMan(RECT NewDirtyRect);

	VOID AddDirtyRectToMan(RECT NewDirtyRect, StlVector<RECT> & DirtyRectLst);

	VOID RemoveDirtyRect(RECT RemoveRect);
	//check whether the givenrect is overlapped with dirty rect list
	BOOL OverlappedWithDirtyRect(RECT GivenRect,StlVector<RECT> & ResultRect);

	//check whether there is overlapping in current rectangle's list
	BOOL CheckOverlapped();
	VOID GetRectLstWithGivenArea(RECT GivenArea, StlVector<RECT> & RectInsideLst, StlVector<RECT> & RectOutsideLst);

	inline StlVector<RECT>  GetDirtyRectLst() { return DirtyRectLst;};

	inline void SetAddAvial(BOOL Value){ 
		m_AddAvial = Value;
	}

	inline BOOL  GetAddAvial(){ return m_AddAvial;}

	inline VOID SetBoundRect(Int Left, Int Top, Int Right, Int Bottom)
	{
		SetRect(&m_DirtyRectBound, Left, Top, Right, Bottom);
	};

	inline VOID AddAllDirty()
	{
		AddDirtyRectUnique(m_DirtyRectBound);
		m_AddAvial = FALSE;
	};
private:
	//stroing the total dirty regions
	StlVector<RECT> DirtyRectLst;

	BOOL m_AddAvial;

	RECT m_DirtyRectBound;
};
#endif