//-- Common
#include "Common.h"
#include "QGL.h"
//-- Self
#include "DirtyRectManager.h"
//-- Library
#include "Math.h"

#include "Reuben\Diagnostics\DebugNew.h"

#define SmallestOverlappedAreaInPrecen 100
	//construct and distruct
CDirtyRectManager::CDirtyRectManager()
{
	m_AddAvial = TRUE;
	SetRect(&m_DirtyRectBound, 0, 0, 0, 0);
}

CDirtyRectManager::~CDirtyRectManager()
{
}

	//clear the dirtyRectLst
VOID CDirtyRectManager::ClearDirtyRect()
{
	DirtyRectLst.clear();
	m_AddAvial = TRUE;
}

VOID CDirtyRectManager::GetRectLstWithGivenArea(RECT GivenArea, StlVector<RECT> & RectInsideLst, StlVector<RECT> & RectOutsideLst)
{
	StlVector<RECT>::iterator DirtyRectItr = DirtyRectLst.begin();

	//check whether there is another rect that is overlapped totally with GivenRectl
	while (DirtyRectItr != DirtyRectLst.end())
	{
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(GivenArea, (*DirtyRectItr),  ResultRect))
		{
			RectInsideLst.push_back(ResultRect);

			StlVector<RECT> BrokenRectLst;
			if (QGLMath::CalculateReductedArea(GivenArea, (*DirtyRectItr), BrokenRectLst))
			{
				for (Int i = 0; i < (Int)BrokenRectLst.size(); i++)
					RectOutsideLst.push_back(BrokenRectLst[i]);
			}
		}
		else
			RectOutsideLst.push_back((*DirtyRectItr));
		DirtyRectItr++;
	}

}

VOID CDirtyRectManager::RemoveDirtyRect(RECT RemoveRect)
{
	StlVector<RECT> tempRect;// = 
	for (StlVector<RECT>::iterator DirtyRectItr = DirtyRectLst.begin();
		DirtyRectItr != DirtyRectLst.end(); ++ DirtyRectItr )
	{
		StlVector<RECT> NewDirtyRectLst;
		if (QGLMath::CalculateReductedArea(RemoveRect, (*DirtyRectItr),  NewDirtyRectLst))
		{
			if (NewDirtyRectLst.size() > 0)
			{
				for (Int i = 0; i < (Int)NewDirtyRectLst.size(); i++)
					tempRect.push_back(NewDirtyRectLst[i]);
			}
			else
				tempRect.push_back(*DirtyRectItr);

		}
		else
			tempRect.push_back(*DirtyRectItr);
	}
	DirtyRectLst = tempRect;
}
VOID CDirtyRectManager::AddDirtyRectUnique(RECT NewDirtyRect)
{
	

	StlVector<RECT>::iterator DirtyRectItr = DirtyRectLst.begin();

	//check whether there is another rect that is overlapped totally with GivenRectl
	while (DirtyRectItr != DirtyRectLst.end())
	{
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(NewDirtyRect, (*DirtyRectItr),  ResultRect))
		{
			if (QGLMath::IsTwoRectEqual(ResultRect, (*DirtyRectItr)))
			{
				//DirtyRectRmLst.push_back(DirtyRectItr);
				DirtyRectItr = DirtyRectLst.erase(DirtyRectItr);
				continue;
			}
				
			if (QGLMath::IsTwoRectEqual(NewDirtyRect, ResultRect))
			{	
				return;
			}
		}
		++DirtyRectItr;
	}
	//cut the rectangles
	StlVector<RECT> DirtyRectAddLst;
	for (DirtyRectItr = DirtyRectLst.begin(); DirtyRectItr != DirtyRectLst.end(); ++DirtyRectItr)
	{
		StlVector<RECT> NewDirtyRectLst;
		if (QGLMath::CalculateReductedArea(NewDirtyRect, (*DirtyRectItr),  NewDirtyRectLst))
		{
			if (NewDirtyRectLst.size() > 0)
				*DirtyRectItr = NewDirtyRectLst[0];

			for (Int i = 1; i < (Int)NewDirtyRectLst.size(); i++)
				DirtyRectAddLst.push_back(NewDirtyRectLst[i]);
		}
	}

	DirtyRectLst.push_back(NewDirtyRect);

	for (Int i = 0; i < (Int)DirtyRectAddLst.size(); i++)
	{
		DirtyRectLst.push_back(DirtyRectAddLst[i]);
	}

	//reduce rectangle in not dirtyrectlist

	//if (CheckOverlapped())
	//	DebugBreak();
}

//add DirtyRect To Lst
VOID CDirtyRectManager::AddDirtyRectToMan(RECT NewDirtyRect)
{
	if (!m_AddAvial)
		return;

	//invalid rect return
	if (NewDirtyRect.left >= NewDirtyRect.right ||
		NewDirtyRect.top >= NewDirtyRect.bottom)
		return;

	RECT ResultRect;
	if (!QGLMath::CalculateOverlappedRect(NewDirtyRect, m_DirtyRectBound, ResultRect))
		return;

	NewDirtyRect = ResultRect;

	for (Int i = 0; i < (Int)DirtyRectLst.size(); i++)
	{
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(DirtyRectLst[i], NewDirtyRect, ResultRect))
		{
			QGLMath::CalculateBiggestRect(DirtyRectLst[i], NewDirtyRect, ResultRect);

			Real32 Rect1Percen = (Real32)((DirtyRectLst[i].right - DirtyRectLst[i].left) * (DirtyRectLst[i].bottom - DirtyRectLst[i].top))
				/(Real32)((ResultRect.right - ResultRect.left) * (ResultRect.bottom - ResultRect.top)) * 100;

			Real32 Rect2Percen = (Real32)((NewDirtyRect.right - NewDirtyRect.left) * (NewDirtyRect.bottom - NewDirtyRect.top))
				/(Real32)((ResultRect.right - ResultRect.left) * (ResultRect.bottom - ResultRect.top)) * 100;

			if (Rect1Percen + Rect2Percen > 
				SmallestOverlappedAreaInPrecen)
			{
				NewDirtyRect = ResultRect;
				break;
			}
		}
	}

	//if no overlapped, divide the current rectangle
	AddDirtyRectUnique(NewDirtyRect);
	
}


