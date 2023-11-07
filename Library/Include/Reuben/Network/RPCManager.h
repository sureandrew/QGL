#pragma once
#ifndef _REUBEN_NETWORK_RPC_MANAGER_H_
#define _REUBEN_NETWORK_RPC_MANAGER_H_

#include "Network.h"
#include "Interface.h"
#include "RPC.h"
#include "..\Math\CRC.h"
#include "..\Platform\Thread.h"
#include "..\Utility\Process.h"
#include "..\Simulation\Simulation.h"
#include "..\Utility\LinkedList.h"

#define ERR_RPC_ENCRYPT			1
#define ERR_RPC_DECRYPT			2
#define ERR_RPC_COMPRESS		3
#define ERR_RPC_DECOMPRESS		4
#define ERR_RPC_CRC				5

namespace Reuben { namespace Network {

	struct RPCMessage
	{
		Peer* pPeer;
		Buffer* pBuffer;
	};

	struct VoidMiddle
	{
		static INLINE VOID Encrypt(Buffer* pBuffer, UInt32 secret) {}
		static INLINE CONST Boolean Decrypt(Buffer* pBuffer, UInt32 secret) { return (BOOLEAN_TRUE); }
		static INLINE VOID Compress(CONST Peer* pPeer, Buffer* pBuffer) {}
		static INLINE CONST Boolean Decompress(CONST Peer* pPeer, Buffer* pBuffer) { return (BOOLEAN_TRUE); }
		static INLINE VOID UpdateCRC(Buffer* pBuffer) {}
		static INLINE CONST Boolean IsValidCRC(Buffer* pBuffer) { return (BOOLEAN_TRUE); }
	};

	struct DefaultMiddle
	{
		static INLINE VOID Encrypt(Buffer* pBuffer, UInt32 secret){}
		static INLINE CONST Boolean Decrypt(Buffer* pBuffer, UInt32 secret) { return (BOOLEAN_TRUE); }
		static INLINE VOID Compress(CONST Peer* pPeer, Buffer* pBuffer){}
		static INLINE CONST Boolean Decompress(CONST Peer* pPeer, Buffer* pBuffer) { return (BOOLEAN_TRUE); }
		static INLINE VOID UpdateCRC(Buffer* pBuffer);
		static INLINE CONST Boolean IsValidCRC(Buffer* pBuffer);
	};

	INLINE VOID DefaultMiddle::UpdateCRC(Buffer* pBuffer)
	{
#ifdef REUBEN_CONFIG_NETWORK_CRC_CHECK_ENABLE
		Byte* pData = pBuffer->GetData();
		UInt32 ignore = (UInt32)(REUBEN_NETWORK_BUFFER_HEADER_SIZE + sizeof(UInt32));
		UInt32& uCrc =  *((UInt32*)(pData + REUBEN_NETWORK_BUFFER_HEADER_SIZE));
		uCrc = Reuben::Math::CRC32(pData + ignore, 
				(UInt32)pBuffer->GetSize() - ignore);
#endif // REUBEN_CONFIG_NETWORK_CRC_CHECK_ENABLE
	}

