#pragma once
#ifndef _PATH_FINDER_H_
#define _PATH_FINDER_H_

#include "Reuben/System/Reuben.h"
#include "..\QGL.h"
#include <vector>

//#define DEBUG_PATHFIND		1

enum BlockEnum
{
	BK_EMPTY = UINT16_MAX_VALUE,
	BK_BLOCKED = UINT16_MAX_VALUE - 1,
	BK_MARKED = UINT16_MAX_VALUE - 2,
	BK_STMARK = UINT16_MAX_VALUE - 3,
	BK_EDMARK = UINT16_MAX_VALUE - 4,
};

// guide point
// 0 1 2
// 3 4 5
// 6 7 8

// Block Position
// 0,0   1,0   2,0
//    0,1   1,1   2,1
// 0,2   1,2   2,2

// SBlock Position (read by x = up-right directiion, y = down-right direction)
// 0,0   1,1   2,2
//    0,1   1,2   2,3
//-1,1   0,2   1,3

#ifndef SBlockPos
struct SBlockPos
{
	Int16 PosX;
	Int16 PosY;
};
#endif // SBlockPos

struct BlockInfo
{
	UInt16	btype;			// block type
	UInt16	nearGuide;		// nearest guide
};

class PathGraph
{
private:
	UInt32 m_size;
	UInt16 *m_graph;
public:
	PathGraph();
	PathGraph(UInt16 size);
	~PathGraph();
	UInt16 GetSize() { return m_size; }
	void Create(UInt16 size);
	UInt16 Get(UInt16 x, UInt16 y);
	void Set(UInt16 x, UInt16 y, UInt16 v);
	void PrintDebug();
	void Set(UInt16* data);
};

class SymmetricGraph
{
private:
	UInt16 m_size;
	float *m_graph;
public:
	SymmetricGraph();
	SymmetricGraph(UInt16 size);
	~SymmetricGraph();
	UInt16 GetSize() { return m_size; }
	void Create(UInt16 size);
	float Get(UInt16 x, UInt16 y);
	void Set(UInt16 x, UInt16 y, float v);
	void PrintDebug();
	void Set(float* data);
};

class PathFinder
{
public:
	typedef std::vector<BlockPos> PointList;
	typedef std::vector<UInt16> GuideList;
	typedef std::map<UInt16, UInt16> DistList;
	typedef std::map<UInt16, float> FDistList;

private:
	PointList	m_guidePoints;
	SymmetricGraph m_weightGraph;
	SymmetricGraph m_distGraph;
	PathGraph m_linkGraph;
	UInt16		m_width;
	UInt16		m_height;
	UInt16		m_guideCount;
	BlockInfo**	m_map;
	BOOL m_BlockingActive;

	float ActualDistance(UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2);
	float ActualDistance(Int16 x1, Int16 y1, Int16 x2, Int16 y2);
	void FreeMap();
	void AllocateGraph();
	void CalcGuildLink();
	void CalcGuildLink(Real32* weightData, Real32* distData, UInt16* linkData);
	//UInt16 FindNearestGuide(UInt16 x, UInt16 y);

public:
	PathFinder(void);
	~PathFinder(void);
	void ConvToSBlock(UInt16 x, UInt16 y, Int16 &cx, Int16 &cy);
	bool ConvToUBlock(Int16 x, Int16 y, UInt16 &cx, UInt16 &cy);
	BlockInfo& GetMapBySBlock(Int16 x, Int16 y);
	UInt16 GetBlockType(UInt16 x, UInt16 y);
	void SetBlockType(UInt16 x, UInt16 y, UInt16 value);
	bool IsEmpty(UInt16 x, UInt16 y);
	bool IsBlocked(UInt16 x, UInt16 y);
	bool IsGuidePoint(UInt16 x, UInt16 y);
	UInt16 GetWidth() { return m_width; }
	UInt16 GetHeight() { return m_height; }
	BlockPos& GetGuidePoint(int i);
	UInt16 GetGuideCount() { return m_guideCount; }
	void AddGuidePoint(UInt16 x, UInt16 y);
	void RemoveGuidePoint(UInt16 x, UInt16 y);
	void PreCalculate();
	void PreCalculate(Real32* weightData, Real32* distData, UInt16* linkData);
	void CreateMap(UInt16 width, UInt16 height);
	bool DrawLine(UInt16 srcX, UInt16 srcY, UInt16 dstX, UInt16 dstY,
					PointList& ptlist, float &dist, 
					bool passGuide = false, bool reverse = false);
	bool GetPath(UInt16 srcX, UInt16 srcY, UInt16 dstX, UInt16 dstY,
						float& dist, PointList& ptlist, GuideList& gresult);
	bool GetWholePath(UInt16 srcX, UInt16 srcY, UInt16 dstX, UInt16 dstY,
						PointList& ptlist);
	bool NextPathLine(UInt16 srcX, UInt16 srcY, UInt16 dstX, UInt16 dstY,
						PointList& ptlist, GuideList& gresult, UInt16& index);
	SymmetricGraph* GetWeightGraph() { return &m_weightGraph; }
	SymmetricGraph* GetDistGraph() { return &m_distGraph; }
	PathGraph* GetLinkGraph() { return &m_linkGraph; }

