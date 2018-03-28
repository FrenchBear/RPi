// 01hello.c
// Basic ncurses application
// 2016-04-29	PV

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>

void exit_program(int sig) {
	endwin();
	printf("Terminated.\n");
	exit(1);
}

int main(int argc, char** argv) {
	// On Ctrl+C
	signal(SIGINT, exit_program);

	initscr();
	printw("Où ça? Là!");
	refresh();
	getch();
	endwin();

	return 0;
}
