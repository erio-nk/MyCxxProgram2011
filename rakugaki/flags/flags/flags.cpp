// flags.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// �w��̃C���f�b�N�X�̃t���O���폜���āA�㑱�̃t���O���ЂƂE�ɂ��炷
	// std::vector<bool> �I�Ȃ��̂́Aerase�ɑ�������
	int flags = 0x00f6;

	//int index = 32 + 4;	// ���V�t�g���Z�q�̍��ӂ͏��32�̏�]���Z������[���B32->0��V�t�g�A36->4��V�t�g�B
	int index = 31;

	int mask = -1 << index;

	int result = (flags & (~mask)) | ((flags >> 1) & mask);

	printf("flags=%x result=%x\n", flags, result);

	return 0;
}

