/*!
 	@file Windows/Window.h
 
 	ウィンドウクラス。
 
 	@author	Dila
 */
//! @addtogroup Windows Windows用クラス
//! @{
//! @addtogroup Window ウィンドウ関連クラス
//! @{

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x500	// Win2k以上 for WM_MOUSEWHEEL
#endif
#include <windows.h>
#include <tchar.h>
#include <map>	// for message map

/*!
 	ウィンドウクラス
 
 	メッセージハンドラを追加する場合は、
 	Window::MessageHandlerを継承したサブクラスを定義し、
 	AddMessageHandler<>()でハンドラを追加する。
 	OnMessage()をオーバーライドしても良い。
 
 	@note	ウィンドウプロシージャを隠す為に作ったは良いが･･･
 			逆に利用者のコード量が増えてしまった気がしまくる。
 */
class Window {
public:
	// 前方宣言
	class MessageHandler;

	/*!
	 	Window::Create()のオプションフラグ
	 */
	struct Option {
		enum {
			// ウィンドウスタイル
			StandardStyle	= 0x0001,	//! 一般的なウィンドウスタイル
			FullscreenStyle	= 0x0002,	//! フルスクリーン用ウィンドウスタイル

			// 幅指定
			WindowSize	= 0x0000,	//! 指定されたサイズはウィンドウのサイズ
			ClientSize	= 0x0004,	//! 指定されたサイズはクライアント領域のサイズ

			// 背景ブラシ
			BGNull		= 0x0000,	//! 背景はNULLブラシ
			BGWhite		= 0x0008,	//! 背景は白ブラシ

			// ウィンドウのセンタリング
			Centering	= 0x0010,	//! ウィンドウをセンタリングする
		};
	};

private:
	//!/ メンバ変数
	HWND hWnd;		// ウィンドウハンドル
	int wndNum;		// ウィンドウ番号

	std::map<UINT, class MessageHandler*> msgMap;	// メッセージマップ

public:
	//!/ メンバ関数
	Window();
	virtual ~Window();
	
	// 作る
	bool Create(
		const TCHAR* caption,	// タイトル
		int x, int y,			// 座標
		int w, int h,			// 幅
		unsigned int option		// オプション (WindowOption)
			= Option::StandardStyle|Option::ClientSize|Option::Centering,	// (Dilaが)よく使うオプション
		LPVOID pParam = NULL	// パラメータ
		);
	bool Create(
		const TCHAR* caption,
		int x, int y,
		int w, int h,
		unsigned int option,
		DWORD style,		// スタイル
		DWORD styleEx,		// 拡張スタイル
		LPVOID pParam = NULL
		);

	void Destroy();	// 壊す

	// 見せる
	bool Show();
	bool Hide();	// 隠す

	//! ウィンドウハンドルを取得する
	HWND GetHandle() { return hWnd; }

	//! HWND型にキャストする
	operator HWND() { return hWnd; }

	// スタイル・オプション
	bool SetOption(unsigned int option);
	bool SetStyle(DWORD style);
	bool SetStyleEx(DWORD styleEx);

	// オブジェクト
	static Window* GetObjectFrom(HWND hWnd);

	// ウィンドウクラス名
	virtual const TCHAR* GetClassName();

	// ウィンドウメッセージハンドラ
	virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	/*!
	 	メッセージとメッセージハンドラを関連付ける
	 	@tparam	MessageHandlerClass	Window::MessageHandlerクラスのサブクラス
	 	@param	msg	関連付けるウィンドウメッセージ(WM_XXX)
	 */
	template <class MessageHandlerClass>
	bool AddMessageHandler(UINT msg)
	{
		std::map<UINT, class MessageHandler*>::iterator i;
		i = msgMap.find(msg);
		if (i != msgMap.end()) {
			delete i->second;
			msgMap.erase(i);
		}
		msgMap.insert(std::make_pair(msg, new MessageHandlerClass()));
			// new/deleteのスコープをWindowクラススコープに収める
		return true;
	}

