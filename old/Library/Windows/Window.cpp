/*!
 	Windows/Window.cpp
 
 	@author	Dila
 */

#include "Windows/Window.h"
#include <stdio.h>

// ウィンドウ数
static unsigned int wndCnt = 0;

// ウィンドウプロパティ名
#define PROPERTY_NAME _T("WindowClassName")


/*
 	ウィンドウプロシージャ
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* pWnd = Window::GetObjectFrom(hWnd);

	if (pWnd == NULL) {
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	// OnMessage()呼び出し
	LRESULT lRet = pWnd->OnMessage(msg, wParam, lParam);

	// プロパティ削除
	if (msg == WM_DESTROY) {
		if (!RemoveProp(hWnd, PROPERTY_NAME)) {
			// エラー
		}
	}

	return lRet;
}

/*!
 	コンストラクタ
 
 */
Window::Window()
{
	hWnd = NULL;
	wndNum = wndCnt++;
}

/*!
 	デストラクタ
 
 */
Window::~Window()
{
	std::map<UINT, Window::MessageHandler*>::iterator i;
	i = msgMap.begin();
	while (i != msgMap.end()) {
		delete i->second;
		msgMap.erase(i);
		i = msgMap.begin();
	}
}


/*!
 	ウィンドウを生成する
 
 	@param	caption		タイトル
 	@param	x, y		座標
 	@param	w, h		幅
 	@param	option		オプションフラグ (Window::Option)
 	@param	pParam		パラメータ
 */
bool Window::Create(const TCHAR* caption,
					int x, int y, int w, int h,
					unsigned int option,
					LPVOID pParam)
{
	return Window::Create(caption, x, y, w, h, option, NULL, NULL, pParam);
}

/*!
 	ウィンドウを生成する
 
 	@param	caption		タイトル
 	@param	x, y		座標
 	@param	w, h		幅
 	@param	option		オプションフラグ (Window::Option)
 	@param	style		ウィンドウスタイル
 	@param	styleEx		拡張スタイル
 	@param	pParam		パラメータ
 	@todo 親ウィンドウとかメニュー対応。
 */
bool Window::Create(const TCHAR* caption,
					int x, int y, int w, int h,
					unsigned int option,
					DWORD style,
					DWORD styleEx,
					LPVOID pParam)
{
	if (hWnd != NULL) {
		return false;	// ウィンドウ生成済み
	}

	WNDCLASSEX wc;

	const TCHAR* className = GetClassName();

	HMENU hMenu = NULL;	// !とりあえずまた今度考えよう
	HWND hWndParent = NULL;	// !こっちもまた今度考えよう
    
	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);	// インスタンスハンドル取得
		
	// ウィンドウ生成の為の設定
	if (_tcsncmp(className, _T("_WINDOW"), _tcslen(_T("WINDOW"))) == 0) {
		// ウィンドウクラスの登録
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize		= sizeof(WNDCLASSEX);
		wc.hInstance	= hInst;
		wc.lpszClassName= className;
		wc.lpfnWndProc	= WndProc;
		wc.style		= CS_BYTEALIGNCLIENT;
		wc.hIcon		= LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor		= LoadCursor(NULL, IDC_ARROW);

		if (option & WindowOption::BGWhite) {
			wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		} else {
			wc.hbrBackground = NULL;
		}

		if (option & WindowOption::StandardStyle) {
			style |= WS_CAPTION|WS_SYSMENU;
		}
		if (option & WindowOption::FullscreenStyle) {
			style |= WS_POPUP;
		}

		if (!RegisterClassEx(&wc)) {
			return false;
		}
	}
	// コントロール生成の為の設定
	else{
		if (!GetClassInfoEx(hInst, className, &wc)) {
			return false;	// 無効なウィンドウクラス名
		}
	}

	// ウィンドウ矩形の調整
	if(option & WindowOption::ClientSize){
		RECT rc;	

		BOOL fMenu	= hMenu != NULL ? TRUE : FALSE;
		SetRect(&rc, 0, 0, w, h);

		if (!AdjustWindowRectEx(&rc, style, fMenu, styleEx)) {
			return false;
		}

		w = rc.right - rc.left;
		h = rc.bottom - rc.top;
	}

	// ウィンドウ位置のセンタリング
	if (option & WindowOption::Centering) {
		RECT rc;
		GetWindowRect(GetDesktopWindow(), &rc);
		x = ((rc.right - rc.left) - w) / 2;
		y = ((rc.bottom - rc.top) - h) / 2;
	}

	// ウィンドウの生成
	hWnd = CreateWindowEx(
		styleEx, className, caption,
		style, x, y, w, h,
		hWndParent, hMenu, hInst, pParam
	);

	if (hWnd == NULL) {
		return false;
	}

	// 親ウィンドウに自動で子ウィンドウクリップスタイルを付加する
	if (hWndParent != NULL && (style & WS_CHILD)) {
		if (!SetWindowLong(hWndParent, GWL_STYLE,
			GetWindowLong(hWndParent, GWL_STYLE)|WS_CLIPCHILDREN))
		{
			return false;
		}
	}

	// インスタンスのポインタをウィンドウハンドルに関連付ける
	if (!SetProp(hWnd, PROPERTY_NAME, (HANDLE)this)) {
		return false;
	}

	// WM_CREATEメッセージ送信
	CREATESTRUCT cs;	
	cs.dwExStyle = styleEx;
    cs.lpszClass = className;
    cs.lpszName  = caption;
    cs.style     = style;
    cs.x  = x;
    cs.y  = y;
    cs.cx = w;
    cs.cy = h;
    cs.hwndParent = hWndParent;
    cs.hMenu      = hMenu;
    cs.hInstance  = hInst;
    cs.lpCreateParams = pParam;

	SendMessage(hWnd, WM_CREATE, 0, (LPARAM)&cs);

	return TRUE;
}

