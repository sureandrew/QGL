#pragma once
#ifndef _REUBEN_PLATFORM_TEXT_FILE_H_
#define _REUBEN_PLATFORM_TEXT_FILE_H_

#include "File.h"

namespace Reuben { namespace Platform {

	class TextFile : public File
	{
	public:

		enum TypeEnum
		{
			TYPE_ASCII,
			TYPE_UTF8,
			TYPE_UTF16LE,
			TYPE_UTF16BE,
			TYPE_UTF32LE,
			TYPE_UTF32BE,

			TYPE_COUNT
		};
	};
/*
00 00 FE FF UTF-32, big-endian 
FF FE 00 00 UTF-32, little-endian 
FE FF UTF-16, big-endian 
FF FE UTF-16, little-endian 
EF BB BF UTF-8 */

}; };  // Reuben::Platform

#endif // _REUBEN_PLATFORM_TEXT_FILE_H_