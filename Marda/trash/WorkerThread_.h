/*!
	@file	Utilities/WorkerThread.h
	
			���[�J�[�X���b�h�N���X�B

	@author	E.Nakayama
*/
#pragma once
	

namespace Marda
{
	/*!
		@brief	���[�J�[�X���b�h�N���X�B
				�}���`�X���b�h�̃f�U�C���p�^�[�� Worker Thread �p�^�[�������������N���X�B
				�d�����I���Ă��X���b�h���I���������A���̎d����҂�������X���b�h���[�e�B���e�B�N���X�B

				�Ăяo�����X���b�h�Ŗ��t���[�� Sync �֐����Ăяo���l�ɂ��AWork::Execute �֐�����
				Work::SyncObject �𐶐����鎖�ŁA�Ăяo�����X���b�h�Ƃ̓����������s�������o����B
	*/
	class WorkerThread : private Uncopyable
	{
	public:
		/*!
			@brief	�d���C���^�[�t�F�[�X�B
					���̃C���^�t�F�[�X������������ۃN���X�� Push �֐��ɓn���Ă������ŏ������s���B
		*/
		class Work
		{
		public:
			enum STATE
			{
				STATE_NONE,
				STATE_QUEUEING,		//!< ���s�҂�
				STATE_EXECUTING,	//!< ���s��
				STATE_COMPLETE,		//!< ���s�I��
				STATE_CANCELED,		//!< ���f���ꂽ
				STATE_ERROR,		//!< �G���[���������ďI������
			};
						
			Work();

			virtual ~Work()
			{
			}
			
			WorkerThread* GetOwner() const { return _pOwner; }
			
			STATE GetState() const { return _state; }
			
			/*!
				@brief	���s�����B
						�T�u�N���X�́A���̊֐���K���I�[�o�[���C�h���Ȃ���΂Ȃ�Ȃ��B
				@return	true ��Ԃ����ꍇ�͐������ASTATE_COMPLETE �ɂȂ�B
						false ��Ԃ����ꍇ�͎��s���ASTATE_ERROR �ɂȂ�B
			*/
			virtual bool Execute() = 0;
			
			/*!
				@brief	��ԕύX�ʒm�C�x���g�B
						��Ԃ��ς��x�ɌĂяo�����C�x���g�n���h���B
						�T�u�N���X�́A�K�v�ł���΂��̃n���h�����I�[�o�[���C�h����
						�K�؂ɏ������鎖���o����B
				@param	state	�ύX��̏��
			*/
			virtual void OnNotify(STATE state) { UNREFERENCED_PARAMETER(state); }

		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		protected:
			class SyncObject;
		#endif

		private:
			friend class WorkerThread;
			WorkerThread* _pOwner;
			STATE _state;
			AUTO_EVENT_HANDLE _hEndEvent;
			void ChangeState(STATE state);
		};	// end class Work
		
		typedef std::shared_ptr<Work> WorkPtr;	// shared_ptr �̓X���b�h�Z�[�t�B
		//typedef smart_ptr<Work, ThreadSafe> WorkPtr;
		
		//! ctor
		WorkerThread();
		
		//! dtor
		~WorkerThread();
		
		/*!
			@brief	���[�J�[�X���b�h���쐬����
		*/
		bool Create();
		
		/*!
			@brief	���[�J�[�X���b�h���������
					���s���^���s�҂��̎d�����������ꍇ�́A�����𒆒f����B
		*/
		void Release();
		
		/*!
			@brief	���̃��[�J�[�X���b�h�̃X���b�hID���擾����
		*/
		DWORD GetThreadID() const;
		
		/*!
			@brief	�d����ǉ�����B
			@param	pWork	�d���I�u�W�F�N�g�B
							�����̌^�� shared_ptr �ł���ׁA�n�����I�u�W�F�N�g�͏�������������܂Ő������Ă��鎖�ɒ��ӁB
			@note	���̊֐��̓X���b�h�Z�[�t�ł���B
		*/
		bool Push(WorkPtr pWork);
		
		/*!
			@brief	�d���𒆒f����B
					�����������f�������d�������s���������ꍇ�́A���̎d�����I���܂Ńu���b�N�����B
					����͒��f�������d�������s���������ꍇ�͒��f�Ɏ��s����B
			@param	pWork	���f����d���I�u�W�F�N�g�B
			@todo	����̎d�����I��������ǂ������S�ɑ҂֐��B
		*/
		bool Cancel(WorkPtr pWork);
		
		/*!
			@brief	�d�����S�ďI���܂ő҂�
			@param	timeout	�^�C���A�E�g����~���b
			@return	�d�����S�ďI������ꍇ��true�A�^�C���A�E�g�����ꍇ��false��Ԃ��B
		*/
		bool WaitForCompletion(DWORD timeout = INFINITE);
		
		/*!
			@brief	�d�����S�ďI��������ǂ�����Ԃ�
			@attention	���̊֐��̎��s�ɂ͍Œ�1ms��v����B
		*/
		bool IsCompleted();
		
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		/*!
			@brief	���[�J�[�X���b�h�Ƃ̓����������s���B
					���[�J�[�X���b�h���������������J�n���Ă��Ȃ��ꍇ�́A�����ɐ��䂪�߂�B
					���[�J�[�X���b�h���������������J�n���Ă���ꍇ�́A�ꎞ��~���Ă��郏�[�J�[�X���b�h���ĊJ���A
					�����������I������܂ŌĂяo�����X���b�h�ɐ����Ԃ��Ȃ��B
					Work::Execute ���� Work::SyncObject �𗘗p���鎖������ꍇ�́A���t���[�����̊֐����Ăяo���K�v������B
					���p���Ȃ��ꍇ�͌Ăяo���K�v�͖����B
		*/
		void Sync();
		#endif
		
	private:
		friend class Work;

		struct Impl;
		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom

		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		void BeginSync();
		void EndSync();
		#endif
	};	// end class WorkerThread


	#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
	/*!
		@brief	���������p�I�u�W�F�N�g�B
				���̃I�u�W�F�N�g���������Ă���Ԃ́A�Ăяo�����X���b�h�� WorkerThread::Sync �֐��Œ�~���Ă���B
	*/
	class WorkerThread::Work::SyncObject : public Uncopyable
	{
	public:
		/*!
			@brief	�Ăяo�����X���b�h�Ƃ̓����������J�n����B
					���̃R���X�g���N�^���Ăяo�����ƁA���[�J�[�X���b�h�͈ꎞ��~����B
					�ꎞ��~��A�Ăяo�����X���b�h�� WorkerThread::Sync �֐����Ăяo�����ƁA
					�������ĊJ���A�Ăяo�����X���b�h�͈ꎞ��~����B
		*/
		SyncObject(WorkerThread* pOwner)
			: _pOwner(pOwner)
		{
			if (is_not_null(_pOwner))
			{
				_pOwner->BeginSync();
			}
		}
		
		/*!
			@brief	�Ăяo�����X���b�h�Ƃ̓����������I������B
					���̃f�X�g���N�^���Ăяo�����ƁAWorkerThread::Sync �֐��Œ�~���Ă���
					�Ăяo�����X���b�h�͏������ĊJ����B
		*/
		~SyncObject()
		{
			if (is_not_null(_pOwner))
			{
				_pOwner->EndSync();
			}
		}
		
	private:
		WorkerThread* _pOwner;
	};	// end class WorkerThread::Work::SyncObject
	#endif
	
}	// end namespace Marda
