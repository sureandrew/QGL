#include "Reuben/System/Reuben.h"
#include "Reuben/Diagnostics/Debug.h"
#include <map>
#include <algorithm>
#include "PathFinder.h"

#define MIN(X, Y)		((X) < (Y) ? (X) : (Y))
#define MAX(X, Y)		((X) > (Y) ? (X) : (Y))
#define WITHIN(X, A, B)		((X) > (A) ? ((X) < (B) ? (X) : (B)) : (A))
#define SGN(X)		((X) == 0 ? 0 : (((X) < 0) ? -1 : 1))
#define ABS(X)		(((X) < 0) ? -(X) : (X))
#define SQRT2		1.41421356f

PathFinder::PathFinder(void)
{
	m_map = NULL;
	m_width = 0;
	m_height = 0;
	m_guideCount = 0;
	m_BlockingActive = true;
}

PathFinder::~PathFinder(void)
{
	FreeMap();
}

void PathFinder::FreeMap()
{
	if (m_map) {
		for (UInt16 x = 0; x < m_width; x++) {
			SafeDeallocate(m_map[x]);
		}
		SafeDeallocate(m_map);
	}
}

void PathFinder::CreateMap(UInt16 width, UInt16 height)
{
	FreeMap();
	m_map = (BlockInfo**) SafeAllocate(BlockInfo*, width);

	for (int x = 0; x < width; x++) {
		m_map[x] = (BlockInfo*) SafeAllocate(BlockInfo, height);
		FillMemory(m_map[x], sizeof(BlockInfo) * height, BK_EMPTY);
	}

	m_width = width;
	m_height = height;
	m_guidePoints.clear();
	m_guideCount = 0;
}

void PathFinder::AllocateGraph()
{
	m_linkGraph.Create(m_guideCount);
	m_distGraph.Create(m_guideCount);
	m_weightGraph.Create(m_guideCount);
}

void PathFinder::CalcGuildLink(Real32* weightData, Real32* distData, UInt16* linkData)
{
	PointList ptList;
	float dist;

	m_weightGraph.Set(weightData);
	m_linkGraph.Set(linkData);
	m_linkGraph.PrintDebug();
	m_distGraph.Set(distData);
	m_distGraph.PrintDebug();
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
			if (DrawLine(srcPos.PosX, srcPos.PosY, dstPos.PosX, dstPos.PosY, ptList, dist)) {
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
#if DEBUG_PATHFIND > 1
			printf("%u -> %u dist %3.1f: ", s, v, distList[v]);
#endif
			m_distGraph.Set(s, v, distList[v]);
			// skip if not path
			if (distList[v] == REAL32_MAX_VALUE) {
#if DEBUG_PATHFIND > 1
				printf("no path\n");
#endif
				m_linkGraph.Set(v, s, BK_BLOCKED);
				continue;
			}
			m_linkGraph.Set(v, s, prevList[v]);
			while (u != s)
			{
#if DEBUG_PATHFIND > 1
				printf("%u < ", u);
#endif
				m_linkGraph.Set(s, v, u);
				u = prevList[u];
			}
#if DEBUG_PATHFIND > 1
			printf("%u\n", u);
#endif
		}
	}
}

/*
UInt16 PathFinder::FindNearestGuide(UInt16 x, UInt16 y)
{
	UInt16 guide = BK_BLOCKED;
	UInt16 minStep = BK_BLOCKED;
	PointList ptList;

	for (UInt16 i = 0; i < m_guideCount; i++)
	{
		BlockPos &pos = m_guidePoints[i];
		if (DrawLine(x, y, pos.PosX, pos.PosY, ptList)) {
			if ((UInt16) ptList.size() < minStep) {
				guide = i;
				minStep = (UInt16) ptList.size();
			}
		}
	}

	return guide;
}
*/

void PathFinder::PreCalculate()
{
	// allocate graphs
	AllocateGraph();

	// calculate links and weights between each guide points
	CalcGuildLink();

	/*
	// find all empty block points to nearest guide point
	UInt16 x, y;
	for (y = 0; y < m_height; y++)
	{
		for (x = 0; x < m_width; x++)
		{
			if (m_map[x][y].btype != BK_BLOCKED)
				m_map[x][y].nearGuide = FindNearestGuide(x, y);
		}
	}
	*/
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

	if (index > (UInt16) gresult.size()) {
		return false;
	}

	if (!gresult.empty()) {
		if (index == 0) {
			guide = gresult.front();
			DrawLine(srcX, srcY, m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, ptlist, dist);
		} else if (index == (UInt16) gresult.size()) {
			guide = gresult.back();
			DrawLine(m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, dstX, dstY, ptlist, dist);
		} else {
			guide = gresult[index - 1];
			guide2 = gresult[index];
			if (guide > guide2) {
				DrawLine(m_guidePoints[guide2].PosX, m_guidePoints[guide2].PosY, 
					m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, ptlist, dist);
				ptlist.pop_back();
				ptlist.insert(ptlist.begin(), m_guidePoints[guide2]);
				reverse(ptlist.begin(), ptlist.end());
			} else
				DrawLine(m_guidePoints[guide].PosX, m_guidePoints[guide].PosY, 
					m_guidePoints[guide2].PosX, m_guidePoints[guide2].PosY, ptlist, dist);
		}
	}
	index ++;

	return true;
}

