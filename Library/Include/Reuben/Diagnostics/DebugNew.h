#pragma once
#ifndef _REUBEN_DIAGNOSTICS_DEBUGNEW_H_
#define _REUBEN_DIAGNOSTICS_DEBUGNEW_H_

#ifdef COMPILER_DEBUG
#	define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // COMPILER_DEBUG

#endif // _REUBEN_DIAGNOSTICS_DEBUGNEW_H_
