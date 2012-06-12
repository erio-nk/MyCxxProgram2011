/*!
 	Windows/Window.cpp
 
 	@author	Dila
 */

#include "Windows/Window.h"
#include <stdio.h>

// �E�B���h�E��
static unsigned int wndCnt = 0;

// �E�B���h�E�v���p�e�B��
#define PROPERTY_NAME _T("WindowClassName")


/*
 	�E�B���h�E�v���V�[�W��
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* pWnd = Window::GetObjectFrom(hWnd);

	if (pWnd == NULL) {
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	// OnMessage()�Ăяo��
	LRESULT lRet = pWnd->OnMessage(msg, wParam, lParam);

	// �v���p�e�B�폜
	if (msg == WM_DESTROY) {
		if (!RemoveProp(hWnd, PROPERTY_NAME)) {
			// �G���[
		}
	}

	return lRet;
}

/*!
 	�R���X�g���N�^
 
 */
Window::Window()
{
	hWnd = NULL;
	wndNum = wndCnt++;
}

/*!
 	�f�X�g���N�^
 
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
 	�E�B���h�E�𐶐�����
 
 	@param	caption		�^�C�g��
 	@param	x, y		���W
 	@param	w, h		��
 	@param	option		�I�v�V�����t���O (Window::Option)
 	@param	pParam		�p�����[�^
 */
bool Window::Create(const TCHAR* caption,
					int x, int y, int w, int h,
					unsigned int option,
					LPVOID pParam)
{
	return Window::Create(caption, x, y, w, h, option, NULL, NULL, pParam);
}

/*!
 	�E�B���h�E�𐶐�����
 
 	@param	caption		�^�C�g��
 	@param	x, y		���W
 	@param	w, h		��
 	@param	option		�I�v�V�����t���O (Window::Option)
 	@param	style		�E�B���h�E�X�^�C��
 	@param	styleEx		�g���X�^�C��
 	@param	pParam		�p�����[�^
 	@todo �e�E�B���h�E�Ƃ����j���[�Ή��B
 */
bool Window::Create(const TCHAR* caption,
					int x, int y, int w, int h,
					unsigned int option,
					DWORD style,
					DWORD styleEx,
					LPVOID pParam)
{
	if (hWnd != NULL) {
		return false;	// �E�B���h�E�����ς�
	}

	WNDCLASSEX wc;

	const TCHAR* className = GetClassName();

	HMENU hMenu = NULL;	// !�Ƃ肠�����܂����x�l���悤
	HWND hWndParent = NULL;	// !���������܂����x�l���悤
    
	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);	// �C���X�^���X�n���h���擾
		
	// �E�B���h�E�����ׂ̈̐ݒ�
	if (_tcsncmp(className, _T("_WINDOW"), _tcslen(_T("WINDOW"))) == 0) {
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
	// �R���g���[�������ׂ̈̐ݒ�
	else{
		if (!GetClassInfoEx(hInst, className, &wc)) {
			return false;	// �����ȃE�B���h�E�N���X��
		}
	}

	// �E�B���h�E��`�̒���
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

	// �E�B���h�E�ʒu�̃Z���^�����O
	if (option & WindowOption::Centering) {
		RECT rc;
		GetWindowRect(GetDesktopWindow(), &rc);
		x = ((rc.right - rc.left) - w) / 2;
		y = ((rc.bottom - rc.top) - h) / 2;
	}

	// �E�B���h�E�̐���
	hWnd = CreateWindowEx(
		styleEx, className, caption,
		style, x, y, w, h,
		hWndParent, hMenu, hInst, pParam
	);

	if (hWnd == NULL) {
		return false;
	}

	// �e�E�B���h�E�Ɏ����Ŏq�E�B���h�E�N���b�v�X�^�C����t������
	if (hWndParent != NULL && (style & WS_CHILD)) {
		if (!SetWindowLong(hWndParent, GWL_STYLE,
			GetWindowLong(hWndParent, GWL_STYLE)|WS_CLIPCHILDREN))
		{
			return false;
		}
	}

	// �C���X�^���X�̃|�C���^���E�B���h�E�n���h���Ɋ֘A�t����
	if (!SetProp(hWnd, PROPERTY_NAME, (HANDLE)this)) {
		return false;
	}

	// WM_CREATE���b�Z�[�W���M
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
 	�E�B���h�E��j�󂷂�
 	@todo ��������B
 */
void Window::Destroy()
{
}

/*!
 	�E�B���h�E������Ԃɂ���
 
 */
bool Window::Show()
{
	return ShowWindow(hWnd, SW_SHOW) == TRUE;
}

/*!
 	�E�B���h�E��s����Ԃɂ���
 
 */
bool Window::Hide()
{
	return ShowWindow(hWnd, SW_HIDE) == TRUE;
}

/*!
 	�E�B���h�E�I�v�V������ݒ肷��
 	@param	option	Create()�̃I�v�V�����t���O(WindowOption)
 */
bool Window::SetOption(unsigned int option)
{
	// ���̂������܂��B
	return true;
}

/*!
 	�E�B���h�E�X�^�C����ݒ肷��
 	@todo ���̂������܂��B
 */
bool Window::SetStyle(DWORD style)
{
	return true;
}

/*!
 	�E�B���h�E�g���X�^�C����ݒ肷��
 	@todo ���̂������܂��B
 */
bool Window::SetStyleEx(DWORD styleEx)
{
	return true;
}

/*!
 	�E�B���h�E�n���h������Window�I�u�W�F�N�g���擾����
 	@param	hWnd	�E�B���h�E�n���h��
 	@return	Window�I�u�W�F�N�g
 	@retval	NULL	�擾���s
 */
Window* Window::GetObjectFrom(HWND hWnd) {
	return (Window*)GetProp(hWnd, PROPERTY_NAME);
}


/*!
 *	�E�B���h�E�N���X�����擾����
 *	@return	�E�B���h�E�N���X��(WNDCLASSEX�p)
 */
const TCHAR* Window::GetClassName()
{
	static TCHAR name[64];
	//_stprintf(name, _T("_WINDOW[%04x]"), wndNum);
	_stprintf(name, _T("_WINDOW[%04x%07x]"), wndNum, GetCurrentProcessId());
	return name;
}


/*!
 	���b�Z�[�W�n���h��
 
 	�Ǝ��̃��b�Z�[�W�n���h��������ꍇ�́A
 	AddMessageHandler<>()��p������@�ȊO�ɂ��A
 	���̃����o�֐����I�[�o�[���C�h������@������B
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
		if (i->second->Handle()) {	// �n���h�����s
			return i->second->lResult;
		}
	}
	
	if (msg == WM_DESTROY) {
		// WM_DESTROY�̃f�t�H���g�����B
		// �e�E�B���h�E��������΁i�����C���E�B���h�E�Ȃ̂Łj�A�v���P�[�V�����I��
		if ((LONG_PTR)GetWindowLongPtr(*this, GWL_HWNDPARENT) == NULL) {
			PostQuitMessage(0);
			return 0;
		}
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);;
}
