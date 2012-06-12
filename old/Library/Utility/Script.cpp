#include "Utility/Script.h"
#include <assert.h>

//// �g�ݍ��݃R�}���h���X�e�[�g�����g
const char CMD_NAME_ECHO[] = "echo";
const char CMD_NAME_SET[] = "set";
const char CMD_NAME_EXIT[] = "exit";
const char CMD_NAME_IF[] = "if";
const char CMD_NAME_END[] = "end";
const char CMD_NAME_GOTO[] = "goto";

// echo�R�}���h�n���h��
class OnEcho : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		if (args.size() < 1) { assert(0); return; }
		printf("echo '%s'\n", args[0]->GetString());
		pSuper->Next();	// �\�������玟��
	}
};

// �ϐ��Z�b�g�R�}���h�n���h��
class OnSetVar : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		if (args.size() < 2) { assert(0); return; }
		args[0]->SetVal(args[1]);	// 0�Ԗڂ�VarName�^�łȂ���΃_��
		pSuper->Next();	// �ݒ肵���玟��
	}
};

// �X�N���v�g�I���R�}���h�n���h��
class OnExit : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		pSuper->Exit();	// �I��点��
	}
};

// if�n���h��
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
			pSuper->Next();	// ����
		} else {
			// �w��̔ԍ��̃R�}���h�܂ŃW�����v����
			pSuper->Jump(pSuper->GetCur()->iParam);
		}
	}
};

// end�n���h��
class OnEnd : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		// �������Ȃ�
		pSuper->Next();
	}
};

// goto�R�}���h�n���h��
class OnGoTo : public Script::CommandHandler
{
public:
	void operator () (Script::Args& args) {
		// �w��̃��x���܂ŃW�����v����
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
		if (*e == 0) return iVal;	// �ϊ�����
		return 0;	// �ϊ����s
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
		if (*e == 0) return fVal;	// �ϊ�����
		return 0.0f;	// �ϊ����s
	}
	return 0.0f;	// �ϊ����s
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
		if (*e == 0) return true;	// INT�ɕϊ��ł���
		return false;
	}
	return true;
}

