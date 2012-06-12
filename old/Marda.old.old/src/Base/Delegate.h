/*!
	@file Delegate.h


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */
#pragma once

namespace Marda
{
	/*!
		Delegateクラス

		C#のDelegate的なクラス。

		@code
			// 関数を委譲する
			bool Func(int x) {...}

			Delegate<bool, int>* pDelegate = Delegate<bool, int>::New(&Func);
			bool ret = (*pDelegate)(1);	// 実行
			delete pDelegate;	// NewしたDelegateは解放する

			// メンバ関数を委譲する
			struct Obj {
				bool MemFunc(int x) {...}
			} obj;

			pDelegate = Delegate<bool, int>::New(&obj, &Obj::MemFunc);
			ret = (*pDelegate)(1);	// 実行
			delete pDelegate;	// やっぱり解放

			// 戻り値がvoidな関数も委譲出来る
			void VoidFunc(int x) {...}
			Delegate<void, int>* pVoidDel = Delegate<void, int>::New(&VoidFunc);
			(*pVoidDel)(1);	// 実行。戻り値は取得出来ない。
			delete pVoidDel;	// しつこいが解放
		@endcode

		@tparam R	関数の戻り値型。voidの指定も可。
		@tparam	P1	関数の第一引数の型。省略可。
		@tparam P2	関数の第二引数の型。省略可。
		@tparam P3	関数の第二引数の型。省略可。とりあえず引数は三つまで。
		@tparam	End	利用者はこのパラメータを指定出来ない。
					引数の数を増やす際(例えばP4を追加する際)の
					コーディング量を減らす為の対策(C2753)。
	 */
	template
	<
		class R,
		class P1 = void,
		class P2 = void,
		class P3 = void,
		class End = void
	>
	class Delegate;

	
}	// end namespace Marda

#include "Delegate.hpp"
