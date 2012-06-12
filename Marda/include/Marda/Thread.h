/*!
	@file	Thread.h
	
			�X���b�h���p���̃w���p�֐��E�N���X�Q�B

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"

namespace Marda
{
	// ------------------------------------------------------------------------
	// Marda ���C�u�������p�Ҍ����X���b�h�����֐��B
	
	typedef unsigned int (__stdcall *ThreadStartRoutine)(void*);
	
	/*!
		@brief	�X���b�h�𐶐�����B
				Marda ���C�u�����Ǝ��̃X���b�h�������������s���ׁA
				Marda ���C�u�������p�҂͂��̊֐��ŃX���b�h���쐬���鎖�B
				������ _beginthreadex �Ɠ����B
	*/
	HANDLE CreateThread(
		void* lpThreadAttributes, // �Z�L�����e�B�L�q�q
		unsigned int dwStackSize,                        // �����̃X�^�b�N�T�C�Y
		ThreadStartRoutine lpStartAddress,    // �X���b�h�̋@�\
		void* lpParameter,                       // �X���b�h�̈���
		unsigned int dwCreationFlags,                    // �쐬�I�v�V����
		unsigned int* lpThreadId                        // �X���b�h���ʎq
		);
	
	// ------------------------------------------------------------------------
	// �����I�u�W�F�N�g�p���b�p�֐��Q�B

	inline bool IsSignaled(HANDLE handle)
	{
		return WaitForSingleObject(handle, 0) == WAIT_OBJECT_0;
	}
	
	inline bool IsNonSignaled(HANDLE handle)
	{
		return !IsSignaled(handle);
	}
	
	// �X���b�h�p�B
	inline bool IsThreadAlive(HANDLE handle)
	{
		return IsNonSignaled(handle);
	}

	inline bool IsThreadDead(HANDLE handle)
	{
		return IsSignaled(handle);
	}
	
	// ------------------------------------------------------------------------
	// �ȈՃX���b�h�N���X�B

	class Thread
	{
	public:
		/*!
			@brief	�w��̊֐��I�u�W�F�N�g�ŃX���b�h���J�n����B
			@tparam	Function �֐��I�u�W�F�N�g
		*/
		template <class Function>
		bool Begin(Function function)
		{
			if (!IsEnd()) return false;
			_hThread.reset(
				CreateThread(
					NULL,	// lpThreadAttributes
					0,	// dwStackSize	; �f�t�H���g
					&ThreadProc<Function>,	// lpStartAddress
					new Function(function),	// lpParameter
					0,	// dwCreationFlags
					NULL	// lpThreadId
					)
				);
			return true;
		}

		template <class Function>
		static unsigned int WINAPI ThreadProc(void* lpParameter)
		{
			Function* func = reinterpret_cast<Function*>(lpParameter);
			(*func)();
			delete func;
			return 0;
		}

		/*!
			@brief	�X���b�h���I�����Ă��邩�ǂ������擾����B
		*/
		bool IsEnd() const
		{
			if (_hThread.is_invalid()) return true;
			return IsThreadDead(_hThread);
		}

		/*!
			@brief	�X���b�h���I������܂ő҂B
		*/
		DWORD Wait(DWORD timeout = INFINITE)
		{
			return WaitForSingleObject(_hThread.get(), timeout);
		}

	private:
		AUTO_THREAD_HANDLE _hThread;
	};	// end class Thread

	// ------------------------------------------------------------------------
	// �r������
	
	/*!
		@brief	�K�[�h�ϐ�
	*/
	class GuardObject
	{
	public:
		GuardObject();
		~GuardObject();
		void Lock();
		void Unlock();
	private:
		CRITICAL_SECTION _criticalSection;
	};	// end class GuardObject

	//! �r�����䂵�Ȃ��K�[�h�ϐ�
	struct NoGuardObject
	{
		void Lock() { /* noop */ };
		void Unlock() { /* noop */ };
	};
	
	/*!
		@brief	�r������
		@endcode
	*/
	template <class GuardObject>
	class Lock
	{
	public:
		/*!
			@brief	�r������J�n
			@param	guardObject	���̃K�[�h�ϐ��ɂ��Ĕr��������s���B
								�r������͂���Lock�I�u�W�F�N�g���������Ă���ԍs����B
			@code
			GuardObject _guard;
			{
				Lock<GuardObject> lock(_guard);	// _guard �ɂ��Ĕr������J�n
				
				// working
				
				// lock �I�u�W�F�N�g�̔j���Ƌ��ɔr�����䂪���������
			}
			@endcode
		*/
		Lock(GuardObject& guard)
			: _pGuard(&guard)
		{
			_pGuard->Lock();
		}

		//! �r������I��
		~Lock()
		{
			_pGuard->Unlock();
		}
	
	private:
		GuardObject* _pGuard;
	};	// end class Lock

	// ------------------------------------------------------------------------
	// �X���b�h���[�J���X�g���[�W�w���p�B

	#include "Thread_Tls_inl.h"

	/*!
		@brief	�X���b�h���[�J���ϐ��N���X�B
				C++0x�ł́Athread_local �Ƃ����C���q�ŃX���b�h���[�J���ϐ��������o����B
				�������AVS2010Express�ł͂܂��g�p�o���Ȃ��͗l�B
		@tparam	Type	�ϐ��^�B
		@tparam	Tag		�����ϐ��^�ɂ��āA�ʂ̃X���b�h���[�J���ϐ��Ƃ���
						�m�ۂ������ꍇ�́A���̃N���X��Ǝ��ɒ�`�������̂ɕύX����B
		@tparam	AllocAPI	�ϐ��̊m��API���`�����|���V�[�N���X�B
							�Ⴆ�Εϐ��m�ہ^�����new/delete�ȊO�ɂ�����B
		@tparam	TlsAPI		TLS�̊m�ہ^���API���`�����|���V�[�N���X�B
							TLS�̊m�ہ^�����Ǝ��������铙�������ꍇ�́A
							���̃N���X��Ǝ��ɒ�`�������̂ɕύX����B
		@todo	GetVariables �̃T�|�[�g�L�����w�肷��i��TLS�m�ێ��̔r����������S�ɖ�������j�ׂ̃|���V�[�N���X�B
				�܂��A�\�ߗ̈���m�ۂ��Ă����ATLS���������ɂ͐ݒ肷�邾���ɂ��A�X���b�h�N�����̃I�[�o�[�w�b�h���Ȃ����@�\�B
				�v�[�����O�Ŏ�������ƁA�v�[���ւ̃A�N�Z�X���ɔr�����䂪�K�v�ƂȂ�A���ǖ���r�������v������Ă��܂��c�B
		@code
		typedef ThreadLocalVariable<int> TLInt;	// �X���b�h���[�J���ϐ��^�̒�`�B

		void ParallelCount()
		{
			TLInt tl_val(0);	// �X���b�h���[�J���ϐ��̐錾�B
			++(*tl_val);	// �ԐڎQ�Ɖ��Z�q�ŕϐ��l�ɃA�N�Z�X����B
		}

		void ParallelProcess()
		{
			// �X���b�h�֐��B
			for (int i = 0; i < 10; ++i)
			{
				ParallelCount();
			}
		}

		void Test_Thread()
		{
			Thread thread1;
			Thread thread2;

			thread1.Begin(std::bind(ParallelProcess));
			thread2.Begin(std::bind(ParallelProcess));

			thread1.Wait();
			thread2.Wait();
			
			// �X���b�h���[�J���ϐ��̓��e��\������B
			TLInt::Variables& vars = TLInt::GetVariables();
			TLInt::Variables::iterator itrVars = vars.begin();
			for (int i = 0; itrVars != vars.end(); ++itrVars, ++i)
			{
				TTRACE(_T("vars[%d]=%d"), i, *(*itrVars));
			}
		}
		@endcode
	*/
	template <
		typename Type,
		typename Tag = ThreadLocalVariable_TagDefault,
		class AllocAPI = ThreadLocalVariable_AllocAPI_Default<Type>,
		class TlsAPI = ThreadLocalVariable_TlsAPI_Default<Type>
	>
	class ThreadLocalVariable
	{
		typedef ThreadLocalStorage<Type, AllocAPI, TlsAPI> Storage;
	public:
		typedef typename Storage::Variables Variables;

		/*!
			@brief	�X���b�h���[�J���ϐ��i���X�g�j������������B
					���̊֐��𖾎��I�ɌĂяo���Ȃ��Ă��A����g�p���ɏ����������B
		*/
		static bool Create()
		{
			return s_storage.Create();
		}

		/*!
			@brief	�X���b�h���[�J���ϐ��i���X�g�j���������B
					���̊֐��𖾎��I�ɌĂяo���Ȃ��Ă��A�A�v���P�[�V�����I�����ɉ�������B
		*/
		static void Release()
		{
			s_storage.Release();
		}

		/*!
			@brief	�X���b�h���[�J���ϐ����X�g���擾����B
					�ԋp���ꂽ���X�g�ւ̃A�N�Z�X�̓X���b�h�Z�[�t�ł͖����ג��ӁB
		*/
		static Variables& GetVariables()
		{
			return s_storage._vars;
		}
		
		/*!
			@brief	�X���b�h���[�J���ϐ����X�g���R�s�[����B
					���̊֐��̓X���b�h�Z�[�t�ł���B
		*/
		static Variables& GetVariables(Variables& dst)
		{
			return s_storage.CopyVariables(dst);
		}
		
		/*!
			@brief	�X���b�h���[�J���ϐ����\�z����B
		*/
		ThreadLocalVariable()
		{
			_pVar = s_storage.Alloc();
		}

		ThreadLocalVariable(Type val)
		{
			_pVar = s_storage.Alloc(val);
		}

		/*!
			@brief	�X���b�h���[�J���ϐ��l���擾����B
		*/
		Type& operator * ()
		{
			return *_pVar;
		}

		const Type& operator * () const
		{
			return *_pVar;
		}

		/*!
			@brief	�X���b�h���[�J���ϐ��̃����o�ɃA�N�Z�X����B
		*/
		Type* operator -> ()
		{
			return _pVar;
		}

		const Type* operator -> () const
		{
			return _pVar;
		}

	private:
		static Storage s_storage;
		Type* _pVar;

	};	// end class ThreadLocalVariable

	template <typename Type, typename Tag, class AllocAPI, class TlsAPI>
	typename ThreadLocalVariable<Type, Tag, AllocAPI, TlsAPI>::Storage
	ThreadLocalVariable<Type, Tag, AllocAPI, TlsAPI>::s_storage;

	/*!
		@brief	�X���b�h���[�J���z��N���X�B
		@tparam	Type	�ϐ��^�B
		@tparam	Size	�z��T�C�Y�B
	*/
	template <
		typename Type,
		int Size,
		typename Tag = ThreadLocalVariable_TagDefault,
		class AllocAPI = ThreadLocalArray_AllocAPI_Default<Type, Size>,
		class TlsAPI = ThreadLocalVariable_TlsAPI_Default<Type>
	>
	class ThreadLocalArray
	{
		typedef ThreadLocalStorage<Type, AllocAPI, TlsAPI> Storage;
	public:
		typedef typename Storage::Variables Arrays;
	
		static const int SIZE = Size;

		static bool Create()
		{
			return s_storage.Create();
		}

		static void Release()
		{
			s_storage.Release();
		}

		static Arrays& GetArrays()
		{
			return s_storage._vars;
		}
		
		ThreadLocalArray()
		{
			_pVar = s_storage.Alloc();
		}

		Type* operator * ()
		{
			return _pVar;
		}

		const Type* operator * () const
		{
			return _pVar;
		}

	private:
		static Storage s_storage;
		Type* _pVar;

	};	// end class ThreadLocalVariable

	template <typename Type, int Size, typename Tag, class AllocAPI, class TlsAPI>
	typename ThreadLocalArray<Type, Size, Tag, AllocAPI, TlsAPI>::Storage
	ThreadLocalArray<Type, Size, Tag, AllocAPI, TlsAPI>::s_storage;
	
}	// end namespace Marda
