/*!
	@file Number.h


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#pragma once

namespace Marda
{
	// char
	namespace Char
	{
		const char MIN = CHAR_MIN;
		const char MAX = CHAR_MAX;
	}

	// signed char
	typedef signed char schar;
	namespace SChar
	{
		const signed char MIN = SCHAR_MIN;
		const signed char MAX = SCHAR_MAX;
	}

	// unsigned char
	typedef unsigned char uchar;
	namespace UChar
	{
		const unsigned char MAX = UCHAR_MAX;
	}

	// short
	namespace Short
	{
		const short MIN = SHRT_MIN;
		const short MAX = SHRT_MAX;
	}

	typedef signed short sshort;
	namespace SShort = Short;

	// unsigned short
	typedef unsigned short ushort;
	namespace UShort
	{
		const unsigned short MAX = USHRT_MAX;
	}

	// int
	namespace Int
	{
		const int MIN = INT_MIN;
		const int MAX = INT_MAX;
	}
	
	typedef signed int sint;
	namespace SInt = Int;

	// unsigned int
	typedef unsigned int uint;
	namespace UInt
	{
		const unsigned int MAX = UINT_MAX;
	}

	// long
	namespace Long
	{
		const long MIN = LONG_MIN;
		const long MAX = LONG_MAX;
	}
	
	typedef signed long slong;
	namespace SLong = Long;

	// unsigned long
	typedef unsigned long ulong;
	namespace ULong
	{
		const unsigned long MAX = ULONG_MAX;
	}

	// long long
	typedef long long llong;
	namespace LLong
	{
		const long long MIN = LLONG_MIN;
		const long long MAX = LLONG_MAX;
	}

	typedef signed long long sllong;
	namespace SLLong = LLong;

	// unsigned long long
	typedef unsigned long long ullong;
	namespace ULLong
	{
		const unsigned long long MAX = ULLONG_MAX;
	}

	// TODO: cent

	// sint8
	typedef schar sint8;
	namespace SInt8 = SChar;

	// uint8
	typedef uchar uint8;
	namespace UInt8 = UChar;

	// sint16
	typedef sshort sint16;
	namespace SInt16 = SShort;

	// uint16
	typedef ushort uint16;
	namespace UInt16 = UShort;
	
	// sint32
	typedef sint sint;
	namespace SInt32 = SInt;
	
	// uint32
	typedef uint uint;
	namespace UInt32 = UInt;

	// sint64
	typedef sllong sint64;
	namespace SInt64 = SInt;
	
	// uint64
	typedef ullong uint64;
	namespace UInt64 = UInt;

}	// end namespace Marda
