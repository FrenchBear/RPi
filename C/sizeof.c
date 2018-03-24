// Test how gcc interprets some c code
// 2016-03-07	PV

#include <stdio.h>
#include <stdlib.h>


int main() {
	short s = 2;
	short *p = &s;
	int r = sizeof((int)*p);

	printf("res: %d\n", r);

	return 0;
}
