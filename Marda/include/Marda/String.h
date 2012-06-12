/*!
	@file	String.h
	
			文字列のヘルパ関数・クラス群。

	@author	E.Nakayama
*/
#pragma once

#include "Misc.h"
	
namespace Marda
{
	// 静的文字列の文字列長を取得するマクロ
	#define STATIC_STR_LEN(str)	(_countof(str) - 1)

	// ------------------------------------------------------------------------
	// Cランタイムライブラリの文字列関数のテンプレート版
	
	/*!
		@brief	文字列長を返す関数テンプレート。
	*/
	template <typename CharType>
	inline size_t StrLen(const CharType* str);
	
	template <>
	inline size_t StrLen<char>(const char* str)
	{
		return strlen(str);
	}
	
	template <>
	inline size_t StrLen<wchar_t>(const wchar_t* str)
	{
		return wcslen(str);
	}
	
	/*!
		@brief	文字列を n 文字コピーする関数テンプレート。
	*/
	template <typename CharType>
	inline void StrNCpy(CharType* dst, const CharType* src, size_t num);

	#pragma warning(push)
	#pragma warning(disable: 4996)
	template <>
	inline void StrNCpy<char>(char* dst, const char* src, size_t num)
	{
		strncpy(dst, src, num);
	}

	template <>
	inline void StrNCpy<wchar_t>(wchar_t* dst, const wchar_t* src, size_t num)
	{
		wcsncpy(dst, src, num);
	}
	#pragma warning(pop)
	
	/*!
		@brief	文字列から特定の文字を検索する関数テンプレート。
		@param	str	NULLで終わる検索対象文字列
		@param	c	検索文字
		@return	cが最初に出現する位置を指すポインタを返す。
				cが見つからなかった場合はNULLを返す。
	*/
	template <typename CharType>
	inline const CharType* StrChr(const CharType* str, CharType c);
	
	template <>
	inline const char* StrChr<char>(const char* str, char c)
	{
		return strchr(str, c);
	}

	template <>
	inline const wchar_t* StrChr<wchar_t>(const wchar_t* str, wchar_t c)
	{
		return wcschr(str, c);
	}
	
	/*!
		@brief	文字列から文字セット内にある文字を検索する関数テンプレート。
		@param	str	NULLで終わる検索対象文字列。
		@param	strCharSet	NULLで終わる文字セット。
		@return	strCharSetにある文字が最初に出現する位置を返す。
				strCharSetにある文字文字が見つからなかった場合は終端の位置（＝NULL文字の位置＝strの長さ）を返す。
		@note	strcspn = String Complement Span らしい。
	*/
	template <typename CharType>
	inline size_t StrCSpn(const CharType* str, const CharType* strCharSet);
	
	template <>
	inline size_t StrCSpn<char>(const char* str, const char* strCharSet)
	{
		return strcspn(str, strCharSet);
	}
	
	template <>
	inline size_t StrCSpn<wchar_t>(const wchar_t* str, const wchar_t* strCharSet)
	{
		return wcscspn(str, strCharSet);
	}

	/*!
		@brief	文字列から文字セット内に無い文字を検索する関数テンプレート。
		@param	str	NULLで終わる検索対象文字列。
		@param	strCharSet	NULLで終わる文字セット。
		@return	strCharSetに無い文字が最初に出現する位置を返す。
				strCharSetに無い文字が見つからなかった場合は終端の位置（＝NULL文字の位置＝strの長さ）を返す。
	*/
	template <typename CharType>
	inline size_t StrSpn(const CharType* str, const CharType* strCharSet);
	
	template <>
	inline size_t StrSpn<char>(const char* str, const char* strCharSet)
	{
		return strspn(str, strCharSet);
	}
	
	template <>
	inline size_t StrSpn<wchar_t>(const wchar_t* str, const wchar_t* strCharSet)
	{
		return wcsspn(str, strCharSet);
	}
	

	// ------------------------------------------------------------------------
	// C標準ライブラリにありそうで無い文字列関数群。
		
	/*!
		@brief	文字セットに属する文字が文字列内に最後に出現する位置のインデックスを返す。
				strcspnの末尾から検索版。
		@tparam	CharType	文字型
		@param	str			NULL で終わる検索文字列。
		@param	strCharSet	NULL で終わる文字セット。
		@return	strCharSet に属する文字が str に最後に出現する位置のインデックス。見つからなかった場合は-1。
	*/
	template <typename CharType>
	int StrRCSpn(const CharType* str, const CharType* strCharSet)
	{
		const CharType* found = NULL;
		for (const CharType* i = str; *i != 0; ++i)
		{
			for (const CharType* k = strCharSet; *k != 0; ++k)
			{
				if (*i == *k)
				{
					found = i;
					break;
				}
			}
		}
			
		if (is_null(found))
		{
			return -1;
		}
		
		return static_cast<int>(found - str);
	}

