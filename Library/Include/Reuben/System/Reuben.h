#pragma once
#ifndef _REUBEN_SYSTEM_REUBEN_H_
#define _REUBEN_SYSTEM_REUBEN_H_

//---------------------------------------------------------
//					Reuben Library
//---------------------------------------------------------

//-- Global Version
#define REUBEN_VER_0_0	0x0000
#define REUBEN_VER		REUBEN_VER_0_0

//-- Global Namespace
namespace Reuben { namespace System { }; };
using namespace Reuben::System;

//-- Preifx
#include "Config.h"
#include "Primitive.h"
#include "ManagedMemory.h"
#include "ManagedAllocator.h"

//-- Local
#include "String.h"
#include "Uuid.h"
#include "Exception.h"

//-- Global Function
namespace Reuben { namespace System {

	CONST Boolean Initialize(VOID);
	VOID Uninitialize(VOID);

	CONST Bit32 GetConfigFlags(VOID);

	CONST String GetCurrentTimeString(PCSTRING pFormat = _T("%04d-%02d-%02d %02d.%02d.%02d.%04d"));
	CONST String GetUserName(VOID);
	CONST String GetComputerName(VOID);
	CONST String GetPlatformVersionString(VOID);
	CONST String GetFullFileName(PCSTRING pFileName);
	CONST String& GetApplicationFullFileName(VOID);
	CONST String& GetApplicationFileName(VOID);
	CONST String& GetApplicationName(VOID);
	CONST String& GetApplicationDirectory(VOID);
	CONST String& GetErrorDirectory(VOID);
	CONST String& GetConfigDirectory(VOID);
	CONST String& GetLogDirectory(VOID);
	CONST String& GetDataDirectory(VOID);

}; }; // Reuben::System

//-- Debug
#include "..\Diagnostics\Trace.h"
#include "..\Diagnostics\Debug.h"

//-- Log
#include "..\Diagnostics\Logger.h"

//-- Memory
#include "MemoryManager.h"
#include "ManagedMemoryImpl.h"

//-- Global Function 2
namespace Reuben { namespace System {

	Reuben::Utility::ConfigFile& GetConfig(VOID);
	Reuben::Diagnostics::Logger& GetLogger(VOID);

}; }; // Reuben::System

#define REUBEN_CONFIG Reuben::System::GetConfig()
#define REUBEN_LOGGER Reuben::System::GetLogger()

#endif // _REUBEN_SYSTEM_REUBEN_H_
