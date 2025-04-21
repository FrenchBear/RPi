// Deep C secrets
// Page 76
//
// 2016-07-04	PV

#include <stdio.h>
#include <stdlib.h>

unsigned const long typedef int volatile *kiwi;

typedef struct foo { int foo;} foo;
struct foo var1;
foo var2;

int main() {
	printf("%d\n", sizeof(foo));
	var1 = var2;
	return 0;
}
