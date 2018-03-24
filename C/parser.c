// C declarations parser
// 2016-97-04	PV

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char *TabDeclarations[] = {
	"int i",
	"extern long j",
	"const volatile unsigned *pk",
	"const char *p",
	"double td[10]",
	"char **argv",
	"int f1()",
	"int f2(int argc, char **argv)",
	"struct { int f1 char *p1; } st1",
	"struct s1 { int f2 char *p2; } st2",
	"struct s1 st3",
	"typedef struct { int f1 char *p1; } s1",
	"typedef struct s2 { int f1 char *p1; } s2",
	"s1 st4",
	"union { char c short s; int i; long l; } uni1",
	"union u1 { char c short s; int i; long l; } uni2",
	"union u1 uni3",
	"typedef union { char c short s; int i; long l; } u1",
	"typedef union u2 { char c short s; int i; long l; } u2",
	"u1 uni4",
	"typedef int (pt1)(void)",
	"typedef int (*ap)[100]",
	"void (*sig(int sig, void (*func)(int)))(int)",
	"typedef void (*sig_t(int sig, void (*func)(int)))(int)",
};

#define MAXTOKENLENGTH 64
#define MAXTOKENS      50

enum tokentype {
	CHAR,
	NUMBER,
	IDENTIFIER,
	KEYWORD,
	TYPE,
	QUALIFIER,
	SPECIAL,
	END,
};

struct token {
	char	text[MAXTOKENLENGTH+1];
	enum tokentype	type;
};

static struct token	TokenStack[MAXTOKENS];

static int NbTokens;
static struct token *CurTokenPtr;
static const char *Line;
static int PosLine;

int getPushToken() {
	char c = Line[PosLine];
	for (;;) {
		if (c==0)
			return END;		// Do not push it
		if (!isspace(c))
			break;
		c = Line[++PosLine];
	}

	// Anything else will be pushed, unless space is exhausted
	if (NbTokens==MAXTOKENS) {
		printf("*** Too many tokens to push\n");
		return -1;
	}

	CurTokenPtr = TokenStack+NbTokens++;
	int posStart = PosLine;

	if (isdigit(c)) {
		int i = 0;
		do
		{
			if (i==MAXTOKENLENGTH) {
				printf("*** Token too long: %s\n", Line+posStart);
				return -1;
			}
			CurTokenPtr->text[i++] = c;
			c = Line[++PosLine];
		} while (isdigit(c));
		CurTokenPtr->text[i] = 0;
		CurTokenPtr->type = NUMBER;
		return CurTokenPtr->type;
	}

	if (isalpha(c) || c=='_') {
		int i = 0;
		do
		{
			if (i==MAXTOKENLENGTH) {
				printf("*** Token too long: %s\n", Line+posStart);
				return -1;
			}
			CurTokenPtr->text[i++] = c;
			c = Line[++PosLine];
		} while (isalnum(c) || c=='_');
		CurTokenPtr->text[i] = 0;

		if (strcmp(CurTokenPtr->text, "char")==0 ||
		    strcmp(CurTokenPtr->text, "short")==0 ||
		    strcmp(CurTokenPtr->text, "int")==0 ||
		    strcmp(CurTokenPtr->text, "long")==0 ||
		    strcmp(CurTokenPtr->text, "float")==0 ||
		    strcmp(CurTokenPtr->text, "double")==0 ||
		    strcmp(CurTokenPtr->text, "signed")==0 ||
		    strcmp(CurTokenPtr->text, "unsigned")==0)
			CurTokenPtr->type = TYPE;
		else if ( strcmp(CurTokenPtr->text, "const")==0 ||
		    strcmp(CurTokenPtr->text, "volatile")==0)
			CurTokenPtr->type = QUALIFIER;
		else if ( strcmp(CurTokenPtr->text, "enum")==0 ||
		    strcmp(CurTokenPtr->text, "union")==0 ||
		    strcmp(CurTokenPtr->text, "struct")==0)
			CurTokenPtr->type = KEYWORD;
		else if ( strcmp(CurTokenPtr->text, "extern")==0 ||
		    strcmp(CurTokenPtr->text, "static")==0 ||
		    strcmp(CurTokenPtr->text, "typedef")==0)
			CurTokenPtr->type = SPECIAL;
		else
			CurTokenPtr->type = IDENTIFIER;
		return CurTokenPtr->type;
	}

	if (c=='(' || c==')' ||
	    c=='{' || c=='}' ||
	    c=='[' || c==']' ||
	    c=='*' || c==',' || c==';')
	{
		PosLine++;
		CurTokenPtr->text[0] = c;
		CurTokenPtr->text[1] = 0;
		CurTokenPtr->type = CHAR;
		return CurTokenPtr->type;
	}

	printf("Unknown char: %c\n", c);
	return -1;
}

static void explain(const char *declaration) {
	NbTokens = 0;
	Line = declaration;
	PosLine = 0;
	for(;;) {
		int t = getPushToken();
		if (t==-1) {
			printf("*** Error getting tokens\n");
			return;
		}
		if (t==END) {
			break;
		}
	}

	printf("\n%s\n", declaration);
	for (int i=0 ; i<NbTokens ; i++) {
		switch (TokenStack[i].type) {
			case CHAR:
				printf("%s ", TokenStack[i].text);
				break;

			case NUMBER:
				printf("N<%s> ", TokenStack[i].text);
				break;

			case IDENTIFIER:
				printf("I<%s> ", TokenStack[i].text);
				break;

			case SPECIAL:
				printf("S<%s> ", TokenStack[i].text);
				break;

			case QUALIFIER:
				printf("Q<%s> ", TokenStack[i].text);
				break;

			case TYPE:
				printf("T<%s> ", TokenStack[i].text);
				break;

			case KEYWORD:
				printf("K<%s> ", TokenStack[i].text);
				break;

			default:
				printf("?????");
		}
	}
	printf("\n");
}


int main() {
	int nDeclarations = sizeof(TabDeclarations)/sizeof(TabDeclarations[0]);
	for (int i=0 ; i<nDeclarations ; i++)
		explain(TabDeclarations[i]);
	return 0;
}
