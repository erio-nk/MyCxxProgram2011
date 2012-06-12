/*!
	@file Assert.h

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */

#pragma once

#include <assert.h>

#if defined(NO_ASSERT)
	#define Assert __noop
#else
	#define Assert(expr, msg) assert(expr && msg)
#endif
