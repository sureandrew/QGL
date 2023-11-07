#pragma once
#ifndef _REUBEN_SYSTEM_ARRAY_H_
#define _REUBEN_SYSTEM_ARRAY_H_


namespace Reuben { namespace System {
	
	class ArrayException : public Exception
	{
	public:

		ArrayException(ErrorCode ec, PCSTRING pSourceLine)
			: Exception(ec, pSourceLine)
		{}

		ArrayException(ErrorCode ec, PCSTRING pSourceLine, PCSTRING pMessage)
			: Exception(ec, pSourceLine, pMessage)
		{}
	};


	//-- please make sure your _TYPE_ is a primitive or a pure structure

	template <typename _TYPE_>
	class Array
	{
	private:

		Size m_size;
		Size m_capacity;
		_TYPE_* m_pData;

	public:

		// -- Constructor

		INLINE Array(VOID)
			: m_size(C_SIZE(0))
			, m_capacity(C_SIZE(0))
			, m_pData(NULL)
		{}

		INLINE Array(CONST Size capacity)
			: m_size(capacity)
			, m_capacity(capacity)
			, m_pData(NULL)
		{
			if (capacity)
				m_pData = SafeAllocate(_TYPE_, capacity);
		}

		INLINE Array(CONST _TYPE_* pData, CONST Size capacity)
			: m_size(capacity)
			, m_capacity(capacity)
			, m_pData(NULL)
		{
			if (pData && capacity)
			{
				m_pData = SafeAllocate(_TYPE_, capacity);
				::memcpy(m_pData, pData, capacity);
			}
		}

		INLINE Array(CONST Array& arr)
			: m_size(arr.m_size)
			, m_capacity(arr.m_capacity)
			, m_pData(NULL)
		{
			if (arr.m_pData && arr.m_capacity)
			{
				m_pData = SafeAllocate(_TYPE_, arr.m_capacity);
				::memcpy(m_pData, arr.m_pData, arr.m_capacity);
			}
		}

		// -- Destructor

		INLINE ~Array(VOID)
		{
			SafeDeallocate(m_pData);
		}

		// -- Operator

		INLINE VOID operator = (CONST Array& arr)
		{
			Clone(arr);
		}

		INLINE CONST _TYPE_& operator [] (CONST Size idx) CONST
		{
			if (idx >= m_size)
			{
				DEBUG_ERRORDTL_3(REUBEN_LOGGER, _FE("read overflow, size %d, capacity %d, index %d"), m_size, m_capacity, idx);
				throw ArrayException(EC_ARRAY_READ_OVERFLOW, _SOURCE_LINE_);
			}
			return (m_pData[idx]);	
		}

		INLINE _TYPE_& operator [] (CONST Size idx)
		{
			if (idx >= m_size)
			{
				DEBUG_ERRORDTL_3(REUBEN_LOGGER, _FE("read overflow, size %d, capacity %d, index %d"), m_size, m_capacity, idx);
				throw ArrayException(EC_ARRAY_READ_OVERFLOW, _SOURCE_LINE_);
			}	
			return (m_pData[idx]);
		}

		INLINE CONST _TYPE_* operator & (VOID) CONST
		{
			return (m_pData);
		}

		INLINE _TYPE_* operator & (VOID)
		{
			return (m_pData);
		}

		INLINE bool operator == (CONST Array& arr) CONST
		{
			if (m_size != arr.m_size)
				return (false);
			for (Size i = 0; i < m_size; ++i)
			{
				if (m_pData[i] != arr.m_pData[i])
					return (false);
			}
			return (true);
		}

		INLINE bool operator != (CONST Array& arr) CONST
		{
			if (m_size != arr.m_size)
				return (true);
			for (Size i = 0; i < m_size; ++i)
			{
				if (m_pData[i] != arr.m_pData[i])
					return (true);
			}
			return (false);
		}

		// -- Setter

		INLINE VOID Set(CONST Size idx, CONST _TYPE_ value)
		{
			if (idx >= m_size)
			{
				DEBUG_ERRORDTL_3(REUBEN_LOGGER, _FE("read overflow, size %d, capacity %d, index %d"), m_size, m_capacity, idx);
				throw ArrayException(EC_ARRAY_READ_OVERFLOW, _SOURCE_LINE_);
			}
			m_pData[idx] = value;
		}

		INLINE VOID SetSize(CONST Size size)
		{
			if (size == m_size)
				return;
			if (size == C_SIZE(0))
			{
				m_size = C_SIZE(0);
				m_capacity = C_SIZE(0);
				SafeDeallocate(m_pData);
			}
			else if (size <= m_capacity)
			{
				m_size = size;
			}
			else
			{
				_TYPE_* pNew = SafeAllocate(_TYPE_, size); 
				if (m_pData)
				{
					::memcpy(pNew, m_pData, m_size * sizeof(_TYPE_));
					SafeDeallocate(m_pData);
				}
				m_pData = pNew;
				m_size = size;
				m_capacity = size;
			}
		}

