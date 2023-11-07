#pragma once
#ifndef _REUBEN_NETWORK_LISTENER_H_
#define _REUBEN_NETWORK_LISTENER_H_

#include "Peer.h"

#define REUBEN_NETWORK_LISTENER_DEFAULT_MAX_QUEUE		200L
#define REUBEN_NETWORK_LISTENER_DEFAULT_MAX_CONNECTION	5000L

namespace Reuben { namespace Network {

	class Listener
		: public Peer
	{
	private:

		LONG m_lMaxInQueue;
		LONG m_lMaxConnections;
		volatile LONG m_lConnections;

	public:

		Listener(VOID)
			: m_lMaxInQueue(REUBEN_NETWORK_LISTENER_DEFAULT_MAX_QUEUE)
			, m_lMaxConnections(REUBEN_NETWORK_LISTENER_DEFAULT_MAX_CONNECTION)
			, m_lConnections(0L)
		{}

		INLINE VOID SetMaxInQueue(CONST LONG lMaxInQueue)
		{
			m_lMaxInQueue = lMaxInQueue;
		}

		INLINE CONST LONG GetMaxInQueue(VOID) CONST
		{
			return (m_lMaxInQueue);
		}

		INLINE VOID SetMaxConnections(CONST LONG lMaxConnections)
		{
			m_lMaxConnections = lMaxConnections;
		}

		INLINE CONST LONG GetMaxConnections(VOID) CONST
		{
			return (m_lMaxConnections);
		}

		INLINE VOID IncrementConnection(VOID)
		{
			::InterlockedIncrement(&m_lConnections);
		}

		INLINE VOID DecrementConnection(VOID)
		{
			::InterlockedDecrement(&m_lConnections);
		}
		
		INLINE CONST LONG GetConnectionCount(VOID) CONST
		{
			return (m_lConnections);
		}

		INLINE virtual VOID Clear(VOID)
		{
			Peer::Clear();
			m_lMaxInQueue = REUBEN_NETWORK_LISTENER_DEFAULT_MAX_QUEUE;
			m_lMaxConnections = REUBEN_NETWORK_LISTENER_DEFAULT_MAX_CONNECTION;
			m_lConnections = 0L;
		}
	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_LISTENER_H_
