#pragma once
#ifndef _EVENTSCUEDULE_MANAGER_H_
#define _EVENTSCUEDULE_MANAGER_H_

struct EventScheduleData;

class CEventScheduleManager
{
public:
	CEventScheduleManager();
	~CEventScheduleManager();

public:
	typedef StlMap<UInt32, EventScheduleData*, std::less<UInt32>, ManagedAllocator<std::pair<Int32, EventScheduleData*> > > EventScheduleMap;

	const EventScheduleData* GetEventSchedule(UInt32 eventSchedule_id);
	BOOL GetEventScheduleVector(EventScheduleMap& schedule);

	UInt32 GetSize();
	BOOL LoadCSV(const TCHAR *filename, const BYTE* pData = NULL, UInt32 pDataLen = 0); // filename: FILE, pData & pDataLen: Data
	BOOL LoadDAT(const TCHAR *filename);
	BOOL SaveDAT(const TCHAR *filename);
	VOID DebugPrint();

private:
	time_t ReadTime(TCHAR *timeString);
	time_t ReadDate(TCHAR *dateString);
	VOID ReadUInt32Vector(UInt32Vector& vector, TCHAR* uintString);
	VOID InitializeTm(tm &timeValue);
	String GetNpcMapCsvFilename(LPCTSTR filename);

	VOID Release();

private:

	EventScheduleMap m_eventSchedules;
};

#endif // _EVENTSCUEDULE_MANAGER_H_
