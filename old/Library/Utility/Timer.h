/*!
 	@file Utility/Timer.h
 
 	タイマークラス
 
 	@author	Dila
 */
//! @addtogroup Utility
//! @{

#pragma once

/*!
 	タイマークラス
 */
class Timer
{
	unsigned long long start;	//! 開始時間
	unsigned long long freq;	//! 周波数
	float elapsed;	//! 蓄積した経過時間

	bool bStop;	//! 停止フラグ

public:
	Timer();
	~Timer();

	//! タイマー開始
	void Start();

	//! タイマー停止
	void Stop();

	//! 秒取得
	float GetTime() const;
};

//! @}
