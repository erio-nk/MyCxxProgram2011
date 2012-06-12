/*!
	@file	Cast.h
	@author	Dila
	
	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.
 */
#pragma once

#include "Types.h"

namespace Marda
{
	// up_cast
	#define up_cast static_cast
		// TODO:

	#if defined(_DEBUG)
		/*!
			@brief	ダウンキャスト

			設計上ダウンキャストを行うという事を明示する為に使用し、
			開発時の誤キャストを未然に検出する。
			Releaseモード時には、単なるstatic_castに置き換わる。

			@code
				struct Hoge {...};
				struct Fuga : public Hoge {...};
				struct Piyo : public Hoge {...};

				Hoge* pHogeFuga = new Fuga();
				Hoge* pHogePiyo = new Piyo();

				Fuga* pFugaFuga = down_cast<Fuga*>(pHogeFuga);	// OK
				Fuga* pFugaPiyo = down_cast<Fuga*>(pHogePiyo);	// NG
			@endcode

			@tparam	From	キャストする型のポインタ型。
			@tparam	To		キャストされる型のポインタ型。
			@param	from	キャストされるポインタ。
			@return	ダウンキャスト結果。
		 */
		template <class To, class From>
		To down_cast(From from) 
		{
			To to = dynamic_cast<To>(from);
			if (to != static_cast<To>(from)) assert(!"Failed to down cast!"); 
			return to;
		}
	#else
		#define down_cast static_cast
	#endif

	#if defined(_DEBUG)
		/*!
			@brief	クロスキャスト

			設計上クロスキャストを行うという事を明示する為に使用し、
			開発時の誤キャストを未然に検出する。
			Releasaeモード時には、単なるdynamic_casatに置き換わる。

			@code
				sturct Moke {...};	// interface
				struct Hoge {...};
				struct Fuga : public Hoge, public Moke {...};
				struct Piyo : public Hoge {...};

				Hoge* pHogeFuga = new Fuga();
				Hoge* pHogePiyo = new Piyo();

				// Mokeインタフェースを実装しているか？
				if (cross_cast<Moke*>(pHogeFuga)) ...	// True
				if (cross_cast<Moke*>(PHogePiyo)) ...	// False
			@endcode

			@tparam	From	キャストする型のポインタ型。
			@tparam	To		キャストされる型のポインタ型。
			@param	from	キャストされるポインタ。
			@return	クロスキャスト結果。
		 */
		template <class To, class From>
		To cross_cast(From from) 
		{
			To to = dynamic_cast(from);
			assert(to && "Failed to down cast!");
			return to;
		}
	#else
		#define cross_cast dynamic_cast
	#endif


	/*!
		@brief	数値 <-> 文字列変換キャスト

		atoiとかsprintfとかを使わずカンタンに数値と文字列の相互変換が出来る関数。
		boost::lexical_castと同じ。

		@code
			int hoge = 2;
			String fuga = lexical_cast<String>(hoge);
			float piyo = lexical_cast<float>(fuga);
		@endcode
	 */
	template <class To, class From>
	To lexical_cast(const From& from) 
	{
		StringStream ss;
		ss << from;

		To to;
		ss >> to;
		return to;
	}

}	// end namespace Marda
 