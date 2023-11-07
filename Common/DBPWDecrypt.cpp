#include "Common.h"
//#include "db_decrypt.h"
#include <Reuben/Math/Hex.h>
#include <Reuben/Math/AES.h>

// : turn on/off db password encryption
//#define ENABLE_DB_PASSWD_ENCRYPT

// : Add the following to remove the dependency on db_decrypt.lib
#ifdef ENABLE_DB_PASSWD_ENCRYPT
INLINE void db_decrypt(Byte* dest, const Byte* source, const size_t size)
{
	::memcpy(dest, source, size);
	UInt32 secret = 0; /* ??? */
	Reuben::Math::AES::Decrypt(dest, size, secret);
}
#endif

String DBPWDecrypt(CONST String& strPassword)
{
#ifdef ENABLE_DB_PASSWD_ENCRYPT
	size_t size = 0;
	Reuben::Math::HexToBytes(NULL, size, strPassword.c_str(), (Size)strPassword.GetLength());
	Byte* pw_bytes = SafeAllocate(Byte, size);
	Reuben::Math::HexToBytes(pw_bytes, size, strPassword.c_str(), (Size)strPassword.GetLength());
	String strpw;
	db_decrypt((Byte*)strpw.GetBuffer((int)(size / sizeof(TCHAR) + 1)), pw_bytes, size);
	SafeDeallocate(pw_bytes);
	strpw.ReleaseBuffer();
	return (strpw);
#else
	// : No encryption, just return the password
	return strPassword;
#endif  // ENABLE_DB_PASSWD_ENCRYPT
}
