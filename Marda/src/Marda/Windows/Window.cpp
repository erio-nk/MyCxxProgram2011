/*!
	@file	Windows/Window.h

	@author	E.Nakayama
*/

#include "Marda/Windows/Window.h"

using namespace Marda;

namespace
{
	// �E�B���h�E��
	static unsigned int wndCnt = 0;

	// �E�B���h�E�v���p�e�B��
	#define PROPERTY_NAME _T("WindowClassName")
		
	// �E�B���h�E�N���X���̃v���t�B�b�N�X
	#define WINDOW_CLASS_NAME_PREFIX _T("Marda.Window")

	//! �E�B���h�E�N���X�����擾����
	void GetWindowClassName(TCHAR* buf, size_t cchBuf)
	{
		_stprintf_s(buf, cchBuf, WINDOW_CLASS_NAME_PREFIX _T("[%04x%07x]"), wndCnt++, GetCurrentProcessId());
	}

}	// end namespace

// ----------------------------------------------------------------------------
// Window �N���X�̎�����
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
	
	// �E�B���h�E���쐬���AWindow �I�u�W�F�N�g������������B
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
			return false;	// �E�B���h�E�����ς�
		}

		WNDCLASSEX wc;

		TCHAR className[64];
		GetWindowClassName(className, _countof(className));

		HMENU hMenu = NULL;	// TODO:
		HWND hWndParent = NULL;	// TODO:
	    
		HINSTANCE hInst = static_cast<HINSTANCE>(GetModuleHandle(NULL));	// �C���X�^���X�n���h���擾
			
		// �E�B���h�E�����ׂ̈̐ݒ�
		if (_tcsncmp(className, WINDOW_CLASS_NAME_PREFIX, STATIC_STR_LEN(WINDOW_CLASS_NAME_PREFIX)) == 0) 
		{
			// �E�B���h�E�N���X�̓o�^
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
		// �R���g���[�������ׂ̈̐ݒ�
		else
		{
			if (!GetClassInfoEx(hInst, className, &wc)) 
			{
				return false;	// �����ȃE�B���h�E�N���X��
			}
		}

		// �E�B���h�E��`�̒���
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

		// �E�B���h�E�ʒu�̃Z���^�����O
		if (option & CENTERING) 
		{
			RECT rc;
			GetWindowRect(GetDesktopWindow(), &rc);
			x = ((rc.right - rc.left) - w) / 2;
			y = ((rc.bottom - rc.top) - h) / 2;
		}

		// �E�B���h�E�̐���
		_hWnd = CreateWindowEx(
			exStyle, className, caption,
			style, x, y, w, h,
			hWndParent, hMenu, hInst, pParam
			);

		if (is_null(_hWnd)) 
		{
			return false;
		}

		// �e�E�B���h�E�Ɏ����Ŏq�E�B���h�E�N���b�v�X�^�C����t������
		if (hWndParent != NULL && (style & WS_CHILD)) 
		{
			if (!SetWindowLong(hWndParent, GWL_STYLE, GetWindowLong(hWndParent, GWL_STYLE)|WS_CLIPCHILDREN))
			{
				return false;
			}
		}

		// �C���X�^���X�̃|�C���^���E�B���h�E�n���h���Ɋ֘A�t����
		if (!SetProp(_hWnd, PROPERTY_NAME, reinterpret_cast<HANDLE>(this))) 
		{
			return false;
		}

		// WM_CREATE���b�Z�[�W���M
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
	
	// �O���ō쐬�����E�B���h�E�̃n���h���� Window �I�u�W�F�N�g������������B
	bool Create(HWND hWnd)
	{
		if (is_not_null(_hWnd)) 
		{
			return false;	// �E�B���h�E�����ς�
		}
		
		_hWnd = hWnd;
		
		// �E�B���h�E�v���V�[�W�����㏑���i�T�u�N���X���j����B
		SetLastError(0);
		_wndProcPrev = reinterpret_cast<WNDPROC>(SetWindowLongPtr(_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc)));
		if (is_null(_wndProcPrev) && GetLastError() != 0)
		{
			return false;
		}
		
		// �C���X�^���X�̃|�C���^���E�B���h�E�n���h���Ɋ֘A�t����
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
	
	// �E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Impl* pThis = reinterpret_cast<Impl*>(GetProp(hWnd, PROPERTY_NAME));
		if (is_null(pThis))
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		// OnMessage()�Ăяo���B
		LRESULT lRet = 0;
		Window::MessageHandler* pHandler = pThis->GetMessageHandler(msg);
		if (!is_null(pHandler))
		{
			lRet = (*pHandler)(hWnd, msg, wParam, lParam);
		}
		else
		{
			// ���b�Z�[�W�n���h���������ꍇ�B
			if (!is_null(pThis->_wndProcPrev))
			{
				// �O�̃E�B���h�E�v���V�[�W��������΂�������Ăяo���B
				lRet = CallWindowProc(pThis->_wndProcPrev, hWnd, msg, wParam, lParam);
			}
			else
			{
				// ������΃f�t�H���g�����B
				lRet = DefWindowProc(hWnd, msg, wParam, lParam);
			}
		}

		// �E�B���h�E��j������ꍇ�́A�v���p�e�B���폜����B
		if (msg == WM_DESTROY) 
		{
			if (!RemoveProp(hWnd, PROPERTY_NAME)) 
			{
				// �G���[
			}
		}

		return lRet;
	}
	
	// �f�t�H���g��WM_DESTROY���b�Z�[�W�n���h��
	LRESULT CloseMessageHandlerDefault(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_DESTROY) 
		{
			// WM_DESTROY�̃f�t�H���g�����B
			// �e�E�B���h�E��������΁i�����C���E�B���h�E�Ȃ̂Łj�A�v���P�[�V�����I��
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
// Window �N���X�̓]����
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
