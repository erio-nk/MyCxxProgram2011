/*!
	@file	Windows/Window.h

	@author	E.Nakayama
*/

#include "Marda/Windows/Window.h"

using namespace Marda;

namespace
{
	// ウィンドウ数
	static unsigned int wndCnt = 0;

	// ウィンドウプロパティ名
	#define PROPERTY_NAME _T("WindowClassName")
		
	// ウィンドウクラス名のプリフィックス
	#define WINDOW_CLASS_NAME_PREFIX _T("Marda.Window")

	//! ウィンドウクラス名を取得する
	void GetWindowClassName(TCHAR* buf, size_t cchBuf)
	{
		_stprintf_s(buf, cchBuf, WINDOW_CLASS_NAME_PREFIX _T("[%04x%07x]"), wndCnt++, GetCurrentProcessId());
	}

}	// end namespace

// ----------------------------------------------------------------------------
// Window クラスの実装部
struct Window::Impl
{
	HWND _hWnd;
	WNDPROC _wndProcPrev;
	
	typedef std::map<UINT, MessageHandler> MessageHandlerMap;
	MessageHandlerMap _handlers;
	
	Impl()
		: _hWnd(NULL)
		, _wndProcPrev(NULL)
	{
		using namespace std::placeholders;
		AddMessageHandler(WM_DESTROY, std::bind(&Impl::CloseMessageHandlerDefault, this, _1, _2, _3, _4));
	}
	
	~Impl()
	{
	}
	
	// ウィンドウを作成し、Window オブジェクトを初期化する。
	bool Create(
		const TCHAR* caption,
		int x, int y,
		int w, int h,
		DWORD style,
		DWORD exStyle,
		DWORD option,
		LPVOID pParam
		)
	{
		if (is_not_null(_hWnd)) 
		{
			return false;	// ウィンドウ生成済み
		}

		WNDCLASSEX wc;

		TCHAR className[64];
		GetWindowClassName(className, _countof(className));

		HMENU hMenu = NULL;	// TODO:
		HWND hWndParent = NULL;	// TODO:
	    
		HINSTANCE hInst = static_cast<HINSTANCE>(GetModuleHandle(NULL));	// インスタンスハンドル取得
			
		// ウィンドウ生成の為の設定
		if (_tcsncmp(className, WINDOW_CLASS_NAME_PREFIX, STATIC_STR_LEN(WINDOW_CLASS_NAME_PREFIX)) == 0) 
		{
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

			if (option & BG_IS_WHITE) 
			{
				wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
			} 
			else 
			{
				wc.hbrBackground = NULL;
			}

			if (!RegisterClassEx(&wc)) 
			{
				return false;
			}
		}
		// コントロール生成の為の設定
		else
		{
			if (!GetClassInfoEx(hInst, className, &wc)) 
			{
				return false;	// 無効なウィンドウクラス名
			}
		}

		// ウィンドウ矩形の調整
		if (option & SIZE_IS_CLIENT)
		{
			RECT rc;	

			BOOL fMenu	= hMenu != NULL ? TRUE : FALSE;
			SetRect(&rc, 0, 0, w, h);

			if (!AdjustWindowRectEx(&rc, style, fMenu, exStyle)) 
			{
				return false;
			}

			w = rc.right - rc.left;
			h = rc.bottom - rc.top;
		}

		// ウィンドウ位置のセンタリング
		if (option & CENTERING) 
		{
			RECT rc;
			GetWindowRect(GetDesktopWindow(), &rc);
			x = ((rc.right - rc.left) - w) / 2;
			y = ((rc.bottom - rc.top) - h) / 2;
		}

		// ウィンドウの生成
		_hWnd = CreateWindowEx(
			exStyle, className, caption,
			style, x, y, w, h,
			hWndParent, hMenu, hInst, pParam
			);

		if (is_null(_hWnd)) 
		{
			return false;
		}

		// 親ウィンドウに自動で子ウィンドウクリップスタイルを付加する
		if (hWndParent != NULL && (style & WS_CHILD)) 
		{
			if (!SetWindowLong(hWndParent, GWL_STYLE, GetWindowLong(hWndParent, GWL_STYLE)|WS_CLIPCHILDREN))
			{
				return false;
			}
		}

		// インスタンスのポインタをウィンドウハンドルに関連付ける
		if (!SetProp(_hWnd, PROPERTY_NAME, reinterpret_cast<HANDLE>(this))) 
		{
			return false;
		}

		// WM_CREATEメッセージ送信
		CREATESTRUCT cs;	
		cs.dwExStyle = exStyle;
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

		SendMessage(_hWnd, WM_CREATE, 0, (LPARAM)&cs);

		return true;
	}
	