	/*!
		@brief	文字列から特定の文字を検索する関数テンプレート。
				こちらは、文字が見つからなかった場合にはNULLではなく終端文字の位置を指すポインタを返す。
				(gccの標準Cライブラリには無い関数strchrnulと同じ処理)
		@param	str	NULLで終わる検索対象文字列。
		@param	c	検索文字。
		@return	cが最初に出現する位置を指すポインタを返す。
				cが見つからなかった場合は終端文字の位置を指すポインタを返す。
	*/
	template <typename CharType>
	const CharType* StrChrNul(const CharType* str, CharType c)
	{
		for (; *str != 0; ++str)
		{
			if (*str == c)
			{
				break;
			}
		}
		return str;
	}
	
	/*!
		@brief	文字列内で最後に出現する検索文字列へのポインタを返す関数テンプレート。
		@param	str	NULLで終わる検索対象文字列。
		@param	strSearch	検索文字列。
		@return	strSearch が最初に出現する位置を指すポインタを返す。
				strSearch が見つからなかった場合はNULLを返す。
	*/
	template <typename CharType>
	const CharType* StrRStr(const CharType* str, const CharType* strSearch)
	{
		const CharType* result = NULL;
		const CharType* current = strSearch;
		
		for (; *str != 0; ++str)
		{
			if (*str == *current)
			{
				++current;
				if (*current == 0)
				{
					result = str - (current - strSearch - 1);
				}
			}
			else
			{
				current = strSearch;
			}
		}
		
		return result;
	}
	
	/*!
		@brief	文字列の取り出しを行う関数テンプレート。
				strncpy 関数では、第二引数の文字列長がコピー文字数より少ない場合に、ヌル文字の付加を行わないが、
				この関数ではヌル文字を必ず最後に付加する。
	*/
	template <typename CharType>
	void SubStr(CharType* dst, const CharType* src, size_t pos, size_t num)
	{
		dst[0] = 0;
		size_t len = StrLen<CharType>(src);
		if (len <= pos)
			return;
		num = min(num, len - pos);
		StrNCpy<CharType>(dst, src + pos, num);
		dst[num] = 0;
	}

	// ------------------------------------------------------------------------
	// 文字列解析クラス。

	/*!
		@brief	文字列解析クラス。
				区切り文字の指定に加えて、引用符（クォーテーション）も対応している。
		@deprecated	boost::tokenizerを使う事。
	*/
	template <typename CharType>
	class Tokens
	{
	public:
		static CharType EMPTY_STRING[];
		
		Tokens()
			: _next(NULL)
		{
		}
		
		~Tokens()
		{
		}
		
		/*!
			@brief	解析する文字列を設定する。
					今までの解析情報はリセットされる。
			@param	source	解析する文字列。
			@param	delims	NULLで終わる区切り文字セット。
			@param	quots	NULLで終わる引用符文字ペアセット。
							引用開始文字１、引用終了文字１、引用開始文字２、引用終了文字２…
							という風に指定する。奇数長の文字列を渡した場合の動作は未定義。
			@code
			Tokens<TCHAR> tokens;
			tokens.Set(_T("I love \"Romancing SaGa\" (Super Famicom software)"), _T(" "), _T("\"\"()")); 
			@endcode
		*/
		void Set(const CharType* source, const CharType* delims, const CharType* quots = EMPTY_STRING);

		/*!
			@brief	次のトークンを返す
			@param[out]	token	トークン
			@
		*/
		bool Next(std::basic_string<CharType>& token);
		
	private:
		const CharType* _next;
		std::vector<CharType> _delims;
		std::vector<CharType> _quots;
		
	};	// end class Tokens
	
	// 実装部
	template <typename CharType>
	CharType 
	Tokens<CharType>::EMPTY_STRING[] = { 0 };
	
	template <typename CharType>
	void
	Tokens<CharType>::Set(const CharType* source, const CharType* delims, const CharType* quots)
	{
		_next = source;
		_delims.assign(delims, delims + StrLen(delims) + 1);
		_quots.assign(quots, quots + StrLen(quots)+1);
	}
	
	template <typename CharType>
	bool 
	Tokens<CharType>::Next(std::basic_string<CharType>& token)
	{
		const CharType* token_begin = StrSpn(_next, &_delims[0]) + _next;
		const CharType* token_end = NULL;
		if (*token_begin == 0)
		{
			return false;
		}

		const CharType* found = NULL;
		found = StrChr(&_quots[0], *token_begin);
		if (is_null(found))
		{
			// 区切り文字か引用符を検索する
			_next = StrCSpn(token_begin, &_delims[0]) + token_begin;
			token_end = _next;
		}
		else
		{
			// 引用符で始まっていたので閉じ引用符を検索する
			++token_begin;	// 引用符は返却しない
			const CharType* close_quot = found + 1;
			token_end = StrChr(token_begin, *close_quot);
			if (is_null(token_end))
			{
				// 閉じられてない
				_next = _next + StrLen(_next);
				token_end = _next;
			}
			else
			{
				_next = token_end + 1;
			}
		}

		if (token_end - token_begin == 0)
		{
			return false;
		}

		token.assign(token_begin, token_end - token_begin);
		return true;
	}

	
}	// end namespace Marda

