#pragma once
#ifndef _REUBEN_MATH_SHA_H_
#define _REUBEN_MATH_SHA_H_

#define SHA224_DIGEST_SIZE ( 224 / 8)
#define SHA256_DIGEST_SIZE ( 256 / 8)
#define SHA384_DIGEST_SIZE ( 384 / 8)
#define SHA512_DIGEST_SIZE ( 512 / 8)

#define SHA256_BLOCK_SIZE  ( 512 / 8)
#define SHA512_BLOCK_SIZE  (1024 / 8)
#define SHA384_BLOCK_SIZE  SHA512_BLOCK_SIZE
#define SHA224_BLOCK_SIZE  SHA256_BLOCK_SIZE

namespace Reuben { namespace Math {

	VOID SHA224(Byte* pDigest, CONST Byte* pMessage, CONST Size size);
	VOID SHA256(Byte* pDigest, CONST Byte* pMessage, CONST Size size);
	VOID SHA384(Byte* pDigest, CONST Byte* pMessage, CONST Size size);
	VOID SHA512(Byte* pDigest, CONST Byte* pMessage, CONST Size size);

};}; // Reuben::Math

#endif // _REUBEN_MATH_SHA_H_
