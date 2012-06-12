// Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// String.h
bool Test_String()
{
	TTRACE(_T("====================================================="));
	// StrRCSpn テスト。
	{
		const TCHAR* strCharSet = _T("abc");
		int ret;
		
		const TCHAR* str1 = _T("nakayama");
		ret = StrRCSpn(str1, strCharSet);
		if (ret != 7)
		{
			TTRACE(_T("StrRCSpn# failed! ret=%d"), ret);
			return false;
		}

		const TCHAR* str2 = _T("erio");
		ret = StrRCSpn(str2, strCharSet);
		if (ret != -1)
		{
			TTRACE(_T("StrRCSpn# failed! ret=%d"), ret);
			return false;
		}

		const TCHAR* str3 = _T("nakayama erio");
		ret = StrRCSpn(str3, strCharSet);
		if (ret != 7)
		{
			TTRACE(_T("StrRCSpn# failed! ret=%d"), ret);
			return false;
		}
	}
	
	// StrRStr テスト。
	{
		const TCHAR* result = NULL;

		const TCHAR* str1 = _T("Romancing SaGa, SaGa Frontier, Unlimited SaGa");
		result = StrRStr(str1, _T("SaGa"));
		if (result == NULL || _tcscmp(_T("SaGa"), result) != 0)
		{
			TTRACE(_T("StrRStr# failed! result=%s"), result);
			return false;
		}
		
		const TCHAR* str2 = _T("Romancing SaGa, SaGa Frontier, Unlimited SaGa, Romancing SaGa -Minstrel Song-");
		result = StrRStr(str2, _T("SaGa"));
		if (result == NULL || _tcscmp(_T("SaGa -Minstrel Song-"), result) != 0)
		{
			TTRACE(_T("StrRStr# failed! result=%s"), result);
			return false;
		}

		result = StrRStr(str2, _T("Itoken"));
		if (result != NULL)
		{
			TTRACE(_T("StrRStr# failed! result=%s"), result);
			return false;
		}
	}

	// Tokens テスト。
	{
		Tokens<TCHAR> tokens;
		tokens.Set(_T("I love \"Romancing SaGa\" (Super Famicom software)"), _T(" "), _T("\"\"()"));
		TString token;
		if (!tokens.Next(token) || token.compare(_T("I")) != 0)
		{
			TTRACE(_T("Tokens# failed! result=%s"), token.c_str());
			return false;
		}

		if (!tokens.Next(token) || token.compare(_T("love")) != 0)
		{
			TTRACE(_T("Tokens# failed! result=%s"), token.c_str());
			return false;
		}

		if (!tokens.Next(token) || token.compare(_T("Romancing SaGa")) != 0)
		{
			TTRACE(_T("Tokens# failed! result=%s"), token.c_str());
			return false;
		}

		if (!tokens.Next(token) || token.compare(_T("Super Famicom software")) != 0)
		{
			TTRACE(_T("Tokens# failed! result=%s"), token.c_str());
			return false;
		}

		if (tokens.Next(token))
		{
			TTRACE(_T("Tokens# failed!"));
			return false;
		}

		if (tokens.Next(token))
		{
			TTRACE(_T("Tokens# failed!"));
			return false;
		}
	}

	return true;
}

// ----------------------------------------------------------------------------
// File.h
bool Test_File()
{
	TTRACE(_T("====================================================="));
	const TCHAR* path = _T("hoge\\fuga\\piyo.txt");
	const TCHAR* answer = _T("hoge\\fuga\\");
	
	TString path1;
	path1 = path;
	TString ret1 = GetDirectoryPathAsString(path1);
	if (ret1.compare(answer) != 0)
	{
		TTRACE(_T("GetDirectoryPathAsString# test failed! ret=%s"), ret1.c_str());
		return false;
	}
	
	TCHAR ret2[256];
	GetDirectoryPath(path, ret2, _countof(ret2));
	if (_tcscmp(answer, ret2) != 0)
	{
		TTRACE(_T("GetDirectoryPath# test failed! ret=%s"), ret2);
		return false;
	}

	return true;
}


// ----------------------------------------------------------------------------
// STL.h

struct Printer
{
	void operator ()(char c) const
	{
		printf("%c\n", c);
	}
};

void Print(char c)
{
	printf("%c\n", c);
}

bool Test_STL()
{
	TTRACE(_T("====================================================="));
	typedef std::map<int, char> MyMap;
	MyMap map;
	map.insert(std::make_pair(1, 'A'));
	map.insert(std::make_pair(2, 'B'));
	std::for_each(map.begin(), map.end(), pair_second(Printer()));
	std::for_each(map.begin(), map.end(), pair_second(&Print));

	return true;
}

// ----------------------------------------------------------------------------
// Thread.h

namespace
{
	static GuardObject s_guard;
}

typedef ThreadLocalVariable<int> TLInt;

