#include <stdio.h>
#include <stdlib.h>

void bar(const char *p) {
}

void foo(char * const *p) {
}

//int main(int argc, char **argv) {
int main(argc, argv)
int argc; 
char **argv;
{
	char *z = NULL;
	bar(z);

	const char *s = "Hello";
	const char c = 'e';
	const char *pc = &c;
	char **ppc0 = &pc;
	const char **ppc1 = &pc;
	const char * const *ppc2 = &pc;
	char * const *ppc3 = &pc;


	foo(argv);
	return 0;
}