/*!
 	ウィンドウを破壊する
 	@todo 処理未定。
 */
void Window::Destroy()
{
}

/*!
 	ウィンドウを可視状態にする
 
 */
bool Window::Show()
{
	return ShowWindow(hWnd, SW_SHOW) == TRUE;
}

/*!
 	ウィンドウを不可視状態にする
 
 */
bool Window::Hide()
{
	return ShowWindow(hWnd, SW_HIDE) == TRUE;
}

/*!
 	ウィンドウオプションを設定する
 	@param	option	Create()のオプションフラグ(WindowOption)
 */
bool Window::SetOption(unsigned int option)
{
	// そのうち作ります。
	return true;
}

/*!
 	ウィンドウスタイルを設定する
 	@todo そのうち作ります。
 */
bool Window::SetStyle(DWORD style)
{
	return true;
}

/*!
 	ウィンドウ拡張スタイルを設定する
 	@todo そのうち作ります。
 */
bool Window::SetStyleEx(DWORD styleEx)
{
	return true;
}

/*!
 	ウィンドウハンドルからWindowオブジェクトを取得する
 	@param	hWnd	ウィンドウハンドル
 	@return	Windowオブジェクト
 	@retval	NULL	取得失敗
 */
Window* Window::GetObjectFrom(HWND hWnd) {
	return (Window*)GetProp(hWnd, PROPERTY_NAME);
}


/*!
 *	ウィンドウクラス名を取得する
 *	@return	ウィンドウクラス名(WNDCLASSEX用)
 */
const TCHAR* Window::GetClassName()
{
	static TCHAR name[64];
	//_stprintf(name, _T("_WINDOW[%04x]"), wndNum);
	_stprintf(name, _T("_WINDOW[%04x%07x]"), wndNum, GetCurrentProcessId());
	return name;
}


/*!
 	メッセージハンドラ
 
 	独自のメッセージハンドルをする場合は、
 	AddMessageHandler<>()を用いる方法以外にも、
 	このメンバ関数をオーバーライドする方法もある。
 */
LRESULT Window::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::map<UINT, MessageHandler*>::iterator i = msgMap.find(msg);
	if (i != msgMap.end()) {
		i->second->pWnd = this;
		i->second->msg = msg;
		i->second->lParam = lParam;
		i->second->wParam = wParam;
		i->second->lResult = 0;
		if (i->second->Handle()) {	// ハンドラ実行
			return i->second->lResult;
		}
	}
	
	if (msg == WM_DESTROY) {
		// WM_DESTROYのデフォルト処理。
		// 親ウィンドウが無ければ（＝メインウィンドウなので）アプリケーション終了
		if ((LONG_PTR)GetWindowLongPtr(*this, GWL_HWNDPARENT) == NULL) {
			PostQuitMessage(0);
			return 0;
		}
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);;
}
