#include "Utility/Script.h"
#include <assert.h>

//// 組み込みコマンド＆ステートメント
const char CMD_NAME_ECHO[] = "echo";
const char CMD_NAME_SET[] = "set";
const char CMD_NAME_EXIT[] = "exit";
const char CMD_NAME_IF[] = "if";
const char CMD_NAME_END[] = "end";
const char CMD_NAME_GOTO[] = "goto";

// echoコマンドハンドラ
class OnEcho : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		if (args.size() < 1) { assert(0); return; }
		printf("echo '%s'\n", args[0]->GetString());
		pSuper->Next();	// 表示したら次へ
	}
};

// 変数セットコマンドハンドラ
class OnSetVar : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		if (args.size() < 2) { assert(0); return; }
		args[0]->SetVal(args[1]);	// 0番目はVarName型でなければダメ
		pSuper->Next();	// 設定したら次へ
	}
};

// スクリプト終了コマンドハンドラ
class OnExit : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		pSuper->Exit();	// 終わらせる
	}
};

// ifハンドラ
class OnIf : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		if (args.size() < 3) { assert(0); return; }

		bool express = false;
		const char* p = args[1]->GetString();
		if (*p == '=' && *p == '=' && *args[0] == *args[2]) express = true;
		else if (*p == '!' && *p == '=' && *args[0] != *args[2]) express = true;
		else if (*p == '<' && *args[0] < *args[2]) express = true;
		else if (*p == '>' && *args[0] > *args[2]) express = true;
		else if (*p == '<' && *p == '=' && *args[0] <= *args[2]) express = true;
		else if (*p == '>' && *p == '=' && *args[0] >= *args[2]) express = true;

		if (express) {
			pSuper->Next();	// 次へ
		} else {
			// 指定の番号のコマンドまでジャンプする
			pSuper->Jump(pSuper->GetCur()->iParam);
		}
	}
};

// endハンドラ
class OnEnd : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		// 何もしない
		pSuper->Next();
	}
};

// gotoコマンドハンドラ
class OnGoTo : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		// 指定のラベルまでジャンプする
		int pos = pSuper->GetLabelPos(args[0]->GetString());
		if (pos >= 0) {
			pSuper->Jump(pos);
			return;
		}
		assert(0);
		pSuper->Next();
	}
};



//// Script::Val

int Script::Val::GetInt()
{
	if (type == INT) return iVal;
	if (type == FLOAT) return (int)fVal;
	if (type == STR) {
		char* e;
		iVal = strtol(sVal.c_str(), &e, 10);
		if (*e == 0) return iVal;	// 変換成功
		return 0;	// 変換失敗
	}
	return 0;
}

float Script::Val::GetFloat()
{
	if (type == INT) return (float)iVal;
	if (type == FLOAT) return fVal;
	if (type == STR) {
		char* e;
		fVal = (float)strtod(sVal.c_str(), &e);
		if (*e == 0) return fVal;	// 変換成功
		return 0.0f;	// 変換失敗
	}
	return 0.0f;	// 変換失敗
}

const char* Script::Val::GetString()
{
	static char buf[64];
	if (type == INT) {
		sprintf(buf, "%d", iVal);
		return buf;
	}
	if (type == FLOAT) {
		sprintf(buf, "%f", fVal);
		return buf;
	}
	if (type == STR) return sVal.c_str();
	return "";
}

void Script::Val::SetInt(int val)
{
	type = INT;
	iVal = val;
}

void Script::Val::SetFloat(float val)
{
	type = FLOAT;
	fVal = val;
}

void Script::Val::SetString(const char* val)
{
	type = STR;
	sVal = val;
}

void Script::Val::SetVal(Script::Val* pVal)
{
	int _type = pVal->GetType();
	if (_type == INT) SetInt(pVal->GetInt());
	if (_type == FLOAT) SetFloat(pVal->GetFloat());
	if (_type == STR) SetString(pVal->GetString());
}

bool Script::Val::IsInt()
{
	if (GetType() == STR) {
		char* e;
		strtol(sVal.c_str(), &e, 10);
		if (*e == 0) return true;	// INTに変換できる
		return false;
	}
	return true;
}

bool Script::Val::IsFloat()
{
	if (GetType() == STR) {
		char* e;
		strtod(sVal.c_str(), &e);
		if (*e == 0) return true;	// FLOATに変換できる
		return false;
	}
	return true;
}

bool Script::Val::operator == (Val& val)
{
	if (GetType() == FLOAT || val.GetType() == FLOAT) return GetFloat() == val.GetFloat();
	if (GetType() == STR || val.GetType() == STR) return strcmp(GetString(), val.GetString()) == 0;
	return GetInt() == val.GetInt();
}

bool Script::Val::operator != (Val& val)
{
	return !(*this == val);
}

