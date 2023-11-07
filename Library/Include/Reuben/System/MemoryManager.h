#pragma once
#ifndef _REUBEN_SYSTEM_MEMORY_MANAGER_H_
#define _REUBEN_SYSTEM_MEMORY_MANAGER_H_

/*
	[] = 32bits | 4bytes

	debug pool additional memory schema
	[prev][next][source line][pointer][flag][0xEDEDEDED][0xEDEDEDED][check integer].....
	..[return addr1][return addr2][return addr3][return addr4][return addr5][return addr6][retrun addr7][check integer]

	debug global additional memory schema
	[prev][next][source line][pointer][flag][size][0xEDEDEDED][check integer].....
	..[return addr1][return addr2][return addr3][return addr4][return addr5][return addr6][retrun addr7][check integer]

	release pool and global additional memory schema
	[flag]

*/

#pragma warning(push)
#pragma warning(disable: 4312)

#ifdef COMPILER_MULTITHREADING
#include "..\Platform\Thread.h"

namespace Reuben { namespace System {

	typedef Reuben::Platform::CriticalSection ThreadKey;
	typedef Reuben::Platform::ThreadGuard<ThreadKey> ThreadGuard;

}; }; // Reuben::System

#endif // COMPILER_MULTITHREADING
#include "..\Utility\Singleton.h"

#include <boost/pool/pool.hpp>

#define REUBEN_SYSTEM_MEMORY_SKIP_FRAME 0
#define REUBEN_SYSTEM_MEMORY_FLAG_SIZE 4
#define REUBEN_SYSTEM_MEMORY_FLAG_MASK 0x0000FF00
#define REUBEN_SYSTEM_MEMORY_INDEX_MASK 0x000000FF
#define REUBEN_SYSTEM_MEMORY_POOL_FLAG 0x0000C500
#define REUBEN_SYSTEM_MEMORY_GLOBAL_FLAG 0x0000B200
#define REUBEN_SYSTEM_MEMORY_USE_FILE_LOG

#if defined(REUBEN_CONFIG_MEMORY_DEBUG_CHECK_ENABLE) && defined (COMPILER_DEBUG)
#define REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
#define REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE		64
#define REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE_HALF	32
#define REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32		16
#define REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF	8
#define REUBEN_SYSTEM_MEMORY_LEAK_REPORT(_filename_)			Reuben::System::MemoryManager::GetInstance().OutputLeakReport(_filename_)
#define REUEBN_SYSTEM_MEMORY_REGISTER_LEAK_REPORT(_filename_)	Reuben::System::MemoryManager::GetInstance().RegisterLeakReport(_filename_)
#else // REUBEN_CONFIG_MEMORY_DEBUG_CHECK_ENABLE && COMPILER_DEBUG
#define REUBEN_SYSTEM_MEMORY_LEAK_REPORT(_filename_)			0
#define REUEBN_SYSTEM_MEMORY_REGISTER_LEAK_REPORT(_filename_)	0
#define REUBEN_SYSTEM_MEMORY_BEGIN_LEAK_CHECK()					0
#define REUBEN_SYSTEM_MEMORY_END_LEAK_CHECK()					0
#endif // REUBEN_CONFIG_MEMORY_DEBUG_CHECK_ENABLE && COMPILER_DEBUG

#ifdef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

#define _HOOK_ALLOC     1
#define _HOOK_REALLOC   2
#define _HOOK_FREE      3

#ifdef __cplusplus
extern "C" {
#endif

typedef int (__cdecl * _CRT_ALLOC_HOOK)(int, void *, size_t, int, long, const unsigned char *, int);
extern _CRT_ALLOC_HOOK _pAllocHook;

INLINE _CRT_ALLOC_HOOK GetMemoryHook() { return _pAllocHook; }
INLINE _CRT_ALLOC_HOOK SetMemoryHook(_CRT_ALLOC_HOOK pHook)
{
	_CRT_ALLOC_HOOK oldHook = _pAllocHook;
	_pAllocHook = pHook;
	return oldHook;
}

#ifdef __cplusplus
}
#endif

