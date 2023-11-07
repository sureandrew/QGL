#pragma once
#ifndef _REUBEN_NETWORK_MIDDLEFILTER_H_
#define _REUBEN_NETWORK_MIDDLEFILTER_H_

#include "Buffer.h"
#include "../Math/CRC.h"
#include "../Math/AHuffman.h"
#include "../Math/AES.h"

#define BUFFER_HEADER_SIZE		REUBEN_NETWORK_BUFFER_HEADER_SIZE
// overhead size = buffer header + CRC + RPCID size
#define BUFFER_OVERHEAD_SIZE	(BUFFER_HEADER_SIZE + sizeof(UInt32) + sizeof(UInt16))
#define AHUFFMAN_MEMORY_POOL	1

namespace Reuben { namespace Network {

	struct FilterData : public Peer::IData 
	{
		Reuben::Math::AHuffman* comp;		// compressor
		Reuben::Math::AHuffman* decomp;		// decompresser

		FilterData(VOID)
		{
#ifdef AHUFFMAN_MEMORY_POOL
			comp = Reuben::Math::AHuffman::NewInstance();
			decomp = Reuben::Math::AHuffman::NewInstance();
#else
			comp = SafeCreateObject(Reuben::Math::AHuffman);
			decomp = SafeCreateObject(Reuben::Math::AHuffman);
#endif
		}

		virtual ~FilterData(VOID)
		{
#ifdef AHUFFMAN_MEMORY_POOL
			Reuben::Math::AHuffman::Release(comp);
			Reuben::Math::AHuffman::Release(decomp);
#else
			SafeDeleteObject(comp);
			SafeDeleteObject(decomp);
#endif
		}
	};

	struct MiddleFilter
	{
		static INLINE VOID Encrypt(Buffer* pBuffer, UInt32 secret);
		static INLINE CONST Boolean Decrypt(Buffer* pBuffer, UInt32 secret);
		static INLINE VOID Compress(CONST Peer* pPeer, Buffer* pBuffer);
		static INLINE CONST Boolean Decompress(CONST Peer* pPeer, Buffer* pBuffer);
		static INLINE VOID UpdateCRC(Buffer* pBuffer);
		static INLINE CONST Boolean IsValidCRC(Buffer* pBuffer);

		template <unsigned int _V_> INLINE static VOID Encrypt(Buffer* pBuffer);
		template <unsigned int _V_> INLINE static CONST Boolean Decrypt(Buffer* pBuffer);
	};

	INLINE VOID MiddleFilter::Encrypt(Buffer* pBuffer, UInt32 secret)
	{
		if (secret == 0 || !pBuffer || pBuffer->GetSize() <= BUFFER_OVERHEAD_SIZE)
			return;

		Byte *pData = pBuffer->GetData() + BUFFER_OVERHEAD_SIZE;
		UInt16 dataSize = (UInt16) pBuffer->GetSize() - BUFFER_OVERHEAD_SIZE;

		Reuben::Math::AES::Encrypt(pData, dataSize, secret);
	}

	INLINE CONST Boolean MiddleFilter::Decrypt(Buffer* pBuffer, UInt32 secret)
	{
		if (secret == 0)
			return BOOLEAN_TRUE;

		if (!pBuffer || pBuffer->GetSize() <= BUFFER_OVERHEAD_SIZE)
			return BOOLEAN_FALSE;

		Byte *pData = pBuffer->GetData() + BUFFER_OVERHEAD_SIZE;
		UInt16 dataSize = (UInt16) pBuffer->GetSize() - BUFFER_OVERHEAD_SIZE;

		Reuben::Math::AES::Decrypt(pData, dataSize, secret);

		return BOOLEAN_TRUE;
	}

	template <unsigned int _V_>
		INLINE VOID MiddleFilter::Encrypt(Buffer* pBuffer)
	{
		if (!pBuffer || pBuffer->GetSize() <= BUFFER_OVERHEAD_SIZE)
			return;

		Byte *pData = pBuffer->GetData() + BUFFER_OVERHEAD_SIZE;
		UInt16 dataSize = (UInt16) pBuffer->GetSize() - BUFFER_OVERHEAD_SIZE;

		Reuben::Math::AES::Encrypt<_V_>(pData, dataSize);
	}

	template <unsigned int _V_>
		INLINE CONST Boolean MiddleFilter::Decrypt(Buffer* pBuffer)
	{
		if (!pBuffer || pBuffer->GetSize() <= BUFFER_OVERHEAD_SIZE)
			return BOOLEAN_FALSE;

		Byte *pData = pBuffer->GetData() + BUFFER_OVERHEAD_SIZE;
		UInt16 dataSize = (UInt16) pBuffer->GetSize() - BUFFER_OVERHEAD_SIZE;

		Reuben::Math::AES::Decrypt<_V_>(pData, dataSize);

		return BOOLEAN_TRUE;
	}

