/*!
	@file DebugTrace.h

	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */
#pragma once

namespace Marda
{
#if (defined(_DEBUG) || defined(USE_DBGTRACE)) && !defined(NO_DBGTRACE)

	/*!
		�f�o�b�O�g���[�X(DBGTRACE)

		�f�o�b�O�p�̃g���[�X�o�͂��s���֐��B
		Debug�r���h���y��USE_DBGTRACE�}�N����`���ɏo�͂����B
		Release�r���h����������NO_DBGTRACE�}�N����`���ɂ͏o�͂��ꂸ�A
		�R�[�h���̂�__noop�ɒu����������B

		Windows�A�v���P�[�V�����ł̗��p���́A�ŏ��̃g���[�X�Ăяo������
		�R���\�[���̏��������s���Ă���o�͂����B

		@code
			DBGTRACE(_T("Message, p1=%, p2=%s"), 10, _T("hoge"));
		@endcode

		@param	format	�t�H�[�}�b�g������
	 */
	void DebugTrace(const TCHAR* format, ...);

	#if MSVC_VER >= MSVC_9
		#define DBGTRACE(format, ...)	Marda::DebugTrace(_T("%-20s> ") format _T("\n"), _T(__FUNCTION__), __VA_ARGS__)
	#else
		#error // TODO: �����l����H	Marda::DebugPreTrace(_T("%-20s> ", _T(__FUNCTION__)); Marda::DebugTrace	// \n��DebugTrace����
	#endif

#else
	#define DBGTRACE(...) __noop
#endif

}	// end namespace Marda
