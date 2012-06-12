/**
 *	@file Type.h
 *
 *	�^�Ɋւ����`�t�@�C��
 *
 *	@author Dila
 */

#pragma once

//// ������
#include <string>
#ifdef WIN32
	#include <tchar.h>
#else
	//TODO: ���H
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

//// ����
#include <float.h>

// �[��
const float _ZEROF = 0.0f;
const double _ZEROD = 0.0;

// Not a Number
const float _NANF (_ZEROF/_ZEROF);
const double _NAND (_ZEROD/_ZEROD);

#ifdef WIN32
	/// x��NAN���ǂ����𒲂ׂ�
	inline bool _IsNan(float x) { return _isnan(x) != 0; }
	/// x��NAN���ǂ����𒲂ׂ�
	inline bool _IsNan(double x) { return _isnan(x) != 0; }
#else
	inline bool _IsNan(float x) { return isnan(x); }
	inline bool _IsNan(double x) { return isnan(x); }
#endif

// Inifinity
const float _INFF (1.0f/_ZEROF);
const double _INFD (1.0/_ZEROD);

#ifdef WIN32
	/// x��Infinity���ǂ����𒲂ׂ�
	inline bool _IsInf(float x) { return _finite(x) == 0; }
	/// x��Infinity���ǂ����𒲂ׂ�
	inline bool _IsInf(double x) { return _finite(x) == 0; }
#else
	inline bool _IsInf(float x) { return isinf(x); }
	inline bool _IsInf(double x) { return isinf(x); }
#endif


/// �s����float�l���ǂ������`�F�b�N����
inline bool _CheckBadF(float n)
{
#ifndef _NO_CHECK_BADF
	if (_IsNan(n) || _IsInf(n)) {
		return false;
	}
#endif
	return true;
}

/// �s����double�l���ǂ������`�F�b�N����
inline bool _CheckBadF(double n)
{
#ifndef _NO_CHECK_BADF
	if (_IsNan(n) || _IsInf(n)) {
		return false;
	}
#endif
	return true;
}

/// �s����float�ł���Βl��x�ɏC������false��Ԃ�
inline bool _CheckBadFX(float& n, float x)
{
	if (_CheckBadF(n)) {
		n = x;
		return false;
	}
	return true;
}

/// �s����float�ł���Βl��x�ɏC������false��Ԃ�
inline bool _CheckBadFX(double& n, double x)
{
	if (_CheckBadF(n)) {
		n = x;
		return false;
	}
	return true;
}


//// �^�ϊ�
/// @addtogroup MetaFunction ���^�֐�
/// @{

/**
 *	T����U�ւ̌^�ϊ��\���𒲂ׂ郁�^�֐�
 *
 *	�T���v��
 *	@code
 *		printf("Int to Double = %d\n", Conversion<int, double>::exists);
 *			// ���ʂ�1(true)
 *		printf("Int to Int* = %d\n", Conversion<int, int*>::exists);
 *			// ���ʂ�0(false)
 *	@endcode
 *	@note	���ЁuModern C++ Design�v���
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
		/// �ϊ��\�������邩�ǂ��� (true/false)
		exists = sizeof(Test(MakeT())) == sizeof(Small),

		/// �����^���ǂ��� (true/false)
		equals = false
	};
};

template <class T> struct Conversion<T,T>
{ enum { exists = true, equals = true }; };

/**
 *	T����U�ւ̃A�b�v�L���X�g�̉\���𒲂ׂ郁�^�֐�
 *
 *	�T���v��
 *	@code
 *		struct A {};
 *		struct ADash : public A {};
 *		struct B {};
 *		printf("ADash* to A* = %d\n", UpCast<ADash, A>::exists);
 *			// ���ʂ�1
 *		printf("A* to B* = %d\n", UpCast<A, B>::exists);
 *			// ���ʂ�0
 *	@endcode
 */
template <class T, class U>
struct UpCast
{
	enum {
		/// �A�b�v�L���X�g�\���ǂ��� (true/false)
		exists = Conversion<const T*, const U*>::exists && 
				!Conversion<const U*, const void*>::equals
	};
};

/// @}
