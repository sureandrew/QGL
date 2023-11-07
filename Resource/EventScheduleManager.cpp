//-- Common
#include "Common.h"
//-- Self
#include "Resource/EventScheduleManager.h"
//-- Library
#include "Common/common_binary.h"
#include <Reuben/Platform/File.h>
#include "Resource/ResEventSchedule.h"
#include "Resource/QGLCSVTReader.h"

//#define EVENTSCUEDULE_DATA_VER		1
#define	WORLDEVENT_CSV_FOLDER _T("Data\\CSV\\WorldEvent\\%s")

typedef Reuben::Platform::File File;
typedef Reuben::Platform::FileException FileException;

String GetTimeStr2(time_t& t)
{
	static char buf[256];
	strftime(buf, 256, "%Y/%m/%d %H:%M:%S", localtime(&t));
	String s(buf);
	return s;
}

////////////// EventSchedule Manager //////////////////////////////////

CEventScheduleManager::CEventScheduleManager()
{
	m_eventSchedules.clear();
}

CEventScheduleManager::~CEventScheduleManager()
{
	Release();
}

VOID CEventScheduleManager::Release()
{
	for (EventScheduleMap::iterator it = m_eventSchedules.begin(); it != m_eventSchedules.end(); it++)
		SafeDeleteObject(it->second);
	m_eventSchedules.clear();
}

const EventScheduleData* CEventScheduleManager::GetEventSchedule(UInt32 eventSchedule_id)
{
	EventScheduleMap::iterator it = m_eventSchedules.find(eventSchedule_id);
	if( it!=m_eventSchedules.end() )
		return it->second;
	else
		return NULL;
}

BOOL CEventScheduleManager::GetEventScheduleVector(EventScheduleMap& schedule)
{
	schedule = m_eventSchedules;
	return TRUE;
}

