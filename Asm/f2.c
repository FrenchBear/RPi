// f2.c
// Sum of digits of i, without division
// 2016-05-19	PV

// int = 32 bits
// long = 32 bits
// long long = 64 bits

#include <stdio.h>

void main() {
	int i = 1024;
	int base = 0x1999999A;	// 2^32/10 rounded up
	int total = 0;
	while (i>0) {
		int iDiv10 = ((long long)i*(long long)base)>>32;
		printf("%4d %4d %4d %4d\n", i, iDiv10, 10*iDiv10, i-10*iDiv10);
		total += i-10*iDiv10;
		i = iDiv10;
	}
	printf("sum digits = %d\n", total);
}

