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
			@brief	�_�E���L���X�g

			�݌v��_�E���L���X�g���s���Ƃ������𖾎�����ׂɎg�p���A
			�J�����̌�L���X�g�𖢑R�Ɍ��o����B
			Release���[�h���ɂ́A�P�Ȃ�static_cast�ɒu�������B

			@code
				struct Hoge {...};
				struct Fuga : public Hoge {...};
				struct Piyo : public Hoge {...};

				Hoge* pHogeFuga = new Fuga();
				Hoge* pHogePiyo = new Piyo();

				Fuga* pFugaFuga = down_cast<Fuga*>(pHogeFuga);	// OK
				Fuga* pFugaPiyo = down_cast<Fuga*>(pHogePiyo);	// NG
			@endcode

			@tparam	From	�L���X�g����^�̃|�C���^�^�B
			@tparam	To		�L���X�g�����^�̃|�C���^�^�B
			@param	from	�L���X�g�����|�C���^�B
			@return	�_�E���L���X�g���ʁB
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
			@brief	�N���X�L���X�g

			�݌v��N���X�L���X�g���s���Ƃ������𖾎�����ׂɎg�p���A
			�J�����̌�L���X�g�𖢑R�Ɍ��o����B
			Releasae���[�h���ɂ́A�P�Ȃ�dynamic_casat�ɒu�������B

			@code
				sturct Moke {...};	// interface
				struct Hoge {...};
				struct Fuga : public Hoge, public Moke {...};
				struct Piyo : public Hoge {...};

				Hoge* pHogeFuga = new Fuga();
				Hoge* pHogePiyo = new Piyo();

				// Moke�C���^�t�F�[�X���������Ă��邩�H
				if (cross_cast<Moke*>(pHogeFuga)) ...	// True
				if (cross_cast<Moke*>(PHogePiyo)) ...	// False
			@endcode

			@tparam	From	�L���X�g����^�̃|�C���^�^�B
			@tparam	To		�L���X�g�����^�̃|�C���^�^�B
			@param	from	�L���X�g�����|�C���^�B
			@return	�N���X�L���X�g���ʁB
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
		@brief	���l <-> ������ϊ��L���X�g

		atoi�Ƃ�sprintf�Ƃ����g�킸�J���^���ɐ��l�ƕ�����̑��ݕϊ����o����֐��B
		boost::lexical_cast�Ɠ����B

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
 