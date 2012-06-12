/*!
	@file Uncopyable.h

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#pragma once

namespace Marda
{
	/*!
		Uncopyableクラス

		コピー不可にするクラス。
		このクラスを継承したクラスをコピーしようとした場合、
		コンパイル時にエラーが検出される。

		仮想デストラクタを持っていない為、
		Uncopyableクラスのポインタでdeleteをしてはならない。

		@code
			struct Hoge : Uncopyable
			{
				...
			};

			Hoge hoge1;
			hoge1 = hoge2;	// error!
		@endcode
	 */
	class Uncopyable
	{
	private:
		// not implements
		Uncopyable(const Uncopyable&);	// Copy ctor
		Uncopyable& operator = (const Uncopyable&);

	protected:
		Uncopyable() {}
		~Uncopyable() {}	// not virtual
	};

}	// end namespace Marda

