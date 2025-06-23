// allcolor.c
// Shows all cocolor combinations background/foreground
// 2016-06		PV
// 2018-03_29	PV	Display Esc sequences

#include <stdio.h>
#include <stdlib.h>

int main() {
	
	printf("         ");
	for (int b=0 ; b<8 ; b++)
		printf("^[[4%dm  ", b);
	printf("\n");
	for (int h=0 ; h<2 ; h++)
		for (int c=0 ; c<8 ; c++) {
			printf("^[[%d;3%dm ", h, c);
			for (int b=0 ; b<8 ; b++) {
				printf("\x1b[%d;3%d;4%dm", h, c, b);
				printf(" Hello \x1b[0;40m ");
			}
			printf("\n");
		}

	return 0;
}
