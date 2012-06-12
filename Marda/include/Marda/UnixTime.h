/*!
	@file	UnixTime.h
	
			UNIXタイムベースの時刻クラス。
			時分秒・日の加減算は可能だが、月は月毎の日数が違う為現状は出来ない。気が向いたら実装する。
			
	@todo	時分秒・年月日取得関数。

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	class UnixTime
	{
	public:
		//! 1秒を表す定数
		static const UnixTime A_SECOND;

		//! 1分(60秒)を表す定数
		static const UnixTime A_MINUTE;
		
		//! 1時間(3600秒)を表す定数
		static const UnixTime A_HOUR;
		
		//! 1日(3600*24秒)を表す定数
		static const UnixTime A_DAY;

		/*!
			@brief	現在時刻でオブジェクトを構築する。
		*/
		UnixTime();
		
		/*!
			@brief	指定の時刻でオブジェクトを構築する。
		*/
		UnixTime(time_t time);

		//! dtor
		~UnixTime();
		
		/*!
			@brief	UNIXタイムの加算を行う。
		*/
		UnixTime operator + (const UnixTime& rhs) const
		{
			return UnixTime(_time + rhs._time);
		}
		
		/*!
			@brief	UNITタイムの減算を行う。
		*/
		UnixTime operator - (const UnixTime& rhs) const
		{
			return UnixTime(_time - rhs._time);
		}

		/*!
			@brief	UNIXタイムの乗算を行う。
		*/
		UnixTime operator * (float rhs) const
		{
			return UnixTime(static_cast<time_t>(_time * rhs));
		}

		/*!
			@brief	UNIXタイムの除算を行う。
		*/
		UnixTime operator / (float rhs) const
		{
			return UnixTime(static_cast<time_t>(_time / rhs));
		}
		
		/*!
			@brief	時刻の文字列表現を取得する。
					この関数はスレッドセーフである。
		*/
		const char* ToStringA() const;
		
		const wchar_t* ToStringW() const;
		
		const TCHAR* ToString() const
		{
			#if defined(UNICODE)
			return ToStringW();
			#else
			return ToStringA();
			#endif
		}
		
	private:
		time_t _time;
		
	};	// end class Timer

}	// end namespace Marda