BOOL CEventScheduleManager::LoadCSV(const TCHAR *filename, const BYTE* pData, UInt32 pDataLen)
{
	CQGLCSVTReader reader;
	BOOL ReadSucceeded = TRUE;
	BOOL openSrcSucceeded = FALSE;
	if( pData && pDataLen && reader.OpenBuf(pData, pDataLen, true))
		openSrcSucceeded = TRUE;
	else if( reader.Open(filename, true) )
		openSrcSucceeded = TRUE;
	else
	{
		TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: EventScheduleManager::LoadCSV: file error, %s"), filename);
	}
	
	if( openSrcSucceeded ) // Either FILE or Data stream uses the same reading algorithm
	{
		EventScheduleData* pEventSchedule = NULL;
		try{
			while( !reader.IsLineEmpty()  )
			{
				TCHAR buf[512];

				pEventSchedule = SafeCreateObject(EventScheduleData);
				// basic info
				reader.Read(_T("WE_ID"),				pEventSchedule->id);
				reader.Read(_T("WE_Type"),				pEventSchedule->type);
				reader.ReadString(_T("WE_Name"),		pEventSchedule->name, 31);
				reader.ReadString(_T("WE_Descript"),	pEventSchedule->descript, 31);
				reader.Read(_T("WE_Line"),				pEventSchedule->line);
				reader.Read(_T("WE_MapType"),			pEventSchedule->mapType);
				ZeroMemory(buf, sizeof(TCHAR)*512);
				reader.ReadString(_T("WE_Map"), buf, 511);
				pEventSchedule->map.clear();
				ReadUInt32Vector(pEventSchedule->map, buf);

				// buff/npc info detail
				switch( pEventSchedule->type )
				{
				case EVENTSCUEDULE_EVENT_TYPE_WORLD_BUFF:
					{
						pEventSchedule->buffData	= SafeCreateObject(EventScheduleBuffData);

						ZeroMemory(buf, sizeof(TCHAR)*512);
						reader.ReadString(_T("WE_Buff"), buf, 511);
						pEventSchedule->buffData->buff.clear();
						ReadUInt32Vector(pEventSchedule->buffData->buff, buf);
						reader.Read(_T("WE_BuffPeriod"),	pEventSchedule->buffData->buffPeriod);
						pEventSchedule->buffData->buffPeriod *= 60;	// in second
						UInt8 tempbyte = 0;
						reader.Read(_T("WE_BuffClear"), tempbyte);
						pEventSchedule->buffData->buffClear = (tempbyte > 0);
						reader.Read(_T("WE_BuffLeaveMapClear"), tempbyte);
						pEventSchedule->buffData->removalOnLeaveMap = (tempbyte > 0);
					}
					break;
				case EVENTSCUEDULE_EVENT_TYPE_NPC:
					{
						pEventSchedule->npcData		= SafeCreateObject(EventScheduleNpcData);

						ZeroMemory(buf, sizeof(TCHAR)*512);
						reader.ReadString(_T("WE_npcID"),	buf, 511);
						pEventSchedule->npcData->npcId.clear();
						ReadUInt32Vector(pEventSchedule->npcData->npcId, buf);
						reader.Read(_T("WE_npcMin"),		pEventSchedule->npcData->npcMin);
						reader.Read(_T("WE_npcMax"),		pEventSchedule->npcData->npcMax);
						reader.Read(_T("WE_npcLiveTime"),	pEventSchedule->npcData->npcLiveTime);
						pEventSchedule->npcData->npcLiveTime *= 60;	// in second
						reader.Read(_T("WE_npcReborn"),		pEventSchedule->npcData->npcReborn);

						ZeroMemory(buf, sizeof(TCHAR)*512);
						reader.ReadString(_T("WE_npcMapCSV"), buf, 511);
						// Code Support server side (non-patch) version only:
						String targetCsvFilename = GetNpcMapCsvFilename(buf);
						if( !targetCsvFilename.IsEmpty() )
						{
							CQGLCSVTReader reader2;
							EventScheduleMapData* npcMapData = NULL;
							try{
								if( reader2.Open(targetCsvFilename.c_str(), true) )
								{
									while( !reader2.IsLineEmpty()  )
									{
										npcMapData = SafeAllocate(EventScheduleMapData, 1);
										reader2.Read(_T("MapID"),	npcMapData->mapId);
										reader2.Read(_T("X"),		npcMapData->x);
										reader2.Read(_T("Y"),		npcMapData->y);
										pEventSchedule->npcData->npcMapData.push_back(npcMapData);
										reader2.MoveNext();
									}
								}
							}catch(...)
							{
								TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: EventScheduleManager::LoadCSV: [EVENTSCUEDULE_EVENT_TYPE_NPC] CSV reading error due to \
																column format or missing MoveNext(), %s"), filename);
								reader2.Close();
								return FALSE;
							}
							reader2.Close();
						}
					}
					break;
				case EVENTSCUEDULE_EVENT_TYPE_SCRIPT:
					{
						reader.ReadString(_T("WE_Script"),		pEventSchedule->script, 255);
					}
					break;
				default:
					break;
				}

				// message
				reader.Read(_T("WE_MsgType"),			pEventSchedule->msgType);		// choose which channel
				reader.ReadString(_T("WE_MsgStart"),	pEventSchedule->msgStart, 255);
				reader.ReadString(_T("WE_MsgPeriod"),	pEventSchedule->msgPeriod, 255);
				reader.ReadString(_T("WE_MsgKill"),		pEventSchedule->msgKill, 255);
				reader.ReadString(_T("WE_MsgEnd"),		pEventSchedule->msgEnd, 255);

				// schedule
				ZeroMemory(buf, sizeof(TCHAR)*512);
				reader.ReadString(_T("WE_ScheduleStart"),	buf, 511);
				pEventSchedule->scheduleStart	= ReadDate(buf);
				ZeroMemory(buf, sizeof(TCHAR)*512);
				reader.ReadString(_T("WE_ScheduleEnd"),		buf, 511);
				pEventSchedule->scheduleEnd		= ReadDate(buf) + 86399;
				ZeroMemory(buf, sizeof(TCHAR)*512);
				reader.ReadString(_T("WE_StartTime"),		buf, 511);
				pEventSchedule->startTime		= (UInt32)ReadTime(buf);
				reader.Read(_T("WE_Length"),			pEventSchedule->length);
				pEventSchedule->length *= 60;		// in second
				reader.Read(_T("WE_Period"),			pEventSchedule->period);
				reader.Read(_T("WE_PeriodDay"),			pEventSchedule->periodDay);
				reader.Read(_T("WE_PeriodWeek"),		pEventSchedule->periodWeek);
				reader.Read(_T("WE_PeriodMonth"),		pEventSchedule->periodMonth);
				reader.Read(_T("WE_Repeat"),			pEventSchedule->repeat);
				//pEventSchedule->repeat *= 60000;	// in millisecond
				pEventSchedule->repeat *= 60;		// in second

				// rule & error check
				// 1. Allow bring buff to jump map but clear buff in the whole world may:
				//    a) decrease sys. performance, and;
				//    b) prone to bug to remove buff across MapSerices
				if( pEventSchedule->buffData && pEventSchedule->buffData->buffClear==1 && pEventSchedule->buffData->removalOnLeaveMap==0 )
				{
					TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("EventScheduleManager::LoadCSV Error (1) [WE_BuffClear = 1 AND WE_BuffLeaveMapClear = 0 not supported] eventId=%i"), pEventSchedule->id);
					exit(-1);
				}

				m_eventSchedules.insert(std::make_pair(pEventSchedule->id, pEventSchedule));
				reader.MoveNext();
			}
			TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Initialization: %i eventSchedule(s) read"), (UInt)m_eventSchedules.size());
		}catch(...)
		{
			TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("Err: EventScheduleManager::LoadCSV: CSV reading error due to \
						 column format or missing MoveNext(), %s"), filename);
			reader.Close();
			return FALSE;
		}
		reader.Close();
	}

	return TRUE;
}