#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

namespace Reuben { namespace System {

	class MemoryException : public Exception
	{
	public:
		MemoryException(ErrorCode ec, PCSTRING pSourceLine)
			: Exception(ec, pSourceLine)
		{}
	};

    class MemoryManager
		: public Reuben::Utility::Singleton<MemoryManager, Reuben::Utility::Allocation::Static>
	{
	private:

		struct PoolType : public BoostPool<boost::default_user_allocator_malloc_free>
		{
			ThreadKey key;
#ifdef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			Size size;
			volatile LONG cAllocated;
			volatile LONG cDeallocated;
			volatile LONG cUsedMemory;
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

			PoolType(CONST Size _size)
				: BoostPool<boost::default_user_allocator_malloc_free>(_size)
#ifdef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
				, size(_size)
				, cAllocated(0)
				, cDeallocated(0)
				, cUsedMemory(0)
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			{}
		};

		PoolType m_Pool8;
		PoolType m_Pool16;
		PoolType m_Pool32;
		PoolType m_Pool64;
		PoolType m_Pool128;
		PoolType m_Pool256;
		PoolType m_Pool512;

		PoolType* m_aPools[7];

#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
		FILE* m_pLogFile;
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG

#ifdef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		Char m_szLeakReport[PATH_SIZE];
		ThreadKey m_keyAllocated;
		UInt32* m_pAllocated;
		
		volatile LONG m_cAllocated;
		volatile LONG m_cDeallocated;
		volatile LONG m_cUsedMemory;
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

	private:

		INLINE MemoryManager(VOID);
		SINGLETON(MemoryManager);

		INLINE CONST Boolean __FASTCALL GetFitPoolIndex(Index & idx, CONST Size size);
		static VOID DoLeakReport(VOID);

	public:

		INLINE VOID __CDECL RegisterLeakReport(PCSTRING pFileName);
		VOID __CDECL OutputLeakReport(PCSTRING pFileName);
		INLINE ErrorCode __CDECL CheckMemory(CONST VOID* pBlock);
		INLINE VOID* __CDECL Allocate(Size size, PCSTRING pSourceLine);
		INLINE VOID __CDECL Deallocate(VOID* pBlock, CONST Size size, PCSTRING pSourceLine);
		String GetStatus(VOID);
	};

	INLINE MemoryManager::MemoryManager(VOID)
#ifndef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		: m_Pool8(C_SIZE(REUBEN_SYSTEM_MEMORY_FLAG_SIZE + 8))
		, m_Pool16(C_SIZE(REUBEN_SYSTEM_MEMORY_FLAG_SIZE + 16))
		, m_Pool32(C_SIZE(REUBEN_SYSTEM_MEMORY_FLAG_SIZE + 32))
		, m_Pool64(C_SIZE(REUBEN_SYSTEM_MEMORY_FLAG_SIZE + 64))
		, m_Pool128(C_SIZE(REUBEN_SYSTEM_MEMORY_FLAG_SIZE + 128))
		, m_Pool256(C_SIZE(REUBEN_SYSTEM_MEMORY_FLAG_SIZE + 256))
		, m_Pool512(C_SIZE(REUBEN_SYSTEM_MEMORY_FLAG_SIZE + 512))
#else // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		: m_Pool8(C_SIZE(REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + 8))
		, m_Pool16(C_SIZE(REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + 16))
		, m_Pool32(C_SIZE(REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + 32))
		, m_Pool64(C_SIZE(REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + 64))
		, m_Pool128(C_SIZE(REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + 128))
		, m_Pool256(C_SIZE(REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + 256))
		, m_Pool512(C_SIZE(REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + 512))
		, m_pAllocated(NULL)
		, m_cAllocated(0)
		, m_cDeallocated(0)
		, m_cUsedMemory(0)
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
	{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
		TCHAR szPath[PATH_SIZE];
		GetModuleFileName(GetModuleHandle(NULL), szPath, PATH_SIZE);
		::_tcscat(szPath, _T("_memory.log"));
		m_pLogFile = _tfopen(szPath, _T("a"));
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG

		m_aPools[0] = &m_Pool8;
		m_aPools[1] = &m_Pool16;
		m_aPools[2] = &m_Pool32;
		m_aPools[3] = &m_Pool64;
		m_aPools[4] = &m_Pool128;
		m_aPools[5] = &m_Pool256;
		m_aPools[6] = &m_Pool512;
	}

