
#include "Utility/Fiber.h"

#pragma warning(disable: 4731)	// ebpレジスタ変更のwarningを無効にする

Fiber::Fiber(unsigned int stackSize) :
	_size(stackSize),
	_run(false)
{
	_Stack.resize(stackSize);
}

Fiber::~Fiber()
{
	delete _pProc;
}

/**
	ファイバー開始処理
	@param	pProc	ファイバープロシージャ
 */
void Fiber::_Start(Fiber::Proc* pProc)
{
	if (_run) return;

	_pProc = pProc;
	_pProc->_pOwner = this;

	_esp = &_Stack[0] + _size;

	_run = true;

	void* pfnCall = (void*)&Fiber::_Call;

	__asm {
		push	offset Hoge
			// _Start()の最後のアドレスをpushする->Yield()のリターンアドレスが_Start()の最後になる

		push	ebx	// 以下四行はお決まりの処理？
		push	ebp	// TODO:もうちょいしっかり関数呼び出しの仕組みを理解する
		push	esi
		push	edi

		mov		eax, this
			// thisポインタを代入する([this]._espのようにはアクセス出来ない為)
		mov		ecx, pfnCall
			// staticな関数_Callのアドレス(メンバ関数はダメ)

		xchg	[eax]._esp, esp
			// スタックポインタを自前のスタック領域(_Stack)に切り返る

		push	eax
			// _Callの第一引数(this = Fiber*)をスタックに設定する
		call	ecx
			// _Callを呼び出す

		/*
			ここまで来たら、Procの処理は全て終わっている。
			_Callの戻り値(つまり_Callで渡した引数=this)がeaxに代入されている。
		 */
		
		xchg	[eax]._esp, esp
			// ので、スタックを元に戻す

		pop		edi	// これもお決まりっぽい？
		pop		esi
		pop		ebp
		pop		ebx

		mov		[eax]._run, 0
			// _runをfalseにする

		ret
	Hoge:	// _Start()の終端
	}
}

void Fiber::Run()
{
	if (!_run) return;

	__asm {
		push	offset Hoge
			// _Start()同様にRun()の最後のアドレスをpushする
			//	->	この後_Yield()が呼ばれれば、_Yield()のリターンがRun()の終端になり、
			//		呼ばれずに終了したら(_Start()のcall ecxまで来たら)、そのリターンがRun()の終端になる。

		push	ebx
		push	ebp
		push	esi
		push	edi
		
		mov		eax, [this]
		xchg	[eax]._esp, esp
			// 自前スタックに切り替える

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

		ret	
			// _Yield()で積んだリターンアドレスに戻る(つまり_Yield()の最後に飛び、_Yield()呼び出しの後ろの命令へ飛ぶ)

	Hoge:	// Run()の終端
	}
}

/**
	ファイバー一時中断関数
 */
void Fiber::_Yield()
{
	__asm {
		push	offset Hoge
			// 自前スタックに_Yield()の終端を積み、Run()のリターンアドレスを_Yield()の終端にする

		push	ebx
		push	ebp
		push	esi
		push	edi

		mov		eax, [this]
		xchg	[eax]._esp, esp
					// _Start()/Run()で切り替えたスタックポインタを元に戻す

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

		ret	// _Start()/Run()でスタックに積んだリターンアドレスに戻る

	Hoge:	// _Yield()の終端
	}
}

/**
	ファイバープロシージャ呼び出し関数

	アセンブラ上でメンバ関数呼び出しが出来ない為、
	thisを引数にしてプロシージャを呼び出すだけの関数。
	@see Fiber::_Start()
 */
Fiber* Fiber::_Call(Fiber* p)
{
	p->_pProc->operator () ();
	return p;	// pを戻す -> _Start()のcallの戻り値としてpがeaxに入る
}
