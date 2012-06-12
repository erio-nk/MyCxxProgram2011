/**
 	@file Date.h
 
 	日付クラス
 
 	@author Dila
 */
#pragma once

#include <time.h>

/**
 	日付クラス
 
 */
class Date
{
public:
	/**
	 	月のenum
	 */
	enum MONTH {
		JANUARY = 1,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER,
	};

	/**
	 	曜日(GetWDay())のenum
	 */
	enum WDAY {
		SUNDAY,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY,
	};

private:
	int year;
	int month;
	int mday;
	int yday;
	WDAY wday;
	int hour;
	int minute;
	int second;

public:

	Date();

	~Date() {}

	int GetYear() { return year; }
	int GetMonth() { return month; }
	int GetMDay() { return mday; }
	int GetYDay() { return yday; }
	WDAY GetWDay() { return wday; }
	int GetHour() { return hour; }
	int GetMinute() { return minute; }
	int GetSecond() { return second; }

	int GetMDayCount();
	int GetMDayCount(int year, int month);
	int GetYDayCount();
	int GetYDayCount(int year);

	int GetTime();

	bool SetDate(int year, int month = 1, int mday = 1, int hour = 0, int minute = 0, int second = 0);
	bool SetTime(const time_t& t);

};	// end class Date

