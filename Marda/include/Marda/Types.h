/*!
	@file	Types.h

	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	// バイトサイズを明示したい場合の型
	typedef signed char		sint8;
	typedef unsigned char	uint8;
	typedef signed short	sint16;
	typedef unsigned short	uint16;
	typedef signed int		sint32;
	typedef unsigned int	uint32;
	typedef signed long long	sint64;
	typedef unsigned long long	uint64;
	
	// 文字列型
	#if defined(_UNICODE)
	typedef std::wstring TString;
	#else
	typedef std::string TString;
	#endif

	// ヌル型。テンプレート用。
	struct NullType {};
	
	// スレッディングポリシー。テンプレート用。
	struct ThreadSafe {};
	struct ThreadUnsafe {};
	
}	// end namespace Marda

#if _MSC_VER <= 1500	// VC++9.0以下なら
// tr1を省略する為の宣言をする。VS10以降では、tr1は無くなる。
namespace std { namespace tr1 {}; using namespace tr1; }
#endif
