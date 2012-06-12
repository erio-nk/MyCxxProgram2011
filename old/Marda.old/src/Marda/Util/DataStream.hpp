/*!
	@file	Util/DataStream.hpp

	@brief	メンバ関数テンプレートの定義

	Copyright(C) 2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author	Dila
 */
#pragma once

template <class T>
Marda::DataStream&
Marda::DataStream::operator << (const T& value)
{
	size_t pos = _data.size();
	_data.resize(_data.size() + sizeof(T));
	T* dst = reinterpret_cast<T*>(&_data[pos]);
	*dst = value;
	return *this;
}

template <class TChar>
Marda::DataStream&
Marda::DataStream::operator << (const std::basic_string<TChar>& value)
{
	// まずサイズを詰める
	uint32 size = static_cast<uint32>(sizeof(TChar) * (value.length() + 1));
	*this << size;

	// 実データ。
	size_t pos = _data.size();
	_data.resize(_data.size() + size);
	TChar* dst = reinterpret_cast<TChar*>(&_data[pos]);
	memcpy(dst, value.c_str(), size);
	return *this;
}

template <class T>
Marda::DataStream&
Marda::DataStream::operator >> (T& value)
{
	size_t size = sizeof(T);
	if (_cursor + size <= _data.size() && !_data.empty()) {
		const T* src = reinterpret_cast<const T*>(&_data[_cursor]);
		value = *src;
		_cursor += size;
	}
	return *this;
}

template <class TChar>
Marda::DataStream&
Marda::DataStream::operator >> (std::basic_string<TChar>& value)
{
	// まずサイズを取得
	uint32 size = 0;
	*this >> size;

	if (_cursor + size <= _data.size() && !_data.empty()) {
		// 実データ。一文字ずつ読み込みじゃないんでヌル終端してないデータに対して読み込むと落ちる。
		const TChar* src = reinterpret_cast<const TChar*>(&_data[_cursor]);
		value = src;
		_cursor += size;
	}
	return *this;
}
