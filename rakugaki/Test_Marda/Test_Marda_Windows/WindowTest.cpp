// WindowTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

using namespace Marda;

void Run()
{
	Sleep(0);
}

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

	int exitCode = MainLoop(Run);

	return exitCode;
}

