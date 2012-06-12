/*!
	@file	Utilities/Coroutine.h
	
			�R���[�`���B
			Helpers�̊ȈՃR���[�`���N���X�̕����ȒP�ɗ��p�o����̂Ŏg��Ȃ����ɂ����B

			Copyright(C) 2011 Erio Nakayama <yamadila@gmail.com>
			All rights reserved.

	@author	E.Nakayama
*/
#pragma once

namespace Marda
{
	class CoroutineStructure
	{
	protected:
		friend void YeildCoroutine();
		template <class ReturnType> friend void YeildCoroutine(ReturnType&);

		PVOID _pCallFiberContext;	//!< �Ăяo�����t�@�C�o�B
		PVOID _pThisFiberContext;	//!< ���g�̃t�@�C�o�B
		void* _pReturnValue;	//!< �R���[�`���̖߂�l�B
		bool _exit;	//!< �R���[�`�����I���������ǂ����B

		CoroutineStructure();

		~CoroutineStructure();
	
		void Switch(PFIBER_START_ROUTINE pFiberProc);

		void Yeild();

		void Exit();
	};

	/*!
		@brief	�R���[�`���𒆒f���A�Ăяo�����ɐ����Ԃ��B
				�R���[�`���ȊO�̊֐����ŌĂяo�����ꍇ�̓���͖���`�B
	*/
	void YeildCoroutine();

	/*!
		@brief	�R���[�`���𒆒f���A�Ăяo�����ɐ����Ԃ��B
		@param	returnValue	�R���[�`���Ăяo��(Call)�̖߂�l�B
	*/
	template <class ReturnType>
	void YeildCoroutine(ReturnType& returnValue)
	{
		CoroutineStructure* pCoroutine = reinterpret_cast<CoroutineStructure*>(GetFiberData());
		*reinterpret_cast<ReturnType*>(pCoroutine->_pReturnValue) = returnValue;
		pCoroutine->Yeild();
	}


	/*!
		@brief	�R���[�`���N���X�e���v���[�g�B
				�R���[�`���ɓn���֐����I���i�֐��̍Ō�܂ŗ��邩return����j�O��
				�R���[�`���I�u�W�F�N�g���j������Ă��܂��ƁA�֐����ō\�z���ꂽ�I�u�W�F�N�g��
				�f�X�g���N�^���Ă΂ꂸ�A���������[�N�������N�����\��������ג��ӁB
		@code
		void Coroutine1()
		{
			for (int i = 0; i < 3; ++i)
			{
				printf("Func1\n");
				YeildCoroutine();
			}
		}

		int main()
		{	
			Coroutine<void> coroutine1(&Coroutine1);
			for (int i = 0; i < 5; ++i)
			{
				coroutine1.Call();
			}
			return 0;
		}
		@endcode
	*/
	template <class ReturnType, class ArgType1 = NullType, class ArgType2 = NullType>
	class Coroutine;

	#define Coroutine_Arg0	NullType, NullType
	#define Coroutine_Arg1	ArgType1, NullType

	//! �߂�l�����A��������
	template <>
	class Coroutine<void, Coroutine_Arg0>
		: public CoroutineStructure
	{
	public:
		typedef void (*FunctionType)();

		Coroutine(FunctionType function)
			: _function(function)
		{
		}

		void Call()
		{
			Switch(FiberProc);
		}

	private:
		FunctionType _function;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			(*pCoroutine->_function)();
			pCoroutine->Exit();
		}
	};	// end class Croutine

	//! �߂�l�L��A��������
	template <class ReturnType>
	class Coroutine<ReturnType,	Coroutine_Arg0>
		: public CoroutineStructure
	{
	public:
		typedef ReturnType (*FunctionType)();

		Coroutine(FunctionType function)
			: _function(function)
		{
			_pReturnValue = &_returnValue;
		}

		ReturnType Call()
		{
			Switch(FiberProc);
			return _returnValue;
		}

	private:
		FunctionType _function;
		ReturnType _returnValue;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			*reinterpret_cast<ReturnType*>(pCoroutine->_pReturnValue)
				= (*pCoroutine->_function)();
			pCoroutine->Exit();
		}
	};	// end class Croutine

	//! �߂�l�����A�����P
	template <class ArgType1>
	class Coroutine<void, Coroutine_Arg1>
		: public CoroutineStructure
	{
	public:
		typedef void (*FunctionType)(ArgType1);

		Coroutine(FunctionType function)
			: _function(function)
		{
		}

		void Call(ArgType1 arg1)
		{
			_pArg1 = &arg1;
			Switch(FiberProc);
		}

	private:
		FunctionType _function;
		ArgType1* _pArg1;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			(*pCoroutine->_function)(*pCoroutine->_pArg1);
			pCoroutine->Exit();
		}
	};	// end class Croutine

	
	//! �߂�l�L��A�����P
	template <class ReturnType, class ArgType1>
	class Coroutine<ReturnType, Coroutine_Arg1>
		: public CoroutineStructure
	{
	public:
		typedef ReturnType (*FunctionType)(ArgType1);

		Coroutine(FunctionType function)
			: _function(function)
		{
			_pReturnValue = &_returnValue;
		}

		ReturnType Call(ArgType1 arg1)
		{
			_pArg1 = &arg1;
			Switch(FiberProc);
			return _returnValue;
		}

	private:
		FunctionType _function;
		ReturnType _returnValue;
		ArgType1* _pArg1;

		static void WINAPI FiberProc(PVOID pParam)
		{
			Coroutine* pCoroutine = reinterpret_cast<Coroutine*>(pParam);
			*reinterpret_cast<ReturnType*>(pCoroutine->_pReturnValue)
				= (*pCoroutine->_function)(*pCoroutine->_pArg1);
			pCoroutine->Exit();
		}
	};	// end class Croutine

}	// end namespace Marda
