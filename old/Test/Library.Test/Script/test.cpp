#include <stdio.h>
#include "Utility/Script.h"

int main()
{
	Script script;

	script.Initialize("hoge.s");

	while (!script.IsEnd()) {
		script.Update();
		printf(">updated.\n");
	}

	script.Finalize();

	return 0;
}
