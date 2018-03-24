// utf8code
// Examples of handling UTF-8 and unicode in C
// From: http://www.cprogramming.com/tutorial/unicode.html
// 2016-06-11	PV

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <locale.h>

#include "utf8.h"


char *locale;


int main(int argc, char **argv) {
	locale = setlocale(LC_ALL, "");
	
	printf("Locale: %s\n", locale);
	
	return 0;
}
