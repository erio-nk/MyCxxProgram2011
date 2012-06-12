// concept.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
	
#include <iostream>
	
#include "boost/concept_check.hpp"


// ------------------------------------------------------------------
template <class T>
T Negate(T t)
{
	boost::function_requires<boost::SignedIntegerConcept<T>>();
	return -t;
}

void SignedIntegerConceptCheck()
{
	int si = 1;
	Negate(si);
	
//	unsigned int ui = 2;
//	Negate(ui);
}

// ------------------------------------------------------------------
	
struct Hoge
{
	void Echo(const char* message) const
	{
		std::cout << message << std::endl;
	};
};

struct Fuga
{
};
	
/*!
	@brief	Echo�����o�֐��������Ă��邩�ǂ����`�F�b�N����R���Z�v�g�B
*/
template <class T>
struct HasEchoMemberConcept
{
	T t;
	void constraints() 
	{
		&T::Echo;
	}
};


template <class T>
void Echo(const char* message)
{
	boost::function_requires<HasEchoMemberConcept<T>>();
//	boost::function_requires<boost::UnaryFunctionConcept<T::Echo, void, const char*>();
//	boost::function_requires<boost::BinaryFunctionConcept<T::Echo, void, T, const char*>();
		// �����o�֐��̓_�����ۂ��Ȃ��B
		
	T t;
	t.Echo(message);
};


template <class T>
class EchoClass
{
	BOOST_CLASS_REQUIRE(T, , HasEchoMemberConcept);
public:
	void Echo(const char* message)
	{
		T t;
		t.Echo(message);
	}
};

void HasMemberFunctionConceptCheck()
{
	Echo<Hoge>("hoge");
//	Echo<Fuga>("fuga");

	EchoClass<Hoge> hogeEcho;
	hogeEcho.Echo("hogege");
//	EchoClass<Fuga> fugaEcho;
//	fugaEcho.Echo("fugaga");
}

// ------------------------------------------------------------------


int _tmain(int argc, _TCHAR* argv[])
{
	SignedIntegerConceptCheck();
	
	HasMemberFunctionConceptCheck();
	
	return 0;
}

