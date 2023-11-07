#pragma once
#ifndef _REUBEN_NETWORK_NETWORK_H_
#define _REUBEN_NETWORK_NETWORK_H_

#define INVALID_IP				C_UINT32(0)
#define INVALID_PORT			C_UINT16(0)
#define INVALID_SOCKET_ID		C_UINT(~0)

namespace Reuben { namespace Network {
	
	typedef UInt32 IP;
	typedef UInt16 Port;
	typedef Int Protocol;
	typedef UInt SocketID;

	static CONST IP IP_ANY = INADDR_ANY;
	static CONST IP IP_LOOPBACK = INADDR_LOOPBACK;
	static CONST IP IP_BROADCAST = INADDR_BROADCAST;
	
	static CONST Protocol PROTOCOL_NONE = -1;
	static CONST Protocol PROTOCOL_TCP = IPPROTO_TCP;
	static CONST Protocol PROTOCOL_UDP = IPPROTO_UDP;

	// Transfer String to Network IP
	INLINE CONST IP StringToIP(PCSTRING pIp)
	{
		return (::inet_addr(StringA(pIp).c_str()));
	}

	// Transfer Network IP to String
	INLINE String IPToString(CONST IP ip)
	{
		in_addr addr;
		addr.S_un.S_addr = ip;
		return (String((PCSTRINGA)::inet_ntoa(addr)));
	}

	// Transfer String to Network Port
	INLINE CONST Port StringToPort(PCSTRING pPort)
	{
		return (::htons(::atoi(StringA(pPort).c_str())));
	}

	// Transfer Network Port to String
	INLINE String PortToString(CONST Port port)
	{
		CharA szBuffer[6];
		::itoa(::ntohs(port), szBuffer, 10);
		return (String(szBuffer));
	}

	INLINE CONST IP NetToHost(CONST IP ip)
	{
		return (::ntohl(ip));
	}

	INLINE CONST Port NetToHost(CONST Port port)
	{
		return (::ntohs(port));
	}

	INLINE CONST IP HostToNet(CONST IP ip)
	{
		return (::htonl(ip));
	}

	INLINE CONST Port HostToNet(CONST Port port)
	{
		return (::htons(port));
	}

	class NetworkException : public Exception
	{
	public:

		NetworkException(ErrorCode ec, PCSTRING pSourceLine)
			: Exception(ec, pSourceLine)
		{}

		NetworkException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pMessage)
			: Exception(ec, pSourceLine, pMessage)
		{}
	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_NETWORK_H_