	INLINE CONST Boolean __FASTCALL MemoryManager::GetFitPoolIndex(Index& idx, CONST Size size)
	{
		idx = INDEX_MAX_VALUE;
		Size sizeFit = C_SIZE(8);
		for (Index i = C_INDEX(0); i < 7; ++i)
		{
			if (sizeFit >= size)
			{
				idx = i;
				return (BOOLEAN_TRUE);
			}
			sizeFit <<= 1;
		}
		return (BOOLEAN_FALSE);
	}

	INLINE VOID __CDECL MemoryManager::RegisterLeakReport(PCSTRING pFileName)
	{
#ifdef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		m_szLeakReport[0] = _T('\0');
		::_tcscpy(m_szLeakReport, pFileName);
		::atexit(DoLeakReport);
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
	}

	INLINE ErrorCode __CDECL MemoryManager::CheckMemory(CONST VOID* pBlock)
	{
		UInt32* pBlockTemp = (UInt32*)pBlock;
#ifndef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		pBlockTemp -= 1;
		if (::IsBadReadPtr(pBlockTemp, sizeof(UInt32)))
			return (EC_MEM_BAD_POINTER);
#else // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

		pBlockTemp -= REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF;
		if (::IsBadReadPtr(pBlockTemp, REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE_HALF))
			return (EC_MEM_BAD_POINTER);
		UInt32 uFlags = *(pBlockTemp + 4) & REUBEN_SYSTEM_MEMORY_FLAG_MASK;
		if (uFlags == REUBEN_SYSTEM_MEMORY_POOL_FLAG)
		{
			UInt32 check = C_UINT32(0);
			for (Index i = C_INDEX(2); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i) // skip first 2, because it is link list may be change anytime
				check ^= *(pBlockTemp + i);
			if (check != *(pBlockTemp + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)))
			{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				if (m_pLogFile != NULL)
				{
					UInt32 buffSize = (8 << CAST_INDEX(REUBEN_SYSTEM_MEMORY_INDEX_MASK & *(pBlockTemp + 4))) + REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE;
					::fwrite(pBlockTemp, buffSize, 1, m_pLogFile);
					::_ftprintf(m_pLogFile, _T("\n^=-- is the buffer dump about pool memory head check for %08X, size %d\n"), PtrToUint(pBlockTemp), buffSize);
					::fflush(m_pLogFile);
				}
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				return (EC_MEM_WRITEOVER);
			}

			Index idxPool = CAST_INDEX(REUBEN_SYSTEM_MEMORY_INDEX_MASK & *(pBlockTemp + 4));
			UInt32* pBlockLast = (UInt32*)((Byte*)(pBlockTemp + REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF) + (C_SIZE(8) << idxPool));
			check = C_UINT32(0);
			for (Index i = C_INDEX(0); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i)
				check ^= *(pBlockLast + i);
			if (check != *(pBlockLast + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)))
			{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				if (m_pLogFile != NULL)
				{
					UInt32 buffSize = (8 << CAST_INDEX(REUBEN_SYSTEM_MEMORY_INDEX_MASK & *(pBlockTemp + 4))) + REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE;
					::fwrite(pBlockTemp, buffSize, 1, m_pLogFile);
					::_ftprintf(m_pLogFile, _T("\n^=-- is the buffer dump about pool memory tail check for %08X, size %d\n"), PtrToUint(pBlockTemp), buffSize);
					::fflush(m_pLogFile);
				}
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				return (EC_MEM_WRITEOVER);
			}
		}
		else if (uFlags == REUBEN_SYSTEM_MEMORY_GLOBAL_FLAG)
		{
			UInt32 check = C_UINT32(0);
			for (Index i = C_INDEX(2); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i) // skip first 2, because it is link list may be change anytime
				check ^= *(pBlockTemp + i);
			if (check != *(pBlockTemp + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)))
			{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				if (m_pLogFile != NULL)
				{
					UInt32 buffSize = *(pBlockTemp + 5) + REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE;
					::fwrite(pBlockTemp, buffSize, 1, m_pLogFile);
					::_ftprintf(m_pLogFile, _T("\n^=-- is the buffer dump about heap memory head check for %08X, size %d\n"), PtrToUint(pBlockTemp), buffSize);
					::fflush(m_pLogFile);
				}
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				return (EC_MEM_WRITEOVER);
			}

