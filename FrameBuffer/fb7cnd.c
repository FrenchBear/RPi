// fb7cnd.c
// Drawings with frame buffer, Courbe continue non dérivable
//
// 2016-06-30	PV	First version

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <time.h>
#include <math.h>
#include <signal.h>

// default framebuffer palette
typedef enum {
    BLACK        =  0, /*   0,   0,   0 */
    BLUE         =  1, /*   0,   0, 172 */
    GREEN        =  2, /*   0, 172,   0 */
    CYAN         =  3, /*   0, 172, 172 */
    RED          =  4, /* 172,   0,   0 */
    PURPLE       =  5, /* 172,   0, 172 */
    ORANGE       =  6, /* 172,  84,   0 */
    LTGREY       =  7, /* 172, 172, 172 */
    GREY         =  8, /*  84,  84,  84 */
    LIGHT_BLUE   =  9, /*  84,  84, 255 */
    LIGHT_GREEN  = 10, /*  84, 255,  84 */
    LIGHT_CYAN   = 11, /*  84, 255, 255 */
    LIGHT_RED    = 12, /* 255,  84,  84 */
    LIGHT_PURPLE = 13, /* 255,  84, 255 */
    YELLOW       = 14, /* 255, 255,  84 */
    WHITE        = 15  /* 255, 255, 255 */
} COLOR_INDEX_T;

static unsigned char def_r[] =
    { 0,   0,   0,   0, 172, 172, 172, 168,
     84,  84,  84,  84, 255, 255, 255, 255};
static unsigned char def_g[] =
    { 0,   0, 168, 168,   0,   0,  84, 168,
     84,  84, 255, 255,  84,  84, 255, 255};
static unsigned char def_b[] =
    { 0, 172,   0, 168,   0, 172,   0, 168,
     84, 255,  84, 255,  84, 255,  84, 255};


// global variables to store screen info
int fbfd = 0;
char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

int page_size = 0;
int cur_page = 0;

int stop_request = 0;


// helper function to plot a pixel in given color in 8-bit depth
void put_pixel_Palette8(unsigned short x, unsigned short y, unsigned char c)
{
    // calculate the pixel's byte offset inside the buffer
    unsigned int pix_offset = x + y * finfo.line_length;

    // offset by the current buffer start
    pix_offset += cur_page * page_size;

    // now this is about the same as 'fbp[pix_offset] = value'
    *((char*)(fbp + pix_offset)) = c;
}

void get_pixel_Palette8(unsigned short x, unsigned short y, unsigned char *c)
{
    unsigned int pix_offset = x + y * finfo.line_length + cur_page * page_size;
    *c = *((char*)(fbp + pix_offset));
}

void put_pixel_RGB565(unsigned short x, unsigned short y, unsigned char r, unsigned char g, unsigned char b)
{
    // note: x * 2 as every pixel is 2 consecutive bytes
    unsigned int pix_offset = x * 2 + y * finfo.line_length;

    // offset by the current buffer start
    pix_offset += cur_page * page_size;

    // now this is about the same as 'fbp[pix_offset] = value'
    // but a bit more complicated for RGB565
    unsigned short c = ((r / 8) << 11) + ((g / 4) << 5) + (b / 8);
    // or: c = ((r / 8) * 2048) + ((g / 4) * 32) + (b / 8);
    // write 'two bytes at once'
    *((unsigned short*)(fbp + pix_offset)) = c;
}

void get_pixel_RGB565(unsigned short x, unsigned short y, unsigned char *r, unsigned char *g, unsigned char *b)
{
    unsigned int pix_offset = x * 2 + y * finfo.line_length + cur_page * page_size;
	unsigned short c = *((unsigned short*)(fbp + pix_offset));

	*r = (c>>11)<<3;
	*g = ((c>>5)&0x3f)<<2;
	*b = (c&0x1f)<<3;
}

unsigned int get_pixel_RGB565p(unsigned short x, unsigned short y)
{
    unsigned int pix_offset = x * 2 + y * finfo.line_length + cur_page * page_size;
	unsigned short c = *((unsigned short*)(fbp + pix_offset));
	return (unsigned int)(((c>>11)<<19)+(((c>>5)&0x3f)<<10)+((c&0x1f)<<3));
}

