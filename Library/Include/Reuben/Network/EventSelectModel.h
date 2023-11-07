#pragma once
#ifndef _REUBEN_NETWORK_EVENT_SELECT_MODEL_H_
#define _REUBEN_NETWORK_EVENT_SELECT_MODEL_H_

#include "Interface.h"

namespace Reuben { namespace Network {

	class EventSelectModel
	{
	public:

		struct PImpl;
		PImpl* _pImpl;

	public:

		EventSelectModel(PCSTRING pName, Reuben::Utility::ConfigFile& config = REUBEN_CONFIG, Reuben::Diagnostics::Logger& logger = REUBEN_LOGGER);
		~EventSelectModel(VOID);

		//-- Stream

		// Open a listener for stream protocol. must release
		CONST Listener* Listen(CONST Address& listen, CONST Protocol protocol, CONST Count cMaxInQueue, CONST Count cMaxConnections, CONST Boolean bTrust);
		// Open a listener for datagram protocol. must release
		CONST Peer* Listen(CONST Address& listen, CONST Protocol protocol, CONST Boolean bTrust);
		// Start listening
		CONST Boolean StartListening(CONST Listener& listener);
		// Stop listening
		CONST Boolean StopListening(CONST Listener& listener);
		// Connect to remote peer for stream protocol. must release
		CONST Peer* Connect(CONST Address& remote, CONST Protocol protocol, CONST Boolean bTrust);
		// Clone data to a new buffer before attach to send list
		CONST Boolean Send(CONST Peer* pPeer, CONST Byte* pData, CONST Size size);
		// Directly attach buffer to send list
		CONST Boolean Send(CONST Peer* pPeer, Buffer* pBuffer);
		// Clone data to a new buffer before attach to send list, address specify
		CONST Boolean SendTo(CONST Peer* pPeer, CONST Address& to, CONST Byte* pData, CONST Size size);
		// Directly attach buffer to send list, address specify
		CONST Boolean SendTo(CONST Peer* pPeer, CONST Address& to, Buffer* pBuffer);
		// Get Listener. must release
		CONST Listener* GetListener(CONST SocketID sid);
		// Get Peer. must release
		CONST Peer* GetPeer(CONST SocketID sid);
		// Close socket
		CONST Boolean CloseSocket(CONST SocketID sid);
		// Shutdown
		VOID Shutdown(VOID);
		// Should call by the thread which manage client list
		VOID Update(VOID);
		// Handler
		VOID SetSocketHandler(ISocketHandler* pHandler);
		VOID SetPacketHandler(IPacketHandler* pHandler);

	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_EVENT_SELECT_MODEL_H_
