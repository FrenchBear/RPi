// strict1.c
// Tests on coge deneration
//
// 2016-07-04	PV	First version

#include <stdio.h>
#include <stdlib.h>

int scr = 0xCAFE;
void *fbp = &scr;

#pragma pack(1)
struct bitslices {
	unsigned char b:5;
	unsigned char g:6;
	unsigned char r:5;
};

union decoder {
	unsigned short packcolor;
	struct bitslices channels;
};

void put_pixel_RGB565(unsigned char r, unsigned char g, unsigned char b)
{
	union decoder d;
	d.channels.r = r>>3;
	d.channels.g = g>>2;
	d.channels.b = b>>3;
    *((unsigned short*)(fbp)) = d.packcolor;
}

void get_pixel_RGB565(unsigned char *r, unsigned char *g, unsigned char *b)
{
	union decoder d;
	d.packcolor = *((unsigned short*)(fbp));
	*r = d.channels.r<<3;
	*g = d.channels.g<<2;
	*b = d.channels.b<<3;
}

int main()
{
	printf("%02x\n", *((unsigned char *)fbp));
	printf("%02x\n\n", *(((unsigned char *)fbp)+1));

	unsigned char r,g,b;
	get_pixel_RGB565(&r, &g, &b);
	printf("%02x %02x %02x\n", r>>3, g>>2, b>>3);

	put_pixel_RGB565(0xA0,0xB0,0xC0);
	get_pixel_RGB565(&r, &g, &b);
	printf("%04x  %02x %02x %02x\n", *((unsigned short *)fbp), r, g, b);

	put_pixel_RGB565(0x19<<3,0x17<<2,0x1E<<3);
	get_pixel_RGB565(&r, &g, &b);
	printf("%04x  %02x %02x %02x\n", *((unsigned short *)fbp), r, g, b);

    return 0;
}