bool Script::Val::operator < (Val& val)
{
	if (GetType() == FLOAT || val.GetType() == FLOAT) return GetFloat() < val.GetFloat();
	if (GetType() == STR || val.GetType() == STR) {
		if (IsFloat() && val.IsFloat()) return GetFloat() < val.GetFloat();
		if (IsInt() && val.IsInt()) return GetInt() < val.GetInt();
		return false;
	}
	return GetInt() < val.GetInt();
}
bool Script::Val::operator > (Val& val)
{
	if (GetType() == FLOAT || val.GetType() == FLOAT) return GetFloat() > val.GetFloat();
	if (GetType() == STR || val.GetType() == STR) {
		if (IsFloat() && val.IsFloat()) return GetFloat() > val.GetFloat();
		if (IsInt() && val.IsInt()) return GetInt() > val.GetInt();
		return false;
	}
	return GetInt() < val.GetInt();
}

bool Script::Val::operator <= (Val& val)
{
	return *this == val || *this < val;
}

bool Script::Val::operator >= (Val& val)
{
	return *this == val || *this > val;
}

// ConstVal	... 定数型
class ConstVal : public Script::Val
{
public:
	ConstVal() : Script::Val() {}
	ConstVal(int val) : Script::Val(val) {}
	ConstVal(float val) : Script::Val(val) {}
	ConstVal(const char* val) : Script::Val(val) {}

	// セット禁止です
	void SetInt(int val) { assert(0); }
	void SetFloat(float val) { assert(0); }
	void SetString(const char* val) { assert(0); }
	void SetVal(Val* pVal) { assert(0); }
};

// VarName	... 変数名型/$xxx
class VarName : public Script::Val
{
	Script* pSuper;
public:
	VarName(Script* pSuper, const char* name) : Script::Val() {
		type = EXTEND;
		sVal = name;
		this->pSuper = pSuper;
	}
	int GetType() { return pSuper->GetVar(sVal.c_str())->GetType(); }
	int GetInt() { return pSuper->GetVar(sVal.c_str())->GetInt(); }
	float GetFloat() { return pSuper->GetVar(sVal.c_str())->GetFloat(); }
	const char* GetString() { return pSuper->GetVar(sVal.c_str())->GetString(); }
	void SetInt(int val) { pSuper->GetVar(sVal.c_str())->SetInt(val); }
	void SetFloat(float val) { pSuper->GetVar(sVal.c_str())->SetFloat(val); }
	void SetString(const char* val) { pSuper->GetVar(sVal.c_str())->SetString(val); }
	void SetVal(Val* pVal) { pSuper->GetVar(sVal.c_str())->SetVal(pVal); }
};


/**
 *	Script::Script()
 *	コンストラクタ。
 */
Script::Script()
{
}


/**
 *	Script::~Script()
 *	デストラクタ。
 */
Script::~Script()
{
	Finalize();
}


/**
 *	Script::Initialize()
 *	初期化。スクリプトファイルの読み込み、パース。
 *	@param	szScriptSoure	スクリプトソースのファイルパス。
 *	@return	true/false	成否
 */
bool Script::Initialize(const char* szScriptSource)
{
	// 組み込みコマンドのハンドラ追加
	AddCommandHandler<OnEcho>(CMD_NAME_ECHO);
	AddCommandHandler<OnSetVar>(CMD_NAME_SET);
	AddCommandHandler<OnExit>(CMD_NAME_EXIT);
	AddCommandHandler<OnIf>(CMD_NAME_IF);
	AddCommandHandler<OnEnd>(CMD_NAME_END);
	AddCommandHandler<OnGoTo>(CMD_NAME_GOTO);

	// ファイル解析
	FILE* fp = fopen(szScriptSource, "r");
	if (fp == NULL) {
		printf("errno=%d\n", errno);
		return false;
	}

	static char szBuf[256];
	char* t;
	int no = 0;

	std::list<Command*> blockList;	// ブロック構文用(ifとか)

	while (!feof(fp)) {
		szBuf[0] = 0;
		fgets(szBuf, sizeof(szBuf), fp);

		if (szBuf[0] == 0) continue;

		// バッファをトークンに分解
		char* strtokex(char* s);	// プロトタイプ

		Command* pCmd;

		char* p = szBuf;
		while ((t = strtokex(p))) {
			if (p != NULL) {
				// コメント行？
				if (*t == '#') break;

				// ラベル？
				if (t[strlen(t)-1] == ':') {
					char buf[32];
					strcpy(buf, t);
					buf[strlen(buf)-1] = 0;
					labels.insert(std::make_pair(std::string(buf), no));
					break;
				}

				// コマンドとして登録されてる？
				std::map<std::string, CommandHandler*>::iterator i = handlers.find(std::string(t));
				if (i == handlers.end())
					break;

				// コマンド名
				cmd.push_back(Command());	// コマンドリストに追加
				pCmd = &cmd.back();
				pCmd->no = no++;
				pCmd->name = t;

				p = NULL;	// 2ループ目以降strtokexに渡すのはNULL

				// ブロック構文用のプリプロセス
				if (pCmd->name.compare(CMD_NAME_IF) == 0) {
					blockList.push_back(pCmd);
				}
				else if(pCmd->name.compare(CMD_NAME_END) == 0) {
					Command* p = blockList.back();
					p->iParam = pCmd->no;	// 対応するendの番号を記憶
					blockList.pop_back();
				}
			} else {
				// 引数
				Val* pVal;
				if (*t == '$') {
					// 変数
					pVal = new VarName(this, t);
				} else {
					// 定数
					pVal = new ConstVal(t);
				}
				pCmd->args.push_back(pVal);
			}
		}
	}

	assert(blockList.size() == 0);

	fclose(fp);

	cur = cmd.begin();

	return true;
}

