#pragma once
#ifndef _REUBEN_PLATFORM_THREAD_H_
#define _REUBEN_PLATFORM_THREAD_H_

#define INVALID_THREAD_ID	C_UINT32(0xFFFFFFFF)

namespace Reuben { namespace Platform {

	typedef UInt32 ThreadID;

	//-- ThreadGuard

	template <typename _ITEM_>
	class ThreadGuard
	{
	private:

		_ITEM_ & m_Lockable;

	public:

		INLINE ThreadGuard(_ITEM_& lockable)
			: m_Lockable(lockable)
		{
			m_Lockable.Lock();
		}

		INLINE ~ThreadGuard(VOID)
		{
			m_Lockable.Unlock();
		}
	};

	//-- CriticalSection

	class CriticalSection
	{
	public:

		CRITICAL_SECTION m_cs;

	public:

		INLINE CriticalSection(VOID)
		{
			::InitializeCriticalSection(&m_cs);
		}

		INLINE ~CriticalSection(VOID)
		{
			::DeleteCriticalSection(&m_cs);
		}

		INLINE VOID Lock(VOID)
		{
			::EnterCriticalSection(&m_cs);
		}

		INLINE VOID Unlock(VOID)
		{
			::LeaveCriticalSection(&m_cs);
		}
	};

	//-- Mutex

	class Mutex
	{
	public:

		HANDLE m_hMutex;

	public:

		INLINE Mutex(VOID)
		{
			m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
		}

		INLINE ~Mutex(VOID)
		{
			::CloseHandle(m_hMutex);
		}

		INLINE VOID Lock(VOID)
		{
			::WaitForSingleObject(m_hMutex, INFINITE);
		}

		INLINE VOID Unlock(VOID)
		{
			::ReleaseMutex(m_hMutex);
		}
		
	};

};}; // Reuben::Platform

#endif // _REUBEN_PLATFORM_THREAD_H_
