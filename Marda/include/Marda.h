/*!
	@file	Marda.h
			Mardaライブラリルートヘッダ。

	@author	E.Nakayama
*/

#pragma once

// C Library
#include <stdio.h>
#include <time.h>
#include <assert.h>
	
// C++ Standard Template Library
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <numeric>
#include <algorithm>

#include <array>
#include <unordered_map>
#include <functional>

// Windows API
#include <windows.h>
#include <tchar.h>

// Marda Library
#include "Marda/Types.h"
#include "Marda/Misc.h"	
#include "Marda/String.h"
#include "Marda/File.h"	
#include "Marda/STLHelpers.h"	
#include "Marda/Container.h"
#include "Marda/MPHelpers.h"
#include "Marda/Thread.h"	
#include "Marda/Coroutine.h"
#include "Marda/PerformanceCounter.h"
#include "Marda/Timer.h"
#include "Marda/UnixTime.h"
#include "Marda/Profiler.h"
