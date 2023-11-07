#pragma once
#ifndef _REUBEN_NETWORK_INTERFACE_H_
#define _REUBEN_NETWORK_INTERFACE_H_

#include "Buffer.h"
#include "Listener.h"

namespace Reuben { namespace Network {
	
	struct ISocketHandler
	{
		virtual VOID OnAccept(CONST Listener* listener, CONST Peer* pPeer) = 0;
		virtual VOID OnFull(CONST Listener* listener) = 0;
		virtual VOID OnOpen(CONST Listener* listener) = 0;
		virtual VOID OnOpen(CONST Listener* listener, CONST Peer* pPeer) = 0;
		virtual VOID OnClose(CONST Listener* listener) = 0;
		virtual VOID OnClose(CONST Listener* listener, CONST Peer* pPeer) = 0;
	};

	struct IPacketHandler
	{
		//-- Stream
		virtual VOID OnReceived(CONST Peer* pPeer, CONST Address* pFrom, Buffer* pBuffer) = 0;
		virtual VOID OnSent(CONST Peer* pPeer, CONST Address* pTo, Buffer* pBuffer) = 0;
	};

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_INTERFACE_H_