	inline SetBlockingActive(bool Value)
	{
		m_BlockingActive = Value;
	}
#ifdef DEBUG_PATHFIND
	void PrintMap(FILE *f);
	void PrintPath(UInt16 srcX, UInt16 srcY, UInt16 dstX, UInt16 dstY);
#endif // DEBUG_PATHFIND
};

/////////////////////////////////////////////////////////////////////////
///                        PathGraph Class                            ///
/////////////////////////////////////////////////////////////////////////
inline PathGraph::PathGraph()
{
	m_size = 0;
	m_graph = NULL;
}

inline PathGraph::PathGraph(UInt16 size)
{
	m_size = 0;
	m_graph = NULL;
	Create(size);
}

inline PathGraph::~PathGraph()
{
	SafeDeallocate(m_graph);
}

inline void PathGraph::Create(UInt16 size)
{
	UInt32 len = UInt32(size) * size;

	m_size = size;
	if (m_graph)
		SafeDeallocate(m_graph);
	m_graph = SafeAllocate(UInt16, len);
	ZeroMemory(m_graph, sizeof(UInt16) * len);
}

inline UInt16 PathGraph::Get(UInt16 x, UInt16 y)
{
	if (x < m_size && y < m_size)
		return m_graph[m_size * x + y];
	else
		return BK_BLOCKED;
}

inline void PathGraph::Set(UInt16 x, UInt16 y, UInt16 v)
{
	if (x < m_size && y < m_size)
		m_graph[m_size * x + y] = v;
}

inline void PathGraph::PrintDebug()
{
	UInt16 v;

	printf("x\\y ");
	for (UInt16 i = 0; i < m_size; i++)
	{
		printf("%3u|", i);
	}
	printf("\n");
	for (UInt16 i = 0; i < m_size; i++)
	{
		printf("%3u|", i);
		for (UInt16 j = 0; j < m_size; j++)
		{
			v = Get(i, j);
			if (v == BK_BLOCKED)
				printf("--- ");
			else
				printf("%3u ", v);
		}
		printf("\n");
	}
}

inline void PathGraph::Set(UInt16* data)
{
	memcpy(m_graph, data, (UInt32(m_size) * m_size) * sizeof(UInt16));
}

/////////////////////////////////////////////////////////////////////////
///                      SymmetricGraph Class                         ///
/////////////////////////////////////////////////////////////////////////
inline SymmetricGraph::SymmetricGraph()
{
	m_size = 0;
	m_graph = NULL;
}

inline SymmetricGraph::SymmetricGraph(UInt16 size)
{
	m_size = 0;
	m_graph = NULL;
	Create(size);
}

inline SymmetricGraph::~SymmetricGraph()
{
	SafeDeallocate(m_graph);
}

inline void SymmetricGraph::Create(UInt16 size)
{
	UInt32 len;

	m_size = size;
	len = UInt32(m_size) * (m_size - 1) / 2;
	if (m_graph)
		SafeDeallocate(m_graph);
	m_graph = SafeAllocate(float, len);
	ZeroMemory(m_graph, sizeof(float) * len);
}

inline float SymmetricGraph::Get(UInt16 x, UInt16 y)
{
	if (x == y)
		return 0;

	if (x > y) {
		UInt16 t;		// swapping
		t = x; x = y; y = t;
	}

	// half square formula for (x, y) to linear:
	// ((n - 1) + (n - x)) * x / 2 + (y - x - 1)
	if (x < m_size && y < m_size && x != y)
		return m_graph[(m_size * 2 - 1 - x) * x / 2 + y - x - 1];
	else
		return BK_BLOCKED;
}

inline void SymmetricGraph::Set(UInt16 x, UInt16 y, float v)
{
	if (x > y) {
		UInt16 t;		// swapping
		t = x; x = y; y = t;
	}

	// half square formula for (x, y) to linear:
	// ((n - 1) + (n - x)) * x / 2 + (y - x - 1)
	if (x < m_size && y < m_size && x != y)
		m_graph[(m_size * 2 - 1 - x) * x / 2 + y - x - 1] = v;
}