// draw line and return true if drawing to end without blocked
bool PathFinder::DrawLine(UInt16 x1, UInt16 y1,	
						  UInt16 x2, UInt16 y2,
						  PointList& ptlist, float &dist, 
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

	// clear point list first
	ptlist.clear();
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
		ptlist.push_back(ub);

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
		//} else if (ActualDistance(cx1 + dx, cy1 + dy, cx2, cy2) >= 
		//			ActualDistance(cx1, cy1, cx2, cy2)) {
		//	// check again when distance is more far away
		//	tx = ABS(cx2 - cx1);
		//	ty = ABS(cy2 - cy1);
		//	if (tx * 5 < ty * 2) {			// aprox. less then 22.5 deg for x / y
		//		dx = 0;
		//		dy = SGN(cy2 - cy1);
		//	} else if (ty * 5 < tx * 2) {	// aprox. less then 22.5 deg for y / x
		//		dx = SGN(cx2 - cx1);
		//		dy = 0;
		//	} else {
		//		dx = SGN(cx2 - cx1);
		//		dy = SGN(cy2 - cy1);
		//	}
	}

	pass = (cx1 == cx2 && cy1 == cy2);
	if (!reverse && !pass && (passGuide || !meetGuide))
	{
		PointList ptlist2;
		float dist2;
		if (DrawLine(x2, y2, x1, y1, ptlist2, dist2, passGuide, true))
		{
			BlockPos pos;
			pos.PosX = x2;
			pos.PosY = y2;
			ptlist2.pop_back();
			ptlist2.insert(ptlist2.begin(), pos);
			ptlist.resize(ptlist2.size());
			std::reverse_copy(ptlist2.begin(), ptlist2.end(), ptlist.begin());
			dist = dist2;
			pass = true;
		}
	}

	// find the nearest final points if not pass through
	if (!reverse && !pass && passGuide && ptlist.size() > 1)
	{
		float d1 = ActualDistance(ptlist.back().PosX, ptlist.back().PosY, x2, y2);
		float d2;
		for (int i = (int)ptlist.size() - 2; i >= 0; i--)
		{
			d2 = ActualDistance(ptlist[i].PosX, ptlist[i].PosY, x2, y2);
			if (d2 > d1)
				break;
			ptlist.erase(ptlist.begin() + i + 1);
			d1 = d2;
		}
	}

	// only valid if reach to the destination
	return pass;
}

