#pragma once
#ifndef _REUBEN_SYSTEM_UUID_H_
#define _REUBEN_SYSTEM_UUID_H_

#include <rpcdce.h>
#pragma comment(lib, "rpcrt4")

namespace Reuben { namespace System {

	class Uuid : public UUID
	{
	public:
		INLINE Uuid();

		INLINE Boolean Create(VOID);
		INLINE String ToString(VOID) const;
		INLINE VOID ToNull(VOID);
		INLINE Boolean IsNull(VOID) const;
		INLINE Boolean FromString(PCSTRING pUuid);
		INLINE UInt16 ToHash(VOID) const;

	};

	INLINE Uuid::Uuid()
	{
		::UuidCreateNil((UUID*)this);
	}

	INLINE Boolean Uuid::Create(VOID)
	{
		return (::UuidCreate((UUID*)this) ==  RPC_S_OK);
	}

	INLINE String Uuid::ToString(VOID) const
	{
		PSTRING pUuid = NULL;
#if defined(COMPILER_UNICODE)
		UuidToStringW((UUID*)this, (unsigned short **)&pUuid);
#else // COMPILER_UNICODE
		UuidToStringA((UUID*)this, (unsigned char **)&pUuid);
#endif // COMPILER_UNICODE
		String strUuid(pUuid);
#if defined(COMPILER_UNICODE)
		RpcStringFreeW((unsigned short **)&pUuid);
#else // COMPILER_UNICODE
		RpcStringFreeA((unsigned char **)&pUuid);
#endif // COMPILER_UNICODE
		return (strUuid);
	}

	INLINE VOID Uuid::ToNull(VOID)
	{
		::UuidCreateNil((UUID*)this);
	}

	INLINE Boolean Uuid::IsNull(VOID) const
	{
		RPC_STATUS status;
		return (Boolean)(::UuidIsNil((UUID*)this, &status) == TRUE);
	}

	INLINE Boolean Uuid::FromString(PCSTRING pUuid)
	{
#if defined(COMPILER_UNICODE)
		return (::UuidFromStringW((unsigned short *)pUuid, (UUID*)this) == RPC_S_OK);
#else // COMPILER_UNICODE
		return (::UuidFromStringA((unsigned char *)pUuid, (UUID*)this) == RPC_S_OK);
#endif // COMPILER_UNICODE
	}

	INLINE UInt16 Uuid::ToHash(VOID) const
	{
		RPC_STATUS status;
		return ((UInt16)::UuidHash((UUID*)this, &status));
	}

	typedef	Uuid		Identifier;
	typedef Identifier	ID;

}; };  // Reuben::System

INLINE bool operator ! (const Reuben::System::Uuid & uuid)
{
	return ((bool)uuid.IsNull());
}

INLINE bool operator == (const Reuben::System::Uuid & uuid1, const Reuben::System::Uuid & uuid2)
{
	RPC_STATUS status;
	return (::UuidEqual((UUID*)&uuid1, (UUID*)&uuid2, &status) == TRUE);
}

INLINE bool operator != (const Reuben::System::Uuid & uuid1, const Reuben::System::Uuid & uuid2)
{
	RPC_STATUS status;
	return (::UuidEqual((UUID*)&uuid1, (UUID*)&uuid2, &status) == FALSE);
}

INLINE bool operator > (const Reuben::System::Uuid & uuid1, const Reuben::System::Uuid & uuid2)
{
	RPC_STATUS status;
	return (::UuidCompare((UUID*)&uuid1, (UUID*)&uuid2, &status) > 0);
}

INLINE bool operator < (const Reuben::System::Uuid & uuid1, const Reuben::System::Uuid & uuid2)
{
	RPC_STATUS status;
	return (::UuidCompare((UUID*)&uuid1, (UUID*)&uuid2, &status) < 0);
}

INLINE bool operator >= (const Reuben::System::Uuid & uuid1, const Reuben::System::Uuid & uuid2)
{
	RPC_STATUS status;
	return (::UuidCompare((UUID*)&uuid1, (UUID*)&uuid2, &status) >= 0);
}

INLINE bool operator <= (const Reuben::System::Uuid & uuid1, const Reuben::System::Uuid & uuid2)
{
	RPC_STATUS status;
	return (::UuidCompare((UUID*)&uuid1, (UUID*)&uuid2, &status) <= 0);
}

#endif // _REUBEN_SYSTEM_UUID_H_