bool Script::Val::IsFloat()
{
	if (GetType() == STR) {
		char* e;
		strtod(sVal.c_str(), &e);
		if (*e == 0) return true;	// FLOAT�ɕϊ��ł���
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

// ConstVal	... �萔�^
class ConstVal : public Script::Val
{
public:
	ConstVal() : Script::Val() {}
	ConstVal(int val) : Script::Val(val) {}
	ConstVal(float val) : Script::Val(val) {}
	ConstVal(const char* val) : Script::Val(val) {}

	// �Z�b�g�֎~�ł�
	void SetInt(int val) { assert(0); }
	void SetFloat(float val) { assert(0); }
	void SetString(const char* val) { assert(0); }
	void SetVal(Val* pVal) { assert(0); }
};

// VarName	... �ϐ����^/$xxx
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
 *	�R���X�g���N�^�B
 */
Script::Script()
{
}


/**
 *	Script::~Script()
 *	�f�X�g���N�^�B
 */
Script::~Script()
{
	Finalize();
}


/**
 *	Script::Initialize()
 *	�������B�X�N���v�g�t�@�C���̓ǂݍ��݁A�p�[�X�B
 *	@param	szScriptSoure	�X�N���v�g�\�[�X�̃t�@�C���p�X�B
 *	@return	true/false	����
 */
bool Script::Initialize(const char* szScriptSource)
{
	// �g�ݍ��݃R�}���h�̃n���h���ǉ�
	AddCommandHandler<OnEcho>(CMD_NAME_ECHO);
	AddCommandHandler<OnSetVar>(CMD_NAME_SET);
	AddCommandHandler<OnExit>(CMD_NAME_EXIT);
	AddCommandHandler<OnIf>(CMD_NAME_IF);
	AddCommandHandler<OnEnd>(CMD_NAME_END);
	AddCommandHandler<OnGoTo>(CMD_NAME_GOTO);

	// �t�@�C�����
	FILE* fp = fopen(szScriptSource, "r");
	if (fp == NULL) {
		printf("errno=%d\n", errno);
		return false;
	}

	static char szBuf[256];
	char* t;
	int no = 0;

	std::list<Command*> blockList;	// �u���b�N�\���p(if�Ƃ�)

	while (!feof(fp)) {
		szBuf[0] = 0;
		fgets(szBuf, sizeof(szBuf), fp);

		if (szBuf[0] == 0) continue;

		// �o�b�t�@���g�[�N���ɕ���
		char* strtokex(char* s);	// �v���g�^�C�v

		Command* pCmd;

		char* p = szBuf;
		while ((t = strtokex(p))) {
			if (p != NULL) {
				// �R�����g�s�H
				if (*t == '#') break;

				// ���x���H
				if (t[strlen(t)-1] == ':') {
					char buf[32];
					strcpy(buf, t);
					buf[strlen(buf)-1] = 0;
					labels.insert(std::make_pair(std::string(buf), no));
					break;
				}

				// �R�}���h�Ƃ��ēo�^����Ă�H
				std::map<std::string, CommandHandler*>::iterator i = handlers.find(std::string(t));
				if (i == handlers.end())
					break;

				// �R�}���h��
				cmd.push_back(Command());	// �R�}���h���X�g�ɒǉ�
				pCmd = &cmd.back();
				pCmd->no = no++;
				pCmd->name = t;

				p = NULL;	// 2���[�v�ڈȍ~strtokex�ɓn���̂�NULL

				// �u���b�N�\���p�̃v���v���Z�X
				if (pCmd->name.compare(CMD_NAME_IF) == 0) {
					blockList.push_back(pCmd);
				}
				else if(pCmd->name.compare(CMD_NAME_END) == 0) {
					Command* p = blockList.back();
					p->iParam = pCmd->no;	// �Ή�����end�̔ԍ����L��
					blockList.pop_back();
				}
			} else {
				// ����
				Val* pVal;
				if (*t == '$') {
					// �ϐ�
					pVal = new VarName(this, t);
				} else {
					// �萔
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
// @todo	strfunc�ɓ����
char* strtokex(char* s)
{
	static char* _s;

	if (s != NULL) _s = s;

	char* b = _s;	// �g�[�N���̊J�n�ʒu 

	// �J�n�ʒu�̔��f
	while (*_s) {
		if (*_s == ',' ||	// �f���~�^
			*_s == ' ' ||
			*_s == '\t'||
			*_s == '\n')
		{
			// �X���[
		} else {
			break;
		}
		++_s;
	}
	b = _s;

	if (*_s == 0) {
		return NULL;	// ����ȏ�g�[�N���Ȃ�
	}

	// �g�[�N���I���ʒu�̔��f
	char q = 0;		// ���蕶��
	if (*_s == '\"' ||
		*_s == '\'')
	{
		q = *_s;
		++_s;
		++b;
	}

	while (*_s) {
		if (q == 0) {
			if (*_s == ',' ||	// �f���~�^
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

	++_s;	// ���̊J�n�ʒu

	return b;
}


/**
 *	Script::Finalize()
 *	�I�������B
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
 *	�X�N���v�g��Update�B
 */
void Script::Update()
{
	if (cur == cmd.end()) return;	// �X�N���v�g�͏I����Ă܂�

	// �R�}���h�n���h�����Ăяo���i���̂����������������j
	std::map<std::string, CommandHandler*>::iterator i = handlers.find(cur->name);
	if (i != handlers.end()) {
		(*i->second)(cur->args);
		return;
	}

	// �s���ȃR�}���h�͂Ƃ肠��������
	Next();
}


/**
 *	Script::Next()
 *	���̃R�}���h�֐i�߂āA���Update���Ăяo���B
 *	update��false�̏ꍇ�̓J�[�\�������i�߂�B
 *	@param	update	Update���邩�ǂ���
 *	@return	�i�߂����ǂ���
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
 *	�J�[�\���ʒu���W�����v�����A���Update���Ăяo���B
 *	@param	no	�W�����v�ʒu
 *	@return	�W�����v�o�������ǂ���
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
 *	�X�N���v�g���I��点��B
 */
void Script::Exit()
{
	cur = cmd.end();
}

/**
 *	Script::GetVar()
 *	�ϐ��̒l���擾����B
 *	@param	name	�ϐ���
 *	@return	�ϐ�
 */
Script::Val* Script::GetVar(std::string name)
{
	std::map<std::string, Val>::iterator i = vars.find(name);
	if (i != vars.end()) {
		return &i->second;
	}

	// ���������̂ŐV�������܂��B
	vars.insert(std::make_pair(name, Val()));
	i = vars.find(name);
	if (i != vars.end()) {
		return &i->second;
	}
	assert(0);
	return NULL;	// �R�R�ɂ͂��Ȃ��͂��E�E
}

/**
 *	Script::GetLabelPos()
 *	���x���̈ʒu���擾����B
 *	@param	label	���x����
 *	@return	���x���ʒu�B-1�͎��s�B
 */
int Script::GetLabelPos(std::string label)
{
	std::map<std::string, int>::iterator i = labels.find(label);
	if (i != labels.end()) {
		return i->second;
	}
	return -1;
}

