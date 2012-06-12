// WindowTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

using namespace Marda;


int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	Window wnd;
	wnd.Create(
		_T("test"),
		0, 0,
		640, 480,
		Window::STANDARD_STYLE,
		0,
		Window::SIZE_IS_CLIENT | Window::BG_IS_WHITE | Window::CENTERING
		);

	FrameRateController frameRate;
	frameRate.SetFrameRate(60);

#if 0
	// メッセージループ。
	MSG msg;

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) 
	{
		if (msg.message == WM_QUIT) 
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#else
	// メインループ。
	MSG msg;

	for (;;)
	{
		if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) 
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else 
		{
			// フレーム更新。
			//Sleep(100);
			frameRate.ChangeFrame();
		}
	}
#endif

	return (int)msg.wParam;
}