		INLINE VOID SetCapacity(CONST Size capacity)
		{
			if (capacity == m_capacity)
				return;
			_TYPE_* pNew = SafeAllocate(_TYPE_, capacity);
			Size new_size = (capacity < m_size) ? capacity : m_size;
			if (new_size && m_pData)
			{
				::memcpy(pNew, m_pData, new_size * sizeof(_TYPE_));
				SafeDeallocate(m_pData);
			}
			m_pData = pNew;
			m_size = new_size;
			m_capacity = capacity;
		}

		// -- Getter

		INLINE CONST _TYPE_& Get(CONST Size idx) CONST
		{
			if (idx >= m_size)
			{
				DEBUG_ERRORDTL_3(REUBEN_LOGGER, _FE("read overflow, size %d, capacity %d, index %d"), m_size, m_capacity, idx);
				throw ArrayException(EC_ARRAY_READ_OVERFLOW, _SOURCE_LINE_);
			}
			return (m_pData[idx]);
		}

		INLINE _TYPE_& Get(CONST Size idx)
		{
			if (idx >= m_size)
			{
				DEBUG_ERRORDTL_3(REUBEN_LOGGER, _FE("read overflow, size %d, capacity %d, index %d"), m_size, m_capacity, idx);
				throw ArrayException(EC_ARRAY_READ_OVERFLOW, _SOURCE_LINE_);
			}
			return (m_pData[idx]);
		}

		INLINE CONST Size GetSize(VOID) CONST
		{
			return (m_size);
		}

		INLINE CONST Size GetCapacity(VOID) CONST
		{
			return (m_capacity);
		}

		INLINE CONST _TYPE_* GetData(VOID) CONST 
		{
			return (m_pData);
		}

		INLINE _TYPE_* GetData(VOID)
		{
			return (m_pData);
		}

		// -- Function
	
		INLINE VOID Clear(VOID)
		{
			m_size = C_SIZE(0);
			m_capacity = C_SIZE(0);
			SafeDeallocate(m_pData);
		}

		INLINE VOID Clone(CONST _TYPE_* pData, CONST Size size)
		{
			if (pData && size)
			{
				if (size != m_size)
				{
					if (size > m_capacity)
					{
						SafeDeallocate(m_pData);
						m_pData = SafeAllocate(_TYPE_, size);
						m_capacity = size;
					}
					m_size = size;
				}
				::memcpy(m_pData, pData, size);
			}
		}

		INLINE VOID Clone(CONST Array& arr)
		{
			Clone(arr.m_pData, arr.m_size);
		}

		INLINE VOID SwapFrom(Array& arr)
		{
			SafeDeallocate(m_pData);
			m_size = arr.m_size;
			m_capacity = arr.m_capacity;
			m_pData = arr.m_pData;
			arr.m_size = C_SIZE(0);
			arr.m_capacity = C_SIZE(0);
			arr.m_pData = NULL;
		}

		INLINE VOID SwapTo(Array& arr)
		{
			SafeDeallocate(arr.m_pData);
			arr.m_size = m_size;
			arr.m_capacity = m_capacity;
			arr.m_pData = m_pData;
			m_size = C_SIZE(0);
			m_capacity = C_SIZE(0);
			m_pData = NULL;
		}

		INLINE VOID Zero(VOID)
		{
			if (m_pData)
				::memset(m_pData, 0, sizeof(_TYPE_) * m_capacity);
		}
	};

	typedef Array<Byte>			ByteArray;
	typedef Array<Int8>			Int8Array;
	typedef Array<Int16>		Int16Array;
	typedef Array<Int32>		Int32Array;
	typedef Array<Int>			IntArray;
	typedef Array<UInt8>		UInt8Array;
	typedef Array<UInt16>		UInt16Array;
	typedef Array<UInt32>		UInt32Array;
	typedef Array<UInt>			UIntArray;
	typedef Array<Real32>		Real32Array;
	typedef Array<Real64>		Real64Array;
	typedef Array<Real80>		Real80Array;
	typedef Array<Real>			RealArray;
	typedef Array<Bit8>			Bit8Array;
	typedef Array<Bit16>		Bit16Array;
	typedef Array<Bit32>		Bit32Array;
	typedef Array<Bit64>		Bit64Array;
	typedef Array<Boolean8>		Boolean8Array;
	typedef Array<Boolean32>	Boolean32Array;
	typedef Array<Boolean>		BooleanArray;
	typedef Array<Index>		IndexArray;
	typedef Array<Size>			SizeArray;
	typedef Array<Count>		CountArray;
	typedef Array<Color8>		Color8Array;
	typedef Array<Color16>		Color16Array;
	typedef Array<Color32>		Color32Array;
	typedef Array<CharA>		CharAArray;
	typedef Array<CharW>		CharWArray;
	typedef Array<Char>			CharArray;

}; }; // Reuben::System

#endif // _REUBEN_SYSTEM_ARRAY_H_