inline void SymmetricGraph::PrintDebug()
{
	float v;

	printf("x\\y   ");
	for (UInt16 i = 0; i < m_size; i++)
	{
		printf("%5u|", i);
	}
	printf("\n");
	for (UInt16 i = 0; i < m_size; i++)
	{
		printf("%5u|", i);
		for (UInt16 j = 0; j < m_size; j++)
		{
			v = Get(i, j);
			if (v == REAL32_MAX_VALUE)
				printf(" ---- ");
			else
				printf("%5.1f ", v);
		}
		printf("\n");
	}
}

inline void SymmetricGraph::Set(Real32* data)
{
	memcpy(m_graph, data, (UInt32(m_size) * (m_size - 1) / 2) * sizeof(Real32));
}

/////////////////////////////////////////////////////////////////////////
///                        PathFinder Class                           ///
/////////////////////////////////////////////////////////////////////////
inline void PathFinder::ConvToSBlock(UInt16 x, UInt16 y, Int16 &cx, Int16 &cy)
{
	cx = x - y / 2;
	cy = x + y / 2 + (y % 2);
}

inline bool PathFinder::ConvToUBlock(Int16 x, Int16 y, UInt16 &cx, UInt16 &cy)
{
	if (y < x)
		return false;

	cx = (x + y) / 2;
	cy = y - x;
	return true;
}

inline BlockInfo& PathFinder::GetMapBySBlock(Int16 x, Int16 y)
{
	UInt16 bx, by;
	ConvToUBlock(x, y, bx, by);
	return m_map[bx][by];
}

inline void PathFinder::AddGuidePoint(UInt16 x, UInt16 y)
{
	BlockPos pos;
	DEBUG_ENSURE_0(m_guideCount < BK_BLOCKED, _T("Excess guide point maximum size."));
	m_map[x][y].btype = m_guideCount++;
	pos.PosX = x;
	pos.PosY = y;
	m_guidePoints.push_back(pos);
}

inline void PathFinder::RemoveGuidePoint(UInt16 x, UInt16 y)
{
	BlockPos pos;
	pos.PosX = x;
	pos.PosY = y;
	PointList::iterator it = std::find(m_guidePoints.begin(), m_guidePoints.end(), pos);
	if (it != m_guidePoints.end())
		m_guidePoints.erase(it);
}

inline BlockPos& PathFinder::GetGuidePoint(int i)
{
	return m_guidePoints[i];
}

inline float PathFinder::ActualDistance(UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2)
{
	Int16 cx1, cy1, cx2, cy2;
	ConvToSBlock(x1, y1, cx1, cy1);
	ConvToSBlock(x2, y2, cx2, cy2);
	float sx = float(cx2 - cx1);
	float sy = float(cy2 - cy1);

	return sqrt((sx * sx) + (sy * sy));
}

inline float PathFinder::ActualDistance(Int16 x1, Int16 y1, Int16 x2, Int16 y2)
{
	float sx = float(x2 - x1);
	float sy = float(y2 - y1);

	return sqrt((sx * sx) + (sy * sy));
}

inline bool PathFinder::IsEmpty(UInt16 x, UInt16 y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return false;

	return (m_map[x][y].btype == BK_EMPTY);
}

inline bool PathFinder::IsGuidePoint(UInt16 x, UInt16 y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return false;

	return (m_map[x][y].btype != BK_BLOCKED && m_map[x][y].btype != BK_EMPTY);
}

inline bool PathFinder::IsBlocked(UInt16 x, UInt16 y)
{
	if (!m_BlockingActive)
		return false;

	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return true;

	return (m_map[x][y].btype == BK_BLOCKED);
}

inline UInt16 PathFinder::GetBlockType(UInt16 x, UInt16 y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return BK_BLOCKED;

	return m_map[x][y].btype;
}

inline void PathFinder::SetBlockType(UInt16 x, UInt16 y, UInt16 value)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return;

	m_map[x][y].btype = value;
}

struct PathFindData
{
	//path find list
	PathFinder::PointList m_ptList;
	StlDeque<BlockPos> PreSteps;
	PathFindData()
	{
	}
	VOID Init()
	{
		m_ptList.clear();	
	}

	VOID Clear()
	{
		m_ptList.clear();
		PreSteps.clear();
	}
};
#endif // _PATH_FINDER_H_
