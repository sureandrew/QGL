#pragma once
#ifndef _REUBEN_UTILITY_PROCESS_MANAGER_H_
#define _REUBEN_UTILITY_PROCESS_MANAGER_H_

#include "LinkedList.h"

namespace Reuben { namespace Utility {

	class Process;

	class ProcessManager
	{
	protected:
		typedef SingleLinkedList<Process*> ProcessList;
		ProcessList m_listProcesses;		// actual process list

	public:

		ProcessManager(VOID) {}
		virtual ~ProcessManager(VOID);
		VOID Attach(Process* pProcess);
		VOID Detach(Process* pProcess);
		Boolean HasProcesses(VOID);
		VOID UpdateProcesses(CONST UInt32 uDeltaMilliseconds);

	};

};}; // Reuben::Utility

#endif // _REUBEN_UTILITY_PROCESS_MANAGER_H_