void ParallelPrint()
{
	TLInt tl_val(0);

	Lock<GuardObject> lock(s_guard);
	for (int i = 0; i < 50; ++i, ++(*tl_val))
	{
		TTRACE(_T("print %d %d"), i, *tl_val);
	}
}

bool Test_Thread()
{
	TTRACE(_T("====================================================="));
	Thread thread1;
	Thread thread2;
	Thread thread3;

	thread1.Begin(std::bind(ParallelPrint));
	thread2.Begin(std::bind(ParallelPrint));
	thread3.Begin(std::bind(ParallelPrint));

	thread1.Wait();
	thread2.Wait();
	thread3.Wait();

	return true;
}

// ----------------------------------------------------------------------------
// Utilities/Container.h

bool Test_Utilities_Container();


// ----------------------------------------------------------------------------
// Utilities/WorkerThread.h

struct MyWork : public WorkerThread::Work
{
	int _no;

	MyWork(int no)
		: _no(no)
	{
	}

	bool Execute()
	{
		for (int i = 0; i < 10; ++i)
		{
			printf("[%u] executing no=%d-%d,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n", 
				GetCurrentThreadId(), _no, i);
		}
		return true;
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

bool Test_Utilities_WorkerThread()
{
	TTRACE(_T("====================================================="));
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

	Sleep(100);

	TTRACE(_T("worker2.WaitForCompletion() calling..."));
	worker2.WaitForCompletion();
	TTRACE(_T("worker2.WaitForCompletion() called"));

	TTRACE(_T("worker1.WaitForCompletion() calling..."));
	worker1.WaitForCompletion();
	TTRACE(_T("worker1.WaitForCompletion() called"));

	TTRACE(_T("worker1.Release()"));
	worker1.Release();

	TTRACE(_T("worker2.Release()"));
	worker2.Release();

	return true;
}


// ----------------------------------------------------------------------------
// Utilities/Coroutine.h

struct MyCoroutine1 : public Coroutine
{
	int Run()
	{
		for (int i = 0; i < 3; ++i)
		{
			TTRACE(_T("%d"), i);
			Yeild(i);
		}
		return -1;
	}
};

struct MyCoroutine2 : public Coroutine
{
	int Run()
	{
		for (int i = 0; i < 3; ++i)
		{
			TTRACE(_T("%d"), i);
			Yeild(i);
		}
		return -1;
	}
};

struct MyCoroutine3 : public Coroutine
{
	MyCoroutine1 _nested;
	int Run()
	{
		for (int i = 0; i < 3; ++i)
		{
			TTRACE(_T("%d"), i);
			_nested();
			Yeild(i);
		}
		return -1;
	}
};

bool Test_Utilities_Coroutine()
{
	MyCoroutine1 myCoroutine1;
	MyCoroutine2 myCoroutine2;
	MyCoroutine3 myCoroutine3;

	for (int i = 0; i < 5; ++i)
	{
		int ret;

		ret = myCoroutine1();
		TTRACE(_T("ret = %d"), ret);

		ret = myCoroutine2();
		TTRACE(_T("ret = %d"), ret);

		ret = myCoroutine3();
		TTRACE(_T("ret = %d"), ret);
	}

	return true;
}

// ----------------------------------------------------------------------------
// Timer.h
bool Test_Timer()
{
	TTRACE(_T("====================================================="));
	{	
		Timer t;
		t.Start();
		Sleep(500);
		TTRACE(_T("time=%f, elapsed=%f"), t.GetTime(), t.GetElapsedTime());
		t.Stop();
		Sleep(500);
		TTRACE(_T("time=%f, elapsed=%f"), t.GetTime(), t.GetElapsedTime());
		t.Start();
		Sleep(500);
		TTRACE(_T("time=%f, elapsed=%f"), t.GetTime(), t.GetElapsedTime());
	}

	return true;
}

// ----------------------------------------------------------------------------
// Date.h
bool Test_Date()
{
	TTRACE(_T("====================================================="));
	{	
		Date d1;
		TTRACE(_T("%s"), d1.ToString());

		Date d2 = d1 - Date::A_HOUR * 0.5f;
		TTRACE(_T("%s"), d2.ToString());
	}

	return true;
}

// ----------------------------------------------------------------------------
// Math
bool Test_Math();


// ----------------------------------------------------------------------------
bool Test()
{
	if (!Test_String()) return false;
	if (!Test_File()) return false;
	if (!Test_STL()) return false;
	if (!Test_Thread()) return false;
	
	if (!Test_Utilities_Container()) return false;
	if (!Test_Utilities_WorkerThread()) return false;
	if (!Test_Utilities_Coroutine()) return false;

	//if (!Test_Timer()) return false;
	if (!Test_Date()) return false;

	if (!Test_Math()) return false;

	return true;
}

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	if (Test()) 
	{
		TTRACE(_T("Test OK"));
	}
	else
	{
		TTRACE(_T("Test NG"));
	}
	getchar();	// 止めたいだけ
	//_tsystem(_T("pause"));	// こっちだとデバッガから起動して、停止してもプログラムが閉じない。

	return 0;
}