void put_pixel_RGB24(unsigned short x, unsigned short y, unsigned char r, unsigned char g, unsigned char b)
{
    // note: x * 3 as every pixel is 3 consecutive bytes
    unsigned int pix_offset = x * 3 + y * finfo.line_length;

    // offset by the current buffer start
    pix_offset += cur_page * page_size;

    // now this is about the same as 'fbp[pix_offset] = value'
    *((char*)(fbp + pix_offset)) = b;
    *((char*)(fbp + pix_offset + 1)) = g;
    *((char*)(fbp + pix_offset + 2)) = r;
}

void get_pixel_RGB24(unsigned short x, unsigned short y, unsigned char *r, unsigned char *g, unsigned char *b)
{
    unsigned int pix_offset = x * 3 + y * finfo.line_length + cur_page * page_size;
    *b = *((char*)(fbp + pix_offset));
    *g = *((char*)(fbp + pix_offset + 1));
    *r = *((char*)(fbp + pix_offset + 2));

	//unsigned int i = *((int *)(fbp+pix_offset));
    //*r = (i>>16)&0xff;
    //*g = (i>>8)&0xff;
    //*b = i&0xff;
}

unsigned int get_pixel_RGB24p(unsigned short x, unsigned short y)
{
    unsigned int pix_offset = x * 3 + y * finfo.line_length + cur_page * page_size;
	return *((unsigned int *)(fbp+pix_offset));
}

void put_pixel_RGB32(unsigned short x, unsigned short y, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int pix_offset = x * 4 + y * finfo.line_length;

    // offset by the current buffer start
    pix_offset += cur_page * page_size;

    *((char*)(fbp + pix_offset)) = b;
    *((char*)(fbp + pix_offset + 1)) = g;
    *((char*)(fbp + pix_offset + 2)) = r;
}

void get_pixel_RGB32(unsigned short x, unsigned short y, unsigned char *r, unsigned char *g, unsigned char *b)
{
    unsigned int pix_offset = x * 4 + y * finfo.line_length + cur_page * page_size;

    *b = *((char*)(fbp + pix_offset));
    *g = *((char*)(fbp + pix_offset + 1));
    *r = *((char*)(fbp + pix_offset + 2));
}

// General pixel display, independent for color depth, but c represents a color index in default palette
void put_pixel(unsigned short x, unsigned short y, unsigned char c)
{
	if (vinfo.bits_per_pixel == 16) {
		put_pixel_RGB565(x, y, def_r[c], def_g[c], def_b[c]);
	}
	else if (vinfo.bits_per_pixel == 24) {
		put_pixel_RGB24(x, y,  def_r[c], def_g[c], def_b[c]);
	}
	else if (vinfo.bits_per_pixel == 32) {
		put_pixel_RGB32(x, y,  def_r[c], def_g[c], def_b[c]);
	}
	else if (vinfo.bits_per_pixel == 8) {
		put_pixel_Palette8(x, y, c);
	}
}

void put_pixel_RGB(unsigned short x, unsigned short y, unsigned char r, unsigned char g, unsigned char b)
{
	if (vinfo.bits_per_pixel == 16) 
		put_pixel_RGB565(x, y, r, g, b);
	else if (vinfo.bits_per_pixel == 24)
		put_pixel_RGB24(x, y, r, g, b);
	else if (vinfo.bits_per_pixel == 32)
		put_pixel_RGB32(x, y, r, g, b);
	else
		put_pixel_Palette8(x, y, r); 	// By convention color is in r component
}

void get_pixel_RGB(unsigned short x, unsigned short y, unsigned char *r, unsigned char *g, unsigned char *b)
{
	if (vinfo.bits_per_pixel == 16) 
		get_pixel_RGB565(x, y, r, g, b);
	else if (vinfo.bits_per_pixel == 24)
		get_pixel_RGB24(x, y, r, g, b);
	else if (vinfo.bits_per_pixel == 32)
		get_pixel_RGB32(x, y, r, g, b);
	else
		get_pixel_Palette8(x, y, r);	// Returns color in r
}

unsigned int get_pixel_RGBp(unsigned short x, unsigned short y)
{
	if (vinfo.bits_per_pixel == 16) 
		return get_pixel_RGB565p(x, y);
	else if (vinfo.bits_per_pixel == 24)
		return get_pixel_RGB24p(x, y);
	return 0;
}

