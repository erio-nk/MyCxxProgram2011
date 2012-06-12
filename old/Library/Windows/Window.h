/*!
 	@file Windows/Window.h
 
 	�E�B���h�E�N���X�B
 
 	@author	Dila
 */
//! @addtogroup Windows Windows�p�N���X
//! @{
//! @addtogroup Window �E�B���h�E�֘A�N���X
//! @{

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x500	// Win2k�ȏ� for WM_MOUSEWHEEL
#endif
#include <windows.h>
#include <tchar.h>
#include <map>	// for message map

/*!
 	�E�B���h�E�N���X
 
 	���b�Z�[�W�n���h����ǉ�����ꍇ�́A
 	Window::MessageHandler���p�������T�u�N���X���`���A
 	AddMessageHandler<>()�Ńn���h����ǉ�����B
 	OnMessage()���I�[�o�[���C�h���Ă��ǂ��B
 
 	@note	�E�B���h�E�v���V�[�W�����B���ׂɍ�����͗ǂ������
 			�t�ɗ��p�҂̃R�[�h�ʂ������Ă��܂����C�����܂���B
 */
class Window {
public:
	// �O���錾
	class MessageHandler;

	/*!
	 	Window::Create()�̃I�v�V�����t���O
	 */
	struct Option {
		enum {
			// �E�B���h�E�X�^�C��
			StandardStyle	= 0x0001,	//! ��ʓI�ȃE�B���h�E�X�^�C��
			FullscreenStyle	= 0x0002,	//! �t���X�N���[���p�E�B���h�E�X�^�C��

			// ���w��
			WindowSize	= 0x0000,	//! �w�肳�ꂽ�T�C�Y�̓E�B���h�E�̃T�C�Y
			ClientSize	= 0x0004,	//! �w�肳�ꂽ�T�C�Y�̓N���C�A���g�̈�̃T�C�Y

			// �w�i�u���V
			BGNull		= 0x0000,	//! �w�i��NULL�u���V
			BGWhite		= 0x0008,	//! �w�i�͔��u���V

			// �E�B���h�E�̃Z���^�����O
			Centering	= 0x0010,	//! �E�B���h�E���Z���^�����O����
		};
	};

private:
	//!/ �����o�ϐ�
	HWND hWnd;		// �E�B���h�E�n���h��
	int wndNum;		// �E�B���h�E�ԍ�

	std::map<UINT, class MessageHandler*> msgMap;	// ���b�Z�[�W�}�b�v

public:
	//!/ �����o�֐�
	Window();
	virtual ~Window();
	
	// ���
	bool Create(
		const TCHAR* caption,	// �^�C�g��
		int x, int y,			// ���W
		int w, int h,			// ��
		unsigned int option		// �I�v�V���� (WindowOption)
			= Option::StandardStyle|Option::ClientSize|Option::Centering,	// (Dila��)�悭�g���I�v�V����
		LPVOID pParam = NULL	// �p�����[�^
		);
	bool Create(
		const TCHAR* caption,
		int x, int y,
		int w, int h,
		unsigned int option,
		DWORD style,		// �X�^�C��
		DWORD styleEx,		// �g���X�^�C��
		LPVOID pParam = NULL
		);

	void Destroy();	// ��

	// ������
	bool Show();
	bool Hide();	// �B��

	//! �E�B���h�E�n���h�����擾����
	HWND GetHandle() { return hWnd; }

	//! HWND�^�ɃL���X�g����
	operator HWND() { return hWnd; }

	// �X�^�C���E�I�v�V����
	bool SetOption(unsigned int option);
	bool SetStyle(DWORD style);
	bool SetStyleEx(DWORD styleEx);

	// �I�u�W�F�N�g
	static Window* GetObjectFrom(HWND hWnd);

	// �E�B���h�E�N���X��
	virtual const TCHAR* GetClassName();

	// �E�B���h�E���b�Z�[�W�n���h��
	virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	/*!
	 	���b�Z�[�W�ƃ��b�Z�[�W�n���h�����֘A�t����
	 	@tparam	MessageHandlerClass	Window::MessageHandler�N���X�̃T�u�N���X
	 	@param	msg	�֘A�t����E�B���h�E���b�Z�[�W(WM_XXX)
	 */
	template <class MessageHandlerClass>
	bool AddMessageHandler(UINT msg)
	{
		std::map<UINT, class MessageHandler*>::iterator i;
		i = msgMap.find(msg);
		if (i != msgMap.end()) {
			delete i->second;
			msgMap.erase(i);
		}
		msgMap.insert(std::make_pair(msg, new MessageHandlerClass()));
			// new/delete�̃X�R�[�v��Window�N���X�X�R�[�v�Ɏ��߂�
		return true;
	}

