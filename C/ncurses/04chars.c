// 04chars.c
// Draws a chars table
// 2016-04-29	PV	v1 DO NOT WORK, lib ncurses doesn't know about utf-8! (maybe the wchar_t version does?)

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

unsigned char *utf8encode(int c) {
	static unsigned char buf[7];

	if (c<=0x7f) {
		buf[0] = c;
		buf[1] = 0;
	}
	else if (c<=0x7ff) {
		buf[0] = 0xC0+c/0x40;
		buf[1] = 0x80+c%0x40;
		buf[2] = 0;
	}
	else {
		buf[0] = 0xE0+(c/0x40)/0x40;
		buf[1] = 0x80+(c/0x40)%0x40;
		buf[2] = 0x80+c%0x40;
		buf[3] = 0;
	}

	return buf;
}

int main(int argc, char** argv) {
	// On Ctrl+C
	signal(SIGINT, exit_program);

	initscr();				// Start curses
	raw();					// Line buffering disabled
	keypad(stdscr, TRUE);	// We get F1, F2...
	noecho();				// Don't echo while we do getch()

	int r, c;
	int ch = 0;
	for (r=0 ; r<LINES ; r++) {
		mvprintw(r, 0, "%04d", ch);
		for (c=6 ; c<COLS ; c+=2) {
			mvprintw(r, c, "%s", utf8encode(ch));
			//mvprintw(r, c, "ù");
			ch++;
		}
	}

	refresh();
	getch();
	endwin();

	return 0;
}

