// struct1.c
// Tests on coge deneration
//
// 2016-07-04	PV	First version

#include <stdio.h>
#include <stdlib.h>

int scr = 0xCAFE;
void *fbp = &scr;

void put_pixel_RGB565(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned short c = ((r>>3)<<11) + ((g>>2)<<5) + (b>>3);
    *((unsigned short*)(fbp)) = c;
}

void get_pixel_RGB565(unsigned char *r, unsigned char *g, unsigned char *b)
{
	unsigned short c = *((unsigned short*)(fbp));
	*r = (c>>11)<<3;
	*g = ((c>>5)&0x3f)<<2;
	*b = (c&0x1f)<<3;
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
