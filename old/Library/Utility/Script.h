/**
 *	Script.h
 *
 *	�ėp�I�ȃX�N���v�g�N���X�B
 *	Initialize()�����疈�t���[��Update()���R�[������B
 *		Script script;
 *		script.Initialize("...");
 *		while (true) script.Update();
 *
 *	�e�R�}���h�́A�R�}���h�n���h��(Script::CommandHandler)��
 *	�������L�q���AScript::AddCommandHandler<>()�Œǉ�����B
 *		struct OnMyCommand : public Script::CommandHandler {
 *		public: 
 *			void operator () (Script::Args& args) { ... }
 *		};
 *		script.AddCommandHandler<OnMyCommand>("my_command");
 *	
 *	�R�}���h�n���h���́A�J�[�\���������Ă������́A
 *	Update�œ����n���h�����Ăяo���ꑱ����B
 *	���̃R�}���h�ֈڂ�ɂ́AScript::Next()���Ăяo���B
 *	Script::Next()���Ăяo���Ǝ��̃R�}���h�ֈڂ�A
 *	������Update()����A���̃R�}���h�̃n���h�����Ăяo�����B
 *		struct OnMyCommand : public Script::CommandHandler {
 *		public:
 *			void operator () (Script::Args& args) {
 *				pSuper->Next();	// ���ցBif�����Ŏ��̃R�}���h�Ɉڂ�������w�肵�Ă��悢�B
 *			}
 *		};
 *
 *
 *	�X�N���v�g�̏������́A�ȉ��̗l�ɒP���ɃR�}���h���ƃp�����[�^��
 *	���ׂ邾���ŁA��؂�͔��p�X�y�[�X�A�����^�u�A�J���}�ɂȂ�B
 *	�R�}���h���ƈ������X�g�̋�؂���J���}�ł悢���A��ʂ��₷��
 *	����ׂɔ��p�X�y�[�X�␅���^�u�𐄏�����B
 *	�����ɂ́A���l�A������(""��''�Ŋ���������)�A�ϐ����w��o����B
 *		command_name arg1, arg2, arg3 ...
 *
 *	�R�����g�s�́A�ŏ��̋󔒂łȂ�������#�̍s�ɂȂ�B
 *		# �R�����g�ł��B
 *		command_name ...
 *
 *
 *	��if, else, for, while, goto, ���x��, �u���b�N�������������ł��B
 *	���R�}���h���X�g�͓����t�@�C���������狤�L�ɂ���Ƃ�����
 *	�@�������ߖ���E�E�E
 *	�����ƃo�C�i���ł��E�E�E
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
	//// �^

	// �l
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

	// �R�}���h�������X�g
	typedef std::vector<Val*> Args;

	// �R�}���h
	struct Command
	{
		int no;	// �R�}���h���X�g��̔ԍ�
		std::string name;	// �R�}���h��
		Script::Args args;	// �R�}���h����

		int iParam;	// �t���[�p�����[�^

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

	// �R�}���h�n���h���i�t�@���N�^�j
	class CommandHandler
	{
	protected:
		Script* pSuper;
		friend class Script;
	public:
		virtual void operator () (Args& args) = 0;
	};

private:
	//// �����o�ϐ�
	std::list<Command> cmd;	// �R�}���h���X�g
	std::list<Command>::iterator cur;	// ���݂̃R�}���h�������J�[�\��

	std::map<std::string, CommandHandler*> handlers;	// �R�}���h�n���h�����X�g

	std::map<std::string, Val> vars;	// �X�N���v�g�ϐ��̒��g
	std::map<std::string, int> labels;	// ���x�����X�g

public:
	//// �����o�֐�
	Script();
	virtual ~Script();

	virtual bool Initialize(const char* szScriptSource);
	virtual void Finalize();
	
	bool IsEnd() { return cur == cmd.end(); }

	// �X�V
	void Update();

	// �R�}���h
	const std::list<Command>& GetCommandList() { return cmd; }	// �R�}���h���X�g
	std::list<Command>::const_iterator GetCur() { return cur; }	// ���݂̃R�}���h

	// ���ɐi�߂�
	bool Next(bool update = true);
	bool Jump(int no);

	// �I��点��
	void Exit();


	// �R�}���h�n���h���ǉ�
	template <class CommandHandlerClass>
	void AddCommandHandler(std::string name) {
		CommandHandler* p = new CommandHandlerClass();
		p->pSuper = this;
		handlers.insert(std::make_pair(name, p));
	}

	// �X�N���v�g�ϐ�
	Val* GetVar(std::string name);

	// ���x���ʒu�擾
	int GetLabelPos(std::string label);
};
