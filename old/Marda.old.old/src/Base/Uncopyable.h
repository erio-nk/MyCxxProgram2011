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
		Uncopyable�N���X

		�R�s�[�s�ɂ���N���X�B
		���̃N���X���p�������N���X���R�s�[���悤�Ƃ����ꍇ�A
		�R���p�C�����ɃG���[�����o�����B

		���z�f�X�g���N�^�������Ă��Ȃ��ׁA
		Uncopyable�N���X�̃|�C���^��delete�����Ă͂Ȃ�Ȃ��B

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

