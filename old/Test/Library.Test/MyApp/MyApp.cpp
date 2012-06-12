#include "MyApp.h"

// Main�֐�
void Main()
{
	MyApp::GetInstance()->Initialize();

	// ���C�����[�v�BMyApp::Update()���񂷁B
	MainLoop(MyApp::GetInstance(), &MyApp::Update);
}

// �A�v���C���X�^���X�擾
MyApp* MyApp::GetInstance()
{
	static MyApp theApp;
	return &theApp;
}

// ������
void MyApp::Initialize()
{
	using namespace WindowOption;
	wnd.Create("MyApp", 0, 0, 500, 500, 
		StandardStyle|ClientSize|BGWhite|Centering);
	wnd.Show();
}

// �X�V
void MyApp::Update()
{
	Sleep(500);
}