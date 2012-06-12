// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include "Marda.h"
#include "Marda/Utilities/FrameRateController.h"
#include "Marda/Windows/Window.h"
#include "Marda/Graphics/Direct3D10.h"

#include "../Rakugaki_MikuMikuDance_common/PMD.h"
#include "../Rakugaki_MikuMikuDance_common/VMD.h"
