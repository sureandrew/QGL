//-- Common
#include "Common.h"

static PCSTRING gServiceTypeString[] =
{
	_T(""),
	_T("Log"),
	_T("World"),
	_T("Object"),
	_T("Message"),
	_T("Manager"),
	_T("Proxy"),
	_T("Login"),
	_T("Map"),
	_T("Battle")
};

CONST ServiceType StringToServiceType(PCSTRING pType)
{
	for (Index i = C_INDEX(0); i < SERVICE_MAX; ++i)
	{
		if (::_tcsicmp(gServiceTypeString[i], pType) == 0)
		{
			return ((ServiceType)i);
		}
	}
	return (SERVICE_NONE);
}

PCSTRING ServiceTypeToString(CONST ServiceType eType)
{
	if (eType > SERVICE_NONE && eType < SERVICE_MAX)
		return (gServiceTypeString[(Int)eType]);
	return (gServiceTypeString[0]);
}

Boolean IsFileExist(PCSTRING pFilename)
{
	DWORD attr = GetFileAttributes(pFilename);
	return (attr != INVALID_FILE_ATTRIBUTES && 
		(attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY);
}

UInt NoCodeStringSize(String &Text)
{
	Int		Result = 0;
	bool	HideNow = false;

	for(Int i = 0; i < (Int)Text.size(); ++i)
	{
		if(Text[i] == 15)
		{
			HideNow = true;
			continue;
		}
		else
		if(Text[i] == 16)
		{
			HideNow = false;
			continue;
		}
		else
		if(HideNow)
		{
			continue;
		}
		else
		if(Text[i] <= 26 && Text[i] != _T('\r'))
		{
			continue;
		}
		else
		if(Text[i] == _T('#'))
		{
			if(Text[i + 1] == _T(':'))
			{
				++i;
				continue;
			}
		}
		
		++Result;
	}

	return Result;
}

VOID SubNoCodeString(String &Text, Int Count)
{
	Int		Result = 0;
	Int		NewCount = 0;
	bool	HideNow = false;

	for(; NewCount < (Int)Text.size(); ++NewCount)
	{
		if(Text[NewCount] == 15)
		{
			HideNow = true;
			continue;
		}
		else
		{
			if(Text[NewCount] == 16)
			{
				HideNow = false;
				continue;
			}
			else
			{
				if(Text[NewCount] <= 26 || HideNow)
				{
					continue;
				}
				else
				{
					if(Text[NewCount] == _T('#'))
					{
						if(Text[NewCount + 1] == _T(':'))
						{
							++NewCount;
							continue;
						}
					}
				}
			}
		}

		++Result;
		if (Result > Count)
		{
			break;
		}
	}
	if (NewCount > 0)
	{
		Text = Text.substr(0,NewCount);
	}
}

PCSTRING GetServiceStatusesString(CONST UInt32 ss)
{
	static PCSTRING service_statuses[] =
	{
		_T("SS_RUNNING"),
		_T("SS_CONNECTION_COUNT"),
		_T("SS_WORLD_PLAYER_COUNT"),
		_T("SS_MAP_PLAYER_COUNT"),
	};

	if (ss < 0 || ss >= SS_MAX)
		return (_T("SS_FAILED"));
	return (service_statuses[ss]);
}

PCSTRING GetServiceRunningStatusString(CONST UInt32 srs)
{
	static PCSTRING service_running_status[] =
	{
		_T("SRS_UNKNOWN"),
		_T("SRS_STARTED"),
		_T("SRS_STOPPED"),
		_T("SRS_STARTING"),
		_T("SRS_STOPPING"),
		_T("SRS_WAITSTART"),
		_T("SRS_WAITSTOP"),
		_T("SRS_WAITRESTART"),
		_T("SRS_ERROR"),
	};

	if (srs < 0 || srs >= SRS_MAX)
		return (_T("SRS_FAILED"));
	return (service_running_status[srs]);
}

void RemoveLink(String &modifyString)
{
	String removedLinkStr;
	bool canInsert = true;
	for (Index i = C_INDEX(0); i < modifyString.size(); ++i)
	{
		if (modifyString[i] == 15)
			canInsert = false;
		else if (modifyString[i] == 16)
			canInsert = true;

		if (canInsert)
		{
			if (modifyString[i] < 15 || modifyString[i] > 20)
				removedLinkStr += modifyString[i];
		}
	}
	modifyString.Empty();
	modifyString = removedLinkStr;
}
PCSTRING gGlobalName = NULL;
ConfigFile* gGlobalConfig = NULL;
Logger* gGlobalLogger = NULL;
