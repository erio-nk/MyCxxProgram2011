/**
 *	@file Type.h
 *
 *	型に関する定義ファイル
 *
 *	@author Dila
 */

#pragma once

//// 文字列
#include <string>
#ifdef WIN32
	#include <tchar.h>
#else
	//TODO: 何？
#endif

// tstring
namespace std
{
	#ifdef _UNICODE
		typedef wstring tstring;
	#else
		typedef string tstring;
	#endif
}

//// 実数
#include <float.h>

// ゼロ
const float _ZEROF = 0.0f;
const double _ZEROD = 0.0;

// Not a Number
const float _NANF (_ZEROF/_ZEROF);
const double _NAND (_ZEROD/_ZEROD);

#ifdef WIN32
	/// xがNANかどうかを調べる
	inline bool _IsNan(float x) { return _isnan(x) != 0; }
	/// xがNANかどうかを調べる
	inline bool _IsNan(double x) { return _isnan(x) != 0; }
#else
	inline bool _IsNan(float x) { return isnan(x); }
	inline bool _IsNan(double x) { return isnan(x); }
#endif

// Inifinity
const float _INFF (1.0f/_ZEROF);
const double _INFD (1.0/_ZEROD);

#ifdef WIN32
	/// xがInfinityかどうかを調べる
	inline bool _IsInf(float x) { return _finite(x) == 0; }
	/// xがInfinityかどうかを調べる
	inline bool _IsInf(double x) { return _finite(x) == 0; }
#else
	inline bool _IsInf(float x) { return isinf(x); }
	inline bool _IsInf(double x) { return isinf(x); }
#endif


/// 不正なfloat値かどうかをチェックする
inline bool _CheckBadF(float n)
{
#ifndef _NO_CHECK_BADF
	if (_IsNan(n) || _IsInf(n)) {
		return false;
	}
#endif
	return true;
}

/// 不正なdouble値かどうかをチェックする
inline bool _CheckBadF(double n)
{
#ifndef _NO_CHECK_BADF
	if (_IsNan(n) || _IsInf(n)) {
		return false;
	}
#endif
	return true;
}

/// 不正なfloatであれば値をxに修正してfalseを返す
inline bool _CheckBadFX(float& n, float x)
{
	if (_CheckBadF(n)) {
		n = x;
		return false;
	}
	return true;
}

/// 不正なfloatであれば値をxに修正してfalseを返す
inline bool _CheckBadFX(double& n, double x)
{
	if (_CheckBadF(n)) {
		n = x;
		return false;
	}
	return true;
}


//// 型変換
/// @addtogroup MetaFunction メタ関数
/// @{

/**
 *	TからUへの型変換可能性を調べるメタ関数
 *
 *	サンプル
 *	@code
 *		printf("Int to Double = %d\n", Conversion<int, double>::exists);
 *			// 結果は1(true)
 *		printf("Int to Int* = %d\n", Conversion<int, int*>::exists);
 *			// 結果は0(false)
 *	@endcode
 *	@note	書籍「Modern C++ Design」より
 */
template <class T, class U>
class Conversion
{
	typedef char Small;
	class Big { char dummy[2]; };
	static Small Test(U);
	static Big Test(...);
	static T MakeT();
public:
	enum { 
		/// 変換可能性があるかどうか (true/false)
		exists = sizeof(Test(MakeT())) == sizeof(Small),

		/// 同じ型かどうか (true/false)
		equals = false
	};
};

template <class T> struct Conversion<T,T>
{ enum { exists = true, equals = true }; };

/**
 *	TからUへのアップキャストの可能性を調べるメタ関数
 *
 *	サンプル
 *	@code
 *		struct A {};
 *		struct ADash : public A {};
 *		struct B {};
 *		printf("ADash* to A* = %d\n", UpCast<ADash, A>::exists);
 *			// 結果は1
 *		printf("A* to B* = %d\n", UpCast<A, B>::exists);
 *			// 結果は0
 *	@endcode
 */
template <class T, class U>
struct UpCast
{
	enum {
		/// アップキャスト可能かどうか (true/false)
		exists = Conversion<const T*, const U*>::exists && 
				!Conversion<const U*, const void*>::equals
	};
};

/// @}
