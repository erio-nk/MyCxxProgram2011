/*!
	@file Variant.h


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#pragma once

namespace Marda
{
	/*!
		Variant

		バリアントな感じの型。
		色んな値(よく使いそうな値)を代入して保持出来る。
		ただし、構造体等は保持出来ない。
	 */
	class Variant
	{
	public:
		// ctor
		Variant();

		Variant(int val);

		Variant(float val);

		Variant(const String& val);

		Variant(bool val);

		// dtor
		~Variant() {}


		/*!
			int型で取得する
		 */
		int GetInt() const;

		/*!
			float型で取得する
		 */
		float GetFloat() const;

		/*!
			String型で取得する
		 */
		const String& GetString() const;

		/*!
			bool型で取得する
		 */
		bool GetBool() const;

		/*!
			int型の値を設定する
		 */
		void SetInt(int val);

		/*!
			float型の値を設定する
		 */
		void SetFloat(float val);
		
		/*!
			String型の値を設定する
		 */
		void SetString(const String& val);
		
		/*!
			bool型値を設定する
		 */
		void SetBool(bool val);
		
	private:
		String _value;	//!< stringで管理。
	};

}	// end namespace Marda
