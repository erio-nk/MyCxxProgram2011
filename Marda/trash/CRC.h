/*!
	@file	CRC.h
	
			CRC を計算する関数。本プロジェクトでの主な用途はハッシュ関数。
	
	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"

namespace Marda
{
	/*!
		@brief	3ビット長のCRCを計算する。
				この関数は勉強用。
				生成多項式は 1101。ビット送りは左送り（＝MostSignificantBit First＝左送り）。
		@return	3ビット長CRC。入力バイト列が空であった場合は、0を返す。
	*/
	uint8 MakeCRC3(const void* pData, size_t size);

	//! テーブルを作成する。
	void InitCRC3Table();
	
	//! テーブルを利用した高速化版。
	uint8 MakeCRC3t(const void* pData, size_t size);
	

	/*!
		@brief	8ビット長のCRCを計算する。
		@return	3ビット長CRC。入力バイト列が空であった場合は、0を返す。
	*/
	uint8 MakeCRC8(const void* pData, size_t size);
	
	//! テーブルを利用した高速化版。
	uint8 MakeCRC8t(const void* pData, size_t size);
	
	//! テーブルを作成する。
	void InitCRC8Table();
	
	uint16 MakeCRC16(const void* pData, size_t size);
	
	uint16 MakeStrCRC16(const char* str);
	uint16 MakeStrCRC16(const wchar_t* str);
	
	uint32 MakeCRC32(const void* pData, size_t size);

	uint64 MakeCRC64(const void* pData, size_t size);

}	// end namespace Marda
