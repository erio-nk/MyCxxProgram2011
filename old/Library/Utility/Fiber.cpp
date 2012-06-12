
#include "Utility/Fiber.h"

#pragma warning(disable: 4731)	// ebp���W�X�^�ύX��warning�𖳌��ɂ���

Fiber::Fiber(unsigned int stackSize) :
	_size(stackSize),
	_run(false)
{
	_Stack.resize(stackSize);
}

Fiber::~Fiber()
{
	delete _pProc;
}

/**
	�t�@�C�o�[�J�n����
	@param	pProc	�t�@�C�o�[�v���V�[�W��
 */
void Fiber::_Start(Fiber::Proc* pProc)
{
	if (_run) return;

	_pProc = pProc;
	_pProc->_pOwner = this;

	_esp = &_Stack[0] + _size;

	_run = true;

	void* pfnCall = (void*)&Fiber::_Call;

	__asm {
		push	offset Hoge
			// _Start()�̍Ō�̃A�h���X��push����->Yield()�̃��^�[���A�h���X��_Start()�̍Ō�ɂȂ�

		push	ebx	// �ȉ��l�s�͂����܂�̏����H
		push	ebp	// TODO:�������傢��������֐��Ăяo���̎d�g�݂𗝉�����
		push	esi
		push	edi

		mov		eax, this
			// this�|�C���^��������([this]._esp�̂悤�ɂ̓A�N�Z�X�o���Ȃ���)
		mov		ecx, pfnCall
			// static�Ȋ֐�_Call�̃A�h���X(�����o�֐��̓_��)

		xchg	[eax]._esp, esp
			// �X�^�b�N�|�C���^�����O�̃X�^�b�N�̈�(_Stack)�ɐ؂�Ԃ�

		push	eax
			// _Call�̑�����(this = Fiber*)���X�^�b�N�ɐݒ肷��
		call	ecx
			// _Call���Ăяo��

		/*
			�����܂ŗ�����AProc�̏����͑S�ďI����Ă���B
			_Call�̖߂�l(�܂�_Call�œn��������=this)��eax�ɑ������Ă���B
		 */
		
		xchg	[eax]._esp, esp
			// �̂ŁA�X�^�b�N�����ɖ߂�

		pop		edi	// ����������܂���ۂ��H
		pop		esi
		pop		ebp
		pop		ebx

		mov		[eax]._run, 0
			// _run��false�ɂ���

		ret
	Hoge:	// _Start()�̏I�[
	}
}

void Fiber::Run()
{
	if (!_run) return;

	__asm {
		push	offset Hoge
			// _Start()���l��Run()�̍Ō�̃A�h���X��push����
			//	->	���̌�_Yield()���Ă΂��΁A_Yield()�̃��^�[����Run()�̏I�[�ɂȂ�A
			//		�Ă΂ꂸ�ɏI��������(_Start()��call ecx�܂ŗ�����)�A���̃��^�[����Run()�̏I�[�ɂȂ�B

		push	ebx
		push	ebp
		push	esi
		push	edi
		
		mov		eax, [this]
		xchg	[eax]._esp, esp
			// ���O�X�^�b�N�ɐ؂�ւ���

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

		ret	
			// _Yield()�Őς񂾃��^�[���A�h���X�ɖ߂�(�܂�_Yield()�̍Ō�ɔ�сA_Yield()�Ăяo���̌��̖��߂֔��)

	Hoge:	// Run()�̏I�[
	}
}

/**
	�t�@�C�o�[�ꎞ���f�֐�
 */
void Fiber::_Yield()
{
	__asm {
		push	offset Hoge
			// ���O�X�^�b�N��_Yield()�̏I�[��ς݁ARun()�̃��^�[���A�h���X��_Yield()�̏I�[�ɂ���

		push	ebx
		push	ebp
		push	esi
		push	edi

		mov		eax, [this]
		xchg	[eax]._esp, esp
					// _Start()/Run()�Ő؂�ւ����X�^�b�N�|�C���^�����ɖ߂�

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

		ret	// _Start()/Run()�ŃX�^�b�N�ɐς񂾃��^�[���A�h���X�ɖ߂�

	Hoge:	// _Yield()�̏I�[
	}
}

/**
	�t�@�C�o�[�v���V�[�W���Ăяo���֐�

	�A�Z���u����Ń����o�֐��Ăяo�����o���Ȃ��ׁA
	this�������ɂ��ăv���V�[�W�����Ăяo�������̊֐��B
	@see Fiber::_Start()
 */
Fiber* Fiber::_Call(Fiber* p)
{
	p->_pProc->operator () ();
	return p;	// p��߂� -> _Start()��call�̖߂�l�Ƃ���p��eax�ɓ���
}
