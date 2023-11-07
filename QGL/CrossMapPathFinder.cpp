#include "Common.h"
#include "common_binary.h"
#include "qgl.h"
#include "Global.h"
#include "CharacterControl.h"
#include "Math.h"
#include "../Resource/ResAutoPath.h"
#include "../Resource/AutoPath.h"
#include "../Resource/ClientResManager.h"
#include "CrossMapPathFinder.h"

//#ifdef AUTOPATHON

#pragma warning(disable:4018)
#pragma warning(disable:4267)

CCrossMapPathFinder* CCrossMapPathFinder::m_pInstance = NULL;

CCrossMapPathFinder* CCrossMapPathFinder::GetInstance()
{
	if (NULL == m_pInstance) 
	{
		static CCrossMapPathFinder gCrossMapPathFinder;
		m_pInstance = &gCrossMapPathFinder;
	}
	return m_pInstance;
}

CCrossMapPathFinder::CCrossMapPathFinder()
	:m_iCurPointIndex(0)
	,m_bActive(FALSE)
{
	m_uvPathPoint.clear();
	m_bpSrc.PosX = 0;
	m_bpSrc.PosY = 0;
	m_bpDest.PosX = 0;
	m_bpDest.PosY = 0;
}
CCrossMapPathFinder::~CCrossMapPathFinder()
{
	m_uvPathPoint.clear();
}
int CCrossMapPathFinder::CreateBestPath(const UInt32 uSrcMap,const BlockPos bpSrc, const UInt32 uDestMap,const BlockPos bpDest)
{
	int iRet = 0;
	if (uSrcMap == uDestMap)//Í¬µØÍ¼ 
	{
		return iRet;
	}
	BlockPos bpPrePos = bpSrc;
	//Init All JumpPoints;
	UInt32Vector uvAllPoints;
	UInt32 uJumpPointNum =(UInt32)gGlobal.m_resource->GetAutoPathDataSize();
	if (uJumpPointNum > 0) 
	{
		for (UInt32 i=0;i<uJumpPointNum;i++) 
		{
			uvAllPoints.push_back(i+1);
		}
	}
	else
		return iRet;
	//Get Start Points set open
	UInt32Vector uvNewPoints;
	UInt32Vector uvSelectedPoints;
	UInt32Vector uvDropPoints;
	PointDistanceVec pdvClosedPath;
	PointDistanceVec pdvOpenedPath;
	uvDropPoints.clear();
	int iPointsNum = GetSourcePoints(uSrcMap,uvNewPoints);
	RemoveNewOpenedPoints(uvAllPoints,uvNewPoints);
	for (UInt32Vector::iterator it = uvNewPoints.begin();
		it != uvNewPoints.end();it++) 
	{
		const JUMPPOINT* pjp = gGlobal.m_resource->GetAutoPathData(*it);
		if (NULL != pjp) 
		{
			UInt32 uDistance;
			if (uSrcMap == pjp->uMapId) 
			{
				BlockPos bpNow;
				bpNow.PosX = pjp->u16PosX;
				bpNow.PosY = pjp->u16PosY;
				PixelDirection pdct;
				uDistance = QGLMath::CalculateBlockPosDistance(bpSrc,bpNow,pdct);
			}
			else
			{
				uDistance = 0;
			}
			POINTDISTANCE pdNow;
			pdNow.uDistance = uDistance;
			pdNow.uvPoints.push_back(*it);
			pdvOpenedPath.push_back(pdNow);
		}
	}
	//select first shortest path
	int iMinOpenPath = 0;
	for (int i=1;i< pdvOpenedPath.size();i++) 
	{
		if (pdvOpenedPath[i-1].uDistance > pdvOpenedPath[i].uDistance) 
		{
			iMinOpenPath = i;
		}
	}
	if (pdvOpenedPath.size() > 0) 
	{
		pdvClosedPath.push_back(pdvOpenedPath[iMinOpenPath]);
		uvSelectedPoints.push_back(pdvOpenedPath[iMinOpenPath].uvPoints[0]);
	}
	else
		return iRet;
	//Generate shortest path
	BOOL bOver = TRUE;
	while (bOver) 
	{
		//get next points
		iPointsNum = 0;
		uvNewPoints.clear();
		if (uvSelectedPoints.size() > 0) 
		{
			GetNextPoints(uvSelectedPoints[uvSelectedPoints.size()-1],uvNewPoints);
			//remove used point From new points;
			iPointsNum = RemoveUsedPoints(uvNewPoints,uvSelectedPoints,uvDropPoints);
		}
		if (iPointsNum > 0)//still have path 
		{
			//remove opened points from all points;
			RemoveNewOpenedPoints(uvAllPoints,uvNewPoints);
			//Generate Opened Path according New Opened Points
			GenerateNewPath(uvSelectedPoints[uvSelectedPoints.size()-1],
				uvNewPoints,pdvOpenedPath);
			//choose new shortest path from opened path And Add new points Used in shortest path
			int iNewPath = ChooseNewClosedPath(pdvOpenedPath,pdvClosedPath,uvSelectedPoints);
			//check if find the path to distinct
			if (0 != iNewPath) 
			{
				if (uvSelectedPoints.size() > 0) 
				{
					ReservePath(uvSelectedPoints,uvDropPoints,pdvOpenedPath,pdvClosedPath);
				}
				else
				{
					bOver = FALSE;
				}
				continue;
			}
			if (pdvClosedPath.size() > 0) 
			{
				POINTDISTANCE& pds = pdvClosedPath[pdvClosedPath.size()-1];
				if (pds.uvPoints.size() > 0) 
				{
					const JUMPPOINT* pjpNew = gGlobal.m_resource->GetAutoPathData(pds.uvPoints[pds.uvPoints.size()-1]);
					if (NULL != pjpNew)
					{
						if (pjpNew->uMapId == uDestMap)//reached the distinct map
						{
							for (int iResault=0;iResault<pds.uvPoints.size();iResault++) 
							{
								m_uvPathPoint.push_back(pds.uvPoints[iResault]);
							}
							iRet = (int)m_uvPathPoint.size();
							m_bpSrc = bpSrc;
							m_bpDest = bpDest;
							bOver = FALSE;
						}
					}
				}
			}
		}
		else//no path to generate,need reserve the path
		{
			if (0 == uvAllPoints.size())// no path can reach
			{
				bOver = FALSE;
			}
			else//still have points not open
			{
				if (uvSelectedPoints.size() > 0) 
				{
					ReservePath(uvSelectedPoints,uvDropPoints,pdvOpenedPath,pdvClosedPath);
				}
				else//no selected point,all path reserved.the distinct unreachable
				{
					bOver = FALSE;
				}
			}
		}
	}
	//record the shortest path
	//clear all used vector
	for (PointDistanceVec::iterator itClose = pdvClosedPath.begin();
		itClose != pdvClosedPath.end();itClose++) 
	{
		(*itClose).uvPoints.clear();
	}

	pdvClosedPath.clear();
	for (PointDistanceVec::iterator itOpen = pdvOpenedPath.begin();
		itOpen != pdvOpenedPath.end();itOpen++) 
	{
		(*itOpen).uvPoints.clear();
	}

	pdvOpenedPath.clear();
	uvDropPoints.clear();
	uvSelectedPoints.clear();
	uvNewPoints.clear();
	uvAllPoints.clear();
	return iRet;
}

