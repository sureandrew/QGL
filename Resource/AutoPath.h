#ifndef _AUTO_PATH_H_
#define _AUTO_PATH_H_

//#ifdef AUTOPATHON
struct JUMPPOINT;

enum ErrorCode_AUTOPATH
{
	AUTOPATH_OK = 0,
	AUTOPATH_INVILAD_PARAM,
	AUTOPATH_FILE_NOT_EXIST,
	AUTOPATH_FILE_CONTEXT_ERROR,
};

class CAutoPathManager
{
public:
	CAutoPathManager() { m_smAutoPath.clear();}
	~CAutoPathManager() { Release();}
public:
	int LoadCSV(const TCHAR *szFileName, const BYTE* pData = NULL, UInt32 pDataLen = 0);
	VOID Release();
	JUMPPOINT* GetAutoPathData(UInt32 uAutoSeq);
	int GetMapJumpPoints(const UInt32 uMapID,UInt32Vector& uvPoints);
	size_t GetAutoPathDataSize();
private:
	typedef StlMap<UInt32, JUMPPOINT*, std::less<UInt32>,ManagedAllocator<std::pair<UInt32, JUMPPOINT*> > > AutoPathMap;
	AutoPathMap m_smAutoPath;
};
//#endif

#endif