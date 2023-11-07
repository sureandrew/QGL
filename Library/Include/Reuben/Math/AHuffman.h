#pragma once
#ifndef _REUBEN_MATH_AHUFFMAN_H_
#define _REUBEN_MATH_AHUFFMAN_H_

#define AH_CODE_MAX				256
#define AH_TABLE_SIZE			(AH_CODE_MAX * 2 + 1)
#define AH_BUFFER_EXTEND		(AH_CODE_MAX * 2)
#define AH_CODE_MAX_LEN			32
//#define AH_HEADER_SIZE			2

namespace Reuben { namespace Math {

class AHuffman
{
public:
	AHuffman();
	Boolean Encode(CONST Byte *inData, UInt16 inSize, Byte *outData, UInt16 &outSize);
	Boolean Decode(CONST Byte *inData, UInt16 inSize, Byte *outData, UInt16 &outSize);
	void DebugTree(UInt16 index, String code);
	float GetAvgRatio() CONST
	{ 
		return ((m_compSize != 0) ? m_uncompSize / m_compSize : 1.0f);
	}

	static AHuffman* NewInstance();				// must use NewInstance to create instance
	static void Release(AHuffman* &pHuffman);	// must use Release to delete instance
	static const TCHAR* ToBinaryStr(Byte* data, int start, int stop);

private:
	Boolean UpdateTree(Byte value, UInt16 current);

	struct Node {
		Byte		value;					// store value
		UInt32		count;					// frequent count
		UInt16		left;					// left node number
		UInt16		right;					// right node number
		UInt16		parent;					// parent node number
	};

	Node		m_tree[AH_TABLE_SIZE];		// node tree array
	UInt16		m_NYT;						// not yet transfer node number
	float		m_compSize;					// compressed size for ratio calculation
	float		m_uncompSize;				// uncompressed size for ratio calculation
	UInt8		m_maxCodeLen;				// maximum code length
	UInt16		m_indice[AH_CODE_MAX];		// node index lookup table
	UInt8		m_code[AH_CODE_MAX_LEN+1];	// code bit for encoding

	typedef BoostPool<boost::default_user_allocator_malloc_free> MemoryPool;
	static MemoryPool& GetPool();

	static ThreadKey m_key;
};

INLINE AHuffman::MemoryPool& AHuffman::GetPool()
{
	static MemoryPool mpool(sizeof(AHuffman));

	return mpool;
}

};}; // Reuben::Utility

#endif // _REUBEN_MATH_AHUFFMAN_H_