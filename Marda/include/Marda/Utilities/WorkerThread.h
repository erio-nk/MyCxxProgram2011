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
	*/
	class WorkerThread : private Uncopyable
	{
	public:
		struct Impl;

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
				STATE_QUEUEING,		//!< ���s�҂��B
				STATE_EXECUTING,	//!< ���s���B
				STATE_COMPLETE,		//!< ���s�����B
				STATE_CANCELED,		//!< ���f���ꂽ�B
				STATE_ERROR,		//!< �G���[�I�������B
			};
			
			Work();

			virtual ~Work()
			{
			}
			
			WorkerThread::Impl* GetOwner() const { return _pOwner; }
			
			STATE GetState() const { return _state; }
			
			/*!
				@brief	���s�����B
						�T�u�N���X�́A���̊֐���K���I�[�o�[���C�h���Ȃ���΂Ȃ�Ȃ��B
				@return	�G���[�R�[�h(ERROR_CODE)�B 
						ERR_NOERROR ��Ԃ����ꍇ�́ASTATE_COMPLETE �ɂȂ�B
						ERR_CANCELD ��Ԃ����ꍇ�́ASTATE_CANCELD �ɂȂ�B
						ERR_UNKNOWN �܂��͏�L�ȊO�̃G���[�R�[�h��Ԃ����ꍇ�́ASTATE_ERROR �ɂȂ�B
			*/
			virtual ERROR_CODE Execute() = 0;
			
			/*!
				@brief	��ԕύX�ʒm�C�x���g�B
						��Ԃ��ς��x�ɌĂяo�����C�x���g�n���h���B
						�T�u�N���X�́A�K�v�ł���΂��̃n���h�����I�[�o�[���C�h����
						�K�؂ɏ������鎖���o����B
				@param	state	�ύX��̏��
			*/
			virtual void OnNotify(STATE state) { UNREFERENCED_PARAMETER(state); }

		protected:
			/*!
				@brief	�������f�v�������邩�ǂ�����Ԃ��B
			*/
			bool NeedsToCancel() const { return _needsToCancel; }

		private:
			friend class WorkerThread;
			WorkerThread::Impl* _pOwner;
			STATE _state;
			AUTO_EVENT_HANDLE _hEndEvent;
			bool _needsToCancel;
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
			@brief	�w��̎d���𒆒f����B
					���f�������d�������s���������ꍇ�́A���̎d�����I���܂Ńu���b�N�����B
					�e Work �T�u�N���X�́ANeedsToCancel �֐��̖߂�l�𒲂ׂāA�������f�v�����o�Ă��邩�ǂ����𔻕ʏo����B
			@param	pWork	���f����d���I�u�W�F�N�g�B
			@code
			struct MyWork : public WorkerThread::Work
			{
				int Execute()
				{
					... ��������1 ...

					if (NeedsToCancel())
						return ERR_CANCELD;	// ���f�v�����������̂ňُ�I���B

					... ��������2 ...

					if (NeedsToCancel())
						return ERR_CANCELD;	// ���f�v�����������̂ňُ�I���B

					... ��������3 ...
					
					return ERR_NOERROR;	// ����I���B
				}
			};
			@endcode
		*/
		bool Cancel(WorkPtr pWork);
		
		/*!
			@brief	�w��̎d�����I���܂ő҂B
			@param	pWork	�d���̏I����҂������d���I�u�W�F�N�g�B
			@param	timeout	�^�C���A�E�g����~���b�B
			@return	�d�����S�ďI������ꍇ��true�A�^�C���A�E�g�����ꍇ��false��Ԃ��B
		*/
		bool WaitForCompletion(WorkPtr pWork, DWORD timeout = INFINITE);
		
		/*!
			@brief	�d�����S�ďI���܂ő҂B
			@param	timeout	�^�C���A�E�g����~���b�B
			@return	�d�����S�ďI������ꍇ��true�A�^�C���A�E�g�����ꍇ��false��Ԃ��B
		*/
		bool WaitForCompletion(DWORD timeout = INFINITE);
		
		/*!
			@brief	�d�����S�ďI��������ǂ�����Ԃ�
			@attention	���̊֐��̎��s�ɂ͍Œ�1ms��v����B
		*/
		bool IsCompleted();
		
		/*!
			@brief	�X���b�h�N�����ɌĂяo�����C�x���g�B
		*/
		virtual bool OnBeginThread() { return true; }
		
		/*!
			@brief	�X���b�h�I�����ɌĂяo�����C�x���g�B
		*/
		virtual void OnExitThread() {}
		
	protected:
		friend class Work;

		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom
		
		WorkerThread(Impl* pimpl);

	};	// end class WorkerThread


	/*!
		@brief	�����\���[�J�[�X���b�h�N���X�B
				�Ăяo�����X���b�h�Ŗ��t���[�� Sync �֐����Ăяo���l�ɂ��AWork::Execute �֐�����
				Work::SyncObject �𐶐����鎖�ŁA�Ăяo�����X���b�h�Ƃ̓����������s�������o����B
		@code
		SynchronizableWorkerThread worker;
		
		struct MyWork : public SynchronizableWorkerThread::Work
		{
			int Execute()
			{
				// �񓯊������B
				int i = 0;
				for (; i < 100; ++i)
					printf("%d\n", i);
					
				// ���C���X���b�h�Ƃ̓��������B���̊Ԃ̓��C���X���b�h��(Sync�֐���)��~���Ă���B
				{
					SyncObject sync(this);
					
					// sync �̔j���Ɠ����ɓ����I���B
				}
				
				// �Ăє񓯊������B
				for (; i < 200; ++i)
					printf("%d\n", i);
				
				return ERR_NOERROR;
			}
		};
		
		worker.Create();
		
		WorkerThread::WorkPtr work(new MyWork());
		worker.Push(work1);
		
		// ���C�����[�v�B
		for (!worker.IsCompleted())
		{
			g_worker.Sync();
			Sleep(0);
		}
		@endcode
	*/
	class SynchronizableWorkerThread : public WorkerThread
	{
	private:
		struct Impl;
	public:
		class Work : public WorkerThread::Work
		{
		protected:
			SynchronizableWorkerThread::Impl* GetOwner();

			/*!
				@brief	���������p�I�u�W�F�N�g�B
						���̃I�u�W�F�N�g���������Ă���Ԃ́A�Ăяo�����X���b�h�� SynchronizableWorkerThread::Sync �֐��Œ�~���Ă���B
			*/
			class SyncObject
			{
			public:
				/*!
					@brief	�Ăяo�����X���b�h�Ƃ̓����������J�n����B
							���̃R���X�g���N�^���Ăяo�����ƁA���[�J�[�X���b�h�͈ꎞ��~����B
							�ꎞ��~��A�Ăяo�����X���b�h�� SynchronizableWorkerThread::Sync �֐����Ăяo�����ƁA
							�������ĊJ���A�Ăяo�����X���b�h�͈ꎞ��~����B
				*/
				SyncObject(SynchronizableWorkerThread::Work* pWork);
				
				/*!
					@brief	�Ăяo�����X���b�h�Ƃ̓����������I������B
							���̃f�X�g���N�^���Ăяo�����ƁASynchronizableWorkerThread::Sync �֐��Œ�~���Ă���
							�Ăяo�����X���b�h�͏������ĊJ����B
				*/
				~SyncObject();
				
			private:
				SynchronizableWorkerThread::Work* _pWork;
			};	// end class SyncObject
			
		};	// end class Work

		SynchronizableWorkerThread();

		~SynchronizableWorkerThread();

		/*!
			@brief	���[�J�[�X���b�h�Ƃ̓����������s���B
					���[�J�[�X���b�h���������������J�n���Ă��Ȃ��ꍇ�́A�����ɐ��䂪�߂�B
					���[�J�[�X���b�h���������������J�n���Ă���ꍇ�́A�ꎞ��~���Ă��郏�[�J�[�X���b�h���ĊJ���A
					�����������I������܂ŌĂяo�����X���b�h�ɐ����Ԃ��Ȃ��B
					Work::Execute ���� Work::SyncObject �𗘗p���鎖������ꍇ�́A���t���[�����̊֐����Ăяo���K�v������B
					���p���Ȃ��ꍇ�͌Ăяo���K�v�͖����B
		*/
		void Sync();
		
	private:
		friend class Work;
	};	// end class SynchronizableWorkerThread

}	// end namespace Marda
