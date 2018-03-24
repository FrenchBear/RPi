#include <stdio.h>
#include <stdlib.h>

void put_pixel_RGB(unsigned char r, unsigned char g, unsigned char b) {
	printf("put_pixel: %d %d %d\n", r, g, b);
}

int main() {

	unsigned char br, bg, bb;		// background color
	unsigned char nr, ng, nb;		// new color to draw
	unsigned char cr=255, cg=255, cb=255;

	unsigned int cp = 0xFFFFFF;

	unsigned short Weighing = 64;

	br = (cp>>16)&0xff;
	bg = (cp>>8)&0xff;
	bb = cp&0xff;

	nr = ((cr*(255-Weighing))>>8) + ((br*Weighing)>>8);
	ng = ((cg*(255-Weighing))>>8) + ((bg*Weighing)>>8);
	nb = ((cb*(255-Weighing))>>8) + ((bb*Weighing)>>8);


	printf("new_color: %d %d %d\n", nr, ng, nb);

	unsigned char gb = ng+nb;
	printf("new_g+new_b: %d\n", gb);

	put_pixel_RGB((nr>255)?255:nr, ng>255?255:ng, nb>255?255:nb);

	return 0;
}
