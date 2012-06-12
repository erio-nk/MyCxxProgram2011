/*!
	@file	Profiler.h
	
			プロファイラ。

	@author	E.Nakayama
*/
#pragma once

#include "Types.h"
#include "STLHelpers.h"
#include "Thread.h"

namespace Marda
{
	template <class ThreadingPolicy, class Tag>
	class ProfileReports;

	struct ProfilerBase
	{
		const TCHAR* _tag;
		LARGE_INTEGER _begin;
		LARGE_INTEGER _sub;
		ProfilerBase* _pParent;

		ProfilerBase(const TCHAR* tag)
			: _tag(tag)
		{
		}
		// virtual ~ProfilerBase() {}	// ProfilerBase* に Profiler を入れて delete する事は無いので不要。
	};
	
	struct ProfilerTagDefault {};
	
	/*!
		@brief	プロファイラクラステンプレート。
				このオブジェクトを生成してから破棄までにかかった時間を計測する。
		@tparam	ThreadingPolicy	計測処理のスレッディングポリシー。
								ThreadSafe を指定しても、GetReports 関数が返却するリストへのアクセスは
								プロファイルとも排他的に行わなければならない。
		@tparam	Tag				タグ。別のレポートとしてプロファイルを取りたい場合は異なるタグクラスを指定する。

		@code
		typedef Profiler<ThreadUnsafe> MyProfiler;
		
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);

		// プロファイルを取る。
		for (int i = 0; i < 5; ++i) 
		{
			{
				MyProfiler prof(TFUNCTION _T("#Hoge"));
				Sleep(500);

				{
					MyProfiler prof(TFUNCTION _T("#Fuga"));
					Sleep(250);
				}
			}
			MyProfiler::ChangeFrame();
		}
		
		// レポートの表示。
		const MyProfiler::Reports& reports = MyProfiler::GetReports();
		const ProfileReport* pReport;
		for (size_t i = 0; (pReport = reports.EnumReports(i)) != NULL; ++i)
		{
			TTRACE(_T("count=%10llu, time=%10.2fsec @ %s"), 
				pReport->_count,
				PerformanceCounterToSeconds(pReport->_time, freq.QuadPart),
				pReport->_tag
				);
		}
		@endcode
	*/
	template <class ThreadingPolicy, class Tag = ProfilerTagDefault>
	class Profiler : public ProfilerBase
	{
	public:
		typedef ProfileReports<ThreadingPolicy, Tag> Reports;
		
		/*!
			@brief	コンストラクタ。
					計測を開始する。
		*/
		Profiler(const TCHAR* tag)
			: ProfilerBase(tag)
		{
			_sub.QuadPart = 0;
			_pParent = s_reports.SetCurrent(this);
			PerformanceCounter::GetCurrentCount(_begin);
		}
		
		/*!
			@brief	デストラクタ。
					計測を終了する。
		*/
		~Profiler()
		{
			LARGE_INTEGER end;
			PerformanceCounter::GetCurrentCount(end);
			
			LARGE_INTEGER time;
			time.QuadPart = end.QuadPart - _begin.QuadPart;

			s_reports.Report(_tag, time, _sub);

			_pParent->_sub.QuadPart += time.QuadPart;
			s_reports.SetCurrent(_pParent);
		}
		
		/*!
			@brief	フレームの切り替わりを通知する。
		*/
		static void ChangeFrame()
		{
			++s_frames;
		};
		
		/*!
			@brief	レポートをリセットする。
		*/
		static void ResetReports()
		{
			s_reports.Reset();
		}
		
		/*!
			@brief	フレーム数を取得する。
		*/
		static unsigned long long GetFrames()
		{
			return s_frames;
		}
		
		/*!
			@brief	レポートを取得する。
		*/
		static const Reports& GetReports()
		{
			return s_reports;
		}
		
	private:
		static Reports s_reports;
		static unsigned long long s_frames;
	};	// end class Profiler
	
	template <class ThreadingPolicy, class Tag>
	typename Profiler<ThreadingPolicy, Tag>::Reports 
	Profiler<ThreadingPolicy, Tag>::s_reports;
	
	template <class ThreadingPolicy, class Tag>
	unsigned long long 
	Profiler<ThreadingPolicy, Tag>::s_frames = 0;
	
	
	/*!
		@brief	１タグに関するプロファイルレポート。
	*/
	struct ProfileReport
	{
		const TCHAR*		_tag;
		LARGE_INTEGER			_time;	//!< 時間（パフォーマンスカウンタ値）。
		LARGE_INTEGER			_sub;	//!< サブルーチンコールにかかった時間（パフォーマンスカウンタ値）。
		unsigned long long 	_count;	//!< 呼び出し回数。
	};	// end struct ProfileReport
	
	typedef std::map<const TCHAR*, ProfileReport> ProfileReportMap;
	typedef map_random_accessor<ProfileReportMap> ProfileReportMapRandomAccessor;
	
	/*!
		@brief	スレッド単位のプロファイルレポート。
	*/
	class ProfileReportsPerThread
	{
	public:
		ProfileReportMap _reports;	//!< スレッド単位のレポート。

		ProfileReportsPerThread()
			: _root(_T("<root>"))
			, _pCurrent(&_root)
		{
		}

		ProfilerBase* SetCurrent(ProfilerBase* p)
		{
			std::swap(_pCurrent, p);
			return p;
		}
		
