// allcolor.c
// Shows all cocolor combinations background/foreground
// 2016-06	PV

#include <stdio.h>
#include <stdlib.h>

int main() {
	
	for (int h=0 ; h<2 ; h++)
		for (int c=0 ; c<8 ; c++) {
			for (int b=0 ; b<8 ; b++) {
				printf("[%d;3%d;4%dm", h, c, b);
				printf(" Hello [0;40m ");
			}
			printf("\n");
		}

	return 0;
}