// helper function to draw a line in given color (index in default palette)
// Uses Bresenham's line algorithm, no anti aliasing
void draw_line_bresenham(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned char c) {
    short dx = x1 - x0;
    dx = (dx >= 0) ? dx : -dx; // abs()
    short dy = y1 - y0;
    dy = (dy >= 0) ? dy : -dy; // abs()
    short sx;
    short sy;
    if (x0 < x1)
        sx = 1;
    else
        sx = -1;
    if (y0 < y1)
        sy = 1;
    else
        sy = -1;
    short err = dx - dy;
    short e2;
    short done = 0;
    while (!done) {
        put_pixel(x0, y0, c);
        if ((x0 == x1) && (y0 == y1))
            done = 1;
        else {
            e2 = 2 * err;
            if (e2 > -dy) {
                err = err - dy;
                x0 = x0 + sx;
            }
            if (e2 < dx) {
                err = err + dx;
                y0 = y0 + sy;
            }
        }
    }
}

// helper function to draw a line in given color (index in default palette)
// Uses Xiaolin Wu's line algorithm, with anti aliasing
void draw_line_wu(unsigned short X0, unsigned short Y0, unsigned short X1, unsigned short Y1, unsigned char Color)
{
	unsigned short IntensityShift, ErrorAdj, ErrorAcc;
	unsigned short ErrorAccTemp, Weighing;
	short DeltaX, DeltaY, Temp, XDir;
	unsigned short IntensityBits = 8;

	// Convert color index into rgb values
	unsigned char cr = def_r[Color];
	unsigned char cg = def_g[Color];
	unsigned char cb = def_b[Color];

	// Make sure the line runs top to bottom
	if (Y0 > Y1) {
	  Temp = Y0; Y0 = Y1; Y1 = Temp;
	  Temp = X0; X0 = X1; X1 = Temp;
	}
	// Draw the initial pixel, which is always exactly intersected by the line and so needs no weighting
	put_pixel_RGB(X0, Y0, cr, cg, cb);

	if ((DeltaX = X1 - X0) >= 0) {
	  XDir = 1;
	} else {
	  XDir = -1;
	  DeltaX = -DeltaX; /* make DeltaX positive */
	}

	// Special-case horizontal, vertical, and diagonal lines, which require
	//  no weighting because they go right through the center of every pixel
	if ((DeltaY = Y1 - Y0) == 0) {
	  // Horizontal line
	  while (DeltaX-- != 0) {
		 X0 += XDir;
		 put_pixel_RGB(X0, Y0, cr, cg, cb);
	  }
	  return;
	}
	if (DeltaX == 0) {
	  // Vertical line
	  do {
		 Y0++;
		 put_pixel_RGB(X0, Y0, cr, cg, cb);
	  } while (--DeltaY != 0);
	  return;
	}
	if (DeltaX == DeltaY) {
	  // Diagonal line
	  do {
		 X0 += XDir;
		 Y0++;
		 put_pixel_RGB(X0, Y0, cr, cg, cb);
	  } while (--DeltaY != 0);
	  return;
	}

	// Line is not horizontal, diagonal, or vertical
	ErrorAcc = 0;  // initialize the line error accumulator
	// # of bits by which to shift ErrorAcc to get intensity level
	IntensityShift = 16 - IntensityBits;
	// Is this an X-major or Y-major line?
	if (DeltaY > DeltaX) {
		// Y-major line; calculate 16-bit fixed-point fractional part of a pixel that X advances each time Y advances 1 pixel, 
		// truncating the result so that we won't overrun the endpoint along the X axis
		ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
		// Draw all pixels other than the first and last
		while (--DeltaY) {
			ErrorAccTemp = ErrorAcc;   // remember currrent accumulated error
			ErrorAcc += ErrorAdj;      // calculate error for next pixel
			if (ErrorAcc <= ErrorAccTemp) {
				// The error accumulator turned over, so advance the X coord
				X0 += XDir;
			}
			Y0++; // Y-major, so always advance Y
			// The IntensityBits most significant bits of ErrorAcc give us the intensity weighting for this pixel, 
			// and the complement of the weighting for the paired pixel
			Weighing = ErrorAcc >> IntensityShift;

			unsigned char br, bg, bb;		// background color
			unsigned short nr, ng, nb;		// new color to draw

			//get_pixel_RGB(X0, Y0, &br, &bg, &bb);
			unsigned int cp = get_pixel_RGBp(X0, Y0);
			br = (cp>>16)&0xff;
			bg = (cp>>8)&0xff;
			bb = cp&0xff;

			nr = ((cr*(255-Weighing))>>8) + ((br*Weighing)>>8);
			ng = ((cg*(255-Weighing))>>8) + ((bg*Weighing)>>8);
			nb = ((cb*(255-Weighing))>>8) + ((bb*Weighing)>>8);
			put_pixel_RGB(X0, Y0, nr>255?255:nr, ng>255?255:ng, nb>255?255:nb);

			//get_pixel_RGB(X0+XDir, Y0, &br, &bg, &bb);
			cp = get_pixel_RGBp(X0+XDir, Y0);
			br = (cp>>16)&0xff;
			bg = (cp>>8)&0xff;
			bb = cp&0xff;
			nr = ((br*(255-Weighing))>>8) + ((cr*Weighing)>>8);
			ng = ((bg*(255-Weighing))>>8) + ((cg*Weighing)>>8);
			nb = ((bb*(255-Weighing))>>8) + ((cb*Weighing)>>8);
			put_pixel_RGB(X0+XDir, Y0, nr>255?255:nr, ng>255?255:ng, nb>255?255:nb);
		}
		// Draw the final pixel
		put_pixel_RGB(X1, Y1, cr, cg, cb);
		return;
	}

	// It's an X-major line; calculate 16-bit fixed-point fractional part of a pixel that Y advances each time X advances 1 pixel,
	// truncating the result to avoid overrunning the endpoint along the X axis
	ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	// Draw all pixels other than the first and last
	while (--DeltaX) {
		ErrorAccTemp = ErrorAcc;   // Remember currrent accumulated error
		ErrorAcc += ErrorAdj;      // Calculate error for next pixel
		if (ErrorAcc <= ErrorAccTemp) {
			// The error accumulator turned over, so advance the Y coord
			Y0++;
		}
		X0 += XDir; // X-major, so always advance X
		// The IntensityBits most significant bits of ErrorAcc give us the intensity weighting for this pixel,
		// and the complement of the weighting for the paired pixel
		Weighing = ErrorAcc >> IntensityShift;

		unsigned char br, bg, bb;		// background color
		unsigned short nr, ng, nb;		// new color to draw

#define crop8(n) (unsigned char)(n>255?255:n)

		//get_pixel_RGB(X0, Y0, &br, &bg, &bb);
		unsigned int cp = get_pixel_RGBp(X0, Y0);
		br = (cp>>16)&0xff;
		bg = (cp>>8)&0xff;
		bb = cp&0xff;
		nr = ((cr*(255-Weighing))>>8) + ((br*Weighing)>>8);
		ng = ((cg*(255-Weighing))>>8) + ((bg*Weighing)>>8);
		nb = ((cb*(255-Weighing))>>8) + ((bb*Weighing)>>8);
		put_pixel_RGB(X0, Y0, crop8(nr), crop8(ng), crop8(nb));

		//get_pixel_RGB(X0, Y0+1, &br, &bg, &bb);
		cp = get_pixel_RGBp(X0, Y0+1);
		br = (cp>>16)&0xff;
		bg = (cp>>8)&0xff;
		bb = cp&0xff;
		nr = ((br*(255-Weighing))>>8) + ((cr*Weighing)>>8);
		ng = ((bg*(255-Weighing))>>8) + ((cg*Weighing)>>8);
		nb = ((bb*(255-Weighing))>>8) + ((cb*Weighing)>>8);
		put_pixel_RGB(X0, Y0+1, crop8(nr), crop8(ng), crop8(nb));
	}
	// Draw the final pixel
	put_pixel_RGB(X1, Y1, cr, cg, cb);
}