UInt32 CCrossMapPathFinder::GetFirstPathPoint()
{
	UInt32 uRet = UINT_MAX;
	m_iCurPointIndex = 0;
	if (m_iCurPointIndex < m_uvPathPoint.size()) 
	{
		uRet = m_uvPathPoint[m_iCurPointIndex];
	}
	return uRet;
}

UInt32 CCrossMapPathFinder::GetNextPathPoint()
{
	UInt32 uRet = UINT_MAX;
	m_iCurPointIndex++;
	if (m_iCurPointIndex < m_uvPathPoint.size()) 
	{
		uRet = m_uvPathPoint[m_iCurPointIndex];
	}
	return uRet;
}

UInt32 CCrossMapPathFinder::GetCurPathPoint()
{
	UInt32 uRet = UINT_MAX;
	if (m_iCurPointIndex < m_uvPathPoint.size()) 
	{
		uRet = m_uvPathPoint[m_iCurPointIndex];
	}
	return uRet;
}

void CCrossMapPathFinder::DestroyPathData()
{
	m_iCurPointIndex = 0;
	m_bpSrc.PosX = 0;
	m_bpSrc.PosY = 0;
	m_bpDest.PosX = 0 ;
	m_bpDest.PosY = 0;
	m_bActive = FALSE;
	m_uvPathPoint.clear();
}

