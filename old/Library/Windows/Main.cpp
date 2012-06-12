/*!
 	@file Windows/Main.cpp
 
 	@author	Dila
 */

#include <tchar.h>
#include "Windows/Main.h"



/*
 	WinMain()
 */
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nCmdShow)
{

	Main();

	return (int)0;
}



void MessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
		if (msg.message == WM_QUIT) {
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



void MainLoop(void (*pfnRun)())
{
	MSG msg;

	while (true) {
		if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			(*pfnRun)();
		}
	}
}
