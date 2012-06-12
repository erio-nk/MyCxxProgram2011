/*!
	@file String.h


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#pragma once

#include <string>

namespace Marda
{
	typedef std::string StringA;
	typedef std::wstring StringW;

	#if defined(_UNICODE)
		typedef StringW String;
	#else
		typedef StringA String;
	#endif
}