	/*!
	 	���b�Z�[�W�n���h���x�[�X�N���X
	 
	 	Window�N���X�̃��b�Z�[�W���n���h�����O����ɂ́A
	 	���̃N���X���p�����AHandle���I�[�o�[���C�h����
	 	�T�u�N���X�̌^���e���v���[�g�����Ɏw�肵��
	 	Window::AddMessageHandler<>()���R�[������B
	 
	 	�p�����[�^��͂����G�ȃE�B���h�E���b�Z�[�W�́A
	 	�K��̃n���h���N���X(OnCreate,OnMouseMessage)��
	 	�p������΁A�p�����[�^��͍ς݂̃f�[�^���擾����
	 	�A�N�Z�T���񋟂����B
	 
	 	�T���v��
	 	@code
	 		// �}�E�X���{�^���C�x���g�n���h��
	 		struct OnLButtonDown : public Window::OnMouseMessage
	 		{
	 			bool Handle() { ... }
	 		};
	 
	 		Window wnd;
	 		wnd.AddMessageHandler<OnLButtonDown>(WM_LBUTTONDOWN);
	 	@endcode
	 */
	class MessageHandler 
	{
	protected:
		Window* pWnd;	//! ���b�Z�[�W���n���h�������E�B���h�E
		UINT msg;		//! ���b�Z�[�W
		LPARAM lParam;	//! L�p�����[�^
		WPARAM wParam;	//! W�p�����[�^
		LRESULT lResult;	//! ��������
		friend class Window;
	public:
		/*!
		 	���b�Z�[�W�n���h�����̃C�x���g
		 	@retval	true	����ȏチ�b�Z�[�W���������Ȃ�
		 	@retval	false	�f�t�H���g�̃��b�Z�[�W�������Ă�
		 */
		virtual bool Handle() = 0;
	};

	/*!
	 	WM_CREATE���b�Z�[�W�n���h���x�[�X�N���X
	 */
	struct OnCreate : public MessageHandler
	{
		LPCREATESTRUCT GetCreateStruct() { (LPCREATESTRUCT)lParam; }
	};

	/*!
	 	�}�E�X���b�Z�[�W�n���h���x�[�X�N���X
	 
	 	�Ή����b�Z�[�W�͈ȉ��̒ʂ�B
		WM_LBUTTONDOWN, WM_LBUTTONUP,
	 	WM_RBUTTONDOWN, WM_RBUTTONUP,
	 	WM_MOUSEMOVE, WM_MOUSEWHEEL
	 */
	struct OnMouseMessage : public MessageHandler
	{
		int GetX() { return LOWORD(lParam); }
		int GetY() { return HIWORD(lParam); }
		int GetZ() { return (short)HIWORD(wParam); }
		bool IsLButton() { return (LOWORD(wParam) & MK_LBUTTON) != 0; }
		bool IsRButton() { return (LOWORD(wParam) & MK_RBUTTON) != 0; }
			// LOWORD(wParam)���Ă�̂�WM_MOUSEWHEEL�p
	};

	/*!
	 	�L�[���b�Z�[�W�n���h���x�[�X�N���X
	 
	 	�Ή����b�Z�[�W�͈ȉ��̒ʂ�B
	 	WM_KEYDOWN, WM_KEYUP
	 */
	struct OnKeyMessage : public MessageHandler
	{
		//! ���z�L�[�R�[�h
		int GetVirtKey() { return static_cast<int>(wParam); }
		//! �L�[�f�[�^
		int GetKeyData() { return static_cast<int>(lParam); }
		//! ���s�[�g�J�E���g
		int GetRepeatCount(){ return static_cast<int>((lParam&0x0000ffff)); }
		//! �X�L�����R�[�h
		int GetScanCode()	{ return static_cast<int>((lParam&0x00ff0000) >> 16); }
		//! �g���L�[�������ꂽ���ǂ���
		bool IsExtendKey()	{ return (lParam&0x01000000) >> 24 != 0; }
		//! �R���e�L�X�g�R�[�h
		int GetContextCode(){ return static_cast<int>((lParam&0x20000000) >> 29); }
		//! ���O�̃L�[���
		int GetPrevState()	{ return static_cast<int>((lParam&0x40000000) >> 30); }
		//! �����ꂽ��
		bool IsKeyDown()	{ return (lParam&0x80000000) >> 31 == 0; }
		//! �����ꂽ��
		bool IsKeyUp()		{ return (lParam&0x80000000) >> 31 != 0; }
	};
};	// end class Window


//! @deprecated	Window::Option�ɒu�������ĉ������B
namespace WindowOption {
	/*!
	 	Window::Create()�̃I�v�V�����t���O
	 	@todo	�O���[�o���l�[���X�y�[�X�ł̐錾�͎~�߂������
	 */
	enum _WindowOption {
		// �E�B���h�E�X�^�C��
		StandardStyle	= 0x0001,	//! ��ʓI�ȃE�B���h�E�X�^�C��
		FullscreenStyle	= 0x0002,	//! �t���X�N���[���p�E�B���h�E�X�^�C��

		// ���w��
		WindowSize	= 0x0000,	//! �w�肳�ꂽ�T�C�Y�̓E�B���h�E�̃T�C�Y
		ClientSize	= 0x0004,	//! �w�肳�ꂽ�T�C�Y�̓N���C�A���g�̈�̃T�C�Y

		// �w�i�u���V
		BGNull		= 0x0000,	//! �w�i��NULL�u���V
		BGWhite		= 0x0008,	//! �w�i�͔��u���V

		// �E�B���h�E�̃Z���^�����O
		Centering	= 0x0010,	//! �E�B���h�E���Z���^�����O����
	};
};


//! @}
//! @}
