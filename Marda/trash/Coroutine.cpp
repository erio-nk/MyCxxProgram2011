/*!
	@file	Utilities/Coroutine.h

			Copyright(C) 2011 Erio Nakayama <yamadila@gmail.com>
			All rights reserved.

	@author	E.Nakayama
*/

#include "Marda/Utilities/Coroutine.h"

using namespace Marda;

CoroutineStructure::CoroutineStructure()
	: _pCallFiberContext(NULL)
	, _pThisFiberContext(NULL)
	, _pReturnValue(NULL)
	, _exit(false)
{
}

CoroutineStructure::~CoroutineStructure()
{
	if (is_not_null(_pThisFiberContext))
	{
		DeleteFiber(_pThisFiberContext);
		_pThisFiberContext = NULL;
	}
}
	
void 
CoroutineStructure::Switch(PFIBER_START_ROUTINE pFiberProc)
{
	if (_exit) return;

	// �Ăяo�����X���b�h���܂��t�@�C�o�ɂȂ��Ă��Ȃ���΃t�@�C�o�ɕϊ�����B
	if (!IsThreadAFiber())
	{
		_pCallFiberContext = ConvertThreadToFiber(NULL);
	}
	else
	{
		_pCallFiberContext = GetCurrentFiber();
	}

	// �R���[�`���p�̃t�@�C�o���܂��쐬����Ă��Ȃ���΍쐬����B
	if (is_null(_pThisFiberContext))
	{
		_pThisFiberContext = CreateFiberEx(0, 0, FIBER_FLAG_FLOAT_SWITCH, pFiberProc, this);
	}
	
	// �R���[�`���p�̃t�@�C�o�ɐ؂�ւ���B
	SwitchToFiber(_pThisFiberContext);

	// �Ăяo�����R���[�`�����I�����Ă�����t�@�C�o���폜����B
	if (_exit)
	{
		DeleteFiber(_pThisFiberContext);
		_pThisFiberContext = NULL;
	}
}

void 
CoroutineStructure::Yeild()
{
	// �Ăяo�����t�@�C�o�ɐ؂�ւ���B
	SwitchToFiber(_pCallFiberContext);
}

void 
CoroutineStructure::Exit()
{
	// �R���[�`���I���t���O�𗧂ĂČĂяo�����t�@�C�o�ɐ؂�ւ���B
	_exit = true;
	SwitchToFiber(_pCallFiberContext);
}

void 
Marda::YeildCoroutine()
{
	CoroutineStructure* pCoroutine = reinterpret_cast<CoroutineStructure*>(GetFiberData());
	pCoroutine->Yeild();
}

