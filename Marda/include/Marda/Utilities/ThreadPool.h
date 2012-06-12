/*!
	@file	Utilities/ThreadPool.h
	
			�X���b�h�v�[���B
			ppl�g���Ηǂ��ˁH

	@author	E.Nakayama
*/
#pragma once
	
#include "WorkerThread.h"

namespace Marda
{
	/*!
		@brief	�X���b�h�v�[���N���X�B
	*/
	class ThreadPool
	{
	public:
		//! ctor
		ThreadPool();
		
		//! dtor
		~ThreadPool();
		
		/*!
			@brief	�X���b�h�v�[�����쐬����B
			@param	numThreads	�쐬���郏�[�J�[�X���b�h���B-1��n�����ꍇ�́ACPU�̘_���R�A�����������[�J�[�X���b�h���쐬����B
		*/
		bool Create(int numThreads = -1);
		
		/*!
			@brief	�X���b�h�v�[�����������B
					���s���^���s�҂��̎d�����������ꍇ�́A�����𒆒f����B
		*/
		void Release();
		
		/*!
			@brief	���̃��[�J�[�X���b�h�̃X���b�hID���擾����
		*/
		DWORD GetThreadID() const;
		
		/*!
			@brief	�d�����v�b�V������
			@param	pWork	�d���I�u�W�F�N�g�B
							���̃p�����[�^�ɓn�����I�u�W�F�N�g�́A�������������邩���f�����܂Ő������Ă���K�v������B
			@note	���̊֐��̓X���b�h�Z�[�t�ł���B
		*/
		bool Push(WorkerThread::Work* pWork);
		
//		/*!
//			@brief	�d���𒆒f����
//					���f�������d�������s���������ꍇ�́A���̎d�����I���܂Ńu���b�N�����B
//			@param	pWork	���f����d���I�u�W�F�N�g�B
//		*/
//		void Cancel(Work* pWork);
//		
//		/*!
//			@brief	�d�����S�ďI���܂ő҂�
//			@param	timeout	�^�C���A�E�g����~���b
//			@return	�d�����S�ďI������ꍇ��true�A�^�C���A�E�g�����ꍇ��false��Ԃ��B
//		*/
//		bool WaitForCompletion(DWORD timeout = INFINITE);
//		
//		/*!
//			@brief	�d�����S�ďI��������ǂ�����Ԃ�
//			@attention	���̊֐��̎��s�ɂ͍Œ�1ms��v����B
//		*/
//		bool IsCompleted();
		
	private:
		struct Impl;
		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom
	};	// end class ThreadPool
	
}	// end namespace Marda