		void Report(const TCHAR* tag, const LARGE_INTEGER& time, const LARGE_INTEGER& sub)
		{
			ProfileReportMap::iterator found = _reports.find(tag);
			if (found != _reports.end())
			{
				ProfileReport& r = found->second;
				r._time.QuadPart += time.QuadPart;
				r._sub.QuadPart += sub.QuadPart;
				++r._count;
			}
			else
			{
				ProfileReport r;
				r._tag = tag;
				r._time = time;
				r._sub = sub;
				r._count = 1;
				_reports.insert(std::make_pair(tag, r));
			}
		}
		
		void Reset()
		{
			_reports.clear();
		}
		
		size_t GetNumReports() const
		{
			return _reports.size();
		}
		
	private:
		ProfilerBase _root;
		ProfilerBase* _pCurrent;
	};	// end class ProfileReportsPerThread
	
	
	/*!
		@brief	プロファイルレポート保持クラステンプレート（シングルスレッド用）。
	*/
	template <class Tag>
	class ProfileReports<ThreadUnsafe, Tag>
	{
	public:
		ProfileReports()
		{
			_reportMapAccessor.reset(_profilerPerThread._reports);
		}
		
		//! 計測結果をレポートする。
		void Report(const TCHAR* tag, const LARGE_INTEGER& time, const LARGE_INTEGER& sub)
		{
			_profilerPerThread.Report(tag, time, sub);
			_reportMapAccessor.reset(_profilerPerThread._reports);
		}
		
		//! 計測結果全てリセットする。
		void Reset()
		{
			_profilerPerThread.Reset();
			_reportMapAccessor.reset(_profilerPerThread._reports);
		}
		
		//! レポートを列挙する。
		ProfileReport* EnumReports(size_t index)
		{
			if (index >= _profilerPerThread.GetNumReports())
			{
				return NULL;
			}
			
			return &(_reportMapAccessor.at(index).second);
		}
		
		//! レポートを列挙する。
		const ProfileReport* EnumReports(size_t index) const
		{
			return const_cast<ProfileReports*>(this)->EnumReports(index);
		}
		
	private:
		friend class Profiler<ThreadUnsafe, Tag>;
		ProfileReportsPerThread _profilerPerThread;
		ProfileReportMapRandomAccessor _reportMapAccessor;

		ProfilerBase* SetCurrent(ProfilerBase* p)
		{
			return _profilerPerThread.SetCurrent(p);
		}
	};	// end class ProfileReports<ThreadUnsafe, Tag>
	
	
	/*!
		@brief	プロファイルレポート保持クラスクラステンプレート（マルチスレッド対応版）。
	*/
	template <class Tag>
	class ProfileReports<ThreadSafe, Tag>
	{
	public:
		typedef ThreadLocalVariable<ProfileReportsPerThread, Tag> TLProfileReportsPerThread;
		
		ProfileReports()
		{
			_tmpAccessor.reset(_tmp);
		}
		
		//! 計測結果をレポートする。
		void Report(const TCHAR* tag, const LARGE_INTEGER& time, const LARGE_INTEGER& sub)
		{
			TLProfileReportsPerThread tlProfileReportsPerThread;
			tlProfileReportsPerThread->Report(tag, time, sub);
			_isChanged = true;
		}
		
		//! 計測結果全てリセットする。
		void Reset()
		{
			TLProfileReportsPerThread::Variables& vars = TLProfileReportsPerThread::GetVariables();
			TLProfileReportsPerThread::Variables::iterator itrVars = vars.begin();
			for (; itrVars != vars.end(); ++itrVars)
			{
				(*itrVars)->Reset();
			}
			_isChanged = true;
		}
		
		//! レポートを列挙する。
		ProfileReport* EnumReports(size_t index)
		{
			if (_isChanged)
			{
				// 一時変数にまとめておく。
				TLProfileReportsPerThread::Variables& vars = TLProfileReportsPerThread::GetVariables();
				TLProfileReportsPerThread::Variables::iterator itrVars = vars.begin();
				
				_tmp.clear();
				for (; itrVars != vars.end(); ++itrVars)
				{
					ProfileReportMap::iterator itr = (*itrVars)->_reports.begin();
					ProfileReportMap::iterator itrEnd = (*itrVars)->_reports.end();
					for (; itr != itrEnd; ++itr)
					{
						ProfileReportMap::iterator found = _tmp.find(itr->second._tag);
						if (found != _tmp.end())
						{
							ProfileReport& r = found->second;
							r._time.QuadPart += itr->second._time.QuadPart;
							r._count += itr->second._count;
							r._sub.QuadPart += itr->second._sub.QuadPart;
						}
						else
						{
							ProfileReport r;
							r._tag = itr->second._tag;
							r._time = itr->second._time;
							r._count = itr->second._count;
							r._sub = itr->second._sub;
							_tmp.insert(std::make_pair(r._tag, r));
						}
					}
				}

				_tmpAccessor.reset(_tmp);
				_isChanged = false;
			}
			
			if (index >= _tmp.size())
			{
				return NULL;
			}
			
			return &(_tmpAccessor.at(index).second);
		}
		
		//! レポートを列挙する。
		const ProfileReport* EnumReports(size_t index) const
		{
			return const_cast<ProfileReports*>(this)->EnumReports(index);
		}
	private:
		friend class Profiler<ThreadSafe, Tag>;

		// Enum 用。
		bool _isChanged;
		ProfileReportMap _tmp;
		ProfileReportMapRandomAccessor _tmpAccessor;

		ProfilerBase* SetCurrent(ProfilerBase* p)
		{
			TLProfileReportsPerThread tlProfileReportsPerThread;
			return tlProfileReportsPerThread->SetCurrent(p);
		}
	};	// end class ProfileReports<ThreadSafe, Tag>

}	// end namespace Marda
