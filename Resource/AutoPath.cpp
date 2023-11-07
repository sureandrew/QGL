#include "Common.h"
#include "Common/common_binary.h"
#include "Resource/QGLCSVTReader.h"
#include "ResAutoPath.h"
#include "AutoPath.h"

//#ifdef AUTOPATHON
int CAutoPathManager::LoadCSV(const TCHAR *szFileName,
							  const BYTE* pData /* = NULL */,
							  UInt32 pDataLen /* = 0 */)
{
	if (NULL == szFileName) {
		return AUTOPATH_INVILAD_PARAM;
	}

	CQGLCSVTReader reader;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(szFileName, true) )
		openSrcSucceeded = TRUE;

	if( openSrcSucceeded )
	{
		JUMPPOINT* pJumpPoint = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				pJumpPoint = SafeCreateObject(JUMPPOINT);
				reader.Read(_T("PointId"),pJumpPoint->uSeq);
				reader.Read(_T("JumpType"),pJumpPoint->uJumpType);
				reader.Read(_T("MapId"),pJumpPoint->uMapId);
				reader.Read(_T("SourcePosX"),pJumpPoint->u16PosX);
				reader.Read(_T("SourcePosY"),pJumpPoint->u16PosY);
				reader.Read(_T("DestPosX"),pJumpPoint->u16PosXJumped);
				reader.Read(_T("DestPosY"),pJumpPoint->u16PosYJumped);
				reader.Read(_T("RelatedPiontNum"),pJumpPoint->uRelatedJumpPointNum);
				TCHAR szKey[512] = {0};
				for (UInt32 i=0;i<pJumpPoint->uRelatedJumpPointNum;i++) 
				{
					::_stprintf(szKey,_T("RelatedPointerId%u"),i);
					UInt32 uRelatedId = 0;
					reader.Read(szKey,uRelatedId);
					if (uRelatedId > 0) 
					{
						pJumpPoint->uvRelatedPoint.push_back(uRelatedId);
					}
				}
				m_smAutoPath.insert(std::make_pair(pJumpPoint->uSeq,pJumpPoint));
				reader.MoveNext();
			}
		}
		catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: AutoPathManager::LoadCSV: CSV reading error due to \
											   column format or missing MoveNext(), %s"), szFileName);
			reader.Close();
			return AUTOPATH_FILE_CONTEXT_ERROR;
		}
	}

	reader.Close();
	return AUTOPATH_OK;
}

VOID CAutoPathManager::Release()
{
	for (AutoPathMap::iterator it = m_smAutoPath.begin(); it != m_smAutoPath.end(); it++)
		SafeDeleteObject(it->second);
	m_smAutoPath.clear();
}

JUMPPOINT* CAutoPathManager::GetAutoPathData(UInt32 uAutoSeq)
{
	AutoPathMap::iterator it = m_smAutoPath.find(uAutoSeq);
	if( it!=m_smAutoPath.end() )
		return it->second;
	else
		return NULL;
}

size_t CAutoPathManager::GetAutoPathDataSize()
{
	return m_smAutoPath.size();
}

int CAutoPathManager::GetMapJumpPoints(const UInt32 uMapID,UInt32Vector& uvPoints)
{
	for (AutoPathMap::iterator it = m_smAutoPath.begin(); it != m_smAutoPath.end(); it++)
	{
		if (it->second->uMapId == uMapID) 
		{
			uvPoints.push_back(it->second->uSeq);
		}
	}
	return (int)uvPoints.size();
}

//#endif