//-- Common
#include "Common.h"
//-- Self
#include "ObjectService/DboSourcePool.h"
//-- Library
#include "Dbo.h"
#include "ObjectService/Global.h"

#define DBS_AGING_TIME				300000		// 5 min aging time for non-used db source

CDboSourcePool::CDboSourcePool()
{
	m_ageCheck = BOOLEAN_FALSE;
	m_minCount = 0;
	m_usedCount = 0;
}

CDboSourcePool::~CDboSourcePool()
{
	ClearAll();
}

VOID CDboSourcePool::ClearAll()
{
	ThreadGuard guard(m_key);

	for (DbsInfoMap::iterator it = m_infoMap.begin(); it != m_infoMap.end(); ++it)
	{
		DbsInfo &info = it->second;
		if (info.src)
			gGlobal.ReleaseSource(info.src);
	}
	m_infoMap.clear();
	m_ageCheck = BOOLEAN_FALSE;
}

VOID CDboSourcePool::InitSource(UInt16 srcCount)
{
	if (!m_infoMap.empty())
		ClearAll();

	DbsInfo newInfo;
	for (UInt16 i = 0; i < srcCount; i++)
	{
		ThreadGuard guard(m_key);

		// create new db source
		CDboSource* newSrc = gGlobal.CreateSource(m_bAccount);
		if (newSrc == NULL)
			break;

		newInfo.src = newSrc;
		newInfo.avail = BOOLEAN_TRUE;
		newInfo.ageTime = UINT32_MAX_VALUE;
		m_infoMap[newSrc] = newInfo;
	}

	TRACE_INFODTL_2(GLOBAL_LOGGER, _F("InitSource: bAccount %d, cached count = %d"),
		m_bAccount, (UInt16) m_infoMap.size());
	m_minCount = srcCount;
}

/*
// just for testing 1 db source connection
CDboSource* CDboSourcePool::GetAvailSource()
{
	if (m_bAccount)
		return gGlobal.m_pAccountDb;
	else
		return gGlobal.m_pGameDb;
}

VOID CDboSourcePool::ReleaseSource(CDboSource* dbsrc)
{
}
*/

CDboSource* CDboSourcePool::GetAvailSource()
{
	ThreadGuard guard(m_key);

	// find old db source
	CDboSource* dbSrc = NULL;
	do {
		CDboSource* secondSrc = NULL;
		int i = GetRand((int) m_infoMap.size());

		for (DbsInfoMap::iterator it = m_infoMap.begin(); it != m_infoMap.end(); ++it, --i)
		{
			DbsInfo &info = it->second;
			if (info.avail && info.src)
			{
				info.ageTime = UINT32_MAX_VALUE;
				info.avail = BOOLEAN_FALSE;
				dbSrc = info.src;
				break;
			}
			if (i == 0)
				secondSrc = info.src;
		}
		if (dbSrc)		// db source found
			break;

		// use old running source, don't create new db source for stability
		//dbSrc = secondSrc;

		TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_T("GetAvailSource Warning: [no more source], bAccount %d, usedCount %d"),
			m_bAccount, m_usedCount);
		// no more old db source, create new db source
		dbSrc = gGlobal.CreateSource(m_bAccount);
		if (dbSrc) {
			DbsInfo newInfo;
			newInfo.src = dbSrc;
			newInfo.avail = BOOLEAN_FALSE;
			newInfo.ageTime = UINT32_MAX_VALUE;
			m_infoMap[dbSrc] = newInfo;
		}
	} while (false);

	if (dbSrc) {
		m_usedCount ++;
		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, 
			_T("GetAvailSource: dbSrc %d, bAccount %d, usedCount %d"),
			dbSrc, m_bAccount, m_usedCount);
	} else {
		TRACE_WARNDTL_2(GLOBAL_LOGGER, 
			_T("GetAvailSource Warning: [can't create source], bAccount %d, usedCount %d"),
			m_bAccount, m_usedCount);
	}
	
	return dbSrc;
}

VOID CDboSourcePool::ReleaseSource(CDboSource* dbsrc)
{
	if (dbsrc == NULL)
		return;

	ThreadGuard guard(m_key);
	DbsInfoMap::iterator it = m_infoMap.find(dbsrc);
	if (it != m_infoMap.end()) {
		DbsInfo &info = it->second;
		info.ageTime = DBS_AGING_TIME;
		info.avail = BOOLEAN_TRUE;
		m_ageCheck = BOOLEAN_TRUE;
		m_usedCount --;
		TRACE_VERBOSEDTL_3(GLOBAL_LOGGER, 
			_T("ReleaseSource: dbSrc %d, bAccount %d, usedCount %d"),
			dbsrc, m_bAccount, m_usedCount);
	} else {
		TRACE_WARNDTL_3(GLOBAL_LOGGER, 
			_T("ReleaseSource Warning: dbSrc %d not found, bAccount %d, usedCount %d"),
			dbsrc, m_bAccount, m_usedCount);
	}
}

VOID CDboSourcePool::OnUpdate(UInt32 dt)
{
	if (m_ageCheck)
	{
		ThreadGuard guard(m_key);

		m_ageCheck = BOOLEAN_FALSE;
		int i = 0;
		for (DbsInfoMap::iterator it = m_infoMap.begin(); it != m_infoMap.end(); ++i)
		{
			DbsInfo &info = it->second;
			if (i >= m_minCount && info.src && info.avail && info.ageTime != UINT32_MAX_VALUE)
			{
				if (info.ageTime < dt)
				{
					TRACE_WARNDTL_3(GLOBAL_LOGGER, 
						_T("CDboSourcePool OnUpdate: aging removed dbSrc %d, bAccount %d, cached count %d"),
						info.src, m_bAccount, m_infoMap.size() - 1);
					gGlobal.ReleaseSource(info.src);
					it = m_infoMap.erase(it);
					continue;
				}
				info.ageTime -= dt;
				m_ageCheck = BOOLEAN_TRUE;
			}
			++it;
		}
	}
}