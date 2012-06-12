/*!
	@file	StaticList.h
	@brief	コンパイル時リスト。

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
				Functor<E>()();	// Functorの一時オブジェクトを生成して、operator()を実行する。
				Next::ForEach<Functor>();	
					// 次の要素のForEachオブジェクトを生成して次のFunctorを呼び出す･･･以降NullTypeまで繰り返し。
			};
		};
	};

	template <class E>
	struct StaticList<E, NullType>	// 要素が一個(次が無い=NullType)のリスト
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
			// エラー。範囲外へのアクセス。
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
				// ココで終わり。
			}
		};
	};


	// 長さ
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