/*!
	@file	Types.h
	@author	Dila
	
	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.
 */
#pragma once

namespace Marda
{
	//// 文字列
	typedef std::string StringA;
	typedef std::wstring StringW;

	typedef std::stringstream StringStreamA;
	typedef std::wstringstream StringStreamW;

	#if defined(_UNICODE)
		typedef StringW String;
		typedef StringStreamW StringStream;
	#else
		typedef StringA String;
		typedef StringStreamA StringStream;
	#endif

	//// 整数型

	// schar
	typedef signed char schar;
	namespace SChar
	{
		static const schar MIN = SCHAR_MIN;
		static const schar MAX = SCHAR_MAX;
	}

	// char
	namespace Char
	{
		static const char MIN = CHAR_MIN;
		static const char MAX = CHAR_MAX;
	}

	// uchar
	typedef unsigned char uchar;
	namespace UChar
	{
		static const uchar MIN = 0;
		static const uchar MAX = UCHAR_MAX;
	}

	// short
	namespace Short
	{
		static const short MIN = SHRT_MIN;
		static const short MAX = SHRT_MAX;
	}

	typedef signed short sshort;
	namespace SShort = Short;

	// ushort
	typedef unsigned short ushort;
	namespace UShort
	{
		static const ushort MIN = 0;
		static const ushort MAX = USHRT_MAX;
	}

	// int
	namespace Int
	{
		static const int MIN = INT_MIN;
		static const int MAX = INT_MAX;
	}

	typedef signed int sint;
	namespace SInt = Int;

	// uint
	typedef unsigned int uint;
	namespace UInt
	{
		static const uint MIN = 0;
		static const uint MAX = UINT_MAX;
	}

	// long
	namespace Long
	{
		static const long MIN = LONG_MIN;
		static const long MAX = LONG_MAX;
	}

	typedef signed long slong;
	namespace SLong = Long;

	// ulong
	typedef unsigned long ulong;
	namespace ULong
	{
		static const ulong MIN = 0;
		static const ulong MAX = ULONG_MAX;
	}

	// llong
	typedef long long llong;
	namespace LLong
	{
		static const llong MIN = LLONG_MIN;
		static const llong MAX = LLONG_MAX;
	}
	
	typedef signed long long sllong;
	namespace SLLong = LLong;

	// ullong
	typedef unsigned long long ullong;
	namespace ULLong
	{
		static const ullong MIN = 0;
		static const ullong MAX = ULLONG_MAX;
	}

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
	typedef sint sint32;
	namespace SInt32 = SInt;
	
	// uint32
	typedef uint uint32;
	namespace UInt32 = UInt;

	// sint64
	typedef sllong sint64;
	namespace SInt64 = SInt;
	
	// uint64
	typedef ullong uint64;
	namespace UInt64 = UInt;


	//// 実数型


	//// Uncopyable

	/*!
		@brief	コピーを不可にさせる型。
	 */
	//template <class Derived>
	//class Uncopyable
	//{
	//private:
	//	Uncopyable(const Uncopyable&);	// Copy ctor
	//	Uncopyable& operator = (const Uncopyable&);
	//};


	//// NullType

	/*!
		@brief	ヌルタイプ。
				タイプリスト等の終端を表すリスト。
	 */
	class NullType {};

}	// end namespace Marda
