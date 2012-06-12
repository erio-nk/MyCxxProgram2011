#include "stdafx.h"

using namespace Marda;

bool Test_Utilities_WorkerThread_Sync();


bool Test_Utilities_WorkerThread()
{
	TTRACE(_T("====================================================="));

	struct MyWork : public WorkerThread::Work
	{
		int _no;

		MyWork(int no)
			: _no(no)
		{
		}

		~MyWork()
		{
			printf("[%u] destruct no=%d\n", 
				GetCurrentThreadId(), _no);
		}

		ERROR_CODE Execute()
		{
			for (int i = 0; i < 10; ++i)
			{
				printf("[%u] executing no=%d-%d,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n", 
					GetCurrentThreadId(), _no, i);
			}
			return ERR_NOERROR;
		}

		void OnNotify(STATE state)
		{
			//switch (state)
			//{
			//	case STATE_COMPLETE:
			//	case STATE_ERROR:
			//		delete this;
			//		break;
			//}
		}
	};

	WorkerThread worker1;
	if (!worker1.Create())
	{	
		return false;
	}

	WorkerThread worker2;
	if (!worker2.Create())
	{	
		return false;
	}

	int no = 0;
	int i = 0;
	for (; i < 10; ++i, ++no)
	{
		WorkerThread::WorkPtr pWork(new MyWork(no));
		worker1.Push(pWork);
		Sleep(1);
	}
	TTRACE(_T("%d works pushed to %d."), i, worker1.GetThreadID());

	Sleep(100);

	i = 0;
	for (; i < 10; ++i, ++no)
	{
		WorkerThread::WorkPtr pWork(new MyWork(no));
		worker1.Push(pWork);
	}
	TTRACE(_T("%d works pushed to %d."), i, worker1.GetThreadID());

	i = 0;
	for (; i < 10; ++i, ++no)
	{
		WorkerThread::WorkPtr pWork(new MyWork(no));
		worker2.Push(pWork);
		Sleep(1);
	}
	printf("%d works pushed to %d.\n", i, worker2.GetThreadID());


	TTRACE(_T("worker1.WaitForCompletion() calling..."));
	worker1.WaitForCompletion();
	TTRACE(_T("worker1.WaitForCompletion() called"));

	i = 0;
	for (; i < 10; ++i, ++no)
	{
		WorkerThread::WorkPtr pWork(new MyWork(no));
		worker1.Push(pWork);
	}
	TTRACE(_T("%d works pushed to %d."), i, worker1.GetThreadID());

	//Sleep(100);

	//TTRACE(_T("worker2.WaitForCompletion() calling..."));
	//worker2.WaitForCompletion();
	//TTRACE(_T("worker2.WaitForCompletion() called"));

	//TTRACE(_T("worker1.WaitForCompletion() calling..."));
	//worker1.WaitForCompletion();
	//TTRACE(_T("worker1.WaitForCompletion() called"));

	TTRACE(_T("worker1.Release() calling..."));
	worker1.Release();
	TTRACE(_T("worker1.Release() called"));

	TTRACE(_T("worker2.Release() calling..."));
	worker2.Release();
	TTRACE(_T("worker2.Release() called"));

	if (!Test_Utilities_WorkerThread_Sync()) return false;

	return true;
}

bool Test_Utilities_WorkerThread_Sync()
{
	TTRACE(_T("-----------------------------------------------------"));
	TTRACE(_T("Sync test"));

	struct MyWork : public SynchronizableWorkerThread::Work
	{
		MyWork()
		{
		}

		ERROR_CODE Execute()
		{
			int i = 0;
			for (; i < 10; ++i)
			{
				printf("[%u:%p] i=%d\n", GetCurrentThreadId(), this, i);
			}

			{
				SyncObject sync(this);
				printf("[%u:%p] begin sync\n", GetCurrentThreadId(), this);
				//Sleep(3*1000);
				Sleep(100);
				printf("[%u:%p] end sync\n", GetCurrentThreadId(), this);
			}

			if (NeedsToCancel())
				return ERR_CANCELED;

			for (; i < 20; ++i)
			{
				printf("[%u:%p] i=%d\n", GetCurrentThreadId(), this, i);
			}

			return ERR_NOERROR;
		}

		void OnNotify(STATE state)
		{
			if (state == STATE_CANCELED)
			{
				printf("[%u:%p] Canceled.\n", GetCurrentThreadId(), this);				
			}
		}
	};

	SynchronizableWorkerThread worker;

	worker.Create();

	WorkerThread::WorkPtr work1(new MyWork());
	worker.Push(work1);

	WorkerThread::WorkPtr work2(new MyWork());
	worker.Push(work2);

	WorkerThread::WorkPtr work3(new MyWork());
	worker.Push(work3);

	int frames = 0;
	while (!worker.IsCompleted() && frames < 4)
	{
		printf("[%u] frame %d ---------------\n", GetCurrentThreadId(), frames);
		worker.Sync();

		//worker.WaitForCompletion(work1, INIFINITE);	// TODO: ŽÀ‘•‚·‚éB

		//worker.Cancel(work2);
		//worker.Cancel(work3);

		Sleep(0);
		++frames;
	}

	printf("[%u] WaitForCompletion ...\n", GetCurrentThreadId(), frames);	
	worker.WaitForCompletion();
	printf("[%u] WaitForCompletion!\n", GetCurrentThreadId(), frames);	

	printf("[%u] Release ...\n", GetCurrentThreadId(), frames);	
	worker.Release();
	printf("[%u] Release!\n", GetCurrentThreadId(), frames);	

	return true;
}
