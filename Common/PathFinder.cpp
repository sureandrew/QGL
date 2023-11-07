//-- Common
#include "Common.h"
//-- Self
#include "Common/PathFinder.h"
//-- Library
#include <map>
#include <algorithm>

#define ABS(X)		(((X) < 0) ? -(X) : (X))
#define SQRT2		1.41421356f

BlockPos	PathFinder::m_tmpPos[TMP_POS_SIZE] = {};
UInt16		PathFinder::m_tmpPosCount = 0;

PathFinder::PathFinder(void)
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
	m_guideCount = 0;
	m_blockActive = BOOLEAN_TRUE;
	m_allocated = BOOLEAN_FALSE;
	m_minGuide1 = UINT16_MAX_VALUE;
	m_minGuide2 = UINT16_MAX_VALUE;
	m_minDist = 0;
}

PathFinder::~PathFinder(void)
{
	FreeMap();
}

void PathFinder::FreeMap()
{
	if (m_map && m_allocated) {
		SafeDeallocate(m_map);
		m_allocated = BOOLEAN_FALSE;
	}
	m_map = NULL;
}

void PathFinder::SetMap(UInt16 *pMapData, UInt16 width, UInt16 height)
{
	TRACE_ENSURE_0(pMapData, _T("PathFinder SetMap: Null Map Data"));

	FreeMap();
	m_map = pMapData;
	m_allocated = BOOLEAN_FALSE;
	m_width = width;
	m_height = height;
	FindGuidePoints();
}

void PathFinder::CreateMap(UInt16 width, UInt16 height)
{
	FreeMap();
	m_map = (UInt16*) SafeAllocate(UInt16, width * height);
	m_allocated = BOOLEAN_TRUE;
	m_width = width;
	m_height = height;
	m_guidePoints.clear();
}

void PathFinder::AllocateGraph()
{
	m_linkGraph.Create(m_guideCount);
	m_distGraph.Create(m_guideCount);
	m_weightGraph.Create(m_guideCount);
}

void PathFinder::FindGuidePoints()
{
	if (m_map == NULL) return;

	UInt16 *p = m_map;
	BlockPos pos;
	m_guidePoints.clear();

	for (UInt16 y = 0; y < m_height; y++)
	{
		for (UInt16 x = 0; x < m_width; x++)
		{
			if ((*p & TILE_GUIDE) == TILE_GUIDE) {
				pos.PosX = x;
				pos.PosY = y;
				m_guidePoints.push_back(pos);
			}
			p++;
		}
	}
	m_guideCount = (UInt16) m_guidePoints.size();
}

void PathFinder::CalcGuildLink(Real32* weightData, Real32* distData, UInt16* linkData)
{
	m_weightGraph.Set(weightData);
	m_linkGraph.Set(linkData);
	m_distGraph.Set(distData);
}

