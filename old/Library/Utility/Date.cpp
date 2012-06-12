/**
 *	@file Date.cpp
 *
 *	日付クラスの実装
 *
 *	@author Dila
 */
#include "Utility/Date.h"


/**
 *	コンストラクタ
 */
Date::Date()
{
	time_t t;
	tm* m;
	time(&t);
	m = localtime(&t);

	year = m->tm_year+1900;
	month = m->tm_mon+1;
	mday = m->tm_mday;
	yday = m->tm_yday+1;
	wday = (WDAY)m->tm_wday;
	hour = m->tm_hour;
	minute = m->tm_min;
	second = m->tm_sec;
}



/**
 *	この月の日数を取得する
 *
 */
int Date::GetMDayCount(int year, int month)
{
	if (month < AUGUST) {
		if (month == FEBRUARY) {
			if (GetYDayCount(year) == 366) {
				// 閏年
				return 29;
			}
			return 28;
		}
		return (month & 1) == 0 ? 30 : 31;
	}
	return (month & 1) == 0 ? 31 : 30;
}

int Date::GetMDayCount()
{
	return GetMDayCount(year, month);
}

/**
 *	この年の日数を取得する
 *
 */
int Date::GetYDayCount(int year)
{
	if ((year % 400) == 0 || ((year % 4) == 0 && (year % 100) != 0)) {
		// 閏年
		return 366;
	}
	return 365;
}

int Date::GetYDayCount()
{
	return GetYDayCount(year);
}


/**
 *
 */
bool Date::SetDate(int year, int month, int mday, int hour, int minute, int second)
{
	// バリデーション
	if (year < 0) {
		return false;
	}
	if (month < 1 || month > 12) {
		return false;
	}
	if (mday < 1 || mday > GetMDayCount(year, month)) {
		return false;
	}
	if (hour < 0 || hour > 24) {
		return false;
	}
	if (minute < 0 || minute > 60) {
		return false;
	}
	if (second < 0 || second > 60) {
		return false;
	}

	tm m;
	m.tm_year = year-1900;
	m.tm_mon = month-1;
	m.tm_mday = mday;
	m.tm_hour = hour;
	m.tm_min = minute;
	m.tm_sec = second;
	
	time_t t = mktime(&m);
	
	m = *localtime(&t);
	
	this->year = m.tm_year+1900;
	this->month = m.tm_mon+1;
	this->mday = m.tm_mday;
	this->yday = m.tm_yday+1;
	this->wday = (WDAY)m.tm_wday;
	this->hour = m.tm_hour;
	this->minute = m.tm_min;
	this->second = m.tm_sec;

	return true;
}