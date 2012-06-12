/*!
	@file	Windows/WindowMessageParameter.h
	
			�E�B���h�E���b�Z�[�W�p�̃w���p�N���X�Q�B
			
	@author	E.Nakayama
*/
#pragma once

#include <functional>

namespace Marda
{
	struct WindowMessageParameters
	{
		WPARAM wParam;
		LPARAM lParam;
		WindowMessageParameters(WPARAM w, LPARAM l)
			: wParam(w)
			, lParam(l)
		{
		}
	};
	
	#define DECLARE_CONSTRUCTOR(name) name (WPARAM w, LPARAM l) : WindowMessageParameters(w, l)
	
	//! WM_CREATE ���b�Z�[�W�p�����[�^�B
	struct WindowCreateMessageParameters : public WindowMessageParameters
	{
		DECLARE_CONSTRUCTOR(WindowCreateMessageParameters) {}
		LPCREATESTRUCT GetCreateStruct() const { return (LPCREATESTRUCT)lParam; }
	};

	//! WM_SIZE ���b�Z�[�W�p�����[�^�B
	struct WindowSizeMessageParameters : public WindowMessageParameters
	{
		enum TYPE
		{
		};
		DECLARE_CONSTRUCTOR(WindowSizeMessageParameters) {}
		WPARAM GetType() const { return wParam; }
			/*!<
				SIZE_RESTORED	0	�E�B���h�E���T�C�Y�ύX����܂����B�������ŏ����܂��͍ő剻�ł͂���܂���B
				SIZE_MINIMIZED	1	�E�B���h�E���ŏ�������܂����B
				SIZE_MAXIMIZED	2	�E�B���h�E���ő剻����܂����B
				SIZE_MAXSHOW	3	���鑼�̃E�B���h�E�����̃T�C�Y�ɖ߂��ꂽ�Ƃ��A���ׂẴ|�b�v�A�b�v�E�B���h�E�ɑ����܂��B
				SIZE_MAXHIDE	4	���鑼�̃E�B���h�E���ő剻���ꂽ�Ƃ��A���ׂẴ|�b�v�A�b�v�E�B���h�E�ɑ����܂��B
			*/
		int GetClientWidth() const { return LOWORD(lParam); }
		int GetClientHeight() const { return HIWORD(lParam); }
	};
	
	//! WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MOUSEMOVE, WM_MOUSEWHEEL ���b�Z�[�W�p�����[�^�B
	struct WindowMouseMessageParameters : public WindowMessageParameters
	{
		DECLARE_CONSTRUCTOR(WindowMouseMessageParameters) {}
		int GetX() const { return LOWORD(lParam); }
		int GetY() const { return HIWORD(lParam); }
		int GetZ() const { return (short)HIWORD(wParam); }
		bool IsLButton() const { return (LOWORD(wParam) & MK_LBUTTON) != 0; }
		bool IsRButton() const { return (LOWORD(wParam) & MK_RBUTTON) != 0; }
				// LOWORD(wParam)���Ă�̂�WM_MOUSEWHEEL�p
	};

	//! WM_KEYDOWN, WM_KEYUP ���b�Z�[�W�p�����[�^�B
	struct WindowKeyMessageParameters : public WindowMessageParameters
	{
		DECLARE_CONSTRUCTOR(WindowKeyMessageParameters) {}
		
		//! ���z�L�[�R�[�h
		inline int GetVirtKey() const { return static_cast<int>(wParam); }
		//! �L�[�f�[�^
		inline int GetKeyData() const { return static_cast<int>(lParam); }
		//! ���s�[�g�J�E���g
		inline int GetRepeatCount() const { return static_cast<int>((lParam&0x0000ffff)); }
		//! �X�L�����R�[�h
		inline int GetScanCode() const { return static_cast<int>((lParam&0x00ff0000) >> 16); }
		//! �g���L�[�������ꂽ���ǂ���
		inline bool IsExtendKey() const { return (lParam&0x01000000) >> 24 != 0; }
		//! �R���e�L�X�g�R�[�h
		inline int GetContextCode() const { return static_cast<int>((lParam&0x20000000) >> 29); }
		//! ���O�̃L�[���
		inline int GetPrevState() const { return static_cast<int>((lParam&0x40000000) >> 30); }
		//! �����ꂽ��
		inline bool IsKeyDown() const { return (lParam&0x80000000) >> 31 == 0; }
		//! �����ꂽ��
		inline bool IsKeyUp() const { return (lParam&0x80000000) >> 31 != 0; }
	};

	#undef DECLARE_CONSTRUCTOR

}	// end namespace Marda
