#pragma once
#ifndef _REUBEN_MATH_AES_H_
#define _REUBEN_MATH_AES_H_

#define AES_CODE_MAX		256		// The maximum number of code
#define AES_ROUNDKEY_LEN	240		// The maximum round key size
#define AES_COLUMNS			4 		// The number of columns comprising a state
#define AES_BLOCK_SIZE		16 		// The size of encryption block
#define AES_KEY_SIZE		16 		// The size of key for initial round keys
#define AES_ROUND_COUNT		3		// Number of rounds for operations

namespace Reuben { namespace Math {

	// xtime is a macro that finds the product of {02} and the argument to xtime modulo {1b}  
	//#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))
#define xtime1(x)	m_xtime[x]
#define xtime2(x)	m_xtime[m_xtime[x]]
#define xtime3(x)	m_xtime[m_xtime[m_xtime[x]]]
#define xtime4(x)	m_xtime[m_xtime[m_xtime[m_xtime[x]]]]

	// Multiplty is a macro used to multiply numbers in the field GF(2^8)
	//#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))

#define Multiply_0e(x)	xtime1(x) ^ xtime2(x) ^ xtime3(x)
#define Multiply_0b(x)	x ^ xtime1(x) ^ xtime3(x)
#define Multiply_0d(x)	x ^ xtime2(x) ^ xtime3(x)
#define Multiply_09(x)	x ^ xtime3(x)

	class AES
	{
	public:
		static void AddRoundKey(Byte *state, UInt8 len, UInt8 round);
		static void SubBytes(Byte *state, UInt8 len);
		static void ShiftRows(Byte *state);
		static void MixColumns(Byte *state);
		static void InvSubBytes(Byte *state, UInt8 len);
		static void InvShiftRows(Byte *state);
		static void InvMixColumns(Byte *state);
		static void Encrypt(Byte* data, UInt16 size, UInt32 secret);
		static void Decrypt(Byte* data, UInt16 size, UInt32 secret);
		template <unsigned int _V> static void Encrypt(Byte* data, UInt16 size);
		template <unsigned int _V> static void Decrypt(Byte* data, UInt16 size);
		
		static int KeyExpansion(Byte* keyData, UInt16 keySize);
		static Boolean KeyChanged() { return m_changed; }
		static VOID ResetKey(VOID) { m_changed = BOOLEAN_FALSE; }

	private:
		static Byte	m_roundKey[AES_ROUNDKEY_LEN];	// round key
		static Byte	m_sbox[AES_CODE_MAX];		// S-box
		static Byte	m_rsbox[AES_CODE_MAX];		// reversed S-box
		static Byte	m_rcon[AES_CODE_MAX];		// round constant word array
		static Byte	m_xtime[AES_CODE_MAX];		// xtime lookup table
		static Byte	m_skey[AES_KEY_SIZE];		// secret key
		static Byte	m_x09[AES_CODE_MAX];		// xtime 09 lookup table
		static Byte	m_x0b[AES_CODE_MAX];		// xtime 0b lookup table
		static Byte	m_x0d[AES_CODE_MAX];		// xtime 0d lookup table
		static Byte	m_x0e[AES_CODE_MAX];		// xtime 0e lookup table
		static Boolean m_changed;				// has secret key changed
	};

#ifdef REUBEN_PUBLISH

	// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
	INLINE int AES::KeyExpansion(Byte* keyData, UInt16 keySize)
	{
		UInt8 i, j;
		Byte temp[4], k;

		if (keyData == NULL) {
			keyData = m_sbox;
			keySize = AES_CODE_MAX;
			// initial other xtime tables
			for (int x = 0; x < AES_CODE_MAX; ++x) {
				m_x09[x] = Multiply_09(x);
				m_x0b[x] = Multiply_0b(x);
				m_x0d[x] = Multiply_0d(x);
				m_x0e[x] = Multiply_0e(x);
			}
		} else {
			m_changed = BOOLEAN_TRUE;
		}

		// calculate real secret key from key data
		int index = keyData[0];
		for (int x = 0; x < AES_KEY_SIZE; x++)
		{
			m_roundKey[x] = keyData[index] ^ index;
			index = (m_roundKey[x] + x) % keySize;
		}
		// The first round key is the key itself.
		//memcpy(m_roundKey, m_skey, sizeof(Byte) * AES_KEY_SIZE);

		// All other round keys are found from the previous round keys.
		i = AES_KEY_SIZE;
		while (i < (AES_COLUMNS * (AES_ROUND_COUNT + 1)))
		{
			for (j = 0; j < 4; j++)
			{
				temp[j] = m_roundKey[(i - 1) * 4 + j];
			}
			if (i % AES_KEY_SIZE == 0)
			{
				// This function rotates the 4 bytes in a word to the left once.
				// [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

				// Function RotWord()
				{
					k = temp[0];
					temp[0] = temp[1];
					temp[1] = temp[2];
					temp[2] = temp[3];
					temp[3] = k;
				}

				// SubWord() is a function that takes a four-byte input word and 
				// applies the S-box to each of the four bytes to produce an output word.

				// Function Subword()
				{
					temp[0] = m_sbox[temp[0]];
					temp[1] = m_sbox[temp[1]];
					temp[2] = m_sbox[temp[2]];
					temp[3] = m_sbox[temp[3]];
				}

				temp[0] =  temp[0] ^ m_rcon[i / AES_KEY_SIZE];
			}
			else if (AES_KEY_SIZE > 6 && i % AES_KEY_SIZE == 4)
			{
				// Function Subword()
				{
					temp[0] = m_sbox[temp[0]];
					temp[1] = m_sbox[temp[1]];
					temp[2] = m_sbox[temp[2]];
					temp[3] = m_sbox[temp[3]];
				}
			}
			m_roundKey[i*4+0] = m_roundKey[(i-AES_KEY_SIZE)*4+0] ^ temp[0];
			m_roundKey[i*4+1] = m_roundKey[(i-AES_KEY_SIZE)*4+1] ^ temp[1];
			m_roundKey[i*4+2] = m_roundKey[(i-AES_KEY_SIZE)*4+2] ^ temp[2];
			m_roundKey[i*4+3] = m_roundKey[(i-AES_KEY_SIZE)*4+3] ^ temp[3];
			i++;
		}

		return 0;
	}

	// This function adds the round key to state.
	// The round key is added to the state by an XOR function.
	INLINE void AES::AddRoundKey(Byte *state, UInt8 len, UInt8 round)
	{
		for (UInt8 i = 0; i < len; i++)
		{
			state[i] ^= m_roundKey[round * AES_COLUMNS * 4 + i];
		}
	}

	// The SubBytes Function Substitutes the values in the
	// state matrix with values in an S-box.
	INLINE void AES::SubBytes(Byte *state, UInt8 len)
	{
		for (UInt8 i = 0; i < len; i++)
		{
			state[i] = m_sbox[state[i]];
		}
	}

	// The ShiftRows() function shifts the rows in the state to the left.
	// Each row is shifted with different offset.
	// Offset = Row number. So the first row is not shifted.
	INLINE void AES::ShiftRows(Byte *state)
	{
		Byte temp;

		// Rotate first row 1 columns to left    
		temp		= state[4];
		state[4]	= state[5];
		state[5]	= state[6];
		state[6]	= state[7];
		state[7]	= temp;

		// Rotate second row 2 columns to left    
		temp		= state[8];
		state[8]	= state[10];
		state[10]	= temp;
		temp		= state[9];
		state[9]	= state[11];
		state[11]	= temp;

		// Rotate third row 3 columns to left
		temp		= state[12];
		state[12]	= state[15];
		state[15]	= state[14];
		state[14]	= state[13];
		state[13]	= temp;
	}

