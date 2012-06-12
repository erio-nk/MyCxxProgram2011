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

		�o���A���g�Ȋ����̌^�B
		�F��Ȓl(�悭�g�������Ȓl)�������ĕێ��o����B
		�������A�\���̓��͕ێ��o���Ȃ��B
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
			int�^�Ŏ擾����
		 */
		int GetInt() const;

		/*!
			float�^�Ŏ擾����
		 */
		float GetFloat() const;

		/*!
			String�^�Ŏ擾����
		 */
		const String& GetString() const;

		/*!
			bool�^�Ŏ擾����
		 */
		bool GetBool() const;

		/*!
			int�^�̒l��ݒ肷��
		 */
		void SetInt(int val);

		/*!
			float�^�̒l��ݒ肷��
		 */
		void SetFloat(float val);
		
		/*!
			String�^�̒l��ݒ肷��
		 */
		void SetString(const String& val);
		
		/*!
			bool�^�l��ݒ肷��
		 */
		void SetBool(bool val);
		
	private:
		String _value;	//!< string�ŊǗ��B
	};

}	// end namespace Marda
