#pragma once
#ifndef _MIDDLEMAP_H_
#define _MIDDLEMAP_H_

#include <Reuben\System\Reuben.h>
#include <vector>

struct DirtyPoint
{
	Int pixelX;
	Int pixelY;
};

struct EngNPCName
{
	String npcEngName;
	String npcChiName;
	UInt   npcPtr;
};


class CImage;

class CMiddleMap
{
private:

	UInt32 m_iMapID;

	double m_dwRatioX;
	double m_dwRatioY;
	bool m_bLoadMap;
	StringA m_sPrevIconName;
	typedef StlMap<StringA, UInt, std::less<StringA>, ManagedAllocator<std::pair<StringA, UInt> > > IconNpcIDMap;
	IconNpcIDMap		m_IconNPCLst;
	IconNpcIDMap		m_JumpPointLst;
	
	StlVector<DirtyPoint , ManagedAllocator<DirtyPoint> >	m_DirtyPointList;
	
	bool m_bChangeSize;
	Int32 m_dX;
	Int32 m_iDesBlockX, m_iDesBlockY;
	
	Int32 m_iCharDesBlockX, m_iCharDesBlockY;
	CImage * m_pPathIm;

public:
	void ShowMidMap();
	void UpdatePosition();
	void MoveCharacterTo();
	
	void ResetText();
	void ResetValue();
	void ShowNpcName();
	void ShowJumpPointName();
	
	void SetLocationPanelSize(UInt16 size);
	
	void MoveCharacterToJumpPoint();
	void ScrollUpOrDown(int up); // 0:up , 1: down
	void SetLocationBoxPos(Int32 x, Int32 y, Int32 textPos = -21 );
	
	void ClearPath();
	void DrawPath(Int32 desBlockX = 0, Int32 desBlockY = 0);

	void SetLocationBoxText(String text);
	void ShowLocationBox();
	void SetLocationBoxPosText(UInt16 x, UInt16 y);
	bool HasPath();

	void MoveCharacterToNPC();

	INLINE Int32 GetCharDesX(){return m_iCharDesBlockX;}
	INLINE Int32 GetCharDesY(){return m_iCharDesBlockY;}

	INLINE void ResetCharDes()
	{
		m_iCharDesBlockX = -1;
		m_iCharDesBlockY = -1;

		m_iDesBlockX = -1;
		m_iDesBlockY = -1;
	}

	
	INLINE void SetCharDes(Int32 blockX, Int32 blockY)
	{
		m_iCharDesBlockX = blockX;
		m_iCharDesBlockY = blockY;
	}

	INLINE void SetPathEnd(Int32 blockX, Int32 blockY)
	{
		m_iDesBlockX = blockX;
		m_iDesBlockY = blockY;
	}

	bool IsOpenMiddleMap();

	CMiddleMap();
	virtual ~CMiddleMap();

};


#endif 
