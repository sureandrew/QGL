#pragma once
#ifndef _BLOCLWORD_H_
#define _BLOCLWORD_H_


struct BlockWordData
{
	UInt32 index;
	TCHAR blockword[100];
	UInt8 type;

	BlockWordData()
		: index(C_UINT32(0))
		, type(C_UINT8(0))
	{
		ZeroMemory(blockword, 100 * sizeof(TCHAR));
	}

	~BlockWordData(){};
	
};

#endif // _BLOCLWORD_H_