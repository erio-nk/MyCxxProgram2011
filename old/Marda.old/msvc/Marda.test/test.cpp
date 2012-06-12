#include "Marda.h"
#include "Marda/Util.h"

using namespace Marda;

//void test_memorypool();
void test_staticlist();
void test_stlhelpers();
void test_datastream();

int main()
{
	{
		Any any(1);

		//int n = any.ToInt();
		if (any.GetType() == typeid(int)) {
			std::cout << "int." << std::endl;
		} else {
			std::cout << "not int." << std::endl;
		}

		struct MajiHoge
		{
			~MajiHoge()
			{
				std::cout << "~MajiHoge" << std::endl;
			}
		};


		any = String(_T("hoge"));
		any = MajiHoge();

		struct MajiMoke
		{
			~MajiMoke()
			{
				std::cout << "~MajiMoke" << std::endl;
			}
		};
		Any any2(22);
		any2 = MajiMoke();

		any = any2;
	}

	std::cout << "--------------------------------------" << std::endl;

	{
		int hoge = 99;
		String fuga = lexical_cast<String>(hoge);
		float piyo = lexical_cast<float>(fuga);

		_tprintf(_T("fuga=%s\n"), fuga.c_str());
		_tprintf(_T("piyo=%f\n"), piyo);
	}
	
	std::cout << "--------------------------------------" << std::endl;

	{
		Serializer s;
		{
			int x = 0x01020304;
			short y = 0x0506;
			char z = 0x07;
			String t = _T("hogehoge");

			s << x << y << z << t;
		}

		Deserializer d(s.m_Data);
		{
			int x;
			short y;
			char z;
			String t;

			d >> x >> y >> z >> t;

			_tprintf(_T("x=%x\n"), x);
			_tprintf(_T("y=%x\n"), y);
			_tprintf(_T("z=%x\n"), z);
			_tprintf(_T("t=%s\n"), t.c_str());
		}
	}

	std::cout << "--------------------------------------" << std::endl;
	//test_memorypool();

	
	std::cout << "--------------------------------------" << std::endl;
	test_staticlist();
	
	std::cout << "--------------------------------------" << std::endl;
	test_stlhelpers();

	std::cout << "--------------------------------------" << std::endl;
	test_datastream();

	return 0;
}

