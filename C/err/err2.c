// err2.c
// Try to access misaligned atomic objects, here 32 bits integers
// Should cause segmentation fault, but actually ARM architecture doesn't complain
// 2016-07-28	PV

#include <stdlib.h>
#include <malloc.h>

int main() {
	char *p = (char *)malloc(100);
	*((unsigned int *)(p+0)) = 0xdeadbeefU;
	*((unsigned int *)(p+1)) = 0xdeadbeefU;
	*((unsigned int *)(p+2)) = 0xdeadbeefU;
	*((unsigned int *)(p+3)) = 0xdeadbeefU;

	return 0;
}
