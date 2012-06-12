#include "stdafx.h"

using namespace Marda;

bool Test_Eexception()
{
	TTRACE(_T("-----------------------------------------------------"));
	EnableStructuredExceptionTranslation();

	try
	{
		*(int*)0 = 0;
	}
	catch (StructuredException& se)
	{
		TTRACE(_T("StructuredException occured. code=%u, address=%p"),
			se._exceptionCode,
			se._exceptionRecord.ExceptionAddress
			);
		return true;
	}

	EnableStructuredExceptionTranslation(false);

	return false;
}

struct MyValue
{
	MyValue()
	{
		TTRACE(_T("[%p] Construct."), this);
	}

	~MyValue()
	{
		TTRACE(_T("[%p] Destruct."), this);
	}

	void Echo()
	{
		TTRACE(_T("[%p] Echo."), this);
	}
};

typedef smart_ptr<MyValue, ThreadSafe> MyValuePtr;

bool Test_SmartPtr()
{
	TTRACE(_T("-----------------------------------------------------"));
	MyValuePtr ptr1(new MyValue);
	MyValuePtr ptr2(ptr1);

	ptr2 = ptr2;
	ptr2 = ptr1;

	{
		MyValuePtr ptr3(new MyValue);
		ptr3->Echo();
	}

	ptr2.reset(new MyValue);
	ptr1.reset(new MyValue);

	return true;
}

bool Test_Flags()
{
	TTRACE(_T("-----------------------------------------------------"));
	
	Flags flags(0xfa021493);

	TTRACE(_T("flags = %08x"), flags._value);

	TTRACE(TFMT_BIN8, format_bin8(flags._value));
	TTRACE(TFMT_BIN16, format_bin16(flags._value));
	TTRACE(TFMT_BIN32, format_bin32(flags._value));

	flags = flags.not(0x000000ff);
	TTRACE(TFMT_BIN32, format_bin32(flags._value));

//	PAUSE;
	
	return true;
}

bool Test_Misc()
{
	if (!Test_Eexception()) return false;
	if (!Test_Flags()) return false;
	if (!Test_SmartPtr()) return false;

	return true;
}
