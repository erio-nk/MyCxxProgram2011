#include <stdio.h>

// �֐��|�C���^�ƃN���X�֌W�̂Ă���
//
// �������N���X�̃|�C���^�^�ŁA���x�[�X�N���X��
// �|�C���^�ł���Ƃ�(SayHoge)�A�T�u�N���X�̃|�C���^��������
// �Ƃ�֐�(SayFuga,SayMoke)�̃|�C���^��SayHoge�̌^�ɃL���X�g���āA
// �X�Ɉ����͂��ꂼ��ړI�̃C���X�^���X�ւ̃|�C���^��^���āA
// ���ꂼ��̏�����������ƌĂяo����邩�ǂ����̃`�F�b�N�B

// �֐��|�C���^�������R�[�h�̐擪�A�h���X�ŁA
// �������P�Ȃ�A������������(�X�^�b�N)�Ƃ���΁A
// �����f�[�^�̍��v�T�C�Y(int,int�Ȃ�8�o�C�g)��
// ���v���Ă���΁A�^���L���X�g�����֐��|�C���^��
// �����Ă�(�����̃T�C�Y�������Ă���̂�)�Ăяo����
// ���Ȃ��Ǝv����B
//
// void num(int* p) {...}
//
// void (*pNum)(int*) = num;	// OK
// void (*pNum2)(long*) = num;	// �T�C�Y�������Ă���̂ł��������OK
//
// ��L�̊֐��|�C���^pNum,pNum2�̌Ăяo�����ʂ́A
// ���ǂ̂Ƃ���num()�̏������ʂƂȂ�Ǝv����B

// SayHoge,SayFuga,SayMoke�̈����́A
// �ǂ���|�C���^�^4�o�C�g�̃f�[�^�Ȃ̂ŁA
// SayFuga,SayMoke��SayHoge�̌^(void (*)(Hoge*))��
// �L���X�g���Ă����Ȃ��Ǝv����B
// �ܘ_�L���X�g�����|�C���^�ɂ��ꂼ��Fuga*��
// Moke*��n���Ă��A�x�[�X�N���X��Hoge*�Ȃ̂Ŗ��Ȃ��B
// �ďo����͂��ꂼ��SayFuga,SayMoke���N������邪�A
// ���̎�Hoge*�Ƃ��ēn�����|�C���^�͂��ꂼ��Fuga*,
// Moke*�ɕϊ����ꂽ��ԂŊ֐����N������(�Ǝv����)�̂ŁA
// SayFuga,SayMoke�͍ŏI�I�ɐ����������ƈ����Ƃ���
// �Ăяo�����Ǝv����B
//
// �܂�A
// void (*p)(Hoge*) = SayHoge; (*p)(&hoge);	// OK
// void (*p)(Hoge*) = SayHoge; (*p)(&fuga);	// OK Hoge�̃T�u�N���X�Ȃ̂�OK
// void (*p)(Hoge*) = SayHoge; (*p)(&moke);	// OK Hoge�̃T�u�N���X�Ȃ̂�OK
//
// void (*p)(Hoge*) = SayFuga; (*p)(&hoge);	// NG �p���̊֌W��NG�H
// void (*p)(Hoge*) = SayFuga; (*p)(&fuga);	// OK
// void (*p)(Hoge*) = SayFuga; (*p)(&moke);	// OK Fuga�̃T�u�N���X�Ȃ̂�OK
//
// void (*p)(Hoge*) = SayMoke; (*p)(&hoge);	// NG �p���̊֌W��NG�H
// void (*p)(Hoge*) = SayMoke; (*p)(&fuga);	// NG �p���̊֌W��NG�H
// void (*p)(Hoge*) = SayMoke; (*p)(&moke);	// OK


class Hoge
{
	int x;
public:
	Hoge() {x=0;}
	virtual void Say() {
		printf("say hoge.\n");
	}
};

class Fuga : public Hoge
{
	int y;
public:
	Fuga() {y=1;}
	void Say() {
		printf("say fuga.\n");
	}
};

class Moke : public Fuga
{
public:
	Moke() {}
	void Say() {
		printf("say moke.\n");
	}
};


void SayHoge (Hoge* p) {
	p->Say();
}

void SayFuga(Fuga* p) {
	p->Say();
}

void SayMoke(Moke* p) {
	p->Say();
}


int main()
{
	Hoge hoge;
	Fuga fuga;
	Moke moke;

	void (*pSay)(Hoge*);

	pSay = SayHoge;
	(*pSay)(&hoge);
	
	(*pSay)(&fuga);
	
	(*pSay)(&moke);
	

	pSay = (void (*)(Hoge*))SayFuga;
	(*pSay)(&fuga);
	
	(*pSay)(&moke);
	
	pSay = (void (*)(Hoge*))SayMoke;
	(*pSay)(&moke);
}

