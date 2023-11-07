#ifndef _GENERATION_PROCESS_H_
#define _GENERATION_PROCESS_H_

enum GENERATIONSORTTYPE
{
	SORT_LEVEL = 0,
	SORT_CLASS,
	SORT_OFFLINE,
	SORT_ACCOREXP,
	SORT_GIFTORHISACC,
	SORT_MAX,
};

class CGenerationProcess
{
public:
	CGenerationProcess();
	~CGenerationProcess();
public:
	typedef StlVector<CGeneration*,ManagedAllocator<CGeneration*> > GenerationVector;
	static GENERATIONSORTTYPE  m_gstCurSortType;
	static bool CompareGeneration(CGeneration* pFrt,CGeneration* pSnd);
public:
	void SortListByLevel();
	void SortListByClass();
	void SortListByOffline();
	void SortListByAccOrExp();
	void SortListByGiftOrHisAcc();
public:
	void GetSavedExp();
	void DisbandGeneration();
	void RequestCreateGeneration(UInt32 ui32TargetId);
	void RequestGenerationAns(bool bAllow);
	void ClearGenerationRequest() {m_ui32CurRequestId = 0;}
	void SystemDesc(UInt16 uiContextId);
public:
	void UpdateGeneration(bool bForceOpen);
	void UpdateDisbandPanel();
	void UpDateRequestPanel();
	UInt32 GetCurRequestId() {return m_ui32CurRequestId;}
	void SetCurRequestId(UInt32 ui32RequestId) { m_ui32CurRequestId = ui32RequestId;}
	BOOL IsPanelOpen() {return m_bOpened;}
	void SetOpenState(BOOL bFlag) { m_bOpened = bFlag;}
public:
	void ShowGenerations();
	void SelectGeneration(int index);
	void GenerationScroll(int scroll);
	void OpenGenCharDetail();
public:
	void HideAllPanel();
	void ShowAllPanel();
private:
	UInt32 m_ui32StarIndex;
	int    m_iSelIndex;
	UInt32 m_uiTotalRecord;
	UInt32 m_ui32CurRequestId;
	BOOL   m_bOpened;
};

#endif