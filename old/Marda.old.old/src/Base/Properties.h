/*!
	@file Properties.h


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */
#pragma once

namespace Marda
{
	/*!
		プロパティクラス

		@code
			Properties _props;

			// プロパティの追加
			_props.AddProperty(_T("key1"), Variant(1));
			_props.AddProperty(_T("key2"), Variant(_T("hoge"));
			_props[_T("key3")].SetFloat(1.0f);

			// プロパティの参照
			int val1 = _props.GetProperty(_T("key1"))->GetInt();
			String val2 = _props.GetProperty(_T("key2"))->GetString();
			float val3 = _props.GetProperty(_T("key3"))->GetFloat();
			
			val1 = _props[_T("key1")].GetInt();
			val2 = _props[_T("key2")].GetString();
			val3 = _props[_T("key3")].GetFloat();

			// プロパティの変更
			_props[_T("key1")].SetInt(2);
			_props[_T("key2")].SetString(_T("Fuga"));
			_props[_T("key3")].SetFloat(2.0f);

			// プロパティの削除
			_props.RemoveProperty(_T("key1"));
			_props.RemoveProperty(_T("key2"));
			_props.RemoveProperty(_T("key3"));
		@endcode
	 */
	class Properties
	{
	public:

		//! ctor
		Properties();

		//! copy ctor
		Properties(const Properties& rhs);

		//! dtor
		~Properties();

		/*!
			プロパティ追加
			@param	key	キー
			@param	val	値
		 */
		bool AddProperty(const String& key, const Variant& val);

		/*!
			プロパティ削除
			@param	key	キー
		 */
		void RemoveProperty(const String& key);

		/*!
			プロパティ取得
			@param	key	キー
			@return	値。キーに対応する値が無い場合はNULL。
		 */
		Variant* GetProperty(const String& key);

		const Variant* GetProperty(const String& key) const;

		// GetKeys()

		/*!
			インデクサ

			連想配列の様にアクセス出来る。
			要素が無い場合は、追加される。

			@param	key	キー
		 */
		Variant& operator [] (const String& key);

		
		/*!
			constインデクサ

			連想配列の様にアクセス出来る。
			要素が無い場合は、Assertionが発生するので注意。
		 */
		const Variant& operator [] (const String& key) const;


		Properties& operator = (const Properties& rhs);

	private:
		// pimpl pattern
		struct Impl;
		std::auto_ptr<Impl> _pimpl;	
	};

}	// end namespace Marda
