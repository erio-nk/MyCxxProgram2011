/*!
 	@file Windows/Main.h
 
 	Windows�A�v���P�[�V�����pMain�֐�
 
 	Windows�A�v���P�[�V�����\�z�҂́A
 	�ʏ�̃R���\�[���A�v���P�[�V�������l��
 	Main�֐����������邾���ŗǂ��B
 
 	@author	Dila
 */
//! @addtogroup Windows
//! @{
//!		@addtogroup Main Main�֐�
//!		@{

#pragma once

#include <windows.h>


/*!
 	���C���֐�
 
 	���p�҂͂��̊֐���K���������鎖�B
 	�C���X�^���X�n���h���̎擾��::GetModuleHandle(NULL)�ŁA
 	�R�}���h���C���̎擾��::GetCommandLine()�ōs���B
 */
void Main();


/*!
 	���b�Z�[�W���[�v�֐�
 
 	���b�Z�[�W���[�v���ȗ������������B
 	Main�֐����ŌĂяo���B
 */
void MessageLoop();


/*!
 	���C�����[�v�֐�
 
 	���C�����[�v���ȗ������������B
 	Main�֐����ŌĂяo���B
 	������͊֐��y�уN���X���\�b�h�p�B
 
 	@param	pfnRun	���C�����[�v�ŌĂяo���֐��B
 */
void MainLoop(void (*pfnRun)());


/*!
 	���C�����[�v�֐�
 
 	���C�����[�v���ȗ������������B
 	Main�֐����ŌĂяo���B
 	������͊֐��y�уC���X�^���X���\�b�h�p�B
 
 	@tparam	T		���C�����[�v�p�̃C���X�^���X���\�b�h���������N���X�^�B
 	@param	pObj	pfnRun���Ăяo���I�u�W�F�N�g�B
 	@param	pfnRun	���C�����[�v�ŌĂяo���C���X�^���X���\�b�h�B
 */
#pragma warning(push)
#pragma warning(disable: 4127)	// for while(true)

template <class T>
void MainLoop(T* pObj, void (T::*pfnRun)())
{
	MSG msg;

	while (true) {
		if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			(pObj->*pfnRun)();
		}
	}
}

#pragma warning(pop)

//!		@}
//!	@}

