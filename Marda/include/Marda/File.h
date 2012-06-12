/*!
	@file	File.h
	
			ファイル関連のヘルパ関数・クラス群。

	@author	E.Nakayama
*/
#pragma once

#include "String.h"

namespace Marda
{
	// ------------------------------------------------------------------------
	// ファイルパス関連。
	
	#define ADIR_DELIMS	"\\/"	// ディレクトリ区切り文字列。
	#define ADIR_DELIM	"\\"	// ディレクトリ区切り文字。
	#define AEXT_DELIM	"."		// 拡張子区切り文字。

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
		
	// テンプレート版。
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
		@brief	ファイルパスからディレクトリ名を取得する。
		@tparam	CharType	文字型。
		@param		path	ファイルパス。
		@param[out]	buf		バッファ。
		@param		buflen	バッファ長（文字数）。
	*/
	template <typename CharType>
	void GetDirectoryPath(const CharType* path, CharType* buf, size_t buflen)
	{
		size_t found = static_cast<size_t>(StrRCSpn<CharType>(path, GetDirectoryDelimiters<CharType>()));
		if (path[found] != 0)
		{
			++found;	// デリミタ分加算する。
		}
		if (found > buflen - 1)	// ヌル文字分差し引いて判定する。
		{
			found = buflen - 1;
		}
		memcpy(buf, path, found * 2);
		buf[found] = 0;
	}
	
	/*!
		@brief	ファイルパスからディレクトリ名を取得する。
		@tparam	StringType	std::string クラスと同じインタフェースを持った文字列クラス。
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
		@brief	ファイルパスからファイル名を取得する。
		@tparam	StringType	std::string クラスと同じインタフェースを持った文字列クラス。
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
