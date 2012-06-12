/**
	@file Utility/Fiber.h

	ファイバークラス

	@author	Dila
 */
/// @addtogroup Utility
/// @{
#pragma once

#include <vector>
#include <assert.h>

/**
	ファイバークラス
	
	ファイバー(＝マイクロスレッド、コルーチン)とは、
	ユーザ制御のスレッド(の様なもの)。
	ファイバーのプロシージャ(Fiber::Proc)で、Yield()を
	任意にコールする事で、処理を一時中断する。
	この時、ローカル変数等スタックの情報は全て一時領域に
	退避され、Run()を呼び出す事で再開される。
	再開後は、次のYield()がコールされるか、終了するまで実行される。

	サンプル
	@code
		// ファイバープロシージャクラス
		struct MyProc : Fiber::Proc
		{
			void operator ()()
			{
				// 何か処理
				Yield();	// ここで一時中断。呼び出し元に復帰する。
				// 何か処理
				Yield();	// 同じ。
				// 何か処理～終了
			}
		};

		Fiber fiber;
		fiber.Start<MyProc>();	// 最初のYield()まで実行される
		fiber.Run();	// 二回目のYield()まで実行される
		fiber.Run();	// 終わりまで実行される
	@endcode
 */
class Fiber
{
public:
	/**
		ファイバープロシージャクラス

		ファイバーで処理したい関数は、このクラスを継承した
		ファンクタとして実装する必要がある。
	 */
	class Proc
	{
		Fiber* _pOwner;		/// このプロシージャを実行するファイバーオブジェクト
		friend class Fiber;	// to call Fiber::_Yield()

	public:
		Proc() : _pOwner(NULL) {}
		virtual ~Proc() {}

	protected:
		/**
			プロシージャ
		 */
		virtual void operator ()() = 0;

		/**
			処理を一時中断する
		 */
		void Yield() { 
			assert(_pOwner);
			_pOwner->_Yield();
		}
	};	// end class Proc
	friend class Proc;


private:
	typedef unsigned char byte;	/// ucharの記述簡略化
	std::vector<byte> _Stack;	/// 自前スタック領域
	unsigned int _size;			/// スタックサイズ
	byte* _esp;		/// スタックポインタ(esp)の退避用
	Proc* _pProc;	/// プロシージャ
	bool _run;		/// ファイバー実行中かどうか

public:
	/**
		コンストラクタ
		@param	stackSize	自前スタックの最大サイズ
		@todo	デフォルトのスタックサイズを吟味する
	 */
	Fiber(unsigned int stackSize = 0x8000);

	/// デストラクタ
	~Fiber();

	/**
		ファイバーを開始する

		実行されたプロシージャは、Yield()が呼び出されるか、
		終了するまで実行される。
		@tparam	ProcClass	プロシージャクラス(Fiber::Procのサブクラス)
		@see	Fiber::Proc::Yield
	 */
	template <class ProcClass>
	void Start() { _Start(new ProcClass()); }

	/**
		ファイバーを再開する

		再開されたプロシージャは、Yield()が呼び出されるか、
		終了するまで実行される。
		@see	Fiber::Proc::Yield
	 */
	void Run();

	/**
		ファイバーが実行中かどうかを取得する
		@return	実行中の場合はtrue、実行していない場合はfalse
	 */
	bool IsRunning() { return _run; }

private:
	void _Start(Proc* pProc);
	void _Yield();
	static Fiber* _Call(Fiber* p);
};	// end class Fiber

/// @}
