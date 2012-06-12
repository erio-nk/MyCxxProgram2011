/*!
	@file	Util/DataStream.h

	@brief	簡易データストリーム。
			STLのストリーム互換を目指したい。

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#pragma once

#include "Marda.h"

namespace Marda
{
	/*!
		@brief	簡易データストリームクラス
	 */
	class DataStream
	{
	public:
		DataStream();

		DataStream(const std::vector<uint8>& src);

		DataStream(const std::vector<sint8>& src);

		DataStream(const void* src, size_t size);

		~DataStream();

		/*!
			@brief	現在の入力カーソルを取得する。
		 */
		size_t GetCursor() const;

		/*!
			@brief	現在のストリームサイズを取得する。
		 */
		size_t GetSize() const;

		/*!
			@brief	現在のストリームの先頭アドレスを取得する。
		 */
		const uint8* GetData() const;

		/*!
			@brief	データをストリームに出力する。
		 */
		template <class T>
		DataStream& operator << (const T& value);

		/*!
			@brief	文字列データをストリームに出力する。
		 */
		template <class TChar>
		DataStream& operator << (const std::basic_string<TChar>& value);

		/*!
			@brief	ストリームからデータを入力する。
		 */
		template <class T>
		DataStream& operator >> (T& value);

		/*!
			@brief	ストリームから文字列データを入力する。
		 */
		template <class TChar>
		DataStream& operator >> (std::basic_string<TChar>& value);

	private:
		std::vector<uint8> _data;	///< データ
		size_t _cursor;	///< 入力カーソル
				
	};

}	// end namespace Marda

#include "DataStream.hpp"