			UInt32 realsize = *(pBlockTemp + 5);
			UInt32* pBlockLast = (UInt32*)((Byte*)(pBlockTemp + REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF) + realsize);
			check = C_UINT32(0);
			for (Index i = C_INDEX(0); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i)
				check ^= *(pBlockLast + i);
			if (check != *(pBlockLast + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)))
			{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				if (m_pLogFile != NULL)
				{
					UInt32 buffSize = *(pBlockTemp + 5) + REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE;
					::fwrite(pBlockTemp, buffSize, 1, m_pLogFile);
					::_ftprintf(m_pLogFile, _T("\n^=-- is the buffer dump about heap memory tail check for %08X, size %d\n"), PtrToUint(pBlockTemp), buffSize);
					::fflush(m_pLogFile);
				}
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				return (EC_MEM_WRITEOVER);
			}
		}
		else
			return (EC_MEM_FLAG_INVALID);
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		return (S_OK);
	}

	INLINE VOID* __CDECL MemoryManager::Allocate(Size size, PCSTRING pSourceLine)
	{
		if (size == C_SIZE(0))
			return (NULL);

		Index idxPool = INDEX_MAX_VALUE;

		if (GetFitPoolIndex(idxPool, size))
		{
			UInt32* pBlock = NULL;
			PoolType* pPool = m_aPools[idxPool];

			// allocate memory from pool
			{
#ifdef COMPILER_MULTITHREADING
				ThreadGuard guard(pPool->key);
#endif // COMPILER_MULTITHREADING
				pBlock = (UInt32*)pPool->malloc();
			}
			if (!pBlock)
			{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				if (m_pLogFile != NULL)
				{
					SYSTEMTIME st;
					::GetLocalTime(&st);
					::_ftprintf(m_pLogFile, _T("[%04d-%02d-%02d %02d.%02d.%02d] MemoryManager::Deallocate Error : allocate pool memory failed, requested at %s, requested size %d\n"), 
						st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, pSourceLine, size);
					::fflush(m_pLogFile);
				}
				return (NULL);
#else // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				throw MemoryException(EC_MEM_POOL_ALLOC_FAILED, pSourceLine);
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
			}

#ifndef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			*pBlock = REUBEN_SYSTEM_MEMORY_POOL_FLAG | CAST_UINT32(idxPool);
			++pBlock;
#else // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			
			memset(pBlock, 0xED, REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + (8 << idxPool));

			// allocate memory from pool for marking before allocated memory
#ifdef COMPILER_MULTITHREADING
			ThreadGuard guard(m_keyAllocated);
#endif // COMPILER_MULTITHREADING

			UInt32* pMarker = pBlock;
			*pMarker = 0;
			*(pMarker + 1) = (UInt32)m_pAllocated;
			*(pMarker + 2) = (UInt32)pSourceLine;
			*(pMarker + 3) = (UInt32)(pBlock + REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF);
			if (m_pAllocated)
				*m_pAllocated = (UInt32)pMarker;
			m_pAllocated = pMarker;

			// call allocate hook
			if (_pAllocHook)
				(*_pAllocHook)(_HOOK_ALLOC, (void*) (*(pMarker + 3)), size, 1, (long)*(pMarker + 3), NULL, 0);

			*(pBlock + 4) = REUBEN_SYSTEM_MEMORY_POOL_FLAG | CAST_UINT32(idxPool);

			// calculate first half check integer
			UInt32 check = C_UINT32(0);
			for (Index i = C_INDEX(2); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i) // skip first 2, because it is link list may be change anytime
				check ^= *(pBlock + i);
			*(pBlock + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)) = check;

			// get call stack and calculate last half check integer
			UInt32* pBlockLast = (UInt32*)((Byte*)(pBlock + REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF) + (C_SIZE(8) << idxPool));
			UInt* _ebp_;
			__asm { mov dword ptr [_ebp_], ebp };
#if REUBEN_SYSTEM_MEMORY_SKIP_FRAME != 0
			for (Index i = C_INDEX(0); i < REUBEN_SYSTEM_MEMORY_SKIP_FRAME; ++i)
			{
				// prevent access violation
				if ((UInt) _ebp_ <= 0x00010000 || IsBadReadPtr(_ebp_ + 1, sizeof(UInt)))
					break;
				_ebp_ = (UInt*)UIntToPtr(*_ebp_);	// retrieve previous frame to ebp pointer
			}
#endif // REUBEN_SYSTEM_MEMORY_SKIP_FRAME
			check = C_UINT32(0);
			for (Index i = C_INDEX(0); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i)
			{
				// prevent access violation
				if ((UInt) _ebp_ > 0x00010000 && !IsBadReadPtr(_ebp_ + 1, sizeof(UInt)))
				{
					*(pBlockLast + i) = *(_ebp_ + 1);	// retrieve return address
					_ebp_ = (UInt*)UIntToPtr(*_ebp_);	// retrieve previous frame to ebp pointer
				}
				check ^= *(pBlockLast + i);
			}
			*(pBlockLast + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)) = check;

			pBlock += REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF;

			::InterlockedIncrement(&pPool->cAllocated);
			::InterlockedExchangeAdd(&pPool->cUsedMemory, (LONG)pPool->size);

