#pragma once
#ifndef _REUBEN_UTILITY_SERIALIZER_H_
#define _REUBEN_UTILITY_SERIALIZER_H_

namespace Reuben { namespace Utility {

	template <typename _STREAM_, typename _TYPE_>
	struct Serializer
	{
		static INLINE VOID Read(CONST _STREAM_& inStream, CONST _TYPE_& data)
		{
			inStream.Read((VOID*)&data, sizeof(_TYPE_));
		}

		static INLINE VOID Write(_STREAM_& outStream, CONST _TYPE_& data)
		{
			outStream.Write((VOID*)&data, sizeof(_TYPE_));
		}

		static INLINE VOID ReadArray(CONST _STREAM_& inStream, _TYPE_* pArray, CONST Count cElements)
		{
			inStream.Read(pArray, sizeof(_TYPE_) * cElements);
		}

		static INLINE VOID WriteArray(_STREAM_& outStream, CONST _TYPE_* pArray, CONST Count cElements)
		{
			outStream.Write(pArray, sizeof(_TYPE_)* cElements);
		}
	};

	struct IStream
	{
		template<typename _TYPE_>
		struct Serializer : public Reuben::Utility::Serializer<IStream, _TYPE_>{};

		virtual VOID Read(VOID* pData, CONST Size size) CONST = 0;
		virtual VOID Write(CONST VOID* pData, CONST Size size) = 0;

		template<typename _TYPE_>
			INLINE VOID ReadData(_TYPE_& data) CONST
		{
			Serializer<_TYPE_>::Read(*this, data);
		}

		template<typename _TYPE_>
			INLINE VOID WriteData(CONST _TYPE_& data)
		{
			Serializer<_TYPE_>::Write(*this, data);
		}

		template<typename _TYPE_>
			INLINE VOID ReadArray(_TYPE_* pArray, CONST Count cItems) CONST
		{
			Serializer<_TYPE_>::ReadArray(*this, pArray, cItems);
		}

		template<typename _TYPE_>
			INLINE VOID WriteArray(CONST _TYPE_* pArray, CONST Count cItems)
		{
			Serializer<_TYPE_>::WriteArray(*this, pArray, cItems);
		}
	};

}; }; // Reuben::Utility

#endif // _REUBEN_UTILITY_SERIALIZER_H_
