/*!
	@file	Thread.h

	@author	E.Nakayama
*/

#include <process.h>

#include "Marda/Misc.h"
#include "Marda/Thread.h"

using namespace Marda;

// ----------------------------------------------------------------------------
// Marda ���C�u�������p�Ҍ����X���b�h�����֐��B

namespace
{
	struct MardaThreadParams
	{
		ThreadStartRoutine _lpStartAddress;
		void* _lpParameter;
		
		enum FLAGS
		{
			ENABLE_STRUCTURED_EXCEPTION_TRANSLATION	= 0x0001
		};
		Flags _flags;
	};
	
	static unsigned int WINAPI MardaThreadProc(void* lpParameter)
	{
		MardaThreadParams* params = reinterpret_cast<MardaThreadParams*>(lpParameter);
		ThreadStartRoutine lpStartAddress = params->_lpStartAddress;
		lpParameter = params->_lpParameter;
		
		// Marda ���C�u�����Ǝ��̏������B
		if (params->_flags.is(MardaThreadParams::ENABLE_STRUCTURED_EXCEPTION_TRANSLATION))
		{
			EnableStructuredExceptionTranslation();
		}

		delete params;
		
		return lpStartAddress(lpParameter);
	}
}

HANDLE 
Marda::CreateThread(
	void* lpThreadAttributes,
	unsigned int dwStackSize,
	ThreadStartRoutine lpStartAddress,
	void* lpParameter,
	unsigned int dwCreationFlags,
	unsigned int* lpThreadId
	)
{
	MardaThreadParams* params = new MardaThreadParams();	// ����ς� new �ō��Ȃ��ƁA�X���b�h����̃A�N�Z�X�͊댯���ȁH�H
	params->_lpStartAddress = lpStartAddress;
	params->_lpParameter = lpParameter;
	
	params->_flags.set(MardaThreadParams::ENABLE_STRUCTURED_EXCEPTION_TRANSLATION, IsEnabledStructuredExceptionTranslation());
	
	return 
		(HANDLE)_beginthreadex(
			lpThreadAttributes,
			dwStackSize,
			MardaThreadProc,
			params,
			dwCreationFlags,
			lpThreadId
			);
}

// ----------------------------------------------------------------------------
// �r������B

GuardObject::GuardObject()
{
	InitializeCriticalSection(&_criticalSection);
}

GuardObject::~GuardObject()
{
	DeleteCriticalSection(&_criticalSection);
}


void
GuardObject::Lock()
{
	EnterCriticalSection(&_criticalSection);
}

void
GuardObject::Unlock()
{
	LeaveCriticalSection(&_criticalSection);
}