#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			return ((VOID*)pBlock);
		}
		else
		{
#ifndef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			UInt32* pBlock = (UInt32*)malloc((size_t)size + REUBEN_SYSTEM_MEMORY_FLAG_SIZE);
#else // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			UInt32* pBlock = (UInt32*)malloc((size_t)size + REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE);
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

			if (!pBlock)
			{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				if (m_pLogFile != NULL)
				{
					SYSTEMTIME st;
					::GetLocalTime(&st);
					::_ftprintf(m_pLogFile, _T("[%04d-%02d-%02d %02d.%02d.%02d] MemoryManager::Deallocate Error : allocate memory failed, requested at %s, requested size %d\n"), 
						st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, pSourceLine, size);
					::fflush(m_pLogFile);
				}
				return (NULL);
#else // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
				throw MemoryException(EC_MEM_ALLOC_FAILED, pSourceLine);
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
			}

#ifndef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			*pBlock = REUBEN_SYSTEM_MEMORY_GLOBAL_FLAG;
			++pBlock;
#else // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

			memset(pBlock, 0xED, size + REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE);

			// allocate memory from pool for marking before allocated memory
#ifdef COMPILER_MULTITHREADING
			ThreadGuard guard(m_keyAllocated);
#endif // COMPILER_MULTITHREADING

			UInt32* pMarker = (UInt32*)(pBlock);
			*pMarker = 0;
			*(pMarker + 1) = (UInt32)m_pAllocated;
			*(pMarker + 2) = (UInt32)pSourceLine;
			*(pMarker + 3) = (UInt32)(pBlock + REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF);
			if (m_pAllocated)
				*m_pAllocated = (UInt32)pMarker;
			m_pAllocated = pMarker;

			// call allocate hook
			if (_pAllocHook)
				(*_pAllocHook)(_HOOK_ALLOC, (void*) (*(pMarker + 3)), size, 1, (long)*(pMarker + 3), NULL, 0);

			*(pBlock + 4) = REUBEN_SYSTEM_MEMORY_GLOBAL_FLAG;
			*(pBlock + 5) = (UInt32)size;

			// calculate first half check integer
			UInt32 check = C_UINT32(0);
			for (Index i = C_INDEX(2); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i) // skip first 2, because it is link list may be change anytime
				check ^= *(pBlock + i);
			*(pBlock + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)) = check;

			// get call stack and calculate last half check integer
			UInt32* pBlockLast = (UInt32*)((Byte*)(pBlock + REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF) + size);
			UInt* _ebp_;
			__asm { mov dword ptr [_ebp_], ebp };