	INLINE VOID MiddleFilter::Compress(CONST Peer* pPeer, Buffer* pBuffer)
	{
		if (pPeer == NULL)
			return;

		if (!pBuffer || pBuffer->GetSize() <= BUFFER_OVERHEAD_SIZE)
		{
			TRACE_WARNDTL_2(REUBEN_LOGGER, _FW("null or buffer size too small, socketId %d, buffer 0x%p"), pPeer->GetSocketID(), pBuffer);
			return;
		}

		FilterData* pFilter = dynamic_cast<FilterData*>(pPeer->GetData());
		if (pFilter == NULL)
		{
			TRACE_WARNDTL_2(REUBEN_LOGGER, _FW("null filter in peer data, socketId %d, buffer 0x%p"), pPeer->GetSocketID(), pBuffer);
			return;
		}

		// create new buffer for compressed data, shouldn't excess to extend data
		UInt16 newSize = (UInt16) pBuffer->GetSize() + sizeof(UInt16) + AH_BUFFER_EXTEND;
		UInt16 overhead = BUFFER_OVERHEAD_SIZE + sizeof(UInt16);
		UInt16 encSize = newSize - overhead;
		Byte *pNewData = SafeAllocate(Byte, newSize);

		// store uncompressed buffer size
		*(UInt16*)(pNewData + BUFFER_OVERHEAD_SIZE) = (UInt16) pBuffer->GetSize();

		// copy data from header
		memcpy(pNewData, pBuffer->GetData(), BUFFER_OVERHEAD_SIZE);

		// encode start from offset
		TRACE_ENSURE(pFilter->comp);
		pFilter->comp->Encode(pBuffer->GetData() + BUFFER_OVERHEAD_SIZE, 
			(UInt16) pBuffer->GetSize() - BUFFER_OVERHEAD_SIZE, pNewData + overhead, encSize);

		// replace data of buffer
		pBuffer->SetData(pNewData, encSize + overhead, newSize);
	}

	INLINE CONST Boolean MiddleFilter::Decompress(CONST Peer* pPeer, Buffer* pBuffer)
	{
		if (pPeer == NULL)
			return BOOLEAN_FALSE;

		if (!pBuffer || pBuffer->GetSize() <= BUFFER_OVERHEAD_SIZE)
		{
			TRACE_WARNDTL_2(REUBEN_LOGGER, _FW("null or buffer size too small, socketId %d, buffer 0x%p"), pPeer->GetSocketID(), pBuffer);
			return BOOLEAN_FALSE;
		}

		FilterData* pFilter = dynamic_cast<FilterData*>(pPeer->GetData());
		if (pFilter == NULL)
		{
			TRACE_WARNDTL_2(REUBEN_LOGGER, _FW("null filter in peer data, socketId %d, buffer 0x%p"), pPeer->GetSocketID(), pBuffer);
			return BOOLEAN_FALSE;
		}

		// get actual buffer size in front of compressed data
		UInt16 newSize = *(UInt16*)(pBuffer->GetData() + BUFFER_OVERHEAD_SIZE);
		UInt16 overhead = BUFFER_OVERHEAD_SIZE + sizeof(UInt16);
		UInt16 actualSize = newSize - BUFFER_OVERHEAD_SIZE;

		// create new buffer for uncompressed data
		Byte *pNewData = SafeAllocate(Byte, newSize);

		// copy data from header
		memcpy(pNewData, pBuffer->GetData(), BUFFER_OVERHEAD_SIZE);

		// encode start from offset
		TRACE_ENSURE(pFilter->decomp);
		Boolean success = pFilter->decomp->Decode(pBuffer->GetData() + overhead, 
			(UInt16) pBuffer->GetSize() - overhead,
			pNewData + BUFFER_OVERHEAD_SIZE, actualSize);

		// check size is match
		if (!success || (actualSize + BUFFER_OVERHEAD_SIZE) != newSize)
		{
			TRACE_WARNDTL_5(REUBEN_LOGGER, _FW("decode failed, success %d, new size %d, size %d, socketId %d, buffer 0x%p"), 
				success, newSize, actualSize + BUFFER_OVERHEAD_SIZE, pPeer->GetSocketID(), pBuffer);
			SafeDeallocate(pNewData);
			return BOOLEAN_FALSE;
		}

		// replace data of buffer
		pBuffer->SetData(pNewData, actualSize + BUFFER_OVERHEAD_SIZE, newSize);

		return BOOLEAN_TRUE;
	}

	INLINE VOID MiddleFilter::UpdateCRC(Buffer* pBuffer)
	{
		Byte* pData = pBuffer->GetData();
		UInt32 ignore = (UInt32)(BUFFER_HEADER_SIZE + sizeof(UInt32));
		UInt32& uCrc =  *((UInt32*)(pData + BUFFER_HEADER_SIZE));
		uCrc = Reuben::Math::CRC32(pData + ignore, 
			(UInt32)pBuffer->GetSize() - ignore);
	}

	INLINE CONST Boolean MiddleFilter::IsValidCRC(Buffer* pBuffer)
	{
		Byte* pData = pBuffer->GetData();
		UInt32 ignore = (UInt32)(BUFFER_HEADER_SIZE + sizeof(UInt32));
		UInt32& uCrc1 = *((UInt32*)(pData + BUFFER_HEADER_SIZE));
		UInt32 uCrc2 = Reuben::Math::CRC32(pData + ignore, 
			(UInt32)pBuffer->GetSize() - ignore);
		return (uCrc1 == uCrc2);
	}

};}; // Reuben::Network

#endif // _REUBEN_NETWORK_MIDDLEFILTER_H_