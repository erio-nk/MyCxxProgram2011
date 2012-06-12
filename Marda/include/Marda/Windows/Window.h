/*!
	@file	Windows/Window.h
	
			�E�B���h�E�֘A�̑����֗��ɂ��邽�߂̊g���N���X�B
			�E�B���h�E�n���h���ƃ��b�Z�[�W�n���h���}�b�v�i�ƈȑO�̃E�B���h�E�v���V�[�W���j���������Ȃ��ׁA
			�O���ō쐬�����E�B���h�E�̃n���h����n���č쐬���鎖���o����B
			����̃E�B���h�E�n���h���ɑ΂��鑀����O������s�Ȃ��Ă��ǂ��B

	@author	E.Nakayama
*/
#pragma once

#include <functional>

#include "WindowMessageHelpers.h"

namespace Marda
{
	/*!
		@brief	�E�B���h�E�N���X�B
	*/
	class Window
	{
	public:
		// ��^�X�^�C��
		static const DWORD STANDARD_STYLE	= WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		static const DWORD FULLSCREEN_STYLE	= WS_POPUP | WS_VISIBLE;
		
		/*!
			@brief	�쐬���̃I�v�V�����t���O�B
		*/
		enum CREATE_OPTIONS
		{
			// ���w��
			SIZE_IS_WINDOW	= 0x0000,	//!< �w�肳�ꂽ�T�C�Y�̓E�B���h�E�̃T�C�Y
			SIZE_IS_CLIENT	= 0x0004,	//!< �w�肳�ꂽ�T�C�Y�̓N���C�A���g�̈�̃T�C�Y

			// �쐬���̔w�i�u���V
			BG_IS_NULL		= 0x0000,	//!< �w�i��NULL�u���V
			BG_IS_WHITE		= 0x0008,	//!< �w�i�͔��u���V

			// �E�B���h�E�̃Z���^�����O
			CENTERING		= 0x0010,
		};

		//! ���b�Z�[�W�n���h���^
		typedef 
			std::function<LRESULT (HWND /*hWnd*/, UINT /*msg*/, LPARAM /*lParam*/, WPARAM /*wParam*/)>
			MessageHandler;

		//! ctor
		Window();

		//! dtor
		~Window();
		
		/*!
			@brief	�E�B���h�E���쐬���AWindow �I�u�W�F�N�g������������B
		*/		
		bool Create(
			const TCHAR* caption,
			int x, int y,
			int w, int h,
			DWORD style,
			DWORD exStyle,
			DWORD option,
			LPVOID pParam = NULL
			);
		
		/*!
			@brief	�O���ō쐬�����E�B���h�E�̃n���h���� Window �I�u�W�F�N�g������������B
		*/
		bool Create(HWND hWnd);

		//void Destroy();
		
		/*! 
			@brief	�E�B���h�E�n���h�����擾����B
		*/
		HWND GetHandle() const;

		/*!
			@brief	���b�Z�[�W�n���h����o�^����B
					�E�B���h�E�쐬�O�iCreate�Ăяo���O�j�ɓo�^���Ă��ǂ��B
		*/
		void AddMessageHandler(UINT msg, MessageHandler handler);
		
		MessageHandler* GetMessageHandler(UINT msg) const;
		
		/*!
			@brief	�E�B���h�E�̃N���C�A���g�̈�̃T�C�Y���擾����B
			@param[out] sz	�N���C�A���g�̈�̃T�C�Y�B
		*/
		bool GetClientSize(SIZE& sz) const;
		
	private:
		struct Impl;
		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom
	};	// end class Window
}	// end namespace Marda
