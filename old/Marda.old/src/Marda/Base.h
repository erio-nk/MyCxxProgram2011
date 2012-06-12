/*!
	@file	Base.h
	@author	Dila
	
	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.
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
#include <sstream>
#include <memory>

#include <list>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include <algorithm>

// Character
#include <wchar.h>
#include <tchar.h>

// Windows
#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>

	// MSVC
	#define _MSVC		// #if defined(_MSVC)的な。
	#define MSVC_VER	_MSC_VER	// 現在VCバージョン。

	#define MSVC_6		1200	// Visual C++ 6.0
	#define MSVC_7		1300	// Visual C++ 7.0 (Visual Studio .NET 2002)
	#define MSVC_7_1	1310	// Visual C++ 7.1 (Visual Studio .NET 2003)
	#define MSVC_8		1400	// Visual C++ 8.0 (Visual Studio 2005)
	#define MSVC_9		1500	// Visual C++ 9.0 (Visual Studio 2008)
#endif

// Technical Report 1
#if defined(_MSVC)
	#include <unordered_map>
	#include <unordered_set>

	#if MSVC_VER >= MSVC_9
		namespace std 
		{
			using namespace tr1;	// std::tr1::xxx -> std::xxx
		}
	#endif
#endif

// Marda
#include "Base/Types.h"
#include "Base/Cast.h"

