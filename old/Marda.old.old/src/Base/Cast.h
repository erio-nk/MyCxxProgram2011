/*!
	@file Cast.h

	�|�����[�t�B�b�N�p�̃L���X�g�֐��B
	

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

			�_�E���L���X�g������֐��B
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

			@tparam	T	�L���X�g����^�̃|�C���^�^�B
			@tparam	U	�L���X�g�����^�̃|�C���^�^�B
			@param	u	�L���X�g�����|�C���^�B
			@return	�_�E���L���X�g���ʁB
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

			�N���X�L���X�g������֐��B
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

			@tparam	T	�L���X�g����^�̃|�C���^�^�B
			@tparam	U	�L���X�g�����^�̃|�C���^�^�B
			@param	u	�L���X�g�����|�C���^�B
			@return	�N���X�L���X�g���ʁB
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

