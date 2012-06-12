#include <iostream>

#include "Marda.h"
#include "Marda/Util/DataStream.h"

using namespace Marda;

void test_datastream()
{
	DataStream ds;

	{
		uint8 a = 0x00;
		uint16 b = 0x0102;
		uint32 c = 0x03040506;

		String s;
		s = _T("hoge");

		ds << a << b << c << s;
	}

	std::cout << "ds.size = " << ds.GetSize() << std::endl;

	{
		uint8 a;
		uint16 b;
		uint32 c;
		String s;
		sint16 d = -1;

		ds >> a >> b >> c >> s >> d;

		//std::cout << "a=" << a << std::endl;
		//std::cout << "b=" << b << std::endl;
		//std::cout << "c=" << c << std::endl;
		std::wcout << "s=" << s << std::endl;
		_tprintf(_T("a=%x\n"), a);
		_tprintf(_T("b=%x\n"), b);
		_tprintf(_T("c=%x\n"), c);
		_tprintf(_T("s=%s\n"), s.c_str());
		_tprintf(_T("d=%x\n"), d);

		//StringA as;
		//StringW ws(L"moke");
		//as = lexical_cast<StringA>(ws);
	}
}
