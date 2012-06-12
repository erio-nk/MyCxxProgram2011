/*!
	@file	Misc.h
	
			分類不可のヘルパ関数・クラス群。

	@author	E.Nakayama
*/
#pragma once
	
#include "Types.h"
	
namespace Marda
{
	// コンソールの入力があるまで止める（テスト用。）
	#define PAUSE	getchar()
	
	// ------------------------------------------------------------------------
	// 各種マクロ
	
	// 文字列化
	#define NUM2STR_(num)	#num
	#define NUM2STR(num)	NUM2STR_(num)	// こっちを使う事

	// wstring化
	#define WSTR_(astr)		L ## astr
	#define WSTR(astr)		WSTR_(astr)	// こっちを使う事
		
	// __FUNCTION__
	#define WFUNCTION	WSTR(__FUNCTION__)

	#if defined(_UNICODE)
		#define TFUNCTION	WFUNCTION
	#else
		#define TFUNCTION	__FUNCTION__
	#endif
		
	// 文字列版 __LINE__
	#define STR_LINE	NUM2STR(__LINE__)
	#define WSTR_LINE	WSTR(NUM2STR(__LINE__))
	
	#if defined(_UNICODE)
		#define TSTR_LINE STR_LINE
	#else
		#define TSTR_LINE WSTR_LINE
	#endif

	// ------------------------------------------------------------------------
	// 書式指定子用マクロ。
	
	// size_t。ターゲットマシンの bit 数が違う場合でも正しく出力出来る。
	#if defined(_WIN64)
		#define FMT_SIZE_T	"%llu"
		#define WFMT_SIZE_T	L"%llu"
		#define TFMT_SIZE_T	_T("%llu")
	#else
		#define FMT_SIZE_T	"%u"
		#define WFMT_SIZE_T	L"%u"
		#define TFMT_SIZE_T	_T("%u")
	#endif
		
	// _UNICODEの定義に関わらず、char/wchar_t列を出力したい場合に用いる。
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
		
	// bool 値を true/false で表示したい場合に用いる。
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


	// ２進数表示。
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
	// 簡易トレース。

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
		
	// デバッグ出力。
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
	// 共通エラーコード。
	enum ERROR_CODE
	{
		ERR_NOERROR,	//!< 正常終了。
		ERR_CANCELED,	//!< 処理が中断された。
		ERR_UNKNOWN,	//!< 不明なエラー。
	};
	
	// ------------------------------------------------------------------------
	// 例外関連。

	//! キャスト失敗例外。
	struct ClassCastException : std::exception
	{
	};	// end struct ClassCastException

	//! 検索失敗例外。
	struct ElementNotFoundException : std::exception
	{
		const char* _msg;
		ElementNotFoundException(const char* msg) : _msg(msg) {}
	};	// end struct ElementNotFoundException

	/*! 
		@brief	現在のスレッドの構造化例外の C++ 標準例外変換処理を有効にする。
				プロジェクトの「構成プロパティ＞C/C++＞コード生成＞C++例外を有効にする」を
				/EHa にしなければならない。
	*/
	void EnableStructuredExceptionTranslation(bool enable = true);
	
	/*!
		@brief	現在のスレッドの構造化例外の C++ 標準例外変換処理が有効になっているかどうかを返す。
	*/
	bool IsEnabledStructuredExceptionTranslation();
	
	//! 構造化例外。
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
	// 禁止クラス。
	
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
	// 同期インクリメント／デクリメント。
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
	// ポインタ、ハンドル用。
	
	/*!
		@brief	ポインタがNULLかどうかを判別する
				この関数を用いることにより、if (!ptr) や if (ptr == NULL) の様な
				バラバラな記述を統一し、ポインタのNULLチェックを行っている事を明示的にする。
		@return	ポインタがNULLであればtrue
	*/
	template <class T>
	inline bool is_null(T ptr)
	{
		return ptr == NULL;
	}
	
	/*!
		@brief	ポインタがNULLでないかどうかを判別する
				使用意図としては is_null 関数と同様。
		@return	ポインタがNULLでなければtrue
	*/
	template <class T>
	inline bool is_not_null(T ptr)
	{
		return ptr != NULL;
	}
	
	/*!
		@brief	ポインタを安全に解放する
				ポインタがNULLで無ければ delete し、ポインタにNULLを代入する事で
				二重解放を防止する。
				配列の解放には safe_delete_array を用いる事。
				std::for_eachの引数に渡す事も可能。
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
		@brief	配列を安全に解放する
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
		@brief	インターフェースを安全に解放する。
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
		@brief	ポインタの値を取得する
				std::auto_ptr, str::shared_ptr版 get_ptr 関数と記述を統一出来る。
	*/
	template <class T>
	inline T* get_ptr(T* ptr)
	{
		return ptr;
	}	
	
