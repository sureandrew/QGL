#pragma once
#ifndef _REUBEN_NETWORK_BUFFER_H_
#define _REUBEN_NETWORK_BUFFER_H_

//-- Local
#include "Network.h"
#include "../Utility/Serializer.h"
#include "../Utility/RefCounter.h"
#include "../System/Array.h"
//-- Library
#include <vector>
#include <map>

#ifdef COMPILER_DEBUG
	#define REUBEN_NETWORK_DEFAULT_BUFFER_CAPACITY			C_SIZE(192)
#else
	#define REUBEN_NETWORK_DEFAULT_BUFFER_CAPACITY			C_SIZE(256)
#endif
#define REUBEN_NETWORK_BUFFER_MAX_CAPACITY				C_SIZE(65408)
#define REUBEN_NETWORK_BUFFER_HEADER_SIZE				sizeof(Reuben::Network::BufferHeader)

namespace Reuben { namespace Network {

	struct BufferHeader
	{
		UInt16 uSize;
#ifdef REUBEN_CONFIG_NETWORK_PACKET_COUNTER_ENABLE
		Count uCounter;
#endif // REUBEN_CONFIG_NETWORK_PACKET_COUNTER_ENABLE
	};

	class Buffer
		: public Reuben::Utility::RefCounter<Buffer, SafeDeleteObjectProc<Buffer> >
	{
		template<typename _TYPE_>
		struct Serializer : public Reuben::Utility::Serializer<Buffer, _TYPE_>{};

	protected:

		BufferHeader* m_pHeader;
		Byte* m_pData;
		mutable Size m_nCursor;
		Size m_nSize;
		Size m_nCapacity;
		Boolean m_bModified;
		Boolean m_bDelete;

	public:

		Buffer(CONST Size capacity = REUBEN_NETWORK_DEFAULT_BUFFER_CAPACITY);
		virtual ~Buffer(VOID);

		INLINE CONST Size GetCursor(VOID) CONST;
		INLINE VOID ResetCursor() CONST;
		INLINE VOID SetCursor(CONST Size size) CONST;
		INLINE VOID ShiftCursor(CONST Size size) CONST;
		INLINE CONST Size GetSize(VOID) CONST;
		INLINE VOID SetSize(CONST Size size);
		INLINE CONST Size GetCapacity(VOID) CONST;
		INLINE VOID PackHeader(VOID);
		INLINE CONST Boolean IsModified(VOID) CONST;
		INLINE VOID SetModified(CONST Boolean bModified);
		INLINE CONST BufferHeader& GetHeader(VOID) CONST;
		INLINE Byte* GetData(VOID);
		INLINE VOID SetData(Byte *pData, Size size, Size capacity, Boolean bDelete = BOOLEAN_TRUE);

		template<typename _TYPE_>
			INLINE VOID Read(_TYPE_& data) CONST;
		template<typename _TYPE_>
			INLINE VOID Write(CONST _TYPE_& data);

		template<typename _TYPE_>
			INLINE VOID ReadRange(CONST _TYPE_& data, CONST _TYPE_& rstart, CONST _TYPE_& rend) CONST;
		template<typename _TYPE_>
			INLINE VOID WriteRange(_TYPE_ data, CONST _TYPE_& rstart, CONST _TYPE_& rend);

		template<typename _TYPE_>
			INLINE VOID ReadArray(_TYPE_* pArray, CONST Count cItems) CONST;
		template<typename _TYPE_>
			INLINE VOID WriteArray(CONST _TYPE_* pArray, CONST Count cItems);

		CONST Boolean Expand(CONST Size capacity);
		VOID Read(VOID* pData, CONST Size size) CONST;
		VOID Write(CONST VOID* pData, CONST Size size);
		VOID DumpString(String& out, CONST Size offset, CONST Size size) CONST;

	};

	INLINE CONST Size Buffer::GetCursor(VOID) CONST
	{
		return (m_nCursor);
	}

	INLINE VOID Buffer::ResetCursor() CONST
	{
		TRACE_CHECK_1(REUBEN_NETWORK_BUFFER_HEADER_SIZE <= m_nSize, _T("m_nSize %d"), m_nSize);
		m_nCursor = REUBEN_NETWORK_BUFFER_HEADER_SIZE;
	}