// Generic version, used by draw_rect, fill_rect, fill_circle
inline void draw_line(int x0, int y0, int x1, int y1, int c) {
	draw_line_bresenham(x0, y0, x1, y1, c);
	//draw_line_wu(x0, y0, x1, y1, c);
}

// helper function to draw a rectangle outline in given color
void draw_rect(int x0, int y0, int w, int h, int c) {
    draw_line(x0, y0, x0 + w, y0, c); // top
    draw_line(x0, y0, x0, y0 + h, c); // left
    draw_line(x0, y0 + h, x0 + w, y0 + h, c); // bottom
    draw_line(x0 + w, y0, x0 + w, y0 + h, c); // right
}

// helper function to draw a rectangle outline in given color
void fill_rect(int x0, int y0, int w, int h, int c) {
    int y;
    for (y = 0; y < h; y++) {
        draw_line(x0, y0 + y, x0 + w, y0 + y, c);
    }
}

// helper function to draw a circle outline in given color
// (uses Bresenham's circle algorithm)
void draw_circle(int x0, int y0, int r, int c)
{
    int x = r;
    int y = 0;
    int radiusError = 1 - x;

    while(x >= y)
    {
        // top left
        put_pixel(-y + x0, -x + y0, c);
        // top right
        put_pixel(y + x0, -x + y0, c);
        // upper middle left
        put_pixel(-x + x0, -y + y0, c);
        // upper middle right
        put_pixel(x + x0, -y + y0, c);
        // lower middle left
        put_pixel(-x + x0, y + y0, c);
        // lower middle right
        put_pixel(x + x0, y + y0, c);
        // bottom left
        put_pixel(-y + x0, x + y0, c);
        // bottom right
        put_pixel(y + x0, x + y0, c);

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError+= 2 * (y - x + 1);
        }
    }
}

