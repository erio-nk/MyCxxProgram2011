/*!
	@file Cast.h

	ポリモーフィック用のキャスト関数。
	

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#pragma once

#include "Assert.h"

namespace Marda
{
	// up_cast
	#define up_cast static_cast
		// TODO:


	#if defined(_DEBUG)
		/*!
			down_cast

			ダウンキャストをする関数。
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

			@tparam	T	キャストする型のポインタ型。
			@tparam	U	キャストされる型のポインタ型。
			@param	u	キャストされるポインタ。
			@return	ダウンキャスト結果。
		 */
		template <class T, class U>
		T down_cast(U u) 
		{
			T t = dynamic_cast<T>(u);
			if (t != static_cast<T>(u)) Assert(0, "Failed to down cast!"); 
			return t;
		}
	#else
		#define down_cast static_cast
	#endif

	#if defined(_DEBUG)
		/*!
			cross_cast

			クロスキャストをする関数。
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

			@tparam	T	キャストする型のポインタ型。
			@tparam	U	キャストされる型のポインタ型。
			@param	u	キャストされるポインタ。
			@return	クロスキャスト結果。
		 */
		template <class T, class U>
		T cross_cast(U* u)
		{
			T t = dynamic_cast(u);
			Assert(t, "Failed to down cast!");
			return t;
		}
	#else
		#define cross_cast dynamic_cast
	#endif

}	// end namespace Marda

