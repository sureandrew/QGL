#pragma once
#ifndef _REUBEN_NETWORK_PEER_H_
#define _REUBEN_NETWORK_PEER_H_

#include "../Utility/RefCounter.h"
#include "Address.h"

namespace Reuben { namespace Network {

	// Because Used SafeDeleteObjectProc for the Reference Counter.
	// So Peer only can construct by SafeCreateObjectX function.

	class Peer
		: public Reuben::Utility::RefCounter<Peer, SafeDeleteObjectProc<Peer> >
	{
	public:

		struct IData
		{
			virtual VOID OnDispose(VOID) = 0;
		};

	private:

		SocketID m_sid;
		Address m_addressLocal;
		Address m_addressRemote;
		Protocol m_protocol;
		Boolean m_bTrust;
		Boolean m_bAlive;
		UInt32 m_uLastActionTime;
		mutable IData* m_pData;

	public:

		Peer(VOID)
			: m_sid(INVALID_SOCKET_ID)
			, m_protocol(PROTOCOL_NONE)
			, m_bTrust(BOOLEAN_FALSE)
			, m_pData(NULL)
			, m_bAlive(BOOLEAN_FALSE)
			, m_uLastActionTime(C_UINT32(0))
		{}

		virtual ~Peer(VOID)
		{
			if (m_pData)
				m_pData->OnDispose();
		}

		INLINE VOID SetSocketID(CONST SocketID sid)
		{
			m_sid = sid;
		}

		INLINE CONST SocketID GetSocketID(VOID) CONST
		{
			return (m_sid);
		}

		INLINE VOID SetAddressLocal(CONST Address& address)
		{
			m_addressLocal = address;
		}

		INLINE CONST Address& GetAddressLocal(VOID) CONST
		{
			return (m_addressLocal);
		}

		INLINE VOID SetAddressRemote(CONST Address& address)
		{
			m_addressRemote = address;
		}

		INLINE CONST Address& GetAddressRemote(VOID) CONST
		{
			return (m_addressRemote);
		}

		INLINE VOID SetProtocol(Protocol protocol)
		{
			m_protocol = protocol;
		}

		INLINE CONST Protocol GetProtocol(VOID) CONST
		{
			return (m_protocol);
		}

		INLINE VOID SetTrust(CONST Boolean bTrust)
		{
			m_bTrust = bTrust;
		}

		INLINE CONST Boolean IsTrust(VOID) CONST
		{
			return (m_bTrust);
		}

		INLINE VOID SetAlive(CONST Boolean bAlive)
		{
			m_bAlive = bAlive;
		}

		INLINE CONST Boolean IsAlive(VOID) CONST
		{
			return (m_bAlive);
		}

		INLINE VOID SetLastActionTime(CONST UInt32 uTime)
		{
			m_uLastActionTime = uTime;
		}

		INLINE CONST UInt32 GetLastActionTime(VOID) CONST
		{
			return (m_uLastActionTime);
		}

		INLINE VOID SetData(IData* pData) CONST
		{
			m_pData = pData;
		}

		INLINE IData* GetData(VOID) CONST
		{
			return (m_pData);
		}

		INLINE virtual VOID Clear(VOID)
		{
			m_sid = INVALID_SOCKET_ID;
			m_addressLocal.Clear();
			m_addressRemote.Clear();
			m_protocol = PROTOCOL_NONE;
			m_bTrust = BOOLEAN_FALSE;
			m_bAlive = BOOLEAN_FALSE;
			m_uLastActionTime = C_UINT32(0);
			m_pData = NULL;
		}
	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_PEER_H_
