#pragma once
#ifndef _REUBEN_NETWORK_ADDRESS_H_
#define _REUBEN_NETWORK_ADDRESS_H_

#include "Network.h"

namespace Reuben { namespace Network {

	class Address
	{
	private:

		IP m_ip;
		Port m_port;

	public:

		Address(VOID)
			: m_ip(INVALID_IP)
			, m_port(INVALID_PORT)
		{}

		Address(CONST IP ip, CONST Port port)
			: m_ip(ip)
			, m_port(port)
		{}

		INLINE VOID SetIP(CONST IP ip)
		{
			m_ip = ip;
		}

		INLINE CONST IP GetIP(VOID) CONST
		{
			return (m_ip);
		}

		INLINE VOID SetPort(CONST Port port)
		{
			m_port = port;
		}

		INLINE CONST Port GetPort(VOID) CONST
		{
			return (m_port);
		}

		INLINE VOID ToHost(VOID)
		{
			m_ip = ::ntohl(m_ip);
			m_port = ::ntohs(m_port);
		}

		INLINE VOID ToNet(VOID)
		{
			m_ip = ::htonl(m_ip);
			m_port = ::htons(m_port);
		}

		INLINE VOID Clear(VOID)
		{
			m_ip = INVALID_IP;
			m_port = INVALID_PORT;
		}

	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_ADDRESS_H_
