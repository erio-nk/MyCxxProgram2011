// ���N���X
class Base
{
public:
	int _publicData;
	void PublicFunc() {}

protected:
	int _protectedData;
	void ProtectedFunc() {}

private:
	int _privateData;
	void PrivateFunc() {}
};

// private�p�������N���X
class Derived : private Base
{
	// �N���X��������N���X�̃����o�փA�N�Z�X
	void Do()
	{
		_publicData = 0;		// OK ; public�ɂ̓A�N�Z�X�B
		PublicFunc();			// OK

		_protectedData = 0;		// OK ; ����B
		ProtectedFunc();		// OK

		_privateData = 0;		// NG ; ���R�̔@��private�ɂ̓A�N�Z�X�s�B
		PrivateFunc();			// NG
	}
};

// main
int main()
{
	Derived obj;

	// ���N���X�̃|�C���^�ɕϊ�
	//Base* pBase = &obj;		// NG ; ���N���X�ɕϊ��o���Ȃ��I

	// �N���X�O������N���X�̃����o�փA�N�Z�X
	//obj._publicData = 0;		// NG ; public�����o�ɂ��A�N�Z�X�o���Ȃ��I
	//obj.PublicFunc();			// NG

//	obj._protectedData = 0;		// NG ; protected�͓��R�A�N�Z�X�s�B
//	obj.ProtectedFunc();		// NG

//	obj._privateData = 0;		// NG ; private�����R�A�N�Z�X�s�B
//	obj.PrivateFunc();			// NG

	return 0;
}
