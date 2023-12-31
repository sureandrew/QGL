#pragma once
#ifndef _REUBEN_SYSTEM_ERROR_CODE_H_
#define _REUBEN_SYSTEM_ERROR_CODE_H_

//---------------------------------------------------------
//					Define Error Code Marco
//---------------------------------------------------------

#define EC_SEVERITY_SUCCESS		0x2
#define EC_SEVERITY_INFORMATION	0x6
#define EC_SEVERITY_WARNING		0xA
#define EC_SEVERITY_ERROR		0xE

#define EC_FACILITY_GLOBAL		1
#define EC_FACILITY_MEMORY		2
#define EC_FACILITY_FILE		3
#define EC_FACILITY_NETWORK		4
#define EC_FACILITY_ARRAY		5

#define MAKE_ERROR_CODE(_severity_, _facility_, _code_) \
	(ErrorCode)((0xF0000000 & ((ErrorCode)_severity_ << 28)) | (0x0FFF0000 & ((ErrorCode)_facility_ << 16)) | (0x0000FFFF & (ErrorCode)_code_))

#ifndef SUCCEEDED
#define SUCCEEDED(_ec_)		((ErrorCode)(_ec_) >= ERROR_CODE_NONE)
#endif

#ifndef FAILED
#define FAILED(_ec_)		((ErrorCode)(_ec_) < ERROR_CODE_NONE)
#endif

//---------------------------------------------------------
//					Define Global Error Code
//---------------------------------------------------------

#define EC_UNKNOWN						MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_GLOBAL, 1)
#define EC_CHECK_INVALID				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_GLOBAL, 2)
#define EC_ENSURE_INVALID				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_GLOBAL, 3)
#define EC_READ_INVALID					MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_GLOBAL, 4)
#define EC_WRITE_INVALID				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_GLOBAL, 5)
#define EC_NOT_IMPLEMENTED				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_GLOBAL, 6)

#define EC_MEM_ALLOC_FAILED				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_MEMORY, 1)
#define EC_MEM_POOL_ALLOC_FAILED		MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_MEMORY, 2)
#define EC_MEM_BAD_POINTER				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_MEMORY, 3)
#define EC_MEM_FLAG_INVALID				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_MEMORY, 4)
#define EC_MEM_WRITEOVER				MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_MEMORY, 5)

#define EC_NET_BUFFER_NULL_DATA			MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_NETWORK, 1)
#define EC_NET_BUFFER_READ_OVERFLOW		MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_NETWORK, 2)
#define EC_NET_BUFFER_WRITE_OVERFLOW	MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_NETWORK, 3)
#define EC_NET_BUFFER_EXPAND_FAILED		MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_NETWORK, 4)
#define EC_NET_NOT_IMPLEMENTED			MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_NETWORK, 5)

#define EC_ARRAY_READ_OVERFLOW			MAKE_ERROR_CODE(EC_SEVERITY_ERROR, EC_FACILITY_ARRAY, 1)

namespace Reuben { namespace System {

	typedef Int32 ErrorCode;

	PCSTRING GetErrorCodeString(CONST ErrorCode ec);

}; }; // Reuben::System

#endif // _REUBEN_SYSTEM_ERROR_CODE_H_