int CCrossMapPathFinder::GetSourcePoints(const UInt32 uMapId,
										 UInt32Vector& uvNewPoints)
{
	return gGlobal.m_resource->GetMapJumpPoints(uMapId,uvNewPoints);
}
int CCrossMapPathFinder::RemoveUsedPoints(UInt32Vector& uvNewPoints,
											  UInt32Vector& uvSelectedPoints,
											  UInt32Vector& uvDropedPoints)
{
	//remove selected point
	for (UInt32Vector::iterator it = uvSelectedPoints.begin();
		it != uvSelectedPoints.end();it++) 
	{
		for(UInt32Vector::iterator itNew = uvNewPoints.begin();
			itNew != uvNewPoints.end();itNew++)
		{
			if (*it == *itNew) 
			{
				uvNewPoints.erase(itNew);
				break;
			}
		}
	}
	//remove dropped point
	for (UInt32Vector::iterator it = uvDropedPoints.begin();
		it != uvDropedPoints.end();it++) 
	{
		for(UInt32Vector::iterator itNew = uvNewPoints.begin();
			itNew != uvNewPoints.end();itNew++)
		{
			if (*it == *itNew) 
			{
				uvNewPoints.erase(itNew);
				break;
			}
		}
	}
	return (int)(uvNewPoints.size());
}
int CCrossMapPathFinder::RemoveNewOpenedPoints(UInt32Vector& uvAllPoints,
											   UInt32Vector& uvNewPoints)
{
	for (UInt32Vector::iterator it = uvNewPoints.begin();
		it != uvNewPoints.end();it++) 
	{
		for(UInt32Vector::iterator itNew = uvAllPoints.begin();
			itNew != uvAllPoints.end();itNew++)
		{
			if (*it == *itNew)
			{
				uvAllPoints.erase(itNew);
				break;
			}
		}

	}
	return (int)(uvAllPoints.size());
}
int CCrossMapPathFinder::GenerateNewPath(UInt32 uCurSeq,
										 UInt32Vector uvNewPoints,
										 PointDistanceVec& pdvOpenedPath)
{
	const JUMPPOINT* pjpPre = gGlobal.m_resource->GetAutoPathData(uCurSeq);
	if (NULL != pjpPre) 
	{
		//find the path with the end point == uCurSeq
		POINTDISTANCE ptSameWithCurSeq;
		for (PointDistanceVec::iterator itOpen = pdvOpenedPath.begin();
			itOpen != pdvOpenedPath.end();itOpen++) 
		{
			if ((*itOpen).uvPoints.size() > 0) 
			{
				if ((*itOpen).uvPoints[(*itOpen).uvPoints.size()-1] == uCurSeq) 
				{
					ptSameWithCurSeq.uDistance = (*itOpen).uDistance;
					for (int l=0;l < (*itOpen).uvPoints.size();l++) 
					{
						ptSameWithCurSeq.uvPoints.push_back((*itOpen).uvPoints[l]);
					}
					pdvOpenedPath.erase(itOpen);
					break;
				}
			}
		}

		PointDistanceVec pdvOpenTemp;
		for (UInt32Vector::iterator it = uvNewPoints.begin();
			it != uvNewPoints.end();it++) 
		{
			const JUMPPOINT* pjp = gGlobal.m_resource->GetAutoPathData(*it);
			if (NULL != pjp) 
			{
				UInt32 uDistance;
				if (pjpPre->uMapId == pjp->uMapId) 
				{
					BlockPos bpNow;
					bpNow.PosX = pjp->u16PosX;
					bpNow.PosY = pjp->u16PosY;
					BlockPos bpPre;
					bpPre.PosX = pjpPre->u16PosX;
					bpPre.PosY = pjpPre->u16PosY;
					PixelDirection pdct;
					uDistance = QGLMath::CalculateBlockPosDistance(bpPre,bpNow,pdct);
				}
				else
				{
					uDistance = 1;
				}
				ptSameWithCurSeq.uDistance+=uDistance;
				ptSameWithCurSeq.uvPoints.push_back((*it));
				pdvOpenTemp.push_back(ptSameWithCurSeq);
				ptSameWithCurSeq.uDistance-=uDistance;
				ptSameWithCurSeq.uvPoints.pop_back();
			}
		}
	
		//Remove the path which is not shortest in paths with same end point
		PointDistanceVec::iterator itOpenRes;
		for (itOpenRes = pdvOpenedPath.begin();
			itOpenRes != pdvOpenedPath.end();itOpenRes++) 
		{
			if ((*itOpenRes).uvPoints.size() > 0) 
			{
				for (PointDistanceVec::iterator itOpenTmp = pdvOpenTemp.begin();
					itOpenTmp != pdvOpenTemp.end();itOpenTmp++)
				{
					if ((*itOpenTmp).uvPoints.size() > 0) 
					{
						if ((*itOpenTmp).uvPoints[(*itOpenTmp).uvPoints.size()-1]
							== (*itOpenRes).uvPoints[(*itOpenRes).uvPoints.size()-1]) 
						{
							if ((*itOpenTmp).uDistance < (*itOpenRes).uDistance) 
							{
								(*itOpenRes).uDistance = (*itOpenTmp).uDistance;
								(*itOpenRes).uvPoints.clear();
								for (int j=0;j<(*itOpenTmp).uvPoints.size();j++) 
								{
									(*itOpenRes).uvPoints.push_back((*itOpenTmp).uvPoints[j]);
								}
							}							
							(*itOpenTmp).uDistance = UINT_MAX;
						}
					}
				}
			}
		}
		
		//Add new path if there is new end point existed
		for (PointDistanceVec::iterator itOpenTmpNew = pdvOpenTemp.begin();
			itOpenTmpNew != pdvOpenTemp.end();itOpenTmpNew++)
		{
			if (UINT_MAX == (*itOpenTmpNew).uDistance) 
				continue;
			if ((*itOpenTmpNew).uvPoints.size() > 0) 
			{
				PointDistanceVec::iterator itOpenNew = pdvOpenedPath.begin();
				for (;itOpenNew != pdvOpenedPath.end();itOpenNew++) 
				{
					if ((*itOpenRes).uvPoints.size() > 0) 
					{
						if ((*itOpenTmpNew).uvPoints[(*itOpenTmpNew).uvPoints.size()-1]
							== (*itOpenNew).uvPoints[(*itOpenNew).uvPoints.size()-1]) 
							break;
					}
				}
				if (itOpenNew == pdvOpenedPath.end())//new path 
				{
					POINTDISTANCE pdNow;
					pdNow.uDistance = (*itOpenTmpNew).uDistance;
					for (int k=0;k<(*itOpenTmpNew).uvPoints.size();k++) 
					{
						pdNow.uvPoints.push_back((*itOpenTmpNew).uvPoints[k]);
					}
					pdvOpenedPath.push_back(pdNow);
				}
			}	
		}
	}

	return (int)pdvOpenedPath.size();
}
int CCrossMapPathFinder::ChooseNewClosedPath(PointDistanceVec& pdvOpenedPath,
										PointDistanceVec& pdvClosedPath,
										UInt32Vector& uvSelectedPoints)
{
	int iRet = 0;
	
	if (pdvOpenedPath.size() > 0) 
	{
		UInt32 uMinDistancePath = UINT_MAX;
		int iMinDistanceIndex = -1;
		UInt32 uNewSelectedPoints = 0;
		int i = 0;
		for (;i<pdvOpenedPath.size();i++) 
		{
			if (uMinDistancePath > pdvOpenedPath[i].uDistance) 
			{
				uMinDistancePath = pdvOpenedPath[i].uDistance;
				iMinDistanceIndex = i;
			}
		}
		if (iMinDistanceIndex > -1) 
		{
			POINTDISTANCE pd;
			pd.uDistance = pdvOpenedPath[iMinDistanceIndex].uDistance;
			for (int j= 0;j < pdvOpenedPath[iMinDistanceIndex].uvPoints.size();j++) 
			{
				pd.uvPoints.push_back(pdvOpenedPath[iMinDistanceIndex].uvPoints[j]);
			}
			if (pd.uvPoints.size() > 0) 
			{
				uNewSelectedPoints = pd.uvPoints[pd.uvPoints.size()-1];
				pdvClosedPath.push_back(pd);
			}
			
			int k=0;
			for (;k<uvSelectedPoints.size();k++) 
			{
				if (uvSelectedPoints[k] == uNewSelectedPoints) 
					break;
			}
			if (k > 0 && uNewSelectedPoints > 0 && k == uvSelectedPoints.size()) 
			{
				uvSelectedPoints.push_back(uNewSelectedPoints);
			}
		}
		else
		{
			iRet = -1;
		}
	}
	else//no new path date,destination unreachable
	{
		iRet = -2;
	}
	
	return iRet;
}

