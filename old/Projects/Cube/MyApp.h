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

// 画面サイズ
#define SCREEN_W	640
#define SCREEN_H	480

// 立体の頂点数
#define CUBE_SIZE	8


/**
 *	MyWindow
 */
class MyWindow : public Window
{
public:
	Vector2 pos;	// マウスカーソルを押した位置

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

	MyWindow wnd;		// ウィンドウ

	Bitmap backBuf;		// バックバッファ
	HDC backDC;

	FpsTimer fps;		// FPS制御用

	Vector4 cube[CUBE_SIZE];	// 立体の頂点
	Vector4 buf[CUBE_SIZE];

	Quaternion qRot;	// 立体の回転情報

	Vector2 rot;		// 回転軸
	float angle;		// 回転角度
	float scale;		// 拡縮係数

public:
	static MyApp& GetInstance() { return theInstance; }

	bool Initialize();
	bool Finalize();

	void Update();

	void Rotate(Vector2& v);
	void Scale(float x);
};
