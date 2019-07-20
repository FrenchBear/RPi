// fb3.c
// Drawings with frame buffer
// From raspberrycompote.blogspot.com/2014/04/low-level-graphics-on-raspberry-pi.html, Code from fbtestXX.c and fbtest6.c
// Compare Bresenham line drawing (non-aliasing) and  Xiaolin Wu line (aliasing) algorithms
//
// 2016-06-05	PV	Adapted fbtestXX.c to support all depths and not only 8-bit palette

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

int frames = 0;
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

void put_pixel_RGB32(unsigned short x, unsigned short y, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int pix_offset = x * 4 + y * finfo.line_length;

    // offset by the current buffer start
    pix_offset += cur_page * page_size;

    *((char*)(fbp + pix_offset)) = b;
    *((char*)(fbp + pix_offset + 1)) = g;
    *((char*)(fbp + pix_offset + 2)) = r;
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

void put_pixel_intensity(unsigned short x, unsigned short y, unsigned char c, unsigned char i)
{
	if (vinfo.bits_per_pixel!=8) {
		i = (255-i);
		unsigned char r = (def_r[c]*i)>>8;
		unsigned char g = (def_g[c]*i)>>8;
		unsigned char b = (def_b[c]*i)>>8;
		if (vinfo.bits_per_pixel == 16) 
			put_pixel_RGB565(x, y, r, g, b);
		else if (vinfo.bits_per_pixel == 24)
			put_pixel_RGB24(x, y, r, g, b);
		else if (vinfo.bits_per_pixel == 32)
			put_pixel_RGB32(x, y, r, g, b);
	}
	else
		put_pixel_Palette8(x, y, c); // Intensity is not processed
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
	unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
	short DeltaX, DeltaY, Temp, XDir;

	short BaseIntensity = 0;
	short NumLevels = 256;
	unsigned short IntensityBits = 8;

	// Make sure the line runs top to bottom
	if (Y0 > Y1) {
	  Temp = Y0; Y0 = Y1; Y1 = Temp;
	  Temp = X0; X0 = X1; X1 = Temp;
	}
	// Draw the initial pixel, which is always exactly intersected by the line and so needs no weighting
	put_pixel_intensity(X0, Y0, Color, BaseIntensity);

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
		 put_pixel_intensity(X0, Y0, Color, BaseIntensity);
	  }
	  return;
	}
	if (DeltaX == 0) {
	  // Vertical line
	  do {
		 Y0++;
		 put_pixel_intensity(X0, Y0, Color, BaseIntensity);
	  } while (--DeltaY != 0);
	  return;
	}
	if (DeltaX == DeltaY) {
	  // Diagonal line
	  do {
		 X0 += XDir;
		 Y0++;
		 put_pixel_intensity(X0, Y0, Color, BaseIntensity);
	  } while (--DeltaY != 0);
	  return;
	}

	// Line is not horizontal, diagonal, or vertical
	ErrorAcc = 0;  // initialize the line error accumulator
	// # of bits by which to shift ErrorAcc to get intensity level
	IntensityShift = 16 - IntensityBits;
	// Mask used to flip all bits in an intensity weighting, producing the result (1 - intensity weighting)
	WeightingComplementMask = NumLevels - 1;
	// Is this an X-major or Y-major line?
	if (DeltaY > DeltaX) {
	  /* Y-major line; calculate 16-bit fixed-point fractional part of a
		 pixel that X advances each time Y advances 1 pixel, truncating the
		 result so that we won't overrun the endpoint along the X axis */
	  ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
	  /* Draw all pixels other than the first and last */
	  while (--DeltaY) {
		 ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
		 ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
		 if (ErrorAcc <= ErrorAccTemp) {
			/* The error accumulator turned over, so advance the X coord */
			X0 += XDir;
		 }
		 Y0++; /* Y-major, so always advance Y */
		 /* The IntensityBits most significant bits of ErrorAcc give us the
			intensity weighting for this pixel, and the complement of the
			weighting for the paired pixel */
		 Weighting = ErrorAcc >> IntensityShift;
		 put_pixel_intensity(X0, Y0, Color, BaseIntensity + Weighting);
		 put_pixel_intensity(X0 + XDir, Y0, Color, BaseIntensity + (Weighting ^ WeightingComplementMask));
	  }
	  /* Draw the final pixel, which is always exactly intersected by the line
		 and so needs no weighting */
	  put_pixel_intensity(X1, Y1, Color, BaseIntensity);
	  return;
	}
	/* It's an X-major line; calculate 16-bit fixed-point fractional part of a
	  pixel that Y advances each time X advances 1 pixel, truncating the
	  result to avoid overrunning the endpoint along the X axis */
	ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	/* Draw all pixels other than the first and last */
	while (--DeltaX) {
	  ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
	  ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
	  if (ErrorAcc <= ErrorAccTemp) {
		 /* The error accumulator turned over, so advance the Y coord */
		 Y0++;
	  }
	  X0 += XDir; /* X-major, so always advance X */
	  /* The IntensityBits most significant bits of ErrorAcc give us the
		 intensity weighting for this pixel, and the complement of the
		 weighting for the paired pixel */
	  Weighting = ErrorAcc >> IntensityShift;
	  put_pixel_intensity(X0, Y0, Color, BaseIntensity + Weighting);
	  put_pixel_intensity(X0, Y0 + 1, Color, BaseIntensity + (Weighting ^ WeightingComplementMask));
	}
	/* Draw the final pixel, which is always exactly intersected by the line
	  and so needs no weighting */
	put_pixel_intensity(X1, Y1, Color, BaseIntensity);
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