	INLINE VOID Buffer::SetCursor(CONST Size size) CONST
	{
		TRACE_CHECK_2(REUBEN_NETWORK_BUFFER_HEADER_SIZE <= size && size <= m_nSize, 
			_T("m_nSize %d, size %d"), m_nSize, size);
		m_nCursor = size;
	}

	INLINE VOID Buffer::ShiftCursor(CONST Size size) CONST
	{
		TRACE_CHECK_2(size > 0 && (m_nCursor + size) <= m_nSize, _T("m_nSize %d, size %d"), m_nSize, size);
		m_nCursor += size;
	}

	INLINE CONST Size Buffer::GetSize(VOID) CONST
	{
		return (m_nSize);
	}

	INLINE VOID Buffer::SetSize(CONST Size size)
	{
		TRACE_CHECK(size >= REUBEN_NETWORK_BUFFER_HEADER_SIZE && size <= m_nCapacity);
		m_nSize = size;
	}

	INLINE CONST Size Buffer::GetCapacity(VOID) CONST
	{
		return (m_nCapacity);
	}

	INLINE VOID Buffer::PackHeader(VOID)
	{
		if (m_bModified)
		{
			m_pHeader->uSize = (UInt16)m_nSize;
		}
	}

	INLINE CONST Boolean Buffer::IsModified(VOID) CONST
	{
		return (m_bModified);
	}

	INLINE VOID Buffer::SetModified(CONST Boolean bModified)
	{
		m_bModified = bModified;
	}

	INLINE CONST BufferHeader& Buffer::GetHeader(VOID) CONST
	{
		return (*m_pHeader);
	}

	INLINE Byte* Buffer::GetData(VOID)
	{
		if (m_pData == NULL)
		{
			m_pData = SafeAllocate(Byte, m_nCapacity);
			m_pHeader = (BufferHeader*)m_pData;
			m_pHeader->uSize = 0;
		}
		return (m_pData);
	}

	INLINE VOID Buffer::SetData(
		Byte *pData,
		Size size,
		Size capacity,
		Boolean bDelete // BOOLEAN_TRUE
		)
	{
		TRACE_CHECK(size >= REUBEN_NETWORK_BUFFER_HEADER_SIZE);
		if (m_bDelete && m_pData)
			SafeDeallocate(m_pData);
		m_pData = pData;
		m_pHeader = (BufferHeader*)m_pData;
		m_nSize = size;
		m_nCapacity = capacity;
		m_bModified = BOOLEAN_TRUE;
		m_bDelete = bDelete;
	}

	template<typename _TYPE_>
		INLINE VOID Buffer::Read(_TYPE_& data) CONST
	{
		Serializer< _TYPE_>::Read(*this, data);
	}

	template<typename _TYPE_>
		INLINE VOID Buffer::Write(CONST _TYPE_& data)
	{
		Serializer<_TYPE_>::Write(*this, data);
	}

	template<typename _TYPE_>
		INLINE VOID Buffer::ReadRange(CONST _TYPE_& data, CONST _TYPE_& rstart, CONST _TYPE_& rend) CONST
	{
		Serializer<_TYPE_>::Read(*this, data);
		if (data < rstart)
			data = rstart;
		else if (data > rend)
			data = rend;
	}

	template<typename _TYPE_>
		INLINE VOID Buffer::WriteRange(_TYPE_ data, CONST _TYPE_& rstart, CONST _TYPE_& rend)
	{
		if (data < rstart)
			data = rstart;
		else if (data > rend)
			data = rend;
		Serializer<_TYPE_>::Write(*this, data);
	}

	template<typename _TYPE_>
		INLINE VOID Buffer::ReadArray(_TYPE_* pArray, CONST Count cItems) CONST
	{
		Serializer<_TYPE_>::ReadArray(*this, pArray, cItems);
	}

	template<typename _TYPE_>
		INLINE VOID Buffer::WriteArray(CONST _TYPE_* pArray, CONST Count cItems)
	{
		Serializer<_TYPE_>::WriteArray(*this, pArray, cItems);
	}

	//-- Specific String

	template<>
	struct Buffer::Serializer<Reuben::System::String>
	{
		static INLINE VOID Read(CONST Buffer& sin, String& data)
		{
			UInt16 uSize;
			sin.Read<UInt16>(uSize);
			if (uSize)
			{
				PSTRING pBuffer = data.GetBuffer(uSize + 1);
				sin.ReadArray<Char>(pBuffer, uSize);
				*(pBuffer + uSize) = _T('\0');
				data.ReleaseBuffer();
			}
			else
				data.Empty();
		}