BOOL CEventScheduleManager::LoadDAT(const TCHAR *filename)
{
	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CEventScheduleManager::LoadDAT Error (1) [function missing], filename=%s"), filename);
	return FALSE;
}

BOOL CEventScheduleManager::SaveDAT(const TCHAR *filename)
{
	TRACE_ERRORDTL_1(GLOBAL_LOGGER, _F("CEventScheduleManager::SaveDAT Error (1) [function missing], filename=%s"), filename);
	return FALSE;
}

UInt32 CEventScheduleManager::GetSize()
{
	return (UInt32)m_eventSchedules.size();
}

time_t CEventScheduleManager::ReadTime(TCHAR *timeString)
{
	if( timeString==NULL )
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CEventScheduleManager::ReadTime Warning (1) [timeString null]"));
		return 0;
	}
	if( _tcslen(timeString)!=8 )
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CEventScheduleManager::ReadTime Warning (2) [timeString invalid, format should be HH-mm-SS] timeString=%s"), timeString);
		return 0;
	}

	TCHAR localstring[512];
	ZeroMemory(localstring, sizeof(TCHAR)*512);
	_tcscpy(localstring, timeString);

	TCHAR *delim = _T(":");
	TCHAR *cmd = NULL; 
	tm fillTime;
	InitializeTm(fillTime);
	UInt32 hh, mm, ss;

	cmd = _tcstok(localstring, delim);
	hh = _ttoi(cmd);
	cmd = _tcstok(NULL, delim);
	mm = _ttoi(cmd);
	cmd = _tcstok(NULL, delim);
	ss = _ttoi(cmd);

	time_t secondFromZero = hh*3600 + mm*60 + ss;
//	struct tm *abc = gmtime(&secondFromZero);
//	time_t def;
//	time(&def);
	return secondFromZero;
}