bool PathFinder::GetPath(UInt16 x1, UInt16 y1,
						 UInt16 x2, UInt16 y2, float& dist,
						 PointList& ptlist, GuideList& gresult)
{
	bool passed;

	// reach destination or invalid source or destination
	if (x1 == x2 && y1 == y2)
		return false;

	dist = 0;
	gresult.clear();
	passed = DrawLine(x1, y1, x2, y2, ptlist, dist, true);
	dist = (UInt16) ptlist.size();
	if (passed)
		return true;

	/*
	// can't directly go through, then find guide points
	UInt16 g1 = m_map[x1][y1].nearGuide;
	UInt16 g2 = m_map[x2][y2].nearGuide;

	// if source or destination point haven't guide point
	// or guide points are not connected
	if (g1 == BK_BLOCKED || g2 == BK_BLOCKED || 
		m_linkGraph.Get(g1, g2) == BK_BLOCKED)
		return true;
	*/

	PointList points;
	FDistList distList1;
	FDistList distList2;
	FDistList::iterator iter1;
	FDistList::iterator iter2;
	UInt16 minGuide1 = UINT16_MAX_VALUE;
	UInt16 minGuide2 = UINT16_MAX_VALUE;
	float minDist = REAL32_MAX_VALUE;
	float d;
	
	/*
	// find possible guide points from source and destination linked by nearest guide points
	for (int i = 0; i < m_guideCount; i++)
	{
		UInt16 &gx = m_guidePoints[i].PosX;
		UInt16 &gy = m_guidePoints[i].PosY;
		if ((i == g1 || m_weightGraph.Get(g1, i) != UINT16_MAX_VALUE) && 
			DrawLine(x1, y1, gx, gy, points))
		{
			distList1[i] = (UInt16) points.size(); //ActualDistance(x1, y1, gx, gy);
		}
		if ((i == g2 || m_weightGraph.Get(g2, i) != UINT16_MAX_VALUE) && 
			DrawLine(gx, gy, x2, y2, points))
		{
			distList2[i] = (UInt16) points.size(); //ActualDistance(gx, gy, x2, y2);
		}
	}
	*/
	// find all possible guide points from source and destination
	for (int i = 0; i < m_guideCount; i++)
	{
		UInt16 &gx = m_guidePoints[i].PosX;
		UInt16 &gy = m_guidePoints[i].PosY;
		if (DrawLine(x1, y1, gx, gy, points, d))
		{
			distList1[i] = d; // (UInt16) points.size(); //ActualDistance(x1, y1, gx, gy);
		}
		if (DrawLine(gx, gy, x2, y2, points, d))
		{
			distList2[i] = d; // (UInt16) points.size(); //ActualDistance(gx, gy, x2, y2);
		}
	}

#ifdef DEBUG_PATHFIND
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
					(UInt16) m_distGraph.Get(iter1->first, iter2->first);
				if (d < minDist) {
						minGuide1 = iter1->first;
						minGuide2 = iter2->first;
						minDist = d;
				}
			}
		}
	}

	if (minGuide1 == UINT16_MAX_VALUE)
		return false;

	dist = minDist;
	// find the guide point between minGuide1 and minGuide2
	gresult.push_back(minGuide1);
	while (minGuide1 != minGuide2)
	{
		minGuide1 = m_linkGraph.Get(minGuide1, minGuide2);
		gresult.push_back(minGuide1);
	}

	return true;
}

bool PathFinder::GetWholePath(UInt16 x1, UInt16 y1,
							  UInt16 x2, UInt16 y2,
							  PointList& ptlist)
{
	float dist;
	GuideList gresult;
	
	if (!GetPath(x1, y1, x2, y2, dist, ptlist, gresult))
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

#ifdef DEBUG_PATHFIND
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
	UInt16 srcBlk = GetBlockType(srcX, srcY);
	UInt16 dstBlk = GetBlockType(dstX, dstY);
	SetBlockType(srcX, srcY, BK_STMARK);
	SetBlockType(dstX, dstY, BK_EDMARK);

	while(NextPathLine(srcX, srcY, dstX, dstY, ptlist, gresult, index))
	{
		for (PathFinder::PointList::iterator iter = ptlist.begin();
			iter != ptlist.end(); iter++)
		{
			UInt16 b = GetBlockType(iter->PosX, iter->PosY);
			if (b == BK_EMPTY)
				SetBlockType(iter->PosX, iter->PosY, BK_MARKED);
			else if (b < 0x4000)
				SetBlockType(iter->PosX, iter->PosY, b + 0x4000);
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
	SetBlockType(srcX, srcY, srcBlk);
	SetBlockType(dstX, dstY, dstBlk);
	for (int y = 0; y < GetHeight(); y++)
	{
		for (int x = 0; x < GetWidth(); x++)
		{
			UInt16 b = GetBlockType(x, y);
			if (b == BK_MARKED)
				SetBlockType(x, y, BK_EMPTY);
			else if (b >= 0x4000 && b <= 0xF000)
				SetBlockType(x, y, b - 0x4000);
		}
	}

	fclose(f);
}

void PathFinder::PrintMap(FILE *f)
{
	for (int y = 0; y < m_height; y++)
	{
		if ((y % 2) == 1)
			fprintf(f, "  ");
		for (int x = 0; x < m_width; x++)
		{
			if (m_map[x][y].btype == BK_EMPTY)
				fprintf(f, "  . ");
			else if (m_map[x][y].btype == BK_BLOCKED)
				fprintf(f, "  # ");
			else if (m_map[x][y].btype == BK_MARKED)
				fprintf(f, "  o ");
			else if (m_map[x][y].btype == BK_STMARK)
				fprintf(f, " [S]");
			else if (m_map[x][y].btype == BK_EDMARK)
				fprintf(f, " [E]");
			else if (m_map[x][y].btype >= 0x4000)
				fprintf(f, "%3uo", m_map[x][y].btype - 0x4000);		// marked guide point
			else
				fprintf(f, "%3u ", m_map[x][y].btype);		// guide point only
		}
		fprintf(f, "\n");
	}
}



#endif // DEBUG_PATHFIND