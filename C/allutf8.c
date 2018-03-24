// allutf8.c
// Sends 256 chars to output, UTF-8 encoding
// 2016-05		PV
// 2016-06-03	PV	Range()
// 2016-06-03	PV	Option -d, -f

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

int width = 32;

// Quick-and-dirty encoding int --> UTF-8
unsigned const char *utf8encode(int c) {
	static unsigned char buf[7];

	assert(c>=0 && c<0x7fffffff);

	if (c<=0x7f) {
		buf[0] = c;
		buf[1] = 0;
	}
	else if (c<=0x7ff) {
		buf[0] = 0xC0+c/0x40;
		buf[1] = 0x80+c%0x40;
		buf[2] = 0;
	}
	else if (c<=0xffff) {
		buf[0] = 0xE0+(c/0x40)/0x40;
		buf[1] = 0x80+(c/0x40)%0x40;
		buf[2] = 0x80+c%0x40;
		buf[3] = 0;
	}
	else if (c<=0x1fffff) {
		buf[0] = 0xF0+(c/0x40)/0x40/0x40;
		buf[1] = 0x80+(c/0x40)/0x40%0x40;
		buf[2] = 0x80+(c/0x40)%0x40;
		buf[3] = 0x80+c%0x40;
		buf[4] = 0;
	}
	else if (c<=0x3ffffff) {
		buf[0] = 0xF0+(c/0x40)/0x40/0x40/0x40;
		buf[1] = 0x80+(c/0x40)/0x40/0x40%0x40;
		buf[2] = 0x80+(c/0x40)/0x40%0x40;
		buf[3] = 0x80+(c/0x40)%0x40;
		buf[4] = 0x80+c%0x40;
		buf[5] = 0;
	}
	else if (c<=0x7fffffff) {
		buf[0] = 0xF0+(c/0x40)/0x40/0x40/0x40/0x40;
		buf[1] = 0x80+(c/0x40)/0x40/0x40/0x40/0x40;
		buf[2] = 0x80+(c/0x40)/0x40/0x40%0x40;
		buf[3] = 0x80+(c/0x40)/0x40%0x40;
		buf[4] = 0x80+(c/0x40)%0x40;
		buf[5] = 0x80+c%0x40;
		buf[6] = 0;
	}

	return buf;
}


void Range(int rStart, int rEnd) {
	for (int i=rStart ; i<=rEnd ; i+=width)
	{
		printf("%5d U+%04x:", i, i);
		for (int j=0 ; j<32 ; j++)
			printf(" %s", utf8encode(i+j));
		printf("\n");
	}
	printf("\n");
}

int isInteger(const char *s) {
	if ((s==NULL) || (s[0]==0) || ((!isdigit(s[0]) && (s[0]!='-') && (s[0]!='+')))) return 0;
	int base = 10;
	int off = 0;
	if (strncmp(s, "0x", 2)==0 || strncmp(s, "0X", 2)==0) {
		base = 16;
		off = 2;
	}
	char *p;
	strtol(s+off, &p, base);
	return *p==0;
}

int getInt(const char *s) {
	int base = 10;
	int off = 0;
	if (strncmp(s, "0x", 2)==0 || strncmp(s, "0X", 2)==0) {
		base = 16;
		off = 2;
	}
	return strtol(s+off, NULL, base);
}

int main(int argc, char **argv) {
	int c;
	int start=0, end=0;
	while ((c = getopt(argc, argv, "hs:e:w:"))!=-1)
		switch(c)
		{
			case 'h':
				fprintf(stderr,	"Usage: allutf8 [-h] [-s start] [-e end] [-w width]\n"
								"-h       Shows this message\n"
								"-s start Start of range\n"
								"-e end   End of range\n"
								"-w width Output width, default 32\n");
				return 1;

			case 's':
				if (!isInteger(optarg)) {
					fprintf(stderr,	"allutf8: start must be an integer");
					return 1;
				}
				start = getInt(optarg);
				if (start<0 || start>0xFFFF) {
					fprintf(stderr,	"allutf8: start must be in 0..65535 range\n");
					return 1;
				}
				break;

			case 'e':
				if (!isInteger(optarg)) {
					fprintf(stderr,	"allutf8: end must be an integer\n");
					return 1;
				}
				end = getInt(optarg);
				if (end<0 || end>0xFFFF) {
					fprintf(stderr,	"allutf8: start must be in 0..65535 range\n");
					return 1;
				}
				break;

			case 'w':
				if (!isInteger(optarg)) {
					fprintf(stderr,	"allutf8: width must be an integer\n");
					return 1;
				}
				width = getInt(optarg);
				if (end<1 || end>128) {
					fprintf(stderr,	"allutf8: width must be in 1..128 range\n");
					return 1;
				}
				break;

			case '?':
				if (optopt=='s' || optopt=='e' || optopt=='w')
					fprintf(stderr,	"allutf8: option -%c requires an argument\n", optopt);
				else
					fprintf(stderr,	"allutf8: unknown option -%c\n", optopt);
				return 1;
		}

	if (optind<argc) {
		for (int i=optind ; i<argc ; i++) 
			fprintf(stderr,	"allutf8: unknown argument %s\n", argv[i]);
		fprintf(stderr,	"Use allutf8 -h to gt usage\n");
		return 1;
	}
	
	
	printf("%sUTF-8 encoding:\n", utf8encode(0xFEFF));

	if (start==0 && end==0) {
		Range(0x20, 0x170);
		Range(0x2c0, 0x2d0);
		Range(0x380, 0x3c0);
		Range(0x400, 0x450);
		Range(0x2500, 0x25C0);
	}
	else {
		Range(start, end);
	}
	return 0;
}
