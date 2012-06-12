/*!
	@file	PerformanceCounter.h

	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	//! 高分解能パフォーマンスカウンタが存在しない事を示す例外クラス。
	struct QueryPerformanceCounterNotSupportedException
		: std::exception
	{};
	
	/*!
		@brief	パフォーマンスカウンタ用ヘルパ関数群。
	*/
	class PerformanceCounter
	{
	public:
		/*!
			@brief	パフォーマンスカウンタの周波数（＝１秒間にカウントアップされる値）を取得する。
		*/
		static void GetFrequency(LARGE_INTEGER& out)
		{
			out = StaticMembers::Get()._frequency;
		}
		
		/*!
			@brief	現在のパフォーマンスカウンタ値を取得する。
		*/
		static void GetCurrentCount(LARGE_INTEGER& out)
		{
			QueryPerformanceCounter(&out);
//			if (failed(QueryPerformanceCounter(&out)))
//			{
//				throw QueryPerformanceCounterNotSupportedException();
//			}
		}
		
		/*!
			@brief	パフォーマンスカウンタ値を秒に変換する。
		*/
		static float ToSecondsAsFloat(const LARGE_INTEGER& count)
		{
			return static_cast<float>(count.QuadPart) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}
		
		static float ToSecondsAsFloat(const LONGLONG& count)
		{
			return static_cast<float>(count) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}
		
		/*!
			@brief	パフォーマンスカウンタ値をミリ秒に変換する。
		*/
		static float ToMilliSecondsAsFloat(const LARGE_INTEGER& count)
		{
			return static_cast<float>(count.QuadPart * 1000) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}

		static float ToMilliSecondsAsFloat(const LONGLONG& count)
		{
			return static_cast<float>(count * 1000) / static_cast<float>(StaticMembers::Get()._frequency.QuadPart);
		}
		
		/*!
			@brief	基準時刻からの経過秒数を取得する。
					基準時刻とは、StaticMembers が初期化された時刻である。
		*/
		static float GetCurrentSecondsAsFloat()
		{
			LARGE_INTEGER count;
			GetCurrentCount(count);
			count.QuadPart -= StaticMembers::Get()._begin.QuadPart;
			return ToSecondsAsFloat(count);
		}

		/*!
			@brief	基準時刻からの経過ミリ秒数を取得する。
					基準時刻とは、StaticMembers が初期化された時刻である。
		*/
		static float GetCurrentMiiliSecondsAsFloat()
		{
			LARGE_INTEGER count;
			GetCurrentCount(count);
			count.QuadPart -= StaticMembers::Get()._begin.QuadPart;
			return ToMilliSecondsAsFloat(count);
		}
		
	private:
		struct StaticMembers
		{
			LARGE_INTEGER _frequency;
			LARGE_INTEGER _begin;
			static StaticMembers& Get()
			{
				static StaticMembers values;
				return values;
			}
		private:
			StaticMembers()
			{
				if (failed(QueryPerformanceFrequency(&_frequency)))
				{
					throw QueryPerformanceCounterNotSupportedException();
				}
				GetCurrentCount(_begin);
			}
		};
	};	// PerformanceCounter
	
}	// end namespace Marda

