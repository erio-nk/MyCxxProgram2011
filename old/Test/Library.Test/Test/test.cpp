#include <stdio.h>

#include "checkf.h"

int main()
{
	float x = _INF;

	if (_CHECKF(x)) {	
		printf("OK! x = %f\n", x);	
	} else {
		printf("NG! x = %f\n", x);
	}

	return 0;
}
