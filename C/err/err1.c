// err1.c
// Causes a segmentation fault (access to unauthorized memory address)
// 2016-07-28	PV

#include <stdlib.h>

int main() {
	char *p = (char *)0xCAFE;
	*p = 'a';

	return 0;
}
