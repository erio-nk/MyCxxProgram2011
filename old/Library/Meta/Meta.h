/**
 *	@file Meta.h
 *
 *	���^�v���O���~���O���C�u����
 *
 *	@author Dila
 */
#pragma once


/**
 *	IF���^�֐�
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
