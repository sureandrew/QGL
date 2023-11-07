#pragma once
#ifndef _REUBEN_UTILITY_PROCESS_H_
#define _REUBEN_UTILITY_PROCESS_H_

#include "ProcessManager.h"

namespace Reuben { namespace Utility {

	class Process
	{
	protected:

		Boolean m_bPaused;
		ProcessManager* m_pProcessManager;
		Process* m_pNext;

	public:

		INLINE Process(VOID);
		virtual ~Process(VOID)
		{
			if (m_pNext)
				m_pNext = NULL;
			if (m_pProcessManager)
				m_pProcessManager->Detach(this);
		}

	public:

		INLINE Boolean IsAttached(VOID) CONST;
		INLINE VOID SetProcessManager(ProcessManager* pProcessManager);
		INLINE Boolean IsPaused(VOID) CONST;
		INLINE VOID TogglePause(VOID);
		INLINE Process* CONST GetNext(VOID) CONST;
		INLINE VOID SetNext(Process* pNext);
		virtual VOID OnAttach(VOID){}
		virtual VOID OnDetach(VOID){}
		virtual VOID OnUpdate(CONST UInt32 uDeltaMilliseconds){};

	private:

		Process(CONST Process & rhs);
		Process & operator = (const Process &);

	};

	INLINE Process::Process(VOID)
		: m_bPaused(BOOLEAN_FALSE)
		, m_pProcessManager(NULL)
		, m_pNext(NULL)
	{}

	INLINE Boolean Process::IsAttached(VOID) CONST
	{
		return (m_pProcessManager != NULL);
	}

	INLINE VOID Process::SetProcessManager(ProcessManager* pProcessManager)
	{
		m_pProcessManager = pProcessManager;
	}

	INLINE Boolean Process::IsPaused(VOID) CONST
	{
		return (m_bPaused);
	}

	INLINE VOID Process::TogglePause(VOID)
	{
		m_bPaused = !m_bPaused;
	}

	INLINE Process* CONST Process::GetNext(VOID) CONST
	{
		return (m_pNext);
	}

	INLINE VOID Process::SetNext(Process* pNext)
	{
		if (m_pNext)
			m_pNext = NULL;
		m_pNext = pNext;
	}

};}; // Reuben::Utility

#endif // _REUBEN_UTILITY_PROCESS_H_
