/*!
	@file	Utilities/WorkerThread.cpp

	@author	E.Nakayama
*/

#include <process.h>

#include "Marda/Utilities/WorkerThread.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// WorkerThread::Work
WorkerThread::Work::Work()
	: _pOwner(NULL)
	, _state(STATE_NONE) 
{
//	_hEndEvent.reset(
//		CreateEvent(
//			NULL,
//			TRUE,	// bManualReset
//			FALSE,	// bInitialState
//			NULL
//			)
//		);
//	if (_hEndEvent.is_invalid())
//	{
//		// TODO:
//	}
}

void
WorkerThread::Work::ChangeState(STATE state)
{
	_state = state;
	OnNotify(state);
}

// ----------------------------------------------------------------------------
// WorkerThread �N���X�̎�����

struct WorkerThread::Impl : private Uncopyable
{
	WorkerThread& _interface;
	
	HANDLE _hThread;
	unsigned int _threadID;
	HANDLE _hPushEvent;	//!< �d����Push�����ꍇ�ɃV�O�i����ԂɂȂ�C�x���g�B
	HANDLE _hCompleteEvent;	//!< WaitForCompletion�p�B
	bool _isCompleted;
	bool _needsToExit;
	
	typedef std::deque<WorkPtr> WorkQueue;
	WorkQueue _workQueue;
	GuardObject _workQueueGuard;
	
	#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
	HANDLE _hBeginSyncEvent;	//!< ���������J�n�p�C�x���g�B
	HANDLE _hEndSyncEvent;	//!< ���������I���p�C�x���g�B
	#endif
	
	//! ctor
	Impl(WorkerThread& iface)
		: _interface(iface)
		, _hThread(NULL)
		, _threadID(0)
		, _hPushEvent(NULL)
		, _hCompleteEvent(NULL)
		, _isCompleted(true)
		, _needsToExit(false)
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		, _hBeginSyncEvent(NULL)
		, _hEndSyncEvent(NULL)
		#endif
	{
	}
	
	//! dtor
	~Impl()
	{
		Release();
	}
	
	//! ���[�J�[�X���b�h���쐬����
	bool Create()
	{
		if (is_null(_hThread)) 
		{
			_hThread = (HANDLE)_beginthreadex(
				NULL,	// lpThreadAttributes
				0,	// dwStackSize	; �f�t�H���g
				&Impl::ThreadProc,	// lpStartAddress
				this,	// lpParameter
				0,	// dwCreationFlags
				&_threadID	// lpThreadId
				);
			if (is_null(_hThread))
			{
				return false;
			}
				
			_hPushEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset	; _workQueue ����ɂȂ������V�O�i����Ԃɖ߂�
				FALSE,	// bInitialState
				NULL
				);
			if (is_null(_hPushEvent))
			{
				return false;
			}

			_hCompleteEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset	; Push �֐��Ŕ�V�O�i����Ԃɖ߂�
				TRUE,	// bInitialState
				NULL
				);
			if (is_null(_hPushEvent))
			{
				return false;
			}

