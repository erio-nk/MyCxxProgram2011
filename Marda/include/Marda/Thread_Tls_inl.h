/*!
	@file	Thread_inl.h
	
			スレッドローカルストレージヘルパの実装部。

	@author	E.Nakayama
*/

	//! スレッドローカル変数デフォルトタグ
	struct ThreadLocalVariable_TagDefault {};
	
	//! スレッドローカルストレージAPI
	template <typename Type>
	struct ThreadLocalVariable_TlsAPI_Default
	{
		// ガードオブジェクト型
		typedef GuardObject GuardObjectType;

		static bool Create(DWORD& storageIndex)
		{
			storageIndex = TlsAlloc();
			return storageIndex != 0xffffffff;
		}

		// スレッドローカルストレージ解放
		static void Release(DWORD storageIndex)
		{
			TlsFree(storageIndex);
		}

		// スレッドローカルストレージ値取得
		static bool GetValue(DWORD storageIndex, Type*& val)
		{
			val = reinterpret_cast<Type*>(TlsGetValue(storageIndex));
			return is_not_null(val) && GetLastError() == NO_ERROR;
		}

		// スレッドローカルストレージ値設定
		static void SetValue(DWORD storageIndex, Type* val)
		{
			TlsSetValue(storageIndex, val);
		}
	};	// end struct ThreadLocalVariable_TlsAPI_Default

	//! 変数確保API
	template <typename Type>
	struct ThreadLocalVariable_AllocAPI_Default
	{
		// 変数確保
		static Type* Alloc()
		{
			return new Type;
		}

		// 変数解放
		static void Free(Type* var)
		{
			delete var;
		}
	};	// end struct ThreadLocalVariable_AllocAPI_Default
	
	//! 配列確保ポリシークラス
	template <typename Type, int SIZE>
	struct ThreadLocalArray_AllocAPI_Default
	{
		// 変数確保
		static Type* Alloc()
		{
			return new Type[SIZE];
		}

		// 変数解放
		static void Free(Type* var)
		{
			delete[] var;
		}
	};	// end struct ThreadLocalArray_AllocPolicyDefault

	//! スレッドローカルストレージ系クラスの共通処理。
	template <
		typename Type,
		class AllocAPI,
		class TlsAPI
	>
	struct ThreadLocalStorage
	{
		typedef std::vector<Type*> Variables;

		DWORD _storageIndex;
		Variables _vars;
		typename TlsAPI::GuardObjectType _guard;
		
		bool (ThreadLocalStorage::*_pfnAllocPreprocess)();
		
		ThreadLocalStorage()
			: _storageIndex(NULL)
			, _pfnAllocPreprocess(&ThreadLocalStorage::Create)
		{
		}

		~ThreadLocalStorage()
		{
			Release();
		}

		bool Create()
		{
			Lock<typename TlsAPI::GuardObjectType> lock(_guard);
			_pfnAllocPreprocess = &ThreadLocalStorage::Noop;
			return TlsAPI::Create(_storageIndex);
		}
		
		bool Noop()
		{
			return true;
		}

		void Release()
		{
			Lock<typename TlsAPI::GuardObjectType> lock(_guard);
			std::for_each(_vars.begin(), _vars.end(), AllocAPI::Free);
			_vars.clear();
			TlsAPI::Release(_storageIndex);
			_storageIndex = NULL;
		}

		Type* Alloc()
		{
			(this->*_pfnAllocPreprocess)();

			Type* pVar = NULL;
			TlsAPI::GetValue(_storageIndex, pVar);	// 速度重視で結果判定なし。
			{
				pVar = AllocAPI::Alloc();
				Lock<typename TlsAPI::GuardObjectType> lock(_guard);
				_vars.push_back(pVar);
				TlsAPI::SetValue(_storageIndex, pVar);
			}
			return pVar;
		}
		
		Type* Alloc(Type& initialValue)
		{
			(this->*_pfnAllocPreprocess)();

			Type* pVar = NULL;
			TlsAPI::GetValue(_storageIndex, pVar);	// 速度重視で結果判定なし。
			{
				pVar = AllocAPI::Alloc();
				*pVar = initialValue;
				Lock<typename TlsAPI::GuardObjectType> lock(_guard);
				_vars.push_back(pVar);
				TlsAPI::SetValue(_storageIndex, pVar);
			}
			return pVar;
		}
		
		void CopyVariables(Variables& dst)
		{
			Lock<typename TlsAPI::GuardObjectType> lock(_guard);
			dst = _vars;
		}
	};	// edn struct ThreadLocalStorage
