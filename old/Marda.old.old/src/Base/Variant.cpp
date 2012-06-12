/*!
	@file Variant.cpp


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#include "Marda.h"

using namespace Marda;


// ctor
Variant::Variant()
{
}

Variant::Variant(int val)
{
	SetInt(val);
}

Variant::Variant(float val)
{
	SetFloat(val);
}

Variant::Variant(const String& val)
{
	SetString(val);
}

Variant::Variant(bool val)
{
	SetBool(val);
}

int Variant::GetInt() const
{
	return _tcstol(_value.c_str(), NULL, 0);
}

float Variant::GetFloat() const
{
	return static_cast<float>(_tstof(_value.c_str()));
}

const String& Variant::GetString() const
{
	return _value;
}

bool Variant::GetBool() const
{
	if (_tcsicmp(_T("false"), _value.c_str()) == 0) return false;
	if (_tcsicmp(_T("0"), _value.c_str()) == 0) return false;
	if (_tcsicmp(_T("disable"), _value.c_str()) == 0) return false;	// ó]åvÇ©Ç»ÅB
	return true;
}

void Variant::SetInt(int val)
{
	TCHAR buf[16];
	_itot_s(val, buf, sizeof(buf)/sizeof(TCHAR), 10);
	_value = buf;
}

void Variant::SetFloat(float val)
{
	TCHAR buf[32];
	#if defined(UNICODE)
		swprintf_s(buf, sizeof(buf)/sizeof(TCHAR), L"%f", val);
	#else
		sprintf_s(buf, sizeof(buf)/sizeof(TCHAR), "%f", val);
	#endif
	_value = buf;
}

void Variant::SetString(const String& val)
{
	_value = val;
}

void Variant::SetBool(bool val)
{
	if (val) _value = _T("true");
	else _value = _T("false");
}
