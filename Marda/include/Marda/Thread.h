/*!
	@file	Thread.h
	
			スレッド利用時のヘルパ関数・クラス群。

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"

namespace Marda
{
	// ------------------------------------------------------------------------
	// Marda ライブラリ利用者向けスレッド生成関数。
	
	typedef unsigned int (__stdcall *ThreadStartRoutine)(void*);
	
	/*!
		@brief	スレッドを生成する。
				Marda ライブラリ独自のスレッド初期化処理を行う為、
				Marda ライブラリ利用者はこの関数でスレッドを作成する事。
				引数は _beginthreadex と同じ。
	*/
	HANDLE CreateThread(
		void* lpThreadAttributes, // セキュリティ記述子
		unsigned int dwStackSize,                        // 初期のスタックサイズ
		ThreadStartRoutine lpStartAddress,    // スレッドの機能
		void* lpParameter,                       // スレッドの引数
		unsigned int dwCreationFlags,                    // 作成オプション
		unsigned int* lpThreadId                        // スレッド識別子
		);
	
	// ------------------------------------------------------------------------
	// 同期オブジェクト用ラッパ関数群。

	inline bool IsSignaled(HANDLE handle)
	{
		return WaitForSingleObject(handle, 0) == WAIT_OBJECT_0;
	}
	
	inline bool IsNonSignaled(HANDLE handle)
	{
		return !IsSignaled(handle);
	}
	
	// スレッド用。
	inline bool IsThreadAlive(HANDLE handle)
	{
		return IsNonSignaled(handle);
	}

	inline bool IsThreadDead(HANDLE handle)
	{
		return IsSignaled(handle);
	}
	
	// ------------------------------------------------------------------------
	// 簡易スレッドクラス。

	class Thread
	{
	public:
		/*!
			@brief	指定の関数オブジェクトでスレッドを開始する。
			@tparam	Function 関数オブジェクト
		*/
		template <class Function>
		bool Begin(Function function)
		{
			if (!IsEnd()) return false;
			_hThread.reset(
				CreateThread(
					NULL,	// lpThreadAttributes
					0,	// dwStackSize	; デフォルト
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
			@brief	スレッドが終了しているかどうかを取得する。
		*/
		bool IsEnd() const
		{
			if (_hThread.is_invalid()) return true;
			return IsThreadDead(_hThread);
		}

		/*!
			@brief	スレッドが終了するまで待つ。
		*/
		DWORD Wait(DWORD timeout = INFINITE)
		{
			return WaitForSingleObject(_hThread.get(), timeout);
		}

	private:
		AUTO_THREAD_HANDLE _hThread;
	};	// end class Thread

	// ------------------------------------------------------------------------
	// 排他制御
	
	/*!
		@brief	ガード変数
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

	//! 排他制御しないガード変数
	struct NoGuardObject
	{
		void Lock() { /* noop */ };
		void Unlock() { /* noop */ };
	};
	
	/*!
		@brief	排他制御
		@endcode
	*/
	template <class GuardObject>
	class Lock
	{
	public:
		/*!
			@brief	排他制御開始
			@param	guardObject	このガード変数について排他制御を行う。
								排他制御はこのLockオブジェクトが生存している間行われる。
			@code
			GuardObject _guard;
			{
				Lock<GuardObject> lock(_guard);	// _guard について排他制御開始
				
				// working
				
				// lock オブジェクトの破棄と共に排他制御が解除される
			}
			@endcode
		*/
		Lock(GuardObject& guard)
			: _pGuard(&guard)
		{
			_pGuard->Lock();
		}

		//! 排他制御終了
		~Lock()
		{
			_pGuard->Unlock();
		}
	
	private:
		GuardObject* _pGuard;
	};	// end class Lock

	// ------------------------------------------------------------------------
	// スレッドローカルストレージヘルパ。

	#include "Thread_Tls_inl.h"

	/*!
		@brief	スレッドローカル変数クラス。
				C++0xでは、thread_local という修飾子でスレッドローカル変数を実現出来る。
				ただし、VS2010Expressではまだ使用出来ない模様。
		@tparam	Type	変数型。
		@tparam	Tag		同じ変数型について、別のスレッドローカル変数として
						確保したい場合は、このクラスを独自に定義したものに変更する。
		@tparam	AllocAPI	変数の確保APIを定義したポリシークラス。
							例えば変数確保／解放をnew/delete以外にしたり。
		@tparam	TlsAPI		TLSの確保／解放APIを定義したポリシークラス。
							TLSの確保／解放を独自実装する等したい場合は、
							このクラスを独自に定義したものに変更する。
		@todo	GetVariables のサポート有無を指定する（≒TLS確保時の排他制御を完全に無くせる）為のポリシークラス。
				また、予め領域を確保しておき、TLS初期化時には設定するだけにし、スレッド起動時のオーバーヘッドをなくす機構。
				プーリングで実装すると、プールへのアクセス時に排他制御が必要となり、結局毎回排他制御を要求されてしまう…。
		@code
		typedef ThreadLocalVariable<int> TLInt;	// スレッドローカル変数型の定義。

		void ParallelCount()
		{
			TLInt tl_val(0);	// スレッドローカル変数の宣言。
			++(*tl_val);	// 間接参照演算子で変数値にアクセスする。
		}

		void ParallelProcess()
		{
			// スレッド関数。
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
			
			// スレッドローカル変数の内容を表示する。
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
			@brief	スレッドローカル変数（リスト）を初期化する。
					この関数を明示的に呼び出さなくても、初回使用時に初期化される。
		*/
		static bool Create()
		{
			return s_storage.Create();
		}

		/*!
			@brief	スレッドローカル変数（リスト）を解放する。
					この関数を明示的に呼び出さなくても、アプリケーション終了時に解放される。
		*/
		static void Release()
		{
			s_storage.Release();
		}

		/*!
			@brief	スレッドローカル変数リストを取得する。
					返却されたリストへのアクセスはスレッドセーフでは無い為注意。
		*/
		static Variables& GetVariables()
		{
			return s_storage._vars;
		}
		
		/*!
			@brief	スレッドローカル変数リストをコピーする。
					この関数はスレッドセーフである。
		*/
		static Variables& GetVariables(Variables& dst)
		{
			return s_storage.CopyVariables(dst);
		}
		
		/*!
			@brief	スレッドローカル変数を構築する。
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
			@brief	スレッドローカル変数値を取得する。
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
			@brief	スレッドローカル変数のメンバにアクセスする。
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
		@brief	スレッドローカル配列クラス。
		@tparam	Type	変数型。
		@tparam	Size	配列サイズ。
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