#if REUBEN_SYSTEM_MEMORY_SKIP_FRAME != 0
			for (Index i = C_INDEX(0); i < REUBEN_SYSTEM_MEMORY_SKIP_FRAME; ++i)
			{
				if (IsBadReadPtr(_ebp_ + 1, sizeof(UInt)))
					break;
				_ebp_ = (UInt*)UIntToPtr(*_ebp_);	// retrieve previous frame to ebp pointer
			}
#endif // REUBEN_SYSTEM_MEMORY_SKIP_FRAME
			check = C_UINT32(0);
			for (Index i = C_INDEX(0); i < (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1); ++i)
			{
				if (!IsBadReadPtr(_ebp_ + 1, sizeof(UInt)))
				{
					*(pBlockLast + i) = *(_ebp_ + 1);	// retrieve return address
					_ebp_ = (UInt*)UIntToPtr(*_ebp_);	// retrieve previous frame to ebp pointer
				}
				check ^= *(pBlockLast + i);
			}
			*(pBlockLast + (REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF - 1)) = check;

			pBlock += REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF;

			::InterlockedIncrement(&m_cAllocated);
			::InterlockedExchangeAdd(&m_cUsedMemory, (LONG)size);

#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			return ((VOID*)pBlock);
		}
	}

	INLINE VOID __CDECL MemoryManager::Deallocate(VOID* pBlock, CONST Size size, PCSTRING pSourceLine)
	{
		if (pBlock == NULL)
			return;

		ErrorCode ec = S_OK;
		if ((ec = CheckMemory(pBlock)) != S_OK)
		{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
			if (m_pLogFile != NULL)
			{
				SYSTEMTIME st;
				::GetLocalTime(&st);
				::_ftprintf(m_pLogFile, _T("[%04d-%02d-%02d %02d.%02d.%02d] MemoryManager::Deallocate Error : CheckMemory Failed, Error Code %08X, SourceLine %s\n"), 
					st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, ec, pSourceLine);
				::fflush(m_pLogFile);
			}
			return;
#else // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
			throw MemoryException(ec, pSourceLine);
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
		}
		UInt32* pBlockTemp = (UInt32*)pBlock;
#ifndef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		--pBlockTemp;
		UInt32 uFlags = *pBlockTemp;
#else // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
		pBlockTemp -= REUBEN_SYSTEM_MEMORY_ADDITIONAL_UINT32_HALF;
		UInt32 uFlags = *(pBlockTemp + 4);
#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

		if ((uFlags & REUBEN_SYSTEM_MEMORY_FLAG_MASK) == REUBEN_SYSTEM_MEMORY_POOL_FLAG)
		{
			Index idxPool = CAST_INDEX(uFlags & REUBEN_SYSTEM_MEMORY_INDEX_MASK);
			PoolType* pPool = m_aPools[idxPool];

#ifdef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			// remove the marker from allocated list
			{
#ifdef COMPILER_MULTITHREADING
				ThreadGuard guard(m_keyAllocated);
#endif // COMPILER_MULTITHREADING
				UInt32* pMarker = (UInt32*)(pBlockTemp);
				if (*pMarker)
					*(((UInt32*)*pMarker) + 1) = (UInt32)*(pMarker + 1);
				else if (m_pAllocated == pMarker)
					m_pAllocated = (UInt32*)*(pMarker + 1);
				if (*(pMarker + 1))
					*(UInt32*)(*(pMarker + 1)) = (UInt32)*pMarker;

				if (_pAllocHook) // call allocate hook
					(*_pAllocHook)(_HOOK_FREE, (void*) (*(pMarker + 3)), size, 1, (long)*(pMarker + 3), NULL, 0);
			}

			memset(pBlockTemp, 0xDE, REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE + (8 << idxPool));
			
			::InterlockedIncrement(&pPool->cDeallocated);
			::InterlockedExchangeAdd(&pPool->cUsedMemory, -(LONG)pPool->size);

#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

			// free memory to pool
			{
#ifdef COMPILER_MULTITHREADING
				ThreadGuard guard(pPool->key);
#endif // COMPILER_MULTITHREADING
				pPool->free((VOID*)pBlockTemp);
			}
		}
		else if ((uFlags & REUBEN_SYSTEM_MEMORY_FLAG_MASK) == REUBEN_SYSTEM_MEMORY_GLOBAL_FLAG)
		{
#ifdef REUBEN_SYSTEM_MEMORY_DEBUG_CHECK
			// remove the marker from allocated list
			{
#ifdef COMPILER_MULTITHREADING
				ThreadGuard guard(m_keyAllocated);
#endif // COMPILER_MULTITHREADING
				UInt32* pMarker = (UInt32*)(pBlockTemp);
				if (*pMarker)
					*(((UInt32*)*pMarker) + 1) = (UInt32)*(pMarker + 1);
				else if (m_pAllocated == pMarker)
					m_pAllocated = (UInt32*)*(pMarker + 1);
				if (*(pMarker + 1))
					*(UInt32*)(*(pMarker + 1)) = (UInt32)*pMarker;

				if (_pAllocHook) // call allocate hook
					(*_pAllocHook)(_HOOK_FREE, (void*) (*(pMarker + 3)), size, 1, (long)*(pMarker + 3), NULL, 0);
			}
			UInt32 uSize = *(pBlockTemp + 5);
			memset(pBlockTemp, 0xDE, uSize + REUBEN_SYSTEM_MEMORY_ADDITIONAL_BYTE);

			::InterlockedIncrement(&m_cDeallocated);
			::InterlockedExchangeAdd(&m_cUsedMemory, -(LONG)uSize);

#endif // REUBEN_SYSTEM_MEMORY_DEBUG_CHECK

			free((VOID*)pBlockTemp);
		}
		else
		{
#ifdef REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
			if (m_pLogFile != NULL)
			{
				SYSTEMTIME st;
				::GetLocalTime(&st);
				::_ftprintf(m_pLogFile, _T("[%04d-%02d-%02d %02d.%02d.%02d] MemoryManager::Deallocate Error : flags not match, SourceLine %s\n"), 
					st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, pSourceLine);
				::fflush(m_pLogFile);
			}
#else // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
			throw MemoryException(EC_MEM_FLAG_INVALID, pSourceLine);
#endif // REUBEN_SYSTEM_MEMORY_USE_FILE_LOG
		}
	}

}; };  // Reuben::System

#pragma warning(pop)

#endif // _REUBEN_SYSTEM_MEMORY_MANAGER_H_
