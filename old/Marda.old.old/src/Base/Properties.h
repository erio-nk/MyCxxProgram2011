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
		�v���p�e�B�N���X

		@code
			Properties _props;

			// �v���p�e�B�̒ǉ�
			_props.AddProperty(_T("key1"), Variant(1));
			_props.AddProperty(_T("key2"), Variant(_T("hoge"));
			_props[_T("key3")].SetFloat(1.0f);

			// �v���p�e�B�̎Q��
			int val1 = _props.GetProperty(_T("key1"))->GetInt();
			String val2 = _props.GetProperty(_T("key2"))->GetString();
			float val3 = _props.GetProperty(_T("key3"))->GetFloat();
			
			val1 = _props[_T("key1")].GetInt();
			val2 = _props[_T("key2")].GetString();
			val3 = _props[_T("key3")].GetFloat();

			// �v���p�e�B�̕ύX
			_props[_T("key1")].SetInt(2);
			_props[_T("key2")].SetString(_T("Fuga"));
			_props[_T("key3")].SetFloat(2.0f);

			// �v���p�e�B�̍폜
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
			�v���p�e�B�ǉ�
			@param	key	�L�[
			@param	val	�l
		 */
		bool AddProperty(const String& key, const Variant& val);

		/*!
			�v���p�e�B�폜
			@param	key	�L�[
		 */
		void RemoveProperty(const String& key);

		/*!
			�v���p�e�B�擾
			@param	key	�L�[
			@return	�l�B�L�[�ɑΉ�����l�������ꍇ��NULL�B
		 */
		Variant* GetProperty(const String& key);

		const Variant* GetProperty(const String& key) const;

		// GetKeys()

		/*!
			�C���f�N�T

			�A�z�z��̗l�ɃA�N�Z�X�o����B
			�v�f�������ꍇ�́A�ǉ������B

			@param	key	�L�[
		 */
		Variant& operator [] (const String& key);

		
		/*!
			const�C���f�N�T

			�A�z�z��̗l�ɃA�N�Z�X�o����B
			�v�f�������ꍇ�́AAssertion����������̂Œ��ӁB
		 */
		const Variant& operator [] (const String& key) const;


		Properties& operator = (const Properties& rhs);

	private:
		// pimpl pattern
		struct Impl;
		std::auto_ptr<Impl> _pimpl;	
	};

}	// end namespace Marda
