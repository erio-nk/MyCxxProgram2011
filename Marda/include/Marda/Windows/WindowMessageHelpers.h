/*!
	@file	Windows/WindowMessageParameter.h
	
			ウィンドウメッセージ用のヘルパクラス群。
			
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
	
	//! WM_CREATE メッセージパラメータ。
	struct WindowCreateMessageParameters : public WindowMessageParameters
	{
		DECLARE_CONSTRUCTOR(WindowCreateMessageParameters) {}
		LPCREATESTRUCT GetCreateStruct() const { return (LPCREATESTRUCT)lParam; }
	};

	//! WM_SIZE メッセージパラメータ。
	struct WindowSizeMessageParameters : public WindowMessageParameters
	{
		enum TYPE
		{
		};
		DECLARE_CONSTRUCTOR(WindowSizeMessageParameters) {}
		WPARAM GetType() const { return wParam; }
			/*!<
				SIZE_RESTORED	0	ウィンドウがサイズ変更されました。ただし最小化または最大化ではありません。
				SIZE_MINIMIZED	1	ウィンドウが最小化されました。
				SIZE_MAXIMIZED	2	ウィンドウが最大化されました。
				SIZE_MAXSHOW	3	ある他のウィンドウが元のサイズに戻されたとき、すべてのポップアップウィンドウに送られます。
				SIZE_MAXHIDE	4	ある他のウィンドウが最大化されたとき、すべてのポップアップウィンドウに送られます。
			*/
		int GetClientWidth() const { return LOWORD(lParam); }
		int GetClientHeight() const { return HIWORD(lParam); }
	};
	
	//! WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MOUSEMOVE, WM_MOUSEWHEEL メッセージパラメータ。
	struct WindowMouseMessageParameters : public WindowMessageParameters
	{
		DECLARE_CONSTRUCTOR(WindowMouseMessageParameters) {}
		int GetX() const { return LOWORD(lParam); }
		int GetY() const { return HIWORD(lParam); }
		int GetZ() const { return (short)HIWORD(wParam); }
		bool IsLButton() const { return (LOWORD(wParam) & MK_LBUTTON) != 0; }
		bool IsRButton() const { return (LOWORD(wParam) & MK_RBUTTON) != 0; }
				// LOWORD(wParam)してるのはWM_MOUSEWHEEL用
	};

	//! WM_KEYDOWN, WM_KEYUP メッセージパラメータ。
	struct WindowKeyMessageParameters : public WindowMessageParameters
	{
		DECLARE_CONSTRUCTOR(WindowKeyMessageParameters) {}
		
		//! 仮想キーコード
		inline int GetVirtKey() const { return static_cast<int>(wParam); }
		//! キーデータ
		inline int GetKeyData() const { return static_cast<int>(lParam); }
		//! リピートカウント
		inline int GetRepeatCount() const { return static_cast<int>((lParam&0x0000ffff)); }
		//! スキャンコード
		inline int GetScanCode() const { return static_cast<int>((lParam&0x00ff0000) >> 16); }
		//! 拡張キーが押されたかどうか
		inline bool IsExtendKey() const { return (lParam&0x01000000) >> 24 != 0; }
		//! コンテキストコード
		inline int GetContextCode() const { return static_cast<int>((lParam&0x20000000) >> 29); }
		//! 直前のキー状態
		inline int GetPrevState() const { return static_cast<int>((lParam&0x40000000) >> 30); }
		//! 押されたか
		inline bool IsKeyDown() const { return (lParam&0x80000000) >> 31 == 0; }
		//! 離されたか
		inline bool IsKeyUp() const { return (lParam&0x80000000) >> 31 != 0; }
	};

	#undef DECLARE_CONSTRUCTOR

}	// end namespace Marda
