// DbSourcePool.h : Declaration of the DbSourcePool

#pragma once
#ifndef _DBO_SOURCE_POOL_H_
#define _DBO_SOURCE_POOL_H_
#include <Reuben/Utility/Singleton.h>
#include <vector>

class CDboSource;

struct DbsInfo
{
	CDboSource*	src;					// Dbo source
	Boolean		avail;					// is available?
	UInt32		ageTime;				// cache aging time

	DbsInfo() : src(NULL), avail(BOOLEAN_FALSE), ageTime(UINT32_MAX_VALUE) {}
};

class CDboSourcePool
{
public:
	CDboSourcePool();
	~CDboSourcePool();
	void ClearAll();
	void InitSource(UInt16 srcCount);
	CDboSource* GetAvailSource();
	void ReleaseSource(CDboSource* dbsrc);
	void OnUpdate(UInt32 dt);
	void SetAccountDb(Boolean bAccount) { m_bAccount = bAccount; }

private:

	typedef StlMap<CDboSource*, DbsInfo, std::less<CDboSource*>, 
		ManagedAllocator<std::pair<CDboSource*, DbsInfo> > > DbsInfoMap;

	ThreadKey		m_key;
	DbsInfoMap		m_infoMap;
	Boolean			m_ageCheck;
	UInt16			m_minCount;
	UInt16			m_usedCount;
	Boolean			m_bAccount;
};

#endif // _DBO_SOURCE_POOL_H_