/*!
	@file	StaticList.h
	@brief	�R���p�C�������X�g�B

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */

#include "Marda.h"

namespace Marda
{
	template <class T, T Val>
	struct StaticValue
	{
		enum { Value = Val };
	};

	template <class E, class T>
	struct StaticList
	{
		typedef E Element;
		typedef T Next;

		//
		//struct Length
		//{
		//	enum { Result = Next::Length::Result + 1 };
		//};
		enum { Length = Next::Length + 1 };

		//
		//template <int Index>
		//struct At
		//{
		//	typedef typename Next::At<Index-1>::Result Result;
		//};
		
		template <int Index>
		struct At;

		template <>
		struct At<0>
		{
			typedef typename Element Result;
		};

		//
		template <template <class E> class Functor>
		struct ForEach
		{
			ForEach()
			{
				Functor<E>()();	// Functor�̈ꎞ�I�u�W�F�N�g�𐶐����āAoperator()�����s����B
				Next::ForEach<Functor>();	
					// ���̗v�f��ForEach�I�u�W�F�N�g�𐶐����Ď���Functor���Ăяo������ȍ~NullType�܂ŌJ��Ԃ��B
			};
		};
	};

	template <class E>
	struct StaticList<E, NullType>	// �v�f�����(��������=NullType)�̃��X�g
	{
		typedef E Element;

		//struct Length
		//{
		//	enum { Result = 1 };
		//};
		enum { Length = 1 };

		template <int Index>
		struct At
		{
			// �G���[�B�͈͊O�ւ̃A�N�Z�X�B
			typedef NullType Result;
		};
		
		template <>
		struct At<0>
		{
			typedef Element Result;
		};

		template <template <class E> class Functor>
		struct ForEach
		{
			ForEach() 
			{
				Functor<E>()();
				// �R�R�ŏI���B
			}
		};
	};


	// ����
	//template <class T>
	//struct StaticList_Length
	//{
	//	enum { Result = 1 + StaticList_Length<T::Next>::Result };
	//};

	//template <>
	//struct StaticList_Length<NullType>
	//{
	//	enum { Result = 0 };
	//};

	//// At
	//template <class T, int Index>
	//struct StaticList_At
	//{
	//	typedef typename StaticList_At<typename T::Next, Index-1>::Result Result;
	//};
	//template <class T>
	//struct StaticList_At<T, 0>
	//{
	//	typedef typename T::Element Result;
	//};


}	// end namespace Marda


template <class E, class T>
template <int Index>
struct Marda::StaticList<E, T>::At
{
	typedef typename Next::At<Index-1>::Result Result;
};