time_t CEventScheduleManager::ReadDate(TCHAR *dateString)
{
	if( dateString==NULL )
	{
		TRACE_WARNDTL(GLOBAL_LOGGER, _F("CEventScheduleManager::ReadDate Warning (1) [dateString null]"));
		return 0;
	}
	if( _tcslen(dateString)!=10 )
	{
		TRACE_WARNDTL_1(GLOBAL_LOGGER, _F("CEventScheduleManager::ReadDate Warning (2) [dateString invalid, format should be YYYY-MM-DD] dateString=%s"), dateString);
		return 0;
	}

	TCHAR localstring[512];
	ZeroMemory(localstring, sizeof(TCHAR)*512);
	_tcscpy(localstring, dateString);

	TCHAR *delim = _T("-");
	TCHAR *cmd = NULL; 
	tm fillTime;
	InitializeTm(fillTime);
	UInt32 YYYY, MM, DD;

	cmd = _tcstok(localstring, delim);
	YYYY = _ttoi(cmd);
	cmd = _tcstok(NULL, delim);
	MM = _ttoi(cmd);
	cmd = _tcstok(NULL, delim);
	DD = _ttoi(cmd);

	fillTime.tm_year	= YYYY - 1900;
	fillTime.tm_mon		= MM - 1;
	fillTime.tm_mday	= DD;

	return mktime(&fillTime);
}

// support uintString up to 65535 bytes.
VOID CEventScheduleManager::ReadUInt32Vector(UInt32Vector& vector, TCHAR* uintString)	
{
	if( uintString==NULL ) return;
	if( _tcslen(uintString)==0 || _tcslen(uintString)>65535 ) return;

	TCHAR *delim = _T("|");
	TCHAR *cmd = NULL; 
	UInt32 value;
	cmd = _tcstok(uintString, delim);
	while (cmd != NULL)
	{
		value = _ttoi(cmd);
		vector.push_back(value);
		cmd = _tcstok(NULL, delim);
	}
}

VOID CEventScheduleManager::DebugPrint()
{
	TRACE_INFODTL_1(GLOBAL_LOGGER, _F("EventScheduleManager: size = %d"), GetSize());
	for( EventScheduleMap::iterator itr = m_eventSchedules.begin(); itr != m_eventSchedules.end(); ++itr )
	{
		EventScheduleData* event = itr->second;
		if( event )
		{
			TRACE_INFODTL_5(GLOBAL_LOGGER, _F("ID=%d, Type=%d, Line=%d, Start=%s, End=%s"), 
				event->id, event->type, event->line, 
				GetTimeStr2(event->scheduleStart).c_str(), 
				GetTimeStr2(event->scheduleEnd).c_str()
				);
//			for( Index i = C_INDEX(0); i < event->map.size(); ++i )
//				TRACE_INFODTL_1(GLOBAL_LOGGER, _F("Map=%d"), event->map[i]);
//
//			if( event->npcData )
//				TRACE_INFODTL(GLOBAL_LOGGER, _F("NpcData exists"));
//			if( event->buffData )
//				TRACE_INFODTL(GLOBAL_LOGGER, _F("BuffData exists"));
		}
	}
}

VOID CEventScheduleManager::InitializeTm(tm &timeValue)
{
	timeValue.tm_hour = 0;
	timeValue.tm_isdst = 0;
	timeValue.tm_mday = 0;
	timeValue.tm_min = 0;
	timeValue.tm_mon = 0;
	timeValue.tm_sec = 0;
	timeValue.tm_wday = 0;
	timeValue.tm_yday = 0;
	timeValue.tm_year = 0;
}

String CEventScheduleManager::GetNpcMapCsvFilename(LPCTSTR filename)
{
	String retString;
	if( _tcslen(filename) > 0 )
		retString.Format(WORLDEVENT_CSV_FOLDER, filename);
	else
		retString.Empty();
	return retString;
}