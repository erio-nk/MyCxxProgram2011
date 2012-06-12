/*!
	@file	Misc.h
	
			���ޕs�̃w���p�֐��E�N���X�Q�B

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"
	
namespace Marda
{
	// �R���\�[���̓��͂�����܂Ŏ~�߂�i�e�X�g�p�B�j
	#define PAUSE	getchar()
	
	// ------------------------------------------------------------------------
	// �e��}�N��
	
	// ������
	#define NUM2STR_(num)	#num
	#define NUM2STR(num)	NUM2STR_(num)	// ���������g����

	// wstring��
	#define WSTR_(astr)		L ## astr
	#define WSTR(astr)		WSTR_(astr)	// ���������g����
		
	// __FUNCTION__
	#define WFUNCTION	WSTR(__FUNCTION__)

	#if defined(_UNICODE)
		#define TFUNCTION	WFUNCTION
	#else
		#define TFUNCTION	__FUNCTION__
	#endif
		
	// ������� __LINE__
	#define STR_LINE	NUM2STR(__LINE__)
	#define WSTR_LINE	WSTR(NUM2STR(__LINE__))
	
	#if defined(_UNICODE)
		#define TSTR_LINE STR_LINE
	#else
		#define TSTR_LINE WSTR_LINE
	#endif

	// ------------------------------------------------------------------------
	// �����w��q�p�}�N���B
	
	// size_t�B�^�[�Q�b�g�}�V���� bit �����Ⴄ�ꍇ�ł��������o�͏o����B
	#if defined(_WIN64)
		#define FMT_SIZE_T	"%llu"
		#define WFMT_SIZE_T	L"%llu"
		#define TFMT_SIZE_T	_T("%llu")
	#else
		#define FMT_SIZE_T	"%u"
		#define WFMT_SIZE_T	L"%u"
		#define TFMT_SIZE_T	_T("%u")
	#endif
		
	// _UNICODE�̒�`�Ɋւ�炸�Achar/wchar_t����o�͂������ꍇ�ɗp����B
	#if defined(_UNICODE)
		#define FMT_ASTR	"%S"
		#define FMT_WSTR	"%s"
		#define WFMT_ASTR	L"%S"
		#define WFMT_WSTR	L"%s"
		#define TFMT_ASTR	_T("%S")
		#define TFMT_WSTR	_T("%s")
	#else
		#define FMT_ASTR	"%s"
		#define FMT_WSTR	"%S"
		#define WFMT_ASTR	L"%s"
		#define WFMT_WSTR	L"%S"
		#define TFMT_ASTR	_T("%s")
		#define TFMT_WSTR	_T("%S")
	#endif
		
	// bool �l�� true/false �ŕ\���������ꍇ�ɗp����B
	// printf("value=" TFMT_BOOL "\n", format_bool(value));
	#if defined(_UNICODE)
		#define FMT_BOOL	"%S"
		#define WFMT_BOOL	L"%s"
		#define TFMT_BOOL	WFMT_BOOL
	#else
		#define FMT_BOOL	"%s"
		#define WFMT_BOOL	L"%S"
		#define TFMT_BOOL	FMT_BOOL
	#endif
	inline const TCHAR* format_bool(bool value)
	{
		return value ? _T("true") : _T("false");
	}
	inline const TCHAR* format_bool(BOOL value)
	{
		return value != FALSE ? _T("true") : _T("false");
	}


	// �Q�i���\���B
	#define FMT_BIN8	"%08d"
	#define WFMT_BIN8	L"%08d"
	#define TFMT_BIN8	_T("%08d")

	inline int format_bin8(int value)
	{
		int result = 0;
		for (int i = 1; i < 100000000 /*10^8*/; i*=10, value >>= 1)
		{
			result += (value & 1) * i;
		}
		return result;
	}
	
	#define FMT_BIN16	"%08d%08d"
	#define WFMT_BIN16	L"%08d%08d"
	#define TFMT_BIN16	_T("%08d%08d")
		
	#define format_bin16(value)	format_bin8(HIBYTE(value)), format_bin8(LOBYTE(value))

	#define FMT_BIN32	"%08d%08d%08d%08d"
	#define WFMT_BIN32	L"%08d%08d%08d%08d"
	#define TFMT_BIN32	_T("%08d%08d%08d%08d")
		
	#define format_bin32(value)	format_bin8(HIBYTE(HIWORD(value))), format_bin8(LOBYTE(HIWORD(value))), format_bin8(HIBYTE(LOWORD(value))), format_bin8(LOBYTE(LOWORD(value)))
	
	// ------------------------------------------------------------------------
	// �ȈՃg���[�X�B

	#if defined(DISABLE_TRACE_THREAD_ID)
		#define AFMT_TRACE_THREAD_ID ""
		#define WFMT_TRACE_THREAD_ID L""
		#define TRACE_THREAD_ID
	#else
		#define AFMT_TRACE_THREAD_ID "[%x]"
		#define WFMT_TRACE_THREAD_ID L"[%x]"
		#define TRACE_THREAD_ID GetCurrentThreadId(),
	#endif

	#define ATRACE(format, ...)	printf(AFMT_TRACE_THREAD_ID __FUNCTION__ "#" STR_LINE "> " format "\n", TRACE_THREAD_ID __VA_ARGS__)
	#define WTRACE(format, ...) wprintf(WFMT_TRACE_THREAD_ID WFUNCTION L"#" WSTR_LINE L"> " format L"\n", TRACE_THREAD_ID __VA_ARGS__)
		
	#if defined(_UNICODE)
		#define TTRACE(format, ...)	WTRACE(format, __VA_ARGS__)
	#else
		#define TTRACE(format, ...)	ATRACE(format, __VA_ARGS__)
	#endif
		
	// �f�o�b�O�o�́B
	void DebugTraceA(const char* format, ...);
	void DebugTraceW(const wchar_t* format, ...);
	
	#if defined(_DEBUG)
		#define DEBUG_TRACEA	DebugTraceA
		#define DEBUG_TRACEW	DebugTraceW
	#else
		#define DEBUG_TRACEA(...)	__noop
		#define DEBUG_TRACEW(...)	__noop
	#endif
	
	#if defined(_UNICODE)
		#define DebugTrace	DebugTraceW
		#define DEBUG_TRACE	DEBUG_TRACEW
	#else
		#define DebugTrace	DebugTraceA
		#define DEBUG_TRACE	DEBUG_TRACEA
	#endif
	
	// ------------------------------------------------------------------------
	// ���ʃG���[�R�[�h�B
	enum ERROR_CODE
	{
		ERR_NOERROR,	//!< ����I���B
		ERR_CANCELED,	//!< ���������f���ꂽ�B
		ERR_UNKNOWN,	//!< �s���ȃG���[�B
	};
	
	// ------------------------------------------------------------------------
	// ��O�֘A�B

	//! �L���X�g���s��O�B
	struct ClassCastException : std::exception
	{
	};	// end struct ClassCastException

	//! �������s��O�B
	struct ElementNotFoundException : std::exception
	{
		const char* _msg;
		ElementNotFoundException(const char* msg) : _msg(msg) {}
	};	// end struct ElementNotFoundException

	/*! 
		@brief	���݂̃X���b�h�̍\������O�� C++ �W����O�ϊ�������L���ɂ���B
				�v���W�F�N�g�́u�\���v���p�e�B��C/C++���R�[�h������C++��O��L���ɂ���v��
				/EHa �ɂ��Ȃ���΂Ȃ�Ȃ��B
	*/
	void EnableStructuredExceptionTranslation(bool enable = true);
	
	/*!
		@brief	���݂̃X���b�h�̍\������O�� C++ �W����O�ϊ��������L���ɂȂ��Ă��邩�ǂ�����Ԃ��B
	*/
	bool IsEnabledStructuredExceptionTranslation();
	
	//! �\������O�B
	struct StructuredException : public std::exception
	{
		unsigned int _exceptionCode;
		EXCEPTION_RECORD _exceptionRecord;
		CONTEXT _context;
		StructuredException(unsigned int exceptionCode, PEXCEPTION_POINTERS exceptionPointers)
			: _exceptionCode(exceptionCode)
		{
			_exceptionRecord = *(exceptionPointers->ExceptionRecord);
			_context = *(exceptionPointers->ContextRecord);
		}
	};	// end struct StructuredException
	
	
	// ------------------------------------------------------------------------
	// �֎~�N���X�B
	
	struct Uncopyable
	{
	protected:
		Uncopyable() {}
		~Uncopyable() {}

	private:
		void operator = (const Uncopyable&);
		Uncopyable(const Uncopyable&);
		
	};	// end struct Uncopyable
	
	// TODO: Un-newable

	// ------------------------------------------------------------------------
	// �����C���N�������g�^�f�N�������g�B
	template <class ThreadingPolicy>
	inline LONG Increment(LONG& value);
	
	template <>
	inline LONG Increment<ThreadSafe>(LONG& value)
	{
		return InterlockedIncrement(&value);
	}

	template <>
	inline LONG Increment<ThreadUnsafe>(LONG& value)
	{
		return ++value;
	}

	template <class ThreadingPolicy>
	inline LONG Decrement(LONG& value);

	template <>
	inline LONG Decrement<ThreadSafe>(LONG& value)
	{
		return InterlockedDecrement(&value);
	}

	template <>
	inline LONG Decrement<ThreadUnsafe>(LONG& value)
	{
		return --value;
	}

	// ------------------------------------------------------------------------
	// �|�C���^�A�n���h���p�B
	
	/*!
		@brief	�|�C���^��NULL���ǂ����𔻕ʂ���
				���̊֐���p���邱�Ƃɂ��Aif (!ptr) �� if (ptr == NULL) �̗l��
				�o���o���ȋL�q�𓝈ꂵ�A�|�C���^��NULL�`�F�b�N���s���Ă��鎖�𖾎��I�ɂ���B
		@return	�|�C���^��NULL�ł����true
	*/
	template <class T>
	inline bool is_null(T ptr)
	{
		return ptr == NULL;
	}
	
	/*!
		@brief	�|�C���^��NULL�łȂ����ǂ����𔻕ʂ���
				�g�p�Ӑ}�Ƃ��Ă� is_null �֐��Ɠ��l�B
		@return	�|�C���^��NULL�łȂ����true
	*/
	template <class T>
	inline bool is_not_null(T ptr)
	{
		return ptr != NULL;
	}
	
	/*!
		@brief	�|�C���^�����S�ɉ������
				�|�C���^��NULL�Ŗ������ delete ���A�|�C���^��NULL�������鎖��
				��d�����h�~����B
				�z��̉���ɂ� safe_delete_array ��p���鎖�B
				std::for_each�̈����ɓn�������\�B
		@code
		void DeleteElements(std::vector<Hoge*>& elements)
		{
			std::for_each(elements.begin(), elements.end(), &safe_delete<Hoge*>);
		}
		@endcode
	*/
	template <class T>
	inline void safe_delete(T*& ptr)
	{
		if (is_not_null(ptr))
		{
			delete ptr;
			ptr = NULL;
		}
	}
	
	/*!
		@brief	�z������S�ɉ������
	*/
	template <class T>
	inline void safe_delete_array(T*& array)
	{
		if (is_not_null(array))
		{
			delete[] array;
			array = NULL;
		}
	}
	
	/*!
		@brief	�C���^�[�t�F�[�X�����S�ɉ������B
	*/
	template <class T>
	inline void safe_release(T*& iface)
	{
		if (is_not_null(iface))
		{
			iface->Release();
			iface = NULL;
		}
	}
	
	/*!
		@brief	�|�C���^�̒l���擾����
				std::auto_ptr, str::shared_ptr�� get_ptr �֐��ƋL�q�𓝈�o����B
	*/
	template <class T>
	inline T* get_ptr(T* ptr)
	{
		return ptr;
	}	
	
	/*!
		@brief	�n���h�������S�ɉ������
				�n���h����NULL�Ŗ������ CloseHandle ���A�n���h����NULL�������鎖��
				��d�����h�~����B
	*/
	template <HANDLE INVALID_VALUE>
	inline void safe_close(HANDLE& handle)
	{
		if (handle != INVALID_VALUE)
		{
			CloseHandle(handle);
			handle = INVALID_VALUE;
		}
	}

	/*!
		@brief	�����n���h���N���X�e���v���[�g
	*/
	template <
		HANDLE INVALID_VALUE = NULL
	>
	class auto_handle
	{
	public:
		auto_handle()
			: _handle(INVALID_VALUE)
		{
		}

		auto_handle(HANDLE handle)
			: _handle(handle)
		{
		}

		~auto_handle()
		{
			safe_close<INVALID_VALUE>(_handle);
		}

		void reset(HANDLE h = INVALID_VALUE)
		{
			if (_handle != INVALID_VALUE)
			{
				safe_close<INVALID_VALUE>(_handle);
			}
			_handle = h;
		}

		HANDLE get() const
		{
			return _handle;
		}

		bool is_valid() const
		{
			return _handle != INVALID_VALUE;
		}

		bool is_invalid() const
		{
			return _handle == INVALID_VALUE;
		}

		operator HANDLE()
		{
			return _handle;
		}

		operator HANDLE() const
		{
			return _handle;
		}

	private:
		HANDLE _handle;
	};	// end class auto_handle

	typedef auto_handle<INVALID_HANDLE_VALUE> AUTO_FILE_HANDLE;
	typedef auto_handle<NULL> AUTO_THREAD_HANDLE;
	typedef auto_handle<NULL> AUTO_EVENT_HANDLE;
	

	/*!
		@brief	�����C���^�[�t�F�[�X�N���X�e���v���[�g�B
	*/
	template <class Interface>
	class auto_interface
	{
	public:
		auto_interface()
			: _interface(NULL)
		{
		}

		auto_interface(Interface* iface)
			: _interface(iface)
		{
		}

		~auto_interface()
		{
			safe_release<Interface>(_interface);
		}

		void reset(Interface* iface = NULL)
		{
			if (is_not_null(_interface))
			{
				safe_release<Interface>(_interface);
			}
			_interface = iface;
		}
		
		Interface* release()
		{
			Interface* result = _interface;
			_interface = NULL;
			return result;
		}

		Interface* get() const
		{
			return _interface;
		}
		
		Interface* operator -> ()
		{
			return _interface;
		}

		const Interface* operator -> () const
		{
			return _interface;
		}

		operator Interface*()
		{
			return _interface;
		}

		operator Interface*() const
		{
			return _interface;
		}

	private:
		Interface* _interface;
	};	// end class auto_interface

	template <class T>
	inline bool is_null(const auto_interface<T>& iface)
	{
		return iface.get() == NULL;
	}

	template <class T>
	inline bool is_not_null(const auto_interface<T>& iface)
	{
		return iface.get() != NULL;
	}

	/*!
		@brief	�X�}�[�g�|�C���^�N���X�e���v���[�g�B
				std::shared_ptr �ƃC���^�t�F�[�X�����킹�Ă���i�������֐��͂��̓s�x�����j�B
				�X���b�f�B���O�|���V�[���w��o����B
				���ɗ��R�������ꍇ�� std::shared_ptr �𗘗p���鎖�B
		@tparam	Type	�^�B
		@tparam	ThreadingPolicy	�X���b�f�B���O�|���V�[�B
	*/
	template <class Type, class ThreadingPolicy>
	class smart_ptr
	{
	public:
		smart_ptr()
			: _ptr(NULL)
			, _pRef(NULL)
		{
		}
		
		smart_ptr(Type* ptr)
			: _ptr(ptr)
		{
			_pRef = new LONG(1);
		}
		
		smart_ptr(const smart_ptr& src)
		{
			_ptr = src._ptr;
			_pRef = src._pRef;
			Increment<ThreadingPolicy>(*_pRef);
		}
		
		~smart_ptr()
		{
			reset();
		}
		
		smart_ptr& operator = (const smart_ptr& src)
		{
			if (&src != this)
			{
				reset();
				_ptr = src._ptr;
				_pRef = src._pRef;
				Increment<ThreadingPolicy>(*_pRef);
			}
			
			return *this;
		}
		
		Type& operator * ()
		{
			return *_ptr;
		}
		
		const Type& operator * () const
		{
			return *_ptr;
		}
		
		Type* operator -> ()
		{
			return _ptr;
		}
		
		const Type* operator -> () const
		{
			return _ptr;
		}

		bool operator == (const smart_ptr& rhs)
		{
			return _ptr == rhs._ptr;
		}
		
		bool operator != (const smart_ptr& rhs)
		{
			return _ptr != rhs._ptr;
		}

		bool operator < (const smart_ptr& rhs)
		{
			return _ptr < rhs._ptr;
		}

		bool operator > (const smart_ptr& rhs)
		{
			return _ptr > rhs._ptr;
		}

		bool operator <= (const smart_ptr& rhs)
		{
			return _ptr <= rhs._ptr;
		}

		bool operator >= (const smart_ptr& rhs)
		{
			return _ptr >= rhs._ptr;
		}
		
		Type* get()
		{
			return _ptr;
		}
		
		const Type* get() const
		{
			return _ptr;
		}

		void reset()
		{
			if (is_not_null(_ptr))
			{
				if (Decrement<ThreadingPolicy>(*_pRef) <= 0)
				{
					safe_delete(_ptr);
					safe_delete(_pRef);
				}
				_ptr = NULL;
				_pRef = NULL;
			}
		}
		
		void reset(Type* src)
		{
			reset();
			_ptr = src;
			_pRef = new LONG(1);
		}
		
	private:
		Type* _ptr;	//!< ���|�C���^�B
		LONG* _pRef;	//!< �Q�ƃJ�E���^�B
		
	};	// end class smart_ptr

	template <class Type, class ThreadingPolicy>
	inline bool is_null(const smart_ptr<Type, ThreadingPolicy>& ptr)
	{
		return ptr.get() == NULL;
	}
	
	template <class Type, class ThreadingPolicy>
	inline bool is_not_null(const smart_ptr<Type, ThreadingPolicy>& ptr)
	{
		return ptr.get() != NULL;
	}

	template <class Type, class ThreadingPolicy>
	inline void safe_delete(smart_ptr<Type, ThreadingPolicy>& ptr)
	{
		ptr.reset();
	}
	
	template <class Type, class ThreadingPolicy>
	inline Type* get_ptr(smart_ptr<Type, ThreadingPolicy>& ptr)
	{
		return ptr.get();
	}
	

	// ------------------------------------------------------------------------
	// �L���X�g�B

	#if defined(_DEBUG)
		/*!
			@brief	�_�E���L���X�g������֐��B
				�݌v��_�E���L���X�g���s���Ƃ������𖾎�����ׂɎg�p���A
				�J�����̌�L���X�g�𖢑R�Ɍ��o����B
				Release���[�h���ɂ́A�P�Ȃ�static_cast�ɒu�������B
			@code
				struct Hoge {...};
				struct Fuga : public Hoge {...};
				struct Piyo : public Hoge {...};

				Hoge* pHogeFuga = new Fuga();
				Hoge* pHogePiyo = new Piyo();

				Fuga* pFugaFuga = down_cast<Fuga*>(pHogeFuga);	// OK
				Fuga* pFugaPiyo = down_cast<Fuga*>(pHogePiyo);	// NG
			@endcode

			@tparam	T	�L���X�g����^�̃|�C���^�^�B
			@tparam	U	�L���X�g�����^�̃|�C���^�^�B
			@param	u	�L���X�g�����|�C���^�B
			@return	�_�E���L���X�g���ʁB
		 */
		template <class T, class U>
		T down_cast(U u) 
		{
			T t = dynamic_cast<T>(u);
			if (t != static_cast<T>(u)) 
				throw ClassCastException();
			return t;
		}
	#else
		#define down_cast static_cast
	#endif

	// ------------------------------------------------------------------------
	// �����̐��ۗp
	
	/*!
		@brief	bool �^�߂�l�̊֐��̏��������������ꍇ��true��Ԃ��B
				���̊֐���p���邱�Ƃɂ��Aif (function()) �� if (function() == true) �̗l��
				�o���o���ȋL�q�𓝈ꂵ�A������������������ if �� �ɓ��鎖�𖾎��I�ɂ���B
		@return	result �� true �Ȃ� true ��Ԃ��B
	*/
	inline bool succeeded(bool result)
	{
		return result == true;
	}
	
	inline bool failed(bool result)
	{
		return result == false;
	}
	
	inline BOOL succeeded(BOOL result)
	{
		return result != FALSE;	// WindowsAPI�͐�������0�ȊO�̒l��Ԃ��ׁB
	}
	
	inline BOOL failed(BOOL result)
	{
		return result == FALSE;
	}
	
	inline bool succeeded(ERROR_CODE result)
	{
		return result == ERR_NOERROR;
	}
	
	inline bool failed(ERROR_CODE result)
	{
		return result != ERR_NOERROR;
	}
	
	// ------------------------------------------------------------------------
	// float �l�`�F�b�N�B
	
	inline bool is_bad_float(float value)
	{
		return _finite(value) == 0;
	}
	
	// float �l�`�F�b�N�����A�s���l��������g���[�X������B
	#define IS_BAD_FLOAT(value) (is_bad_float(value) ? (TTRACE(_T(#value) _T(" is bad!")), true), false)

	// ------------------------------------------------------------------------
	// �t���O�B
	
	/*!
		@brief	�t���O�N���X�B32�܂ŁB
				bool�����o���R���悤�ȃN���X�ł́A���̃N���X�ɂ܂Ƃ߂Ă��܂�������
				�������I�ɂ����B���ʂɃN���X�T�C�Y��傫�����Ȃ��̂ŃL���b�V���q�b�g���ɂ��v���o����H
		@code
		// RPG�̃o�b�h�X�e�[�^�X�t���O
		enum BAD_STATUS_FLAGS
		{
			POISON		= 0x0001,
			BLIND		= 0x0002,
			PARALYSIS	= 0x0004,
			SILENCE		= 0x0008,
			SLEEP		= 0x0010,
			CONFUSE		= 0x0020,
			STONE		= 0x0040,
			ZOMBIE		= 0x0080,
		};
		
		Flags badStatus;
		badStatus.set(POISON | SILENCE);
		if (badStatus.is(POISON))
		{
			// �ł̎��̏����c�B
		}
		
		@endcode
	*/
	struct Flags
	{
		int _value;
		
		Flags()
			: _value(0)
		{
		}
		
		Flags(int value)
			: _value(value)
		{
		}
		
		/*!
			@brief	�t���O���Z�b�g����B
			@param	Flags	�Z�b�g����t���O�B
		*/
		void set(int flags)
		{
			_value |= flags;
		}
		
		/*!
			@brief	�t���O���Z�b�g����B
			@param	flags	�Z�b�g����t���O�B
			@param	b		�Z�b�g�̏ꍇ��true�A���Z�b�g�̏ꍇ��false�B
		*/
		void set(int flags, bool b)
		{
			if (b)
			{
				set(flags);
			}
			else
			{
				reset(flags);
			}
		}
		
		/*!
			@brief	�t���O�����Z�b�g����B
			@param	flags	���Z�b�g����t���O�B
		*/
		void reset(int flags)
		{
			_value &= ~flags;
		}
		
		/*!
			@brief	�t���O���N���A����
		*/
		void clear()
		{
			_value = 0;
		}
		
		/*!
			@brief	�w��̃t���O�������Ă��邩�ǂ������ׂ�B
			@param	flags	���ׂ�t���O�B
		*/
		bool is(int flags) const
		{
			return (_value & flags) != 0;
		}
		
		bool empty() const
		{
			return _value == 0;
		}
		
		/*!
			@brief	�t���O���Ƃ̘_���a��Ԃ��B
		*/
		Flags or(int flags) const
		{
			return _value | flags;
		}
		
		/*!
			@brief	�t���O���Ƃ̘_���ς�Ԃ��B
		*/
		Flags and(int flags) const
		{
			return _value & flags;
		}
		
		/*!
			@brief	�t���O���Ƃ̔r���I�_���a��Ԃ��B
		*/
		Flags xor(int flags) const
		{
			return _value ^ flags;
		}
		
		/*!
			@brief	�t���O���Ƃ̘_���ے��Ԃ��B
		*/
		Flags not() const
		{
			return ~_value;
		}
		
		/*!
			@brief	�w��t���O�����_���ے�������l��Ԃ��B
		*/
		Flags not(int flags) const
		{
			return not().and(flags).or(and(~flags));
		}
		
		/*!
			@brief	�t���O���Ƃ̔ے�_���a��Ԃ��B
		*/
		Flags nor(int flags) const
		{
			return or(flags).not();
		}

		/*!
			@brief	�t���O���Ƃ̔ے�_���ς�Ԃ��B
		*/
		Flags nand(int flags) const
		{
			return and(flags).not();
		}
		
		operator int ()
		{
			return _value;
		}
		
	};	// end struct Flags
	
	// class Flags64
	
	// class VFlags


	// ------------------------------------------------------------------------
	// �z�񓙂̃J�[�\���p�B
	
	//! �J�[�\����O���ɐi�߂�B���ʂ� max �𒴂���ꍇ�́Amax ��ԋp����B
	inline size_t cursor_forward(size_t current, size_t forward, size_t max)
	{
		if (max - current < forward)
			return max;
		return current + forward;
	}
	
	//! �J�[�\��������ɐi�߂�B���ʂ� 0�@�������ꍇ�́A0 ��ԋp����B
	inline size_t cursor_backward(size_t current, size_t backward)
	{
		if (current < backward)
			return 0;
		return current + backward;
	}
	
	//! �J�[�\����O���ɐi�߂�B���ʂ� max �𒴂���ꍇ�́A0 �ɖ߂�B
	inline size_t cursor_forward_rot(size_t current, size_t forward, size_t max)
	{
		return (current + forward) % max;
	}
	
	//! �J�[�\��������ɐi�߂�B���ʂ� 0 �������ꍇ�́Amax - 1 �ɖ߂�B
	inline size_t cursor_backward_rot(size_t current, size_t backward, size_t max)
	{
		return (current + (max - backward)) % max;
	}

	// ------------------------------------------------------------------------
	// �}���`�o�C�g������E���C�h������ϊ��B
	inline void StringToWString(const std::string& src, std::wstring& dst)
	{
		unsigned int numWChars = 0;
		numWChars = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0); 
		std::vector<wchar_t> buf;
		buf.resize(numWChars);
		MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], numWChars);
		dst = &buf[0];
	}
	
	#if defined(_UNICODE)
	#define StringToTString StringToWString
	#else
	#define StringToTString(src, dst)	(dst = src)
	#endif
	
}	// end namespace Marda