	INLINE CONST Boolean DefaultMiddle::IsValidCRC(Buffer* pBuffer)
	{
#ifdef REUBEN_CONFIG_NETWORK_CRC_CHECK_ENABLE
		Byte* pData = pBuffer->GetData();
		UInt32 ignore = (UInt32)(REUBEN_NETWORK_BUFFER_HEADER_SIZE + sizeof(UInt32));
		UInt32& uCrc1 = *((UInt32*)(pData + REUBEN_NETWORK_BUFFER_HEADER_SIZE));
		UInt32 uCrc2 = Reuben::Math::CRC32(pData + ignore, 
						(UInt32)pBuffer->GetSize() - ignore);
		return (uCrc1 == uCrc2);
#else // REUBEN_CONFIG_NETWORK_CRC_CHECK_ENABLE
		return (BOOLEAN_TRUE);
#endif // REUBEN_CONFIG_NETWORK_CRC_CHECK_ENABLE
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_ = DefaultMiddle>
	class RPCManager
		: public Reuben::Utility::Process
		, public IPacketHandler
	{

		//-- Template Required Interface
		//{
		//	RPCResult OnProcess(RPCMessage* pMsg);
		//}

	protected:

		typedef Reuben::Utility::SingleLinkedList<RPCMessage*> MessageList;

	protected:

		_MODEL_ m_model;
		Reuben::Diagnostics::Logger& m_logger;
		Boolean m_bDelete;
		ThreadKey m_key;
		MessageList m_queueStore;

	public:

		INLINE RPCManager(PCSTRING pName,
			Reuben::Utility::ConfigFile& config = REUBEN_CONFIG,
			Reuben::Diagnostics::Logger& logger = REUBEN_LOGGER,
			Boolean bDelete = BOOLEAN_TRUE);
		INLINE virtual ~RPCManager(VOID);

		INLINE _MODEL_& GetModel(VOID);
		INLINE VOID Terminate(VOID);

		INLINE virtual VOID OnUpdate(CONST UInt32 uDeltaMilliseconds);

		// interface from IPacketHandler
		INLINE virtual VOID OnReceived(CONST Peer* pPeer, CONST Address* pFrom, Buffer* pBuffer);

		INLINE RPCResult Send(CONST Peer* pPeer, Buffer* pBuffer);
		template <unsigned int _V_> 
			INLINE RPCResult Send(CONST Peer* pPeer, Buffer* pBuffer);

		INLINE RPCResult Decode(CONST Peer* pPeer, Buffer* pBuffer);
		template <unsigned int _V_> 
			INLINE RPCResult Decode(CONST Peer* pPeer, Buffer* pBuffer);

		static INLINE VOID DeleteMessage(RPCMessage*& pMsg);

	protected:

		INLINE virtual VOID OnSendError(UInt16 err, CONST Peer* pPeer, Buffer* pBuffer);
		INLINE virtual VOID OnRecvError(UInt16 err, CONST Peer* pPeer, Buffer* pBuffer);

	private:

		INLINE _TYPE_& _This(VOID)
		{
			return (*((_TYPE_*)this));
		}
	};

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::RPCManager(
		PCSTRING pName,
		Reuben::Utility::ConfigFile& config,	// REUBEN_CONFIG
		Reuben::Diagnostics::Logger& logger,	// REUBEN_LOGGER
		Boolean bDelete							// BOOLEAN_TRUE
		)
		: m_model(pName, config, logger)
		, m_logger(logger)
		, m_bDelete(bDelete)
	{
		m_model.SetPacketHandler(this);
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::~RPCManager(VOID)
	{
		Terminate();
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE _MODEL_& RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::GetModel(VOID)
	{
		return (m_model);
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE VOID RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::Terminate(VOID)
	{
		ThreadGuard guard(m_key);

		if (m_queueStore.IsEmpty())
			return;

		TRACE_ERRORDTL_1(m_logger, _FE("termination, packets remain %u"), m_queueStore.GetCount());

		if (m_queueStore.MoveToFirst())
		{
			do
			{
				DeleteMessage(m_queueStore.GetCurrent());
			}
			while (m_queueStore.Forward());
			m_queueStore.RemoveAll();
		}
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE VOID RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::OnUpdate(CONST UInt32 uDeltaMilliseconds)
	{
		MessageList queueExecute;

		// Swap store list to execute list
		{
			ThreadGuard guard(m_key);
			m_queueStore.Swap(queueExecute);
		}

		while (!queueExecute.IsEmpty())
		{
			RPCMessage* pMsg = queueExecute.RemoveFirst();
			// Should check crc here if needed!!
			_This().OnProcess(pMsg);
			if (m_bDelete)
			{
				DeleteMessage(pMsg);
			}
		}

		m_model.Update();
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE VOID RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::OnSendError(UInt16 err, CONST Peer* pPeer, Buffer* pBuffer)
	{
		if (pPeer == NULL) return;

		String errStr;
		if (err == ERR_RPC_ENCRYPT)
			errStr = _T("Encrypt");
		else if (err == ERR_RPC_COMPRESS)
			errStr = _T("Compress");
		else if (err == ERR_RPC_CRC)
			errStr = _T("CRC");

		TRACE_ERRORDTL_4(m_logger, _FE("%s, socketId %d, buffer 0x%p, size %d"),
			errStr.c_str(), pPeer->GetSocketID(), pBuffer, pBuffer->GetSize());
		String s;
		pBuffer->DumpString(s, 0, pBuffer->GetSize());
		TRACE_ERRORDTL_1(m_logger, _T("%s"), s.c_str());
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE VOID RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::OnRecvError(UInt16 err, CONST Peer* pPeer, Buffer* pBuffer)
	{
		if (pPeer == NULL) return;

		String errStr;
		if (err == ERR_RPC_DECRYPT)
			errStr = _T("Decrypt");
		else if (err == ERR_RPC_DECOMPRESS)
			errStr = _T("Decompress");
		else if (err == ERR_RPC_CRC)
			errStr = _T("CRC");

		TRACE_ERRORDTL_4(m_logger, _FE("%s, socketId %d, buffer 0x%p, size %d"),
			errStr.c_str(), pPeer->GetSocketID(), pBuffer, pBuffer->GetSize());
		String s;
		pBuffer->DumpString(s, 0, pBuffer->GetSize());
		TRACE_ERRORDTL_1(m_logger, _T("%s"), s.c_str());
		//m_model.CloseSocket(pPeer->GetSocketID());
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE VOID RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::OnReceived(CONST Peer* pPeer, CONST Address* pFrom, Buffer* pBuffer)
	{
		if (pPeer == NULL) return;

		RPCMessage *pmsg = SafeAllocate(RPCMessage, 1);
		pmsg->pPeer = pPeer->AddRef();
		pmsg->pBuffer = pBuffer;

		//-- Add to list
		{
			ThreadGuard guard(m_key);
			m_queueStore.AddLast(pmsg);
		}
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE RPCResult RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::Send(CONST Peer* pPeer, Buffer* pBuffer)
	{
		_MIDDLE_::UpdateCRC(pBuffer);
		return (m_model.Send(pPeer, pBuffer) ? RPC_RESULT_OK : RPC_RESULT_FAIL);
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
	template <unsigned int _V_>
		INLINE RPCResult RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::Send(CONST Peer* pPeer, Buffer* pBuffer)
	{
		_MIDDLE_::UpdateCRC(pBuffer);
		_MIDDLE_::Compress(pPeer, pBuffer);
		_MIDDLE_::Encrypt<_V_>(pBuffer);
		return (m_model.Send(pPeer, pBuffer) ? RPC_RESULT_OK : RPC_RESULT_FAIL);
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE RPCResult RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::Decode(CONST Peer* pPeer, Buffer* pBuffer)
	{
		if (!_MIDDLE_::IsValidCRC(pBuffer))
		{
			OnRecvError(ERR_RPC_CRC, pPeer, pBuffer);
			return RPC_RESULT_FAIL;
		}
		return RPC_RESULT_OK;
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
	template <unsigned int _V_>
		INLINE RPCResult RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::Decode(CONST Peer* pPeer, Buffer* pBuffer)
	{
		if (!_MIDDLE_::Decrypt<_V_>(pBuffer))
		{
			OnRecvError(ERR_RPC_DECRYPT, pPeer, pBuffer);
			return RPC_RESULT_FAIL;
		}
		if (!_MIDDLE_::Decompress(pPeer, pBuffer))
		{
			OnRecvError(ERR_RPC_DECOMPRESS, pPeer, pBuffer);
			return RPC_RESULT_FAIL;
		}
		if (!_MIDDLE_::IsValidCRC(pBuffer))
		{
			OnRecvError(ERR_RPC_CRC, pPeer, pBuffer);
			return RPC_RESULT_FAIL;
		}
		return RPC_RESULT_OK;
	}

	template <typename _MODEL_, typename _TYPE_, typename _MIDDLE_>
		INLINE VOID RPCManager<_MODEL_, _TYPE_, _MIDDLE_>::DeleteMessage(RPCMessage*& pMsg)
	{
		DEBUG_CHECK(pMsg != NULL);
		SafeRelease(pMsg->pPeer);
		SafeRelease(pMsg->pBuffer);
		SafeDeallocate(pMsg);
	}

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_RPC_MANAGER_H_