// strtokex()
// @todo	strfuncに入れる
char* strtokex(char* s)
{
	static char* _s;

	if (s != NULL) _s = s;

	char* b = _s;	// トークンの開始位置 

	// 開始位置の判断
	while (*_s) {
		if (*_s == ',' ||	// デリミタ
			*_s == ' ' ||
			*_s == '\t'||
			*_s == '\n')
		{
			// スルー
		} else {
			break;
		}
		++_s;
	}
	b = _s;

	if (*_s == 0) {
		return NULL;	// これ以上トークンなし
	}

	// トークン終了位置の判断
	char q = 0;		// 括り文字
	if (*_s == '\"' ||
		*_s == '\'')
	{
		q = *_s;
		++_s;
		++b;
	}

	while (*_s) {
		if (q == 0) {
			if (*_s == ',' ||	// デリミタ
				*_s == ' ' ||
				*_s == '\t'||
				*_s == '\n')
			{
				*_s = 0;
				break;
			}
		} else {
			if (*_s == q) {
				*_s = 0;
				break;
			}
		}
		++_s;
	}

	++_s;	// 次の開始位置

	return b;
}


/**
 *	Script::Finalize()
 *	終了処理。
 */
void Script::Finalize()
{
	std::map<std::string, CommandHandler*>::iterator k = handlers.begin();
	while (k != handlers.end()) {
		delete k->second;
		handlers.erase(k);
		k = handlers.begin();
	}
}


/**
 *	Script::Update()
 *	スクリプトのUpdate。
 */
void Script::Update()
{
	if (cur == cmd.end()) return;	// スクリプトは終わってます

	// コマンドハンドラを呼び出す（そのうち高速化したい）
	std::map<std::string, CommandHandler*>::iterator i = handlers.find(cur->name);
	if (i != handlers.end()) {
		(*i->second)(cur->args);
		return;
	}

	// 不明なコマンドはとりあえず無視
	Next();
}


/**
 *	Script::Next()
 *	次のコマンドへ進めて、一回Updateを呼び出す。
 *	updateがfalseの場合はカーソルだけ進める。
 *	@param	update	Updateするかどうか
 *	@return	進めたかどうか
 */
bool Script::Next(bool update)
{
	if (cur == cmd.end()) return false;

	++cur;
	if (update) {
		Update();
	}

	return true;
}

/**
 *	Script::Jump()
 *	カーソル位置をジャンプさせ、一回Updateを呼び出す。
 *	@param	no	ジャンプ位置
 *	@return	ジャンプ出来たかどうか
 */
bool Script::Jump(int no)
{
	std::list<Command>::iterator i = cur;
	if (i->no < no) {
		while (true) {
			if (i == cmd.end()) return false;
			++i;
			if (i->no == no) {
				break;
			}
		}
	}
	else if (i->no > no) {
		while (true) {
			if (i == cmd.begin()) return false;
			--i;
			if (i->no == no) {
				break;
			}
		}
	}
	cur = i;
	Update();
	return true;
}

/**
 *	Script::Exit()
 *	スクリプトを終わらせる。
 */
void Script::Exit()
{
	cur = cmd.end();
}

/**
 *	Script::GetVar()
 *	変数の値を取得する。
 *	@param	name	変数名
 *	@return	変数
 */
Script::Val* Script::GetVar(std::string name)
{
	std::map<std::string, Val>::iterator i = vars.find(name);
	if (i != vars.end()) {
		return &i->second;
	}

	// 無かったので新しく作ります。
	vars.insert(std::make_pair(name, Val()));
	i = vars.find(name);
	if (i != vars.end()) {
		return &i->second;
	}
	assert(0);
	return NULL;	// ココにはこないはず・・
}

/**
 *	Script::GetLabelPos()
 *	ラベルの位置を取得する。
 *	@param	label	ラベル名
 *	@return	ラベル位置。-1は失敗。
 */
int Script::GetLabelPos(std::string label)
{
	std::map<std::string, int>::iterator i = labels.find(label);
	if (i != labels.end()) {
		return i->second;
	}
	return -1;
}