// helper function to draw a filled circle in given color
// (uses Bresenham's circle algorithm)
void fill_circle(int x0, int y0, int r, int c) {
    int x = r;
    int y = 0;
    int radiusError = 1 - x;

    while(x >= y)
    {
        // top
        draw_line(-y + x0, -x + y0, y + x0, -x + y0, c);
        // upper middle
        draw_line(-x + x0, -y + y0, x + x0, -y + y0, c);
        // lower middle
        draw_line(-x + x0, y + y0, x + x0, y + y0, c);
        // bottom 
        draw_line(-y + x0, x + y0, y + x0, x + y0, c);

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError+= 2 * (y - x + 1);
        }
    }
}

// helper to clear screen in black
void clear_screen(unsigned char c) {
    memset(fbp + cur_page * page_size, c, page_size);
}


// --------------------------------------------------------------
// Courbe continue non dérivable

// Offset to keep drawing centered on screen
int xoff, yoff;

void cnd(int color, int level, int xs, int ys, int xe, int ye) {
	if (level==1) {
		draw_line_bresenham(xoff+xs, yoff-ys, xoff+xe, yoff-ye, color);
	}
	else {
		int xm = (xs+xe)/2;
		int ym = ys + (ye-ys)/4;
		cnd(color, level-1, xs, ys, xm, ym);
		cnd(color, level-1, xm, ym, xe, ye);
	}
}


// Main drawing function
// Courbe continue non dérivable
void draw(int level) {
	xoff = (vinfo.xres-1024)/2;
	yoff = vinfo.yres-(vinfo.yres-1024)/2;

	clear_screen(0);
	for (int i=1 ; i<=10 ; i++)
		cnd(i, i, 0, 0, 1024, 1024);
}

// --------------------------------------------------------------

void exit_program(int sig) {
	stop_request = 1;
}


int main(int argc, char* argv[])
{
    long int screensize = 0;
    struct fb_var_screeninfo orig_vinfo;

	signal(SIGINT, exit_program);

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        printf("Error: cannot open framebuffer device.\n");
        return(1);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        printf("Error reading variable information.\n");
    }
	// printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

    // Store for reset (copy vinfo to vinfo_orig)
    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));


/*
    // Change variable info
    //vinfo.bits_per_pixel = 8;
    //vinfo.xres = 960;
    //vinfo.yres = 540;
    vinfo.xres_virtual = vinfo.xres;
    vinfo.yres_virtual = vinfo.yres * 2;
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
      printf("Error setting variable information.\n");
    }

*/
/*
    // hide cursor
    char *kbfds = "/dev/tty";
    int kbfd = open(kbfds, O_WRONLY);
    if (kbfd >= 0)
        ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
    else
        printf("Could not open %s.\n", kbfds);
*/

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        printf("Error reading fixed information.\n");
    }

    page_size = finfo.line_length * vinfo.yres;

    // map fb to user mem
    screensize = finfo.smem_len;
    fbp = (char*)mmap(0,
                        screensize,
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        fbfd,
                        0);

    if ((int)fbp == -1) {
        printf("Failed to mmap.\n");
    }
    else {
		int level = 6;
		if (argc==2)
			level = atoi(argv[1]);
		if (level<1 || level>10) level = 6;
		struct timespec tstart, tend;
		clock_gettime(CLOCK_MONOTONIC, &tstart);
		draw(level);
		clock_gettime(CLOCK_MONOTONIC, &tend);

		long ms = (tend.tv_sec-tstart.tv_sec)*1000L+(tend.tv_nsec-tstart.tv_nsec)/1000000L;
		printf("Level: %d, Duration: %ld ms\n", level, ms);
    }

    // cleanup
    // unmap fb file from memory
    munmap(fbp, screensize);


/*
    // reset cursor
    if (kbfd >= 0) {
        ioctl(kbfd, KDSETMODE, KD_TEXT);
        close(kbfd);
    }
*/

/*
    // reset the display mode
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
        printf("Error re-setting variable information.\n");
    }
*/
    // close fb file    
    close(fbfd);

    return 0;
}