void PathFinder::CalcGuildLink()
{
	PointList ptList;
	float dist;

	// travel single guide point
	for (UInt16 i = 0; i < m_guideCount; i++)
	{
		BlockPos &srcPos = m_guidePoints[i];
		for (UInt16 j = i + 1; j < m_guideCount; j++)
		{
			BlockPos &dstPos = m_guidePoints[j];
			if (DrawLine(srcPos.PosX, srcPos.PosY, dstPos.PosX, dstPos.PosY, NULL, dist)) {
				//m_weightGraph.Set(i, j, 
				//	ActualDistance(srcPos.PosX, srcPos.PosY, dstPos.PosX, dstPos.PosY));
				m_weightGraph.Set(i, j, dist); // (UInt16) ptList.size());
			} else {
				m_weightGraph.Set(i, j, REAL32_MAX_VALUE);
			}
		}
	}

	// recursive travel using Dijkstra's algorithm
	/*
	Dijkstra's algorithm
	1  function Dijkstra(G, w, s)
	2     for each vertex v in V[G]                        // Initializations
	3           d[v] := infinity                           // Unknown distance function from s to v
	4           previous[v] := undefined
	5     d[s] := 0                                        // Distance from s to s
	6     S := empty set                                   // Set of all visited vertices
	7     Q := V[G]                                        // Set of all unvisited vertices
	8     while Q is not an empty set                      // The algorithm itself
	9           u := Extract_Min(Q)                        // Remove best vertex from priority queue
	10           S := S union {u}                          // Mark it 'visited'
	11           for each edge (u,v) outgoing from u
	12                  if d[u] + w(u,v) < d[v]            // Relax (u,v)
	13                        d[v] := d[u] + w(u,v) 
	14                        previous[v] := u

	shortest path from s to t by iteration:
	1 S := empty sequence 
	2 u := t
	3 while defined previous[u]
	4       insert u to the beginning of S
	5       u := previous[u]
	*/
	FDistList openList;
	std::map<UInt16, bool> closeList;
	FDistList distList;
	std::map<UInt16, UInt16> prevList;
	FDistList::iterator iter;
	UInt16 u, v;				// current guide
	float min_v = 0.0f;
	float d;

	for (UInt16 s = 0; s < m_guideCount; s++)
	{
		openList.clear();
		for (v = s+1; v < m_guideCount; v++)
		{
			openList[v] = REAL32_MAX_VALUE;
			m_linkGraph.Set(s, v, BK_BLOCKED);
		}
		for (v = 0; v < m_guideCount; v++)
		{
			distList[v] = REAL32_MAX_VALUE;
		}
		openList[s] = 0;
		distList[s] = 0;
		closeList.clear();
		prevList.clear();
		while (!openList.empty())
		{
			// pop min distance guide
			iter = min_element(openList.begin(), openList.end());
			u = iter->first;
			openList.erase(iter);
			closeList[u] = true;
			for (v = 0; v < m_guideCount; v++)
			{
				if (u == v || m_weightGraph.Get(u, v) == REAL32_MAX_VALUE || distList[u] == REAL32_MAX_VALUE)
					continue;
				d = distList[u] + m_weightGraph.Get(u, v);
				if (d < distList[v]) {
					distList[v] = d;
					openList[v] = d;
					prevList[v] = u;
					min_v = v;
				}
			}
		}
		// print shortest path out
		m_linkGraph.Set(s, s, BK_BLOCKED);
		for (v = s+1; v < m_guideCount; v++)
		{
			u = v;
#if TRACE_PATHFIND > 1
			TRACE_INFODT_3(GLOBAL_LOGGER, _F("%u -> %u dist %3.1f: "), s, v, distList[v]);
#endif
			m_distGraph.Set(s, v, distList[v]);
			// skip if not path
			if (distList[v] == REAL32_MAX_VALUE) {
#if TRACE_PATHFIND > 1
				TRACE_INFODTL(GLOBAL_LOGGER, _F("no path"));
#endif
				m_linkGraph.Set(v, s, BK_BLOCKED);
				continue;
			}
			m_linkGraph.Set(v, s, prevList[v]);
			while (u != s)
			{
#if TRACE_PATHFIND > 1
				TRACE_INFO_1(GLOBAL_LOGGER, _F("%u < "), u);
#endif
				m_linkGraph.Set(s, v, u);
				u = prevList[u];
			}
#if TRACE_PATHFIND > 1
			TRACE_INFOL_1(GLOBAL_LOGGER, _("%u"), u);
#endif
		}
	}
}

void PathFinder::PreCalculate()
{
	// allocate graphs
	AllocateGraph();

	// calculate links and weights between each guide points
	CalcGuildLink();
}

void PathFinder::PreCalculate(Real32* weightData, Real32* distData, UInt16* linkData)
{
	// allocate graphs
	AllocateGraph();

	// calculate links and weights between each guide points
	CalcGuildLink(weightData, distData, linkData);
}

