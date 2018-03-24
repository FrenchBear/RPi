// allchar.c
// Sends 256 chars to output, no encoding
// 2016-05	PV

#include<stdio.h>

int main() {
	printf("No encoding:\n");
	for (int i=2 ; i<16 ; i++)
	{
		printf("%03x:", 16*i);
		for (int j=0 ; j<16 ; j++)
			printf(" %c", 16*i+j);
		printf("\n");
	}
	return 0;
}
