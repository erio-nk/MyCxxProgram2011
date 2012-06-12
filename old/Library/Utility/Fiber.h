/**
	@file Utility/Fiber.h

	�t�@�C�o�[�N���X

	@author	Dila
 */
/// @addtogroup Utility
/// @{
#pragma once

#include <vector>
#include <assert.h>

/**
	�t�@�C�o�[�N���X
	
	�t�@�C�o�[(���}�C�N���X���b�h�A�R���[�`��)�Ƃ́A
	���[�U����̃X���b�h(�̗l�Ȃ���)�B
	�t�@�C�o�[�̃v���V�[�W��(Fiber::Proc)�ŁAYield()��
	�C�ӂɃR�[�����鎖�ŁA�������ꎞ���f����B
	���̎��A���[�J���ϐ����X�^�b�N�̏��͑S�Ĉꎞ�̈��
	�ޔ�����ARun()���Ăяo�����ōĊJ�����B
	�ĊJ��́A����Yield()���R�[������邩�A�I������܂Ŏ��s�����B

	�T���v��
	@code
		// �t�@�C�o�[�v���V�[�W���N���X
		struct MyProc : Fiber::Proc
		{
			void operator ()()
			{
				// ��������
				Yield();	// �����ňꎞ���f�B�Ăяo�����ɕ��A����B
				// ��������
				Yield();	// �����B
				// ���������`�I��
			}
		};

		Fiber fiber;
		fiber.Start<MyProc>();	// �ŏ���Yield()�܂Ŏ��s�����
		fiber.Run();	// ���ڂ�Yield()�܂Ŏ��s�����
		fiber.Run();	// �I���܂Ŏ��s�����
	@endcode
 */
class Fiber
{
public:
	/**
		�t�@�C�o�[�v���V�[�W���N���X

		�t�@�C�o�[�ŏ����������֐��́A���̃N���X���p������
		�t�@���N�^�Ƃ��Ď�������K�v������B
	 */
	class Proc
	{
		Fiber* _pOwner;		/// ���̃v���V�[�W�������s����t�@�C�o�[�I�u�W�F�N�g
		friend class Fiber;	// to call Fiber::_Yield()

	public:
		Proc() : _pOwner(NULL) {}
		virtual ~Proc() {}

	protected:
		/**
			�v���V�[�W��
		 */
		virtual void operator ()() = 0;

		/**
			�������ꎞ���f����
		 */
		void Yield() { 
			assert(_pOwner);
			_pOwner->_Yield();
		}
	};	// end class Proc
	friend class Proc;


private:
	typedef unsigned char byte;	/// uchar�̋L�q�ȗ���
	std::vector<byte> _Stack;	/// ���O�X�^�b�N�̈�
	unsigned int _size;			/// �X�^�b�N�T�C�Y
	byte* _esp;		/// �X�^�b�N�|�C���^(esp)�̑ޔ�p
	Proc* _pProc;	/// �v���V�[�W��
	bool _run;		/// �t�@�C�o�[���s�����ǂ���

public:
	/**
		�R���X�g���N�^
		@param	stackSize	���O�X�^�b�N�̍ő�T�C�Y
		@todo	�f�t�H���g�̃X�^�b�N�T�C�Y���ᖡ����
	 */
	Fiber(unsigned int stackSize = 0x8000);

	/// �f�X�g���N�^
	~Fiber();

	/**
		�t�@�C�o�[���J�n����

		���s���ꂽ�v���V�[�W���́AYield()���Ăяo����邩�A
		�I������܂Ŏ��s�����B
		@tparam	ProcClass	�v���V�[�W���N���X(Fiber::Proc�̃T�u�N���X)
		@see	Fiber::Proc::Yield
	 */
	template <class ProcClass>
	void Start() { _Start(new ProcClass()); }

	/**
		�t�@�C�o�[���ĊJ����

		�ĊJ���ꂽ�v���V�[�W���́AYield()���Ăяo����邩�A
		�I������܂Ŏ��s�����B
		@see	Fiber::Proc::Yield
	 */
	void Run();

	/**
		�t�@�C�o�[�����s�����ǂ������擾����
		@return	���s���̏ꍇ��true�A���s���Ă��Ȃ��ꍇ��false
	 */
	bool IsRunning() { return _run; }

private:
	void _Start(Proc* pProc);
	void _Yield();
	static Fiber* _Call(Fiber* p);
};	// end class Fiber

/// @}
