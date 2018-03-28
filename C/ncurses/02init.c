// 02init.c
// Detailed initialization of ncurses
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

	initscr();				// Start curses
	raw();					// Line buffering disabled
	keypad(stdscr, TRUE);	// We get F1, F2...
	noecho();				// Don't echo while we do getch()

	printw("Type any character to see it in bold\n");
	int ch = getch();

	if (ch==KEY_F(1))
		printw("F1 key pressed");
	else {
		printw("The key pressed is ");
		attron(A_BOLD);
		printw("%c", ch);
		attroff(A_BOLD);
	}
	refresh();
	getch();
	endwin();

	return 0;
}

