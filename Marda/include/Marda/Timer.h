/*!
	@file	Timer.h
	
			時間計測クラス。
			timeGetTime利用版は気が向いたら。

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	class Timer
	{
	public:
		typedef float float_t;
		
		Timer();

		~Timer();
		
		/*!
			@brief	タイマーを開始する。
		*/
		void Start();
		
		/*!
			@brief	タイマーを停止する。
		*/
		void Stop();
		
		/*!
			@brief	タイマーをリセットする。
		*/
		void Reset();
		
		/*!
			@brief	タイマーを開始してからの経過時間を取得する。
					タイマーを停止していた時間は含まれない。
			@return	経過時間。秒。
		*/
		float GetTime() const;
		
		/*!
			@brief	前回の GetElapsedTime 呼び出し時からの経過時間を取得する。
					タイマーを停止していた時間は含まれない。
			@param	reset	false を指定した場合は、経過時間のリセットを行わない。
			@return	前回の GetElapsedTime 呼び出し時からの経過時間。秒。
		*/
		float GetElapsedTime(bool reset = true);
		
	private:
		struct Impl;
		std::auto_ptr<Impl> _pimpl;
		
	};	// end class Timer

}	// end namespace Marda