	// MixColumns function mixes the columns of the state matrix
	// The method used may look complicated, but it is easy if you know the underlying theory.
	// Refer the documents specified above.
	INLINE void AES::MixColumns(Byte *state)
	{
		Byte a,b,c,d;
		for (UInt8 i = 0; i < 4; i++)
		{
			a = state[i];
			b = state[4 + i];
			c = state[8 + i];
			d = state[12 + i];
			state[i] =		xtime1(a ^ b) ^ b ^ c ^ d;
			state[4 + i] =	xtime1(b ^ c) ^ a ^ c ^ d;
			state[8 + i] =	xtime1(c ^ d) ^ a ^ b ^ d;
			state[12 + i] =	xtime1(d ^ a) ^ a ^ b ^ c;
		}
	}

	// The SubBytes Function Substitutes the values in the
	// state matrix with values in an S-box.
	INLINE void AES::InvSubBytes(Byte *state, UInt8 len)
	{
		for (UInt8 i = 0; i < len; i++)
		{
			state[i] = m_rsbox[state[i]];
		}
	}

	// The ShiftRows() function shifts the rows in the state to the left.
	// Each row is shifted with different offset.
	// Offset = Row number. So the first row is not shifted.
	INLINE void AES::InvShiftRows(Byte *state)
	{
		Byte temp;

		// Rotate first row 1 columns to right    
		temp		= state[7];
		state[7]	= state[6];
		state[6]	= state[5];
		state[5]	= state[4];
		state[4]	= temp;

		// Rotate second row 2 columns to right    
		temp		= state[8];
		state[8]	= state[10];
		state[10]	= temp;

		temp		= state[9];
		state[9]	= state[11];
		state[11]	= temp;

		// Rotate third row 3 columns to right
		temp		= state[12];
		state[12]	= state[13];
		state[13]	= state[14];
		state[14]	= state[15];
		state[15]	= temp;
	}

	// MixColumns function mixes the columns of the state matrix.
	// The method used to multiply may be difficult to understand for beginners.
	// Please use the references to gain more information.
	INLINE void AES::InvMixColumns(Byte *state)
	{
		Byte a,b,c,d;
		for (UInt8 i = 0; i < 4; i++)
		{    
			a = state[i];
			b = state[4 + i];
			c = state[8 + i];
			d = state[12 + i];
			state[i] = m_x0e[a] ^ m_x0b[b] ^ m_x0d[c] ^ m_x09[d];
			state[4 + i] = m_x09[a] ^ m_x0e[b] ^ m_x0b[c] ^ m_x0d[d];
			state[8 + i] = m_x0d[a] ^ m_x09[b] ^ m_x0e[c] ^ m_x0b[d];
			state[12 + i] = m_x0b[a] ^ m_x0d[b] ^ m_x09[c] ^ m_x0e[d];
		}
	}
#endif // REUBEN_PUBLISH

#define AES_KEY				0xffffffff
#define AES_SUB_BYTES		SubBytes(state, len);
#define AES_ISUB_BYTES		InvSubBytes(state, len);
#define AES_SHIFT_ROWS		if (bComplete) ShiftRows(state);
#define AES_ISHIFT_ROWS		if (bComplete) InvShiftRows(state);
#define AES_MIX_COLUMNS		if (bComplete) MixColumns(state);
#define AES_IMIX_COLUMNS	if (bComplete) InvMixColumns(state);
#define AES_ADD_ROUNDKEY	AddRoundKey(state, len, round++);
#define AES_IADD_ROUNDKEY	AddRoundKey(state, len, round--);
#define AES_ROUND_NUM(x)	(((x >> 3) & 1) + ((x >> 7) & 1) + ((x >> 11) & 1) + ((x >> 15) & 1) + ((x >> 19) & 1) + ((x >> 23) & 1) + ((x >> 27) & 1) + ((x >> 31) & 1))

#define AES_ENCRYPT_HEAD(_v_)												\
	template <>																\
	INLINE void Reuben::Math::AES::Encrypt<_v_>(Byte* data, UInt16 size)	\
	{																		\
		Byte *state;														\
		UInt8 len = AES_BLOCK_SIZE;											\
		bool bComplete = true;												\
		UInt8 round;														\
		for (UInt16 p = 0; p < size; p += AES_BLOCK_SIZE) {					\
			if (p + AES_BLOCK_SIZE > size) {								\
				len = size - p;												\
				bComplete = false;											\
			}																\
			state = data + p;												\
			round = 0;														\
			AddRoundKey(state, len, round++);

#define AES_ENCRYPT_TAIL													\
		}																	\
	}

#define AES_DECRYPT_HEAD(_v_)												\
	template <>																\
	INLINE void Reuben::Math::AES::Decrypt<_v_>(Byte* data, UInt16 size)	\
	{																		\
		Byte *state;														\
		UInt8 len = AES_BLOCK_SIZE;											\
		bool bComplete = true;												\
		UInt8 round;														\
		for (UInt16 p = 0; p < size; p += AES_BLOCK_SIZE) {					\
			if (p + AES_BLOCK_SIZE > size) {								\
				len = size - p;												\
				bComplete = false;											\
			}																\
			state = data + p;												\
			round = AES_ROUND_NUM(_v_);

	#define AES_DECRYPT_TAIL												\
			AddRoundKey(state, len, 0);										\
		}																	\
	}

