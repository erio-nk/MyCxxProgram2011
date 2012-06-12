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
		Delegate�N���X

		C#��Delegate�I�ȃN���X�B

		@code
			// �֐����Ϗ�����
			bool Func(int x) {...}

			Delegate<bool, int>* pDelegate = Delegate<bool, int>::New(&Func);
			bool ret = (*pDelegate)(1);	// ���s
			delete pDelegate;	// New����Delegate�͉������

			// �����o�֐����Ϗ�����
			struct Obj {
				bool MemFunc(int x) {...}
			} obj;

			pDelegate = Delegate<bool, int>::New(&obj, &Obj::MemFunc);
			ret = (*pDelegate)(1);	// ���s
			delete pDelegate;	// ����ς���

			// �߂�l��void�Ȋ֐����Ϗ��o����
			void VoidFunc(int x) {...}
			Delegate<void, int>* pVoidDel = Delegate<void, int>::New(&VoidFunc);
			(*pVoidDel)(1);	// ���s�B�߂�l�͎擾�o���Ȃ��B
			delete pVoidDel;	// �����������
		@endcode

		@tparam R	�֐��̖߂�l�^�Bvoid�̎w����B
		@tparam	P1	�֐��̑������̌^�B�ȗ��B
		@tparam P2	�֐��̑������̌^�B�ȗ��B
		@tparam P3	�֐��̑������̌^�B�ȗ��B�Ƃ肠���������͎O�܂ŁB
		@tparam	End	���p�҂͂��̃p�����[�^���w��o���Ȃ��B
					�����̐��𑝂₷��(�Ⴆ��P4��ǉ������)��
					�R�[�f�B���O�ʂ����炷�ׂ̑΍�(C2753)�B
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