			#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
			_hBeginSyncEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset
				TRUE,	// bInitialState
				NULL
				);
			if (is_null(_hBeginSyncEvent))
			{
				return false;
			}

			_hEndSyncEvent = CreateEvent(
				NULL,
				TRUE,	// bManualReset
				FALSE,	// bInitialState
				NULL
				);
			if (is_null(_hEndSyncEvent))
			{
				return false;
			}
			#endif
		}
		return true;
	}
	
	//! ���[�J�[�X���b�h���������
	void Release()
	{
		_needsToExit = true;
		if (!is_null(_hThread))
		{
			// �S�Ă̎d�����L�����Z������B
			{
				Lock<GuardObject> lock(_workQueueGuard);	// ���b�N
				WorkQueue::iterator itrQueue = _workQueue.begin();
				WorkQueue::iterator itrQueueEnd = _workQueue.end();
				for (; itrQueue != itrQueueEnd; ++itrQueue)
				{
					(*itrQueue)->ChangeState(Work::STATE_CANCELED);
				}
				_workQueue.clear();
			}
			
			WaitForCompletion(INFINITE);	// �d�����n�܂�������Ă���̂̓L�����Z���ł��Ȃ��̂ő҂B

			// �d���҂��̏�ԉ����ׁ̈A�V�O�i����Ԃɂ���B
			SetEvent(_hPushEvent);

			// �X���b�h�I���҂�
//			DWORD ret = 
				WaitForSingleObject(_hThread, INFINITE);
//			if (ret == WAIT_TIMEOUT)
//			{
//				// TODO:
//			}

			safe_close<NULL>(_hThread);
		}
		safe_close<NULL>(_hPushEvent);
		safe_close<NULL>(_hCompleteEvent);
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		safe_close<NULL>(_hBeginSyncEvent);
		safe_close<NULL>(_hEndSyncEvent);
		#endif
	}
	
	//! ���̃��[�J�[�X���b�h�̃X���b�hID���擾����
	DWORD GetThreadID()
	{
		return _threadID;
	}
	
	//! �d����ǉ�����B
	bool Push(WorkPtr pWork)
	{
		Lock<GuardObject> lock(_workQueueGuard);	// ���b�N
		
		pWork->_pOwner = &_interface;
		
		pWork->ChangeState(Work::STATE_QUEUEING);
		_workQueue.push_back(pWork);
		
		ResetEvent(_hCompleteEvent);	// Push��������ɓ��X���b�h����WaitForCompletion���Ăяo���Ă��ǂ��l�ɃR�R�Ŕ�V�O�i����Ԃɂ���
		SetEvent(_hPushEvent);	// �d���҂��̏�ԉ����ׁ̈A�V�O�i����Ԃɂ���
		
		return true;
	}
	
	//! �d���𒆒f����B
	bool Cancel(WorkPtr pWork)
	{
		bool result = false;
		Lock<GuardObject> lock(_workQueueGuard);	// ���b�N
		WorkQueue::iterator itrQueue = _workQueue.begin();
		while (itrQueue != _workQueue.end())
		{
			if (*itrQueue == pWork)
			{
				(*itrQueue)->ChangeState(Work::STATE_CANCELED);
				itrQueue = _workQueue.erase(itrQueue);
				result = true;
			}
			else
			{
				++itrQueue;
			}
		}
		return result;
	}
	
	//! �d�����S�ďI���܂ő҂B
	bool WaitForCompletion(DWORD timeout)
	{
		#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
		DWORD begin = GetTickCount();
		DWORD current;
		for (;;)
		{
			Sync();	// ������������������Ȃ��̂ŌĂяo���B
			
			if (IsSignaled(_hCompleteEvent))
				break;
			
			if (timeout != INFINITE)
			{
				current = GetTickCount();
				if (current < begin)
				{
					// TODO: �V�X�e���N������ 49.7 ���Ԍo�߂��ăJ�E���g�����Z�b�g���ꂽ�B�^�C���A�E�g�ƌ��Ȃ����H
				}
				else
				if (current - begin >= timeout)
				{
					// �^�C���A�E�g�B
					return false;
				}
			}
			
			Sleep(0);	// �X���b�h�R���e�L�X�g�̃X�C�b�`�B
		}
		#else
		DWORD ret = WaitForSingleObject(_hCompleteEvent, timeout);
		if (ret == WAIT_TIMEOUT)
		{
			return false;
		}
		#endif
		return true;
	}
	
	//! �d�����S�ďI��������ǂ�����Ԃ�
	bool IsCompleted()
	{
		// ��V�O�i����ԁi���d�����S�ďI����Ă���j���ǂ������`�F�b�N����
		return IsNonSignaled(_hPushEvent);
		//return WaitForSingleObject(_hPushEvent, 0) != WAIT_OBJECT_0;
	}
	
	//! �X���b�h�v���V�[�W��
	static unsigned int WINAPI ThreadProc(void* lpParameter)
	{
		Impl* pThis = reinterpret_cast<Impl*>(lpParameter);
		for (;;)
		{
			// �d��������܂ő҂�
			WaitForSingleObject(pThis->_hPushEvent, INFINITE);
			
			// �I���v�������Ă��邩�H
			if (pThis->_needsToExit)
			{
				break;
			}
			else
			{
				// �ς�ł���d�������ׂĂ��Ȃ��B
				for (;;)
				{
					// �I���v�������Ă�����ȍ~�̎d���̓L�����Z������B
					if (pThis->_needsToExit)
					{
						break;
					}

					WorkPtr pWork;
					{
						Lock<GuardObject> lock(pThis->_workQueueGuard);	// ���b�N
						if (pThis->_workQueue.empty())
						{
							break;	// �d���Ȃ�
						}
						
						pWork = pThis->_workQueue.front();
						pThis->_workQueue.pop_front();
						pWork->ChangeState(Work::STATE_EXECUTING);
					}
					
					// ���s
					if (pWork->Execute())
					{
						pWork->ChangeState(Work::STATE_COMPLETE);
					}
					else
					{
						pWork->ChangeState(Work::STATE_ERROR);
					}
				}
			}
			ResetEvent(pThis->_hPushEvent);	// ��V�O�i����Ԃɖ߂��Ďd���҂���Ԃɂ���
			SetEvent(pThis->_hCompleteEvent);	// �V�O�i����Ԃɂ���WaitForCompletion�̑ҋ@��Ԃ�����������
		}
		return 0;
	}

	#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
	void Sync()
	{
		// ���������J�n�p�C�x���g����V�O�i����Ԃ��ǂ������`�F�b�N����B
		if (IsNonSignaled(_hBeginSyncEvent))
		{
			ResetEvent(_hEndSyncEvent);	// �I���p�C�x���g���V�O�i����Ԃɂ���B
			SetEvent(_hBeginSyncEvent);	// �J�n�p�C�x���g���V�O�i����Ԃɂ��A���[�J�[�X���b�h���̏������ĊJ������B
			WaitForSingleObject(_hEndSyncEvent, INFINITE);	// �I���p�C�x���g���V�O�i����ԂɂȂ�i��EndSync���Ăяo�����j�̂�҂B
		}
	}
	
	void BeginSync()
	{
		ResetEvent(_hBeginSyncEvent);	// �J�n�p�C�x���g���V�O�i����Ԃɂ���B
		WaitForSingleObject(_hBeginSyncEvent, INFINITE);	// �J�n�p�C�x���g���V�O�i����ԂɂȂ�i��Sync���Ăяo�����j�̂�҂B
	}
	
	void EndSync()
	{
		SetEvent(_hEndSyncEvent);	// �I���p�C�x���g���V�O�i����Ԃɂ���B
	}
	#endif

};	// end struct WorkerThread::Impl

// ----------------------------------------------------------------------------
// WorkerThread �N���X�̓]����

WorkerThread::WorkerThread()
{
	_pimpl.reset(new Impl(*this));
}

WorkerThread::~WorkerThread()
{
}

bool
WorkerThread::Create()
{
	return _pimpl->Create();
}

void
WorkerThread::Release()
{
	_pimpl->Release();
}

DWORD
WorkerThread::GetThreadID() const
{
	return _pimpl->GetThreadID();
}

bool
WorkerThread::Push(WorkPtr pWork)
{
	return _pimpl->Push(pWork);
}

bool
WorkerThread::Cancel(WorkPtr pWork)
{
	return _pimpl->Cancel(pWork);
}

bool
WorkerThread::WaitForCompletion(DWORD timeout)
{
	return _pimpl->WaitForCompletion(timeout);
}

bool
WorkerThread::IsCompleted()
{
	return _pimpl->IsCompleted();
}

#if !defined(DISABLE_WORKER_THREAD_SYNC_EXEC)
void
WorkerThread::Sync()
{
	return _pimpl->Sync();
}

void
WorkerThread::BeginSync()
{
	return _pimpl->BeginSync();
}

void
WorkerThread::EndSync()
{
	return _pimpl->EndSync();
}
#endif
