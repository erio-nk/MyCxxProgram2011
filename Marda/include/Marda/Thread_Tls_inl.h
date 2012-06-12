/*!
	@file	Thread_inl.h
	
			�X���b�h���[�J���X�g���[�W�w���p�̎������B

	@author	E.Nakayama
*/

	//! �X���b�h���[�J���ϐ��f�t�H���g�^�O
	struct ThreadLocalVariable_TagDefault {};
	
	//! �X���b�h���[�J���X�g���[�WAPI
	template <typename Type>
	struct ThreadLocalVariable_TlsAPI_Default
	{
		// �K�[�h�I�u�W�F�N�g�^
		typedef GuardObject GuardObjectType;

		static bool Create(DWORD& storageIndex)
		{
			storageIndex = TlsAlloc();
			return storageIndex != 0xffffffff;
		}

		// �X���b�h���[�J���X�g���[�W���
		static void Release(DWORD storageIndex)
		{
			TlsFree(storageIndex);
		}

		// �X���b�h���[�J���X�g���[�W�l�擾
		static bool GetValue(DWORD storageIndex, Type*& val)
		{
			val = reinterpret_cast<Type*>(TlsGetValue(storageIndex));
			return is_not_null(val) && GetLastError() == NO_ERROR;
		}

		// �X���b�h���[�J���X�g���[�W�l�ݒ�
		static void SetValue(DWORD storageIndex, Type* val)
		{
			TlsSetValue(storageIndex, val);
		}
	};	// end struct ThreadLocalVariable_TlsAPI_Default

	//! �ϐ��m��API
	template <typename Type>
	struct ThreadLocalVariable_AllocAPI_Default
	{
		// �ϐ��m��
		static Type* Alloc()
		{
			return new Type;
		}

		// �ϐ����
		static void Free(Type* var)
		{
			delete var;
		}
	};	// end struct ThreadLocalVariable_AllocAPI_Default
	
	//! �z��m�ۃ|���V�[�N���X
	template <typename Type, int SIZE>
	struct ThreadLocalArray_AllocAPI_Default
	{
		// �ϐ��m��
		static Type* Alloc()
		{
			return new Type[SIZE];
		}

		// �ϐ����
		static void Free(Type* var)
		{
			delete[] var;
		}
	};	// end struct ThreadLocalArray_AllocPolicyDefault

	//! �X���b�h���[�J���X�g���[�W�n�N���X�̋��ʏ����B
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
			TlsAPI::GetValue(_storageIndex, pVar);	// ���x�d���Ō��ʔ���Ȃ��B
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
			TlsAPI::GetValue(_storageIndex, pVar);	// ���x�d���Ō��ʔ���Ȃ��B
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