// helper function for drawing - no more need to go mess with
// the main function when just want to change what to draw...
void draw() {
#define min(a,b) ((a<b)?(a):(b))
	int r = 0.45*min(vinfo.yres, vinfo.xres/2);
	int xc1 = vinfo.xres/4;
	int xc2 = 3*vinfo.xres/4;
	int yc = vinfo.yres/2;

	int div = 72;
	for (double phase=0.0 ; phase<1 ; phase+=0.002) {
		if (stop_request) 
			return;
		frames++;

        // change page to draw to (between 0 and 1)
        cur_page = (cur_page + 1) % 2;

        // clear the previous image (= fill entire screen)
		clear_screen(0);

		for (int i=0 ; i<div ; i++) {
			double a = (i*2.0*M_PI)/div+phase;
			draw_line_bresenham(xc1, yc, xc1+r*cos(a)+0.5, yc+r*sin(a)+0.5, GREEN);
			draw_line_wu(xc2, yc, xc2+r*cos(a)+0.5, yc+r*sin(a)+0.5, GREEN);
		}

        // switch page (from fbtestXIV.c)
        vinfo.yoffset = cur_page * vinfo.yres;
        ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo);
        __u32 dummy = 0;
        ioctl(fbfd, FBIO_WAITFORVSYNC, &dummy);
	}
}

void exit_program(int sig) {
	stop_request = 1;
}


int main(int argc, char **aargv)
{
    long int screensize = 0;
    struct fb_var_screeninfo orig_vinfo;
	int kbfd = 0;

	signal(SIGINT, exit_program);

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        printf("Error: cannot open framebuffer device.\n");
        return 1;
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        printf("Error reading variable information.\n");
		return 1;
    }
    printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres,
         vinfo.bits_per_pixel );

