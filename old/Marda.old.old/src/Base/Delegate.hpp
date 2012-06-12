/*!
	@file Delegate.hpp

	デリゲートの実装。
	力技ですが何か。


	Copyright(C) 2008-2009 Dila <yamadila@gmail.com>
		All rights reserved.

	@author Dila
 */
#pragma once

namespace Marda
{
	template
	<
		class R,
		class P1 = void,
		class P2 = void,
		class P3 = void,
		class End = void
	>
	class Delegate_Func;

	template
	<
		class T,
		class R,
		class P1 = void,
		class P2 = void,
		class P3 = void,
		class End = void
	>
	class Delegate_MemFunc;


	// ----------------------------------------------------------------------------------------- //
	// 引数0個で特殊化
	
	template <class R>
	class Delegate<R>
	{
	public:
		virtual ~Delegate() {}
		virtual R operator () () = 0;

		static Delegate<R>* New(R (*pfn)())
		{ return new Delegate_Func<R>(pfn); }

		template <class T>
		static Delegate<R>* New(T* pObj, R (T::*pfn)())
		{ return new Delegate_MemFunc<T, R>(pObj, pfn); }
	};

	// 関数用
	template <class R>
	class Delegate_Func<R> : public Delegate<R>
	{
	public:
		Delegate_Func(R (*pfn)()) : _pfn(pfn) {}
		R operator () ()
		{ return (*_pfn)(); }
	private:
		R (*_pfn)();
	};

	// メンバ関数用
	template <class T, class R>
	class Delegate_MemFunc<T, R> : public Delegate<R>
	{
	public:
		Delegate_MemFunc(T* pObj, R (T::*pfn)()) : _pObj(pObj), _pfn(pfn) {}
		R operator () ()
		{ return (_pObj->*_pfn)(); }
	private:
		T* _pObj;
		R (T::*_pfn)();
	};

	//// 更に戻り値voidで特殊化
	// 関数用
	template <>
	class Delegate_Func<void> : public Delegate<void>
	{
	public:
		Delegate_Func(void (*pfn)()) : _pfn(pfn) {}
		void operator () ()
		{ (*_pfn)(); /* no return */ }
	private:
		void (*_pfn)();
	};
	
	// メンバ関数用
	template <class T>
	class Delegate_MemFunc<T, void> : public Delegate<void>
	{
	public:
		Delegate_MemFunc(T* pObj, void (T::*pfn)()) : _pObj(pObj), _pfn(pfn) {}
		void operator () ()
		{ (_pObj->*_pfn)(); /* no return */ }
	private:
		T* _pObj;
		void (T::*_pfn)();
	};


	// ----------------------------------------------------------------------------------------- //
	// 引数1個で特殊化
	template <class R, class P1>
	class Delegate<R, P1>
	{
	public:
		virtual ~Delegate() {}
		virtual R operator () (P1 p1) = 0;

		static Delegate<R, P1>* New(R (*pfn)(P1))
		{ return new Delegate_Func<R, P1>(pfn); }

		template <class T>
		static Delegate<R, P1>* New(T* pObj, R (T::*pfn)(P1))
		{ return new Delegate_MemFunc<T, R, P1>(pObj, pfn); }
	};

	// 関数用
	template <class R, class P1>
	class Delegate_Func<R, P1> : public Delegate<R, P1>
	{
	public:
		Delegate_Func(R (*pfn)(P1)) : _pfn(pfn) {}
		R operator () (P1 p1)
		{ return (*_pfn)(p1); }
	private:
		R (*_pfn)(P1);
	};
	
	// メンバ関数用
	template <class T, class R, class P1>
	class Delegate_MemFunc<T, R, P1> : public Delegate<R, P1>
	{
	public:
		Delegate_MemFunc(T* pObj, R (T::*pfn)(P1)) : _pObj(pObj), _pfn(pfn) {}
		R operator () (P1 p1)
		{ return (_pObj->*_pfn)(p1); }
	private:
		T* _pObj;
		R (T::*_pfn)(P1);
	};

	//// 更に戻り値voidで特殊化
	// 関数用
	template <class P1>
	class Delegate_Func<void, P1> : public Delegate<void, P1>
	{
	public:
		Delegate_Func(void (*pfn)(P1)) : _pfn(pfn) {}
		void operator () (P1 p1)
		{ (*_pfn)(p1); /* no return */ }
	private:
		void (*_pfn)(P1);
	};

	// メンバ関数用
	template <class T, class P1>
	class Delegate_MemFunc<T, void, P1> : public Delegate<void, P1>
	{
	public:
		Delegate_MemFunc(T* pObj, void (T::*pfn)(P1)) : _pObj(pObj), _pfn(pfn) {}
		void operator () (P1 p1)
		{ (_pObj->*_pfn)(p1); /* no return */ }
	private:
		T* _pObj;
		void (T::*_pfn)(P1);
	};

	
	// ----------------------------------------------------------------------------------------- //
	// 引数2個で特殊化
	template <class R, class P1, class P2>
	class Delegate<R, P1, P2>
	{
	public:
		virtual ~Delegate() {}
		virtual R operator () (P1 p1, P2 p2) = 0;

