/*!
 	@file DebugConsole.h
 
 	�f�o�b�O�p�R���\�[���B
 
 	@author Dila
 */
#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdarg.h>

#include "Windows/_MSVC.h"

#if defined(_DEBUG) && !defined(_NO_USE_DBGCONSOLE)
	#ifndef _USE_DBGCONSOLE
		#define _USE_DBGCONSOLE
	#endif
#endif

/*!
 	�f�o�b�O�p�R���\�[���N���X
 
 	Win32�A�v���P�[�V�����p�̃f�o�b�O�R���\�[���������N���X�B
 */
class DebugConsole
{
public:
	/*!
	 	�R���X�g���N�^
	 */
	DebugConsole();

	//! �f�X�g���N�^
	~DebugConsole();

	/*!
	 	�C���X�^���X���擾����
	 	@return	�f�o�b�O�R���\�[���I�u�W�F�N�g
	 */
	static DebugConsole& GetInstance();

	/*!
	 	�g���[�X
	 */
	void Trace(const TCHAR* format, ...);

	void Trace(const TCHAR* format, va_list& va);

};	// end class DebugConsole


/*!
 	�g���[�X�֐�
 	@see _MSVC.h
 */
#ifdef _USE_DBGCONSOLE
	#if _MSC_VER < _MSVC_VER_8
		// ���̂����l����
	#else
		#define _DBGTRACE(format, ...)	DebugConsole::GetInstance().Trace(_T(__FUNCTION__) _T("> ") format _T("\n"), __VA_ARGS__)
	#endif
#else
	#define _DBGTRACE	_noop
#endif