bool PathFinder::NextPathLine(UInt16 srcX, UInt16 srcY, UInt16 dstX, UInt16 dstY,
							  PointList& ptlist, GuideList& gresult, UInt16& index)
{
	UInt16 guide, guide2;
	float dist;

	ptlist.clear();
	if (index > (UInt16) gresult.size()) {
		return false;
	}

	if (!gresult.empty()) {
		if (index == 0) {
			guide = gresult.front();
			DrawLine(srcX, srcY, m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, &ptlist, dist);
		} else if (index == (UInt16) gresult.size()) {
			guide = gresult.back();
			DrawLine(m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, dstX, dstY, &ptlist, dist);
		} else {
			guide = gresult[index - 1];
			guide2 = gresult[index];
			if (guide > guide2) {
				DrawLine(m_guidePoints[guide2].PosX, m_guidePoints[guide2].PosY, 
					m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, &ptlist, dist);
				ptlist.pop_back();
				ptlist.push_front(m_guidePoints[guide2]);
				reverse(ptlist.begin(), ptlist.end());
			} else
				DrawLine(m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, 
					m_guidePoints[guide2].PosX, m_guidePoints[guide2].PosY, &ptlist, dist);
		}
	}
	index ++;

	return true;
}

// draw line and return true if drawing to end without blocked
bool PathFinder::DrawLine(UInt16 x1, UInt16 y1,	
						  UInt16 x2, UInt16 y2,
						  PointList* ptlist, float &dist, 
						  bool passGuide, bool reverse)
{
	Int16 cx1, cy1, cx2, cy2;			// converted pos1 and pos2
	Int16 nx1, ny1, nx2, ny2;			// next 2 moving positions
	Int16 dx, dy;						// directional x, y
	Int16 tx, ty;						// temp x, y
	Int16 px, py;						// previous position
	BlockPos ub;						// converted back to UBlock
	bool meetGuide = false;				// meet guide point?
	bool pass;							// can pass through?
	bool chgdir = false;				// can change direction only once

	if (ptlist)
		m_tmpPosCount = 0;
	dist = 0.0f;

	// convert source and destination block position to SBlock position
	ConvToSBlock(x1, y1, cx1, cy1);
	ConvToSBlock(x2, y2, cx2, cy2);
	px = cx1;
	py = cy1;

	// first direction
	tx = ABS(cx2 - cx1);
	ty = ABS(cy2 - cy1);
	if (tx * 5 < ty * 2) {			// aprox. less then 22.5 deg for x / y
		dx = 0;
		dy = SGN(cy2 - cy1);
	} else if (ty * 5 < tx * 2) {	// aprox. less then 22.5 deg for y / x
		dx = SGN(cx2 - cx1);
		dy = 0;
	} else {
		dx = SGN(cx2 - cx1);
		dy = SGN(cy2 - cy1);
	}

	while ((cx1 != cx2 || cy1 != cy2) && (passGuide || !meetGuide))
	{
		// try the straight direction to the destination
		if (!ConvToUBlock(cx1 + dx, cy1 + dy, ub.PosX, ub.PosY) || 
			IsBlocked(ub.PosX, ub.PosY) || (px == cx1 + dx && py == cy1 + dy))
		{
			// find the next 2 movable positions
			if (dx == 0) {
				// move in diagonal direction
				nx1 = cx1 + 1;
				ny1 = cy1 + dy;
				nx2 = cx1 - 1;
				ny2 = cy1 + dy;
			} else if (dy == 0) {
				// move in diagonal direction
				nx1 = cx1 + dx;
				ny1 = cy1 + 1;
				nx2 = cx1 + dx;
				ny2 = cy1 - 1;
			} else {
				// move in vertical or horizontal direction
				nx1 = cx1 + dx;
				ny1 = cy1;
				nx2 = cx1;
				ny2 = cy1 + dy;
			}

			// go the nearest movable position first
			if (ActualDistance(nx1, ny1, cx2, cy2) > ActualDistance(nx2, ny2, cx2, cy2))
			{
				// swap n1 and n2 position
				tx  = nx1;	ty  = ny1;
				nx1 = nx2;	ny1 = ny2;
				nx2 = tx;	ny2 = ty;
			}

			// try first moving position
			if (!ConvToUBlock(nx1, ny1, ub.PosX, ub.PosY) || 
				IsBlocked(ub.PosX, ub.PosY) || (nx1 == px && ny1 == py))
			{
				// try second moving position
				if (!ConvToUBlock(nx2, ny2, ub.PosX, ub.PosY) || 
					IsBlocked(ub.PosX, ub.PosY) || (nx2 == px && ny2 == py))
				{
					// totally blocked
					break;
				} else {
					if (IsGuidePoint(ub.PosX, ub.PosY))
						meetGuide = true;
					// add distance for vertical or horizontal move, or diagonal move distance
					dist += ((cx1 == nx2 || cy1 == ny2) ? 1.0f : SQRT2);
					//dx = SGN(nx2 - cx1);		// change direction
					//dy = SGN(ny2 - cy1);
					px = cx1;
					py = cy1;
					cx1 = nx2;
					cy1 = ny2;
				}
			} else {
				if (IsGuidePoint(ub.PosX, ub.PosY))
					meetGuide = true;
				// add distance for vertical or horizontal move, or diagonal move distance
				dist += ((cx1 == nx1 || cy1 == ny1) ? 1.0f : SQRT2);
				//dx = SGN(nx1 - cx1);			// change direction
				//dy = SGN(ny1 - cy1);
				px = cx1;
				py = cy1;
				cx1 = nx1;
				cy1 = ny1;
			}
		} else {
			if (IsGuidePoint(ub.PosX, ub.PosY))
				meetGuide = true;
			// add distance for vertical or horizontal move, or diagonal move distance
			dist += ((dx == 0 || dy == 0) ? 1.0f : SQRT2);
			px = cx1;
			py = cy1;
			cx1 = cx1 + dx;
			cy1 = cy1 + dy;
		}
		/*if (IsBlocked(ub.PosX, ub.PosY))
		{
			int a = 0;
			a ++;
		}*/

		if (ptlist && m_tmpPosCount < TMP_POS_SIZE)
			m_tmpPos[m_tmpPosCount++] = ub;

		// change direction if only need (optimized changes)
		if (dx != 0 && cx2 == cx1) {
			dx = 0;
			dy = SGN(cy2 - cy1);
		} else if (dy != 0 && cy2 == cy1) {
			dx = SGN(cx2 - cx1);
			dy = 0;
		} else if ((dx == 0 || dy == 0) && (ABS(cx2 - cx1) == ABS(cy2 - cy1))) {
			dx = SGN(cx2 - cx1);
			dy = SGN(cy2 - cy1);
		}
	}

	if (ptlist) {
		for (UInt16 i = 0; i < m_tmpPosCount; i++)
			ptlist->push_back(m_tmpPos[i]);
	}

	pass = (cx1 == cx2 && cy1 == cy2);
	if (!reverse && !pass && !IsBlocked(x2, y2) && (passGuide || !meetGuide))
	{
		PointList newPtlist;
		PointList* ptlist2 = (ptlist ? &newPtlist : NULL);
		float dist2;
		if (DrawLine(x2, y2, x1, y1, ptlist2, dist2, passGuide, true))
		{
			BlockPos pos;
			pos.PosX = x2;
			pos.PosY = y2;
			/*if (IsBlocked(pos.PosX, pos.PosY))
			{
				int a = 0;
				a ++;
			}*/
			if (ptlist) {
				ptlist2->pop_back();
				ptlist2->push_front(pos);
				ptlist->resize(ptlist2->size());
				std::reverse_copy(ptlist2->begin(), ptlist2->end(), ptlist->begin());
			}
			dist = dist2;
			pass = true;
		}
	}

	// find the nearest final points if not pass through
	if (ptlist && !reverse && !pass && passGuide && ptlist->size() > 1)
	{
		float d1 = ActualDistance(ptlist->back().PosX, ptlist->back().PosY, x2, y2);
		float d2;
		for (int i = (int)ptlist->size() - 2; i >= 0; i--)
		{
			d2 = ActualDistance((*ptlist)[i].PosX, (*ptlist)[i].PosY, x2, y2);
			if (d2 > d1)
				break;
			ptlist->erase(ptlist->begin() + i + 1);
			d1 = d2;
		}
	}

	// only valid if reach to the destination
	return pass;
}