		static Delegate<R, P1, P2>* New(R (*pfn)(P1, P2))
		{ return new Delegate_Func<R, P1, P2>(pfn); }

		template <class T>
		static Delegate<R, P1, P2>* New(T* pObj, R (T::*pfn)(P1, P2))
		{ return new Delegate_MemFunc<T, R, P1, P2>(pObj, pfn); }
	};

	// 関数用
	template <class R, class P1, class P2>
	class Delegate_Func<R, P1, P2> : public Delegate<R, P1, P2>
	{
	public:
		Delegate_Func(R (*pfn)(P1, P2)) : _pfn(pfn) {}
		R operator () (P1 p1, P2 p2)
		{ return (*_pfn)(p1, p2); }
	private:
		R (*_pfn)(P1, P2);
	};

	// メンバ関数用
	template <class T, class R, class P1, class P2>
	class Delegate_MemFunc<T, R, P1, P2> : public Delegate<R, P1, P2>
	{
	public:
		Delegate_MemFunc(T* pObj, R (T::*pfn)(P1, P2)) : _pObj(pObj), _pfn(pfn) {}
		R operator () (P1 p1, P2 p2)
		{ return (_pObj->*_pfn)(p1, p2); }
	private:
		T* _pObj;
		R (T::*_pfn)(P1, P2);
	};

	//// 更に戻り値voidで特殊化
	// 関数用
	template <class P1, class P2>
	class Delegate_Func<void, P1, P2> : public Delegate<void, P1, P2>
	{
	public:
		Delegate_Func(void (*pfn)(P1, P2)) : _pfn(pfn) {}
		void operator () (P1 p1, P2 p2)
		{ (*_pfn)(p1, p2); /* no return */ }
	private:
		void (*_pfn)(P1, P2);
	};
	
	// メンバ関数用
	template <class T, class P1, class P2>
	class Delegate_MemFunc<T, void, P1, P2> : public Delegate<void, P1, P2>
	{
	public:
		Delegate_MemFunc(T* pObj, void (T::*pfn)(P1, P2)) : _pObj(pObj), _pfn(pfn) {}
		void operator () (P1 p1, P2 p2)
		{ (_pObj->*_pfn)(p1, p2); /* no return */ }
	private:
		T* _pObj;
		void (T::*_pfn)(P1, P2);
	};

	// ----------------------------------------------------------------------------------------- //
	// 引数3個で特殊化
	template <class R, class P1, class P2, class P3>
	class Delegate<R, P1, P2, P3>
	{
	public:
		virtual ~Delegate() {}
		virtual R operator () (P1 p1, P2 p2, P3) = 0;

		static Delegate<R, P1, P2, P3>* New(R (*pfn)(P1, P2, P3))
		{ return new Delegate_Func<R, P1, P2, P3>(pfn); }

		template <class T>
		static Delegate<R, P1, P2, P3>* New(T* pObj, R (T::*pfn)(P1, P2, P3))
		{ return new Delegate_MemFunc<T, R, P1, P2, P3>(pObj, pfn); }
	};

	// 関数用
	template <class R, class P1, class P2, class P3>
	class Delegate_Func<R, P1, P2, P3> : public Delegate<R, P1, P2, P3>
	{
	public:
		Delegate_Func(R (*pfn)(P1, P2, P3)) : _pfn(pfn) {}
		R operator () (P1 p1, P2 p2, P3 p3)
		{ return (*_pfn)(p1, p2, p3); }
	private:
		R (*_pfn)(P1, P2, P3);
	};

	// メンバ関数用
	template <class T, class R, class P1, class P2, class P3>
	class Delegate_MemFunc<T, R, P1, P2, P3> : public Delegate<R, P1, P2, P3>
	{
	public:
		Delegate_MemFunc(T* pObj, R (T::*pfn)(P1, P2, P3)) : _pObj(pObj), _pfn(pfn) {}
		R operator () (P1 p1, P2 p2, P3 p3)
		{ return (_pObj->*_pfn)(p1, p2, p3); }
	private:
		T* _pObj;
		R (T::*_pfn)(P1, P2, P3);
	};

	//// 更に戻り値voidで特殊化
	// 関数用
	template <class P1, class P2, class P3>
	class Delegate_Func<void, P1, P2, P3> : public Delegate<void, P1, P2, P3>
	{
	public:
		Delegate_Func(void (*pfn)(P1, P2, P3)) : _pfn(pfn) {}
		void operator () (P1 p1, P2 p2, P3 p3)
		{ (*_pfn)(p1, p2, p3); /* no return */ }
	private:
		void (*_pfn)(P1, P2, P3);
	};

	// メンバ関数用
	template <class T, class P1, class P2, class P3>
	class Delegate_MemFunc<T, void, P1, P2, P3> : public Delegate<void, P1, P2, P3>
	{
	public:
		Delegate_MemFunc(T* pObj, void (T::*pfn)(P1, P2, P3)) : _pObj(pObj), _pfn(pfn) {}
		void operator () (P1 p1, P2 p2, P3 p3)
		{ (_pObj->*_pfn)(p1, p2, p3); /* no return */ }
	private:
		T* _pObj;
		void (T::*_pfn)(P1, P2, P3);
	};

}	// end namespace Marda