VOID CDirtyRectManager::AddDirtyRectUnique(RECT NewDirtyRect, StlVector<RECT> & DirtyRectLst)
{
	StlVector<RECT>::iterator DirtyRectItr = DirtyRectLst.begin();

	//check whether there is another rect that is overlapped totally with GivenRectl
	while (DirtyRectItr != DirtyRectLst.end())
	{
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(NewDirtyRect, (*DirtyRectItr),  ResultRect))
		{
			if (QGLMath::IsTwoRectEqual(ResultRect, (*DirtyRectItr)))
			{
				//DirtyRectRmLst.push_back(DirtyRectItr);
				DirtyRectLst.erase(DirtyRectItr);
				DirtyRectItr = DirtyRectLst.begin();
				continue;
			}
				
			if (QGLMath::IsTwoRectEqual(NewDirtyRect, ResultRect))
			{	
				return;
			}
		}
		DirtyRectItr++;
	}
	//cut the rectangles
	StlVector<RECT> DirtyRectAddLst;
	for (DirtyRectItr = DirtyRectLst.begin(); DirtyRectItr != DirtyRectLst.end(); DirtyRectItr ++)
	{
		StlVector<RECT> NewDirtyRectLst;
		if (QGLMath::CalculateReductedArea(NewDirtyRect, (*DirtyRectItr),  NewDirtyRectLst))
		{
			if (NewDirtyRectLst.size() > 0)
				*DirtyRectItr = NewDirtyRectLst[0];

			for (Int i = 1; i < (Int)NewDirtyRectLst.size(); i++)
				DirtyRectAddLst.push_back(NewDirtyRectLst[i]);
		}
	}

	DirtyRectLst.push_back(NewDirtyRect);

	for (Int i = 0; i < (Int)DirtyRectAddLst.size(); i++)
	{
		DirtyRectLst.push_back(DirtyRectAddLst[i]);
	}

	//if (CheckOverlapped())
	//	DebugBreak();
}

//add DirtyRect To Lst
VOID CDirtyRectManager::AddDirtyRectToMan(RECT NewDirtyRect, StlVector<RECT> & DirtyRectLst)
{
	for (Int i = 0; i < (Int)DirtyRectLst.size(); i++)
	{
		RECT ResultRect;
		if (QGLMath::CalculateOverlappedRect(DirtyRectLst[i], NewDirtyRect, ResultRect))
		{
			QGLMath::CalculateBiggestRect(DirtyRectLst[i], NewDirtyRect, ResultRect);

			Real32 Rect1Percen = (Real32)((DirtyRectLst[i].right - DirtyRectLst[i].left) * (DirtyRectLst[i].bottom - DirtyRectLst[i].top))
				/(Real32)((ResultRect.right - ResultRect.left) * (ResultRect.bottom - ResultRect.top)) * 100;

			Real32 Rect2Percen = (Real32)((NewDirtyRect.right - NewDirtyRect.left) * (NewDirtyRect.bottom - NewDirtyRect.top))
				/(Real32)((ResultRect.right - ResultRect.left) * (ResultRect.bottom - ResultRect.top)) * 100;

			if (Rect1Percen + Rect2Percen > 
				SmallestOverlappedAreaInPrecen)
			{
				NewDirtyRect = ResultRect;
				break;
			}
		}
	}

	//if no overlapped, divide the current rectangle
	AddDirtyRectUnique(NewDirtyRect);
	
}

//check whether the givenrect is overlapped with dirty rect list
BOOL CDirtyRectManager::OverlappedWithDirtyRect(RECT GivenRect,StlVector<RECT> & ResultRect)
{
	BOOL Added = FALSE;

	//return Added;

	for (Int i = 0; i < (Int)DirtyRectLst.size(); i++)
	{
		RECT TempResultRect;
		if (QGLMath::CalculateOverlappedRect(DirtyRectLst[i], GivenRect, TempResultRect))
		{
			//add the new area without overlapped with the previous areas
			BOOL found = FALSE;
			for (Int j = 0; j < (Int)ResultRect.size(); j++)
			{
				RECT BiggestResultRect;
				if (QGLMath::CalculateOverlappedRect(ResultRect[j], TempResultRect, BiggestResultRect))
				{
					found = TRUE;
					break;
				}
			}
			if (!found)
			{
				ResultRect.push_back(TempResultRect);
			}
			else
			{
				AddDirtyRectToMan(TempResultRect, ResultRect);	
			}
			Added =TRUE;
		}
	}

	return Added;
}


BOOL CDirtyRectManager::CheckOverlapped()
{
	for (Int i = 0; i < (Int)DirtyRectLst.size(); i++)
	{
		for (Int j = i + 1; j < (Int)DirtyRectLst.size(); j++)
		{
			RECT ResultRect;
			if (QGLMath::CalculateOverlappedRect(DirtyRectLst[i], DirtyRectLst[j], ResultRect))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}