	// Cipher is the main function that encrypts the PlainText.
	INLINE void AES::Encrypt(Byte* data, UInt16 size, UInt32 secret)
	{
		Byte *state;
		UInt8 len = AES_BLOCK_SIZE;
		bool bComplete = true;
		UInt8 round;

		for (UInt16 p = 0; p < size; p += AES_BLOCK_SIZE) {
			if (p + AES_BLOCK_SIZE > size) {				// incomplete block size
				len = size - p;
				bComplete = false;
			}
			state = data + p;

			// Add the First round key to the state before starting the rounds.
			round = 0;
			AddRoundKey(state, len, round++);

			if ( secret        & 1)		AES_SUB_BYTES
			if ((secret >>  1) & 1)		AES_SHIFT_ROWS
			if ((secret >>  2) & 1)		AES_MIX_COLUMNS
			if ((secret >>  3) & 1)		AES_ADD_ROUNDKEY
			if ((secret >>  4) & 1)		AES_SUB_BYTES
			if ((secret >>  5) & 1)		AES_SHIFT_ROWS
			if ((secret >>  6) & 1)		AES_MIX_COLUMNS
			if ((secret >>  7) & 1)		AES_ADD_ROUNDKEY
			if ((secret >>  8) & 1)		AES_SUB_BYTES
			if ((secret >>  9) & 1)		AES_SHIFT_ROWS
			if ((secret >> 10) & 1)		AES_MIX_COLUMNS
			if ((secret >> 11) & 1)		AES_ADD_ROUNDKEY
			if ((secret >> 12) & 1)		AES_SUB_BYTES
			if ((secret >> 13) & 1)		AES_SHIFT_ROWS
			if ((secret >> 14) & 1)		AES_MIX_COLUMNS
			if ((secret >> 15) & 1)		AES_ADD_ROUNDKEY
			if ((secret >> 16) & 1)		AES_SUB_BYTES
			if ((secret >> 17) & 1)		AES_SHIFT_ROWS
			if ((secret >> 18) & 1)		AES_MIX_COLUMNS
			if ((secret >> 19) & 1)		AES_ADD_ROUNDKEY
			if ((secret >> 20) & 1)		AES_SUB_BYTES
			if ((secret >> 21) & 1)		AES_SHIFT_ROWS
			if ((secret >> 22) & 1)		AES_MIX_COLUMNS
			if ((secret >> 23) & 1)		AES_ADD_ROUNDKEY
			if ((secret >> 24) & 1)		AES_SUB_BYTES
			if ((secret >> 25) & 1)		AES_SHIFT_ROWS
			if ((secret >> 26) & 1)		AES_MIX_COLUMNS
			if ((secret >> 27) & 1)		AES_ADD_ROUNDKEY
			if ((secret >> 28) & 1)		AES_SUB_BYTES
			if ((secret >> 29) & 1)		AES_SHIFT_ROWS
			if ((secret >> 30) & 1)		AES_MIX_COLUMNS
			if ((secret >> 31) & 1)		AES_ADD_ROUNDKEY
		}
	}

