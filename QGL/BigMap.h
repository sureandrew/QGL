#pragma once
#ifndef _BIGMAP_H_
#define _BIGMAP_H_

#include "Reuben/System/Reuben.h"
#include <vector>

typedef StlMap<StringA, UInt16, std::less<StringA>, ManagedAllocator<std::pair<StringA, UInt16> > > MapNameList;


class CBigMap
{
private:
	UInt32Vector m_MapTypeCount;
	MapNameList m_ButtonMapIDLst;
	UInt16 m_iPathStationCount;
	UInt32 m_iDesMap;
	bool m_bOpenBigMap;
public:
	CBigMap();
	virtual ~CBigMap();

	void DrawButton();
	void FindPath(UInt16 src, UInt16 des, UInt16Vector &path);
	UInt32 GetButtonCount(int index); //there are 6 type of button
	void ShowPath();
	UInt16 GetMinPathMap( const std::map<UInt16, UInt16> openList );
	void StartBigMapDescMode(bool start);
	void ShowDesc();
	INLINE void ResetDes(){ m_iDesMap = 0; } //reset the destination
	void DrawFlag(UInt16Vector path);
	void DisableAllFlag();
	INLINE void SetOpenBigMap(bool open){ m_bOpenBigMap = open;}
	INLINE bool IsOpenBigMap(){ return m_bOpenBigMap;}
};



#endif 
