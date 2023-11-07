#ifndef _CROSS_MAP_PATH_FINDER_
#define _CROSS_MAP_PATH_FINDER_

//#ifdef AUTOPATHON

class CCrossMapPathFinder
{
private:
	CCrossMapPathFinder();
	~CCrossMapPathFinder();
	typedef struct _POINT_DISTANCE_
	{
		UInt32Vector uvPoints;
		UInt32 uDistance;
	}POINTDISTANCE,PPOINTDISTANCE;
	typedef StlVector<POINTDISTANCE,ManagedAllocator<POINTDISTANCE> > PointDistanceVec;
public:
	static CCrossMapPathFinder* GetInstance();
public:
	int CreateBestPath(const UInt32 uSrcMap,const BlockPos bpSrc,
				 const UInt32 uDestMap,const BlockPos bpDest);
	UInt32 GetFirstPathPoint();
	UInt32 GetNextPathPoint();
	UInt32 GetCurPathPoint();
	void GetSrcPoint(BlockPos& bpSrc);
	void GetDestPoint(BlockPos& bpDest);
	void DestroyPathData();
	BOOL GetActiveState() const {return m_bActive;}
	BOOL SetActiveState(BOOL bActive) { return m_bActive=bActive;}
protected:
	int GetSourcePoints(const UInt32 uMapId,UInt32Vector& uvNewPoints);
	int RemoveUsedPoints(UInt32Vector& uvNewPoints,
							UInt32Vector& uvSelectedPoints,
							UInt32Vector& uvDropedPoints);
	int RemoveNewOpenedPoints(UInt32Vector& uvAllPoints,UInt32Vector& uvNewPoints);
	int GenerateNewPath(UInt32 uCurSeq,UInt32Vector uvNewPoints,PointDistanceVec& pdvOpenedPath);
	int ChooseNewClosedPath(PointDistanceVec& pdvOpenedPath,
							PointDistanceVec& pdvClosedPath,
							UInt32Vector& uvSelectedPoints);
	int GetNextPoints(UInt32 uCurPoints,UInt32Vector& uvNewPoints);
	int ReservePath(UInt32Vector& uvSelectedPoints,
					UInt32Vector& uvDropedPoints,
					PointDistanceVec& pdvOpenedPath,
					PointDistanceVec& pdvClosedPath);
private:
	BOOL m_bActive;
	UInt32Vector m_uvPathPoint;
	BlockPos m_bpSrc;
	BlockPos m_bpDest;
	int m_iCurPointIndex;
	static CCrossMapPathFinder* m_pInstance;
};

//#endif

#endif