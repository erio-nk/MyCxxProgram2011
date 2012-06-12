/**
 *	Script.h
 *
 *	汎用的なスクリプトクラス。
 *	Initialize()したら毎フレームUpdate()をコールする。
 *		Script script;
 *		script.Initialize("...");
 *		while (true) script.Update();
 *
 *	各コマンドは、コマンドハンドラ(Script::CommandHandler)に
 *	処理を記述し、Script::AddCommandHandler<>()で追加する。
 *		struct OnMyCommand : public Script::CommandHandler {
 *		public: 
 *			void operator () (Script::Args& args) { ... }
 *		};
 *		script.AddCommandHandler<OnMyCommand>("my_command");
 *	
 *	コマンドハンドラは、カーソルが合っている限りは、
 *	Updateで同じハンドラが呼び出され続ける。
 *	次のコマンドへ移るには、Script::Next()を呼び出す。
 *	Script::Next()を呼び出すと次のコマンドへ移り、
 *	即時にUpdate()され、次のコマンドのハンドラが呼び出される。
 *		struct OnMyCommand : public Script::CommandHandler {
 *		public:
 *			void operator () (Script::Args& args) {
 *				pSuper->Next();	// 次へ。if文等で次のコマンドに移る条件を指定してもよい。
 *			}
 *		};
 *
 *
 *	スクリプトの書き方は、以下の様に単純にコマンド名とパラメータを
 *	並べるだけで、区切りは半角スペース、水平タブ、カンマになる。
 *	コマンド名と引数リストの区切りもカンマでよいが、区別しやすく
 *	する為に半角スペースや水平タブを推奨する。
 *	引数には、数値、文字列(""か''で括ったもの)、変数が指定出来る。
 *		command_name arg1, arg2, arg3 ...
 *
 *	コメント行は、最初の空白でない文字が#の行になる。
 *		# コメントです。
 *		command_name ...
 *
 *
 *	※if, else, for, while, goto, ラベル, ブロックを実装したいです。
 *	※コマンドリストは同じファイルだったら共有にするとかして
 *	　メモリ節約を・・・
 *	※あとバイナリ版を・・・
 *
 *	@author Dila
 */
#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <map>

class Script
{
public:
	//// 型

	// 値
	class Val
	{
	protected:
		enum { INT, FLOAT, STR, EMPTY, EXTEND } type;
		union {
			int iVal;
			float fVal;
		};
		std::string sVal;
	public:
		Val() { type = EMPTY; }
		Val(int val) { SetInt(val); }
		Val(float val) { SetFloat(val); }
		Val(const char* val) { SetString(val); }
		virtual int GetType() { return type; }
		virtual int GetInt();
		virtual float GetFloat();
		virtual const char* GetString();
		virtual void SetInt(int val);
		virtual void SetFloat(float val);
		virtual void SetString(const char* val);
		virtual void SetVal(Val* pVal);
		bool IsInt();
		bool IsFloat();
		bool operator == (Val& val);
		bool operator != (Val& val);
		bool operator < (Val& val);
		bool operator > (Val& val);
		bool operator <= (Val& val);
		bool operator >= (Val& val);
	};

	// コマンド引数リスト
	typedef std::vector<Val*> Args;

	// コマンド
	struct Command
	{
		int no;	// コマンドリスト上の番号
		std::string name;	// コマンド名
		Script::Args args;	// コマンド引数

		int iParam;	// フリーパラメータ

		Command() {
			no = 0;
			iParam = 0;
		}

		~Command() {
			for (int i = 0; i < args.size(); ++i) {
				delete args[i];
			}
		}
	};

	// コマンドハンドラ（ファンクタ）
	class CommandHandler
	{
	protected:
		Script* pSuper;
		friend class Script;
	public:
		virtual void operator () (Args& args) = 0;
	};

private:
	//// メンバ変数
	std::list<Command> cmd;	// コマンドリスト
	std::list<Command>::iterator cur;	// 現在のコマンドを示すカーソル

	std::map<std::string, CommandHandler*> handlers;	// コマンドハンドラリスト

	std::map<std::string, Val> vars;	// スクリプト変数の中身
	std::map<std::string, int> labels;	// ラベルリスト

public:
	//// メンバ関数
	Script();
	virtual ~Script();

	virtual bool Initialize(const char* szScriptSource);
	virtual void Finalize();
	
	bool IsEnd() { return cur == cmd.end(); }

	// 更新
	void Update();

	// コマンド
	const std::list<Command>& GetCommandList() { return cmd; }	// コマンドリスト
	std::list<Command>::const_iterator GetCur() { return cur; }	// 現在のコマンド

	// 次に進める
	bool Next(bool update = true);
	bool Jump(int no);

	// 終わらせる
	void Exit();


	// コマンドハンドラ追加
	template <class CommandHandlerClass>
	void AddCommandHandler(std::string name) {
		CommandHandler* p = new CommandHandlerClass();
		p->pSuper = this;
		handlers.insert(std::make_pair(name, p));
	}

	// スクリプト変数
	Val* GetVar(std::string name);

	// ラベル位置取得
	int GetLabelPos(std::string label);
};