bool PathFinder::GetPath(UInt16 x1, UInt16 y1,
						 UInt16 x2, UInt16 y2, float& dist,
						 PointList& ptlist, GuideList& gresult, 
						 UInt16 h1, UInt16 h2)
{
	bool passed;

	// reach destination or invalid source or destination
	if (x1 == x2 && y1 == y2)
		return false;

	dist = 0;
	gresult.clear();
	ptlist.clear();
	passed = DrawLine(x1, y1, x2, y2, &ptlist, dist, true);
	dist = (UInt16) ptlist.size();
	if (passed)
		return true;

	float minDist = REAL32_MAX_VALUE;

	m_minGuide1 = UINT16_MAX_VALUE;
	m_minGuide2 = UINT16_MAX_VALUE;
	m_minDist = 0;

	// try to use hint for faster finding if hint provided
	if (h1 < m_guideCount && h2 < m_guideCount) {
		float d1;
		float d2;
		// check if line can draw there and link not blocked between two hint guild points
		if (DrawLine(x1, y1, m_guidePoints[h1].PosX, m_guidePoints[h1].PosY, NULL, d1) && 
			DrawLine(m_guidePoints[h2].PosX, m_guidePoints[h2].PosY, x2, y2, NULL, d2) &&
			m_linkGraph.Get(h1, h2) != BK_BLOCKED)
		{
			m_minGuide1 = h1;
			m_minGuide2 = h2;
			minDist = m_distGraph.Get(h1, h2) + d1 + d2;
		}
	}

	// no hint
	if (m_minGuide1 == UINT16_MAX_VALUE) {
		FDistList distList1;
		FDistList distList2;
		FDistList::iterator iter1;
		FDistList::iterator iter2;
		float d;

		// find all possible guide points from source and destination
		for (int i = 0; i < m_guideCount; i++)
		{
			UInt16 &gx = m_guidePoints[i].PosX;
			UInt16 &gy = m_guidePoints[i].PosY;
			if (DrawLine(x1, y1, gx, gy, NULL, d))
			{
				distList1[i] = d; // (UInt16) points.size(); //ActualDistance(x1, y1, gx, gy);
			}
			if (DrawLine(gx, gy, x2, y2, NULL, d))
			{
				distList2[i] = d; // (UInt16) points.size(); //ActualDistance(gx, gy, x2, y2);
			}
		}

#ifdef TRACE_PATHFIND
		printf("find shortest path times: %d x %d = %d\n", (int) distList1.size(),
			(int) distList2.size(), (int) distList1.size() * distList2.size());
#endif
		if (distList1.empty() || distList2.empty()) return false;

		// find the shortest path from distList1 to distList2
		for (iter1 = distList1.begin(); iter1 != distList1.end(); iter1++)
		{
			for (iter2 = distList2.begin(); iter2 != distList2.end(); iter2++)
			{
				if (iter1->first == iter2->first || 
					m_linkGraph.Get(iter1->first, iter2->first) != BK_BLOCKED)
				{
					// (dist to guide1) + (dist from guide1 to guide2) + (dist to guide2)
					d = m_distGraph.Get(iter1->first, iter2->first) +
						iter1->second + iter2->second;
					if (d < minDist) {
							m_minGuide1 = iter1->first;
							m_minGuide2 = iter2->first;
							minDist = d;
					}
				}
			}
		}
	}

	if (m_minGuide1 == UINT16_MAX_VALUE)
		return false;

	dist = minDist;
	m_minDist = (UInt16) minDist;

	// find the guide point between m_minGuide1 and m_minGuide2
	UInt16 g = m_minGuide1;

	gresult.push_back(g);
	while (g != m_minGuide2)
	{
		g = m_linkGraph.Get(g, m_minGuide2);
		gresult.push_back(g);
	}

	return true;
}