		static INLINE VOID Write(Buffer& sout, CONST String& data)
		{
			UInt16 uSize = data.c_str() ? (UInt16)data.size() : C_UINT16(0);
			sout.Write<UInt16>(uSize);
			if (uSize)
				sout.WriteArray<Char>(data.c_str(), uSize);
		}
	};

	//-- Specific Array

	template<typename _TYPE_>
	struct Buffer::Serializer<Array<_TYPE_> >
	{
		static INLINE VOID Read(CONST Buffer& sin, Array<_TYPE_>& data)
		{
			Size size;
			sin.Read<Size>(size);
			if (size)
			{
				data.SetSize(size);
				sin.ReadArray<_TYPE_>(&data, (CONST Count)size);
			}
			else
				data.Clear();
		}

		static INLINE VOID Write(Buffer& sout, CONST Array<_TYPE_>& data)
		{
			Size size = data.GetSize();
			sout.Write<Size>(size);
			if (size)
				sout.WriteArray<_TYPE_>(&data, (CONST Count)size);
		}
	};

	//-- Specific Buffer

	template<>
	struct Buffer::Serializer<Buffer>
	{
		static INLINE VOID Read(CONST Buffer& sin, Buffer& data)
		{
			Size size;
			sin.Read<Size>(size);
			if (size == 0)
				return;
			if ((sin.m_nCursor + size) > sin.m_nSize)
			{
				DEBUG_ERRORDTL_3(REUBEN_LOGGER, _FE("Read Overflow, Buffer Size %d, Cursor %d, Read Size %d"), sin.m_nSize, sin.m_nCursor, size);
				throw NetworkException(EC_NET_BUFFER_READ_OVERFLOW, _SOURCE_LINE_);
			}
			
			data.~Buffer();
			data.m_pData = sin.m_pData + sin.m_nCursor;
			data.m_pHeader = (BufferHeader*)(data.m_pData);
			data.m_nCursor = REUBEN_NETWORK_BUFFER_HEADER_SIZE;
			data.m_nSize = size;
			data.m_nCapacity = size;
			data.m_bDelete = BOOLEAN_FALSE;
			sin.m_nCursor += size;
		}

		static INLINE VOID Write(Buffer& sout, CONST Buffer& data)
		{
			if (data.m_pData == NULL)
				sout.Write<Size>(0);
			else
			{
				sout.Write<Size>(data.m_nSize);
				sout.Write(data.m_pData, data.m_nSize);
			}
		}
	};

	//-- Specific STL vector

	template<typename _T, class _A>
	struct Buffer::Serializer<StlVector<_T, _A> >
	{
		static INLINE VOID Read(CONST Buffer& sin, StlVector<_T, _A>& data)
		{
			Size size;
			sin.Read<Size>(size);
			data.clear();
			for (Size i = 0; i < size; ++i)
			{
				_T value;
				sin.Read<_T>(value);
				data.push_back(value);
			}
		}

		static INLINE VOID Write(Buffer& sout, CONST StlVector<_T, _A>& data)
		{
			sout.Write<Size>((Size)data.size());
			for (StlVector<_T, _A>::const_iterator it = data.begin(); it != data.end(); ++it)
				sout.Write<_T>(*it);
		}
	};

	//-- Specific STL map
	template<typename _K, typename _T, class _L, class _A>
	struct Buffer::Serializer<StlMap<_K, _T, _L, _A> >
	{
		static INLINE VOID Read(CONST Buffer& sin, StlMap<_K, _T, _L, _A>& data)
		{
			Size size;
			sin.Read<Size>(size);
			data.clear();
			for (Size i = 0; i < size; ++i)
			{
				_K key;
				_T value;
				sin.Read<_K>(key);
				sin.Read<_T>(value);
				data.insert(std::make_pair(key, value));
			}
		}

		static INLINE VOID Write(Buffer& sout, CONST StlMap<_K, _T, _L, _A>& data)
		{
			sout.Write<Size>((Size)data.size());
			for (StlMap<_K, _T, _L, _A>::const_iterator it = data.begin(); it != data.end(); ++it)
			{
				sout.Write<_K>(it->first);
				sout.Write<_T>(it->second);
			}
		}
	};

};}; // Reuben::Network


#endif // _REUBEN_NETWORK_BUFFER_H_
