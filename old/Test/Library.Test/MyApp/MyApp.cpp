#include "MyApp.h"

// Main関数
void Main()
{
	MyApp::GetInstance()->Initialize();

	// メインループ。MyApp::Update()を回す。
	MainLoop(MyApp::GetInstance(), &MyApp::Update);
}

// アプリインスタンス取得
MyApp* MyApp::GetInstance()
{
	static MyApp theApp;
	return &theApp;
}

// 初期化
void MyApp::Initialize()
{
	using namespace WindowOption;
	wnd.Create("MyApp", 0, 0, 500, 500, 
		StandardStyle|ClientSize|BGWhite|Centering);
	wnd.Show();
}

// 更新
void MyApp::Update()
{
	Sleep(500);
}