bool PathFinder::GetWholePath(UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2,
							  PointList& ptlist, 
							  UInt16 hintGuide1, UInt16 hintGuide2)

{
	float dist;
	GuideList gresult;
	
	if (!GetPath(x1, y1, x2, y2, dist, ptlist, gresult, hintGuide1, hintGuide2))
		return false;

	if (!gresult.empty()) {
		UInt16 index = 0;
		PointList segment;

		ptlist.clear();
		while(NextPathLine(x1, y1, x2, y2, segment, gresult, index))
			ptlist.insert(ptlist.end(), segment.begin(), segment.end());
	}

	return true;
}

#ifdef TRACE_PATHFIND
void PathFinder::PrintPath(UInt16 srcX, UInt16 srcY, UInt16 dstX, UInt16 dstY)
{
	FILE *f;
	f = _tfopen(_T("pathfind.log"), _T("wt"));
	if (f == NULL)
		return;

	PathFinder::PointList ptlist;
	PathFinder::GuideList gresult;
	float dist;

	GetPath(srcX, srcY, dstX, dstY, dist, ptlist, gresult);

	UInt16 index = 0;
	MarkBlockType(srcX, srcY, TILE_STMARK);
	MarkBlockType(dstX, dstY, TILE_EDMARK);

	while(NextPathLine(srcX, srcY, dstX, dstY, ptlist, gresult, index))
	{
		for (PathFinder::PointList::iterator iter = ptlist.begin();
			iter != ptlist.end(); iter++)
		{
			UInt16 b = GetBlockType(iter->PosX, iter->PosY);
			if (b & TILE_NORMAL == TILE_NORMAL)
				MarkBlockType(iter->PosX, iter->PosY, TILE_MARKED);
		}
	}

	PrintMap(f);

	fprintf(f, "Test Path (%u, %u) to (%u, %u) ...\n", srcX, srcY, dstX, dstY);
	fprintf(f, "guides: ");
	for (int i = 0; i < (int) gresult.size(); i++)
	{
		BlockPos &pos = GetGuidePoint(gresult[i]);
		fprintf(f, "step %d: guide = %d, %d, %d\n", i++, gresult[i], pos.PosX, pos.PosY);
	}
	fprintf(f, "\nsteps: %u\n", dist);

	// unmark points
	UnmarkBlockType(srcX, srcY, TILE_STMARK);
	UnmarkBlockType(dstX, dstY, TILE_EDMARK);
	for (int y = 0; y < GetHeight(); y++)
	{
		for (int x = 0; x < GetWidth(); x++)
		{
			UInt16 b = GetBlockType(x, y);
			if (b & TILE_MARKED == TILE_MARKED)
				UnmarkBlockType(x, y, TILE_MARKED);
		}
	}

	fclose(f);
}

void PathFinder::PrintMap(FILE *f)
{
	int gi;
	int b;

	for (int y = 0; y < m_height; y++)
	{
		if ((y % 2) == 1)
			fprintf(f, "  ");
		for (int x = 0; x < m_width; x++)
		{
			b = GetBlockType(x, y);
			if (b & TILE_NORMAL == TILE_NORMAL) {
				gi = GetGuideIndex(x, y);
				if (gi != -1)
					fprintf(f, "%3d ", gi);
				else
					fprintf(f, "  . ");
			} else if (b & TILE_BLOCK == TILE_BLOCK)
				fprintf(f, "  # ");
			else if (b & TILE_MARKED == TILE_MARKED) {
				gi = GetGuideIndex(x, y);
				if (gi != -1)
					fprintf(f, "%3do", gi);
				else
					fprintf(f, "  o ");
			} else if (b & TILE_STMARK == TILE_STMARK)
				fprintf(f, " [S]");
			else if (b & TILE_EDMARK == TILE_EDMARK)
				fprintf(f, " [E]");
		}
		fprintf(f, "\n");
	}
}


#endif // TRACE_PATHFIND
