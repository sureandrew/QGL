#pragma once
#ifndef _REUBEN_SYSTEM_SUFFIX_H_
#define _REUBEN_SYSTEM_SUFFIX_H_

#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0500
//-- Library
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#include <Mswsock.h>
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <math.h>

//---------------------------------------------------------
//					 Intrinsic C Function
//---------------------------------------------------------

#if defined(COMPILER_VC)
#	pragma intrinsic(memcmp)
#	pragma intrinsic(memcpy)
#	pragma intrinsic(memset)
#	pragma intrinsic(strcat)
#	pragma intrinsic(strcmp)
#	pragma intrinsic(strcpy)
#	pragma intrinsic(strlen)
#endif

#if defined(COMPILER_VC)
#	pragma intrinsic(abs)
#	pragma intrinsic(fabs)
#	pragma intrinsic(labs)
#	pragma intrinsic(acos)
#	pragma intrinsic(asin)
#	pragma intrinsic(cosh)
#	pragma intrinsic(fmod)
#	pragma intrinsic(pow)
#	pragma intrinsic(sinh)
#	pragma intrinsic(tanh)
#	pragma intrinsic(atan)
#	pragma intrinsic(exp)
#	pragma intrinsic(log10)
#	pragma intrinsic(sqrt)
#	pragma intrinsic(atan2)
#	pragma intrinsic(log)
#	pragma intrinsic(sin)
#	pragma intrinsic(tan)
#	pragma intrinsic(cos)
#endif

//---------------------------------------------------------
//					Library Wappers
//---------------------------------------------------------

#define StlVector				std::vector
#define StlMap					std::map
#define StlSet					std::set
#define StlList					std::list
#define StlMultiMap				std::multimap
#define StlMultiSet				std::multiset
#define StlStack				std::stack
#define StlDeque				std::deque
#define BoostArray				boost::array
#define BoostOptional			boost::optional
#define BoostPool				boost::pool

#define LIST_OF					boost::assign::list_of
#define MAP_LIST_OF				boost::assign::map_list_of

#endif // _REUBEN_SYSTEM_SUFFIX_H_
