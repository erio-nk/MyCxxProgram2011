#include <windows.h>

#include "Windows/Main.h"
#include "Windows/Window.h"
#include "Windows/Bitmap.h"



//
class MyWindow : public Window {
public:
	Bitmap bmp;
	DIBitmap bmp2;

	MyWindow() {
		AddMessageHandler<OnCreate>(WM_CREATE);
		AddMessageHandler<OnDestroy>(WM_DESTROY);
		AddMessageHandler<OnPaint>(WM_PAINT);
	}

	class OnCreate : public Window::OnCreate {
		bool Handle() {
			MyWindow* _This = (MyWindow*)pWnd;

			_This->bmp.Load(_T("nagato1.bmp"));

			_This->bmp2.Create(16, 16);
			char* pBits = (char*)_This->bmp2.GetBits();
			for (int i = 0; i < _This->bmp2.GetHeight(); ++i) {
				RGBTRIPLE* pRgb = (RGBTRIPLE*)pBits;
				for (int k = 0; k < _This->bmp2.GetWidth(); ++k) {
					pRgb[k].rgbtRed = pRgb[k].rgbtGreen = pRgb[k].rgbtBlue = 255 * k / _This->bmp2.GetWidth();
				}
				pBits += GetBitmapRasterSize(_This->bmp2.GetWidth(), _This->bmp2.GetDepth());
			}

			return true;
		}	
	};

	class OnDestroy : public Window::MessageHandler {
		bool Handle() {
			MyWindow* _This = (MyWindow*)pWnd;
			_This->bmp2.Release();
			_This->bmp.Release();
			return false;
		}
	};

	class OnPaint : public Window::MessageHandler {
		bool Handle() {
			MyWindow* _This = (MyWindow*)pWnd;
			PAINTSTRUCT ps;
			BeginPaint(*_This, &ps);
			BitBlt(ps.hdc, 0, 0, _This->bmp.GetWidth(), _This->bmp.GetHeight(), _This->bmp.GetDC(), 0, 0, SRCCOPY);
			BitBlt(ps.hdc, 0, 0, _This->bmp2.GetWidth(), _This->bmp2.GetHeight(), _This->bmp2.GetDC(), 0, 0, SRCCOPY);
			EndPaint(*_This, &ps);
			return true;
		}
	};
};


//
void Main()
{
	MyWindow wnd;

	wnd.Create("‚Ä‚·‚Æ", 0, 0, 500, 500, 
		WindowOption::StandardStyle|
		WindowOption::ClientSize|
		WindowOption::BGWhite|
		WindowOption::Centering);
	wnd.Show();

	MessageLoop();
	//MainLoop(Run);
}

