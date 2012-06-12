/*!
	@file	String.h
	
			������̃w���p�֐��E�N���X�Q�B

	@author	E.Nakayama
*/
#pragma once

#include "Misc.h"
	
namespace Marda
{
	// �ÓI������̕����񒷂��擾����}�N��
	#define STATIC_STR_LEN(str)	(_countof(str) - 1)

	// ------------------------------------------------------------------------
	// C�����^�C�����C�u�����̕�����֐��̃e���v���[�g��
	
	/*!
		@brief	�����񒷂�Ԃ��֐��e���v���[�g�B
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
		@brief	������� n �����R�s�[����֐��e���v���[�g�B
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
		@brief	�����񂩂����̕�������������֐��e���v���[�g�B
		@param	str	NULL�ŏI��錟���Ώە�����
		@param	c	��������
		@return	c���ŏ��ɏo������ʒu���w���|�C���^��Ԃ��B
				c��������Ȃ������ꍇ��NULL��Ԃ��B
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
		@brief	�����񂩂當���Z�b�g���ɂ��镶������������֐��e���v���[�g�B
		@param	str	NULL�ŏI��錟���Ώە�����B
		@param	strCharSet	NULL�ŏI��镶���Z�b�g�B
		@return	strCharSet�ɂ��镶�����ŏ��ɏo������ʒu��Ԃ��B
				strCharSet�ɂ��镶��������������Ȃ������ꍇ�͏I�[�̈ʒu�i��NULL�����̈ʒu��str�̒����j��Ԃ��B
		@note	strcspn = String Complement Span �炵���B
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
		@brief	�����񂩂當���Z�b�g���ɖ�����������������֐��e���v���[�g�B
		@param	str	NULL�ŏI��錟���Ώە�����B
		@param	strCharSet	NULL�ŏI��镶���Z�b�g�B
		@return	strCharSet�ɖ����������ŏ��ɏo������ʒu��Ԃ��B
				strCharSet�ɖ���������������Ȃ������ꍇ�͏I�[�̈ʒu�i��NULL�����̈ʒu��str�̒����j��Ԃ��B
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
	// C�W�����C�u�����ɂ��肻���Ŗ���������֐��Q�B
		
	/*!
		@brief	�����Z�b�g�ɑ����镶������������ɍŌ�ɏo������ʒu�̃C���f�b�N�X��Ԃ��B
				strcspn�̖������猟���ŁB
		@tparam	CharType	�����^
		@param	str			NULL �ŏI��錟��������B
		@param	strCharSet	NULL �ŏI��镶���Z�b�g�B
		@return	strCharSet �ɑ����镶���� str �ɍŌ�ɏo������ʒu�̃C���f�b�N�X�B������Ȃ������ꍇ��-1�B
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
		@brief	�����񂩂����̕�������������֐��e���v���[�g�B
				������́A������������Ȃ������ꍇ�ɂ�NULL�ł͂Ȃ��I�[�����̈ʒu���w���|�C���^��Ԃ��B
				(gcc�̕W��C���C�u�����ɂ͖����֐�strchrnul�Ɠ�������)
		@param	str	NULL�ŏI��錟���Ώە�����B
		@param	c	���������B
		@return	c���ŏ��ɏo������ʒu���w���|�C���^��Ԃ��B
				c��������Ȃ������ꍇ�͏I�[�����̈ʒu���w���|�C���^��Ԃ��B
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
		@brief	��������ōŌ�ɏo�����錟��������ւ̃|�C���^��Ԃ��֐��e���v���[�g�B
		@param	str	NULL�ŏI��錟���Ώە�����B
		@param	strSearch	����������B
		@return	strSearch ���ŏ��ɏo������ʒu���w���|�C���^��Ԃ��B
				strSearch ��������Ȃ������ꍇ��NULL��Ԃ��B
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
		@brief	������̎��o�����s���֐��e���v���[�g�B
				strncpy �֐��ł́A�������̕����񒷂��R�s�[��������菭�Ȃ��ꍇ�ɁA�k�������̕t�����s��Ȃ����A
				���̊֐��ł̓k��������K���Ō�ɕt������B
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
	// �������̓N���X�B

	/*!
		@brief	�������̓N���X�B
				��؂蕶���̎w��ɉ����āA���p���i�N�H�[�e�[�V�����j���Ή����Ă���B
		@deprecated	boost::tokenizer���g�����B
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
			@brief	��͂��镶�����ݒ肷��B
					���܂ł̉�͏��̓��Z�b�g�����B
			@param	source	��͂��镶����B
			@param	delims	NULL�ŏI����؂蕶���Z�b�g�B
			@param	quots	NULL�ŏI�����p�������y�A�Z�b�g�B
							���p�J�n�����P�A���p�I�������P�A���p�J�n�����Q�A���p�I�������Q�c
							�Ƃ������Ɏw�肷��B����̕������n�����ꍇ�̓���͖���`�B
			@code
			Tokens<TCHAR> tokens;
			tokens.Set(_T("I love \"Romancing SaGa\" (Super Famicom software)"), _T(" "), _T("\"\"()")); 
			@endcode
		*/
		void Set(const CharType* source, const CharType* delims, const CharType* quots = EMPTY_STRING);

		/*!
			@brief	���̃g�[�N����Ԃ�
			@param[out]	token	�g�[�N��
			@
		*/
		bool Next(std::basic_string<CharType>& token);
		
	private:
		const CharType* _next;
		std::vector<CharType> _delims;
		std::vector<CharType> _quots;
		
	};	// end class Tokens
	
	// ������
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
			// ��؂蕶�������p������������
			_next = StrCSpn(token_begin, &_delims[0]) + token_begin;
			token_end = _next;
		}
		else
		{
			// ���p���Ŏn�܂��Ă����̂ŕ����p������������
			++token_begin;	// ���p���͕ԋp���Ȃ�
			const CharType* close_quot = found + 1;
			token_end = StrChr(token_begin, *close_quot);
			if (is_null(token_end))
			{
				// �����ĂȂ�
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

