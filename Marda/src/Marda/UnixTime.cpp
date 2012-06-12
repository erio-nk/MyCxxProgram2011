/*!
	@file	UnixTime.cpp

	@author	E.Nakayama
*/

#include "Marda/Thread.h"
#include "Marda/UnixTime.h"

using namespace Marda;

const UnixTime
UnixTime::A_SECOND(1);

const UnixTime
UnixTime::A_MINUTE(60);

const UnixTime
UnixTime::A_HOUR(60 * 60);

const UnixTime
UnixTime::A_DAY(60 * 60 * 24);

UnixTime::UnixTime()
	: _time(time(NULL))
{
}

UnixTime::UnixTime(time_t t)
	: _time(t)
{
}

UnixTime::~UnixTime()
{
}

namespace
{
	struct TLBufferTag {};
}

const char*
UnixTime::ToStringA() const
{
	ThreadLocalArray<char, 32, TLBufferTag> buf;
	struct tm ts;
	localtime_s(&ts, &_time);
	strftime(*buf, buf.SIZE, "%Y/%m/%d %H:%M:%S", &ts);
	return *buf;
}

const wchar_t*
UnixTime::ToStringW() const
{
	ThreadLocalArray<wchar_t, 32, TLBufferTag> buf;
	struct tm ts;
	localtime_s(&ts, &_time);
	wcsftime(*buf, buf.SIZE, L"%Y/%m/%d %H:%M:%S", &ts);
	return *buf;
}