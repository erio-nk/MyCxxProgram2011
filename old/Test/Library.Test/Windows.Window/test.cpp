#include <windows.h>

#include "Windows/Main.h"
#include "Windows/Window.h"


void Run() {}

//
void Main()
{
	Window wnd;

	wnd.Create("‚Ä‚·‚Æ", 0, 0, 100, 100, 
		WindowOption::StandardStyle|
		WindowOption::ClientSize|
		WindowOption::Centering);
	wnd.Show();

	MessageLoop();
	//MainLoop(Run);
}

