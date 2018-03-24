// jump.c
// 2016-07-28	PV

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <time.h>

jmp_buf buf;
int mustrestart = 0;

struct timespec lastint;

void interruption(int sig) {
	struct timespec tint = lastint;
	clock_gettime(CLOCK_MONOTONIC, &lastint);
	long ms = (lastint.tv_sec-tint.tv_sec)*1000L+(lastint.tv_nsec-tint.tv_nsec)/1000000L;

	printf("Handling interruption, last was %ld ms ago\n", ms);
	if (ms<500) {
		printf("Forced exit requested\n");
		exit(0);
	}
	mustrestart = 1;
}

int main() {
	printf("Start\n");
	for (;;) {
		if (setjmp(buf)) {
			printf("Restart\n");
		}
		else
			break;
	}

	signal(SIGINT, interruption);

	for (int i=5 ; i>0 ; i--)
	{
		printf("%d\n", i);
		sleep(1);
		if (mustrestart)
		{
			mustrestart = 0;
			longjmp(buf, 1);
		}
	}
	printf("Terminated\n");

	return 0;
}