	// 外部で作成したウィンドウのハンドルで Window オブジェクトを初期化する。
	bool Create(HWND hWnd)
	{
		if (is_not_null(_hWnd)) 
		{
			return false;	// ウィンドウ生成済み
		}
		
		_hWnd = hWnd;
		
		// ウィンドウプロシージャを上書き（サブクラス化）する。
		SetLastError(0);
		_wndProcPrev = reinterpret_cast<WNDPROC>(SetWindowLongPtr(_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));
		if (is_null(_wndProcPrev) && GetLastError() != 0)
		{
			return false;
		}
		
		// インスタンスのポインタをウィンドウハンドルに関連付ける
		if (!SetProp(_hWnd, PROPERTY_NAME, reinterpret_cast<HANDLE>(this))) 
		{
			return false;
		}
		
		return true;
	}
	
	HWND GetHandle()
	{
		return _hWnd;
	}
	
	void AddMessageHandler(UINT msg, MessageHandler handler)
	{
		_handlers[msg] = handler;
	}
	
	MessageHandler* GetMessageHandler(UINT msg)
	{
		MessageHandlerMap::iterator found = _handlers.find(msg);
		if (found == _handlers.end())
		{
			return NULL;
		}
		return &found->second;
	}
	
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Impl* pThis = reinterpret_cast<Impl*>(GetProp(hWnd, PROPERTY_NAME));
		if (is_null(pThis))
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		// OnMessage()呼び出し。
		LRESULT lRet = 0;
		Window::MessageHandler* pHandler = pThis->GetMessageHandler(msg);
		if (!is_null(pHandler))
		{
			lRet = (*pHandler)(hWnd, msg, wParam, lParam);
		}
		else
		{
			// メッセージハンドラが無い場合。
			if (!is_null(pThis->_wndProcPrev))
			{
				// 前のウィンドウプロシージャがあればそちらを呼び出す。
				lRet = CallWindowProc(pThis->_wndProcPrev, hWnd, msg, wParam, lParam);
			}
			else
			{
				// 無ければデフォルト処理。
				lRet = DefWindowProc(hWnd, msg, wParam, lParam);
			}
		}

		// ウィンドウを破棄する場合は、プロパティを削除する。
		if (msg == WM_DESTROY) 
		{
			if (!RemoveProp(hWnd, PROPERTY_NAME)) 
			{
				// エラー
			}
		}

		return lRet;
	}
	
	// デフォルトのWM_DESTROYメッセージハンドラ
	LRESULT CloseMessageHandlerDefault(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_DESTROY) 
		{
			// WM_DESTROYのデフォルト処理。
			// 親ウィンドウが無ければ（＝メインウィンドウなので）アプリケーション終了
			if (is_null(GetWindowLongPtr(hWnd, GWLP_HWNDPARENT))) 
			{
				PostQuitMessage(0);
				return 0;
			}
		}
		
		return DefWindowProc(hWnd, msg, wParam, lParam);;
	}

	bool GetClientSize(SIZE& sz)
	{
		RECT rc;
		if (failed(GetClientRect(_hWnd, &rc)))
		{
			return false;
		}
		sz.cx = rc.right - rc.left;
		sz.cy = rc.bottom - rc.top;
		return true;
	}

};	// end Window::Impl

// ----------------------------------------------------------------------------
// Window クラスの転送部
Window::Window()
	: _pimpl(new Impl())
{
}

Window::~Window()
{
}

bool 
Window::Create(
	const TCHAR* caption,
	int x, int y,
	int w, int h,
	DWORD style,
	DWORD exStyle,
	DWORD option,
	LPVOID pParam
	)
{
	return _pimpl->Create(
		caption,
		x, y,
		w, h,
		style,
		exStyle,
		option,
		pParam
		);
}

bool
Window::Create(HWND hWnd)
{
	return _pimpl->Create(hWnd);
}

HWND 
Window::GetHandle() const
{
	return _pimpl->GetHandle();
}

void 
Window::AddMessageHandler(UINT msg, MessageHandler handler)
{
	return _pimpl->AddMessageHandler(msg, handler);
}
		
Window::MessageHandler* 
Window::GetMessageHandler(UINT msg) const
{
	return _pimpl->GetMessageHandler(msg);
}

bool
Window::GetClientSize(SIZE& sz) const
{
	return _pimpl->GetClientSize(sz);
}
