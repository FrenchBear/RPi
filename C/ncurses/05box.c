// 05box.c
// Draws a box
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

int map2(int value, int min_in, int max_in, int min_out, int max_out) {
	return (value-min_in)/(double)(max_in-min_in)*(double)(max_out-min_out)+min_out;
}

void wp2(WINDOW *w, int y, int x, char *s) {
	mvwprintw(w, map2(y,0,255,1,LINES-4), map2(x,0,255,1,2*(LINES-3)), s);
}

int main(int argc, char** argv) {
	// On Ctrl+C
	signal(SIGINT, exit_program);

	initscr();				// Start curses
	raw();					// Line buffering disabled
	keypad(stdscr, TRUE);	// We get F1, F2...
	noecho();				// Don't echo while we do getch()
	refresh();

	WINDOW *nw = newwin(LINES-2, 2*(LINES-2), 1, COLS/2-LINES);
	box(nw, 0, 0);
	/*
	mvwprintw(nw, 1, 1, "A");
	mvwprintw(nw, LINES-4, 1, "B");
	mvwprintw(nw, 1, 2*(LINES-3), "C");
	mvwprintw(nw, LINES-4, 2*(LINES-3), "D");
	*/
	wp2(nw, 0, 0, "A");
	wp2(nw, 255, 0, "B");
	wp2(nw, 0, 255, "C");
	wp2(nw, 255, 255, "D");

	wp2(nw, 64, 64, "1");
	wp2(nw, 128, 128, "2");
	wp2(nw, 192, 192, "3");

	wrefresh(nw);

	getch();
	endwin();

	return 0;
}