int CCrossMapPathFinder::GetNextPoints(UInt32 uCurPoints,UInt32Vector& uvNewPoints)
{
	if (uCurPoints == 0) 
		return 0;
	if (uCurPoints == 1) 
	{
		CCharacter* pChar = gGlobal.g_pCharacter->GetCharNetObject();
		if (NULL != pChar) 
			return 0;
		if (pChar->GetLevel() < 10) 
			return 0;
	}
	const JUMPPOINT* pjp = gGlobal.m_resource->GetAutoPathData(uCurPoints);
	if (NULL != pjp) 
	{
		gGlobal.m_resource->GetMapJumpPoints(pjp->uMapId,uvNewPoints);
		
		for (UInt32 i= 0;i < pjp->uRelatedJumpPointNum;i++) 
		{
			uvNewPoints.push_back(pjp->uvRelatedPoint[i]);
		}
	}
	
	return (int)uvNewPoints.size();
}

int CCrossMapPathFinder::ReservePath(UInt32Vector& uvSelectedPoints,
				UInt32Vector& uvDropedPoints,
				PointDistanceVec& pdvOpenedPath,
				PointDistanceVec& pdvClosedPath)
{
	UInt32 uReservedPoint = uvSelectedPoints[uvSelectedPoints.size()-1];
	
	uvDropedPoints.push_back(uReservedPoint);
	for (PointDistanceVec::iterator itOpen = pdvOpenedPath.begin();
		itOpen != pdvOpenedPath.end();itOpen++) 
	{
		if ((*itOpen).uvPoints.size()>0) 
		{
			if ((*itOpen).uvPoints[(*itOpen).uvPoints.size()-1] == uReservedPoint) 
			{
				(*itOpen).uvPoints.clear();
				pdvOpenedPath.erase(itOpen);
				break;
			}
		}
	}

	for (PointDistanceVec::iterator itClosed = pdvClosedPath.begin();
		itClosed != pdvClosedPath.end();itClosed++) 
	{
		if ((*itClosed).uvPoints.size()>0) 
		{
			if ((*itClosed).uvPoints[(*itClosed).uvPoints.size()-1] == uReservedPoint) 
			{
				(*itClosed).uvPoints.clear();
				pdvClosedPath.erase(itClosed);
				break;
			}
		}
	}
	uvSelectedPoints.pop_back();
	
	return (int)uvSelectedPoints.size();
}

void CCrossMapPathFinder::GetSrcPoint(BlockPos& bpSrc)
{
	bpSrc = m_bpSrc;
}
void CCrossMapPathFinder::GetDestPoint(BlockPos& bpDest)
{
	bpDest = m_bpDest;
}
#pragma warning(default:4267)
#pragma warning(default:4018)

//#endif