/*!
	@brief	Util/DataStream.cpp

	ƒƒ“ƒoŠÖ”‚Ì’è‹`

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#include "Marda.h"
#include "DataStream.h"


Marda::DataStream::DataStream() :
	_cursor(0)
{
}

Marda::DataStream::DataStream(const std::vector<uint8>& src) :
	_cursor(0),
	_data(src.begin(), src.end())
{
}

Marda::DataStream::DataStream(const std::vector<sint8>& src) :
	_cursor(0),
	_data(src.begin(), src.end())
{
}

Marda::DataStream::DataStream(const void* src, size_t size) :
	_cursor(0),
	_data(static_cast<const uint8*>(src), static_cast<const uint8*>(src)+size)
{
}

Marda::DataStream::~DataStream()
{
}

size_t
Marda::DataStream::GetCursor() const
{
	return _cursor;
}

size_t
Marda::DataStream::GetSize() const
{
	return _data.size() - _cursor;
}


