/*!
	@file	Coroutine.h
	
			�ȈՃR���[�`���N���X�B

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"

namespace Marda
{
	// ------------------------------------------------------------------------
	// �ȈՃR���[�`���N���X�B
	
	class Coroutine
	{
	public:
		Coroutine();
	
		~Coroutine();
		
		/*!
			@brief	�R���[�`�������s����B
		*/
		int operator () ();
		

	protected:
		/*!
			@brief	�R���[�`���Ŏ��s���鏈�����L�q����B
			@attention	���̊֐����I������i�֐��̍Ō�܂ŗ��邩�Areturn����j�O�ɃR���[�`���I�u�W�F�N�g��
						�j������Ă��܂��ƁA�֐����ō\�z���ꂽ�I�u�W�F�N�g�̃f�X�g���N�^���Ă΂ꂸ�A
						���������[�N�������N�����\��������ג��ӁB
						�֐����ł����������I�u�W�F�N�g�𗘗p����K�v������ꍇ�́A
						�����o�ϐ��Ƃ��Ē�`����������S�B
			@return	�R���[�`���Ăяo��(operator ())�̖߂�l�B
		*/
		virtual int Run() = 0;

		/*!
			@brief	�R���[�`���𒆒f���ČĂяo�����ɐ����Ԃ��B
			@return	�R���[�`���Ăяo��(operator ())�̖߂�l�B
		*/
		void Yeild(int returnValue);
		
	private:
		PVOID _pCallFiberContext;	//!< �Ăяo�����t�@�C�o�B
		PVOID _pThisFiberContext;	//!< ���g�̃t�@�C�o�B
		int _returnValue;	//!< �R���[�`���̖߂�l�B
		bool _exit;	//!< �R���[�`�����I���������ǂ����B
		
		void Exit();

		static void WINAPI FiberProc(PVOID pParam);
	};	// end class Coroutine


}	// end namespace Marda