#define PrintVar(s)  printf(#s "= %d\n", vinfo.s)
	PrintVar(xres);			/* visible resolution		*/
	PrintVar(yres);
	PrintVar(xres_virtual);		/* virtual resolution		*/
	PrintVar(yres_virtual);
	PrintVar(xoffset);			/* offset from virtual to visible */
	PrintVar(yoffset);			/* resolution			*/
	PrintVar(bits_per_pixel);		/* guess what			*/
	PrintVar(grayscale);		/* 0 = color, 1 = grayscale, >1 = FOURCC			*/
	PrintVar(nonstd);			/* != 0 Non standard pixel format */
	PrintVar(activate);			/* see FB_ACTIVATE_*		*/
	PrintVar(height);			/* height of picture in mm    */
	PrintVar(width);			/* width of picture in mm     */
	PrintVar(accel_flags);		/* (OBSOLETE) see fb_info.flags */
	PrintVar(pixclock);			/* pixel clock in ps (pico seconds) */
	PrintVar(left_margin);		/* time from sync to picture	*/
	PrintVar(right_margin);		/* time from picture to sync	*/
	PrintVar(upper_margin);		/* time from sync to picture	*/
	PrintVar(lower_margin);
	PrintVar(hsync_len);		/* length of horizontal sync	*/
	PrintVar(vsync_len);		/* length of vertical sync	*/
	PrintVar(sync);			/* see FB_SYNC_*		*/
	PrintVar(vmode);			/* see FB_VMODE_*		*/
	PrintVar(rotate);			/* angle we rotate counter clockwise */
	PrintVar(colorspace);		/* colorspace for FOURCC-based modes */

    // Store for reset (copy vinfo to vinfo_orig)
    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

    // Change variable info
    //vinfo.bits_per_pixel = 8;
    //vinfo.xres = 960;
    //vinfo.yres = 540;
    vinfo.xres_virtual = vinfo.xres;
    vinfo.yres_virtual = vinfo.yres * 1;
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
      printf("Error setting variable information.\n");
	  goto exit;
    }

	printf("$0\n");

    // hide cursor
    char *kbfds = "/dev/tty";
    kbfd = open(kbfds, O_WRONLY);
    if (kbfd >= 0)
        ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
    else
	{
        printf("Could not open %s.\n", kbfds);
		goto exit;
	}
	
	printf("$1\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        printf("Error reading fixed information.\n");
		goto exit;
    }

#define PrintVar_s(s)   printf(#s "= %s\n", finfo.s)
#define PrintVar_32(s)  printf(#s "= %d\n", finfo.s)
#define PrintVar_16(s)  printf(#s "= %d\n", finfo.s)
#define PrintVar_ul(s)  printf(#s "= %lx\n", finfo.s)
	PrintVar_s(id);				/* identification string eg "TT Builtin" */
	PrintVar_ul(smem_start);	/* Start of frame buffer mem */
	PrintVar_ul(mmio_start);	/* Start of Memory Mapped I/O   */
	PrintVar_32(smem_len);			/* Length of frame buffer mem */
	PrintVar_32(type);			/* see FB_TYPE_*		*/
	PrintVar_32(type_aux);			/* Interleave for interleaved Planes */
	PrintVar_32(visual);			/* see FB_VISUAL_*		*/ 
	PrintVar_16(xpanstep);			/* zero if no hardware panning  */
	PrintVar_16(ypanstep);			/* zero if no hardware panning  */
	PrintVar_16(ywrapstep);		/* zero if no hardware ywrap    */
	PrintVar_32(line_length);		/* length of a line in bytes    */
	PrintVar_32(mmio_len);			/* Length of Memory Mapped I/O  */
	PrintVar_32(accel);			/* Indicate to driver which	*/
	PrintVar_16(capabilities);		/* see FB_CAP_*			*/

    page_size = finfo.line_length * vinfo.yres;
	printf("\npage_size=%d\n", page_size);

	printf("$2\n");
	goto exit;

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
		goto exit;
    }
    else {
		struct timespec tstart, tend;
		clock_gettime(CLOCK_MONOTONIC, &tstart);
		draw();
		clock_gettime(CLOCK_MONOTONIC, &tend);

		long ms = (tend.tv_sec-tstart.tv_sec)*1000L+(tend.tv_nsec-tstart.tv_nsec)/1000000L;
		printf("Duration: %ld ms, %d frames drawn = %.1f fps\n", ms, frames, frames*1000.0/ms);
    }

exit:
    // cleanup
    // unmap fb file from memory
	if (fbp!=0)
		munmap(fbp, screensize);

    // reset cursor
    if (kbfd >= 0) {
        ioctl(kbfd, KDSETMODE, KD_TEXT);
        close(kbfd);
    }

    // reset the display mode
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)) {
        printf("Error re-setting variable information.\n");
    }

    // close fb file    
    close(fbfd);

    return 0;
}
