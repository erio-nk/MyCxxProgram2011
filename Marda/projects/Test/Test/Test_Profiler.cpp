#include "stdafx.h"

using namespace Marda;

typedef Profiler<ThreadUnsafe> ThreadUnsafeProfiler;
bool Test_Profiler_ThreadUnsafe()
{
	TTRACE(_T("-----------------------------------------------------"));
	
	for (int i = 0; i < 5; ++i) 
	{
		ThreadUnsafeProfiler prof(TFUNCTION _T("#Hoge"));
		Sleep(100);

		for (int k = 0; k < 3; ++k)
		{
			ThreadUnsafeProfiler prof(TFUNCTION _T("#Fuga"));
			Sleep(100);

			{
				ThreadUnsafeProfiler prof(TFUNCTION _T("#Piyo"));
				Sleep(100);
			}
		}

		ThreadUnsafeProfiler::ChangeFrame();
	}
	
	const ThreadUnsafeProfiler::Reports& reports = ThreadUnsafeProfiler::GetReports();
	const ProfileReport* pReport;
	for (size_t i = 0; (pReport = reports.EnumReports(i)) != NULL; ++i)
	{
		TTRACE(_T("count=%6llu, time=%6.2fsec , sub=%6.2fsec @ %s"), 
			pReport->_count,
			PerformanceCounter::ToSecondsAsFloat(pReport->_time),
			PerformanceCounter::ToSecondsAsFloat(pReport->_sub),
			pReport->_tag
			);
	}

	return true;
}

typedef Profiler<ThreadSafe> ThreadSafeProfiler;

void ThreadFunc()
{
	ThreadSafeProfiler prof(TFUNCTION _T("#Hoge"));
	Sleep(500);

	{
		ThreadSafeProfiler prof(TFUNCTION _T("#Fuga"));
		Sleep(250);
	}
}

bool Test_Profiler_ThreadSafe()
{
	TTRACE(_T("-----------------------------------------------------"));
	
	Thread thread1;
	Thread thread2;

	for (int i = 0; i < 5; ++i) {
		thread1.Begin(std::bind(ThreadFunc));
		thread2.Begin(std::bind(ThreadFunc));

		thread1.Wait();
		thread2.Wait();

		ThreadSafeProfiler::ChangeFrame();
	}
	
	const ThreadSafeProfiler::Reports& reports = ThreadSafeProfiler::GetReports();
	const ProfileReport* pReport;
	for (size_t i = 0; (pReport = reports.EnumReports(i)) != NULL; ++i)
	{
		TTRACE(_T("count=%6llu, time=%6.2fsec , sub=%6.2fsec @ %s"), 
			pReport->_count,
			PerformanceCounter::ToSecondsAsFloat(pReport->_time),
			PerformanceCounter::ToSecondsAsFloat(pReport->_sub),
			pReport->_tag
			);
	}

	return true;
}

bool Test_Profiler()
{
	TTRACE(_T("====================================================="));
	if (!Test_Profiler_ThreadUnsafe()) return false;
	if (!Test_Profiler_ThreadSafe()) return false;

	return true;
}
