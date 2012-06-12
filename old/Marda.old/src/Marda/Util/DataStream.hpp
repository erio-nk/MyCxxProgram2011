/*!
	@file	Util/DataStream.hpp

	@brief	�����o�֐��e���v���[�g�̒�`

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
	// �܂��T�C�Y���l�߂�
	uint32 size = static_cast<uint32>(sizeof(TChar) * (value.length() + 1));
	*this << size;

	// ���f�[�^�B
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
	// �܂��T�C�Y���擾
	uint32 size = 0;
	*this >> size;

	if (_cursor + size <= _data.size() && !_data.empty()) {
		// ���f�[�^�B�ꕶ�����ǂݍ��݂���Ȃ���Ńk���I�[���ĂȂ��f�[�^�ɑ΂��ēǂݍ��ނƗ�����B
		const TChar* src = reinterpret_cast<const TChar*>(&_data[_cursor]);
		value = src;
		_cursor += size;
	}
	return *this;
}
