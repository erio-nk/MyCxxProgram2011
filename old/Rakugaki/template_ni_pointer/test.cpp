/*!
	template_ni_pointer
 */

#include <iostream>

// ������ (char*)
char hoge[] = "hoge";
static char fuga[] = "fuga";
const char piyo[] = "piyo";

// �֐��e���v���[�g
template <char* String> //<const char* String> �ɂ��Ă�Print<hoge>�ȊO�̓_���B
void Print()
{
	std::cout << String << std::endl;
}

// main�֐�
int main()
{
	Print<hoge>();		// OK. ��const�̃O���[�o���ϐ��̃|�C���^�͓n����
	//Print<fuga>();	// NG. ��const�ł�static�ϐ��̃|�C���^�͓n���Ȃ�
	//Print<piyo>();	// NG. const�ɂ���ƃO���[�o���ϐ��ł��n���Ȃ�
	//Print<"moke">();	// NG. �����񃊃e�������n���Ȃ��B

	return 0;
}