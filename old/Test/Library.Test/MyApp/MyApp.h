#include <windows.h>
#include "Windows/Main.h"
#include "Windows/Window.h"

class MyWnd : public Window
{
public:
	MyWnd () {
		AddMessageHandler<MyWnd::OnCreate>(WM_CREATE);
		AddMessageHandler<MyWnd::OnDestroy>(WM_DESTROY);
	}

	struct OnCreate : public Window::OnCreate {
		bool Handle() {
			MessageBox(*pWnd, _T("OnCreate!"), "", MB_OK);
			return true;
		}
	};
	struct OnDestroy : public Window::MessageHandler {
		bool Handle() {
			MessageBox(*pWnd, _T("OnDestroy!"), "", MB_OK);
			return false;	// false‚ª‚¢‚¢
		}
	};
};

class MyApp
{
	MyWnd wnd;

public:
	static MyApp* GetInstance();

	void Initialize();

	void Update();
};
