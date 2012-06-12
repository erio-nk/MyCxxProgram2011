/*!
	@file Base.h

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */
#pragma once

// C Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <assert.h>
#include <stdarg.h>

// C++ Library
#include <iostream>
#include <new>
#include <string>
#include <memory>

#include <list>
#include <vector>
#include <deque>
#include <map>

#include <algorithm>

// Character
#include <wchar.h>
#include <tchar.h>

// Windows
#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif

// MSVC
#include "Base/MSVC.h"

// Technical Report 1
#if MSVC_VER >= MSVC_9
	#include <unordered_map>
	#include <unordered_set>

	#if MSVC_VER == MSVC_9
		namespace std
		{
			using namespace tr1;	// std::tr1::xxx -> std::xxx
		}
	#endif
#endif


// Base
#include "Base/Assert.h"
#include "Base/Number.h"
#include "Base/Real.h"
#include "Base/Bool.h"
#include "Base/Cast.h"
#include "Base/String.h"
#include "Base/Uncopyable.h"

#include "Base/DebugTrace.h"

//#include "Base/Main.h"

#include "Base/Variant.h"
#include "Base/Delegate.h"
#include "Base/Properties.h"


