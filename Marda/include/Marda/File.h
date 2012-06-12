/*!
	@file	File.h
	
			�t�@�C���֘A�̃w���p�֐��E�N���X�Q�B

	@author	E.Nakayama
*/
#pragma once

#include "String.h"

namespace Marda
{
	// ------------------------------------------------------------------------
	// �t�@�C���p�X�֘A�B
	
	#define ADIR_DELIMS	"\\/"	// �f�B���N�g����؂蕶����B
	#define ADIR_DELIM	"\\"	// �f�B���N�g����؂蕶���B
	#define AEXT_DELIM	"."		// �g���q��؂蕶���B

	#define WDIR_DELIMS	L"\\/"
	#define WDIR_DELIM	L"\\"
	#define WEXT_DELIM	L"."
	
	#if defined(_UNICODE)
		#define TDIR_DELIMS	WDIR_DELIMS
		#define TDIR_DELIM	WDIR_DELIM
		#define TEXT_DELIM	WEXT_DELIM
	#else
		#define TDIR_DELIMS	ADIR_DELIMS
		#define TDIR_DELIM	ADIR_DELIM
		#define TEXT_DELIM	AEXT_DELIM
	#endif
		
	// �e���v���[�g�ŁB
	template <typename CharType>
	inline const CharType* GetDirectoryDelimiters();
	
	template <>
	inline const char* GetDirectoryDelimiters<char>()
	{
		return ADIR_DELIMS;
	}

	template <>
	inline const wchar_t* GetDirectoryDelimiters<wchar_t>()
	{
		return WDIR_DELIMS;
	}

	template <typename CharType>
	inline const CharType* GetDirectoryDelimiter();

	template <>
	inline const char* GetDirectoryDelimiter<char>()
	{
		return ADIR_DELIM;
	}

	template <>
	inline const wchar_t* GetDirectoryDelimiter<wchar_t>()
	{
		return WDIR_DELIM;
	}

	template <typename CharType>
	inline const CharType* GetExtensionDelimiter();

	template <>
	inline const char* GetExtensionDelimiter<char>()
	{
		return AEXT_DELIM;
	}

	template <>
	inline const wchar_t* GetExtensionDelimiter<wchar_t>()
	{
		return WEXT_DELIM;
	}


	/*!
		@brief	�t�@�C���p�X����f�B���N�g�������擾����B
		@tparam	CharType	�����^�B
		@param		path	�t�@�C���p�X�B
		@param[out]	buf		�o�b�t�@�B
		@param		buflen	�o�b�t�@���i�������j�B
	*/
	template <typename CharType>
	void GetDirectoryPath(const CharType* path, CharType* buf, size_t buflen)
	{
		size_t found = static_cast<size_t>(StrRCSpn<CharType>(path, GetDirectoryDelimiters<CharType>()));
		if (path[found] != 0)
		{
			++found;	// �f���~�^�����Z����B
		}
		if (found > buflen - 1)	// �k�����������������Ĕ��肷��B
		{
			found = buflen - 1;
		}
		memcpy(buf, path, found * 2);
		buf[found] = 0;
	}
	
	/*!
		@brief	�t�@�C���p�X����f�B���N�g�������擾����B
		@tparam	StringType	std::string �N���X�Ɠ����C���^�t�F�[�X��������������N���X�B
	*/
	template <class StringType>
	StringType GetDirectoryPathAsString(const StringType& path)
	{
		StringType::size_type found = path.find_last_of(GetDirectoryDelimiters<StringType::value_type>());
		if (found == StringType::npos)
		{
			return StringType();
		}
		return path.substr(0, found + 1);
	}
	
	/*!
		@brief	�t�@�C���p�X����t�@�C�������擾����B
		@tparam	StringType	std::string �N���X�Ɠ����C���^�t�F�[�X��������������N���X�B
	*/
	template <class StringType>
	StringType GetFileNameAsString(const StringType& path)
	{
		StringType::size_type found_dir = path.find_last_of(GetDirectoryDelimiters<StringType::value_type>());
		if (found == StringType::npos)
		{
			return path;
		}
		return path.substr(found, path.length());
	}

}	// end namespace Marda
