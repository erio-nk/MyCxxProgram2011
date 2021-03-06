#include <stdio.h>

#include "Utility/Fiber.h"

struct MyProc : public Fiber::Proc
{
	void operator ()()
	{
		printf("hoge\n");
		Yield();

		printf("hogehoge\n");
		Yield();

		printf("hogehogehoge\n");
	}
};

void main()
{
	Fiber fiber;
	fiber.Start<MyProc>();

	fiber.Run();

	printf("とちゅうです。\n");

	fiber.Run();
	
	//fiber.Run();
}