	/*!
		@brief	ハンドルを安全に解放する
				ハンドルがNULLで無ければ CloseHandle し、ハンドルにNULLを代入する事で
				二重解放を防止する。
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
		@brief	自動ハンドルクラステンプレート
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
		@brief	自動インターフェースクラステンプレート。
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
		@brief	スマートポインタクラステンプレート。
				std::shared_ptr とインタフェースを合わせてある（未実装関数はその都度実装）。
				スレッディングポリシーを指定出来る。
				特に理由が無い場合は std::shared_ptr を利用する事。
		@tparam	Type	型。
		@tparam	ThreadingPolicy	スレッディングポリシー。
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
		Type* _ptr;	//!< 生ポインタ。
		LONG* _pRef;	//!< 参照カウンタ。
		
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
	// キャスト。

	#if defined(_DEBUG)
		/*!
			@brief	ダウンキャストをする関数。
				設計上ダウンキャストを行うという事を明示する為に使用し、
				開発時の誤キャストを未然に検出する。
				Releaseモード時には、単なるstatic_castに置き換わる。
			@code
				struct Hoge {...};
				struct Fuga : public Hoge {...};
				struct Piyo : public Hoge {...};

				Hoge* pHogeFuga = new Fuga();
				Hoge* pHogePiyo = new Piyo();

				Fuga* pFugaFuga = down_cast<Fuga*>(pHogeFuga);	// OK
				Fuga* pFugaPiyo = down_cast<Fuga*>(pHogePiyo);	// NG
			@endcode

			@tparam	T	キャストする型のポインタ型。
			@tparam	U	キャストされる型のポインタ型。
			@param	u	キャストされるポインタ。
			@return	ダウンキャスト結果。
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
	// 処理の成否用
	
	/*!
		@brief	bool 型戻り値の関数の処理が成功した場合はtrueを返す。
				この関数を用いることにより、if (function()) や if (function() == true) の様な
				バラバラな記述を統一し、処理が成功した時に if 文 に入る事を明示的にする。
		@return	result が true なら true を返す。
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
		return result != FALSE;	// WindowsAPIは成功時に0以外の値を返す為。
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
	// float 値チェック。
	
	inline bool is_bad_float(float value)
	{
		return _finite(value) == 0;
	}
	
	// float 値チェックをしつつ、不正値だったらトレースをする。
	#define IS_BAD_FLOAT(value) (is_bad_float(value) ? (TTRACE(_T(#value) _T(" is bad!")), true), false)

	// ------------------------------------------------------------------------
	// フラグ。
	
	/*!
		@brief	フラグクラス。32個まで。
				boolメンバを沢山持つようなクラスでは、このクラスにまとめてしまった方が
				メモリ的にお得。無駄にクラスサイズを大きくしないのでキャッシュヒット率にも貢献出来る？
		@code
		// RPGのバッドステータスフラグ
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
			// 毒の時の処理…。
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
			@brief	フラグをセットする。
			@param	Flags	セットするフラグ。
		*/
		void set(int flags)
		{
			_value |= flags;
		}
		
		/*!
			@brief	フラグをセットする。
			@param	flags	セットするフラグ。
			@param	b		セットの場合はtrue、リセットの場合はfalse。
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
			@brief	フラグをリセットする。
			@param	flags	リセットするフラグ。
		*/
		void reset(int flags)
		{
			_value &= ~flags;
		}
		
		/*!
			@brief	フラグをクリアする
		*/
		void clear()
		{
			_value = 0;
		}
		
		/*!
			@brief	指定のフラグが立っているかどうか調べる。
			@param	flags	調べるフラグ。
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
			@brief	フラグごとの論理和を返す。
		*/
		Flags or(int flags) const
		{
			return _value | flags;
		}
		
		/*!
			@brief	フラグごとの論理積を返す。
		*/
		Flags and(int flags) const
		{
			return _value & flags;
		}
		
		/*!
			@brief	フラグごとの排他的論理和を返す。
		*/
		Flags xor(int flags) const
		{
			return _value ^ flags;
		}
		
		/*!
			@brief	フラグごとの論理否定を返す。
		*/
		Flags not() const
		{
			return ~_value;
		}
		
		/*!
			@brief	指定フラグだけ論理否定をした値を返す。
		*/
		Flags not(int flags) const
		{
			return not().and(flags).or(and(~flags));
		}
		
		/*!
			@brief	フラグごとの否定論理和を返す。
		*/
		Flags nor(int flags) const
		{
			return or(flags).not();
		}

		/*!
			@brief	フラグごとの否定論理積を返す。
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
	// 配列等のカーソル用。
	
	//! カーソルを前方に進める。結果が max を超える場合は、max を返却する。
	inline size_t cursor_forward(size_t current, size_t forward, size_t max)
	{
		if (max - current < forward)
			return max;
		return current + forward;
	}
	
	//! カーソルを後方に進める。結果が 0　を下回る場合は、0 を返却する。
	inline size_t cursor_backward(size_t current, size_t backward)
	{
		if (current < backward)
			return 0;
		return current + backward;
	}
	
	//! カーソルを前方に進める。結果が max を超える場合は、0 に戻る。
	inline size_t cursor_forward_rot(size_t current, size_t forward, size_t max)
	{
		return (current + forward) % max;
	}
	
	//! カーソルを後方に進める。結果が 0 を下回る場合は、max - 1 に戻る。
	inline size_t cursor_backward_rot(size_t current, size_t backward, size_t max)
	{
		return (current + (max - backward)) % max;
	}

	// ------------------------------------------------------------------------
	// マルチバイト文字列・ワイド文字列変換。
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
