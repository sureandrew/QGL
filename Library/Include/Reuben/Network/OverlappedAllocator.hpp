#pragma once
#ifndef _REUBEN_NETWORK_OVERLAPPED_ALLOCATOR_H_
#define _REUBEN_NETWORK_OVERLAPPED_ALLOCATOR_H_

namespace Reuben { namespace Network {

	template<typename _OVERLAPPED_>
	class OverlappedAllocator
	{
	private:

		UInt m_cPartition;
		ThreadKey m_key;
		UInt* m_pFreeList;
		UInt* m_pVirtualMemoryList;

	private:

		VOID PartitionVirtualMemory(UInt* pVirtualMemory)
		{
			Byte* pMem = (Byte*)++pVirtualMemory;
			ThreadGuard guard(m_key);
			for (Index idx = 0; idx < m_cPartition; ++idx)
			{
				*((UInt*)pMem) = (UInt)m_pFreeList;
				m_pFreeList = (UInt*)pMem;
				pMem += sizeof(_OVERLAPPED_);
			}
		}

		VOID RequestVirtualMemory(VOID)
		{
			DEBUG_CHECK(!m_pFreeList);

			UInt* pOldList = m_pVirtualMemoryList;
			m_pVirtualMemoryList = (UInt*)VirtualAlloc(NULL, Reuben::Platform::GetPageSize(), MEM_COMMIT, PAGE_READWRITE);
			*m_pVirtualMemoryList = (UInt)pOldList;
			PartitionVirtualMemory(m_pVirtualMemoryList);
		}

	public:

		OverlappedAllocator(VOID)
		{
			m_cPartition = ((Reuben::Platform::GetPageSize() - sizeof(UInt)) / sizeof(_OVERLAPPED_));
			m_pFreeList = NULL;
			m_pVirtualMemoryList = NULL;
			RequestVirtualMemory();
		}

		virtual ~OverlappedAllocator(VOID)
		{
			UInt* pVirtualMemory = m_pVirtualMemoryList;
			while (pVirtualMemory)
			{
				UInt* pNow = pVirtualMemory;
				pVirtualMemory = (UInt*)*pVirtualMemory;
				::VirtualFree(pNow, 0, MEM_RELEASE);
			}
			m_pVirtualMemoryList = NULL;
		}

		_OVERLAPPED_* Allocate(VOID)
		{
			ThreadGuard guard(m_key);
			if (m_pFreeList)
			{
				_OVERLAPPED_* pOverlapped = reinterpret_cast<_OVERLAPPED_*>(m_pFreeList);
				m_pFreeList = (UInt*)*m_pFreeList;
				if (!m_pFreeList)
					RequestVirtualMemory();
				return (pOverlapped);
			}
			return (NULL);
		}

		VOID Deallocate(_OVERLAPPED_* pOverlapped)
		{
			if (pOverlapped == NULL)
				return;
			ThreadGuard guard(m_key);
			*((UInt*)pOverlapped) = (UInt)m_pFreeList;
			m_pFreeList = (UInt*)pOverlapped;
		}

	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_OVERLAPPED_ALLOCATOR_H_
