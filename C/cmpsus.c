// Comparisons between signed and unsigned variables
// 2016-07-07	PV

#include <stdio.h>
#include <stdlib.h>

int main() {
	int i = -1;
	unsigned int max = 5;

	if (i<max-2)
		printf("Normal (int)\n");
	else
		printf("Y'a un problème (int)\n");

	short is = -1;
	unsigned short maxs = 5;

	if (is<maxs-2)
		printf("Normal (short)\n");
	else
		printf("Y'a un problème (short)\n");

	return 0;
}
