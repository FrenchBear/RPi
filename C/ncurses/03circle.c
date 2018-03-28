// 03circle.c
// Draws a text circle
// 2016-04-29	PV

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>
#include <math.h>

void exit_program(int sig) {
	endwin();
	printf("Terminated.\n");
	exit(1);
}

int main(int argc, char** argv) {
	// On Ctrl+C
	signal(SIGINT, exit_program);

	initscr();				// Start curses
	raw();					// Line buffering disabled
	keypad(stdscr, TRUE);	// We get F1, F2...
	noecho();				// Don't echo while we do getch()

	double ry = LINES/2.0-2.0;
	while (ry>1.0) {
		double rx = 2.0*ry;
		double a;
		for (a=0.0 ; a<M_PI_2+0.015 ; a+=0.015) {
			int x = 0.5+rx*cos(a);
			int y = 0.5+ry*sin(a);

			mvprintw(LINES/2+y, COLS/2+x, "#");
			mvprintw(LINES/2-y, COLS/2+x, "#");
			mvprintw(LINES/2+y, COLS/2-x, "#");
			mvprintw(LINES/2-y, COLS/2-x, "#");
		}
		ry -= 2.0;
	}

	refresh();
	getch();
	endwin();

	return 0;
}

