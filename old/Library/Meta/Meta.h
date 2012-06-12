/**
 *	@file Meta.h
 *
 *	メタプログラミングライブラリ
 *
 *	@author Dila
 */
#pragma once


/**
 *	IFメタ関数
 */
template <bool Expression, class TrueType, class FalseType>
struct IF
{
	typedef TrueType ResultType;
};

template <class TrueType, class FlaseType>
struct IF<false, TrueType, FalseType>
{
	typedef FalseType ResultType;
};

//
template <bool Expression, int trueVal, int falseVal>
struct IF
{
	enum { result = trueVal };
};

template <int trueVal, int falseVal>
struct IF<false, trueVal, falseVal>
{
	enum { result = falseVal };
};