	// InvCipher is the main function that decrypts the CipherText.
	INLINE void AES::Decrypt(Byte* data, UInt16 size, UInt32 secret)
	{
		Byte *state;
		UInt8 len = AES_BLOCK_SIZE;
		bool bComplete = true;
		UInt8 round;

		for (UInt16 p = 0; p < size; p += AES_BLOCK_SIZE) {
			if (p + AES_BLOCK_SIZE > size) {				// incomplete block size
				len = size - p;
				bComplete = false;
			}
			state = data + p;
			round = AES_ROUND_NUM(secret);

			if ((secret >> 31) & 1)		AES_IADD_ROUNDKEY
			if ((secret >> 30) & 1)		AES_IMIX_COLUMNS
			if ((secret >> 29) & 1)		AES_ISHIFT_ROWS
			if ((secret >> 28) & 1)		AES_ISUB_BYTES
			if ((secret >> 27) & 1)		AES_IADD_ROUNDKEY
			if ((secret >> 26) & 1)		AES_IMIX_COLUMNS
			if ((secret >> 25) & 1)		AES_ISHIFT_ROWS
			if ((secret >> 24) & 1)		AES_ISUB_BYTES
			if ((secret >> 23) & 1)		AES_IADD_ROUNDKEY
			if ((secret >> 22) & 1)		AES_IMIX_COLUMNS
			if ((secret >> 21) & 1)		AES_ISHIFT_ROWS
			if ((secret >> 20) & 1)		AES_ISUB_BYTES
			if ((secret >> 19) & 1)		AES_IADD_ROUNDKEY
			if ((secret >> 18) & 1)		AES_IMIX_COLUMNS
			if ((secret >> 17) & 1)		AES_ISHIFT_ROWS
			if ((secret >> 16) & 1)		AES_ISUB_BYTES
			if ((secret >> 15) & 1)		AES_IADD_ROUNDKEY
			if ((secret >> 14) & 1)		AES_IMIX_COLUMNS
			if ((secret >> 13) & 1)		AES_ISHIFT_ROWS
			if ((secret >> 12) & 1)		AES_ISUB_BYTES
			if ((secret >> 11) & 1)		AES_IADD_ROUNDKEY
			if ((secret >> 10) & 1)		AES_IMIX_COLUMNS
			if ((secret >>  9) & 1)		AES_ISHIFT_ROWS
			if ((secret >>  8) & 1)		AES_ISUB_BYTES
			if ((secret >>  7) & 1)		AES_IADD_ROUNDKEY
			if ((secret >>  6) & 1)		AES_IMIX_COLUMNS
			if ((secret >>  5) & 1)		AES_ISHIFT_ROWS
			if ((secret >>  4) & 1)		AES_ISUB_BYTES
			if ((secret >>  3) & 1)		AES_IADD_ROUNDKEY
			if ((secret >>  2) & 1)		AES_IMIX_COLUMNS
			if ((secret >>  1) & 1)		AES_ISHIFT_ROWS
			if ( secret        & 1)		AES_ISUB_BYTES

			AddRoundKey(state, len, 0);
		}
	}

};}; // Reuben::Utility

#endif // _REUBEN_MATH_AES_H_