	/*!
	 	メッセージハンドラベースクラス
	 
	 	Windowクラスのメッセージをハンドリングするには、
	 	このクラスを継承し、Handleをオーバーライドした
	 	サブクラスの型をテンプレート引数に指定して
	 	Window::AddMessageHandler<>()をコールする。
	 
	 	パラメータ解析が複雑なウィンドウメッセージは、
	 	規定のハンドラクラス(OnCreate,OnMouseMessage)を
	 	継承すれば、パラメータ解析済みのデータを取得する
	 	アクセサが提供される。
	 
	 	サンプル
	 	@code
	 		// マウス左ボタンイベントハンドラ
	 		struct OnLButtonDown : public Window::OnMouseMessage
	 		{
	 			bool Handle() { ... }
	 		};
	 
	 		Window wnd;
	 		wnd.AddMessageHandler<OnLButtonDown>(WM_LBUTTONDOWN);
	 	@endcode
	 */
	class MessageHandler 
	{
	protected:
		Window* pWnd;	//! メッセージをハンドルしたウィンドウ
		UINT msg;		//! メッセージ
		LPARAM lParam;	//! Lパラメータ
		WPARAM wParam;	//! Wパラメータ
		LRESULT lResult;	//! 処理結果
		friend class Window;
	public:
		/*!
		 	メッセージハンドル時のイベント
		 	@retval	true	これ以上メッセージを処理しない
		 	@retval	false	デフォルトのメッセージ処理を呼ぶ
		 */
		virtual bool Handle() = 0;
	};

	/*!
	 	WM_CREATEメッセージハンドラベースクラス
	 */
	struct OnCreate : public MessageHandler
	{
		LPCREATESTRUCT GetCreateStruct() { (LPCREATESTRUCT)lParam; }
	};

	/*!
	 	マウスメッセージハンドラベースクラス
	 
	 	対応メッセージは以下の通り。
		WM_LBUTTONDOWN, WM_LBUTTONUP,
	 	WM_RBUTTONDOWN, WM_RBUTTONUP,
	 	WM_MOUSEMOVE, WM_MOUSEWHEEL
	 */
	struct OnMouseMessage : public MessageHandler
	{
		int GetX() { return LOWORD(lParam); }
		int GetY() { return HIWORD(lParam); }
		int GetZ() { return (short)HIWORD(wParam); }
		bool IsLButton() { return (LOWORD(wParam) & MK_LBUTTON) != 0; }
		bool IsRButton() { return (LOWORD(wParam) & MK_RBUTTON) != 0; }
			// LOWORD(wParam)してるのはWM_MOUSEWHEEL用
	};

	/*!
	 	キーメッセージハンドラベースクラス
	 
	 	対応メッセージは以下の通り。
	 	WM_KEYDOWN, WM_KEYUP
	 */
	struct OnKeyMessage : public MessageHandler
	{
		//! 仮想キーコード
		int GetVirtKey() { return static_cast<int>(wParam); }
		//! キーデータ
		int GetKeyData() { return static_cast<int>(lParam); }
		//! リピートカウント
		int GetRepeatCount(){ return static_cast<int>((lParam&0x0000ffff)); }
		//! スキャンコード
		int GetScanCode()	{ return static_cast<int>((lParam&0x00ff0000) >> 16); }
		//! 拡張キーが押されたかどうか
		bool IsExtendKey()	{ return (lParam&0x01000000) >> 24 != 0; }
		//! コンテキストコード
		int GetContextCode(){ return static_cast<int>((lParam&0x20000000) >> 29); }
		//! 直前のキー状態
		int GetPrevState()	{ return static_cast<int>((lParam&0x40000000) >> 30); }
		//! 押されたか
		bool IsKeyDown()	{ return (lParam&0x80000000) >> 31 == 0; }
		//! 離されたか
		bool IsKeyUp()		{ return (lParam&0x80000000) >> 31 != 0; }
	};
};	// end class Window


//! @deprecated	Window::Optionに置き換えて下さい。
namespace WindowOption {
	/*!
	 	Window::Create()のオプションフラグ
	 	@todo	グローバルネームスペースでの宣言は止めたい･･･
	 */
	enum _WindowOption {
		// ウィンドウスタイル
		StandardStyle	= 0x0001,	//! 一般的なウィンドウスタイル
		FullscreenStyle	= 0x0002,	//! フルスクリーン用ウィンドウスタイル

		// 幅指定
		WindowSize	= 0x0000,	//! 指定されたサイズはウィンドウのサイズ
		ClientSize	= 0x0004,	//! 指定されたサイズはクライアント領域のサイズ

		// 背景ブラシ
		BGNull		= 0x0000,	//! 背景はNULLブラシ
		BGWhite		= 0x0008,	//! 背景は白ブラシ

		// ウィンドウのセンタリング
		Centering	= 0x0010,	//! ウィンドウをセンタリングする
	};
};


//! @}
//! @}
