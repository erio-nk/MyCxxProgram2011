// Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// Misc.h
bool Test_Misc();

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

void ParallelCount()
{
	TLInt tl_val(0);
	++(*tl_val);
}

void ParallelProcess()
{
	Lock<GuardObject> lock(s_guard);
	for (int i = 0; i < 10; ++i)
	{
		TTRACE(_T("print %d"), i);
		ParallelCount();
	}
}

bool Test_Thread()
{
	TTRACE(_T("====================================================="));
	Thread thread1;
	Thread thread2;
	Thread thread3;

	thread1.Begin(std::bind(ParallelProcess));
	thread2.Begin(std::bind(ParallelProcess));
	thread3.Begin(std::bind(ParallelProcess));

	thread1.Wait();
	thread2.Wait();
	thread3.Wait();
	
	thread1.Begin(std::bind(ParallelProcess));
	thread1.Wait();
	
	TLInt::Variables& vars = TLInt::GetVariables();
	TLInt::Variables::iterator itrVars = vars.begin();
	for (int i = 0; itrVars != vars.end(); ++itrVars, ++i)
	{
		TTRACE(_T("vars[%d]=%d"), i, *(*itrVars));
	}

	return true;
}

// ----------------------------------------------------------------------------
// Container.h

bool Test_Container();


// ----------------------------------------------------------------------------
// Utilities/WorkerThread.h

bool Test_Utilities_WorkerThread();


// ----------------------------------------------------------------------------
// Coroutine.h

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

bool Test_Coroutine()
{
	TTRACE(_T("====================================================="));	MyCoroutine1 myCoroutine1;
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
// UnixTime.h
bool Test_UnixTime()
{
	TTRACE(_T("====================================================="));
	{	
		UnixTime d1;
		TTRACE(_T("%s"), d1.ToString());

		UnixTime d2 = d1 - UnixTime::A_HOUR * 0.5f;
		TTRACE(_T("%s"), d2.ToString());
	}

	return true;
}

// ----------------------------------------------------------------------------
// CRC
bool Test_CRC();

// ----------------------------------------------------------------------------
// Math
bool Test_Math();

// ----------------------------------------------------------------------------
// Profiler
bool Test_Profiler();

// ----------------------------------------------------------------------------
bool Test()
{
	if (!Test_Misc()) return false;
	if (!Test_String()) return false;
	if (!Test_File()) return false;
	if (!Test_STL()) return false;
	if (!Test_Thread()) return false;
	if (!Test_Coroutine()) return false;
	
	if (!Test_Utilities_WorkerThread()) return false;

	if (!Test_Timer()) return false;
	if (!Test_UnixTime()) return false;
	if (!Test_CRC()) return false;
	if (!Test_Profiler()) return false;

	if (!Test_Container()) return false;

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
	
	PAUSE;
	
	return 0;
}

