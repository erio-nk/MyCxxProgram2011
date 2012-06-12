/**
 *	MyApp.h
 *
 *	@author	Dila
 */

#include "Windows/Window.h"
#include "Windows/Bitmap.h"

#include "Game/FpsTimer.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

//// define

// ��ʃT�C�Y
#define SCREEN_W	640
#define SCREEN_H	480

// ���̂̒��_��
#define CUBE_SIZE	8


/**
 *	MyWindow
 */
class MyWindow : public Window
{
public:
	Vector2 pos;	// �}�E�X�J�[�\�����������ʒu

	//bool OnLButtonDown(MouseMessage* pMsg);
	//bool OnMouseMove(MouseMessage* pMsg);
	//bool OnMouseWheel(MouseMessage* pMsg);

	struct OnLButtonDown : public Window::OnMouseMessage
	{
		bool Handle();
	};

	struct OnMouseMove : public Window::OnMouseMessage
	{
		bool Handle();
	};

	struct OnMouseWheel : public Window::OnMouseMessage
	{
		bool Handle();
	};

	MyWindow()
	{
		AddMessageHandler<OnLButtonDown>(WM_LBUTTONDOWN);
		AddMessageHandler<OnMouseMove>(WM_MOUSEMOVE);
		AddMessageHandler<OnMouseWheel>(WM_MOUSEWHEEL);
	}
};



/**
 *	MyApp
 */
class MyApp
{
	static MyApp theInstance;

	MyWindow wnd;		// �E�B���h�E

	Bitmap backBuf;		// �o�b�N�o�b�t�@
	HDC backDC;

	FpsTimer fps;		// FPS����p

	Vector4 cube[CUBE_SIZE];	// ���̂̒��_
	Vector4 buf[CUBE_SIZE];

	Quaternion qRot;	// ���̂̉�]���

	Vector2 rot;		// ��]��
	float angle;		// ��]�p�x
	float scale;		// �g�k�W��

public:
	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();

	void Rotate(Vector2& v);
	void Scale(float x);
};
