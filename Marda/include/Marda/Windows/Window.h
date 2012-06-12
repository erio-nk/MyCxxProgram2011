/*!
	@file	Windows/Window.h
	
			ウィンドウ関連の操作を便利にするための拡張クラス。
			ウィンドウハンドルとメッセージハンドラマップ（と以前のウィンドウプロシージャ）しか持たない為、
			外部で作成したウィンドウのハンドルを渡して作成する事も出来る。
			同一のウィンドウハンドルに対する操作を外部から行なっても良い。

	@author	E.Nakayama
*/
#pragma once

#include <functional>

#include "WindowMessageHelpers.h"

namespace Marda
{
	/*!
		@brief	ウィンドウクラス。
	*/
	class Window
	{
	public:
		// 定型スタイル
		static const DWORD STANDARD_STYLE	= WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		static const DWORD FULLSCREEN_STYLE	= WS_POPUP | WS_VISIBLE;
		
		/*!
			@brief	作成時のオプションフラグ。
		*/
		enum CREATE_OPTIONS
		{
			// 幅指定
			SIZE_IS_WINDOW	= 0x0000,	//!< 指定されたサイズはウィンドウのサイズ
			SIZE_IS_CLIENT	= 0x0004,	//!< 指定されたサイズはクライアント領域のサイズ

			// 作成時の背景ブラシ
			BG_IS_NULL		= 0x0000,	//!< 背景はNULLブラシ
			BG_IS_WHITE		= 0x0008,	//!< 背景は白ブラシ

			// ウィンドウのセンタリング
			CENTERING		= 0x0010,
		};

		//! メッセージハンドラ型
		typedef 
			std::function<LRESULT (HWND /*hWnd*/, UINT /*msg*/, LPARAM /*lParam*/, WPARAM /*wParam*/)>
			MessageHandler;

		//! ctor
		Window();

		//! dtor
		~Window();
		
		/*!
			@brief	ウィンドウを作成し、Window オブジェクトを初期化する。
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
			@brief	外部で作成したウィンドウのハンドルで Window オブジェクトを初期化する。
		*/
		bool Create(HWND hWnd);

		//void Destroy();
		
		/*! 
			@brief	ウィンドウハンドルを取得する。
		*/
		HWND GetHandle() const;

		/*!
			@brief	メッセージハンドラを登録する。
					ウィンドウ作成前（Create呼び出し前）に登録しても良い。
		*/
		void AddMessageHandler(UINT msg, MessageHandler handler);
		
		MessageHandler* GetMessageHandler(UINT msg) const;
		
		/*!
			@brief	ウィンドウのクライアント領域のサイズを取得する。
			@param[out] sz	クライアント領域のサイズ。
		*/
		bool GetClientSize(SIZE& sz) const;
		
	private:
		struct Impl;
		std::auto_ptr<Impl> _pimpl;	// pimpl Idiom
	};	// end class Window
}	// end namespace Marda
